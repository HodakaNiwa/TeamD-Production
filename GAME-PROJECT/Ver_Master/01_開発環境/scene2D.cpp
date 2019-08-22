//=============================================================================
//
// オブジェクト2D処理 [scene2D.cpp]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "functionlib.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================


//*****************************************************************************
//    CScene2Dの処理
//*****************************************************************************
//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority,OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pTexture = NULL;					        // テクスチャへのポインタ
	m_pVtxBuff = NULL;					        // 頂点バッファへのポインタ
	m_pos = INITIALIZE_D3DXVECTOR3;		        // ポリゴンの位置
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// ポリゴンの色
	m_fRot = 0.0f;							    // ポリゴンの向き
	m_fWidth = 0.0f;					        // ポリゴンの幅
	m_fHeight = 0.0f;					        // ポリゴンの高さ
	m_fAngle = 0.0f;					        // ベクトルの角度
	m_fLength = 0.0f;					        // ベクトルの長さ
	m_fStartTexU = 0.0f;                        // テクスチャU座標(始点)
	m_fStartTexV = 0.0f;                        // テクスチャV座標(始点)
	m_fAddTexU = 0.0f;                          // テクスチャU座標に加える値
	m_fAddTexV = 0.0f;                          // テクスチャV座標に加える値

}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
//オブジェクトの生成
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CScene2D *pScene2D = NULL;	// CScene2Dのポインタ

	if (pScene2D == NULL)
	{
		pScene2D = new CScene2D(nPriority);
		if (pScene2D != NULL)
		{
			// 値の設定
			pScene2D->SetPos(pos);
			pScene2D->SetCol(col);
			pScene2D->SetWidth(fWidth);
			pScene2D->SetHeight(fHeight);
			pScene2D->SetRot(fRot);
			pScene2D->SetStartTexU(fStartTexU);
			pScene2D->SetStartTexV(fStartTexV);
			pScene2D->SetAddTexU(fAddTexU);
			pScene2D->SetAddTexV(fAddTexV);

			if (FAILED(pScene2D->Init()))
			{// 初期化に失敗
				return NULL;
			}
		}
	}
	return pScene2D;
}

//=============================================================================
// オブジェクトの初期化処理
//=============================================================================
HRESULT CScene2D::Init(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{
			// 頂点バッファの生成
			MakeVertex(pDevice);
		}
	}

	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 頂点バッファの生成処理
//=============================================================================
void CScene2D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ポリゴンの大きさ設定
	SetScale(m_fWidth, m_fHeight);

	// 頂点バッファへ値を設定する処理
	SetVtxBuffValue();
}

//=============================================================================
// ポリゴンの大きさ設定処理
//=============================================================================
void CScene2D::SetScale(const float fWidth, const float fHeight)
{
	// 幅、高さの値設定
	SetWidth(fWidth);
	SetHeight(fHeight);

	// ベクトルの長さと角度を計算
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));
	m_fAngle = atan2f(m_fWidth, -m_fHeight);
}

//=============================================================================
// 頂点バッファへ値を設定する処理
//=============================================================================
void CScene2D::SetVtxBuffValue(void)
{
	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の回転を考慮して座標を計算
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	// 頂点座標
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 頂点の数だけ繰り返し
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_pos;
	}

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(m_fStartTexU, m_fStartTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fStartTexU + m_fAddTexU, m_fStartTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fStartTexU, m_fStartTexV + m_fAddTexV);
	pVtx[3].tex = D3DXVECTOR2(m_fStartTexU + m_fAddTexU, m_fStartTexV + m_fAddTexV);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点バッファへ座標を設定する処理
