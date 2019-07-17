//*****************************************************************************
//
//     配置用ビルボードの処理[billboardObject.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "billboardObject.h"
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


//*****************************************************************************
//    CBillboardObjectの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CBillboardObject::CBillboardObject(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// 各種値のクリア
	m_nTexIdx = 0;           // テクスチャ番号
	m_bDrawAddtive = false;  // 加算合成するかしないか
	m_fColRange = 0.0f;      // 当たり判定を取る範囲
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBillboardObject::~CBillboardObject()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBillboardObject *CBillboardObject::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange, int nPriority)
{
	CBillboardObject *pBillboardObject = NULL;      // 配置ビルボードクラス型のポインタ
	if (pBillboardObject == NULL)
	{// メモリが空になっている
		pBillboardObject = new CBillboardObject(nPriority);
		if (pBillboardObject != NULL)
		{// インスタンスを生成できた
			if (FAILED(pBillboardObject->Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, fColRange)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pBillboardObject;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBillboardObject::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, float fColRange)
{
	// 各種値の代入
	SetPos(pos);                    // ポリゴンの座標
	SetCol(col);                    // ポリゴンの色
	SetRot(fRot);                   // ポリゴンの向き
	SetWidth(fWidth);               // ポリゴンの幅
	SetHeight(fHeight);             // ポリゴンの高さ
	SetLighting(bLighting);         // ライティングするかしないか
	SetDrawAddtive(bDrawAddtive);   // 加算合成で描画するかしないか
	SetTexIdx(nTexIdx);             // 使用しているテクスチャの番号
	SetColRange(fColRange);         // 当たり判定を取る範囲

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
void CBillboardObject::Uninit(void)
{
	// 共通の終了処理
	CSceneBillboard::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CBillboardObject::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBillboardObject::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			if (m_bDrawAddtive == true)
			{// αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // 有効に
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);               // 透明度が0
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // より大きいものを描画する

			// 共通の描画処理
			CSceneBillboard::Draw();

			// アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CBillboardObject::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxWorld, mtxView, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// カメラの向きを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ワールドマトリックスに向きを設定(カメラの逆行列を入れることでカメラの正面に向きを補正)
	mtxWorld._11 = mtxView._11;
	mtxWorld._12 = mtxView._21;
	mtxWorld._13 = mtxView._31;
	mtxWorld._21 = mtxView._12;
	mtxWorld._22 = mtxView._22;
	mtxWorld._23 = mtxView._32;
	mtxWorld._31 = mtxView._13;
	mtxWorld._32 = mtxView._23;
	mtxWorld._33 = mtxView._33;

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CSceneBillboard::SetMtxWorld(mtxWorld);
}

//=============================================================================
//    使用しているテクスチャの番号設定処理
//=============================================================================
void CBillboardObject::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    加算合成で描画するかしないか設定処理
//=============================================================================
void CBillboardObject::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    当たり判定を取る範囲設定処理
//=============================================================================
void CBillboardObject::SetColRange(const float fColRange)
{
	m_fColRange = fColRange;
}

//=============================================================================
//    使用しているテクスチャの番号取得処理
//=============================================================================
int CBillboardObject::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    加算合成で描画するかしないか取得処理
//=============================================================================
bool CBillboardObject::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}

//=============================================================================
//    当たり判定を取る範囲取得処理
//=============================================================================
float CBillboardObject::GetColRange(void)
{
	return m_fColRange;
}