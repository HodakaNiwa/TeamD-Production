//=============================================================================
//
// オブジェクト3D処理 [scene3D.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene3D : public CScene
{
public:	//誰からもアクセス可能
	//メンバ関数
	CScene3D(int nPriority = 2, OBJTYPE objtype = OBJTYPE_SCENE3D);
	~CScene3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetNor(void);
	float GetHeight(D3DXVECTOR3 pos,bool bR);
	float GetSizeX(void);
	float GetSizeZ(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetSizeX(float sizeX);
	void SetSizeZ(float sizeZ);
	void BindTexture(LPDIRECT3DTEXTURE9 texture);

	//静的メンバ関数
	static CScene3D *Create(void);

private:	//自分だけがアクセス可能
	//メンバ関数
	void SetNor(void);
	void MovePos(void);
	//メンバ変数
	D3DXVECTOR3					m_pos;								//位置
	D3DXVECTOR3					m_rot;								//向き
	float						m_fSizeX;							//Xサイズ
	float						m_fSizeZ;							//Zサイズ
	D3DXMATRIX					m_mtxWorld;							//マトリックス
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;					// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;					// テクスチャへのポインタ
	D3DXVECTOR3					m_vec0, m_vec1;						//ベクトル
	D3DXVECTOR3					m_nor0, m_nor1;						//法線
	D3DXVECTOR3					m_dData;							//データ格納
	//静的メンバ変数
};
#endif
