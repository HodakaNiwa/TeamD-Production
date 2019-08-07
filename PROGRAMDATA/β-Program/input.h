//*****************************************************************************
//
//     入力の処理[input.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX      (256)     // キーボードの最大数
#define MAX_DEVICE       (4)       // 接続できる入力デバイスの数

//*****************************************************************************
//     Inputクラスの定義
//*****************************************************************************
class CInput
{
public:     // 誰でもアクセス可能
	CInput(int nNumDevice = 1);
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:  // このクラスと派生クラスだけがアクセス可能
	static HWND           m_hWnd;        // ウィンドウハンドル
	static LPDIRECTINPUT8 m_pInput;      // DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8  *m_pDevice;    // 入力デバイスへのポインタ
	int                   m_nDeviceNum;  // 入力デバイスをいくつ生成したか
};

//*****************************************************************************
//     キーボードクラスの定義
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:     // 誰でもアクセス可能
	CInputKeyboard(int nNumDevice = 1);
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
private:    // このクラスだけがアクセス可能
	BYTE   m_aKeyState[NUM_KEY_MAX];          // キーボードの入力情報(プレス情報)
	BYTE   m_aKeyStateTrigger[NUM_KEY_MAX];   // キーボードの入力情報(トリガー情報)
	BYTE   m_aKeyStateRelease[NUM_KEY_MAX];   // キーボードの入力情報(リリース情報)
	BYTE   m_aKeyStateRepeat[NUM_KEY_MAX];    // キーボードの入力情報(リピート情報)
	DWORD  m_aKeyboardCounter[NUM_KEY_MAX];   // キーボードが入力されてる間を数えるカウンター
};

//*****************************************************************************
//     マウスクラスの定義
//*****************************************************************************
class CInputMouse : public CInput
{
public:     // 誰でもアクセス可能
	//------------------------
	//  ボタンの列挙
	//------------------------
	typedef enum
	{
		DIMS_BUTTON_0 = 0,  // マウスの左ボタン
		DIMS_BUTTON_1,      // マウスの右ボタン
		DIMS_BUTTON_2,      // マウスのホイールボタン
		// 以下はボタン自体がないマウスも存在
		DIMS_BUTTON_3,
		DIMS_BUTTON_4,
		DIMS_BUTTON_5,
		DIMS_BUTTON_6,
		DIMS_BUTTON_7,
		DIMS_BUTTON_MAX
	}DIMS_BUTTON;

	CInputMouse(int nNumDevice = 1);
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelease(int nButton);
	bool GetRepeat(int nButton);

	LONG GetMouseAxisX(void);
	LONG GetMouseAxisY(void);
	LONG GetMouseAxisZ(void);

	POINT GetPoint(void);
	POINT GetPointOld(void);

	D3DXVECTOR3 GetWorldPosZn(void);
	D3DXVECTOR3 GetWorldPosOldZn(void);
	D3DXVECTOR3 GetWorldPosZf(void);
	D3DXVECTOR3 GetWorldPosOldZf(void);

private:    // このクラスだけがアクセス可能
	void CalcWorldPos(void);

	BYTE   m_aMouseState[DIMS_BUTTON_MAX];         // マウスの入力情報(プレス情報)
	BYTE   m_aMouseStateTrigger[DIMS_BUTTON_MAX];  // マウスの入力情報(トリガー情報)
	BYTE   m_aMouseStateRelease[DIMS_BUTTON_MAX];  // マウスの入力情報(リリース情報)
	BYTE   m_aMouseStateRepeat[DIMS_BUTTON_MAX];   // マウスの入力情報(リピート情報)
	DWORD  m_aMouseCounter[DIMS_BUTTON_MAX];       // マウスが入力されてる間を数えるカウンター
	LONG   m_MouseXAxis;                           // マウスのX軸を格納する
	LONG   m_MouseYAxis;                           // マウスのY軸を格納する
	LONG   m_MouseZAxis;                           // マウスのZ軸を格納する
	HWND   m_hWnd;                                 // ウィンドウハンドル(カーソル座標を取得するために必要)
	POINT  m_Point;                                // 現在のマウスのカーソル座標
	POINT  m_PointOld;                             // 前回のマウスのカーソル座標
	D3DXVECTOR3 m_WorldPosZn;                      // 現在のマウスのワールド座標(カメラから近い方)
	D3DXVECTOR3 m_WorldPosOldZn;                   // 前回のマウスのワールド座標(カメラから近い方)
	D3DXVECTOR3 m_WorldPosZf;                      // 現在のマウスのワールド座標(カメラから遠い方)
	D3DXVECTOR3 m_WorldPosOldZf;                   // 前回のマウスのワールド座標(カメラから遠い方)
};

