//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "title.h"
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
#include "sound.h"
#include "debugproc.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TITLE_SYSTEM_FILENAME    "data/TEXT/MODE/title.ini"    // 初期化に使用するシステムファイル名
#define TITLE_FADE_SPEED         (0.015f)                      // フェードのスピード
#define TITLE_FADEOUT_TIMING     (180)                         // チームロゴ状態からフェードアウト状態に移る時間
#define TITLE_CHANGEMODE_TIMING  (600)                         // 通常状態から画面遷移するまでの時間
#define TITLE_BG_SLIDE_UV        (0.001f)                      // 背景のテクスチャ座標をどれくらい動かすか

// チームロゴ用背景ポリゴン初期化用
#define TITLETEAMBG_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLETEAMBG_COL_INI      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLETEAMBG_WIDTH_INI    (SCREEN_WIDTH / 2.0f)
#define TITLETEAMBG_HEIGHT_INI   (SCREEN_HEIGHT / 2.0f)

// フェード用ポリゴン初期化用
#define TITLEFADE_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLEFADE_COL_INI        (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
#define TITLEFADE_WIDTH_INI      (SCREEN_WIDTH / 2.0f)
#define TITLEFADE_HEIGHT_INI     (SCREEN_HEIGHT / 2.0f)

// チームロゴ用ポリゴン初期化用
#define TITLETEAMLOGO_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLETEAMLOGO_COL_INI    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLETEAMLOGO_WIDTH_INI  (100.0f)
#define TITLETEAMLOGO_HEIGHT_INI (100.0f)

// 背景ポリゴン初期化用
#define TITLEBG_POS_INI          (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLEBG_COL_INI          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEBG_WIDTH_INI        (SCREEN_WIDTH / 2.0f)
#define TITLEBG_HEIGHT_INI       (SCREEN_HEIGHT / 2.0f)

// タイトルロゴ初期化用
#define TITLELOGO_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 280.0f, 0.0f))
#define TITLELOGO_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLELOGO_WIDTH_INI      (200.0f)
#define TITLELOGO_HEIGHT_INI     (120.0f)

// プレス待機初期化用
#define TITLEPRESS_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 500.0f, 0.0f))
#define TITLEPRESS_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEPRESS_WIDTH_INI     (150.0f)
#define TITLEPRESS_HEIGHT_INI    (60.0f)

// 値読み込み用のパス
// テクスチャ用
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// 背景ポリゴン用
#define BGPOLYGONSET "BGPOLYGONSET"
#define END_BGPOLYGONSET "END_BGPOLYGONSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// タイトルロゴ用
#define TITLELOGOSET "TITLELOGOSET"
#define END_TITLELOGOSET "END_TITLELOGOSET"

// プレス待機用
#define PRESSSET "PRESSSET"
#define END_PRESSSET "END_PRESSSET"

// チームロゴ用
#define TEAMLOGOSET "TEAMLOGOSET"
#define END_TEAMLOGOSET "END_TEAMLOGOSET"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


//*****************************************************************************
// CTitleの処理
//*****************************************************************************
//=============================================================================
// タイトルのコンストラクタ
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// タイトルのデストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// タイトルの生成処理
//=============================================================================
CTitle *CTitle::Create()
{
	CTitle *pTitle = NULL;
	if (pTitle == NULL)
	{
		pTitle = new CTitle;
		if (pTitle != NULL)
		{
			pTitle->Init();
		}
	}
	return pTitle;
}

//=============================================================================
// タイトルの初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	// 変数を初期化する
	ClearVariable();

	// システムの初期化
	LoadSystem();

	// カメラの生成
	CreateCamera();

	return S_OK;
}

//=============================================================================
// タイトルの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// 共通の終了処理
	CBasemode::Uninit();

	// 各種ポリゴンを開放
	ReleaseFadePolygon();
	ReleaseTeamLogoPolygon();
	ReleaseTeamBgPolygon();
	ReleaseBgPolygon();
	ReleaseTitleLogoPolygon();
	ReleasePress();

	// 全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// タイトルの更新処理
