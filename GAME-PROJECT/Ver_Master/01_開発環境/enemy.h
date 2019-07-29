//=============================================================================
//
// 敵の処理 [enemy.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "character.h"
#include "player.h"
#include "block.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_PRIORITY (4)

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CBullet;
class CPlayer;
class CBlock;
class CHeadQuarters;

//*****************************************************************************
// 敵クラスの定義
//*****************************************************************************
class CEnemy : public CCharacter
{
public:	// 誰からもアクセス可能
	//*****************************************************************************
	// 敵の種類
	//*****************************************************************************
	typedef enum
	{
		TYPE_NONE = -1,      // 無し
		TYPE_NORMAL,         // 通常
		TYPE_ARMORE,         // 装甲車
		TYPE_FAST,           // 速射砲タンク
		TYPE_HEAVY,          // ヘビータンク
		TYPE_MAX,
	}TYPE;

	//*****************************************************************************
	// AIの状態
	//*****************************************************************************
	typedef enum
	{
		AI_STATE_NONE = 0,   // 何もしない状態
		AI_STATE_CHANGEROT,  // 向きを変える状態
		AI_STATE_MOVE,       // 動いている状態
		AI_STATE_MAX,
	}AI_STATE;

	// メンバ関数
	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemy();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(CScene *pScene);
	void SetItemData(const int nItemType);
	void SetAIData(const int nBulletEva, const int nMassEva, const int nDownEva);

	void SetIdx(const int nIdx);
	void SetLife(const int nLife);
	void SetType(const TYPE type);
	void SetAI_State(const AI_STATE AI_State);
	void SetAI_StateCounter(const int nStateCounter);
	void SetAI_BulletEva(const int nBulletEva);
	void SetAI_MassEva(const int nMassEva);
	void SetAI_MassCount(const int nMassCount);
	void SetAI_DownEva(const int nDownEva);
	void SetAI_NotDownCount(const int nNotDownCount);

	int GetIdx(void);
	int GetLife(void);
	TYPE GetType(void);
	AI_STATE GetAI_State(void);
	int GetAI_StateCounter(void);
	int GetAI_BulletEva(void);
	int GetAI_MassEva(void);
	int GetAI_MassCount(void);
	int GetAI_DownEva(void);
	int GetAI_NotDownCount(void);

	// 静的メンバ関数
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);
	static int GetNumAll(void);
	static int GetSpawnCounter(void);
	static void ResetSpawnCounter(void);

protected:	// 自分と派生クラスだけがアクセス可能
	// メンバ関数
	void AI(void);
	void AI_None(void);
	void AI_ChangeRot(void);
	void AI_AutoMove(void);
	void Movement(void);
	void SetBullet(void);
	void Collision(void);

private:	// 自分だけがアクセス可能
	// メンバ関数
	void ClearVariable(void);
	void HostUpdate(void);
	void SetNextMoveDirection(void);
	void CalcDestMass(void);
	bool CheckNextMovingMass(void);
	void ResetAIState(void);
	void CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath);
	bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer);
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy);
	bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet);
	bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock);
	bool CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead);
	void Destroy(void);

	// 静的メンバ変数
	static int m_nNumAll;
	static int m_nSpawnCounter;

	// メンバ変数
	int m_nIdx;                 // 敵の番号(クライアント間で同期させるのに必要)
	int m_nLife;                // 敵の体力
	bool m_bItem;               // アイテムを落とすかどうか
	int m_nItemType;            // アイテムの番号
	TYPE m_Type;                // 敵の種類
	AI_STATE m_AIState;         // AIの状態
	int m_nAI_StateCounter;     // AIの状態を管理するカウンター
	int m_nAI_MoveCounter;      // 動き始めてからどれくらい時間がたったか
	int m_nAI_BulletEva;        // 弾を打つ際のランダム評価値
	int m_nAI_MassEva;          // マスを移動した際に向きを変えさせるか判定するときのランダム評価値
	int m_nAI_MassCount;        // どれくらいマスを移動したかを数えるカウンター
	int m_nAI_DownEva;          // 下向きに動かすかどうかを判定する際のランダム評価値
	int m_nAI_NotDownCount;     // 下向きに動かなかった回数を数えるカウンター
	int m_nAI_DestMassX;        // 目的地のマス目(横方向)
	int m_nAI_DestMassZ;        // 目的地のマス目(縦方向)
	NOW_ROT_INFO m_RotInfoOld;  // 前回向いていた向き
};

//*****************************************************************************
// 通常の敵タンククラスの定義
//*****************************************************************************
class CEnemyNormal : public CEnemy
{
public:	// 誰からもアクセス可能
	// メンバ関数
	CEnemyNormal(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemyNormal();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CEnemyNormal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

protected:	// 自分と派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
};


//*****************************************************************************
// 装甲車の敵クラスの定義
//*****************************************************************************
class CEnemyArmore : public CEnemy
{
public:	// 誰からもアクセス可能
	// メンバ関数
	CEnemyArmore(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemyArmore();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CEnemyArmore *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

protected:	// 自分と派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
};

//*****************************************************************************
// 速射砲タンクの敵クラスの定義
//*****************************************************************************
class CEnemyFast : public CEnemy
{
public:	// 誰からもアクセス可能
	// メンバ関数
	CEnemyFast(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemyFast();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CEnemyFast *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

protected:	// 自分と派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
};

//*****************************************************************************
// ヘビータンクの敵クラスの定義
//*****************************************************************************
class CEnemyHeavy : public CEnemy
{
public:	// 誰からもアクセス可能
	// メンバ関数
	CEnemyHeavy(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemyHeavy();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CEnemyHeavy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

protected:	// 自分と派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
};

#endif
