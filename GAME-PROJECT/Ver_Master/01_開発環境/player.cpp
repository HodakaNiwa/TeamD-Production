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
#include "river.h"
#include "icefield.h"
#include "meshfield.h"
#include "hinaarare.h"
#include "headquarters.h"
#include "map.h"
#include "modelcreate.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_MOVE                      (2.8f)     // プレイヤーの移動量
#define PLAYER_MOVE_POWERUP              (4.2f)     // パワーアップ時のプレイヤーの移動量
#define PLAYER_DEATH_EFFECT_IDX          (7)        // 死んだときのエフェクト番号
#define PLAYER_MOVE_EFFECT_IDX           (15)       // 移動している時のエフェクト番号
#define PLAYER_INVINCIBLE_EFFECT_IDX     (16)       // 無敵状態時のエフェクト番号
#define PLAYER_INVINCIBLE_EFFECT_APPEAR  (5)        // 無敵状態時のエフェクトを出す間隔
#define PLAYER_SE_BULLET_IDX             (6)        // 弾発射時の音番号
#define PLAYER_SE_DEATH_IDX              (18)       // 死んだときのエフェクト番号
#define PLAYER_SE_DAMAGE_IDX             (12)       // プレイヤーが動けなくなる攻撃をくらったときの音番号
#define PLAYER_COL_PRIORITY              (4)        // コリジョンを行う優先順位
#define PLAYER_INVINCIBLE_TIME           (1200)     // 無敵状態を維持する時間

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
	m_nMaxBullet = 0;			//最大弾数
	m_nCntAbility = 0;			//能力カウンター
	m_bAllBlockDestroy = false;	//全てのブロックを壊せるかどうか
	m_bSplash = false;			//汚れているかどうか
	m_nCntSplash = 0;			//汚れカウンター
	m_motion = MOTION_NEUTAL;	//モーション情報
	m_nCntBullet = 0;
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
		pPlayer = new CPlayer(nPriority);	//動的確保
		if (pPlayer != NULL)
		{//NULLでない場合
			pPlayer->SetPos(pos);										//位置の設置処理
			pPlayer->SetRot(rot);										//向きの設置処理
			pPlayer->SetModel(apModel);									//モデルの設置処理
			pPlayer->SetMotionManager(pMotionManager);					//モーションマネージャーの設置処理
			pPlayer->SetNumPart(nNumParts);								//パーツ総数の設置処理
			pPlayer->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));		//当たり判定の設置処理
			pPlayer->SetPlayerIdx(nPlayerIdx);							//プレイヤー番号の設置処理
			pPlayer->Init();											//初期化処理
			pPlayer->SetPlayer(pPlayer);								//プレイヤーポインタの設置処理
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

	SetAccel(PLAYER_MOVE);
	SetInertia(0.8f);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// 弾を打っていれば自身が消えることをお知らせ
	for (int nCntBullet = 0; nCntBullet < PLAYER_MAX_BULLET; nCntBullet++)
	{
		if (m_pBulletPlayer[nCntBullet] != NULL)
		{
			m_pBulletPlayer[nCntBullet]->SetParent(NULL);
		}
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if (CManager::GetGame() != NULL)
		{
			//能力の初期化
		/*	m_nCntAbility = 0;
			m_ability = PLAYER_ABILITY_NOMAL;*/
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
	if (pScene->GetObjType() == OBJTYPE_BULLET && m_bHelmet == false)
	{// 弾だったかつ無敵状態でない
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_ENEMY)
		{// 敵の弾だった
			// エフェクトを出す
			SetDeathEffect();

			// 終了処理
			Uninit();
		}
		else if(pBullet->GetType() != m_nPlayerIdx)
		{// 違うプレイヤーの弾だった
			m_state = STATE_STOP;
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// ゲーム画面だったら
				CGame *pGame = CManager::GetGame();
				if (pGame == NULL) { return; }
				pGame->HitBullet();
			}
			else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
			{// チュートリアル画面だったら
				CTutorial *pTutorial = CManager::GetTutorial();
				if (pTutorial == NULL) { return; }
			}
		}
	}
}

//=============================================================================
// プレイヤーの無敵状態のエフェクト生成処理
//=============================================================================
void CPlayer::SetInvincibleEffect(void)
{
	D3DXVECTOR3 EffectPos = GetPos();
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, PLAYER_INVINCIBLE_EFFECT_IDX);
	}
}

