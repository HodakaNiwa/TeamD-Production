//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "map.h"
#include "respawn.h"
#include "server.h"
#include "player.h"
#include "playerManager.h"
#include "charaselect.h"
#include "characterManager.h"
#include "effectManager.h"
#include "bullet.h"
#include "modelcreate.h"
#include "title.h"
#include "sound.h"
#include "debugproc.h"

#include "goalCylinder.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TUTORIAL_SYSTEM_FILENAME        "data/TEXT/MODE/tutorial.ini"  // 初期化に使用するシステムファイル名
#define TUTORIAL_PLAYER_RESPAWN_TIMING  (180)                          // プレイヤーがリスポーンするまでの時間
#define TUTORIAL_MAPSTAGE_MAX           (3)                            // チュートリアルで使用するマップの最大数
#define TUTORIAL_END_TIME               (180)                          // 終了状態になってから状態が変わるまでの時間
#define TUTORIAL_BGM_IDX                (2)                            // チュートリアルで再生するBGMの番号
#define TUTORIAL_SE_SELECT_IDX          (8)                            // 項目を選択している時のSEの音番号
#define TUTORIAL_SE_DECIDE_IDX          (9)                            // 決定ボタンを押された時のSEの音番号

// 終了状態時のカメラ用マクロ
#define TUTORIAL_END_CAMERA_POSY        (130.0f)                       // カメラの高さ
#define TUTORIAL_END_CAMERA_LENGTH      (240.0f)                       // 視点注視点間の距離
#define TUTORIAL_END_CAMERA_MOVING      (0.09f)                        // 目的の位置まで移動する倍率

// マップ更新時に消されるオブジェクト(判定用のマクロなのでここに追加)
#define TUTORIAL_DELETE_OBJECT (pScene->GetObjType() == CScene::OBJTYPE_ITEM || pScene->GetObjType() == CScene::OBJTYPE_BULLET || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER || pScene->GetObjType() == CScene::OBJTYPE_EMITTER || pScene->GetObjType() == CScene::OBJTYPE_BLOSSOMS)

// スキップロゴ初期化用
#define TUTORIAL_SKIPLOGO_POS_INI              (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f, 0.0f))
#define TUTORIAL_SKIPLOGO_COL_INI              (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_SKIPLOGO_WIDTH_INI            (300.0f)
#define TUTORIAL_SKIPLOGO_HEIGHT_INI           (90.0f)

// スキップチェックロゴ初期化用
// １つ目
#define TUTORIAL_SKIPCHECKLOGO_0_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 6.0f, SCREEN_HEIGHT / 3.0f * 2.0f, 0.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_COL_NONE      (D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE    (180.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE   (60.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_COL_SELECT    (D3DXCOLOR(0.9f,0.9f,0.9f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_WIDTH_SELECT  (200.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_SELECT (80.0f)

// ２つ目
#define TUTORIAL_SKIPCHECKLOGO_1_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 6.0f * 4.0f, SCREEN_HEIGHT / 3.0f * 2.0f, 0.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_COL_NONE      (D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE    (180.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE   (60.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_COL_SELECT    (D3DXCOLOR(0.9f,0.9f,0.9f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_WIDTH_SELECT  (200.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_SELECT (80.0f)

// 操作方法表示用ポリゴン初期化用
#define TUTORIAL_OPEINFO_POS_INI               (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 78.0f, 0.0f))
#define TUTORIAL_OPEINFO_COL_INI               (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_OPEINFO_WIDTH_INI             (220.0f)
#define TUTORIAL_OPEINFO_HEIGHT_INI            (80.0f)
#define TUTORIAL_OPEINFO_TEXIDX                (3)
#define TUTORIAL_OPEINFO_PRIORITY              (7)

// ブロックを壊したレスポンス表示用ポリゴン初期化用
#define TUTORIAL_BREAKBLOCKINFO_POS_INI        (D3DXVECTOR3(145.0f, 210.0f, 0.0f))
#define TUTORIAL_BREAKBLOCKINFO_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_BREAKBLOCKINFO_WIDTH_INI      (145.0f)
#define TUTORIAL_BREAKBLOCKINFO_HEIGHT_INI     (80.0f)
#define TUTORIAL_BREAKBLOCKINFO_TEXIDX         (4)
#define TUTORIAL_BREAKBLOCKINFO_PRIORITY       (7)
#define TUTORIAL_BREAKBLOCKINFO_DISP           (180)
#define TUTORIAL_BREAKBLOCKINFO_ALPHACUT       (0.02f)

// プレイヤーに弾が当たった時のレスポンス表示用ポリゴン初期化用
#define TUTORIAL_ATTACKPLAYERINFO_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH - 145.0f, 390.0f, 0.0f))
#define TUTORIAL_ATTACKPLAYERINFO_COL_INI      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_ATTACKPLAYERINFO_WIDTH_INI    (145.0f)
#define TUTORIAL_ATTACKPLAYERINFO_HEIGHT_INI   (80.0f)
#define TUTORIAL_ATTACKPLAYERINFO_TEXIDX       (5)
#define TUTORIAL_ATTACKPLAYERINFO_PRIORITY     (7)
#define TUTORIAL_ATTACKPLAYERINFO_DISP         (180)
#define TUTORIAL_ATTACKPLAYERINFO_ALPHACUT     (0.02f)

// ゴールが近いレスポンス表示用ポリゴン初期化用
#define TUTORIAL_NEARGOALINFO_POS_INI          (D3DXVECTOR3(145.0f, 560.0f, 0.0f))
#define TUTORIAL_NEARGOALINFO_COL_INI          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_NEARGOALINFO_WIDTH_INI        (145.0f)
#define TUTORIAL_NEARGOALINFO_HEIGHT_INI       (80.0f)
#define TUTORIAL_NEARGOALINFO_TEXIDX           (6)
#define TUTORIAL_NEARGOALINFO_PRIORITY         (7)
#define TUTORIAL_NEARGOALINFO_DISP             (180)
#define TUTORIAL_NEARGOALINFO_ALPHACUT         (0.02f)

// 値読み込み用のパス
// テクスチャ用
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// モデル用
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// エフェクト用
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// プレイヤーデータ用
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "

// 弾用
#define BULLET_MODEL_IDX "BULLET_MODEL_IDX = "

// マップ用
#define NUM_MAP "NUM_MAP = "