//*****************************************************************************
//     ジョイスティッククラスの定義
//*****************************************************************************
#define DI_JOY_STICK_INPUT   (1000)   // スティックの方向入力受付を行う範囲
#define DI_STICK_AXIS_MIN    (15.0f)  // スティックの入力を開始するスティックの軸の最小値

class CJoyStick
{
public:     // 誰でもアクセス可能
	//------------------------
	//  ボタンの列挙
	//------------------------
	typedef enum
	{
		DIJS_BUTTON_0 = 0,  // Xボタン
		DIJS_BUTTON_1,      // Yボタン
		DIJS_BUTTON_2,      // Aボタン
		DIJS_BUTTON_3,      // Bボタン
		DIJS_BUTTON_4,      // LBボタン
		DIJS_BUTTON_5,      // RBボタン
		DIJS_BUTTON_6,      // LTボタン
		DIJS_BUTTON_7,      // RTボタン
		DIJS_BUTTON_8,      // 左スティック押し込み
		DIJS_BUTTON_9,      // 右スティック押し込み
		DIJS_BUTTON_10,     // BACKボタン
		DIJS_BUTTON_11,     // STARTボタン
		DIJS_BUTTON_12,
		DIJS_BUTTON_13,
		DIJS_BUTTON_14,
		DIJS_BUTTON_15,
		DIJS_BUTTON_16,
		DIJS_BUTTON_17,
		DIJS_BUTTON_18,
		DIJS_BUTTON_19,
		DIJS_BUTTON_20,      // 左アナログスティック上方向入力
		DIJS_BUTTON_21,      // 左アナログスティック下方向入力
		DIJS_BUTTON_22,      // 左アナログスティック右方向入力
		DIJS_BUTTON_23,      // 左アナログスティック左方向入力
		DIJS_BUTTON_24,      // 右アナログスティック上方向入力
		DIJS_BUTTON_25,      // 右アナログスティック下方向入力
		DIJS_BUTTON_26,      // 右アナログスティック右方向入力
		DIJS_BUTTON_27,      // 右アナログスティック左方向入力
		DIJS_BUTTON_28,      // 十字キー上方向入力
		DIJS_BUTTON_29,      // 十字キー下方向入力
		DIJS_BUTTON_30,      // 十字キー右方向入力
		DIJS_BUTTON_31,      // 十字キー左方向入力
		DIJS_BUTTON_MAX
	}DIJS_BUTTON;

	CJoyStick();
	~CJoyStick();
	HRESULT Init(void);
	void Uninit(void);
	void Update(LPDIRECTINPUTDEVICE8 pDevice);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelease(int nButton);
	bool GetRepeat(int nButton);
	float GetRadian(void);
	float GetLeftAxisX(void);
	float GetLeftAxisY(void);
	float GetLeftAxis(void);
	float GetRightAxisX(void);
	float GetRightAxisY(void);
	float GetRightAxis(void);

private:    // このクラスだけがアクセス可能
	BYTE   m_aJoyState[DIJS_BUTTON_MAX];          // ジョイスティックの入力情報(プレス情報)
	BYTE   m_aJoyStateTrigger[DIJS_BUTTON_MAX];   // ジョイスティックの入力情報(トリガー情報)
	BYTE   m_aJoyStateRelease[DIJS_BUTTON_MAX];   // ジョイスティックの入力情報(リリース情報)
	BYTE   m_aJoyStateRepeat[DIJS_BUTTON_MAX];    // ジョイスティックの入力情報(リピート情報)
	DWORD  m_aJoyCounter[DIJS_BUTTON_MAX];        // ジョイスティックの入力されてる間を数えるカウンター
	float  m_Radian;                              // 十字キーのラジアンを格納する
	float  m_LeftAxis_X;                          // 左アナログスティックのX軸を格納する
	float  m_LeftAxis_Y;                          // 左アナログスティックのY軸を格納する
	float  m_RightAxis_X;                         // 右アナログスティックのX軸を格納する
	float  m_RightAxis_Y;                         // 右アナログスティックのY軸を格納する
};

