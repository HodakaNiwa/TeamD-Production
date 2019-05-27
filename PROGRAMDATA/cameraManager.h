//*****************************************************************************
//
//     カメラ管轄の処理[cameraManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CAMERAMANAGER_H_
#define _CAMERAMANAGER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CCamera;

//*****************************************************************************
//    カメラ管轄クラスの定義
//*****************************************************************************
class CCameraManager
{
public:    // 誰でもアクセス可能
	CCameraManager();
	~CCameraManager();

	static CCameraManager *Create(int nNumCamera = 1);

	HRESULT Init(int nNumCamera = 1);
	void Uninit(void);
	void Update(void);
	void SetCamera(int nIdxCamera = 0);

	void SetNumCamera(const int nNumCamera);
	void SetCamera(CCamera *pCamera, const int nIdxCamera = 0);

	int GetNumCamera(void);
	CCamera *GetCamera(const int nIdxCamera = 0);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int m_nNumCamera;        // カメラの数
	CCamera **m_apCamera;    // カメラクラスへのポインタ
};

#endif