// スキップロゴ用
#define SKIPLOGOSET "SKIPLOGOSET"
#define END_SKIPLOGOSET "END_SKIPLOGOSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// スキップチェックロゴ用
#define SKIPCHECKLOGOSET "SKIPCHECKLOGOSET"
#define END_SKIPCHECKLOGOSET "END_SKIPCHECKLOGOSET"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// チュートリアルのコンストラクタ
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// チュートリアルのデストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// チュートリアルの生成処理
//=============================================================================
CTutorial *CTutorial::Create()
{
	CTutorial *pTutorial = NULL;
	if (pTutorial == NULL)
	{
		pTutorial = new CTutorial;
		if (pTutorial != NULL)
		{
			pTutorial->Init();
		}
	}

	return pTutorial;
}

//=============================================================================
// チュートリアルの初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// 変数を初期化する
	ClearVariable();

	// システムの初期化
	LoadSystem();

	// カメラの生成
	CreateCamera();

	return S_OK;
}

//=============================================================================
// チュートリアルの終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// 共通の終了処理
	CBasemode::Uninit();

	// 各種クラスの開放
	ReleaseSkipLogo();
	ReleaseSkipCheckLogo();
	ReleaseMapFilePointer();
	ReleasePlayer();
	ReleasePlayerManager();
	ReleaseOpeInfo();
	ReleaseBreakBlockInfo();
	ReleaseAttackPlayerInfo();
	ReleaseNearGoalInfo();

	// 全てのオブジェクト開放
	CScene::ReleaseAll();

	// BGMを停止
	CManager::GetSound()->StopSound(TUTORIAL_BGM_IDX);
}

//=============================================================================
// チュートリアルの更新処理
//=============================================================================
void CTutorial::Update(void)
{
	CDebugProc::Print(1, "チュートリアル画面\n");

	// ローカル1Pプレイならばもう一人のプレイヤーを消しておく
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		if (m_pPlayer[MAX_NUM_PLAYER - 1] != NULL)
		{
			m_pPlayer[MAX_NUM_PLAYER - 1]->Uninit();
			m_pPlayer[MAX_NUM_PLAYER - 1] = NULL;
		}
	}

	// レスポンスポリゴンの確認
	CheckBreakBlockInfo();
	CheckAttackPlayerInfo();
	CheckNearGoalInfo();

	// 全ての更新処理
	CScene::UpdateAll();

	// 状態によって処理わけ
	switch (m_State)
	{
	case STATE_SKIP_CHECK:
		SkipCheckUpdate();
		break;
	case STATE_SKIP_END:
		SkipEndUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	case STATE_GOAL:
		GoalUpdate();
		break;
	case STATE_MAP_CHANGE:
		MapChangeUpdate();
		break;
	case STATE_END:
		EndUpdate();
		break;
	}


	// プレイヤーをリスポーンさせるかチェック
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P && m_State != STATE_SKIP_CHECK)
	{// ローカル1Pプレイならば
		CheckPlayerResSpawn(0);
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P && m_State != STATE_SKIP_CHECK)
	{// ローカル2Pプレイならば
		for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			CheckPlayerResSpawn(nCntPlayer);
		}
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P && m_State != STATE_SKIP_CHECK)
	{// オンライン2Pプレイならば
		CheckPlayerResSpawn(CManager::GetClient()->GetClientId());
	}

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		// サーバーへ送るデータを設定する
		SetDataToServer();

		// クライアントの更新処理
		CManager::ClientUpdate();

		// サーバーから送られたデータを設定する
		GetDataFromServer();
	}

	// データクリア
	m_bDeletePlayerFlag = false;
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
}

//=============================================================================
// チュートリアルの描画処理
//=============================================================================
void CTutorial::Draw(void)
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
void CTutorial::DeleteBlock(const int nIdx)
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
void CTutorial::DeleteEnemy(const int nIdx)
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
void CTutorial::DeletePlayer(CPlayer *pPlayer, const int nIdx)
{
	SetPlayer(pPlayer, nIdx);

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
void CTutorial::SetDataToServer(void)
{
	if (CManager::GetClient() == NULL || CManager::GetClient()->GetConnected() == false)return;

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
void CTutorial::SetDataToServerFromPlayer(void)
{
	// プレイヤーの人数を設定
	if (m_pPlayer[CManager::GetClient()->GetClientId()] == NULL)
	{
		CManager::GetClient()->Print("0");
		CManager::GetClient()->Print(" ");
		if (CManager::GetClient()->GetClientId() == 0)
		{
			CManager::GetClient()->Print("%d", (int)m_bDeletePlayerFlag);
			CManager::GetClient()->Print(" ");
		}
		return;
	}
	else if (m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
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

		// プレイヤーの状態を設定
		int nPlayerState = m_pPlayer[CManager::GetClient()->GetClientId()]->GetState();
		CManager::GetClient()->Print("%d", nPlayerState);
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
void CTutorial::SetDataToServerFromGame(void)
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
void CTutorial::SetDataToServerFromEnemy(void)
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
char *CTutorial::GetDataToEnemy(CEnemy *pEnemy, char *pStr)
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
	int nEnemyType = pEnemy->GetType();
	sprintf(aData, "%d", nEnemyType);
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
void CTutorial::SetDataToServerFromPlayerBullet(void)
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
char *CTutorial::GetDataToPlayerBullet(CBullet *pBullet, char *pStr)
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
void CTutorial::SetDataToServerFromEnemyBullet(void)
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
char *CTutorial::GetDataToEnemyBullet(CBullet *pBullet, char *pStr)
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
void CTutorial::SetDataToServerFromDeleteBlock(void)
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
void CTutorial::SetDataToServerFromDeleteEnemy(void)
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
void CTutorial::GetDataFromServer(void)
{
	// 送られたメッセージを取得
	if (CManager::GetClient() == NULL)return;
	char *pStr = CManager::GetClient()->GetReceiveMessage();
	if (pStr == NULL || CManager::GetClient()->GetConnected() == false) return;
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
char *CTutorial::SetDataToPlayerFromServer(char *pStr)
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
		}
	}
	else if (nNumPlayer == 1)
	{// プレイヤーが存在する
		CMap *pMap = GetMap();
		if (pMap == NULL)
		{
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

			// プレイヤーの状態を読み取る
			int nGetPlayerState = CFunctionLib::ReadInt(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
		}

		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] == NULL && pMap != NULL)
		{// メモリが確保されていない
			int nNumber = (CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER;
			m_pPlayer[nNumber] = m_pPlayerManager[nNumber]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nNumber);
		}
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL)
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

			// プレイヤーの状態を読み取る
			int nGetPlayerState = CFunctionLib::ReadInt(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// プレイヤーに値を設定
			if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL)
			{
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetPos(D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ));
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetRot(D3DXVECTOR3(fGetPlayerRotX, fGetPlayerRotY, fGetPlayerRotZ));
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetState((CPlayer::STATE)nGetPlayerState);
			}
		}
	}


	// ホストじゃなかったら
	if (CManager::GetClient()->GetClientId() != 0)
	{
		int nDeletePlayerFlag = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		if (nDeletePlayerFlag == 1 && m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
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
char *CTutorial::SetDataToGameFromServer(char *pStr)
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

	// マップの番号を設定
	m_nMapIdx = nMapIdx;

	// 状態によって処理わけ
	if (m_State == STATE_NORMAL && m_StateOld == STATE_SKIP_CHECK)
	{// マップ更新状態だったら
		ReleaseSkipLogo();
		ReleaseSkipCheckLogo();
		CScene::DeathCheck();
		CreateMap();
		CreatePlayer();
		CreateOpeInfo();
		SetPlayerPosToSpawn();
	}

	return pStr;
}

//=============================================================================
// ゲームのサーバーから送られたデータを敵に設定する処理
//=============================================================================
char *CTutorial::SetDataToEnemyFromServer(char *pStr)
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
char *CTutorial::SetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyType = 0;
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

	// 敵の種類番号
	nEnemyType = CFunctionLib::ReadInt(pStr, "");
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
char *CTutorial::SetDataToCreateEnemy(char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyType = 0;
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

	// 敵の種類番号
	nEnemyType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の番号
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵を生成する(同時に必要なデータを設定)
	CEnemy_ListData *pEnemyData = GetMap()->GetEnemyListData(nEnemyIdx);
	D3DXVECTOR3 pos = D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ);
	D3DXVECTOR3 rot = D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ);
	CEnemy *pEnemy = m_pEnemyManager[nEnemyType]->SetEnemy(pos, rot, nEnemyType);

	if (pEnemy == NULL)return NULL;
	// 敵のデータを設定する
	if (pEnemyData->GetItem() == true)
	{
		pEnemy->SetItemData(pEnemyData->GetItemType());
	}
	pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());
	pEnemy->SetIdx(nEnemyIdx);
	return pStr;
}