//*****************************************************************************
//     ジョイスティックオブジェクトクラスの定義
//*****************************************************************************
class CInputJoyStick : public CInput
{
public:     // 誰でもアクセス可能
	CInputJoyStick(int nNumDevice = 1);
	~CInputJoyStick();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	static BOOL PASCAL CallEnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	BOOL FAR PASCAL EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	CJoyStick *GetJoyStickDevice(int nIdxDevice = 0);

private:    // このクラスだけがアクセス可能
	static int m_nNumAll;                        // コントローラーのデバイス数
	CJoyStick *m_apJoyStick[MAX_DEVICE];         // ジョイスティッククラス型のポインタ
};

//*****************************************************************************
//     XInputクラスの定義
//*****************************************************************************
class CXInput
{
public:     // 誰でもアクセス可能
#define MIN_GAMEPAD_LEFT_THUMB_X  (25000)   // 左スティックの横方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_LEFT_THUMB_Y  (25000)   // 左スティックの縦方向の入力を受け付ける軸の最小値

#define MIN_GAMEPAD_RIGHT_THUMB_X (25000)   // 右スティックの横方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_RIGHT_THUMB_Y (25000)   // 右スティックの縦方向の入力を受け付ける軸の最小値

#define MIN_GAMEPAD_LEFT_TRIGGER  (130)     // LTボタンの入力を受け付ける値の最小値
#define MIN_GAMEPAD_RIGHT_TRIGGER (130)     // RTボタンの入力を受け付ける値の最小値
			//------------------------
			//  ボタンの列挙
			//------------------------
			// XINPUT_GAMEPAD_DPAD_UP          // 十字キー上入力
			// XINPUT_GAMEPAD_DPAD_DOWN        // 十字キー下入力
			// XINPUT_GAMEPAD_DPAD_LEFT        // 十字キー左入力
			// XINPUT_GAMEPAD_DPAD_RIGHT       // 十字キー右入力
			// XINPUT_GAMEPAD_START            // STARTボタン
			// XINPUT_GAMEPAD_BACK             // BACKボタン
			// XINPUT_GAMEPAD_LEFT_THUMB       // 左スティックボタン
			// XINPUT_GAMEPAD_RIGHT_THUMB      // 右スティックボタン
			// XINPUT_GAMEPAD_LEFT_SHOULDER    // LBボタン
			// XINPUT_GAMEPAD_RIGHT_SHOULDER   // RBボタン
			// XINPUT_GAMEPAD_A                // Aボタン
			// XINPUT_GAMEPAD_B                // Bボタン
			// XINPUT_GAMEPAD_X                // Xボタン
			// XINPUT_GAMEPAD_Y                // Yボタン
	typedef enum
	{
		XIJS_BUTTON_0,     // 十字キー上入力
		XIJS_BUTTON_1,     // 十字キー下入力
		XIJS_BUTTON_2,     // 十字キー左入力
		XIJS_BUTTON_3,     // 十字キー右入力
		XIJS_BUTTON_4,     // STARTボタン
		XIJS_BUTTON_5,     // BACKボタン
		XIJS_BUTTON_6,     // 左スティック押し込み入力
		XIJS_BUTTON_7,     // 右スティック押し込み入力
		XIJS_BUTTON_8,     // LBボタン
		XIJS_BUTTON_9,     // RBボタン
		XIJS_BUTTON_10,    // Aボタン
		XIJS_BUTTON_11,    // Bボタン
		XIJS_BUTTON_12,    // Xボタン
		XIJS_BUTTON_13,    // Yボタン
		XIJS_BUTTON_14,    // LTボタン
		XIJS_BUTTON_15,    // RTボタン
		XIJS_BUTTON_16,    // 左スティック上入力
		XIJS_BUTTON_17,    // 左スティック下入力
		XIJS_BUTTON_18,    // 左スティック左入力
		XIJS_BUTTON_19,    // 左スティック右入力
		XIJS_BUTTON_20,    // 右スティック上入力
		XIJS_BUTTON_21,    // 右スティック下入力
		XIJS_BUTTON_22,    // 右スティック左入力
		XIJS_BUTTON_23,    // 右スティック右入力

						   // 以下割り当てなし
						   XIJS_BUTTON_24,
						   XIJS_BUTTON_25,
						   XIJS_BUTTON_26,
						   XIJS_BUTTON_27,
						   XIJS_BUTTON_28,
						   XIJS_BUTTON_29,
						   XIJS_BUTTON_30,
						   XIJS_BUTTON_31,
						   XIJS_BUTTON_MAX,
	}XIJS_BUTTON;

