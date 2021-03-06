//=============================================================================
//
// 敵処理 [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "boxCollider.h"
#include "debugproc.h"
#include "bullet.h"
#include "player.h"
#include "block.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_MOVE (2.5f)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH			CEnemy::m_pMesh = NULL;
LPD3DXBUFFER		CEnemy::m_pBuffMat = NULL;
DWORD				CEnemy::m_nNumMat = NULL;

//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemy::CEnemy() : CCharacter(3, OBJTYPE_ENEMY)
{
	m_pModel = NULL;
	m_pEnemy = NULL;
}
//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}
//=============================================================================
// 敵の生成
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = NULL;				//敵のポインタ

	if (pEnemy == NULL)
	{//NULLの場合
		pEnemy = new CEnemy;				//動的確保
		if (pEnemy != NULL)
		{//NULLでない場合
			pEnemy->SetPos(pos);			//位置の設置処理
			pEnemy->SetRot(rot);			//向きの設置処理
			pEnemy->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pEnemy->SetReturnFlag(true);	//当たった際戻すかどうかの設置処理
			pEnemy->Init();					//初期化処理
			pEnemy->SetEnemy(pEnemy);
		}
	}
	return pEnemy;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\enemy.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//モデルの生成
	m_pModel = CModel::Create(CCharacter::GetPos());
	if (m_pModel != NULL)
	{
		m_pModel->BindX(m_pMesh, m_pBuffMat, m_nNumMat);
	}

	//移動種類の設置処理
	m_moveType = MOVE_DOWN;
	//オブジェクト3Dの初期化処理
	CObject3D::Init();
	//種類の設置処理
	SetObjType(OBJTYPE_ENEMY);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	//モデルの終了処理
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	//終了処理
	CCharacter::Uninit();

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	//過去の位置の設置
	SetPosOld(GetPos());

	//移動処理
	Move();

	//当たり判定の処理
	Collision();

}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}

	//オブジェクト3Dの描画処理
	CObject3D::Draw();
}

//=============================================================================
// 敵の設置処理
//=============================================================================
void CEnemy::SetEnemy(CEnemy *pEnemy)
{
	m_pEnemy = pEnemy;
}

//=============================================================================
// 敵の設置処理
//=============================================================================
CEnemy *CEnemy::GetEnemy(void)
{
	return m_pEnemy;
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CEnemy::Hit(CScene * pScene)
{

}

//=============================================================================
// 移動処理
//=============================================================================
void CEnemy::Move(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetJoypad();

	//移動量の取得
	D3DXVECTOR3 move = CCharacter::GetMove();

	//位置の取得
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//向きの取得
	D3DXVECTOR3 rot = CCharacter::GetRot();

	float fDiffAngle;

	//移動の種類別処理
	switch (m_moveType)
	{
	case MOVE_UP:	//上移動
		move.z = ENEMY_MOVE;
		fDiffAngle = (D3DX_PI)-rot.y;
		if (fDiffAngle > D3DX_PI)
		{
			fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (fDiffAngle < -D3DX_PI)
		{
			fDiffAngle += D3DX_PI * 2.0f;
		}
		rot.y += fDiffAngle * 0.8f;
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		SetNowRotInfo(ROT_UP);	//現在の向きを上にする
		break;

	case MOVE_DOWN:	//下移動
		move.z = -ENEMY_MOVE;
		fDiffAngle = (D3DX_PI * 0) - rot.y;
		if (fDiffAngle > D3DX_PI)
		{
			fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (fDiffAngle < -D3DX_PI)
		{
			fDiffAngle += D3DX_PI * 2.0f;
		}
		rot.y += fDiffAngle * 0.8f;
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		SetNowRotInfo(ROT_DOWN);	//現在の向きを下にする
		break;
	case MOVE_LEFT:	//左移動
		move.x = -ENEMY_MOVE;
		fDiffAngle = (D3DX_PI * 0.5f) - rot.y;
		if (fDiffAngle > D3DX_PI)
		{
			fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (fDiffAngle < -D3DX_PI)
		{
			fDiffAngle += D3DX_PI * 2.0f;
		}
		rot.y += fDiffAngle * 0.8f;
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		SetNowRotInfo(ROT_LEFT);	//現在の向きを左にする
		break;
	case MOVE_RIGHT:	//右移動
		move.x = ENEMY_MOVE;

		fDiffAngle = (D3DX_PI * -0.5f) - rot.y;
		if (fDiffAngle > D3DX_PI)
		{
			fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (fDiffAngle < -D3DX_PI)
		{
			fDiffAngle += D3DX_PI * 2.0f;
		}
		rot.y += fDiffAngle * 0.8f;
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		SetNowRotInfo(ROT_RIGHT);	//現在の向きを右にする
		break;
	}

	if (rot.y == D3DX_PI ||
		rot.y == D3DX_PI * 0 ||
		rot.y == D3DX_PI * 0.5f ||
		rot.y == D3DX_PI * -0.5f)
	{//向きが四方を向いている場合
		if (GetShoot() == false)
		{//撃っていない場合
			//CBullet::Create(pos, rot, D3DXVECTOR3(sinf(rot.y) * -8.0f, 0.0f, cosf(rot.y) * -8.0f), CBullet::BULLET_TYPE_ENEMY, m_pEnemy);
			switch (GetNowRotInfo())
			{
			case ROT_UP:	//上を向いている場合
				CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 8.0f), CBullet::BULLET_TYPE_PLAYER, m_pEnemy); //弾の生成
				break;
			case ROT_DOWN:	//上を向いている場合
				CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -8.0f), CBullet::BULLET_TYPE_PLAYER, m_pEnemy); //弾の生成
				break;
			case ROT_RIGHT:	//上を向いている場合
				CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8.0f, 0.0f, 0.0f), CBullet::BULLET_TYPE_PLAYER, m_pEnemy); //弾の生成
				break;
			case ROT_LEFT:	//上を向いている場合
				CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-8.0f, 0.0f, 0.0f), CBullet::BULLET_TYPE_PLAYER, m_pEnemy); //弾の生成
				break;

			}
			SetShoot(true);
		}
	}

	//当たり判定箱の取得処理
	CBoxCollider *pCollider = NULL;
	pCollider = CObject3D::GetBoxCollider();

	//当たり判定箱の位置設置処理
	if (pCollider != NULL)
	{
		pCollider->SetPos(pos);
	}

	//位置の加算処理
	pos += move;
	//慣性処理
	move.x += (0.0f - move.x) * 0.8f;
	move.z += (0.0f - move.z) * 0.8f;

	//位置の設置処理
	SetPos(pos);
	//向き設置処理
	SetRot(rot);
	//モデルの位置設置処理
	m_pModel->SetPos(pos);
	//モデルの向き設置処理
	m_pModel->SetRot(rot);
	//移動の設置処理
	SetMove(move);
	//向きの設置処理
	SetRot(rot);
}

