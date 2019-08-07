//*****************************************************************************
//
//     グリッド線の処理[grid.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GRID_H_
#define _GRID_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    グリッド線クラスの定義
//*****************************************************************************
class CGrid
{
public:    // 誰でもアクセス可能
	CGrid();
	~CGrid();

	static CGrid *Create(D3DXVECTOR3 pos, float fWidth, float fDepth, int nXBlock, int nZBlock);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(const D3DXVECTOR3 pos);
	void SetWidth(const float fWidth);
	void SetDepth(const float fDepth);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);

	D3DXVECTOR3 GetPos(void);
	float GetWidth(void);
	float GetDepth(void);
	int GetXBlock(void);
	int GetZBlock(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void MakeVertex(void);

	D3DXVECTOR3             m_Pos;          // 座標
	float                   m_fWidth;       // 1マス分の横幅
	float                   m_fDepth;       // 1マス分の奥行
	int                     m_nXBlock;      // 横の分割数
	int                     m_nZBlock;      // 奥行の分割数
	D3DXVECTOR3             *m_pVecPos;     // 頂点座標
	int                     *m_pIndex;      // 頂点番号
	ID3DXLine*              m_pLine;        // 線描画用インスタンスへのポインタ
	int                     m_nNumVertex;   // 頂点座標の数
	int                     m_nNumIdx;      // インデックス番号数
};

#endif