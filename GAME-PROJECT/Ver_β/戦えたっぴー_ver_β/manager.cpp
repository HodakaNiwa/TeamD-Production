//=============================================================================
//
// マネージャ処理 [manager.cpp]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#include "manager.h"
#include "fileLoader.h"
#include "functionlib.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "server.h"
#include "debugproc.h"
#include "basemode.h"
#include "title.h"
#include "charaselect.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MANAGER_SYSTEM_FILENAME "data/TEXT/system.ini"   // 読み込むシステムファイル名

// 値読み込み用のパス名
#define SERVER_ADDRESS "SERVER_ADDRESS = "
#define SOUND_FILENAME "SOUND_FILENAME = "

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CManager::MODE	CManager::m_mode = MODE::MODE_GAME;  		// モードの情報
CRenderer		*CManager::m_pRenderer = NULL;				// レンダリングのポインタ
CInputKeyboard	*CManager::m_pInputKeyboard = NULL;			// キーボードのポインタ
CInputMouse     *CManager::m_pMouse = NULL;                 // マウスのポインタ
CInputJoyStick	*CManager::m_pInputJoypad = NULL;			// ジョイパッドポインタ
CXInput	        *CManager::m_pXInput = NULL;			    // XInputポインタ
CSound			*CManager::m_pSound = NULL;					// サウンドのポインタ
CClient         *CManager::m_pClient = NULL;                // クライアントのポインタ
CDebugProc		*CManager::m_pDebugproc = NULL;				// デバッグのポインタ
CBasemode		*CManager::m_pBasemode = NULL;				// ベースモードのポインタ
CFade			*CManager::m_pFade = NULL;					// フェードのポインタ
char            CManager::m_aServerAddr[256] = "\0";        // サーバーのIPアドレス

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
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// サーバーのスタートアップ
	CServer::ServerStartUp();

	// レンダリングの生成
	CreateRenderer(hWnd, bWindow);

	// インプットの生成
	CreateInput(hInstance, hWnd);

	// フェードの生成
	CreateFade();

	// デバッグログの生成
	CreateDebugProc();

	// システムを読み込む
	LoadSystem(hWnd);

	// ゲームかチュートリアルから開始の場合クライアントを生成する処理を入れる
	if (m_mode == MODE_GAME || m_mode == MODE_TUTORIAL)
	{
		CreateClient();
	}

	return S_OK;
}

//=============================================================================
// マネージャの終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// クライアントの破棄
	ReleaseClient();

	// ベースモードの破棄
	if (m_pBasemode != NULL)
	{
		m_pBasemode->Uninit();
		delete m_pBasemode;
		m_pBasemode = NULL;
	}

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
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// XInputの破棄
	if (m_pXInput != NULL)
	{
		m_pXInput->Uninit();
		delete m_pXInput;
		m_pXInput = NULL;
	}

	// レンダリングの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// フェードの破棄
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}

	// サウンドの破棄
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// デバッグログの破棄
	if (m_pDebugproc != NULL)
	{
		m_pDebugproc->Uninit();
		delete m_pDebugproc;
		m_pDebugproc = NULL;
	}

	//全ての開放処理
	CScene::ReleaseAll();

	// サーバーのクリーンアップ
	CServer::ServerCleanUp();
}

//=============================================================================
// マネージャの更新処理
//=============================================================================
void CManager::Update(void)
{
	// デバッグコマンド
	if (m_pInputKeyboard->GetTrigger(DIK_F1) == true)
	{
		m_pDebugproc->SwitchDisp();
	}

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
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();

	}

	// XInputの更新処理
	if (m_pXInput != NULL)
	{
		m_pXInput->Update();
	}

	// フェードの更新処理
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	// レンダリングの更新処理
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
	// レンダリングの描画処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// システムを読み込む
//=============================================================================
void CManager::LoadSystem(HWND hWnd)
{
	// ファイルオープン
	CFileLoader *pFileLoader = CFileLoader::Create(MANAGER_SYSTEM_FILENAME);
	if (pFileLoader == NULL) return;

	// スクリプトを読み込む
	char aStr[256] = "\0";
	strcpy(aStr, pFileLoader->GetString(aStr));
	if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
	{// スクリプト読み込み開始の合図だった
		LoadSystemScript(hWnd, pFileLoader, aStr);
	}

	// ファイルを閉じる
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}
}

//=============================================================================
// スクリプトを読み込む
//=============================================================================
void CManager::LoadSystemScript(HWND hWnd, CFileLoader *pFileLoader, char *pStr)
{
	// サーバーのIPアドレスを読み込む
	strcpy(pStr, pFileLoader->GetString(pStr));
	strcpy(m_aServerAddr, CFunctionLib::ReadString(pStr, m_aServerAddr, SERVER_ADDRESS));


	// サウンドデータのファイル名を読み込む
	char aSoundFileName[256] = "\0";
	strcpy(pStr, pFileLoader->GetString(pStr));
	strcpy(aSoundFileName, CFunctionLib::ReadString(pStr, aSoundFileName, SOUND_FILENAME));
	CreateSound(hWnd, aSoundFileName);
}

//=============================================================================
// レンダリングの生成
//=============================================================================
void CManager::CreateRenderer(HWND hWnd, bool bWindow)
{
	if (m_pRenderer == NULL)
	{// メモリが確保できる状態にある
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{// メモリが確保できた
			m_pRenderer->Init(hWnd, bWindow);
		}
	}
}

