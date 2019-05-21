//=============================================================================
//
// オブジェクトXの処理 [sceneX.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSceneX : public CScene
{
public:	//誰からもアクセス可能
	CSceneX(int nPriority = 3, OBJTYPE objtype = OBJTYPE_SCENEX);
	~CSceneX();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	void SetSize(D3DXVECTOR3 size);
	void SetVtxMax(D3DXVECTOR3 vtxMax);
	void SetVtxMin(D3DXVECTOR3 vtxMin);
	void SetHit(bool bHit);
	void SetDestroy(bool bDestroy);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetSize(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);
	bool GetHit(void);
	bool GetDestroy(void);

	void BindX(LPD3DXMESH mesh, LPD3DXBUFFER buffmat, DWORD nummat);

	//静的メンバ関数
	static CSceneX *Create(void);
private:	//自分だけがアクセス可能
	LPD3DXMESH			m_pMesh;					//メッシュ情報へのポインタ
	LPD3DXBUFFER		m_pBuffMat;					//マテリアル情報へのポインタ
	DWORD				m_nNumMat;					//マテリアル情報の数
	LPDIRECT3DTEXTURE9	*m_pTexture;				//テクスチャのポインタ
	D3DXVECTOR3			m_pos;						//位置
	D3DXVECTOR3			m_rot;						//向き
	D3DXMATRIX			m_mtxWorld;					//ワールドマトリックス
	D3DXVECTOR3			m_vtxMax;					//頂点座標の最大値
	D3DXVECTOR3			m_vtxMin;					//頂点座標の最小値
	D3DXVECTOR3			m_size;						//大きさ
	bool				m_bHit;						//当たり判定
	bool				m_bDestroy;					//破壊できるかどうか
};
#endif
