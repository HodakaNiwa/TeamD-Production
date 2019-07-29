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
#include "motion.h"
#include "goalCylinder.h"
#include "tutorial.h"
#include "title.h"
#include "effectManager.h"
#include "server.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MOVE (2.5f)
#define PLAYER_DEATH_EFFECT_IDX (7)
#define PLAYER_SE_BULLET_IDX (6)

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
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerIdx, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, int nPriority)
{
	CPlayer *pPlayer = NULL;				//プレイヤーのポインタ

	if (pPlayer == NULL)
	{//NULLの場合
		pPlayer = new CPlayer(nPriority);				//動的確保
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
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (CManager::GetGame() != NULL)
		{
			CManager::GetGame()->DeletePlayer(NULL, m_nPlayerIdx);
		}
	}
	if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		if (CManager::GetTutorial() != NULL)
		{
			CManager::GetTutorial()->DeletePlayer(NULL, m_nPlayerIdx);
		}
	}

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

	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム画面だったら
		// 入力による移動量計算処理
		InputMove_Game();

		//移動処理
		Move();

		if (GetMotionManager() != NULL)
		{//モーションマネージャーがNULLではない場合
			GetMotionManager()->Update(GetModel());
		}

		//当たり判定の処理
		Collision();
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		// 入力による移動量計算処理
		InputMove_Tutorial();

		//移動処理
		Move();

		if (GetMotionManager() != NULL)
		{//モーションマネージャーがNULLではない場合
			GetMotionManager()->Update(GetModel());
		}

		//当たり判定の処理
		Collision();
	}

	// 状態の処理
	State();

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
		    // エフェクトを出す
			CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
			if (pEffectManager != NULL)
			{
				pEffectManager->SetEffect(GetPos(), INITIALIZE_D3DXVECTOR3, PLAYER_DEATH_EFFECT_IDX);
			}

			Uninit();
		}
		else if(pBullet->GetType() != m_nPlayerIdx)
		{// 違うプレイヤーの弾だった
			m_state = STATE_STOP;
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
// ゲーム画面の入力移動処理
//=============================================================================
void CPlayer::InputMove_Game(void)
{
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// オンラインプレイだったら
		if (m_nPlayerIdx == CManager::GetClient()->GetClientId())
		{
			InputMove();
		}
	}
	else
	{// オンラインプレイじゃなかったら
		InputMove();
	}
}

//=============================================================================
// チュートリアル画面の入力移動処理
//=============================================================================
void CPlayer::InputMove_Tutorial(void)
{
	CTutorial *pTutorial = CManager::GetTutorial();
	if (pTutorial == NULL) { return; }

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// オンラインプレイだったら
		if (m_nPlayerIdx == CManager::GetClient()->GetClientId() && pTutorial->GetState() != CTutorial::STATE_END)
		{
			InputMove();
		}
	}
	else
	{// オンラインプレイじゃなかったら
		if (pTutorial->GetState() != CTutorial::STATE_END)
		{
			InputMove();
		}
	}
}

//=============================================================================
// 入力移動処理
//=============================================================================
void CPlayer::InputMove(void)
{
	// 各種値の取得
	D3DXVECTOR3 rot = CCharacter::GetRot();
	D3DXVECTOR3 move = CCharacter::GetMove();
	float fDiffAngle = 0.0f;
	bool bMoving = false;

	// 動けない状態なら処理しない
	if (m_state == STATE_STOP) { return; }

	// キーボードでの移動処理
	bMoving = InputMove_Keyboard(&move, &fDiffAngle, rot);
	if (bMoving == false)
	{
		// コントローラーでの移動処理
		bMoving = InputMove_Controller(&move, &fDiffAngle, rot);
	}

	// 各種値の設定
	SetMove(move);
	SetDiffAngle(fDiffAngle);
}

