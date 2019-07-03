//*****************************************************************************
//
//     エミッタの処理[emitter.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "emitter.h"
#include "manager.h"
#include "basemode.h"
#include "camera.h"
#include "cameraManager.h"
#include "particle.h"
#include "ringEffect.h"
#include "effectManager.h"
#include "input.h"

#include "debugproc.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define EMITTER_EDIT_EFFECTALPAH  (0.2f) // マップエディット時のエフェクトの透明度

//*****************************************************************************
//    CEmitterDataの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CEmitterData::CEmitterData()
{
	m_nType = 0;         // 使用するエフェクトの種類
	m_nTexIdx = 0;       // 使用するテクスチャの番号
	m_nEffectIdx = 0;    // 使用するエフェクトの番号
	m_fRangeMax = 0.0f;  // エフェクトを生成する範囲の最大値
	m_fRangeMin = 0.0f;  // エフェクトを生成する範囲の最小値
	m_nLife = 0;         // 寿命
	m_nLap = 0;          // 1回の放出で出すエフェクトの個数
	m_nAppear = 0;       // エフェクトを出す間隔
	m_bLoop = false;     // ループするかしないか
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
	m_pTexture = NULL;                      // テクスチャへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 向き
	D3DXMatrixIdentity(&m_MtxWorld);        // ワールドマトリックス
	m_nType = 0;                            // 使用するエフェクトの種類
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
//    移動処理
//=============================================================================
void CEmitter::Movement(void)
{
	// キーボードの取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// カメラを取得
	CCamera *pCamera = CManager::GetBaseMode()->GetCameraManager()->GetCamera();

	if (pKeyboard->GetPress(DIK_A) == true)
	{// 左方向の入力がされた
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_Pos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
			m_Pos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_Pos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
			m_Pos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_Pos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
			m_Pos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
		}
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// 右方向の入力がされた
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_Pos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
			m_Pos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_Pos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
			m_Pos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_Pos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
			m_Pos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
		}
	}
	else if (pKeyboard->GetPress(DIK_W) == true)
	{// 上方向の入力がされた
	    // 移動処理
		m_Pos.x += sinf(pCamera->GetRot().y) * EMITTER_MOVEMENT;
		m_Pos.z += cosf(pCamera->GetRot().y) * EMITTER_MOVEMENT;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{// 下方向の入力がされた
	    // 移動処理
		m_Pos.x += sinf(pCamera->GetRot().y + D3DX_PI) * EMITTER_MOVEMENT;
		m_Pos.z += cosf(pCamera->GetRot().y + D3DX_PI) * EMITTER_MOVEMENT;
	}

	if (pKeyboard->GetPress(DIK_R) == true)
	{// 縦方向の入力がされた
	    // 移動処理
		m_Pos.y += EMITTER_MOVEMENT;
	}
	else if (pKeyboard->GetPress(DIK_V) == true)
	{// 縦方向の入力がされた
	    // 移動処理
		m_Pos.y -= EMITTER_MOVEMENT;
	}

	if (pKeyboard->GetPress(DIK_Q) == true)
	{// Qキーの入力がされた
	    // 回転処理
		m_Rot.y += EMITTER_ROTATION;
		if (m_Rot.y > D3DX_PI)
		{// 円周率を超えた
			m_Rot.y -= D3DX_PI * 2.0f;
		}
	}
	else if (pKeyboard->GetPress(DIK_E) == true)
	{// Eキーの入力がされた
	    // 回転処理
		m_Rot.y -= EMITTER_ROTATION;
		if (m_Rot.y < -D3DX_PI)
		{// 円周率を超えた
			m_Rot.y += D3DX_PI * 2.0f;
		}
	}
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
//    使用するエフェクトの種類設定処理
//=============================================================================
void CEmitter::SetType(const int nType)
{
	m_nType = nType;
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
void CEmitter::SetRangeMax(const float fRangeMax)
{
	m_fRangeMax = fRangeMax;
}

//=============================================================================
//    エフェクトを生成する範囲の最小値設定処理
//=============================================================================
void CEmitter::SetRangeMin(const float fRangeMin)
{
	m_fRangeMin = fRangeMin;
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
//    使用するエフェクトの種類取得処理
//=============================================================================
int CEmitter::GetType(void)
{
	return m_nType;
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
float CEmitter::GetRangeMax(void)
{
	return m_fRangeMax;
}

//=============================================================================
//    エフェクトを生成する範囲の最小値取得処理
//=============================================================================
float CEmitter::GetRangeMin(void)
{
	return m_fRangeMin;
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
CParEmitter *CParEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, float fRangeMax, float fRangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nPriority)
{
	CParEmitter *pParEmitter = NULL;       // パーティクルエミッタクラス型のポインタ
	if (pParEmitter == NULL)
	{// メモリが空になっている
		pParEmitter = new CParEmitter(nPriority);
		if (pParEmitter != NULL)
		{// インスタンスを生成できた
		 // 各種値の設定
			pParEmitter->SetParData(pParData);
			pParEmitter->BindTexture(pTexture);
			pParEmitter->SetPos(pos);
			pParEmitter->SetRot(rot);
			pParEmitter->SetType(nType);
			pParEmitter->SetTexIdx(nTexIdx);
			pParEmitter->SetEffectIdx(nEffectIdx);
			pParEmitter->SetRangeMax(fRangeMax);
			pParEmitter->SetRangeMin(fRangeMin);
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
	D3DXVECTOR3 Range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // 生成する範囲
	float fMaxRot = 0.0f;                                     // 生成時の向き(最大値)
	float fMinRot = 0.0f;                                     // 生成時の向き(最小値)
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // 初期化時の色
	float fWidth = 0.0f;                                      // 幅
	float fHeight = 0.0f;                                     // 高さ
	float fRot = 0.0f;                                        // 向き
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // 色の変化量
	float fGravity = 0.0f;                                    // 重力
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
		fMaxRot = D3DXToRadian(m_pParData->GetMaxRot());
		fMinRot = D3DXToRadian(m_pParData->GetMinRot());
		Range = m_pParData->GetRange();
		ChangeMove = m_pParData->GetChangeMove();
		InitCol = m_pParData->GetInitCol();
		ChangeCol = m_pParData->GetChangeCol();
		fSpread = m_pParData->GetSpread();
		fGravity = m_pParData->GetGravity();
		fBouncing = m_pParData->GetBouncing();
		fChangeRot = m_pParData->GetRotSpeed();
		nRotPattern = m_pParData->GetRotPattern();
		bCollision = m_pParData->GetCollision();
		bDrawAddtive = m_pParData->GetDrawAddtive();
		bLighting = m_pParData->GetLighting();

		// ランダムに出力するところだけ計算する
		float fRandom = 0.0f;
		float fRange = 0.0f;
		float fAngle = 0.0f;

		// 向き
		fMaxRot *= 100.0f;
		fMinRot *= 100.0f;
		fRandom = (fMaxRot - fMinRot);
		if (fRandom > 0 && fRandom >= 0.1f)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fMinRot + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRot = fAngle;

		// 座標の計算
		float fMag = D3DXToRadian(m_pParData->GetRangeMag());

		// X方向
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.x += sinf(fRandom / 100.0f) * Range.x;
		}

		// Y方向
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.y += sinf(fRandom / 100.0f) * Range.y;
		}

		// Z方向
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.z += cosf(fRandom / 100.0f) * Range.z;
		}

		// 移動量の計算
		D3DXVECTOR3 MaxMove = m_pParData->GetMaxMove();
		D3DXVECTOR3 MinMove = m_pParData->GetMinMove();

		// X方向
		// 角度の算出
		float fAngleMax = D3DXToRadian(GetRangeMax());
		float fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 0.1f)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.x - MinMove.x);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.x;
			Move.x = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.x = MinMove.x;
		}


		// Y方向
		// 角度の算出
		fAngleMax = D3DXToRadian(GetRangeMax());
		fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.y - MinMove.y);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.y;
			Move.y = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.y = MinMove.y;
		}

		// Z方向
		// 角度の算出
		fAngleMax = D3DXToRadian(GetRangeMax());
		fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.z - MinMove.z);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.z;
			Move.z = cosf(fAngle) * fRandom;
		}
		else
		{
			Move.z = MinMove.z;
		}

		// 大きさ
		// 幅
		float MaxWidth = m_pParData->GetMaxWidth();
		float MinWidth = m_pParData->GetMinWidth();
		fRange = MaxWidth - MinWidth;
		if (fRange > 0 && fRange >= 1)
		{
			fWidth = rand() % (int)fRange + MinWidth;
		}
		else
		{
			fWidth = MinWidth;
		}

		// 高さ
		float MaxHeight = m_pParData->GetMaxHeight();
		float MinHeight = m_pParData->GetMinHeight();
		fRange = MaxHeight - MinHeight;
		if (fRange > 0 && fRange >= 1)
		{
			fHeight = rand() % (int)fRange + MinHeight;
		}
		else
		{
			fHeight = MinHeight;
		}

		// 寿命
		int MaxLife = m_pParData->GetMaxLife();
		int MinLife = m_pParData->GetMinLife();
		fRange = (float)(MaxLife - MinLife);
		if (fRange > 0 && fRange >= 1)
		{
			nLife = rand() % (int)fRange + MinLife;
		}
		else
		{
			nLife = MinLife;
		}

		// パーティクルの生成
		CParticle *pParticle = CParticle::Create(pos, Move, ChangeMove, InitCol, fWidth, fHeight, fRot, ChangeCol, fGravity, fSpread, fChangeRot, fBouncing, nLife, nRotPattern, bCollision, bDrawAddtive, bLighting, EFFECT_PRIORITY);
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

