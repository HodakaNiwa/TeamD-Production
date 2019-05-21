//=============================================================================
//
// モデル生成の処理 [modelcreate.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "modelcreate.h"
#include "manager.h"
#include "renderer.h"
#include "loadfilefunction.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CModelCreate::CModelCreate()
{
	m_nNumModel = 0;    //モデルの総数初期化
	m_pMesh = NULL;     //メッシュ初期化
	m_pBuffMat = NULL;  //マテリアル初期化
	m_pNumMat = NULL;   //マテリアル総数初期化
	m_pVtxMax = NULL;   //頂点座標最大値初期化
	m_pVtxMin = NULL;   //頂点座標最小値初期化
}

//=============================================================================
// デストラクタ
//=============================================================================
CModelCreate::~CModelCreate()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CModelCreate *CModelCreate::Create(int nNumModel)
{
	CModelCreate *pModelCreate = NULL;  //モデル生成ポインタ
	if (pModelCreate == NULL)
	{//NULLの場合
		pModelCreate = new CModelCreate;	//動的確保
		if (pModelCreate != NULL)
		{//NULLでない場合
			pModelCreate->SetNumModel(nNumModel);  //モデル総数設置処理
			pModelCreate->Init();					//初期化処理
		}
	}
	return pModelCreate;
}

//=============================================================================
// モデルの読み込み処理
//=============================================================================
HRESULT CModelCreate::Init(void)
{
	//メッシュを総数分動的確保
	m_pMesh = new LPD3DXMESH[m_nNumModel];

	//マテリアルを総数分動的確保
	m_pBuffMat = new LPD3DXBUFFER[m_nNumModel];

	//マテリアル総数を総数分動的確保
	m_pNumMat = new DWORD[m_nNumModel];

	//頂点座標最大値を総数分動的確保
	m_pVtxMax = new D3DXVECTOR3[m_nNumModel];

	//頂点座標最小値を総数分動的確保
	m_pVtxMin = new D3DXVECTOR3[m_nNumModel];

	return S_OK;
}

//=============================================================================
// モデルの開放処理
//=============================================================================
void CModelCreate::Uninit(void)
{
	// メッシュ開放
	if (m_pMesh != NULL)
	{
		for (int nCntMesh = 0; nCntMesh < m_nNumModel; nCntMesh++)
		{
			m_pMesh[nCntMesh]->Release();
		}
		delete[] m_pMesh;
		m_pMesh = NULL;
	}

	// マテリアル開放
	if (m_pBuffMat != NULL)
	{
		for (int nCntBuffMat = 0; nCntBuffMat < m_nNumModel; nCntBuffMat++)
		{
			m_pBuffMat[nCntBuffMat]->Release();
		}
		delete[] m_pBuffMat;
		m_pBuffMat = NULL;
	}

	// マテリアル総数開放
	if (m_pNumMat != NULL)
	{
		delete[] m_pNumMat;
		m_pNumMat = NULL;
	}

	// 頂点座標の最大値開放
	if (m_pVtxMax != NULL)
	{
		delete[] m_pVtxMax;
		m_pVtxMax = NULL;
	}

	// 頂点の最小値情報の開放
	if (m_pVtxMin != NULL)
	{
		delete[] m_pVtxMin;
		m_pVtxMin = NULL;
	}
}

//=============================================================================
// 読み込むモデルの数を設定する
//=============================================================================
void CModelCreate::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//=============================================================================
// モデルのメッシュ情報を設定する
//=============================================================================
void CModelCreate::SetMesh(LPD3DXMESH pMesh, int nIdx)
{
	m_pMesh[nIdx] = pMesh;

	if (m_pMesh[nIdx] != NULL)
	{// モデルが読み込めた
	 // 最小値と最大値を初期化
		m_pVtxMin[nIdx] = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		m_pVtxMax[nIdx] = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);

		int nNumVtx;     // 頂点数
		DWORD sizeFVF;   // 頂点フォーマットのサイズ
		BYTE *pVtxBuff;  // 頂点バッファへのポインタ

		// 頂点数を獲得
		nNumVtx = m_pMesh[nIdx]->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nIdx]->GetFVF());

		// 頂点バッファをロック
		m_pMesh[nIdx]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{// すべての頂点を比較してモデルの最小値,最大値を抜き出す
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

			if (Vtx.x < m_pVtxMin[nIdx].x)
			{// 取り出したX座標の最小値が今までのよりも小さい
				m_pVtxMin[nIdx].x = Vtx.x;  // 最小値更新
			}
			if (Vtx.y < m_pVtxMin[nIdx].y)
			{// 取り出したY座標の最小値が今までのよりも小さい
				m_pVtxMin[nIdx].y = Vtx.y;  // 最小値更新
			}
			if (Vtx.z < m_pVtxMin[nIdx].z)
			{// 取り出したZ座標の最小値が今までのよりも小さい
				m_pVtxMin[nIdx].z = Vtx.z;  // 最小値更新
			}
			if (Vtx.x > m_pVtxMax[nIdx].x)
			{// 取り出したX座標の最大値が今までのよりも大きい
				m_pVtxMax[nIdx].x = Vtx.x;  // 最大値更新
			}
			if (Vtx.y > m_pVtxMax[nIdx].y)
			{// 取り出したY座標の最大値が今までのよりも大きい
				m_pVtxMax[nIdx].y = Vtx.y;  // 最大値更新
			}
			if (Vtx.z > m_pVtxMax[nIdx].z)
			{// 取り出したZ座標の最大値が今までのよりも大きい
				m_pVtxMax[nIdx].z = Vtx.z;  // 最大値更新
			}

			pVtxBuff += sizeFVF;  // サイズ分ポインタを進める
		}
		// 頂点バッファをアンロック
		m_pMesh[nIdx]->UnlockVertexBuffer();
	}
}

