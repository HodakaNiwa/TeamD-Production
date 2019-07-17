//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "map.h"
#include "modelcreate.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "input.h"
#include "fade.h"
#include "UI.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "respawn.h"
#include "item.h"
#include "scene2D.h"
#include "server.h"
#include "sound.h"
#include "charaselect.h"
#include "effectManager.h"
#include "blossoms.h"
#include "debugproc.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define GAME_DEBUG                                                    // 宣言時デバッグコマンド適用
#ifdef GAME_DEBUG
#define GAME_DEBUG_STARTMAP (3)
#endif

#define GAME_SYSTEM_FILENAME             "data/TEXT/MODE/game.ini"    // 初期化に使用するシステムファイル名
#define GAME_STAGEDISP_TIME              (120)                        // ステージ番号表示状態になってから通常状態に切り替わるまでの時間
#define GAME_NEXTMODE_TIMING             (60)                         // 終了状態になってから次のモードに切り替わるまでの時間
#define GAME_GAMEOVER_TIMING             (180)                        // ゲームオーバー状態になってから次のモードに切り替わるまでの時間
#define GAME_GAMEOVER_POSUP              (8.0f)                       // ゲームオーバーロゴをどれくらい上げるか
#define GAME_GAMEOVER_POSYMAX            (10.0f)                      // ゲームオーバーポリゴンのY座標最大値(ポリゴンの高さに加えて判定)
#define GAME_NUMBER_INTERVAL_CUT         (0.8f)                       // 数字の間隔の倍率(ポリゴンの幅をこれで乗算する)
#define GAME_PLAYER_STOCK_INI            (3)                          // プレイヤーの残機数初期値
#define GAME_PLAYER_RESPAWN_TIMING       (180)                        // プレイヤーがリスポーンするまでの時間
#define GAME_MAPSTAGE_MAX                (3)                          // ステージの種類ごとの数
#define GAME_STAGEPOLYGON_PRIORITY       (6)                          // ステージポリゴンの描画優先順位
#define GAME_GAMEOVERPOLYGON_PRIORITY    (6)                          // ゲームオーバーポリゴンの描画優先順位

// アイテムイベント用マクロ
#define GAME_ITEMEVENT_ENEMYSTOPTIME     (300)                        // 敵をどれくらい止めておくか

// マップ更新時に消されるオブジェクト(判定用のマクロなのでここに追加)
#define GAME_DELETE_OBJECT (pScene->GetObjType() == CScene::OBJTYPE_BULLET || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER || pScene->GetObjType() == CScene::OBJTYPE_EMITTER || pScene->GetObjType() == CScene::OBJTYPE_BLOSSOMS)

// ステージ背景ポリゴン初期化用
#define GAME_STAGEBG_POS_INI             (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGEBG_COL_INI             (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))
#define GAME_STAGEBG_WIDTH_INI           (SCREEN_WIDTH / 2.0f)
#define GAME_STAGEBG_HEIGHT_INI          (SCREEN_HEIGHT / 2.0f)

// ステージロゴポリゴン初期化用
#define GAME_STAGELOGO_POS_INI           (D3DXVECTOR3(500.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGELOGO_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_STAGELOGO_WIDTH_INI         (170.0f)
#define GAME_STAGELOGO_HEIGHT_INI        (60.0f)

// ステージ番号表示用ポリゴン初期化用
#define GAME_STAGENUMBER_POS_INI         (D3DXVECTOR3(750.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGENUMBER_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_STAGENUMBER_WIDTH_INI       (40.0f)
#define GAME_STAGENUMBER_HEIGHT_INI      (40.0f)

// ゲームオーバーロゴポリゴン初期化用
#define GAME_GAMEOVERLOGO_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 900.0f, 0.0f))
#define GAME_GAMEOVERLOGO_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMEOVERLOGO_WIDTH_INI      (80.0f)
#define GAME_GAMEOVERLOGO_HEIGHT_INI     (80.0f)

// 桜の花びらデータ初期化用
#define GAME_BLOSSOMS_POS_INI            (D3DXVECTOR3(SCREEN_WIDTH - UI_BG_WIDTH_INI + 100.0f, -100.0f, 0.0f))
#define GAME_BLOSSOMS_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_BLOSSOMS_TIME_INI           (300)
#define GAME_BLOSSOMS_APPEAR_INI         (3)
#define GAME_BLOSSOMS_MOVEX_MAX_INI      (15)
#define GAME_BLOSSOMS_MOVEX_MIN_INI      (5)
#define GAME_BLOSSOMS_MOVEY_MAX_INI      (10)
#define GAME_BLOSSOMS_MOVEY_MIN_INI      (2)
#define GAME_BLOSSOMS_WIDTH_MAX_INI      (20)
#define GAME_BLOSSOMS_WIDTH_MIN_INI      (10)
#define GAME_BLOSSOMS_HEIGHT_MAX_INI     (20)
#define GAME_BLOSSOMS_HEIGHT_MIN_INI     (10)
#define GAME_BLOSSOMS_ANGLESPEED_MAX_INI (5)
#define GAME_BLOSSOMS_ANGLESPEED_MIN_INI (0)
#define GAME_BLOSSOMS_ROTSPEED_MAX_INI   (5)
#define GAME_BLOSSOMS_ROTSPEED_MIN_INI   (0)

// 値読み込み用のパス
// テクスチャ用
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// モデル用
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// マップ用
#define NUM_MAP "NUM_MAP = "

// アイテムイベント用
#define ITEMEVENT_FILENAME "ITEMEVENT_FILENAME = "
#define ENEMY_STOP "ENEMY_STOP = "

// エフェクト用
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// 弾用
#define BULLET_MODEL_IDX "BULLET_MODEL_IDX = "

// アイテム用
#define ITEM_MODEL_IDX "ITEM_MODEL_IDX = "

// 背景ポリゴン用
#define BGPOLYGONSET "BGPOLYGONSET"
#define END_BGPOLYGONSET "END_BGPOLYGONSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// 数字テクスチャ用
#define NUMBER_TEX_IDX "NUMBER_TEX_IDX = "

// ステージ背景ポリゴン用
#define STAGEBGSET "STAGEBGSET"
#define END_STAGEBGSET "END_STAGEBGSET"

// ステージロゴポリゴン用
#define STAGELOGOSET "STAGELOGOSET"
#define END_STAGELOGOSET "END_STAGELOGOSET"

// ステージ番号ポリゴン用
#define STAGENUMBERSET "STAGENUMBERSET"
#define END_STAGENUMBERSET "END_STAGENUMBERSET"

// ゲームオーバーロゴポリゴン用
#define GAMEOVERPOLYGONSET "GAMEOVERPOLYGONSET"
#define END_GAMEOVERPOLYGONSET "END_GAMEOVERPOLYGONSET"

// ひな祭りイベント用
#define HINAMATSURI_FILENAME "HINAMATSURI_FILENAME = "

// 桜の花びらデータ用
#define CHERRYBLOSSOMSSET "CHERRYBLOSSOMSSET"
#define END_CHERRYBLOSSOMSSET "END_CHERRYBLOSSOMSSET"
#define TIME "TIME = "
#define APPEAR "APPEAR = "
#define MOVEX "MOVEX = "
#define MOVEY "MOVEY = "
#define ANGLESPEED "ANGLESPEED = "
#define ROTSPEED "ROTSPEED = "

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
	CGame *pGame = NULL;
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
	// 変数を初期化する
	ClearVariable();

	// マップの番号を設定する
#ifdef GAME_DEBUG
	CCharaSelect::SetStageType(GAME_DEBUG_STARTMAP);
#endif
	m_nMapIdx = CCharaSelect::GetStageType() * GAME_MAPSTAGE_MAX;

	// システムの初期化
	LoadSystem();

	// カメラの生成
	CreateCamera();

	// UIを生成
	CreateUI();

	// プレイヤーの生成
	CreatePlayer();

	// ステージ番号の桁数を計算する
	CalcStageDigits();

	// マップの生成
	CreateMap();


	// 残機数を設定(後で外部ファイル化)
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nPlayerStock[nCntPlayer] = GAME_PLAYER_STOCK_INI;
	}

	return S_OK;
}

//=============================================================================
// ゲームの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// 各種ポインタの開放
	ReleaseUI();
	ReleasePlayer();
	ReleaseGameOverLogo();
	ReleaseStageBg();
	ReleaseStageLogo();
	ReleaseStageNumber();

	// 読み込むマップのファイル名のポインタを開放
	ReleaseMapFilePointer();

	// 共通の終了処理
	CBasemode::Uninit();

	// 全てのオブジェクト開放
	CScene::ReleaseAll();

	// クライアントを開放する
	CManager::ReleaseClient();
}

