//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CModel::CModel()
{
	// 位置・向きの初期設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期設定
}
//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{

}
//=============================================================================
// オブジェクトXの生成
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos)
{
	CModel *pModel = NULL;

	if (pModel == NULL)
	{
		pModel = new CModel;		//動的確保
		if (pModel != NULL)
		{
			pModel->Init();			//初期化処理
			pModel->m_pos = pos;	//位置の代入8
		}
	}
	return pModel;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{
	////バッファの開放
	//if (m_pBuffMat != NULL)
	//{
	//	m_pBuffMat->Release();
	//	m_pBuffMat = NULL;
	//}
	//// メッシュの開放
	//if (m_pMesh != NULL)
	//{
	//	m_pMesh->Release();
	//	m_pMesh = NULL;
	//}
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス
	D3DMATERIAL9 matDef;							//現在のマテリアル保存用
	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != NULL)
	{//親がいる場合
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//親がいない場合
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	//親モデルのマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャ無効
		pDevice->SetTexture(0, NULL);

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 位置の設置処理
//=============================================================================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 位置の設置処理
//=============================================================================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// 位置の取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 向きの取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// Xファイルの取得処理
//=============================================================================
void CModel::BindX(LPD3DXMESH mesh, LPD3DXBUFFER buffmat, DWORD nummat)
{
	m_pMesh = mesh;
	m_pBuffMat = buffmat;
	m_nNumMat = nummat;
}

//=============================================================================
// 親の設定
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}

//=============================================================================
// ワールドマトリックスの取得
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_mtxWorld;
}