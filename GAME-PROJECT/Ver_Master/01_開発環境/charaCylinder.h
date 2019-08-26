//*****************************************************************************
//
//     キャラセレクト用の円筒処理[charaCylinder.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CHARACYLINDER_H_
#define _CHARACYLINDER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "meshCylinder.h"

//*****************************************************************************
//    キャラセレクト用の円筒クラスの定義
//*****************************************************************************
class CCharaCylinder : public CMeshCylinder
{
public:    // 誰でもアクセス可能
	CCharaCylinder(int nPriority = 3, OBJTYPE objType = OBJTYPE_CHARACYLINDER);
	~CCharaCylinder();
	static CCharaCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, float fDecay, float fMag, float fUpValue, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, float fDecay, float fMag, float fUpValue);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

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