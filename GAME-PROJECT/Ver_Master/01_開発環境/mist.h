//=============================================================================
//
// クリーム靄の処理 [mist.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _MIST_H_
#define _MIST_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCreamMist : public CScene2D
{
public:    // 誰でもアクセス可能
	//メンバ関数
	CCreamMist(int nPriority = 3, OBJTYPE objtype = OBJTYPE_MIST);
	~CCreamMist();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CCreamMist *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

#endif