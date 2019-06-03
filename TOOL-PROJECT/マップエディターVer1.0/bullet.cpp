//=============================================================================
//
// 弾の処理 [bullet.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "boxCollider.h"
#include "debugproc.h"
#include "enemy.h"
#include "player.h"
#include "block.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH			CBullet::m_pMesh = NULL;
LPD3DXBUFFER		CBullet::m_pBuffMat = NULL;
DWORD				CBullet::m_nNumMat = NULL;

//=============================================================================
// 弾のコンストラクタ
//=============================================================================
CBullet::CBullet()
{
	m_posOld = INITIALIZE_D3DXVECTOR3;		//過去の位置
	m_move = INITIALIZE_D3DXVECTOR3;		//移動量
	m_type = BULLET_TYPE_NONE;				//弾の種類
	m_pModel = NULL;						//モデルのポインタ
	m_pScene = NULL;						//オブジェクトのポインタ
}
//=============================================================================
// 弾のコンストラクタ
//=============================================================================
CBullet::~CBullet()
{

}
//=============================================================================
// 弾の生成
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 move, BULLET_TYPE type,CScene *pScene)
{
	CBullet *pBullet = NULL;				//弾のポインタ

	if (pBullet == NULL)
	{//NULLの場合
		pBullet = new CBullet;				//動的確保
		if (pBullet != NULL)
		{//NULLでない場合
			pBullet->SetPos(pos);			//位置の設置処理
			pBullet->SetRot(rot);			//向きの設置処理
			pBullet->SetMove(move);			//移動量の設置処理
			pBullet->SetBulletType(type);	//弾の種類の設置処理
			pBullet->SetColRange(D3DXVECTOR3(18.75f, 18.75f, 18.75f));	//当たり判定の大きさの設置処理
			pBullet->m_pScene = pScene;
			pBullet->Init();				//初期化処理
		}
	}
	return pBullet;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\bullet.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//モデルの生成
	m_pModel = CModel::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f,0.0f,0.0f),
		m_pMesh, m_pBuffMat, m_nNumMat);

	//オブジェクト3Dの初期化処理
	CObject3D::Init();

	SetObjType(OBJTYPE_BULLET);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	//モデルの終了処理
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	//終了処理
	CObject3D::Uninit();

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	//過去の位置設置処理
	SetPosOld(GetPos());

	//移動処理
	Move();

	//当たり判定処理
	Collision();

}

//=============================================================================
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxWorld;			//ワールドマトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//モデルの描画処理
	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}

	CObject3D::Draw();
}

//=============================================================================
// 弾の過去の位置設置処理
//=============================================================================
void CBullet::SetPosOld(D3DXVECTOR3 posOld)
{
	m_posOld = posOld;
}

//=============================================================================
// 弾の移動量の設置処理
//=============================================================================
void CBullet::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//=============================================================================
// 弾の設置処理
//=============================================================================
void CBullet::SetBulletType(CBullet::BULLET_TYPE type)
{
	m_type = type;
}

//=============================================================================
// オブジェクトの設置処理
//=============================================================================
void CBullet::SetScene(CScene *pScene)
{
	m_pScene = pScene;
}

//=============================================================================
// 弾の過去の位置取得処理
//=============================================================================
D3DXVECTOR3 CBullet::GetPosOld(void)
{
	return m_posOld;
}

//=============================================================================
// 弾の移動量取得処理
//=============================================================================
D3DXVECTOR3 CBullet::GetMove(void)
{
	return m_move;
}

//=============================================================================
// 弾の取得処理
//=============================================================================
CBullet::BULLET_TYPE CBullet::GetBulletType(void)
{
	return m_type;
}

//=============================================================================
// オブジェクトの取得処理
//=============================================================================
CScene *CBullet::GetScene(void)
{
	return m_pScene;
}

//=============================================================================
// 弾の移動処理
//=============================================================================
void CBullet::Move(void)
{
	D3DXVECTOR3 pos = GetPos();					//位置の取得処理
	D3DXVECTOR3 move = GetMove();				//移動量の取得処理
	pos += move;								//位置の代入

	//当たり判定箱の取得処理
	CBoxCollider *pCollider = NULL;
	pCollider = CObject3D::GetBoxCollider();

	//当たり判定箱の位置設置処理
	if (pCollider != NULL)
	{
		pCollider->SetPos(pos);
	}

	//位置の設置処理
	SetPos(pos);

	//モデルの位置設置処理
	if (m_pModel != NULL)
	{
		m_pModel->SetPos(pos);
	}

	//範囲外に入った場合終了処理
	if (pos.x > 712.5 || pos.x < -712.5 ||
		pos.z > 562.5f || pos.z < -562.5f)
	{
		//消滅処理
		Destroy();
	}
}

