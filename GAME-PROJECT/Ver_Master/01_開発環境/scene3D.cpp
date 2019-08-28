//*****************************************************************************
//
//     3Dポリゴンの処理[scene3D.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"
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
CScene3D::CScene3D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
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
CScene3D::~CScene3D()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CScene3D *CScene3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nPriority)
{
	CScene3D *pScene3D = NULL;             // 3Dポリゴンクラス型のポインタ
	if (pScene3D == NULL)
	{// メモリが空になっている
		pScene3D = new CScene3D(nPriority);
		if (pScene3D != NULL)
		{// インスタンスを生成できた
		 // 各種値の代入
			pScene3D->SetPos(pos);         // ポリゴンの座標
			pScene3D->SetRot(rot);         // ポリゴンの向き
			pScene3D->SetCol(col);         // ポリゴンの色
			pScene3D->SetWidth(fWidth);    // ポリゴンの幅
			pScene3D->SetHeight(fHeight);  // ポリゴンの高さ

			if (FAILED(pScene3D->Init()))
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

	return pScene3D;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CScene3D::Init(void)
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
void CScene3D::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CScene3D::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CScene3D::Draw(void)
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
void CScene3D::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転行列を作成(D3DXMatrixRotationYawPitchRoll参照)
	float fSinPitch = sinf(m_Rot.x);
	float fCosPitch = cosf(m_Rot.x);
	float fSinYaw = sinf(m_Rot.y);
	float fCosYaw = cosf(m_Rot.y);
	float fSinRoll = sinf(m_Rot.z);
	float fCosRoll = cosf(m_Rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// 回転を反映する
	m_MtxWorld._11 = mtxRot._11;
	m_MtxWorld._12 = mtxRot._12;
	m_MtxWorld._13 = mtxRot._13;
	m_MtxWorld._21 = mtxRot._21;
	m_MtxWorld._22 = mtxRot._22;
	m_MtxWorld._23 = mtxRot._23;
	m_MtxWorld._31 = mtxRot._31;
	m_MtxWorld._32 = mtxRot._32;
	m_MtxWorld._33 = mtxRot._33;

	// オフセット位置を反映
	m_MtxWorld._41 = m_Pos.x;
	m_MtxWorld._42 = m_Pos.y;
	m_MtxWorld._43 = m_Pos.z;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CScene3D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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
void CScene3D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CScene3D::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    ポリゴンの座標設定処理
//=============================================================================
void CScene3D::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ポリゴンの向き設定処理
//=============================================================================
void CScene3D::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ポリゴンの色設定処理
//=============================================================================
void CScene3D::SetCol(const D3DXCOLOR col)
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
void CScene3D::SetWidth(const float fWidth)
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
void CScene3D::SetHeight(const float fHeight)
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
void CScene3D::SetAngle(const float fAngle)
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
void CScene3D::SetLength(const float fLength)
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
void CScene3D::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene3D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CScene3D::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ポリゴンの座標取得処理
//=============================================================================
D3DXVECTOR3 CScene3D::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ポリゴンの向き取得処理
//=============================================================================
D3DXVECTOR3 CScene3D::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ポリゴンの色取得処理
//=============================================================================
D3DXCOLOR CScene3D::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    ポリゴンの幅取得処理
//=============================================================================
float CScene3D::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ポリゴンの高さ取得処理
//=============================================================================
float CScene3D::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ポリゴンの角度取得処理
//=============================================================================
float CScene3D::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
//    ポリゴンの長さ取得処理
//=============================================================================
float CScene3D::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CScene3D::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ポリゴンのワールド座標における高さ取得する処理
//=============================================================================
float CScene3D::GetPolyHeight(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;        // 高さ
	D3DXVECTOR3 PolygonNormal;   // 頂点の法線
	D3DXVECTOR3 PolygonPos;      // プレイヤーとのベクトルを作るポリゴンの頂点座標
	D3DXVECTOR3 Vec;             // プレイヤーと頂点座標とのベクトル
	D3DXVECTOR3 VerPos[4];       // 3Dポリゴンの頂点座標
	D3DXVECTOR3 VerNor[4];       // 3Dポリゴンの頂点法線
	D3DXMATRIX VerMtxWorld[4];   // 3Dポリゴンの頂点ワールドマトリックス
	bool bLeftPolygon = false;   // 左側のポリゴンに乗っているかどうか
	bool bRightPolygon = false;  // 右側のポリゴンに乗っているかどうか
	int nCntLeftPolygon = 0;     // 左側ポリゴンのベクトルの判定を通った回数
	int nCntRightPolygon = 0;    // 右側ポリゴンのベクトルの判定を通った回数

	// 頂点情報を取得する
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 判定するベクトルの座標を取得
	VerPos[0] = pVtx[0].pos;
	VerPos[1] = pVtx[1].pos;
	VerPos[2] = pVtx[2].pos;
	VerPos[3] = pVtx[3].pos;

	// 判定に使う法線を取得
	VerNor[0] = pVtx[0].nor;
	VerNor[1] = pVtx[1].nor;
	VerNor[2] = pVtx[2].nor;
	VerNor[3] = pVtx[3].nor;

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 3Dポリゴンの向きを考慮して頂点のワールドマトリックスを計算する
	D3DXMATRIX mtxRot, mtxTrans;        // 計算用マトリックス
	D3DXMATRIX mtxParent = m_MtxWorld;  // 3Dポリゴンのワールドマトリックスを取得
	D3DXVECTOR3 VerRot = m_Rot;         // 3Dポリゴンの向きを取得
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// 判定する頂点の数だけ繰り返し
	 // ワールドマトリックスの初期化
		D3DXMatrixIdentity(&VerMtxWorld[nCntVer]);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, VerRot.y, VerRot.x, VerRot.z);
		D3DXMatrixMultiply(&VerMtxWorld[nCntVer], &VerMtxWorld[nCntVer], &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, VerPos[nCntVer].x, VerPos[nCntVer].y, VerPos[nCntVer].z);
		D3DXMatrixMultiply(&VerMtxWorld[nCntVer], &VerMtxWorld[nCntVer], &mtxTrans);

		// 親のワールドマトリックスを掛け合わせる
		D3DXMatrixMultiply(&VerMtxWorld[nCntVer], &VerMtxWorld[nCntVer], &mtxParent);
	}


	// 各種値の取得
	D3DXVECTOR3 WorldPolygonPos[4] =
	{// 向きを含めた3Dポリゴンの頂点ワールド座標
		D3DXVECTOR3(VerMtxWorld[0]._41,VerMtxWorld[0]._42,VerMtxWorld[0]._43),
		D3DXVECTOR3(VerMtxWorld[1]._41,VerMtxWorld[1]._42,VerMtxWorld[1]._43),
		D3DXVECTOR3(VerMtxWorld[2]._41,VerMtxWorld[2]._42,VerMtxWorld[2]._43),
		D3DXVECTOR3(VerMtxWorld[3]._41,VerMtxWorld[3]._42,VerMtxWorld[3]._43),
	};

	D3DXVECTOR3 WorldLeftPolygonPos[3] =
	{// 向きを含めた3Dポリゴン(左側)のワールド座標
		WorldPolygonPos[1],
		WorldPolygonPos[2],
		WorldPolygonPos[0],
	};
	D3DXVECTOR3 WorldRightPolygonPos[3] =
	{// 向きを含めた3Dポリゴン(右側)のワールド座標
		WorldPolygonPos[1],
		WorldPolygonPos[3],
		WorldPolygonPos[2],
	};

	// 計算用外積ベクトル
	D3DXVECTOR3 VecVerToVer, VecPlayerToVer;
	// 左側のポリゴンに乗っているか判定
	for (int nCntVer = 0; nCntVer < 3; nCntVer++)
	{// 判定する頂点の数だけ繰り返し
		VecVerToVer = WorldRightPolygonPos[(nCntVer + 1) % 3] - WorldRightPolygonPos[nCntVer];
		VecPlayerToVer = pos - WorldRightPolygonPos[nCntVer];

		if ((VecVerToVer.x * VecPlayerToVer.z) - (VecVerToVer.z * VecPlayerToVer.x) <= 0)
		{// ベクトルの右側にいる
			nCntRightPolygon++;
		}
	}

	// 右側のポリゴンに乗っているか判定
	for (int nCntVer = 0; nCntVer < 3; nCntVer++)
	{// 判定する頂点の数だけ繰り返し
		VecVerToVer = WorldLeftPolygonPos[(nCntVer + 1) % 3] - WorldLeftPolygonPos[nCntVer];
		VecPlayerToVer = pos - WorldLeftPolygonPos[nCntVer];

		if ((VecVerToVer.x * VecPlayerToVer.z) - (VecVerToVer.z * VecPlayerToVer.x) <= 0)
		{// ベクトルの右側にいる
			nCntLeftPolygon++;
		}
	}

	// 判定に必要な変数を宣言
	D3DXVECTOR3 PolygonVerPos[3];
	D3DXVECTOR3 VecPolygon[2];
	D3DXVECTOR3 nor;
	float fData = 0.0f;

	if (nCntLeftPolygon == 3)
	{// 左側のポリゴンのすべてのベクトルの内側にいる
		bLeftPolygon = true;
		PolygonVerPos[0] = D3DXVECTOR3(WorldPolygonPos[0].x, WorldPolygonPos[0].y - WorldPolygonPos[0].y, WorldPolygonPos[0].z);
		PolygonVerPos[1] = D3DXVECTOR3(WorldPolygonPos[1].x, WorldPolygonPos[1].y - WorldPolygonPos[0].y, WorldPolygonPos[1].z);
		PolygonVerPos[2] = D3DXVECTOR3(WorldPolygonPos[2].x, WorldPolygonPos[2].y - WorldPolygonPos[0].y, WorldPolygonPos[2].z);

		VecPolygon[0] = PolygonVerPos[1] - PolygonVerPos[0];
		VecPolygon[1] = PolygonVerPos[2] - PolygonVerPos[0];

		D3DXVec3Cross(&PolygonNormal, &VecPolygon[0], &VecPolygon[1]);
		D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);
		fData = WorldPolygonPos[0].y;

		// 引数の座標と頂点座標のベクトルを計算
		Vec = pos - PolygonVerPos[0];

		// 内積の概念を利用して高さを割り出す
		float DotX = (PolygonNormal.x * Vec.x);      // X成分の内積を計算
		float DotZ = (PolygonNormal.z * Vec.z);      // Z成分の内積を計算
		float Dot = -DotX - DotZ;                    // X成分とZ成分の内積同士を引く
		fHeight = (Dot / PolygonNormal.y) + fData;   // 引いたものをY成分の法線で割る
	}
	if (nCntRightPolygon == 3)
	{// 右側のポリゴンのすべてのベクトルの内側にいる
		bRightPolygon = true;
		PolygonVerPos[0] = D3DXVECTOR3(WorldPolygonPos[3].x, WorldPolygonPos[3].y - WorldPolygonPos[3].y, WorldPolygonPos[3].z);
		PolygonVerPos[1] = D3DXVECTOR3(WorldPolygonPos[1].x, WorldPolygonPos[1].y - WorldPolygonPos[3].y, WorldPolygonPos[1].z);
		PolygonVerPos[2] = D3DXVECTOR3(WorldPolygonPos[2].x, WorldPolygonPos[2].y - WorldPolygonPos[3].y, WorldPolygonPos[2].z);

		VecPolygon[0] = PolygonVerPos[1] - PolygonVerPos[0];
		VecPolygon[1] = PolygonVerPos[2] - PolygonVerPos[0];

		D3DXVec3Cross(&PolygonNormal, &VecPolygon[0], &VecPolygon[1]);
		D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);
		fData = WorldPolygonPos[3].y;

		// 引数の座標と頂点座標のベクトルを計算
		Vec = pos - PolygonVerPos[0];

		// 内積の概念を利用して高さを割り出す
		float DotX = (PolygonNormal.x * Vec.x);      // X成分の内積を計算
		float DotZ = (PolygonNormal.z * Vec.z);      // Z成分の内積を計算
		float Dot = -DotX - DotZ;                    // X成分とZ成分の内積同士を引く
		fHeight = (Dot / PolygonNormal.y) + fData;   // 引いたものをY成分の法線で割る
	}

	return fHeight;  // 高さを返す
}