//=============================================================================
// キーボードの入力移動処理
//=============================================================================
bool CPlayer::InputMove_Keyboard(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot)
{
	// キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	if (m_nPlayerIdx == 0 || CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// 0番のプレイヤーならば
	    // 弾発射の処理
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{// 弾発射ボタン押下
			CreateBullet();
		}

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{// 上移動ボタン押下
			pMove->z = PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI)-rot.y;
			SetNowRotInfo(ROT_UP);	//現在の向きを上にする
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{// 下移動ボタン押下
			pMove->z = -PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI * 0) - rot.y;
			SetNowRotInfo(ROT_DOWN);	//現在の向きを下にする
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_A) == true)
		{// 左移動ボタン押下
			pMove->x = -PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
			SetNowRotInfo(ROT_LEFT);	//現在の向きを左にする
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{// 右移動ボタン押下
			pMove->x = PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
			SetNowRotInfo(ROT_RIGHT);	//現在の向きを右にする
			return true;
		}
	}
	else
	{// 1番のプレイヤーならば
	 // 弾発射の処理
		if (pInputKeyboard->GetTrigger(DIK_NUMPADENTER) == true)
		{// 弾発射ボタン押下
			CreateBullet();
		}

		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{// 上移動ボタン押下
			pMove->z = PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI)-rot.y;
			SetNowRotInfo(ROT_UP);	//現在の向きを上にする
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_DOWN) == true)
		{// 下移動ボタン押下
			pMove->z = -PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI * 0) - rot.y;
			SetNowRotInfo(ROT_DOWN);	//現在の向きを下にする
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_LEFT) == true)
		{// 左移動ボタン押下
			pMove->x = -PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
			SetNowRotInfo(ROT_LEFT);	//現在の向きを左にする
			return true;
		}
		else if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
		{// 右移動ボタン押下
			pMove->x = PLAYER_MOVE;
			*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
			SetNowRotInfo(ROT_RIGHT);	//現在の向きを右にする
			return true;
		}
	}

	return false;
}

//=============================================================================
// コントローラーの入力移動処理
//=============================================================================
bool CPlayer::InputMove_Controller(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot)
{
	// コントローラーの取得
	CXInput *pXInput = CManager::GetXInput();
	if (pXInput == NULL) { return false; }

	// 弾発射の処理
	if (pXInput->GetTrigger(m_nPlayerIdx, CXInput::XIJS_BUTTON_11) == true)
	{// 弾発射ボタン押下
		CreateBullet();
	}

	if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_16) == true)
	{// 上移動ボタン押下
		pMove->z = PLAYER_MOVE;
		*pDiffAngle = (D3DX_PI)-rot.y;
		SetNowRotInfo(ROT_UP);	//現在の向きを上にする
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_17) == true)
	{// 下移動ボタン押下
		pMove->z = -PLAYER_MOVE;
		*pDiffAngle = (D3DX_PI * 0) - rot.y;
		SetNowRotInfo(ROT_DOWN);	//現在の向きを下にする
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_18) == true)
	{// 左移動ボタン押下
		pMove->x = -PLAYER_MOVE;
		*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
		SetNowRotInfo(ROT_LEFT);	//現在の向きを左にする
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_19) == true)
	{// 右移動ボタン押下
		pMove->x = PLAYER_MOVE;
		*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
		SetNowRotInfo(ROT_RIGHT);	//現在の向きを右にする
		return true;
	}

	return false;
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
			// モーション切り替え処理
			if (GetMotionManager() != NULL)
			{
				GetMotionManager()->SwitchMotion(GetModel(), MOTION_NEUTAL);
			}
		}
	}
	else
	{// 移動している
		if (m_motion != MOTION_MOVE)
		{
			m_motion = MOTION_MOVE;
			// モーション切り替え処理
			if (GetMotionManager() != NULL)
			{
				GetMotionManager()->SwitchMotion(GetModel(), MOTION_MOVE);
			}
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
	CGoalCylinder *pGoal = NULL;         // ゴール用円筒のポインタ

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
						}
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
						}
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
						}
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
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_GOALCYLINDER)
			{
				pGoal = (CGoalCylinder*)pScene;
				if (pGoal != NULL)
				{
					if (pGoal->Collision(&pos) == true)
					{
						if (CManager::GetMode() == CManager::MODE_TUTORIAL)
						{
							CTutorial *pTutorial = CManager::GetTutorial();
							if (pTutorial->GetState() != CTutorial::STATE_END)
							{
								pTutorial->SetState(CTutorial::STATE_END);
							}
						}
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
		// 音を鳴らす
		CManager::GetSound()->PlaySound(PLAYER_SE_BULLET_IDX);

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
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, 8.0f), type, this);
				break;
			case ROT_DOWN:	//下を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(0.0f, 0.0f, -8.0f), type, this);
				break;
			case ROT_RIGHT:	//右を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(8.0f, 0.0f, 0.0f), type, this);
				break;
			case ROT_LEFT:	//左を向いている場合
				CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, D3DXVECTOR3(-8.0f, 0.0f, 0.0f), type, this);
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