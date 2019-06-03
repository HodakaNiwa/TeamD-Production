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
class CBasemode;
class CGame;
class CFade;
class CDebugProc;

//*****************************************************************************
//    マネージャクラスの定義
//*****************************************************************************
class CManager
{
public:    // 誰でもアクセス可能
	typedef enum
	{
		MODE_NONE = 0,
		MODE_GAME,
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void	SetMode(MODE mode);
	static MODE	GetMode(void);
	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick *GetJoyStick(void);
	static CClient *GetClient(void);
	static CBasemode *GetBaseMode(void);
	static CGame *GetGame(void);
	static CFade *GetFade(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateServer(void);
	void CreateClient(void);

	static CRenderer *m_pRenderer;              // レンダリングクラス型のポインタ
	static CInputKeyboard *m_pInputKeyboard;	// キーボートクラス型のポインタ
	static CInputMouse *m_pMouse;               // マウスクラス型のポインタ
	static CInputJoyStick *m_pInputJoyStick;	// ジョイパッドクラス型のポインタ
	static CServer *m_pServer;                  // サーバークラス型のポインタ
	static CClient *m_pClient;                  // クライアントクラス型のポインタ
	static MODE	m_mode;				            // 現在のモード
	static CBasemode *m_pBasemode;		        // ベースモードクラス型のポインタ
	static CFade *m_pFade;			            // フェードクラス型のポインタ
	static CDebugProc *m_pDebugProc;            // デバッグログクラス型のポインタ
};

#endif