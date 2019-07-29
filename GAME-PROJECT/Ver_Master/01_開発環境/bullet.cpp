//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "modelcreate.h"
#include "boxCollider.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "headquarters.h"
#include "server.h"
#include "game.h"
#include "effectManager.h"
#include "title.h"
#include "tutorial.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_EFFECT_IDX (2)

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CBulletPlayer::m_nNumAll_0 = 0;
int CBulletPlayer::m_nNumAll_1 = 0;
int CBulletEnemy::m_nNumAll = 0;

//*****************************************************************************
// CBulletの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	m_pModel = NULL;
	m_pParent = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(void)
{
	// ボックスコライダー生成
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), GetColRange().x, GetColRange().y, GetColRange().z, false);
	SetBoxCollider(pBoxCollider);

	// 弾のモデルを設定
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		pGame->SetBulletModel(this);
	}
	else if(CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		CTutorial *pTutorial = CManager::GetTutorial();
		pTutorial->SetBulletModel(this);
	}

	// エフェクトを出す
	D3DXVECTOR3 rot = INITIALIZE_D3DXVECTOR3;
	if (m_Move.x > 0.0f)
	{// 右側に動いている
		rot.z = D3DX_PI * 0.5f;
	}
	else if(m_Move.x < 0.0f)
	{// 左側に動いている
		rot.z = -D3DX_PI * 0.5f;
	}
	else if (m_Move.z > 0.0f)
	{// 奥側に動いている
		rot.x = -D3DX_PI * 0.5f;
	}
	else if(m_Move.z < 0.0f)
	{// 手前側に動いている
		rot.x = D3DX_PI * 0.5f;
	}

	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(GetPos() + m_Move, rot, BULLET_EFFECT_IDX);
		pEffectManager->SetEffect(GetPos() - (m_Move / 2) + m_Move, rot, BULLET_EFFECT_IDX);
		pEffectManager->SetEffect(GetPos() - m_Move + (m_Move * 2), rot, BULLET_EFFECT_IDX);
		pEffectManager->SetEffect(GetPos() + (m_Move / 2) + m_Move, rot, BULLET_EFFECT_IDX);
		pEffectManager->SetEffect(GetPos() + m_Move + m_Move, rot, BULLET_EFFECT_IDX);
	}
	SetPosOld(GetPos());

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	// モデルの終了処理
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// 終了処理
	CObject3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	// 過去の位置設置処理
	SetPosOld(GetPos());

	if (CManager::GetClient() != NULL)
	{
		if (CManager::GetClient()->GetConnected() == true && CManager::GetClient()->GetClientId() == m_Type)
		{
			// 移動処理
			Move();
		}
	}
	else
	{
		// 移動処理
		Move();
	}

	// 当たり判定処理
	Collision();
}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	// レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ワールドマトリックスの設定処理
	SetMtxWorld(pDevice);

	// モデルの描画処理
	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}

	// 共通の描画処理
	CObject3D::Draw();
}

//=============================================================================
// 弾の移動処理
//=============================================================================
void CBullet::Move(void)
{
	D3DXVECTOR3 pos = GetPos();

	// 移動させる
	pos += m_Move;

	// 当たり判定用箱モデルの位置も設定する
	CBoxCollider *pCollider = NULL;
	pCollider = CObject3D::GetBoxCollider();
	if (pCollider != NULL)
	{
		pCollider->SetPos(pos);
	}

	// 座標の設定
	SetPos(pos);
}

//=============================================================================
// ボックスコライダーの位置を設定させる
//=============================================================================
void CBullet::SetBoxColliderPos(void)
{
	D3DXVECTOR3 pos = GetPos();
	if (GetBoxCollider() != NULL)
	{
		GetBoxCollider()->SetPos(pos);
	}
}

//=============================================================================
// 弾のモデル割り当て処理
//=============================================================================
void CBullet::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture)
{
	if (m_pModel == NULL)
	{
		m_pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, pMesh, pBuffMat, nNumMat, pTexture);
	}
}

//=============================================================================
// 弾の前回の位置設定処理
//=============================================================================
void CBullet::SetPosOld(D3DXVECTOR3 posOld)
{
	m_PosOld = posOld;
}

