//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "textureManager.h"
#include "scene2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_PRIORITY             (8)
#define PAUSE_SELECT_SOUNDIDX      (8)
#define PAUSE_DECIDE_SOUNDIDX      (9)

#define PAUSE_BLACKBG_COL          (D3DXCOLOR(0.0f,0.0f,0.0f,0.5f))

#define PAUSE_BG_POS               (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define PAUSE_BG_COL               (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define PAUSE_BG_WIDTH             (250.0f)
#define PAUSE_BG_HEIGHT            (250.0f)
#define PAUSE_BG_TEXIDX            (20)

#define PAUSE_SELECT_POS           (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 290.0f, 0.0f))
#define PAUSE_SELECT_COL_SELECT    (D3DXCOLOR(0.8f,0.8f,0.8f,1.0f))
#define PAUSE_SELECT_WIDTH_SELECT  (220.0f)
#define PAUSE_SELECT_HEIGHT_SELECT (50.0f)
#define PAUSE_SELECT_COL_NONE      (D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))
#define PAUSE_SELECT_WIDTH_NONE    (180.0f)
#define PAUSE_SELECT_HEIGHT_NONE   (30.0f)
#define PAUSE_SELECT_INTERVAL      (115.0f)
#define PAUSE_SELECT_TEXIDX        (21)

