//=============================================================================
//
// メッシュフィールドの処理 [meshField.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include "scene.h"

#define MESH_X (10)		//x
#define MESH_Z (10)		//z

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshField : public CScene
{
public:	//誰からもアクセス可能
	//メンバ関数
	CMeshField(int nPriority = 1, OBJTYPE objtype = OBJTYPE_MESHFIELD);
	~CMeshField();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	float GetHeight(D3DXVECTOR3 pos);
	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);
	//静的メンバ関数
	static CMeshField *Create(D3DXVECTOR3 pos);

private:	//自分だけがアクセス可能
	//メンバ変数
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;			// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;			// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff = NULL;			//インデックスバッファへのポインタ
	D3DXMATRIX					m_mtxWorld;					//ワールドマトリックス（行列)

	D3DXVECTOR3					m_pos;						//位置
	D3DXVECTOR3					m_vecA,m_vecC;				//ベクトル
	D3DXVECTOR3					m_aNor[10000];				//法線
	D3DXCOLOR					m_col;						//色
	D3DXVECTOR3					m_rot;						//向き
	int							m_nNumVartex;				//頂点数
	int							m_nNumIdx;					//インデックス数
	int							m_nNumPolygon;				//ポリゴン数
	int							m_aVartex;					//頂点

};
#endif