//*****************************************************************************
//
//     モードベースの処理[basemode.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "basemode.h"
#include "map.h"
#include "cameraManager.h"
#include "modelcreate.h"
#include "textureManager.h"
#include "effectManager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CBasemode::CBasemode()
{
	// 各種値のクリア
	m_pMap = NULL;             // マップクラスへのポインタ
	m_pCameraManager = NULL;   // カメラ管轄クラスへのポインタ
	m_pModelCreate = NULL;     // モデル管轄クラスへのポインタ
	m_pTextureManager = NULL;  // テクスチャ管轄クラスへのポインタ
	m_pEffectManager = NULL;   // エフェクト管轄クラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBasemode::~CBasemode()
{

}

//=============================================================================
//    終了処理
//=============================================================================
void CBasemode::Uninit(void)
{
	// マップの破棄
	ReleaseMap();

	// カメラ管轄クラスの破棄
	ReleaseCameraManager();

	// テクスチャ管轄クラスの破棄
	ReleaseTextureManager();

	// モデル管轄クラスの破棄
	ReleaseModelCreate();

	// エフェクト管轄クラスの破棄
	ReleaseEffectManager();
}

//=============================================================================
//    マップクラスへのポインタを設定する
//=============================================================================
void CBasemode::SetMap(CMap *pMap)
{
	m_pMap = pMap;
}

//=============================================================================
//    カメラ管轄クラスへのポインタを設定する
//=============================================================================
void CBasemode::SetCameraManager(CCameraManager *pCameraManager)
{
	m_pCameraManager = pCameraManager;
}

//=============================================================================
//    モデル管轄クラスへのポインタを設定する
//=============================================================================
void CBasemode::SetModelCreate(CModelCreate *pModelCreate)
{
	m_pModelCreate = pModelCreate;
}

//=============================================================================
//     テクスチャ管轄クラスへのポインタを設定する
//=============================================================================
void CBasemode::SetTextureManager(CTextureManager *pTextureManager)
{
	m_pTextureManager = pTextureManager;
}

//=============================================================================
//     エフェクト管轄クラスへのポインタを設定する
//=============================================================================
void CBasemode::SetEffectManager(CEffectManager *pEffectManager)
{
	m_pEffectManager = pEffectManager;
}

//=============================================================================
//    マップクラスへのポインタを取得する
//=============================================================================
CMap *CBasemode::GetMap(void)
{
	return m_pMap;
}

//=============================================================================
//    カメラ管轄クラスへのポインタを取得する
//=============================================================================
CCameraManager *CBasemode::GetCameraManager(void)
{
	return m_pCameraManager;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する
//=============================================================================
CModelCreate *CBasemode::GetModelCreate(void)
{
	return m_pModelCreate;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを取得する
//=============================================================================
CTextureManager *CBasemode::GetTextureManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    エフェクト管轄クラスへのポインタを取得する
//=============================================================================
CEffectManager *CBasemode::GetEffectManager(void)
{
	return m_pEffectManager;
}

//=============================================================================
//    マップクラスを破棄する
//=============================================================================
void CBasemode::ReleaseMap(void)
{
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();
		delete m_pMap;
		m_pMap = NULL;
	}
}

//=============================================================================
//    カメラ管轄クラスを破棄する
//=============================================================================
void CBasemode::ReleaseCameraManager(void)
{
	if (m_pCameraManager != NULL)
	{
		m_pCameraManager->Uninit();
		delete m_pCameraManager;
		m_pCameraManager = NULL;
	}
}

//=============================================================================
//    モデル管轄クラスを破棄する
//=============================================================================
void CBasemode::ReleaseModelCreate(void)
{
	if (m_pModelCreate != NULL)
	{
		m_pModelCreate->Uninit();
		delete m_pModelCreate;
		m_pModelCreate = NULL;
	}
}

//=============================================================================
//    テクスチャ管轄クラスを破棄する
//=============================================================================
void CBasemode::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    エフェクト管轄クラスを破棄する
//=============================================================================
void CBasemode::ReleaseEffectManager(void)
{
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager->Uninit();
		m_pEffectManager = NULL;
	}
}