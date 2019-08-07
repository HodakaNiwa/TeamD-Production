//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "boxCollider.h"
#include "bullet.h"
#include "enemy.h"
#include "basemode.h"
#include "cameraManager.h"
#include "block.h"
#include "game.h"
#include "motion.h"
#include "goalCylinder.h"
#include "tutorial.h"
#include "title.h"
#include "effectManager.h"
#include "server.h"
#include "sound.h"
#include "river.h"
#include "icefield.h"
#include "meshfield.h"
#include "headquarters.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MOVE (2.5f)
#define PLAYER_DEATH_EFFECT_IDX (7)
#define PLAYER_SE_BULLET_IDX (6)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �v���C���[�̃R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_pPlayer = NULL;			//�v���C���[�̃|�C���^
	m_state = STATE_NOMAL;		//���
	m_nCntState = 0;			//��ԃJ�E���^�[
	m_nMaxBullet = 0;			//�ő�e��
	m_nCntAbility = 0;			//�\�̓J�E���^�[
	m_bAllBlockDestroy = false;	//�S�Ẵu���b�N���󂹂邩�ǂ���
	m_bSplash = false;			//����Ă��邩�ǂ���
	m_nCntSplash = 0;			//����J�E���^�[
	m_motion = MOTION_NEUTAL;	//���[�V�������
	m_nCntBullet = 0;
}

//=============================================================================
// �v���C���[�̃R���X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}
//=============================================================================
// �v���C���[�̐���
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerIdx, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, int nPriority)
{
	CPlayer *pPlayer = NULL;				//�v���C���[�̃|�C���^

	if (pPlayer == NULL)
	{//NULL�̏ꍇ
		pPlayer = new CPlayer(nPriority);	//���I�m��
		if (pPlayer != NULL)
		{//NULL�łȂ��ꍇ
			pPlayer->SetPos(pos);										//�ʒu�̐ݒu����
			pPlayer->SetRot(rot);										//�����̐ݒu����
			pPlayer->SetModel(apModel);									//���f���̐ݒu����
			pPlayer->SetMotionManager(pMotionManager);					//���[�V�����}�l�[�W���[�̐ݒu����
			pPlayer->SetNumPart(nNumParts);								//�p�[�c�����̐ݒu����
			pPlayer->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));		//�����蔻��̐ݒu����
			pPlayer->SetPlayerIdx(nPlayerIdx);							//�v���C���[�ԍ��̐ݒu����
			pPlayer->Init();											//����������
			pPlayer->SetPlayer(pPlayer);								//�v���C���[�|�C���^�̐ݒu����
		}
	}
	return pPlayer;
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	//�ϐ��̃N���A����
	ClearVariable();

	//����������
	CCharacter::Init();

	SetAccel(PLAYER_MOVE);
	SetInertia(0.8f);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (CManager::GetGame() != NULL)
		{
			//�\�͂̏�����
		/*	m_nCntAbility = 0;
			m_ability = PLAYER_ABILITY_NOMAL;*/
			CManager::GetGame()->DeletePlayer(NULL, m_nPlayerIdx);
		}
	}
	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (CManager::GetTutorial() != NULL)
		{
			CManager::GetTutorial()->DeletePlayer(NULL, m_nPlayerIdx);
		}
	}

	//�I������
	CCharacter::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	//�ߋ��̈ʒu�̐ݒu
	SetPosOld(GetPos());

	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		// ���͂ɂ��ړ��ʌv�Z����
		InputMove_Game();

		//�ړ�����
		Move();

		if (GetMotionManager() != NULL)
		{//���[�V�����}�l�[�W���[��NULL�ł͂Ȃ��ꍇ
			GetMotionManager()->Update(GetModel());
		}

		//�����蔻��̏���
		Collision();
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		// ���͂ɂ��ړ��ʌv�Z����
		InputMove_Tutorial();

		//�ړ�����
		Move();

		if (GetMotionManager() != NULL)
		{//���[�V�����}�l�[�W���[��NULL�ł͂Ȃ��ꍇ
			GetMotionManager()->Update(GetModel());
		}

		//�����蔻��̏���
		Collision();
	}

	// ��Ԃ̏���
	State();

}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	SetMtxWorld(pDevice);

	if (m_state == STATE_RESPAWN || m_state == STATE_STOP)
	{//���X�|�[����Ԃ܂��͒�~��Ԃ̏ꍇ
		if (m_nCntState % 2 == 0)
		{
			//�L�����N�^�[�̕`�揈��
			CCharacter::Draw();
		}
	}
	else
	{
		//�L�����N�^�[�̕`�揈��
		CCharacter::Draw();
	}

