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
#include "camera.h"
#include "light.h"
#include "scene3D.h"
#include "sceneX.h"
#include "debugproc.h"
#include "billboard.h"
#include "meshfield.h"
#include "editor.h"
#include "fade.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CManager::MODE	CManager::m_mode = MODE::MODE_GAME;		//モードの情報
CRenderer		*CManager::m_pRenderer = NULL;				//レンダリングポインタ
CInputKeyboard	*CManager::m_pInputKeyboard = NULL;			//キーボードポインタ
CInputJoypad	*CManager::m_pInputJoypad = NULL;			//ジョイパッドポインタ
CSound			*CManager::m_pSound = NULL;					//サウンドのポインタ
CCamera			*CManager::m_pCamera = NULL;				//カメラのポインタ
CLight			*CManager::m_pLight = NULL;					//ライトのポインタ
CDebugProc		*CManager::m_pDebugproc = NULL;				//デバッグのポインタ
CTitle			*CManager::m_pTitle = NULL;					//タイトルのポインタ
CTutorial		*CManager::m_pTutorial = NULL;				//チュートリアルのポインタ
CEditor			*CManager::m_pEditor = NULL;					//ゲームのポインタ
CResult			*CManager::m_pResult = NULL;				//リザルトのポインタ
CRanking		*CManager::m_pRanking = NULL;				//ランキングのポインタ
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

	//ジョイパッドの動的確保
	if (m_pInputJoypad == NULL)
	{
		m_pInputJoypad = new CInputJoypad;		//ジョイパッドの動的確保
	}

	//サウンドの動的確保
	//if (m_pSound == NULL)
	//{
	//	m_pSound = new CSound;					//サウンドの動的確保
	//}

	//カメラの動的確保
	if (m_pCamera == NULL)
	{
		m_pCamera = new CCamera;
	}

	//ライトの動的確保
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;
	}

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

	//ジョイパッドの初期化処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Init(hInstance, hWnd);
	}

	//サウンドの初期化処理
	/*if (m_pSound != NULL)
	{
		m_pSound->InitSound(hWnd);
	}*/

	//カメラの初期化処理
	if (m_pCamera != NULL)
	{
		m_pCamera->Init();
	}

	//ライトの初期化処理
	if (m_pLight != NULL)
	{
		m_pLight->Init();
	}

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

	//カメラの終了処理
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//ライトの終了処理
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	//デバッグの終了処理
	if (m_pDebugproc != NULL)
	{
		m_pDebugproc->Uninit();
		delete m_pDebugproc;
		m_pDebugproc = NULL;
	}

	if (m_pEditor != NULL)
	{
		//ゲームの終了処理
		m_pEditor->Uninit();
		//メモリの開放
		delete m_pEditor;
		m_pEditor = NULL;
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

	//ジョイパッドの更新処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();

	}
	//レンダリングの更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
	//フェードの更新処理
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}
	//カメラの更新処理
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	//ライトの更新処理
	if (m_pLight != NULL)
	{
		m_pLight->Update();
	}

	switch (m_mode)
	{
	case MODE_GAME:					//ゲームの場合
		if (m_pEditor != NULL)
		{
			//ゲームの更新処理
			m_pEditor->Update();
		}
		break;
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

	//カメラの設置処理
	if (m_pCamera != NULL)
	{
		m_pCamera->SetCamera();
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
// ジョイパッドの取得
//=============================================================================
CInputJoypad *CManager::GetJoypad(void)
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
// カメラの取得
//=============================================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;				//値を返す
}

//=============================================================================
// ライトの取得
//=============================================================================
CLight *CManager::GetLight(void)
{
	return m_pLight;				//値を返す
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
	switch (m_mode)
	{
	case MODE_GAME:					//ゲームの場合
		if (m_pEditor != NULL)
		{
			//ゲームの終了処理
			m_pEditor->Uninit();
			//メモリの開放
			delete m_pEditor;
			m_pEditor = NULL;
		}
		break;
	}

	m_mode = mode;	//代入

	switch (mode)
	{
	case MODE_GAME:					//ゲームの場合
		if (m_pEditor == NULL)
		{
			//ゲームの生成
			m_pEditor = CEditor::Create();
		}
		break;
	}
}