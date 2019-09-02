//=============================================================================
//
// メイン処理 [main.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			            // ウインドウのクラス名
#define WINDOW_NAME		"戦えたっぴーver_Master"        // ウインドウのキャプション名
#define WINDOW_MODE     (true)                          // ウィンドウのモード(falseにするとフルスクリーン)
#define WINDOW_ICON     (MAKEINTRESOURCE(IDI_ICON1))    // ウィンドウのアイコン識別子
#define WINDOW_SM_ICON  (MAKEINTRESOURCE(IDI_ICON1))    // ウィンドウのスモールアイコン識別子

#ifdef _DEBUG
#define MEMORY_CHECK    (false)                         // メモリリークチェックをするかどうか
#endif

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************


//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	if (MEMORY_CHECK)
	{// メモリリークチェックをするなら
	 // メモリリークチェックのフラグを立てる
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}
#endif

	CManager *pManager = NULL;	//マネージャのポインタ

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, WINDOW_ICON),
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		(HICON)LoadImage(hInstance, WINDOW_SM_ICON,  IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED)
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	int nCntFPS = 0;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	if (pManager == NULL)
	{
		pManager = new CManager;	//マネージャの動的確保
	}

	if (pManager != NULL)
	{
		// マネージャの初期化処理
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
		{
			return -1;
		}
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwCurrentTime =
	dwFrameCount = 0;
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();	// 現在の時間を取得
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒ごとに実行
				// FPSを算出
				nCntFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

				// FPS描画
				CDebugProc::Print(1, "FPS:%d\n", nCntFPS);

				// 更新処理
				pManager->Update();

				// 描画処理
				pManager->Draw();

				dwFrameCount++;
			}
		}
	}

	if (pManager != NULL)
	{
		// 終了処理
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

#ifdef _DEBUG
	if (MEMORY_CHECK)
	{// メモリリークチェックをしていたなら
	 // この時点で開放されていないメモリの情報の表示
		_CrtDumpMemoryLeaks();
	}
#endif

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		CManager::ReleaseClient();
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			CManager::ReleaseClient();
			DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}