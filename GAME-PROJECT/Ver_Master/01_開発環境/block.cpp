//*****************************************************************************
//
//     ブロックの処理[block.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "boxCollider.h"
#include "splash.h"
#include "game.h"
#include "charaselect.h"
#include "tutorial.h"
#include "bullet.h"
#include "effectManager.h"
#include "map.h"
#include "modelcreate.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define BLOCK_EFFECT_POSUP                (30.0f)  // エフェクトをどれくらい高くして生成するか
#define BLOCK_DESTROY_EFFECT_IDX_BLOSSOMS (6)      // ブロック破壊時のエフェクト番号(桜の場合)
#define BLOCK_UNINIT_TIMER				  (1200)   // 終了するまでのカウンター
#define BLOCK_UNINIT_SIGN				  (1000)   // 終了の点滅サイン

// 飛沫用
#define BLOCK_SPLASH_PRIORITY             (5)

//*****************************************************************************
//    静的メンバ変数
//*****************************************************************************
int CBlock::m_nNumAll = 0;

//*****************************************************************************
//    CBlockの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CBlock::CBlock(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	// 各種値のクリア
	m_nType = 0;
	m_pModel = NULL;
	m_fAlpha = 0.0f;
	m_nModelIdx = 0;
	m_nIdx = 0;
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
	// 番号の設定
	SetIdx(m_nNumAll);

	// 総数を増やす
	m_nNumAll++;

	// 種類の設置処理
	SetObjType(OBJTYPE_BLOCK);

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBlock::Uninit(void)
{
	// 総数を減らす
	m_nNumAll--;

	// モデルの開放
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// いなくなったことを伝える
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CManager::GetGame()->DeleteBlock(m_nIdx);
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		CManager::GetTutorial()->DeleteBlock(m_nIdx);
	}

	// エフェクトを出す
	D3DXVECTOR3 EffectPos = GetPos();
	EffectPos.y += BLOCK_EFFECT_POSUP;
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		switch (CCharaSelect::GetStageType())
		{// ステージの種類によって処理わけ
		case CGame::MAPTYPE_HINAMATSURI:  // ひな祭りステージ
			pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, BLOCK_DESTROY_EFFECT_IDX_BLOSSOMS);
			break;
		}
	}

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

			// モデルの描画
			if (m_pModel != NULL)
			{
				m_pModel->SetAlpha(m_fAlpha);
				m_pModel->Draw();
				m_pModel->SetAlpha(1.0f);
			}
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
	m_pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, pMesh, pBuffMat, nNumMat, pTexture);
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
//    モデルの透明度設定処理
//=============================================================================
void CBlock::SetAlpha(const float fAlpha)
{
	m_fAlpha = fAlpha;
}

//=============================================================================
//    番号の設定処理
//=============================================================================
void CBlock::SetIdx(const int nIdx)
{
	m_nIdx = nIdx;
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
//    モデルの透明度取得処理
//=============================================================================
float CBlock::GetAlpha(void)
{
	return m_fAlpha;
}

//=============================================================================
//    番号の取得処理
//=============================================================================
int CBlock::GetIdx(void)
{
	return m_nIdx;
}

//=============================================================================
//    総数の取得処理
//=============================================================================
int CBlock::GetNumAll(void)
{
	return m_nNumAll;
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
	// 共通の初期化処理
	if (FAILED(CBlock::Init()))
	{
		return E_FAIL;
	}

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
	// 共通の初期化処理
	if (FAILED(CBlock::Init()))
	{
		return E_FAIL;
	}

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
	// クリーム靄を出す
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム画面だったら
		CGame *pGame = CManager::GetGame();
		if (pGame == NULL) { return; }
		pGame->CreateMist();
	}

	// 飛沫ポリゴン出す
	CSplash::Create(D3DXVECTOR3(30.0f + (float)(rand() % 350), 35.0f + (float)(rand() % 200), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(0.1f, -1.0f, 0.0f), 0.0f, 0.005f, 125.0f, -1.0f, 360, BLOCK_SPLASH_PRIORITY);
	CSplash::Create(D3DXVECTOR3(45.0f + (float)(rand() % 350), 495.0f + (float)(rand() % 200), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(0.1f, -1.0f, 0.0f), 0.0f, 0.005f, 115.0f, -1.0f, 360, BLOCK_SPLASH_PRIORITY);
	CSplash::Create(D3DXVECTOR3(690.0f + (float)(rand() % 350), 45.0f + (float)(rand() % 200), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(0.1f, -1.0f, 0.0f), 0.0f, 0.005f, 140.0f, -1.0f, 360, BLOCK_SPLASH_PRIORITY);
	CSplash::Create(D3DXVECTOR3(810.0f + (float)(rand() % 350), 505.0f + (float)(rand() % 200), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(0.1f, -1.0f, 0.0f), 0.0f, 0.005f, 105.0f, -1.0f, 360, BLOCK_SPLASH_PRIORITY);
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
	// 共通の初期化処理
	if (FAILED(CBlock::Init()))
	{
		return E_FAIL;
	}

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
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// 弾だった
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_ENEMY) { return; }

		// プレイヤーの弾の場合最大強化時ならば壊れるようにする
		CPlayer *pPlayer = (CPlayer*)pBullet->GetParent();
		if (pPlayer == NULL || pPlayer->GetObjType() != OBJTYPE_PLAYER) { return; }
		if (pPlayer->GetAllBlockDestroy() == true)
		{
			Uninit();
		}
	}
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
	// 共通の初期化処理
	if (FAILED(CBlock::Init()))
	{
		return E_FAIL;
	}

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
	else if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// 弾だった
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_ENEMY) { return; }

		// プレイヤーの弾の場合最大強化時ならば壊れるようにする
		CPlayer *pPlayer = (CPlayer*)pBullet->GetParent();
		if (pPlayer == NULL || pPlayer->GetObjType() != OBJTYPE_PLAYER) { return;}
		if (pPlayer->GetAllBlockDestroy() == true)
		{
			Uninit();
		}
	}
}

