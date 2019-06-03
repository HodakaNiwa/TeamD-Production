//=============================================================================
//
// �e�̏��� [bullet.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "boxCollider.h"
#include "debugproc.h"
#include "enemy.h"
#include "player.h"
#include "block.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
LPD3DXMESH			CBullet::m_pMesh = NULL;
LPD3DXBUFFER		CBullet::m_pBuffMat = NULL;
DWORD				CBullet::m_nNumMat = NULL;

//=============================================================================
// �e�̃R���X�g���N�^
//=============================================================================
CBullet::CBullet()
{
	m_posOld = INITIALIZE_D3DXVECTOR3;		//�ߋ��̈ʒu
	m_move = INITIALIZE_D3DXVECTOR3;		//�ړ���
	m_type = BULLET_TYPE_NONE;				//�e�̎��
	m_pModel = NULL;						//���f���̃|�C���^
	m_pScene = NULL;						//�I�u�W�F�N�g�̃|�C���^
}
//=============================================================================
// �e�̃R���X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}
//=============================================================================
// �e�̐���
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 move, BULLET_TYPE type,CScene *pScene)
{
	CBullet *pBullet = NULL;				//�e�̃|�C���^

	if (pBullet == NULL)
	{//NULL�̏ꍇ
		pBullet = new CBullet;				//���I�m��
		if (pBullet != NULL)
		{//NULL�łȂ��ꍇ
			pBullet->SetPos(pos);			//�ʒu�̐ݒu����
			pBullet->SetRot(rot);			//�����̐ݒu����
			pBullet->SetMove(move);			//�ړ��ʂ̐ݒu����
			pBullet->SetBulletType(type);	//�e�̎�ނ̐ݒu����
			pBullet->SetColRange(D3DXVECTOR3(18.75f, 18.75f, 18.75f));	//�����蔻��̑傫���̐ݒu����
			pBullet->m_pScene = pScene;
			pBullet->Init();				//����������
		}
	}
	return pBullet;
}
//=============================================================================
// ����������
//=============================================================================
HRESULT CBullet::Init(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\bullet.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//���f���̐���
	m_pModel = CModel::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f),
		m_pMesh, m_pBuffMat, m_nNumMat);

	//�I�u�W�F�N�g3D�̏���������
	CObject3D::Init();

	SetObjType(OBJTYPE_BULLET);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	//���f���̏I������
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	//�I������
	CObject3D::Uninit();

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	//�ߋ��̈ʒu�ݒu����
	SetPosOld(GetPos());

	//�ړ�����
	Move();

	//�����蔻�菈��
	Collision();

}

//=============================================================================
// �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//���f���̕`�揈��
	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}

	CObject3D::Draw();
}

//=============================================================================
// �e�̉ߋ��̈ʒu�ݒu����
//=============================================================================
void CBullet::SetPosOld(D3DXVECTOR3 posOld)
{
	m_posOld = posOld;
}

//=============================================================================
// �e�̈ړ��ʂ̐ݒu����
//=============================================================================
void CBullet::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// �e�̐ݒu����
//=============================================================================
void CBullet::SetBulletType(CBullet::BULLET_TYPE type)
{
	m_type = type;
}

//=============================================================================
// �I�u�W�F�N�g�̐ݒu����
//=============================================================================
void CBullet::SetScene(CScene *pScene)
{
	m_pScene = pScene;
}

//=============================================================================
// �e�̉ߋ��̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CBullet::GetPosOld(void)
{
	return m_posOld;
}

//=============================================================================
// �e�̈ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CBullet::GetMove(void)
{
	return m_move;
}

//=============================================================================
// �e�̎擾����
//=============================================================================
CBullet::BULLET_TYPE CBullet::GetBulletType(void)
{
	return m_type;
}

//=============================================================================
// �I�u�W�F�N�g�̎擾����
//=============================================================================
CScene *CBullet::GetScene(void)
{
	return m_pScene;
}

//=============================================================================
// �e�̈ړ�����
//=============================================================================
void CBullet::Move(void)
{
	D3DXVECTOR3 pos = GetPos();					//�ʒu�̎擾����
	D3DXVECTOR3 move = GetMove();				//�ړ��ʂ̎擾����
	pos += move;								//�ʒu�̑��

	//�����蔻�蔠�̎擾����
	CBoxCollider *pCollider = NULL;
	pCollider = CObject3D::GetBoxCollider();

	//�����蔻�蔠�̈ʒu�ݒu����
	if (pCollider != NULL)
	{
		pCollider->SetPos(pos);
	}

	//�ʒu�̐ݒu����
	SetPos(pos);

	//���f���̈ʒu�ݒu����
	if (m_pModel != NULL)
	{
		m_pModel->SetPos(pos);
	}

	//�͈͊O�ɓ������ꍇ�I������
	if (pos.x > 712.5 || pos.x < -712.5 ||
		pos.z > 562.5f || pos.z < -562.5f)
	{
		//���ŏ���
		Destroy();
	}
}

