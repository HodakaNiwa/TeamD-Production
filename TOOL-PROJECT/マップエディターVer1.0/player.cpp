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
#include "motion.h"
#include "item.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MOVE (2.5f)

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
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerIdx, CModel **apModel, CMotionManager *pMotionManager, int nNumParts)
{
	CPlayer *pPlayer = NULL;				//�v���C���[�̃|�C���^

	if (pPlayer == NULL)
	{//NULL�̏ꍇ
		pPlayer = new CPlayer;				//���I�m��
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

	// ���͂ɂ��ړ��ʌv�Z����
	InputMove();

	//�ړ�����
	Move();

	//�����蔻��̏���
	Collision();

	if (GetMotionManager() != NULL)
	{//���[�V�����}�l�[�W���[��NULL�ł͂Ȃ��ꍇ
		GetMotionManager()->Update(GetModel());
	}

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
			Uninit();
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
// ���͈ړ�����
//=============================================================================
void CPlayer::InputMove(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//�ړ��ʂ̎擾
	D3DXVECTOR3 move = CCharacter::GetMove();

	//�ʒu�̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//�����̎擾
	D3DXVECTOR3 rot = CCharacter::GetRot();

	float fDiffAngle = 0.0f;

	if (m_nPlayerIdx == 0)
	{
		if (m_state != STATE_STOP)
		{//��~��Ԃł͂Ȃ��ꍇ
			if (pInputKeyboard->GetPress(DIK_W) == true)
			{// �L�[�{�[�h��[W]�L�[�������ꂽ�i��ړ��j
				move.z = PLAYER_MOVE;
				fDiffAngle = (D3DX_PI)-rot.y;
				SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true)
			{// �L�[�{�[�h��[S]�L�[�������ꂽ�i���ړ��j
				move.z = -PLAYER_MOVE;
				fDiffAngle = (D3DX_PI * 0) - rot.y;
				SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
			}
			else if (pInputKeyboard->GetPress(DIK_A) == true)
			{// �L�[�{�[�h��[A]�L�[�������ꂽ�i���ړ��j
				move.x = -PLAYER_MOVE;
				fDiffAngle = (D3DX_PI * 0.5f) - rot.y;
				SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
			}
			else if (pInputKeyboard->GetPress(DIK_D) == true)
			{// �L�[�{�[�h��[D]�L�[�������ꂽ�i�E�ړ��j
				move.x = PLAYER_MOVE;
				fDiffAngle = (D3DX_PI * -0.5f) - rot.y;
				SetNowRotInfo(ROT_RIGHT);	//���݂̌������E�ɂ���
			}
			SetDiffAngle(fDiffAngle);	//�ړI�̌����ݒu����

			if (pInputKeyboard->GetTrigger(DIK_1) == true)
			{//
				SwitchAbility();
			}
		}
	}
	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{// �L�[�{�[�h��[SPACE]�L�[�������ꂽ�i�e���ˁj
		CreateBullet();
	}
	SetMove(move);
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
			// ���n�̃��[�V�����ɐ؂�ւ���
			GetMotionManager()->SwitchMotion(GetModel(), MOTION_NEUTAL);
		}
	}
	else
	{// �ړ����Ă���
		if (m_motion != MOTION_MOVE)
		{
			m_motion = MOTION_MOVE;
			// ���[�V�����؂�ւ�����
			GetMotionManager()->SwitchMotion(GetModel(), MOTION_MOVE);
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
						};
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
						};
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
						};
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
						};
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
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, 8.0f), type,this);
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