//*****************************************************************************
//
//     ライトの処理[light.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "light.h"

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
//    ライトの種類を変更しアドレスを返す処理
//=============================================================================
CLight *CLight::ChangeLightType(TYPE type)
{
	CLight *pLight = NULL;

	// ライトを生成
	if (type == TYPE_DIRECTIONAL)
	{// ディレクショナルライトだったら
		pLight = CDirectionalLight::Create(D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (type == TYPE_POINT)
	{// ポイントライトだったら
		pLight = CPointLight::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.1f, 0.01f, 0.0f, 300.0f);
	}
	else if (type == TYPE_SPOT)
	{// ポイントライトだったら
		pLight = CSpotLight::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			0.1f, 0.01f, 0.0f, 300.0f, 1.0f, D3DX_PI * 0.5f, D3DX_PI * 2.0f);
	}

	return pLight;
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
CDirectionalLight *CDirectionalLight::Create(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, bool bSwitch)
{
	CDirectionalLight *pDirLight = NULL;  // ディレクショナルライトクラス型のポインタ
	if (pDirLight == NULL)
	{// メモリが空になっている
		pDirLight = new CDirectionalLight;
		if (pDirLight != NULL)
		{// インスタンスを生成できた
			// ライト情報を設定
			pDirLight->CreateLight(Dir, Diffuse, Ambient, Specular, bSwitch);

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
void CDirectionalLight::CreateLight(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, bool bSwitch)
{
	D3DLIGHT9 Light;   // ライト情報

	// 各種値の設定
	// ライトの種類
	Light.Type = D3DLIGHT_DIRECTIONAL;
	SetType(TYPE_DIRECTIONAL);

	// ライトの拡散光
	Light.Diffuse = Diffuse;

	// ライトの環境光
	Light.Ambient = Ambient;

	// ライトの反射光
	Light.Specular = Specular;

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
CPointLight *CPointLight::Create(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, bool bSwitch)
{
	CPointLight *pPointLight = NULL;  // ポイントライトクラス型のポインタ
	if (pPointLight == NULL)
	{// メモリが空になっている
		pPointLight = new CPointLight;
		if (pPointLight != NULL)
		{// インスタンスを生成できた
		    // ライト情報を設定
			pPointLight->CreateLight(pos, Diffuse, Ambient, Specular, fAttenuation0, fAttenuation1, fAttenuation2, fRange, bSwitch);

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
void CPointLight::CreateLight(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2,  float fRange, bool bSwitch)
{
	D3DLIGHT9 Light;   // ライト情報

	// 各種値の設定
	// ライトの種類
	Light.Type = D3DLIGHT_POINT;
	SetType(TYPE_POINT);

	// ライトの位置
	Light.Position = pos;

	// ライトの拡散光
	Light.Diffuse = Diffuse;

	// ライトの周囲色
	Light.Ambient = Ambient;

	// ライトの鏡面色
	Light.Specular = Specular;

	// ライトの減衰値
	Light.Attenuation0 = fAttenuation0;	   // 線形減衰係数
	Light.Attenuation1 = fAttenuation1;	   // 2次減衰係数
	Light.Attenuation2 = fAttenuation2;	   // 指数減衰係数

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
CSpotLight *CSpotLight::Create(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch)
{
	CSpotLight *pSpotLight = NULL;  // スポットライトクラス型のポインタ
	if (pSpotLight == NULL)
	{// メモリが空になっている
		pSpotLight = new CSpotLight;
		if (pSpotLight != NULL)
		{// インスタンスを生成できた
		    // ライト情報を設定
			pSpotLight->CreateLight(pos, Dir, Diffuse, Ambient, Specular, fAttenuation0, fAttenuation1, fAttenuation2, fRange, fFalloff, fTheta, fPhi, bSwitch);

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
void CSpotLight::CreateLight(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch)
{
	D3DLIGHT9 Light;   // ライト情報

	// 各種値の設定
	// ライトの種類
	Light.Type = D3DLIGHT_SPOT;
	SetType(TYPE_SPOT);

	// ライトの位置
	Light.Position = pos;

	// ライトの向き
	Light.Direction = Dir;

	// ライトの拡散光
	Light.Diffuse = Diffuse;

	// ライトの周囲色
	Light.Ambient = Ambient;

	// ライトの鏡面色
	Light.Specular = Specular;

	// ライトの減衰値
	Light.Attenuation0 = fAttenuation0;	  // 線形減衰係数
	Light.Attenuation1 = fAttenuation1;	  // 2次減衰係数
	Light.Attenuation2 = fAttenuation2;	  // 指数減衰係数

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