//=============================================================================
// ゲームの更新処理
//=============================================================================
void CGame::Update(void)
{
	// 敵を動かせるかチェック
	m_nEnemyMoveCounter--;
	if (m_nEnemyMoveCounter <= 0)
	{
		m_nEnemyMoveCounter = 0;
		m_bEnemyMove = true;
	}

	// 状態によって処理わけ
	switch (m_State)
	{
	case STATE_STAGE_SELECT:
		StageSelectUpdate();
		CScene::DeathCheck();
		break;
	case STATE_STAGE_DISP:
		StageDispUpdate();
		CScene::DeathCheck();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		CScene::UpdateAll();
		break;
	case STATE_GAMEOVER:
		GameOverUpdate();
		CScene::UpdateAll();
		break;
	case STATE_CHANGE_MAP:
		ChangeMapUpdate();
		CScene::UpdateAll();
		break;
	case STATE_PREV_MAP:
		ChangePrevMap();
		CScene::DeathCheck();
		break;
	case STATE_NEXT_MAP:
		ChangeNextMap();
		CScene::DeathCheck();
		break;
	case STATE_END:
		EndUpdate();
		CScene::UpdateAll();
		break;
	}

	// マップの状態によって処理わけ
	switch (CCharaSelect::GetStageType())
	{
	case MAPTYPE_CANDY:        // お菓子
		MapEvent_Candy();
		break;
	case MAPTYPE_CHRISTMAS:    // クリスマス
		MapEvent_Christmas();
		break;
	case MAPTYPE_NEWYEAR:      // お正月
		MapEvent_NewYear();
		break;
	case MAPTYPE_HINAMATSURI:  // ひな祭り
		MapEvent_Hinamatsuri();
		break;
	}

	// プレイヤーをリスポーンさせるかチェック
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (CManager::GetClient()->GetClientId() == nCntPlayer)
		{
			CheckPlayerResSpawn(nCntPlayer);
		}
	}

	// 敵を生成するかチェックする
	if (CManager::GetClient()->GetClientId() == 0 && m_State != STATE_STAGE_SELECT && m_State != STATE_STAGE_DISP)
	CheckEnemySpawn(m_nGameCounter);

	// サーバーへ送るデータを設定する
	SetDataToServer();

	// クライアントの更新処理
	CManager::ClientUpdate();

	// サーバーから送られたデータを設定する
	GetDataFromServer();

	// データクリア
	m_bDeletePlayerFlag = false;
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");


	if (CManager::GetClient() == NULL)return;
	CDebugProc::Print(1, "ゲーム画面\n");
	CDebugProc::Print(1, "クライアント番号      : %d\n", CManager::GetClient()->GetClientId());
	CDebugProc::Print(1, "読み込んだマップ数    : %d\n", m_nNumMap);
	CDebugProc::Print(1, "マップの種類番号      : %d\n", CCharaSelect::GetStageType());
	CDebugProc::Print(1, "プレイヤー1の種類番号 : %d\n", CCharaSelect::GetPlayerNumber(0));
	CDebugProc::Print(1, "プレイヤー2の種類番号 : %d\n\n", CCharaSelect::GetPlayerNumber(1));
	CDebugProc::Print(1, "現在のマップ番号      : %d\n", m_nMapIdx);
	CDebugProc::Print(1, "現在のステージ番号    : %d\n", m_nStageIdx);
	CDebugProc::Print(1, "ゲームカウンター      : %d\n", m_nGameCounter);
	CDebugProc::Print(1, "現在生成された敵の数  : %d\n", m_nSpawnEnemyCount);
	CDebugProc::Print(1, "残りの敵の数          : %d\n", m_nNumEnemy);
	CDebugProc::Print(1, "プレイヤー1の残機数   : %d\n", m_nPlayerStock[0]);
	CDebugProc::Print(1, "プレイヤー2の残機数   : %d\n", m_nPlayerStock[1]);
	CDebugProc::Print(1, "リスポーンカウンター  : %d\n", m_nPlayerRespawnCounter);
	CDebugProc::Print(1, "プレイヤー1の弾の数   : %d\n", CBulletPlayer::GetNumAll_0());
	CDebugProc::Print(1, "プレイヤー2の弾の数   : %d\n", CBulletPlayer::GetNumAll_1());
	CDebugProc::Print(1, "敵の弾の数            : %d\n", CBulletEnemy::GetNumAll());
	CDebugProc::Print(1, "ブロックの数          : %d\n", CBlock::GetNumAll());
	CDebugProc::Print(1, "スポーンした敵の数    : %d\n", CEnemy::GetSpawnCounter());
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


//*****************************************************************************
//
// サーバーの更新用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームの消すブロックのデータを設定
//=============================================================================
void CGame::DeleteBlock(const int nIdx)
{
	m_nNumDeleteBlock++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteBlock, aData);
	strcat(m_aDeleteBlock, " ");
}

//=============================================================================
// ゲームの消す敵のデータを設定
//=============================================================================
void CGame::DeleteEnemy(const int nIdx)
{
	m_nNumDeleteEnemy++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteEnemy, aData);
	strcat(m_aDeleteEnemy, " ");
}

//=============================================================================
// ゲームのプレイヤーを空にする処理
//=============================================================================
void CGame::DeletePlayer(CPlayer *pPlayer, const int nIdx)
{
	SetPlayer(pPlayer, nIdx);
	if (m_nPlayerStock[nIdx] <= 0)
	{
		SetState(STATE_GAMEOVER);
	}

	// ホストじゃなかったら
	if (CManager::GetClient() == NULL)return;
	if (CManager::GetClient()->GetClientId() == 0 && nIdx != 0)
	{
		m_bDeletePlayerFlag = true;
	}
}

//=============================================================================
// ゲームのサーバーに送るデータを設定する処理
//=============================================================================
void CGame::SetDataToServer(void)
{
	// プレイヤーのデータを設定
	SetDataToServerFromPlayer();

	if (CManager::GetClient()->GetClientId() == 0)
	{// 自分がホストなら
	    // ゲームのデータを設定
		SetDataToServerFromGame();
		// 敵のデータを設定
		SetDataToServerFromEnemy();
		// 敵の弾のデータを設定
		SetDataToServerFromEnemyBullet();
		// 消すブロックのデータを設定
		SetDataToServerFromDeleteBlock();
	}

	// プレイヤーの弾情報を設定
	SetDataToServerFromPlayerBullet();

	if (CManager::GetClient()->GetClientId() != 0)
	{// 自分がホストでないなら
		// 消すブロックのデータを設定
		SetDataToServerFromDeleteBlock();

		// 消す敵のデータを設定
		SetDataToServerFromDeleteEnemy();
	}
}

