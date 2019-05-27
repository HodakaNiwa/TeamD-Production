//*****************************************************************************
//
//     ライト管轄の処理[lightManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LIGHTMANAGER_H_
#define _LIGHTMANAGER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CLight;

//*****************************************************************************
//    ライト管轄クラスの定義
//*****************************************************************************
class CLightManager
{
public:    // 誰でもアクセス可能
	CLightManager();
	~CLightManager();

	static CLightManager *Create(int nNumLight = 3);

	HRESULT Init(int nNumLight = 3);
	void Uninit(void);
	void Update(void);
	void SettingLight(CLight *pLight, const int nIdx);
	void SwitchLight(const int nIdx);

	void SetNumLight(const int nNumLight);

	int GetNumLight(void);
	CLight *GetLight(const int nIdx);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int    m_nNumLight;     // ライトの数
	CLight **m_apLight;     // ライトクラスへのポインタ
};

#endif