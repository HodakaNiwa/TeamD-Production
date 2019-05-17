//=============================================================================
//
// ライト処理 [light.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// ライトのコンストラクタ
//=============================================================================
CLight::CLight()
{

}
//=============================================================================
// ライトのデストラクタ
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// ライトの生成
//=============================================================================
CLight *CLight::Create(void)
{
	CLight *pLight = NULL;		//ライトのポインタ

	if (pLight == NULL)
	{
		pLight = new CLight;	//動的確保
		if (pLight != NULL)
		{
			pLight->Init();		//初期化処理
		}
	}
	return pLight;
}

//=============================================================================
// ライトの初期化処理
//=============================================================================
void CLight::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 vecDir;

	// ライトをクリアする
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	// ライトの種類を設定
	m_light[0].Type = D3DLIGHT_DIRECTIONAL;
	m_light[1].Type = D3DLIGHT_DIRECTIONAL;
	m_light[2].Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	m_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_light[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
	m_light[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 0.15f);

	// ライトの方向の設定
	vecDir= D3DXVECTOR3(-0.25f, -0.87f, 0.44f);
	//vecDir = D3DXVECTOR3(0.8f, -0.5f, -0.2f);

	D3DXVec3Normalize(&vecDir, &vecDir);		//正規化する

	m_light[0].Direction = vecDir;

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);

	D3DXVec3Normalize(&vecDir, &vecDir);		//正規化する

	m_light[1].Direction = vecDir;

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);

	D3DXVec3Normalize(&vecDir, &vecDir);		//正規化する

	m_light[2].Direction = vecDir;

	if (pDevice != NULL)
	{
		// ライトを設定する
		pDevice->SetLight(0, &m_light[0]);
		pDevice->SetLight(1, &m_light[1]);
		pDevice->SetLight(2, &m_light[2]);
		// ライトを有効にする
		pDevice->LightEnable(0, TRUE);
		// ライトを有効にする
		pDevice->LightEnable(1, TRUE);
		// ライトを有効にする
		pDevice->LightEnable(2, TRUE);
	}
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void CLight::Update(void)
{
}

