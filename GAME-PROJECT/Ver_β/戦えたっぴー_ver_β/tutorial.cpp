//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "tutorial.h"
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
#include "map.h"
#include "debugproc.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TUTORIAL_SYSTEM_FILENAME  "data/TEXT/MODE/tutorial.ini"  // 初期化に使用するシステムファイル名
#define TUTORIAL_WAITSTATE_TIME   (60)                           // 待機状態から通常状態に変わるまでの時間
#define TUTORIAL_CHANGEMODE_TIME  (600)                          // 通常状態から終了状態に変わるまでの時間


// スキップロゴ初期化用
#define TUTORIAL_SKIPLOGO_POS_INI              (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f, 0.0f))
#define TUTORIAL_SKIPLOGO_COL_INI              (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_SKIPLOGO_WIDTH_INI            (300.0f)
#define TUTORIAL_SKIPLOGO_HEIGHT_INI           (90.0f)

// スキップチェックロゴ初期化用
// １つ目
#define TUTORIAL_SKIPCHECKLOGO_0_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 3.0f * 2.0f, 0.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_COL_NONE      (D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE    (80.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE   (40.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_COL_SELECT    (D3DXCOLOR(0.9f,0.9f,0.9f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_WIDTH_SELECT  (120.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_SELECT (60.0f)

// ２つ目
#define TUTORIAL_SKIPCHECKLOGO_1_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 4.0f * 3.0f, SCREEN_HEIGHT / 3.0f * 2.0f, 0.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_COL_NONE      (D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE    (80.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE   (40.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_COL_SELECT    (D3DXCOLOR(0.9f,0.9f,0.9f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_WIDTH_SELECT  (120.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_SELECT (60.0f)

// 値読み込み用のパス
// テクスチャ用
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// マップ用
#define NUM_MAP "NUM_MAP = "

// スキップロゴ用
#define SKIPLOGOSET "SKIPLOGOSET"
#define END_SKIPLOGOSET "END_SKIPLOGOSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// スキップチェックロゴ用
#define SKIPCHECKLOGOSET "SKIPCHECKLOGOSET"
#define END_SKIPCHECKLOGOSET "END_SKIPCHECKLOGOSET"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// チュートリアルのコンストラクタ
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// チュートリアルのデストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// チュートリアルの生成処理
//=============================================================================
CTutorial *CTutorial::Create()
{
	CTutorial *pTutorial = NULL;
	if (pTutorial == NULL)
	{
		pTutorial = new CTutorial;
		if (pTutorial != NULL)
		{
			pTutorial->Init();
		}
	}
	return pTutorial;
}

//=============================================================================
// チュートリアルの初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
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
// チュートリアルの終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	// 共通の終了処理
	CBasemode::Uninit();

	// 各種クラスの開放
	ReleaseSkipLogo();
	ReleaseSkipCheckLogo();
	ReleaseMapFilePointer();

	// 全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// チュートリアルの更新処理
//=============================================================================
void CTutorial::Update(void)
{
	CDebugProc::Print(1, "チュートリアル画面\n");

	// 状態によって処理わけ
	switch (m_State)
	{
	case STATE_SKIP_CHECK:
		SkipCheckUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	case STATE_GOAL:
		GoalUpdate();
		break;
	case STATE_MAP_CHANGE:
		MapChangeUpdate();
		break;
	case STATE_END:
		EndUpdate();
		break;
	}

	// 全ての更新処理
	CScene::UpdateAll();
}

//=============================================================================
// チュートリアルの描画処理
//=============================================================================
void CTutorial::Draw(void)
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
// チュートリアルのテクスチャ管轄クラス生成処理
//=============================================================================
void CTutorial::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// チュートリアルのカメラ生成処理
//=============================================================================
void CTutorial::CreateCamera(void)
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
// チュートリアルの読み込むマップファイル名のポインタを生成する
//=============================================================================
void CTutorial::CreateMapFilePointer(void)
{
	m_pMapFileName = new char*[m_nNumMap];
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			m_pMapFileName[nCnt] = new char[256];
			strcpy(m_pMapFileName[nCnt], "\0");
		}
	}
}

//=============================================================================
// チュートリアルの読み込むマップファイル名のポインタを開放する
//=============================================================================
void CTutorial::ReleaseMapFilePointer(void)
{
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			delete[] m_pMapFileName[nCnt];
			m_pMapFileName[nCnt] = NULL;
		}

		delete[] m_pMapFileName;
		m_pMapFileName = NULL;
	}
}

//=============================================================================
// チュートリアルのスキップロゴポリゴンを開放する
//=============================================================================
void CTutorial::ReleaseSkipLogo(void)
{
	if (m_pSkipLogo != NULL)
	{
		m_pSkipLogo->Uninit();
		m_pSkipLogo = NULL;
	}
}

