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
#include "motion.h"
#include "item.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MOVE (2.5f)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// プレイヤーのコンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	m_pPlayer = NULL;			//プレイヤーのポインタ
	m_state = STATE_NOMAL;		//状態
	m_nCntState = 0;			//状態カウンター
	m_nMaxBullet = 1;			//最大弾数
	m_nCntAbility = 0;			//能力カウンター
	m_bAllBlockDestroy = false;	//全てのブロックを壊せるかどうか
	m_bSplash = false;			//汚れているかどうか
	m_nCntSplash = 0;			//汚れカウンター
	m_motion = MOTION_NEUTAL;	//モーション情報
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
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerIdx, CModel **apModel, CMotionManager *pMotionManager, int nNumParts)
{
	CPlayer *pPlayer = NULL;				//プレイヤーのポインタ

	if (pPlayer == NULL)
	{//NULLの場合
		pPlayer = new CPlayer;				//動的確保
		if (pPlayer != NULL)
		{//NULLでない場合
			pPlayer->SetPos(pos);			//位置の設置処理
			pPlayer->SetRot(rot);			//向きの設置処理
			pPlayer->SetModel(apModel);
			pPlayer->SetMotionManager(pMotionManager);
			pPlayer->SetNumPart(nNumParts);
			pPlayer->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pPlayer->SetPlayerIdx(nPlayerIdx);
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
	//変数のクリア処理
	ClearVariable();

	//初期化処理
	CCharacter::Init();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	//終了処理
	CCharacter::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	//過去の位置の設置
	SetPosOld(GetPos());

	// 入力による移動量計算処理
	InputMove();

	//移動処理
	Move();

	//当たり判定の処理
	Collision();

	if (GetMotionManager() != NULL)
	{//モーションマネージャーがNULLではない場合
		GetMotionManager()->Update(GetModel());
	}

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

	SetMtxWorld(pDevice);

	if (m_state == STATE_RESPAWN || m_state == STATE_STOP)
	{//リスポーン状態または停止状態の場合
		if (m_nCntState % 2 == 0)
		{
			//キャラクターの描画処理
			CCharacter::Draw();
		}
	}
	else
	{
		//キャラクターの描画処理
		CCharacter::Draw();
	}

#ifdef _DEBUG
	//オブジェクト3Dの描画処理
	CObject3D::Draw();
#endif // !_DEBUG
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
// プレイヤーの設置処理
//=============================================================================
void CPlayer::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

//=============================================================================
// プレイヤー番号の設置処理
//=============================================================================
void CPlayer::SetPlayerIdx(int nPlayerIdx)
{
	m_nPlayerIdx = nPlayerIdx;
}

//=============================================================================
// プレイヤー状態の設置処理
//=============================================================================
void CPlayer::SetState(STATE state)
{
	m_state = state;
}

//=============================================================================
// プレイヤー弾最大数の設置処理
//=============================================================================
void CPlayer::SetMaxBullet(int nMaxBullet)
{
	m_nMaxBullet = nMaxBullet;
}

//=============================================================================
// プレイヤー汚れているかどうか設置処理
//=============================================================================
void CPlayer::SetSplash(bool bSplash)
{
	m_bSplash = bSplash;
}

//=============================================================================
// プレイヤーの取得処理
//=============================================================================
CPlayer *CPlayer::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
// プレイヤー番号の取得処理
//=============================================================================
int CPlayer::GetPlayerIdx(void)
{
	return m_nPlayerIdx;
}

//=============================================================================
// プレイヤー状態の取得処理
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_state;
}

//=============================================================================
// プレイヤー汚れているかどうかの取得処理
//=============================================================================
bool CPlayer::GetSplash(void)
{
	return m_bSplash;
}

//=============================================================================
// プレイヤー弾最大数の取得処理
//=============================================================================
int CPlayer::GetMaxBullet(void)
{
	return m_nMaxBullet;
}

//=============================================================================
// 入力移動処理
//=============================================================================
void CPlayer::InputMove(void)
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

	float fDiffAngle = 0.0f;

	if (m_nPlayerIdx == 0)
	{
		if (m_state != STATE_STOP)
		{//停止状態ではない場合
			if (pInputKeyboard->GetPress(DIK_W) == true)
			{// キーボードの[W]キーが押された（上移動）
				move.z = PLAYER_MOVE;
				fDiffAngle = (D3DX_PI)-rot.y;
				SetNowRotInfo(ROT_UP);	//現在の向きを上にする
			}
			else if (pInputKeyboard->GetPress(DIK_S) == true)
			{// キーボードの[S]キーが押された（下移動）
				move.z = -PLAYER_MOVE;
				fDiffAngle = (D3DX_PI * 0) - rot.y;
				SetNowRotInfo(ROT_DOWN);	//現在の向きを下にする
			}
			else if (pInputKeyboard->GetPress(DIK_A) == true)
			{// キーボードの[A]キーが押された（左移動）
				move.x = -PLAYER_MOVE;
				fDiffAngle = (D3DX_PI * 0.5f) - rot.y;
				SetNowRotInfo(ROT_LEFT);	//現在の向きを左にする
			}
			else if (pInputKeyboard->GetPress(DIK_D) == true)
			{// キーボードの[D]キーが押された（右移動）
				move.x = PLAYER_MOVE;
				fDiffAngle = (D3DX_PI * -0.5f) - rot.y;
				SetNowRotInfo(ROT_RIGHT);	//現在の向きを右にする
			}
			SetDiffAngle(fDiffAngle);	//目的の向き設置処理

			if (pInputKeyboard->GetTrigger(DIK_1) == true)
			{//
				SwitchAbility();
			}
		}
	}
	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)
	{// キーボードの[SPACE]キーが押された（弾発射）
		CreateBullet();
	}
	SetMove(move);
}

