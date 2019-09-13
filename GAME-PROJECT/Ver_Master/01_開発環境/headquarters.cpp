//*****************************************************************************
//
//     司令部の処理[headquarters.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "headquarters.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "boxCollider.h"
#include "game.h"
#include "debugproc.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define HEADQUARTERS_COL_HEIGHT (75.0f)   // 当たり判定の高さ

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CHeadQuarters::CHeadQuarters(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	// 各種値のクリア
	m_pModel = NULL;   // モデルクラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CHeadQuarters::~CHeadQuarters()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CHeadQuarters *CHeadQuarters::Create(int nAreaX, int nAreaZ, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, int nPriority)
{
	CHeadQuarters *pHeadQuarters = NULL;  // 司令部クラス型のポインタ
	if (pHeadQuarters == NULL)
	{// メモリが空になっている
		pHeadQuarters = new CHeadQuarters(nPriority);
		if (pHeadQuarters != NULL)
		{// インスタンスを生成できた
			if (FAILED(pHeadQuarters->Init(nAreaX, nAreaZ, pMesh, pBuffMat, nNumMat, pTexture)))
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

	return pHeadQuarters;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CHeadQuarters::Init(int nAreaX, int nAreaZ, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture)
{
	// 今いるエリアから座標を計算
	SetArea(nAreaX, nAreaZ);

	// モデルクラスの作成
	m_pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, GetRot(), pMesh, pBuffMat, nNumMat, pTexture);

	// 当たり判定用箱モデルの作成
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), MASS_SIZE_X, HEADQUARTERS_COL_HEIGHT, MASS_SIZE_X, true);
	SetBoxCollider(pBoxCollider);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CHeadQuarters::Uninit(void)
{
	// モデルの破棄
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CHeadQuarters::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CHeadQuarters::Draw(void)
{
	// レンダリングクラスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // ワールドマトリックス設定処理
			SetMtxWorld(pDevice);

			// モデルの描画
			if (m_pModel != NULL)
			{
				m_pModel->Draw();
			}

			// オブジェクト3Dの描画処理
			CObject3D::Draw();
		}
	}
}

//=============================================================================
//    当たった時の処理
//=============================================================================
void CHeadQuarters::Hit(CScene *pScene)
{
	// 弾ではなかったら処理しない
	if (pScene->GetObjType() != OBJTYPE_BULLET) return;

	// 崩れた状態にする


	// ゲームオーバー状態にさせる
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		if (pGame == NULL) { return; }
		pGame->SetState(CGame::STATE_GAMEOVER);
	}
}

//=============================================================================
//    モデルクラス型のポインタ設定処理
//=============================================================================
void CHeadQuarters::SetModel(CModel *pModel)
{
	m_pModel = pModel;
}

//=============================================================================
//    今どのエリアにいるかを設定
//=============================================================================
void CHeadQuarters::SetArea(int nAreaX, int nAreaZ)
{
	// エリアを設定
	m_nAreaX = nAreaX;
	m_nAreaZ = nAreaZ;

	// エリアから座標を求める
	D3DXVECTOR3 pos = INITIALIZE_D3DXVECTOR3;
	pos.x = -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + (MASS_SIZE_X * m_nAreaX) - MASS_SIZE_X_HALF;
	pos.z = ((MASS_SIZE_X * MASS_BLOCK_Z) / 2) - (MASS_SIZE_X * m_nAreaZ) + MASS_SIZE_X_HALF;
	SetPos(pos);

	// 当たり判定用箱モデルもずらす
	if (GetBoxCollider() != NULL)
	{
		GetBoxCollider()->SetPos(pos);
	}
}

//=============================================================================
//    モデルクラス型のポインタ取得処理
//=============================================================================
CModel *CHeadQuarters::GetModel(void)
{
	return m_pModel;
}

//=============================================================================
//    今どのエリア(横)にいるかを取得
//=============================================================================
int CHeadQuarters::GetAreaX(void)
{
	return m_nAreaX;
}

//=============================================================================
//    今どのエリア(奥行)にいるかを取得
//=============================================================================
int CHeadQuarters::GetAreaZ(void)
{
	return m_nAreaZ;
}