//=============================================================================
// ゲームのサーバーに送るプレイヤーデータを設定する処理
//=============================================================================
void CGame::SetDataToServerFromPlayer(void)
{
	// プレイヤーの人数を設定
	if (m_pPlayer[CManager::GetClient()->GetClientId()] == NULL)
	{
		CManager::GetClient()->Print("0");
		CManager::GetClient()->Print(" ");
		return;
	}
	else if(m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
	{
		CManager::GetClient()->Print("1");
		CManager::GetClient()->Print(" ");

		// プレイヤーの座標を設定
		D3DXVECTOR3 PlayerPos = m_pPlayer[CManager::GetClient()->GetClientId()]->GetPos();
		CManager::GetClient()->Print("%.1f %.1f %.1f", PlayerPos.x, PlayerPos.y, PlayerPos.z);
		CManager::GetClient()->Print(" ");

		// プレイヤーの向きを設定
		D3DXVECTOR3 PlayerRot = m_pPlayer[CManager::GetClient()->GetClientId()]->GetRot();
		CManager::GetClient()->Print("%.1f %.1f %.1f", PlayerRot.x, PlayerRot.y, PlayerRot.z);
		CManager::GetClient()->Print(" ");
	}

	// ホストだったら
	if (CManager::GetClient()->GetClientId() == 0)
	{
		CManager::GetClient()->Print("%d", (int)m_bDeletePlayerFlag);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// ゲームのサーバーに送るゲームデータを設定する処理
//=============================================================================
void CGame::SetDataToServerFromGame(void)
{
	// 状態を設定
	CManager::GetClient()->Print("%d", m_State);
	CManager::GetClient()->Print(" ");

	// マップの番号を設定
	CManager::GetClient()->Print("%d", m_nMapIdx);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// ゲームのサーバーに送る敵データを設定する処理
//=============================================================================
void CGame::SetDataToServerFromEnemy(void)
{
	int nNumEnemy = 0;
	char aData[64] = "\0";
	char aEnemyData[MAX_SERVER_DATA] = "\0";

	// 敵のデータを設定
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// 敵クラスだった
				strcat(aEnemyData, GetDataToEnemy((CEnemy*)pScene, aData));
				strcpy(aData, "\0");
				nNumEnemy++;
			}
			pScene = pSceneNext;
		}
	}

	// 敵の数を設定
	CManager::GetClient()->Print("%d", nNumEnemy);
	CManager::GetClient()->Print(" ");

	// 敵のデータを設定
	CManager::GetClient()->Print("%s", aEnemyData);
}

//=============================================================================
// 敵のデータを文字列に変換する処理
//=============================================================================
char *CGame::GetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	char aData[64] = "\0";

	// 敵の座標を設定
	D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
	sprintf(aData, "%.1f %.1f %.1f", EnemyPos.x, EnemyPos.y, EnemyPos.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// 敵の向きを設定
	D3DXVECTOR3 EnemyRot = pEnemy->GetRot();
	sprintf(aData, "%.1f %.1f %.1f", EnemyRot.x, EnemyRot.y, EnemyRot.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// 敵の番号
	int nEnemyIdx = pEnemy->GetIdx();
	sprintf(aData, "%d", nEnemyIdx);
	strcat(pStr, aData);
	strcat(pStr, " ");

	return pStr;
}

//=============================================================================
// ゲームのサーバーに送るプレイヤーの弾データを設定する処理
//=============================================================================
void CGame::SetDataToServerFromPlayerBullet(void)
{
	int nNumBullet = 0;
	char aData[64] = "\0";
	char aBulletData[MAX_SERVER_DATA] = "\0";

	// 現在のプレイヤーの弾の数を設定
	if (CManager::GetClient()->GetClientId() == 0)
	{
		nNumBullet = CBulletPlayer::GetNumAll_0();
	}
	else
	{
		nNumBullet = CBulletPlayer::GetNumAll_1();
	}

	// 弾の数を設定
	CManager::GetClient()->Print("%d", nNumBullet);
	CManager::GetClient()->Print(" ");
	if (nNumBullet < 1) return;

	// 弾のデータを設定する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// 弾クラスだった
				strcat(aBulletData, GetDataToPlayerBullet((CBullet*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// 弾のデータを設定
	CManager::GetClient()->Print("%s", aBulletData);
}

//=============================================================================
// プレイヤーの弾のデータを文字列に変換する処理
//=============================================================================
char *CGame::GetDataToPlayerBullet(CBullet *pBullet, char *pStr)
{
	if (pBullet->GetType() == CManager::GetClient()->GetClientId())
	{// 自分のプレイヤーが撃った弾の情報を書き込む
		char aData[64] = "\0";

		// 弾の座標を設定
		D3DXVECTOR3 BulletPos = pBullet->GetPos();
		sprintf(aData, "%.1f %.1f %.1f", BulletPos.x, BulletPos.y, BulletPos.z);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// 弾の番号を設定
		int nBulletIdx = pBullet->GetIdx();
		sprintf(aData, "%d", nBulletIdx);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// 弾の種類を設定
		int nBulletType = pBullet->GetType();
		sprintf(aData, "%d", nBulletType);
		strcat(pStr, aData);
		strcat(pStr, " ");
	}

	return pStr;
}

//=============================================================================
// ゲームのサーバーに送る敵の弾データを設定する処理
//=============================================================================
void CGame::SetDataToServerFromEnemyBullet(void)
{
	int nNumBullet = 0;
	char aData[64] = "\0";
	char aBulletData[MAX_SERVER_DATA] = "\0";

	// 敵の弾の数を設定
	nNumBullet = CBulletEnemy::GetNumAll();

	// 弾の数を設定
	CManager::GetClient()->Print("%d", nNumBullet);
	CManager::GetClient()->Print(" ");
	if (nNumBullet < 1) return;

	// 弾のデータを設定する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// 弾クラスだった
				strcat(aBulletData, GetDataToEnemyBullet((CBullet*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// 弾のデータを設定
	CManager::GetClient()->Print("%s", aBulletData);
}

//=============================================================================
// 敵の弾のデータを文字列に変換する処理
//=============================================================================
char *CGame::GetDataToEnemyBullet(CBullet *pBullet, char *pStr)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{// 敵の弾データを設定する
		char aData[64] = "\0";

		// 弾の座標を設定
		D3DXVECTOR3 BulletPos = pBullet->GetPos();
		sprintf(aData, "%.1f %.1f %.1f", BulletPos.x, BulletPos.y, BulletPos.z);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// 弾の番号を設定
		int nBulletIdx = pBullet->GetIdx();
		sprintf(aData, "%d", nBulletIdx);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// 弾の種類を設定
		int nBulletType = pBullet->GetType();
		sprintf(aData, "%d", nBulletType);
		strcat(pStr, aData);
		strcat(pStr, " ");
	}

	return pStr;
}

//=============================================================================
// ゲームのサーバーに送る消すブロックのデータを設定する処理
//=============================================================================
void CGame::SetDataToServerFromDeleteBlock(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteBlock);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteBlock > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteBlock);
		CManager::GetClient()->Print(" ");
	}
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
}

//=============================================================================
// ゲームのサーバーに送る消す敵のデータを設定する処理
//=============================================================================
void CGame::SetDataToServerFromDeleteEnemy(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteEnemy);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteEnemy > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteEnemy);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// ゲームのサーバーから送られたデータを設定する処理
//=============================================================================
void CGame::GetDataFromServer(void)
{
	// 送られたメッセージを取得
	char *pStr = CManager::GetClient()->GetReceiveMessage();
	if (pStr == NULL) return;
	if (*pStr == *"???") return;

	// プレイヤーのデータを設定
	pStr = SetDataToPlayerFromServer(pStr);

	if (CManager::GetClient()->GetClientId() != 0)
	{// ホストじゃなかったら
	    // ゲームの状態を設定
		pStr = SetDataToGameFromServer(pStr);
		// 敵のデータを設定
		pStr = SetDataToEnemyFromServer(pStr);
		// 敵の弾のデータを設定
		pStr = SetDataToEnemyBulletFromServer(pStr);
		// 消すブロックのデータを設定
		pStr = SetDataToDeleteBlock(pStr);
	}

	// プレイヤーの弾のデータを設定
	pStr = SetDataToPlayerBulletFromServer(pStr);

	if (CManager::GetClient()->GetClientId() == 0)
	{// ホストだったら
		// 消すブロックのデータを設定
		pStr = SetDataToDeleteBlock(pStr);

		// 消す敵のデータを設定
		pStr = SetDataToDeleteEnemy(pStr);
	}

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// ゲームのサーバーから送られたデータをプレイヤーに設定する処理
//=============================================================================
char *CGame::SetDataToPlayerFromServer(char *pStr)
{
	int nWord = 0;
	int nNumPlayer = 0;

	// プレイヤーの人数を読み取る
	nNumPlayer = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumPlayer == 0)
	{// プレイヤーが存在しない
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL && m_State == STATE_NORMAL)
		{// プレイヤーが生きている
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->Uninit();
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] = NULL;
			m_nPlayerStock[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]--;
		}
	}
	else if(nNumPlayer == 1)
	{// プレイヤーが存在する
		CMap *pMap = GetMap();
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] == NULL && pMap != NULL)
		{// メモリが確保されていない
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] =
				CPlayer::Create(INITIALIZE_D3DXVECTOR3,
					INITIALIZE_D3DXVECTOR3, (CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER);
		}
		if(m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL)
		{// メモリが確保できている
			// プレイヤーの座標を読み取る
			float fGetPlayerPosX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// プレイヤーの向きを読み取る
			float fGetPlayerRotX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// プレイヤーに値を設定
			if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL)
			{
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetPos(D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ));
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetRot(D3DXVECTOR3(fGetPlayerRotX, fGetPlayerRotY, fGetPlayerRotZ));
			}
		}
	}


	// ホストじゃなかったら
	if (CManager::GetClient()->GetClientId() != 0)
	{
		nNumPlayer = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		if (nNumPlayer == 1 && m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->Uninit();
			m_pPlayer[CManager::GetClient()->GetClientId()] = NULL;
		}
	}

	return pStr;
}

//=============================================================================
// ゲームのサーバーから送られたデータをゲームに設定する処理
//=============================================================================
char *CGame::SetDataToGameFromServer(char *pStr)
{
	int nGameState = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	int nMapIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 状態を設定
	m_StateOld = m_State;
	m_State = (STATE)nGameState;
	if (m_State != STATE_END && m_State != STATE_GAMEOVER)
	{// 終了状態でもゲームオーバー状態でもない
		m_nStateCounter = 0;
	}

	// マップの番号を設定
	m_nMapIdx = nMapIdx;

	// 状態によって処理わけ
	if (m_State == STATE_STAGE_DISP && GetMap() == NULL)
	{
		CreateMap();
		CManager::GetSound()->PlaySound(1);
	}
	else if (m_State == STATE_STAGE_SELECT && GetMap() != NULL)
	{
		ReleaseMap();
		DeleteGameObject();
	}
	else if (m_State == STATE_STAGE_DISP && m_StateOld == STATE_NORMAL)
	{
		ReleaseMap();
		DeleteGameObject();
	}
	else if (m_State == STATE_NORMAL && m_StateOld == STATE_STAGE_DISP)
	{
		ReleaseStageBg();
		ReleaseStageLogo();
		ReleaseStageNumber();
		SetPlayerPosToSpawn();
	}
	else if (m_State == STATE_STAGE_DISP || m_State == STATE_STAGE_SELECT)
	{
		ReleaseStageNumber();
		CalcStageDigits();
		CreateStageNumber();
	}

	return pStr;
}

//=============================================================================
// ゲームのサーバーから送られたデータを敵に設定する処理
//=============================================================================
char *CGame::SetDataToEnemyFromServer(char *pStr)
{
	int nWord = 0;
	int nNumEnemy = 0;

	// 生成されている敵の数を読み取る
	nNumEnemy = CFunctionLib::ReadInt(pStr, "");
	m_nNumEnemy = nNumEnemy;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 数合わせ
	ReleaseEnemy(nNumEnemy);
	if (nNumEnemy == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// 敵クラスだった
				pStr = SetDataToEnemy((CEnemy*)pScene, pStr);
				nNumEnemy--;
			}
			pScene = pSceneNext;
		}
	}

	// 足りない敵は新たに生成しデータを設定
	for (int nCntEnemy = 0; nCntEnemy < nNumEnemy; nCntEnemy++)
	{
		pStr = SetDataToCreateEnemy(pStr);
	}

	return pStr;
}

//=============================================================================
// 文字列をデータに変換し敵に設定する
//=============================================================================
char *CGame::SetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyIdx = 0;

	// 敵の座標を読み取る
	fEnemyPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の向きを読み取る
	fEnemyRotX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の番号
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵にデータを設定
	pEnemy->SetPos(D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ));
	pEnemy->SetRot(D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ));
	pEnemy->SetIdx(nEnemyIdx);

	return pStr;
}

//=============================================================================
// 文字列をデータに変換し生成した敵に設定する
//=============================================================================
char *CGame::SetDataToCreateEnemy(char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyIdx = 0;

	// 敵の座標を読み取る
	fEnemyPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の向きを読み取る
	fEnemyRotX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の番号
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵を生成する(同時に必要なデータを設定)
	CEnemy *pEnemy = CEnemy::Create(D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ), D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ));
	pEnemy->SetIdx(nEnemyIdx);
	return pStr;
}

