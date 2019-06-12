//*****************************************************************************
//
//     司令部の処理[headquarters.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _HEADQUARTERS_H_
#define _HEADQUARTERS_H_

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
//    司令部クラスの定義
//*****************************************************************************
class CHeadQuarters : public CObject3D
{
public:    // 誰でもアクセス可能
	CHeadQuarters();
	~CHeadQuarters();

	static CHeadQuarters *Create(int nAreaX, int nAreaZ, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture);

	HRESULT Init(int nAreaX, int nAreaZ, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(CScene *pScene);

	void SetModel(CModel *pModel);
	void SetArea(int nAreaX, int nAreaZ);

	CModel *GetModel(void);
	int GetAreaX(void);
	int GetAreaZ(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int m_nAreaX, m_nAreaZ; // 今どのエリアにいるかを数値化したもの
	CModel *m_pModel;       // モデルクラス型のポインタ
};

#endif