//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "boxCollider.h"
#include "bullet.h"
#include "enemy.h"
#include "basemode.h"
#include "cameraManager.h"
#include "block.h"
#include "game.h"
#include "server.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MOVE (2.5f)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPD3DXMESH			CPlayer::m_pMesh = NULL;
LPD3DXBUFFER		CPlayer::m_pBuffMat = NULL;
DWORD				CPlayer::m_nNumMat = NULL;

//=============================================================================
// プレイヤーのコンストラクタ
//=============================================================================
CPlayer::CPlayer() : CCharacter(3,OBJTYPE_PLAYER)
{
	m_pModel = NULL;	//モデルのポインタ
	m_pPlayer = NULL;	//プレイヤーのポインタ
}
//=============================================================================
// プレイヤーのコンストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}
//=============================================================================
// プレイヤーの生成
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdx)
{
	CPlayer *pPlayer = NULL;				//プレイヤーのポインタ

	if (pPlayer == NULL)
	{//NULLの場合
		pPlayer = new CPlayer;				//動的確保
		if (pPlayer != NULL)
		{//NULLでない場合
			pPlayer->SetPos(pos);			//位置の設置処理
			pPlayer->SetRot(rot);			//向きの設置処理
			pPlayer->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pPlayer->m_nIdx = nIdx;
			pPlayer->Init();				//初期化処理
			pPlayer->SetPlayer(pPlayer);
		}
	}
	return pPlayer;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\player.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	//モデルの生成
	m_pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
		m_pMesh, m_pBuffMat, m_nNumMat, NULL);

	CCharacter::Init();

	SetObjType(OBJTYPE_PLAYER);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
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

	if (CManager::GetGame() != NULL)
	{
		CManager::GetGame()->DeletePlayer(NULL, m_nIdx);
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	//過去の位置の設置
	SetPosOld(GetPos());

	// 入力による移動量計算処理
	if (m_nIdx == CManager::GetClient()->GetClientId())
	{
		InputMove();
	}

	//移動処理
	Move();

	//当たり判定の処理
	Collision();
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxWorld;
	D3DXMatrixIdentity(&mtxWorld);
	m_pModel->SetPos(GetPos());
	GetBoxCollider()->SetPos(GetPos());

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}

	//オブジェクト3Dの描画処理
	CObject3D::Draw();
}

//=============================================================================
// プレイヤーの設置処理
//=============================================================================
void CPlayer::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}


