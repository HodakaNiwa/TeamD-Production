//=============================================================================
//
// オブジェクトX処理 [sceneX.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "sceneX.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
//=============================================================================
// コンストラクタ
//=============================================================================
CSceneX::CSceneX(int nPrioriry,OBJTYPE objtype) : CScene(nPrioriry,objtype)
{
	// 位置・向きの初期設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期設定
	m_pTexture = NULL;
}
//=============================================================================
// デストラクタ
//=============================================================================
CSceneX::~CSceneX()
{

}
//=============================================================================
// オブジェクトXの生成
//=============================================================================
CSceneX *CSceneX::Create(void)
{
	CSceneX *pSceneX = NULL;

	if (pSceneX == NULL)
	{
		pSceneX = new CSceneX;
		pSceneX->Init();
	}
	return pSceneX;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CSceneX::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//int nNumvtx;	//頂点数
	//DWORD sizeFVF;	//頂点フォーマットのサイズ
	//BYTE *pvtxBuff;	//頂点バッファへのポインタ


		////最大値の設定
		//m_vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

		////最小値の設定
		//m_vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);

		////頂点数を取得
		//nNumvtx = m_pMesh->GetNumVertices();

		////頂点フォーマットのサイズを取得
		//sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		////頂点バッファをロックし、頂点データへのポインタを取得
		//m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pvtxBuff);

		//for (int nCntvtx = 0; nCntvtx < nNumvtx; nCntvtx++)
		//{
		//	D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pvtxBuff;	//頂点座標の代入

		//												//比較
		//	if (vtx.x > m_vtxMax.x)
		//	{//
		//		m_vtxMax.x = vtx.x;
		//	}
		//	if (vtx.y > m_vtxMax.y)
		//	{
		//		m_vtxMax.y = vtx.y;
		//	}
		//	if (vtx.z > m_vtxMax.z)
		//	{
		//		m_vtxMax.z = vtx.z;
		//	}

		//	if (vtx.x < m_vtxMin.x)
		//	{
		//		m_vtxMin.x = vtx.x;
		//	}
		//	if (vtx.y < m_vtxMin.y)
		//	{
		//		m_vtxMin.y = vtx.y;
		//	}
		//	if (vtx.z < m_vtxMin.z)
		//	{
		//		m_vtxMin.z = vtx.z;
		//	}
		//	pvtxBuff += sizeFVF;		//サイズ分ポインタを進める
		//}
		////頂点バッファをアンロックする
		//m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneX::Uninit(void)
{
	if (m_pTexture != NULL)
	{//テクスチャがNULLでない場合(家)
		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{//マテリアルの数分繰り返し
			if (m_pTexture[nCntMat] != NULL)
			{//ポインタの中のNULLチェック(家具)
				m_pTexture[nCntMat]->Release();
				m_pTexture[nCntMat] = NULL;
			}
		}
		//メモリ開放(解体)
		delete[] m_pTexture;
		//NULLにする(更地)
		m_pTexture = NULL;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneX::Update(void)
{
#ifdef _DEBUG
	CDebugProc::Print(1, "モデルの位置  : x[%.1f],y[%.1f],z[%.1f]\n", m_pos.x, m_pos.y, m_pos.z);
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneX::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans,mtxScale;					//計算用マトリックス
	D3DMATERIAL9 matDef;									//現在のマテリアル保存用
	D3DXMATERIAL *pMat;										//マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 大きさの反映
	D3DXMatrixScaling(&mtxScale,
		m_size.x, m_size.y, m_size.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	if (GetObjType() == OBJTYPE_PLAYER)
	{
		pMat->MatD3D.Diffuse.a = 0.5f;
	}
	else if(GetObjType() != OBJTYPE_PLAYER)
	{
		pMat->MatD3D.Diffuse.a = 1.0f;
	}
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャ無効
			pDevice->SetTexture(0, m_pTexture[nCntMat]);
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// 位置の設置処理
//=============================================================================
void CSceneX::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 位置の設置処理
//=============================================================================
void CSceneX::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// 頂点座標最大値の設置処理
//=============================================================================
void CSceneX::SetVtxMax(D3DXVECTOR3 vtxMax)
{
	m_vtxMax = vtxMax;
}

//=============================================================================
// 頂点座標最小値の設置処理
//=============================================================================
void CSceneX::SetVtxMin(D3DXVECTOR3 vtxMin)
{
	m_vtxMin = vtxMin;
}


//=============================================================================
// 大きさの設置処理
//=============================================================================
void CSceneX::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
}

//=============================================================================
// 当たり判定の設置処理
//=============================================================================
void CSceneX::SetHit(bool bHit)
{
	m_bHit = bHit;
}

//=============================================================================
// 破壊できるかどうかの設置処理
//=============================================================================
void CSceneX::SetDestroy(bool bDestroy)
{
	m_bDestroy = bDestroy;
}

//=============================================================================
// 位置の取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 向きの取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// 大きさの取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetSize(void)
{
	return m_size;
}

//=============================================================================
// 頂点座標最大値の取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMax(void)
{
	return m_vtxMax;
}

//=============================================================================
// 頂点座標最小値の取得処理
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMin(void)
{
	return m_vtxMin;
}

//=============================================================================
// 当たり判定の取得処理
//=============================================================================
bool CSceneX::GetHit(void)
{
	return m_bHit;
}

//=============================================================================
// 破壊できるかどうかの取得処理
//=============================================================================
bool CSceneX::GetDestroy(void)
{
	return m_bDestroy;
}

//=============================================================================
// Xファイルの取得処理
//=============================================================================
void CSceneX::BindX(LPD3DXMESH mesh, LPD3DXBUFFER buffmat, DWORD nummat)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATERIAL *pMat;								//マテリアルデータへのポインタ

	m_pMesh = mesh;
	m_pBuffMat = buffmat;
	m_nNumMat = nummat;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	//テクスチャの数分を動的確保
	m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//テクスチャ情報を入れる前にNULLにする
		m_pTexture[nCntMat] = NULL;

		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_pTexture[nCntMat]);
		}
		//pMat->MatD3D.Diffuse.a = 0.5f;
	}
}