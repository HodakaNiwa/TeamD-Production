//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Hodaka Niwa
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
#define CAMERA_ANGEL_VIEW      (45.0f)                           // カメラの画角
#define CAMERA_ANGLE_SPEED     (0.01f)                           // カメラをマウスで回転させるスピード倍率
#define CAMERA_MOVE_SPEED      (1.3f)                            // カメラをマウスで移動させるスピード倍率
#define CAMERA_LENGTH_MIN      (100.0f)                          // 視点注視点の距離の最小値
#define CAMERA_LENGTH_MAX      (2000.0f)                         // 視点注視点の距離の最大値

#define CAMERA_LENGTH_INI      (1700.0f)                         // 視点注視点間の距離(初期値)
#define CAMERA_ROT_INI         (D3DXVECTOR3(-1.2f,0.0f,0.0f))    // カメラの向き(初期値)
#define TOPVIEW_CAMERA_ROT_INI (D3DXVECTOR3(-1.57f,0.0f,0.0f))   // トップビューカメラの向き(初期値)
#define CAMERA_MOVE            (3.0f)                            // カメラの動くスピード
#define CAMERA_POS_X           (175.0f)

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************

//*****************************************************************************
// CCameraの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CCamera::CCamera()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CCamera *CCamera::Create(void)
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
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	// カメラの初期情報を設定
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);          // カメラの上方向ベクトル
	m_posR = D3DXVECTOR3(CAMERA_POS_X, 0.0f, 0.0f);  // カメラの注視点
	m_rot = CAMERA_ROT_INI;                          // カメラの現在の向き
	m_rotDest = m_rot;                               // カメラの目的の向き
	m_rotDiff = INITIALIZE_D3DXVECTOR3;              // 現在の向きと目的の向きの差分
	m_fLength = CAMERA_LENGTH_INI;                   // 視点注視点間の距離
	m_bMouseMove = true;                             // マウスで動かせるかどうか
	m_bChengeLength = true;                          // 視点注視点間の距離を変更できるかどうか

	// カメラの視点の位置を計算
	m_posV = D3DXVECTOR3(CAMERA_POS_X, 0.0f, 0.0f);
	m_posV.y = m_posR.y - sinf(m_rot.x) * m_fLength;
	m_posV.z = m_posR.z - cosf(m_rot.x) * m_fLength;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
#ifdef _DEBUG
	//注視点の表示
	CDebugProc::Print(1, "視点の移動    : x[%.1f],y[%.1f],z[%.1f]\n", m_posV.x, m_posV.y, m_posV.z);
	//視点の表示
	CDebugProc::Print(1, "注視点の移動  : x[%.1f],y[%.1f],z[%.1f]\n", m_posR.x, m_posR.y, m_posR.z);
#endif
}

//=============================================================================
// カメラのリセット処理
//=============================================================================
void CCamera::Reset(void)
{
	m_rot = CAMERA_ROT_INI;
	m_fLength = CAMERA_LENGTH_INI;

	// 視点注視点の位置を設定
	m_posR = INITIALIZE_D3DXVECTOR3;
	m_posV.y = m_posR.y - sinf(m_rot.x) * m_fLength;
	m_posV.z = m_posR.z - cosf(m_rot.x) * m_fLength;
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
		20000.0f);									//奥行き

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
// カメラの長さ取得
//=============================================================================
float CCamera::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
// カメラをマウスで動かせるかどうか取得
//=============================================================================
bool CCamera::GetMouseMove(void)
{
	return m_bMouseMove;
}

