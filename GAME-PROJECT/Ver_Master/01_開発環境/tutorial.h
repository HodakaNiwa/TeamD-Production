//=============================================================================
//
// チュートリアルの処理 [tutorial.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "basemode.h"
#include "item.h"
#include "enemy.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CFileLoader;
class CFileSaver;
class CScene2D;
class CScene2DFlash;
class CPlayer;
class CPlayerManager;
class CEnemy_ListData;
class CBullet;
class CBlock;
class CCharacterManager;
class CGoalCylinder;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTutorial : public CBasemode
{
public:	// 誰からもアクセス可能
	//--------------
	// スキップ機能
	//--------------
	typedef enum
	{
		SKIP_NONE = -1,
		SKIP_YES,
		SKIP_NO,
		SKIP_MAX
	}SKIP;

	//--------------
	// 状態
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_SKIP_CHECK,
		STATE_SKIP_END,
		STATE_NORMAL,
		STATE_GOAL,
		STATE_MAP_CHANGE,
		STATE_END,
		STATE_MAX
	}STATE;

	// メンバ関数
	CTutorial();
	~CTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void DeletePlayer(CPlayer *pPlayer, const int nIdx);
	void DeleteBlock(const int nIdx);
	void DeleteEnemy(const int nIdx);

	void SetBulletModel(CBullet *pBullet);
	void SetPlayer(CPlayer *pPlayer, const int nIdx);
	void SetState(const STATE state);

	CPlayer *GetPlayer(const int nIdx);
	STATE GetState(void);

	// 静的メンバ関数
	static CTutorial *Create(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
	void ClearVariable(void);

	// 生成用関数
	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateMap(void);
	void CreateMapFilePointer(void);
	void CreatePlayer(void);
	CEnemy *CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData);


	// 開放用関数
	void ReleaseMapFilePointer(void);
	void ReleaseSkipLogo(void);
	void ReleaseSkipCheckLogo(void);
	void ReleasePlayer(void);
	void ReleasePlayerManager(void);
	void ReleaseEnemyManager(void);


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


	// 状態による処理分け用関数
	void SkipCheckUpdate(void);
	void SkipEndUpdate(void);
	void NormalUpdate(void);
	void GoalUpdate(void);
	void MapChangeUpdate(void);
	void EndUpdate(void);
	void EndCameraMoving(void);
	void ChangeNoneSkipCheckPolygon(int nSelect);
	void ChangeSelectSkipCheckPolygon(int nSelect);


	// スクリプト読み込み用関数
	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadModel(char *pStr, int nCntModel);
	void LoadEffectFileName(char *pStr);
	void LoadMapFileName(char *pStr, int nCntMap);
	void LoadPlayerFileName(char *pStr, int nCntPlayer);
	void LoadSkipLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadSkipCheckLogo(CFileLoader *pFileLoader, char *pStr, int nCntSkipCheck);


	// ゲーム内スポーン処理用関数
	void SetPlayerPosToSpawn(void);
	void CheckPlayerResSpawn(int nCntPlayer);
	void CheckEnemySpawn(int nTime);
	void EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt);
	void CalcStageDigits(void);

	// マップを変える用関数(デバッグ用の関数込み)
	void ResetCounter(void);
	void ChangeNextMap(void);
	void ChangePrevMap(void);
	void DeleteGameObject(void);

	STATE m_State;                              // 現在の状態
	STATE m_StateOld;                           // 前回の状態
	int m_nStateCounter;                        // 状態を管理するカウンター
	int m_nNumMap;                              // マップを読み込む数
	int m_nMapIdx;                              // 現在のマップ番号
	char **m_pMapFileName;                      // 読み込むマップのファイル名
	CScene2D *m_pSkipLogo;                      // スキップするかを表示するロゴポリゴン
	CScene2DFlash *m_pSkipCheckLogo[SKIP_MAX];  // スキップ確認時に はい いいえ を表示する用のロゴポリゴン
	int m_nSelectSkip;                          // スキップするかどうか
	int m_nTutorialCounter;                     // チュートリアルの時間を数えるカウンター
	int m_nSpawnEnemyCount;                     // スポーンした敵の数を数えるカウンター
	int m_nNumEnemy;                            // 出現する敵の数
	int m_nNumberTexIdx;                        // 数字ポリゴンが使用するテクスチャの番号
	int m_nNumNumberDigits;                     // 現在のステージ番号の桁数
	int m_nBulletModelIdx;                      // 弾が使用するモデルの番号
	CGoalCylinder *m_pGoalCylinder;             // ゴール用円筒クラスへのポインタ

	// プレイヤー用
	CPlayer *m_pPlayer[MAX_NUM_PLAYER];
	CPlayerManager *m_pPlayerManager[MAX_NUM_PLAYER];
	int m_nPlayerRespawnCounter;

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
};
#endif