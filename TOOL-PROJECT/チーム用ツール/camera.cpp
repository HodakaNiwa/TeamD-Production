//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_MOVE (3.0f);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// カメラコンストラクタ
//=============================================================================
CCamera::CCamera()
{

}

//=============================================================================
// カメラデストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// カメラの生成
//=============================================================================
CCamera *CCamera::Create()
{
	CCamera *pCamera = NULL;

	if (pCamera == NULL)
	{
		pCamera = new CCamera;
		pCamera->Init();
	}
	return pCamera;
}

//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 100.0f, -150.0f);						//視点
	m_posR = D3DXVECTOR3(0.0f, 50.0f, 0.0f);								//注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);								//上方向ベクトル
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//目的の視点
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							//目的の注視点
	m_rot.y =atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z) ;			//向き
	m_rotDest = m_rot;													//目的の向き
	m_rotDiff = 0.0f;
	m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));	//距離

	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	if (pInputKeyboard->GetPress(DIK_LEFT) == true)
	{// キーボードの[A]キーが押された
		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{// キーボードの[W]キーが押された(左上)
			m_posV.x += -sinf(m_rot.y + D3DX_PI * 0.75f) * CAMERA_MOVE;
			m_posV.z += -cosf(m_rot.y + D3DX_PI * 0.75f) * CAMERA_MOVE;
			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
		{// キーボードの[S]キーが押された(左下)
			m_posV.x += -sinf(m_rot.y + D3DX_PI * 0.25f) * CAMERA_MOVE;
			m_posV.z += -cosf(m_rot.y + D3DX_PI * 0.25f) * CAMERA_MOVE;
			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else
		{// キーボードの[A]キーが押された(左)
			m_posV.x += -sinf(m_rot.y + D3DX_PI / 2) * CAMERA_MOVE;
			m_posV.z += -cosf(m_rot.y + D3DX_PI / 2) * CAMERA_MOVE;
			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
	}

	else if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{// キーボードの[D]キーが押された(左)
		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{// キーボードの[W]キーが押された(左上)
			m_posV.x += -sinf(m_rot.y + D3DX_PI * -0.75f) * CAMERA_MOVE;
			m_posV.z += -cosf(m_rot.y + D3DX_PI * -0.75f) * CAMERA_MOVE;
			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
		{// キーボードの[S]キーが押された(左下)
			m_posV.x += -sinf(m_rot.y + D3DX_PI * -0.25f) * CAMERA_MOVE;
			m_posV.z += -cosf(m_rot.y + D3DX_PI * -0.25f) * CAMERA_MOVE;
			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
		else
		{// キーボードの[D]キーが押された(左)
			m_posV.x += sinf(m_rot.y + D3DX_PI / 2) * CAMERA_MOVE;
			m_posV.z += cosf(m_rot.y + D3DX_PI / 2) * CAMERA_MOVE;
			m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
			m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_UP) == true)
	{// キーボードの[W]キーが押された(前)
		m_posV.x += sinf(m_rot.y + D3DX_PI * 0.0f) * CAMERA_MOVE;
		m_posV.z += cosf(m_rot.y + D3DX_PI * 0.0f) * CAMERA_MOVE;
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}
	else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
	{// キーボードの[S]キーが押された(後ろ)
		m_posV.x += sinf(m_rot.y + D3DX_PI * 1.0f) * CAMERA_MOVE;
		m_posV.z += cosf(m_rot.y + D3DX_PI * 1.0f) * CAMERA_MOVE;
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}
	//===================================================
	//　カメラの注視点処理
	//===================================================
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{// キーボードの[T]キーが押された
		m_posR.y += 2.0f;
	}
	if (pInputKeyboard->GetPress(DIK_B) == true)
	{// キーボードの[B]キーが押された
		m_posR.y -= 2.0f;
	}
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{// キーボードの[Q]キーが押された（左回りに旋回）
		m_rot.y -= 0.025f;
		if (m_rot.y <= -D3DX_PI)
		{
			m_rot.y = D3DX_PI;
		}
		m_posR.x = m_posV.x + sinf(m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}
	if (pInputKeyboard->GetPress(DIK_E) == true)
	{// キーボードの[E]キーが押された
		m_rot.y += 0.025f;
		if (m_rot.y >= D3DX_PI)
		{
			m_rot.y = -D3DX_PI;
		}
		m_posR.x = m_posV.x + sinf( m_rot.y) * m_fLength;
		m_posR.z = m_posV.z + cosf(m_rot.y) * m_fLength;
	}

	//===================================================
	//　カメラの視点処理
	//===================================================
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{// キーボードの[Y]キーが押された
		m_posV.y += 2.0f;
	}
	if (pInputKeyboard->GetPress(DIK_N) == true)
	{// キーボードの[N]キーが押された
		m_posV.y -= 2.0f;
	}
	if (pInputKeyboard->GetPress(DIK_Z) == true)
	{// キーボードの[Z]キーが押された
		m_rot.y += 0.025f;
		if (m_rot.y >= D3DX_PI)
		{
			m_rot.y = -D3DX_PI;
		}
		m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
		m_posV.z = m_posR.z - cosf(m_rot.y) *m_fLength;
	}
	if (pInputKeyboard->GetPress(DIK_C) == true)
	{// キーボードの[C]キーが押された
		m_rot.y -= 0.025f;
		if (m_rot.y <= -D3DX_PI)
		{
			m_rot.y = D3DX_PI;
		}
		m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;
		m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;
	}

#ifdef _DEBUG
	//注視点の表示
	CDebugProc::Print(1, "視点の移動    : x[%.1f],y[%.1f],z[%.1f]\n", m_posV.x, m_posV.y, m_posV.z);
	//視点の表示
	CDebugProc::Print(1, "注視点の移動  : x[%.1f],y[%.1f],z[%.1f]\n", m_posR.x, m_posR.y, m_posR.z);
#endif
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void CCamera::SetCamera(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),						//画角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//画面比率
		10.0f,										//手前
		2500.0f);									//奥行き	変更中元1000

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
				       &m_posV,
					   &m_posR,
					   &m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}
//=============================================================================
// カメラの視点取得
//=============================================================================
D3DXVECTOR3 CCamera::GetPosV(void)
{
	return m_posV;
}

//=============================================================================
// カメラの注視点取得
//=============================================================================
D3DXVECTOR3 CCamera::GetPosR(void)
{
	return m_posR;
}

//=============================================================================
// カメラの向き取得
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// カメラの視点設置処理
//=============================================================================
void CCamera::SetPosV(D3DXVECTOR3 posV)
{
	m_posV = posV;
}

//=============================================================================
// カメラの注視点設置処理
//=============================================================================
void CCamera::SetPosR(D3DXVECTOR3 posR)
{
	m_posR = posR;
}
