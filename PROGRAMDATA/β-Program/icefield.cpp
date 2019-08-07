//*****************************************************************************
//
//     氷の処理[icefield.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "icefield.h"
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
CIceField::CIceField(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CIceField::~CIceField()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CIceField *CIceField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx, int nPriority)
{
	CIceField *pIceField = NULL;  // 氷クラス型のポインタ
	if (pIceField == NULL)
	{// メモリが空になっている
		pIceField = new CIceField(nPriority);
		if (pIceField != NULL)
		{// インスタンスを生成できた
			if (FAILED(pIceField->Init(pos, rot, pTexture, fBoxWidth, fBoxHeight, fBoxDepth, nXBlock, nZBlock, nTexIdx)))
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

	return pIceField;
}


//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CIceField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nXBlock, int nZBlock, int nTexIdx)
{
	// 各種値の設定
	SetPos(pos);
	SetRot(rot);
	SetTexIdx(nTexIdx);

	// 当たり判定用箱モデルを生成
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	// 氷に使うポリゴンを生成
	m_pMeshField = CMeshField::Create(pos, rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASU_SIZE_X, MASU_SIZE_Z, nXBlock, nZBlock, nXBlock, nZBlock, NULL, true);
	m_pMeshField->BindTexture(pTexture);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CIceField::Uninit(void)
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
void CIceField::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CIceField::Draw(void)
{
	// 共通の描画処理
	CObject3D::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作成する処理
//=============================================================================
void CIceField::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, false);
	SetBoxCollider(pBoxCollider);
}

//=============================================================================
//    使用しているテクスチャの番号を設定する
//=============================================================================
void CIceField::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    使用しているテクスチャの番号を取得する
//=============================================================================
int CIceField::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    氷用のメッシュフィールドクラスを取得
//=============================================================================
CMeshField *CIceField::GetMeshField(void)
{
	return m_pMeshField;
}