//=============================================================================
// プレイヤーの弾の数を合わせる処理
//=============================================================================
void CGame::ReleaseEnemy(int nNumEnemy)
{
	// 現在の敵の数を設定
	int nAllEnemy = 0;
	nAllEnemy = CEnemy::GetNumAll();
	if (nAllEnemy < nNumEnemy)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && nAllEnemy > nNumEnemy)
			{// 弾クラスだった
				ReleaseCheckEnemy((CEnemy*)pScene, &nAllEnemy);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// 敵を破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckEnemy(CEnemy *pEnemy, int *pNumEnemy)
{
	pEnemy->Uninit();
	pEnemy = NULL;
	*pNumEnemy = *pNumEnemy - 1;
}

//=============================================================================
// ゲームのサーバーから送られたデータをプレイヤーの弾に設定する処理
//=============================================================================
char *CGame::SetDataToPlayerBulletFromServer(char *pStr)
{
	int nWord = 0;
	int nNumBullet = 0;

	// 生成されている弾の数を読み取る
	nNumBullet = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 数合わせ
	ReleasePlayerBullet(nNumBullet);
	if (nNumBullet == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// 弾クラスだった
				pStr = SetDataToPlayerBullet((CBullet*)pScene, pStr, &nNumBullet);
			}
			pScene = pSceneNext;
		}
	}

	// 足りない弾は新たに生成しデータを設定
	for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
	{
		pStr = SetDataToCreatePlayerBullet(pStr);
	}

	return pStr;
}

//=============================================================================
// プレイヤーの弾の数を合わせる処理
//=============================================================================
void CGame::ReleasePlayerBullet(int nNumBullet)
{
	// 現在のプレイヤーの弾の数を設定
	int nAllBullet = 0;
	if (CManager::GetClient()->GetClientId() == 0)
	{
		nAllBullet = CBulletPlayer::GetNumAll_1();
	}
	else
	{
		nAllBullet = CBulletPlayer::GetNumAll_0();
	}
	if (nAllBullet < nNumBullet)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nAllBullet > nNumBullet)
			{// 弾クラスだった
				ReleaseCheckPlayerBullet((CBullet*)pScene, &nAllBullet);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// プレイヤーの弾を破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckPlayerBullet(CBullet *pBullet, int *pNumBullet)
{
	if (pBullet->GetType() != CBullet::TYPE_ENEMY &&
		pBullet->GetType() != CManager::GetClient()->GetClientId())
	{
		pBullet->Uninit();
		pBullet = NULL;
		*pNumBullet = *pNumBullet - 1;
	}
}

//=============================================================================
// 文字列をデータに変換しプレイヤーの弾に設定する
//=============================================================================
char *CGame::SetDataToPlayerBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY ||
		pBullet->GetType() == CManager::GetClient()->GetClientId()) return pStr;

	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// 弾の座標を読み取る
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の番号を読み取る
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の種類を読み取る
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// プレイヤーの弾にデータを設定
	pBullet->SetPos(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ));
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);
	*pNumBullet = *pNumBullet - 1;

	return pStr;
}

//=============================================================================
// 文字列をデータに変換し生成したプレイヤーの弾に設定する
//=============================================================================
char *CGame::SetDataToCreatePlayerBullet(char *pStr)
{
	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// 弾の座標を読み取る
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の番号を読み取る
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の種類を読み取る
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾を生成する(同時に必要なデータを設定)
	CBulletPlayer *pBullet = CBulletPlayer::Create(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ), INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
		(CBullet::TYPE)((CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER), m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]);
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);

	return pStr;
}

//=============================================================================
// ゲームのサーバーから送られたデータを敵の弾に設定する処理
//=============================================================================
char *CGame::SetDataToEnemyBulletFromServer(char *pStr)
{
	int nWord = 0;
	int nNumBullet = 0;

	// 生成されている弾の数を読み取る
	nNumBullet = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 数合わせ
	ReleaseEnemyBullet(nNumBullet);
	if (nNumBullet == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nNumBullet > 0)
			{// 弾クラスだった
				pStr = SetDataToEnemyBullet((CBullet*)pScene, pStr, &nNumBullet);
			}
			pScene = pSceneNext;
		}
	}

	// 足りない弾は新たに生成しデータを設定
	for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
	{
		pStr = SetDataToCreateEnemyBullet(pStr);
	}

	return pStr;
}

//=============================================================================
// 敵の弾の数を合わせる処理
//=============================================================================
void CGame::ReleaseEnemyBullet(int nNumBullet)
{
	// 現在の敵の弾の数を設定
	int nAllBullet = 0;
	nAllBullet = CBulletEnemy::GetNumAll();
	if (nAllBullet < nNumBullet)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nAllBullet > nNumBullet)
			{// 弾クラスだった
				ReleaseCheckEnemyBullet((CBullet*)pScene, &nAllBullet);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// 敵の弾を破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckEnemyBullet(CBullet *pBullet, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{
		pBullet->Uninit();
		pBullet = NULL;
		*pNumBullet = *pNumBullet - 1;
	}
}

//=============================================================================
// 文字列をデータに変換し弾に設定する
//=============================================================================
char *CGame::SetDataToEnemyBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{
		int nWord = 0;
		float fBulletPosX = 0.0f;
		float fBulletPosY = 0.0f;
		float fBulletPosZ = 0.0f;
		int nBulletIdx = 0;
		int nBulletType = 0;

		// 弾の座標を読み取る
		fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// 弾の番号を読み取る
		nBulletIdx = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// 弾の種類を読み取る
		nBulletType = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// 弾にデータを設定
		pBullet->SetPos(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ));
		pBullet->SetIdx(nBulletIdx);
		pBullet->SetType((CBullet::TYPE)nBulletType);
		*pNumBullet = *pNumBullet - 1;
	}

	return pStr;
}

//=============================================================================
// 文字列をデータに変換し生成した敵の弾に設定する
//=============================================================================
char *CGame::SetDataToCreateEnemyBullet(char *pStr)
{
	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// 弾の座標を読み取る
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の番号を読み取る
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の種類を読み取る
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾を生成する(同時に必要なデータを設定)
	CBulletEnemy *pBullet = CBulletEnemy::Create(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ), INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, NULL);
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);

	return pStr;
}

//=============================================================================
// ゲームの消すブロックのデータを設定
//=============================================================================
char *CGame::SetDataToDeleteBlock(char *pStr)
{
	int nWord = 0;
	int nNumDeleteBlock = 0;
	int *pDeleteIdx = NULL;

	// 消すブロックの数を読み取る
	nNumDeleteBlock = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteBlock == 0) return pStr;

	// 消す分だけメモリを確保
	pDeleteIdx = new int[nNumDeleteBlock];
	for (int nCnt = 0; nCnt < nNumDeleteBlock; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// 消す数だけブロックを開放する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && m_State == STATE_NORMAL)
			{// ブロッククラスだった
				ReleaseCheckBlock((CBlock*)pScene, pDeleteIdx, &nNumDeleteBlock);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();

	// メモリの開放
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// ゲームのブロックを破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckBlock(CBlock *pBlock, int *pDeleteIdx, int *nNumDeleteBlock)
{
	int nIdx = pBlock->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteBlock; nCntDeleteNum++)
	{// 消す数分繰り返し
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pBlock != NULL)
		{// 番号一致
			pBlock->Uninit();
			pBlock = NULL;
		}
	}
}

//=============================================================================
// ゲームの消す敵のデータを設定
//=============================================================================
char *CGame::SetDataToDeleteEnemy(char *pStr)
{
	int nWord = 0;
	int nNumDeleteEnemy = 0;
	int nCntDelete = 0;
	int *pDeleteIdx = NULL;

	// 消す敵の数を読み取る
	nNumDeleteEnemy = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteEnemy == 0) return pStr;

	// 消す分だけメモリを確保
	pDeleteIdx = new int[nNumDeleteEnemy];
	for (int nCnt = 0; nCnt < nNumDeleteEnemy; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// 消す数だけ敵を開放する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && m_State == STATE_NORMAL)
			{// 敵クラスだった
				ReleaseCheckDeleteEnemy((CEnemy*)pScene, pDeleteIdx, &nNumDeleteEnemy);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();

	// メモリの開放
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// ゲームの敵を破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckDeleteEnemy(CEnemy *pEnemy, int *pDeleteIdx, int *nNumDeleteEnemy)
{
	int nIdx = pEnemy->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteEnemy; nCntDeleteNum++)
	{// 消す数分繰り返し
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pEnemy != NULL)
		{// 番号一致
			pEnemy->Uninit();
			pEnemy = NULL;
		}
	}
}


//*****************************************************************************
//
// 状態による処理わけ用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのステージセレクト状態の更新処理
//=============================================================================
void CGame::StageSelectUpdate(void)
{
	// 背景生成
	if (m_pStageBg == NULL)
	{
		CreateStageBg();
	}

	// ステージロゴ生成
	if (m_pStageLogo == NULL)
	{
		CreateStageLogo();
	}

	// ステージ番号生成
	if (m_pStageNumber == NULL)
	{
		CreateStageNumber();
	}


	if (CManager::GetClient()->GetClientId() != 0)
	{
		ReleaseStageNumber();
		CalcStageDigits();
		CreateStageNumber();
	}

	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL || CManager::GetClient()->GetClientId() != 0) return;

	if (pKey->GetTrigger(DIK_W) == true || pKey->GetRepeat(DIK_W) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_0) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_0) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_16) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_16) == true)
	{// 上方向の入力がされた
		if (m_nStageIdx < GAME_MAPSTAGE_MAX - 1)
		{
			m_nStageIdx++;
			m_nMapIdx++;
			ReleaseStageNumber();
			CalcStageDigits();
			CreateStageNumber();
		}
	}
	else if (pKey->GetTrigger(DIK_S) == true || pKey->GetRepeat(DIK_S) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_1) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_1) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_17) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_17) == true)
	{// 下方向の入力がされた
		if (m_nStageIdx > 0)
		{
			m_nStageIdx--;
			m_nMapIdx--;
			ReleaseStageNumber();
			CalcStageDigits();
			CreateStageNumber();
		}
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		SetState(STATE_STAGE_DISP);
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
		CManager::GetSound()->PlaySound(1);
	}
}

