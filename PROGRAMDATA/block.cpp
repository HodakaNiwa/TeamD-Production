//*****************************************************************************
//
//     ブロックの処理[block.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "block.h"
#include "manager.h"
#include "boxCollider.h"
#include "renderer.h"
#include "splash.h"
#include "basemode.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数
//*****************************************************************************


//*****************************************************************************
//    CBlockの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CBlock::CBlock(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	// 各種値のクリア
	m_pMesh = NULL;                      // メッシュへのポインタ
	m_pBuffMat = NULL;                   // マテリアル情報へのポインタ
	m_nNumMat = 0;                       // マテリアル情報の数
	m_VtxMax = INITIALIZE_D3DXVECTOR3;   // 最大の頂点座標
	m_VtxMin = INITIALIZE_D3DXVECTOR3;   // 最小の頂点座標
	m_fAlpha = 0.0f;                     // モデルの透明度
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBlock::~CBlock()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlock *pBlock = NULL;      // ブロッククラス型のポインタ
	if (pBlock == NULL)
	{// メモリが空になっている
		pBlock = new CBlock(nPriority);
		if (pBlock != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pBlock->SetPos(pos);                                     // 座標
			pBlock->SetRot(rot);                                     // 向き
			pBlock->SetType(nType);                                  // 種類番号
			pBlock->SetModelIdx(nModelIdx);                          // 使用するモデルの番号
			pBlock->SetAlpha(1.0f);                                  // モデルの透明度
			pBlock->BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // モデル情報割り当て

			// 当たり判定用箱モデルを作成
			pBlock->SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
			pBlock->CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

			if (FAILED(pBlock->Init()))
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

	return pBlock;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBlock::Init(void)
{
	// 種類の設置処理
	SetObjType(OBJTYPE_BLOCK);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBlock::Uninit(void)
{
	// オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CBlock::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBlock::Draw(void)
{
	D3DMATERIAL9 matDef;     // 現在のマテリアル保存用
	D3DXMATERIAL *pMat;      // マテリアルデータへのポインタ
	float fAlphaDef = 0.0f;  // デフォルトの透明度

	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // ワールドマトリックス設定処理
			SetMtxWorld(pDevice);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			if (m_pBuffMat != NULL && m_pMesh != NULL)
			{// Xファイルからモデルデータが読み込めている
			    // マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
				{// 設定されていたマテリアルの数だけ繰り返し
				    // 透明度の設定
					fAlphaDef = pMat[nCntMat].MatD3D.Diffuse.a;
					pMat[nCntMat].MatD3D.Diffuse.a = m_fAlpha;

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

					// 透明度を戻す
					pMat[nCntMat].MatD3D.Diffuse.a = fAlphaDef;
				}
			}

			// 保存していたマテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
	}

	// 3Dオブジェクトの描画
	CObject3D::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作成する処理
//=============================================================================
void CBlock::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, bReturnFlag);
	SetBoxCollider(pBoxCollider);
}


//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CBlock::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// 現在の当たり判定用箱モデルを開放する
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// 当たり判定用箱モデルを生成しなおす
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    オブジェクトに当たったときの処理
//=============================================================================
void CBlock::Hit(CScene *pScene)
{
	Uninit();
}

//=============================================================================
//    モデル情報設定処理
//=============================================================================
void CBlock::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = nNumMat;
	m_pTexture = pTexture;
	m_VtxMax = VtxMax;
	m_VtxMin = VtxMin;
}

//=============================================================================
//    種類番号設定処理
//=============================================================================
void CBlock::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    使用するモデルの番号設定処理
//=============================================================================
void CBlock::SetModelIdx(const int nModelIdx)
{
	m_nModelIdx = nModelIdx;
}

//=============================================================================
//    メッシュへのポインタ設定処理
//=============================================================================
void CBlock::SetMesh(const LPD3DXMESH pMesh)
{
	m_pMesh = pMesh;
}

//=============================================================================
//    マテリアル情報へのポインタ設定処理
//=============================================================================
void CBlock::SetBuffMat(const LPD3DXBUFFER pBuffMat)
{
	m_pBuffMat = pBuffMat;
}

//=============================================================================
//    マテリアル情報の数設定処理
//=============================================================================
void CBlock::SetNumMat(const DWORD nNumMat)
{
	m_nNumMat = nNumMat;
}

//=============================================================================
//    テクスチャへのポインタ設定処理
//=============================================================================
void CBlock::SetTexture(LPDIRECT3DTEXTURE9 *pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    最大の頂点座標設定処理
//=============================================================================
void CBlock::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    最小の頂点座標設定処理
//=============================================================================
void CBlock::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    モデルの透明度設定処理
//=============================================================================
void CBlock::SetAlpha(const float fAlpha)
{
	m_fAlpha = fAlpha;
}

//=============================================================================
//    種類番号取得処理
//=============================================================================
int CBlock::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    使用するモデルの番号取得処理
//=============================================================================
int CBlock::GetModelIdx(void)
{
	return m_nModelIdx;
}

//=============================================================================
//    メッシュへのポインタ取得処理
//=============================================================================
LPD3DXMESH CBlock::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    マテリアル情報へのポインタ取得処理
//=============================================================================
LPD3DXBUFFER CBlock::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    マテリアル情報の数取得処理
//=============================================================================
DWORD CBlock::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    テクスチャへのポインタ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 *CBlock::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    最大の頂点座標取得処理
//=============================================================================
D3DXVECTOR3 CBlock::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    最小の頂点座標取得処理
//=============================================================================
D3DXVECTOR3 CBlock::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    モデルの透明度取得処理
//=============================================================================
float CBlock::GetAlpha(void)
{
	return m_fAlpha;
}

//*****************************************************************************
//    CBlockType0の処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CBlockType0::CBlockType0(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CBlockType0::~CBlockType0()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBlockType0 *CBlockType0::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType0 *pBlockType0 = NULL;      // タイプ0のブロッククラス型のポインタ
	if (pBlockType0 == NULL)
	{// メモリが空になっている
		pBlockType0 = new CBlockType0(nPriority);
		if (pBlockType0 != NULL)
		{// インスタンスを生成できた
			if (FAILED(pBlockType0->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType0;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBlockType0::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// 各種値の設定
	SetPos(pos);                                     // 座標
	SetRot(rot);                                     // 向き
	SetType(nType);                                  // 種類番号
	SetModelIdx(nModelIdx);                          // 使用するモデルの番号
	SetAlpha(1.0f);                                  // モデルの透明度
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // モデル情報割り当て

	// 当たり判定用箱モデルを作成
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth, true);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBlockType0::Uninit(void)
{
	// 共通の終了処理
	CBlock::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CBlockType0::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBlockType0::Draw(void)
{
	// 共通の描画処理
	CBlock::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CBlockType0::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// 現在の当たり判定用箱モデルを開放する
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// 当たり判定用箱モデルを生成しなおす
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    当たったときの処理
//=============================================================================
void CBlockType0::Hit(CScene *pScene)
{
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// 弾だった
		Uninit();
	}
}

//*****************************************************************************
//    CBlockType1の処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CBlockType1::CBlockType1(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CBlockType1::~CBlockType1()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBlockType1 *CBlockType1::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType1 *pBlockType1 = NULL;      // タイプ1のブロッククラス型のポインタ
	if (pBlockType1 == NULL)
	{// メモリが空になっている
		pBlockType1 = new CBlockType1(nPriority);
		if (pBlockType1 != NULL)
		{// インスタンスを生成できた
			if (FAILED(pBlockType1->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType1;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBlockType1::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// 各種値の設定
	SetPos(pos);                                     // 座標
	SetRot(rot);                                     // 向き
	SetType(nType);                                  // 種類番号
	SetModelIdx(nModelIdx);                          // 使用するモデルの番号
	SetAlpha(1.0f);                                  // モデルの透明度
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // モデル情報割り当て

	// 当たり判定用箱モデルを作成
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth, false);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBlockType1::Uninit(void)
{
	// 共通の終了処理
	CBlock::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CBlockType1::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBlockType1::Draw(void)
{
	// 共通の描画処理
	CBlock::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CBlockType1::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// 現在の当たり判定用箱モデルを開放する
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// 当たり判定用箱モデルを生成しなおす
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth, false);
}

//=============================================================================
//    当たったときの処理
//=============================================================================
void CBlockType1::Hit(CScene *pScene)
{
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{// プレイヤーだった
		// 飛沫ポリゴン生成
		CreateSplash();

		// 終了処理
		Uninit();
	}
	else if (pScene->GetObjType() == OBJTYPE_ENEMY)
	{// 敵だった
		// 敵AIパターン変化

		// 終了処理
		Uninit();
	}
}

//=============================================================================
//    飛沫ポリゴンを生成する処理
//=============================================================================
void CBlockType1::CreateSplash(void)
{
	CSplash::Create(D3DXVECTOR3(130.0f, 260.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(1.0f, -1.0f, 0.0f), 0.0f, 0.005f, 250.0f, -1.0f, 180);
	CSplash::Create(D3DXVECTOR3(350.0f, 490.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(1.0f, -1.0f, 0.0f), 0.0f, 0.005f, 240.0f, -1.0f, 180);
	CSplash::Create(D3DXVECTOR3(600.0f, 160.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(1.0f, -1.0f, 0.0f), 0.0f, 0.005f, 265.0f, -1.0f, 180);
	CSplash::Create(D3DXVECTOR3(890.0f, 560.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(1.0f, -1.0f, 0.0f), 0.0f, 0.005f, 230.0f, -1.0f, 180);
}

//*****************************************************************************
//    CBlockType2の処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CBlockType2::CBlockType2(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CBlockType2::~CBlockType2()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBlockType2 *CBlockType2::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType2 *pBlockType2 = NULL;      // タイプ2のブロッククラス型のポインタ
	if (pBlockType2 == NULL)
	{// メモリが空になっている
		pBlockType2 = new CBlockType2(nPriority);
		if (pBlockType2 != NULL)
		{// インスタンスを生成できた
			if (FAILED(pBlockType2->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType2;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBlockType2::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// 各種値の設定
	SetPos(pos);                                     // 座標
	SetRot(rot);                                     // 向き
	SetType(nType);                                  // 種類番号
	SetModelIdx(nModelIdx);                          // 使用するモデルの番号
	SetAlpha(1.0f);                                  // モデルの透明度
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // モデル情報割り当て

	// 当たり判定用箱モデルを作成
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBlockType2::Uninit(void)
{
	// 共通の終了処理
	CBlock::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CBlockType2::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBlockType2::Draw(void)
{
	// 共通の描画処理
	CBlock::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CBlockType2::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// 現在の当たり判定用箱モデルを開放する
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// 当たり判定用箱モデルを生成しなおす
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    当たったときの処理
//=============================================================================
void CBlockType2::Hit(CScene *pScene)
{

}

//*****************************************************************************
//    CBlockType3の処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CBlockType3::CBlockType3(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CBlockType3::~CBlockType3()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBlockType3 *CBlockType3::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType3 *pBlockType3 = NULL;      // タイプ3のブロッククラス型のポインタ
	if (pBlockType3 == NULL)
	{// メモリが空になっている
		pBlockType3 = new CBlockType3(nPriority);
		if (pBlockType3 != NULL)
		{// インスタンスを生成できた
			if (FAILED(pBlockType3->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType3;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBlockType3::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// 各種値の設定
	SetPos(pos);                                     // 座標
	SetRot(rot);                                     // 向き
	SetType(nType);                                  // 種類番号
	SetModelIdx(nModelIdx);                          // 使用するモデルの番号
	SetAlpha(1.0f);                                  // モデルの透明度
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // モデル情報割り当て

	// 当たり判定用箱モデルを作成
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBlockType3::Uninit(void)
{
	// 共通の終了処理
	CBlock::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CBlockType3::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBlockType3::Draw(void)
{
	// 共通の描画処理
	CBlock::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CBlockType3::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// 現在の当たり判定用箱モデルを開放する
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// 当たり判定用箱モデルを生成しなおす
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    当たったときの処理
//=============================================================================
void CBlockType3::Hit(CScene *pScene)
{
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{// プレイヤーだった
		// 飛沫ポリゴン生成
	}
	else if (pScene->GetObjType() == OBJTYPE_ENEMY)
	{// 敵だった
		// 敵AIパターン変化
	}
}