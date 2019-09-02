//=============================================================================
//
// ゲームの処理 [game.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "basemode.h"
#include "item.h"
#include "enemy.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAME_BGM_HINAMAP_IDX (3)     // ゲームのBGM番号(ひな祭りマップ)


// 前方宣言
class CFileLoader;
class CFileSaver;
class CUI;
class CPlayer;
class CPlayerManager;
class CEnemy_ListData;
class CScene2D;
class CScene2DFlash;
class CNumber;
class CBullet;
class CBlock;
class CCharacterManager;
class CItem;
class CItemCylinder;
class CRiver;
class CCreamMist;

//*****************************************************************************
// ポーズクラス定義
//*****************************************************************************
class CPause
{
public:	   // 誰からもアクセス可能
	//--------------
	// 状態
	//--------------
	typedef enum
	{
		SELECT_CONTINUE = 0,
		SELECT_RETRY,
		SELECT_QUIT,
		SELECT_MAX
	}SELECT;

	CPause();
	~CPause();

	static CPause *Create(CTextureManager *pTexManager);

	HRESULT Init(CTextureManager *pTexManager);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetSelect(const int nSelect);
	void SetOpenId(const int nId);

	CScene2D *GetBlackBg(void);
	CScene2D *GetPauseBg(void);
	CScene2DFlash *GetPauseSelect(int nIdx);
	int GetSelect(void);
	int GetOpenId(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // 自分だけがアクセス可能
	void ClearVariable(void);
	void CreateBlackBg(void);
	void CreatePauseBg(CTextureManager *pTexManager);
	void CreatePauseSelect(CTextureManager *pTexManager);
	void ReleaseBlackBg(void);
	void ReleasePauseBg(void);
	void ReleasePauseSelect(void);
	void WaitInput(void);
	void ChangeNoneStagePolygon(int nSelect);
	void ChangeSelectStagePolygon(int nSelect);

	CScene2D      *m_pBlackBg;                   // 黒背景用ポリゴン
	CScene2D      *m_pPauseBg;                   // ポーズ背景用ポリゴン
	CScene2DFlash *m_apPauseSelect[SELECT_MAX];  // 選択項目用ポリゴン
	int           m_nSelect;                     // 現在選択されている番号
	int           m_nOpenId;                     // ポーズを開いたプレイヤーの番号
};

//*****************************************************************************
// ゲームクラス定義
//*****************************************************************************
class CGame : public CBasemode
{
public:	// 誰からもアクセス可能
	//--------------
	// 状態
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_STAGE_SELECT,
		STATE_STAGE_DISP,
		STATE_WAIT,
		STATE_NORMAL,
		STATE_PAUSE,
		STATE_GAMEOVER,
		STATE_RESULT,
		STATE_CHANGE_MAP,
		STATE_PREV_MAP,
		STATE_NEXT_MAP,
		STATE_END,
		STATE_END_RETRY,
		STATE_END_QUIT,
		STATE_MAX
	}STATE;

	//--------------
	// マップの種類
	//--------------
	typedef enum
	{
		MAPTYPE_NONE = -1,   // 無し
		MAPTYPE_CANDY,       // お菓子
		MAPTYPE_CHRISTMAS,   // クリスマス
		MAPTYPE_HINAMATSURI, // ひな祭り
		MAPTYPE_MAX
	}MAPTYPE;

	//---------------------
	// ひな祭りのイベント
	//---------------------
	typedef enum
	{
		HINAEVENT_NORMAL = 0,
		HINAEVENT_CHERRYBLOSSOMS,
		HINAEVENT_DROP_ITEM,
		HINAEVENT_DROP_HINAARARE,
		HINAEVENT_EVENTSTART,
		HINAEVENT_MAX
	}HINAEVENT;

	//---------------------------
	// ゲーム内リザルトの進行度
	//---------------------------
	typedef enum
	{
		GAMERESULT_NONE = -1,
		GAMERESULT_NORMALENEMY,
		GAMERESULT_ARMOREENEMY,
		GAMERESULT_FASTENEMY,
		GAMERESULT_HEAVYENEMY,
		GAMERESULT_TORTAL,
		GAMERESULT_MAX
	}GAMERESULT;