#ifdef _DEBUG
	//�I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();
#endif // !_DEBUG
}

//=============================================================================
// ���������Ƃ��̏���
//=============================================================================
void CPlayer::Hit(CScene *pScene)
{
	// ���������I�u�W�F�N�g�ɂ���ď����킯
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// �e������
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_ENEMY)
		{// �G�̒e������
		    // �G�t�F�N�g���o��
			CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
			if (pEffectManager != NULL)
			{
				pEffectManager->SetEffect(GetPos(), INITIALIZE_D3DXVECTOR3, PLAYER_DEATH_EFFECT_IDX);
			}

			Uninit();
		}
		else if(pBullet->GetType() != m_nPlayerIdx)
		{// �Ⴄ�v���C���[�̒e������
			m_state = STATE_STOP;
		}
	}
}

//=============================================================================
// �v���C���[�̎��񂾂Ƃ��̃G�t�F�N�g��������
//=============================================================================
void CPlayer::SetDeathEffect(void)
{
	D3DXVECTOR3 EffectPos = GetPos();
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, PLAYER_DEATH_EFFECT_IDX);
	}
}

//=============================================================================
// �v���C���[�̐ݒu����
//=============================================================================
void CPlayer::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

//=============================================================================
// �v���C���[�ԍ��̐ݒu����
//=============================================================================
void CPlayer::SetPlayerIdx(int nPlayerIdx)
{
	m_nPlayerIdx = nPlayerIdx;
}

//=============================================================================
// �v���C���[��Ԃ̐ݒu����
//=============================================================================
void CPlayer::SetState(STATE state)
{
	m_state = state;
}

//=============================================================================
// �v���C���[�e�ő吔�̐ݒu����
//=============================================================================
void CPlayer::SetMaxBullet(int nMaxBullet)
{
	m_nMaxBullet = nMaxBullet;
}

//=============================================================================
// �v���C���[����Ă��邩�ǂ����ݒu����
//=============================================================================
void CPlayer::SetSplash(bool bSplash)
{
	m_bSplash = bSplash;
}

//=============================================================================
// �S�Ẵu���b�N���󂹂邩�ǂ����̐ݒu����
//=============================================================================
void CPlayer::SetAllBlockDestroy(bool bAllBlockDestroy)
{
	m_bAllBlockDestroy = bAllBlockDestroy;
}

//=============================================================================
// �v���C���[�̎擾����
//=============================================================================
CPlayer *CPlayer::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// �v���C���[�ԍ��̎擾����
//=============================================================================
int CPlayer::GetPlayerIdx(void)
{
	return m_nPlayerIdx;
}

//=============================================================================
// �v���C���[��Ԃ̎擾����
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_state;
}

//=============================================================================
// �v���C���[����Ă��邩�ǂ����̎擾����
//=============================================================================
bool CPlayer::GetSplash(void)
{
	return m_bSplash;
}

//=============================================================================
// �v���C���[�e�ő吔�̎擾����
//=============================================================================
int CPlayer::GetMaxBullet(void)
{
	return m_nMaxBullet;
}