//=============================================================================
// チュートリアルのスキップ確認ロゴ用ポリゴンを開放する
//=============================================================================
void CTutorial::ReleaseSkipCheckLogo(void)
{
	for (int nCntSkipCheck = 0; nCntSkipCheck < SKIP_MAX; nCntSkipCheck++)
	{
		if (m_pSkipCheckLogo[nCntSkipCheck] != NULL)
		{
			m_pSkipCheckLogo[nCntSkipCheck]->Uninit();
			m_pSkipCheckLogo[nCntSkipCheck] = NULL;
		}
	}
}

//=============================================================================
// チュートリアルのスキップ確認状態の更新処理
//=============================================================================
void CTutorial::SkipCheckUpdate(void)
{
	CDebugProc::Print(1, "スキップ確認状態\n");
	CDebugProc::Print(1, "選択番号 : %d\n", m_nSelectSkip);

	// 入力クラスを取得
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	if (pKey == NULL || pXInput == NULL)return;

	if (pKey->GetTrigger(DIK_A) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_18) == true ||
		pKey->GetRepeat(DIK_A) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_18) == true)
	{// 左方向の入力がされた
		ChangeNoneSkipCheckPolygon(m_nSelectSkip);
		m_nSelectSkip = (m_nSelectSkip + (SKIP_MAX - 1)) % SKIP_MAX;
		ChangeSelectSkipCheckPolygon(m_nSelectSkip);
	}
	else if (pKey->GetTrigger(DIK_D) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_19) == true ||
		pKey->GetRepeat(DIK_D) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_19) == true)
	{// 右方向の入力がされた
		ChangeNoneSkipCheckPolygon(m_nSelectSkip);
		m_nSelectSkip = (m_nSelectSkip + 1) % SKIP_MAX;
		ChangeSelectSkipCheckPolygon(m_nSelectSkip);
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		if (m_nSelectSkip == SKIP_YES)
		{// はいだったら
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
		}
		else if(m_nSelectSkip == SKIP_NO)
		{// いいえだったら
			SetState(STATE_NORMAL);
			ReleaseSkipLogo();
			ReleaseSkipCheckLogo();
			CScene::DeathCheck();
		}
	}
}

//=============================================================================
// チュートリアルの通常状態の更新処理
//=============================================================================
void CTutorial::NormalUpdate(void)
{
	CDebugProc::Print(1, "通常状態\n");
}

//=============================================================================
// チュートリアルのゴール状態の更新処理
//=============================================================================
void CTutorial::GoalUpdate(void)
{
	CDebugProc::Print(1, "ゴール状態\n");
}

//=============================================================================
// チュートリアルのマップ変更状態の更新処理
//=============================================================================
void CTutorial::MapChangeUpdate(void)
{
	CDebugProc::Print(1, "マップ変更状態\n");
}

//=============================================================================
// チュートリアルの終了状態の更新処理
//=============================================================================
void CTutorial::EndUpdate(void)
{
	CDebugProc::Print(1, "終了状態\n");
}

