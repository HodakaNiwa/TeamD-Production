//=============================================================================
//
// �G���� [player.cpp]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "boxCollider.h"
#include "bullet.h"
#include "player.h"
#include "block.h"
#include "server.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_MOVE (2.5f)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CEnemy::m_nNumAll = 0;
int CEnemy::m_nSpawnCounter = 0;

LPD3DXMESH			CEnemy::m_pMesh = NULL;
LPD3DXBUFFER		CEnemy::m_pBuffMat = NULL;
DWORD				CEnemy::m_nNumMat = NULL;

//=============================================================================
// �G�̃R���X�g���N�^
//=============================================================================
CEnemy::CEnemy() : CCharacter(3, OBJTYPE_ENEMY)
{
	m_pModel = NULL;
	m_pEnemy = NULL;
}
//=============================================================================
// �G�̃R���X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}
//=============================================================================
// �G�̐���
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = NULL;				//�G�̃|�C���^

	if (pEnemy == NULL)
	{//NULL�̏ꍇ
		pEnemy = new CEnemy;				//���I�m��
		if (pEnemy != NULL)
		{//NULL�łȂ��ꍇ
			pEnemy->SetPos(pos);			//�ʒu�̐ݒu����
			pEnemy->SetRot(rot);			//�����̐ݒu����
			pEnemy->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pEnemy->SetReturnFlag(true);	//���������ۖ߂����ǂ����̐ݒu����
			pEnemy->Init();					//����������
			pEnemy->SetEnemy(pEnemy);
		}
	}
	return pEnemy;
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(void)
{
	// �ԍ��̐ݒ�
	SetIdx(m_nSpawnCounter);

	// �����𑝂₷
	m_nNumAll++;

	// �X�|�[�����𑝂₷
	m_nSpawnCounter++;

	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\enemy.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//���f���̐���
	m_pModel = m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		m_pMesh, m_pBuffMat, m_nNumMat);

	//�ړ���ނ̐ݒu����
	m_moveType = MOVE_DOWN;
	//�I�u�W�F�N�g3D�̏���������
	CObject3D::Init();
	//��ނ̐ݒu����
	SetObjType(OBJTYPE_ENEMY);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// ���������炷
	m_nNumAll--;

	//���f���̏I������
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// �G�̐������炷
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		int nNumEnemy = pGame->GetNumEnemy();
		nNumEnemy--;
		pGame->SetNumEnemy(nNumEnemy);
		CClient *pClient = CManager::GetClient();
		if (nNumEnemy <= 0 && pClient != NULL)
		{// �G���������Ȃ�
			if (pClient->GetClientId() == 0)
			{// �z�X�g��������I����Ԃ�
				pGame->SetState(CGame::STATE_END);
			}
		}
	}

	// �I������
	CCharacter::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	//�ߋ��̈ʒu�̐ݒu
	SetPosOld(GetPos());

	if (CManager::GetClient() != NULL)
	{
		if (CManager::GetClient()->GetClientId() == 0)
		{
			//�ړ�����
			Move();

			// �e��ł���
			SetBullet();
		}
	}

	if (GetBoxCollider() != NULL)
	{
		GetBoxCollider()->SetPos(GetPos());
	}

	//�����蔻��̏���
	Collision();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ���[���h�}�g���b�N�X�ݒ菈��
	SetMtxWorld(pDevice);

	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}

	//�I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();
}

//=============================================================================
// �G�̐ݒu����
//=============================================================================
void CEnemy::SetEnemy(CEnemy *pEnemy)
{
	m_pEnemy = pEnemy;
}

//=============================================================================
// �G�̐ݒu����
//=============================================================================
CEnemy *CEnemy::GetEnemy(void)
{
	return m_pEnemy;
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CEnemy::Hit(CScene * pScene)
{
	// ���������I�u�W�F�N�g�ɂ���ď����킯
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// �e������
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_PLAYER_0 || pBullet->GetType() == CBullet::TYPE_PLAYER_1)
		{// �v���C���[�̒e������
		    // �G�����������Ƃ����m�点
			if (CManager::GetMode() == CManager::MODE_GAME)
			{
				CGame *pGame = CManager::GetGame();
				CClient *pClient = CManager::GetClient();
				if (pClient != NULL && pClient->GetClientId() == 0)
				{
					Uninit();
					return;
				}
				if (pClient != NULL && CManager::GetClient()->GetClientId() != 0)
				{
					pGame->DeleteEnemy(m_nIdx);
				}
			}
			else
			{
				Uninit();
			}
		}
	}
}

//=============================================================================
// �ړ�����
//=============================================================================
void CEnemy::Move(void)
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

	float fDiffAngle;

	//�ړ��̎�ޕʏ���
	switch (m_moveType)
	{
	case MOVE_UP:	//��ړ�
		move.z = ENEMY_MOVE;
		fDiffAngle = (D3DX_PI)-rot.y;
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
		SetNowRotInfo(ROT_UP);	//���݂̌�������ɂ���
		break;

	case MOVE_DOWN:	//���ړ�
		move.z = -ENEMY_MOVE;
		fDiffAngle = (D3DX_PI * 0) - rot.y;
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
		SetNowRotInfo(ROT_DOWN);	//���݂̌��������ɂ���
		break;
	case MOVE_LEFT:	//���ړ�
		move.x = -ENEMY_MOVE;
		fDiffAngle = (D3DX_PI * 0.5f) - rot.y;
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
		SetNowRotInfo(ROT_LEFT);	//���݂̌��������ɂ���
		break;
	case MOVE_RIGHT:	//�E�ړ�
		move.x = ENEMY_MOVE;

		fDiffAngle = (D3DX_PI * -0.5f) - rot.y;
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
		SetNowRotInfo(ROT_RIGHT);	//���݂̌������E�ɂ���
		break;
	}

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
	//��������
	move.x += (0.0f - move.x) * 0.8f;
	move.z += (0.0f - move.z) * 0.8f;

	//�ʒu�̐ݒu����
	SetPos(pos);
	//�����ݒu����
	SetRot(rot);
	//�ړ��̐ݒu����
	SetMove(move);
}

