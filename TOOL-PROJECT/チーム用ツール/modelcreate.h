//=============================================================================
//
// モデル生成処理 [modelcreate.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _MODELCREATE_H_
#define _MODELCREATE_H_

#include "main.h"
#include "sceneX.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CModelCreate
{
public:    //誰からもアクセス可能
	//メンバ関数
	CModelCreate();
	~CModelCreate();
	HRESULT Init(void);
	void Uninit(void);

	void SetNumModel(int nNumModel);
	void SetMesh(LPD3DXMESH pMesh, int nIdx);
	void SetBuffMat(LPD3DXBUFFER pBuffMat, int nIdx);
	void SetNumMat(DWORD nNumMat, int nIdx);
	void SetVtxMax(D3DXVECTOR3 nNumMat, int nIdx);
	void SetVtxMin(D3DXVECTOR3 nNumMat, int nIdx);
	LPD3DXMESH GetMesh(int nIdx);
	LPD3DXBUFFER GetBuffMat(int nIdx);
	DWORD GetNumMat(int nIdx);
	D3DXVECTOR3 GetVtxMax(int nIdx);
	D3DXVECTOR3 GetVtxMin(int nIdx);
	int			GetNumModel(void);
	//静的メンバ変数
	static CModelCreate *Create(int nNumModel);

private:   //自分だけがアクセス可能
	//メンバ変数
	int                   m_nNumModel;    // モデル総数
	LPD3DXMESH            *m_pMesh;       // メッシュのポインタ
	LPD3DXBUFFER          *m_pBuffMat;    // マテリアルのポインタ
	DWORD                 *m_pNumMat;     // マテリアル総数のポインタ
	D3DXVECTOR3           *m_pVtxMax;     // 頂点座標最大値のポインタ
	D3DXVECTOR3           *m_pVtxMin;     // 頂点座標最小値のポインタ
};

#endif