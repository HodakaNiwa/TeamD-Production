//*****************************************************************************
//
//     飛沫の処理[splash.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "splash.h"
#include "scene2D.h"
#include "manager.h"
#include "basemode.h"
#include "textureManager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define SPLASH_ALPHA_CUT         (0.02f)  // 飛沫の透明度を1フレームでどれくらい下げるか
#define SPLASH_APPEAR_SCALE_UP   (5.0f)   // 飛沫が出現したときどれくらい大きくなるか
#define SPLASH_CHANGE_TIMING     (60)     // 飛沫の大きさ変化をどのタイミングで変えるか

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CSplash::CSplash(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	m_State = STATE_APPEAR;      // 状態
	m_nCounter = 0;              // 飛沫の大きさ変化を管理するカウンター
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSplash::~CSplash()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSplash *CSplash::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nTexIdx, D3DXVECTOR3 StartMove, float fPosYDown, float fRollSpeed, float fLengthMax, float fLengthTransition, int nLife, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CSplash *pSplash = NULL;  // 飛沫クラス型のポインタ
	if (pSplash == NULL)
	{// メモリが空になっている
		pSplash = new CSplash(nPriority);
		if (pSplash != NULL)
		{// インスタンスを生成できた
			// 各種値の設定
			pSplash->SetPos(pos);
			pSplash->SetCol(col);
			pSplash->SetWidth(fWidth);
			pSplash->SetHeight(fHeight);
			pSplash->SetTexIdx(nTexIdx);
			pSplash->SetStartMove(StartMove);
			pSplash->SetPosYDown(fPosYDown);
			pSplash->SetRollSpeed(fRollSpeed);
			pSplash->SetLengthMax(fLengthMax);
			pSplash->SetLengthTransition(fLengthTransition);
			pSplash->SetLife(nLife);
			pSplash->SetRot(fRot);
			pSplash->SetStartTexU(fStartTexU);
			pSplash->SetStartTexV(fStartTexV);
			pSplash->SetAddTexU(fAddTexU);
			pSplash->SetAddTexV(fAddTexV);

			if (FAILED(pSplash->Init()))
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

	return pSplash;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSplash::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	// テクスチャを設定する
	CBasemode *pBase = CManager::GetBaseMode();
	if (pBase != NULL)
	{
		CTextureManager *pTexManager = pBase->GetTextureManager();
		if (pTexManager != NULL)
		{
			BindTexture(pTexManager->GetTexture(m_nTexIdx));
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSplash::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSplash::Update(void)
{
	// 回転させる
	float fRot = GetRot();
	fRot += m_fRollSpeed;
	if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2.0f;
	}
	SetRot(fRot);

	switch (m_State)
	{// 状態によって処理わけ
	case STATE_APPEAR:   // 出現状態
		AppearUpdate();
		break;
	case STATE_NORMAL:   // 通常状態
		NormalUpdate();
		break;
	case STATE_VANISH:   // 消える状態
		VanishUpdate();
		break;
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CSplash::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}

//=============================================================================
//    出現状態の更新処理
//=============================================================================
void CSplash::AppearUpdate(void)
{
	float fLength = GetLength();
	D3DXVECTOR3 pos = GetPos();

	// 拡大させる
	fLength += SPLASH_APPEAR_SCALE_UP;
	if (fLength >= m_fLengthMax)
	{// 大きさが最大値を超えた
		fLength = m_fLengthMax;  // 大きさを戻す
		m_State = STATE_NORMAL;  // 通常状態にする
	}

	// 位置をだんだんずらす
	pos += m_StartMove;
	SetPos(pos);

	// 大きさの設定
	SetLength(fLength);
	SetVtxBuffPos();
}

//=============================================================================
//    通常状態の更新処理
//=============================================================================
void CSplash::NormalUpdate(void)
{
	float fLength = GetLength();
	D3DXVECTOR3 pos = GetPos();

	// カウンターを増やす
	m_nCounter++;

	// 大きさを変化させる
	fLength += m_fLengthTransition;
	if (m_nCounter % SPLASH_CHANGE_TIMING == 0)
	{// 大きさの変化がある程度たった
		m_fLengthTransition *= -1;
	}

	// 位置をだんだんずらす
	pos.y += m_fPosYDown;
	SetPos(pos);

	// 頂点バッファへ値を設定
	SetLength(fLength);
	SetVtxBuffPos();

	// 寿命を減らす
	m_nLife--;
	if (m_nLife <= 0)
	{
		m_nLife = 0;
		m_State = STATE_VANISH;
	}
}

//=============================================================================
//    消える状態の更新処理
//=============================================================================
void CSplash::VanishUpdate(void)
{
	// 通常時の処理
	NormalUpdate();

	// 透明度を減らす
	D3DXCOLOR col = GetCol();
	col.a -= SPLASH_ALPHA_CUT;

	// 色の設定
	SetCol(col);
	SetVtxBuffCol();

	if (col.a <= 0.0f)
	{// 透明度がなくなった
		Uninit();
	}
}

//=============================================================================
//    状態設定処理
//=============================================================================
void CSplash::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
//    使用するテクスチャの番号設定処理
//=============================================================================
void CSplash::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    飛沫ポリゴンが出現したときの移動量設定処理
//=============================================================================
void CSplash::SetStartMove(const D3DXVECTOR3 StartMove)
{
	m_StartMove = StartMove;
}

//=============================================================================
//    飛沫ポリゴンが下がるときの移動量設定処理
//=============================================================================
void CSplash::SetPosYDown(const float fPosYDown)
{
	m_fPosYDown = fPosYDown;
}

//=============================================================================
//    飛沫ポリゴンの回転スピード設定処理
//=============================================================================
void CSplash::SetRollSpeed(const float fRollSpeed)
{
	m_fRollSpeed = fRollSpeed;
}

//=============================================================================
//    飛沫ポリゴンの最大の大きさ設定処理
//=============================================================================
void CSplash::SetLengthMax(const float fLengthMax)
{
	m_fLengthMax = fLengthMax;
}

//=============================================================================
//    飛沫ポリゴンの大きさ変化量設定処理
//=============================================================================
void CSplash::SetLengthTransition(const float fLengthTransition)
{
	m_fLengthTransition = fLengthTransition;
}

//=============================================================================
//    寿命設定処理
//=============================================================================
void CSplash::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    飛沫の大きさ変化を管理するカウンター設定処理
//=============================================================================
void CSplash::SetCounter(const int nCounter)
{
	m_nCounter = nCounter;
}

//=============================================================================
//    状態取得処理
//=============================================================================
CSplash::STATE CSplash::GetState(void)
{
	return m_State;
}

//=============================================================================
//    使用するテクスチャの番号取得処理
//=============================================================================
int CSplash::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    飛沫ポリゴンの最大の大きさ取得処理
//=============================================================================
float CSplash::GetLengthMax(void)
{
	return m_fLengthMax;
}

//=============================================================================
//    飛沫ポリゴンの大きさ変化量取得処理
//=============================================================================
float CSplash::GetLengthTransition(void)
{
	return m_fLengthTransition;
}

//=============================================================================
//    寿命取得処理
//=============================================================================
int CSplash::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    飛沫の大きさ変化を管理するカウンター取得処理
//=============================================================================
int CSplash::GetCounter(void)
{
	return m_nCounter;
}

//=============================================================================
//    飛沫ポリゴンが出現したときの移動量取得処理
//=============================================================================
D3DXVECTOR3 CSplash::GetMove(void)
{
	return m_StartMove;
}

//=============================================================================
//    飛沫ポリゴンが下がるときの移動量取得処理
//=============================================================================
float CSplash::GetPosYDown(void)
{
	return m_fPosYDown;
}

//=============================================================================
//    飛沫ポリゴンの回転スピード取得処理
//=============================================================================
float CSplash::GetRollSpeed(void)
{
	return m_fRollSpeed;
}