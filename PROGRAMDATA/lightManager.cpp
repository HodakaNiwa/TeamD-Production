//*****************************************************************************
//
//     ���C�g�Ǌ��̏���[lightManager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "lightManager.h"
#include "light.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLightManager::CLightManager()
{
	m_nNumLight = 0;    // ���C�g�̐�
	m_apLight = NULL;   // ���C�g�N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLightManager::~CLightManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CLightManager *CLightManager::Create(int nNumLight)
{
	CLightManager *pLightManager = NULL;  // ���C�g�Ǌ��N���X�^�̃|�C���^
	if (pLightManager == NULL)
	{// ����������ɂȂ��Ă���
		pLightManager = new CLightManager;
		if (pLightManager != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pLightManager->Init(nNumLight)))
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

	return pLightManager;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CLightManager::Init(int nNumLight)
{
	// ���C�g�̐���ݒ�
	m_nNumLight = nNumLight;

	// ���C�g�N���X�̃|�C���^���m�ۂ���
	if (m_apLight == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apLight = new CLight*[nNumLight];
		if (m_apLight != NULL)
		{// ���������m�ۂł���
			for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
			{// ���C�g�̐������J��Ԃ�
				m_apLight[nCntLight] = NULL;
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CLightManager::Uninit(void)
{
	if (m_apLight != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
		{// ���C�g�̐������J��Ԃ�
			if (m_apLight[nCntLight] != NULL)
			{// ���������m�ۂ���Ă���
				m_apLight[nCntLight]->Uninit();

				// �������̊J��
				delete m_apLight[nCntLight];
				m_apLight[nCntLight] = NULL;
			}
		}

		// �������̊J��
		delete m_apLight;
		m_apLight = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CLightManager::Update(void)
{
	if (m_apLight != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
		{// ���C�g�̐������J��Ԃ�
			if (m_apLight[nCntLight] != NULL)
			{// ���������m�ۂ���Ă���
				m_apLight[nCntLight]->Update();
			}
		}
	}
}

//=============================================================================
//    ���C�g��ݒ肷��
//=============================================================================
void CLightManager::SettingLight(CLight *pLight, const int nIdx)
{
	m_apLight[nIdx] = pLight;

	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		 // ���C�g��ݒ肷��
			pDevice->SetLight(nIdx, &m_apLight[nIdx]->GetLight());

			// ���C�g��ON/OFF��؂�ւ�
			pDevice->LightEnable(nIdx, m_apLight[nIdx]->GetSwitch());
		}
	}
}

//=============================================================================
//    ���C�g��ON/OFF��؂�ւ��邷��
//=============================================================================
void CLightManager::SwitchLight(const int nIdx)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		 // ���C�g��ON/OFF��؂�ւ�
			bool bSwitch = m_apLight[nIdx]->GetSwitch();
			bSwitch = bSwitch ? false : true;
			m_apLight[nIdx]->SetSwitch(bSwitch);
			pDevice->LightEnable(nIdx, m_apLight[nIdx]->GetSwitch());
		}
	}
}

//=============================================================================
//    ���C�g�̐���ݒ肷��
//=============================================================================
void CLightManager::SetNumLight(const int nNumLight)
{
	m_nNumLight = nNumLight;
}

//=============================================================================
//    ���C�g�̐����擾����
//=============================================================================
int CLightManager::GetNumLight(void)
{
	return m_nNumLight;
}

//=============================================================================
//    ���C�g�N���X�ւ̃|�C���^���擾����
//=============================================================================
CLight *CLightManager::GetLight(const int nIdx)
{
	return m_apLight[nIdx];
}