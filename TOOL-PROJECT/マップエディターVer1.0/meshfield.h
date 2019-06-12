//=============================================================================
//
// メッシュフィールドの処理 [meshField.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshField : public CScene
{
public:    // 誰でもアクセス可能
	CMeshField(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHFIELD);
	~CMeshField();

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nZBlock, int nTexSplitU, int nTexSplitV, char *pFileName, bool bCulling, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeNormal(void);

	void SetBinaryFileName(char *pFileName);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);
	void SetTexSplitU(const int nTexSplitU);
	void SetTexSplitV(const int nTexSplitV);
	void SetCulling(const bool bCulling);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	char *GetBinaryFileName(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetXBlock(void);
	int GetZBlock(void);
	int GetTexSplitU(void);
	int GetTexSplitV(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	bool GetCulling(void);
	D3DXMATRIX GetMtxWorld(void);

	float GetPolyHeight(D3DXVECTOR3 pos, bool *pLand, D3DXVECTOR3 *pPolyNormal);
	void SetPolyHeight(const D3DXVECTOR3 pos, const float fValue, const float fRange);
	bool CheckPenetration(D3DXVECTOR3 pos, int *pXBlock = NULL, int *pYBlock = NULL, int *pLeftVerIndex = NULL);
	void SaveMesh(void);
	void LoadMesh(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	char                    m_aBinaryFileName[256];   // 頂点情報が格納されたバイナリファイル名
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;               // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	              // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;                    // メッシュフィールドの座標
	D3DXVECTOR3             m_Rot;                    // メッシュフィールドの向き
	D3DXCOLOR               m_Col;                    // メッシュフィールドの色
	float                   m_fWidth;                 // 1ブロック分の幅
	float                   m_fHeight;                // 1ブロック分高さ
	int                     m_nXBlock;                // 横の分割数
	int                     m_nZBlock;                // 奥行の分割数
	int                     m_nTexSplitU;             // テクスチャU座標の分割数
	int                     m_nTexSplitV;             // テクスチャV座標の分割数
	int                     m_nNumVertex;             // 頂点数
	int                     m_nNumIndex;              // インデックス数
	int                     m_nNumPolygon;            // ポリゴン数
	bool                    m_bCulling;               // カリングするかどうか
	D3DXMATRIX              m_MtxWorld;               // ワールドマトリックス
};

#endif