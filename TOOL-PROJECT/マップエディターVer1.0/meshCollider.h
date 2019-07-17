//*****************************************************************************
//
//     メッシュコライダーの処理[meshCollider.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHCOLLIDER_H_
#define _MESHCOLLIDER_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMeshCollider : public CScene
{
public:    // 誰でもアクセス可能
	CMeshCollider(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHFIELD);
	~CMeshCollider();

	static CMeshCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nXBlock, int nZBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetXBlock(void);
	int GetZBlock(void);
	int GetNumVertex(void);
	D3DXMATRIX GetMtxWorld(void);

	float GetPolyHeight(D3DXVECTOR3 pos, bool *pLand, D3DXVECTOR3 *pPolyNormal);
	bool CheckPenetration(D3DXVECTOR3 pos, int *pXBlock = NULL, int *pYBlock = NULL, int *pLeftVerIndex = NULL);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // 頂点バッファへのポインタ
	D3DXVECTOR3             m_Pos;                    // メッシュフィールドの座標
	D3DXVECTOR3             m_Rot;                    // メッシュフィールドの向き
	float                   m_fWidth;                 // 1ブロック分の幅
	float                   m_fHeight;                // 1ブロック分高さ
	int                     m_nXBlock;                // 横の分割数
	int                     m_nZBlock;                // 奥行の分割数
	int                     m_nNumVertex;             // 頂点数
	D3DXMATRIX              m_MtxWorld;               // ワールドマトリックス
};

#endif