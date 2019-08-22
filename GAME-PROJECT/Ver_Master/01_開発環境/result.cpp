//=============================================================================
//
// リザルト処理 [result.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "number.h"
#include "title.h"
#include "sound.h"
#include "debugproc.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define RESULT_SYSTEM_FILENAME               "data/TEXT/MODE/result.ini"    // 初期化に使用するシステムファイル名
#define RESULT_WAITSTATE_TIME                (60)                           // 待機状態から通常状態に変わるまでの時間
#define RESULT_NEXTHIGHSCORE_TIME            (240)                          // 通常状態からハイスコアを表示する状態に変わるまでの時間
#define RESULT_CHANGEMODE_TIME               (600)                          // 通常状態から終了状態に変わるまでの時間
#define RESULT_HIGHSCORE_FALSH_TIME          (2)                            // ハイスコアロゴを点滅させるタイミング
#define RESULT_BGM_IDX                       (4)                            // リザルトで再生するBGMの番号

// リザルトロゴ初期化用
#define RESULT_LOGO_POS_INI                  (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define RESULT_LOGO_COL_INI                  (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_LOGO_WIDTH_INI                (150.0f)
#define RESULT_LOGO_HEIGHT_INI               (100.0f)

// リザルトハイスコアロゴ初期化用
#define RESULT_HIGHSCORE_FALSH_COL           (D3DXCOLOR(0.5f,0.5f,1.0f,1.0f))
#define RESULT_HIGHSCORELOGO_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 220.0f, 0.0f))
#define RESULT_HIGHSCORELOGO_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_HIGHSCORELOGO_WIDTH_INI       (400.0f)
#define RESULT_HIGHSCORELOGO_HEIGHT_INI      (60.0f)

// リザルトハイスコアロゴ初期化用
#define RESULT_HIGHSCORENUMBER_POS_INI       (D3DXVECTOR3(900.0f, 380.0f, 0.0f))
#define RESULT_HIGHSCORENUMBER_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_HIGHSCORENUMBER_WIDTH_INI     (40.0f)
#define RESULT_HIGHSCORENUMBER_HEIGHT_INI    (40.0f)
#define RESULT_HIGHSCORENUMBER_INTERVAL_INI  (D3DXVECTOR3(-90.0f, 0.0f, 0.0f))

// 値読み込み用のパス
// テクスチャ用
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// リザルトロゴ用
#define RESULTLOGOSET "RESULTLOGOSET"
#define END_RESULTLOGOSET "END_RESULTLOGOSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
bool CResult::m_bHighScore = true;   // ハイスコアを表示するかどうか

//=============================================================================
// リザルトのコンストラクタ
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// リザルトのデストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// リザルトの生成処理
//=============================================================================
CResult *CResult::Create()
{
	CResult *pResult = NULL;
	if (pResult == NULL)
	{
		pResult = new CResult;
		if (pResult != NULL)
		{
			pResult->Init();
		}
	}
	return pResult;
}

//=============================================================================
// リザルトの初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	// 変数を初期化する
	ClearVariable();

	// システムの初期化
	LoadSystem();

	// カメラの生成
	CreateCamera();

	// BGMの再生
	CManager::GetSound()->PlaySound(RESULT_BGM_IDX);

	return S_OK;
}

//=============================================================================
// リザルトの終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// 共通の終了処理
	CBasemode::Uninit();

	// 各種クラスの開放
	ReleaseLogo();
	ReleaseHighScore();

	// 全てのオブジェクト開放
	CScene::ReleaseAll();

	// BGMを停止
	CManager::GetSound()->StopSound(RESULT_BGM_IDX);

	// ハイスコアを表示しない状態に
	m_bHighScore = false;
}

//=============================================================================
// リザルトの更新処理
//=============================================================================
void CResult::Update(void)
{
	CDebugProc::Print(1, "リザルト画面\n");

	// 状態によって処理わけ
	switch (m_State)
	{
	case STATE_WAIT:
		WaitUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	case STATE_HIGHSCORE:
		HighScoreUpdate();
		break;
	case STATE_END:
		EndUpdate();
		break;
	}

	// 全ての更新処理
	CScene::UpdateAll();
}

//=============================================================================
// リザルトの描画処理
//=============================================================================
void CResult::Draw(void)
{
	// カメラの設定
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera();
	}

	// シーンクラスの描画処理
	CScene::DrawAll();
}

//=============================================================================
// リザルトのテクスチャ管轄クラス生成処理
//=============================================================================
void CResult::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// リザルトのカメラ生成処理
//=============================================================================
void CResult::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager == NULL) return;

	// カメラを生成
	CCamera *pCamera = CCamera::Create();
	if (pCamera != NULL)
	{
		pCameraManager->SetCamera(pCamera);
	}
	SetCameraManager(pCameraManager);
}

