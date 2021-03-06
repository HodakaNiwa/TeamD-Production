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

//前方宣言
class CFileLoader;
class CFileSaver;
class CUI;
class CPlayer;
class CEnemy_ListData;
class CScene2D;
class CNumber;
class CEnemy;
class CBullet;
class CBlock;

//*****************************************************************************
// クラス定義
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
		STATE_GAMEOVER,
		STATE_GAMECLEAR,
		STATE_CHANGE_MAP,
		STATE_PREV_MAP,
		STATE_NEXT_MAP,
		STATE_END,
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
		MAPTYPE_NEWYEAR,     // お正月
		MAPTYPE_HINAMATSURI, // ひな祭り
		MAPTYPE_MAX
	}MAPTYPE;

	//---------------------
	// ひな祭りのイベント
	//---------------------
	typedef enum
	{
		HINAEVENT_NONE = -1,
		HINAEVENT_NORMAL,
		HINAEVENT_CHERRY_BLOSSOMS,
		HINAEVENT_DROP_ITEM,
		HINAEVENT_MAX
	}HINAEVENT;

	// メンバ関数
	CGame();
	~CGame();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DeletePlayer(CPlayer *pPlayer, const int nIdx);
	void DeleteBlock(const int nIdx);
	void DeleteEnemy(const int nIdx);

	// アイテムの処理実行用関数
	void ItemEvent_Star(int nPlayerNumber);
	void ItemEvent_Grenade(void);
	void ItemEvent_1Up(int nPlayerNumber);
	void ItemEvent_Scoop(void);
	void ItemEvent_Clock(void);
	void ItemEvent_Helmet(int nPlayerNumber);


	void SetBulletModel(CBullet *pBullet);
	void SetItemModel(CItem *pItem, int nType);
	void SetState(const STATE state);
	void SetPlayer(CPlayer *pPlayer, const int nIdx);
	void SetNumEnemy(const int nNumEnemy);

	STATE GetState(void);
	CPlayer *GetPlayer(const int nIdx);
	int GetNumEnemy(void);
	bool GetEnemyMove(void);

	// 静的メンバ関数
	static CGame *Create(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
	void ClearVariable(void);

	// 生成処理用関数
	void CreateMapFilePointer(void);
	void CreateTexture(int nNumTex);
	void CreateItemModel(int nNumItem);
	void CreateCamera(void);
	void CreateMap(void);
	void CreateUI(void);
	void CreatePlayer(void);
	void CreateGameOverLogo(void);
	void CreateStageBg(void);
	void CreateStageLogo(void);
	void CreateStageNumber(void);
	void CreateBlossoms(void);


	// 開放処理用関数
	void ReleaseMapFilePointer(void);
	void ReleaseUI(void);
	void ReleasePlayer(void);
	void ReleaseGameOverLogo(void);
	void ReleaseStageBg(void);
	void ReleaseStageLogo(void);
	void ReleaseStageNumber(void);


	// サーバーとのメッセージ交換用関数
	void SetDataToServer(void);
	void SetDataToServerFromPlayer(void);
	void SetDataToServerFromGame(void);
	void SetDataToServerFromEnemy(void);
	void SetDataToServerFromEnemyBullet(void);
	void SetDataToServerFromPlayerBullet(void);
	void SetDataToServerFromDeleteBlock(void);
	void SetDataToServerFromDeleteEnemy(void);
	char *GetDataToEnemy(CEnemy *pEnemy, char *pStr);
	char *GetDataToEnemyBullet(CBullet *pBullet, char *pStr);
	char *GetDataToPlayerBullet(CBullet *pBullet, char *pStr);
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


	// 状態による更新処理分け用関数
	void StageSelectUpdate(void);
	void StageDispUpdate(void);
	void NormalUpdate(void);
	void GameOverUpdate(void);
	void GameOverLogoUp(void);
	void GameClearUpdate(void);
	void ChangeMapUpdate(void);
	void EndUpdate(void);


	// マップイベント用関数(みんながいじる OR 追加するならここ！！ それ以外は別途相談して・・・)
	void MapEvent_Candy(void);
	void MapEvent_Christmas(void);
	void MapEvent_NewYear(void);
	void MapEvent_Hinamatsuri(void);
	void MapEvent_Hinamatsuri_Normal(void);
	void MapEvent_Hinamatsuri_CherryBlossoms(void);
	void MapEvent_Hinamatsuri_DropItem(void);


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
	void LoadEffectFileName(char *pStr);
	void LoadStageBg(CFileLoader *pFileLoader, char *pStr);
	void LoadStageLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadStageNumber(CFileLoader *pFileLoader, char *pStr);
	void LoadGameOverLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadHinamatsuriEvent(char *pStr);
	void LoadHinamatsuriEventScript(CFileLoader *pFileLoader, char *pStr);
	void LoadCherryBlossomsData(CFileLoader *pFileLoader, char *pStr);


	// メンバ変数
	int m_nNumMap;                        // 読み込むマップの数
	char **m_pMapFileName;                // 読み込むマップのファイル名
	int m_nMapIdx;                        // 現在のマップの番号
	int m_nStageIdx;                      // 現在のステージ番号
	CUI *m_pUI;                           // UIクラスへのポインタ
	STATE m_State;                        // 今回の状態
	STATE m_StateOld;                     // 前回の状態
	int m_nStateCounter;                  // 状態を管理するカウンター
	CPlayer *m_pPlayer[MAX_NUM_PLAYER];   // プレイヤークラス型のポインタ
	int m_nGameCounter;                   // ゲームの時間を数えるカウンター
	int m_nSpawnEnemyCount;               // スポーンした敵の数を数えるカウンター
	int m_nNumEnemy;                      // 出現する敵の数
	CScene2D *m_pGameOverLogo;            // ゲームオーバーを知らせるロゴポリゴン
	CScene2D *m_pStageBg;                 // ステージ表示状態の時の背景ポリゴン
	CScene2D *m_pStageLogo;               // ステージロゴ用ポリゴン
	CNumber **m_pStageNumber;             // ステージ番号表示用ポリゴン
	int m_nNumberTexIdx;                  // 数字ポリゴンが使用するテクスチャの番号
	int m_nNumNumberDigits;               // 現在のステージ番号の桁数
	int m_nPlayerStock[MAX_NUM_PLAYER];   // プレイヤーの残機数
	int m_nPlayerRespawnCounter;          // プレイヤーのリスポーンを管理するカウンター
	int m_nBulletModelIdx;                // 弾が使用するモデルの番号
	int m_nItemModelIdx[CItem::TYPE_MAX]; // アイテムが使用するモデルの番号
	bool m_bEnemyMove;                    // 敵が動けるかどうか
	int m_nEnemyMoveCounter;              // 敵が動けない状態になってからの時間を数えるカウンター
	HINAEVENT m_HinaEvent;                // ひな祭りマップのイベントを分ける変数
	int m_nEventCounter;                  // イベントカウンター

	// プレイヤーの同期に必要
	bool m_bDeletePlayerFlag;

	// ブロックの同期に必要
	int m_nNumDeleteBlock;
	char m_aDeleteBlock[2048];

	// 敵の同期に必要
	int m_nNumDeleteEnemy;
	char m_aDeleteEnemy[2048];

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
		int nStop;
	}ITEMEVENT_DATA;
	ITEMEVENT_DATA m_ItemEventData;

	typedef struct
	{// 桜の花びらデータ
		int nTime;
		int nAppear;
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
};
#endif