//=============================================================================
void CTitle::Update(void)
{
	// 背景スライド
	SlideBgUV();

	// 状態によって処理わけ
	switch (m_State)
	{
	case STATE_TEAMLOGO:
		TeamLogoUpdate();
		break;
	case STATE_FADEOUT:
		FadeOutUpdate();
		break;
	case STATE_FADEIN:
		FadeInUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	}

	// 全ての更新処理
	CScene::UpdateAll();

	CDebugProc::Print(1, "タイトル画面\n");
}

//=============================================================================
// タイトルの描画処理
//=============================================================================
void CTitle::Draw(void)
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
// タイトルのテクスチャ管轄クラス生成処理
//=============================================================================
void CTitle::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}


//=============================================================================
// タイトルのカメラ生成処理
//=============================================================================
void CTitle::CreateCamera(void)
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
// タイトルのフェード用ポリゴン生成処理
//=============================================================================
void CTitle::CreateFadePolygon(void)
{
	m_pFade = CScene2D::Create(TITLEFADE_POS_INI, TITLEFADE_COL_INI, TITLEFADE_WIDTH_INI, TITLEFADE_HEIGHT_INI);
}

//=============================================================================
// タイトルのチームロゴ背景用ポリゴン生成処理
//=============================================================================
void CTitle::CreateTeamBgPolygon(void)
{
	m_pTeamBg = CScene2D::Create(TITLETEAMBG_POS_INI, TITLETEAMBG_COL_INI, TITLETEAMBG_WIDTH_INI, TITLETEAMBG_HEIGHT_INI);
}

//=============================================================================
// タイトルのプレス待機用ポリゴン生成処理
//=============================================================================
void CTitle::CreatePress(void)
{
	m_pPress = CScene2DPress::Create(m_PressPos, m_PressCol, m_fPressWidth, m_fPressHeight);
	if (m_pPress != NULL && GetTextureManager() != NULL)
	{
		m_pPress->BindTexture(GetTextureManager()->GetTexture(m_nPressTexIdx));
	}
}

//=============================================================================
// タイトルのフェード用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseFadePolygon(void)
{
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		m_pFade = NULL;
	}
}

//=============================================================================
// タイトルのチームロゴ背景用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseTeamLogoPolygon(void)
{
	if (m_pTeamLogo != NULL)
	{
		m_pTeamLogo->Uninit();
		m_pTeamLogo = NULL;
	}
}

//=============================================================================
// タイトルのチームロゴ背景用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseTeamBgPolygon(void)
{
	if (m_pTeamBg != NULL)
	{
		m_pTeamBg->Uninit();
		m_pTeamBg = NULL;
	}
}

//=============================================================================
// タイトルの背景用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseBgPolygon(void)
{
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}
}

//=============================================================================
// タイトルのタイトルロゴ用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseTitleLogoPolygon(void)
{
	if (m_pTitleLogo != NULL)
	{
		m_pTitleLogo->Uninit();
		m_pTitleLogo = NULL;
	}
}

//=============================================================================
// タイトルのプレス待機用ポリゴン開放処理
//=============================================================================
void CTitle::ReleasePress(void)
{
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}
}

//=============================================================================
// タイトルの背景テクスチャ座標を動かす処理
//=============================================================================
void CTitle::SlideBgUV(void)
{
	if (m_pBg != NULL)
	{
		// 値の取得
		float fTexU = m_pBg->GetStartTexU();
		float fTexV = m_pBg->GetStartTexV();
		float fAddTexU = m_pBg->GetAddTexU();
		float fAddTexV = m_pBg->GetAddTexV();

		// テクスチャ座標を動かす
		fTexU += TITLE_BG_SLIDE_UV;
		fTexV += TITLE_BG_SLIDE_UV;
		fAddTexU += TITLE_BG_SLIDE_UV;
		fAddTexV += TITLE_BG_SLIDE_UV;

		// 値の設定
		m_pBg->SetStartTexU(fTexU);
		m_pBg->SetStartTexV(fTexV);
		m_pBg->SetAddTexU(fAddTexU);
		m_pBg->SetAddTexV(fAddTexV);
		m_pBg->SetVtxBuffTex();
	}
}