//=============================================================================
// プレイヤーの死んだときのエフェクト生成処理
//=============================================================================
void CPlayer::SetDeathEffect(void)
{
	D3DXVECTOR3 EffectPos = GetPos();
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, PLAYER_DEATH_EFFECT_IDX);
	}

	// 音を鳴らす
	CManager::GetSound()->PlaySound(PLAYER_SE_DEATH_IDX);
}

//=============================================================================
// プレイヤーが移動した時のエフェクト生成処理
//=============================================================================
void CPlayer::SetMoveEffect(void)
{
	D3DXVECTOR3 EffectPosDef = GetPos();
	D3DXVECTOR3 EffectPos = EffectPosDef;
	D3DXVECTOR3 EffectRot = GetRot();
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		EffectPos.x += sinf(EffectRot.y - (D3DX_PI * 0.5f)) * 20.0f + sinf(EffectRot.y) * 30.0f;
		EffectPos.z += cosf(EffectRot.y - (D3DX_PI * 0.5f)) * 20.0f + cosf(EffectRot.y) * 30.0f;
		pEffectManager->SetEffect(EffectPos, EffectRot, PLAYER_MOVE_EFFECT_IDX);
		EffectPos = EffectPosDef;
		EffectPos.x -= sinf(EffectRot.y - (D3DX_PI * 0.5f)) * 20.0f - sinf(EffectRot.y) * 30.0f;
		EffectPos.z -= cosf(EffectRot.y - (D3DX_PI * 0.5f)) * 20.0f - cosf(EffectRot.y) * 30.0f;
		pEffectManager->SetEffect(EffectPos, EffectRot, PLAYER_MOVE_EFFECT_IDX);
	}
}