//=============================================================================
// ゲームのステージ番号を表示する状態の更新処理
//=============================================================================
void CGame::StageDispUpdate(void)
{
	// 背景生成
	if (m_pStageBg == NULL)
	{
		CreateStageBg();
	}

	// ステージロゴ生成
	if (m_pStageLogo == NULL)
	{
		CreateStageLogo();
	}

	// ステージ番号生成
	if (m_pStageNumber == NULL)
	{
		CreateStageNumber();
	}

	// カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter % GAME_STAGEDISP_TIME == 0)
	{// ステージ番号を出してからある程度たった
		SetState(STATE_NORMAL);
		ReleaseStageBg();
		ReleaseStageLogo();
		ReleaseStageNumber();
	}
}

//=============================================================================
// ゲームの通常状態の更新処理
//=============================================================================
void CGame::NormalUpdate(void)
{
#ifdef GAME_DEBUG  // デバッグコマンド適用時

	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL) return;

	// ステージセレクトデバッグ
	if (CManager::GetClient()->GetClientId() == 0)
	{
		if (pKey->GetTrigger(DIK_1) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_PREV_MAP);
		}
		else if (pKey->GetTrigger(DIK_2) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_NEXT_MAP);
		}
		else if (pKey->GetTrigger(DIK_3) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_STAGE_SELECT);
		}
	}

	// 状態チェックデバッグ
	if (pKey->GetTrigger(DIK_4) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		SetState(STATE_GAMEOVER);
	}
	else if (pKey->GetTrigger(DIK_5) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		SetState(STATE_END);
	}


	// アイテムイベントデバッグ
	if (pKey->GetTrigger(DIK_F1) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Star(0);
	}
	else if (pKey->GetTrigger(DIK_F2) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Star(1);
	}
	else if (pKey->GetTrigger(DIK_F3) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Grenade();
	}
	else if (pKey->GetTrigger(DIK_F4) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_1Up(0);
	}
	else if (pKey->GetTrigger(DIK_F5) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_1Up(1);
	}
	else if (pKey->GetTrigger(DIK_F6) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Scoop();
	}
	else if (pKey->GetTrigger(DIK_F7) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Clock();
	}
	else if (pKey->GetTrigger(DIK_F8) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Helmet(0);
	}
	else if (pKey->GetTrigger(DIK_F9) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Helmet(1);
	}

	// ステージイベントデバッグ
	if (CManager::GetKeyboard()->GetTrigger(DIK_1) == true && CManager::GetKeyboard()->GetPress(DIK_RSHIFT) == true)
	{
		m_HinaEvent = HINAEVENT_CHERRY_BLOSSOMS;
		m_nEventCounter = 0;
	}

	CDebugProc::Print(1, "デバッグコマンド\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [1]キー        : 前のステージにする\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [2]キー        : 次のステージにする\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [3]キー        : ステージセレクト状態にする\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [4]キー        : ゲームオーバー状態にする\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [5]キー        : 終了状態にする\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F1]～[F2]キー : スターアイテムイベント\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F3]キー       : グレネードアイテムイベント\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F4]～[F5]キー : 1Upアイテムイベント\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F6]キー       : スコップアイテムイベント\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F7]キー       : 時計アイテムイベント\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F8]～[F9]キー : ヘルメットアイテムイベント\n");
	CDebugProc::Print(1, "[RSHIFT]キー + [1]キー        : 花びら舞い散る\n\n");

#endif

	CDebugProc::Print(1, "通常状態\n");
}

//=============================================================================
// ゲームのゲームオーバー状態の更新処理
//=============================================================================
void CGame::GameOverUpdate(void)
{
	// ゲームオーバーのロゴを出す
	if (m_pGameOverLogo == NULL)
	{
		CreateGameOverLogo();
		return;
	}

	// ゲームオーバーロゴポリゴンを上げる
	GameOverLogoUp();

	// カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter % GAME_GAMEOVER_TIMING == 0)
	{
		CFade *pFade = CManager::GetFade();
		if (pFade == NULL) return;
		if (pFade->GetFade() != CFade::FADE_NONE) return;

		// 次のモードに設定
		pFade->SetFade(CManager::MODE_TITLE);
	}

	CDebugProc::Print(1, "ゲームオーバー状態\n");
}

//=============================================================================
// ゲームのゲームオーバーロゴを上にあげる処理
//=============================================================================
void CGame::GameOverLogoUp(void)
{
	if (m_pGameOverLogo != NULL)
	{
		D3DXVECTOR3 GameOverLogoPos = m_pGameOverLogo->GetPos();
		GameOverLogoPos.y -= GAME_GAMEOVER_POSUP;
		if (GameOverLogoPos.y <= m_pGameOverLogo->GetHeight() + GAME_GAMEOVER_POSYMAX)
		{
			GameOverLogoPos.y += GAME_GAMEOVER_POSUP;
		}

		m_pGameOverLogo->SetPos(GameOverLogoPos);
		m_pGameOverLogo->SetVtxBuffPos();
	}
}

//=============================================================================
// ゲームのゲームクリア状態の更新処理
//=============================================================================
void CGame::GameClearUpdate(void)
{

}

//=============================================================================
// ゲームのマップを変える状態の更新処理
//=============================================================================
void CGame::ChangeMapUpdate(void)
{
	if (CManager::GetClient()->GetClientId() == 0)
	{// ホストだったら
		m_nMapIdx++;
		m_nStageIdx++;
		CalcStageDigits();
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
		SetState(STATE_STAGE_DISP);
		CManager::GetSound()->PlaySound(1);
	}
	else
	{// ホストじゃなかったら
		m_nMapIdx++;
		m_nStageIdx++;
		CalcStageDigits();
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
		SetState(STATE_STAGE_DISP);
		CManager::GetSound()->PlaySound(1);
	}

	// データクリア
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
}

//=============================================================================
// ゲームの終了状態の更新処理
//=============================================================================
void CGame::EndUpdate(void)
{
	m_nStateCounter++;
	if (m_nStateCounter % GAME_NEXTMODE_TIMING == 0)
	{
		if (m_nStageIdx >= GAME_MAPSTAGE_MAX - 1)
		{// これ以上ステージがない
			CFade *pFade = CManager::GetFade();
			if (pFade == NULL) return;
			if (pFade->GetFade() != CFade::FADE_NONE) return;

			// 次のモードに設定
			pFade->SetFade(CManager::MODE_RESULT);
		}
		else
		{// まだ次のマップが存在する
			if (CManager::GetClient()->GetClientId() == 0)
			{// ホストだった
				SetState(STATE_CHANGE_MAP);
			}
		}
	}

	CDebugProc::Print(1, "終了状態\n");
}



//*****************************************************************************
//
// アイテムイベント処理用関数
// Auther : Hodaka Niwa & Jukiya Hayakawa
//
//*****************************************************************************
//=============================================================================
// ゲームのスターアイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_Star(int nPlayerNumber)
{
	// プレイヤーのパラメータをパワーアップさせる

}

//=============================================================================
// ゲームのグレネードアイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_Grenade(void)
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
// ゲームの1Upアイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_1Up(int nPlayerNumber)
{
	// プレイヤーの残機数を増やす
	m_nPlayerStock[nPlayerNumber]++;
}

//=============================================================================
// ゲームのスコップアイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_Scoop(void)
{
	// 司令部の周りに壊せないブロックを置く
}

//=============================================================================
// ゲームの時計アイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_Clock(void)
{
	// 敵が動けない状態にする
	m_bEnemyMove = false;
	m_nEnemyMoveCounter = m_ItemEventData.nStop;
}

//=============================================================================
// ゲームのヘルメットアイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_Helmet(int nPlayerNumber)
{
	// プレイヤーを無敵状態にする
}


//*****************************************************************************
//
// マップイベント用関数
// Auther : Hodaka Niwa & Jukiya Hayakawa & Kobayashi Shouhei & Gokan Akane
//
//*****************************************************************************
//=============================================================================
// ゲームのお菓子のマップイベント処理
//=============================================================================
void CGame::MapEvent_Candy(void)
{

}

//=============================================================================
// ゲームのクリスマスのマップイベント処理
//=============================================================================
void CGame::MapEvent_Christmas(void)
{

}

//=============================================================================
// ゲームのお正月のマップイベント処理
//=============================================================================
void CGame::MapEvent_NewYear(void)
{

}

//=============================================================================
// ゲームのひな祭りのマップイベント処理
//=============================================================================
void CGame::MapEvent_Hinamatsuri(void)
{
	CDebugProc::Print(1, "ひな祭りイベント\n");

	switch (m_HinaEvent)
	{
	case HINAEVENT_NORMAL:
		MapEvent_Hinamatsuri_Normal();
		break;
	case HINAEVENT_CHERRY_BLOSSOMS:
		MapEvent_Hinamatsuri_CherryBlossoms();
		break;
	case HINAEVENT_DROP_ITEM:
		MapEvent_Hinamatsuri_DropItem();
		break;
	}
}

//=============================================================================
// ゲームのひな祭りの通常のマップイベント処理
//=============================================================================
void CGame::MapEvent_Hinamatsuri_Normal(void)
{
	CDebugProc::Print(1, "通常イベント\n");
}