//=============================================================================
// タイトルのチームロゴ状態の更新処理
//=============================================================================
void CTitle::TeamLogoUpdate(void)
{
	CDebugProc::Print(1, "チームロゴ状態\n");
	if (CManager::GetFade()->GetFade() != CFade::FADE_NONE) return;

	m_nStateCounter++;
	CDebugProc::Print(1, "%d\n", m_nStateCounter);
	if (m_nStateCounter >= TITLE_FADEOUT_TIMING)
	{// 一定値を超えた
		m_nStateCounter = 0;
		CreateFadePolygon();
		m_State = STATE_FADEOUT;
	}
	else
	{
		WaitInputToNormal();
	}
	if (m_nStateCounter == 5)
	{
		CManager::GetSound()->PlaySound(3);
	}
}

//=============================================================================
// タイトルのフェードアウト状態の更新処理
//=============================================================================
void CTitle::FadeOutUpdate(void)
{
	CDebugProc::Print(1, "フェードアウト状態\n");
	D3DXCOLOR FadeCol = m_pFade->GetCol();
	FadeCol.a += TITLE_FADE_SPEED;
	CDebugProc::Print(1, "%.1f\n", FadeCol.a);
	if (FadeCol.a >= 1.0f)
	{
		ReleaseTeamBgPolygon();
		ReleaseTeamLogoPolygon();
		FadeCol.a = 1.0f;
		m_State = STATE_FADEIN;
	}

	if (m_pFade != NULL)
	{
		m_pFade->SetCol(FadeCol);
		m_pFade->SetVtxBuffCol();
	}
}

//=============================================================================
// タイトルのフェードイン状態の更新処理
//=============================================================================
void CTitle::FadeInUpdate(void)
{
	CDebugProc::Print(1, "フェードイン状態\n");
	D3DXCOLOR FadeCol = m_pFade->GetCol();
	FadeCol.a -= TITLE_FADE_SPEED;
	CDebugProc::Print(1, "%.1f\n", FadeCol.a);
	if (FadeCol.a <= 0.0f)
	{
		ReleaseFadePolygon();
		CreatePress();
		m_State = STATE_NORMAL;
	}

	if (m_pFade != NULL)
	{
		m_pFade->SetCol(FadeCol);
		m_pFade->SetVtxBuffCol();
	}
}

//=============================================================================
// タイトルの通常状態の更新処理
//=============================================================================
void CTitle::NormalUpdate(void)
{
	CDebugProc::Print(1, "通常状態\n");
	if (CManager::GetFade()->GetFade() != CFade::FADE_NONE) return;

	m_nStateCounter++;
	CDebugProc::Print(1, "%d\n", m_nStateCounter);
	if (m_nStateCounter >= TITLE_CHANGEMODE_TIMING)
	{// 一定値を超えた
		SetNextMode_None();
	}
	else
	{// まだ超えていない
		WaitInputToNextMode();
	}
}

//=============================================================================
// タイトルの通常状態に行くための入力待機処理
//=============================================================================
void CTitle::WaitInputToNormal(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;

	if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		m_nStateCounter = 0;
		CreateFadePolygon();
		m_State = STATE_FADEOUT;
	}
}

