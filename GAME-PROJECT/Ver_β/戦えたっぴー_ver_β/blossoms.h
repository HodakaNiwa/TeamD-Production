//=============================================================================
//
// 花びらの処理 [blossoms.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _BLOSSOMS_H_
#define _BLOSSOMS_H_

#include "main.h"
#include "scene2D.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBlossoms : public CScene2D
{
public:    // 誰でもアクセス可能
		   //メンバ関数
	CBlossoms(int nPriority = 3, OBJTYPE objtype = OBJTYPE_BLOSSOMS);
	~CBlossoms();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(const D3DXVECTOR3 move);
	void SetAngleSpeed(const float fAngleSpeed);
	void SetRotSpeed(const float fRotSpeed);

	D3DXVECTOR3 GetMove(void);
	float GetAngleSpeed(void);
	float GetRotSpeed(void);

	// 静的メンバ関数
	static CBlossoms *Create(D3DXVECTOR3 move, float fAngleSpeed, float fRotSpeed, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	bool CheckOutOfScreen(D3DXVECTOR3 pos);

	D3DXVECTOR3 m_Move;     // 移動量
	float m_fAngleSpeed;    // 角度を回転させるスピード
	float m_fRotSpeed;      // 向きを回転させるスピード
};

#endif