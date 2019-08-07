//*****************************************************************************
//
//     モデルの処理[model.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MODEL_H_
#define _MODEL_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    モデルクラスの定義
//*****************************************************************************
class CModel
{
public:    // 誰でもアクセス可能
	CModel();
	~CModel();

	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture = NULL, D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture = NULL, D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void SetAlpha(float fAlpha);

	void BindModel(const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat, const D3DXVECTOR3 VtxMax, const D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture);
	void BindTexture(LPDIRECT3DTEXTURE9 *pTexture);
	void SetMesh(const LPD3DXMESH pMesh);
	void SetBuffMat(const LPD3DXBUFFER pBuffMat);
	void SetNumMat(const DWORD nNumMat);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void SetScale(const D3DXVECTOR3 Scale);
	void SetPos(const D3DXVECTOR3 pos);
	void SetAddPos(const D3DXVECTOR3 Addpos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetParent(CModel *pModel);

	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	D3DXVECTOR3 GetVtxMin(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetScale(void);
	LPDIRECT3DTEXTURE9 *GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetAddPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXMATRIX *GetMtxWorldPointer(void);
	CModel *GetParent(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9    *m_pTexture;  // テクスチャへのポインタ
	LPD3DXMESH            m_pMesh;      // メッシュ情報へのポインタ
	LPD3DXBUFFER          m_pBuffMat;   // マテリアル情報へのポインタ
	DWORD                 m_nNumMat;    // マテリアル情報の数
	D3DXVECTOR3           m_VtxMin;     // 頂点座標の最小値
	D3DXVECTOR3           m_VtxMax;     // 頂点座標の最大値
	D3DXVECTOR3           m_Scale;      // スケールの倍率
	D3DXVECTOR3           m_Pos;        // 現在の座標
	D3DXVECTOR3           m_AddPos;     // オフセットに加える座標
	D3DXVECTOR3           m_Rot;        // 向き
	D3DXMATRIX            m_MtxWorld;   // ワールドマトリックス
	CModel                *m_pParent;   // 親モデルへのポインタ
};

#endif