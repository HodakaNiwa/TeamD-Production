//=============================================================================
//
// マネージャ処理 [manager.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//前方宣言
class CFileLoader;
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CXInput;
class CSound;
class CCamera;
class CLight;
class CDebugProc;
class CClient;
class CBasemode;
class CTitle;
class CCharaSelect;
class CGame;
class CResult;
class CFade;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CManager
{
public:	//誰からもアクセス可能
	typedef enum
	{
		MODE_NONE = -1,
		MODE_TITLE,
		MODE_CHARASELECT,
		MODE_GAME,
		MODE_TUTORIAL,
		MODE_RESULT,
		MODE_MAX,
	}MODE;

	//メンバ関数
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static void CreateClient(void);
	static void ClientUpdate(void);
	static void ReleaseClient(void);

	static CRenderer		*GetRenderer(void);
	static CInputKeyboard	*GetKeyboard(void);
	static CInputMouse      *GetMouse(void);
	static CInputJoyStick   *GetJoypad(void);
	static CXInput          *GetXInput(void);
	static CSound			*GetSound(void);
	static CClient          *GetClient(void);
	static CBasemode        *GetBaseMode(void);
	static CTitle           *GetTitle(void);
	static CCharaSelect     *GetCharaSelect(void);
	static CGame            *GetGame(void);
	static CResult          *GetResult(void);
	static MODE				GetMode(void);
	static void				SetMode(MODE mode);
	static CFade			*GetFade(void);

protected:	//自分と派生クラスだけがアクセス可能

private:	//自分だけがアクセス可能
	void LoadSystem(HWND hWnd);
	void LoadSystemScript(HWND hWnd, CFileLoader *pFileLoader, char *pStr);
	void CreateRenderer(HWND hWnd, bool bWindow);
	void CreateInput(HINSTANCE hInstance, HWND hWnd);
	void CreateKeyboard(HINSTANCE hInstance, HWND hWnd);
	void CreateMouse(HINSTANCE hInstance, HWND hWnd);
	void CreateJoyStick(HINSTANCE hInstance, HWND hWnd);
	void CreateXInput(void);
	void CreateSound(HWND hWnd, char *pFileName);
	void CreateFade(void);
	void CreateDebugProc(void);

	//静的メンバ変数
	static CRenderer		*m_pRenderer;		// レンダリングのポインタ
	static CInputKeyboard	*m_pInputKeyboard;	// キーボートのポインタ
	static CInputMouse      *m_pMouse;          // マウスクラス型のポインタ
	static CInputJoyStick   *m_pInputJoypad;	// ジョイパッドのポインタ
	static CXInput          *m_pXInput;         // XInputクラス型のポインタ
	static CSound			*m_pSound;			// サウンドのポインタ
	static CClient          *m_pClient;         // クライアントのポインタ
	static CDebugProc		*m_pDebugproc;		// デバッグのポインタ
	static MODE				m_mode;				// モードの情報
	static CBasemode		*m_pBasemode;		// ベースモードのポインタ
	static CFade			*m_pFade;			// フェードのポインタ
	static char             m_aServerAddr[256]; // サーバーのIPアドレス
};
#endif