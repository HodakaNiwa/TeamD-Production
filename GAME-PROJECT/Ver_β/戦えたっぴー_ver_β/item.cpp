//*****************************************************************************
//
//     アイテムの処理[item.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "item.h"
#include "manager.h"
#include "boxCollider.h"
#include "renderer.h"
#include "basemode.h"
#include "game.h"
#include "player.h"

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
CItem::CItem(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
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
CItem::~CItem()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItem *pItem = NULL;      // アイテムクラス型のポインタ
	if (pItem == NULL)
	{// メモリが空になっている
		pItem = new CItem(nPriority);
		if (pItem != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pItem->SetPos(pos);                                     // 座標
			pItem->SetRot(rot);                                     // 向き
			pItem->SetType(nType);                                  // 種類番号
			pItem->SetModelIdx(nModelIdx);                          // 使用するモデルの番号
			pItem->SetAlpha(1.0f);                                  // モデルの透明度
			pItem->BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // モデル情報割り当て

			// 当たり判定用箱モデルを作成
			pItem->SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
			pItem->CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

			if (FAILED(pItem->Init()))
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

	return pItem;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CItem::Init(void)
{
	// アイテムのモデルを設定
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		pGame->SetItemModel(this, m_nType);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CItem::Uninit(void)
{
	// オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CItem::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CItem::Draw(void)
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
void CItem::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, bReturnFlag);
	SetBoxCollider(pBoxCollider);
}


//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CItem::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItem::Hit(CScene *pScene)
{
	Uninit();
}

//=============================================================================
//    モデル情報設定処理
//=============================================================================
void CItem::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
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
void CItem::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    使用するモデルの番号設定処理
//=============================================================================
void CItem::SetModelIdx(const int nModelIdx)
{
	m_nModelIdx = nModelIdx;
}

//=============================================================================
//    メッシュへのポインタ設定処理
//=============================================================================
void CItem::SetMesh(const LPD3DXMESH pMesh)
{
	m_pMesh = pMesh;
}

//=============================================================================
//    マテリアル情報へのポインタ設定処理
//=============================================================================
void CItem::SetBuffMat(const LPD3DXBUFFER pBuffMat)
{
	m_pBuffMat = pBuffMat;
}

//=============================================================================
//    マテリアル情報の数設定処理
//=============================================================================
void CItem::SetNumMat(const DWORD nNumMat)
{
	m_nNumMat = nNumMat;
}

//=============================================================================
//    テクスチャへのポインタ設定処理
//=============================================================================
void CItem::SetTexture(LPDIRECT3DTEXTURE9 *pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    最大の頂点座標設定処理
//=============================================================================
void CItem::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    最小の頂点座標設定処理
//=============================================================================
void CItem::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    モデルの透明度設定処理
//=============================================================================
void CItem::SetAlpha(const float fAlpha)
{
	m_fAlpha = fAlpha;
}

//=============================================================================
//    種類番号取得処理
//=============================================================================
int CItem::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    使用するモデルの番号取得処理
//=============================================================================
int CItem::GetModelIdx(void)
{
	return m_nModelIdx;
}

//=============================================================================
//    メッシュへのポインタ取得処理
//=============================================================================
LPD3DXMESH CItem::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    マテリアル情報へのポインタ取得処理
//=============================================================================
LPD3DXBUFFER CItem::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    マテリアル情報の数取得処理
//=============================================================================
DWORD CItem::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    テクスチャへのポインタ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 *CItem::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    最大の頂点座標取得処理
//=============================================================================
D3DXVECTOR3 CItem::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    最小の頂点座標取得処理
//=============================================================================
D3DXVECTOR3 CItem::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    モデルの透明度取得処理
//=============================================================================
float CItem::GetAlpha(void)
{
	return m_fAlpha;
}

//*****************************************************************************
//    CItemStarの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CItemStar::CItemStar(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CItemStar::~CItemStar()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CItemStar *CItemStar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemStar *pItemStar = NULL;      // アイテム(スター)クラス型のポインタ
	if (pItemStar == NULL)
	{// メモリが空になっている
		pItemStar = new CItemStar(nPriority);
		if (pItemStar != NULL)
		{// インスタンスを生成できた
			if (FAILED(pItemStar->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemStar;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CItemStar::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemStar::Uninit(void)
{
	// 共通の終了処理
	CItem::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CItemStar::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CItemStar::Draw(void)
{
	// 共通の描画処理
	CItem::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CItemStar::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemStar::Hit(CScene *pScene)
{
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// プレイヤークラスのポインタにキャスト
		CPlayer *pPlayer = (CPlayer*)pScene;

		// アイテムのイベントを起こす
		int nPlayerNumber = pPlayer->GetIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Star(nPlayerNumber);
		}

		// アイテムを消す
		Uninit();
	}
}

//*****************************************************************************
//    CItemGrenadeの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CItemGrenade::CItemGrenade(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CItemGrenade::~CItemGrenade()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CItemGrenade *CItemGrenade::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemGrenade *pItemGrenade = NULL;      // アイテム(手榴弾)クラス型のポインタ
	if (pItemGrenade == NULL)
	{// メモリが空になっている
		pItemGrenade = new CItemGrenade(nPriority);
		if (pItemGrenade != NULL)
		{// インスタンスを生成できた
			if (FAILED(pItemGrenade->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemGrenade;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CItemGrenade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemGrenade::Uninit(void)
{
	// 共通の終了処理
	CItem::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CItemGrenade::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CItemGrenade::Draw(void)
{
	// 共通の描画処理
	CItem::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CItemGrenade::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemGrenade::Hit(CScene *pScene)
{
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// アイテムのイベントを起こす
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Grenade();
		}

		// アイテムを消す
		Uninit();
	}
}

//*****************************************************************************
//    CItem1upの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CItem1up::CItem1up(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CItem1up::~CItem1up()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CItem1up *CItem1up::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItem1up *pItem1Up = NULL;      // アイテム(残機UP)クラス型のポインタ
	if (pItem1Up == NULL)
	{// メモリが空になっている
		pItem1Up = new CItem1up(nPriority);
		if (pItem1Up != NULL)
		{// インスタンスを生成できた
			if (FAILED(pItem1Up->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItem1Up;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CItem1up::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItem1up::Uninit(void)
{
	// 共通の終了処理
	CItem::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CItem1up::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CItem1up::Draw(void)
{
	// 共通の描画処理
	CItem::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CItem1up::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItem1up::Hit(CScene *pScene)
{
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// プレイヤークラスのポインタにキャスト
		CPlayer *pPlayer = (CPlayer*)pScene;

		// アイテムのイベントを起こす
		int nPlayerNumber = pPlayer->GetIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_1Up(nPlayerNumber);
		}

		// アイテムを消す
		Uninit();
	}
}

//*****************************************************************************
//    CItemScoopの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CItemScoop::CItemScoop(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CItemScoop::~CItemScoop()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CItemScoop *CItemScoop::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemScoop *pItemScoop = NULL;      // アイテム(スコップ)クラス型のポインタ
	if (pItemScoop == NULL)
	{// メモリが空になっている
		pItemScoop = new CItemScoop(nPriority);
		if (pItemScoop != NULL)
		{// インスタンスを生成できた
			if (FAILED(pItemScoop->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemScoop;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CItemScoop::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemScoop::Uninit(void)
{
	// 共通の終了処理
	CItem::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CItemScoop::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CItemScoop::Draw(void)
{
	// 共通の描画処理
	CItem::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CItemScoop::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemScoop::Hit(CScene *pScene)
{
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// アイテムのイベントを起こす
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Scoop();
		}

		// アイテムを消す
		Uninit();
	}
}

//*****************************************************************************
//    CItemClockの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CItemClock::CItemClock(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CItemClock::~CItemClock()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CItemClock *CItemClock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemClock *pItemClock = NULL;      // アイテム(時計)クラス型のポインタ
	if (pItemClock == NULL)
	{// メモリが空になっている
		pItemClock = new CItemClock(nPriority);
		if (pItemClock != NULL)
		{// インスタンスを生成できた
			if (FAILED(pItemClock->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemClock;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CItemClock::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemClock::Uninit(void)
{
	// 共通の終了処理
	CItem::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CItemClock::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CItemClock::Draw(void)
{
	// 共通の描画処理
	CItem::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CItemClock::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemClock::Hit(CScene *pScene)
{
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// アイテムのイベントを起こす
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Clock();
		}

		// アイテムを消す
		Uninit();
	}
}

//*****************************************************************************
//    CItemHelmetの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CItemHelmet::CItemHelmet(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CItemHelmet::~CItemHelmet()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CItemHelmet *CItemHelmet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemHelmet *pItemHelmet = NULL;      // アイテム(ヘルメット)クラス型のポインタ
	if (pItemHelmet == NULL)
	{// メモリが空になっている
		pItemHelmet = new CItemHelmet(nPriority);
		if (pItemHelmet != NULL)
		{// インスタンスを生成できた
			if (FAILED(pItemHelmet->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemHelmet;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CItemHelmet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemHelmet::Uninit(void)
{
	// 共通の終了処理
	CItem::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CItemHelmet::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CItemHelmet::Draw(void)
{
	// 共通の描画処理
	CItem::Draw();
}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CItemHelmet::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemHelmet::Hit(CScene *pScene)
{
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// プレイヤークラスのポインタにキャスト
		CPlayer *pPlayer = (CPlayer*)pScene;

		// アイテムのイベントを起こす
		int nPlayerNumber = pPlayer->GetIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Helmet(nPlayerNumber);
		}

		// アイテムを消す
		Uninit();
	}
}