//=============================================================================
// プレイヤーの弾の数を合わせる処理
//=============================================================================
void CTutorial::ReleaseEnemy(int nNumEnemy)
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
void CTutorial::ReleaseCheckEnemy(CEnemy *pEnemy, int *pNumEnemy)
{
	pEnemy->Uninit();
	pEnemy = NULL;
	*pNumEnemy = *pNumEnemy - 1;
}

//=============================================================================
// ゲームのサーバーから送られたデータをプレイヤーの弾に設定する処理
//=============================================================================
char *CTutorial::SetDataToPlayerBulletFromServer(char *pStr)
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
void CTutorial::ReleasePlayerBullet(int nNumBullet)
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
void CTutorial::ReleaseCheckPlayerBullet(CBullet *pBullet, int *pNumBullet)
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
char *CTutorial::SetDataToPlayerBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
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
char *CTutorial::SetDataToCreatePlayerBullet(char *pStr)
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
char *CTutorial::SetDataToEnemyBulletFromServer(char *pStr)
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
void CTutorial::ReleaseEnemyBullet(int nNumBullet)
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
void CTutorial::ReleaseCheckEnemyBullet(CBullet *pBullet, int *pNumBullet)
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
char *CTutorial::SetDataToEnemyBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
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
char *CTutorial::SetDataToCreateEnemyBullet(char *pStr)
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
char *CTutorial::SetDataToDeleteBlock(char *pStr)
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
void CTutorial::ReleaseCheckBlock(CBlock *pBlock, int *pDeleteIdx, int *nNumDeleteBlock)
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
char *CTutorial::SetDataToDeleteEnemy(char *pStr)
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
void CTutorial::ReleaseCheckDeleteEnemy(CEnemy *pEnemy, int *pDeleteIdx, int *nNumDeleteEnemy)
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
// 生成用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// チュートリアルのテクスチャ管轄クラス生成処理
//=============================================================================
void CTutorial::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// チュートリアルのカメラ生成処理
//=============================================================================
void CTutorial::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager == NULL) return;

	// カメラを生成
	CCamera *pCamera = CCamera::Create();
	if (pCamera != NULL)
	{
		pCamera->SetPosV(pCamera->GetPosV() + D3DXVECTOR3(-175.0f, 0.0f, 0.0f));
		pCamera->SetPosR(pCamera->GetPosR() + D3DXVECTOR3(-175.0f, 0.0f, 0.0f));
		pCameraManager->SetCamera(pCamera);
	}
	SetCameraManager(pCameraManager);
}

//=============================================================================
// チュートリアルのマップ生成処理
//=============================================================================
void CTutorial::CreateMap(void)
{
	CMap *pMap = CMap::Create(m_pMapFileName[m_nMapIdx]);
	SetMap(pMap);
	ResetCounter();
	CEnemy::ResetSpawnCounter();
	SetPlayerPosToSpawn();
	CheckEnemySpawn(m_nTutorialCounter);
}

//=============================================================================
// チュートリアルの読み込むマップファイル名のポインタを生成する
//=============================================================================
void CTutorial::CreateMapFilePointer(void)
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
// チュートリアルのプレイヤー生成処理
//=============================================================================
void CTutorial::CreatePlayer(void)
{
	// プレイヤーを生成する
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = m_pPlayerManager[nCntPlayer]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);
	}
}

//=============================================================================
// チュートリアルの敵生成処理
//=============================================================================
CEnemy *CTutorial::CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData)
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
// チュートリアルの操作方法表示用ポリゴン生成処理
//=============================================================================
void CTutorial::CreateOpeInfo(void)
{
	if (m_pOpeInfo != NULL) { return; }

	m_pOpeInfo = CScene2D::Create(TUTORIAL_OPEINFO_POS_INI, TUTORIAL_OPEINFO_COL_INI, TUTORIAL_OPEINFO_WIDTH_INI,
		TUTORIAL_OPEINFO_HEIGHT_INI, TUTORIAL_OPEINFO_PRIORITY);
	if (m_pOpeInfo != NULL)
	{
		m_pOpeInfo->BindTexture(GetTextureManager()->GetTexture(TUTORIAL_OPEINFO_TEXIDX));
	}
}