//=============================================================================
void CScene2D::SetVtxBuffPos(void)
{
	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の回転を考慮して座標を計算
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	// 頂点座標
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 頂点の数だけ繰り返し
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_pos;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点バッファへ色を設定する処理
//=============================================================================
void CScene2D::SetVtxBuffCol(void)
{
	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点カラー
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点バッファへテクスチャ座標を設定する処理
//=============================================================================
void CScene2D::SetVtxBuffTex(void)
{
	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(m_fStartTexU, m_fStartTexV);
	pVtx[1].tex = D3DXVECTOR2(m_fStartTexU + m_fAddTexU, m_fStartTexV);
	pVtx[2].tex = D3DXVECTOR2(m_fStartTexU, m_fStartTexV + m_fAddTexV);
	pVtx[3].tex = D3DXVECTOR2(m_fStartTexU + m_fAddTexU, m_fStartTexV + m_fAddTexV);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャの割り当て処理
//=============================================================================
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
// 頂点バッファの設定処理
//=============================================================================
void CScene2D::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
// ポリゴンの位置設定処理
//=============================================================================
void CScene2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// ポリゴンの色設定処理
//=============================================================================
void CScene2D::SetCol(const D3DXCOLOR col)
{
	m_col = col;
}

//=============================================================================
// ポリゴンの幅設定処理
//=============================================================================
void CScene2D::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
// ポリゴンの高さ設定処理
//=============================================================================
void CScene2D::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
// ポリゴンの向き設定処理
//=============================================================================
void CScene2D::SetRot(const float fRot)
{
	m_fRot = fRot;
}

//=============================================================================
// ベクトルの角度設定処理
//=============================================================================
void CScene2D::SetAngle(const float fAngle)
{
	m_fAngle = fAngle;
}

//=============================================================================
// ベクトルの長さ設定処理
//=============================================================================
void CScene2D::SetLength(const float fLength)
{
	m_fLength = fLength;
}

//=============================================================================
// テクスチャU座標(始点)設定処理
//=============================================================================
void CScene2D::SetStartTexU(const float fStartTexU)
{
	m_fStartTexU = fStartTexU;
}

//=============================================================================
// テクスチャV座標(始点)設定処理
//=============================================================================
void CScene2D::SetStartTexV(const float fStartTexV)
{
	m_fStartTexV = fStartTexV;
}

//=============================================================================
// テクスチャU座標に加える値設定処理
//=============================================================================
void CScene2D::SetAddTexU(const float fAddTexU)
{
	m_fAddTexU = fAddTexU;
}

//=============================================================================
// テクスチャV座標に加える値設定処理
//=============================================================================
void CScene2D::SetAddTexV(const float fAddTexV)
{
	m_fAddTexV = fAddTexV;
}

//=============================================================================
// テクスチャへのポインタ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CScene2D::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
// 頂点バッファへのポインタ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
// ポリゴンの位置取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// ポリゴンの色取得処理
//=============================================================================
D3DXCOLOR CScene2D::GetCol(void)
{
	return m_col;
}

//=============================================================================
// ポリゴンの幅取得処理
//=============================================================================
float CScene2D::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
// ポリゴンの高さ取得処理
//=============================================================================
float CScene2D::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
// ポリゴンの向き取得処理
//=============================================================================
float CScene2D::GetRot(void)
{
	return m_fRot;
}

//=============================================================================
// ベクトルの角度取得処理
//=============================================================================
float CScene2D::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
// ベクトルの長さ取得処理
//=============================================================================
float CScene2D::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
// テクスチャU座標(始点)取得処理
//=============================================================================
float CScene2D::GetStartTexU(void)
{
	return m_fStartTexU;
}

//=============================================================================
// テクスチャV座標(始点)取得処理
//=============================================================================
float CScene2D::GetStartTexV(void)
{
	return m_fStartTexV;
}

//=============================================================================
// テクスチャU座標に加える値取得処理
//=============================================================================
float CScene2D::GetAddTexU(void)
{
	return m_fAddTexU;
}

//=============================================================================
// テクスチャV座標に加える値取得処理
//=============================================================================
float CScene2D::GetAddTexV(void)
{
	return m_fAddTexV;
}

//*****************************************************************************
//    CScene2DFlashの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CScene2DFlash::CScene2DFlash(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// 各種値のクリア
	m_State = STATE_NONE;                           // 状態
	m_ColMax = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // 色の最大値
	m_ColMin = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // 色の最小値
	m_fColChange = 0.0f;                            // 色の変化量
	m_nPushTiming = 0;                              // 描画を切り替えるタイミング(押された状態の時)
	m_nCounter = 0;                                 // 描画を管理するカウンター
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScene2DFlash::~CScene2DFlash()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CScene2DFlash *CScene2DFlash::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot,
	STATE State, D3DXCOLOR ColMax, D3DXCOLOR ColMin, float fColChange, int nPushTiming, int nPriority)
{
	CScene2DFlash *pScene2DFlash = NULL;   // 2Dポリゴン点滅クラス型のポインタ
	if (pScene2DFlash == NULL)
	{// メモリが空になっている
		pScene2DFlash = new CScene2DFlash(nPriority);
		if (pScene2DFlash != NULL)
		{// インスタンスを生成できた
			if (FAILED(pScene2DFlash->Init(pos, col, fWidth, fHeight, fRot, State, ColMax, ColMin, fColChange, nPushTiming)))
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

	return pScene2DFlash;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CScene2DFlash::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot,
	STATE State, D3DXCOLOR ColMax, D3DXCOLOR ColMin, float fColChange, int nPushTiming)
{
	// 各種値の設定
	SetPos(pos);                  // ポリゴンの座標
	SetRot(fRot);                 // ポリゴンの向き
	SetCol(col);                  // ポリゴンの色
	SetWidth(fWidth);             // ポリゴンの幅
	SetHeight(fHeight);           // ポリゴンの高さ
	m_State = State;              // 状態
	m_ColMax = ColMax;            // 色の最大値
	m_ColMin = ColMin;            // 色の最小値
	m_fColChange = fColChange;    // 色の変化量
	m_nPushTiming = nPushTiming;  // 描画を切り替えるタイミング(押された状態の時)
	SetStartTexU(1.0f);           // 左上のテクスチャU座標
	SetStartTexV(1.0f);           // 左上のテクスチャV座標
	SetAddTexU(1.0f);             // テクスチャU座標に加える値
	SetAddTexV(1.0f);             // テクスチャV座標に加える値

	if (FAILED(CScene2D::Init()))
	{// 初期化処理に失敗した
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CScene2DFlash::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CScene2DFlash::Update(void)
{
	switch (m_State)
	{// 状態によって処理わけ
	case STATE_NONE:   // 通常状態
		break;
	case STATE_SELECT: // 選択された状態
		Select();
		break;
	case STATE_PUSH:   // 押された状態
		m_nCounter++;  // カウンターを増やす
		break;
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CScene2DFlash::Draw(void)
{
	if (m_State != STATE_PUSH)
	{// 押された状態でない
	 // 共通の描画処理
		CScene2D::Draw();
	}
	else
	{// 押された状態である
		if (m_nCounter % m_nPushTiming == 0)
		{// カウンターが既定の値になった
		 // 共通の描画処理
			CScene2D::Draw();
		}
	}
}

//=============================================================================
//    選択されているときの処理
//=============================================================================
void CScene2DFlash::Select(void)
{
	D3DXCOLOR col = GetCol();  // 色を取得

	// 色を変化させる
	col.r += m_fColChange;  // 赤色
	col.g += m_fColChange;  // 緑色
	col.b += m_fColChange;  // 青色

	if (col.r < 0.3f || col.r > 1.0f
		|| col.g < 0.3f || col.g > 1.0f
		|| col.b < 0.3f || col.b > 1.0f)
	{// 色が既定の値を超えた
		m_fColChange *= -1;  // 色の変化を逆転させる
	}

	// 色の設定
	SetCol(col);
	SetVtxBuffCol();
}

//=============================================================================
//    状態を設定する処理
//=============================================================================
void CScene2DFlash::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    色の最大値を設定する処理
//=============================================================================
void CScene2DFlash::SetColMax(const D3DXCOLOR ColMax)
{
	m_ColMax = ColMax;
}

//=============================================================================
//    色の最小値を設定する処理
//=============================================================================
void CScene2DFlash::SetColMin(const D3DXCOLOR ColMin)
{
	m_ColMin = ColMin;
}

//=============================================================================
//    色の変化量を設定する処理
//=============================================================================
void CScene2DFlash::SetColChange(const float fColChange)
{
	m_fColChange = fColChange;
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CScene2DFlash::STATE CScene2DFlash::GetState(void)
{
	return m_State;
}

//=============================================================================
//    色の最大値を取得する処理
//=============================================================================
D3DXCOLOR CScene2DFlash::GetColMax(void)
{
	return m_ColMax;
}

//=============================================================================
//    色の最小値を取得する処理
//=============================================================================
D3DXCOLOR CScene2DFlash::GetColMin(void)
{
	return m_ColMin;
}

//=============================================================================
//    色の変化量を取得する処理
//=============================================================================
float CScene2DFlash::GetColChange(void)
{
	return m_fColChange;
}


//*****************************************************************************
//    CScene2DPressの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CScene2DPress::CScene2DPress(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// 各種値のクリア
	m_State = STATE_NONE;   // 状態
	m_nStopTiming = 0;      // 描画を一時停止するタイミング
	m_nAppearTiming = 0;    // 描画を再開するタイミング
	m_nCounter = 0;         // 描画を管理するカウンター
	m_nPushTiming = 0;      // 描画を切り替えるタイミング(押された状態の時)
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScene2DPress::~CScene2DPress()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CScene2DPress *CScene2DPress::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot,
	int nStopTiming, int nAppearTiming, int nPushTiming, int nPriority)
{
	CScene2DPress *pScene2DPress = NULL;   // 2Dポリゴンプレスクラス型のポインタ
	if (pScene2DPress == NULL)
	{// メモリが空になっている
		pScene2DPress = new CScene2DPress(nPriority);
		if (pScene2DPress != NULL)
		{// インスタンスを生成できた
			if (FAILED(pScene2DPress->Init(pos, col, fWidth, fHeight, fRot, nStopTiming, nAppearTiming, nPushTiming)))
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

	return pScene2DPress;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CScene2DPress::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot,
	int nStopTiming, int nAppearTiming, int nPushTiming)
{
	// 各種値の設定
	SetPos(pos);                      // ポリゴンの座標
	SetRot(fRot);                     // ポリゴンの向き
	SetCol(col);                      // ポリゴンの色
	SetWidth(fWidth);                 // ポリゴンの幅
	SetHeight(fHeight);               // ポリゴンの高さ
	m_nStopTiming = nStopTiming;      // 描画を一時停止するタイミング
	m_nAppearTiming = nAppearTiming;  // 描画を再開するタイミング
	m_nPushTiming = nPushTiming;      // 描画を切り替えるタイミング(押された状態の時)
	SetStartTexU(1.0f);               // 左上のテクスチャU座標
	SetStartTexV(1.0f);               // 左上のテクスチャV座標
	SetAddTexU(1.0f);                 // テクスチャU座標に加える値
	SetAddTexV(1.0f);                 // テクスチャV座標に加える値

	if (FAILED(CScene2D::Init()))
	{// 初期化処理に失敗した
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CScene2DPress::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CScene2DPress::Update(void)
{
	m_nCounter++;  // カウンターを増やす
	switch (m_State)
	{// 状態によって処理わけ
	case STATE_NONE:   // 通常状態
		None();
		break;
	case STATE_PUSH:   // 押された状態
		break;
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CScene2DPress::Draw(void)
{
	if (m_State != STATE_PUSH)
	{// 押された状態でない
		if (m_nCounter / m_nStopTiming == 0)
		{// カウンターが既定の値より小さい
		    // 共通の描画処理
			CScene2D::Draw();
		}
	}
	else
	{// 押された状態である
		if (m_nCounter % m_nPushTiming == 0)
		{// カウンターが既定の値になった
		 // 共通の描画処理
			CScene2D::Draw();
		}
	}
}

//=============================================================================
//    通常状態の更新処理
//=============================================================================
void CScene2DPress::FlashStart(void)
{
	m_State = STATE_PUSH;  // 押された状態に
	m_nCounter = 0;        // カウンターを戻しておく
}

//=============================================================================
//    通常状態の更新処理
//=============================================================================
void CScene2DPress::None(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();   // キーボードの取得

	if (m_nCounter >= m_nAppearTiming)
	{// カウンターが既定の値に達した
		m_nCounter = 0;   // カウンターを戻す
	}
}

//=============================================================================
//    状態を設定する処理
//=============================================================================
void CScene2DPress::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    描画を再開するタイミングを設定する処理
//=============================================================================
void CScene2DPress::SetAppearTiming(const int nAppearTiming)
{
	m_nAppearTiming = nAppearTiming;
}

//=============================================================================
//    描画を一時停止するタイミングを設定する処理
//=============================================================================
void CScene2DPress::SetStopTiming(const int nStopTiming)
{
	m_nStopTiming = nStopTiming;
}

//=============================================================================
//    描画を管理するカウンターを設定する処理
//=============================================================================
void CScene2DPress::SetCounter(const int nCounter)
{
	m_nCounter = nCounter;
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CScene2DPress::STATE CScene2DPress::GetState(void)
{
	return m_State;
}

//=============================================================================
//    描画を再開するタイミングを取得する処理
//=============================================================================
int CScene2DPress::GetAppearTiming(void)
{
	return m_nAppearTiming;
}

//=============================================================================
//    描画を一時停止するタイミングを取得する処理
//=============================================================================
int CScene2DPress::GetStopTiming(void)
{
	return m_nStopTiming;
}

//=============================================================================
//    描画を管理するカウンターを取得する処理
//=============================================================================
int CScene2DPress::GetCounter(void)
{
	return m_nCounter;
}