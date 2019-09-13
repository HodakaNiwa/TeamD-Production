//=============================================================================
//
// 勢力図の処理 [power_map.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "power_map.h"
#include "renderer.h"
#include "manager.h"
#include "scene2D.h"
#include "textureManager.h"
#include "UI.h"
#include "functionlib.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define POWERMAP_PRIORITY        (6)

// プレイヤーのアイコンポリゴン初期化用
#define POWERMAP_PLAYERICON_POS_INI       (D3DXVECTOR3(169.0f, 685.0f, 0.0f))
#define POWERMAP_PLAYERICON_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define POWERMAP_PLAYERICON_WIDTH_INI     (32.0f)
#define POWERMAP_PLAYERICON_HEIGHT_INI    (32.0f)
#define POWERMAP_PLAYERICON_TEXIDX        (5)
#define POWERMAP_PLAYERICON_DESTADD       (35.5f)
#define POWERMAP_PLAYERICON_MOVEADD       (1.0f)

// 敵のアイコンポリゴン初期化用
#define POWERMAP_ENEMYICON_POS_INI        (D3DXVECTOR3(929.0f, 685.0f, 0.0f))
#define POWERMAP_ENEMYICON_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define POWERMAP_ENEMYICON_WIDTH_INI      (35.0f)
#define POWERMAP_ENEMYICON_HEIGHT_INI     (35.0f)
#define POWERMAP_ENEMYICON_TEXIDX         (15)

// ストローポリゴン初期化用
#define POWERMAP_STRAW_POS_INI            (D3DXVECTOR3(SCREEN_WIDTH / 2.0f - UI_BG_WIDTH_INI - 1.0f, 690.0f, 0.0f))
#define POWERMAP_STRAW_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define POWERMAP_STRAW_WIDTH_INI          (400.0f)
#define POWERMAP_STRAW_HEIGHT_INI         (35.0f)
#define POWERMAP_STRAW_TEXIDX             (32)

// ストローの中身ポリゴン初期化用
#define POWERMAP_STRAWCONTENTS_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH / 2.0f - UI_BG_WIDTH_INI, 690.0f, 0.0f))
#define POWERMAP_STRAWCONTENTS_COL_INI    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define POWERMAP_STRAWCONTENTS_LEFT_INI   (170.0f)
#define POWERMAP_STRAWCONTENTS_HEIGHT_INI (18.0f)
#define POWERMAP_STRAWCONTENTS_TEXIDX     (33)
#define POWERMAP_STRAWCONTENTS_MOVEADDTEX (0.004f)

//=============================================================================
// コンストラクタ
//=============================================================================
CPowerMap::CPowerMap(int nPriority, OBJTYPE objtype) : CScene(nPriority, objtype)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CPowerMap::~CPowerMap()
{

}

