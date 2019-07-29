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
	m_nMaxBullet = 1;			//�ő�e��
	m_nCntAbility = 0;			//�\�̓J�E���^�[
	m_bAllBlockDestroy = false;	//�S�Ẵu���b�N���󂹂邩�ǂ���
	m_bSplash = false;			//����Ă��邩�ǂ���
	m_nCntSplash = 0;			//����J�E���^�[
	m_motion = MOTION_NEUTAL;	//���[�V�������
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
		pPlayer = new CPlayer(nPriority);				//���I�m��
		if (pPlayer != NULL)
		{//NULL�łȂ��ꍇ
			pPlayer->SetPos(pos);			//�ʒu�̐ݒu����
			pPlayer->SetRot(rot);			//�����̐ݒu����
			pPlayer->SetModel(apModel);
			pPlayer->SetMotionManager(pMotionManager);
			pPlayer->SetNumPart(nNumParts);
			pPlayer->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pPlayer->SetPlayerIdx(nPlayerIdx);
			pPlayer->Init();				//����������
			pPlayer->SetPlayer(pPlayer);
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

	if (m_nPlayerIdx == 0 || CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// 0�Ԃ̃v���C���[�Ȃ��
	    // �e���˂̏���
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{// �e���˃{�^������
			CreateBullet();
		}

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{// ��ړ��{�^������
			pMove->z = PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI)-rot.y;
			SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{// ���ړ��{�^������
			pMove->z = -PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI * 0) - rot.y;
			SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_A) == true)
		{// ���ړ��{�^������
			pMove->x = -PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
			SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{// �E�ړ��{�^������
			pMove->x = PLAYER_MOVE;
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
			pMove->z = PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI)-rot.y;
			SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
		{// ���ړ��{�^������
			pMove->z = -PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI * 0) - rot.y;
			SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_LEFT) == true)
		{// ���ړ��{�^������
			pMove->x = -PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
			SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
		{// �E�ړ��{�^������
			pMove->x = PLAYER_MOVE;
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

	// �e���˂̏���
	if (pXInput->GetTrigger(m_nPlayerIdx, CXInput::XIJS_BUTTON_11) == true)
	{// �e���˃{�^������
		CreateBullet();
	}

	if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_16) == true)
	{// ��ړ��{�^������
		pMove->z = PLAYER_MOVE;
		*pDiffAngle = (D3DX_PI)-rot.y;
		SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_17) == true)
	{// ���ړ��{�^������
		pMove->z = -PLAYER_MOVE;
		*pDiffAngle = (D3DX_PI * 0) - rot.y;
		SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_18) == true)
	{// ���ړ��{�^������
		pMove->x = -PLAYER_MOVE;
		*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
		SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_19) == true)
	{// �E�ړ��{�^������
		pMove->x = PLAYER_MOVE;
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
	move.x += (0.0f - move.x) * 0.8f;
	move.z += (0.0f - move.z) * 0.8f;

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
	CObject3D *pObject = NULL;			 // �I�u�W�F�N�g�̃|�C���^
	CBoxCollider *pBoxCollider = NULL;	 // �����蔻�蔠�̃|�C���^
	CEnemy *pEnemy = NULL;				 // �G�̃|�C���^
	CBlock *pBlock = NULL;				 // �u���b�N�̃|�C���^
	CItem *pItem = NULL;                 // �A�C�e���̃|�C���^
	CGoalCylinder *pGoal = NULL;         // �S�[���p�~���̃|�C���^

	bool bland = false;

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_3D)
			{// �I�u�W�F�N�g3D��������
				pObject = (CObject3D*)pScene;
				if (pObject != NULL)
				{
					pBoxCollider = pObject->GetBoxCollider();
					if (pBoxCollider != NULL)
					{
						if (pBoxCollider->Collision(&pos, &posOld, &move, colRange / 2, NULL) == true)
						{
							bland = true;
						}
					}
				}

			}
			else if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{
				pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{
					pBoxCollider = pEnemy->GetBoxCollider();
					if (pBoxCollider != NULL)
					{
						if (pBoxCollider->Collision(&pos, &posOld, &move, colRange / 2, NULL) == true)
						{
							bland = true;
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_BLOCK)
			{
				pBlock = (CBlock*)pScene;
				if (pBlock != NULL)
				{
					pBoxCollider = pBlock->GetBoxCollider();
					if (pBoxCollider != NULL)
					{
						if (pBoxCollider->Collision(&pos, &posOld, &move, colRange / 2, NULL) == true)
						{
							bland = true;
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_ITEM)
			{
				pItem = (CItem*)pScene;
				if (pItem != NULL)
				{
					pBoxCollider = pItem->GetBoxCollider();
					if (pBoxCollider != NULL)
					{
						if (pBoxCollider->Collision(&pos, &posOld, &move, colRange / 2, NULL) == true)
						{
							pItem->Hit(this);
							bland = true;
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_GOALCYLINDER)
			{
				pGoal = (CGoalCylinder*)pScene;
				if (pGoal != NULL)
				{
					if (pGoal->Collision(&pos) == true)
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
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}
	switch (bland)
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

	if (bShoot == false)
	{//�e���܂�����ꍇ
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
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, -8.0f), type,this);
				break;
			case ROT_RIGHT:	//�E�������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(8.0f, 0.0f, 0.0f), type,this);
				break;
			case ROT_LEFT:	//���������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(-8.0f, 0.0f, 0.0f), type,this);
				break;

			}
		}
		else
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
		SetShoot(true);		//�e�̐ݒu����
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
		m_nMaxBullet = 2;
		//SetShoot(m_nMaxBullet);
		break;
	case PLAYER_ABILITY_ALLBLOCKDESTROY:
		//m_nMaxBullet = 2;
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
	m_nMaxBullet = 1;			//�ő�e��
	m_nCntAbility = 0;			//�\�̓J�E���^�[
	m_bAllBlockDestroy = false;	//�S�Ẵu���b�N���󂹂邩�ǂ���
	m_bSplash = false;			//����Ă��邩�ǂ���
	m_nCntSplash = 0;			//����J�E���^�[
	m_motion = MOTION_NEUTAL;	//���[�V�������
}