	// メンバ関数
	CGame();
	~CGame();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CItem *CreateItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nItemType);
	void DeletePlayer(CPlayer *pPlayer, const int nIdx);
	void DeleteBlock(const int nIdx);
	void DeleteEnemy(const int nIdx);
	void DeleteItem(const int nIdx);
	void HitBullet(void);
	void ReleasePause(void);
	void CreateMist(void);

	// アイテムの処理実行用関数
	void ItemEvent_Star(int nPlayerNumber);
	void ItemEvent_Grenade(int nPlayerNumber);
	void ItemEvent_Granade_Shake(void);
	void ItemEvent_1Up(int nPlayerNumber);
	void ItemEvent_Scoop(int nPlayerNumber);
	void ItemEvent_Clock(int nPlayerNumber);
	void ItemEvent_Helmet(int nPlayerNumber);


	// スコア加算処理実行用関数
	void ScoreUp_NormalEnemy(int nPlayerIdx);
	void ScoreUp_ArmoreEnemy(int nPlayerIdx);
	void ScoreUp_FastEnemy(int nPlayerIdx);
	void ScoreUp_HeavyEnemy(int nPlayerIdx);


	void SetBulletModel(CBullet *pBullet);
	void SetState(const STATE state);
	void SetPlayer(CPlayer *pPlayer, const int nIdx);
	void SetNumEnemy(const int nNumEnemy);

	STATE GetState(void);
	CPlayer *GetPlayer(const int nIdx);
	int GetNumEnemy(void);
	bool GetEnemyMove(void);
	int GetStageIdx(void);

	// 静的メンバ関数
	static CGame *Create(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
	void ClearVariable(void);
	void StopBGM(void);
	void MistCheck(void);

	// 生成処理用関数
	void CreateMapFilePointer(void);
	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateMap(void);
	void CreateUI(void);
	void CreatePlayer(void);
	CEnemy *CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData);
	void CreateGameOverLogo(void);
	void CreateStageBg(void);
	void CreateStageLogo(void);
	void CreateStageNumber(void);
	void CreateBlossoms(void);
	void CreateHinaarare(void);
	void CreateGameResult(void);
	void CreateGameResult_Bg(void);
	void CreateGameResult_Stage(void);
	void CreateGameResult_HighScore(void);
	void CreateGameResult_PlayerScore(void);
	void CreateGameResult_EnemyScore_Logo(void);
	void CreateGameResult_EnemyScore_Number(void);
	void CreateGameResult_TortalScore_Logo(void);
	void CreateGameResult_TortalScore_Number(void);
	void CreateGameResult_BonusScore(void);
	void CreatePause(int nIdxPlayer = 0);
	void CreateNotPause(void);
	void CreateEventStartLogo(void);


	// 開放処理用関数
	void ReleaseMapFilePointer(void);
	void ReleaseUI(void);
	void ReleasePlayer(void);
	void ReleasePlayerManager(void);
	void ReleaseEnemyManager(void);
	void ReleaseGameOverLogo(void);
	void ReleaseStageBg(void);
	void ReleaseStageLogo(void);
	void ReleaseStageNumber(void);
	void ReleaseGameResult(void);
	void ReleaseGameResult_Bg(void);
	void ReleaseGameResult_Stage(void);
	void ReleaseGameResult_HighScore(void);
	void ReleaseGameResult_PlayerScore(void);
	void ReleaseGameResult_EnemyScore(void);
	void ReleaseGameResult_TortalScore(void);
	void ReleaseGameResult_BonusScore(void);
	void ReleaseNotPause(void);
	void ReleaseEventStartLogo(void);
	void ReleaseCreamMist(void);


	// サーバーとのメッセージ交換用関数
	void SetDataToServer(void);
	void SetDataToServerFromPlayer(void);
	void SetDataToServerFromGame(void);
	void SetDataToServerFromEnemy(void);
	void SetDataToServerFromEnemyBullet(void);
	void SetDataToServerFromPlayerBullet(void);
	void SetDataToServerFromDeleteBlock(void);
	void SetDataToServerFromDeleteEnemy(void);
	void SetDataToServerFromDeleteItem(void);
	void SetDataToServerFromItem(void);
	void SetDataToServerFromClientState(void);
	void SetDataToServerFromBreakEnemy(void);
	void SetDataToServerFromSpawnEnemyCount(void);
	void SetDataToServerFromMapEvent(void);
	void SetDataToServerFromHitBullet(void);
	char *GetDataToEnemy(CEnemy *pEnemy, char *pStr);
	char *GetDataToEnemyBullet(CBullet *pBullet, char *pStr);
	char *GetDataToPlayerBullet(CBullet *pBullet, char *pStr);
	char *GetDataToItem(CItem *pItem, char *pStr);
	void GetDataFromServer(void);
	char *SetDataToPlayerFromServer(char *pStr);
	char *SetDataToGameFromServer(char *pStr);
	char *SetDataToEnemyFromServer(char *pStr);
	char *SetDataToEnemy(CEnemy *pEnemy, char *pStr);
	char *SetDataToCreateEnemy(char *pStr);
	void ReleaseEnemy(int nNumEnemy);
	void ReleaseCheckEnemy(CEnemy *pEnemy, int *pNumEnemy);
	char *SetDataToPlayerBulletFromServer(char *pStr);
	void ReleasePlayerBullet(int nNumBullet);
	void ReleaseCheckPlayerBullet(CBullet *pBullet, int *pNumBullet);
	char *SetDataToPlayerBullet(CBullet *pBullet, char *pStr, int *pNumBullet);
	char *SetDataToCreatePlayerBullet(char *pStr);
	char *SetDataToEnemyBulletFromServer(char *pStr);
	void ReleaseEnemyBullet(int nNumBullet);
	void ReleaseCheckEnemyBullet(CBullet *pBullet, int *pNumBullet);
	char *SetDataToEnemyBullet(CBullet *pBullet, char *pStr, int *pNumBullet);
	char *SetDataToCreateEnemyBullet(char *pStr);
	char *SetDataToDeleteBlock(char *pStr);
	void ReleaseCheckBlock(CBlock *pBlock, int *pDeleteIdx, int *nNumDeleteBlock);
	char *SetDataToDeleteEnemy(char *pStr);
	void ReleaseCheckDeleteEnemy(CEnemy *pEnemy, int *pDeleteIdx, int *nNumDeleteEnemy);
	char *SetDataToItemFromServer(char *pStr);
	char *SetDataToItem(CItem *pItem, char *pStr, int *pNumItem);
	char *SetDataToCreateItem(char *pStr);
	void ReleaseItem(int nNumItem);
	void ReleaseCheckItem(CItem *pItem, int *pNumItem);
	char *SetDataToDeleteItem(char *pStr);
	void ReleaseCheckDeleteItem(CItem *pItem, int *pDeleteIdx, int *nNumDeleteItem);
	char *SetDataToClientState(char *pStr);
	char *SetDataToBreakEnemy(char *pStr);
	char *SetDataToSpawnEnemyCount(char *pStr);
	char *SetDataToMapEvent(char *pStr);
	char *SetDataToHitBullet(char *pStr);


	// 状態による更新処理分け用関数
	void StageSelectUpdate(void);
	void StageDispUpdate(void);
	void NormalUpdate(void);
	void PauseUpdate(void);
	void GameOverUpdate(void);
	void GameOverLogoUp(void);
	void ResultUpdate(void);
	void ResultUpdate_None(void);
	void ResultUpdate_Normal(void);
	void ResultUpdate_Armore(void);
	void ResultUpdate_Fast(void);
	void ResultUpdate_Heavy(void);
	void ResultUpdate_Tortal(void);
	void ChangeMapUpdate(void);
	void EndUpdate(void);
	void EndRetryUpdate(void);
	void EndQuitUpdate(void);


	// マップイベント用関数
	void MapEvent_Hinamatsuri(void);
	void MapEvent_Hinamatsuri_Normal(void);
	void MapEvent_Hinamatsuri_CherryBlossoms(void);
	void MapEvent_Hinamatsuri_Drop_Item(void);
	bool MapEvent_Hinamatsuri_Drop_Item_CheckPos(void);
	bool MapEvent_Hinamatsuri_Drop_Item_CheckRiver(CRiver *pRiver);
	void MapEvent_Hinamatsuri_Drop_Hinaarare(void);
	void MapEvent_Hinamatsuri_EventStart(void);


	// ゲーム内スポーン処理用関数
	void SetPlayerPosToSpawn(void);
	void CheckPlayerResSpawn(int nCntPlayer);
	void CheckEnemySpawn(int nTime);
	void EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt);


	// マップを変える用関数(デバッグ用の関数込み)
	void ResetCounter(void);
	void ChangeNextMap(void);
	void ChangePrevMap(void);
	void DeleteGameObject(void);
	void CalcStageDigits(void);


	// スクリプト読み込み用関数(マップイベント用にスクリプトを作るならここに！！)
	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadModel(char *pStr, int nCntModel);
	void LoadMapFileName(char *pStr, int nCntMap);
	void LoadItemEvent(char *pStr);
	void LoadItemEventScript(CFileLoader *pFileLoader, char *pStr);
	void LoadItemEvent_Shake(CFileLoader *pFileLoader, char *pStr);
	void LoadEffectFileName(char *pStr);
	void LoadPlayerFileName(char *pStr, int nCntPlayer);
	void LoadEnemyFileName(char *pStr, int nCntEnemy);
	void LoadStageBg(CFileLoader *pFileLoader, char *pStr);
	void LoadStageLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadStageNumber(CFileLoader *pFileLoader, char *pStr);
	void LoadGameOverLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadHinamatsuriEvent(char *pStr);
	void LoadHinamatsuriEventScript(CFileLoader *pFileLoader, char *pStr);
	void LoadCherryBlossomsData(CFileLoader *pFileLoader, char *pStr);
	void LoadHinaarareData(CFileLoader *pFileLoader, char *pStr);


	// メンバ変数
	int m_nNumMap;                           // 読み込むマップの数
	char **m_pMapFileName;                   // 読み込むマップのファイル名
	int m_nMapIdx;                           // 現在のマップの番号
	int m_nStageIdx;                         // 現在のステージ番号
	CUI *m_pUI;                              // UIクラスへのポインタ
	STATE m_State;                           // 今回の状態
	STATE m_StateOld;                        // 前回の状態
	int m_nStateCounter;                     // 状態を管理するカウンター
	int m_nGameCounter;                      // ゲームの時間を数えるカウンター
	int m_nSpawnEnemyCount;                  // スポーンした敵の数を数えるカウンター
	int m_nNumEnemy;                         // 出現する敵の数
	CScene2D *m_pGameOverLogo;               // ゲームオーバーを知らせるロゴポリゴン
	CScene2D *m_pStageBg;                    // ステージ表示状態の時の背景ポリゴン
	CScene2D *m_pStageLogo;                  // ステージロゴ用ポリゴン
	CNumber *m_pStageNumber;                 // ステージ番号表示用ポリゴン
	int m_nNumberTexIdx;                     // 数字ポリゴンが使用するテクスチャの番号
	int m_nNumNumberDigits;                  // 現在のステージ番号の桁数
	int m_nBulletModelIdx;                   // 弾が使用するモデルの番号
	int m_nItemModelIdx[CItem::TYPE_MAX];    // アイテムが使用するモデルの番号
	bool m_bEnemyMove;                       // 敵が動けるかどうか
	int m_nEnemyMoveCounter;                 // 敵が動けない状態になってからの時間を数えるカウンター

	// マップイベント用
	int m_nNextEvent;                        // 次に起こすマップイベント
	HINAEVENT m_HinaEvent;                   // ひな祭りマップのイベントを分ける変数
	int m_nEventCounter;                     // イベントカウンター
	int m_nRandomEventEva;                   // イベントを起こすかどうかのランダム評価値
	int m_nNotEventCounter;                  // イベントが起こっていない時間を数えるカウンター
	int m_nEventWakeUpCount[HINAEVENT_MAX];  // 連続してイベントが起こってないかを判定するためのカウンター
	CScene2D *m_pEventStartLogo;             // イベントが始まるときに表示する

	// アイテムドロップイベント用
	CItem *m_pItem;                          // アイテムクラスへのポインタ
	CItemCylinder *m_pItemCylinder;          // アイテム用円筒クラスへのポインタ
	D3DXVECTOR3 m_ItemDropPos;               // アイテムをドロップする位置

	// プレイヤー用
	CPlayer *m_pPlayer[MAX_NUM_PLAYER];
	CPlayerManager *m_pPlayerManager[MAX_NUM_PLAYER];
	int m_nPlayerStock[MAX_NUM_PLAYER];
	int m_nPlayerRespawnCounter;
	int m_nPlayerStockIni;
	int m_nPlayerStockMax;

	// 敵データ用
	CCharacterManager *m_pEnemyManager[CEnemy::TYPE_MAX];

	// プレイヤーの同期に必要
	bool m_bDeletePlayerFlag;

	// ブロックの同期に必要
	int m_nNumDeleteBlock;
	char m_aDeleteBlock[2048];

	// 敵の同期に必要
	int m_nNumDeleteEnemy;
	char m_aDeleteEnemy[2048];

	// アイテムの同期に必要
	int m_nNumDeleteItem;
	char m_aDeleteItem[2048];

	// プレイヤー同士の弾ヒットの同期に必要
	bool m_bHitBulletFlag;

	// ポーズ用
	bool m_bPauseOpen;
	CPause *m_pPause;
	CScene2D *m_pPausePlayerNumber;
	CScene2D *m_pNotPause;
	CScene2D *m_pNotPauseBlackBg;

	// クリーム靄用
	CCreamMist *m_pCreamMist;
	int m_nMistCounter;

	// 各種クラス生成用情報
	typedef struct
	{// ステージ背景データ
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}STAGEBG_DATA;
	STAGEBG_DATA m_StageBgData;

	typedef struct
	{// ステージロゴデータ
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}STAGELOGO_DATA;
	STAGELOGO_DATA m_StageLogoData;

	typedef struct
	{// ステージ番号データ
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}STAGENUMBER_DATA;
	STAGENUMBER_DATA m_StageNumberData;

	typedef struct
	{// ゲームオーバーロゴデータ
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}GAMEOVERPOLY_DATA;
	GAMEOVERPOLY_DATA m_GameOverPolyData;

	typedef struct
	{// アイテムイベントデータ
		int nShakeTime;
		float fShake;
		float fShakeCut;
		int nStop;
	}ITEMEVENT_DATA;
	ITEMEVENT_DATA m_ItemEventData;

	typedef struct
	{// 桜の花びらデータ
		int nTime;
		int nLap;
		int nTexIdx;
		int nMoveXMax;
		int nMoveXMin;
		int nMoveYMax;
		int nMoveYMin;
		int nWidthMax;
		int nWidthMin;
		int nHeightMax;
		int nHeightMin;
		int nAngleSpeedMax;
		int nAngleSpeedMin;
		int nRotSpeedMax;
		int nRotSpeedMin;
	}CHERRYBLOSSOMS_DATA;
	CHERRYBLOSSOMS_DATA m_CherryBlossomsData;

	typedef struct
	{// ひなあられデータ
		int nModelIdx;
		int nTime;
		int nAppear;
	}HINAARARE_DATA;
	HINAARARE_DATA m_HinarareData;

	// ゲーム内リザルト用データ
	bool m_bNextGameResult;
	bool m_bAddBonus;
	GAMERESULT m_GameResultAddvance;

	// 背景用
	CScene2D *m_pGameResultBg;

	// ステージ表示用
	CScene2D *m_pGameResultStage;
	CNumber *m_pGameResultStageNumber;

	// ハイスコア用
	CNumber *m_pHighScore;
	CScene2D *m_pHighScoreLogo;

	// プレイヤースコア用
	int m_nScore[MAX_NUM_PLAYER];
	CNumber *m_apScore[MAX_NUM_PLAYER];
	CScene2D *m_apPlayerLogo[MAX_NUM_PLAYER];

	// エネミースコア用
	int m_nNumBreakEnemy[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	CNumber *m_apNumBreakEnemy[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	CNumber *m_apEnemyScore[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	CScene2D *m_apEnemyScorePointLogo[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	CScene2D *m_apEnemyScoreArrow[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	CScene2D *m_apEnemyScoreIcon[CEnemy::TYPE_MAX];

	// トータルスコア用
	int m_nNumAllBreakEnemy[MAX_NUM_PLAYER];
	CNumber *m_apNumAllBreakEnemy[MAX_NUM_PLAYER];
	CScene2D *m_pTortalLogo;
	CScene2D *m_pTortalLine;

	// ボーナススコア用
	CNumber *m_pBonus;
	CScene2D *m_pBonusPointLogo;
	CScene2D *m_pBonusScoreLogo;
};
#endif