//=============================================================================
// プレイヤーの取得処理
//=============================================================================
CPlayer *CPlayer::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::InputMove(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	// XInputの取得
	CXInput *pXInput = CManager::GetXInput();

	//移動量の取得
	D3DXVECTOR3 move = CCharacter::GetMove();

	//位置の取得
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//向きの取得
	D3DXVECTOR3 rot = CCharacter::GetRot();

	float fDiffAngle;

	if (pInputKeyboard->GetPress(DIK_W) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_16) == true)
	{// 上方向の入力がされた
		move.z = PLAYER_MOVE;
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
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_17) == true)
	{// 下方向の入力がされた
		move.z = -PLAYER_MOVE;
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
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_18) == true)
	{// 左方向の入力がされた
		move.x = -PLAYER_MOVE;
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
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_19) == true)
	{// 右方向の入力がされた
		move.x = PLAYER_MOVE;

		fDiffAngle = (D3DX_PI * -0.5f) - rot.y;
		if (fDiffAngle > D3DX_PI)
		{
			fDiffAngle -= D3DX_PI * 2.0f;
		}
		if (fDiffAngle < -D3DX_PI)
		{
			fDiffAngle += D3DX_PI * 2.0f;
		}
		rot.y += fDiffAngle * 1.0f;
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		SetNowRotInfo(ROT_RIGHT);	//現在の向きを右にする
	}

	if (GetShoot() == false)
	{//撃っていない場合
		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
			pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
		{// 弾発射ボタンが押された
			switch (GetNowRotInfo())
			{
			case ROT_UP:	//上を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 8.0f), (CBullet::TYPE)CManager::GetClient()->GetClientId(), this); //弾の生成
				break;
			case ROT_DOWN:	//上を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -8.0f), (CBullet::TYPE)CManager::GetClient()->GetClientId(), this); //弾の生成
				break;
			case ROT_RIGHT:	//上を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(8.0f, 0.0f, 0.0f), (CBullet::TYPE)CManager::GetClient()->GetClientId(), this); //弾の生成
				break;
			case ROT_LEFT:	//上を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-8.0f, 0.0f, 0.0f), (CBullet::TYPE)CManager::GetClient()->GetClientId(), this); //弾の生成
				break;

			}
			SetShoot(true);		//弾を撃った状態にする
			CManager::GetSound()->PlaySound(2);
		}
	}


	//位置の設置処理
	SetPos(pos);
	//向き設置処理
	SetRot(rot);
	//移動の設置処理
	SetMove(move);
	//向きの設置処理
	SetRot(rot);

	//モデルの位置設置処理
	m_pModel->SetPos(pos);
	//モデルの向き設置処理
	m_pModel->SetRot(rot);
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::Move(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	//移動量の取得
	D3DXVECTOR3 move = CCharacter::GetMove();

	//位置の取得
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//向きの取得
	D3DXVECTOR3 rot = CCharacter::GetRot();


	//if (rot.y == D3DX_PI ||
	//	rot.y == D3DX_PI * 0.5f ||
	//	rot.y == D3DX_PI * -0.5f)
	//{//向きが四方を向いている場合
	//	if (GetShoot() == false)
	//	{//撃っていない場合
	//		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	//		{// キーボードの[SPACE]キーが押された（弾発射）
	//			CBullet::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(rot.y) * -8.0f, 0.0f, cosf(rot.y) * -8.0f), CBullet::BULLET_TYPE_PLAYER, m_pPlayer); //弾の生成

	//			//弾を撃った状態にする
	//			SetShoot(true);
	//		}
	//	}
	//}

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

	//慣性の処理
	move.x += (0.0f - move.x) * 0.8f;
	move.z += (0.0f - move.z) * 0.8f;

	//範囲外に行った場合もとに戻す
	if (pos.x > (MASS_SIZE_X * MASS_BLOCK_X) - 35.0f)
	{
		pos.x = (MASS_SIZE_X * MASS_BLOCK_X) - 35.0f;
	}
	if(pos.x < -(MASS_SIZE_X * MASS_BLOCK_X) + 35.0f)
	{
		pos.x = -(MASS_SIZE_X * MASS_BLOCK_X) + 35.0f;
	}
	if (pos.z > (MASS_SIZE_X* MASS_BLOCK_Z) - 35.0f)
	{
		pos.z = (MASS_SIZE_X * MASS_BLOCK_Z) - 35.0f;
	}
	if (pos.z < -(MASS_SIZE_X * MASS_BLOCK_Z) + 35.0f)
	{
		pos.z = -(MASS_SIZE_X * MASS_BLOCK_Z) + 35.0f;
	}

	//位置の設置処理
	SetPos(pos);
	//向き設置処理
	SetRot(rot);
	//移動の設置処理
	SetMove(move);
	//向きの設置処理
	SetRot(rot);

	//モデルの位置設置処理
	m_pModel->SetPos(pos);
	//モデルの向き設置処理
	m_pModel->SetRot(rot);

}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CPlayer::Collision(void)
{
	//過去の位置の取得
	D3DXVECTOR3 posOld = GetPosOld();

	//移動量の取得処理
	D3DXVECTOR3 move = GetMove();

	//当たり判定の大きさの取得
	D3DXVECTOR3 colRange = GetColRange();

	//位置の取得
	D3DXVECTOR3 pos = GetPos();

	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ
	CObject3D *pObject = NULL;			 // オブジェクトのポインタ
	CBoxCollider *pBoxCollider = NULL;	 // 当たり判定箱のポインタ
	CEnemy *pEnemy = NULL;				 // 敵のポインタ
	CBlock *pBlock = NULL;				 // ブロックのポインタ

	bool bland = false;

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_3D)
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
			else if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{
				pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{
					pBoxCollider = pEnemy->GetBoxCollider();
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
	switch (bland)
	{
	case false:
		break;
	case true:
		break;
	}

	//位置の設置処理
	SetPos(pos);
	//過去の設置処理
	SetPosOld(posOld);
	//移動量の設置処理
	SetMove(move);
	//モデルの位置設置処理
	m_pModel->SetPos(pos);
}


//=============================================================================
// 当たったときの処理
//=============================================================================
void CPlayer::Hit(CScene *pScene)
{
	// 当たったオブジェクトによって処理わけ
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// 弾だった
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_ENEMY)
		{// 敵の弾だった
			Uninit();
		}
	}
}

//=============================================================================
// 変数をクリアする処理
//=============================================================================
void CPlayer::ClearVariable(void)
{

}