//=============================================================================
// モデルのマテリアル情報を設定する
//=============================================================================
void CModelCreate::SetBuffMat(LPD3DXBUFFER pBuffMat, int nIdx)
{
	m_pBuffMat[nIdx] = pBuffMat;
}

//=============================================================================
// モデルのマテリアルの数情報を設定する
//=============================================================================
void CModelCreate::SetNumMat(DWORD nNumMat, int nIdx)
{
	m_pNumMat[nIdx] = nNumMat;
}

//=============================================================================
// モデルの頂点の最大値情報を設定する
//=============================================================================
void CModelCreate::SetVtxMax(D3DXVECTOR3 VtxMax, int nIdx)
{
	m_pVtxMax[nIdx] = VtxMax;
}

//=============================================================================
// モデルの頂点の最小値情報を設定する
//=============================================================================
void CModelCreate::SetVtxMin(D3DXVECTOR3 VtxMin, int nIdx)
{
	m_pVtxMin[nIdx] = VtxMin;
}

//=============================================================================
// モデルのメッシュ情報を取得する
//=============================================================================
LPD3DXMESH CModelCreate::GetMesh(int nIdx)
{
	LPD3DXMESH pMesh = NULL;  // メッシュ情報

	if (m_pMesh != NULL)
	{// メモリが確保されてる
		pMesh = m_pMesh[nIdx];   // 保存されたメッシュ情報を代入
	}

	return pMesh;  // メッシュ情報を返す
}

//=============================================================================
// モデルのマテリアル情報を取得する
//=============================================================================
LPD3DXBUFFER CModelCreate::GetBuffMat(int nIdx)
{
	LPD3DXBUFFER pBuffMat = NULL;  // マテリアル情報

	if (m_pBuffMat != NULL)
	{// メモリが確保されてる
		pBuffMat = m_pBuffMat[nIdx];   // 保存されたマテリアル情報を代入
	}

	return pBuffMat;  // マテリアル情報を返す
}

//=============================================================================
// モデルのマテリアルの数情報を取得する
//=============================================================================
DWORD CModelCreate::GetNumMat(int nIdx)
{
	DWORD nNumMat = 0;  // マテリアルの数情報

	if (m_pNumMat != NULL)
	{// メモリが確保されてる
		nNumMat = m_pNumMat[nIdx];   // 保存されたマテリアルの数情報を代入
	}

	return nNumMat;  // マテリアルの数情報を返す
}

//=============================================================================
// モデルの頂点の最大値情報を取得する
//=============================================================================
D3DXVECTOR3 CModelCreate::GetVtxMax(int nIdx)
{
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 頂点の最大値情報

	if (m_pVtxMax != NULL)
	{// メモリが確保されてる
		VtxMax = m_pVtxMax[nIdx];   // 保存された頂点の最大値情報を代入
	}

	return VtxMax;  // 頂点の最大値情報を返す
}

//=============================================================================
// モデルの頂点の最小値情報を取得する
//=============================================================================
D3DXVECTOR3 CModelCreate::GetVtxMin(int nIdx)
{
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 頂点の最小値情報

	if (m_pVtxMin != NULL)
	{// メモリが確保されてる
		VtxMin = m_pVtxMin[nIdx];   // 保存された頂点の最小値情報を代入
	}

	return VtxMin;  // 頂点の最小値情報を返す
}

//=============================================================================
// モデルの総数取得
//=============================================================================
int CModelCreate::GetNumModel(void)
{
	return m_nNumModel;
}