//=============================================================================
// チュートリアルのスキップ確認用ポリゴンを選択されていない状態にする
//=============================================================================
void CTutorial::ChangeNoneSkipCheckPolygon(int nSelect)
{
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_NONE,  TUTORIAL_SKIPCHECKLOGO_1_COL_NONE };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE };
	if (m_pSkipCheckLogo[nSelect] != NULL)
	{
		m_pSkipCheckLogo[nSelect]->SetState(CScene2DFlash::STATE_NONE);
		m_pSkipCheckLogo[nSelect]->SetScale(fSkipCheckWidthLogo[nSelect], fSkipCheckHeightLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetCol(SkipCheckColLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// チュートリアルのスキップ確認用ポリゴンを選択された状態にする
//=============================================================================
void CTutorial::ChangeSelectSkipCheckPolygon(int nSelect)
{
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_COL_SELECT };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_SELECT };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_SELECT };

	if (m_pSkipCheckLogo[nSelect] != NULL)
	{
		m_pSkipCheckLogo[nSelect]->SetState(CScene2DFlash::STATE_SELECT);
		m_pSkipCheckLogo[nSelect]->SetScale(fSkipCheckWidthLogo[nSelect], fSkipCheckHeightLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetCol(SkipCheckColLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// チュートリアルのシステム情報を読み込む
//=============================================================================
void CTutorial::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(TUTORIAL_SYSTEM_FILENAME);
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
// チュートリアルのシステム情報をファイルから読み込む
//=============================================================================
void CTutorial::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntMap = 0;
	int nCntSkipCheck = 0;

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
		else if (CFunctionLib::Memcmp(pStr, NUM_MAP) == 0)
		{// 読み込むマップの数だった
			m_nNumMap = CFunctionLib::ReadInt(pStr, NUM_MAP);
			CreateMapFilePointer();
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// 読み込むマップファイル名だった
			LoadMapFileName(pStr, nCntMap);
			nCntMap++;
		}
		else if (CFunctionLib::Memcmp(pStr, SKIPLOGOSET) == 0)
		{// スキップロゴポリゴン情報だった
			LoadSkipLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, SKIPCHECKLOGOSET) == 0)
		{// スキップ確認ロゴ用ポリゴン情報だった
			LoadSkipCheckLogo(pFileLoader, pStr, nCntSkipCheck);
			nCntSkipCheck++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// チュートリアルの読み込むテクスチャファイル名を読み込む
//=============================================================================
void CTutorial::LoadTexFileName(char *pStr, int nCntTex)
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
// チュートリアルの読み込むマップファイル名を読み込む
//=============================================================================
void CTutorial::LoadMapFileName(char *pStr, int nCntMap)
{
	strcpy(m_pMapFileName[nCntMap], CFunctionLib::ReadString(pStr, m_pMapFileName[nCntMap], MAP_FILENAME));
}

//=============================================================================
// チュートリアルのスキップロゴポリゴン情報を読み込む
//=============================================================================
void CTutorial::LoadSkipLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nSkipLogoTexIdx = 0;
	D3DXVECTOR3 SkipLogoPos = TUTORIAL_SKIPLOGO_POS_INI;
	D3DXCOLOR SkipLogoCol = TUTORIAL_SKIPLOGO_COL_INI;
	float fSkipLogoWidth = TUTORIAL_SKIPLOGO_WIDTH_INI;
	float fSkipLogoHeight = TUTORIAL_SKIPLOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nSkipLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			SkipLogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			SkipLogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fSkipLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fSkipLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKIPLOGOSET) == 0)
		{// スキップロゴポリゴン情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_pSkipLogo = CScene2D::Create(SkipLogoPos, SkipLogoCol, fSkipLogoWidth, fSkipLogoHeight);

	// テクスチャ割り当て
	if (m_pSkipLogo != NULL && GetTextureManager() != NULL)
	{
		m_pSkipLogo->BindTexture(GetTextureManager()->GetTexture(nSkipLogoTexIdx));
	}
}

//=============================================================================
// チュートリアルのスキップ確認ロゴ用ポリゴン情報を読み込む
//=============================================================================
void CTutorial::LoadSkipCheckLogo(CFileLoader *pFileLoader, char *pStr, int nCntSkipCheck)
{
	int nSkipCheckTexIdx[SKIP_MAX] = {};
	D3DXVECTOR3 SkipCheckPosLogo[SKIP_MAX] = {TUTORIAL_SKIPCHECKLOGO_0_POS_INI,  TUTORIAL_SKIPCHECKLOGO_1_POS_INI };
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_NONE,  TUTORIAL_SKIPCHECKLOGO_1_COL_NONE };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE };

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nSkipCheckTexIdx[nCntSkipCheck] = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			SkipCheckPosLogo[nCntSkipCheck] = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			SkipCheckColLogo[nCntSkipCheck] = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fSkipCheckWidthLogo[nCntSkipCheck] = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fSkipCheckHeightLogo[nCntSkipCheck] = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKIPCHECKLOGOSET) == 0)
		{// スキップ確認ロゴ用ポリゴン情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_pSkipCheckLogo[nCntSkipCheck] = CScene2DFlash::Create(SkipCheckPosLogo[nCntSkipCheck],
		SkipCheckColLogo[nCntSkipCheck], fSkipCheckWidthLogo[nCntSkipCheck],
		fSkipCheckHeightLogo[nCntSkipCheck], 0.0f, CScene2DFlash::STATE_NONE);
	if (nCntSkipCheck == 0)m_pSkipCheckLogo[nCntSkipCheck]->SetState(CScene2DFlash::STATE_SELECT);

	// テクスチャ割り当て
	if (m_pSkipCheckLogo[nCntSkipCheck] != NULL && GetTextureManager() != NULL)
	{
		m_pSkipCheckLogo[nCntSkipCheck]->BindTexture(GetTextureManager()->GetTexture(nSkipCheckTexIdx[nCntSkipCheck]));
	}
}

//=============================================================================
// チュートリアルの変数を初期化する
//=============================================================================
void CTutorial::ClearVariable(void)
{
	m_State = STATE_SKIP_CHECK;
	m_nStateCounter = 0;
	m_nSelectSkip = 0;
	m_pSkipLogo = NULL;
	m_nNumMap = 0;
	m_pMapFileName = NULL;
	for (int nCntSkip = 0; nCntSkip < SKIP_MAX; nCntSkip++)
	{
		m_pSkipCheckLogo[nCntSkip] = NULL;
	}
}

//=============================================================================
// チュートリアルの状態を設定する
//=============================================================================
void CTutorial::SetState(const STATE state)
{
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// チュートリアルの状態を取得する
//=============================================================================
CTutorial::STATE CTutorial::GetState(void)
{
	return m_State;
}