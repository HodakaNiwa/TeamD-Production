//*****************************************************************************
//
//     川の処理[river.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RIVER_H_
#define _RIVER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMeshField;

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define RIVER_DOWN_FIELD (-10.0f)   // 川を作る際にメッシュフィールドをどれだけ下げるか

//*****************************************************************************
//    川クラスの定義
//*****************************************************************************
class CRiver : public CObject3D
{
public:    // 誰でもアクセス可能
	CRiver(int nPriority = 3, OBJTYPE objType = OBJTYPE_RIVER);
	~CRiver();

	static CRiver *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTexIdx(const int nTexIdx);
	int GetTexIdx(void);
	CMeshField *GetMeshField(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);

	int        m_nTexIdx;       // 使用しているテクスチャの番号
	CMeshField *m_pMeshField;   // メッシュフィールドクラス型のポインタ
};

#endif