//=============================================================================
// �S�Ẵu���b�N���󂹂邩�ǂ����̎擾����
//=============================================================================
bool CPlayer::GetAllBlockDestroy(void)
{
	return m_bAllBlockDestroy;
}

//=============================================================================
// �Q�[����ʂ̓��͈ړ�����
//=============================================================================
void CPlayer::InputMove_Game(void)
{
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C��������
		if (m_nPlayerIdx == CManager::GetClient()->GetClientId())
		{
			InputMove();
		}
	}
	else
	{// �I�����C���v���C����Ȃ�������
		InputMove();
	}
}

//=============================================================================
// �`���[�g���A����ʂ̓��͈ړ�����
//=============================================================================
void CPlayer::InputMove_Tutorial(void)
{
	CTutorial *pTutorial = CManager::GetTutorial();
	if (pTutorial == NULL) { return; }

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C��������
		if (m_nPlayerIdx == CManager::GetClient()->GetClientId() && pTutorial->GetState() != CTutorial::STATE_END)
		{
			InputMove();
		}
	}
	else
	{// �I�����C���v���C����Ȃ�������
		if (pTutorial->GetState() != CTutorial::STATE_END)
		{
			InputMove();
		}
	}
}

//=============================================================================
// ���͈ړ�����
//=============================================================================
void CPlayer::InputMove(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 rot = CCharacter::GetRot();
	D3DXVECTOR3 move = CCharacter::GetMove();
	float fDiffAngle = 0.0f;
	bool bMoving = false;

	// �����Ȃ���ԂȂ珈�����Ȃ�
	if (m_state == STATE_STOP) { return; }

	// �L�[�{�[�h�ł̈ړ�����
	bMoving = InputMove_Keyboard(&move, &fDiffAngle, rot);
	if (bMoving == false)
	{
		// �R���g���[���[�ł̈ړ�����
		bMoving = InputMove_Controller(&move, &fDiffAngle, rot);
	}

	// �e��l�̐ݒ�
	SetMove(move);
	SetDiffAngle(fDiffAngle);
}