//=============================================================================
// インプットの生成
//=============================================================================
void CManager::CreateInput(HINSTANCE hInstance, HWND hWnd)
{
	// キーボードの生成
	CreateKeyboard(hInstance, hWnd);

	// マウスの生成
	CreateMouse(hInstance, hWnd);

	// ジョイスティックの生成
	CreateJoyStick(hInstance, hWnd);

	// XInputの生成
	CreateXInput();
}

//=============================================================================
// キーボードの生成
//=============================================================================
void CManager::CreateKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// キーボードの作成
	if (m_pInputKeyboard == NULL)
	{// メモリが確保できる状態にある
		m_pInputKeyboard = new CInputKeyboard;
		if (m_pInputKeyboard != NULL)
		{// メモリが確保できた
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}
}

//=============================================================================
// マウスの生成
//=============================================================================
void CManager::CreateMouse(HINSTANCE hInstance, HWND hWnd)
{
	// マウスの作成
	if (m_pMouse == NULL)
	{// メモリが確保できる状態にある
		m_pMouse = new CInputMouse;
		if (m_pMouse != NULL)
		{// メモリが確保できた
			m_pMouse->Init(hInstance, hWnd);
		}
	}
}

//=============================================================================
// ジョイスティックの生成
//=============================================================================
void CManager::CreateJoyStick(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInputJoypad == NULL)
	{// メモリが確保できる状態にある
		m_pInputJoypad = new CInputJoyStick(1);
		if (m_pInputJoypad != NULL)
		{// メモリが確保できた
			m_pInputJoypad->Init(hInstance, hWnd);
		}
	}
}

//=============================================================================
// XInputの生成
//=============================================================================
void CManager::CreateXInput(void)
{
	if (m_pXInput == NULL)
	{// メモリが確保できる状態にある
		m_pXInput = new CXInput;
		if (m_pXInput != NULL)
		{// メモリが確保できた
			m_pXInput->Init();
		}
	}
}

//=============================================================================
// サウンドの生成
//=============================================================================
void CManager::CreateSound(HWND hWnd, char *pFileName)
{
	if (m_pSound == NULL)
	{// メモリが確保できる状態にある
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{// メモリが確保できた
			m_pSound->Init(hWnd, pFileName);
		}
	}
}

//=============================================================================
// フェードの生成
//=============================================================================
void CManager::CreateFade(void)
{
	m_pFade = CFade::Create();
	m_pFade->SetFade(m_mode, m_pFade->FADE_IN);
}

//=============================================================================
// デバッグログの生成
//=============================================================================
void CManager::CreateDebugProc(void)
{
	if (m_pDebugproc == NULL)
	{// メモリが確保できる状態にある
		m_pDebugproc = new CDebugProc;
		if (m_pDebugproc != NULL)
		{// メモリが確保できた
			m_pDebugproc->Init();
		}
	}
}

//=============================================================================
// クライアントの生成
//=============================================================================
void CManager::CreateClient(void)
{
	m_pClient = CClient::Create(m_aServerAddr);
}

//=============================================================================
// クライアントの更新
//=============================================================================
void CManager::ClientUpdate(void)
{
	if (m_pClient != NULL)
	{
		m_pClient->Update();
	}
}

//=============================================================================
// クライアントの開放
//=============================================================================
void CManager::ReleaseClient(void)
{
	if (m_pClient != NULL)
	{
		m_pClient->Uninit();
		delete m_pClient;
		m_pClient = NULL;
	}
}

//=============================================================================
// レンダリングの取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
// キーボードの取得
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
// マウスクラスの取得
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
	return m_pInputJoypad;
}

//=============================================================================
// XInputの取得
//=============================================================================
CXInput *CManager::GetXInput(void)
{
	return m_pXInput;
}

//=============================================================================
// サウンドの取得
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
// クライアントの取得
//=============================================================================
CClient *CManager::GetClient(void)
{
	return m_pClient;
}

//=============================================================================
// ベースモードの取得
//=============================================================================
CBasemode *CManager::GetBaseMode(void)
{
	return m_pBasemode;
}

//=============================================================================
// タイトルの取得
//=============================================================================
CTitle *CManager::GetTitle(void)
{
	return (CTitle*)m_pBasemode;
}

//=============================================================================
// キャラセレクトの取得
//=============================================================================
CCharaSelect *CManager::GetCharaSelect(void)
{
	return (CCharaSelect*)m_pBasemode;
}

//=============================================================================
// ゲームの取得
//=============================================================================
CGame *CManager::GetGame(void)
{
	return (CGame*)m_pBasemode;
}

//=============================================================================
// リザルトの取得
//=============================================================================
CResult *CManager::GetResult(void)
{
	return (CResult*)m_pBasemode;
}

//=============================================================================
// モードの取得
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
// モードの取得
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;
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
	{// モードのよって処理わけ
	case MODE_TITLE:        // タイトル
		if (m_pBasemode == NULL)
		{
			m_pBasemode = CTitle::Create();
		}
		break;
	case MODE_CHARASELECT:  // キャラセレクト
		if (m_pBasemode == NULL)
		{
			m_pBasemode = CCharaSelect::Create();
		}
		break;
	case MODE_TUTORIAL:     // チュートリアル
		if (m_pBasemode == NULL)
		{
			m_pBasemode = CTutorial::Create();
		}
		break;
	case MODE_GAME:         // ゲーム
		if (m_pBasemode == NULL)
		{
			m_pBasemode = CGame::Create();
		}
		break;
	case MODE_RESULT:       // リザルト
		if (m_pBasemode == NULL)
		{
			m_pBasemode = CResult::Create();
		}
		break;
	}
}