//=============================================================================
// タイトルの次のモードに行くための入力待機処理
//=============================================================================
void CTitle::WaitInputToNextMode(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;


	if (pFade->GetFade() == CFade::FADE_NONE)
	{// フェードが使用されていない
		if (pKey->GetTrigger(DIK_RETURN) == true ||
			CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
		{// 決定ボタンが押された
			SetNextMode_Push();
		}
	}
}

//=============================================================================
// タイトルの次のモードに設定する処理(入力されなかった場合)
//=============================================================================
void CTitle::SetNextMode_None(void)
{
	CManager::GetFade()->SetFade(CManager::MODE_TITLE);
}

//=============================================================================
// タイトルの次のモードに設定する処理(入力された場合)
//=============================================================================
void CTitle::SetNextMode_Push(void)
{
	CManager::GetFade()->SetFade(CManager::MODE_CHARASELECT);
	m_pPress->SetState(CScene2DPress::STATE_PUSH);
}

//=============================================================================
// タイトルのシステム情報を読み込む
//=============================================================================
void CTitle::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(TITLE_SYSTEM_FILENAME);
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
// タイトルのシステム情報をファイルから読み込む
//=============================================================================
void CTitle::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
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
		else if (CFunctionLib::Memcmp(pStr, BGPOLYGONSET) == 0)
		{// 背景用ポリゴン情報だった
			LoadBgPolygon(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TITLELOGOSET) == 0)
		{// タイトルロゴ情報だった
			LoadTitleLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, PRESSSET) == 0)
		{// プレス待機情報だった
			LoadPress(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TEAMLOGOSET) == 0)
		{// チームロゴ情報だった
			LoadTeamLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルの読み込むテクスチャファイル名を読み込む
//=============================================================================
void CTitle::LoadTexFileName(char *pStr, int nCntTex)
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
// タイトルの背景ポリゴンの値を読み込む
//=============================================================================
void CTitle::LoadBgPolygon(CFileLoader *pFileLoader, char *pStr)
{
	int nBgTexIdx = 0;
	D3DXVECTOR3 BgPos = TITLEBG_POS_INI;
	D3DXCOLOR BgCol = TITLEBG_COL_INI;
	float fBgWidth = TITLEBG_WIDTH_INI;
	float fBgHeight = TITLEBG_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nBgTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			BgPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			BgCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fBgWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fBgHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_BGPOLYGONSET) == 0)
		{// 背景ポリゴン情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_pBg = CScene2D::Create(BgPos, BgCol, fBgWidth, fBgHeight);
	if (m_pBg != NULL && GetTextureManager() != NULL)
	{
		m_pBg->BindTexture(GetTextureManager()->GetTexture(nBgTexIdx));
	}

	// テクスチャ座標を設定
	m_pBg->SetStartTexU(0.0f);
	m_pBg->SetStartTexV(0.0f);
	m_pBg->SetAddTexU(0.5f);
	m_pBg->SetAddTexV(0.5f);
	m_pBg->SetVtxBuffTex();
}

//=============================================================================
// タイトルのタイトルロゴポリゴンの値を読み込む
//=============================================================================
void CTitle::LoadTitleLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nLogoTexIdx = 0;
	D3DXVECTOR3 LogoPos = TITLELOGO_POS_INI;
	D3DXCOLOR LogoCol = TITLELOGO_COL_INI;
	float fLogoWidth = TITLELOGO_WIDTH_INI;
	float fLogoHeight = TITLELOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			LogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			LogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_TITLELOGOSET) == 0)
		{// タイトルロゴ情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_pTitleLogo = CTitleLogo::Create(LogoPos, LogoCol, fLogoWidth, fLogoHeight);
	if (m_pTitleLogo != NULL && GetTextureManager() != NULL)
	{
		m_pTitleLogo->BindTexture(GetTextureManager()->GetTexture(nLogoTexIdx));
	}
}

