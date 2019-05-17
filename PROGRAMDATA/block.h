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
	CBlock(int nPriority = 3, OBJTYPE objType = OBJTYPE_3D);
	~CBlock();

	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax = INITIALIZE_D3DXVECTOR3, D3DXVECTOR3 VtxMin = INITIALIZE_D3DXVECTOR3);

	void SetMesh(const LPD3DXMESH pMesh);
	void SetBuffMat(const LPD3DXBUFFER pBuffMat);
	void SetNumMat(const DWORD nNumMat);
	void SetTexture(LPDIRECT3DTEXTURE9 *pTexture);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetAlpha(const float fAlpha);

	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	LPDIRECT3DTEXTURE9 *GetTexture(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);
	float GetAlpha(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPD3DXMESH          m_pMesh;        // メッシュへのポインタ
	LPD3DXBUFFER        m_pBuffMat;     // マテリアル情報へのポインタ
 	DWORD               m_nNumMat;      // マテリアル情報の数
	LPDIRECT3DTEXTURE9  *m_pTexture;    // テクスチャへのポインタ
	D3DXVECTOR3         m_VtxMax;       // 最大の頂点座標
	D3DXVECTOR3         m_VtxMin;       // 最小の頂点座標
	float               m_fAlpha;       // モデルの透明度
};

#endif