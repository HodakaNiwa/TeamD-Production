//*****************************************************************************
//
//     エミッタの処理[emitter.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "emitter.h"
#include "manager.h"
#include "camera.h"
#include "particle.h"
#include "ringEffect.h"
#include "effectManager.h"
#include "input.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define EMITTER_EFFECT_PRIORITY   (5)    // エフェクトの処理優先順位

//*****************************************************************************
//    CEmitterDataの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CEmitterData::CEmitterData()
{
	m_nType = 0;                                // 使用するエフェクトの種類
	m_nTexIdx = 0;                              // 使用するテクスチャの番号
	m_nEffectIdx = 0;                           // 使用するエフェクトの番号
	m_RangeMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // エフェクトを生成する範囲の最大値
	m_RangeMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // エフェクトを生成する範囲の最小値
	m_nLife = 0;                                // 寿命
	m_nLap = 0;                                 // 1回の放出で出すエフェクトの個数
	m_nAppear = 0;                              // エフェクトを出す間隔
	m_bLoop = false;                            // ループするかしないか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEmitterData::~CEmitterData()
{

}

//*****************************************************************************
//    CEmitterの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CEmitter::CEmitter(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値の設定
	m_nType = 0;                            // 種類番号
	m_pTexture = NULL;                      // テクスチャへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 向き
	D3DXMatrixIdentity(&m_MtxWorld);        // ワールドマトリックス
	m_nTexIdx = 0;                          // 使用するテクスチャ番号
	m_nEffectIdx = 0;                       // 使用するエフェクトの番号
	m_nLife = 0;                            // エミッタの寿命
	m_nLap = 0;                             // 1回の放出タイミングに出すエフェクトの個数
	m_nAppear = 0;                          // 放出する間隔
	m_nCounter = 0;                         // 放出タイミングを管理するカウンター
	m_bLoop = false;                        // ループするかしないか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEmitter::~CEmitter()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CEmitter::Init(void)
{
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CEmitter::Uninit(void)
{
	// 自身のポインタを削除
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CEmitter::Update(void)
{
	if (m_bLoop != true)
	{// ループしない設定ならば
		m_nLife--;  // 寿命を減らす
	}

	if (m_nLife <= 0 && m_bLoop == false)
	{// 寿命が尽きてループしない設定ならば
		Uninit();
	}
	else
	{// 寿命がまだある
		m_nCounter++;  // カウンターを増やす
		if (m_nCounter % m_nAppear == 0)
		{// 放出するタイミングになったら
			for (int nCntParticle = 0; nCntParticle < m_nLap; nCntParticle++)
			{// 1回の放出で出す個数分繰り返し
				SetEffect();
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CEmitter::Draw(void)
{

}

//=============================================================================
//    種類番号を設定する処理
//=============================================================================
void CEmitter::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    テクスチャへのポインタ設定処理
//=============================================================================
void CEmitter::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    座標設定処理
//=============================================================================
void CEmitter::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    向き設定処理
//=============================================================================
void CEmitter::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CEmitter::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    使用するテクスチャ番号設定処理
//=============================================================================
void CEmitter::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    種類設定処理
//=============================================================================
void CEmitter::SetEffectIdx(const int nEffectIdx)
{
	m_nEffectIdx = nEffectIdx;
}

//=============================================================================
//    エフェクトを生成する範囲の最大値設定処理
//=============================================================================
void CEmitter::SetRangeMax(const D3DXVECTOR3 RangeMax)
{
	m_RangeMax = RangeMax;
}

//=============================================================================
//    エフェクトを生成する範囲の最小値設定処理
//=============================================================================
void CEmitter::SetRangeMin(const D3DXVECTOR3 RangeMin)
{
	m_RangeMin = RangeMin;
}

//=============================================================================
//    寿命設定処理
//=============================================================================
void CEmitter::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    1回の放出で生成するエフェクトの個数設定処理
//=============================================================================
void CEmitter::SetLap(const int nLap)
{
	m_nLap = nLap;
}

//=============================================================================
//    放出する間隔設定処理
//=============================================================================
void CEmitter::SetAppear(const int nAppear)
{
	m_nAppear = nAppear;
}

//=============================================================================
//    ループするかしないか設定処理
//=============================================================================
void CEmitter::SetLoop(const bool bLoop)
{
	m_bLoop = bLoop;
}

//=============================================================================
//    種類番号を取得する処理
//=============================================================================
int CEmitter::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    テクスチャへのポインタ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CEmitter::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    座標取得処理
//=============================================================================
D3DXVECTOR3 CEmitter::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    向き取得処理
//=============================================================================
D3DXVECTOR3 CEmitter::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CEmitter::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    使用するテクスチャ番号取得処理
//=============================================================================
int CEmitter::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    種類取得処理
//=============================================================================
int CEmitter::GetEffectIdx(void)
{
	return m_nEffectIdx;
}

//=============================================================================
//    エフェクトを生成する範囲の最大値取得処理
//=============================================================================
D3DXVECTOR3 CEmitter::GetRangeMax(void)
{
	return m_RangeMax;
}

//=============================================================================
//    エフェクトを生成する範囲の最小値取得処理
//=============================================================================
D3DXVECTOR3 CEmitter::GetRangeMin(void)
{
	return m_RangeMin;
}

//=============================================================================
//    寿命取得処理
//=============================================================================
int CEmitter::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    1回の放出で生成するエフェクトの個数取得処理
//=============================================================================
int CEmitter::GetLap(void)
{
	return m_nLap;
}

//=============================================================================
//    放出する間隔取得処理
//=============================================================================
int CEmitter::GetAppear(void)
{
	return m_nAppear;
}

//=============================================================================
//    ループするかしないか取得処理
//=============================================================================
bool CEmitter::GetLoop(void)
{
	return m_bLoop;
}

//*****************************************************************************
//    CParEmitterの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CParEmitter::CParEmitter(int nPriority, OBJTYPE objType) : CEmitter(nPriority, objType)
{
	// 各種値のクリア
	m_pParData = NULL;  // パーティクルデータクラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CParEmitter::~CParEmitter()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CParEmitter *CParEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nType, int nPriority)
{
	CParEmitter *pParEmitter = NULL;       // パーティクルエミッタクラス型のポインタ
	if (pParEmitter == NULL)
	{// メモリが空になっている
		pParEmitter = new CParEmitter(nPriority);
		if (pParEmitter != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pParEmitter->SetType(nType);
			pParEmitter->SetParData(pParData);
			pParEmitter->BindTexture(pTexture);
			pParEmitter->SetPos(pos);
			pParEmitter->SetRot(rot);
			pParEmitter->SetTexIdx(nTexIdx);
			pParEmitter->SetEffectIdx(nEffectIdx);
			pParEmitter->SetRangeMax(RangeMax);
			pParEmitter->SetRangeMin(RangeMin);
			pParEmitter->SetLife(nLife);
			pParEmitter->SetLap(nLap);
			pParEmitter->SetAppear(nAppear);
			pParEmitter->SetLoop(bLoop);

			if (FAILED(pParEmitter->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pParEmitter;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CParEmitter::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CEmitter::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CParEmitter::Uninit(void)
{
	// 共通の終了処理
	CEmitter::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CParEmitter::Update(void)
{
	// 共通の更新処理
	CEmitter::Update();
}

//=============================================================================
//    描画処理
//=============================================================================
void CParEmitter::Draw(void)
{
	// 共通の描画処理
	CEmitter::Draw();
}

//=============================================================================
//    パーティクルをセットする処理
//=============================================================================
void CParEmitter::SetEffect(void)
{
	// 生成に必要なデータを作成
	D3DXVECTOR3 pos = GetPos();                               // 座標
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 移動量
	D3DXVECTOR3 ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 移動量の変化量
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // 初期化時の色
	float fWidth = 0.0f;                                      // 幅
	float fHeight = 0.0f;                                     // 高さ
	float fRot = 0.0f;                                        // 向き
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // 色の変化量
	float fSpread = 0.0f;                                     // 半径の変化量
	float fChangeRot = 0.0f;                                  // 向きの変化量
	float fBouncing = 0.0f;                                   // バウンド量
	int nLife = 0;                                            // 体力
	int nRotPattern = 0;                                      // 回転のパターン
	bool bCollision = false;                                  // 当たり判定をするかしないか
	bool bDrawAddtive = false;                                // 加算合成で描画するかしないか
	bool bLighting = false;                                   // ライティングするかしないか

	// データを作成する
	if (m_pParData != NULL)
	{// メモリが確保されている
		// データから代入する部分はデータから引っ張ってくる
		ChangeMove = m_pParData->GetChangeMove();
		InitCol = m_pParData->GetInitCol();
		ChangeCol = m_pParData->GetChangeCol();
		fSpread = m_pParData->GetSpread();
		fBouncing = m_pParData->GetBouncing();
		fChangeRot = m_pParData->GetRotSpeed();
		nRotPattern = m_pParData->GetRotPattern();
		bCollision = m_pParData->GetCollision();
		bDrawAddtive = m_pParData->GetDrawAddtive();
		bLighting = m_pParData->GetLighting();

		// エミッタ自身の向きを反映させるために行列計算
		D3DXMATRIX mtxRot;
		D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);

		// ランダムに出力するところだけ計算する
		int nRange = 0;

		// 向き
		int nMaxRot = (int)m_pParData->GetMaxRot();
		int nMinRot = (int)m_pParData->GetMinRot();
		nRange = nMaxRot - nMinRot;
		if (nRange >= 1)
		{
			fRot = (float)((rand() % nRange));
		}
		else
		{
			fRot = m_pParData->GetMinRot();
		}
		fRot = D3DXToRadian(fRot);

		// 範囲の計算
		D3DXVECTOR3 Range;
		D3DXVECTOR3 RangeMax = GetRangeMax();
		D3DXVECTOR3 RangeMin = GetRangeMin();
		// X方向
		float fRangeX = 0.0f;
		int nRangeMaxX = (int)RangeMax.x;
		int nRangeMinX = (int)RangeMin.x;
		nRange = nRangeMaxX - nRangeMinX;
		if (nRange >= 1)
		{
			fRangeX = (float)(rand() % nRange + nRangeMinX);
		}
		else
		{
			fRangeX = RangeMin.x;
		}

		// Y方向
		float fRangeY = 0.0f;
		int nRangeMaxY = (int)RangeMax.y;
		int nRangeMinY = (int)RangeMin.y;
		nRange = nRangeMaxY - nRangeMinY;
		if (nRange >= 1)
		{
			fRangeY = (float)(rand() % nRange + nRangeMinY);
		}
		else
		{
			fRangeY = RangeMin.y;
		}

		// Z方向
		float fRangeZ = 0.0f;
		int nRangeMaxZ = (int)RangeMax.z;
		int nRangeMinZ = (int)RangeMin.z;
		nRange = nRangeMaxZ - nRangeMinZ;
		if (nRange >= 1)
		{
			fRangeZ = (float)(rand() % nRange + nRangeMinZ);
		}
		else
		{
			fRangeZ = RangeMin.z;
		}
		Range = D3DXVECTOR3(fRangeX, fRangeY, fRangeZ);
		D3DXVec3TransformCoord(&Range, &Range, &mtxRot);

		// 算出された範囲をエミッタの座標に加えたものをパーティクルの放出位置とする
		pos += Range;

		// 移動量の計算
		D3DXVECTOR3 MaxMove = m_pParData->GetMaxMove();
		D3DXVECTOR3 MinMove = m_pParData->GetMinMove();
		D3DXVECTOR3 Move;

		// 移動量を計算する
		// X方向
		float fMoveX = 0.0f;
		int nMaxMoveX = (int)MaxMove.x;
		int nMinMoveX = (int)MinMove.x;
		nRange = nMaxMoveX - nMinMoveX;
		if (nRange >= 1)
		{
			fMoveX = (float)(rand() % nRange + nMinMoveX);
		}
		else
		{
			fMoveX = MinMove.x;
		}

		// Y方向
		float fMoveY = 0.0f;
		int nMaxMoveY = (int)MaxMove.y;
		int nMinMoveY = (int)MinMove.y;
		nRange = nMaxMoveY - nMinMoveY;
		if (nRange >= 1)
		{
			fMoveY = (float)(rand() % nRange + nMinMoveY);
		}
		else
		{
			fMoveY = MinMove.y;
		}

		// Z方向
		float fMoveZ = 0.0f;
		int nMaxMoveZ = (int)MaxMove.z;
		int nMinMoveZ = (int)MinMove.z;
		nRange = nMaxMoveZ - nMinMoveZ;
		if (nRange >= 1)
		{
			fMoveZ = (float)(rand() % nRange + nMinMoveZ);
		}
		else
		{
			fMoveZ = MinMove.z;
		}

		// 向きを反映させ移動量を設定
		Move = D3DXVECTOR3(fMoveX, fMoveY, fMoveZ);
		D3DXVec3TransformCoord(&Move, &Move, &mtxRot);
		D3DXVec3TransformCoord(&ChangeMove, &ChangeMove, &mtxRot);

		// 大きさ
		// 幅
		int nMaxWidth = (int)m_pParData->GetMaxWidth();
		int nMinWidth = (int)m_pParData->GetMinWidth();
		nRange = nMaxWidth - nMinWidth;
		if (nRange >= 1)
		{
			fWidth = (float)((rand() % nRange) + nMinWidth);
		}
		else
		{
			fWidth = (float)nMinWidth;
		}

		// 高さ
		int nMaxHeight = (int)m_pParData->GetMaxHeight();
		int nMinHeight = (int)m_pParData->GetMinHeight();
		nRange = nMaxHeight - nMinHeight;
		if (nRange >= 1)
		{
			fHeight = (float)((rand() % nRange) + nMinHeight);
		}
		else
		{
			fHeight = (float)nMinHeight;
		}

		// 寿命
		int nMaxLife = m_pParData->GetMaxLife();
		int nMinLife = m_pParData->GetMinLife();
		nRange = nMaxLife - nMinLife;
		if (nRange >= 1)
		{
			nLife = rand() % nRange + nMinLife;
		}
		else
		{
			nLife = nMinLife;
		}

		// パーティクルの生成
		CParticle *pParticle = CParticle::Create(pos, Move, ChangeMove, InitCol, fWidth, fHeight, fRot, ChangeCol, fSpread, fChangeRot, fBouncing, nLife, nRotPattern, bCollision, bDrawAddtive, bLighting, EMITTER_EFFECT_PRIORITY);
		if (pParticle != NULL)
		{// パーティクルの生成に成功した
			pParticle->BindTexture(GetTexture());
		}
	}
}

//=============================================================================
//    パーティクルデータへのポインタ設定処理
//=============================================================================
void CParEmitter::SetParData(CParData *pParData)
{
	m_pParData = pParData;
}

//*****************************************************************************
//    CRingEmitterの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CRingEmitter::CRingEmitter(int nPriority, OBJTYPE objType) : CEmitter(nPriority, objType)
{
	// 各種値のクリア
	m_pRingData = NULL;  // リングエフェクトデータクラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRingEmitter::~CRingEmitter()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRingEmitter *CRingEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CRingData *pRingData, LPDIRECT3DTEXTURE9 pTexture, int nType, int nPriority)
{
	CRingEmitter *pRingEmitter = NULL;     // リングエフェクトエミッタクラス型のポインタ
	if (pRingEmitter == NULL)
	{// メモリが空になっている
		pRingEmitter = new CRingEmitter(nPriority);
		if (pRingEmitter != NULL)
		{// インスタンスを生成できた
		 // 各種値の設定
			pRingEmitter->SetType(nType);
			pRingEmitter->SetRingData(pRingData);
			pRingEmitter->BindTexture(pTexture);
			pRingEmitter->SetPos(pos);
			pRingEmitter->SetRot(rot);
			pRingEmitter->SetTexIdx(nTexIdx);
			pRingEmitter->SetEffectIdx(nEffectIdx);
			pRingEmitter->SetRangeMax(RangeMax);
			pRingEmitter->SetRangeMin(RangeMin);
			pRingEmitter->SetLife(nLife);
			pRingEmitter->SetLap(nLap);
			pRingEmitter->SetAppear(nAppear);
			pRingEmitter->SetLoop(bLoop);

			if (FAILED(pRingEmitter->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pRingEmitter;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRingEmitter::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CEmitter::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRingEmitter::Uninit(void)
{
	// 共通の終了処理
	CEmitter::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRingEmitter::Update(void)
{
	// 共通の更新処理
	CEmitter::Update();
}

//=============================================================================
//    描画処理
//=============================================================================
void CRingEmitter::Draw(void)
{
	// 共通の描画処理
	CEmitter::Draw();
}

//=============================================================================
//    リングエフェクトをセットする処理
//=============================================================================
void CRingEmitter::SetEffect(void)
{
	// 生成に必要なデータを作成
	D3DXVECTOR3 pos = GetPos();                               // 座標
	D3DXVECTOR3 ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 移動量の変化量
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // 初期化時の色
	float fHeight = 0.0f;                                     // 高さ
	float fRadius = 0.0f;                                     // 半径
	float fDiffusion = 0.0f;                                  // 幅
	float fChangeHeight = 0.0f;                               // 高さを毎フレームどれくらい変化させるか
	float fChangeRadius = 0.0f;                               // 半径を毎フレームどれくらい変化させるか
	float fChangeDiffusion = 0.0f;                            // 幅を毎フレームどれくらい変化させるか
	float fRot = 0.0f;                                        // 向き
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // 色の変化量
	float fGravity = 0.0f;                                    // 重力
	float fSpread = 0.0f;                                     // 半径の変化量
	D3DXVECTOR3 RotSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);       // 向きの変化量
	float fBouncing = 0.0f;                                   // バウンド量
	int nLife = 0;                                            // 体力
	int nXBlock = 0;                                          // 横の分割数
	int nYBlock = 0;                                          // 縦の分割数
	int nRotPattern = 0;                                      // 回転のパターン
	bool bCulling = false;                                    // カリングをするかしないか
	bool bDrawAddtive = false;                                // 加算合成で描画するかしないか

	// データを作成する
	if (m_pRingData != NULL)
	{// メモリが確保されている
	    // データから代入する部分はデータから引っ張ってくる
		ChangeMove = m_pRingData->GetChangeMove();
		InitCol = m_pRingData->GetInitCol();
		ChangeCol = m_pRingData->GetChangeCol();
		fHeight = m_pRingData->GetHeight();
		fRadius = m_pRingData->GetRadius();
		fDiffusion = m_pRingData->GetDiffusion();
		fChangeHeight = m_pRingData->GetChangeHeight();
		fChangeRadius = m_pRingData->GetChangeRadius();
		fChangeDiffusion = m_pRingData->GetChangeDiffusion();
		RotSpeed = m_pRingData->GetRotSpeed();
		nXBlock = m_pRingData->GetXBlock();
		nYBlock = m_pRingData->GetYBlock();
		nRotPattern = m_pRingData->GetRotPattern();
		bCulling = m_pRingData->GetCulling();
		bDrawAddtive = m_pRingData->GetDrawAddtive();

		// エミッタ自身の向きを反映させるために行列計算
		D3DXMATRIX mtxRot;
		D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);

		// ランダムに出力するところだけ計算する
		int nRange = 0;

		// 範囲の計算
		D3DXVECTOR3 Range;
		D3DXVECTOR3 RangeMax = GetRangeMax();
		D3DXVECTOR3 RangeMin = GetRangeMin();
		// X方向
		float fRangeX = 0.0f;
		int nRangeMaxX = (int)RangeMax.x;
		int nRangeMinX = (int)RangeMin.x;
		nRange = nRangeMaxX - nRangeMinX;
		if (nRange >= 1)
		{
			fRangeX = (float)(rand() % nRange + nRangeMinX);
		}
		else
		{
			fRangeX = RangeMin.x;
		}

		// Y方向
		float fRangeY = 0.0f;
		int nRangeMaxY = (int)RangeMax.y;
		int nRangeMinY = (int)RangeMin.y;
		nRange = nRangeMaxY - nRangeMinY;
		if (nRange >= 1)
		{
			fRangeY = (float)(rand() % nRange + nRangeMinY);
		}
		else
		{
			fRangeY = RangeMin.y;
		}

		// Z方向
		float fRangeZ = 0.0f;
		int nRangeMaxZ = (int)RangeMax.z;
		int nRangeMinZ = (int)RangeMin.z;
		nRange = nRangeMaxZ - nRangeMinZ;
		if (nRange >= 1)
		{
			fRangeZ = (float)(rand() % nRange + nRangeMinZ);
		}
		else
		{
			fRangeZ = RangeMin.z;
		}
		Range = D3DXVECTOR3(fRangeX, fRangeY, fRangeZ);
		D3DXVec3TransformCoord(&Range, &Range, &mtxRot);

		// 算出された範囲をエミッタの座標に加えたものをパーティクルの放出位置とする
		pos += Range;

		// 向きの計算
		D3DXVECTOR3 MaxRot = m_pRingData->GetMaxRot();
		D3DXVECTOR3 MinRot = m_pRingData->GetMinRot();
		D3DXVECTOR3 Rot;

		// X方向
		float fRotX = 0.0f;
		int nMaxRotX = (int)MaxRot.x;
		int nMinRotX = (int)MinRot.x;
		nRange = nMaxRotX - nMinRotX;
		if (nRange >= 1)
		{
			fRotX = (float)(rand() % nRange + nMinRotX);
		}
		else
		{
			fRotX = MinRot.x;
		}

		// Y方向
		float fRotY = 0.0f;
		int nMaxRotY = (int)MaxRot.y;
		int nMinRotY = (int)MinRot.y;
		nRange = nMaxRotY - nMinRotY;
		if (nRange >= 1)
		{
			fRotY = (float)(rand() % nRange + nMinRotY);
		}
		else
		{
			fRotY = MinRot.y;
		}

		// Z方向
		float fRotZ = 0.0f;
		int nMaxRotZ = (int)MaxRot.z;
		int nMinRotZ = (int)MinRot.z;
		nRange = nMaxRotZ - nMinRotZ;
		if (nRange >= 1)
		{
			fRotZ = (float)(rand() % nRange + nMinRotZ);
		}
		else
		{
			fRotZ = MinRot.z;
		}
		Rot = D3DXVECTOR3(fRotX, fRotY, fRotZ);

		// エミッタ自身の向きも加算する
		D3DXVECTOR3 EmitterRot = GetRot();
		EmitterRot.x = D3DXToDegree(EmitterRot.x);
		EmitterRot.y = D3DXToDegree(EmitterRot.y);
		EmitterRot.z = D3DXToDegree(EmitterRot.z);
		Rot += EmitterRot;

		// 向きを円周率換算にしておく
		Rot.x = D3DXToRadian(Rot.x);
		Rot.y = D3DXToRadian(Rot.y);
		Rot.z = D3DXToRadian(Rot.z);


		// 移動量の計算
		D3DXVECTOR3 MaxMove = m_pRingData->GetMaxMove();
		D3DXVECTOR3 MinMove = m_pRingData->GetMinMove();
		D3DXVECTOR3 Move;

		// 移動量を計算する
		// X方向
		float fMoveX = 0.0f;
		int nMaxMoveX = (int)MaxMove.x;
		int nMinMoveX = (int)MinMove.x;
		nRange = nMaxMoveX - nMinMoveX;
		if (nRange >= 1)
		{
			fMoveX = (float)(rand() % nRange + nMinMoveX);
		}
		else
		{
			fMoveX = MinMove.x;
		}

		// Y方向
		float fMoveY = 0.0f;
		int nMaxMoveY = (int)MaxMove.y;
		int nMinMoveY = (int)MinMove.y;
		nRange = nMaxMoveY - nMinMoveY;
		if (nRange >= 1)
		{
			fMoveY = (float)(rand() % nRange + nMinMoveY);
		}
		else
		{
			fMoveY = MinMove.y;
		}

		// Z方向
		float fMoveZ = 0.0f;
		int nMaxMoveZ = (int)MaxMove.z;
		int nMinMoveZ = (int)MinMove.z;
		nRange = nMaxMoveZ - nMinMoveZ;
		if (nRange >= 1)
		{
			fMoveZ = (float)(rand() % nRange + nMinMoveZ);
		}
		else
		{
			fMoveZ = MinMove.z;
		}

		// 向きを反映させ移動量を設定
		Move = D3DXVECTOR3(fMoveX, fMoveY, fMoveZ);
		D3DXVec3TransformCoord(&Move, &Move, &mtxRot);
		D3DXVec3TransformCoord(&ChangeMove, &ChangeMove, &mtxRot);

		// 寿命
		int nMaxLife = m_pRingData->GetMaxLife();
		int nMinLife = m_pRingData->GetMinLife();
		nRange = nMaxLife - nMinLife;
		if (nRange >= 1)
		{
			nLife = rand() % nRange + nMinLife;
		}
		else
		{
			nLife = nMinLife;
		}

		// リングエフェクトの生成
		CRingEffect *pRingEffect = CRingEffect::Create(pos, Rot, InitCol, fHeight, fRadius, fDiffusion, nXBlock, nYBlock, nLife, RotSpeed, nRotPattern, Move, ChangeMove, ChangeCol, fChangeHeight, fChangeRadius, fChangeDiffusion, bCulling, bDrawAddtive, EMITTER_EFFECT_PRIORITY);
		if (pRingEffect != NULL)
		{// リングエフェクトの生成に成功した
			pRingEffect->BindTexture(GetTexture());
		}
	}
}

//=============================================================================
//    リングエフェクトデータへのポインタ設定処理
//=============================================================================
void CRingEmitter::SetRingData(CRingData *pRingData)
{
	m_pRingData = pRingData;
}