//=============================================================================
// チュートリアルのブロックを壊したレスポンス表示用ポリゴン生成処理
//=============================================================================
void CTutorial::CreateBreakBlockInfo(void)
{
	if (m_pBreakBlockInfo == NULL)
	{// 生成されていない
		m_pBreakBlockInfo = CScene2D::Create(TUTORIAL_BREAKBLOCKINFO_POS_INI, TUTORIAL_BREAKBLOCKINFO_COL_INI,
			TUTORIAL_BREAKBLOCKINFO_WIDTH_INI, TUTORIAL_BREAKBLOCKINFO_HEIGHT_INI, TUTORIAL_BREAKBLOCKINFO_PRIORITY);
		if (m_pBreakBlockInfo != NULL)
		{
			m_pBreakBlockInfo->BindTexture(GetTextureManager()->GetTexture(TUTORIAL_BREAKBLOCKINFO_TEXIDX));
		}
	}
	else
	{// 既に生成されている
		D3DXCOLOR col = m_pBreakBlockInfo->GetCol();
		col.a = 1.0f;
		m_pBreakBlockInfo->SetCol(col);
		m_pBreakBlockInfo->SetVtxBuffCol();
	}

	// 表示カウンターリセット
	m_nCntBreakBlockInfoDisp = 0;
}

//=============================================================================
// チュートリアルのプレイヤーを攻撃したレスポンス表示用ポリゴン生成処理
//=============================================================================
void CTutorial::CreateAttackPlayerInfo(void)
{
	if (m_pAttackPlayerInfo == NULL)
	{// 生成されていない
		m_pAttackPlayerInfo = CScene2D::Create(TUTORIAL_ATTACKPLAYERINFO_POS_INI, TUTORIAL_ATTACKPLAYERINFO_COL_INI,
			TUTORIAL_ATTACKPLAYERINFO_WIDTH_INI, TUTORIAL_ATTACKPLAYERINFO_HEIGHT_INI,
			TUTORIAL_ATTACKPLAYERINFO_PRIORITY);
		if (m_pAttackPlayerInfo != NULL)
		{
			m_pAttackPlayerInfo->BindTexture(GetTextureManager()->GetTexture(TUTORIAL_ATTACKPLAYERINFO_TEXIDX));
		}
	}
	else
	{// 既に生成されている
		D3DXCOLOR col = m_pAttackPlayerInfo->GetCol();
		col.a = 1.0f;
		m_pAttackPlayerInfo->SetCol(col);
		m_pAttackPlayerInfo->SetVtxBuffCol();
	}

	// 表示カウンターリセット
	m_nCntAttackInfoDisp = 0;
}

//=============================================================================
// チュートリアルのゴールが近いレスポンス表示用ポリゴン生成処理
//=============================================================================
void CTutorial::CreateNearGoalInfo(void)
{
	if (m_pNearGoalInfo == NULL)
	{// 生成されていない
		m_pNearGoalInfo = CScene2D::Create(TUTORIAL_NEARGOALINFO_POS_INI, TUTORIAL_NEARGOALINFO_COL_INI,
			TUTORIAL_NEARGOALINFO_WIDTH_INI, TUTORIAL_NEARGOALINFO_HEIGHT_INI, TUTORIAL_NEARGOALINFO_PRIORITY);
		if (m_pNearGoalInfo != NULL)
		{
			m_pNearGoalInfo->BindTexture(GetTextureManager()->GetTexture(TUTORIAL_NEARGOALINFO_TEXIDX));
		}
	}
	else
	{// 既に生成されている
		D3DXCOLOR col = m_pNearGoalInfo->GetCol();
		col.a = 1.0f;
		m_pNearGoalInfo->SetCol(col);
		m_pNearGoalInfo->SetVtxBuffCol();
	}

	// 表示カウンターリセット
	m_nCntNearGoalInfoDisp = 0;
}

//=============================================================================
// チュートリアルのゴールが近いレスポンス表示用ポリゴンチェック処理
//=============================================================================
void CTutorial::CheckBreakBlockInfo(void)
{
	// 生成されていなかったら処理しない
	if (m_pBreakBlockInfo == NULL) { return; }

	// カウンター増加
	m_nCntBreakBlockInfoDisp++;

	if (m_nCntBreakBlockInfoDisp >= TUTORIAL_ATTACKPLAYERINFO_DISP)
	{// 一定時間表示され続けた
		// 透明度を上げる
		D3DXCOLOR col = m_pBreakBlockInfo->GetCol();
		col.a -= TUTORIAL_ATTACKPLAYERINFO_ALPHACUT;
		m_pBreakBlockInfo->SetCol(col);
		m_pBreakBlockInfo->SetVtxBuffCol();

		if (col.a <= 0.0f)
		{// 透明になり切った
			ReleaseBreakBlockInfo();
		}
	}
}

//=============================================================================
// チュートリアルのゴールが近いレスポンス表示用ポリゴンチェック処理
//=============================================================================
void CTutorial::CheckAttackPlayerInfo(void)
{
	// 生成されていなかったら処理しない
	if (m_pAttackPlayerInfo == NULL) { return; }

	// カウンター増加
	m_nCntAttackInfoDisp++;

	if (m_nCntAttackInfoDisp >= TUTORIAL_ATTACKPLAYERINFO_DISP)
	{// 一定時間表示され続けた
	 // 透明度を上げる
		D3DXCOLOR col = m_pAttackPlayerInfo->GetCol();
		col.a -= TUTORIAL_ATTACKPLAYERINFO_ALPHACUT;
		m_pAttackPlayerInfo->SetCol(col);
		m_pAttackPlayerInfo->SetVtxBuffCol();

		if (col.a <= 0.0f)
		{// 透明になり切った
			ReleaseAttackPlayerInfo();
		}
	}
}

//=============================================================================
// チュートリアルのゴールが近いレスポンス表示用ポリゴンチェック処理
//=============================================================================
void CTutorial::CheckNearGoalInfo(void)
{
	// 生成されていなかったら処理しない
	if (m_pNearGoalInfo == NULL) { return; }

	// カウンター増加
	m_nCntNearGoalInfoDisp++;

	if (m_nCntNearGoalInfoDisp >= TUTORIAL_NEARGOALINFO_DISP)
	{// 一定時間表示され続けた
	 // 透明度を上げる
		D3DXCOLOR col = m_pNearGoalInfo->GetCol();
		col.a -= TUTORIAL_NEARGOALINFO_ALPHACUT;
		m_pNearGoalInfo->SetCol(col);
		m_pNearGoalInfo->SetVtxBuffCol();

		if (col.a <= 0.0f)
		{// 透明になり切った
			ReleaseNearGoalInfo();
		}
	}
}


