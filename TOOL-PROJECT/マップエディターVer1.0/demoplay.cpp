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
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEMOPLAY_SYSTEM_FILENAME  "data/TEXT/demoplay.ini"    // 初期化に使用するシステムファイル名

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
	// システムの初期化
	LoadSystem();

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
	CMap *pMap = CMap::Create(m_aMapFileName);
	SetMap(pMap);
}

//=============================================================================
// デモプレイのシステム情報を読み込む
//=============================================================================
void CDemoplay::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(DEMOPLAY_SYSTEM_FILENAME);
	if (pFileLoader != NULL)
	{
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// 読み込み開始の合図だった
			LoadSystemScript(pFileLoader, aStr);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// デモプレイのシステム情報をファイルから読み込む
//=============================================================================
void CDemoplay::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// マップファイル名だった
			LoadMapFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// デモプレイの読み込むマップファイル名を読み込む
//=============================================================================
void CDemoplay::LoadMapFileName(char *pStr)
{
	strcpy(m_aMapFileName, CFunctionLib::ReadString(pStr, m_aMapFileName, MAP_FILENAME));
}

//=============================================================================
// デモプレイのシステム情報を保存する
//=============================================================================
void CDemoplay::SaveSystem(void)
{
	char aStr[256] = "\0";
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(DEMOPLAY_SYSTEM_FILENAME);
	if (pFileSaver != NULL)
	{
		// ファイルの冒頭分を書き込み
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# デモプレイシステムファイル [demoplay.ini]\n");
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対消さないこと！\n", SCRIPT);
		pFileSaver->Print("\n");

		// 読み込むマップのファイル名を書き込み
		SaveMapFileName(pFileSaver);

		// スクリプト終了の合図を書き込み
		pFileSaver->Print("%s		# この行は絶対消さないこと！\n", END_SCRIPT);

		// メモリの開放
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}
}

//=============================================================================
// デモプレイの読み込むマップファイル名を保存する
//=============================================================================
void CDemoplay::SaveMapFileName(CFileSaver *pFileSaver)
{
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 読み込むマップのファイル名\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("%s%s", MAP_FILENAME, m_aMapFileName);
	pFileSaver->Print("\n");
}