//=============================================================================
// タイトルのプレス待機ポリゴンの値を読み込む
//=============================================================================
void CTitle::LoadPress(CFileLoader *pFileLoader, char *pStr)
{
	m_nPressTexIdx = 0;
	m_PressPos = TITLEPRESS_POS_INI;
	m_PressCol = TITLEPRESS_COL_INI;
	m_fPressWidth = TITLEPRESS_WIDTH_INI;
	m_fPressHeight = TITLEPRESS_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_nPressTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_PressPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_PressCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_fPressWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_fPressHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PRESSSET) == 0)
		{// プレス待機情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルのチームロゴ用ポリゴンの値を読み込む
//=============================================================================
void CTitle::LoadTeamLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nTeamLogoTexIdx = 0;
	D3DXVECTOR3 TeamLogoPos = TITLETEAMLOGO_POS_INI;
	D3DXCOLOR TeamLogoCol = TITLETEAMLOGO_COL_INI;
	float fTeamLogoWidth = TITLETEAMLOGO_WIDTH_INI;
	float fTeamLogoHeight = TITLETEAMLOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nTeamLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			TeamLogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			TeamLogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fTeamLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fTeamLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_TEAMLOGOSET) == 0)
		{// チームロゴ情報終了の合図だった
			break;
		}
	}

	// チームロゴ用背景ポリゴンの生成
	CreateTeamBgPolygon();

	// ポリゴン生成
	m_pTeamLogo = CScene2D::Create(TeamLogoPos, TeamLogoCol, fTeamLogoWidth, fTeamLogoHeight);
	if (m_pTeamLogo != NULL && GetTextureManager() != NULL)
	{
		m_pTeamLogo->BindTexture(GetTextureManager()->GetTexture(nTeamLogoTexIdx));
	}
}

//=============================================================================
// タイトルの変数を初期化する
//=============================================================================
void CTitle::ClearVariable(void)
{
	m_State = STATE_TEAMLOGO;
	m_pFade = NULL;
	m_pTeamLogo = NULL;
	m_pTeamBg = NULL;
	m_pBg = NULL;
	m_pTitleLogo = NULL;
	m_pPress = NULL;
	m_nStateCounter = 0;
	ClearPress();
}

//=============================================================================
// タイトルのプレス待機用変数を初期化する
//=============================================================================
void CTitle::ClearPress(void)
{
	m_nPressTexIdx = 0;
	m_PressPos = INITIALIZE_D3DXVECTOR3;
	m_PressCol = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	m_fPressWidth = 0.0f;
	m_fPressHeight = 0.0f;
}

//=============================================================================
// タイトルの状態を設定する
//=============================================================================
void CTitle::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
// タイトルの状態を取得する
//=============================================================================
CTitle::STATE CTitle::GetState(void)
{
	return m_State;
}

//*****************************************************************************
// CTitleLogoの処理
//*****************************************************************************
//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CTitleLogo::CTitleLogo(int nPriority, OBJTYPE objtype) : CScene2D(nPriority, objtype)
{

}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
//オブジェクトの生成
//=============================================================================
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CTitleLogo *pTitleLogo = NULL;	// CTitleLogoクラス型のポインタ
	if (pTitleLogo == NULL)
	{
		pTitleLogo = new CTitleLogo(nPriority);
		if (pTitleLogo != NULL)
		{
			// 値の設定
			pTitleLogo->SetPos(pos);
			pTitleLogo->SetCol(col);
			pTitleLogo->SetWidth(fWidth);
			pTitleLogo->SetHeight(fHeight);
			pTitleLogo->SetRot(fRot);
			pTitleLogo->SetStartTexU(fStartTexU);
			pTitleLogo->SetStartTexV(fStartTexV);
			pTitleLogo->SetAddTexU(fAddTexU);
			pTitleLogo->SetAddTexV(fAddTexV);

			if (FAILED(pTitleLogo->Init()))
			{// 初期化に失敗
				return NULL;
			}
		}
	}
	return pTitleLogo;
}

//=============================================================================
// オブジェクトの初期化処理
//=============================================================================
HRESULT CTitleLogo::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CTitleLogo::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CTitleLogo::Update(void)
{

}

//=============================================================================
// タイトルロゴの描画処理
//=============================================================================
void CTitleLogo::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}