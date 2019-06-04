//*****************************************************************************
//
//     ライト管轄の処理[lightManager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "lightManager.h"
#include "light.h"
#include "manager.h"
#include "renderer.h"

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
//    中身をコピーする処理
//=============================================================================
CLightManager *CLightManager::Cpy(CLightManager *pLightManagerOld, int nNumLight)
{
	// 前回のライトの数を記憶
	int nNumLightOld = pLightManagerOld->GetNumLight();

	// 前回のライトを全てOffに設定
	for (int nCntLight = 0; nCntLight < nNumLightOld; nCntLight++)
	{// 今回必要な数だけ繰り返し
		CManager::GetRenderer()->GetDevice()->LightEnable(nCntLight, false);
	}

	// 今回必要な数だけライトを設定する
	CLight *pLight = NULL;
	CLightManager *pLightManager = CLightManager::Create(nNumLight);
	for (int nCntLight = 0; nCntLight < nNumLightOld; nCntLight++)
	{// コピーのために前回のライトの数分繰り返し
		if (nCntLight < nNumLight)
		{// 今回必要な数を超えていない
			// ライトを生成
			pLight = pLightManagerOld->GetCpyLight(nCntLight);

			// ライトを設定
			pLightManager->SettingLight(pLight, nCntLight);
		}
	}

	// 足りない分のライトは適当に値を決めて生成
	for (int nCntLight = nNumLightOld; nCntLight < nNumLight; nCntLight++)
	{// コピーのために前回のライトの数分繰り返し
	    // ライトを生成
		pLight = CDirectionalLight::Create(D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// ライトを設定
		pLightManager->SettingLight(pLight, nCntLight);
	}


	// 前回のライト管轄クラスを破棄する
	if (pLightManagerOld != NULL)
	{
		pLightManagerOld->Uninit();
		delete pLightManagerOld;
		pLightManagerOld = NULL;
	}

	return pLightManager;
}

//=============================================================================
//    ライト情報をコピーする処理
//=============================================================================
CLight *CLightManager::GetCpyLight(int nIdx)
{
	// ライトをゲットする
	CLight *pLightCpy = GetLight(nIdx);

	// 情報をコピーしてアドレスを返す
	CLight *pLight = NULL;
	D3DLIGHT9 Light = pLightCpy->GetLight();
	if (pLightCpy->GetType() == CLight::TYPE_DIRECTIONAL)
	{// ディレクショナルライトだったら
		pLight = CDirectionalLight::Create(Light.Direction, Light.Diffuse, Light.Ambient, Light.Specular);
	}
	else if (pLightCpy->GetType() == CLight::TYPE_POINT)
	{// ポイントライトだったら
		pLight = CPointLight::Create(Light.Position, Light.Diffuse, Light.Ambient, Light.Specular,
			Light.Attenuation0, Light.Attenuation1, Light.Attenuation2, Light.Range);
	}
	else if (pLightCpy->GetType() == CLight::TYPE_SPOT)
	{// スポットライトだったら
		pLight = CSpotLight::Create(Light.Position, Light.Direction, Light.Diffuse, Light.Ambient, Light.Specular,
			Light.Attenuation0, Light.Attenuation1, Light.Attenuation2, Light.Range, Light.Falloff,Light.Theta,Light.Phi);
	}

	return pLight;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CLightManager::Init(int nNumLight)
{
	// ライトの数を設定
	m_nNumLight = nNumLight;

	// ライトクラスのポインタを確保する
	if (m_apLight == NULL && m_nNumLight > 0)
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
	if (m_apLight != NULL)
	{// ライト用のアドレスが確保されている
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