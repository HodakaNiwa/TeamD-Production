//*****************************************************************************
//
//     入力の処理[input.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "input.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define DI_JOY_I_INPUT   (1000)  // スティックの方向入力受付を行う範囲
#define REPEAT_FRAME     (20)    // リピート情報を起動し始めるフレーム数の合計
#define REPEAT_TRIGGER   (7)     // リピート情報を起動するフレームの間隔

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPDIRECTINPUT8    CInput::m_pInput = NULL;         // DirectInputオブジェクトへのポインタ
HWND              CInput::m_hWnd = 0;              // ウィンドウハンドル
int               CInputJoyStick::m_nNumAll = 0;   // コントローラーのデバイス数

//*****************************************************************************
//    CInputの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CInput::CInput(int nNumDevice)
{
	m_pDevice = NULL;           // 入力デバイスへのポインタ
	m_nDeviceNum = nNumDevice;  // 入力デバイスの生成数
}

//=============================================================================
//    デストラクタ
//=============================================================================
CInput::~CInput()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (m_pInput == NULL)
	{// まだ作られていない
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
		m_hWnd = hWnd;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CInput::Uninit(void)
{
	// 入力デバイスの開放
	if (m_pDevice != NULL)
	{
		for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
		{// デバイスを確保した数だけ繰り返し
			if (m_pDevice[nCntDevice] != NULL)
			{// デバイスが確保されている
				m_pDevice[nCntDevice]->Unacquire();   // デバイスへのアクセス権を放棄
				m_pDevice[nCntDevice]->Release();
				m_pDevice[nCntDevice] = NULL;
			}
		}
		// メモリの開放
		delete m_pDevice;
		m_pDevice = NULL;
	}

	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//*****************************************************************************
//    CInputKeyboardの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard(int nNumDevice) : CInput(nNumDevice)
{
	for (int nCntKeyboard = 0; nCntKeyboard < NUM_KEY_MAX; nCntKeyboard++)
	{// キーボードのキーの数だけ繰り返し
		// キーボードの情報を初期化
		m_aKeyState[nCntKeyboard] = 0x00;         // プレス情報
		m_aKeyStateTrigger[nCntKeyboard] = 0x00;  // トリガー情報
		m_aKeyStateRelease[nCntKeyboard] = 0x00;  // リリース情報
		m_aKeyStateRepeat[nCntKeyboard] = 0x00;   // リピート情報
		m_aKeyboardCounter[nCntKeyboard] = 0;     // 入力カウンター
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力オブジェクトの作成
	CInput::Init(hInstance, hWnd);

	// 入力デバイスを動的に確保する
	m_pDevice = new LPDIRECTINPUTDEVICE8[m_nDeviceNum];
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// 生成するデバイスの数だけ繰り返し
		m_pDevice[nCntDevice] = NULL;
	}

	// 入力デバイスが生成されていない
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// デバイスの生成数だけ繰り返し
		if (m_pDevice[nCntDevice] == NULL)
		{// メモリが確保できた
		     // 入力デバイス(キーボード)の生成
			if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice[nCntDevice], NULL)))
			{
				return E_FAIL;
			}

			// データフォーマットを設定
			if (FAILED(m_pDevice[nCntDevice]->SetDataFormat(&c_dfDIKeyboard)))
			{
				return E_FAIL;
			}

			// 協調モードを設定
			if (FAILED(m_pDevice[nCntDevice]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			// キーボードへのアクセス権を獲得
			m_pDevice[nCntDevice]->Acquire();
		}
	}
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// Inputクラスの開放
	CInput::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX]; // キーボードの入力情報

	// 入力デバイス(キーボード)からデータを取得
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// デバイスの生成数だけ繰り返し
		if (m_pDevice[nCntDevice] != NULL)
		{// 生成できた
			if (SUCCEEDED(m_pDevice[nCntDevice]->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
			{// デバイスの入力状態を取得できた
				for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
				{// キーボードのキーの数だけ繰り返し
					m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];   // キーボードの入力情報(トリガー情報)保存
					m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey]; // キーボードの入力情報(リリース情報)保存
					if (aKeyState[nCntKey] == 0x80)
					{// キーボードが入力されている
						m_aKeyboardCounter[nCntKey]++;    // カウンターを進める
						if (m_aKeyboardCounter[nCntKey] >= REPEAT_FRAME)
						{// 20フレーム分キーが押されている
							if (m_aKeyboardCounter[nCntKey] % REPEAT_TRIGGER == 0)
							{// 3フレームごと
								m_aKeyStateRepeat[nCntKey] = 0x80;   // キーボードの入力情報(リピート情報)を起動
							}
							else
							{// それ以外のフレーム
								m_aKeyStateRepeat[nCntKey] = 0;      // キーボードの入力情報(リピート情報)を一時停止
							}
						}
					}
					else
					{// キーボードが入力されていない
						m_aKeyboardCounter[nCntKey] = 0;        // カウンターを戻す
						m_aKeyStateRepeat[nCntKey] = 0;         // キーボードの入力情報(リピート情報)を停止
					}
					m_aKeyState[nCntKey] = aKeyState[nCntKey];  // キーボードの入力情報(プレス情報)保存
				}
			}
			else
			{// デバイスの入力状態を取得できなかった
				m_pDevice[nCntDevice]->Acquire(); // キーボードへのアクセス権を獲得
			}
		}
	}
}