//=============================================================================
// リザルトのロゴ生成処理
//=============================================================================
void CResult::CreateLogo(void)
{
	m_pLogo = CScene2D::Create(m_ResultLogoData.pos, m_ResultLogoData.col, m_ResultLogoData.fWidth, m_ResultLogoData.fHeight);
	if (m_pLogo != NULL && GetTextureManager() != NULL)
	{
		m_pLogo->BindTexture(GetTextureManager()->GetTexture(m_ResultLogoData.nTexIdx));
	}
}

//=============================================================================
// リザルトのロゴ生成処理
//=============================================================================
void CResult::CreateHighScore(void)
{
	// ロゴポリゴン生成
	if (m_pHighScoreLogo == NULL)
	{
		m_pHighScoreLogo = CScene2D::Create(RESULT_HIGHSCORELOGO_POS_INI, RESULT_HIGHSCORELOGO_COL_INI,
			RESULT_HIGHSCORELOGO_WIDTH_INI, RESULT_HIGHSCORELOGO_HEIGHT_INI);
		if (m_pHighScoreLogo != NULL && GetTextureManager() != NULL)
		{
			m_pHighScoreLogo->BindTexture(GetTextureManager()->GetTexture(m_ResultLogoData.nTexIdx));
		}
	}

	// 数字ポリゴン生成
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(1);
	}
	if (m_pHighScore == NULL)
	{
		m_pHighScore = CNumber::Create(RESULT_HIGHSCORENUMBER_POS_INI, RESULT_HIGHSCORENUMBER_COL_INI,
			RESULT_HIGHSCORENUMBER_WIDTH_INI, RESULT_HIGHSCORENUMBER_HEIGHT_INI, RESULT_HIGHSCORENUMBER_INTERVAL_INI,
			pTexture, CTitle::GetHighScore());
	}
}

//=============================================================================
// リザルトのロゴ開放処理
//=============================================================================
void CResult::ReleaseLogo(void)
{
	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}
}

//=============================================================================
// リザルトのハイスコア開放処理
//=============================================================================
void CResult::ReleaseHighScore(void)
{
	// ロゴポリゴン開放
	if (m_pHighScoreLogo != NULL)
	{
		m_pHighScoreLogo->Uninit();
		m_pHighScoreLogo = NULL;
	}

	// 数字ポリゴン開放
	if (m_pHighScore != NULL)
	{
		m_pHighScore->Uninit();
		m_pHighScore = NULL;
	}
}

//=============================================================================
// リザルトの待機状態の更新処理
//=============================================================================
void CResult::WaitUpdate(void)
{
	CDebugProc::Print(1, "待機状態\n");

	m_nStateCounter++;
	if (m_nStateCounter % RESULT_WAITSTATE_TIME == 0)
	{// カウンターが一定値に達した
		SetState(STATE_NORMAL);
		m_nStateCounter = 0;
	}

	CDebugProc::Print(1, "%d\n", m_nStateCounter);
}

//=============================================================================
// リザルトの通常状態の更新処理
//=============================================================================
void CResult::NormalUpdate(void)
{
	CDebugProc::Print(1, "通常状態\n");

	// カウンター加算
	m_nStateCounter++;

	if (m_bHighScore == true)
	{// ハイスコアが更新されている
		if (m_nStateCounter % RESULT_NEXTHIGHSCORE_TIME == 0)
		{// カウンターが一定値に達した
			// ハイスコアを表示する状態に
			SetState(STATE_HIGHSCORE);
			m_nStateCounter = 0;

			// リザルトロゴを開放
			ReleaseLogo();

			// ハイスコア表示に必要なものを生成
			CreateHighScore();
		}

		// 次の状態待機処理
		WaitInputToHighScore();
	}
	else
	{// ハイスコアが更新されていない
		if (m_nStateCounter % RESULT_CHANGEMODE_TIME == 0)
		{// カウンターが一定値に達した
			SetState(STATE_END);
			m_nStateCounter = 0;
		}

		// 次のモード待機処理
		WaitInputToNextMode();
	}

	CDebugProc::Print(1, "%d\n", m_nStateCounter);
}

