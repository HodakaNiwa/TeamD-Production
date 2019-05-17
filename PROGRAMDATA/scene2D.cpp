//*****************************************************************************
//
//     2Dポリゴンの処理[scene2D.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "2DPolygon.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//*****************************************************************************
//    CScene2Dの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTexture = NULL;                         // テクスチャへのポインタ
	m_pVtxBuff = NULL;                         // 頂点バッファへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ポリゴンの座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // ポリゴンの色
	m_fRot = 0.0f;                             // ポリゴンの向き
	m_fWidth = 0.0f;                           // ポリゴンの幅
	m_fHeight = 0.0f;                          // ポリゴン高さ
	m_fAngle = 0.0f;                           // ポリゴンを出す角度
	m_fLength = 0.0f;                          // ポリゴンの長さ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, int nPriority)
{
	CScene2D *pScene2D = NULL;             // 2Dポリゴンクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pScene2D == NULL)
		{// メモリが空になっている
			pScene2D = new CScene2D(nPriority);
			if (pScene2D != NULL)
			{// インスタンスを生成できた
			 // 各種値の設定
				pScene2D->SetPos(pos);         // ポリゴンの座標
				pScene2D->SetRot(fRot);        // ポリゴンの向き
				pScene2D->SetCol(col);         // ポリゴンの色
				pScene2D->SetWidth(fWidth);    // ポリゴンの幅
				pScene2D->SetHeight(fHeight);  // ポリゴンの高さ

				if (FAILED(pScene2D->Init()))
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
	}
	else
	{// これ以上シーンを作成できない
		return NULL;
	}

	return pScene2D;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CScene2D::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		 // 頂点バッファの生成
			MakeVertex(pDevice);
		}
		else
		{// デバイスを取得できなかった
			return E_FAIL;
		}
	}
	else
	{// レンダリングクラスが生成されていない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CScene2D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// オフセットの長さを求める
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	// オフセットの角度を求める
	m_fAngle = atan2f(m_fWidth, -m_fHeight);

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
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
	}

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CScene2D::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    ポリゴンの位置設定処理
//=============================================================================
void CScene2D::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
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
			pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの色設定処理
//=============================================================================
void CScene2D::SetCol(const D3DXCOLOR col)
{
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	 // 頂点情報
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラー
		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの向き設定処理
//=============================================================================
void CScene2D::SetRot(const float fRot)
{
	m_fRot = fRot;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
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
			pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの幅設定処理
//=============================================================================
void CScene2D::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	 // 頂点情報
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// オフセットの長さを求める
		m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

		// オフセットの角度を求める
		m_fAngle = atan2f(m_fWidth, -m_fHeight);

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
			pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの高さ設定処理
//=============================================================================
void CScene2D::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	 // 頂点情報
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// オフセットの長さを求める
		m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

		// オフセットの角度を求める
		m_fAngle = atan2f(m_fWidth, -m_fHeight);

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
			pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンを出す角度設定処理
//=============================================================================
void CScene2D::SetAngle(const float fAngle)
{
	// 角度を設定
	m_fAngle = fAngle;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
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
			pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの長さ設定処理
//=============================================================================
void CScene2D::SetLength(const float fLength)
{
	// 長さを設定
	m_fLength = fLength;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
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
			pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + m_Pos;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    ポリゴンの位置取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ポリゴンの色取得処理
//=============================================================================
D3DXCOLOR CScene2D::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    ポリゴンの向き取得処理
//=============================================================================
float CScene2D::GetRot(void)
{
	return m_fRot;
}

//=============================================================================
//    ポリゴンの幅取得処理
//=============================================================================
float CScene2D::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ポリゴンの高さ取得処理
//=============================================================================
float CScene2D::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ポリゴンを出す角度取得処理
//=============================================================================
float CScene2D::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
//    ポリゴンの長さ取得処理
//=============================================================================
float CScene2D::GetLength(void)
{
	return m_fLength;
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
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
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
	}
	else
	{// これ以上シーンを作成できない
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
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
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
	}
	else
	{// これ以上シーンを作成できない
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

//*****************************************************************************
//    CNumberの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CNumber::CNumber(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
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
CNumber::~CNumber()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, STATE State,
	int nNumber, int nFlashTiming, int nPriority)
{
	CNumber *pNumber = NULL;               // 数字クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pNumber == NULL)
		{// メモリが空になっている
			pNumber = new CNumber(nPriority);
			if (pNumber != NULL)
			{// インスタンスを生成できた
				if (FAILED(pNumber->Init(pos, col, fWidth, fHeight, fRot, State, nNumber, nFlashTiming)))
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
	}
	else
	{// これ以上シーンを作成できない
		return NULL;
	}

	return pNumber;  // インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, STATE State,
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
void CNumber::Uninit(void)
{
	// 共通の終了処理
	CScene2D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CNumber::Update(void)
{
	if (m_State == STATE_FLASH)
	{// 点滅させる状態だったら
		Flash();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CNumber::Draw(void)
{
	// 共通の描画処理
	CScene2D::Draw();
}

//=============================================================================
//    点滅させる状態の時の更新処理
//=============================================================================
void CNumber::Flash(void)
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
void CNumber::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
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
void CNumber::SetNumber(const int nNumber)
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
void CNumber::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
//    表示する数字の取得処理
//=============================================================================
int CNumber::GetNumber(void)
{
	return m_nNumber;
}

//=============================================================================
//    状態の取得処理
//=============================================================================
CNumber::STATE CNumber::GetState(void)
{
	return m_State;
}