//=============================================================================
//
// ビルボードの処理 [billboard.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
#include "scene.h"
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBillboard : public CScene
{
public:
	//メンバ関数
	CBillboard(int nPriority = 4,OBJTYPE objtype = OBJTYPE_BILLBOARD);
	~CBillboard();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 texture);
	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	void SetWidth(float fWidth);
	void SetHeight(float fHwight);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	//静的メンバ関数
	static CBillboard *Create(void);
private:
	//メンバ変数
	D3DXVECTOR3 m_pos;			//位置
	float		m_fWidth;		//幅
	float		m_fHeight;		//高さ
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX	m_mtxWorld;		//ワールドマトリックス

	//静的メンバ変数
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;	// テクスチャへのポインタ

};
#endif
