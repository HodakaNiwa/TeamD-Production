//*****************************************************************************
//
//     モデルの処理[model.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CModel::CModel()
{
	// 各種値のクリア
	m_pTexture = NULL;                        // テクスチャへのポインタ
	m_pMesh = NULL;                           // メッシュ情報へのポインタ
	m_pBuffMat = NULL;                        // マテリアル情報へのポインタ
	m_nNumMat = 0;                            // マテリアル情報の数
	m_Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // スケール倍率
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 現在の座標
	m_AddPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 現在の座標に加える値
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 向き
	m_pParent = NULL;                         // 親モデルへのポインタ
	D3DXMatrixIdentity(&m_MtxWorld);          // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	CModel *pModel = NULL;  // モデルクラス型のポインタ
	if (pModel == NULL)
	{// メモリが空になっている
		pModel = new CModel;
		if (pModel != NULL)
		{// インスタンスを生成できた
			if (FAILED(pModel->Init(pos, rot, pMesh, pBuffMat, nNumMat, pTexture, VtxMax, VtxMin)))
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

	return pModel;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	// レンダリングクラスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
		 // 各種値の設定
			m_Pos = pos;           // 座標
			m_Rot = rot;           // 向き
			m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			BindModel(pMesh, pBuffMat, nNumMat, VtxMax, VtxMin, pTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CModel::Uninit(void)
{
	// ポインタを空にしておく
	// メッシュ情報
	if (m_pMesh != NULL)
	{
		m_pMesh = NULL;
	}
	// マテリアル情報へのポインタ
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat = NULL;
	}
	// テクスチャへのポインタ
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}
	// 親モデルへのポインタ
	if (m_pParent != NULL)
	{
		m_pParent = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CModel::Draw(void)
{
	D3DMATERIAL9 matDef;   // 現在のマテリアル保存用
	D3DXMATERIAL *pMat;    // マテリアルデータへのポインタ

						   // レンダリングクラスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
		 // ワールドマトリックス設定処理
			SetMtxWorld(pDevice);

			// 法線情報を正規化する(スケーリング時に法線情報がいじられるので)
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			if (m_pBuffMat != NULL && m_pMesh != NULL)
			{// Xファイルからモデルデータが読み込めている
			 // マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
				{// 設定されていたマテリアルの数だけ繰り返し
				 // マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					if (m_pTexture != NULL)
					{// テクスチャ情報が引き出せている
					 // テクスチャの設定
						pDevice->SetTexture(0, m_pTexture[nCntMat]);
					}
					else
					{// テクスチャ情報が引き出せていない
					 // テクスチャの設定
						pDevice->SetTexture(0, NULL);
					}

					// モデル(パーツ)の描画
					m_pMesh->DrawSubset(nCntMat);
				}
			}

			// 保存していたマテリアルに戻す
			pDevice->SetMaterial(&matDef);

			// 法線を正規化しない
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
		}
	}
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CModel::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxParent; // 計算用マトリックス

	 // ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 大きさを反映
	m_MtxWorld._11 = m_Scale.x;
	m_MtxWorld._22 = m_Scale.y;
	m_MtxWorld._33 = m_Scale.z;

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x + m_AddPos.x, m_Pos.y + m_AddPos.y, m_Pos.z + m_AddPos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// 親モデルのワールドマトリックスを掛け合わせる
	if (m_pParent != NULL)
	{// 親モデルが存在する
		mtxParent = m_pParent->GetMtxWorld();            // 親モデルからワールドマトリックスを取得する
	}
	else
	{// 親モデルが存在しない
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);  // デバイスからワールドマトリックスを取得する
	}
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    モデル情報設定処理
//=============================================================================
void CModel::BindModel(const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat, const D3DXVECTOR3 VtxMax, const D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture)
{
	// モデル情報の割り当て
	if (pMesh != NULL)
	{// メッシュ情報が読み込めた
		m_pMesh = pMesh;          // メッシュ情報へのポインタ
	}
	if (pBuffMat != NULL)
	{// マテリアル情報へのポインタを読み込めた
		m_pBuffMat = pBuffMat;    // マテリアル情報へのポインタ
	}
	if (nNumMat != -1)
	{// マテリアルの数情報が読み込めた
		m_nNumMat = nNumMat;      // マテリアル情報の数
	}
	if (pTexture != NULL)
	{// テクスチャへのポインタ情報が読み込めた
		m_pTexture = pTexture;    // テクスチャへのポインタ情報
	}
	m_VtxMax = VtxMax;            // 頂点の最大値情報
	m_VtxMin = VtxMin;            // 頂点の最小値情報
}

//=============================================================================
//    モデルの値をコピーする
//=============================================================================
void CModel::Cpy(CModel *pModel)
{
	if (pModel == NULL) return;

	SetPos(pModel->GetPos());
	SetRot(pModel->GetRot());
	SetScale(pModel->GetScale());
	SetAddPos(pModel->GetAddPos());
	SetMtxWorld(pModel->GetMtxWorld());
	SetParent(pModel->GetParent());
	BindModel(pModel->GetMesh(), pModel->GetBuffMat(), pModel->GetNumMat(),
		pModel->GetVtxMax(), pModel->GetVtxMin(), pModel->GetTexture());
}

//=============================================================================
//    モデルの透明度設定処理
//=============================================================================
void CModel::SetAlpha(float fAlpha)
{
	LPD3DXBUFFER pBuffMat = GetBuffMat();  // マテリアル情報へのポインタ
	DWORD nNumMat = GetNumMat();           // マテリアル情報の数
	D3DXMATERIAL *pMat;                    // マテリアルデータへのポインタ

	if (pBuffMat != NULL)
	{// マテリアル情報へのポインタが取得できた
	 // マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
		{// 設定されていたマテリアルの数だけ繰り返し
			pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;
		}
	}
}

//=============================================================================
//    テクスチャへのポインタ設定処理
//=============================================================================
void CModel::BindTexture(LPDIRECT3DTEXTURE9 *pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    メッシュ情報へのポインタ設定処理
//=============================================================================
void CModel::SetMesh(const LPD3DXMESH pMesh)
{
	m_pMesh = pMesh;
}

//=============================================================================
//    マテリアル情報へのポインタ設定処理
//=============================================================================
void CModel::SetBuffMat(const LPD3DXBUFFER pBuffMat)
{
	m_pBuffMat = pBuffMat;
}

//=============================================================================
//    マテリアル情報の数設定処理
//=============================================================================
void CModel::SetNumMat(const DWORD nNumMat)
{
	m_nNumMat = nNumMat;
}

//=============================================================================
//    メッシュの頂点座標最小値設定処理
//=============================================================================
void CModel::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    メッシュの頂点座標最大値設定処理
//=============================================================================
void CModel::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    スケールの倍率設定処理
//=============================================================================
void CModel::SetScale(const D3DXVECTOR3 Scale)
{
	m_Scale = Scale;
}

//=============================================================================
//    現在の座標設定処理
//=============================================================================
void CModel::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    現在の座標に加える値設定処理
//=============================================================================
void CModel::SetAddPos(const D3DXVECTOR3 Addpos)
{
	m_AddPos = Addpos;
}

//=============================================================================
//    向き設定処理
//=============================================================================
void CModel::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CModel::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    親モデルへのポインタ設定処理
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}

//=============================================================================
//   メッシュの頂点座標最小値取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    メッシュの頂点座標最大値取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    スケールの倍率取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetScale(void)
{
	return m_Scale;
}

//=============================================================================
//    テクスチャへのポインタ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 *CModel::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    現在の座標取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    現在の座標に加える値取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetAddPos(void)
{
	return m_AddPos;
}

//=============================================================================
//    向き取得処理
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_MtxWorld;
}

D3DXMATRIX *CModel::GetMtxWorldPointer(void)
{
	return &m_MtxWorld;
}

//=============================================================================
//    メッシュ情報取得処理
//=============================================================================
LPD3DXMESH CModel::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    マテリアル情報取得処理
//=============================================================================
LPD3DXBUFFER CModel::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    マテリアルの数取得処理
//=============================================================================
DWORD CModel::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    親モデルへのポインタ取得処理
//=============================================================================
CModel *CModel::GetParent(void)
{
	return m_pParent;
}