//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPause *CPause::Create(CTextureManager *pTexManager)
{
	CPause *pPause = NULL;
	if (pPause == NULL)
	{
		pPause = new CPause;
		if (pPause != NULL)
		{
			pPause->Init(pTexManager);
		}
	}
	return pPause;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(CTextureManager *pTexManager)
{
	// 変数のクリア
	ClearVariable();

	// 黒背景を生成
	CreateBlackBg();

	// ポーズ背景を生成
	CreatePauseBg(pTexManager);

	// 選択項目を生成
	CreatePauseSelect(pTexManager);

	// 選択番号を初期化
	m_nSelect = SELECT_CONTINUE;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	// 黒背景を開放
	ReleaseBlackBg();

	// ポーズ背景を開放
	ReleasePauseBg();

	// 選択項目を開放
	ReleasePauseSelect();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{
		if (m_apPauseSelect[nCntSelect] != NULL)
		{
			m_apPauseSelect[nCntSelect]->Update();
		}
	}

	// 入力待ち
	WaitInput();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
// 入力待ち処理
//=============================================================================
void CPause::WaitInput(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;
	if (pFade->GetFade() != CFade::FADE_NONE) { return; }

	if (pKey->GetTrigger(DIK_W) == true || pKey->GetRepeat(DIK_W) == true ||
		pXInput->GetTrigger(m_nOpenId, CXInput::XIJS_BUTTON_0) == true || pXInput->GetRepeat(m_nOpenId, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetTrigger(m_nOpenId, CXInput::XIJS_BUTTON_16) == true || pXInput->GetRepeat(m_nOpenId, CXInput::XIJS_BUTTON_16) == true)
	{// 上方向の入力がされた
		ChangeNoneStagePolygon(m_nSelect);
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;
		CManager::GetSound()->PlaySound(PAUSE_SELECT_SOUNDIDX);
		ChangeSelectStagePolygon(m_nSelect);
	}
	else if (pKey->GetTrigger(DIK_S) == true || pKey->GetRepeat(DIK_S) == true ||
		pXInput->GetTrigger(m_nOpenId, CXInput::XIJS_BUTTON_1) == true || pXInput->GetRepeat(m_nOpenId, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetTrigger(m_nOpenId, CXInput::XIJS_BUTTON_17) == true || pXInput->GetRepeat(m_nOpenId, CXInput::XIJS_BUTTON_17) == true)
	{// 下方向の入力がされた
		ChangeNoneStagePolygon(m_nSelect);
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;
		CManager::GetSound()->PlaySound(PAUSE_SELECT_SOUNDIDX);
		ChangeSelectStagePolygon(m_nSelect);
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		pXInput->GetTrigger(m_nOpenId, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		CGame *pGame = CManager::GetGame();
		if (pGame == NULL) { return; }

		switch (m_nSelect)
		{
		case SELECT_CONTINUE:
			pGame->SetState(CGame::STATE_NORMAL);
			CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);
			break;
		case SELECT_RETRY:
			pGame->SetState(CGame::STATE_END_RETRY);
			CManager::GetSound()->PlaySound(PAUSE_DECIDE_SOUNDIDX);
			break;
		case SELECT_QUIT:
			pGame->SetState(CGame::STATE_END_QUIT);
			CManager::GetSound()->PlaySound(PAUSE_DECIDE_SOUNDIDX);
			break;
		}
		pGame->ReleasePause();
		CScene::DeathCheck();
	}
}

//=============================================================================
// キャラセレクトのステージ選択用ポリゴンを選択されていない状態にする
//=============================================================================
void CPause::ChangeNoneStagePolygon(int nSelect)
{
	if (m_apPauseSelect[nSelect] != NULL)
	{
		m_apPauseSelect[nSelect]->SetState(CScene2DFlash::STATE_NONE);
		m_apPauseSelect[nSelect]->SetScale(PAUSE_SELECT_WIDTH_NONE, PAUSE_SELECT_HEIGHT_NONE);
		m_apPauseSelect[nSelect]->SetCol(PAUSE_SELECT_COL_NONE);
		m_apPauseSelect[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// キャラセレクトのステージ選択用ポリゴンを選択された状態にする
//=============================================================================
void CPause::ChangeSelectStagePolygon(int nSelect)
{
	if (m_apPauseSelect[nSelect] != NULL)
	{
		m_apPauseSelect[nSelect]->SetState(CScene2DFlash::STATE_SELECT);
		m_apPauseSelect[nSelect]->SetScale(PAUSE_SELECT_WIDTH_SELECT, PAUSE_SELECT_HEIGHT_SELECT);
		m_apPauseSelect[nSelect]->SetCol(PAUSE_SELECT_COL_SELECT);
		m_apPauseSelect[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// 変数のクリア処理
//=============================================================================
void CPause::ClearVariable(void)
{
	m_pBlackBg = NULL;
	m_pPauseBg = NULL;
	m_nSelect = 0;
	m_nOpenId = 0;
	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{
		m_apPauseSelect[nCntSelect] = NULL;
	}
}

//=============================================================================
// 黒背景を生成する処理
//=============================================================================
void CPause::CreateBlackBg(void)
{
	if (m_pBlackBg != NULL) { return; }
	m_pBlackBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
		PAUSE_BLACKBG_COL, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, PAUSE_PRIORITY);
}

//=============================================================================
// ポーズ背景を生成する処理
//=============================================================================
void CPause::CreatePauseBg(CTextureManager *pTexManager)
{
	if (m_pPauseBg != NULL) { return; }
	m_pPauseBg = CScene2D::Create(PAUSE_BG_POS,PAUSE_BG_COL, PAUSE_BG_WIDTH, PAUSE_BG_HEIGHT, PAUSE_PRIORITY);
	if (m_pPauseBg != NULL)
	{
		m_pPauseBg->BindTexture(pTexManager->GetTexture(PAUSE_BG_TEXIDX));
	}
}

//=============================================================================
// 選択項目を生成する処理
//=============================================================================
void CPause::CreatePauseSelect(CTextureManager *pTexManager)
{
	D3DXVECTOR3 SelectPos = PAUSE_SELECT_POS;
	D3DXCOLOR SelectCol = PAUSE_SELECT_COL_SELECT;
	float fSelectWidth = PAUSE_SELECT_WIDTH_SELECT;
	float fSelectHeight = PAUSE_SELECT_HEIGHT_SELECT;
	CScene2DFlash::STATE SelectState = CScene2DFlash::STATE_SELECT;

	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{
		// ポリゴン生成
		m_apPauseSelect[nCntSelect] = CScene2DFlash::Create(SelectPos, SelectCol, fSelectWidth, fSelectHeight,
			0.0f, SelectState, D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), D3DXCOLOR(0.5f,0.5f,0.5f,0.5f), 0.01f,
			4, PAUSE_PRIORITY);
		if (m_apPauseSelect[nCntSelect] != NULL)
		{
			m_apPauseSelect[nCntSelect]->BindTexture(pTexManager->GetTexture(PAUSE_SELECT_TEXIDX + nCntSelect));
		}

		if (nCntSelect == 0)
		{// 最初以外は選択項目ではないので値を変える
			SelectCol = PAUSE_SELECT_COL_NONE;
			fSelectWidth = PAUSE_SELECT_WIDTH_NONE;
			fSelectHeight = PAUSE_SELECT_HEIGHT_NONE;
			SelectState = CScene2DFlash::STATE_NONE;
		}

		// 下に進める
		SelectPos.y += PAUSE_SELECT_INTERVAL;
	}
}

//=============================================================================
// 黒背景を開放する処理
//=============================================================================
void CPause::ReleaseBlackBg(void)
{
	if (m_pBlackBg != NULL)
	{
		m_pBlackBg->Uninit();
		m_pBlackBg = NULL;
	}
}

//=============================================================================
// ポーズ背景を開放する処理
//=============================================================================
void CPause::ReleasePauseBg(void)
{
	if (m_pPauseBg != NULL)
	{
		m_pPauseBg->Uninit();
		m_pPauseBg = NULL;
	}
}

//=============================================================================
// 選択項目を開放する処理
//=============================================================================
void CPause::ReleasePauseSelect(void)
{
	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{
		if (m_apPauseSelect[nCntSelect] != NULL)
		{
			m_apPauseSelect[nCntSelect]->Uninit();
			m_apPauseSelect[nCntSelect] = NULL;
		}
	}
}

//=============================================================================
// 選択番号を設定する処理
//=============================================================================
void CPause::SetSelect(const int nSelect)
{
	m_nSelect = nSelect;
}

//=============================================================================
// ポーズを開いたプレイヤーの番号を取得する処理
//=============================================================================
void CPause::SetOpenId(const int nId)
{
	m_nOpenId = nId;
}

//=============================================================================
// 黒背景を取得する処理
//=============================================================================
CScene2D *CPause::GetBlackBg(void)
{
	return m_pBlackBg;
}

//=============================================================================
// ポーズ用背景を取得する処理
//=============================================================================
CScene2D *CPause::GetPauseBg(void)
{
	return m_pPauseBg;
}

//=============================================================================
// 選択項目を取得する処理
//=============================================================================
CScene2DFlash *CPause::GetPauseSelect(int nIdx)
{
	return m_apPauseSelect[nIdx];
}

//=============================================================================
// 選択番号を取得する処理
//=============================================================================
int CPause::GetSelect(void)
{
	return m_nSelect;
}

//=============================================================================
// ポーズを開いたプレイヤーの番号を取得する処理
//=============================================================================
int CPause::GetOpenId(void)
{
	return m_nOpenId;
}