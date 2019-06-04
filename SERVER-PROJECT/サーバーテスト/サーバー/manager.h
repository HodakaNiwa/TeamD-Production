//*****************************************************************************
//
//     マネージャーの処理[manager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言(実体がない宣言)
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CServer;
class CClient;
class CDebugProc;

//*****************************************************************************
//    マネージャクラスの定義
//*****************************************************************************
class CManager
{
public:    // 誰でもアクセス可能
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick *GetJoyStick(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateServer(void);
	void CreateClient(void);

	static CInputKeyboard *m_pInputKeyboard;	// キーボートクラス型へのポインタ
	static CInputMouse *m_pMouse;               // マウスクラス型へのポインタ
	static CInputJoyStick *m_pInputJoyStick;	// ジョイパッドクラス型へのポインタ
	static CServer *m_pServer;                  // サーバークラス型へのポインタ
	static CClient *m_pClient;                  // クライアントクラス型へのポインタ
	static CRenderer *m_pRenderer;              // レンダリングクラス型へのポインタ
	static CDebugProc *m_pDebugProc;            // デバッグログクラスへのポインタ
};

#endif