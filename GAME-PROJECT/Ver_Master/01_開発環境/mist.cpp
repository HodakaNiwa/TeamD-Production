//=============================================================================
//
// クリーム靄の処理 [mist.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "mist.h"
#include "renderer.h"
#include "manager.h"
#include "functionlib.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CCreamMist::CCreamMist(int nPriority, OBJTYPE objtype) : CScene2D(nPriority, objtype)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CCreamMist::~CCreamMist()
{

}

//=============================================================================
//生成処理
//=============================================================================
CCreamMist *CCreamMist::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CCreamMist *pCreamMist = NULL;	// CCreamMistのポインタ

	if (pCreamMist == NULL)
	{
		pCreamMist = new CCreamMist(nPriority);
		if (pCreamMist != NULL)
		{
			// 値の設定
			pCreamMist->SetPos(pos);
			pCreamMist->SetCol(col);
			pCreamMist->SetWidth(fWidth);
			pCreamMist->SetHeight(fHeight);
			pCreamMist->SetRot(fRot);
			pCreamMist->SetStartTexU(fStartTexU);
			pCreamMist->SetStartTexV(fStartTexV);
			pCreamMist->SetAddTexU(fAddTexU);
			pCreamMist->SetAddTexV(fAddTexV);

			if (FAILED(pCreamMist->Init()))
			{// 初期化に失敗
				return NULL;
			}
		}
	}
	return pCreamMist;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCreamMist::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCreamMist::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CCreamMist::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CCreamMist::Draw(void)
{
	// レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL)return;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	if (pDevice == NULL)return;

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 共通の描画処理
	CScene2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}