//=============================================================================
//    キーボードの入力情報(プレス情報)を取得
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
//    キーボードの入力情報(トリガー情報)を取得
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
//    キーボードの入力情報(リリース情報)を取得
//=============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
//    キーボードの入力情報(リピート情報)を取得
//=============================================================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//*****************************************************************************
//    CInputMouseの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CInputMouse::CInputMouse(int nNumDevice) : CInput(nNumDevice)
{
	for (int nCntMouse = 0; nCntMouse < DIMS_BUTTON_MAX; nCntMouse++)
	{// マウスのボタンの数だけ繰り返し
	    // マウスの情報を初期化
		m_aMouseState[nCntMouse] = 0x00;         // プレス情報
		m_aMouseStateTrigger[nCntMouse] = 0x00;  // トリガー情報
		m_aMouseStateRelease[nCntMouse] = 0x00;  // リリース情報
		m_aMouseStateRepeat[nCntMouse] = 0x00;   // リピート情報
		m_aMouseCounter[nCntMouse] = 0;          // 入力カウンター
	}

	m_MouseXAxis = 0;  // マウスのX軸
	m_MouseYAxis = 0;  // マウスのY軸
	m_MouseZAxis = 0;  // マウスのZ軸
}

//=============================================================================
//    デストラクタ
//=============================================================================
CInputMouse::~CInputMouse()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力オブジェクトの作成
	CInput::Init(hInstance, hWnd);

	// 入力デバイスを動的に確保する
	m_pDevice = new LPDIRECTINPUTDEVICE8[m_nDeviceNum];
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// 生成するデバイスの数だけ繰り返し
		m_pDevice[nCntDevice] = NULL;
	}

	// 入力デバイスが生成されていない
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// デバイスの生成数だけ繰り返し
		if (m_pDevice[nCntDevice] == NULL)
		{// メモリが確保できた
		    // 入力デバイス(マウス)の生成
			if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice[nCntDevice], NULL)))
			{
				return E_FAIL;
			}

			// データフォーマットを設定
			if (FAILED(m_pDevice[nCntDevice]->SetDataFormat(&c_dfDIMouse2)))
			{
				return E_FAIL;
			}

			// 協調モードを設定
			if (FAILED(m_pDevice[nCntDevice]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			// 軸の設定
			DIPROPDWORD diprop;
			diprop.diph.dwSize = sizeof(diprop);
			diprop.diph.dwHeaderSize = sizeof(diprop.diph);
			diprop.diph.dwObj = 0;
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）
			m_pDevice[nCntDevice]->SetProperty(DIPROP_AXISMODE, &diprop.diph);

			// ウィンドウハンドルを記憶しておく
			m_hWnd = hWnd;

			// マウスへのアクセス権を獲得
			m_pDevice[nCntDevice]->Acquire();
		}
	}
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CInputMouse::Uninit(void)
{
	// Inputクラスの開放
	CInput::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 aMouseState; // マウスの入力情報

	// 入力デバイス(マウス)からデータを取得
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// デバイスの生成数だけ繰り返し
		if (m_pDevice[nCntDevice] != NULL)
		{// 生成できた
			if (SUCCEEDED(m_pDevice[nCntDevice]->GetDeviceState(sizeof(DIMOUSESTATE2), &aMouseState)))
			{// デバイスの入力状態を取得できた
				// 軸の取得
				m_MouseXAxis = aMouseState.lX;  // マウスのX軸を格納
				m_MouseYAxis = aMouseState.lY;  // マウスのY軸を格納
				m_MouseZAxis = aMouseState.lZ;  // マウスのZ軸を格納

				// 前回のカーソル座標を記憶
				m_PointOld = m_Point;

				// カーソル位置の絶対座標を取得
				GetCursorPos(&m_Point);

				// カーソル絶対座標から、そのウィンドウ内の座標に補正
				ScreenToClient(m_hWnd, &m_Point);

				// さらにサイズ変更を考慮して、現在のサイズで補正（枠サイズ等あるので厳密ではない）
				m_Point.x = LONG((FLOAT)m_Point.x * ((FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_WIDTH));
				m_Point.y = LONG((FLOAT)m_Point.y * ((FLOAT)SCREEN_HEIGHT / (FLOAT)SCREEN_HEIGHT));

				// マウスのワールド座標を計算する
				CalcWorldPos();

				for (int nCntMouse = 0; nCntMouse < DIMS_BUTTON_MAX; nCntMouse++)
				{// マウスのボタンの数だけ繰り返し
					m_aMouseStateTrigger[nCntMouse] = (m_aMouseState[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) & aMouseState.rgbButtons[nCntMouse];  // マウスの入力情報(トリガー情報)保存
					m_aMouseStateRelease[nCntMouse] = (m_aMouseState[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) & m_aMouseState[nCntMouse];           // マウスの入力情報(リリース情報)保存
					if (aMouseState.rgbButtons[nCntMouse] == 0x80)
					{// マウスが入力されている
						m_aMouseCounter[nCntMouse]++;    // カウンターを進める
						if (m_aMouseCounter[nCntMouse] >= REPEAT_FRAME)
						{// 20フレーム分キーが押されている
							if (m_aMouseCounter[nCntMouse] % REPEAT_TRIGGER == 0)
							{// 3フレームごと
								m_aMouseStateRepeat[nCntMouse] = 0x80;   // マウスの入力情報(リピート情報)を起動
							}
							else
							{// それ以外のフレーム
								m_aMouseStateRepeat[nCntMouse] = 0;      // マウスの入力情報(リピート情報)を一時停止
							}
						}
					}
					else
					{// キーボードが入力されていない
						m_aMouseCounter[nCntMouse] = 0;               // カウンターを戻す
						m_aMouseStateRepeat[nCntMouse] = 0;           // マウスの入力情報(リピート情報)を停止
					}
					m_aMouseState[nCntMouse] = aMouseState.rgbButtons[nCntMouse];// マウスの入力情報(プレス情報)保存
				}
			}
			else
			{// デバイスの入力状態を取得できなかった
				m_pDevice[nCntDevice]->Acquire(); // マウスへのアクセス権を獲得
			}
		}
	}
}

//=============================================================================
//    マウスのワールド座標を計算する
//=============================================================================
void CInputMouse::CalcWorldPos(void)
{
	// 前回のワールド座標を保存
	m_WorldPosOldZn = m_WorldPosZn;
	m_WorldPosOldZf = m_WorldPosZf;

	// デバイスを取得する
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{
			// 透視投影変換行列を作る
			D3DXMATRIX mtxView, mtxProj, mtxViewPort;
			CManager::GetRenderer()->GetDevice()->GetTransform(D3DTS_VIEW, &mtxView);
			CManager::GetRenderer()->GetDevice()->GetTransform(D3DTS_PROJECTION, &mtxProj);
			D3DXMatrixIdentity(&mtxViewPort);
			mtxViewPort._11 = SCREEN_WIDTH / 2;
			mtxViewPort._22 = -SCREEN_HEIGHT / 2;
			mtxViewPort._41 = SCREEN_WIDTH / 2;
			mtxViewPort._42 = SCREEN_HEIGHT / 2;
			mtxViewPort._33 = 1.0f;
			mtxViewPort._44 = 1.0f;

			// 行列の逆行列を計算
			D3DXMATRIX mtxInvView, mtxInvProj, mtxInvViewPort;
			D3DXMatrixInverse(&mtxInvView, NULL, &mtxView);
			D3DXMatrixInverse(&mtxInvProj, NULL, &mtxProj);
			D3DXMatrixInverse(&mtxInvViewPort, NULL, &mtxViewPort);

			// スクリーン座標をワールド空間上に配置
			D3DXVECTOR3 MousePos = D3DXVECTOR3((float)m_Point.x, (float)m_Point.y, 0.01f);
			D3DXVec3TransformCoord(&m_WorldPosZn, &MousePos, &mtxInvViewPort);
			D3DXVec3TransformCoord(&m_WorldPosZn, &m_WorldPosZn, &mtxInvProj);
			D3DXVec3TransformCoord(&m_WorldPosZn, &m_WorldPosZn, &mtxInvView);
			MousePos.z = 1.0f;
			D3DXVec3TransformCoord(&m_WorldPosZf, &MousePos, &mtxInvViewPort);
			D3DXVec3TransformCoord(&m_WorldPosZf, &m_WorldPosZf, &mtxInvProj);
			D3DXVec3TransformCoord(&m_WorldPosZf, &m_WorldPosZf, &mtxInvView);
		}
	}
}

//=============================================================================
//    マウスの入力情報(プレス情報)を取得
//=============================================================================
bool CInputMouse::GetPress(int nButton)
{
	return (m_aMouseState[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    マウスの入力情報(トリガー情報)を取得
//=============================================================================
bool CInputMouse::GetTrigger(int nButton)
{
	return (m_aMouseStateTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    マウスの入力情報(リリース情報)を取得
//=============================================================================
bool CInputMouse::GetRelease(int nButton)
{
	return (m_aMouseStateRelease[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    マウスの入力情報(リピート情報)を取得
//=============================================================================
bool CInputMouse::GetRepeat(int nButton)
{
	return (m_aMouseStateRepeat[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    マウスのX軸を取得
//=============================================================================
LONG CInputMouse::GetMouseAxisX(void)
{
	return m_MouseXAxis;
}

//=============================================================================
//    マウスのY軸を取得
//=============================================================================
LONG CInputMouse::GetMouseAxisY(void)
{
	return m_MouseYAxis;
}

//=============================================================================
//    マウスのZ軸を取得
//=============================================================================
LONG CInputMouse::GetMouseAxisZ(void)
{
	return m_MouseZAxis;
}

//=============================================================================
//    マウスの現在のカーソル座標を取得
//=============================================================================
POINT CInputMouse::GetPoint(void)
{
	return m_Point;
}

//=============================================================================
//    マウスの前回のカーソル座標を取得
//=============================================================================
POINT CInputMouse::GetPointOld(void)
{
	return m_PointOld;
}

//=============================================================================
//    マウスの現在のワールド座標(カメラから近い方)を取得
//=============================================================================
D3DXVECTOR3 CInputMouse::GetWorldPosZn(void)
{
	return m_WorldPosZn;
}

//=============================================================================
//    マウスの前回のワールド座標(カメラから近い方)を取得
//=============================================================================
D3DXVECTOR3 CInputMouse::GetWorldPosOldZn(void)
{
	return m_WorldPosOldZn;
}

//=============================================================================
//    マウスの現在のワールド座標(カメラから遠い方)を取得
//=============================================================================
D3DXVECTOR3 CInputMouse::GetWorldPosZf(void)
{
	return m_WorldPosZf;
}

//=============================================================================
//    マウスの前回のワールド座標(カメラから遠い方)を取得
//=============================================================================
D3DXVECTOR3 CInputMouse::GetWorldPosOldZf(void)
{
	return m_WorldPosOldZf;
}

//*****************************************************************************
//    CInputJoyStickの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CInputJoyStick::CInputJoyStick(int nNumDevice) : CInput(nNumDevice)
{
	for (int nCntJoyStick = 0; nCntJoyStick < MAX_DEVICE; nCntJoyStick++)
	{// 生成できるデバイスの数だけ繰り返し
		m_apJoyStick[nCntJoyStick] = NULL;
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CInputJoyStick::~CInputJoyStick()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CInputJoyStick::Init(HINSTANCE hInstance, HWND hWnd)
{
	// 入力オブジェクトの作成
	CInput::Init(hInstance, hWnd);

	// 入力デバイスを動的に確保する
	m_pDevice = new LPDIRECTINPUTDEVICE8[m_nDeviceNum];
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// 生成するデバイスの数だけ繰り返し
		m_pDevice[nCntDevice] = NULL;
	}

	// ジョイスティックを列挙する
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, CallEnumJoyDeviceProc, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	// ジョイスティッククラスのインスタンス生成
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// 生成するデバイスの数だけ繰り返し
		if (m_apJoyStick[nCntDevice] == NULL)
		{// メモリが確保できる状態にある
			m_apJoyStick[nCntDevice] = new CJoyStick;
			if (m_apJoyStick[nCntDevice] != NULL)
			{// メモリが確保できた
				m_apJoyStick[nCntDevice]->Init();
			}
			else
			{// メモリが確保できなかった
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できる状態にない
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CInputJoyStick::Uninit(void)
{
	// ジョイスティッククラスの開放
	for (int nCntDevice = 0; nCntDevice < MAX_DEVICE; nCntDevice++)
	{// 生成できるデバイスの数だけ繰り返し
		if (m_apJoyStick[nCntDevice] != NULL)
		{// メモリが確保されている
			// 終了処理
			m_apJoyStick[nCntDevice]->Uninit();

		    // メモリの開放
			delete m_apJoyStick[nCntDevice];
			m_apJoyStick[nCntDevice] = NULL;
		}
	}

	// Inputクラスの開放
	CInput::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CInputJoyStick::Update(void)
{
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// 生成するデバイスの数だけ繰り返し
		if (m_apJoyStick[nCntDevice] != NULL && m_pDevice[nCntDevice] != NULL)
		{// メモリが確保できた
			m_apJoyStick[nCntDevice]->Update(m_pDevice[nCntDevice]);
		}
	}
}

//=============================================================================
//    ジョイスティックの情報を取得する関数
//=============================================================================
CJoyStick *CInputJoyStick::GetJoyStickDevice(int nIdxDevice)
{
	return m_apJoyStick[nIdxDevice];
}

//=============================================================================
//    ジョイスティックの列挙処理呼び出し関数
//=============================================================================
BOOL FAR PASCAL CInputJoyStick::CallEnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	CInputJoyStick *pJoyStickWk = CManager::GetJoypad();
	// デバイスを列挙する
	return pJoyStickWk->EnumJoyDeviceProc(lpddi, pvRef);
}

//=============================================================================
//    ジョイスティックの列挙処理
//=============================================================================
BOOL FAR PASCAL CInputJoyStick::EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	// 入力デバイス(ジョイスティック)の生成
	if (FAILED(m_pInput->CreateDevice(lpddi->guidInstance, &m_pDevice[m_nNumAll], NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice[m_nNumAll]->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice[m_nNumAll]->SetCooperativeLevel(m_hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	if (m_pDevice[m_nNumAll] != NULL)
	{// 生成できた
		DIPROPRANGE diprg;

		// 軸の値の範囲を設定
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -DI_JOY_I_INPUT;
		diprg.lMax = DI_JOY_I_INPUT;

		// 軸の設定
		// 左アナログスティック
		diprg.diph.dwObj = DIJOFS_X;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Z;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 右アナログスティック
		diprg.diph.dwObj = DIJOFS_RX;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RY;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 十字キー(上を0度とし時計回りに角度 * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ジョイスティックへのアクセス権を獲得(入力制御開始)
		m_pDevice[m_nNumAll]->Acquire();
	}

	//  複数列挙の場合はこっち
	if (m_nNumAll < m_nDeviceNum)
	{// まだデバイスを列挙できる
		m_nNumAll++;
		return DIENUM_CONTINUE;
	}
	else
	{// もうデバイスを列挙できない
		return DIENUM_STOP;
	}
}

//*****************************************************************************
//    CJoyStickの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CJoyStick::CJoyStick()
{
	for (int nCntJoyStick = 0; nCntJoyStick < DIJS_BUTTON_MAX; nCntJoyStick++)
	{// ジョイスティックのボタンの数だけ繰り返し
	    // ジョイスティックの情報を初期化
		m_aJoyState[nCntJoyStick] = 0x00;          // プレス情報
		m_aJoyStateTrigger[nCntJoyStick] = 0x00;   // トリガー情報
		m_aJoyStateRelease[nCntJoyStick] = 0x00;   // リリース情報
		m_aJoyStateRepeat[nCntJoyStick] = 0x00;    // リピート情報
		m_aJoyCounter[nCntJoyStick] = 0;           // 入力カウンター
	}
	m_Radian = 0.0f;                           // 十字キーのラジアン
	m_LeftAxis_X = 0.0f;                       // 左スティックのX軸
	m_LeftAxis_Y = 0.0f;                       // 左スティックのY軸
	m_RightAxis_X = 0.0f;                      // 右スティックのX軸
	m_RightAxis_Y = 0.0f;                      // 右スティックのY軸
}

//=============================================================================
//    デストラクタ
//=============================================================================
CJoyStick::~CJoyStick()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CJoyStick::Init(void)
{
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CJoyStick::Uninit(void)
{
	for (int nCntJoyStick = 0; nCntJoyStick < DIJS_BUTTON_MAX; nCntJoyStick++)
	{// ジョイスティックのボタンの数だけ繰り返し
	    // ジョイスティックの情報を初期化
		m_aJoyState[nCntJoyStick] = 0x00;          // プレス情報
		m_aJoyStateTrigger[nCntJoyStick] = 0x00;   // トリガー情報
		m_aJoyStateRelease[nCntJoyStick] = 0x00;   // リリース情報
		m_aJoyStateRepeat[nCntJoyStick] = 0x00;    // リピート情報
		m_aJoyCounter[nCntJoyStick] = 0;           // 入力カウンター
	}
	m_Radian = 0.0f;                           // 十字キーのラジアン
	m_LeftAxis_X = 0.0f;                       // 左スティックのX軸
	m_LeftAxis_Y = 0.0f;                       // 左スティックのY軸
	m_RightAxis_X = 0.0f;                      // 右スティックのX軸
	m_RightAxis_Y = 0.0f;                      // 右スティックのY軸
}

//=============================================================================
//    更新処理
//=============================================================================
void CJoyStick::Update(LPDIRECTINPUTDEVICE8 pDevice)
{
	DIJOYSTATE dijs;    // ジョイスティックの入力情報を格納する
	if (pDevice != NULL)
	{// 生成できた
	    // 入力デバイス(ジョイスティック)からデータを取得
		pDevice->Poll();
		if (SUCCEEDED(pDevice->GetDeviceState(sizeof(DIJOYSTATE), &dijs)))
		{// ジョイスティックの状態を取得できた
		    // スティックの軸を更新
			m_LeftAxis_X = (float)dijs.lX;
			m_LeftAxis_Y = (float)dijs.lY;
			m_RightAxis_X = (float)dijs.lZ;
			m_RightAxis_Y = (float)dijs.lRz;

			//-----------------------------
			// 左アナログスティック
			//-----------------------------
			if (dijs.lY <= -100)
			{// 左アナログスティックが上に倒された
				dijs.rgbButtons[DIJS_BUTTON_20] = 0x80;
			}
			if (dijs.lY >= 100)
			{// 左アナログスティックが下に倒された
				dijs.rgbButtons[DIJS_BUTTON_21] = 0x80;
			}
			if (dijs.lX <= -100)
			{// 左アナログスティックが左に倒された
				dijs.rgbButtons[DIJS_BUTTON_23] = 0x80;
			}
			if (dijs.lX >= 100)
			{// 左アナログスティックが右に倒された
				dijs.rgbButtons[DIJS_BUTTON_22] = 0x80;
			}

			//-----------------------------
			// 右アナログスティック
			//-----------------------------
			if (dijs.lRz <= -100)
			{// 右アナログスティックが上に倒された
				dijs.rgbButtons[DIJS_BUTTON_24] = 0x80;
			}
			if (dijs.lRz >= 100)
			{// 右アナログスティックが下に倒された
				dijs.rgbButtons[DIJS_BUTTON_25] = 0x80;
			}
			if (dijs.lZ <= -100)
			{// 右アナログスティックが左に倒された
				dijs.rgbButtons[DIJS_BUTTON_27] = 0x80;
			}
			if (dijs.lZ >= 100)
			{// 右アナログスティックが右に倒された
				dijs.rgbButtons[DIJS_BUTTON_26] = 0x80;
			}

			//-----------------------------
			// 十字キー
			//-----------------------------
			if (dijs.rgdwPOV[0] / 100 == 0)
			{// 十字キーの上が押された
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 45)
			{// 十字キーの右上が押された
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 90)
			{// 十字キーの右が押された
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 135)
			{// 十字キーの右下が押された
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 180)
			{// 十字キーの下が押された
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 225)
			{// 十字キーの左下が押された
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 270)
			{// 十字キーの左が押された
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 315)
			{// 十字キーの左上が押された
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			for (int nCntJoyButton = 0; nCntJoyButton < DIJS_BUTTON_MAX; nCntJoyButton++)
			{// ジョイスティックのボタンの数だけ繰り返し
				m_aJoyStateTrigger[nCntJoyButton] = (m_aJoyState[nCntJoyButton] ^ dijs.rgbButtons[nCntJoyButton]) & dijs.rgbButtons[nCntJoyButton];      // ジョイスティックの入力情報(トリガー情報)保存
				m_aJoyStateRelease[nCntJoyButton] = (m_aJoyState[nCntJoyButton] ^ dijs.rgbButtons[nCntJoyButton]) & m_aJoyState[nCntJoyButton]; // ジョイスティックの入力情報(リリース情報)保存
				if (dijs.rgbButtons[nCntJoyButton] == 0x80)
				{// ジョイスティックが入力されている
					m_aJoyCounter[nCntJoyButton]++;    // カウンターを進める
					if (m_aJoyCounter[nCntJoyButton] >= REPEAT_FRAME)
					{// 20フレーム分キーが押されている
						if (m_aJoyCounter[nCntJoyButton] % REPEAT_TRIGGER == 0)
						{// 3フレームごと
							m_aJoyStateRepeat[nCntJoyButton] = 0x80;   // ジョイスティックの入力情報(リピート情報)を起動
						}
						else
						{// それ以外のフレーム
							m_aJoyStateRepeat[nCntJoyButton] = 0;     // ジョイスティックの入力情報(リピート情報)を一時停止
						}
					}
				}
				else
				{// ジョイスティックが入力されていない
					m_aJoyCounter[nCntJoyButton] = 0;          // カウンターを戻す
					m_aJoyStateRepeat[nCntJoyButton] = 0;      // ジョイスティックの入力情報(リピート情報)を停止
				}
				m_aJoyState[nCntJoyButton] = dijs.rgbButtons[nCntJoyButton];   // ジョイスティックの入力情報(プレス情報)保存
			}
		}
		else
		{// ジョイパッドへのアクセス権がない
			pDevice->Acquire(); //  ジョイパッドへのアクセス権を獲得
		}
	}
}

//=============================================================================
//    ジョイスティックの入力情報(プレス情報)を取得
//=============================================================================
bool CJoyStick::GetPress(int nButton)
{
	return (m_aJoyState[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    ジョイスティックの入力情報(トリガー情報)を取得
//=============================================================================
bool CJoyStick::GetTrigger(int nButton)
{
	return (m_aJoyStateTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    ジョイスティックの入力情報(リリース情報)を取得
//=============================================================================
bool CJoyStick::GetRelease(int nButton)
{
	return (m_aJoyStateRelease[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    ジョイスティックの入力情報(リピート情報)を取得
//=============================================================================
bool CJoyStick::GetRepeat(int nButton)
{
	return (m_aJoyStateRepeat[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    ジョイスティックの十字キーのラジアンを取得
//=============================================================================
float CJoyStick::GetRadian(void)
{
	return m_Radian / 100;
}

//=============================================================================
//    ジョイスティックの左スティックのX軸を取得
//=============================================================================
float CJoyStick::GetLeftAxisX(void)
{
	return m_LeftAxis_X;
}

//=============================================================================
//    ジョイスティックの左スティックのY軸を取得
//=============================================================================
float CJoyStick::GetLeftAxisY(void)
{
	return m_LeftAxis_Y;
}

//=============================================================================
//    ジョイスティックの左スティックの軸を取得
//=============================================================================
float CJoyStick::GetLeftAxis(void)
{
	float Axis = atan2f(m_LeftAxis_X, m_LeftAxis_Y);  // 円周率に変換
	return Axis;
}

//=============================================================================
//    ジョイスティックの右スティックのX軸を取得
//=============================================================================
float CJoyStick::GetRightAxisX(void)
{
	return m_RightAxis_X;
}

//=============================================================================
//    ジョイスティックの右スティックのY軸を取得
//=============================================================================
float CJoyStick::GetRightAxisY(void)
{
	return m_RightAxis_Y;
}

//=============================================================================
//    ジョイスティックの右スティックの軸を取得
//=============================================================================
float CJoyStick::GetRightAxis(void)
{
	float Axis = atan2f(m_RightAxis_X, m_RightAxis_Y); // 円周率に変換
	return Axis;
}

//*****************************************************************************
//    XInputの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CXInput::CXInput()
{
	// 各種値のクリア
	for (int nCntPad = 0; nCntPad < MAX_DEVICE; nCntPad++)
	{// 生成できるデバイスの数だけ繰り返し
		for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
		{// ボタンの数だけ繰り返し
			m_aGamePadState[nCntPad][nCntButton] = 0x00;          // プレス情報
			m_aGamePadStateTrigger[nCntPad][nCntButton] = 0x00;   // トリガー情報
			m_aGamePadStateRelease[nCntPad][nCntButton] = 0x00;   // リリース情報
			m_aGamePadStateRepeat[nCntPad][nCntButton] = 0x00;    // リピート情報
			m_aGamePadCounter[nCntPad][nCntButton] = 0;           // 入力カウンター
		}
		m_aGamePad[nCntPad].m_bConnected = false;                 // 接続の状態
		m_aGamePad[nCntPad].m_State.Gamepad.wButtons = 0;         // ボタンのビット情報
		m_aGamePad[nCntPad].m_State.Gamepad.bLeftTrigger = 0;     // LTボタンの状態
		m_aGamePad[nCntPad].m_State.Gamepad.bRightTrigger = 0;    // RTボタンの状態
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbLX = 0;         // 左スティックのX軸
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbLY = 0;         // 左スティックのY軸
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbRX = 0;         // 右スティックのX軸
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbRY = 0;         // 右スティックのY軸

		m_bLeftTrigger[nCntPad] = 0;                              // LTボタンの状態
		m_bRightTrigger[nCntPad] = 0;                             // RTボタンの状態
		m_sThumbLX[nCntPad] = 0;                                  // 左スティックのX軸の値
		m_sThumbLY[nCntPad] = 0;                                  // 左スティックのY軸の値
		m_sThumbRX[nCntPad] = 0;                                  // 右スティックのX軸の値
		m_sThumbRY[nCntPad] = 0;                                  // 右スティックのY軸の値
		m_wButtons[nCntPad] = 0;                                  // ボタンの状態
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CXInput::~CXInput()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CXInput::Init(void)
{
	// ゲームパッドのクリア
	ZeroMemory(m_aGamePad, sizeof(CONTROLER_STATE) * MAX_DEVICE);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CXInput::Uninit(void)
{
	// ゲームパッドの開放
	XInputEnable(false);
}

//=============================================================================
//    更新処理
//=============================================================================
void CXInput::Update(void)
{
	DWORD dwResult;                             // ゲームパッドが取得できたかどうか
	BYTE aGamePadState[XIJS_BUTTON_MAX] = {};   // ゲームパッドのボタン入力情報
	for (DWORD nCntPad = 0; nCntPad < MAX_DEVICE; nCntPad++)
	{// 接続できるゲームパッド(最大4つまで)の数だけ繰り返し
		// ゲームパッドの状態を取得
 		dwResult = XInputGetState(nCntPad, &m_aGamePad[nCntPad].m_State);
		if (dwResult == ERROR_SUCCESS)
		{// 状態取得に成功した
			// 状態を格納する
			m_aGamePad[nCntPad].m_bConnected = true;                                       // 接続された状態にする
			m_wButtons[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.wButtons;            // ボタンのビット情報を取得
			m_bLeftTrigger[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.bLeftTrigger;    // LTボタンの状態を取得
			m_bRightTrigger[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.bRightTrigger;  // RTボタンの状態を取得
			m_sThumbLX[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbLX;            // 左スティックのX軸を取得
			m_sThumbLY[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbLY;            // 左スティックのY軸を取得
			m_sThumbRX[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbRX;            // 右スティックのX軸を取得
			m_sThumbRY[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbRY;            // 右スティックのY軸を取得

			// 入力の状態を取得する
			// ボタン
			DWORD wButtons = m_wButtons[nCntPad]; // ボタンのビット情報
			if (wButtons >= XINPUT_GAMEPAD_Y)
			{// Yボタンが押されている
				aGamePadState[XIJS_BUTTON_13] = 0x80;   // 入力を起動
				wButtons -= XINPUT_GAMEPAD_Y;
			}
			if (wButtons >= XINPUT_GAMEPAD_X)
			{// Xボタンが押されている
				aGamePadState[XIJS_BUTTON_12] = 0x80;   // 入力を起動
				wButtons -= XINPUT_GAMEPAD_X;
			}
			if (wButtons >= XINPUT_GAMEPAD_B)
			{// Bボタンが押されている
				aGamePadState[XIJS_BUTTON_11] = 0x80;   // 入力を起動
				wButtons -= XINPUT_GAMEPAD_B;
			}
			if (wButtons >= XINPUT_GAMEPAD_A)
			{// Aボタンが押されている
				aGamePadState[XIJS_BUTTON_10] = 0x80;   // 入力を起動
				wButtons -= XINPUT_GAMEPAD_A;
			}
			if (wButtons >= XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{// RBボタンが押されている
				aGamePadState[XIJS_BUTTON_9] = 0x80;    // 入力を起動
				wButtons -= XINPUT_GAMEPAD_RIGHT_SHOULDER;
			}
			if (wButtons >= XINPUT_GAMEPAD_LEFT_SHOULDER)
			{// LBボタンが押されている
				aGamePadState[XIJS_BUTTON_8] = 0x80;    // 入力を起動
				wButtons -= XINPUT_GAMEPAD_LEFT_SHOULDER;
			}
			if (wButtons >= XINPUT_GAMEPAD_RIGHT_THUMB)
			{// 右スティックが押し込まれている
				aGamePadState[XIJS_BUTTON_7] = 0x80;    // 入力を起動
				wButtons -= XINPUT_GAMEPAD_RIGHT_THUMB;
			}
			if (wButtons >= XINPUT_GAMEPAD_LEFT_THUMB)
			{// 右スティックが押し込まれている
				aGamePadState[XIJS_BUTTON_6] = 0x80;    // 入力を起動
				wButtons -= XINPUT_GAMEPAD_LEFT_THUMB;
			}
			if (wButtons >= XINPUT_GAMEPAD_BACK)
			{// BACKボタンが押されている
				aGamePadState[XIJS_BUTTON_5] = 0x80;    // 入力を起動
				wButtons -= XINPUT_GAMEPAD_BACK;
			}
			if (wButtons >= XINPUT_GAMEPAD_START)
			{// BACKボタンが押されている
				aGamePadState[XIJS_BUTTON_4] = 0x80;    // 入力を起動
				wButtons -= XINPUT_GAMEPAD_START;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_RIGHT)
			{// 十字キーの右が押されている
				aGamePadState[XIJS_BUTTON_3] = 0x80;    // 入力を起動
				wButtons -= XINPUT_GAMEPAD_DPAD_RIGHT;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_LEFT)
			{// 十字キーの左が押されている
				aGamePadState[XIJS_BUTTON_2] = 0x80;    // 入力を起動
				wButtons -= XINPUT_GAMEPAD_DPAD_LEFT;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_DOWN)
			{// 十字キーの下が押されている
				aGamePadState[XIJS_BUTTON_1] = 0x80;    // 入力を起動
				wButtons -= XINPUT_GAMEPAD_DPAD_DOWN;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_UP)
			{// 十字キーの上が押されている
				aGamePadState[XIJS_BUTTON_0] = 0x80;    // 入力を起動
			}

			// LTトリガー
			if (m_bLeftTrigger[nCntPad] >= MIN_GAMEPAD_LEFT_TRIGGER)
			{// LTトリガーが押されている
				aGamePadState[XIJS_BUTTON_14] = 0x80;   // 入力を起動する
			}
			// RTトリガー
			if (m_bRightTrigger[nCntPad] >= MIN_GAMEPAD_RIGHT_TRIGGER)
			{// LRトリガーが押されている
				aGamePadState[XIJS_BUTTON_15] = 0x80;   // 入力を起動する
			}

			// 左スティック
			if (m_sThumbLX[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_X)
			{// 左スティックが右に倒された
				aGamePadState[XIJS_BUTTON_19] = 0x80;   // 入力を起動する
			}
			if (m_sThumbLX[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_X)
			{// 左スティックが左に倒された
				aGamePadState[XIJS_BUTTON_18] = 0x80;   // 入力を起動する
			}
			if (m_sThumbLY[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 左スティックが上に倒された
				aGamePadState[XIJS_BUTTON_16] = 0x80;   // 入力を起動する
			}
			if (m_sThumbLY[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 左スティックが下に倒された
				aGamePadState[XIJS_BUTTON_17] = 0x80;   // 入力を起動する
			}

			// 右スティック
			if (m_sThumbRX[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_X)
			{// 右スティックが右に倒された
				aGamePadState[XIJS_BUTTON_23] = 0x80;   // 入力を起動する
			}
			if (m_sThumbRX[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_X)
			{// 右スティックが左に倒された
				aGamePadState[XIJS_BUTTON_22] = 0x80;   // 入力を起動する
			}
			if (m_sThumbRY[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 右スティックが上に倒された
				aGamePadState[XIJS_BUTTON_20] = 0x80;   // 入力を起動する
			}
			if (m_sThumbRY[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 右スティックが下に倒された
				aGamePadState[XIJS_BUTTON_21] = 0x80;   // 入力を起動する
			}

			// ボタンの状態を格納する
			for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
			{// ボタンの数だけ繰り返し
				m_aGamePadStateTrigger[nCntPad][nCntButton] = (m_aGamePadState[nCntPad][nCntButton] ^ aGamePadState[nCntButton]) & aGamePadState[nCntButton];            // ボタンの入力情報(トリガー情報)保存
				m_aGamePadStateRelease[nCntPad][nCntButton] = (m_aGamePadState[nCntPad][nCntButton] ^ aGamePadState[nCntButton]) & m_aGamePadState[nCntPad][nCntButton]; // ボタンの入力情報(リリース情報)保存
				if (aGamePadState[nCntButton] == 0x80)
				{// ボタンが入力されている
					m_aGamePadCounter[nCntPad][nCntButton]++;    // カウンターを進める
					if (m_aGamePadCounter[nCntPad][nCntButton] >= REPEAT_FRAME)
					{// 20フレーム分ボタンが押されている
						if (m_aGamePadCounter[nCntPad][nCntButton] % REPEAT_TRIGGER == 0)
						{// 3フレームごと
							m_aGamePadStateRepeat[nCntPad][nCntButton] = 0x80;   // ボタンの入力情報(リピート情報)を起動
						}
						else
						{// それ以外のフレーム
							m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;      // ボタンの入力情報(リピート情報)を一時停止
						}
					}
				}
				else
				{// ゲームパッドのボタンが入力されていない
					m_aGamePadCounter[nCntPad][nCntButton] = 0;        // カウンターを戻す
					m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;    // ボタンの入力情報(リピート情報)を停止
				}
				m_aGamePadState[nCntPad][nCntButton] = aGamePadState[nCntButton];  // ボタンの入力情報(プレス情報)保存
			}
		}
		else
		{// 状態取得に失敗した
			m_aGamePad[nCntPad].m_bConnected = false;  // 接続されていない状態にする
		}
	}
}

//=============================================================================
//    ゲームパッドの入力情報(プレス情報)を取得
//=============================================================================
bool CXInput::GetPress(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadState[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    ゲームパッドの入力情報(トリガー情報)を取得
//=============================================================================
bool CXInput::GetTrigger(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadStateTrigger[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    ゲームパッドの入力情報(リリース情報)を取得
//=============================================================================
bool CXInput::GetRelease(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadStateRelease[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    ゲームパッドの入力情報(リピート情報)を取得
//=============================================================================
bool CXInput::GetRepeat(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadStateRepeat[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    LTボタンの状態を取得
//=============================================================================
BYTE CXInput::GetLeftTrigger(int nCntPad)
{
	return m_bLeftTrigger[nCntPad];
}

//=============================================================================
//    RTボタンの状態を取得
//=============================================================================
BYTE CXInput::GetRightTrigger(int nCntPad)
{
	return m_bRightTrigger[nCntPad];
}

//=============================================================================
//    左スティックの向きを取得
//=============================================================================
float CXInput::GetLeftAxiz(int nCntPad)
{
	float axiz = atan2f(m_sThumbLX[nCntPad], m_sThumbLY[nCntPad]);
	return axiz;
}

//=============================================================================
//    右スティックの向きを取得
//=============================================================================
float CXInput::GetRightAxiz(int nCntPad)
{
	float axiz = atan2f(m_sThumbRX[nCntPad], m_sThumbRY[nCntPad]);
	return axiz;
}

//=============================================================================
//    左スティックのX軸を取得
//=============================================================================
SHORT CXInput::GetThumbLX(int nCntPad)
{
	return m_sThumbLX[nCntPad];
}

//=============================================================================
//    左スティックのY軸を取得
//=============================================================================
SHORT CXInput::GetThumbLY(int nCntPad)
{
	return m_sThumbLY[nCntPad];
}

//=============================================================================
//    右スティックのX軸を取得
//=============================================================================
SHORT CXInput::GetThumbRX(int nCntPad)
{
	return m_sThumbRX[nCntPad];
}

//=============================================================================
//    右スティックのY軸を取得
//=============================================================================
SHORT CXInput::GetThumbRY(int nCntPad)
{
	return m_sThumbRY[nCntPad];
}
