//*****************************************************************************
//
//     ライトの処理[light.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "system.h"
#include "manager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#ifdef _DEBUG  // デバッグ中の処理
#define LIGHT_DEBUG_VECTOR_UPDOWN  (0.01f)  // ライトの向き情報をどれくらい動かすか
#define LIGHT_DEBUG_COLOR_UPDOWN   (0.01f)  // ライトの色情報をどれくらい変化させるか
#endif

//*****************************************************************************
//    CLightの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CLight::CLight()
{
	// 各種値のクリア
	m_Type = TYPE_NONE;                       // ライトの種類
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));  // ライト情報
	m_bSwitch = false;                        // ライトのON/OFF
}

//=============================================================================
//    デストラクタ
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
//    終了処理
//=============================================================================
void CLight::Uninit(void)
{
	// ライトをクリアする
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

//=============================================================================
//    ライトの種類を設定する
//=============================================================================
void CLight::SetType(const TYPE type)
{
	m_Type = type;
}

//=============================================================================
//    ライト情報を設定する
//=============================================================================
void CLight::SetLight(const D3DLIGHT9 Light)
{
	m_Light = Light;
}

//=============================================================================
//    ライトのON/OFFを設定する
//=============================================================================
void CLight::SetSwitch(const bool bSwitch)
{
	m_bSwitch = bSwitch;
}

//=============================================================================
//    ライトの種類を取得する
//=============================================================================
CLight::TYPE CLight::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    ライト情報を取得する
//=============================================================================
D3DLIGHT9 CLight::GetLight(void)
{
	return m_Light;
}

//=============================================================================
//    ライトのON/OFFを取得する
//=============================================================================
bool CLight::GetSwitch(void)
{
	return m_bSwitch;
}

//*****************************************************************************
//    CDirectionalLightの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CDirectionalLight::CDirectionalLight()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CDirectionalLight::~CDirectionalLight()
{

}


//=============================================================================
//    生成処理
//=============================================================================
CDirectionalLight *CDirectionalLight::Create(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, bool bSwitch)
{
	CDirectionalLight *pDirLight = NULL;  // ディレクショナルライトクラス型のポインタ
	if (pDirLight == NULL)
	{// メモリが空になっている
		pDirLight = new CDirectionalLight;
		if (pDirLight != NULL)
		{// インスタンスを生成できた
			// ライト情報を設定
			pDirLight->CreateLight(Dir, Diffuse, bSwitch);

			if (FAILED(pDirLight->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pDirLight;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CDirectionalLight::Init(void)
{
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CDirectionalLight::Uninit(void)
{

}

//=============================================================================
//    更新処理
//=============================================================================
void CDirectionalLight::Update(void)
{

}

//=============================================================================
//    ライト情報を設定する処理
//=============================================================================
void CDirectionalLight::CreateLight(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, bool bSwitch)
{
	D3DLIGHT9 Light;   // ライト情報

	// 各種値の設定
	// ライトの種類
	Light.Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光
	Light.Diffuse = Diffuse;

	// ライトの向き
	Light.Direction = Dir;

	// ライト情報を設定
	SetLight(Light);

	// ライトのON/OFFを設定
	SetSwitch(bSwitch);
}

//*****************************************************************************
//    CPointLightの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CPointLight::CPointLight()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CPointLight::~CPointLight()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CPointLight *CPointLight::Create(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation, float fRange, bool bSwitch)
{
	CPointLight *pPointLight = NULL;  // ポイントライトクラス型のポインタ
	if (pPointLight == NULL)
	{// メモリが空になっている
		pPointLight = new CPointLight;
		if (pPointLight != NULL)
		{// インスタンスを生成できた
		    // ライト情報を設定
			pPointLight->CreateLight(pos, Diffuse, Ambient, Specular, fAttenuation, fRange, bSwitch);

			if (FAILED(pPointLight->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pPointLight;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CPointLight::Init(void)
{
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CPointLight::Uninit(void)
{

}

//=============================================================================
//    更新処理
//=============================================================================
void CPointLight::Update(void)
{

}

//=============================================================================
//    ライト情報を設定する処理
//=============================================================================
void CPointLight::CreateLight(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation, float fRange, bool bSwitch)
{
	D3DLIGHT9 Light;   // ライト情報

	// 各種値の設定
	// ライトの種類
	Light.Type = D3DLIGHT_POINT;

	// ライトの位置
	Light.Position = pos;

	// ライトの拡散光
	Light.Diffuse = Diffuse;

	// ライトの周囲色
	Light.Ambient = Ambient;

	// ライトの鏡面色
	Light.Specular = Specular;

	// ライトの減衰値
	Light.Attenuation0 = fAttenuation;	// 線形減衰係数(基本的にここで変わる)
	Light.Attenuation1 = 0.0f;			// 2次減衰係数(基本的に0.0fで大丈夫)
	Light.Attenuation2 = 0.0f;			// 指数減衰係数(基本的に0.0fで大丈夫)

	// ライトの範囲
	Light.Range = fRange;

	// ライト情報を設定
	SetLight(Light);

	// ライトのON/OFFを設定
	SetSwitch(bSwitch);
}

//*****************************************************************************
//    CSpotLightの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CSpotLight::CSpotLight()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CSpotLight::~CSpotLight()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSpotLight *CSpotLight::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch)
{
	CSpotLight *pSpotLight = NULL;  // スポットライトクラス型のポインタ
	if (pSpotLight == NULL)
	{// メモリが空になっている
		pSpotLight = new CSpotLight;
		if (pSpotLight != NULL)
		{// インスタンスを生成できた
		    // ライト情報を設定
			pSpotLight->CreateLight(pos, Dir, Diffuse, Ambient, Specular, fAttenuation, fRange, fFalloff, fTheta, fPhi, bSwitch);

			if (FAILED(pSpotLight->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pSpotLight;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSpotLight::Init(void)
{
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSpotLight::Uninit(void)
{

}

//=============================================================================
//    更新処理
//=============================================================================
void CSpotLight::Update(void)
{

}

//=============================================================================
//    ライト情報を設定する処理
//=============================================================================
void CSpotLight::CreateLight(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch)
{
	D3DLIGHT9 Light;   // ライト情報

	// 各種値の設定
	// ライトの種類
	Light.Type = D3DLIGHT_SPOT;

	// ライトの位置
	Light.Position = pos;

	// ライトの拡散光
	Light.Diffuse = Diffuse;

	// ライトの周囲色
	Light.Ambient = Ambient;

	// ライトの鏡面色
	Light.Specular = Specular;

	// ライトの減衰値
	Light.Attenuation0 = fAttenuation;	// 線形減衰係数(基本的にここで変わる)
	Light.Attenuation1 = 0.0f;			// 2次減衰係数(基本的に0.0fで大丈夫)
	Light.Attenuation2 = 0.0f;			// 指数減衰係数(基本的に0.0fで大丈夫)

	// ライトの範囲
	Light.Range = fRange;

	// ライトのフォールオフ情報
	Light.Falloff = fFalloff;

	// ライトの内部コーンの角度
	Light.Theta = fTheta;

	// ライトの外部コーンの角度
	Light.Phi = fPhi;

	// ライト情報を設定
	SetLight(Light);

	// ライトのON/OFFを設定
	SetSwitch(bSwitch);
}

//*****************************************************************************
//    CLightManagerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CLightManager::CLightManager()
{
	m_nNumLight = 0;    // ライトの数
	m_apLight = NULL;   // ライトクラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CLightManager::~CLightManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CLightManager *CLightManager::Create(int nNumLight)
{
	CLightManager *pLightManager = NULL;  // ライト管轄クラス型のポインタ
	if (pLightManager == NULL)
	{// メモリが空になっている
		pLightManager = new CLightManager;
		if (pLightManager != NULL)
		{// インスタンスを生成できた
			if (FAILED(pLightManager->Init(nNumLight)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pLightManager;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CLightManager::Init(int nNumLight)
{
	// ライトの数を設定
	m_nNumLight = nNumLight;

	// ライトクラスのポインタを確保する
	if (m_apLight == NULL)
	{// メモリが確保できる状態である
		m_apLight = new CLight*[nNumLight];
		if (m_apLight != NULL)
		{// メモリが確保できた
			for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
			{// ライトの数だけ繰り返し
				m_apLight[nCntLight] = NULL;
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CLightManager::Uninit(void)
{
	if (m_apLight != NULL)
	{// メモリが確保されている
		for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
		{// ライトの数だけ繰り返し
			if (m_apLight[nCntLight] != NULL)
			{// メモリが確保されている
				m_apLight[nCntLight]->Uninit();

				// メモリの開放
				delete m_apLight[nCntLight];
				m_apLight[nCntLight] = NULL;
			}
		}

		// メモリの開放
		delete m_apLight;
		m_apLight = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CLightManager::Update(void)
{
	if (m_apLight != NULL)
	{// メモリが確保されている
		for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
		{// ライトの数だけ繰り返し
			if (m_apLight[nCntLight] != NULL)
			{// メモリが確保されている
				m_apLight[nCntLight]->Update();
			}
		}
	}
}

//=============================================================================
//    ライトを設定する
//=============================================================================
void CLightManager::SettingLight(CLight *pLight, const int nIdx)
{
	m_apLight[nIdx] = pLight;

	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
			// ライトを設定する
			pDevice->SetLight(nIdx, &m_apLight[nIdx]->GetLight());

			// ライトのON/OFFを切り替え
			pDevice->LightEnable(nIdx, m_apLight[nIdx]->GetSwitch());
		}
	}
}

//=============================================================================
//    ライトのON/OFFを切り替えるする
//=============================================================================
void CLightManager::SwitchLight(const int nIdx)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
			// ライトのON/OFFを切り替え
			bool bSwitch = m_apLight[nIdx]->GetSwitch();
			bSwitch = bSwitch ? false : true;
			m_apLight[nIdx]->SetSwitch(bSwitch);
			pDevice->LightEnable(nIdx, m_apLight[nIdx]->GetSwitch());
		}
	}
}

//=============================================================================
//    ライトの数を設定する
//=============================================================================
void CLightManager::SetNumLight(const int nNumLight)
{
	m_nNumLight = nNumLight;
}

//=============================================================================
//    ライトの数を取得する
//=============================================================================
int CLightManager::GetNumLight(void)
{
	return m_nNumLight;
}

//=============================================================================
//    ライトクラスへのポインタを取得する
//=============================================================================
CLight *CLightManager::GetLight(const int nIdx)
{
	return m_apLight[nIdx];
}