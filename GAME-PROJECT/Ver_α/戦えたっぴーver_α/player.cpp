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
#include "server.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MOVE (2.5f)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXMESH			CPlayer::m_pMesh = NULL;
LPD3DXBUFFER		CPlayer::m_pBuffMat = NULL;
DWORD				CPlayer::m_nNumMat = NULL;

//=============================================================================
// �v���C���[�̃R���X�g���N�^
//=============================================================================
CPlayer::CPlayer() : CCharacter(3,OBJTYPE_PLAYER)
{
	m_pModel = NULL;	//���f���̃|�C���^
	m_pPlayer = NULL;	//�v���C���[�̃|�C���^
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
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdx)
{
	CPlayer *pPlayer = NULL;				//�v���C���[�̃|�C���^

	if (pPlayer == NULL)
	{//NULL�̏ꍇ
		pPlayer = new CPlayer;				//���I�m��
		if (pPlayer != NULL)
		{//NULL�łȂ��ꍇ
			pPlayer->SetPos(pos);			//�ʒu�̐ݒu����
			pPlayer->SetRot(rot);			//�����̐ݒu����
			pPlayer->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pPlayer->m_nIdx = nIdx;
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
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\player.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//���f���̐���
	m_pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
		m_pMesh, m_pBuffMat, m_nNumMat, NULL);

	CCharacter::Init();

	SetObjType(OBJTYPE_PLAYER);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	//���f���̏I������
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	//�I������
	CCharacter::Uninit();

	if (CManager::GetGame() != NULL)
	{
		CManager::GetGame()->DeletePlayer(NULL, m_nIdx);
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	//�ߋ��̈ʒu�̐ݒu
	SetPosOld(GetPos());

	// ���͂ɂ��ړ��ʌv�Z����
	if (m_nIdx == CManager::GetClient()->GetClientId())
	{
		InputMove();
	}

	//�ړ�����
	Move();

	//�����蔻��̏���
	Collision();
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

	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	m_pModel->SetPos(GetPos());
	GetBoxCollider()->SetPos(GetPos());

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}

	//�I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();
}

//=============================================================================
// �v���C���[�̐ݒu����
//=============================================================================
void CPlayer::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}


//=============================================================================
// �v���C���[�̎擾����
//=============================================================================
CPlayer *CPlayer::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer::InputMove(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	// XInput�̎擾
	CXInput *pXInput = CManager::GetXInput();

	//�ړ��ʂ̎擾
	D3DXVECTOR3 move = CCharacter::GetMove();

	//�ʒu�̎擾
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//�����̎擾
	D3DXVECTOR3 rot = CCharacter::GetRot();

	float fDiffAngle;

	if (pInputKeyboard->GetPress(DIK_W) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_16) == true)
	{// ������̓��͂����ꂽ
		move.z = PLAYER_MOVE;
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
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_17) == true)
	{// �������̓��͂����ꂽ
		move.z = -PLAYER_MOVE;
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
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_18) == true)
	{// �������̓��͂����ꂽ
		move.x = -PLAYER_MOVE;
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
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_19) == true)
	{// �E�����̓��͂����ꂽ
		move.x = PLAYER_MOVE;

		fDiffAngle = (D3DX_PI * -0.5f) - rot.y;
		if (fDiffAngle > D3DX_PI)
		{
			fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (fDiffAngle < -D3DX_PI)
		{
			fDiffAngle += D3DX_PI * 2.0f;
		}
		rot.y += fDiffAngle * 1.0f;
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		SetNowRotInfo(ROT_RIGHT);	//���݂̌������E�ɂ���
	}

	if (GetShoot() == false)
	{//�����Ă��Ȃ��ꍇ
		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
			pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
		{// �e���˃{�^���������ꂽ
			switch (GetNowRotInfo())
			{
			case ROT_UP:	//��������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 8.0f), (CBullet::TYPE)CManager::GetClient()->GetClientId(), this); //�e�̐���
				break;
			case ROT_DOWN:	//��������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -8.0f), (CBullet::TYPE)CManager::GetClient()->GetClientId(), this); //�e�̐���
				break;
			case ROT_RIGHT:	//��������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8.0f, 0.0f, 0.0f), (CBullet::TYPE)CManager::GetClient()->GetClientId(), this); //�e�̐���
				break;
			case ROT_LEFT:	//��������Ă���ꍇ
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-8.0f, 0.0f, 0.0f), (CBullet::TYPE)CManager::GetClient()->GetClientId(), this); //�e�̐���
				break;

			}
			SetShoot(true);		//�e����������Ԃɂ���
			CManager::GetSound()->PlaySound(2);
		}
	}


	//�ʒu�̐ݒu����
	SetPos(pos);
	//�����ݒu����
	SetRot(rot);
	//�ړ��̐ݒu����
	SetMove(move);
	//�����̐ݒu����
	SetRot(rot);

	//���f���̈ʒu�ݒu����
	m_pModel->SetPos(pos);
	//���f���̌����ݒu����
	m_pModel->SetRot(rot);
}

//=============================================================================
// �ړ�����
//=============================================================================
void CPlayer::Move(void)
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


	//if (rot.y == D3DX_PI ||
	//	rot.y == D3DX_PI * 0.5f ||
	//	rot.y == D3DX_PI * -0.5f)
	//{//�������l���������Ă���ꍇ
	//	if (GetShoot() == false)
	//	{//�����Ă��Ȃ��ꍇ
	//		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	//		{// �L�[�{�[�h��[SPACE]�L�[�������ꂽ�i�e���ˁj
	//			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(rot.y) * -8.0f, 0.0f, cosf(rot.y) * -8.0f), CBullet::BULLET_TYPE_PLAYER, m_pPlayer); //�e�̐���

	//			//�e����������Ԃɂ���
	//			SetShoot(true);
	//		}
	//	}
	//}

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
	if (pos.x > (MASS_SIZE_X * MASS_BLOCK_X) - 35.0f)
	{
		pos.x = (MASS_SIZE_X * MASS_BLOCK_X) - 35.0f;
	}
	if(pos.x < -(MASS_SIZE_X * MASS_BLOCK_X) + 35.0f)
	{
		pos.x = -(MASS_SIZE_X * MASS_BLOCK_X) + 35.0f;
	}
	if (pos.z > (MASS_SIZE_X* MASS_BLOCK_Z) - 35.0f)
	{
		pos.z = (MASS_SIZE_X * MASS_BLOCK_Z) - 35.0f;
	}
	if (pos.z < -(MASS_SIZE_X * MASS_BLOCK_Z) + 35.0f)
	{
		pos.z = -(MASS_SIZE_X * MASS_BLOCK_Z) + 35.0f;
	}

	//�ʒu�̐ݒu����
	SetPos(pos);
	//�����ݒu����
	SetRot(rot);
	//�ړ��̐ݒu����
	SetMove(move);
	//�����̐ݒu����
	SetRot(rot);

	//���f���̈ʒu�ݒu����
	m_pModel->SetPos(pos);
	//���f���̌����ݒu����
	m_pModel->SetRot(rot);

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
	//���f���̈ʒu�ݒu����
	m_pModel->SetPos(pos);
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
// �ϐ����N���A���鏈��
//=============================================================================
void CPlayer::ClearVariable(void)
{

}