//=============================================================================
// ゲームのひな祭りの桜が舞い散るマップイベント処理
//=============================================================================
void CGame::MapEvent_Hinamatsuri_CherryBlossoms(void)
{
	CDebugProc::Print(1, "桜舞い散るイベント\n");
	if (m_State != STATE_NORMAL) { return; }

	// 桜の花びら生成
	for (int nCnt = 0; nCnt < m_CherryBlossomsData.nAppear; nCnt++)
	{
		CreateBlossoms();
	}

	// イベントカウンター進行
	m_nEventCounter++;
	if (m_nEventCounter >= m_CherryBlossomsData.nTime)
	{// カウンターが一定値になった
		m_nEventCounter = 0;
		m_HinaEvent = HINAEVENT_NORMAL;
	}
}

//=============================================================================
// ゲームのひな祭りのアイテムを投下するマップイベント処理
//=============================================================================
void CGame::MapEvent_Hinamatsuri_DropItem(void)
{
	CDebugProc::Print(1, "アイテムドロップイベント\n");
}


//*****************************************************************************
//
// ゲーム内スポーン処理用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのプレイヤーをリスポーン位置にずらす処理
//=============================================================================
void CGame::SetPlayerPosToSpawn(void)
{
	// プレイヤーのリスポーン位置を取得
	D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
	CMap *pMap = GetMap();
	if (pMap != NULL)
	{
		PlayerPos = pMap->GetPlayerRespawn(CManager::GetClient()->GetClientId())->GetPos();
	}

	if (m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
	{
		m_pPlayer[CManager::GetClient()->GetClientId()]->SetPos(PlayerPos);
	}
}

//=============================================================================
// ゲームのプレイヤーをリスポーンさせるかチェックする
//=============================================================================
void CGame::CheckPlayerResSpawn(int nCntPlayer)
{
	if (m_pPlayer[nCntPlayer] == NULL && m_nPlayerStock[nCntPlayer] >= 1)
	{
		m_nPlayerRespawnCounter++;
		if (m_nPlayerRespawnCounter % GAME_PLAYER_RESPAWN_TIMING == 0)
		{
			m_nPlayerRespawnCounter = 0;
			m_nPlayerStock[nCntPlayer]--;
			m_pPlayer[nCntPlayer] = CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);

			// プレイヤーのリスポーン位置を取得
			D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
			CMap *pMap = GetMap();
			if (pMap != NULL)
			{
				PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			}

			// プレイヤーの位置をずらす
			m_pPlayer[nCntPlayer]->SetPos(PlayerPos);
		}
	}
}

//=============================================================================
// ゲームの敵の生成するタイミングかどうか判定する
//=============================================================================
void CGame::CheckEnemySpawn(int nTime)
{
	CMap *pMap = GetMap();
	if (pMap == NULL)return;

	int nNumEnemyListData = pMap->GetNumEnemyListData();
	CEnemy_ListData *pEnemyData = NULL;

	// もう敵が出ないのであればこの時点で処理終了
	if (m_nSpawnEnemyCount == nNumEnemyListData) return;

	// カウンター増加
	m_nGameCounter++;
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
// ゲームの敵の生成する
//=============================================================================
void CGame::EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt)
{
	// 敵のリスポーン位置を取得
	D3DXVECTOR3 EnemyPos = pMap->GetEnemyRespawn(pEnemyData->GetRespawnIdx())->GetPos();

	// 敵の生成(ここは後で種類分けしておく)
	CEnemy::Create(EnemyPos, INITIALIZE_D3DXVECTOR3);
}



//*****************************************************************************
//
// マップを変える用関数(デバッグ用の関数込み)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームの各種カウンターを初期化する
//=============================================================================
void CGame::ResetCounter(void)
{
	m_nSpawnEnemyCount = 0;
	m_nGameCounter = 0;
	m_nStateCounter = 0;
	m_nEventCounter = 0;
	m_nEnemyMoveCounter = 0;
}

//=============================================================================
// ゲームの次のマップに切り替える処理
//=============================================================================
void CGame::ChangeNextMap(void)
{
	// 現在のマップを破棄
	ReleaseMap();

	// ゲームに使用しているオブジェクトを破棄
	DeleteGameObject();

	// マップ番号を増やしマップを読み込む
	m_nMapIdx++;
	m_nStageIdx++;
	if (m_nStageIdx >= GAME_MAPSTAGE_MAX)
	{
		m_nMapIdx--;
		m_nStageIdx--;
	}
	CreateMap();

	// マップ番号の桁数を計算しておく
	CalcStageDigits();

	// プレイヤーの位置をスポーン位置を設定
	SetPlayerPosToSpawn();

	// 各種カウンター初期化
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;

	// 状態を設定
	SetState(STATE_STAGE_DISP);
}

//=============================================================================
// ゲームの前のマップに切り替える処理
//=============================================================================
void CGame::ChangePrevMap(void)
{
	// 現在のマップを破棄
	ReleaseMap();

	// ゲームに使用しているオブジェクトを破棄
	DeleteGameObject();

	// マップ番号を減らしマップを読み込む
	m_nMapIdx--;
	m_nStageIdx--;
	if (m_nStageIdx < 0)
	{
		m_nMapIdx++;
		m_nStageIdx++;
	}
	CreateMap();

	// マップ番号の桁数を計算しておく
	CalcStageDigits();

	// プレイヤーの位置をスポーン位置に設定
	SetPlayerPosToSpawn();

	// 各種カウンター初期化
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;

	// 状態を設定
	SetState(STATE_STAGE_DISP);
}

//=============================================================================
// ゲームのゲームに使用しているオブジェクトを破棄する処理
//=============================================================================
void CGame::DeleteGameObject(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (GAME_DELETE_OBJECT)
			{// ゲームに使用しているオブジェクトクラスだった
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// ゲームのステージ番号の桁数を計算する処理
//=============================================================================
void CGame::CalcStageDigits(void)
{
	m_nNumNumberDigits = (int)log10f((float)(m_nStageIdx + 1)) + 1;	// 桁数を求める
	if (m_nNumNumberDigits <= 0) { m_nNumNumberDigits = 1; }	    // 0以下のとき1にする
}



//*****************************************************************************
//
// 生成用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームの読み込むマップファイル名のポインタを生成する
//=============================================================================
void CGame::CreateMapFilePointer(void)
{
	m_pMapFileName = new char*[m_nNumMap];
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			m_pMapFileName[nCnt] = new char[256];
			strcpy(m_pMapFileName[nCnt], "\0");
		}
	}
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
// ゲームのテクスチャ管轄クラス生成処理
//=============================================================================
void CGame::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// ゲームのモデル管轄クラス生成処理
//=============================================================================
void CGame::CreateItemModel(int nNumItem)
{
	CModelCreate *pModelCreate = CModelCreate::Create(nNumItem);
	SetModelCreate(pModelCreate);
}

//=============================================================================
// ゲームのマップ生成処理
//=============================================================================
void CGame::CreateMap(void)
{
	CMap *pMap = CMap::Create(m_pMapFileName[m_nMapIdx]);
	SetMap(pMap);
	m_nNumEnemy = pMap->GetNumEnemyListData();
	ResetCounter();
	CEnemy::ResetSpawnCounter();
	SetPlayerPosToSpawn();
	CheckEnemySpawn(m_nGameCounter);
}

//=============================================================================
// ゲームのUI生成処理
//=============================================================================
void CGame::CreateUI(void)
{
	m_pUI = CUI::Create(GetTextureManager(), 0, m_nMapIdx);
}

//=============================================================================
// ゲームのプレイヤー生成処理
//=============================================================================
void CGame::CreatePlayer(void)
{
	// プレイヤーを生成する
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);
	}
}

//=============================================================================
// ゲームのゲームオーバーロゴ表示用ポリゴンを生成する
//=============================================================================
void CGame::CreateGameOverLogo(void)
{
	m_pGameOverLogo = CScene2D::Create(m_GameOverPolyData.pos, m_GameOverPolyData.col,
		m_GameOverPolyData.fWidth, m_GameOverPolyData.fHeight, GAME_GAMEOVERPOLYGON_PRIORITY);
	if (m_pGameOverLogo != NULL, GetTextureManager() != NULL)
	{
		m_pGameOverLogo->BindTexture(GetTextureManager()->GetTexture(m_GameOverPolyData.nTexIdx));
	}
}

//=============================================================================
// ゲームのステージ背景用ポリゴンを生成する
//=============================================================================
void CGame::CreateStageBg(void)
{
	m_pStageBg = CScene2D::Create(m_StageBgData.pos, m_StageBgData.col, m_StageBgData.fWidth,
		m_StageBgData.fHeight, GAME_STAGEPOLYGON_PRIORITY);
}

//=============================================================================
// ゲームのステージロゴ表示用ポリゴンを生成する
//=============================================================================
void CGame::CreateStageLogo(void)
{
	m_pStageLogo = CScene2D::Create(m_StageLogoData.pos, m_StageLogoData.col,
		m_StageLogoData.fWidth, m_StageLogoData.fHeight, GAME_STAGEPOLYGON_PRIORITY);
	if (m_pStageLogo != NULL && GetTextureManager() != NULL)
	{
		m_pStageLogo->BindTexture(GetTextureManager()->GetTexture(m_StageLogoData.nTexIdx));
	}
}