//=============================================================================
// リザルトのハイスコアを表示する状態の更新処理
//=============================================================================
void CResult::HighScoreUpdate(void)
{
	CDebugProc::Print(1, "ハイスコア表示状態\n");

	// スコアを点滅させる
	if (m_nStateCounter % RESULT_HIGHSCORE_FALSH_TIME == 0)
	{
		// 点滅の色を設定
		if (m_FalshCol.r < 1.0f)
		{
			m_FalshCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			m_FalshCol = RESULT_HIGHSCORE_FALSH_COL;
		}

		// ポリゴンの色を設定
		// ロゴポリゴン
		if (m_pHighScoreLogo != NULL)
		{
			m_pHighScoreLogo->SetCol(m_FalshCol);
			m_pHighScoreLogo->SetVtxBuffCol();
		}

		// 数字ポリゴン
		if (m_pHighScore != NULL)
		{
			m_pHighScore->SetNumberCol(m_FalshCol);
			m_pHighScore->SetNumberVtxBuffCol();
		}
	}

	// カウンター加算
	m_nStateCounter++;

	if (m_nStateCounter % RESULT_CHANGEMODE_TIME == 0)
	{// カウンターが一定値に達した
		SetState(STATE_END);
		m_nStateCounter = 0;
	}

	CDebugProc::Print(1, "%d\n", m_nStateCounter);

	// 次のモード待機処理
	WaitInputToNextMode();
}

//=============================================================================
// リザルトの終了状態の更新処理
//=============================================================================
void CResult::EndUpdate(void)
{
	CDebugProc::Print(1, "終了状態\n");

	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) return;

	if (pFade->GetFade() == CFade::FADE_NONE)
	{// フェードが使用されていない
		pFade->SetFade(CManager::MODE_TITLE);
	}
}

//=============================================================================
// リザルトのハイスコアを表示する状態に行くための待機処理
//=============================================================================
void CResult::WaitInputToHighScore(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL) return;

	if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		// 状態を設定
		SetState(STATE_HIGHSCORE);
		m_nStateCounter = 0;

		// リザルトロゴを開放
		ReleaseLogo();

		// ハイスコア表示に必要なものを生成
		CreateHighScore();
	}
}

//=============================================================================
// リザルトの次のモードに行くための待機処理
//=============================================================================
void CResult::WaitInputToNextMode(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL) return;

	if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		SetState(STATE_END);
	}
}

//=============================================================================
// リザルトのシステム情報を読み込む
//=============================================================================
void CResult::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(RESULT_SYSTEM_FILENAME);
	if (pFileLoader != NULL)
	{
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// 読み込み開始の合図だった
			LoadSystemScript(pFileLoader, aStr);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// リザルトのシステム情報をファイルから読み込む
//=============================================================================
void CResult::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// 読み込むテクスチャの数だった
			CreateTexture(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// 読み込むテクスチャのファイル名だった
			LoadTexFileName(pStr, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, RESULTLOGOSET) == 0)
		{// リザルトロゴ情報だった
			LoadLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// リザルトの読み込むテクスチャファイル名を読み込む
//=============================================================================
void CResult::LoadTexFileName(char *pStr, int nCntTex)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	char aTexFileName[256] = "\0";
	strcpy(aTexFileName, CFunctionLib::ReadString(pStr, aTexFileName, TEXTURE_FILENAME));

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), aTexFileName, &pTexture);

	// テクスチャ管轄クラスにポインタを設定する
	CTextureManager *pTextureManager = GetTextureManager();
	pTextureManager->SetTexture(pTexture, nCntTex);
	pTextureManager->SetFileName(pStr, nCntTex);
}

//=============================================================================
// リザルトのロゴポリゴン情報を読み込む
//=============================================================================
void CResult::LoadLogo(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_ResultLogoData.nTexIdx = 0;
	m_ResultLogoData.pos = RESULT_LOGO_POS_INI;
	m_ResultLogoData.col = RESULT_LOGO_COL_INI;
	m_ResultLogoData.fWidth = RESULT_LOGO_WIDTH_INI;
	m_ResultLogoData.fHeight = RESULT_LOGO_HEIGHT_INI;

	// データ読み込み
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_ResultLogoData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_ResultLogoData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_ResultLogoData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_ResultLogoData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_ResultLogoData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_RESULTLOGOSET) == 0)
		{// リザルトロゴポリゴン情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	CreateLogo();
}

//=============================================================================
// リザルトの変数を初期化する
//=============================================================================
void CResult::ClearVariable(void)
{
	m_State = STATE_WAIT;
	m_nStateCounter = 0;
	m_pLogo = NULL;
	m_pHighScore = NULL;
	m_pHighScoreLogo = NULL;
}

//=============================================================================
// リザルトの状態を設定する
//=============================================================================
void CResult::SetState(const STATE state)
{
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// リザルトのハイスコアを出すかどうかを設定する
//=============================================================================
void CResult::SetHighScore(void)
{
	m_bHighScore = true;
}

//=============================================================================
// リザルトの状態を取得する
//=============================================================================
CResult::STATE CResult::GetState(void)
{
	return m_State;
}