//*****************************************************************************
//
//     マネージャーの処理[manager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "server.h"
#include "fileLoader.h"
#include "functionlib.h"
#include "basemode.h"
#include "game.h"
#include "fade.h"
#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MANAGER_SYSTEM_FILENAME "data/system.ini"
#define SERVER_ADDRESS "SERVER_ADDRESS = "

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;              // レンダリングクラス型のポインタ
CInputKeyboard *CManager::m_pInputKeyboard = NULL;    // キーボードクラス型のポインタ
CInputMouse *CManager::m_pMouse = NULL;               // マウスクラス型のポインタ
CInputJoyStick *CManager::m_pInputJoyStick = NULL;	  // ジョイパッドクラス型のポインタ
CServer *CManager::m_pServer = NULL;                  // サーバークラス型のポインタ
CClient *CManager::m_pClient = NULL;                  // クライアントクラス型のポインタ
CManager::MODE CManager::m_mode = MODE::MODE_GAME;    // 現在のモード
CBasemode *CManager::m_pBasemode = NULL;              // ベースモードクラス型のポインタ
CFade *CManager::m_pFade = NULL;			          // フェードクラス型へのポインタ
CDebugProc *CManager::m_pDebugProc = NULL;            // デバッグログクラス型のポインタ

//=============================================================================
//    コンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// サーバーのスタートアップ
	CServer::ServerStartUp();

	// レンダリングの作成
	if (m_pRenderer == NULL)
	{// メモリを確保できる状態にある
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{// メモリを確保できた
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))  // FALSEにするとフルスクリーン
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// キーボードの作成
	if (m_pInputKeyboard == NULL)
	{// メモリが確保できる状態にある
		m_pInputKeyboard = new CInputKeyboard;
		if (m_pInputKeyboard != NULL)
		{// メモリが確保できた
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
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
	if (m_pInputJoyStick == NULL)
	{// メモリが確保できる状態にある
		m_pInputJoyStick = new CInputJoyStick(1);
		if (m_pInputJoyStick != NULL)
		{// メモリが確保できた
			m_pInputJoyStick->Init(hInstance, hWnd);
		}
	}

	// デバッグログクラスの作成
	if (m_pDebugProc == NULL)
	{
		m_pDebugProc = new CDebugProc;
		if (m_pDebugProc != NULL)
		{
			m_pDebugProc->Init();
		}
	}


#ifdef SERVER_MODE
	CreateServer();
#else
	CreateClient();
#endif

	// フェードの生成
	m_pFade = CFade::Create();
	if (m_pFade != NULL)
	{
		// フェードの設定
		m_pFade->SetFade(m_mode, m_pFade->FADE_IN);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// キーボードの破棄
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

	// ジョイパッドの破棄
	if (m_pInputJoyStick != NULL)
	{
		m_pInputJoyStick->Uninit();
		delete m_pInputJoyStick;
		m_pInputJoyStick = NULL;
	}

	// デバッグログの破棄
	if (m_pDebugProc != NULL)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	// レンダリングの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// サーバーの破棄
	if (m_pServer != NULL)
	{
		m_pServer->Uninit();
		delete m_pServer;
		m_pServer = NULL;
	}

	// クライアントの破棄
	if (m_pClient != NULL)
	{
		m_pClient->Uninit();
		delete m_pClient;
		m_pClient = NULL;
	}

	// サーバーのクリーンアップ
	CServer::ServerCleanUp();
}

//=============================================================================
//    更新処理
//=============================================================================
void CManager::Update(void)
{
	// キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// マウスの更新処理
	if (m_pMouse != NULL)
	{
		m_pMouse->Update();
	}

	// ジョイパッドの更新処理
	if (m_pInputJoyStick != NULL)
	{
		m_pInputJoyStick->Update();

	}

	// フェードの更新処理
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	// ベースモードの更新処理
	if (m_pBasemode != NULL)
	{
		m_pBasemode->Update();
	}

#ifdef SERVER_MODE
	// サーバーの更新処理
	if (m_pServer != NULL)
	{
		m_pServer->Update();
	}
#else
	// クライアントの更新処理
	if (m_pClient != NULL)
	{
		m_pClient->Update();
	}
#endif

	// レンダリングの更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// メモリが確保されている
		m_pRenderer->Draw();
	}

	// デバッグログの文字列クリア
	CDebugProc::ClearString();
}

//=============================================================================
//    サーバーを生成する
//=============================================================================
void CManager::CreateServer(void)
{
	if (m_pServer == NULL)
	{
		m_pServer = CServer::Create();
	}
}

//=============================================================================
//    クライアントを生成する
//=============================================================================
void CManager::CreateClient(void)
{
	char aStr[256];
	char aServerAddr[256] = "\0";
	CFileLoader *pFileLoader = CFileLoader::Create(MANAGER_SYSTEM_FILENAME);
	if (pFileLoader != NULL)
	{
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SERVER_ADDRESS) == 0)
		{// サーバーのIPアドレスだった
			strcpy(aServerAddr, CFunctionLib::ReadString(aStr, aServerAddr, SERVER_ADDRESS));
			if (m_pClient == NULL)
			{
				//m_pClient = CClient::Create("127.0.1.1");  <- ローカルPC上のホストアドレスなので他PCとは通信できない!
				m_pClient = CClient::Create(aServerAddr);
			}
		}

		if (pFileLoader != NULL)
		{
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
//    現在のモードを設定
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
	case MODE_GAME:    // ゲーム
		m_pBasemode = CGame::Create();
		break;
	}
}

//=============================================================================
//    現在のモードを取得
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
//    レンダリングクラスのポインタを取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
//    キーボードクラスのポインタを取得
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
//    マウスクラスのポインタを取得
//=============================================================================
CInputMouse *CManager::GetMouse(void)
{
	return m_pMouse;
}

//=============================================================================
//    ジョイスティッククラスのポインタを取得
//=============================================================================
CInputJoyStick *CManager::GetJoyStick(void)
{
	return m_pInputJoyStick;
}

//=============================================================================
//    クライアントクラスのポインタを取得
//=============================================================================
CClient *CManager::GetClient(void)
{
	return m_pClient;
}

//=============================================================================
//    ベースモードクラスのポインタを取得
//=============================================================================
CBasemode *CManager::GetBaseMode(void)
{
	return m_pBasemode;
}

//=============================================================================
//    ゲームクラスのポインタを取得
//=============================================================================
CGame *CManager::GetGame(void)
{
	return (CGame*)m_pBasemode;
}

//=============================================================================
//    フェードクラスのポインタを取得
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;
}