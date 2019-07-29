//*****************************************************************************
//
//     ビルボードの処理[sceneBillboard.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENEBILLBOARD_H_
#define _SCENEBILLBOARD_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    ビルボードクラスの定義
//*****************************************************************************
class CSceneBillboard : public CScene
{
public:    // 誰でもアクセス可能
	CSceneBillboard(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLBOARD);
	~CSceneBillboard();

	static CSceneBillboard *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, bool bLighting = false, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetRot(const float fRot);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetAngle(const float fAngle);
	void SetLength(const float fLength);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetLighting(const bool bLighting);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetRot(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetAngle(void);
	float GetLength(void);
	D3DXMATRIX GetMtxWorld(void);
	bool GetLighting(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	    // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;          // ポリゴンの座標
	D3DXCOLOR               m_Col;          // ポリゴンの色
	float                   m_fRot;         // ポリゴンの向き
	float                   m_fWidth;       // ポリゴンの幅
	float                   m_fHeight;      // ポリゴンの高さ
	float                   m_fAngle;       // ポリゴンを出す角度
	float                   m_fLength;      // ポリゴンの長さ
	D3DXMATRIX              m_MtxWorld;     // ワールドマトリックス
	bool                    m_bLighting;    // ライティングするかしないか
};

#endif