//*****************************************************************************
//
//     UIの処理[UI.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "UI.h"
#include "scene2D.h"
#include "number.h"
#include "title.h"
#include "charaselect.h"
#include "textureManager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
// 初期化用マクロ
// 背景用
#define UI_BG_HEIGHT_INI            (SCREEN_HEIGHT / 2)
#define UI_BG_POS_INI               (D3DXVECTOR3(SCREEN_WIDTH - UI_BG_WIDTH_INI,SCREEN_HEIGHT / 2,0.0f))
#define UI_BG_COL_INI               (D3DXCOLOR(0.3f,0.3f,0.3f,1.0f))

// 敵アイコン用
#define UI_ENEMYICON_WIDTH_INI      (16.0f)
#define UI_ENEMYICON_HEIGHT_INI     (16.0f)
#define UI_ENEMYICON_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH - 110.0f,36.0f,0.0f))
#define UI_ENEMYICON_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define UI_ENEMYICON_INTERVAL_X     (30.0f)
#define UI_ENEMYICON_INTERVAL_Y     (30.0f)
#define UI_ENEMYICON_TEXIDX         (19)

// プレイヤーアイコン用
#define UI_PLAYERICON_TEXIDX        (5)

// １つ目
#define UI_PLAYERICON_0_WIDTH_INI   (16.0f)
#define UI_PLAYERICON_0_HEIGHT_INI  (16.0f)
#define UI_PLAYERICON_0_POS_INI     (D3DXVECTOR3(SCREEN_WIDTH - 150.0f,410.0f,0.0f))
#define UI_PLAYERICON_0_COL_INI     (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

// ２つ目
#define UI_PLAYERICON_1_WIDTH_INI   (16.0f)
#define UI_PLAYERICON_1_HEIGHT_INI  (16.0f)
#define UI_PLAYERICON_1_POS_INI     (D3DXVECTOR3(SCREEN_WIDTH - 70.0f ,410.0f,0.0f))
#define UI_PLAYERICON_1_COL_INI     (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

// プレイヤーの残機数用
// １つ目
#define UI_PLAYERSTOCK_0_WIDTH_INI  (15.0f)
#define UI_PLAYERSTOCK_0_HEIGHT_INI (15.0f)
#define UI_PLAYERSTOCK_0_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH - 115.0f ,410.0f,0.0f))
#define UI_PLAYERSTOCK_0_COL_INI    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

// ２つ目
#define UI_PLAYERSTOCK_1_WIDTH_INI  (15.0f)
#define UI_PLAYERSTOCK_1_HEIGHT_INI (15.0f)
#define UI_PLAYERSTOCK_1_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH - 35.0f ,410.0f,0.0f))
#define UI_PLAYERSTOCK_1_COL_INI    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

// ステージアイコン用
#define UI_STAGEICON_WIDTH_INI      (25.0f)
#define UI_STAGEICON_HEIGHT_INI     (25.0f)
#define UI_STAGEICON_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH - 112.5f ,550.0f,0.0f))
#define UI_STAGEICON_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define UI_STAGEICON_TEXIDX         (24)

