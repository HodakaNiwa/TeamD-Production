//*****************************************************************************
//
//     カメラ管轄の処理[cameraManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "cameraManager.h"
#include "camera.h"

//=============================================================================
//    コンストラクタ
//=============================================================================
CCameraManager::CCameraManager()
{
	// 各種値のクリア
	m_nNumCamera = 0;     // カメラの数
	m_apCamera = NULL;    // カメラクラスへのポインタ
}

//=============================================================================
//    デスストラクタ
//=============================================================================
CCameraManager::~CCameraManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CCameraManager *CCameraManager::Create(int nNumCamera)
{
	CCameraManager *pCameraManager = NULL;  // カメラ管轄クラス型のポインタ
	if (pCameraManager == NULL)
	{// メモリが空になっている
		pCameraManager = new CCameraManager;
		if (pCameraManager != NULL)
		{// インスタンスを生成できた
			if (FAILED(pCameraManager->Init(nNumCamera)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pCameraManager;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CCameraManager::Init(int nNumCamera)
{
	// カメラの数を設定
	m_nNumCamera = nNumCamera;

	// カメラクラスのポインタを確保する
	if (m_apCamera == NULL)
	{// メモリが確保できる状態である
		m_apCamera = new CCamera*[nNumCamera];
		if (m_apCamera != NULL)
		{// メモリが確保できた
			for (int nCntCamera = 0; nCntCamera < nNumCamera; nCntCamera++)
			{// カメラの数だけ繰り返し
				m_apCamera[nCntCamera] = NULL;
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CCameraManager::Uninit(void)
{
	if (m_apCamera != NULL)
	{// メモリが確保されている
		for (int nCntCamera = 0; nCntCamera < m_nNumCamera; nCntCamera++)
		{// ライトの数だけ繰り返し
			if (m_apCamera[nCntCamera] != NULL)
			{// メモリが確保されている
				m_apCamera[nCntCamera]->Uninit();

				// メモリの開放
				delete m_apCamera[nCntCamera];
				m_apCamera[nCntCamera] = NULL;
			}
		}

		// メモリの開放
		delete m_apCamera;
		m_apCamera = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CCameraManager::Update(void)
{
	if (m_apCamera != NULL)
	{// メモリが確保されている
		for (int nCntCamera = 0; nCntCamera < m_nNumCamera; nCntCamera++)
		{// ライトの数だけ繰り返し
			if (m_apCamera[nCntCamera] != NULL)
			{// メモリが確保されている
				m_apCamera[nCntCamera]->Update();
			}
		}
	}
}

//=============================================================================
//    カメラの設定
//=============================================================================
void CCameraManager::BindCamera(int nIdxCamera)
{
	if (m_apCamera != NULL)
	{// メモリが確保されている
		if (m_apCamera[nIdxCamera] != NULL)
		{// メモリが確保されている
			m_apCamera[nIdxCamera]->SetCamera();
		}
	}
}

//=============================================================================
//    カメラの数を設定
//=============================================================================
void CCameraManager::SetNumCamera(const int nNumCamera)
{
	m_nNumCamera = nNumCamera;
}

//=============================================================================
//    カメラクラスへのポインタを設定
//=============================================================================
void CCameraManager::SetCamera(CCamera *pCamera, const int nIdxCamera)
{
	m_apCamera[nIdxCamera] = pCamera;
}

//=============================================================================
//    カメラの数を取得
//=============================================================================
int CCameraManager::GetNumCamera(void)
{
	return m_nNumCamera;
}

//=============================================================================
//    カメラクラスへのポインタを取得
//=============================================================================
CCamera *CCameraManager::GetCamera(const int nIdxCamera)
{
	return m_apCamera[nIdxCamera];
}