//=============================================================================
//生成処理
//=============================================================================
CPowerMap *CPowerMap::Create(CTextureManager *pTextureManager, int nPriority)
{
	CPowerMap *pPowerMap = NULL;	// CPowerMapのポインタ

	if (pPowerMap == NULL)
	{
		pPowerMap = new CPowerMap(nPriority);
		if (pPowerMap != NULL)
		{
			pPowerMap->SetTextureManager(pTextureManager);
			if (FAILED(pPowerMap->Init()))
			{// 初期化に失敗
				return NULL;
			}
		}
	}
	return pPowerMap;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPowerMap::Init(void)
{
	// 変数のクリア
	ClearVariable();

	// ストローの中身の生成
	CreateStrawContents();

	// ストローの生成
	CreateStraw();

	// 敵のアイコンの生成
	CreateEnemyIcon();

	// プレイヤーのアイコン生成
	CreatePlayerIcon();

	// プレイヤーのアイコンの目的地を設定
	m_fPlayerIconDestPosX = POWERMAP_PLAYERICON_POS_INI.x;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPowerMap::Uninit(void)
{
	// ストローの中身の開放
	ReleaseStrawContents();

	// ストローの開放
	ReleaseStraw();

	// 敵のアイコンの開放
	ReleaseEnemyIcon();

	// プレイヤーのアイコン開放
	ReleasePlayerIcon();

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPowerMap::Update(void)
{
	// ポインタチェック
	if (m_pPlayerIcon == NULL || m_pStrawContents == NULL) { return; }

	// プレイヤーのアイコンを右に動かす
	D3DXVECTOR3 PlayerIconPos = m_pPlayerIcon->GetPos();
	PlayerIconPos.x += POWERMAP_PLAYERICON_MOVEADD;
	m_pPlayerIcon->SetPos(PlayerIconPos);
	m_pPlayerIcon->SetVtxBuffPos();
	if (PlayerIconPos.x >= m_fPlayerIconDestPosX)
	{
		PlayerIconPos.x = m_fPlayerIconDestPosX;
		m_pPlayerIcon->SetPos(PlayerIconPos);
		m_pPlayerIcon->SetVtxBuffPos();
		return;
	}

	// ストローの中身の右端をアイコンにそろえる
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pStrawContents->GetVtxBuff();
	if (pVtxBuff == NULL) { return; }

	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;
	pVtx[1].pos.x = PlayerIconPos.x;
	pVtx[2].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;
	pVtx[3].pos.x = PlayerIconPos.x;

	// テクスチャ座標
	pVtx[1].tex.x += POWERMAP_STRAWCONTENTS_MOVEADDTEX;
	pVtx[3].tex.x += POWERMAP_STRAWCONTENTS_MOVEADDTEX;

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPowerMap::Draw(void)
{

}

//=============================================================================
// ゲージを増やす処理
//=============================================================================
void CPowerMap::AddGauge(void)
{
	// プレイヤーのアイコンの目的地を設定
	m_fPlayerIconDestPosX += POWERMAP_PLAYERICON_DESTADD;
}

//=============================================================================
// ゲージをリセットする処理
//=============================================================================
void CPowerMap::ResetGauge(void)
{
	// ポインタチェック
	if (m_pPlayerIcon == NULL || m_pStrawContents == NULL) { return; }

	// プレイヤーのアイコンを初期値に
	D3DXVECTOR3 PlayerIconPos = m_pPlayerIcon->GetPos();
	PlayerIconPos.x = POWERMAP_PLAYERICON_POS_INI.x;
	m_pPlayerIcon->SetPos(PlayerIconPos);
	m_pPlayerIcon->SetVtxBuffPos();

	// ストローの中身の右端をアイコンにそろえる
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pStrawContents->GetVtxBuff();
	if (pVtxBuff == NULL) { return; }

	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;
	pVtx[1].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;
	pVtx[2].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;
	pVtx[3].pos.x = POWERMAP_STRAWCONTENTS_LEFT_INI;

	// テクスチャ座標
	pVtx[1].tex.x = 0.0f;
	pVtx[3].tex.x = 0.0f;

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 目的の値をリセット
	m_fPlayerIconDestPosX = POWERMAP_PLAYERICON_POS_INI.x;
}

//=============================================================================
// 変数のクリア処理
//=============================================================================
void CPowerMap::ClearVariable(void)
{
	m_pPlayerIcon = NULL;
	m_pEnemyIcon = NULL;
	m_pStraw = NULL;
	m_pStrawContents = NULL;
	m_fPlayerIconDestPosX = 0.0f;
}

//=============================================================================
// プレイヤーのアイコン生成処理
//=============================================================================
void CPowerMap::CreatePlayerIcon(void)
{
	if (m_pPlayerIcon != NULL) { return; }

	m_pPlayerIcon = CScene2D::Create(POWERMAP_PLAYERICON_POS_INI, POWERMAP_PLAYERICON_COL_INI,
		POWERMAP_PLAYERICON_WIDTH_INI, POWERMAP_PLAYERICON_HEIGHT_INI, POWERMAP_PRIORITY);

	// テクスチャを設定
	if (m_pPlayerIcon == NULL || m_pTextureManager == NULL) { return; }
	m_pPlayerIcon->BindTexture(m_pTextureManager->GetTexture(POWERMAP_PLAYERICON_TEXIDX));
}

//=============================================================================
// 敵のアイコン生成処理
//=============================================================================
void CPowerMap::CreateEnemyIcon(void)
{
	if (m_pEnemyIcon != NULL) { return; }

	m_pEnemyIcon = CScene2D::Create(POWERMAP_ENEMYICON_POS_INI, POWERMAP_ENEMYICON_COL_INI,
		POWERMAP_ENEMYICON_WIDTH_INI, POWERMAP_ENEMYICON_HEIGHT_INI, POWERMAP_PRIORITY);

	// テクスチャを設定
	if (m_pEnemyIcon == NULL || m_pTextureManager == NULL) { return; }
	m_pEnemyIcon->BindTexture(m_pTextureManager->GetTexture(POWERMAP_ENEMYICON_TEXIDX));
}

//=============================================================================
// ストローの生成処理
//=============================================================================
void CPowerMap::CreateStraw(void)
{
	if (m_pStraw != NULL) { return; }

	m_pStraw = CScene2D::Create(POWERMAP_STRAW_POS_INI, POWERMAP_STRAW_COL_INI,
		POWERMAP_STRAW_WIDTH_INI, POWERMAP_STRAW_HEIGHT_INI, POWERMAP_PRIORITY);

	// テクスチャを設定
	if (m_pStraw == NULL || m_pTextureManager == NULL) { return; }
	m_pStraw->BindTexture(m_pTextureManager->GetTexture(POWERMAP_STRAW_TEXIDX));
}

//=============================================================================
// ストローの中身生成処理
//=============================================================================
void CPowerMap::CreateStrawContents(void)
{
	if (m_pStrawContents != NULL) { return; }

	m_pStrawContents = CScene2D::Create(POWERMAP_STRAWCONTENTS_POS_INI, POWERMAP_STRAWCONTENTS_COL_INI,
		0.0f, POWERMAP_STRAWCONTENTS_HEIGHT_INI, POWERMAP_PRIORITY, 0.0f, 0.0f, 0.0f,
		0.0f);

	// テクスチャを設定
	if (m_pStrawContents == NULL || m_pTextureManager == NULL) { return; }
	m_pStrawContents->BindTexture(m_pTextureManager->GetTexture(POWERMAP_STRAWCONTENTS_TEXIDX));
}

//=============================================================================
// プレイヤーのアイコン開放処理
//=============================================================================
void CPowerMap::ReleasePlayerIcon(void)
{
	if (m_pPlayerIcon != NULL)
	{
		m_pPlayerIcon->Uninit();
		m_pPlayerIcon = NULL;
	}
}

//=============================================================================
// 敵のアイコンの開放処理
//=============================================================================
void CPowerMap::ReleaseEnemyIcon(void)
{
	if (m_pEnemyIcon != NULL)
	{
		m_pEnemyIcon->Uninit();
		m_pEnemyIcon = NULL;
	}
}

//=============================================================================
// ストローの開放処理
//=============================================================================
void CPowerMap::ReleaseStraw(void)
{
	if (m_pStraw != NULL)
	{
		m_pStraw->Uninit();
		m_pStraw = NULL;
	}
}

//=============================================================================
// ストローの中身開放処理
//=============================================================================
void CPowerMap::ReleaseStrawContents(void)
{
	if (m_pStrawContents != NULL)
	{
		m_pStrawContents->Uninit();
		m_pStrawContents = NULL;
	}
}

//=============================================================================
// プレイヤーのアイコンへのポインタを取得
//=============================================================================
void CPowerMap::SetPlayerIcon(CScene2D *pPlayerIcon)
{
	m_pPlayerIcon = pPlayerIcon;
}

//=============================================================================
// 敵のアイコンへのポインタを取得
//=============================================================================
void CPowerMap::SetEnemyIcon(CScene2D *pEnemyIcon)
{
	m_pEnemyIcon = pEnemyIcon;
}

//=============================================================================
// ストローへのポインタを取得
//=============================================================================
void CPowerMap::SetStraw(CScene2D *pStraw)
{
	m_pStraw = pStraw;
}

//=============================================================================
// ストローの中身へのポインタを取得
//=============================================================================
void CPowerMap::SetStrawContents(CScene2D *pStrawContents)
{
	 m_pStrawContents= pStrawContents;
}

//=============================================================================
// テクスチャ管轄クラスへのポインタを設定
//=============================================================================
void CPowerMap::SetTextureManager(CTextureManager *pTextureManager)
{
	m_pTextureManager = pTextureManager;
}

//=============================================================================
// プレイヤーのアイコンへのポインタを取得
//=============================================================================
CScene2D *CPowerMap::GetPlayerIcon(void)
{
	return m_pPlayerIcon;
}

//=============================================================================
// 敵のアイコンへのポインタを取得
//=============================================================================
CScene2D *CPowerMap::GetEnemyIcon(void)
{
	return m_pEnemyIcon;
}

//=============================================================================
// ストローへのポインタを取得
//=============================================================================
CScene2D *CPowerMap::GetStraw(void)
{
	return m_pStraw;
}

//=============================================================================
// ストローの中身へのポインタを取得
//=============================================================================
CScene2D *CPowerMap::GetStrawContents(void)
{
	return m_pStrawContents;
}

//=============================================================================
// テクスチャ管轄クラスへのポインタを取得
//=============================================================================
CTextureManager *CPowerMap::GetTextureManager(void)
{
	return m_pTextureManager;
}