//=============================================================================
// �L�[�{�[�h�̓��͈ړ�����
//=============================================================================
bool CPlayer::InputMove_Keyboard(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	float fAccel = GetAccel();

	if (m_nPlayerIdx == 0 || CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// 0�Ԃ̃v���C���[�Ȃ��
	    // �e���˂̏���
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{// �e���˃{�^������
			CreateBullet();
		}

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{// ��ړ��{�^������
			pMove->z = fAccel;
			*pDiffAngle = (D3DX_PI)-rot.y;
			SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{// ���ړ��{�^������
			pMove->z = -fAccel;
			*pDiffAngle = (D3DX_PI * 0) - rot.y;
			SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_A) == true)
		{// ���ړ��{�^������
			pMove->x = -fAccel;
			*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
			SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{// �E�ړ��{�^������
			pMove->x = fAccel;
			*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
			SetNowRotInfo(ROT_RIGHT);	//���݂̌������E�ɂ���
			return true;
		}
	}
	else
	{// 1�Ԃ̃v���C���[�Ȃ��
	 // �e���˂̏���
		if (pInputKeyboard->GetTrigger(DIK_NUMPADENTER) == true)
		{// �e���˃{�^������
			CreateBullet();
		}

		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{// ��ړ��{�^������
			pMove->z = fAccel;
			*pDiffAngle = (D3DX_PI)-rot.y;
			SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
		{// ���ړ��{�^������
			pMove->z = -fAccel;
			*pDiffAngle = (D3DX_PI * 0) - rot.y;
			SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_LEFT) == true)
		{// ���ړ��{�^������
			pMove->x = -fAccel;
			*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
			SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
		{// �E�ړ��{�^������
			pMove->x = fAccel;
			*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
			SetNowRotInfo(ROT_RIGHT);	//���݂̌������E�ɂ���
			return true;
		}
	}

	return false;
}

//=============================================================================
// �R���g���[���[�̓��͈ړ�����
//=============================================================================
bool CPlayer::InputMove_Controller(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot)
{
	// �R���g���[���[�̎擾
	CXInput *pXInput = CManager::GetXInput();
	if (pXInput == NULL) { return false; }

	float fAccel = GetAccel();

	// �e���˂̏���
	if (pXInput->GetTrigger(m_nPlayerIdx, CXInput::XIJS_BUTTON_11) == true)
	{// �e���˃{�^������
		CreateBullet();
	}

	if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_16) == true)
	{// ��ړ��{�^������
		pMove->z = fAccel;
		*pDiffAngle = (D3DX_PI)-rot.y;
		SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_17) == true)
	{// ���ړ��{�^������
		pMove->z = -fAccel;
		*pDiffAngle = (D3DX_PI * 0) - rot.y;
		SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_18) == true)
	{// ���ړ��{�^������
		pMove->x = -fAccel;
		*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
		SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_19) == true)
	{// �E�ړ��{�^������
		pMove->x = fAccel;
		*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
		SetNowRotInfo(ROT_RIGHT);	//���݂̌������E�ɂ���
		return true;
	}

	return false;
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer::Move(void)
{
	//�ړ��ʂ̎擾
	D3DXVECTOR3 move = CCharacter::GetMove();

	//�ʒu�̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//�����̎擾
	D3DXVECTOR3 rot = CCharacter::GetRot();

	//�����ʂ̎擾
	float fInertia = CCharacter::GetInertia();

	//�����蔻�蔠�̎擾����
	CBoxCollider *pCollider = NULL;
	pCollider = CObject3D::GetBoxCollider();

	//�����蔻�蔠�̈ʒu�ݒu����
	if (pCollider != NULL)
	{
		pCollider->SetPos(pos);
	}

	//�ʒu�̉��Z����
	pos += move;

	//�����̏���
	if (GetNowRotInfo() == ROT_UP ||
		GetNowRotInfo() == ROT_DOWN)
	{//�������O��̏ꍇ
		move.x = 0.0f;							//�΂߈ړ��������Ȃ����߂�x���̈ړ��ʂ�0�ɂ���
		move.z += (0.0f - move.z) * fInertia;	//z���̈ړ��ʉ��Z����
	}
	else if (GetNowRotInfo() == ROT_RIGHT ||
		GetNowRotInfo() == ROT_LEFT)
	{
		move.z = 0.0f;							//�΂߈ړ��������Ȃ����߂�z���̈ړ��ʂ�0�ɂ���
		move.x += (0.0f - move.x) * fInertia;	//x���̈ړ��ʉ��Z����
	}

	//�͈͊O�ɍs�����ꍇ���Ƃɖ߂�
	if (pos.x > ((MASS_SIZE_X * MASS_BLOCK_X) / 2) - 35.0f)
	{
		pos.x = ((MASS_SIZE_X * MASS_BLOCK_X) / 2) - 35.0f;
	}
	if (pos.x < -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + 35.0f)
	{
		pos.x = -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + 35.0f;
	}
	if (pos.z >((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) - 35.0f)
	{
		pos.z = ((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) - 35.0f;
	}
	if (pos.z < -((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) + 35.0f)
	{
		pos.z = -((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) + 35.0f;
	}

	if (move.x <= 0.1f && move.x >= -0.1f
		&& move.z <= 0.1f && move.z >= -0.1f)
	{// �ړ����Ă��Ȃ�
		if (m_motion == MOTION_MOVE)
		{
			m_motion = MOTION_NEUTAL;
			// ���[�V�����؂�ւ�����
			if (GetMotionManager() != NULL)
			{
				GetMotionManager()->SwitchMotion(GetModel(), MOTION_NEUTAL);
			}
		}
	}
	else
	{// �ړ����Ă���
		if (m_motion != MOTION_MOVE)
		{
			m_motion = MOTION_MOVE;
			// ���[�V�����؂�ւ�����
			if (GetMotionManager() != NULL)
			{
				GetMotionManager()->SwitchMotion(GetModel(), MOTION_MOVE);
			}
		}
	}

	//�ʒu�̐ݒu����
	SetPos(pos);
	//�����ݒu����
	SetRot(rot);
	//�ړ��̐ݒu����
	SetMove(move);
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CPlayer::Collision(void)
{
	//�ߋ��̈ʒu�̎擾
	D3DXVECTOR3 posOld = GetPosOld();

	//�ړ��ʂ̎擾����
	D3DXVECTOR3 move = GetMove();

	//�����蔻��̑傫���̎擾
	D3DXVECTOR3 colRange = GetColRange();

	//�ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();

	CScene *pScene = NULL;               // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃I�u�W�F�N�g�̃|�C���^
	bool bLand = false;					 // ���n���Ă��邩�ǂ���
	bool bIceLand = false;				 // �X�̏�ɂ��邩�ǂ���

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_3D)
			{// �I�u�W�F�N�g3D��������
				bLand = CollisionObject3D(&pos, &posOld, &move, colRange, (CObject3D*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// �G��������
				bLand = CollisionEnemy(&pos, &posOld, &move, colRange / 2, (CEnemy*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_BLOCK)
			{// �u���b�N��������
					bLand = CollisionBlock(&pos, &posOld, &move, colRange / 2, (CBlock*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_ITEM)
			{// �A�C�e����������
				CItem *pItem = (CItem*)pScene;

				if (CollisionItem(&pos, &posOld, &move, colRange, pItem) == true)
				{
					bLand = true;


					switch (pItem->GetType())
					{
					case CItem::TYPE_STAR:
						SwitchAbility();
						break;

					case CItem::TYPE_GRENADE:

						break;

					case CItem::TYPE_1UP_TANK:

						break;

					case CItem::TYPE_SCOOP:

						break;

					case CItem::TYPE_CLOCK:

						break;

					case CItem::TYPE_HELMET:

						break;
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_GOALCYLINDER)
			{// �S�[���~����������
				CollisionGoalCylinder(&pos, (CGoalCylinder*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_RIVER)
			{// �삾������
				bLand = CollisionRiver(&pos, &posOld, &move, colRange, (CRiver*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_ICEFIELD)
			{// �X��������
				CollisionIceField(pos, (CIceField*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_HEADQUARTERS)
			{// �i�ߕ���������
				bLand = CollisionHeadQuarters(&pos, &posOld, &move, colRange, (CHeadQuarters*)pScene);
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}
	switch (bLand)
	{
	case false:
		break;
	case true:
		break;
	}

	//�ʒu�̐ݒu����
	SetPos(pos);
	//�ߋ��̐ݒu����
	SetPosOld(posOld);
	//�ړ��ʂ̐ݒu����
	SetMove(move);
}

//=============================================================================
// ��Ԃ̏���
//=============================================================================
void CPlayer::State(void)
{
	switch (m_state)
	{
	case STATE_NOMAL:	//�ʏ���

		break;
	case STATE_STOP:	//��~���
		m_nCntState++;	//��ԃJ�E���^�[�̉��Z
		if (m_nCntState >= 80)
		{//��ԃJ�E���^�[��120�ȏ�̏ꍇ
			m_state = STATE_NOMAL;	//�ʏ��Ԃɖ߂�
			m_nCntState = 0;		//��ԃJ�E���^�[�̏�����
		}
		break;
	case STATE_RESPAWN: //���X�|�[�����
		m_nCntState++;	//��ԃJ�E���^�[�̉��Z
		if (m_nCntState >= 120)
		{//��ԃJ�E���^�[��120�ȏ�̏ꍇ
			m_state = STATE_NOMAL;	//�ʏ��Ԃɖ߂�
			m_nCntState = 0;		//��ԃJ�E���^�[�̏�����
		}
		break;
	case STATE_DEATH:	//���S���

		break;
	}
}

//=============================================================================
// �ړI�̌����ݒu����
//=============================================================================
void CPlayer::SetDiffAngle(float fDiffAngle)
{
	//�����̎擾
	D3DXVECTOR3 rot = CCharacter::GetRot();

	if (fDiffAngle > D3DX_PI)
	{
		fDiffAngle -= D3DX_PI * 2.0f;
	}
	if (fDiffAngle < -D3DX_PI)
	{
		fDiffAngle += D3DX_PI * 2.0f;
	}
	rot.y += fDiffAngle * 0.8f;
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	//�����̐ݒu����
	SetRot(rot);
}

//=============================================================================
// �e�̐���
//=============================================================================
void CPlayer::CreateBullet(void)
{
	//�ړ��ʂ̎擾
	D3DXVECTOR3 move = CCharacter::GetMove();

	//�ʒu�̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//�����̎擾
	D3DXVECTOR3 rot = CCharacter::GetRot();

	bool bShoot = GetShoot();

	//�e�̎�ނ̐؂�ւ�����
	CBullet::TYPE type;
	switch (m_nPlayerIdx)
	{
	case 0:
		type = CBullet::TYPE_PLAYER_0;
		break;
	case 1:
		type = CBullet::TYPE_PLAYER_1;
		break;
	}

	if (GetCntBullet() <= m_nMaxBullet)
	{
		// ����炷
		CManager::GetSound()->PlaySound(PLAYER_SE_BULLET_IDX);

		if (m_ability == PLAYER_ABILITY_NOMAL)
		{
			switch (GetNowRotInfo())
			{
			case ROT_UP:	//��������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, 8.0f), type, this);
				break;
			case ROT_DOWN:	//���������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, -8.0f), type, this);
				break;
			case ROT_RIGHT:	//�E�������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(8.0f, 0.0f, 0.0f), type, this);
				break;
			case ROT_LEFT:	//���������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(-8.0f, 0.0f, 0.0f), type, this);
				break;

			}
		}
		else
		{

			switch (GetNowRotInfo())
			{
			case ROT_UP:	//��������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, 15.0f), type, this);
				break;
			case ROT_DOWN:	//���������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, -15.0f), type, this);
				break;
			case ROT_RIGHT:	//�E�������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(15.0f, 0.0f, 0.0f), type, this);
				break;
			case ROT_LEFT:	//���������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(-15.0f, 0.0f, 0.0f), type, this);
				break;

			}
		}
		//�e�̐��ݒu����
		SetCntBullet(GetCntBullet() + 1);
	}
}

//=============================================================================
// �\�͂̐؂�ւ�����
//=============================================================================
void CPlayer::SwitchAbility(void)
{
	if (m_nCntAbility < PLAYER_ABILITY_MAX)
	{//�\�̓J�E���^�[���\�͂��ő�܂ŒB���Ă��Ȃ��ꍇ
		m_nCntAbility++;
	}

	switch (m_nCntAbility)
	{
	case 0:
		m_ability = PLAYER_ABILITY_NOMAL;
		break;
	case 1:
		m_ability = PLAYER_ABILITY_SPEEDUP;
		break;
	case 2:
		m_ability = PLAYER_ABILITY_DOUBLEBULLET;
		break;
	case 3:
		m_ability = PLAYER_ABILITY_ALLBLOCKDESTROY;
		break;
	}

	switch (m_ability)
	{
	case PLAYER_ABILITY_NOMAL:
		break;
	case PLAYER_ABILITY_SPEEDUP:
		break;
	case PLAYER_ABILITY_DOUBLEBULLET:
		m_nMaxBullet = 1;	//�ő�e�����Q���ɂ���
		break;
	case PLAYER_ABILITY_ALLBLOCKDESTROY:
		m_bAllBlockDestroy = true;
		break;
	}
}

//=============================================================================
// �ϐ����N���A���鏈��
//=============================================================================
void CPlayer::ClearVariable(void)
{
	m_pPlayer = NULL;			//�v���C���[�̃|�C���^
	m_state = STATE_NOMAL;		//���
	m_nCntState = 0;			//��ԃJ�E���^�[
	m_nMaxBullet = 0;			//�ő�e��
	m_nCntAbility = 0;			//�\�̓J�E���^�[
	m_bAllBlockDestroy = false;	//�S�Ẵu���b�N���󂹂邩�ǂ���
	m_bSplash = false;			//����Ă��邩�ǂ���
	m_nCntSplash = 0;			//����J�E���^�[
	m_motion = MOTION_NEUTAL;	//���[�V�������
	m_ability = PLAYER_ABILITY_NOMAL;	//�\�͂̏��
}

//=============================================================================
// �I�u�W�F�N�g3D�����蔻��̏���
//=============================================================================
bool CPlayer::CollisionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CObject3D *pObject3D)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pObject3D->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos,pPosOld,pMove,colRange) == true)
		{
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// �G�����蔻��̏���
//=============================================================================
bool CPlayer::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pEnemy->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange) == true)
		{
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// �u���b�N�����蔻��̏���
//=============================================================================
bool CPlayer::CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pBlock->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange) == true)
		{
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// �A�C�e�������蔻��̏���
//=============================================================================
bool CPlayer::CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CItem *pItem)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pItem->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange / 2, NULL) == true)
		{
			pItem->Hit(this);
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// �S�[���~�������蔻��̏���
//=============================================================================
void CPlayer::CollisionGoalCylinder(D3DXVECTOR3 *pPos, CGoalCylinder *pGoalCylinder)
{
	if (pGoalCylinder->Collision(pPos) == true)
	{
		if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{
			CTutorial *pTutorial = CManager::GetTutorial();
			if (pTutorial->GetState() != CTutorial::STATE_END)
			{
				pTutorial->SetState(CTutorial::STATE_END);
			}
		}
	}
}

//=============================================================================
// �쓖���蔻��̏���
//=============================================================================
bool CPlayer::CollisionRiver(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CRiver *pRiver)
{
	bool bLand = false;
	if (pRiver != NULL)
	{
		CBoxCollider *pBoxCollider = pRiver->GetBoxCollider();
		if (pBoxCollider != NULL)
		{
			if (pBoxCollider->Collision(pPos,pPosOld,pMove, colRange / 2, NULL) == true)
			{
				bLand = true;
			}
		}
	}
	return bLand;
}

//=============================================================================
// �X�����蔻��̏���
//=============================================================================
void CPlayer::CollisionIceField(D3DXVECTOR3 pos, CIceField *pIceField)
{
	bool  bIceLand = false;
	//���b�V���t�B�[���h�̎擾
	CMeshField *pMeshField = pIceField->GetMeshField();
	if (pMeshField != NULL)
	{//NULL�ł͂Ȃ��ꍇ
	 //���b�V���t�B�[���h�̃��[���h���W�ɂ����鍂���擾���鏈��
		pMeshField->GetPolyHeight(pos, &bIceLand, NULL);

		//�X�̏�ɂ��邩�ǂ����ʂ̏���
		switch (bIceLand)
		{
		case false:	//�X�̏�ɂ��Ȃ�
			SetInertia(0.8f);
			break;
		case true:	//�X�̏�ɂ���
			SetInertia(0.05f);
			break;
		}
	}

}

//=============================================================================
// �i�ߕ������蔻��̏���
//=============================================================================
bool CPlayer::CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHeadQuarters)
{
	bool bLand = false;
	if (pHeadQuarters != NULL)
	{
		CBoxCollider *pBoxCollider = pHeadQuarters->GetBoxCollider();
		if (pBoxCollider != NULL)
		{
			if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange / 2, NULL) == true)
			{
				bLand = true;
			}
		}
	}
	return bLand;
}
