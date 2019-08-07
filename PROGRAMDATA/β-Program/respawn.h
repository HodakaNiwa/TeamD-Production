//*****************************************************************************
//
//     リスポーンの処理[respawn.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RESPAWN_H_
#define _RESPAWN_H_

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
//    リスポーンクラスの定義
//*****************************************************************************
class CRespawn : public CObject3D
{
public:    // 誰でもアクセス可能
	CRespawn();
	~CRespawn();

	static CRespawn *Create(int nAreaX, int nAreaZ);

	HRESULT Init(int nAreaX, int nAreaZ);
	void Uninit(void);
	void Update(void);
	void Draw(void);

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