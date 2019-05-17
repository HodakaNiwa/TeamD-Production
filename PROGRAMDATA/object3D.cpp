//=============================================================================
//
// �I�u�W�F�N�g3D���� [object3D.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "model.h"
#include "boxCollider.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
//CBoxCollider *CObject3D::m_pBoxCollider = NULL;

//=============================================================================
// �I�u�W�F�N�g3D�̃R���X�g���N�^
//=============================================================================
CObject3D::CObject3D(int nPriority) : CScene(nPriority,OBJTYPE_3D)
{
	m_pos = INITIALIZE_D3DXVECTOR3;				//�ʒu
	m_rot = INITIALIZE_D3DXVECTOR3;				//����
	m_pBoxCollider = NULL;
}
//=============================================================================
// �I�u�W�F�N�g3D�̃f�X�g���N�^
//=============================================================================
CObject3D::~CObject3D()
{

}
//=============================================================================
// �|���S���̐���
//=============================================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObject3D *pObject3D = NULL;		//�v���C���[�̃|�C���^

	if (pObject3D == NULL)
	{//NULL�̏ꍇ
		pObject3D = new CObject3D;	//���I�m��
		if (pObject3D != NULL)
		{//NULL�łȂ��ꍇ
			pObject3D->SetPos(pos);			//�ʒu�̐ݒu����
			pObject3D->SetRot(rot);			//�����̐ݒu����
			pObject3D->Init();				//����������
		}
	}
	return pObject3D;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject3D::Init()
{
	//�ʒu�̎擾����
	D3DXVECTOR3 pos = GetPos();

	//�����̎擾����
	D3DXVECTOR3 rot = GetRot();

	//�����蔻�蔠�̐�������
	if (m_pBoxCollider == NULL)
	{
		m_pBoxCollider = CBoxCollider::Create(pos, 25.0f, 25.0f, 25.0f,true);
	}

	//��ނ̐ݒu����
	SetObjType(OBJTYPE_3D);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObject3D::Uninit(void)
{
	//�����蔻�蔠�̏I������
	if (m_pBoxCollider != NULL)
	{
		m_pBoxCollider->Uninit();
		delete m_pBoxCollider;
		m_pBoxCollider = NULL;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject3D::Update(void)
{
	CDebugProc::Print(1, "�ʒu  : x[%.1f],y[%.1f],z[%.1f]\n", m_pos.x, m_pos.y, m_pos.z);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CObject3D::Draw(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;					//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;							//���݂̃}�e���A���ۑ��p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);

	//�����蔻�蔠�̕`�揈��
	if (m_pBoxCollider != NULL)
	{
		m_pBoxCollider->Draw();
	}
}

//=============================================================================
// �ʒu�̐ݒu����
//=============================================================================
void CObject3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// �����̐ݒu����
//=============================================================================
void CObject3D::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// �ʒu�̎擾����
//=============================================================================
D3DXVECTOR3 CObject3D::GetPos(void)
{
	return m_pos;
}


//=============================================================================
// �����̎擾����
//=============================================================================
D3DXVECTOR3 CObject3D::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// �����蔻��̏���
//=============================================================================
void CObject3D::Hit(CScene *pScene)
{

}

//=============================================================================
// �����蔻�蔠�̎擾����
//=============================================================================
CBoxCollider * CObject3D::GetBoxCollider(void)
{
	return m_pBoxCollider;
}