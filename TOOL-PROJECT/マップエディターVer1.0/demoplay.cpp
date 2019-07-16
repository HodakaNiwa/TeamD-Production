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
#include "textureManager.h"
#include "camera.h"
#include "cameraManager.h"
#include "player.h"
#include "enemy.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "UI.h"
#include "respawn.h"
#include "effectManager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "
#define DEMOPLAY_SYSTEM_FILENAME  "data/TEXT/demoplay.ini"    // 初期化に使用するシステムファイル名

// 読み込み用のパス
#define EFFECT_FILENAME "EFFECT_FILENAME = "

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
	CDemoplay *pDemoplay = NULL;
	if (pDemoplay == NULL)
	{
		pDemoplay = new CDemoplay;
		if (pDemoplay != NULL)
		{
			pDemoplay->Init();
		}
	}
	return pDemoplay;
}

//=============================================================================
// デモプレイの初期化処理
//=============================================================================
HRESULT CDemoplay::Init(void)
{
	// 変数の初期化
	ClearVariable();

	// システムの初期化
	LoadSystem();

	// すでに起動しているならマップのファイル名を改めて設定
	if (CManager::GetStartUp() == true)
	{
		strcpy(m_aMapFileName, CManager::GetLoadMapFileName());
	}

	// カメラの生成
	CreateCamera();

	//マップを生成
	CreateMap();

	// UIを生成
	CreateUI();

	// プレイヤーの生成
	CPlayer *pPlayer = CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3);
	pPlayer->SetPos(GetMap()->GetPlayerRespawn(0)->GetPos());

	// 敵を生成するかチェックする
	CheckEnemySpawn(m_nGameCounter);

	return S_OK;
}

//=============================================================================
// デモプレイの終了処理
//=============================================================================
void CDemoplay::Uninit(void)
{
	// UIの破棄
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = NULL;
	}

	// 共通の終了処理
	CBasemode::Uninit();
}

//=============================================================================
// デモプレイの更新処理
//=============================================================================
void CDemoplay::Update(void)
{
	// カウンター増加
	m_nGameCounter++;

	// 全ての更新処理
	CScene::UpdateAll();

	// 敵を生成するかチェックする
	CheckEnemySpawn(m_nGameCounter);
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
// デモプレイの変数をクリアする
//=============================================================================
void CDemoplay::ClearVariable(void)
{
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;
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
// デモプレイのUI生成処理
//=============================================================================
void CDemoplay::CreateUI(void)
{
	m_pUI = CUI::Create();
}

//=============================================================================
// デモプレイの敵の生成するタイミングかどうか判定する
//=============================================================================
void CDemoplay::CheckEnemySpawn(int nTime)
{
	CMap *pMap = GetMap();
	int nNumEnemyListData = pMap->GetNumEnemyListData();
	CEnemy_ListData *pEnemyData = NULL;

	// もう敵が出ないのであればこの時点で処理終了
	if (m_nSpawnEnemyCount == nNumEnemyListData) return;

	for (int nCnt = 0; nCnt < nNumEnemyListData; nCnt++)
	{// 敵を生成する数分繰り返し
		pEnemyData = pMap->GetEnemyListData(nCnt);
		if (pEnemyData->GetRespawnTime() == nTime)
		{// リスポーンするタイミングである
			EnemySpawn(pMap, pEnemyData, nCnt);
			m_nSpawnEnemyCount++;
		}
	}
}

//=============================================================================
// デモプレイの敵の生成する
//=============================================================================
void CDemoplay::EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt)
{
	// 敵のリスポーン位置を取得
	D3DXVECTOR3 EnemyPos = pMap->GetEnemyRespawn(pEnemyData->GetRespawnIdx())->GetPos();

	// 敵の生成(ここは後で種類分けしておく)
	CEnemy::Create(EnemyPos, INITIALIZE_D3DXVECTOR3);
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
	int nCntTex = 0;  // テクスチャを読み込んだ数

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// 読み込むテクスチャの数だった
			CreateTextureManager(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// テクスチャのファイル名だった
			LoadTexture(CFunctionLib::ReadString(pStr, pStr, TEXTURE_FILENAME), nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_FILENAME) == 0)
		{// エフェクトファイル名だった
			LoadEffectFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
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
// デモプレイのテクスチャ管轄クラスを生成する
//=============================================================================
void CDemoplay::CreateTextureManager(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// デモプレイのテクスチャを読み込む
//=============================================================================
void CDemoplay::LoadTexture(char *pFileName, int nCntTex)
{
	// テクスチャを読み込む
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pFileName, &pTexture);

	// テクスチャ管轄クラスにポインタを設定
	CTextureManager *pTextureManager = GetTextureManager();
	if (pTextureManager != NULL && pTexture != NULL)
	{
		pTextureManager->SetTexture(pTexture, nCntTex);
	}
}

//=============================================================================
// エディターの読み込むエフェクトファイル名を読み込む
//=============================================================================
void CDemoplay::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// エフェクト管轄クラスの生成
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
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