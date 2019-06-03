//=============================================================================
//
// デモプレイ処理 [demoplay.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "demoplay.h"
#include "scene.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "meshfield.h"
#include "debugproc.h"
#include "fade.h"
#include "map.h"
#include "modelcreate.h"
#include "camera.h"
#include "cameraManager.h"
#include "player.h"
#include "enemy.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEMOPLAY_MAP_FILENAME "data/TEXT/MAP/map000.txt"


//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デモプレイのコンストラクタ
//=============================================================================
CDemoplay::CDemoplay()
{

}

//=============================================================================
// デモプレイのデストラクタ
//=============================================================================
CDemoplay::~CDemoplay()
{

}

//=============================================================================
// デモプレイの生成処理
//=============================================================================
CDemoplay *CDemoplay::Create()
{
	CDemoplay *pDemoplay = NULL;	//デモプレイのポインタ
	if (pDemoplay == NULL)
	{
		pDemoplay = new CDemoplay;	//動的確保
		if (pDemoplay != NULL)
		{
			pDemoplay->Init();	//初期化処理
		}
	}
	return pDemoplay;
}

//=============================================================================
// デモプレイの初期化処理
//=============================================================================
HRESULT CDemoplay::Init(void)
{
	// カメラの生成
	CreateCamera();

	//マップを生成
	CreateMap();

	//メッシュフィールドの生成
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASU_SIZE_X, MASU_SIZE_Z, 19, 15, 1, 1, NULL, true);

	// プレイヤーの生成
	CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3);

	// 敵の生成
	CEnemy::Create(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), INITIALIZE_D3DXVECTOR3);

	return S_OK;
}

//=============================================================================
// デモプレイの終了処理
//=============================================================================
void CDemoplay::Uninit(void)
{
	// 共通の終了処理
	CBasemode::Uninit();

	//全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// デモプレイの更新処理
//=============================================================================
void CDemoplay::Update(void)
{
	//全ての更新処理
	CScene::UpdateAll();
}

//=============================================================================
// デモプレイの描画処理
//=============================================================================
void CDemoplay::Draw(void)
{
	// カメラの設定
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera();
	}

	// シーンクラスの描画処理
	CScene::DrawAll();
}

//=============================================================================
// デモプレイのカメラ生成処理
//=============================================================================
void CDemoplay::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager != NULL)
	{
		CCamera *pCamera = CCamera::Create();
		if (pCamera != NULL)
		{
			pCameraManager->SetCamera(pCamera);
		}
		SetCameraManager(pCameraManager);
	}
}

//=============================================================================
// デモプレイのマップ生成処理
//=============================================================================
void CDemoplay::CreateMap(void)
{
	CMap *pMap = CMap::Create(DEMOPLAY_MAP_FILENAME);
	SetMap(pMap);
}