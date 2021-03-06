//*****************************************************************************
//
//     空の処理[sky.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sky.h"
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
CSky::CSky(int nPriority, OBJTYPE objType) : CMeshSphere(nPriority, objType)
{
	// 各種値のクリア
	m_nTexIdx = 0;
	m_fRollSpeed = 0.0f;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSky::~CSky()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSky *CSky::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nTexIdx, float fRollSpeed, int nPriority)
{
	CSky *pSky = NULL;       // 空クラス型のポインタ
	if (pSky == NULL)
	{// メモリが空になっている
		pSky = new CSky(nPriority);
		if (pSky != NULL)
		{// インスタンスを生成できた
		    // 各種値の代入
			pSky->SetPos(pos);               // 空の座標
			pSky->SetRot(rot);               // 空の向き
			pSky->SetCol(col);               // 空の色
			pSky->SetRadius(fRadius);        // 空の半径
			pSky->SetXBlock(nXBlock);        // 横の分割数
			pSky->SetYBlock(nYBlock);        // 縦の分割数
			pSky->SetTexIdx(nTexIdx);        // 使用しているテクスチャの番号
			pSky->SetRollSpeed(fRollSpeed);  // 回転させるスピード

			if (FAILED(pSky->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pSky;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSky::Init(void)
{
	if (FAILED(CMeshSphere::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSky::Uninit(void)
{
	// 共通の終了処理
	CMeshSphere::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CSky::Update(void)
{
	D3DXVECTOR3 rot = GetRot();

	rot.y += D3DX_PI * m_fRollSpeed;
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	SetRot(rot);
}

//=============================================================================
//    描画処理
//=============================================================================
void CSky::Draw(void)
{
	// 共通の描画処理
	CMeshSphere::Draw();
}

//=============================================================================
//    使用しているテクスチャの番号を設定
//=============================================================================
void CSky::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    回転させるスピードを取得
//=============================================================================
void CSky::SetRollSpeed(const float fRollSpeed)
{
	m_fRollSpeed = fRollSpeed;
}

//=============================================================================
//    使用しているテクスチャの番号を取得
//=============================================================================
int CSky::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    回転させるスピードを取得
//=============================================================================
float CSky::GetRollSpeed(void)
{
	return m_fRollSpeed;
}