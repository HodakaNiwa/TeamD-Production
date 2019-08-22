//*****************************************************************************
//
//     ひなあられの処理[hinaarare.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _HINAARARE_H_
#define _HINAARARE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
//    ひなあられクラスの定義
//*****************************************************************************
class CHinaarare : public CObject3D
{
public:    // 誰でもアクセス可能
	CHinaarare(int nPriority = 3, OBJTYPE objType = OBJTYPE_HINAARARE);
	~CHinaarare();

	static CHinaarare *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(LPDIRECT3DDEVICE9 pDevice);
	void CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag = true);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void Hit(CScene *pScene);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void SetEffect(D3DXVECTOR3 pos);

	int m_nLife;          // 体力
	CModel *m_pModel;     // モデルクラスへのポインタ
};

#endif