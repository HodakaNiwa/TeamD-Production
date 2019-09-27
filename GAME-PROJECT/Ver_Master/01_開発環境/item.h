//*****************************************************************************
//
//     アイテムの処理[item.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ITEM_H_
#define _ITEM_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define ITEM_PRIORITY (4)

//*****************************************************************************
//    アイテムクラスの定義
//*****************************************************************************
class CItem : public CObject3D
{
public:    // 誰でもアクセス可能
	//-------------------
	//   状態
	//-------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // 通常
		STATE_DEATH,        // 死亡
		STATE_MAX
	}STATE;

	//-------------------
	//   種類
	//-------------------
	typedef enum
	{
		TYPE_NONE = -1,    // 無し
		TYPE_STAR,         // スター(パワーアップ3段階まで)
		TYPE_GRENADE,      // 手榴弾(敵全滅)
		TYPE_1UP_TANK,     // タンク(残機UP)
		TYPE_SCOOP,        // スコップ(一定時間司令部無敵)
		TYPE_CLOCK,        // 時計(一定時間敵を止める)
		TYPE_HELMET,       // ヘルメット(一定時間無敵)
		TYPE_MAX
	}TYPE;

	CItem(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItem();

	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(LPDIRECT3DDEVICE9 pDevice);
	void CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag = true);
	virtual void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void OnlineUninit(void);

	void SetType(const int nType);
	void SetModelIdx(const int nModelIdx);
	void SetMesh(const LPD3DXMESH pMesh);
	void SetBuffMat(const LPD3DXBUFFER pBuffMat);
	void SetNumMat(const DWORD nNumMat);
	void SetTexture(LPDIRECT3DTEXTURE9 *pTexture);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetAlpha(const float fAlpha);
	void SetIdx(const int nIdx);

	int GetType(void);
	int GetModelIdx(void);
	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	LPDIRECT3DTEXTURE9 *GetTexture(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);
	float GetAlpha(void);
	int GetIdx(void);

	static int GetNumAll(void);
	static void ResetNumAll(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static int m_nNumAll;

	int                 m_nIdx;         // 番号
	int                 m_nType;        // 種類番号
	int                 m_nModelIdx;    // 使用するモデルの番号
	LPD3DXMESH          m_pMesh;        // メッシュへのポインタ
	LPD3DXBUFFER        m_pBuffMat;     // マテリアル情報へのポインタ
	DWORD               m_nNumMat;      // マテリアル情報の数
	LPDIRECT3DTEXTURE9  *m_pTexture;    // テクスチャへのポインタ
	D3DXVECTOR3         m_VtxMax;       // 最大の頂点座標
	D3DXVECTOR3         m_VtxMin;       // 最小の頂点座標
	float               m_fAlpha;       // モデルの透明度
	D3DXVECTOR3         m_Scale;        // 現在のスケール
	float               m_fChangeScale; // スケールの倍率を変える値
	int                 m_nLife;        // 寿命
	STATE               m_State;        // 状態
};

//*****************************************************************************
//    アイテム(スター)クラスの定義
//*****************************************************************************
class CItemStar : public CItem
{
public:    // 誰でもアクセス可能
	CItemStar(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItemStar();

	static CItemStar *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

//*****************************************************************************
//    アイテム(手榴弾)クラスの定義
//*****************************************************************************
class CItemGrenade : public CItem
{
public:    // 誰でもアクセス可能
	CItemGrenade(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItemGrenade();

	static CItemGrenade *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

//*****************************************************************************
//    アイテム(残機UP)クラスの定義
//*****************************************************************************
class CItem1up : public CItem
{
public:    // 誰でもアクセス可能
	CItem1up(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItem1up();

	static CItem1up *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

//*****************************************************************************
//    アイテム(スコップ)クラスの定義
//*****************************************************************************
class CItemScoop : public CItem
{
public:    // 誰でもアクセス可能
	CItemScoop(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItemScoop();

	static CItemScoop *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

//*****************************************************************************
//    アイテム(時計)クラスの定義
//*****************************************************************************
class CItemClock : public CItem
{
public:    // 誰でもアクセス可能
	CItemClock(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItemClock();

	static CItemClock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

//*****************************************************************************
//    アイテム(ヘルメット)クラスの定義
//*****************************************************************************
class CItemHelmet : public CItem
{
public:    // 誰でもアクセス可能
	CItemHelmet(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEM);
	~CItemHelmet();

	static CItemHelmet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

#endif