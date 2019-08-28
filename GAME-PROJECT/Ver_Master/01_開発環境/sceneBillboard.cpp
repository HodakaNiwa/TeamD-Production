//*****************************************************************************
//
//     ビルボードの処理[sceneBillboard.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sceneBillboard.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debugproc.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CSceneBillboard::CSceneBillboard(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pTexture = NULL;                         // テクスチャへのポインタ
	m_pVtxBuff = NULL;                         // 頂点バッファへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ポリゴンの座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // ポリゴンの色
	m_fRot = 0.0f;                             // ポリゴンの向き
	m_fWidth = 0.0f;                           // ポリゴンの幅
	m_fHeight = 0.0f;                          // ポリゴンの高さ
	m_fAngle = 0.0f;                           // ポリゴンを出す角度
	m_fLength = 0.0f;                          // ポリゴンの長さ
	D3DXMatrixIdentity(&m_MtxWorld);           // ワールドマトリックス
	m_bLighting = false;                       // ライティングするかしないか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSceneBillboard::~CSceneBillboard()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSceneBillboard *CSceneBillboard::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, bool bLighting, int nPriority)
{
	CSceneBillboard *pSceneBillboard = NULL;    // ビルボードクラス型のポインタ
	if (pSceneBillboard == NULL)
	{// メモリが空になっている
		pSceneBillboard = new CSceneBillboard(nPriority);
		if (pSceneBillboard != NULL)
		{// インスタンスを生成できた
		    // 各種値の代入
			pSceneBillboard->SetPos(pos);             // ポリゴンの座標
			pSceneBillboard->SetCol(col);             // ポリゴンの色
			pSceneBillboard->SetRot(fRot);            // ポリゴンの向き
			pSceneBillboard->SetWidth(fWidth);        // ポリゴンの幅
			pSceneBillboard->SetHeight(fHeight);      // ポリゴンの高さ
			pSceneBillboard->SetLighting(bLighting);  // ライティングするかしないか

			if (FAILED(pSceneBillboard->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pSceneBillboard;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSceneBillboard::Init(void)
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
void CSceneBillboard::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSceneBillboard::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CSceneBillboard::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Lighting;

		    // ライティングの設定
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			if (m_bLighting == false)
			{// ライティングを外す
				pDevice->SetRenderState(D3DRS_LIGHTING, false);
			}

		    // ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			if (m_pTexture != NULL)
			{
				pDevice->SetTexture(0, m_pTexture);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			// ライティングを戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CSceneBillboard::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxView, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// カメラの向きを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ワールドマトリックスに向きを設定(カメラの逆行列を入れることでカメラの正面に向きを補正)
	m_MtxWorld._11 = mtxView._11;
	m_MtxWorld._12 = mtxView._21;
	m_MtxWorld._13 = mtxView._31;
	m_MtxWorld._21 = mtxView._12;
	m_MtxWorld._22 = mtxView._22;
	m_MtxWorld._23 = mtxView._32;
	m_MtxWorld._31 = mtxView._13;
	m_MtxWorld._32 = mtxView._23;
	m_MtxWorld._33 = mtxView._33;

	// 位置を反映
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CSceneBillboard::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// オフセットの長さを求める
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	// オフセットの角度を求める
	m_fAngle = atan2f(m_fWidth, m_fHeight);

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
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
	}

	// 法線
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[1].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[2].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[3].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

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
void CSceneBillboard::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CSceneBillboard::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    ポリゴンの座標設定処理
//=============================================================================
void CSceneBillboard::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ポリゴンの色設定処理
//=============================================================================
void CSceneBillboard::SetCol(const D3DXCOLOR col)
{
	// 色を設定
	m_Col = col;

	//if (m_pVtxBuff != NULL)
	//{// 頂点バッファが取得できている
	//    // 頂点情報の設定
	//	VERTEX_3D *pVtx;

	//	// 頂点バッファをロックし,頂点データへのポインタを取得
	//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	// 頂点カラー
	//	pVtx[0].col = m_Col;
	//	pVtx[1].col = m_Col;
	//	pVtx[2].col = m_Col;
	//	pVtx[3].col = m_Col;

	//	// 頂点バッファをアンロックする
	//	m_pVtxBuff->Unlock();
	//}
}

//=============================================================================
//    ポリゴンの向き設定処理
//=============================================================================
void CSceneBillboard::SetRot(const float fRot)
{
	m_fRot = fRot;

	//if (m_pVtxBuff != NULL)
	//{// 頂点バッファが取得できている
	//    // 頂点情報の設定
	//	VERTEX_3D *pVtx;

	//	// 頂点バッファをロックし,頂点データへのポインタを取得
	//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	// 頂点の回転を考慮して座標を計算
	//	float XPos[4];
	//	float YPos[4];
	//	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	//	// 頂点座標
	//	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	//	{// 頂点の数だけ繰り返し
	//		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
	//	}
	//	// 頂点バッファをアンロックする
	//	m_pVtxBuff->Unlock();
	//}
}

//=============================================================================
//    ポリゴンの幅設定処理
//=============================================================================
void CSceneBillboard::SetWidth(const float fWidth)
{
	// 幅を設定
	m_fWidth = fWidth;

	//if (m_pVtxBuff != NULL)
	//{// 頂点バッファが取得できている
	//    // 頂点情報の設定
	//	VERTEX_3D *pVtx;

	//	// 頂点バッファをロックし,頂点データへのポインタを取得
	//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	// オフセットの長さを求める
	//	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	//	// オフセットの角度を求める
	//	m_fAngle = atan2f(m_fWidth, m_fHeight);

	//	// 頂点の回転を考慮して座標を計算
	//	float XPos[4];
	//	float YPos[4];
	//	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	//	// 頂点座標
	//	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	//	{// 頂点の数だけ繰り返し
	//		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
	//	}

	//	// 頂点バッファをアンロックする
	//	m_pVtxBuff->Unlock();
	//}
}

//=============================================================================
//    ポリゴンの高さ設定処理
//=============================================================================
void CSceneBillboard::SetHeight(const float fHeight)
{
	// 高さを設定
	m_fHeight = fHeight;

	//if (m_pVtxBuff != NULL)
	//{// 頂点バッファが取得できている
	//    // 頂点情報の設定
	//	VERTEX_3D *pVtx;

	//	// 頂点バッファをロックし,頂点データへのポインタを取得
	//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	// オフセットの長さを求める
	//	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	//	// オフセットの角度を求める
	//	m_fAngle = atan2f(m_fWidth, m_fHeight);

	//	// 頂点の回転を考慮して座標を計算
	//	float XPos[4];
	//	float YPos[4];
	//	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	//	// 頂点座標
	//	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	//	{// 頂点の数だけ繰り返し
	//		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
	//	}

	//	// 頂点バッファをアンロックする
	//	m_pVtxBuff->Unlock();
	//}
}

//=============================================================================
//    ポリゴンを出す角度設定処理
//=============================================================================
void CSceneBillboard::SetAngle(const float fAngle)
{
	// 角度を設定
	m_fAngle = fAngle;

	//if (m_pVtxBuff != NULL)
	//{// 頂点バッファが生成されている
	//    // 頂点情報
	//	VERTEX_3D *pVtx;

	//	// 頂点バッファをロックし,頂点データへのポインタを取得
	//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	// 頂点の回転を考慮して座標を計算
	//	float XPos[4];
	//	float YPos[4];
	//	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	//	// 頂点座標
	//	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	//	{// 頂点の数だけ繰り返し
	//		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
	//	}

	//	// 頂点バッファをアンロックする
	//	m_pVtxBuff->Unlock();
	//}
}

//=============================================================================
//    ポリゴンの長さ設定処理
//=============================================================================
void CSceneBillboard::SetLength(const float fLength)
{
	// 長さを設定
	m_fLength = fLength;

	//if (m_pVtxBuff != NULL)
	//{// 頂点バッファが生成されている
	//    // 頂点情報
	//	VERTEX_3D *pVtx;

	//	// 頂点バッファをロックし,頂点データへのポインタを取得
	//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//	// 頂点の回転を考慮して座標を計算
	//	float XPos[4];
	//	float YPos[4];
	//	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	//	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	//	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	//	// 頂点座標
	//	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	//	{// 頂点の数だけ繰り返し
	//		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
	//	}

	//	// 頂点バッファをアンロックする
	//	m_pVtxBuff->Unlock();
	//}
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CSceneBillboard::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}


//=============================================================================
//    ライティングするかしないか設定処理
//=============================================================================
void CSceneBillboard::SetLighting(const bool bLighting)
{
	m_bLighting = bLighting;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CSceneBillboard::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CSceneBillboard::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ポリゴンの座標取得処理
//=============================================================================
D3DXVECTOR3 CSceneBillboard::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ポリゴンの色取得処理
//=============================================================================
D3DXCOLOR CSceneBillboard::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    ポリゴンの向き取得処理
//=============================================================================
float CSceneBillboard::GetRot(void)
{
	return m_fRot;
}

//=============================================================================
//    ポリゴンの幅取得処理
//=============================================================================
float CSceneBillboard::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ポリゴンの高さ取得処理
//=============================================================================
float CSceneBillboard::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ポリゴンを出す角度取得処理
//=============================================================================
float CSceneBillboard::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
//    ポリゴンの長さ取得処理
//=============================================================================
float CSceneBillboard::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CSceneBillboard::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ライティングするかしないか取得処理
//=============================================================================
bool CSceneBillboard::GetLighting(void)
{
	return m_bLighting;
}