//=============================================================================
// 弾の移動量の設定処理
//=============================================================================
void CBullet::SetMove(D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
// 弾の番号の設定処理
//=============================================================================
void CBullet::SetIdx(const int nIdx)
{
	m_nIdx = nIdx;
}

//=============================================================================
// 弾の種類の設定処理
//=============================================================================
void CBullet::SetType(const TYPE type)
{
	m_Type = type;
}

//=============================================================================
// 弾の弾を発射したオブジェクトへのポインタの設定処理
//=============================================================================
void CBullet::SetParent(CScene *pParent)
{
	m_pParent = pParent;
}

//=============================================================================
// 弾のモデルクラスへのポインタの設定処理
//=============================================================================
void CBullet::SetModel(CModel *pModel)
{
	m_pModel = pModel;
}

//=============================================================================
// 弾の前回の位置取得処理
//=============================================================================
D3DXVECTOR3 CBullet::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
// 弾の移動量取得処理
//=============================================================================
D3DXVECTOR3 CBullet::GetMove(void)
{
	return m_Move;
}

//=============================================================================
// 弾の番号取得処理
//=============================================================================
int CBullet::GetIdx(void)
{
	return m_nIdx;
}

//=============================================================================
// 弾の種類取得処理
//=============================================================================
CBullet::TYPE CBullet::GetType(void)
{
	return m_Type;
}

//=============================================================================
// 弾の弾を発射したオブジェクトへのポインタ取得処理
//=============================================================================
CScene *CBullet::GetParent(void)
{
	return m_pParent;
}

//=============================================================================
// 弾のモデルクラスへのポインタ取得処理
//=============================================================================
CModel *CBullet::GetModel(void)
{
	return m_pModel;
}


//*****************************************************************************
// CBulletPlayerの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CBulletPlayer::CBulletPlayer(int nPriority, OBJTYPE objType) : CBullet(nPriority, objType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBulletPlayer::~CBulletPlayer()
{

}

//=============================================================================
// 弾の生成
//=============================================================================
CBulletPlayer *CBulletPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, TYPE type, CScene *pParent,
	LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, int nPriority)
{
	CBulletPlayer *pBullet = NULL;

	if (pBullet == NULL)
	{
		pBullet = new CBulletPlayer;
		if (pBullet != NULL)
		{
			// 各種値の設定
			pBullet->SetPos(pos);
			pBullet->SetRot(rot);
			pBullet->SetMove(move);
			pBullet->SetType(type);
			pBullet->SetColRange(D3DXVECTOR3(18.75f, 18.75f, 18.75f));
			pBullet->SetParent(pParent);

			if (FAILED(pBullet->Init()))
			{
				return NULL;
			}
		}
	}
	return pBullet;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBulletPlayer::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CBullet::Init()))
	{
		return E_FAIL;
	}

	// 自身の種類によって処理わけ
	switch (GetType())
	{
	case TYPE_PLAYER_0:
		SetIdx(m_nNumAll_0);
		m_nNumAll_0++;
		break;
	case TYPE_PLAYER_1:
		SetIdx(m_nNumAll_1);
		m_nNumAll_1++;
		break;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBulletPlayer::Uninit(void)
{
	// 自身の種類によって処理わけ
	switch (GetType())
	{
	case TYPE_PLAYER_0:
		m_nNumAll_0--;
		break;
	case TYPE_PLAYER_1:
		m_nNumAll_1--;
		break;
	}

	CPlayer *pPlayer = NULL;	// プレイヤーのポインタ情報

	// プレイヤーが弾を打てるように設定する
	pPlayer = (CPlayer*)GetParent();
	if (pPlayer != NULL)
	{
		pPlayer->SetShoot(false);
	}

	// 共通の終了処理
	CBullet::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBulletPlayer::Update(void)
{
	// 前回の位置設定
	SetPosOld(GetPos());

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// オンラインプレイならば
		if (GetType() == CManager::GetClient()->GetClientId())
		{
			// 移動処理
			Move();

			// 当たり判定処理
			Collision();
		}
	}
	else
	{// ローカルプレイならば
		// 移動処理
		Move();

		// 当たり判定処理
		Collision();
	}

	// ボックスコライダーを移動させる
	SetBoxColliderPos();
}

//=============================================================================
// 描画処理
//=============================================================================
void CBulletPlayer::Draw(void)
{
	// 共通の描画処理
	CBullet::Draw();
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CBulletPlayer::Collision(void)
{
	bool bDeath = false;                  // 死亡したかどうか
	D3DXVECTOR3 pos = GetPos();           // 今回の座標
	D3DXVECTOR3 posDef = pos;             // デフォルトの今回の位置(戻されないように保存)
	D3DXVECTOR3 posOld = GetPosOld();     // 前回の位置
	D3DXVECTOR3 move = GetMove();         // 移動量
	D3DXVECTOR3 colRange = GetColRange(); // 当たり判定を取る範囲

	// 当たり判定開始
	CScene *pScene = NULL;     // オブジェクトのポインタ
	CScene *pSceneNext = NULL; // 次のオブジェクトのポインタ
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{
			pSceneNext = pScene->GetNext();
			CollisionCheck(&pos, &posOld, &move, colRange, pScene, &bDeath);
			pos = posDef;
			pScene = pSceneNext;
		}
	}

	// 矩形の当たり判定
	if (pos.x >= (MASS_SIZE_X * MASS_BLOCK_X) / 2)
	{// 右側
		bDeath = true;
	}
	if (pos.x <= (-MASS_SIZE_X * MASS_BLOCK_X) / 2)
	{// 左側
		bDeath = true;
	}
	if (pos.z >= (MASS_SIZE_Z * MASS_BLOCK_Z) / 2)
	{// 奥側
		bDeath = true;
	}
	if (pos.z <= (-MASS_SIZE_Z * MASS_BLOCK_Z) / 2)
	{// 手前側
		bDeath = true;
	}


	// 死亡判定チェック
	if (bDeath == true)
	{
		Destroy();
	}
}

//=============================================================================
// 当たり判定チェック処理
//=============================================================================
void CBulletPlayer::CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath)
{
	CObject3D *pObj = (CObject3D*)pScene;
	if (pObj->GetObjType() == OBJTYPE_PLAYER)
	{// プレイヤーだったら
		CPlayer *pPlayer = (CPlayer*)pScene;
		if (CollisionPlayer(pPos, pPosOld, pMove, colRange, (CPlayer*)pScene) == true)
		{// 当たっている
			if (pPlayer->GetPlayerIdx() != GetType())
			{
				pObj->Hit(this);
			}
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_ENEMY)
	{// 敵だったら
		if (CollisionEnemy(pPos, pPosOld, pMove, colRange, (CEnemy*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BULLET)
	{// 弾だったら
		if (CollisionBullet(pPos, pPosOld, pMove, colRange, (CBullet*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BLOCK)
	{// ブロックだったら
		if (CollisionBlock(pPos, pPosOld, pMove, colRange, (CBlock*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_HEADQUARTERS)
	{// 司令部だったら
		if (CollisionHeadQuarters(pPos, pPosOld, pMove, colRange, (CHeadQuarters*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			*pDeath = true;
		}
	}
}

//=============================================================================
// プレイヤーとの当たり判定処理
//=============================================================================
bool CBulletPlayer::CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer)
{
	CBoxCollider *pBoxCollider = pPlayer->GetBoxCollider();	//当たり判定箱の取得処理
	if (pBoxCollider != NULL)
	{//当たり判定箱がNULLでない場合
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//当たり判定箱に当たった場合
			return true;
		};
	}

	return false;
}

//=============================================================================
// 敵との当たり判定処理
//=============================================================================
bool CBulletPlayer::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy)
{
	CBoxCollider *pBoxCollider = pEnemy->GetBoxCollider();	//当たり判定箱の取得処理
	if (pBoxCollider != NULL)
	{//当たり判定箱がNULLでない場合
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//当たり判定箱に当たった場合
			return true;
		};
	}

	return false;
}

//=============================================================================
// 弾との当たり判定処理
//=============================================================================
bool CBulletPlayer::CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet)
{
	CBoxCollider *pBoxCollider = pBullet->GetBoxCollider();	//当たり判定箱の取得処理
	if (pBoxCollider != NULL)
	{//当たり判定箱がNULLでない場合
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//当たり判定箱に当たった場合
			return true;
		};
	}

	return false;
}

//=============================================================================
// ブロックとの当たり判定処理
//=============================================================================
bool CBulletPlayer::CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock)
{
	CBoxCollider *pBoxCollider = pBlock->GetBoxCollider();	//当たり判定箱の取得処理
	if (pBoxCollider != NULL)
	{//当たり判定箱がNULLでない場合
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//当たり判定箱に当たった場合
			return true;
		};
	}

	return false;
}

//=============================================================================
// 司令部との当たり判定処理
//=============================================================================
bool CBulletPlayer::CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead)
{
	CBoxCollider *pBoxCollider = pHead->GetBoxCollider();	//当たり判定箱の取得処理
	if (pBoxCollider != NULL)
	{//当たり判定箱がNULLでない場合
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//当たり判定箱に当たった場合
			return true;
		};
	}

	return false;
}

//=============================================================================
// 死亡処理
//=============================================================================
void CBulletPlayer::Destroy(void)
{
	Uninit();
}

//=============================================================================
// プレイヤーの弾の総数を取得する処理
//=============================================================================
int CBulletPlayer::GetNumAll_0(void)
{
	return m_nNumAll_0;
}
int CBulletPlayer::GetNumAll_1(void)
{
	return m_nNumAll_1;
}

//*****************************************************************************
// CBulletEnemyの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CBulletEnemy::CBulletEnemy(int nPriority, OBJTYPE objType) : CBullet(nPriority, objType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBulletEnemy::~CBulletEnemy()
{

}

//=============================================================================
// 弾の生成
//=============================================================================
CBulletEnemy *CBulletEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, CScene *pParent,
	LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, int nPriority)
{
	CBulletEnemy *pBullet = NULL;

	if (pBullet == NULL)
	{
		pBullet = new CBulletEnemy;
		if (pBullet != NULL)
		{
			// 各種値の設定
			pBullet->SetPos(pos);
			pBullet->SetRot(rot);
			pBullet->SetMove(move);
			pBullet->SetType(TYPE_ENEMY);
			pBullet->SetColRange(D3DXVECTOR3(18.75f, 18.75f, 18.75f));
			pBullet->SetParent(pParent);

			if (FAILED(pBullet->Init()))
			{
				return NULL;
			}
		}
	}
	return pBullet;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBulletEnemy::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CBullet::Init()))
	{
		return E_FAIL;
	}

	// 番号を設定
	SetIdx(m_nNumAll);

	// 弾の総数を増やす
	m_nNumAll++;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBulletEnemy::Uninit(void)
{
	// 弾の総数を減らす
	m_nNumAll--;

	// 共通の終了処理
	CBullet::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBulletEnemy::Update(void)
{
	// 前回の位置設定
	SetPosOld(GetPos());

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// オンラインプレイならば
		if (CManager::GetClient()->GetClientId() == 0)
		{
			// 移動処理
			Move();

			// 当たり判定処理
			Collision();
		}
	}
	else
	{// ローカルプレイならば
	    // 移動処理
		Move();

		// 当たり判定処理
		Collision();
	}

	// ボックスコライダーを動かす
	SetBoxColliderPos();
}

