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


#ifdef SERVER_MODE
	if (m_pServer != NULL)
	{
		m_pServer->Update();
	}
#else
	if (m_pClient != NULL)
	{
		m_pClient->Update();
	}
#endif
	if (m_pRenderer != NULL)
	{// メモリが確保されている
	    // レンダリングの更新処理
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
		{
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