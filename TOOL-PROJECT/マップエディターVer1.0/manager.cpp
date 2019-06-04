//=============================================================================
//
// マネージャ処理 [manager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "basemode.h"
#include "editor.h"
#include "demoplay.h"
#include "fade.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CManager::MODE	CManager::m_mode = MODE::MODE_EDITOR;		//モードの情報
CRenderer		*CManager::m_pRenderer = NULL;				//レンダリングポインタ
CInputKeyboard	*CManager::m_pInputKeyboard = NULL;			//キーボードポインタ
CInputMouse     *CManager::m_pMouse = NULL;                 // マウスクラス型のポインタ
CInputJoyStick	*CManager::m_pInputJoypad = NULL;			//ジョイパッドポインタ
CSound			*CManager::m_pSound = NULL;					//サウンドのポインタ
CDebugProc		*CManager::m_pDebugproc = NULL;				//デバッグのポインタ
CBasemode		*CManager::m_pBasemode = NULL;				//ベースモードのポインタ
CFade			*CManager::m_pFade = NULL;					//フェードのポインタ

//=============================================================================
// マネージャのコンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// マネージャのデストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// マネージャの初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダリングの動的確保
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;			//レンダリングの動的確保
	}

	//キーボードの動的確保
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = new CInputKeyboard;	//キーボードの動的確保
	}

	// マウスの作成
	if (m_pMouse == NULL)
	{// メモリが確保できる状態にある
		m_pMouse = new CInputMouse;
		if (m_pMouse != NULL)
		{// メモリが確保できた
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	// ジョイスティックの作成
	if (m_pInputJoypad == NULL)
	{// メモリが確保できる状態にある
		m_pInputJoypad = new CInputJoyStick(1);
		if (m_pInputJoypad != NULL)
		{// メモリが確保できた
			m_pInputJoypad->Init(hInstance, hWnd);
		}
	}

	//サウンドの動的確保
	//if (m_pSound == NULL)
	//{
	//	m_pSound = new CSound;					//サウンドの動的確保
	//}

	//デバッグの動的確保
	if (m_pDebugproc == NULL)
	{
		m_pDebugproc = new CDebugProc;
	}

	//レンダリングの初期化処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Init(hWnd, TRUE);
	}

	//キーボードの初期化処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Init(hInstance,hWnd);
	}

	//サウンドの初期化処理
	/*if (m_pSound != NULL)
	{
		m_pSound->InitSound(hWnd);
	}*/

	//デバッグの初期化処理
	if (m_pDebugproc != NULL)
	{
		m_pDebugproc->Init();
	}

	//フェードの生成
	m_pFade = CFade::Create();
	//フェードの設置処理
	m_pFade->SetFade(m_mode, m_pFade->FADE_IN);

	//モードの設置
	//SetMode(m_mode);

	return S_OK;
}

//=============================================================================
// マネージャの終了処理
//=============================================================================
void CManager::Uninit(void)
{
	//キーボードの終了処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// マウスの破棄
	if (m_pMouse != NULL)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = NULL;
	}

	//ジョイパッドの終了処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	//レンダリングの終了処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//フェードの終了処理
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}
	//サウンドの終了処理
	//if (m_pSound != NULL)
	//{
	//	m_pSound->UninitSound();
	//	delete m_pSound;
	//	m_pSound = NULL;
	//}

	//デバッグの終了処理
	if (m_pDebugproc != NULL)
	{
		m_pDebugproc->Uninit();
		delete m_pDebugproc;
		m_pDebugproc = NULL;
	}

	if (m_pBasemode != NULL)
	{
		// ベースモードの終了処理
		m_pBasemode->Uninit();
		// メモリの開放
		delete m_pBasemode;
		m_pBasemode = NULL;
	}

	//全ての開放処理
	CScene::ReleaseAll();
}

//=============================================================================
// マネージャの更新処理
//=============================================================================
void CManager::Update(void)
{
	//キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	if (m_pMouse != NULL)
	{// メモリが確保されている
		m_pMouse->Update();
	}

	//ジョイパッドの更新処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();

	}

	//フェードの更新処理
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}
	//レンダリングの更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// ベースモードの更新処理
	if (m_pBasemode != NULL)
	{
		m_pBasemode->Update();
	}
}

//=============================================================================
// マネージャの描画処理
//=============================================================================
void CManager::Draw(void)
{
	//レンダリングの描画処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// レンダリングの取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;	//値を返す
}

//=============================================================================
// キーボードの取得
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pInputKeyboard;	//値を返す
}

//=============================================================================
//    マウスクラスのポインタを取得
//=============================================================================
CInputMouse *CManager::GetMouse(void)
{
	return m_pMouse;
}

//=============================================================================
// ジョイパッドの取得
//=============================================================================
CInputJoyStick *CManager::GetJoypad(void)
{
	return m_pInputJoypad;	//値を返す
}

//=============================================================================
// サウンドの取得
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;				//値を返す
}

//=============================================================================
// ベースモードの取得
//=============================================================================
CBasemode *CManager::GetBaseMode(void)
{
	return m_pBasemode;				//値を返す
}

//=============================================================================
// エディターの取得
//=============================================================================
CEditor *CManager::GetEditor(void)
{
	return (CEditor*)m_pBasemode;
}

//=============================================================================
// デモプレイの取得
//=============================================================================
CDemoplay *CManager::GetDemoplay(void)
{
	return (CDemoplay*)m_pBasemode;
}

//=============================================================================
// モードの取得
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;				//値を返す
}
//=============================================================================
// モードの取得
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;				//値を返す
}

//=============================================================================
// モードの設置
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// 前回のモードを破棄
	if (m_pBasemode != NULL)
	{
		m_pBasemode->Uninit();
		delete m_pBasemode;
		m_pBasemode = NULL;
	}

	m_mode = mode;	//代入

	switch (mode)
	{
	case MODE_EDITOR:					//エディターの場合
		if (m_pBasemode == NULL)
		{
			//エディターの生成
			m_pBasemode = CEditor::Create();
		}
		break;

	case MODE_DEMOPLAY:                  // デモプレイの場合
		if (m_pBasemode == NULL)
		{
			//デモプレイの生成
			m_pBasemode = CDemoplay::Create();
		}
		break;
	}
}