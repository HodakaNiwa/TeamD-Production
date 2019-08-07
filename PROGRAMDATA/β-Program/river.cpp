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
	m_pMeshField = CMeshField::Create(pos, rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASU_SIZE_X, MASU_SIZE_Z, nXBlock, nZBlock, nXBlock, nZBlock, NULL, true);
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