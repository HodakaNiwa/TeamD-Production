//=============================================================================
//
// 敵の処理 [enemy.cpp]
// Author : Hodaka Niwa
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
#include "bullet.h"
#include "player.h"
#include "block.h"
#include "server.h"
#include "headquarters.h"
#include "river.h"
#include "game.h"
#include "item.h"
#include "tutorial.h"
#include "title.h"
#include "effectManager.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_SPAWN_EFFECT_IDX   (3)     // 敵生成時のエフェクト番号
#define ENEMY_DEATH_EFFECT_IDX   (7)     // 敵死亡時のエフェクト番号
#define ENEMY_SPAWN_SE_IDX       (15)    // 敵生成時の音番号
#define ENEMY_DEATH_SE_IDX       (16)    // 敵死亡時の音番号
#define ENEMY_AI_NONE_TIME       (2)     // 動きを止めてから動き出すまでの時間
#define ENEMY_AI_MOVE_TIME       (120)   // 動いてから動きを止められるまでの時間
#define ENEMY_MASS_COLRANGE      (2.0f)  // マスの範囲内と判定される範囲
#define ENEMY_LIFE               (1)     // 敵の体力
#define ENEMY_HEAVY_LIFE         (3)     // ヘビータンクの体力
#define ENEMY_FAST_BULLETMOVE_UP (1.2f)  // 速射砲タンクの弾の速さを上げる倍率
#define ENEMY_COL_PRIORITY       (4)     // コリジョンを行う優先順位

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int CEnemy::m_nNumAll = 0;
int CEnemy::m_nSpawnCounter = 0;


//*****************************************************************************
// CEnemyの処理
//*****************************************************************************
//=============================================================================
// 敵のコンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{

}