//*****************************************************************************
//
// 開放用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// チュートリアルの読み込むマップファイル名のポインタを開放する
//=============================================================================
void CTutorial::ReleaseMapFilePointer(void)
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
// チュートリアルのスキップロゴポリゴンを開放する
//=============================================================================
void CTutorial::ReleaseSkipLogo(void)
{
	if (m_pSkipLogo != NULL)
	{
		m_pSkipLogo->Uninit();
		m_pSkipLogo = NULL;
	}
}

//=============================================================================
// チュートリアルのスキップ確認ロゴ用ポリゴンを開放する
//=============================================================================
void CTutorial::ReleaseSkipCheckLogo(void)
{
	for (int nCntSkipCheck = 0; nCntSkipCheck < SKIP_MAX; nCntSkipCheck++)
	{
		if (m_pSkipCheckLogo[nCntSkipCheck] != NULL)
		{
			m_pSkipCheckLogo[nCntSkipCheck]->Uninit();
			m_pSkipCheckLogo[nCntSkipCheck] = NULL;
		}
	}
}

//=============================================================================
// チュートリアルのプレイヤークラスを開放する
//=============================================================================
void CTutorial::ReleasePlayer(void)
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
// チュートリアルのプレイヤー管轄クラスを開放する
//=============================================================================
void CTutorial::ReleasePlayerManager(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayerManager[nCntPlayer] != NULL)
		{
			m_pPlayerManager[nCntPlayer]->Uninit();
			delete m_pPlayerManager[nCntPlayer];
			m_pPlayerManager[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// チュートリアルの敵データ管轄クラスを開放する
//=============================================================================
void CTutorial::ReleaseEnemyManager(void)
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

//=============================================================================
// チュートリアルの操作方法表示用ポリゴンを開放する
//=============================================================================
void CTutorial::ReleaseOpeInfo(void)
{
	if (m_pOpeInfo != NULL)
	{
		m_pOpeInfo->Uninit();
		m_pOpeInfo = NULL;
	}
}

//=============================================================================
// チュートリアルのブロックを壊したレスポンス表示用ポリゴン開放処理
//=============================================================================
void CTutorial::ReleaseBreakBlockInfo(void)
{
	if (m_pBreakBlockInfo != NULL)
	{
		m_pBreakBlockInfo->Uninit();
		m_pBreakBlockInfo = NULL;
	}
}

//=============================================================================
// チュートリアルのプレイヤーを攻撃したレスポンス表示用ポリゴン開放処理
//=============================================================================
void CTutorial::ReleaseAttackPlayerInfo(void)
{
	if (m_pAttackPlayerInfo != NULL)
	{
		m_pAttackPlayerInfo->Uninit();
		m_pAttackPlayerInfo = NULL;
	}
}

//=============================================================================
// チュートリアルのゴールが近いレスポンス表示用ポリゴン開放処理
//=============================================================================
void CTutorial::ReleaseNearGoalInfo(void)
{
	if (m_pNearGoalInfo != NULL)
	{
		m_pNearGoalInfo->Uninit();
		m_pNearGoalInfo = NULL;
	}
}


//*****************************************************************************
//
// 状態による処理分け用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// チュートリアルのスキップ確認状態の更新処理
//=============================================================================
void CTutorial::SkipCheckUpdate(void)
{
	CDebugProc::Print(1, "スキップ確認状態\n");
	CDebugProc::Print(1, "選択番号 : %d\n", m_nSelectSkip);

	// ホスト側でないならこの処理をしない
	if (CManager::GetClient() != NULL)
	{
		if (CManager::GetClient()->GetConnected() == true && CManager::GetClient()->GetClientId() != 0)
		{
			return;
		}
	}

	// 入力クラスを取得
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pXInput == NULL || pFade == NULL)return;
	if (pFade->GetFade() != CFade::FADE_NONE) { return; }

	if (pKey->GetTrigger(DIK_A) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_18) == true ||
		pKey->GetRepeat(DIK_A) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_18) == true)
	{// 左方向の入力がされた
		ChangeNoneSkipCheckPolygon(m_nSelectSkip);
		m_nSelectSkip = (m_nSelectSkip + (SKIP_MAX - 1)) % SKIP_MAX;
		ChangeSelectSkipCheckPolygon(m_nSelectSkip);
		CManager::GetSound()->PlaySound(TUTORIAL_SE_SELECT_IDX);
	}
	else if (pKey->GetTrigger(DIK_D) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_19) == true ||
		pKey->GetRepeat(DIK_D) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_19) == true)
	{// 右方向の入力がされた
		ChangeNoneSkipCheckPolygon(m_nSelectSkip);
		m_nSelectSkip = (m_nSelectSkip + 1) % SKIP_MAX;
		ChangeSelectSkipCheckPolygon(m_nSelectSkip);
		CManager::GetSound()->PlaySound(TUTORIAL_SE_SELECT_IDX);
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		CManager::GetSound()->PlaySound(TUTORIAL_SE_DECIDE_IDX);
		if (m_nSelectSkip == SKIP_YES)
		{// はいだったら
			SetState(STATE_SKIP_END);
		}
		else if(m_nSelectSkip == SKIP_NO)
		{// いいえだったら
			SetState(STATE_NORMAL);
			ReleaseSkipLogo();
			ReleaseSkipCheckLogo();
			CScene::DeathCheck();
			CreateMap();
			CreatePlayer();
			CreateOpeInfo();
			SetPlayerPosToSpawn();
			CManager::GetSound()->PlaySound(TUTORIAL_BGM_IDX);
		}
	}
}

//=============================================================================
// チュートリアルのスキップして終わる状態の更新処理
//=============================================================================
void CTutorial::SkipEndUpdate(void)
{
	CDebugProc::Print(1, "スキップして終わる状態\n");

	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) return;

	if (pFade->GetFade() == CFade::FADE_NONE)
	{// フェードが使用されていない
		CManager::GetFade()->SetFade(CManager::MODE_GAME);
	}
}

