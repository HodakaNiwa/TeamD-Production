//*****************************************************************************
//
//     UIの処理[UI.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "UI.h"
#include "scene2D.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
// 初期化用マクロ
// 背景用
#define UI_BG_HEIGHT_INI            (SCREEN_HEIGHT / 2)
#define UI_BG_POS_INI               (D3DXVECTOR3(SCREEN_WIDTH - UI_BG_WIDTH_INI,SCREEN_HEIGHT / 2,0.0f))
#define UI_BG_COL_INI               (D3DXCOLOR(0.3f,0.3f,0.3f,1.0f))

// 敵アイコン用
#define UI_ENEMYICON_WIDTH_INI      (90.0f)
#define UI_ENEMYICON_HEIGHT_INI     (SCREEN_HEIGHT / 2)
#define UI_ENEMYICON_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH ,SCREEN_HEIGHT / 2,0.0f))
#define UI_ENEMYICON_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

// プレイヤーアイコン用
// １つ目
#define UI_PLAYERICON_0_WIDTH_INI   (16.0f)
#define UI_PLAYERICON_0_HEIGHT_INI  (16.0f)
#define UI_PLAYERICON_0_POS_INI     (D3DXVECTOR3(SCREEN_WIDTH - 150.0f,410.0f,0.0f))
#define UI_PLAYERICON_0_COL_INI     (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))

// ２つ目
#define UI_PLAYERICON_1_WIDTH_INI   (16.0f)
#define UI_PLAYERICON_1_HEIGHT_INI  (16.0f)
#define UI_PLAYERICON_1_POS_INI     (D3DXVECTOR3(SCREEN_WIDTH - 70.0f ,410.0f,0.0f))
#define UI_PLAYERICON_1_COL_INI     (D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))

// プレイヤーの残機数用
#define UI_PLAYERSTOCK_WIDTH_INI    (90.0f)
#define UI_PLAYERSTOCK_HEIGHT_INI   (SCREEN_HEIGHT / 2)
#define UI_PLAYERSTOCK_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH ,SCREEN_HEIGHT / 2,0.0f))
#define UI_PLAYERSTOCK_COL_INI      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

// ステージアイコン用
#define UI_STAGEICON_WIDTH_INI      (25.0f)
#define UI_STAGEICON_HEIGHT_INI     (25.0f)
#define UI_STAGEICON_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH - 112.5f ,550.0f,0.0f))
#define UI_STAGEICON_COL_INI        (D3DXCOLOR(0.0f,1.0f,0.0f,1.0f))

// ステージ数表示用ポリゴン用
#define UI_STAGENUMBER_WIDTH_INI     (20.0f)
#define UI_STAGENUMBER_HEIGHT_INI    (SCREEN_HEIGHT / 2)
#define UI_STAGENUMBER_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH ,SCREEN_HEIGHT / 2,0.0f))
#define UI_STAGENUMBER_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

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
CUI *CUI::Create(CTextureManager *pTextureManager, int nNumEnemy, int nMapIdx)
{
	CUI *pUI = NULL;  // UIクラス型のポインタ
	if (pUI == NULL)
	{// メモリが空になっている
		pUI = new CUI;
		if (pUI != NULL)
		{// インスタンスを生成できた
			if (FAILED(pUI->Init(pTextureManager, nNumEnemy, nMapIdx)))
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
HRESULT CUI::Init(CTextureManager *pTextureManager, int nNumEnemy, int nMapIdx)
{
	// テクスチャ管轄クラスへのポインタを設定
	m_pTextureManager = pTextureManager;

	// 敵の数を保存
	m_nNumEnemy = nNumEnemy;

	// 変数をクリアする
	ClearVariable();

	// 背景を作成する
	CreateBg();

	// 敵アイコンを作成
	CreateEnemyIcon(nNumEnemy);

	// プレイヤーのアイコンを作成
	CreatePlayerIcon();

	// プレイヤーの残機数表示用ポリゴンを作成
	CreatePlayerStock();

	// ステージアイコンを作成
	CreateStageIcon();

	// 現在のステージ数表示用ポリゴンを作成
	CreateStageNumber(nMapIdx);

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

}

//=============================================================================
//    敵のアイコンを削っていく処理
//=============================================================================
void CUI::CutEnemyIcon(int nIdx)
{

}

//=============================================================================
//    プレイヤーの残機数を再度生成する処理
//=============================================================================
void CUI::ReCreatePlayerStock(int nStock)
{

}

//=============================================================================
//    ステージ数表示用ポリゴンを再度生成する処理
//=============================================================================
void CUI::ReCreateStageNumber(int nMapIdx)
{

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
	m_nStageNumberDigit = 0;      // 現在のステージ数の桁数

	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		m_nCharaStockDigit[nCnt] = 0;    // プレイヤーの残機数の桁数
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

	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		m_pPlayerIcon[nCnt] = CScene2D::Create(PlayerIconPos[nCnt], PlayerIconCol[nCnt],
			fPlayerIconWidth[nCnt], fPlayerIconHeight[nCnt], UI_PRIORITY);
	}
}

//=============================================================================
//    プレイヤーの残機数表示用ポリゴンを生成する
//=============================================================================
void CUI::CreatePlayerStock(void)
{

}

//=============================================================================
//    ステージアイコンを生成する
//=============================================================================
void CUI::CreateStageIcon(void)
{
	m_pStageIcon = CScene2D::Create(UI_STAGEICON_POS_INI, UI_STAGEICON_COL_INI,
		UI_STAGEICON_WIDTH_INI, UI_STAGEICON_HEIGHT_INI, UI_PRIORITY);
}

//=============================================================================
//    現在のステージ数表示用ポリゴンを生成する
//=============================================================================
void CUI::CreateStageNumber(int nMapIdx)
{

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
			for (int nCntDigit = 0; nCntDigit < m_nCharaStockDigit[nCnt]; nCntDigit++)
			{
				if (m_pPlayerStock[nCnt][nCntDigit] != NULL)
				{
					m_pPlayerStock[nCnt][nCntDigit]->Uninit();
					m_pPlayerStock[nCnt][nCntDigit] = NULL;
				}
			}
			delete[] m_pPlayerStock[nCnt];
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
		for (int nCnt = 0; nCnt < m_nStageNumberDigit; nCnt++)
		{
			if (m_pStageNumber[nCnt] != NULL)
			{
				m_pStageNumber[nCnt]->Uninit();
				m_pStageNumber[nCnt] = NULL;
			}
		}
		delete[] m_pStageNumber;
		m_pStageNumber = NULL;
	}
}