//=============================================================================
// �e�̓����蔻�菈��
//=============================================================================
void CBullet::Collision(void)
{
	CScene *pScene = NULL;               // �I�u�W�F�N�g�̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃I�u�W�F�N�g�̃|�C���^
	CObject3D *pObject = NULL;			 // �I�u�W�F�N�g�̃|�C���^
	CBoxCollider *pBoxCollider = NULL;	 //	�����蔻�蔠�̃|�C���^
	CEnemy *pEnemy = NULL;				 // �G�̃|�C���^
	CPlayer *pPlayer = NULL;			 // �v���C���[�̃|�C���^
	CBullet *pBullet = NULL;			 // �e�̃|�C���^
	CBlock *pBlock = NULL;				 // �u���b�N�̃|�C���^

	//�ʒu�̎擾����
	D3DXVECTOR3 pos = GetPos();

	//�����蔻��̎擾����
	D3DXVECTOR3 colRange = GetColRange();

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULL�ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if(m_type == BULLET_TYPE_PLAYER)
			{//�������e���v���C���[�̏ꍇ
				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// ��ނ��G�̏ꍇ
					pEnemy = (CEnemy*)pScene;	//�G�̓��I�m��
					if (pEnemy != NULL)
					{//NULL�łȂ��ꍇ
						pBoxCollider = pEnemy->GetBoxCollider();	//�����蔻�蔠�̎擾����
						if (pBoxCollider != NULL)
						{//�����蔻�蔠��NULL�łȂ��ꍇ
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move,colRange / 2 , NULL) == true)
							{//�����蔻�蔠�ɓ��������ꍇ
								//�I������
								//Uninit();
								//�G�̏I������
								pEnemy->Uninit();
								Destroy();
							};
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BULLET)
				{// ��ނ��G�̏ꍇ
					pBullet = (CBullet*)pScene;	//�G�̓��I�m��
					if (pBullet != NULL)
					{//NULL�łȂ��ꍇ
						pBoxCollider = pBullet->GetBoxCollider();	//�����蔻�蔠�̎擾����
						if (pBoxCollider != NULL)
						{//�����蔻�蔠��NULL�łȂ��ꍇ
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move, colRange / 2, NULL) == true)
							{//�����蔻�蔠�ɓ��������ꍇ
								//�I������
								//Uninit();
								//�G�̏I������
								pBullet->Destroy();
								Destroy();
							};
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BLOCK)
				{
					pBlock = (CBlock*)pScene;	//�u���b�N�̓��I�m��
					if (pBlock != NULL)
					{
						pBoxCollider = pBlock->GetBoxCollider();	//�����蔻�蔠�̎擾����
						if (pBoxCollider != NULL)
						{//�����蔻�蔠��NULL�łȂ��ꍇ
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move, colRange / 2, NULL) == true)
							{//�����蔻�蔠�ɓ��������ꍇ
							 //�I������
							 //Uninit();
							 //�G�̏I������
								pBlock->Hit(m_pScene);
								Destroy();
							};
						}
					}
				}
			}
			else if(m_type == BULLET_TYPE_ENEMY)
			{//�������e���G�̏ꍇ
				if (pScene->GetObjType() == OBJTYPE_PLAYER)
				{// ��ނ��G�̏ꍇ
					pPlayer = (CPlayer*)pScene;	//�G�̓��I�m��
					if (pPlayer != NULL)
					{//NULL�łȂ��ꍇ
						pBoxCollider = pPlayer->GetBoxCollider();	//�����蔻�蔠�̎擾����
						if (pBoxCollider != NULL)
						{//�����蔻�蔠��NULL�łȂ��ꍇ
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move, colRange, NULL) == true)
							{//�����蔻�蔠�ɓ��������ꍇ
							 //�I������
								//Uninit();
								//�G�̏I������
								pPlayer->Uninit();
								Destroy();
							};
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BULLET)
				{// ��ނ��G�̏ꍇ
					pBullet = (CBullet*)pScene;	//�G�̓��I�m��
					if (pBullet != NULL)
					{//NULL�łȂ��ꍇ
						pBoxCollider = pBullet->GetBoxCollider();	//�����蔻�蔠�̎擾����
						if (pBoxCollider != NULL)
						{//�����蔻�蔠��NULL�łȂ��ꍇ
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move, colRange, NULL) == true)
							{//�����蔻�蔠�ɓ��������ꍇ
							    //�I������
							    //Uninit();
							    //�G�̏I������
								pBullet->Destroy();
								Destroy();
							};
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BLOCK)
				{
					pBlock = (CBlock*)pScene;	//�u���b�N�̓��I�m��
					if (pBlock != NULL)
					{
						pBoxCollider = pBlock->GetBoxCollider();	//�����蔻�蔠�̎擾����
						if (pBoxCollider != NULL)
						{//�����蔻�蔠��NULL�łȂ��ꍇ
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move, colRange / 2, NULL) == true)
							{//�����蔻�蔠�ɓ��������ꍇ
							    //�I������
							    //Uninit();
							    //�G�̏I������
								pBlock->Hit(m_pScene);
								Destroy();
							};
						}
					}
				}

			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}

}

//=============================================================================
// �e�̔j�󏈗�
//=============================================================================
void CBullet::Destroy(void)
{
	CPlayer *pPlayer = NULL;	//�v���C���[�̃|�C���^���
	CEnemy *pEnemy = NULL;		//�G�̃|�C���^���

	switch (m_pScene->GetObjType())
	{
	case OBJTYPE_PLAYER:
		pPlayer = (CPlayer*) m_pScene;

		if (pPlayer != NULL)
		{
			pPlayer->SetShoot(false);
		}

		break;
	case OBJTYPE_ENEMY:
		pEnemy = (CEnemy*)m_pScene;

		if (pEnemy != NULL)
		{
			pEnemy->SetShoot(false);
		}

		break;
	}
	Uninit();
}