//*****************************************************************************
//
//     パーティクルの処理[particle.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"

//*****************************************************************************
//    CParDataの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CParData::CParData()
{
	m_fMaxRot = 0.0f;                                // 生成時の向き(最大値)
	m_fMinRot = 0.0f;                                // 生成時の向き(最小値)
	m_MaxMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 移動量の最大値
	m_MinMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 移動量の最小値
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 移動量の変化量
	m_InitCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // 生成時の色
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // フレーム毎の色の変化量
	m_fMaxWidth = 0.0f;                              // 幅の最大値
	m_fMinWidth = 0.0f;                              // 幅の最小値
	m_fMaxHeight = 0.0f;                             // 高さの最大値
	m_fMinHeight = 0.0f;                             // 高さの最小値
	m_fSpread = 0.0f;                                // 大きさの変化量
	m_nMaxLife = 0;                                  // 寿命の最大値
	m_nMinLife = 0;                                  // 寿命の最小値
	m_fBouncing = 0.0f;                              // バウンド量
	m_nRotPattern = 0;                               // 回転の種類[ 0:時計回り 1:反時計回り 2:ランダム ]
	m_fRotSpeed = 0.0f;                              // 回転するスピード
	m_bCollision = false;                            // 当たり判定をするかどうか
	m_bDrawAddtive = true;                           // 加算合成するかしないか[ 0:なし 1:あり ]
	m_bLighting = false;                             // ライティングするかしないか[ 0:なし 1:あり ]
}

//=============================================================================
//    デストラクタ
//=============================================================================
CParData::~CParData()
{

}

