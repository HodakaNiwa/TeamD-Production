//=============================================================================
//
// ゲームの処理 [game.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "game.h"
#include "scene.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "meshfield.h"
#include "debuglog.h"
#include "fade.h"
#include "map.h"
#include "modelcreate.h"
#include "camera.h"
#include "cameraManager.h"
#include "player.h"


#include "server.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define GAME_MAP_FILENAME "data/TEXT/MAP/map000.txt"


//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


//=============================================================================
// ゲームのコンストラクタ
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// ゲームのデストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ゲームの生成処理
//=============================================================================
CGame *CGame::Create()
{
	CGame *pGame = NULL;	// ゲームのポインタ
	if (pGame == NULL)
	{
		pGame = new CGame;
		if (pGame != NULL)
		{
			pGame->Init();
		}
	}
	return pGame;
}

//=============================================================================
// ゲームの初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	// カメラの生成
	CreateCamera();

	//マップを生成
	CreateMap();

	//メッシュフィールドの生成
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASU_SIZE_X, MASU_SIZE_Z, 19, 15, 1, 1, NULL, true);

	// プレイヤーの生成
	m_pPlayer[0] = CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, 0);
	m_pPlayer[1] = CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, 1);

	return S_OK;
}

//=============================================================================
// ゲームの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// 共通の終了処理
	CBasemode::Uninit();

	//全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// ゲームの更新処理
//=============================================================================
void CGame::Update(void)
{
	CClient *pClient = CManager::GetClient();
	int nClientId = pClient->GetClientId();

	CScene::UpdateAll();

	// プレイヤーの座標を設定
	m_pPlayer[(nClientId + 1) % 2]->SetPos(D3DXVECTOR3(pClient->GetPlayerPos(0), pClient->GetPlayerPos(1), pClient->GetPlayerPos(2)));

	// デバッグ情報表示
	Debug();
}

//=============================================================================
// ゲームの描画処理
//=============================================================================
void CGame::Draw(void)
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
// ゲームのカメラ生成処理
//=============================================================================
void CGame::CreateCamera(void)
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
// ゲームのマップ生成処理
//=============================================================================
void CGame::CreateMap(void)
{
	CMap *pMap = CMap::Create(GAME_MAP_FILENAME);
	SetMap(pMap);
}

//=============================================================================
// ゲームのデバッグ情報処理
//=============================================================================
void CGame::Debug(void)
{
	CClient *pClient = CManager::GetClient();
	int nClientId = pClient->GetClientId();

	// クライアント番号表示
	CDebugProc::Print("自分のクライアント番号 : %d\n\n", nClientId);

	// プレイヤーの情報表示
	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerRot;
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		PlayerPos = m_pPlayer[nCnt]->GetPos();
		PlayerRot = m_pPlayer[nCnt]->GetRot();

		CDebugProc::Print("%d番のプレイヤーの位置 : %.1f %.1f %.1f\n", nCnt, PlayerPos.x, PlayerPos.y, PlayerPos.z);
		CDebugProc::Print("%d番のプレイヤーの向き : %.1f %.1f %.1f\n", nCnt, PlayerRot.x, PlayerRot.y, PlayerRot.z);
	}
}

//=============================================================================
// ゲームのプレイヤー取得
//=============================================================================
CPlayer *CGame::GetPlayer(int nIdx)
{
	return m_pPlayer[nIdx];
}