//*****************************************************************************
//    CBlockScoopの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CBlockScoop::CBlockScoop(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{
	m_nUninitTimer = 0;		//終了するまでのカウンター
	m_bUninitSign = false;	//終了の点滅サイン
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBlockScoop::~CBlockScoop()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBlockScoop *CBlockScoop::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockScoop *pBlockScoop = NULL;      // タイプ2のブロッククラス型のポインタ
	if (pBlockScoop == NULL)
	{// メモリが空になっている
		pBlockScoop = new CBlockScoop(nPriority);
		if (pBlockScoop != NULL)
		{// インスタンスを生成できた
			if (FAILED(pBlockScoop->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockScoop;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBlockScoop::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// 共通の初期化処理
	if (FAILED(CBlock::Init()))
	{
		return E_FAIL;
	}

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

	m_nUninitTimer = 0;								 // 終了するまでのカウンター
	m_bUninitSign = false;							 // 終了の点滅サイン
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBlockScoop::Uninit(void)
{
	// 共通の終了処理
	CBlock::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CBlockScoop::Update(void)
{
	//終了するまでのカウンターの加算
	m_nUninitTimer++;

	if (m_nUninitTimer == BLOCK_UNINIT_SIGN)
	{
		//ブロックの生成
		CreateBlock();
	}
	if (m_nUninitTimer >= BLOCK_UNINIT_SIGN)
	{
		m_bUninitSign = true;
	}
	if (m_nUninitTimer >= BLOCK_UNINIT_TIMER)
	{//終了するまでのカウンターが一定数を超えた場合
		//終了処理
		Uninit();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CBlockScoop::Draw(void)
{
	if (m_bUninitSign == false)
	{//終了の点滅サインがfalseの場合
	 // 共通の描画処理
		CBlock::Draw();
	}
	else if (m_bUninitSign == true)
	{//終了の点滅サインがfalseの場合
		if (m_nUninitTimer % 4 != 0)
		{
			// 共通の描画処理
			CBlock::Draw();
		}
	}

}

//=============================================================================
//    当たり判定用箱モデルを作り直す処理
//=============================================================================
void CBlockScoop::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CBlockScoop::Hit(CScene *pScene)
{
	// 当たってきたオブジェクトの種類で処理わけ
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// 弾だった
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_ENEMY) { return; }

		// プレイヤーの弾の場合最大強化時ならば壊れるようにする
		CPlayer *pPlayer = (CPlayer*)pBullet->GetParent();
		if (pPlayer == NULL || pPlayer->GetObjType() != OBJTYPE_PLAYER) { return; }
		if (pPlayer->GetAllBlockDestroy() == true)
		{
			Uninit();
		}
	}
}

//=============================================================================
//    ブロックの生成処理
//=============================================================================
void CBlockScoop::CreateBlock(void)
{
	//マップの取得
	CMap *pMap = CManager::GetBaseMode()->GetMap();

	//ゲームの取得
	CGame *pGame = CManager::GetGame();

	D3DXVECTOR3 BlockPos = GetPos();
	if (pGame->GetStageIdx() == 0)
	{//ステージ0の場合
		CBlockType0::Create(D3DXVECTOR3(BlockPos.x, 0.0f, BlockPos.z), INITIALIZE_D3DXVECTOR3,
			0, 0, pMap->GetModelCreate()->GetMesh(7), pMap->GetModelCreate()->GetBuffMat(7), pMap->GetModelCreate()->GetNumMat(7), pMap->GetModelCreate()->GetTexture(7)
			, MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_SIZE_Z_HALF);
	}
	else if (pGame->GetStageIdx() == 1 ||
		pGame->GetStageIdx() == 2)
	{//ステージ1～2の場合
		CBlockType0::Create(D3DXVECTOR3(BlockPos.x, 0.0f, BlockPos.z), INITIALIZE_D3DXVECTOR3,
			0, 0, pMap->GetModelCreate()->GetMesh(6), pMap->GetModelCreate()->GetBuffMat(6), pMap->GetModelCreate()->GetNumMat(6), pMap->GetModelCreate()->GetTexture(6)
			, MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_SIZE_Z_HALF);
	}
}