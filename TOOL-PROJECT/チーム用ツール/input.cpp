//=============================================================================
//
// 入力処理処理 [input.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "input.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//*****************************************************************************
// 入力処理
//*****************************************************************************
//=============================================================================
// 入力処理のコンストラクタ
//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//=============================================================================
// 入力処理のデストラクタ
//=============================================================================
CInput::~CInput()
{

}

//=============================================================================
// 入力処理の初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void CInput::Uninit(void)
{
	// 入力デバイス(キーボード)の開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();	// キーボードへのアクセス権を放棄
		m_pDevice->Release();
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
// キーボードの入力処理
//*****************************************************************************
//=============================================================================
// キーボードのコンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	//これは絶対に禁止！！！
	//m_aKeyState[NUM_KEY_MAX] = {};
	//m_aKeyStateTrigger[NUM_KEY_MAX] = {};

	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		m_aKeyState[nCntKey] = 0;
		m_aKeyStateTrigger[nCntKey] = 0;
	}
}

//=============================================================================
// キーボードのデストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// キーボードの初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	//入力処理の終了処理
	CInput::Uninit();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// キーボードの入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	// キーボード入力情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();	// キーボードへのアクセス権を獲得
	}
}

//=============================================================================
// キーボードの入力情報(プレス情報)を取得
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードの入力情報(トリガー情報)を取得
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//*****************************************************************************
// ジョイパッドの入力処理
//*****************************************************************************
//=============================================================================
// ジョイパッドのコンストラクタ
//=============================================================================
CInputJoypad::CInputJoypad()
{
	//これは絶対に禁止！！！
	//m_aKeyState[NUM_KEY_MAX] = {};
	//m_aKeyStateTrigger[NUM_KEY_MAX] = {};

	for (int nCntKey = 0; nCntKey < 32; nCntKey++)
	{
		m_aButtonState[nCntKey] = 0;
		m_aButtonStateTrigger[nCntKey] = 0;
	}
}

//=============================================================================
// ジョイパッドのデストラクタ
//=============================================================================
CInputJoypad::~CInputJoypad()
{

}

//=============================================================================
// ジョイパッドの初期化処理
//=============================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	if (m_pDevice != NULL)
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
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Z;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 右アナログスティック
		diprg.diph.dwObj = DIJOFS_RX;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RY;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 十字キー(上を0度とし時計回りに角度 * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ジョイスティックへのアクセス権を獲得(入力制御開始)
		m_pDevice->Acquire();
	}
	return S_OK;
}

//=============================================================================
// ジョイパッドの終了処理
//=============================================================================
void CInputJoypad::Uninit(void)
{
	//入力処理の終了処理
	CInput::Uninit();
}