//=============================================================================
// カメラの視点注視点間の距離を変更できるかどうか取得
//=============================================================================
bool CCamera::GetChengeLength(void)
{
	return m_bChengeLength;
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

//=============================================================================
// カメラの向き設定処理
//=============================================================================
void CCamera::SetRot(D3DXVECTOR3 Rot)
{
	m_rot = Rot;
}

//=============================================================================
// カメラの視点注視点間の距離設定処理
//=============================================================================
void CCamera::SetLength(float fLength)
{
	m_fLength = fLength;
}

//=============================================================================
// カメラをマウスで動かせるかどうか設定
//=============================================================================
void CCamera::SetMouseMove(bool bMouseMove)
{
	m_bMouseMove = bMouseMove;
}

//=============================================================================
// カメラの視点注視点間の距離を変更できるかどうか設定
//=============================================================================
void CCamera::SetChengeLength(bool bChengeLength)
{
	m_bChengeLength = bChengeLength;
}


//*****************************************************************************
// CEditorCameraの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CEditorCamera::CEditorCamera()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CEditorCamera::~CEditorCamera()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CEditorCamera *CEditorCamera::Create(void)
{
	CEditorCamera *pEditCamera = NULL;

	if (pEditCamera == NULL)
	{
		pEditCamera = new CEditorCamera;
		pEditCamera->Init();
	}
	return pEditCamera;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEditorCamera::Init(void)
{
	CCamera::Init();
	Reset();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEditorCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEditorCamera::Update(void)
{
	// マウスのカメラ操作
	if (GetMouseMove() == true)
	{
		MouseMove();
	}
}

//=============================================================================
// エディットカメラのリセット処理
//=============================================================================
void CEditorCamera::Reset(void)
{
	// 値の取得
	float fLength = GetLength();
	D3DXVECTOR3 Rot = GetRot();
	D3DXVECTOR3 PosV = GetPosV();
	D3DXVECTOR3 PosR = GetPosR();

	Rot = CAMERA_ROT_INI;
	fLength = CAMERA_LENGTH_INI;

	// 視点注視点の位置を設定
	PosR = INITIALIZE_D3DXVECTOR3;
	PosV.x = PosR.x - sinf(Rot.y) * fLength;
	PosV.y = PosR.y + cosf(Rot.z) * fLength;
	PosV.z = PosR.z - cosf(Rot.y) * fLength;

	// 値の設定
	SetLength(fLength);
	SetRot(Rot);
	SetPosV(PosV);
	SetPosR(PosR);
}

//=============================================================================
// マウスのカメラ操作処理
//=============================================================================
void CEditorCamera::MouseMove(void)
{
	CInputMouse *pMouse = CManager::GetMouse();
	float fLength = GetLength();
	D3DXVECTOR3 Rot = GetRot();
	D3DXVECTOR3 PosV = GetPosV();
	D3DXVECTOR3 PosR = GetPosR();
	if (pMouse != NULL)
	{// マウスが取得できた
		if (pMouse->GetMouseAxisZ() != 0 && GetChengeLength() == true)
		{// マウスのホイールボタンがスクロールされた
		    fLength += 0.1f * pMouse->GetMouseAxisZ();
			if (fLength < CAMERA_LENGTH_MIN)
			{// 距離が一定の値より近くなった
				fLength = CAMERA_LENGTH_MIN;
			}
			else if (fLength > CAMERA_LENGTH_MAX)
			{// 距離が一定の値より遠くなった
				fLength = CAMERA_LENGTH_MAX;
			}

			// 視点の計算
			PosV.x = PosR.x - sinf(Rot.y) * fLength;
			PosV.y = PosR.y + cosf(Rot.z) * fLength;
			PosV.z = PosR.z - cosf(Rot.y) * fLength;
		}

		if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
		{// マウスの左ボタンと右ボタンが同時に押された
			if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
			{// マウスが動かされている
			 // 同時に押されたときの位置から今のマウスカーソルの位置から視点注視点を移動させる向きを計算
				float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
				fAngle -= Rot.y;
				if (fAngle < -D3DX_PI)
				{// 向きが円周率を超えた
					fAngle += D3DX_PI * 2.0f;
				}
				if (fAngle > D3DX_PI)
				{// 向きが円周率を超えた
					fAngle -= D3DX_PI * 2.0f;
				}

				// マウスのX軸,Y軸から移動量を計算
				float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
				fMovement *= CAMERA_MOVE_SPEED;

				// 注視点を移動させる
				PosR.x += sinf(fAngle) * fMovement;
				PosR.z -= cosf(fAngle) * fMovement;

				// 視点を移動させる
				PosV.x = PosR.x - sinf(Rot.y) * fLength;
				PosV.z = PosR.z - cosf(Rot.y) * fLength;
			}
		}
		else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && CManager::GetKeyboard()->GetPress(DIK_LALT) == false)
		{// マウスの左クリックのみが押された
		    // 向きをマウスの速度に合わせて回転させる
		    // 縦軸
			Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
			if (Rot.y > D3DX_PI)
			{// 向きが円周率を超えた
				Rot.y -= D3DX_PI * 2.0f;
			}
			if (Rot.y < -D3DX_PI)
			{// 向きが円周率を超えた
				Rot.y += D3DX_PI * 2.0f;
			}

			// 横軸
			Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
			if (Rot.z < -D3DX_PI)
			{// 向きが円周率を超えた
				Rot.z = -D3DX_PI;
			}
			if (Rot.z > 0.0f)
			{// 向きが円周率を超えた
				Rot.z = 0.0f;
			}

			// 視点の位置を計算する
			PosV.x = PosR.x - sinf(Rot.y) * fLength;
			PosV.y = PosR.y + cosf(Rot.z) * fLength;
			PosV.z = PosR.z - cosf(Rot.y) * fLength;
		}
		else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
		{// マウスの右クリックのみが押された
		    // 向きをマウスの速度に合わせて回転させる
		    // 縦軸
			Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
			if (Rot.y > D3DX_PI)
			{// 向きが円周率を超えた
				Rot.y -= D3DX_PI * 2.0f;
			}
			if (Rot.y < -D3DX_PI)
			{// 向きが円周率を超えた
				Rot.y += D3DX_PI * 2.0f;
			}

			// 横軸
			Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
			if (Rot.z < -D3DX_PI)
			{// 向きが円周率を超えた
				Rot.z = -D3DX_PI;
			}
			if (Rot.z > 0.0f)
			{// 向きが円周率を超えた
				Rot.z = 0.0f;
			}

			// 注視点の位置を計算する
			PosR.x = PosV.x + sinf(Rot.y) * fLength;
			PosR.y = PosV.y - cosf(Rot.z) * fLength;
			PosR.z = PosV.z + cosf(Rot.y) * fLength;
		}
	}

	// 値の設定
	SetLength(fLength);
	SetRot(Rot);
	SetPosV(PosV);
	SetPosR(PosR);
}

//*****************************************************************************
// CTopViewCameraの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CTopViewCamera::CTopViewCamera()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTopViewCamera::~CTopViewCamera()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CTopViewCamera *CTopViewCamera::Create(void)
{
	CTopViewCamera *pTopViewCamera = NULL;

	if (pTopViewCamera == NULL)
	{
		pTopViewCamera = new CTopViewCamera;
		pTopViewCamera->Init();
	}
	return pTopViewCamera;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTopViewCamera::Init(void)
{
	CCamera::Init();
	Reset();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTopViewCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTopViewCamera::Update(void)
{

}

//=============================================================================
// リセット処理
//=============================================================================
void CTopViewCamera::Reset(void)
{
		// 値の取得
	float fLength = GetLength();
	D3DXVECTOR3 Rot = GetRot();
	D3DXVECTOR3 PosV = GetPosV();
	D3DXVECTOR3 PosR = GetPosR();

	Rot = TOPVIEW_CAMERA_ROT_INI;
	fLength = CAMERA_LENGTH_INI;

	// 視点注視点の位置を設定
	PosR = INITIALIZE_D3DXVECTOR3 + D3DXVECTOR3(CAMERA_POS_X,0.0f,1.0f);
	PosV.x = CAMERA_POS_X;
	PosV.y = PosR.y - sinf(Rot.x) * fLength;
	PosV.z = PosR.z - cosf(Rot.x) * fLength;

	// 値の設定
	SetLength(fLength);
	SetRot(Rot);
	SetPosV(PosV);
	SetPosR(PosR);
}