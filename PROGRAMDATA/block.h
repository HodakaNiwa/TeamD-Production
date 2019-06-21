//*****************************************************************************
//
//     ブロックの処理[block.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    ブロッククラスの定義
//*****************************************************************************
class CBlock : public CObject3D
{
public:    // 誰でもアクセス可能
	//-------------------
	//   種類
	//-------------------
	typedef enum
	{
		TYPE_NONE = -1,               // 無し
		TYPE_BREAK_BULLET_NOT_DIRTY,  // 弾で壊せて汚れない
		TYPE_BREAK_TANK_DIRTY,        // タンクで壊せて汚れる
		TYPE_NOT_BREAK_NOT_DIRTY,     // 壊せないし汚れない
		TYPE_NOT_BREAK_TANK_DIRTY,    // 壊せないがタンクが汚れる
		TYPE_MAX
	}TYPE;

	CBlock(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlock();

	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag = true);
	virtual void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	void SetType(const int nType);
	void SetModelIdx(const int nModelIdx);
	void SetMesh(const LPD3DXMESH pMesh);
	void SetBuffMat(const LPD3DXBUFFER pBuffMat);
	void SetNumMat(const DWORD nNumMat);
	void SetTexture(LPDIRECT3DTEXTURE9 *pTexture);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetAlpha(const float fAlpha);

	int GetType(void);
	int GetModelIdx(void);
	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	LPDIRECT3DTEXTURE9 *GetTexture(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);
	float GetAlpha(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int                 m_nType;        // 種類番号
	int                 m_nModelIdx;    // 使用するモデルの番号
	LPD3DXMESH          m_pMesh;        // メッシュへのポインタ
	LPD3DXBUFFER        m_pBuffMat;     // マテリアル情報へのポインタ
	DWORD               m_nNumMat;      // マテリアル情報の数
	LPDIRECT3DTEXTURE9  *m_pTexture;    // テクスチャへのポインタ
	D3DXVECTOR3         m_VtxMax;       // 最大の頂点座標
	D3DXVECTOR3         m_VtxMin;       // 最小の頂点座標
	float               m_fAlpha;       // モデルの透明度
};

//*****************************************************************************
//    タイプ0のブロッククラスの定義
//*****************************************************************************
class CBlockType0 : public CBlock
{
public:    // 誰でもアクセス可能
	CBlockType0(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlockType0();

	static CBlockType0 *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

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
//    タイプ1のブロッククラスの定義
//*****************************************************************************
class CBlockType1 : public CBlock
{
public:    // 誰でもアクセス可能
	CBlockType1(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlockType1();

	static CBlockType1 *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateSplash(void);
};

//*****************************************************************************
//    タイプ2のブロッククラスの定義
//*****************************************************************************
class CBlockType2 : public CBlock
{
public:    // 誰でもアクセス可能
	CBlockType2(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlockType2();

	static CBlockType2 *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

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
//    タイプ3のブロッククラスの定義
//*****************************************************************************
class CBlockType3 : public CBlock
{
public:    // 誰でもアクセス可能
	CBlockType3(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlockType3();

	static CBlockType3 *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateSplash(void);
};

#endif