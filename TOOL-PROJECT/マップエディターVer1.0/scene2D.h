//=============================================================================
//
// オブジェクト2D処理 [scene2D.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene2D : public CScene
{
public:	//誰からもアクセス可能
	//メンバ関数
	CScene2D(int nPriority = 5,OBJTYPE objtype = OBJTYPE_SCENE2D);
	~CScene2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetWidth(float width);
	void SetHeight(float height);
	void SetVtxPos(D3DXVECTOR3 pos);
	void SetVtxLenghtPos(D3DXVECTOR3 pos);
	void SetVtxRotPos(D3DXVECTOR3 pos);
	void SetLength(float fLength);
	float GetWidth(void);
	float GetHeight(void);
	float GetfLength(void);
	void SetCol(D3DXCOLOR col);
	void BindTexture(LPDIRECT3DTEXTURE9 texture);
	LPDIRECT3DVERTEXBUFFER9 GetVtx(void);

	//静的メンバ関数
	static CScene2D *Create(D3DXVECTOR3 pos, float fWight, float fHeight);
private:
	//メンバ変数
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;				// 位置
	D3DXCOLOR					m_col;				//色
	float						m_rot;				//向き
	float						m_fAddLength;		//加算距離
	float						m_fLength;			//距離
	float						m_fWidth;			//幅
	float						m_fHeight;			//高さ

};
#endif