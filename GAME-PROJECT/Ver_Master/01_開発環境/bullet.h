//*****************************************************************************
//
//     弾の処理[bullet.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CModel;
class CPlayer;
class CEnemy;
class CBlock;
class CHeadQuarters;

//*****************************************************************************
//    弾クラスの定義
//*****************************************************************************
class CBullet : public CObject3D
{
public:    // 誰でもアクセス可能
	//------------------
	// 弾の種類
	//------------------
	typedef enum
	{
		TYPE_PLAYER_0 = 0,
		TYPE_PLAYER_1,
		TYPE_ENEMY,
		TYPE_MAX,
	}TYPE;

	CBullet(int nPriority = 3, OBJTYPE objType = OBJTYPE_BULLET);
	~CBullet();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindModel(LPD3DXMESH pMesh = NULL, LPD3DXBUFFER pBuffMat = NULL, DWORD nNumMat = 0, LPDIRECT3DTEXTURE9 *pTexture = NULL);
	void Hit(CScene *pScene);

	void SetPosOld(const D3DXVECTOR3 posold);
	void SetMove(const D3DXVECTOR3 move);
	void SetIdx(const int nIdx);
	void SetType(const TYPE type);
	void SetParent(CScene *pParent);
	void SetModel(CModel *pModel);

	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetMove(void);
	int GetIdx(void);
	TYPE GetType(void);
	CScene *GetParent(void);
	CModel *GetModel(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void Move(void);
	void SetBoxColliderPos(void);
	virtual void Collision(void) = 0;
	virtual void Destroy(void) = 0;

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_PosOld;   // 前回の弾の位置
	D3DXVECTOR3 m_Move;     // 移動量
	int m_nIdx;             // 番号(サーバーで管理するときに必要)
	TYPE m_Type;            // 種類
	CScene *m_pParent;      // 弾を発射したオブジェクトへのポインタ
	CModel *m_pModel;       // モデルクラスへのポインタ
};

//*****************************************************************************
//    プレイヤーの弾クラスの定義
//*****************************************************************************
class CBulletPlayer : public CBullet
{
public:    // 誰でもアクセス可能
	CBulletPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_BULLET);
	~CBulletPlayer();

	static CBulletPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, TYPE type, CScene *pParent = NULL,
		LPD3DXMESH pMesh = NULL, LPD3DXBUFFER pBuffMat = NULL, DWORD nNumMat = 0, LPDIRECT3DTEXTURE9 *pTexture = NULL, int nPriority = 3);
	static int GetNumAll_0(void);
	static int GetNumAll_1(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void Collision(void);
	void Destroy(void);

private:   // このクラスだけがアクセス可能
	static int m_nNumAll_0;
	static int m_nNumAll_1;

	void CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath);
	bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer);
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy);
	bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet);
	bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock);
	bool CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead);
};

//*****************************************************************************
//    敵の弾クラスの定義
//*****************************************************************************
class CBulletEnemy : public CBullet
{
public:    // 誰でもアクセス可能
	CBulletEnemy(int nPriority = 2, OBJTYPE objType = OBJTYPE_BULLET);
	~CBulletEnemy();

	static CBulletEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, CScene *pParent = NULL,
		LPD3DXMESH pMesh = NULL, LPD3DXBUFFER pBuffMat = NULL, DWORD nNumMat = 0, LPDIRECT3DTEXTURE9 *pTexture = NULL, int nPriority = 3);
	static int GetNumAll(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void Collision(void);
	void Destroy(void);

private:   // このクラスだけがアクセス可能
	static int m_nNumAll;

	void CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath);
	bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer);
	bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy);
	bool CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet);
	bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock);
	bool CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead);
};


//*****************************************************************************
//    弾のエフェクトクラスの定義
//*****************************************************************************
class CBulletEffect : public CBullet
{
public:    // 誰でもアクセス可能
	CBulletEffect(int nPriority = 3, OBJTYPE objType = OBJTYPE_BULLETEFFECT);
	~CBulletEffect();

	static CBulletEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, CScene *pParent = NULL,
		LPD3DXMESH pMesh = NULL, LPD3DXBUFFER pBuffMat = NULL, DWORD nNumMat = 0, LPDIRECT3DTEXTURE9 *pTexture = NULL, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void Collision(void);
	void Destroy(void);

private:   // このクラスだけがアクセス可能
};

#endif