//=============================================================================
// 移動処理
//=============================================================================
void CPlayer::Move(void)
{
	//移動量の取得
	D3DXVECTOR3 move = CCharacter::GetMove();

	//位置の取得
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//向きの取得
	D3DXVECTOR3 rot = CCharacter::GetRot();

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
	if (pos.x > ((MASS_SIZE_X * MASS_BLOCK_X) / 2) - 35.0f)
	{
		pos.x = ((MASS_SIZE_X * MASS_BLOCK_X) / 2) - 35.0f;
	}
	if (pos.x < -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + 35.0f)
	{
		pos.x = -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + 35.0f;
	}
	if (pos.z >((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) - 35.0f)
	{
		pos.z = ((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) - 35.0f;
	}
	if (pos.z < -((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) + 35.0f)
	{
		pos.z = -((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) + 35.0f;
	}

	if (move.x <= 0.1f && move.x >= -0.1f
		&& move.z <= 0.1f && move.z >= -0.1f)
	{// 移動していない
		if (m_motion == MOTION_MOVE)
		{
			m_motion = MOTION_NEUTAL;
			// 着地のモーションに切り替える
			GetMotionManager()->SwitchMotion(GetModel(), MOTION_NEUTAL);
		}
	}
	else
	{// 移動している
		if (m_motion != MOTION_MOVE)
		{
			m_motion = MOTION_MOVE;
			// モーション切り替え処理
			GetMotionManager()->SwitchMotion(GetModel(), MOTION_MOVE);
		}
	}

	//位置の設置処理
	SetPos(pos);
	//向き設置処理
	SetRot(rot);
	//移動の設置処理
	SetMove(move);
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
	CItem *pItem = NULL;                 // アイテムのポインタ

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
			else if (pScene->GetObjType() == OBJTYPE_ITEM)
			{
				pItem = (CItem*)pScene;
				if (pItem != NULL)
				{
					pBoxCollider = pItem->GetBoxCollider();
					if (pBoxCollider != NULL)
					{
						if (pBoxCollider->Collision(&pos, &posOld, &move, colRange / 2, NULL) == true)
						{
							pItem->Hit(this);
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
}

//=============================================================================
// 状態の処理
//=============================================================================
void CPlayer::State(void)
{
	switch (m_state)
	{
	case STATE_NOMAL:	//通常状態

		break;
	case STATE_STOP:	//停止状態
		m_nCntState++;	//状態カウンターの加算
		if (m_nCntState >= 80)
		{//状態カウンターが120以上の場合
			m_state = STATE_NOMAL;	//通常状態に戻す
			m_nCntState = 0;		//状態カウンターの初期化
		}
		break;
	case STATE_RESPAWN: //リスポーン状態
		m_nCntState++;	//状態カウンターの加算
		if (m_nCntState >= 120)
		{//状態カウンターが120以上の場合
			m_state = STATE_NOMAL;	//通常状態に戻す
			m_nCntState = 0;		//状態カウンターの初期化
		}
		break;
	case STATE_DEATH:	//死亡状態

		break;
	}
}

//=============================================================================
// 目的の向き設置処理
//=============================================================================
void CPlayer::SetDiffAngle(float fDiffAngle)
{
	//向きの取得
	D3DXVECTOR3 rot = CCharacter::GetRot();

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

	//向きの設置処理
	SetRot(rot);
}

//=============================================================================
// 弾の生成
//=============================================================================
void CPlayer::CreateBullet(void)
{
	//移動量の取得
	D3DXVECTOR3 move = CCharacter::GetMove();

	//位置の取得
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//向きの取得
	D3DXVECTOR3 rot = CCharacter::GetRot();

	bool bShoot = GetShoot();

	//弾の種類の切り替え処理
	CBullet::TYPE type;
	switch (m_nPlayerIdx)
	{
	case 0:
		type = CBullet::TYPE_PLAYER_0;
		break;
	case 1:
		type = CBullet::TYPE_PLAYER_1;
		break;
	}

	if (bShoot == false)
	{//弾がまだある場合
		if (m_ability == PLAYER_ABILITY_NOMAL)
		{
			switch (GetNowRotInfo())
			{
			case ROT_UP:	//上を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, 8.0f), type, this);
				break;
			case ROT_DOWN:	//下を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, -8.0f), type,this);
				break;
			case ROT_RIGHT:	//右を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(8.0f, 0.0f, 0.0f), type,this);
				break;
			case ROT_LEFT:	//左を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(-8.0f, 0.0f, 0.0f), type,this);
				break;

			}
		}
		else
		{
			switch (GetNowRotInfo())
			{
			case ROT_UP:	//上を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, 8.0f), type,this);
				break;
			case ROT_DOWN:	//下を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, -8.0f), type,this);
				break;
			case ROT_RIGHT:	//右を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(8.0f, 0.0f, 0.0f), type,this);
				break;
			case ROT_LEFT:	//左を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(-8.0f, 0.0f, 0.0f), type,this);
				break;

			}
		}
		SetShoot(true);		//弾の設置処理
	}
}

//=============================================================================
// 能力の切り替え処理
//=============================================================================
void CPlayer::SwitchAbility(void)
{
	if (m_nCntAbility < PLAYER_ABILITY_MAX)
	{//能力カウンターが能力が最大まで達していない場合
		m_nCntAbility++;
	}

	switch (m_nCntAbility)
	{
	case 0:
		m_ability = PLAYER_ABILITY_NOMAL;
		break;
	case 1:
		m_ability = PLAYER_ABILITY_SPEEDUP;
		break;
	case 2:
		m_ability = PLAYER_ABILITY_DOUBLEBULLET;
		break;
	case 3:
		m_ability = PLAYER_ABILITY_ALLBLOCKDESTROY;
		break;
	}

	switch (m_ability)
	{
	case PLAYER_ABILITY_NOMAL:
		break;
	case PLAYER_ABILITY_SPEEDUP:
		break;
	case PLAYER_ABILITY_DOUBLEBULLET:
		m_nMaxBullet = 2;
		//SetShoot(m_nMaxBullet);
		break;
	case PLAYER_ABILITY_ALLBLOCKDESTROY:
		//m_nMaxBullet = 2;
		m_bAllBlockDestroy = true;
		break;
	}
}

//=============================================================================
// 変数をクリアする処理
//=============================================================================
void CPlayer::ClearVariable(void)
{
	m_pPlayer = NULL;			//プレイヤーのポインタ
	m_state = STATE_NOMAL;		//状態
	m_nCntState = 0;			//状態カウンター
	m_nMaxBullet = 1;			//最大弾数
	m_nCntAbility = 0;			//能力カウンター
	m_bAllBlockDestroy = false;	//全てのブロックを壊せるかどうか
	m_bSplash = false;			//汚れているかどうか
	m_nCntSplash = 0;			//汚れカウンター
	m_motion = MOTION_NEUTAL;	//モーション情報
}