//*****************************************************************************
//    CParticleの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CParticle::CParticle(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// 各種値のクリア
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // どれくらい色が変化するか
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 移動量
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 移動量の変化量
	m_fSpread = 0.0f;                                // どれくらい変形するか
	m_fChangeRot = 0.0f;                             // どれくらい回転するか
	m_fBouncing = 0.0f;                              // 地面についた際にバウンドする値
	m_nLife = 0;                                     // 寿命
	m_nRotPattern = 0;                               // 回転の種類
	m_bCollision = false;                            // バウンドするかどうか
	m_bDrawAddtive = false;                          // 加算合成で描画するかどうか
	m_bLighting = false;                             // ライティングするかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
	float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fSpread,
	float fChangeRot, float fBouncing, int nLife, int nRotPattern,
	bool bCol, bool bDrawAddtive, bool bLighting, int nPriority)
{
	CParticle *pParticle = NULL;  // パーティクルクラス型のポインタ
	if (pParticle == NULL)
	{// メモリが空になっている
		pParticle = new CParticle(nPriority);
		if (pParticle != NULL)
		{// インスタンスを生成できた
			if (FAILED(pParticle->Init(pos, move, ChangeMove, col, fWidth, fHeight, fRot, ChangeCol, fSpread, fChangeRot, fBouncing, nLife, nRotPattern, bCol, bDrawAddtive, bLighting)))
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

	return pParticle;   // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
	float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fSpread,
	float fChangeRot, float fBouncing, int nLife, int nRotPattern,
	bool bCol, bool bDrawAddtive, bool bLighting)
{
	// 各種値の設定
	SetPos(pos);                       // 座標
	SetCol(col);                       // 色
	SetWidth(fWidth);                  // 幅
	SetHeight(fHeight);                // 高さ
	SetRot(fRot);                      // 向き
	m_Move = move;                     // 移動量
	m_ChangeMove = ChangeMove;         // 移動量の変化量
	m_ChangeCol = ChangeCol;           // 色
	m_fSpread = fSpread;               // 半径の変化量
	m_fChangeRot = fChangeRot;         // 向きの変化量
	m_fBouncing = fBouncing;           // バウンド量
	m_nLife = nLife;                   // 寿命
	m_nRotPattern = nRotPattern;       // 回転のパターン
	m_bCollision = bCol;               // バウンドするかしないか
	m_bDrawAddtive = bDrawAddtive;     // 加算合成で描画するかしないか
	m_bLighting = bLighting;           // ライティングするかしないか

	// 共通の初期化処理
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	// 共通の終了処理
	CSceneBillboard::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CParticle::Update(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();  // 座標
	D3DXCOLOR col = GetCol();    // 色
	float fRot = GetRot();       // 向き
	float fRadius = GetLength(); // 半径

	m_nLife--;               // 寿命を減らす
	fRadius += m_fSpread;    // 大きさの更新

	if (m_nLife <= 0 || fRadius < 0.0f)
	{// 寿命が0以下になった
		Uninit();	// 終了処理
	}
	else
	{// まだ表示時間がある
		// 各種値の更新
		pos += m_Move;				 // 位置の更新
		col += m_ChangeCol;          // 色の更新

		// 移動量を変化させる
		m_Move += m_ChangeMove;

		// 当たり判定を取るかチェック
		if (pos.y <= 0 && m_bCollision == true)
		{// バウンド
			m_Move.y += (m_fBouncing - pos.y) * 2.0f;
			m_fBouncing = pos.y;   // 値を記憶
		}

		// 向きを回転させる
		if (m_nRotPattern == 0)
		{// 時計回り
			fRot -= m_fChangeRot;
		}
		else if (m_nRotPattern == 1)
		{// 反時計回り
			fRot += m_fChangeRot;
		}
		else if (m_nRotPattern == 2)
		{// ランダム
			if (rand() % 2 == 0)
			{// 時計回り
				fRot += m_fChangeRot;
			}
			else
			{// 反時計回り
				fRot -= m_fChangeRot;
			}
		}
		if (fRot > D3DX_PI)
		{// 向きが円周率を超えた
			fRot -= D3DX_PI * 2.0f;
		}
		if (fRot > D3DX_PI)
		{// 向きが円周率を超えた
			fRot += D3DX_PI * 2.0f;
		}

		// 各種値の設定
		SetPos(pos);          // 座標
		SetCol(col);          // 色
		SetRot(fRot);         // 向き
		SetLength(fRadius);   // 大きさ
		ChangeVertex();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CParticle::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Lighting;

		    // Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			if (m_bDrawAddtive == true)
			{// αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// ライティングの設定
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			if (m_bLighting == false)
			{// ライティングを外す
				pDevice->SetRenderState(D3DRS_LIGHTING, false);
			}

			// 共通の描画処理
			CSceneBillboard::Draw();


			// ライティングを戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// Zテストを元の値に戻す
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CParticle::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// オフセットの長さを求める
	float fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));
	SetLength(fLength);

	// オフセットの角度を求める
	float fAngle = atan2f(GetWidth(), GetHeight());
	SetAngle(fAngle);

	// 頂点の回転を考慮して座標を計算
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(GetRot(), -sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	YPos[0] = CFunctionLib::RotationVectorY(GetRot(), -sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	XPos[1] = CFunctionLib::RotationVectorX(GetRot(), sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	YPos[1] = CFunctionLib::RotationVectorY(GetRot(), sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	XPos[2] = CFunctionLib::RotationVectorX(GetRot(), -sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	YPos[2] = CFunctionLib::RotationVectorY(GetRot(), -sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	XPos[3] = CFunctionLib::RotationVectorX(GetRot(), sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	YPos[3] = CFunctionLib::RotationVectorY(GetRot(), sinf(fAngle) * fLength, -cosf(fAngle) * fLength);

	// 頂点座標
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 頂点の数だけ繰り返し
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
	}

	// 法線
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[1].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[2].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[3].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファの設定
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    頂点情報を変更する処理
//=============================================================================
void CParticle::ChangeVertex(void)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == NULL) { return; }

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の回転を考慮して座標を計算
	float fAngle = GetAngle();
	float fLength = GetLength();
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(GetRot(), -sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	YPos[0] = CFunctionLib::RotationVectorY(GetRot(), -sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	XPos[1] = CFunctionLib::RotationVectorX(GetRot(), sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	YPos[1] = CFunctionLib::RotationVectorY(GetRot(), sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	XPos[2] = CFunctionLib::RotationVectorX(GetRot(), -sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	YPos[2] = CFunctionLib::RotationVectorY(GetRot(), -sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	XPos[3] = CFunctionLib::RotationVectorX(GetRot(), sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	YPos[3] = CFunctionLib::RotationVectorY(GetRot(), sinf(fAngle) * fLength, -cosf(fAngle) * fLength);

	// 頂点座標
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 頂点の数だけ繰り返し
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
	}

	// 法線
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[1].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[2].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[3].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
//    移動量設定処理
//=============================================================================
void CParticle::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    移動量の変化量設定処理
//=============================================================================
void CParticle::SetChangeMove(const D3DXVECTOR3 ChangeMove)
{
	m_ChangeMove = ChangeMove;
}

//=============================================================================
//    色の変化量設定処理
//=============================================================================
void CParticle::SetChangeCol(const D3DXCOLOR ChangeCol)
{
	m_ChangeCol = ChangeCol;
}

//=============================================================================
//    寿命設定処理
//=============================================================================
void CParticle::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    回転パターン設定処理
//=============================================================================
void CParticle::SetRotPattern(const int nRotPattern)
{
	m_nRotPattern = nRotPattern;
}

//=============================================================================
//    大きさの変化量設定処理
//=============================================================================
void CParticle::SetSpread(const float fSpread)
{
	m_fSpread = fSpread;
}

//=============================================================================
//    向きの変化量設定処理
//=============================================================================
void CParticle::SetChangeRot(const float fChangeRot)
{
	m_fChangeRot = fChangeRot;
}

//=============================================================================
//    バウンド量設定処理
//=============================================================================
void CParticle::SetBouncing(const float fBouncing)
{
	m_fBouncing = fBouncing;
}

//=============================================================================
//    当たり判定を取るか設定処理
//=============================================================================
void CParticle::SetCollision(const bool bCollision)
{
	m_bCollision = bCollision;
}

//=============================================================================
//    加算合成で描画するかしないか設定処理
//=============================================================================
void CParticle::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    移動量取得処理
//=============================================================================
D3DXVECTOR3 CParticle::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    移動量の変化量取得処理
//=============================================================================
D3DXVECTOR3 CParticle::GetChangeMove(void)
{
	return m_ChangeMove;
}

//=============================================================================
//    色の変化量取得処理
//=============================================================================
D3DXCOLOR CParticle::GetChangeCol(void)
{
	return m_ChangeCol;
}

//=============================================================================
//    寿命取得処理
//=============================================================================
int CParticle::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    回転パターン取得処理
//=============================================================================
int CParticle::GetRotPattern(void)
{
	return m_nRotPattern;
}

//=============================================================================
//    大きさの変化量取得処理
//=============================================================================
float CParticle::GetSpread(void)
{
	return m_fSpread;
}

//=============================================================================
//    向きの変化量取得処理
//=============================================================================
float CParticle::GetChangeRot(void)
{
	return m_fChangeRot;
}

//=============================================================================
//    バウンド量取得処理
//=============================================================================
float CParticle::GetBouncing(void)
{
	return m_fBouncing;
}

//=============================================================================
//    当たり判定を取るか取得処理
//=============================================================================
bool CParticle::GetCollision(void)
{
	return m_bCollision;
}

//=============================================================================
//    加算合成で描画するかしないか取得処理
//=============================================================================
bool CParticle::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}