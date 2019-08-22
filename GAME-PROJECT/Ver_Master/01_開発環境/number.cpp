//*****************************************************************************
//
//     数字の処理[number.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "number.h"
#include "functionlib.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************

//*****************************************************************************
//    CNumberPolygonの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CNumberPolygon::CNumberPolygon(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// 各種値のクリア
	m_State = STATE_NONE;  // 状態
	m_nNumber = 0;         // 表示する数字
	m_nCounter = 0;        // 描画を管理するカウンター
	m_nFlashTiming = 0;    // 点滅させるタイミング
}

//=============================================================================
//    デストラクタ
//=============================================================================
CNumberPolygon::~CNumberPolygon()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CNumberPolygon *CNumberPolygon::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, STATE State,
	int nNumber, int nFlashTiming, int nPriority)
{
	CNumberPolygon *pNumberPolygon = NULL;  // 数字ポリゴンクラス型のポインタ
	if (pNumberPolygon == NULL)
	{// メモリが空になっている
		pNumberPolygon = new CNumberPolygon(nPriority);
		if (pNumberPolygon != NULL)
		{// インスタンスを生成できた
			if (FAILED(pNumberPolygon->Init(pos, col, fWidth, fHeight, fRot, State, nNumber, nFlashTiming)))
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

	return pNumberPolygon;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CNumberPolygon::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, STATE State,
	int nNumber, int nFlashTiming)
{
	// 各種値の設定
	SetPos(pos);                    // ポリゴンの座標
	SetRot(fRot);                   // ポリゴンの向き
	SetCol(col);                    // ポリゴンの色
	SetWidth(fWidth);               // ポリゴンの幅
	SetHeight(fHeight);             // ポリゴンの高さ
	m_nNumber = nNumber;            // 数字
	m_State = State;                // 状態
	m_nFlashTiming = nFlashTiming;  // 点滅させるタイミング

	if (FAILED(CScene2D::Init()))
	{// 初期化処理に失敗した
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CNumberPolygon::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CNumberPolygon::Update(void)
{
	if (m_State == STATE_FLASH)
	{// 点滅させる状態だったら
		Flash();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CNumberPolygon::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}

//=============================================================================
//    点滅させる状態の時の更新処理
//=============================================================================
void CNumberPolygon::Flash(void)
{
	m_nCounter++;   // カウンターを進める
	if (m_nCounter / m_nFlashTiming == 0)
	{
		SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (m_nCounter >= m_nFlashTiming * 2)
	{// カウンターが既定の値に達した
		m_nCounter = 0;
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CNumberPolygon::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// オフセットの長さを求める
	float fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));
	SetLength(fLength);

	// オフセットの角度を求める
	float fAngle = atan2f(GetWidth(), -GetHeight());
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
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + GetPos();
	}

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nNumber * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (m_nNumber * 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nNumber * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (m_nNumber * 0.1f), 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファの設定
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    表示する数字の設定処理
//=============================================================================
void CNumberPolygon::SetNumber(const int nNumber)
{
	// 数字の設定
	m_nNumber = nNumber;

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファへのポインタが取得できた
	 // 頂点情報を設定
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 1.0f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    状態の設定処理
//=============================================================================
void CNumberPolygon::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
//    表示する数字の取得処理
//=============================================================================
int CNumberPolygon::GetNumber(void)
{
	return m_nNumber;
}

//=============================================================================
//    状態の取得処理
//=============================================================================
CNumberPolygon::STATE CNumberPolygon::GetState(void)
{
	return m_State;
}

//*****************************************************************************
//    CNumberの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CNumber::CNumber(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_nNumber = 0;                                 // 現在表示している数字
	m_nNumberDigit = 0;                            // 表示している数字の桁数
	m_pNumberTexture = NULL;                       // 数字ポリゴンに使用しているテクスチャへのポインタ
	m_pNumberPolygon = NULL;                       // 数字ポリゴンクラスへのポインタ
	m_NumberPos = INITIALIZE_D3DXVECTOR3;          // 数字ポリゴンの位置
	m_NumberCol = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);  // 数字ポリゴンの色
	m_fNumberWidth = 0.0f;                         // 数字ポリゴンの幅
	m_fNumberHeight = 0.0f;                        // 数字ポリゴンの高さ
	m_NumberInterval = INITIALIZE_D3DXVECTOR3;     // 数字ポリゴン同士の間隔
	m_fNumberRot = 0.0f;                           // 数字ポリゴンの向き
	m_NumberState = CNumberPolygon::STATE_NONE;    // 数字ポリゴンの状態
	m_nNumberFlashTiming = 0;                      // 数字ポリゴンを点滅させるのに使う
}

//=============================================================================
//    デストラクタ
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight,
	D3DXVECTOR3 Interval, LPDIRECT3DTEXTURE9 pNumberTex, int nNumber,
	CNumberPolygon::STATE State, int nFlashTiming, float fRot, int nPriority)
{
	CNumber *pNumber = NULL;     // 数字クラス型のポインタ
	if (pNumber == NULL)
	{// メモリが空になっている
		pNumber = new CNumber(nPriority);
		if (pNumber != NULL)
		{// インスタンスを生成できた
			// 各種値の設定
			pNumber->SetNumberPos(pos);
			pNumber->SetNumberCol(col);
			pNumber->SetNumberWidth(fWidth);
			pNumber->SetNumberHeight(fHeight);
			pNumber->SetNumberInterval(Interval);
			pNumber->SetNumberTexture(pNumberTex);
			pNumber->SetNumberRot(fRot);
			pNumber->SetNumberState(State);
			pNumber->SetNumberFlashTiming(nFlashTiming);
			pNumber->SetNumberValue(nNumber);

			if (FAILED(pNumber->Init()))
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

	return pNumber;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CNumber::Init(void)
{
	// 表示する数字の桁数を計算
	CalcNumberDigit();

	// 数字ポリゴンの生成
	CreateNumberPolygon();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	// 数字ポリゴンを開放する
	ReleaseNumberPolygon(m_nNumberDigit);

	// 死亡フラグを立てる
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CNumber::Draw(void)
{

}

//=============================================================================
//    表示する数字を設定する処理
//=============================================================================
void CNumber::SetNumber(const int nNumber)
{
	// 数字を設定
	m_nNumber = nNumber;

	// 桁数を保存
	int nReleaseDigit = m_nNumberDigit;

	// 数字の桁数を計算
	CalcNumberDigit();

	if (nReleaseDigit != m_nNumberDigit)
	{// 現在表示されている数字の桁数と異なる
		ReleaseNumberPolygon(nReleaseDigit);
		CreateNumberPolygon();
	}

	// 表示する数字を変える
	if (m_pNumberPolygon == NULL) { return; }
	int nDispNumber = 0;
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumberPolygon[nCntDigit] != NULL)
		{
			// 表示する数字の計算
			nDispNumber = m_nNumber % ((int)powf(10.0f, (float)nCntDigit) * 10) / (int)powf(10.0f, (float)nCntDigit);

			// 表示する数字の設定
			m_pNumberPolygon[nCntDigit]->SetNumber(nDispNumber);
		}
	}
}

//=============================================================================
//    表示する数字ポリゴンの色を設定する処理
//=============================================================================
void CNumber::SetNumberVtxBuffCol()
{
	if (m_pNumberPolygon == NULL) { return; }

	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		if (m_pNumberPolygon[nCntDigit] != NULL)
		{
			m_pNumberPolygon[nCntDigit]->SetCol(m_NumberCol);
			m_pNumberPolygon[nCntDigit]->SetVtxBuffCol();
		}
	}
}

//=============================================================================
//    表示する数字の桁数計算処理
//=============================================================================
void CNumber::CalcNumberDigit(void)
{
	m_nNumberDigit = (int)log10f((float)m_nNumber) + 1;	// 桁数を求める
	if (m_nNumberDigit <= 0) { m_nNumberDigit = 1; }	// 0以下のとき1にする
}

//=============================================================================
//    数字ポリゴンの生成処理
//=============================================================================
void CNumber::CreateNumberPolygon(void)
{
	// 必要な桁数分メモリを確保
	m_pNumberPolygon = new CNumberPolygon*[m_nNumberDigit];
	if (m_pNumberPolygon == NULL) { return; }

	// 数字ポリゴンを生成する
	D3DXVECTOR3 NumberPos = m_NumberPos; // 座標
	D3DXCOLOR NumberCol = m_NumberCol;   // 色
	int nDispNumber = 0;                 // 表示する数字の計算用
	for (int nCntDigit = 0; nCntDigit < m_nNumberDigit; nCntDigit++)
	{
		// 表示する数字の計算
		nDispNumber = m_nNumber % ((int)powf(10.0f, (float)nCntDigit) * 10) / (int)powf(10.0f, (float)nCntDigit);

		// 数字ポリゴンのインスタンス生成処理
		m_pNumberPolygon[nCntDigit] = NULL;
		m_pNumberPolygon[nCntDigit] = CNumberPolygon::Create(NumberPos, NumberCol, m_fNumberWidth, m_fNumberHeight, 0.0f, CNumberPolygon::STATE_NONE, nDispNumber, 3, GetPriority());
		if (m_pNumberPolygon[nCntDigit] != NULL)
		{
			m_pNumberPolygon[nCntDigit]->BindTexture(m_pNumberTexture);
		}

		// 横に進める
		NumberPos.x += m_NumberInterval.x;
		NumberPos.y += m_NumberInterval.y;
	}
}

//=============================================================================
//    数字ポリゴンの開放処理
//=============================================================================
void CNumber::ReleaseNumberPolygon(int nDigit)
{
	if (m_pNumberPolygon == NULL) { return; }

	for (int nCntDigit = 0; nCntDigit < nDigit; nCntDigit++)
	{
		if (m_pNumberPolygon[nCntDigit] != NULL)
		{
			m_pNumberPolygon[nCntDigit]->Uninit();
			m_pNumberPolygon[nCntDigit] = NULL;
		}
	}

	delete[] m_pNumberPolygon;
	m_pNumberPolygon = NULL;

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
//    表示する数字を設定する
//=============================================================================
void CNumber::SetNumberValue(const int nNumber)
{
	m_nNumber = nNumber;
}

//=============================================================================
//    数字ポリゴンに使用するテクスチャへのポインタ設定処理
//=============================================================================
void CNumber::SetNumberTexture(const LPDIRECT3DTEXTURE9 pNumberTex)
{
	m_pNumberTexture = pNumberTex;
}

//=============================================================================
//    数字ポリゴンの向き設定処理
//=============================================================================
void CNumber::SetNumberPos(const D3DXVECTOR3 NumberPos)
{
	m_NumberPos = NumberPos;
}

//=============================================================================
//    数字ポリゴンの向き設定処理
//=============================================================================
void CNumber::SetNumberCol(const D3DXCOLOR NumberCol)
{
	m_NumberCol = NumberCol;
}

//=============================================================================
//    数字ポリゴンの向き設定処理
//=============================================================================
void CNumber::SetNumberWidth(const float fNumberWidth)
{
	m_fNumberWidth = fNumberWidth;
}

//=============================================================================
//    数字ポリゴンの向き設定処理
//=============================================================================
void CNumber::SetNumberHeight(const float fNumberHeight)
{
	m_fNumberHeight = fNumberHeight;
}

//=============================================================================
//    数字ポリゴン同士の間隔設定処理
//=============================================================================
void CNumber::SetNumberInterval(const D3DXVECTOR3 NumberInterval)
{
	m_NumberInterval = NumberInterval;
}

//=============================================================================
//    数字ポリゴンの向き設定処理
//=============================================================================
void CNumber::SetNumberRot(const float fNumberRot)
{
	m_fNumberRot = fNumberRot;
}

//=============================================================================
//    数字ポリゴンの状態設定処理
//=============================================================================
void CNumber::SetNumberState(const CNumberPolygon::STATE state)
{
	m_NumberState = state;
}

//=============================================================================
//    数字ポリゴンの点滅値設定処理
//=============================================================================
void CNumber::SetNumberFlashTiming(const int nNumberFlashTiming)
{
	m_nNumberFlashTiming = nNumberFlashTiming;
}

//=============================================================================
//    現在表示している数字の取得処理
//=============================================================================
int CNumber::GetNumber(void)
{
	return m_nNumber;
}

//=============================================================================
//    数字の桁数取得処理
//=============================================================================
int CNumber::GetNumberDigit(void)
{
	return m_nNumberDigit;
}

//=============================================================================
//    数字ポリゴンクラスへのポインタ取得処理
//=============================================================================
CNumberPolygon **CNumber::GetNumberPolygon(void)
{
	return m_pNumberPolygon;
}

//=============================================================================
//    数字ポリゴンに使用しているテクスチャへのポインタ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CNumber::GetNumberTexture(void)
{
	return m_pNumberTexture;
}

//=============================================================================
//    数字ポリゴンの位置取得処理
//=============================================================================
D3DXVECTOR3 CNumber::GetNumberPos(void)
{
	return m_NumberPos;
}

//=============================================================================
//    数字ポリゴンの色取得処理
//=============================================================================
D3DXCOLOR CNumber::GetNumberCol(void)
{
	return m_NumberCol;
}

//=============================================================================
//    数字ポリゴンの幅取得処理
//=============================================================================
float CNumber::GetNumberWidth(void)
{
	return m_fNumberWidth;
}

//=============================================================================
//    数字ポリゴンの高さ取得処理
//=============================================================================
float CNumber::GetNumberHeight(void)
{
	return m_fNumberHeight;
}

//=============================================================================
//    数字ポリゴン同士の間隔取得処理
//=============================================================================
D3DXVECTOR3 CNumber::GetNumberInterval(void)
{
	return m_NumberInterval;
}

//=============================================================================
//    数字ポリゴンの向き取得処理
//=============================================================================
float CNumber::GetNumberRot(void)
{
	return m_fNumberRot;
}

//=============================================================================
//    数字ポリゴンの状態取得処理
//=============================================================================
CNumberPolygon::STATE CNumber::GetNumberState(void)
{
	return m_NumberState;
}

//=============================================================================
//    数字ポリゴンの点滅値取得処理
//=============================================================================
int CNumber::GetNumberFlashTiming(void)
{
	return m_nNumberFlashTiming;
}