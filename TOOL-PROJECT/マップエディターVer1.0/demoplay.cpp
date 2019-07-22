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
#include "playerManager.h"
#include "characterManager.h"
#include "bullet.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "
#define DEMOPLAY_SYSTEM_FILENAME  "data/TEXT/demoplay.ini"    // 初期化に使用するシステムファイル名

// 読み込み用のパス
// モデル用
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// マップ用
#define NUM_MAP "NUM_MAP = "

// エフェクト用
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// 弾用
#define BULLET_MODEL_IDX "BULLET_MODEL_IDX = "

// アイテム用
#define ITEM_MODEL_IDX "ITEM_MODEL_IDX = "

// アイテムイベント用
#define ITEMEVENT_FILENAME "ITEMEVENT_FILENAME = "
#define ENEMY_STOP "ENEMY_STOP = "

// プレイヤーデータ用
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "

// 敵データ用
#define ENEMY_FILENAME "ENEMY_FILENAME = "


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
	CreatePlayer();

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

	// 各種クラスの破棄
	ReleasePlayer();
	ReleasePlayerManager();
	ReleaseEnemyManager();

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


//*****************************************************************************
//
// 生成用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
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
// デモプレイのプレイヤー生成処理
//=============================================================================
void CDemoplay::CreatePlayer(void)
{
	// プレイヤーを生成する
	m_pPlayer = m_pPlayerManager->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, 0);
}

//=============================================================================
// デモプレイの敵生成処理
//=============================================================================
CEnemy *CDemoplay::CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData)
{
	CEnemy *pEnemy = m_pEnemyManager[pEnemyData->GetEnemyType()]->SetEnemy(pos, rot, pEnemyData->GetEnemyType());

	if (pEnemy == NULL)return NULL;
	// アイテムのデータを設定する
	if (pEnemyData->GetItem() == true)
	{
		pEnemy->SetItemData(pEnemyData->GetItemType());
	}

	// AIのデータを設定する
	pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());

	return pEnemy;
}

//=============================================================================
// デモプレイのアイテムを生成する処理
//=============================================================================
void CDemoplay::CreateItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nItemType)
{
	int nModelIdx = 0;
	switch (nItemType)
	{
	case CItem::TYPE_STAR:
		nModelIdx = m_nItemModelIdx[0];
		CItemStar::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_GRENADE:
		nModelIdx = m_nItemModelIdx[1];
		CItemGrenade::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_1UP_TANK:
		nModelIdx = m_nItemModelIdx[2];
		CItem1up::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_SCOOP:
		nModelIdx = m_nItemModelIdx[3];
		CItemScoop::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_CLOCK:
		nModelIdx = m_nItemModelIdx[4];
		CItemClock::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_HELMET:
		nModelIdx = m_nItemModelIdx[5];
		CItemHelmet::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	}
}




//*****************************************************************************
//
// デモプレイ内スポーン処理用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
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
	CreateEnemy(EnemyPos, INITIALIZE_D3DXVECTOR3, pEnemyData);
}


//*****************************************************************************
//
// 開放用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// デモプレイのプレイヤークラスを開放する
//=============================================================================
void CDemoplay::ReleasePlayer(void)
{
	if (m_pPlayer != NULL)
	{
		m_pPlayer->Uninit();
		m_pPlayer = NULL;
	}
}

//=============================================================================
// デモプレイのプレイヤー管轄クラスを開放する
//=============================================================================
void CDemoplay::ReleasePlayerManager(void)
{
	if (m_pPlayerManager != NULL)
	{
		m_pPlayerManager->Uninit();
		delete m_pPlayerManager;
		m_pPlayerManager = NULL;
	}
}

//=============================================================================
// デモプレイの敵データ管轄クラスを開放する
//=============================================================================
void CDemoplay::ReleaseEnemyManager(void)
{
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		if (m_pEnemyManager[nCntEnemy] != NULL)
		{
			m_pEnemyManager[nCntEnemy]->Uninit();
			delete m_pEnemyManager[nCntEnemy];
			m_pEnemyManager[nCntEnemy] = NULL;
		}
	}
}