//=============================================================================
//    パーティクルデータへのポインタ取得処理
//=============================================================================
CParData *CParEmitter::GetParData(void)
{
	return m_pParData;
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
CRingEmitter *CRingEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, float fRangeMax, float fRangeMin, int nLife, int nLap, int nAppear, bool bLoop, CRingData *pRingData, LPDIRECT3DTEXTURE9 pTexture, int nPriority)
{
	CRingEmitter *pRingEmitter = NULL;     // リングエフェクトエミッタクラス型のポインタ
	if (pRingEmitter == NULL)
	{// メモリが空になっている
		pRingEmitter = new CRingEmitter(nPriority);
		if (pRingEmitter != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pRingEmitter->SetRingData(pRingData);
			pRingEmitter->BindTexture(pTexture);
			pRingEmitter->SetPos(pos);
			pRingEmitter->SetRot(rot);
			pRingEmitter->SetType(nType);
			pRingEmitter->SetTexIdx(nTexIdx);
			pRingEmitter->SetEffectIdx(nEffectIdx);
			pRingEmitter->SetRangeMax(fRangeMax);
			pRingEmitter->SetRangeMin(fRangeMin);
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
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 移動量
	D3DXVECTOR3 ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 移動量の変化量
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 向き
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
		rot = m_pRingData->GetRot();
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

		// ランダムに出力するところだけ計算する
		float fRandom = 0.0f;
		float fRange = 0.0f;
		float fAngle = 0.0f;

		// 移動量の計算
		D3DXVECTOR3 MaxMove = m_pRingData->GetMaxMove();
		D3DXVECTOR3 MinMove = m_pRingData->GetMinMove();

		// X方向
		// 角度の算出
		float fAngleMax = D3DXToRadian(GetRangeMax());
		float fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 0.1f)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.x - MinMove.x);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.x;
			Move.x = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.x = MinMove.x;
		}


		// Y方向
		// 角度の算出
		fAngleMax = D3DXToRadian(GetRangeMax());
		fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.y - MinMove.y);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.y;
			Move.y = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.y = MinMove.y;
		}

		// Z方向
		// 角度の算出
		fAngleMax = D3DXToRadian(GetRangeMax());
		fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.z - MinMove.z);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.z;
			Move.z = cosf(fAngle) * fRandom;
		}
		else
		{
			Move.z = MinMove.z;
		}

		// 寿命
		int MaxLife = m_pRingData->GetMaxLife();
		int MinLife = m_pRingData->GetMinLife();
		fRange = (float)(MaxLife - MinLife);
		if (fRange > 0 && fRange >= 1)
		{
			nLife = rand() % (int)fRange + MinLife;
		}
		else
		{
			nLife = MinLife;
		}

		// リングエフェクトの生成
		CRingEffect *pRingEffect = CRingEffect::Create(pos, rot, InitCol, fHeight, fRadius, fDiffusion, nXBlock, nYBlock, nLife, RotSpeed, nRotPattern, Move, ChangeMove, ChangeCol, fChangeHeight, fChangeRadius, fChangeDiffusion, bCulling, bDrawAddtive, EFFECT_PRIORITY);
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