//=============================================================================
// 敵のデストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// 敵の生成
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEnemy *pEnemy = NULL;

	if (pEnemy == NULL)
	{// NULLの場合
		pEnemy = new CEnemy(nPriority);
		if (pEnemy != NULL)
		{// NULLでない場合
			pEnemy->SetPos(pos);
			pEnemy->SetRot(rot);
			pEnemy->SetColRange(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pEnemy->SetReturnFlag(true);
			pEnemy->Init();
		}
	}
	return pEnemy;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemy::Init(void)
{
	// 変数をクリアする
	ClearVariable();

	// 番号の設定
	SetIdx(m_nSpawnCounter);

	// 総数を増やす
	m_nNumAll++;

	// スポーン数を増やす
	m_nSpawnCounter++;

	// 体力を設定
	m_nLife = ENEMY_LIFE;

	// キャラクターの初期化処理
	if (FAILED(CCharacter::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	// 総数を減らす
	m_nNumAll--;

	// 敵の数を減らす
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		int nNumEnemy = pGame->GetNumEnemy();
		nNumEnemy--;
		pGame->SetNumEnemy(nNumEnemy);
		CClient *pClient = CManager::GetClient();
		if (nNumEnemy <= 0)
		{// 敵がもういない
			if (CTitle::GetGameMode() != CTitle::GAMEMODE_ONLINE2P || pClient == NULL)
			{// ローカルプレイならば終了状態に
				pGame->SetState(CGame::STATE_END);
				m_nSpawnCounter = 0;
			}
			else if(pClient != NULL && pClient->GetClientId() == 0)
			{// ホストだったら終了状態に
				pGame->SetState(CGame::STATE_END);
				m_nSpawnCounter = 0;
			}
		}
		else
		{// 敵がまだいる
			// 自身のクライアント番号を取得
			int nIdxClient = 0;
			CClient *pClient = CManager::GetClient();
			if (pClient != NULL)
			{
				nIdxClient = pClient->GetClientId();
			}

			// アイテムを生成する
			if (m_bItem == true && nIdxClient == 0)
			{
				pGame->CreateItem(GetPos(), INITIALIZE_D3DXVECTOR3, m_nItemType);
			}
		}
	}

	// 終了処理
	CCharacter::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemy::Update(void)
{
	// 前回の位置を更新
	SetPosOld(GetPos());

	// ホストだったら敵のAI処理をする
	if (CManager::GetClient() != NULL)
	{
		if (CManager::GetClient()->GetClientId() == 0)
		{
			HostUpdate();
		}
		else if(CManager::GetClient()->GetConnected() == false)
		{
			HostUpdate();
		}
	}
	else
	{
		HostUpdate();
	}

	// 当たり判定用箱モデルを動かす
	if (GetBoxCollider() != NULL)
	{
		GetBoxCollider()->SetPos(GetPos());
	}

	// 当たり判定の処理
	Collision();
}

//=============================================================================
// ホストだった時の更新処理
//=============================================================================
void CEnemy::HostUpdate(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// ゲーム画面で敵が動ける状態である
		if (CManager::GetGame()->GetEnemyMove() == true)
		{
			// AIの処理
			AI();

			// 移動量加算処理
			Movement();
		}
	}
	else
	{// ゲーム画面ではない
		// AIの処理
		AI();

		// 移動量加算処理
		Movement();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// ワールドマトリックス設定処理
	SetMtxWorld(pDevice);

	// キャラクターの描画処理
	CCharacter::Draw();
}

//=============================================================================
// オブジェクトにヒットしたときの処理
//=============================================================================
void CEnemy::Hit(CScene * pScene)
{
	// 当たったオブジェクトによって処理わけ
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// 弾だった
		CBullet *pBullet = (CBullet*)pScene;
		if (pBullet->GetType() == CBullet::TYPE_PLAYER_0 || pBullet->GetType() == CBullet::TYPE_PLAYER_1)
		{// プレイヤーの弾だった
			// 体力を減らす
			m_nLife--;
			if (m_nLife <= 0)
			{// 体力がもうない
			    // 死亡時処理
				Destroy();

				// スコアを加算する
				ScoreUp(pBullet);
			}
			else
			{// 体力がまだある
				// パーツを削る
				DeleteParts();
			}
		}
	}
}

//=============================================================================
// スコアを加算させる処理
//=============================================================================
void CEnemy::ScoreUp(CBullet *pBullet)
{
	if (pBullet == NULL) { return; }

	// スコアをアップさせるプレイヤーの番号を設定
	int nPlayerIdx = pBullet->GetType();

	// スコアを加算する
	if (CManager::GetMode() != CManager::MODE_GAME) { return; }
	CGame *pGame = CManager::GetGame();
	if (pGame == NULL) { return; }

	switch (m_Type)
	{// 自身の種類によって処理わけ
	case TYPE_NORMAL:
		pGame->ScoreUp_NormalEnemy(nPlayerIdx);
		break;
	case TYPE_ARMORE:
		pGame->ScoreUp_ArmoreEnemy(nPlayerIdx);
		break;
	case TYPE_FAST:
		pGame->ScoreUp_FastEnemy(nPlayerIdx);
		break;
	case TYPE_HEAVY:
		pGame->ScoreUp_HeavyEnemy(nPlayerIdx);
		break;
	}
}

//=============================================================================
// 敵のパーツを削る処理
//=============================================================================
void CEnemy::DeleteParts(void)
{
	// パーツ数を取得
	int nNumParts = GetNumPart();

	// モデルクラスへのポインタを破棄する
	CModel **pModel = GetModel();
	if (pModel == NULL || nNumParts <= 0) { return; }
	pModel[nNumParts - 1]->Uninit();
	delete pModel[nNumParts - 1];
	pModel[nNumParts - 1] = NULL;

	// パーツ数を一つ減らす
	SetNumPart(nNumParts - 1);
}

//=============================================================================
// AIの処理
//=============================================================================
void CEnemy::AI(void)
{
	switch (m_AIState)
	{// 状態によって処理わけ
	case AI_STATE_NONE:
		AI_None();
		break;
	case AI_STATE_CHANGEROT:
		AI_ChangeRot();
		break;
	case AI_STATE_MOVE:
		AI_AutoMove();
		break;
	}
}

//=============================================================================
// AIの移動処理
//=============================================================================
void CEnemy::AI_None(void)
{
	// 移動量を無くしておく
	SetMove(INITIALIZE_D3DXVECTOR3);

	m_nAI_StateCounter++;
	if (m_nAI_StateCounter % ENEMY_AI_NONE_TIME == 0)
	{// ある程度時間が止まった
		// 次に移動する方向を決定する
		SetNextMoveDirection();

		m_AIState = AI_STATE_CHANGEROT;
		m_nAI_StateCounter = 0;
	}
}

//=============================================================================
// AIの次に移動する方向を決定する処理
//=============================================================================
void CEnemy::SetNextMoveDirection(void)
{
	int nNextRot = m_RotInfoOld;  // 次に移動する方向

	// ランダムの評価値から下向きに移動するか決定
	while (m_RotInfoOld == nNextRot)
	{
		nNextRot = rand() % (m_nAI_DownEva + 1);
	}
	if (nNextRot == 0)
	{// 下向きに移動するならば
		SetNowRotInfo(ROT_DOWN);
	}
	else
	{
		// 次に移動する方向をランダムで決定
		while (nNextRot == ROT_DOWN)
		{
			nNextRot = rand() % ROT_MAX;
			if (m_RotInfoOld == nNextRot)
			{
				nNextRot = ROT_DOWN;
			}
		}
		SetNowRotInfo((NOW_ROT_INFO)nNextRot);
	}

	// 次の目的地のマスを計算
	CalcDestMass();
}

//=============================================================================
// 次の目的地のマス目を算出する
//=============================================================================
void CEnemy::CalcDestMass(void)
{
	// 現在いるマス目を算出
	D3DXVECTOR3 pos = GetPos();
	int nMassX = (int)((pos.x + (MASS_SIZE_X * MASS_BLOCK_X / 2)) / MASS_SIZE_X) + 1;
	int nMassZ = (int)((-pos.z + (MASS_SIZE_Z * MASS_BLOCK_Z / 2)) / MASS_SIZE_Z) + 1;

	// 次に向かう向きから目的のマスを決定する
	NOW_ROT_INFO nowRot = GetNowRotInfo();
	switch (GetNowRotInfo())
	{// 現在の向き情報によって処理わけ
	case ROT_UP:	// 上方向に向いている
		nMassZ--;
		break;
	case ROT_DOWN:	// 下方向に向いている
		nMassZ++;
		break;
	case ROT_LEFT:	// 左方向に向いている
		nMassX--;
		break;
	case ROT_RIGHT:	// 右方向に向いている
		nMassX++;
		break;
	}

	// 目的のマスを決定
	m_nAI_DestMassX = nMassX;
	m_nAI_DestMassZ = nMassZ;
}

//=============================================================================
// AIの移動処理
//=============================================================================
void CEnemy::AI_ChangeRot(void)
{
	// 移動量を無くしておく
	SetMove(INITIALIZE_D3DXVECTOR3);

	// 各種値の取得
	D3DXVECTOR3 rot = GetRot();

	float fDiffAngle = 0.0f;
	float fDestAngle = 0.0f;
	switch (GetNowRotInfo())
	{// 現在の向き情報によって処理わけ
	case ROT_UP:	// 上方向に向いている
		fDestAngle = D3DX_PI;
		break;
	case ROT_DOWN:	// 下方向に向いている
		fDestAngle = 0.0f;
		break;
	case ROT_LEFT:	// 左方向に向いている
		fDestAngle = (D3DX_PI * 0.5f);
		break;
	case ROT_RIGHT:	// 右方向に向いている
		fDestAngle = (D3DX_PI * -0.5f);
		break;
	}

	// 差分の向きを求める
	fDiffAngle = fDestAngle - rot.y;

	// 差分の補正
	if (fDiffAngle > D3DX_PI)
	{
		fDiffAngle -= D3DX_PI * 2.0f;
	}
	if (fDiffAngle < -D3DX_PI)
	{
		fDiffAngle += D3DX_PI * 2.0f;
	}

	// 向きの加算
	rot.y += fDiffAngle * GetRivisionRot();
	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}

	// 目的の向きに達していたら動き出す
	if (rot.y <= fDestAngle + 0.2f && rot.y >= fDestAngle - 0.2f)
	{
		rot.y = fDestAngle;
		m_AIState = AI_STATE_MOVE;
	}
	if (fDestAngle == D3DX_PI && rot.y >= -D3DX_PI - 0.2f && rot.y <= D3DX_PI + 0.2f)
	{
		rot.y = fDestAngle;
		m_AIState = AI_STATE_MOVE;
	}

	// 向きを設定する
	SetRot(rot);
}

//=============================================================================
// AIの移動処理
//=============================================================================
void CEnemy::AI_AutoMove(void)
{
	// 各種値の取得
	D3DXVECTOR3 move = GetMove();

	// 移動の種類別処理
	switch (GetNowRotInfo())
	{
	case ROT_UP:	// 上方向に向いている
		move.z = GetAccel();
		break;
	case ROT_DOWN:	// 下方向に向いている
		move.z = -GetAccel();
		break;
	case ROT_LEFT:	// 左方向に向いている
		move.x = -GetAccel();
		break;
	case ROT_RIGHT:	// 右方向に向いている
		move.x = GetAccel();
		break;
	}

	// 各種値の設定
	SetMove(move);

	// 弾を打つ処理
	if (rand() % m_nAI_BulletEva == 0)
	{// ランダムの評価値で決定
		SetBullet();
	}

	// 次のマスに到達しているかどうか判定
	if (CheckNextMovingMass() == true)
	{// 次のマスに到達している
		m_nAI_MassCount++;  // マスに移動した回数を加算

		// 次に向かう方向を変えるかどうかをランダムの評価値から算出
		int nChangeMoving = 0;
		int nMassRandom = m_nAI_MassEva - m_nAI_MassCount;
		if (nMassRandom > 0)
		{
			nChangeMoving = rand() % nMassRandom;
		}
		if (nChangeMoving == 0)
		{// 次に向かう方向を変えるなら
			ResetAIState();
		}
	}

	// 動いた時間を加算
	m_nAI_MoveCounter++;
}

//=============================================================================
// 次のマスに到達しているかどうか判定
//=============================================================================
bool CEnemy::CheckNextMovingMass(void)
{
	// 目的のマス目から座標を算出
	D3DXVECTOR3 DestPos = INITIALIZE_D3DXVECTOR3;
	float fAddWidth = (MASS_SIZE_X * MASS_BLOCK_X) / 2 + (MASS_SIZE_X / 2);
	float fAddDepth = (MASS_SIZE_Z * MASS_BLOCK_Z) / 2 + (MASS_SIZE_Z / 2);
	float fWidth = MASS_SIZE_X;
	float fDepth = MASS_SIZE_Z;
	DestPos.x = (m_nAI_DestMassX * fWidth) - fAddWidth;
	DestPos.z = -(m_nAI_DestMassZ * fDepth) + fAddDepth;

	// 算出した座標から範囲を設定しその範囲内であれば到達していると判定
	D3DXVECTOR3 pos = GetPos();
	if (pos.x >= DestPos.x - ENEMY_MASS_COLRANGE && pos.x <= DestPos.x + ENEMY_MASS_COLRANGE
		&& pos.z >= DestPos.z - ENEMY_MASS_COLRANGE && pos.z <= DestPos.z + ENEMY_MASS_COLRANGE)
	{
		return true;
	}

	return false;
}

//=============================================================================
// 移動量加算処理
//=============================================================================
void CEnemy::Movement(void)
{
	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 move = GetMove();

	// 移動量加算
	pos += move;

	// 慣性を加える
	move.x += (0.0f - move.x) * GetInertia();
	move.z += (0.0f - move.z) * GetInertia();

	// 各種値の設定
	SetPos(pos);
}

//=============================================================================
// 弾を打つ処理
//=============================================================================
void CEnemy::SetBullet(void)
{
	D3DXVECTOR3 BulletMove;
	D3DXVECTOR3 pos = GetPos();

	switch (GetNowRotInfo())
	{// 現在向いている向きで設定
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

	// 速射砲タンクだったら弾の速度を上げる
	if (m_Type == TYPE_FAST)
	{
		BulletMove *= ENEMY_FAST_BULLETMOVE_UP;
	}

	// 弾の生成
	CBulletEnemy::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BulletMove, this); //弾の生成
}

//=============================================================================
// 当たり判定の処理
//=============================================================================
void CEnemy::Collision(void)
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
	for (int nCntPriority = 0; nCntPriority < ENEMY_COL_PRIORITY + 1; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{
			pSceneNext = pScene->GetNext();
			CollisionCheck(&pos, &posOld, &move, colRange, pScene, &bDeath);
			pScene = pSceneNext;
		}
	}

	// 範囲外に行った場合もとに戻す
	if (pos.x > ((MASS_SIZE_X * MASS_BLOCK_X) / 2) - MASS_SIZE_X_HALF)
	{
		pos.x = ((MASS_SIZE_X * MASS_BLOCK_X) / 2) - MASS_SIZE_X_HALF;
		ResetAIState();
	}
	if (pos.x < -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + MASS_SIZE_X_HALF)
	{
		pos.x = -((MASS_SIZE_X * MASS_BLOCK_X) / 2) + MASS_SIZE_X_HALF;
		ResetAIState();
	}
	if (pos.z > ((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) - MASS_SIZE_Z_HALF)
	{
		pos.z = ((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) - MASS_SIZE_Z_HALF;
		ResetAIState();
	}
	if (pos.z < -((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) + MASS_SIZE_Z_HALF)
	{
		pos.z = -((MASS_SIZE_Z * MASS_BLOCK_Z) / 2) + MASS_SIZE_Z_HALF;
		ResetAIState();
	}

	// 各種値の設定
	SetPos(pos);
	SetPosOld(posOld);
	SetMove(move);

	// 死亡判定
	if (bDeath == true)
	{
		if (m_nLife <= 0)
		{// 体力がもうない
		    // 死亡時処理
			Destroy();
		}
		else
		{// 体力がまだある
		    // パーツを削る
			DeleteParts();
		}
	}
}

//=============================================================================
// 当たっているかどうかチェックする処理
//=============================================================================
void CEnemy::CollisionCheck(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CScene *pScene, bool *pDeath)
{
	CObject3D *pObj = (CObject3D*)pScene;
	if (pObj->GetObjType() == OBJTYPE_PLAYER)
	{// プレイヤーだったら
		if (CollisionPlayer(pPos, pPosOld, pMove, colRange, (CPlayer*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			ResetAIState();
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_ENEMY)
	{// 敵だったら
		if (CollisionEnemy(pPos, pPosOld, pMove, colRange, (CEnemy*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			ResetAIState();
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BULLET)
	{// 弾だったら
		CBullet *pBullet = (CBullet*)pScene;
		if (CollisionBullet(pPos, pPosOld, pMove, colRange, pBullet) == true)
		{// 当たっている
			if (pBullet->GetType() == CBullet::TYPE_PLAYER_0 || pBullet->GetType() == CBullet::TYPE_PLAYER_1)
			{// プレイヤーの弾だった
				pBullet->Hit(this);
				*pDeath = true;

				// 体力を減らす
				m_nLife--;
				if (m_nLife <= 0)
				{// 体力がもう無い
					ScoreUp(pBullet);
				}
			}
			else if(pBullet->GetParent() != this)
			{// 自分以外の敵が放った敵の弾だった
				pBullet->Hit(this);
			}
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_BLOCK)
	{// ブロックだったら
		if (CollisionBlock(pPos, pPosOld, pMove, colRange / 2, (CBlock*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			ResetAIState();
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_HEADQUARTERS)
	{// 司令部だったら
		if (CollisionHeadQuarters(pPos, pPosOld, pMove, colRange, (CHeadQuarters*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			ResetAIState();
		}
	}
	else if (pObj->GetObjType() == OBJTYPE_RIVER)
	{// 川だったら
		if (CollisionRiver(pPos, pPosOld, pMove, colRange, (CRiver*)pScene) == true)
		{// 当たっている
			pObj->Hit(this);
			ResetAIState();
		}
	}
}

//=============================================================================
// プレイヤーとの当たり判定処理
//=============================================================================
bool CEnemy::CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CPlayer *pPlayer)
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
bool CEnemy::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CEnemy *pEnemy)
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
bool CEnemy::CollisionBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBullet *pBullet)
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
bool CEnemy::CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CBlock *pBlock)
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
bool CEnemy::CollisionHeadQuarters(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CHeadQuarters *pHead)
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
// 敵との当たり判定処理
//=============================================================================
bool CEnemy::CollisionRiver(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 colRange, CRiver *pRiver)
{
	CBoxCollider *pBoxCollider = pRiver->GetBoxCollider();	//当たり判定箱の取得処理
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
// 死んだときの処理
//=============================================================================
void CEnemy::Destroy(void)
{
	// 敵が減ったことをお知らせ
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		CClient *pClient = CManager::GetClient();
		if (pClient == NULL)
		{
			SetDeathEffect();
			pGame->AddPowerMap();
			Uninit();
			return;
		}
		else if (pClient != NULL && pClient->GetConnected() == false)
		{
			SetDeathEffect();
			pGame->AddPowerMap();
			Uninit();
			return;
		}
		else if (pClient != NULL && pClient->GetClientId() == 0)
		{
			SetDeathEffect();
			pGame->AddPowerMap();
			Uninit();
			return;
		}
		if (pClient != NULL && CManager::GetClient()->GetClientId() != 0)
		{
			SetDeathEffect();
			pGame->DeleteEnemy(m_nIdx);
			pGame->AddPowerMap();
		}
	}
	else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
	{
		CTutorial *pTutorial = CManager::GetTutorial();
		CClient *pClient = CManager::GetClient();
		if (pClient == NULL)
		{
			SetDeathEffect();
			Uninit();
			return;
		}
		else if (pClient != NULL && pClient->GetConnected() == false)
		{
			SetDeathEffect();
			Uninit();
			return;
		}
		else if (pClient != NULL && pClient->GetClientId() == 0)
		{
			SetDeathEffect();
			Uninit();
			return;
		}
		if (pClient != NULL && CManager::GetClient()->GetClientId() != 0)
		{
			SetDeathEffect();
			pTutorial->DeleteEnemy(m_nIdx);
		}
	}
	else
	{
		SetDeathEffect();
		Uninit();
	}
}

//=============================================================================
// スポーンしたときのエフェクトを生成する処理
//=============================================================================
void CEnemy::SetSpawnEffect(void)
{
	D3DXVECTOR3 rot = INITIALIZE_D3DXVECTOR3;
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(GetPos(), rot, ENEMY_SPAWN_EFFECT_IDX);
		rot.y += D3DX_PI * 0.1f;
		pEffectManager->SetEffect(GetPos(), rot, ENEMY_SPAWN_EFFECT_IDX + 1);
		rot.y += D3DX_PI * 0.1f;
		pEffectManager->SetEffect(GetPos(), rot, ENEMY_SPAWN_EFFECT_IDX + 2);
	}

	// 音を出す
	CManager::GetSound()->PlaySound(ENEMY_SPAWN_SE_IDX);
}

//=============================================================================
// 死んだときのエフェクトを生成する処理
//=============================================================================
void CEnemy::SetDeathEffect(void)
{
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager != NULL)
	{
		pEffectManager->SetEffect(GetPos(), INITIALIZE_D3DXVECTOR3, ENEMY_DEATH_EFFECT_IDX);
	}

	// 音を出す
	CManager::GetSound()->PlaySound(ENEMY_DEATH_SE_IDX);
}

//=============================================================================
// アイテムのデータを設定する処理
//=============================================================================
void CEnemy::SetItemData(const int nItemType)
{
	m_bItem = true;
	m_nItemType = nItemType;
}

//=============================================================================
// AIの状態を元に戻す処理
//=============================================================================
void CEnemy::ResetAIState(void)
{
	if (GetAI_State() == AI_STATE_MOVE && m_nAI_MoveCounter >= ENEMY_AI_MOVE_TIME)
	{
		m_RotInfoOld = GetNowRotInfo();
		SetAI_State(AI_STATE_NONE);
		m_nAI_StateCounter = 0;
		m_nAI_MoveCounter = 0;
	}
}

//=============================================================================
// 変数をクリアする処理
//=============================================================================
void CEnemy::ClearVariable(void)
{
	m_nIdx = 0;                 // 敵の番号(クライアント間で同期させるのに必要)
	m_nLife = 0;                // 敵の体力
	m_bItem = false;            // アイテムを落とすかどうか
	m_nItemType = 0;            // アイテムの番号
	m_Type = TYPE_NORMAL;       // 敵の種類
	m_AIState = AI_STATE_NONE;  // AIの状態
	m_nAI_StateCounter = 0;     // AIの状態を管理するカウンター
	m_nAI_MoveCounter = 0;      // 動き始めてからどれくらい時間がたったか
	m_nAI_BulletEva = 0;        // 弾を打つ際のランダム評価値
	m_nAI_MassEva = 0;          // マスを移動した際に向きを変えさせるか判定するときのランダム評価値
	m_nAI_MassCount = 0;        // どれくらいマスを移動したかを数えるカウンター
	m_nAI_DownEva = 0;          // 下向きに動かすかどうかを判定する際のランダム評価値
	m_nAI_NotDownCount = 0;     // 下向きに動かなかった回数を数えるカウンター
	m_nAI_DestMassX = 0;        // 目的地のマス目(横方向)
	m_nAI_DestMassZ = 0;        // 目的地のマス目(縦方向)
}

//=============================================================================
// 敵のAIデータを設定する処理
//=============================================================================
void CEnemy::SetAIData(const int nBulletEva, const int nMassEva, const int nDownEva)
{
	m_nAI_BulletEva = nBulletEva;
	m_nAI_MassEva = nMassEva;
	m_nAI_DownEva = nDownEva;

	// AIの状態は移動する状態に
	SetAI_State(AI_STATE_MOVE);

	// 移動する向きを算出
	SetNextMoveDirection();
	switch (GetNowRotInfo())
	{// 現在の向き情報によって処理わけ
	case ROT_UP:	// 上方向に向いている
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
		break;
	case ROT_DOWN:	// 下方向に向いている
		SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;
	case ROT_LEFT:	// 左方向に向いている
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		break;
	case ROT_RIGHT:	// 右方向に向いている
		SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f));
		break;
	}
}

//=============================================================================
// 敵の総数を設定する
//=============================================================================
void CEnemy::SetNumAll(const int nNumAll)
{
	m_nNumAll = nNumAll;
}

//=============================================================================
// 敵の総数を取得する
//=============================================================================
int CEnemy::GetNumAll(void)
{
	return m_nNumAll;
}

//=============================================================================
// スポーンカウンターを取得する処理
//=============================================================================
int CEnemy::GetSpawnCounter(void)
{
	return m_nSpawnCounter;
}

//=============================================================================
// スポーンカウンターを初期化する処理
//=============================================================================
void CEnemy::ResetSpawnCounter(void)
{
	m_nSpawnCounter = 0;
}

//=============================================================================
// 番号を設定する
//=============================================================================
void CEnemy::SetIdx(const int nIdx)
{
	m_nIdx = nIdx;
}

//=============================================================================
// 体力を設定する
//=============================================================================
void CEnemy::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
// 種類を設定する
//=============================================================================
void CEnemy::SetType(const TYPE type)
{
	m_Type = type;
}

//=============================================================================
// AIの状態を設定する
//=============================================================================
void CEnemy::SetAI_State(const AI_STATE AI_State)
{
	m_AIState = AI_State;
}

//=============================================================================
// AIを管理するカウンターを設定する
//=============================================================================
void CEnemy::SetAI_StateCounter(const int nStateCounter)
{
	m_nAI_StateCounter = nStateCounter;
}

//=============================================================================
// 弾を打つ際のランダム評価値を設定する
//=============================================================================
void CEnemy::SetAI_BulletEva(const int nBulletEva)
{
	m_nAI_BulletEva = nBulletEva;
}

//=============================================================================
// マスを移動した際に向きを変えさせるか判定するときのランダム評価値を取得する
//=============================================================================
void CEnemy::SetAI_MassEva(const int nMassEva)
{
	m_nAI_MassEva = nMassEva;
}

//=============================================================================
// どれくらいマスを移動したかを設定する
//=============================================================================
void CEnemy::SetAI_MassCount(const int nMassCount)
{
	m_nAI_MassCount = nMassCount;
}

//=============================================================================
// 下向きに動かすかどうかを判定する際のランダム評価値を設定する
//=============================================================================
void CEnemy::SetAI_DownEva(const int nDownEva)
{
	m_nAI_DownEva = nDownEva;
}

//=============================================================================
// 下向きに動かなかった回数を数えるカウンターを設定する
//=============================================================================
void CEnemy::SetAI_NotDownCount(const int nNotDownCount)
{
	m_nAI_NotDownCount = nNotDownCount;
}

//=============================================================================
// 番号を取得する
//=============================================================================
int CEnemy::GetIdx(void)
{
	return m_nIdx;
}

//=============================================================================
// 体力を取得する
//=============================================================================
int CEnemy::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
// 敵の種類を取得する
//=============================================================================
CEnemy::TYPE CEnemy::GetType(void)
{
	return m_Type;
}

//=============================================================================
// AIの状態を取得する
//=============================================================================
CEnemy::AI_STATE CEnemy::GetAI_State(void)
{
	return m_AIState;
}

//=============================================================================
// AIの状態を管理するカウンターを取得する
//=============================================================================
int CEnemy::GetAI_StateCounter(void)
{
	return m_nAI_StateCounter;
}

//=============================================================================
// 弾を打つ際のランダム評価値を取得する
//=============================================================================
int CEnemy::GetAI_BulletEva(void)
{
	return m_nAI_BulletEva;
}

//=============================================================================
// マスを移動した際に向きを変えさせるか判定するときのランダム評価値を取得する
//=============================================================================
int CEnemy::GetAI_MassEva(void)
{
	return m_nAI_MassEva;
}

//=============================================================================
// どれくらいマスを移動したかを数えるカウンターを取得する
//=============================================================================
int CEnemy::GetAI_MassCount(void)
{
	return m_nAI_MassCount;
}

//=============================================================================
// 下向きに動かすかどうかを判定する際のランダム評価値を取得する
//=============================================================================
int CEnemy::GetAI_DownEva(void)
{
	return m_nAI_DownEva;
}

//=============================================================================
// 下向きに動かなかった回数を数えるカウンターを取得する
//=============================================================================
int CEnemy::GetAI_NotDownCount(void)
{
	return m_nAI_NotDownCount;
}


//*****************************************************************************
// CEnemyNormalの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CEnemyNormal::CEnemyNormal(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CEnemyNormal *CEnemyNormal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEnemyNormal *pEnemyNormal = NULL;

	if (pEnemyNormal == NULL)
	{// NULLの場合
		pEnemyNormal = new CEnemyNormal(nPriority);
		if (pEnemyNormal != NULL)
		{// NULLでない場合
			pEnemyNormal->SetPos(pos);
			pEnemyNormal->SetRot(rot);
			pEnemyNormal->SetType((CEnemy::TYPE)nType);
			pEnemyNormal->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pEnemyNormal->SetReturnFlag(true);
			pEnemyNormal->Init();
		}
	}
	return pEnemyNormal;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemyNormal::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CEnemy::Init()))
	{
		return E_FAIL;
	}

	// 敵の種類を設定
	SetType(TYPE_NORMAL);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemyNormal::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemyNormal::Update(void)
{
	CEnemy::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemyNormal::Draw(void)
{
	CEnemy::Draw();
}


//*****************************************************************************
// CEnemyArmoreの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CEnemyArmore::CEnemyArmore(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemyArmore::~CEnemyArmore()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CEnemyArmore *CEnemyArmore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEnemyArmore *pEnemyArmore = NULL;

	if (pEnemyArmore == NULL)
	{// NULLの場合
		pEnemyArmore = new CEnemyArmore(nPriority);
		if (pEnemyArmore != NULL)
		{// NULLでない場合
			pEnemyArmore->SetPos(pos);
			pEnemyArmore->SetRot(rot);
			pEnemyArmore->SetType((CEnemy::TYPE)nType);
			pEnemyArmore->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pEnemyArmore->SetReturnFlag(true);
			pEnemyArmore->Init();
		}
	}
	return pEnemyArmore;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemyArmore::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CEnemy::Init()))
	{
		return E_FAIL;
	}

	// 敵の種類を設定
	SetType(TYPE_ARMORE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemyArmore::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemyArmore::Update(void)
{
	CEnemy::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemyArmore::Draw(void)
{
	CEnemy::Draw();
}


//*****************************************************************************
// CEnemyFastの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CEnemyFast::CEnemyFast(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemyFast::~CEnemyFast()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CEnemyFast *CEnemyFast::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEnemyFast *pEnemyFast = NULL;

	if (pEnemyFast == NULL)
	{// NULLの場合
		pEnemyFast = new CEnemyFast(nPriority);
		if (pEnemyFast != NULL)
		{// NULLでない場合
			pEnemyFast->SetPos(pos);
			pEnemyFast->SetRot(rot);
			pEnemyFast->SetType((CEnemy::TYPE)nType);
			pEnemyFast->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pEnemyFast->SetReturnFlag(true);
			pEnemyFast->Init();
		}
	}
	return pEnemyFast;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemyFast::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CEnemy::Init()))
	{
		return E_FAIL;
	}

	// 敵の種類を設定
	SetType(TYPE_FAST);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemyFast::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemyFast::Update(void)
{
	CEnemy::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemyFast::Draw(void)
{
	CEnemy::Draw();
}


//*****************************************************************************
// CEnemyHeavyの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CEnemyHeavy::CEnemyHeavy(int nPriority, OBJTYPE objType) : CEnemy(nPriority, objType)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemyHeavy::~CEnemyHeavy()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CEnemyHeavy *CEnemyHeavy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEnemyHeavy *pEnemyHeavy = NULL;

	if (pEnemyHeavy == NULL)
	{// NULLの場合
		pEnemyHeavy = new CEnemyHeavy(nPriority);
		if (pEnemyHeavy != NULL)
		{// NULLでない場合
			pEnemyHeavy->SetPos(pos);
			pEnemyHeavy->SetRot(rot);
			pEnemyHeavy->SetType((CEnemy::TYPE)nType);
			pEnemyHeavy->SetColRange(D3DXVECTOR3(70.0f, 70.0f, 70.0f));
			pEnemyHeavy->SetReturnFlag(true);
			pEnemyHeavy->Init();
		}
	}
	return pEnemyHeavy;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEnemyHeavy::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CEnemy::Init()))
	{
		return E_FAIL;
	}

	// 敵の種類を設定
	SetType(TYPE_HEAVY);

	// 体力を設定
	SetLife(ENEMY_HEAVY_LIFE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEnemyHeavy::Uninit(void)
{
	CEnemy::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CEnemyHeavy::Update(void)
{
	CEnemy::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void CEnemyHeavy::Draw(void)
{
	CEnemy::Draw();
}