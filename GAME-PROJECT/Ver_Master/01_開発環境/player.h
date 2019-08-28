//=============================================================================
//
// プレイヤーの処理 [player.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"
#include "character.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject3D;
class CBullet;
class CEnemy;
class CBlock;
class CItem;
class CGoalCylinder;
class CRiver;
class CIceField;
class CHeadQuarters;
class CHinaarare;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CCharacter
{
public:	//誰からもアクセス可能
	//*************************************************************************
	// 構造体の定義
	//*************************************************************************
	typedef enum
	{
		MOTION_NEUTAL = 0,
		MOTION_MOVE,
		MOTION_MAX,
	}MOTION;

	typedef enum
	{
		STATE_NOMAL = 0,//通常状態
		STATE_STOP,		//停止状態
		STATE_RESPAWN,	//リスポーン状態
		STATE_DEATH,	//死亡状態
		STATE_MAX,
	}STATE;

	typedef enum
	{
		PLAYER_ABILITY_NOMAL = 0,		//通常
		PLAYER_ABILITY_BULLET_SPEEDUP,	//弾スピードアップ
		PLAYER_ABILITY_MOVE_SPEEDUP,	//移動速度アップ
		PLAYER_ABILITY_DOUBLEBULLET,	//弾２発撃てる
		PLAYER_ABILITY_ALLBLOCKDESTROY,	//全てのブロックが壊れる
		PLAYER_ABILITY_MAX,
	}PLAYER_ABILITY;

	//メンバ関数
	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(CScene *pScene);
	void SetDeathEffect(void);
	void SwitchAbility(void);

	void SetPlayer(CPlayer *pPlayer);
	void SetPlayerIdx(int nPlayerIdx);
	void SetState(STATE state);
	void SetMaxBullet(int nMaxBullet);
	void SetSplash(bool bSplash);
	void SetAllBlockDestroy(bool bAllBlockDestroy);
	void SetHelmet(bool bHelmet);

	CPlayer *GetPlayer(void);
	int GetPlayerIdx(void);
	STATE GetState(void);
	int GetMaxBullet(void);
	bool GetSplash(void);
	bool GetAllBlockDestroy(void);
	bool GetHelmet(void);

	//静的メンバ関数
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerIdx, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, int nPriority = 3);

private:	//自分だけがアクセス可能
	void ClearVariable(void);

	void InputMove_Game(void);
	void InputMove_Tutorial(void);
	void InputMove(void);
	bool InputMove_Keyboard(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot);
	bool InputMove_Controller(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot);
	void Move(void);
	void Collision(void);
	void State(void);
	void SetDiffAngle(float fDiffAngle);
	void CreateBullet(void);
	void SetInvincibleEffect(void);
	void SetMoveEffect(void);
	void SwitchItem(CItem *pItem);

	bool CollisionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CObject3D *pObject3D);
	bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet);
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy);
	bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock);
	bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CItem *pItem);
	void CollisionGoalCylinder(D3DXVECTOR3 *pPos,CGoalCylinder *pGoalCylinder);
	bool CollisionRiver(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CRiver *pRiver);
	void CollisionIceField(D3DXVECTOR3 pos ,CIceField *pIceField, bool *pLandIce);
	bool CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHeadQuarters);
	bool CollisionHinaarare(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHinaarare *pHinaarare);

	//メンバ変数
	CPlayer						*m_pPlayer;					//プレイヤー
	int							m_nPlayerIdx;				//プレイヤーの番号
	STATE						m_state;					//プレイヤーの状態
	int							m_nCntState;				//プレイヤーの状態カウンター
	int							m_nMaxBullet;				//最大弾数
	int							m_nCntAbility;				//能力カウンター
	bool						m_bAllBlockDestroy;			//全てのブロックを消せるかどうか
	bool						m_bSplash;					//汚れているかどうか
	int							m_nCntSplash;				//汚れカウンター
	MOTION						m_motion;                   //モーション情報
	PLAYER_ABILITY				m_ability;					//プレイヤー能力
	int							m_nCntBullet;               //弾のカウンター
	bool						m_bHelmet;					//ヘルメットをしようしているかどうか
	int							m_nCntHelmet;				//ヘルメットカウンター
};
#endif