//=============================================================================
// �����蔻��̏���
//=============================================================================
void CEnemy::Collision(void)
{
	//�ʒu�̎擾
	D3DXVECTOR3 pos = GetPos();

	//�ߋ��̈ʒu�̎擾
	D3DXVECTOR3 posOld = GetPosOld();

	//�ړ��ʂ̎擾����
	D3DXVECTOR3 move = GetMove();

	//�����蔻��̑傫���̎擾
	D3DXVECTOR3 colRange = GetColRange();

	CScene *pScene = NULL;               // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃I�u�W�F�N�g�̃|�C���^
	CObject3D *pObject = NULL;			 // �I�u�W�F�N�g�̃|�C���^
	CBoxCollider *pBoxCollider = NULL;	 // �����蔻�蔠�̃|�C���^
	CPlayer *pPlayer = NULL;			 // �G�̃|�C���^
	CBlock *pBlock = NULL;				 // �u���b�N�̃|�C���^

	bool bland = false;

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_PLAYER)
			{// �v���C���[��������
				pPlayer = (CPlayer*)pScene;
				if (pPlayer != NULL)
				{
					pBoxCollider = pPlayer->GetBoxCollider();
					if (pBoxCollider != NULL)
					{
						if (pBoxCollider->Collision(&pos, &posOld, &move, colRange / 2, NULL) == true)
						{
							bland = true;
						};
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_3D)
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
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}

	//�͈͊O�ɍs�����ꍇ���Ƃɖ߂�
	if (pos.x > 712.5f - 35.0f)
	{
		pos.x = 712.5f - 35.0f;
		bland = true;
	}
	if (pos.x < -712.5f + 35.0f)
	{
		pos.x = -712.5f + 35.0f;
		bland = true;
	}
	if (pos.z > 562.5f - 35.0f)
	{
		pos.z = 562.5f - 35.0f;
		bland = true;
	}
	if (pos.z < -562.5f + 35.0f)
	{
		pos.z = -562.5f + 35.0f;
		bland = true;
	}

	if (bland == true)
	{//�������Ă����ꍇ
		//�ړ������������_���Ō��߂�
		int nAnswer = rand() % 11;

		if (nAnswer >= 0 && nAnswer <= 1)
		{
			m_moveType = MOVE_UP;	//��ړ�
		}
		else if(nAnswer >= 2 && nAnswer <= 4)
		{
			m_moveType = MOVE_LEFT;	//���ړ�
		}
		else if (nAnswer >= 5 && nAnswer <= 7)
		{
			m_moveType = MOVE_RIGHT;	//�E�ړ�
		}
		else if (nAnswer >= 8 && nAnswer <= 10)
		{
			m_moveType = MOVE_DOWN;		//���ړ�
		}
	}
	//�ʒu�̐ݒu����
	SetPos(pos);
	//�ߋ��̈ʒu�ݒu����
	SetPosOld(posOld);
	//�ړ��ʐݒu����
	SetMove(move);
}

//=============================================================================
// �e��ł���
//=============================================================================
void CEnemy::SetBullet(void)
{
	D3DXVECTOR3 pos = GetPos();

	if (rand() % 200 == 0)
	{// ���̓����_��
		switch (GetNowRotInfo())
		{
		case ROT_UP:	//��������Ă���ꍇ
			CBulletEnemy::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 8.0f), this); //�e�̐���
			break;
		case ROT_DOWN:	//��������Ă���ꍇ
			CBulletEnemy::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -8.0f), this); //�e�̐���
			break;
		case ROT_RIGHT:	//��������Ă���ꍇ
			CBulletEnemy::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8.0f, 0.0f, 0.0f), this); //�e�̐���
			break;
		case ROT_LEFT:	//��������Ă���ꍇ
			CBulletEnemy::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-8.0f, 0.0f, 0.0f), this); //�e�̐���
			break;

		}
	}
}

//=============================================================================
// �G�̑������擾����
//=============================================================================
int CEnemy::GetNumAll(void)
{
	return m_nNumAll;
}

//=============================================================================
// �ԍ����擾����
//=============================================================================
int CEnemy::GetIdx(void)
{
	return m_nIdx;
}

//=============================================================================
// �ԍ���ݒ肷��
//=============================================================================
void CEnemy::SetIdx(const int nIdx)
{
	m_nIdx = nIdx;
}

//=============================================================================
// �X�|�[���J�E���^�[���擾���鏈��
//=============================================================================
int CEnemy::GetSpawnCounter(void)
{
	return m_nSpawnCounter;
}

//=============================================================================
// �X�|�[���J�E���^�[�����������鏈��
//=============================================================================
void CEnemy::ResetSpawnCounter(void)
{
	m_nSpawnCounter = 0;
}