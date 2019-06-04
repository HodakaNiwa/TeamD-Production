//=============================================================================
//
// マネージャ処理 [manager.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//前方宣言
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CSound;
class CCamera;
class CLight;
class CDebugProc;
class CBasemode;
class CEditor;
class CDemoplay;
class CFade;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CManager
{
public:	//誰からもアクセス可能
	typedef enum
	{
		MODE_NONE = 0,
		MODE_EDITOR,
		MODE_DEMOPLAY,
		MODE_MAX,
	}MODE;

	//メンバ関数
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CRenderer		*GetRenderer(void);
	static CInputKeyboard	*GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick		*GetJoypad(void);
	static CSound			*GetSound(void);
	static CBasemode        *GetBaseMode(void);
	static CEditor          *GetEditor(void);
	static CDemoplay        *GetDemoplay(void);
	static MODE				GetMode(void);
	static void				SetMode(MODE mode);
	static CFade			*GetFade(void);

private:	//自分だけがアクセス可能
	//静的メンバ変数
	static CRenderer		*m_pRenderer;		//レンダリングのポインタ
	static CInputKeyboard	*m_pInputKeyboard;	//キーボートのポインタ
	static CInputMouse       *m_pMouse;         // マウスクラス型のポインタ
	static CInputJoyStick   *m_pInputJoypad;	//ジョイパッドのポインタ
	static CSound			*m_pSound;			//サウンドのポインタ
	static CDebugProc		*m_pDebugproc;		//デバッグのポインタ
	static MODE				m_mode;				//モードの情報
	static CBasemode		*m_pBasemode;		//ベースモードのポインタ
	static CFade			*m_pFade;			//フェードのポインタ

protected:	//自分と派生クラスだけがアクセス可能
};
#endif