// ステージ数表示用ポリゴン用
#define UI_STAGENUMBER_WIDTH_INI    (20.0f)
#define UI_STAGENUMBER_HEIGHT_INI   (20.0f)
#define UI_STAGENUMBER_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH - 68.5f ,550.0f,0.0f))
#define UI_STAGENUMBER_COL_INI      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CUI::CUI()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CUI *CUI::Create(CTextureManager *pTextureManager, int nNumEnemy, int nStageIdx, int *pPlayerStock, int nTexNumber)
{
	CUI *pUI = NULL;  // UIクラス型のポインタ
	if (pUI == NULL)
	{// メモリが空になっている
		pUI = new CUI;
		if (pUI != NULL)
		{// インスタンスを生成できた
			if (FAILED(pUI->Init(pTextureManager, nNumEnemy, nStageIdx, pPlayerStock, nTexNumber)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pUI;
}


//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CUI::Init(CTextureManager *pTextureManager, int nNumEnemy, int nStageIdx, int *pPlayerStock, int nTexNumber)
{
	// 変数をクリアする
	ClearVariable();

	// テクスチャ管轄クラスへのポインタを設定
	m_pTextureManager = pTextureManager;

	// 敵の数を保存
	m_nNumEnemy = nNumEnemy;

	// 数字に使用するテクスチャの番号を保存
	m_nNumberTexIdx = nTexNumber;

	// 背景を作成する
	CreateBg();

	// 敵アイコンを作成
	CreateEnemyIcon(nNumEnemy);

	// プレイヤーのアイコンを作成
	CreatePlayerIcon();

	// プレイヤーの残機数表示用ポリゴンを作成
	CreatePlayerStock(pPlayerStock);

	// ステージアイコンを作成
	CreateStageIcon();

	// 現在のステージ数表示用ポリゴンを作成
	CreateStageNumber(nStageIdx);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CUI::Uninit(void)
{
	// 背景の破棄
	CreateBg();

	// 敵アイコンの破棄
	ReleaseEnemyIcon();

	// プレイヤーのアイコンの破棄
	ReleasePlayerIcon();

	// プレイヤーの残機数表示用ポリゴンの破棄
	ReleasePlayerStock();

	// ステージアイコンの破棄
	ReleaseStageIcon();

	// 現在のステージ数表示用ポリゴンの破棄
	ReleaseStageNumber();
}

//=============================================================================
//    敵のアイコンを再度生成する処理
//=============================================================================
void CUI::ReCreateEnemyIcon(int nNumEnemy)
{
	// 現在のポリゴンを開放
	ReleaseEnemyIcon();

	// もう一度作る
	CreateEnemyIcon(nNumEnemy);

	// 数を保存
	m_nNumEnemy = nNumEnemy;
}

//=============================================================================
//    敵のアイコンを削っていく処理
//=============================================================================
void CUI::CutEnemyIcon(void)
{
	if (m_pEnemyIcon[(m_nNumEnemy - 1)] != NULL)
	{
		m_pEnemyIcon[(m_nNumEnemy - 1)]->Uninit();
		m_pEnemyIcon[(m_nNumEnemy - 1)] = NULL;
		m_nNumEnemy--;
	}
}

//=============================================================================
//    プレイヤーの残機数を再度生成する処理
//=============================================================================
void CUI::ReCreatePlayerStock(int *pStock)
{
	// 現在のポリゴンを開放
	ReleasePlayerStock();

	// もう一度作る
	CreatePlayerStock(pStock);
}

//=============================================================================
//    ステージ数表示用ポリゴンを再度生成する処理
//=============================================================================
void CUI::ReCreateStageNumber(int nStageIdx)
{
	// 現在のポリゴンを開放
	ReleaseStageNumber();

	// もう一度作る
	CreateStageNumber(nStageIdx);
}

//=============================================================================
//    変数をクリアする
//=============================================================================
void CUI::ClearVariable(void)
{
	m_pTextureManager = NULL;     // テクスチャ管轄クラスへのポインタ(ベースモードクラスから取得する)
	m_pBg = NULL;                 // 背景に使用するポリゴンクラスへのポインタ
	m_pEnemyIcon = NULL;          // 敵のアイコン表示用ポリゴンクラスへのポインタ
	m_pStageIcon = NULL;          // ステージアイコン表示用ポリゴン
	m_pStageNumber = NULL;        // 現在のステージ数表示用ポリゴン
	m_nNumEnemy = 0;              // 敵のアイコンの数
	m_nNumberTexIdx = 0;          // 数字に使用するテクスチャの番号

	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		m_pPlayerIcon[nCnt] = NULL;       // プレイヤーアイコン表示用ポリゴン
		m_pPlayerStock[nCnt] = NULL;      // プレイヤーの残機数表示用ポリゴン
	}
}

//=============================================================================
//    背景を生成する
//=============================================================================
void CUI::CreateBg(void)
{
	m_pBg = CScene2D::Create(UI_BG_POS_INI, UI_BG_COL_INI, UI_BG_WIDTH_INI, UI_BG_HEIGHT_INI, UI_PRIORITY);
}

//=============================================================================
//    敵のアイコンを生成する
//=============================================================================
void CUI::CreateEnemyIcon(int nNumEnemy)
{
	if (nNumEnemy == 0) { return; }

	m_pEnemyIcon = new CScene2D*[nNumEnemy];
	if (m_pEnemyIcon == NULL) { return; }

	// アイコン生成
	D3DXVECTOR3 EnemyIconPos = UI_ENEMYICON_POS_INI;
	D3DXCOLOR EnemyIconCol = UI_ENEMYICON_COL_INI;
	float fEnemyIconWidth = UI_ENEMYICON_WIDTH_INI;
	float fEnemyIconHeight = UI_ENEMYICON_HEIGHT_INI;
	for (int nCntEnemy = 0; nCntEnemy < nNumEnemy; nCntEnemy++)
	{
		m_pEnemyIcon[nCntEnemy] = CScene2D::Create(EnemyIconPos, EnemyIconCol, fEnemyIconWidth, fEnemyIconHeight, UI_PRIORITY);
		if (m_pEnemyIcon[nCntEnemy] != NULL)
		{
			m_pEnemyIcon[nCntEnemy]->BindTexture(m_pTextureManager->GetTexture(UI_ENEMYICON_TEXIDX));
		}

		// アイコンの位置をずらす
		EnemyIconPos.x += UI_ENEMYICON_INTERVAL_X;
		if ((nCntEnemy + 1) % 2 == 0)
		{
			EnemyIconPos.x = UI_ENEMYICON_POS_INI.x;
			EnemyIconPos.y += UI_ENEMYICON_INTERVAL_Y;
		}
	}
}

//=============================================================================
//    プレイヤーアイコンを生成する
//=============================================================================
void CUI::CreatePlayerIcon(void)
{
	D3DXVECTOR3 PlayerIconPos[MAX_NUM_PLAYER] = { UI_PLAYERICON_0_POS_INI, UI_PLAYERICON_1_POS_INI };
	D3DXCOLOR PlayerIconCol[MAX_NUM_PLAYER] = { UI_PLAYERICON_0_COL_INI, UI_PLAYERICON_1_COL_INI };
	float fPlayerIconWidth[MAX_NUM_PLAYER] = { UI_PLAYERICON_0_WIDTH_INI, UI_PLAYERICON_1_WIDTH_INI };
	float fPlayerIconHeight[MAX_NUM_PLAYER] = { UI_PLAYERICON_0_HEIGHT_INI, UI_PLAYERICON_1_HEIGHT_INI };

	// 作成する数をプレイする人数で決定
	int nMaxPlayer = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// 1人プレイならば
		nMaxPlayer--;
	}

	for (int nCnt = 0; nCnt < nMaxPlayer; nCnt++)
	{
		m_pPlayerIcon[nCnt] = CScene2D::Create(PlayerIconPos[nCnt], PlayerIconCol[nCnt],
			fPlayerIconWidth[nCnt], fPlayerIconHeight[nCnt], UI_PRIORITY);
		if (m_pTextureManager != NULL && m_pPlayerIcon[nCnt] != NULL)
		{
			m_pPlayerIcon[nCnt]->BindTexture(m_pTextureManager->GetTexture(UI_PLAYERICON_TEXIDX + CCharaSelect::GetPlayerNumber(nCnt)));
		}
	}
}

//=============================================================================
//    プレイヤーの残機数表示用ポリゴンを生成する
//=============================================================================
void CUI::CreatePlayerStock(int *pStock)
{
	D3DXVECTOR3 PlayerStockPos[MAX_NUM_PLAYER] = { UI_PLAYERSTOCK_0_POS_INI, UI_PLAYERSTOCK_1_POS_INI };
	D3DXCOLOR PlayerStockCol[MAX_NUM_PLAYER] = { UI_PLAYERSTOCK_0_COL_INI, UI_PLAYERSTOCK_1_COL_INI };
	float fPlayerStockWidth[MAX_NUM_PLAYER] = { UI_PLAYERSTOCK_0_WIDTH_INI, UI_PLAYERSTOCK_1_WIDTH_INI };
	float fPlayerStockHeight[MAX_NUM_PLAYER] = { UI_PLAYERSTOCK_0_HEIGHT_INI, UI_PLAYERSTOCK_1_HEIGHT_INI };
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (m_pTextureManager != NULL)
	{
		pTexture = m_pTextureManager->GetTexture(m_nNumberTexIdx);
	}

	// 作成する数をプレイする人数で決定
	int nMaxPlayer = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// 1人プレイならば
		nMaxPlayer--;
	}

	for (int nCnt = 0; nCnt < nMaxPlayer; nCnt++)
	{
		m_pPlayerStock[nCnt] = CNumber::Create(PlayerStockPos[nCnt], PlayerStockCol[nCnt],
			fPlayerStockWidth[nCnt], fPlayerStockHeight[nCnt], INITIALIZE_D3DXVECTOR3, pTexture,
			pStock[nCnt], CNumberPolygon::STATE_NONE, 3, 0.0f, UI_PRIORITY);
	}
}

//=============================================================================
//    ステージアイコンを生成する
//=============================================================================
void CUI::CreateStageIcon(void)
{
	m_pStageIcon = CScene2D::Create(UI_STAGEICON_POS_INI, UI_STAGEICON_COL_INI,
		UI_STAGEICON_WIDTH_INI, UI_STAGEICON_HEIGHT_INI, UI_PRIORITY);
	if (m_pStageIcon != NULL)
	{
		m_pStageIcon->BindTexture(m_pTextureManager->GetTexture(UI_STAGEICON_TEXIDX));
	}
}

//=============================================================================
//    現在のステージ数表示用ポリゴンを生成する
//=============================================================================
void CUI::CreateStageNumber(int nStageIdx)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (m_pTextureManager != NULL)
	{
		pTexture = m_pTextureManager->GetTexture(m_nNumberTexIdx);
	}

	m_pStageNumber = CNumber::Create(UI_STAGENUMBER_POS_INI, UI_STAGENUMBER_COL_INI,
		UI_STAGENUMBER_WIDTH_INI, UI_STAGENUMBER_HEIGHT_INI, INITIALIZE_D3DXVECTOR3, pTexture,
		nStageIdx, CNumberPolygon::STATE_NONE, 3, 0.0f, UI_PRIORITY);
}

//=============================================================================
//    背景を開放する
//=============================================================================
void CUI::ReleaseBg(void)
{
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}
}

