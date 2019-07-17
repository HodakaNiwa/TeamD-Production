//*****************************************************************************
//
//     3Dポリゴンの処理[3Dpolygon.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "3Dpolygon.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
C3DPolygon::C3DPolygon()
{
	// 各種値のクリア
	m_pTexture = NULL;                         // テクスチャへのポインタ
	m_pVtxBuff = NULL;                         // 頂点バッファへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ポリゴンの座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ポリゴンの向き
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // ポリゴンの色
	m_fWidth = 0.0f;                           // ポリゴンの幅
	m_fHeight = 0.0f;                          // ポリゴンの高さ
	m_fAngle = 0.0f;                           // ポリゴンの角度
	m_fLength = 0.0f;                          // ポリゴンの長さ
	D3DXMatrixIdentity(&m_MtxWorld);           // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
C3DPolygon::~C3DPolygon()
{

}

//=============================================================================
//    生成処理
//=============================================================================
C3DPolygon *C3DPolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight)
{
	C3DPolygon *p3DPolygon = NULL; // 3Dポリゴンクラス型のポインタ
	if (p3DPolygon == NULL)
	{// メモリが空になっている
		p3DPolygon = new C3DPolygon;
		if (p3DPolygon != NULL)
		{// インスタンスを生成できた
		 // 各種値の代入
			p3DPolygon->SetPos(pos);         // ポリゴンの座標
			p3DPolygon->SetRot(rot);         // ポリゴンの向き
			p3DPolygon->SetCol(col);         // ポリゴンの色
			p3DPolygon->SetWidth(fWidth);    // ポリゴンの幅
			p3DPolygon->SetHeight(fHeight);  // ポリゴンの高さ

			if (FAILED(p3DPolygon->Init()))
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

	return p3DPolygon;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT C3DPolygon::Init(void)
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
void C3DPolygon::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);
}

//=============================================================================
//    更新処理
//=============================================================================
void C3DPolygon::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void C3DPolygon::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void C3DPolygon::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void C3DPolygon::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

	// 法線計算用ベクトル
	D3DXVECTOR3 vecVer[4];

	// 頂点の法線
	D3DXVECTOR3 nor1, nor2;

	// 外積用にベクトルを作る
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// ベクトルを外積で計算して法線を出す
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// 法線を代入
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

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
void C3DPolygon::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void C3DPolygon::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    ポリゴンの座標設定処理
//=============================================================================
void C3DPolygon::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ポリゴンの向き設定処理
//=============================================================================
void C3DPolygon::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ポリゴンの色設定処理
//=============================================================================
void C3DPolygon::SetCol(const D3DXCOLOR col)
{
	// 色を設定
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	 // 頂点情報の設定
		VERTEX_3D *pVtx;

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
//    ポリゴンの幅設定処理
//=============================================================================
void C3DPolygon::SetWidth(const float fWidth)
{
	// 幅を設定
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	 // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// オフセットの長さを求める
		m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

		// オフセットの角度を求める
		m_fAngle = atan2f(m_fWidth, m_fHeight);

		// 頂点座標
		pVtx[0].pos.x = -sinf(m_fAngle) * m_fLength;
		pVtx[1].pos.x = sinf(m_fAngle) * m_fLength;
		pVtx[2].pos.x = -sinf(m_fAngle) * m_fLength;
		pVtx[3].pos.x = sinf(m_fAngle) * m_fLength;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの高さ設定処理
//=============================================================================
void C3DPolygon::SetHeight(const float fHeight)
{
	// 高さを設定
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	 // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// オフセットの長さを求める
		m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

		// オフセットの角度を求める
		m_fAngle = atan2f(m_fWidth, m_fHeight);

		// 頂点座標
		pVtx[0].pos.y = cosf(m_fAngle) * m_fLength;
		pVtx[1].pos.y = cosf(m_fAngle) * m_fLength;
		pVtx[2].pos.y = -cosf(m_fAngle) * m_fLength;
		pVtx[3].pos.y = -cosf(m_fAngle) * m_fLength;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの角度設定処理
//=============================================================================
void C3DPolygon::SetAngle(const float fAngle)
{
	// 角度を設定
	m_fAngle = fAngle;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	 // 頂点情報
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ポリゴンの長さ設定処理
//=============================================================================
void C3DPolygon::SetLength(const float fLength)
{
	// 長さを設定
	m_fLength = fLength;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが生成されている
	 // 頂点情報
		VERTEX_2D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void C3DPolygon::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 C3DPolygon::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 C3DPolygon::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ポリゴンの座標取得処理
//=============================================================================
D3DXVECTOR3 C3DPolygon::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ポリゴンの向き取得処理
//=============================================================================
D3DXVECTOR3 C3DPolygon::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ポリゴンの色取得処理
//=============================================================================
D3DXCOLOR C3DPolygon::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    ポリゴンの幅取得処理
//=============================================================================
float C3DPolygon::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ポリゴンの高さ取得処理
//=============================================================================
float C3DPolygon::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ポリゴンの角度取得処理
//=============================================================================
float C3DPolygon::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
//    ポリゴンの長さ取得処理
//=============================================================================
float C3DPolygon::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX C3DPolygon::GetMtxWorld(void)
{
	return m_MtxWorld;
}