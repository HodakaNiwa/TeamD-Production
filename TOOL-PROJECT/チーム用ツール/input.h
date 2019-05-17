//=============================================================================
//
// 入力処理 [input.h]
// Author :　Jukiya Hayakawa
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// キーの最大数
#define DI_JOY_I_INPUT		(1000)	// DirectInput

//*****************************************************************************
// クラス定義(入力処理)
//*****************************************************************************
class CInput
{
public:		//誰からもアクセス可能
	//メンバ関数
	CInput();
	//仮想関数
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	//純粋仮想関数
	virtual void Update(void) = 0;

private:	//自分だけがアクセス可能

protected:	//自分と派生クラスだけがアクセス可能
	//メンバ変数
	LPDIRECTINPUTDEVICE8 m_pDevice;	//デバイスのポインタ
	//静的メンバ変数
	static LPDIRECTINPUT8 m_pInput;	//入力のポインタ
};

//*****************************************************************************
// クラス定義(キーボードの入力処理)
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:		//誰からもアクセス可能
	//メンバ関数
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);

private:	//自分だけがアクセス可能
	//メンバ変数
	BYTE m_aKeyState[NUM_KEY_MAX];					// キーボードの入力情報（プレス情報）
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];			// キーボードの入力情報（トリガー情報）

protected:	//自分と派生クラスだけがアクセス可能
};
//*****************************************************************************
// クラス定義(ジョイパッドの入力処理)
//*****************************************************************************
class CInputJoypad : public CInput
{
public:		//誰からもアクセス可能
	typedef enum
	{
		DIJS_BUTTON_X = 0,			//Xボタン
		DIJS_BUTTON_A,				//Aボタン
		DIJS_BUTTON_B,				//Bボタン
		DIJS_BUTTON_Y,				//Yボタン
		DIJS_BUTTON_LB,				//LBボタン
		DIJS_BUTTON_RB,				//RBボタン
		DIJS_BUTTON_LT,				//LTボタン
		DIJS_BUTTON_RT,				//RTbutton
		DIJS_BUTTON_BACK,			//BACKボタン
		DIJS_BUTTON_START,			//STARTボタン
		DIJS_BUTTON_LSTICK_PUSH,	//左スティック押下
		DIJS_BUTTON_RSTICK_PUSH,	//右スティック押下
		DIJS_BUTTON_13,
		DIJS_BUTTON_14,
		DIJS_BUTTON_15,
		DIJS_BUTTON_16,
		DIJS_BUTTON_17,
		DIJS_BUTTON_18,
		DIJS_BUTTON_19,
		DIJS_BUTTON_UP,				//十字キー上
		DIJS_BUTTON_DOWN,			//十字キー下
		DIJS_BUTTON_LEFT,			//十字キー左
		DIJS_BUTTON_RIGHT,			//十字キー右
		DIJS_BUTTON_LSTICK_UP,		//左スティック上
		DIJS_BUTTON_LSTICK_DOWN,	//左スティック下
		DIJS_BUTTON_LSTICK_LEFT,	//左スティック左
		DIJS_BUTTON_LSTICK_RIGHT,	//左スティック右
		DIJS_BUTTON_RSTICK_UP,		//右スティック上
		DIJS_BUTTON_RSTICK_DOWN,	//右スティック下
		DIJS_BUTTON_RSTICK_LEFT,	//右スティック左
		DIJS_BUTTON_RSTICK_RIGHT,	//右スティック右
		DIJS_BUTTON_MAX,
	} DIJS_BUTTON;

	//メンバ関数
	CInputJoypad();
	~CInputJoypad();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(DIJS_BUTTON button);
	bool GetTrigger(DIJS_BUTTON button);
	float GetLeftAxiz(void);
	float GetRightAxiz(void);
	float GetRadian(void);

private:	//自分だけがアクセス可能
	//メンバ変数
	BYTE m_aButtonState[32];					// キーボードの入力情報（プレス情報）
	BYTE m_aButtonStateTrigger[32];				// キーボードの入力情報（トリガー情報）
	float m_LeftAxizX;							// 左スティックのX軸
	float m_LeftAxizY;							// 左スティックのY軸
	float m_RightAxizX;							// 右スティックのX軸
	float m_RightAxizY;							// 右スティックのY軸
	float m_Radian;								// 半径

protected:	//自分と派生クラスだけがアクセス可能
};
#endif