	CXInput();
	~CXInput();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	BYTE GetLeftTrigger(int nCntPad);
	BYTE GetRightTrigger(int nCntPad);
	float GetLeftAxiz(int nCntPad);
	float GetRightAxiz(int nCntPad);
	SHORT GetThumbLX(int nCntPad);
	SHORT GetThumbLY(int nCntPad);
	SHORT GetThumbRX(int nCntPad);
	SHORT GetThumbRY(int nCntPad);

	bool GetPress(int nCntPad, XIJS_BUTTON Button);
	bool GetTrigger(int nCntPad, XIJS_BUTTON Button);
	bool GetRelease(int nCntPad, XIJS_BUTTON Button);
	bool GetRepeat(int nCntPad, XIJS_BUTTON Button);

private:    // このクラスだけがアクセス可能
	typedef struct
	{
		XINPUT_STATE m_State;     // ゲームパッドの状態
		bool m_bConnected;        // 接続されたかどうか
	}CONTROLER_STATE;

	CONTROLER_STATE m_aGamePad[MAX_DEVICE];                                // ゲームパッドの情報
	DWORD           m_wButtons[MAX_DEVICE];                                // ボタンの状態
	BYTE            m_bLeftTrigger[MAX_DEVICE];                            // LTボタンの状態
	BYTE            m_bRightTrigger[MAX_DEVICE];                           // RTボタンの状態
	SHORT           m_sThumbLX[MAX_DEVICE];                                // 左スティックのX軸
	SHORT           m_sThumbLY[MAX_DEVICE];                                // 左スティックのY軸
	SHORT           m_sThumbRX[MAX_DEVICE];                                // 右スティックのX軸
	SHORT           m_sThumbRY[MAX_DEVICE];                                // 右スティックのY軸
	BYTE            m_aGamePadState[MAX_DEVICE][XIJS_BUTTON_MAX];          // ゲームパッドの入力情報(プレス情報)
	BYTE            m_aGamePadStateTrigger[MAX_DEVICE][XIJS_BUTTON_MAX];   // ゲームパッドの入力情報(トリガー情報)
	BYTE            m_aGamePadStateRelease[MAX_DEVICE][XIJS_BUTTON_MAX];   // ゲームパッドの入力情報(リリース情報)
	BYTE            m_aGamePadStateRepeat[MAX_DEVICE][XIJS_BUTTON_MAX];    // ゲームパッドの入力情報(リピート情報)
	DWORD           m_aGamePadCounter[MAX_DEVICE][XIJS_BUTTON_MAX];        // ゲームパッドの入力されてる間を数えるカウンター
};

#endif