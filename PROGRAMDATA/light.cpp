//*****************************************************************************
//
//     ���C�g�̏���[light.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "system.h"
#include "manager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#ifdef _DEBUG  // �f�o�b�O���̏���
#define LIGHT_DEBUG_VECTOR_UPDOWN  (0.01f)  // ���C�g�̌��������ǂꂭ�炢��������
#define LIGHT_DEBUG_COLOR_UPDOWN   (0.01f)  // ���C�g�̐F�����ǂꂭ�炢�ω������邩
#endif

//*****************************************************************************
//    CLight�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLight::CLight()
{
	// �e��l�̃N���A
	m_Type = TYPE_NONE;                       // ���C�g�̎��
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));  // ���C�g���
	m_bSwitch = false;                        // ���C�g��ON/OFF
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
//    �I������
//=============================================================================
void CLight::Uninit(void)
{
	// ���C�g���N���A����
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

//=============================================================================
//    ���C�g�̎�ނ�ݒ肷��
//=============================================================================
void CLight::SetType(const TYPE type)
{
	m_Type = type;
}

//=============================================================================
//    ���C�g����ݒ肷��
//=============================================================================
void CLight::SetLight(const D3DLIGHT9 Light)
{
	m_Light = Light;
}

//=============================================================================
//    ���C�g��ON/OFF��ݒ肷��
//=============================================================================
void CLight::SetSwitch(const bool bSwitch)
{
	m_bSwitch = bSwitch;
}

//=============================================================================
//    ���C�g�̎�ނ��擾����
//=============================================================================
CLight::TYPE CLight::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    ���C�g�����擾����
//=============================================================================
D3DLIGHT9 CLight::GetLight(void)
{
	return m_Light;
}

//=============================================================================
//    ���C�g��ON/OFF���擾����
//=============================================================================
bool CLight::GetSwitch(void)
{
	return m_bSwitch;
}

//*****************************************************************************
//    CDirectionalLight�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CDirectionalLight::CDirectionalLight()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CDirectionalLight::~CDirectionalLight()
{

}


//=============================================================================
//    ��������
//=============================================================================
CDirectionalLight *CDirectionalLight::Create(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, bool bSwitch)
{
	CDirectionalLight *pDirLight = NULL;  // �f�B���N�V���i�����C�g�N���X�^�̃|�C���^
	if (pDirLight == NULL)
	{// ����������ɂȂ��Ă���
		pDirLight = new CDirectionalLight;
		if (pDirLight != NULL)
		{// �C���X�^���X�𐶐��ł���
			// ���C�g����ݒ�
			pDirLight->CreateLight(Dir, Diffuse, bSwitch);

			if (FAILED(pDirLight->Init()))
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

	return pDirLight;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CDirectionalLight::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CDirectionalLight::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CDirectionalLight::Update(void)
{

}

//=============================================================================
//    ���C�g����ݒ肷�鏈��
//=============================================================================
void CDirectionalLight::CreateLight(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, bool bSwitch)
{
	D3DLIGHT9 Light;   // ���C�g���

	// �e��l�̐ݒ�
	// ���C�g�̎��
	Light.Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g�̊g�U��
	Light.Diffuse = Diffuse;

	// ���C�g�̌���
	Light.Direction = Dir;

	// ���C�g����ݒ�
	SetLight(Light);

	// ���C�g��ON/OFF��ݒ�
	SetSwitch(bSwitch);
}

//*****************************************************************************
//    CPointLight�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPointLight::CPointLight()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPointLight::~CPointLight()
{

}

//=============================================================================
//    ��������
//=============================================================================
CPointLight *CPointLight::Create(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation, float fRange, bool bSwitch)
{
	CPointLight *pPointLight = NULL;  // �|�C���g���C�g�N���X�^�̃|�C���^
	if (pPointLight == NULL)
	{// ����������ɂȂ��Ă���
		pPointLight = new CPointLight;
		if (pPointLight != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // ���C�g����ݒ�
			pPointLight->CreateLight(pos, Diffuse, Ambient, Specular, fAttenuation, fRange, bSwitch);

			if (FAILED(pPointLight->Init()))
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

	return pPointLight;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CPointLight::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CPointLight::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CPointLight::Update(void)
{

}

//=============================================================================
//    ���C�g����ݒ肷�鏈��
//=============================================================================
void CPointLight::CreateLight(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation, float fRange, bool bSwitch)
{
	D3DLIGHT9 Light;   // ���C�g���

	// �e��l�̐ݒ�
	// ���C�g�̎��
	Light.Type = D3DLIGHT_POINT;

	// ���C�g�̈ʒu
	Light.Position = pos;

	// ���C�g�̊g�U��
	Light.Diffuse = Diffuse;

	// ���C�g�̎��͐F
	Light.Ambient = Ambient;

	// ���C�g�̋��ʐF
	Light.Specular = Specular;

	// ���C�g�̌����l
	Light.Attenuation0 = fAttenuation;	// ���`�����W��(��{�I�ɂ����ŕς��)
	Light.Attenuation1 = 0.0f;			// 2�������W��(��{�I��0.0f�ő��v)
	Light.Attenuation2 = 0.0f;			// �w�������W��(��{�I��0.0f�ő��v)

	// ���C�g�͈̔�
	Light.Range = fRange;

	// ���C�g����ݒ�
	SetLight(Light);

	// ���C�g��ON/OFF��ݒ�
	SetSwitch(bSwitch);
}

//*****************************************************************************
//    CSpotLight�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSpotLight::CSpotLight()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSpotLight::~CSpotLight()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSpotLight *CSpotLight::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch)
{
	CSpotLight *pSpotLight = NULL;  // �X�|�b�g���C�g�N���X�^�̃|�C���^
	if (pSpotLight == NULL)
	{// ����������ɂȂ��Ă���
		pSpotLight = new CSpotLight;
		if (pSpotLight != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // ���C�g����ݒ�
			pSpotLight->CreateLight(pos, Dir, Diffuse, Ambient, Specular, fAttenuation, fRange, fFalloff, fTheta, fPhi, bSwitch);

			if (FAILED(pSpotLight->Init()))
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

	return pSpotLight;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSpotLight::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSpotLight::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CSpotLight::Update(void)
{

}

//=============================================================================
//    ���C�g����ݒ肷�鏈��
//=============================================================================
void CSpotLight::CreateLight(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch)
{
	D3DLIGHT9 Light;   // ���C�g���

	// �e��l�̐ݒ�
	// ���C�g�̎��
	Light.Type = D3DLIGHT_SPOT;

	// ���C�g�̈ʒu
	Light.Position = pos;

	// ���C�g�̊g�U��
	Light.Diffuse = Diffuse;

	// ���C�g�̎��͐F
	Light.Ambient = Ambient;

	// ���C�g�̋��ʐF
	Light.Specular = Specular;

	// ���C�g�̌����l
	Light.Attenuation0 = fAttenuation;	// ���`�����W��(��{�I�ɂ����ŕς��)
	Light.Attenuation1 = 0.0f;			// 2�������W��(��{�I��0.0f�ő��v)
	Light.Attenuation2 = 0.0f;			// �w�������W��(��{�I��0.0f�ő��v)

	// ���C�g�͈̔�
	Light.Range = fRange;

	// ���C�g�̃t�H�[���I�t���
	Light.Falloff = fFalloff;

	// ���C�g�̓����R�[���̊p�x
	Light.Theta = fTheta;

	// ���C�g�̊O���R�[���̊p�x
	Light.Phi = fPhi;

	// ���C�g����ݒ�
	SetLight(Light);

	// ���C�g��ON/OFF��ݒ�
	SetSwitch(bSwitch);
}

//*****************************************************************************
//    CLightManager�̏���
//*****************************************************************************
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