//=============================================================================
// 弾の当たり判定処理
//=============================================================================
void CBullet::Collision(void)
{
	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject3D *pObject = NULL;			 // オブジェクトのポインタ
	CBoxCollider *pBoxCollider = NULL;	 //	当たり判定箱のポインタ
	CEnemy *pEnemy = NULL;				 // 敵のポインタ
	CPlayer *pPlayer = NULL;			 // プレイヤーのポインタ
	CBullet *pBullet = NULL;			 // 弾のポインタ
	CBlock *pBlock = NULL;				 // ブロックのポインタ

	//位置の取得処理
	D3DXVECTOR3 pos = GetPos();

	//当たり判定の取得処理
	D3DXVECTOR3 colRange = GetColRange();

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if(m_type == BULLET_TYPE_PLAYER)
			{//撃った弾がプレイヤーの場合
				if (pScene->GetObjType() == OBJTYPE_ENEMY)
				{// 種類が敵の場合
					pEnemy = (CEnemy*)pScene;	//敵の動的確保
					if (pEnemy != NULL)
					{//NULLでない場合
						pBoxCollider = pEnemy->GetBoxCollider();	//当たり判定箱の取得処理
						if (pBoxCollider != NULL)
						{//当たり判定箱がNULLでない場合
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move,colRange / 2 , NULL) == true)
							{//当たり判定箱に当たった場合
								//終了処理
								//Uninit();
								//敵の終了処理
								pEnemy->Uninit();
								Destroy();
							};
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BULLET)
				{// 種類が敵の場合
					pBullet = (CBullet*)pScene;	//敵の動的確保
					if (pBullet != NULL)
					{//NULLでない場合
						pBoxCollider = pBullet->GetBoxCollider();	//当たり判定箱の取得処理
						if (pBoxCollider != NULL)
						{//当たり判定箱がNULLでない場合
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move, colRange / 2, NULL) == true)
							{//当たり判定箱に当たった場合
								//終了処理
								//Uninit();
								//敵の終了処理
								pBullet->Destroy();
								Destroy();
							};
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BLOCK)
				{
					pBlock = (CBlock*)pScene;	//ブロックの動的確保
					if (pBlock != NULL)
					{
						pBoxCollider = pBlock->GetBoxCollider();	//当たり判定箱の取得処理
						if (pBoxCollider != NULL)
						{//当たり判定箱がNULLでない場合
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move, colRange / 2, NULL) == true)
							{//当たり判定箱に当たった場合
							 //終了処理
							 //Uninit();
							 //敵の終了処理
								pBlock->Hit(m_pScene);
								Destroy();
							};
						}
					}
				}
			}
			else if(m_type == BULLET_TYPE_ENEMY)
			{//撃った弾が敵の場合
				if (pScene->GetObjType() == OBJTYPE_PLAYER)
				{// 種類が敵の場合
					pPlayer = (CPlayer*)pScene;	//敵の動的確保
					if (pPlayer != NULL)
					{//NULLでない場合
						pBoxCollider = pPlayer->GetBoxCollider();	//当たり判定箱の取得処理
						if (pBoxCollider != NULL)
						{//当たり判定箱がNULLでない場合
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move, colRange, NULL) == true)
							{//当たり判定箱に当たった場合
							 //終了処理
								//Uninit();
								//敵の終了処理
								pPlayer->Uninit();
								Destroy();
							};
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BULLET)
				{// 種類が敵の場合
					pBullet = (CBullet*)pScene;	//敵の動的確保
					if (pBullet != NULL)
					{//NULLでない場合
						pBoxCollider = pBullet->GetBoxCollider();	//当たり判定箱の取得処理
						if (pBoxCollider != NULL)
						{//当たり判定箱がNULLでない場合
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move, colRange, NULL) == true)
							{//当たり判定箱に当たった場合
							    //終了処理
							    //Uninit();
							    //敵の終了処理
								pBullet->Destroy();
								Destroy();
							};
						}
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_BLOCK)
				{
					pBlock = (CBlock*)pScene;	//ブロックの動的確保
					if (pBlock != NULL)
					{
						pBoxCollider = pBlock->GetBoxCollider();	//当たり判定箱の取得処理
						if (pBoxCollider != NULL)
						{//当たり判定箱がNULLでない場合
							if (pBoxCollider->Collision(&pos, &m_posOld, &m_move, colRange / 2, NULL) == true)
							{//当たり判定箱に当たった場合
							    //終了処理
							    //Uninit();
							    //敵の終了処理
								pBlock->Hit(m_pScene);
								Destroy();
							};
						}
					}
				}

			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}

}

//=============================================================================
// 弾の破壊処理
//=============================================================================
void CBullet::Destroy(void)
{
	CPlayer *pPlayer = NULL;	//プレイヤーのポインタ情報
	CEnemy *pEnemy = NULL;		//敵のポインタ情報

	switch (m_pScene->GetObjType())
	{
	case OBJTYPE_PLAYER:
		pPlayer = (CPlayer*) m_pScene;

		if (pPlayer != NULL)
		{
			pPlayer->SetShoot(false);
		}

		break;
	case OBJTYPE_ENEMY:
		pEnemy = (CEnemy*)m_pScene;

		if (pEnemy != NULL)
		{
			pEnemy->SetShoot(false);
		}

		break;
	}
	Uninit();
}