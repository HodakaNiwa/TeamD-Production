//*****************************************************************************
//
//     ゴール用の円筒処理[goalCylinder.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GOALCYLINDER_H_
#define _GOALCYLINDER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "meshCylinder.h"

//*****************************************************************************
//    ゴール用の円筒クラスの定義
//*****************************************************************************
class CGoalCylinder : public CMeshCylinder
{
public:    // 誰でもアクセス可能
	CGoalCylinder(int nPriority = 3, OBJTYPE objType = OBJTYPE_GOALCYLINDER);
	~CGoalCylinder();
	static CGoalCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, float fDecay, float fMag, float fUpValue, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, float fDecay, float fMag, float fUpValue);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos);

	void SetDecay(const float fDecay);
	void SetMag(const float fMag);
	void SetUpValue(const float fUpValue);

	float GetDecay(void);
	float GetMag(void);
	float GetUpValue(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fDecay;     // 透明度の減衰値
	float m_fMag;       // 現在の倍率
	float m_fUpValue;   // 倍率を加算する量
};

#endif