//*****************************************************************************
//
// アイテムイベント処理用関数
// Auther : Hodaka Niwa & Jukiya Hayakawa
//
//*****************************************************************************
//=============================================================================
// チュートリアルのスターアイテムのイベント処理
//=============================================================================
void CDemoplay::ItemEvent_Star(int nPlayerNumber)
{
	// プレイヤーのパラメータをパワーアップさせる

}

//=============================================================================
// チュートリアルのグレネードアイテムのイベント処理
//=============================================================================
void CDemoplay::ItemEvent_Grenade(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// 敵だった
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// チュートリアルの1Upアイテムのイベント処理
//=============================================================================
void CDemoplay::ItemEvent_1Up(int nPlayerNumber)
{
	// プレイヤーの残機数を増やす
	m_nPlayerStock++;
}

//=============================================================================
// チュートリアルのスコップアイテムのイベント処理
//=============================================================================
void CDemoplay::ItemEvent_Scoop(void)
{
	// 司令部の周りに壊せないブロックを置く
}

//=============================================================================
// チュートリアルの時計アイテムのイベント処理
//=============================================================================
void CDemoplay::ItemEvent_Clock(void)
{
	// 敵が動けない状態にする
	m_bEnemyMove = false;
	m_nEnemyMoveCounter = m_ItemEventData.nStop;
}

//=============================================================================
// チュートリアルのヘルメットアイテムのイベント処理
//=============================================================================
void CDemoplay::ItemEvent_Helmet(int nPlayerNumber)
{
	// プレイヤーを無敵状態にする
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
	int nCntMap = 0;
	int nCntTex = 0;
	int nCntModel = 0;
	int nCntItemModel = 0;
	int nCntEnemy = 0;
	int nCntPlayer = 0;

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// 読み込むテクスチャの数だった
			int nNumTex = CFunctionLib::ReadInt(pStr, NUM_TEXTURE);
			CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
			SetTextureManager(pTextureManager);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// 読み込むテクスチャのファイル名だった
			LoadTexture(pStr, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{// 読み込むモデルの数だった
			int nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			CModelCreate *pModelCreate = CModelCreate::Create(nNumModel);
			SetModelCreate(pModelCreate);
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// 読み込むモデルのファイル名だった
			LoadModel(pStr, nCntModel);
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_FILENAME) == 0)
		{// 読み込むエフェクトのファイル名だった
			LoadEffectFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// 読み込むマップファイル名だった
			strcpy(m_aMapFileName, CFunctionLib::ReadString(pStr, m_aMapFileName, MAP_FILENAME));
		}
		else if (CFunctionLib::Memcmp(pStr, ITEMEVENT_FILENAME) == 0)
		{// アイテムイベント用スクリプトファイル名だった
			LoadItemEvent(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, BULLET_MODEL_IDX) == 0)
		{// 弾が使用するモデルの番号だった
			m_nBulletModelIdx = CFunctionLib::ReadInt(pStr, BULLET_MODEL_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, ITEM_MODEL_IDX) == 0)
		{// アイテムが使用するモデルの番号だった
			m_nItemModelIdx[nCntItemModel] = CFunctionLib::ReadInt(pStr, ITEM_MODEL_IDX);
			nCntItemModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERDATA_FILENAME) == 0)
		{// プレイヤーデータのスクリプトファイル名だった
			LoadPlayerFileName(pStr, nCntPlayer);
			nCntPlayer++;
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMY_FILENAME) == 0)
		{// 敵データのスクリプトファイル名だった
			LoadEnemyFileName(pStr, nCntEnemy);
			nCntEnemy++;
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
// デモプレイの読み込むアイテムモデルを読み込む
//=============================================================================
void CDemoplay::LoadModel(char *pStr, int nCntModel)
{
	// モデルのファイルパス名を読み取る
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat = 0;
	char aModelFileName[256] = "\0";
	strcpy(aModelFileName, CFunctionLib::ReadString(pStr, aModelFileName, MODEL_FILENAME));

	// xファイルの読み込み
	D3DXLoadMeshFromX(aModelFileName, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// モデル管轄クラスに値を設定する
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL)return;
	pModelCreate->SetMesh(pMesh, nCntModel);
	pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);
	pModelCreate->SetFileName(pStr, nCntModel);
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
// ゲームの読み込むプレイヤーデータファイル名を読み込む
//=============================================================================
void CDemoplay::LoadPlayerFileName(char *pStr, int nCntPlayer)
{
	char aPlayerFileName[256] = "\0";
	strcpy(aPlayerFileName, CFunctionLib::ReadString(pStr, aPlayerFileName, PLAYERDATA_FILENAME));

	if (m_pPlayerManager != NULL)return;
	m_pPlayerManager = CPlayerManager::Create(aPlayerFileName);
}

//=============================================================================
// ゲームの読み込む敵データファイル名を読み込む
//=============================================================================
void CDemoplay::LoadEnemyFileName(char *pStr, int nCntEnemy)
{
	char aEnemyFileName[256] = "\0";
	strcpy(aEnemyFileName, CFunctionLib::ReadString(pStr, aEnemyFileName, ENEMY_FILENAME));

	// キャラクター管轄クラスを作成
	m_pEnemyManager[nCntEnemy] = CCharacterManager::Create(aEnemyFileName);
}

//=============================================================================
// デモプレイの読み込むマップファイル名を読み込む
//=============================================================================
void CDemoplay::LoadMapFileName(char *pStr)
{
	strcpy(m_aMapFileName, CFunctionLib::ReadString(pStr, m_aMapFileName, MAP_FILENAME));
}

//*****************************************************************************
//
// アイテムイベント用スクリプト読み込み処理
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// デモプレイのアイテムイベントスクリプトを読み込む
//=============================================================================
void CDemoplay::LoadItemEvent(char *pStr)
{
	char aFileName[256] = "\0";
	strcpy(aFileName, CFunctionLib::ReadString(pStr, aFileName, ITEMEVENT_FILENAME));
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(aFileName);
	if (pFileLoader != NULL)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// 読み込み開始の合図だった
			LoadItemEventScript(pFileLoader, pStr);
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
// デモプレイのアイテムイベント情報をファイルから読み込む
//=============================================================================
void CDemoplay::LoadItemEventScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, ENEMY_STOP) == 0)
		{// 敵をどれくらい止めるか情報だった
			m_ItemEventData.nStop = CFunctionLib::ReadInt(pStr, ENEMY_STOP);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}


//*****************************************************************************
//
// 設定、取得等色々関数(外部との窓口も含めてここに書いてます)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// デモプレイの変数をクリアする
//=============================================================================
void CDemoplay::ClearVariable(void)
{
	m_pUI = NULL;
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;
	m_nNumberTexIdx = 0;
	m_nPlayerRespawnCounter = 0;
	m_bEnemyMove = true;
	m_nEnemyMoveCounter = 0;
	m_pPlayer = NULL;
	m_pPlayerManager = NULL;
	m_nPlayerStock = 0;
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		m_pEnemyManager[nCntEnemy] = NULL;
	}
}


//=============================================================================
// デモプレイの弾モデルを設定する処理
//=============================================================================
void CDemoplay::SetBulletModel(CBullet *pBullet)
{
	pBullet->BindModel(GetModelCreate()->GetMesh(m_nBulletModelIdx), GetModelCreate()->GetBuffMat(m_nBulletModelIdx),
		GetModelCreate()->GetNumMat(m_nBulletModelIdx), GetModelCreate()->GetTexture(m_nBulletModelIdx));
}