//=============================================================================
// プレイヤーがアイテムを取得時の処理
//=============================================================================
void CPlayer::SwitchItem(CItem *pItem)
{
	//マップの取得
	CMap *pMap = CManager::GetBaseMode()->GetMap();

	//アイテムの種類別処理
	switch (pItem->GetType())
	{
	case CItem::TYPE_STAR:	//スターの場合
		break;

	case CItem::TYPE_GRENADE: //グレネードの場合
		break;

	case CItem::TYPE_1UP_TANK: //1UPの場合
		break;

	case CItem::TYPE_SCOOP: //スコップの場合
		if (pMap != NULL)
		{
			//司令部の取得
			CHeadQuarters *pHeadQuarters = pMap->GetHeadQuarters();

			int nVirtical = 0; //縦の配置個数の情報

			for (int nCntSide = 0; nCntSide < 6; nCntSide++)
			{//
				if (nCntSide <= 1 || nCntSide >= 4)
				{//1以下または4以上の場合
					nVirtical = 4;
				}
				else if (nCntSide >= 2 || nCntSide <= 3)
				{//2以下または3以上の場合
					nVirtical = 2;
				}

				for (int nCntVirtical = 0; nCntVirtical < nVirtical; nCntVirtical++)
				{//ブロックの生成
					CBlockScoop::Create(D3DXVECTOR3(-93.8f + (MASS_SIZE_X_HALF * nCntSide), 30.0f, -431.3f - (MASS_SIZE_Z_HALF * nCntVirtical)), INITIALIZE_D3DXVECTOR3,
						0, 0, pMap->GetModelCreate()->GetMesh(14), pMap->GetModelCreate()->GetBuffMat(14), pMap->GetModelCreate()->GetNumMat(14), pMap->GetModelCreate()->GetTexture(14)
						, MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF * 2, MASS_SIZE_Z_HALF);
				}
			}
		}
		break;

	case CItem::TYPE_CLOCK:	//時計の場合
		break;

	case CItem::TYPE_HELMET: //ヘルメットの場合
		m_bHelmet = true;
		break;
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
// プレイヤー状態管理カウンターの設置処理
//=============================================================================
void CPlayer::SetStateCounter(int nStateCounter)
{
	m_nCntState = nStateCounter;
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
// 全てのブロックを壊せるかどうかの設置処理
//=============================================================================
void CPlayer::SetAllBlockDestroy(bool bAllBlockDestroy)
{
	m_bAllBlockDestroy = bAllBlockDestroy;
}

//=============================================================================
// ヘルメットを使用しているかどうかの設置処理
//=============================================================================
void CPlayer::SetHelmet(bool bHelmet)
{
	m_bHelmet = bHelmet;
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
// 全てのブロックを壊せるかどうかの取得処理
//=============================================================================
bool CPlayer::GetAllBlockDestroy(void)
{
	return m_bAllBlockDestroy;
}

//=============================================================================
// ヘルメットを使用しているかどうか取得処理
//=============================================================================
bool CPlayer::GetHelmet(void)
{
	return m_bHelmet;
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
	float fAccel = GetAccel();

	// キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	// コントローラーの取得
	CXInput *pXInput = CManager::GetXInput();

	// アクション処理
	InputAction(pInputKeyboard, pXInput);

	// 動けない状態なら処理しない
	if (m_state == STATE_STOP) { return; }

	// キーボードでの移動処理
	bMoving = InputMove_Keyboard(&move, &fDiffAngle, rot, fAccel, pInputKeyboard);
	if (bMoving == false)
	{
		// コントローラーでの移動処理
		bMoving = InputMove_Controller(&move, &fDiffAngle, rot, fAccel, pXInput);
	}

	// 各種値の設定
	SetMove(move);
	SetDiffAngle(fDiffAngle);
}

//=============================================================================
// 入力アクション処理
//=============================================================================
void CPlayer::InputAction(CInputKeyboard *pKeyboard, CXInput *pXInput)
{
	// 弾発射の処理
	if (m_nPlayerIdx == 0 || CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// 0番のプレイヤーならば
		if (pKeyboard->GetTrigger(DIK_RETURN) == true ||
			pXInput->GetTrigger(m_nPlayerIdx, CXInput::XIJS_BUTTON_11) == true)
		{// 弾発射ボタン押下
			CreateBullet();
		}
	}
	else
	{// 1番のプレイヤーならば
		if (pKeyboard->GetTrigger(DIK_NUMPADENTER) == true)
		{// 弾発射ボタン押下
			CreateBullet();
		}
	}
}

//=============================================================================
// キーボードの入力移動処理
//=============================================================================
bool CPlayer::InputMove_Keyboard(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot, float fAccel, CInputKeyboard *pKeyboard)
{
	if (m_nPlayerIdx == 0 || CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// 0番のプレイヤーならば
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 上移動ボタン押下
			pMove->z = fAccel;
			*pDiffAngle = (D3DX_PI)-rot.y;
			SetNowRotInfo(ROT_UP);	//現在の向きを上にする
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 下移動ボタン押下
			pMove->z = -fAccel;
			*pDiffAngle = (D3DX_PI * 0) - rot.y;
			SetNowRotInfo(ROT_DOWN);	//現在の向きを下にする
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_A) == true)
		{// 左移動ボタン押下
			pMove->x = -fAccel;
			*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
			SetNowRotInfo(ROT_LEFT);	//現在の向きを左にする
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_D) == true)
		{// 右移動ボタン押下
			pMove->x = fAccel;
			*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
			SetNowRotInfo(ROT_RIGHT);	//現在の向きを右にする
			SetMoveEffect();
			return true;
		}
	}
	else
	{// 1番のプレイヤーならば
		if (pKeyboard->GetPress(DIK_UP) == true)
		{// 上移動ボタン押下
			pMove->z = fAccel;
			*pDiffAngle = (D3DX_PI)-rot.y;
			SetNowRotInfo(ROT_UP);	//現在の向きを上にする
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_DOWN) == true)
		{// 下移動ボタン押下
			pMove->z = -fAccel;
			*pDiffAngle = (D3DX_PI * 0) - rot.y;
			SetNowRotInfo(ROT_DOWN);	//現在の向きを下にする
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_LEFT) == true)
		{// 左移動ボタン押下
			pMove->x = -fAccel;
			*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
			SetNowRotInfo(ROT_LEFT);	//現在の向きを左にする
			SetMoveEffect();
			return true;
		}
		else if (pKeyboard->GetPress(DIK_RIGHT) == true)
		{// 右移動ボタン押下
			pMove->x = fAccel;
			*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
			SetNowRotInfo(ROT_RIGHT);	//現在の向きを右にする
			SetMoveEffect();
			return true;
		}
	}

	return false;
}