//=============================================================================
// 当たり判定の処理
//=============================================================================
void CEnemy::Collision(void)
{
	//位置の取得
	D3DXVECTOR3 pos = GetPos();

	//過去の位置の取得
	D3DXVECTOR3 posOld = GetPosOld();

	//移動量の取得処理
	D3DXVECTOR3 move = GetMove();

	//当たり判定の大きさの取得
	D3DXVECTOR3 colRange = GetColRange();

	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject3D *pObject = NULL;			 // オブジェクトのポインタ
	CBoxCollider *pBoxCollider = NULL;	 // 当たり判定箱のポインタ
	CPlayer *pPlayer = NULL;			 // 敵のポインタ
	CBlock *pBlock = NULL;				 // ブロックのポインタ

	bool bland = false;

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_PLAYER)
			{// プレイヤーだったら
				pPlayer = (CPlayer*)pScene;
				if (pPlayer != NULL)
				{
					pBoxCollider = pPlayer->GetBoxCollider();
					if (pBoxCollider != NULL)
					{
						if (pBoxCollider->Collision(&pos, &posOld, &move, colRange / 2, NULL) == true)
						{
							bland = true;
						};
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_3D)
			{// オブジェクト3Dだったら
				pObject = (CObject3D*)pScene;
				if (pObject != NULL)
				{
					pBoxCollider = pObject->GetBoxCollider();
					if (pBoxCollider != NULL)
					{
						if (pBoxCollider->Collision(&pos, &posOld, &move, colRange / 2, NULL) == true)
						{
							bland = true;
						};
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_BLOCK)
			{
				pBlock = (CBlock*)pScene;
				if (pBlock != NULL)
				{
					pBoxCollider = pBlock->GetBoxCollider();
					if (pBoxCollider != NULL)
					{
						if (pBoxCollider->Collision(&pos, &posOld, &move, colRange / 2, NULL) == true)
						{
							bland = true;
						};
					}
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}

	//範囲外に行った場合もとに戻す
	if (pos.x > 712.5f - 35.0f)
	{
		pos.x = 712.5f - 35.0f;
		bland = true;
	}
	if (pos.x < -712.5f + 35.0f)
	{
		pos.x = -712.5f + 35.0f;
		bland = true;
	}
	if (pos.z > 562.5f - 35.0f)
	{
		pos.z = 562.5f - 35.0f;
		bland = true;
	}
	if (pos.z < -562.5f + 35.0f)
	{
		pos.z = -562.5f + 35.0f;
		bland = true;
	}

	if (bland == true)
	{//当たっていた場合
		//移動向きをランダムで決める
		int nAnswer = rand() % 11;

		if (nAnswer >= 0 && nAnswer <= 1)
		{
			m_moveType = MOVE_UP;	//上移動
		}
		else if(nAnswer >= 2 && nAnswer <= 4)
		{
			m_moveType = MOVE_LEFT;	//左移動
		}
		else if (nAnswer >= 5 && nAnswer <= 7)
		{
			m_moveType = MOVE_RIGHT;	//右移動
		}
		else if (nAnswer >= 8 && nAnswer <= 10)
		{
			m_moveType = MOVE_DOWN;		//左移動
		}
	}
	//位置の設置処理
	SetPos(pos);
	//過去の位置設置処理
	SetPosOld(posOld);
	//移動量設置処理
	SetMove(move);
	//モデルの位置設置処理
	m_pModel->SetPos(pos);
}