//=============================================================================
// チュートリアルの通常状態の更新処理
//=============================================================================
void CTutorial::NormalUpdate(void)
{
	CDebugProc::Print(1, "通常状態\n");

	if (m_pGoalCylinder == NULL)
	{
		m_pGoalCylinder = CGoalCylinder::Create(D3DXVECTOR3(300.0f, 0.0f, 300.0f), INITIALIZE_D3DXVECTOR3,
			D3DXCOLOR(0.2f, 0.7f, 1.0f, 1.0f), 100.0f, 100.0f, 20, 10, (D3DX_PI * 0.1f), 0.0f, (D3DX_PI * 0.01f), 4);
	}
}

//=============================================================================
// チュートリアルのゴール状態の更新処理
//=============================================================================
void CTutorial::GoalUpdate(void)
{
	CDebugProc::Print(1, "ゴール状態\n");
}

//=============================================================================
// チュートリアルのマップ変更状態の更新処理
//=============================================================================
void CTutorial::MapChangeUpdate(void)
{
	CDebugProc::Print(1, "マップ変更状態\n");
}

//=============================================================================
// チュートリアルの終了状態の更新処理
//=============================================================================
void CTutorial::EndUpdate(void)
{
	CDebugProc::Print(1, "終了状態\n");

	// プレイヤーにカメラを合わせる
	EndCameraMoving();

	// 状態カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter >= TUTORIAL_END_TIME)
	{// 状態が変わる時間になっている
		CFade *pFade = CManager::GetFade();
		if (pFade == NULL) return;
		if (pFade->GetFade() != CFade::FADE_NONE) return;

		// 次のモードに設定
		pFade->SetFade(CManager::MODE_GAME);
	}
}

//=============================================================================
// チュートリアルの終了状態時のカメラ移動処理
//=============================================================================
void CTutorial::EndCameraMoving(void)
{
	if (GetCameraManager() == NULL) { return; }

	// カメラを取得する
	CCamera *pCamera = GetCameraManager()->GetCamera();
	if (pCamera == NULL) { return; }

	// カメラの注視点をプレイヤーの位置に合わせる
	int nClientId = 0;
	if (CManager::GetClient() != NULL)
	{
		nClientId = CManager::GetClient()->GetClientId();
	}
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P)
	{// ローカル2人プレイならば
		nClientId = m_nGoalPlayIdx;
	}
	if (m_pPlayer[nClientId] == NULL) { return; }
	D3DXVECTOR3 DestPos = m_pPlayer[nClientId]->GetPos();
	pCamera->SetPosR(DestPos);

	// カメラの視点目的位置を計算
	D3DXVECTOR3 PlayerRot = m_pPlayer[nClientId]->GetRot();
	D3DXVECTOR3 CameraPosVDest;
	CameraPosVDest.x = DestPos.x + sinf(PlayerRot.y - (D3DX_PI) + (D3DX_PI * 0.1f)) * TUTORIAL_END_CAMERA_LENGTH;
	CameraPosVDest.y = TUTORIAL_END_CAMERA_POSY;
	CameraPosVDest.z = DestPos.z + cosf(PlayerRot.y - (D3DX_PI) + (D3DX_PI * 0.1f)) * TUTORIAL_END_CAMERA_LENGTH;

	// カメラの視点を目的の位置まで少しずつ動かす
	D3DXVECTOR3 CameraPosVDiff;
	D3DXVECTOR3 CameraPosV = pCamera->GetPosV();
	CameraPosVDiff = CameraPosVDest - CameraPosV;
	CameraPosV += CameraPosVDiff * TUTORIAL_END_CAMERA_MOVING;
	pCamera->SetPosV(CameraPosV);
}