//=============================================================================
// 描画処理
//=============================================================================
void CBulletEnemy::Draw(void)
{
	// 共通の描画処理
	CBullet::Draw();
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CBulletEnemy::Collision(void)
{
	bool bDeath = false;                  // 死亡したかどうか
	D3DXVECTOR3 pos = GetPos();           // 今回の座標
	D3DXVECTOR3 posOld = GetPosOld();     // 前回の位置
	D3DXVECTOR3 move = GetMove();         // 移動量
	D3DXVECTOR3 colRange = GetColRange(); // 当たり判定を取る範囲

	// 当たり判定開始
	CScene *pScene = NULL;     // オブジェクトのポインタ
	CScene *pSceneNext = NULL; // 次のオブジェクトのポインタ
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{
			pSceneNext = pScene->GetNext();
			CollisionCheck(&pos, &posOld, &move, colRange, pScene, &bDeath);
			pScene = pSceneNext;
		}
	}

	// 矩形の当たり判定
	if (pos.x >= (MASS_SIZE_X * MASS_BLOCK_X) / 2)
	{// 右側
		bDeath = true;
	}
	if (pos.x <= (-MASS_SIZE_X * MASS_BLOCK_X) / 2)
	{// 左側
		bDeath = true;
	}
	if (pos.z >= (MASS_SIZE_Z * MASS_BLOCK_Z) / 2)
	{// 奥側
		bDeath = true;
	}
	if (pos.z <= (-MASS_SIZE_Z * MASS_BLOCK_Z) / 2)
	{// 手前側
		bDeath = true;
	}

	// 死亡判定チェック
	if (bDeath == true)
	{
		Destroy();
	}
}