//=============================================================================
//    リングエフェクトデータへのポインタ取得処理
//=============================================================================
CRingData *CRingEmitter::GetRingData(void)
{
	return m_pRingData;
}


//*****************************************************************************
//    CSetParEmitterの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CSetParEmitter::CSetParEmitter(int nPriority, OBJTYPE objType) : CParEmitter(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CSetParEmitter::~CSetParEmitter()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSetParEmitter *CSetParEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, float fRangeMax, float fRangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nPriority)
{
	CSetParEmitter *pSetParEmitter = NULL; // 配置用パーティクルエミッタクラス型のポインタ
	if (pSetParEmitter == NULL)
	{// メモリが空になっている
		pSetParEmitter = new CSetParEmitter(nPriority);
		if (pSetParEmitter != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pSetParEmitter->SetParData(pParData);
			pSetParEmitter->BindTexture(pTexture);
			pSetParEmitter->SetPos(pos);
			pSetParEmitter->SetRot(rot);
			pSetParEmitter->SetType(nType);
			pSetParEmitter->SetTexIdx(nTexIdx);
			pSetParEmitter->SetEffectIdx(nEffectIdx);
			pSetParEmitter->SetRangeMax(fRangeMax);
			pSetParEmitter->SetRangeMin(fRangeMin);
			pSetParEmitter->SetLife(nLife);
			pSetParEmitter->SetLap(nLap);
			pSetParEmitter->SetAppear(nAppear);
			pSetParEmitter->SetLoop(bLoop);

			if (FAILED(pSetParEmitter->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pSetParEmitter;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSetParEmitter::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CParEmitter::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSetParEmitter::Uninit(void)
{
	// 共通の終了処理
	CParEmitter::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSetParEmitter::Update(void)
{
	// 共通の更新処理
	CParEmitter::Update();
}

//=============================================================================
//    描画処理
//=============================================================================
void CSetParEmitter::Draw(void)
{
	// 共通の描画処理
	CParEmitter::Draw();
}

//=============================================================================
//    パーティクルをセットする処理
//=============================================================================
void CSetParEmitter::SetEffect(void)
{
	// 生成に必要なデータを作成
	D3DXVECTOR3 pos = GetPos();                               // 座標
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 移動量
	D3DXVECTOR3 ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 移動量の変化量
	D3DXVECTOR3 Range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // 生成する範囲
	float fMaxRot = 0.0f;                                     // 生成時の向き(最大値)
	float fMinRot = 0.0f;                                     // 生成時の向き(最小値)
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // 初期化時の色
	float fWidth = 0.0f;                                      // 幅
	float fHeight = 0.0f;                                     // 高さ
	float fRot = 0.0f;                                        // 向き
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // 色の変化量
	float fGravity = 0.0f;                                    // 重力
	float fSpread = 0.0f;                                     // 半径の変化量
	float fChangeRot = 0.0f;                                  // 向きの変化量
	float fBouncing = 0.0f;                                   // バウンド量
	int nLife = 0;                                            // 体力
	int nRotPattern = 0;                                      // 回転のパターン
	bool bCollision = false;                                  // 当たり判定をするかしないか
	bool bDrawAddtive = false;                                // 加算合成で描画するかしないか
	bool bLighting = false;                                   // ライティングするかしないか
	CParData *pParData = GetParData();                        // パーティクルデータクラスを取得

	// データを作成する
	if (pParData != NULL)
	{// メモリが確保されている
	    // データから代入する部分はデータから引っ張ってくる
		fMaxRot = D3DXToRadian(pParData->GetMaxRot());
		fMinRot = D3DXToRadian(pParData->GetMinRot());
		Range = pParData->GetRange();
		ChangeMove = pParData->GetChangeMove();
		InitCol = pParData->GetInitCol();
		InitCol.a = EMITTER_EDIT_EFFECTALPAH;
		ChangeCol = pParData->GetChangeCol();
		fSpread = pParData->GetSpread();
		fGravity = pParData->GetGravity();
		fBouncing = pParData->GetBouncing();
		fChangeRot = pParData->GetRotSpeed();
		nRotPattern = pParData->GetRotPattern();
		bCollision = pParData->GetCollision();
		bDrawAddtive = pParData->GetDrawAddtive();
		bLighting = pParData->GetLighting();

		// ランダムに出力するところだけ計算する
		float fRandom = 0.0f;
		float fRange = 0.0f;
		float fAngle = 0.0f;

		// 向き
		fMaxRot *= 100.0f;
		fMinRot *= 100.0f;
		fRandom = (fMaxRot - fMinRot);
		if (fRandom > 0 && fRandom >= 0.1f)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fMinRot + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRot = fAngle;

		// 座標の計算
		float fMag = D3DXToRadian(pParData->GetRangeMag());

		// X方向
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.x += sinf(fRandom / 100.0f) * Range.x;
		}

		// Y方向
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.y += sinf(fRandom / 100.0f) * Range.y;
		}

		// Z方向
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.z += cosf(fRandom / 100.0f) * Range.z;
		}

		// 移動量の計算
		D3DXVECTOR3 MaxMove = pParData->GetMaxMove();
		D3DXVECTOR3 MinMove = pParData->GetMinMove();

		// X方向
		// 角度の算出
		float fAngleMax = D3DXToRadian(GetRangeMax());
		float fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 0.1f)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.x - MinMove.x);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.x;
			Move.x = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.x = MinMove.x;
		}


		// Y方向
		// 角度の算出
		fAngleMax = D3DXToRadian(GetRangeMax());
		fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.y - MinMove.y);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.y;
			Move.y = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.y = MinMove.y;
		}

		// Z方向
		// 角度の算出
		fAngleMax = D3DXToRadian(GetRangeMax());
		fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.z - MinMove.z);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.z;
			Move.z = cosf(fAngle) * fRandom;
		}
		else
		{
			Move.z = MinMove.z;
		}

		// 大きさ
		// 幅
		float MaxWidth = pParData->GetMaxWidth();
		float MinWidth = pParData->GetMinWidth();
		fRange = MaxWidth - MinWidth;
		if (fRange > 0 && fRange >= 1)
		{
			fWidth = rand() % (int)fRange + MinWidth;
		}
		else
		{
			fWidth = MinWidth;
		}

		// 高さ
		float MaxHeight = pParData->GetMaxHeight();
		float MinHeight = pParData->GetMinHeight();
		fRange = MaxHeight - MinHeight;
		if (fRange > 0 && fRange >= 1)
		{
			fHeight = rand() % (int)fRange + MinHeight;
		}
		else
		{
			fHeight = MinHeight;
		}

		// 寿命
		int MaxLife = pParData->GetMaxLife();
		int MinLife = pParData->GetMinLife();
		fRange = (float)(MaxLife - MinLife);
		if (fRange > 0 && fRange >= 1)
		{
			nLife = rand() % (int)fRange + MinLife;
		}
		else
		{
			nLife = MinLife;
		}

		// パーティクルの生成
		CParticle *pParticle = CParticle::Create(pos, Move, ChangeMove, InitCol, fWidth, fHeight, fRot, ChangeCol, fGravity, fSpread, fChangeRot, fBouncing, nLife, nRotPattern, bCollision, bDrawAddtive, bLighting, EFFECT_PRIORITY);
		if (pParticle != NULL)
		{// パーティクルの生成に成功した
			pParticle->BindTexture(GetTexture());
		}
	}
}

