//=============================================================================
//
// 花びらの処理 [blossoms.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "blossoms.h"
#include "renderer.h"
#include "manager.h"
#include "functionlib.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CBlossoms::CBlossoms(int nPriority, OBJTYPE objtype) : CScene2D(nPriority, objtype)
{
	// 各種値のクリア
	m_Move = INITIALIZE_D3DXVECTOR3;
	m_fAngleSpeed = 0.0f;
	m_fRotSpeed = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBlossoms::~CBlossoms()
{

}

//=============================================================================
//生成処理
//=============================================================================
CBlossoms *CBlossoms::Create(D3DXVECTOR3 move, float fAngleSpeed, float fRotSpeed, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CBlossoms *pBlossoms = NULL;	// CBlossomsのポインタ

	if (pBlossoms == NULL)
	{
		pBlossoms = new CBlossoms(nPriority);
		if (pBlossoms != NULL)
		{
			// 値の設定
			pBlossoms->SetMove(move);
			pBlossoms->SetAngleSpeed(fAngleSpeed);
			pBlossoms->SetRotSpeed(fRotSpeed);
			pBlossoms->SetPos(pos);
			pBlossoms->SetCol(col);
			pBlossoms->SetWidth(fWidth);
			pBlossoms->SetHeight(fHeight);
			pBlossoms->SetRot(fRot);
			pBlossoms->SetStartTexU(fStartTexU);
			pBlossoms->SetStartTexV(fStartTexV);
			pBlossoms->SetAddTexU(fAddTexU);
			pBlossoms->SetAddTexV(fAddTexV);

			if (FAILED(pBlossoms->Init()))
			{// 初期化に失敗
				return NULL;
			}
		}
	}
	return pBlossoms;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBlossoms::Init(void)
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
void CBlossoms::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBlossoms::Update(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();
	float fAngle = GetAngle();
	float fRot = GetRot();

	// 加算する
	pos += m_Move;
	fAngle += m_fAngleSpeed;
	fRot += m_fRotSpeed;
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
	if (fAngle > D3DX_PI)
	{
		fAngle -= D3DX_PI * 2.0f;
	}
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2.0f;
	}
	if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2.0f;
	}

	if (CheckOutOfScreen(pos) == true)
	{// 画面外に行った
		Uninit();
	}
	else
	{
		// 各種値の設定
		SetPos(pos);
		SetAngle(fAngle);
		SetRot(fRot);
		SetVtxBuffPos();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBlossoms::Draw(void)
{
	// レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL)return;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();
	if (pDevice == NULL)return;

	// アルファテストを行う
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 共通の描画処理
	CScene2D::Draw();

	// アルファテストを行わない設定に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}

//=============================================================================
// 画面外判定処理
//=============================================================================
bool CBlossoms::CheckOutOfScreen(D3DXVECTOR3 pos)
{
	// 各種値の取得
	float fWidth = GetWidth();
	float fHeight = GetHeight();

	if (pos.x + fWidth < 0.0f && pos.y - fHeight > SCREEN_HEIGHT)
	{// 画面外だった
		return true;
	}

	return false;
}

//=============================================================================
// 移動量設定処理
//=============================================================================
void CBlossoms::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
// 角度を回転させるスピード設定処理
//=============================================================================
void CBlossoms::SetAngleSpeed(const float fAngleSpeed)
{
	m_fAngleSpeed = fAngleSpeed;
}

//=============================================================================
// 向きを回転させるスピード設定処理
//=============================================================================
void CBlossoms::SetRotSpeed(const float fRotSpeed)
{
	m_fRotSpeed = fRotSpeed;
}

//=============================================================================
// 移動量取得処理
//=============================================================================
D3DXVECTOR3 CBlossoms::GetMove(void)
{
	return m_Move;
}

//=============================================================================
// 角度を回転させるスピード取得処理
//=============================================================================
float CBlossoms::GetAngleSpeed(void)
{
	return m_fAngleSpeed;
}

//=============================================================================
// 向きを回転させるスピード取得処理
//=============================================================================
float CBlossoms::GetRotSpeed(void)
{
	return m_fRotSpeed;
}