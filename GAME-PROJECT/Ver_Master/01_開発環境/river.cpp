//*****************************************************************************
//
//     川の処理[river.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "river.h"
#include "boxCollider.h"
#include "meshfield.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define RIVER_ADDTEX_U (0.0012f)
#define RIVER_ADDTEX_V (0.0004f)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CRiver::CRiver(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CRiver::~CRiver()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRiver *CRiver::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx, int nPriority)
{
	CRiver *pRiver = NULL;  // 川クラス型のポインタ
	if (pRiver == NULL)
	{// メモリが空になっている
		pRiver = new CRiver(nPriority);
		if (pRiver != NULL)
		{// インスタンスを生成できた
			if (FAILED(pRiver->Init(pos, rot, pTexture, fBoxWidth, fBoxHeight, fBoxDepth, nXBlock, nZBlock, nTexIdx)))
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

	return pRiver;
}


//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRiver::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx)
{
	// 各種値の設定
	SetPos(pos);
	SetRot(rot);
	SetTexIdx(nTexIdx);

	// 当たり判定用箱モデルを生成
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	// 川に使うポリゴンを生成
	m_pMeshField = CMeshField::Create(pos, rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASS_SIZE_X, MASS_SIZE_X, nXBlock, nZBlock, nXBlock, nZBlock, NULL, true);
	m_pMeshField->BindTexture(pTexture);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRiver::Uninit(void)
{
	// 地面の破棄
	if (m_pMeshField != NULL)
	{
		m_pMeshField->Uninit();
		m_pMeshField = NULL;
	}

	// 共通の終了処理
	CObject3D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRiver::Update(void)
{
	if (m_pMeshField == NULL) { return; }
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();
	if (pVtxBuff == NULL) { return; }

	// テクスチャUV座標を動かす
	VERTEX_3D *pVtx;
	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nXBlock = m_pMeshField->GetXBlock();
	int nZBlock = m_pMeshField->GetZBlock();
	for (int nCntV = 0; nCntV < nZBlock; nCntV++)
	{// 垂直方向の分割数 + 1だけ繰り返し
		for (int nCntH = 0; nCntH < nXBlock; nCntH++)
		{// 水平方向の分割数 + 1だけ繰り返し
			pVtx[nCntH].tex.x += RIVER_ADDTEX_U;
			pVtx[nCntH].tex.y += RIVER_ADDTEX_V;
		}

		// ポインタを進める
		pVtx += nXBlock + 1;
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
//    描画処理
//=============================================================================
void CRiver::Draw(void)
{
	// 共通の描画処理
	CObject3D::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作成する処理
//=============================================================================
void CRiver::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, true);
	SetBoxCollider(pBoxCollider);
}

//=============================================================================
//    使用しているテクスチャの番号を設定する
//=============================================================================
void CRiver::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    使用しているテクスチャの番号を取得する
//=============================================================================
int CRiver::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    川用のメッシュフィールドクラスを取得
//=============================================================================
CMeshField *CRiver::GetMeshField(void)
{
	return m_pMeshField;
}