//=============================================================================
// ゲームのステージ番号表示用ポリゴンを生成する
//=============================================================================
void CGame::CreateStageNumber(void)
{
	int nStageNumber = 0;
	D3DXVECTOR3 NumberPos = m_StageNumberData.pos;
	m_pStageNumber = new CNumber*[m_nNumNumberDigits];
	if (m_pStageNumber != NULL)
	{
		for (int nCntNumber = 0; nCntNumber < m_nNumNumberDigits; nCntNumber++)
		{// 現在のステージ番号の桁数分繰り返し
			m_pStageNumber[nCntNumber] = NULL;
			m_pStageNumber[nCntNumber] = CNumber::Create(NumberPos, m_StageNumberData.col,
				m_StageNumberData.fWidth, m_StageNumberData.fHeight, 0.0f, CNumber::STATE_NONE,
				0, 3, GAME_STAGEPOLYGON_PRIORITY);
			if (m_pStageNumber[nCntNumber] != NULL && GetTextureManager() != NULL)
			{// 生成できた
				nStageNumber = (m_nStageIdx + 1) % ((int)powf(10.0f, (float)nCntNumber) * 10) / (int)powf(10.0f, (float)nCntNumber);
				m_pStageNumber[nCntNumber]->SetNumber(nStageNumber);
				m_pStageNumber[nCntNumber]->BindTexture(GetTextureManager()->GetTexture(m_nNumberTexIdx));

			}
			NumberPos.x -= m_StageNumberData.fWidth + (m_StageNumberData.fWidth * GAME_NUMBER_INTERVAL_CUT);
		}
	}
}

//=============================================================================
// ゲームの桜の花びらを生成する処理
//=============================================================================
void CGame::CreateBlossoms(void)
{
	// 各種値の計算
	// 移動量
	D3DXVECTOR3 Move = INITIALIZE_D3DXVECTOR3;
	Move.x = (float)((rand() % (m_CherryBlossomsData.nMoveXMax - m_CherryBlossomsData.nMoveXMin)) + m_CherryBlossomsData.nMoveXMin);
	Move.x *= -1.0f;
	Move.y = (float)((rand() % (m_CherryBlossomsData.nMoveYMax - m_CherryBlossomsData.nMoveYMin)) + m_CherryBlossomsData.nMoveYMin);

	// 角度の回転スピード
	float fAngleSpeed = (float)(rand() % (m_CherryBlossomsData.nAngleSpeedMax - m_CherryBlossomsData.nAngleSpeedMin) + m_CherryBlossomsData.nAngleSpeedMin);

	// 向きの回転スピード
	float fRotSpeed = (float)(rand() % (m_CherryBlossomsData.nRotSpeedMax - m_CherryBlossomsData.nRotSpeedMin) + m_CherryBlossomsData.nRotSpeedMin);

	// 幅
	float fWidth = (float)((rand() % (m_CherryBlossomsData.nWidthMax - m_CherryBlossomsData.nWidthMin)) + m_CherryBlossomsData.nWidthMin);

	// 高さ
	float fHeight = (float)((rand() % (m_CherryBlossomsData.nHeightMax - m_CherryBlossomsData.nHeightMin)) + m_CherryBlossomsData.nHeightMin);

	// 花びらポリゴン生成
	CBlossoms *pBlossoms = CBlossoms::Create(Move, D3DXToRadian(fAngleSpeed), D3DXToRadian(fRotSpeed), GAME_BLOSSOMS_POS_INI, GAME_BLOSSOMS_COL_INI, fWidth, fHeight);
	if (pBlossoms != NULL && GetTextureManager() != NULL)
	{
		pBlossoms->BindTexture(GetTextureManager()->GetTexture(m_CherryBlossomsData.nTexIdx));
	}
}


//*****************************************************************************
//
// 開放用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームの読み込むマップファイル名のポインタを開放する
//=============================================================================
void CGame::ReleaseMapFilePointer(void)
{
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			delete[] m_pMapFileName[nCnt];
			m_pMapFileName[nCnt] = NULL;
		}

		delete[] m_pMapFileName;
		m_pMapFileName = NULL;
	}
}

//=============================================================================
// ゲームのUIクラスを開放する
//=============================================================================
void CGame::ReleaseUI(void)
{
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = NULL;
	}
}

//=============================================================================
// ゲームのプレイヤークラスを開放する
//=============================================================================
void CGame::ReleasePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// ゲームのゲームオーバーロゴ表示用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseGameOverLogo(void)
{
	if (m_pGameOverLogo != NULL)
	{
		m_pGameOverLogo->Uninit();
		m_pGameOverLogo = NULL;
	}
}

//=============================================================================
// ゲームのステージ背景表示用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseStageBg(void)
{
	if (m_pStageBg != NULL)
	{
		m_pStageBg->Uninit();
		m_pStageBg = NULL;
	}
}

//=============================================================================
// ゲームのステージロゴ表示用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseStageLogo(void)
{
	if (m_pStageLogo != NULL)
	{
		m_pStageLogo->Uninit();
		m_pStageLogo = NULL;
	}
}

//=============================================================================
// ゲームのステージ番号表示用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseStageNumber(void)
{
	if (m_pStageNumber != NULL)
	{
		for (int nCntNumber = 0; nCntNumber < m_nNumNumberDigits; nCntNumber++)
		{
			if (m_pStageNumber[nCntNumber] != NULL)
			{
				m_pStageNumber[nCntNumber]->Uninit();
				m_pStageNumber[nCntNumber] = NULL;
			}
		}
		delete[] m_pStageNumber;
		m_pStageNumber = NULL;
	}
}

//*****************************************************************************
//
// スクリプト読み込み用関数(マップイベント用にスクリプトを作るならここに！！)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのシステム情報を読み込む
//=============================================================================
void CGame::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(GAME_SYSTEM_FILENAME);
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
// ゲームのシステム情報をファイルから読み込む
//=============================================================================
void CGame::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntMap = 0;
	int nCntTex = 0;
	int nCntModel = 0;
	int nCntItemModel = 0;

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// 読み込むテクスチャの数だった
			CreateTexture(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// 読み込むテクスチャのファイル名だった
			LoadTexFileName(pStr, nCntTex);
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
		else if (CFunctionLib::Memcmp(pStr, NUM_MAP) == 0)
		{// 読み込むマップの数だった
			m_nNumMap = CFunctionLib::ReadInt(pStr, NUM_MAP);
			CreateMapFilePointer();
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// 読み込むマップファイル名だった
			LoadMapFileName(pStr, nCntMap);
			nCntMap++;
		}
		else if (CFunctionLib::Memcmp(pStr, ITEMEVENT_FILENAME) == 0)
		{// アイテムイベント用スクリプトファイル名だった
			LoadItemEvent(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, NUMBER_TEX_IDX) == 0)
		{// 数字の使用するテクスチャ番号だった
			m_nNumberTexIdx = CFunctionLib::ReadInt(pStr, NUMBER_TEX_IDX);
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
		else if (CFunctionLib::Memcmp(pStr, STAGEBGSET) == 0)
		{// ステージ背景用ポリゴン情報だった
			LoadStageBg(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, STAGELOGOSET) == 0)
		{// ステージロゴ用ポリゴン情報だった
			LoadStageLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, STAGENUMBERSET) == 0)
		{// ステージ番号表示用ポリゴン情報だった
			LoadStageNumber(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEOVERPOLYGONSET) == 0)
		{// ゲームオーバーロゴポリゴン情報だった
			LoadGameOverLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, HINAMATSURI_FILENAME) == 0)
		{// ひな祭りイベント用スクリプトファイル名だった
			LoadHinamatsuriEvent(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームの読み込むテクスチャファイル名を読み込む
//=============================================================================
void CGame::LoadTexFileName(char *pStr, int nCntTex)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	char aTexFileName[256] = "\0";
	strcpy(aTexFileName, CFunctionLib::ReadString(pStr, aTexFileName, TEXTURE_FILENAME));

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), aTexFileName, &pTexture);

	// テクスチャ管轄クラスにポインタを設定する
	CTextureManager *pTextureManager = GetTextureManager();
	pTextureManager->SetTexture(pTexture, nCntTex);
	pTextureManager->SetFileName(pStr, nCntTex);
}

//=============================================================================
// ゲームの読み込むアイテムモデルを読み込む
//=============================================================================
void CGame::LoadModel(char *pStr, int nCntModel)
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
// ゲームの読み込むマップファイル名を読み込む
//=============================================================================
void CGame::LoadMapFileName(char *pStr, int nCntMap)
{
	strcpy(m_pMapFileName[nCntMap], CFunctionLib::ReadString(pStr, m_pMapFileName[nCntMap], MAP_FILENAME));
}

//=============================================================================
// ゲームの読み込むエフェクトファイル名を読み込む
//=============================================================================
void CGame::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// エフェクト管轄クラスを生成
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
}

//=============================================================================
// ゲームのステージ背景ポリゴン情報を読み込む
//=============================================================================
void CGame::LoadStageBg(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_StageBgData.pos = GAME_STAGEBG_POS_INI;
	m_StageBgData.col = GAME_STAGEBG_COL_INI;
	m_StageBgData.fWidth = GAME_STAGEBG_WIDTH_INI;
	m_StageBgData.fHeight = GAME_STAGEBG_HEIGHT_INI;

	// データ読み込み
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_StageBgData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_StageBgData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_StageBgData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_StageBgData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGEBGSET) == 0)
		{// ステージ背景ポリゴン情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームのステージロゴポリゴン情報を読み込む
//=============================================================================
void CGame::LoadStageLogo(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_StageLogoData.nTexIdx = 0;
	m_StageLogoData.pos = GAME_STAGELOGO_POS_INI;
	m_StageLogoData.col = GAME_STAGELOGO_COL_INI;
	m_StageLogoData.fWidth = GAME_STAGELOGO_WIDTH_INI;
	m_StageLogoData.fHeight = GAME_STAGELOGO_HEIGHT_INI;

	// データ読み込み
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_StageLogoData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_StageLogoData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_StageLogoData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_StageLogoData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_StageLogoData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGELOGOSET) == 0)
		{// ステージロゴポリゴン情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームのステージ番号表示用ポリゴン情報を読み込む
//=============================================================================
void CGame::LoadStageNumber(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_StageNumberData.pos = GAME_STAGENUMBER_POS_INI;
	m_StageNumberData.col = GAME_STAGENUMBER_COL_INI;
	m_StageNumberData.fWidth = GAME_STAGENUMBER_WIDTH_INI;
	m_StageNumberData.fHeight = GAME_STAGENUMBER_HEIGHT_INI;

	// データ読み込み
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_StageNumberData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_StageNumberData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_StageNumberData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_StageNumberData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGENUMBERSET) == 0)
		{// ステージ番号表示用ポリゴン情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームのゲームオーバーロゴポリゴン情報を読み込む
//=============================================================================
void CGame::LoadGameOverLogo(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_GameOverPolyData.nTexIdx = 0;
	m_GameOverPolyData.pos = GAME_GAMEOVERLOGO_POS_INI;
	m_GameOverPolyData.col = GAME_GAMEOVERLOGO_COL_INI;
	m_GameOverPolyData.fWidth = GAME_GAMEOVERLOGO_WIDTH_INI;
	m_GameOverPolyData.fHeight = GAME_GAMEOVERLOGO_HEIGHT_INI;

	// データ読み込み
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_GameOverPolyData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_GameOverPolyData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_GameOverPolyData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_GameOverPolyData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_GameOverPolyData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_GAMEOVERPOLYGONSET) == 0)
		{// ゲームオーバーロゴポリゴン情報終了の合図だった
			break;
		}
	}
}