//=============================================================================
// チュートリアルのスキップ確認用ポリゴンを選択されていない状態にする
//=============================================================================
void CTutorial::ChangeNoneSkipCheckPolygon(int nSelect)
{
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_NONE,  TUTORIAL_SKIPCHECKLOGO_1_COL_NONE };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE };
	if (m_pSkipCheckLogo[nSelect] != NULL)
	{
		m_pSkipCheckLogo[nSelect]->SetState(CScene2DFlash::STATE_NONE);
		m_pSkipCheckLogo[nSelect]->SetScale(fSkipCheckWidthLogo[nSelect], fSkipCheckHeightLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetCol(SkipCheckColLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// チュートリアルのスキップ確認用ポリゴンを選択された状態にする
//=============================================================================
void CTutorial::ChangeSelectSkipCheckPolygon(int nSelect)
{
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_COL_SELECT };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_SELECT };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_SELECT };

	if (m_pSkipCheckLogo[nSelect] != NULL)
	{
		m_pSkipCheckLogo[nSelect]->SetState(CScene2DFlash::STATE_SELECT);
		m_pSkipCheckLogo[nSelect]->SetScale(fSkipCheckWidthLogo[nSelect], fSkipCheckHeightLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetCol(SkipCheckColLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetVtxBuffValue();
	}
}


//*****************************************************************************
//
// マップを変える用関数(デバッグ用の関数込み)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// チュートリアルの各種カウンターを初期化する
//=============================================================================
void CTutorial::ResetCounter(void)
{
	m_nSpawnEnemyCount = 0;
	m_nTutorialCounter = 0;
	m_nStateCounter = 0;
}

//=============================================================================
// チュートリアルの次のマップに切り替える処理
//=============================================================================
void CTutorial::ChangeNextMap(void)
{
	// 現在のマップを破棄
	ReleaseMap();

	// ゲームに使用しているオブジェクトを破棄
	DeleteGameObject();

	// マップ番号を増やしマップを読み込む
	m_nMapIdx++;
	if (m_nMapIdx >= TUTORIAL_MAPSTAGE_MAX)
	{
		m_nMapIdx--;
	}
	CreateMap();

	// マップ番号の桁数を計算しておく
	CalcStageDigits();

	// プレイヤーの位置をスポーン位置を設定
	SetPlayerPosToSpawn();

	// 各種カウンター初期化
	m_nTutorialCounter = 0;
	m_nSpawnEnemyCount = 0;

	// 状態を設定
	SetState(STATE_NORMAL);
}

//=============================================================================
// チュートリアルの前のマップに切り替える処理
//=============================================================================
void CTutorial::ChangePrevMap(void)
{
	// 現在のマップを破棄
	ReleaseMap();

	// ゲームに使用しているオブジェクトを破棄
	DeleteGameObject();

	// マップ番号を減らしマップを読み込む
	m_nMapIdx--;
	if (m_nMapIdx < 0)
	{
		m_nMapIdx++;
	}
	CreateMap();

	// マップ番号の桁数を計算しておく
	CalcStageDigits();

	// プレイヤーの位置をスポーン位置に設定
	SetPlayerPosToSpawn();

	// 各種カウンター初期化
	m_nTutorialCounter = 0;
	m_nSpawnEnemyCount = 0;

	// 状態を設定
	SetState(STATE_NORMAL);
}

//=============================================================================
// チュートリアルのチュートリアルに使用しているオブジェクトを破棄する処理
//=============================================================================
void CTutorial::DeleteGameObject(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (TUTORIAL_DELETE_OBJECT)
			{// チュートリアルに使用しているオブジェクトクラスだった
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
// チュートリアルのステージ番号の桁数を計算する処理
//=============================================================================
void CTutorial::CalcStageDigits(void)
{
	m_nNumNumberDigits = (int)log10f((float)(m_nMapIdx + 1)) + 1;	// 桁数を求める
	if (m_nNumNumberDigits <= 0) { m_nNumNumberDigits = 1; }	    // 0以下のとき1にする
}



//*****************************************************************************
//
// チュートリアル内スポーン処理用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// チュートリアルのプレイヤーをリスポーン位置にずらす処理
//=============================================================================
void CTutorial::SetPlayerPosToSpawn(void)
{
	// プレイヤーのリスポーン位置を取得
	D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
	CMap *pMap = GetMap();
	if (pMap == NULL) { return; }

	// プレイヤーの位置をリスポーン位置に設定
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// オンラインプレイならば
		PlayerPos = pMap->GetPlayerRespawn(CManager::GetClient()->GetClientId())->GetPos();
		if (m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetPos(PlayerPos);
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetNowRotInfo(CCharacter::ROT_UP);
		}
	}
	else
	{// ローカルプレイならば
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{// プレイヤーの人数分繰り返し
			PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			if (m_pPlayer[nCntPlayer] != NULL)
			{
				m_pPlayer[nCntPlayer]->SetPos(PlayerPos);
				m_pPlayer[nCntPlayer]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
				m_pPlayer[nCntPlayer]->SetNowRotInfo(CCharacter::ROT_UP);
			}
		}
	}
}

//=============================================================================
// チュートリアルのプレイヤーをリスポーンさせるかチェックする
//=============================================================================
void CTutorial::CheckPlayerResSpawn(int nCntPlayer)
{
	if (m_pPlayer[nCntPlayer] == NULL)
	{
		m_nPlayerRespawnCounter++;
		if (m_nPlayerRespawnCounter % TUTORIAL_PLAYER_RESPAWN_TIMING == 0)
		{
			m_nPlayerRespawnCounter = 0;
			m_pPlayer[nCntPlayer] = m_pPlayerManager[nCntPlayer]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);

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
// チュートリアルの敵の生成するタイミングかどうか判定する
//=============================================================================
void CTutorial::CheckEnemySpawn(int nTime)
{
	CMap *pMap = GetMap();
	if (pMap == NULL)return;

	int nNumEnemyListData = pMap->GetNumEnemyListData();
	CEnemy_ListData *pEnemyData = NULL;

	// もう敵が出ないのであればこの時点で処理終了
	if (m_nSpawnEnemyCount == nNumEnemyListData) return;

	// カウンター増加
	m_nTutorialCounter++;
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
// チュートリアルの敵をスポーンさせる
//=============================================================================
void CTutorial::EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt)
{
	// 敵のリスポーン位置を取得
	D3DXVECTOR3 EnemyPos = pMap->GetEnemyRespawn(pEnemyData->GetRespawnIdx())->GetPos();

	// 敵の生成
	CreateEnemy(EnemyPos, INITIALIZE_D3DXVECTOR3, pEnemyData);
}




//*****************************************************************************
//
// スクリプト読み込み用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// チュートリアルのシステム情報を読み込む
//=============================================================================
void CTutorial::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(TUTORIAL_SYSTEM_FILENAME);
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
// チュートリアルのシステム情報をファイルから読み込む
//=============================================================================
void CTutorial::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntModel = 0;
	int nCntPlayer = 0;
	int nCntMap = 0;
	int nCntSkipCheck = 0;

	while (1)
	{
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
		else if (CFunctionLib::Memcmp(pStr, PLAYERDATA_FILENAME) == 0)
		{// プレイヤーデータのスクリプトファイル名だった
			LoadPlayerFileName(pStr, nCntPlayer);
			nCntPlayer++;
		}
		else if (CFunctionLib::Memcmp(pStr, BULLET_MODEL_IDX) == 0)
		{// 弾が使用するモデルの番号だった
			m_nBulletModelIdx = CFunctionLib::ReadInt(pStr, BULLET_MODEL_IDX);
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
		else if (CFunctionLib::Memcmp(pStr, SKIPLOGOSET) == 0)
		{// スキップロゴポリゴン情報だった
			LoadSkipLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, SKIPCHECKLOGOSET) == 0)
		{// スキップ確認ロゴ用ポリゴン情報だった
			LoadSkipCheckLogo(pFileLoader, pStr, nCntSkipCheck);
			nCntSkipCheck++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// チュートリアルの読み込むテクスチャファイル名を読み込む
//=============================================================================
void CTutorial::LoadTexFileName(char *pStr, int nCntTex)
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
// チュートリアルの読み込むアイテムモデルを読み込む
//=============================================================================
void CTutorial::LoadModel(char *pStr, int nCntModel)
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
// チュートリアルの読み込むエフェクトファイル名を読み込む
//=============================================================================
void CTutorial::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// エフェクト管轄クラスを生成
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
}

//=============================================================================
// チュートリアルの読み込むプレイヤーファイル名を読み込む
//=============================================================================
void CTutorial::LoadPlayerFileName(char *pStr, int nCntPlayer)
{
	char aPlayerFileName[256] = "\0";
	strcpy(aPlayerFileName, CFunctionLib::ReadString(pStr, aPlayerFileName, PLAYERDATA_FILENAME));

	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (nCntPlayer == CCharaSelect::GetPlayerNumber(nCnt))
		{
			m_pPlayerManager[nCnt] = CPlayerManager::Create(aPlayerFileName);
		}
	}
}

//=============================================================================
// チュートリアルの読み込むマップファイル名を読み込む
//=============================================================================
void CTutorial::LoadMapFileName(char *pStr, int nCntMap)
{
	strcpy(m_pMapFileName[nCntMap], CFunctionLib::ReadString(pStr, m_pMapFileName[nCntMap], MAP_FILENAME));
}

//=============================================================================
// チュートリアルのスキップロゴポリゴン情報を読み込む
//=============================================================================
void CTutorial::LoadSkipLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nSkipLogoTexIdx = 0;
	D3DXVECTOR3 SkipLogoPos = TUTORIAL_SKIPLOGO_POS_INI;
	D3DXCOLOR SkipLogoCol = TUTORIAL_SKIPLOGO_COL_INI;
	float fSkipLogoWidth = TUTORIAL_SKIPLOGO_WIDTH_INI;
	float fSkipLogoHeight = TUTORIAL_SKIPLOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nSkipLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			SkipLogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			SkipLogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fSkipLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fSkipLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKIPLOGOSET) == 0)
		{// スキップロゴポリゴン情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_pSkipLogo = CScene2D::Create(SkipLogoPos, SkipLogoCol, fSkipLogoWidth, fSkipLogoHeight);

	// テクスチャ割り当て
	if (m_pSkipLogo != NULL && GetTextureManager() != NULL)
	{
		m_pSkipLogo->BindTexture(GetTextureManager()->GetTexture(nSkipLogoTexIdx));
	}
}

//=============================================================================
// チュートリアルのスキップ確認ロゴ用ポリゴン情報を読み込む
//=============================================================================
void CTutorial::LoadSkipCheckLogo(CFileLoader *pFileLoader, char *pStr, int nCntSkipCheck)
{
	int nSkipCheckTexIdx[SKIP_MAX] = {};
	D3DXVECTOR3 SkipCheckPosLogo[SKIP_MAX] = {TUTORIAL_SKIPCHECKLOGO_0_POS_INI,  TUTORIAL_SKIPCHECKLOGO_1_POS_INI };
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_NONE,  TUTORIAL_SKIPCHECKLOGO_1_COL_NONE };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE };

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nSkipCheckTexIdx[nCntSkipCheck] = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			SkipCheckPosLogo[nCntSkipCheck] = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			SkipCheckColLogo[nCntSkipCheck] = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fSkipCheckWidthLogo[nCntSkipCheck] = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fSkipCheckHeightLogo[nCntSkipCheck] = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKIPCHECKLOGOSET) == 0)
		{// スキップ確認ロゴ用ポリゴン情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_pSkipCheckLogo[nCntSkipCheck] = CScene2DFlash::Create(SkipCheckPosLogo[nCntSkipCheck],
		SkipCheckColLogo[nCntSkipCheck], fSkipCheckWidthLogo[nCntSkipCheck],
		fSkipCheckHeightLogo[nCntSkipCheck], 0.0f, CScene2DFlash::STATE_NONE);
	if (nCntSkipCheck == 0)m_pSkipCheckLogo[nCntSkipCheck]->SetState(CScene2DFlash::STATE_SELECT);

	// テクスチャ割り当て
	if (m_pSkipCheckLogo[nCntSkipCheck] != NULL && GetTextureManager() != NULL)
	{
		m_pSkipCheckLogo[nCntSkipCheck]->BindTexture(GetTextureManager()->GetTexture(nSkipCheckTexIdx[nCntSkipCheck]));
	}
}