//=============================================================================
// コントローラーの入力移動処理
//=============================================================================
bool CPlayer::InputMove_Controller(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot, float fAccel, CXInput *pXInput)
{
	if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_16) == true)
	{// 上移動ボタン押下
		pMove->z = fAccel;
		*pDiffAngle = (D3DX_PI)-rot.y;
		SetNowRotInfo(ROT_UP);	//現在の向きを上にする
		SetMoveEffect();
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_17) == true)
	{// 下移動ボタン押下
		pMove->z = -fAccel;
		*pDiffAngle = (D3DX_PI * 0) - rot.y;
		SetNowRotInfo(ROT_DOWN);	//現在の向きを下にする
		SetMoveEffect();
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_18) == true)
	{// 左移動ボタン押下
		pMove->x = -fAccel;
		*pDiffAngle = (D3DX_PI * 0.5f) - rot.y;
		SetNowRotInfo(ROT_LEFT);	//現在の向きを左にする
		SetMoveEffect();
		return true;
	}
	else if (pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetPress(m_nPlayerIdx, CXInput::XIJS_BUTTON_19) == true)
	{// 右移動ボタン押下
		pMove->x = fAccel;
		*pDiffAngle = (D3DX_PI * -0.5f) - rot.y;
		SetNowRotInfo(ROT_RIGHT);	//現在の向きを右にする
		SetMoveEffect();
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

	//慣性量の取得
	float fInertia = CCharacter::GetInertia();

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
	if (GetNowRotInfo() == ROT_UP ||
		GetNowRotInfo() == ROT_DOWN)
	{//向きが前後の場合
		move.x = 0.0f;							//斜め移動をさせないためにx軸の移動量を0にする
		move.z += (0.0f - move.z) * fInertia;	//z軸の移動量加算処理
	}
	else if (GetNowRotInfo() == ROT_RIGHT ||
		GetNowRotInfo() == ROT_LEFT)
	{
		move.z = 0.0f;							//斜め移動をさせないためにz軸の移動量を0にする
		move.x += (0.0f - move.x) * fInertia;	//x軸の移動量加算処理
	}

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
	bool bLand = false;					 // 着地しているかどうか
	bool bIceLand = false;				 // 氷の上にいるかどうか

	for (int nCntPriority = 0; nCntPriority < PLAYER_COL_PRIORITY + 1; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_3D)
			{// オブジェクト3Dだったら
				bLand = CollisionObject3D(&pos, &posOld, &move, colRange, (CObject3D*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_BULLET)
			{// 弾だったら
				bLand = CollisionBullet(&pos, &posOld, &move, colRange / 2, (CBullet*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// 敵だったら
				bLand = CollisionEnemy(&pos, &posOld, &move, colRange / 2, (CEnemy*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_BLOCK)
			{// ブロックだったら
					bLand = CollisionBlock(&pos, &posOld, &move, colRange / 2, (CBlock*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_ITEM)
			{// アイテムだったら
				CItem *pItem = (CItem*)pScene;

				if (CollisionItem(&pos, &posOld, &move, colRange, pItem) == true)
				{// アイテムに当たっている
					bLand = true;

					//アイテム取得時の処理
					SwitchItem(pItem);
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_GOALCYLINDER)
			{// ゴール円筒だったら
				CollisionGoalCylinder(&pos, (CGoalCylinder*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_RIVER)
			{// 川だったら
				bLand = CollisionRiver(&pos, &posOld, &move, colRange, (CRiver*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_ICEFIELD)
			{// 氷だったら
				CollisionIceField(pos, (CIceField*)pScene, &bIceLand);
			}
			else if (pScene->GetObjType() == OBJTYPE_HEADQUARTERS)
			{// 司令部だったら
				bLand = CollisionHeadQuarters(&pos, &posOld, &move, colRange, (CHeadQuarters*)pScene);
			}
			else if (pScene->GetObjType() == OBJTYPE_HINAARARE)
			{// ひなあられだったら
				bLand = CollisionHinaarare(&pos, &posOld, &move, colRange, (CHinaarare*)pScene);
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}
	switch (bLand)
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
	// 無敵状態時の処理
	if (m_bHelmet == true)
	{
		// エフェクトを出す
		if (m_nCntHelmet % PLAYER_INVINCIBLE_EFFECT_APPEAR == 0)
		{
			SetInvincibleEffect();
		}

		// カウンター増加
		m_nCntHelmet++;
		if (m_nCntHelmet >= PLAYER_INVINCIBLE_TIME)
		{
			m_bHelmet = false;
			m_nCntHelmet = 0;
		}
	}

	switch (m_state)
	{
	case STATE_NOMAL:	//通常状態

		break;
	case STATE_STOP:	//停止状態
		m_nCntState++;	//状態カウンターの加算
		if (m_nCntState == 1)
		{// 停止状態になってからすぐの時に音を鳴らす
			CManager::GetSound()->PlaySound(PLAYER_SE_DAMAGE_IDX);
		}
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
	//位置の取得
	D3DXVECTOR3 pos = CCharacter::GetPos();

	//向きの取得
	D3DXVECTOR3 rot = CCharacter::GetRot();

	// 弾を打っているか取得
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


	int nGetCntBullet = GetCntBullet();
	if (nGetCntBullet <= m_nMaxBullet)
	{
		// 音を鳴らす
		CManager::GetSound()->PlaySound(PLAYER_SE_BULLET_IDX);

		// 移動量を設定
		D3DXVECTOR3 BulletMove;

		if (m_ability == PLAYER_ABILITY_NOMAL)
		{
			switch (GetNowRotInfo())
			{
			case ROT_UP:	//上を向いている場合
				BulletMove = D3DXVECTOR3(0.0f, 0.0f, 8.0f);
				break;
			case ROT_DOWN:	//下を向いている場合
				BulletMove = D3DXVECTOR3(0.0f, 0.0f, -8.0f);
				break;
			case ROT_RIGHT:	//右を向いている場合
				BulletMove = D3DXVECTOR3(8.0f, 0.0f, 0.0f);
				break;
			case ROT_LEFT:	//左を向いている場合
				BulletMove = D3DXVECTOR3(-8.0f, 0.0f, 0.0f);
				break;
			}
		}
		else
		{
			switch (GetNowRotInfo())
			{
			case ROT_UP:	//上を向いている場合
				BulletMove = D3DXVECTOR3(0.0f, 0.0f, 15.0f);
				break;
			case ROT_DOWN:	//下を向いている場合
				BulletMove = D3DXVECTOR3(0.0f, 0.0f, -15.0f);
				break;
			case ROT_RIGHT:	//右を向いている場合
				BulletMove = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
				break;
			case ROT_LEFT:	//左を向いている場合
				BulletMove = D3DXVECTOR3(-15.0f, 0.0f, 0.0f);
				break;
			}
		}

		// 弾を生成
		m_pBulletPlayer[nGetCntBullet] = CBulletPlayer::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), INITIALIZE_D3DXVECTOR3, BulletMove, type, this);

		//弾の数設置処理
		SetCntBullet(nGetCntBullet + 1);
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

	// アビリティ上昇
	m_ability = (PLAYER_ABILITY)m_nCntAbility;

	switch (m_ability)
	{
	case PLAYER_ABILITY_NOMAL:
		break;
	case PLAYER_ABILITY_BULLET_SPEEDUP:
		break;
	case PLAYER_ABILITY_MOVE_SPEEDUP:
		SetAccel(PLAYER_MOVE_POWERUP);
		break;
	case PLAYER_ABILITY_DOUBLEBULLET:
		m_nMaxBullet = PLAYER_MAX_BULLET - 1;	//最大弾数を２発にする
		break;
	case PLAYER_ABILITY_ALLBLOCKDESTROY:
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
	m_nMaxBullet = 0;			//最大弾数
	m_nCntAbility = 0;			//能力カウンター
	m_bAllBlockDestroy = false;	//全てのブロックを壊せるかどうか
	m_bSplash = false;			//汚れているかどうか
	m_nCntSplash = 0;			//汚れカウンター
	m_motion = MOTION_NEUTAL;	//モーション情報
	m_ability = PLAYER_ABILITY_NOMAL;	//能力の情報
	m_bHelmet = false;			//ヘルメットを使用しているかどうか
	m_nCntHelmet = 0;			//ヘルメットカウンター
}

//=============================================================================
// オブジェクト3D当たり判定の処理
//=============================================================================
bool CPlayer::CollisionObject3D(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CObject3D *pObject3D)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pObject3D->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos,pPosOld,pMove,colRange) == true)
		{
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// 弾当たり判定の処理
//=============================================================================
bool CPlayer::CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet)
{
	bool bLand = false;
	if (CTitle::GetGameMode() != CTitle::GAMEMODE_ONLINE2P) { return false; }

	CBoxCollider *pBoxCollider = pBullet->GetBoxCollider();
	if (pBoxCollider == NULL) { return false; }

	int nBulletType = pBullet->GetType();
	if (nBulletType != CBulletPlayer::TYPE_PLAYER_0 && nBulletType != CBulletPlayer::TYPE_PLAYER_1) { return false; }


	if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange) == true)
	{
		if (nBulletType != m_nPlayerIdx)
		{// 自分以外が放った種類の弾に当たった
			Hit(pBullet);
		}
		bLand = true;
	}

	return bLand;
}

//=============================================================================
// 敵当たり判定の処理
//=============================================================================
bool CPlayer::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pEnemy->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange) == true)
		{
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// ブロック当たり判定の処理
//=============================================================================
bool CPlayer::CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pBlock->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange) == true)
		{
			pBlock->Hit(this);
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// アイテム当たり判定の処理
//=============================================================================
bool CPlayer::CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CItem *pItem)
{
	bool bLand = false;

	CBoxCollider *pBoxCollider = pItem->GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange / 2, NULL) == true)
		{
			pItem->Hit(this);
			bLand = true;
		}
	}
	return bLand;
}

//=============================================================================
// ゴール円筒当たり判定の処理
//=============================================================================
void CPlayer::CollisionGoalCylinder(D3DXVECTOR3 *pPos, CGoalCylinder *pGoalCylinder)
{
	if (pGoalCylinder->Collision(pPos) == true)
	{
		if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{
			CTutorial *pTutorial = CManager::GetTutorial();
			if (pTutorial->GetState() != CTutorial::STATE_END)
			{
				pTutorial->SetGoalPlayerIdx(m_nPlayerIdx);
				pTutorial->SetState(CTutorial::STATE_END);
			}
		}
	}
}

//=============================================================================
// 川当たり判定の処理
//=============================================================================
bool CPlayer::CollisionRiver(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CRiver *pRiver)
{
	bool bLand = false;
	if (pRiver != NULL)
	{
		CBoxCollider *pBoxCollider = pRiver->GetBoxCollider();
		if (pBoxCollider != NULL)
		{
			if (pBoxCollider->Collision(pPos,pPosOld,pMove, colRange / 2, NULL) == true)
			{
				bLand = true;
			}
		}
	}
	return bLand;
}

//=============================================================================
// 氷当たり判定の処理
//=============================================================================
void CPlayer::CollisionIceField(D3DXVECTOR3 pos, CIceField *pIceField, bool *pLandIce)
{
	bool  bIceLand = false;
	//メッシュフィールドの取得
	CMeshField *pMeshField = pIceField->GetMeshField();
	if (pMeshField != NULL)
	{//NULLではない場合
	 //メッシュフィールドのワールド座標における高さ取得する処理
		pMeshField->GetPolyHeight(pos, &bIceLand, NULL);

		//氷の上にいるかどうか別の処理
		switch (bIceLand)
		{
		case false:	//氷の上にいない
			if (*pLandIce == false)
			{
				SetInertia(0.8f);
			}
			break;
		case true:	//氷の上にいる
			*pLandIce = true;
			SetInertia(0.05f);
			break;
		}
	}

}

//=============================================================================
// 司令部当たり判定の処理
//=============================================================================
bool CPlayer::CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHeadQuarters)
{
	bool bLand = false;
	if (pHeadQuarters != NULL)
	{
		CBoxCollider *pBoxCollider = pHeadQuarters->GetBoxCollider();
		if (pBoxCollider != NULL)
		{
			if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange / 2, NULL) == true)
			{
				bLand = true;
			}
		}
	}
	return bLand;
}

//=============================================================================
// ひなあられ当たり判定処理
//=============================================================================
bool CPlayer::CollisionHinaarare(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHinaarare *pHinaarare)
{
	bool bLand = false;
	if (pHinaarare != NULL)
	{
		CBoxCollider *pBoxCollider = pHinaarare->GetBoxCollider();
		if (pBoxCollider != NULL)
		{
			if (pBoxCollider->Collision(pPos, pPosOld, pMove, colRange / 2, NULL) == true)
			{
				pHinaarare->Hit(this);
				if (m_bHelmet == false)
				{
					SetState(STATE_STOP);
					bLand = true;
				}
			}
		}
	}
	return bLand;
}