//*****************************************************************************
//
// アイテムイベント用スクリプト読み込み処理
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのアイテムイベントスクリプトを読み込む
//=============================================================================
void CGame::LoadItemEvent(char *pStr)
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
// ゲームのアイテムイベント情報をファイルから読み込む
//=============================================================================
void CGame::LoadItemEventScript(CFileLoader *pFileLoader, char *pStr)
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
// ひな祭りイベント用スクリプト読み込み処理
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのひな祭りイベントスクリプトを読み込む
//=============================================================================
void CGame::LoadHinamatsuriEvent(char *pStr)
{
	char aFileName[256] = "\0";
	strcpy(aFileName, CFunctionLib::ReadString(pStr, aFileName, HINAMATSURI_FILENAME));
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(aFileName);
	if (pFileLoader != NULL)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// 読み込み開始の合図だった
			LoadHinamatsuriEventScript(pFileLoader, pStr);
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
// ゲームのひな祭りイベント情報をファイルから読み込む
//=============================================================================
void CGame::LoadHinamatsuriEventScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, CHERRYBLOSSOMSSET) == 0)
		{// 桜の花びら情報だった
			LoadCherryBlossomsData(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームの桜の花びらデータ情報を読み込む
//=============================================================================
void CGame::LoadCherryBlossomsData(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_CherryBlossomsData.nTime = GAME_BLOSSOMS_TIME_INI;
	m_CherryBlossomsData.nAppear = GAME_BLOSSOMS_APPEAR_INI;
	m_CherryBlossomsData.nTexIdx = 0;
	m_CherryBlossomsData.nMoveXMax = GAME_BLOSSOMS_MOVEX_MAX_INI;
	m_CherryBlossomsData.nMoveXMin = GAME_BLOSSOMS_MOVEX_MIN_INI;
	m_CherryBlossomsData.nMoveYMax = GAME_BLOSSOMS_MOVEY_MAX_INI;
	m_CherryBlossomsData.nMoveYMin = GAME_BLOSSOMS_MOVEY_MIN_INI;
	m_CherryBlossomsData.nWidthMax = GAME_BLOSSOMS_WIDTH_MAX_INI;
	m_CherryBlossomsData.nWidthMin = GAME_BLOSSOMS_WIDTH_MIN_INI;
	m_CherryBlossomsData.nHeightMax = GAME_BLOSSOMS_HEIGHT_MAX_INI;
	m_CherryBlossomsData.nHeightMin = GAME_BLOSSOMS_HEIGHT_MIN_INI;
	m_CherryBlossomsData.nAngleSpeedMax = GAME_BLOSSOMS_ANGLESPEED_MAX_INI;
	m_CherryBlossomsData.nAngleSpeedMin = GAME_BLOSSOMS_ANGLESPEED_MIN_INI;
	m_CherryBlossomsData.nRotSpeedMax = GAME_BLOSSOMS_ROTSPEED_MAX_INI;
	m_CherryBlossomsData.nRotSpeedMin = GAME_BLOSSOMS_ROTSPEED_MIN_INI;

	// データ読み込み
	int nWord = 0;
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// 桜の花びらが降り注ぐ時間
			m_CherryBlossomsData.nTime = CFunctionLib::ReadInt(pStr, TIME);
		}
		if (CFunctionLib::Memcmp(pStr, APPEAR) == 0)
		{// 桜の花びらを1フレーム毎にどれくらい出すか
			m_CherryBlossomsData.nAppear = CFunctionLib::ReadInt(pStr, APPEAR);
		}
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_CherryBlossomsData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, MOVEX) == 0)
		{// X方向の移動量だった
			m_CherryBlossomsData.nMoveXMax = CFunctionLib::ReadInt(pStr, MOVEX);
			pStr = CFunctionLib::HeadPutout(pStr, MOVEX);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nMoveXMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, MOVEY) == 0)
		{// Y方向の移動量だった
			m_CherryBlossomsData.nMoveYMax = CFunctionLib::ReadInt(pStr, MOVEY);
			pStr = CFunctionLib::HeadPutout(pStr, MOVEY);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nMoveYMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_CherryBlossomsData.nWidthMax = CFunctionLib::ReadInt(pStr, WIDTH);
			pStr = CFunctionLib::HeadPutout(pStr, WIDTH);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nWidthMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_CherryBlossomsData.nHeightMax = CFunctionLib::ReadInt(pStr, HEIGHT);
			pStr = CFunctionLib::HeadPutout(pStr, HEIGHT);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nHeightMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, ANGLESPEED) == 0)
		{// 角度を回転させるスピードだった
			m_CherryBlossomsData.nRotSpeedMax = CFunctionLib::ReadInt(pStr, ANGLESPEED);
			pStr = CFunctionLib::HeadPutout(pStr, ANGLESPEED);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nRotSpeedMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, ROTSPEED) == 0)
		{// 向きを回転させるスピードだった
			m_CherryBlossomsData.nRotSpeedMax = CFunctionLib::ReadInt(pStr, ROTSPEED);
			pStr = CFunctionLib::HeadPutout(pStr, ROTSPEED);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nRotSpeedMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, END_CHERRYBLOSSOMSSET) == 0)
		{// 桜の花びらデータ終了の合図だった
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
// ゲームの変数を初期化する
//=============================================================================
void CGame::ClearVariable(void)
{
	m_State = STATE_STAGE_DISP;
	m_pUI = NULL;
	m_pMapFileName = NULL;
	m_nStateCounter = 0;
	m_nMapIdx = 0;
	m_nStageIdx = 0;
	m_nNumMap = 0;
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;
	m_nNumEnemy = 0;
	m_pGameOverLogo = NULL;
	m_pStageBg = NULL;
	m_pStageLogo = NULL;
	m_pStageNumber = NULL;
	m_nNumberTexIdx = 0;
	m_nNumNumberDigits = NULL;
	m_nPlayerRespawnCounter = 0;
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_bDeletePlayerFlag = 0;
	m_bEnemyMove = true;
	m_nEnemyMoveCounter = 0;
	m_HinaEvent = HINAEVENT_CHERRY_BLOSSOMS;
	m_nEventCounter = 0;
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;
		m_nPlayerStock[nCntPlayer] = 0;
	}
}

//=============================================================================
// ゲームの弾モデルを設定する処理
//=============================================================================
void CGame::SetBulletModel(CBullet *pBullet)
{
	pBullet->BindModel(GetModelCreate()->GetMesh(m_nBulletModelIdx), GetModelCreate()->GetBuffMat(m_nBulletModelIdx),
		GetModelCreate()->GetNumMat(m_nBulletModelIdx), GetModelCreate()->GetTexture(m_nBulletModelIdx));
}

//=============================================================================
// ゲームのアイテムモデルを設定する処理
//=============================================================================
void CGame::SetItemModel(CItem *pItem, int nType)
{
	pItem->BindModel(GetModelCreate()->GetMesh(m_nItemModelIdx[nType]), GetModelCreate()->GetBuffMat(m_nItemModelIdx[nType]),
		GetModelCreate()->GetNumMat(m_nItemModelIdx[nType]), GetModelCreate()->GetTexture(m_nItemModelIdx[nType]));
}

//=============================================================================
// ゲームの状態を設定する
//=============================================================================
void CGame::SetState(const STATE state)
{
	m_StateOld = m_State;
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// ゲームのプレイヤークラスへのポインタを設定する
//=============================================================================
void CGame::SetPlayer(CPlayer *pPlayer, const int nIdx)
{
	m_pPlayer[nIdx] = pPlayer;
}

//=============================================================================
// ゲームの出現する敵の数を設定する
//=============================================================================
void CGame::SetNumEnemy(const int nNumEnemy)
{
	m_nNumEnemy = nNumEnemy;
}

//=============================================================================
// ゲームの状態を取得する
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
// ゲームのプレイヤークラスへのポインタを取得する
//=============================================================================
CPlayer *CGame::GetPlayer(const int nIdx)
{
	return m_pPlayer[nIdx];
}

//=============================================================================
// ゲームの出現する敵の数を取得する
//=============================================================================
int CGame::GetNumEnemy(void)
{
	return m_nNumEnemy;
}

//=============================================================================
// ゲームの敵を動かせるかどうか取得する
//=============================================================================
bool CGame::GetEnemyMove(void)
{
	return m_bEnemyMove;
}