//=============================================================================
//
// オブジェクト2D処理 [scene2D.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
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
public:    // 誰でもアクセス可能
	//メンバ関数
	CScene2D(int nPriority = 3, OBJTYPE objtype = OBJTYPE_SCENE2D);
	~CScene2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(LPDIRECT3DDEVICE9 pDevice);
	void SetScale(const float fWidth, const float fHeight);
	void SetVtxBuffValue(void);
	void SetVtxBuffPos(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffTex(void);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetRot(const float fRot);
	void SetAngle(const float fAngle);
	void SetLength(const float fLength);
	void SetStartTexU(const float fStartTexU);
	void SetStartTexV(const float fStartTexV);
	void SetAddTexU(const float fAddTexU);
	void SetAddTexV(const float fAddTexV);

	LPDIRECT3DTEXTURE9 GetTexture(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetRot(void);
	float GetAngle(void);
	float GetLength(void);
	float GetStartTexU(void);
	float GetStartTexV(void);
	float GetAddTexU(void);
	float GetAddTexV(void);

	// 静的メンバ関数
	static CScene2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;				// ポリゴンの位置
	D3DXCOLOR					m_col;				// ポリゴンの色
	float						m_fWidth;			// ポリゴンの幅
	float						m_fHeight;			// ポリゴンの高さ
	float						m_fRot;				// ポリゴンの向き
	float                       m_fAngle;           // ベクトルの角度
	float						m_fLength;			// ベクトルの長さ
	float                       m_fStartTexU;       // テクスチャU座標(始点)
	float                       m_fStartTexV;       // テクスチャV座標(始点)
	float                       m_fAddTexU;         // テクスチャU座標に加える値
	float                       m_fAddTexV;         // テクスチャV座標に加える値
};
#endif