//=============================================================================
// 当たり判定チェック処理
//=============================================================================
void CBulletEnemy::CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath)
{
	CObject3D *pObj = (CObject3D*)pScene;
	if (pObj->GetObjType() == OBJTYPE_PLAYER)
	{// プレイヤーだったら
		if (CollisionPlayer(pPos, pPosOld, pMove, colRange, (CPlayer*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_ENEMY)
	{// 敵だったら
		if (CollisionEnemy(pPos, pPosOld, pMove, colRange, (CEnemy*)pScene) == true)
		{// 当たっている
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BULLET)
	{// 弾だったら
		if (CollisionBullet(pPos, pPosOld, pMove, colRange, (CBullet*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BLOCK)
	{// ブロックだったら
		if (CollisionBlock(pPos, pPosOld, pMove, colRange, (CBlock*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			*pDeath = true;
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_HEADQUARTERS)
	{// 司令部だったら
		if (CollisionHeadQuarters(pPos, pPosOld, pMove, colRange, (CHeadQuarters*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			*pDeath = true;
		}
	}
}

//=============================================================================
// プレイヤーとの当たり判定処理
//=============================================================================
bool CBulletEnemy::CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer)
{
	CBoxCollider *pBoxCollider = pPlayer->GetBoxCollider();	//当たり判定箱の取得処理
	if (pBoxCollider != NULL)
	{//当たり判定箱がNULLでない場合
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//当たり判定箱に当たった場合
			return true;
		};
	}

	return false;
}

//=============================================================================
// 敵との当たり判定処理
//=============================================================================
bool CBulletEnemy::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy)
{
	CBoxCollider *pBoxCollider = pEnemy->GetBoxCollider();	//当たり判定箱の取得処理
	if (pBoxCollider != NULL)
	{//当たり判定箱がNULLでない場合
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//当たり判定箱に当たった場合
			return true;
		};
	}

	return false;
}

//=============================================================================
// 弾との当たり判定処理
//=============================================================================
bool CBulletEnemy::CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet)
{
	CBoxCollider *pBoxCollider = pBullet->GetBoxCollider();	//当たり判定箱の取得処理
	if (pBoxCollider != NULL)
	{//当たり判定箱がNULLでない場合
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//当たり判定箱に当たった場合
			return true;
		};
	}

	return false;
}

//=============================================================================
// ブロックとの当たり判定処理
//=============================================================================
bool CBulletEnemy::CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock)
{
	CBoxCollider *pBoxCollider = pBlock->GetBoxCollider();	//当たり判定箱の取得処理
	if (pBoxCollider != NULL)
	{//当たり判定箱がNULLでない場合
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//当たり判定箱に当たった場合
			return true;
		};
	}

	return false;
}

//=============================================================================
// 司令部との当たり判定処理
//=============================================================================
bool CBulletEnemy::CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead)
{
	CBoxCollider *pBoxCollider = pHead->GetBoxCollider();	//当たり判定箱の取得処理
	if (pBoxCollider != NULL)
	{//当たり判定箱がNULLでない場合
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange, NULL) == true)
		{//当たり判定箱に当たった場合
			//return true;
		};
	}

	return false;
}

//=============================================================================
// 死亡処理
//=============================================================================
void CBulletEnemy::Destroy(void)
{
	Uninit();
}

//=============================================================================
// 敵の弾の総数を取得する処理
//=============================================================================
int CBulletEnemy::GetNumAll(void)
{
	return m_nNumAll;
}