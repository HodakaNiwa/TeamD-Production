//=============================================================================
//
// デモプレイの処理 [demoplay.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _DEMOPLAY_H_
#define _DEMOPLAY_H_

#include "main.h"
#include "basemode.h"
#include "item.h"
#include "enemy.h"

//前方宣言
class CFileLoader;
class CFileSaver;
class CUI;
class CPlayer;
class CPlayerManager;
class CEnemy_ListData;
class CScene2D;
class CNumber;
class CBullet;
class CBlock;
class CCharacterManager;


//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDemoplay : public CBasemode
{
public:	//誰からもアクセス可能
	//メンバ関数
	CDemoplay();
	~CDemoplay();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nItemType);

	// アイテムの処理実行用関数
	void ItemEvent_Star(int nPlayerNumber);
	void ItemEvent_Grenade(void);
	void ItemEvent_1Up(int nPlayerNumber);
	void ItemEvent_Scoop(void);
	void ItemEvent_Clock(void);
	void ItemEvent_Helmet(int nPlayerNumber);

	void SetBulletModel(CBullet *pBullet);

	//静的メンバ関数
	static CDemoplay *Create(void);

private:	//自分だけがアクセス可能
	void ClearVariable(void);

	void CreateCamera(void);
	void CreateMap(void);
	void CreateUI(void);
	void CreatePlayer(void);
	CEnemy *CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData);

	void ReleasePlayer(void);
	void ReleasePlayerManager(void);
	void ReleaseEnemyManager(void);

	void CheckEnemySpawn(int nTime);
	void EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void CreateTextureManager(int nNumTex);
	void LoadTexture(char *pFileName, int nCntTex);
	void LoadModel(char *pStr, int nCntModel);
	void LoadItemEvent(char *pStr);
	void LoadItemEventScript(CFileLoader *pFileLoader, char *pStr);
	void LoadEffectFileName(char *pStr);
	void LoadPlayerFileName(char *pStr, int nCntPlayer);
	void LoadEnemyFileName(char *pStr, int nCntEnemy);
	void LoadMapFileName(char *pStr);

	char m_aMapFileName[256];             // 読み込むマップのファイル名
	CUI *m_pUI;                           // UIクラスへのポインタ
	int m_nGameCounter;                   // ゲームの時間を数えるカウンター
	int m_nSpawnEnemyCount;               // スポーンした敵の数を数えるカウンター
	int m_nNumberTexIdx;                  // 数字ポリゴンが使用するテクスチャの番号
	int m_nBulletModelIdx;                // 弾が使用するモデルの番号
	int m_nItemModelIdx[CItem::TYPE_MAX]; // アイテムが使用するモデルの番号
	bool m_bEnemyMove;                    // 敵が動けるかどうか
	int m_nEnemyMoveCounter;              // 敵が動けない状態になってからの時間を数えるカウンター

	// プレイヤー用
	CPlayer *m_pPlayer;
	CPlayerManager *m_pPlayerManager;
	int m_nPlayerStock;
	int m_nPlayerRespawnCounter;

	// 敵データ用
	CCharacterManager *m_pEnemyManager[CEnemy::TYPE_MAX];

	typedef struct
	{// アイテムイベントデータ
		int nStop;
	}ITEMEVENT_DATA;
	ITEMEVENT_DATA m_ItemEventData;
};
#endif