//=============================================================================
// ジョイパッドの更新処理
//=============================================================================
void CInputJoypad::Update(void)
{
	DIJOYSTATE aJoypadState;	// ジョイパッドの入力情報
	int nCntJoypad;

	if (m_pDevice != NULL)
	{
		m_pDevice->Poll();
		// 入力デバイスからデータを取得
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &aJoypadState)))
		{
			m_LeftAxizX = (float)aJoypadState.lX;
			m_LeftAxizY = (float)aJoypadState.lY;
			m_RightAxizX = (float)aJoypadState.lZ;
			m_RightAxizY = (float)aJoypadState.lRz;

			if (m_LeftAxizY <= -100)
			{//左スティックが上に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_LSTICK_UP] = 0x80;
			}
			if (m_LeftAxizY >= 100)
			{//左スティックが下に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_LSTICK_DOWN] = 0x80;
			}
			if (m_LeftAxizX >= 100)
			{//左スティックが右に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_LSTICK_RIGHT] = 0x80;
			}
			if (m_LeftAxizX <= -100)
			{//左スティックが左に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_LSTICK_LEFT] = 0x80;
			}

			if (m_RightAxizY <= -100)
			{//右スティックが上に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_RSTICK_UP] = 0x80;
			}
			if (m_RightAxizY >= 100)
			{//右スティックが下に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_RSTICK_DOWN] = 0x80;
			}
			if (m_RightAxizX <= -100)
			{//右スティックが右に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_RSTICK_RIGHT] = 0x80;
			}
			if (m_RightAxizX >= 100)
			{//右スティックが左に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_RSTICK_LEFT] = 0x80;
			}
			if (aJoypadState.rgdwPOV[0] == 0)
			{//十字キーが上に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_UP] = 0x80;
				m_Radian = D3DXToRadian(aJoypadState.rgdwPOV[0]);
			}
			if (aJoypadState.rgdwPOV[0] == 4500)
			{//十字キーが右上に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_RIGHT] = 0x80;
				m_Radian = D3DXToRadian(aJoypadState.rgdwPOV[0]);
			}
			if (aJoypadState.rgdwPOV[0] == 9000)
			{//十字キーが右に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_RIGHT] = 0x80;
				m_Radian = D3DXToRadian(aJoypadState.rgdwPOV[0]);
			}
			if (aJoypadState.rgdwPOV[0] == 13500)
			{//十字キーが右下に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_RIGHT] = 0x80;
				m_Radian = D3DXToRadian(aJoypadState.rgdwPOV[0]);
			}
			if (aJoypadState.rgdwPOV[0] == 18000)
			{//十字キーが下に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_DOWN] = 0x80;
				m_Radian = D3DXToRadian(aJoypadState.rgdwPOV[0]);
			}
			if (aJoypadState.rgdwPOV[0] == 22500)
			{//十字キーが右下に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_DOWN] = 0x80;
				m_Radian = D3DXToRadian(aJoypadState.rgdwPOV[0]);
			}
			if (aJoypadState.rgdwPOV[0] == 27000)
			{//十字キーが左に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_LEFT] = 0x80;
				m_Radian = D3DXToRadian(aJoypadState.rgdwPOV[0]);
			}
			if (aJoypadState.rgdwPOV[0] == 31500)
			{//十字キーが左上に押された
				aJoypadState.rgbButtons[DIJS_BUTTON_LEFT] = 0x80;
				m_Radian = D3DXToRadian(aJoypadState.rgdwPOV[0]);
			}

			for (nCntJoypad = 0; nCntJoypad < 32; nCntJoypad++)
			{
				m_aButtonStateTrigger[nCntJoypad] = (m_aButtonState[nCntJoypad] ^ aJoypadState.rgbButtons[nCntJoypad]) &aJoypadState.rgbButtons[nCntJoypad];
				m_aButtonState[nCntJoypad] = aJoypadState.rgbButtons[nCntJoypad];	// キーボード入力情報保存
			}

		}
		else
		{
			m_pDevice->Acquire();	// キーボードへのアクセス権を獲得
		}
	}
}

//=============================================================================
// ジョイパッドの入力情報(プレス情報)を取得
//=============================================================================
bool CInputJoypad::GetPress(DIJS_BUTTON button)
{
	return(m_aButtonState[button] & 0x80) ? true : false;
}

//=============================================================================
// ジョイパッドの入力情報(トリガー情報)を取得
//=============================================================================
bool CInputJoypad::GetTrigger(DIJS_BUTTON button)
{
	return(m_aButtonStateTrigger[button] & 0x80) ? true : false;
}

//=============================================================================
// 左スティックの取得
//=============================================================================
float CInputJoypad::GetLeftAxiz(void)
{
	float Axiz = atan2f(m_LeftAxizX, m_LeftAxizY);
	return Axiz;
}

//=============================================================================
// 右スティックの取得
//=============================================================================
float CInputJoypad::GetRightAxiz(void)
{
	float RightAxiz = atan2f(m_RightAxizX, m_RightAxizY);
	return RightAxiz;
}

//=============================================================================
// 角度の取得
//=============================================================================
float CInputJoypad::GetRadian(void)
{
	return m_Radian / 100;
}