//*****************************************************************************
//    CRingEmitterの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CSetRingEmitter::CSetRingEmitter(int nPriority, OBJTYPE objType) : CRingEmitter(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CSetRingEmitter::~CSetRingEmitter()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSetRingEmitter *CSetRingEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, float fRangeMax, float fRangeMin, int nLife, int nLap, int nAppear, bool bLoop, CRingData *pRingData, LPDIRECT3DTEXTURE9 pTexture, int nPriority)
{
	CSetRingEmitter *pSetRingEmitter = NULL;   // 配置用リングエフェクトエミッタクラス型のポインタ
	if (pSetRingEmitter == NULL)
	{// メモリが空になっている
		pSetRingEmitter = new CSetRingEmitter(nPriority);
		if (pSetRingEmitter != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pSetRingEmitter->SetRingData(pRingData);
			pSetRingEmitter->BindTexture(pTexture);
			pSetRingEmitter->SetPos(pos);
			pSetRingEmitter->SetRot(rot);
			pSetRingEmitter->SetType(nType);
			pSetRingEmitter->SetTexIdx(nTexIdx);
			pSetRingEmitter->SetEffectIdx(nEffectIdx);
			pSetRingEmitter->SetRangeMax(fRangeMax);
			pSetRingEmitter->SetRangeMin(fRangeMin);
			pSetRingEmitter->SetLife(nLife);
			pSetRingEmitter->SetLap(nLap);
			pSetRingEmitter->SetAppear(nAppear);
			pSetRingEmitter->SetLoop(bLoop);

			if (FAILED(pSetRingEmitter->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pSetRingEmitter;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSetRingEmitter::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CRingEmitter::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSetRingEmitter::Uninit(void)
{
	// 共通の終了処理
	CRingEmitter::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSetRingEmitter::Update(void)
{
	// 共通の更新処理
	CRingEmitter::Update();
}

//=============================================================================
//    描画処理
//=============================================================================
void CSetRingEmitter::Draw(void)
{
	// 共通の描画処理
	CRingEmitter::Draw();
}

//=============================================================================
//    リングエフェクトをセットする処理
//=============================================================================
void CSetRingEmitter::SetEffect(void)
{
	// 生成に必要なデータを作成
	D3DXVECTOR3 pos = GetPos();                               // 座標
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 移動量
	D3DXVECTOR3 ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 移動量の変化量
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 向き
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
	D3DXVECTOR3 RotSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 向きの変化量
	float fBouncing = 0.0f;                                   // バウンド量
	int nLife = 0;                                            // 体力
	int nXBlock = 0;                                          // 横の分割数
	int nYBlock = 0;                                          // 縦の分割数
	int nRotPattern = 0;                                      // 回転のパターン
	bool bCulling = false;                                    // カリングをするかしないか
	bool bDrawAddtive = false;                                // 加算合成で描画するかしないか
	CRingData *pRingData = GetRingData();                     // リングエフェクトデータクラスを取得

	// データを作成する
	if (pRingData != NULL)
	{// メモリが確保されている
	    // データから代入する部分はデータから引っ張ってくる
		rot = pRingData->GetRot();
		ChangeMove = pRingData->GetChangeMove();
		InitCol = pRingData->GetInitCol();
		InitCol.a = EMITTER_EDIT_EFFECTALPAH;
		ChangeCol = pRingData->GetChangeCol();
		fHeight = pRingData->GetHeight();
		fRadius = pRingData->GetRadius();
		fDiffusion = pRingData->GetDiffusion();
		fChangeHeight = pRingData->GetChangeHeight();
		fChangeRadius = pRingData->GetChangeRadius();
		fChangeDiffusion = pRingData->GetChangeDiffusion();
		RotSpeed = pRingData->GetRotSpeed();
		nXBlock = pRingData->GetXBlock();
		nYBlock = pRingData->GetYBlock();
		nRotPattern = pRingData->GetRotPattern();
		bCulling = pRingData->GetCulling();
		bDrawAddtive = pRingData->GetDrawAddtive();

		// ランダムに出力するところだけ計算する
		float fRandom = 0.0f;
		float fRange = 0.0f;
		float fAngle = 0.0f;

		// 移動量の計算
		D3DXVECTOR3 MaxMove = pRingData->GetMaxMove();
		D3DXVECTOR3 MinMove = pRingData->GetMinMove();

		// X方向
		// 角度の算出
		float fAngleMax = D3DXToRadian(GetRangeMax());
		float fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 0.1f)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.x - MinMove.x);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.x;
			Move.x = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.x = MinMove.x;
		}


		// Y方向
		// 角度の算出
		fAngleMax = D3DXToRadian(GetRangeMax());
		fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.y - MinMove.y);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.y;
			Move.y = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.y = MinMove.y;
		}

		// Z方向
		// 角度の算出
		fAngleMax = D3DXToRadian(GetRangeMax());
		fAngleMin = D3DXToRadian(GetRangeMin());
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + GetRot().y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.z - MinMove.z);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.z;
			Move.z = cosf(fAngle) * fRandom;
		}
		else
		{
			Move.z = MinMove.z;
		}

		// 寿命
		int MaxLife = pRingData->GetMaxLife();
		int MinLife = pRingData->GetMinLife();
		fRange = (float)(MaxLife - MinLife);
		if (fRange > 0 && fRange >= 1)
		{
			nLife = rand() % (int)fRange + MinLife;
		}
		else
		{
			nLife = MinLife;
		}

		// リングエフェクトの生成
		CRingEffect *pRingEffect = CRingEffect::Create(pos, rot, InitCol, fHeight, fRadius, fDiffusion, nXBlock, nYBlock, nLife, RotSpeed, nRotPattern, Move, ChangeMove, ChangeCol, fChangeHeight, fChangeRadius, fChangeDiffusion, bCulling, bDrawAddtive, EFFECT_PRIORITY);
		if (pRingEffect != NULL)
		{// リングエフェクトの生成に成功した
			pRingEffect->BindTexture(GetTexture());
		}
	}
}