//*****************************************************************************
//
// 設定、取得等色々関数(外部との窓口も含めてここに書いてます)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// チュートリアルの変数を初期化する
//=============================================================================
void CTutorial::ClearVariable(void)
{
	m_State = STATE_SKIP_CHECK;
	m_nStateCounter = 0;
	m_nSelectSkip = 0;
	m_pSkipLogo = NULL;
	m_nNumMap = 0;
	m_pMapFileName = NULL;
	for (int nCntSkip = 0; nCntSkip < SKIP_MAX; nCntSkip++)
	{
		m_pSkipCheckLogo[nCntSkip] = NULL;
	}
	m_pMapFileName = NULL;
	m_nStateCounter = 0;
	m_nMapIdx = 0;
	m_nNumMap = 0;
	m_nSpawnEnemyCount = 0;
	m_nNumEnemy = 0;
	m_nNumberTexIdx = 0;
	m_nNumNumberDigits = NULL;
	m_nBulletModelIdx = 0;
	m_nPlayerRespawnCounter = 0;
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_bDeletePlayerFlag = 0;
	m_pGoalCylinder = NULL;
	m_pOpeInfo = NULL;
	m_nGoalPlayIdx = 0;
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;
		m_pPlayerManager[nCntPlayer] = NULL;
	}
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		m_pEnemyManager[nCntEnemy] = NULL;
	}

	// レスポンス用
	m_pBreakBlockInfo = NULL;
	m_nCntBreakBlockInfoDisp = 0;
	m_pAttackPlayerInfo = NULL;
	m_nCntAttackInfoDisp = 0;
	m_pNearGoalInfo = NULL;
	m_nCntNearGoalInfoDisp = 0;
}

//=============================================================================
// チュートリアルの弾モデルを設定する処理
//=============================================================================
void CTutorial::SetBulletModel(CBullet *pBullet)
{
	pBullet->BindModel(GetModelCreate()->GetMesh(m_nBulletModelIdx), GetModelCreate()->GetBuffMat(m_nBulletModelIdx),
		GetModelCreate()->GetNumMat(m_nBulletModelIdx), GetModelCreate()->GetTexture(m_nBulletModelIdx));
}

//=============================================================================
// チュートリアルの状態を設定する
//=============================================================================
void CTutorial::SetState(const STATE state)
{
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// チュートリアルのゴールしたプレイヤーの番号を設定する
//=============================================================================
void CTutorial::SetGoalPlayerIdx(const int nIdx)
{
	m_nGoalPlayIdx = nIdx;
}

//=============================================================================
// チュートリアルのプレイヤークラスへのポインタを設定する
//=============================================================================
void CTutorial::SetPlayer(CPlayer *pPlayer, const int nIdx)
{
	m_pPlayer[nIdx] = pPlayer;
}

//=============================================================================
// チュートリアルの状態を取得する
//=============================================================================
CTutorial::STATE CTutorial::GetState(void)
{
	return m_State;
}

//=============================================================================
// チュートリアルのプレイヤークラスへのポインタを取得する
//=============================================================================
CPlayer *CTutorial::GetPlayer(const int nIdx)
{
	return m_pPlayer[nIdx];
}
