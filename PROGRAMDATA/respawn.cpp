//*****************************************************************************
//
//     ���X�|�[���̏���[respawn.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "respawn.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "debugproc.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRespawn::CRespawn()
{
	// �e��l�̃N���A
	m_pModel = NULL;                          // ���f���N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRespawn::~CRespawn()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRespawn *CRespawn::Create(int nAreaX, int nAreaZ)
{
	CRespawn *pRespawn = NULL;  // ���X�|�[���N���X�^�̃|�C���^
	if (pRespawn == NULL)
	{// ����������ɂȂ��Ă���
		pRespawn = new CRespawn;
		if (pRespawn != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pRespawn->Init(nAreaX, nAreaZ)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pRespawn;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRespawn::Init(int nAreaX, int nAreaZ)
{
	// ������G���A������W���v�Z
	SetArea(nAreaX, nAreaZ);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRespawn::Uninit(void)
{
	// ���f���̔j��
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// �I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRespawn::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRespawn::Draw(void)
{
	// �����_�����O�N���X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���[���h�}�g���b�N�X�ݒ菈��
			SetMtxWorld(pDevice);

			// ���f���̕`��
			if (m_pModel != NULL)
			{
				m_pModel->Draw();
			}
		}
	}
}

//=============================================================================
//    ���f���N���X�^�̃|�C���^�ݒ菈��
//=============================================================================
void CRespawn::SetModel(CModel *pModel)
{
	m_pModel = pModel;
}

//=============================================================================
//    ���ǂ̃G���A�ɂ��邩��ݒ�
//=============================================================================
void CRespawn::SetArea(int nAreaX, int nAreaZ)
{
	// �G���A��ݒ�
	m_nAreaX = nAreaX;
	m_nAreaZ = nAreaZ;

	// �G���A������W�����߂�
	D3DXVECTOR3 pos = INITIALIZE_D3DXVECTOR3;
	pos.x = -((MASU_SIZE_X * MASU_BLOCK_X) / 2) + (MASU_SIZE_X * m_nAreaX) - MASU_SIZE_X_HALF;
	pos.z = ((MASU_SIZE_Z * MASU_BLOCK_Z) / 2) - (MASU_SIZE_Z * m_nAreaZ) + MASU_SIZE_Z_HALF;
	SetPos(pos);
}

//=============================================================================
//    ���f���N���X�^�̃|�C���^�擾����
//=============================================================================
CModel *CRespawn::GetModel(void)
{
	return m_pModel;
}

//=============================================================================
//    ���ǂ̃G���A(��)�ɂ��邩���擾
//=============================================================================
int CRespawn::GetAreaX(void)
{
	return m_nAreaX;
}

//=============================================================================
//    ���ǂ̃G���A(���s)�ɂ��邩���擾
//=============================================================================
int CRespawn::GetAreaZ(void)
{
	return m_nAreaZ;
}