//=============================================================================
//    敵のアイコンを開放する
//=============================================================================
void CUI::ReleaseEnemyIcon(void)
{
	if (m_pEnemyIcon != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumEnemy; nCnt++)
		{
			if (m_pEnemyIcon[nCnt] != NULL)
			{
				m_pEnemyIcon[nCnt]->Uninit();
				m_pEnemyIcon[nCnt] = NULL;
			}
		}
		delete[] m_pEnemyIcon;
		m_pEnemyIcon = NULL;
	}
}

//=============================================================================
//    プレイヤーアイコンを開放する
//=============================================================================
void CUI::ReleasePlayerIcon(void)
{
	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (m_pPlayerIcon[nCnt] != NULL)
		{
			m_pPlayerIcon[nCnt]->Uninit();
			m_pPlayerIcon[nCnt] = NULL;
		}
	}
}

//=============================================================================
//    プレイヤーの残機数表示用ポリゴンを開放する
//=============================================================================
void CUI::ReleasePlayerStock(void)
{
	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (m_pPlayerStock[nCnt] != NULL)
		{
			m_pPlayerStock[nCnt]->Uninit();
			m_pPlayerStock[nCnt] = NULL;
		}
	}
}

//=============================================================================
//    ステージアイコンを開放する
//=============================================================================
void CUI::ReleaseStageIcon(void)
{
	if (m_pStageIcon != NULL)
	{
		m_pStageIcon->Uninit();
		m_pStageIcon = NULL;
	}
}

//=============================================================================
//    現在のステージ数表示用ポリゴンを開放する
//=============================================================================
void CUI::ReleaseStageNumber(void)
{
	if (m_pStageNumber != NULL)
	{
		m_pStageNumber->Uninit();
		m_pStageNumber = NULL;
	}
}