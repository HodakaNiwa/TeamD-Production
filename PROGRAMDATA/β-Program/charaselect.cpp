//=============================================================================
//
// キャラセレクト処理 [charaselect.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "charaselect.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "input.h"
#include "fade.h"
#include "server.h"
#include "scene2D.h"
#include "debugproc.h"


// 後にプレイヤーに変わる
#include "player.h"
#include "playerManager.h"
#include "light.h"
#include "lightManager.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define CHARASELECT_SYSTEM_FILENAME              "data/TEXT/MODE/charaselect.ini"    // 初期化に使用するシステムファイル名
#define CHARASELECT_CAMERA_POSV                  (D3DXVECTOR3(0.0f,40.0f,400.0f))    // キャラを映すカメラの視点位置
#define CHARASELECT_CIRCLE_RADIUS                (200.0f)                            // 円の半径(円形回転の計算に使用)
#define CHARASELECT_CIRCLE_ROLLSPEED             (D3DX_PI * 2.0f / 120.0f)           // 円形回転時の回転スピード
#define CHARASELECT_MODECHANGE_TIMING            (120)                               // 終了状態から画面遷移するまでの時間

// 計算置き換え用マクロ
#define CHARASELECT_MAPTYPE_NUMBER               (m_nSelectStageSide + (m_nSelectStageVertical * (CGame::MAPTYPE_MAX / 2)))

// 背景ポリゴン初期化用
#define CHARASELECTBG_POS_INI                    (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define CHARASELECTBG_COL_INI                    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECTBG_WIDTH_INI                  (SCREEN_WIDTH / 2.0f)
#define CHARASELECTBG_HEIGHT_INI                 (SCREEN_HEIGHT / 2.0f)

// 準備中ポリゴン初期化用
// １つ目
#define CHARASELECTPREPARATION_0_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 4.0f, 600.0f, 0.0f))
#define CHARASELECTPREPARATION_0_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECTPREPARATION_0_WIDTH_INI       (200.0f)
#define CHARASELECTPREPARATION_0_HEIGHT_INI      (80.0f)

// ２つ目
#define CHARASELECTPREPARATION_1_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 4.0f * 3.0f, 600.0f, 0.0f))
#define CHARASELECTPREPARATION_1_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECTPREPARATION_1_WIDTH_INI       (200.0f)
#define CHARASELECTPREPARATION_1_HEIGHT_INI      (80.0f)


// プレイヤー番号ポリゴン初期化用
// １つ目
#define CHARASELECTPLAYERNUMBER_0_POS_INI        (D3DXVECTOR3(110.0f, 50.0f, 0.0f))
#define CHARASELECTPLAYERNUMBER_0_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECTPLAYERNUMBER_0_WIDTH_INI      (100.0f)
#define CHARASELECTPLAYERNUMBER_0_HEIGHT_INI     (40.0f)

// ２つ目
#define CHARASELECTPLAYERNUMBER_1_POS_INI        (D3DXVECTOR3(750.0f, 50.0f, 0.0f))
#define CHARASELECTPLAYERNUMBER_1_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECTPLAYERNUMBER_1_WIDTH_INI      (100.0f)
#define CHARASELECTPLAYERNUMBER_1_HEIGHT_INI     (40.0f)

// 自分の番号表示用ポリゴン用
#define CHARASELECT_YOUPOLYGON_POS_INI           (D3DXVECTOR3(360.0f, 50.0f, 0.0f))
#define CHARASELECT_YOUPOLYGON_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECT_YOUPOLYGON_WIDTH_INI         (50.0f)
#define CHARASELECT_YOUPOLYGON_HEIGHT_INI        (20.0f)

// レンダリングポリゴン初期化用
// １つ目
#define CHARASELECT_RENDERERPOLYGON_0_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH / 4.0f, 250.0f, 0.0f))
#define CHARASELECT_RENDERERPOLYGON_0_COL_INI    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECT_RENDERERPOLYGON_0_WIDTH_INI  (310.0f)
#define CHARASELECT_RENDERERPOLYGON_0_HEIGHT_INI (250.0f)

// ２つ目
#define CHARASELECT_RENDERERPOLYGON_1_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH / 4.0f * 3.0f, 250.0f, 0.0f))
#define CHARASELECT_RENDERERPOLYGON_1_COL_INI    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define CHARASELECT_RENDERERPOLYGON_1_WIDTH_INI  (310.0f)
#define CHARASELECT_RENDERERPOLYGON_1_HEIGHT_INI (250.0f)

// ステージ選択用ポリゴン初期化用
#define CHARASELECT_STAGEPOLYGON_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 4.0f, 0.0f))
#define CHARASELECT_STAGEPOLYGON_COL_NONE        (D3DXCOLOR(0.2f,0.2f,0.0f,1.0f))
#define CHARASELECT_STAGEPOLYGON_WIDTH_NONE      (180.0f)
#define CHARASELECT_STAGEPOLYGON_HEIGHT_NONE     (100.0f)
#define CHARASELECT_STAGEPOLYGON_COL_SELECT      (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))
#define CHARASELECT_STAGEPOLYGON_WIDTH_SELECT    (230.0f)
#define CHARASELECT_STAGEPOLYGON_HEIGHT_SELECT   (130.0f)
#define CHARASELECT_STAGEPOLYGON_INTERVAL_X      (SCREEN_WIDTH / 2.0f)
#define CHARASELECT_STAGEPOLYGON_INTERVAL_Y      (SCREEN_HEIGHT / 2.0f)

// 値読み込み用のパス
// テクスチャ用
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// プレイヤーデータ用
#define NUM_PLAYERDATA "NUM_PLAYERDATA = "
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "

// 背景ポリゴン用
#define BGPOLYGONSET "BGPOLYGONSET"
#define END_BGPOLYGONSET "END_BGPOLYGONSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// 準備中ポリゴン用
#define PREPARATION_TEX_IDX "PREPARATION_TEX_IDX = "
#define PREPARATIONSET "PREPARATIONSET"
#define END_PREPARATIONSET "END_PREPARATIONSET"

// プレイヤー番号表示ポリゴン用
#define PLAYERNUMBER_TEX_IDX "PLAYERNUMBER_TEX_IDX = "
#define PLAYERNUMBERSET "PLAYERNUMBERSET"
#define END_PLAYERNUMBERSET "END_PLAYERNUMBERSET"

// 自分の番号表示用ポリゴン用
#define YOUPOLYGONSET "YOUPOLYGONSET"
#define END_YOUPOLYGONSET "END_YOUPOLYGONSET"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CCharaSelect::m_nSelectPlayer[MAX_NUM_PLAYER] = {};
int CCharaSelect::m_nSelectStage = 0;

//=============================================================================
// キャラセレクトのコンストラクタ
//=============================================================================
CCharaSelect::CCharaSelect()
{

}

//=============================================================================
// キャラセレクトのデストラクタ
//=============================================================================
CCharaSelect::~CCharaSelect()
{

}

//=============================================================================
// キャラセレクトの生成処理
//=============================================================================
CCharaSelect *CCharaSelect::Create()
{
	CCharaSelect *pCharaSelect = NULL;
	if (pCharaSelect == NULL)
	{
		pCharaSelect = new CCharaSelect;
		if (pCharaSelect != NULL)
		{
			pCharaSelect->Init();
		}
	}
	return pCharaSelect;
}

//=============================================================================
// キャラセレクトの初期化処理
//=============================================================================
HRESULT CCharaSelect::Init(void)
{
	// 変数を初期化する
	ClearVariable();

	// システムの初期化
	LoadSystem();

	// カメラの生成
	CreateCamera();

	// プレイヤーマネージャーの生成
	CreatePlayerManager();

	// プレイヤーの生成
	CreatePlayer();

	// レンダリング用テクスチャの生成
	CreateRenderTexture();

	// クライアントの生成
	CManager::CreateClient();

	return S_OK;
}

//=============================================================================
// キャラセレクトの終了処理
//=============================================================================
void CCharaSelect::Uninit(void)
{
	// 共通の終了処理
	CBasemode::Uninit();

	// 各種ポインタを開放
	ReleaseBg();
	ReleasePlayer();
	ReleaseRenderTexture();
	ReleaseRenderPolygon();
	ReleasePreparation();
	ReleasePlayerNumber();
	ReleaseYouPolygon();
	ReleaseStagePolygon();
	ReleasePlayerDataPointer();
	ReleasePlayerManager();

	// 全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// キャラセレクトの更新処理
//=============================================================================
void CCharaSelect::Update(void)
{
	CDebugProc::Print(1, "キャラセレクト画面\n");
	CDebugProc::Print(1, "自分のクライアント番号 : %d\n", CManager::GetClient()->GetClientId());

	// 全ての更新処理
	CScene::UpdateAll();

	// 状態によって処理わけ
	switch (m_State[CManager::GetClient()->GetClientId()])
	{
	case STATE_SELECT:
		SelectUpdate(CManager::GetClient()->GetClientId());
		break;
	case STATE_CHARACHANGE_TO_LEFT:
		CharaChangeToLeftUpdate(CManager::GetClient()->GetClientId());
		break;
	case STATE_CHARACHANGE_TO_RIGHT:
		CharaChangeToRightUpdate(CManager::GetClient()->GetClientId());
		break;
	case STATE_WAIT_PARTNER:
		WaitPartnerUpdate(CManager::GetClient()->GetClientId());
		break;
	case STATE_STAGE_SELECT:
		StageSelectUpdate(CManager::GetClient()->GetClientId());
		break;
	case STATE_WAIT_STAGESELECT:
		WaitStageSelectUpdate(CManager::GetClient()->GetClientId());
		break;
	case STATE_END:
		EndUpdate();
		break;
	}

	// 状態によって処理わけ
	switch (m_State[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER])
	{
	case STATE_SELECT:
		SelectUpdate((CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER);
		break;
	case STATE_CHARACHANGE_TO_LEFT:
		CharaChangeToLeftUpdate((CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER);
		break;
	case STATE_CHARACHANGE_TO_RIGHT:
		CharaChangeToRightUpdate((CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER);
		break;
	}

	// サーバーへ送るデータを設定する
	SetDataToServer();

	// クライアントの更新処理
	CManager::ClientUpdate();

	// サーバーから送られたデータを設定する
	GetDataFromServer();
}

//=============================================================================
// キャラセレクトの描画処理
//=============================================================================
void CCharaSelect::Draw(void)
{
	// カメラの設定
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera();
	}

	// テクスチャにプレイヤーを描画する
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		CharaDraw(nCntPlayer);
	}

	// 作られたテクスチャを使って画面を完成させる
	TextureDraw();
}

//=============================================================================
// キャラセレクトのテクスチャ管轄クラス生成処理
//=============================================================================
void CCharaSelect::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// キャラセレクトのカメラ生成処理
//=============================================================================
void CCharaSelect::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager == NULL) return;

	// カメラを生成
	CCharaSelectCamera *pCamera = CCharaSelectCamera::Create(CHARASELECT_CAMERA_POSV, INITIALIZE_D3DXVECTOR3);
	if (pCamera != NULL)
	{
		pCameraManager->SetCamera(pCamera);
	}
	SetCameraManager(pCameraManager);
}

//=============================================================================
// キャラセレクトのプレイヤー生成処理
//=============================================================================
void CCharaSelect::CreatePlayer(void)
{
	// プレイヤーを生成する
	float fAddRot = 0.0f;
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			//m_pPlayer[nCntPlayer][nCntType] = CScene3D::Create(D3DXVECTOR3(sinf(m_fSelectRot[nCntPlayer] + fAddRot) * CHARASELECT_CIRCLE_RADIUS, 0.0f, cosf(m_fSelectRot[nCntPlayer] + fAddRot) * CHARASELECT_CIRCLE_RADIUS), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 30.0f, 30.0f);

			//プレイヤーの生成
			m_pPlayer[nCntPlayer][nCntType] = CPlayer::Create(D3DXVECTOR3(sinf(m_fSelectRot[nCntPlayer] + fAddRot) * CHARASELECT_CIRCLE_RADIUS, 0.0f, cosf(m_fSelectRot[nCntPlayer] + fAddRot) * CHARASELECT_CIRCLE_RADIUS),
				D3DXVECTOR3(0.0f, -D3DX_PI, 0.0f), 1, m_pPlayerManager[nCntPlayer][nCntType]->GetModel(),
				m_pPlayerManager[nCntPlayer][nCntType]->GetMotionManager(), m_pPlayerManager[nCntPlayer][nCntType]->GetParts());

			// 回転量加算
			fAddRot += D3DX_PI * 2.0f / m_nNumPlayerData;
			if (fAddRot > D3DX_PI)
			{
				fAddRot -= D3DX_PI * 2.0f;
			}
		}
	}
}

//=============================================================================
// キャラセレクトのステージの種類表示用のポリゴンを生成する処理
//=============================================================================
void CCharaSelect::CreateStagePolygon(void)
{
	D3DXVECTOR3 StagePolygonPos = CHARASELECT_STAGEPOLYGON_POS_INI;
	D3DXCOLOR StagePolygonCol = CHARASELECT_STAGEPOLYGON_COL_SELECT;
	float fStagePolygonWidth = CHARASELECT_STAGEPOLYGON_WIDTH_SELECT;
	float fStagePolygonHeight = CHARASELECT_STAGEPOLYGON_HEIGHT_SELECT;
	float fStagePolygonPosXDef = StagePolygonPos.x;

	for (int nCntType = 0; nCntType < CGame::MAPTYPE_MAX; nCntType++)
	{
		m_apStage[nCntType] = CScene2DFlash::Create(StagePolygonPos, StagePolygonCol,
			fStagePolygonWidth, fStagePolygonHeight, 0.0f, CScene2DFlash::STATE_NONE);

		StagePolygonPos.x += CHARASELECT_STAGEPOLYGON_INTERVAL_X;
		if ((nCntType + 1) % 2 == 0)
		{
			StagePolygonPos.x = fStagePolygonPosXDef;
			StagePolygonPos.y += CHARASELECT_STAGEPOLYGON_INTERVAL_Y;
		}
		if (nCntType == 0)
		{
			StagePolygonCol = CHARASELECT_STAGEPOLYGON_COL_NONE;
			fStagePolygonWidth = CHARASELECT_STAGEPOLYGON_WIDTH_NONE;
			fStagePolygonHeight = CHARASELECT_STAGEPOLYGON_HEIGHT_NONE;
		}
	}
}

//=============================================================================
// キャラセレクトのプレイヤーデータ用のポインタを生成する処理
//=============================================================================
void CCharaSelect::CreatePlayerDataPointer(void)
{
	// ファイル名
	m_aPlayerDataFileName = new char*[m_nNumPlayerData];
	if (m_aPlayerDataFileName != NULL)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			m_aPlayerDataFileName[nCntType] = new char[256];
			strcpy(m_aPlayerDataFileName[nCntType], "\0");
		}
	}

	// プレイヤー
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = new CPlayer*[m_nNumPlayerData];
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			m_pPlayer[nCntPlayer][nCntType] = NULL;
		}
	}

	// プレイヤーマネージャー
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayerManager[nCntPlayer] = new CPlayerManager*[m_nNumPlayerData];
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			m_pPlayerManager[nCntPlayer][nCntType] = NULL;
		}
	}

}

//=============================================================================
// キャラセレクトのレンダリング用のテクスチャ生成処理
//=============================================================================
void CCharaSelect::CreateRenderTexture(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	LPDIRECT3DSURFACE9 pRenderMT = NULL;
	LPDIRECT3DSURFACE9 pBuffMT = NULL;
	D3DXVECTOR3 PolygonPos[MAX_NUM_PLAYER] = { CHARASELECT_RENDERERPOLYGON_0_POS_INI, CHARASELECT_RENDERERPOLYGON_1_POS_INI };
	D3DXCOLOR PolygonCol[MAX_NUM_PLAYER] = { CHARASELECT_RENDERERPOLYGON_0_COL_INI, CHARASELECT_RENDERERPOLYGON_1_COL_INI };
	float fPolygonWidth[MAX_NUM_PLAYER] = { CHARASELECT_RENDERERPOLYGON_0_WIDTH_INI, CHARASELECT_RENDERERPOLYGON_1_WIDTH_INI };
	float fPolygonHeight[MAX_NUM_PLAYER] = { CHARASELECT_RENDERERPOLYGON_0_HEIGHT_INI, CHARASELECT_RENDERERPOLYGON_1_HEIGHT_INI };

	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// プレイヤーの総数分繰り返し
		// レンダリング用のテクスチャを作成
		pDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT, 1,
			D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_apRenderTexture[nCntPlayer], NULL);

		// レンダリングターゲットを設定
		CManager::GetRenderer()->SetRenderTarget(m_apRenderTexture[nCntPlayer]);

		// レンダリングに使うポリゴンも生成
		m_apPolygonPlayer[nCntPlayer] = CScene2D::Create(PolygonPos[nCntPlayer], PolygonCol[nCntPlayer], fPolygonWidth[nCntPlayer], fPolygonHeight[nCntPlayer]);
		if (m_apPolygonPlayer[nCntPlayer] != NULL)
		{
			m_apPolygonPlayer[nCntPlayer]->BindTexture(m_apRenderTexture[nCntPlayer]);
		}
	}

	// デフォルトのレンダリングターゲットに戻す
	CManager::GetRenderer()->ResetRenderTarget();
}

//=============================================================================
// プレイヤーマネージャーの生成
//=============================================================================
void CCharaSelect::CreatePlayerManager(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			m_pPlayerManager[nCntPlayer][nCntType] = CPlayerManager::Create(m_aPlayerDataFileName[nCntType]);
		}
	}
}

//=============================================================================
// キャラセレクトの背景ポリゴンを開放する
//=============================================================================
void CCharaSelect::ReleaseBg(void)
{
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}
}

//=============================================================================
// キャラセレクトのプレイヤーを開放する
//=============================================================================
void CCharaSelect::ReleasePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{
			for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
			{
				if (m_pPlayer[nCntPlayer][nCntType] != NULL)
				{
					m_pPlayer[nCntPlayer][nCntType]->Uninit();
					m_pPlayer[nCntPlayer][nCntType] = NULL;
				}
			}
			delete[] m_pPlayer[nCntPlayer];
			m_pPlayer[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// キャラセレクトのレンダリング用テクスチャを開放する
//=============================================================================
void CCharaSelect::ReleaseRenderTexture(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apRenderTexture[nCntPlayer] != NULL)
		{
			m_apRenderTexture[nCntPlayer]->Release();
			m_apRenderTexture[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// キャラセレクトのレンダリング用ポリゴンを開放する
//=============================================================================
void CCharaSelect::ReleaseRenderPolygon(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPolygonPlayer[nCntPlayer] != NULL)
		{
			m_apPolygonPlayer[nCntPlayer]->Uninit();
			m_apPolygonPlayer[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// キャラセレクトの準備中ポリゴンを開放する
//=============================================================================
void CCharaSelect::ReleasePreparation(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPreparation[nCntPlayer] != NULL)
		{
			m_apPreparation[nCntPlayer]->Uninit();
			m_apPreparation[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// キャラセレクトのプレイヤー番号表示用ポリゴンを開放する
//=============================================================================
void CCharaSelect::ReleasePlayerNumber(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPlayerNumber[nCntPlayer] != NULL)
		{
			m_apPlayerNumber[nCntPlayer]->Uninit();
			m_apPlayerNumber[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// キャラセレクトの自分の番号表示用ポリゴンを開放する
//=============================================================================
void CCharaSelect::ReleaseYouPolygon(void)
{
	if (m_pYouPolygon != NULL)
	{
		m_pYouPolygon->Uninit();
		m_pYouPolygon = NULL;
	}
}

//=============================================================================
// キャラセレクトのステージの種類表示用ポリゴンを開放する
//=============================================================================
void CCharaSelect::ReleaseStagePolygon(void)
{
	for (int nCntType = 0; nCntType < CGame::MAPTYPE_MAX; nCntType++)
	{
		if (m_apStage[nCntType] != NULL)
		{
			m_apStage[nCntType]->Uninit();
			m_apStage[nCntType] = NULL;
		}
	}
}

//=============================================================================
// キャラセレクトのプレイヤーデータ用のポインタを生成する処理
//=============================================================================
void CCharaSelect::ReleasePlayerDataPointer(void)
{
	// プレイヤーデータ
	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{
		if (m_aPlayerDataFileName[nCntType] != NULL)
		{
			delete[] m_aPlayerDataFileName[nCntType];
			m_aPlayerDataFileName[nCntType] = NULL;
		}
	}
	delete[] m_aPlayerDataFileName;
	m_aPlayerDataFileName = NULL;
}

//=============================================================================
// プレイヤーマネージャーの破棄
//=============================================================================
void CCharaSelect::ReleasePlayerManager(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
		{
			if (m_pPlayerManager[nCntPlayer][nCntType] != NULL)
			{
				m_pPlayerManager[nCntPlayer][nCntType]->Uninit();
				delete m_pPlayerManager[nCntPlayer][nCntType];
				m_pPlayerManager[nCntPlayer][nCntType] = NULL;
			}
		}
		delete[] m_pPlayerManager[nCntPlayer];
		m_pPlayerManager[nCntPlayer] = NULL;
	}
}

//=============================================================================
// キャラセレクトのサーバーへ送るデータを設定する処理
//=============================================================================
void CCharaSelect::SetDataToServer(void)
{
	// 現在の状態を設定
	CManager::GetClient()->Print("%d ", (int)m_State[CManager::GetClient()->GetClientId()]);

	// 現在のプレイヤー番号を設定
	CManager::GetClient()->Print("%d ", m_nSelectPlayer[CManager::GetClient()->GetClientId()]);
}

//=============================================================================
// キャラセレクトのサーバーから送られたデータを設定する処理
//=============================================================================
void CCharaSelect::GetDataFromServer(void)
{
	char *pStr = CManager::GetClient()->GetReceiveMessage();

	// 現在の状態を読み取る
	int nState = CFunctionLib::ReadInt(pStr, "");
	m_State[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] = (STATE)nState;

	// 現在のプレイヤー番号を読み取る
	int nPlayerNumber = CFunctionLib::ReadInt(pStr, " ");
	m_nSelectPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] = nPlayerNumber;

	CDebugProc::Print(1, "相手の状態 : %d\n", nState);
	CDebugProc::Print(1, "相手の選択しているプレイヤー番号 : %d\n", nPlayerNumber);

	// 相手の状態によって画面遷移するか決める
	if (nState == STATE_WAIT_PARTNER && m_State[CManager::GetClient()->GetClientId()] == STATE_WAIT_PARTNER)
	{
		if (m_apPreparation[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL && GetTextureManager() != NULL)
		{
			m_apPreparation[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->BindTexture(GetTextureManager()->GetTexture(m_nPreparationTexIdx[1]));
		}
		m_State[CManager::GetClient()->GetClientId()] = STATE_END;
	}
	else if (nState == STATE_WAIT_PARTNER && m_State[CManager::GetClient()->GetClientId()] != STATE_WAIT_PARTNER)
	{
		if (m_apPreparation[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL && GetTextureManager() != NULL)
		{
			m_apPreparation[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->BindTexture(GetTextureManager()->GetTexture(m_nPreparationTexIdx[1]));
		}
	}
}

//=============================================================================
// キャラセレクトのセレクト状態の更新処理
//=============================================================================
void CCharaSelect::SelectUpdate(int nIdx)
{
	CDebugProc::Print(1, "セレクト状態\n");
	CDebugProc::Print(1, "自分の現在のキャラ番号 : %d\n", m_nSelectPlayer[CManager::GetClient()->GetClientId()]);

	// キャラ変更を受け付ける
	if (m_State[CManager::GetClient()->GetClientId()] != STATE_END && m_State[CManager::GetClient()->GetClientId()] != STATE_WAIT_PARTNER)
	{
		WaitInputToChangeChara(nIdx);
	}
}

//=============================================================================
// キャラセレクトの左側のキャラに変更する状態の更新処理
//=============================================================================
void CCharaSelect::CharaChangeToLeftUpdate(int nIdx)
{
	CDebugProc::Print(1, "左側のキャラに変更状態\n");
	CDebugProc::Print(1, "自分の現在のキャラ番号 : %d\n", m_nSelectPlayer[CManager::GetClient()->GetClientId()]);

	// 円形回転を進める
	m_fSelectRot[nIdx] += CHARASELECT_CIRCLE_ROLLSPEED;
	if (m_fSelectRot[nIdx] < D3DX_PI)
	{
		m_fSelectRot[nIdx] -= D3DX_PI * 2.0f;
	}

	// 回転した量を増やす
	m_fAddSelectRot[nIdx] += CHARASELECT_CIRCLE_ROLLSPEED;
	if (m_fAddSelectRot[nIdx] > D3DX_PI * 2.0f / m_nNumPlayerData)
	{
		m_fAddSelectRot[nIdx] = 0.0f;
		m_fSelectRot[nIdx] -= CHARASELECT_CIRCLE_ROLLSPEED;
		m_State[nIdx] = STATE_SELECT;
	}

	// プレイヤーの座標をすべて移動させる
	CircleRotation(nIdx);
}

//=============================================================================
// キャラセレクトの右側のキャラに変更する状態の更新処理
//=============================================================================
void CCharaSelect::CharaChangeToRightUpdate(int nIdx)
{
	CDebugProc::Print(1, "右側のキャラに変更状態\n");
	CDebugProc::Print(1, "自分の現在のキャラ番号 : %d\n", m_nSelectPlayer[CManager::GetClient()->GetClientId()]);

	// 円形回転を進める
	m_fSelectRot[nIdx] -= CHARASELECT_CIRCLE_ROLLSPEED;
	if (m_fSelectRot[nIdx] < -D3DX_PI)
	{
		m_fSelectRot[nIdx] += D3DX_PI * 2.0f;
	}

	// 回転した量を増やす
	m_fAddSelectRot[nIdx] += CHARASELECT_CIRCLE_ROLLSPEED;
	if (m_fAddSelectRot[nIdx] > D3DX_PI * 2.0f / m_nNumPlayerData)
	{
		m_fAddSelectRot[nIdx] = 0.0f;
		m_fSelectRot[nIdx] += CHARASELECT_CIRCLE_ROLLSPEED;
		m_State[nIdx] = STATE_SELECT;
	}

	// プレイヤーの座標をすべて移動させる
	CircleRotation(nIdx);
}

//=============================================================================
// キャラセレクトの相手のキャラセレクトを待機する状態の更新処理
//=============================================================================
void CCharaSelect::WaitPartnerUpdate(int nIdx)
{
	CDebugProc::Print(1, "相手のキャラセレクト待機状態\n");
	CDebugProc::Print(1, "自分の現在のキャラ番号 : %d\n", m_nSelectPlayer[CManager::GetClient()->GetClientId()]);
	CDebugProc::Print(1, "相手の現在のキャラ番号 : %d\n", m_nSelectPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]);


	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;

	if (pFade->GetFade() == CFade::FADE_NONE)
	{// フェードが使用されていない
		if (pKey->GetTrigger(DIK_RETURN) == true ||
			CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
		{// 決定ボタンが押された
			m_State[nIdx] = STATE_STAGE_SELECT;
			// キャラセレクト用のポリゴンを破棄
			ReleasePlayer();
			ReleaseRenderPolygon();
			ReleasePreparation();
			ReleasePlayerNumber();
			ReleaseYouPolygon();

			// ステージ選択用のポリゴンを生成
			CreateStagePolygon();
		}
	}
}

//=============================================================================
// キャラセレクトのステージ選択状態の更新処理
//=============================================================================
void CCharaSelect::StageSelectUpdate(int nIdx)
{
	CDebugProc::Print(1, "マップセレクト待機状態\n");
	CDebugProc::Print(1, "現在選択されている番号 : %d\n", m_nSelectStageSide + (m_nSelectStageVertical * 2));
	WaitInputToStageSelect(nIdx);
}

//=============================================================================
// キャラセレクトのステージ選択を待っている状態の更新処理
//=============================================================================
void CCharaSelect::WaitStageSelectUpdate(int nIdx)
{
	CDebugProc::Print(1, "相手のマップセレクト待機状態\n");
}

//=============================================================================
// キャラセレクトの終了状態の更新処理
//=============================================================================
void CCharaSelect::EndUpdate(void)
{
	CDebugProc::Print(1, "終了状態\n");
	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) return;

	m_nStateCounter++;
	if (m_nStateCounter % CHARASELECT_MODECHANGE_TIMING == 0)
	{// カウンターが一定値以上になった
		if (pFade->GetFade() == CFade::FADE_NONE)
		{// まだフェードが開始されていない
			pFade->SetFade(CManager::MODE_TUTORIAL);
		}
	}
}

//=============================================================================
// キャラセレクトのキャラ番号を変えるための入力待機処理
//=============================================================================
void CCharaSelect::WaitInputToChangeChara(int nIdx)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL || CManager::GetClient() == NULL) return;

	if (pKey->GetTrigger(DIK_A) == true ||
		CManager::GetXInput()->GetPress(0, CXInput::XIJS_BUTTON_2) == true ||
		CManager::GetXInput()->GetPress(0, CXInput::XIJS_BUTTON_18) == true)
	{// 左方向の入力がされた
		m_nSelectPlayer[nIdx] = (m_nSelectPlayer[nIdx] + (m_nNumPlayerData - 1)) % m_nNumPlayerData;
		m_State[nIdx] = STATE_CHARACHANGE_TO_RIGHT;
	}
	else if (pKey->GetTrigger(DIK_D) == true ||
		CManager::GetXInput()->GetPress(0, CXInput::XIJS_BUTTON_3) == true ||
		CManager::GetXInput()->GetPress(0, CXInput::XIJS_BUTTON_19) == true)
	{// 右方向の入力がされた
		m_nSelectPlayer[nIdx] = (m_nSelectPlayer[nIdx] + 1) % m_nNumPlayerData;
		m_State[nIdx] = STATE_CHARACHANGE_TO_LEFT;
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		if (m_State[(nIdx + 1) % MAX_NUM_PLAYER] != STATE_WAIT_PARTNER)
		{// まだ相手がプレイヤーの種類を決定していない
			if (m_apPreparation[nIdx] != NULL && GetTextureManager() != NULL)
			{
				m_apPreparation[nIdx]->BindTexture(GetTextureManager()->GetTexture(m_nPreparationTexIdx[1]));
			}
			m_State[nIdx] = STATE_WAIT_PARTNER;
		}
		else if (m_nSelectPlayer[nIdx] != m_nSelectPlayer[(nIdx + 1) % MAX_NUM_PLAYER])
		{// すでに相手が選んだ番号ではない
			if (m_apPreparation[nIdx] != NULL && GetTextureManager() != NULL)
			{
				m_apPreparation[nIdx]->BindTexture(GetTextureManager()->GetTexture(m_nPreparationTexIdx[1]));
			}
			m_State[nIdx] = STATE_WAIT_PARTNER;
		}
	}
}

//=============================================================================
// キャラセレクトのステージの種類番号を変えるための入力待機処理
//=============================================================================
void CCharaSelect::WaitInputToStageSelect(int nIdx)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL || CManager::GetClient() == NULL) return;

	if (pKey->GetTrigger(DIK_A) == true ||
		CManager::GetXInput()->GetPress(0, CXInput::XIJS_BUTTON_2) == true ||
		CManager::GetXInput()->GetPress(0, CXInput::XIJS_BUTTON_18) == true ||
		pKey->GetRepeat(DIK_A) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_2) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_18) == true)
	{// 左方向の入力がされた
		ChangeNoneStagePolygon(CHARASELECT_MAPTYPE_NUMBER);
		m_nSelectStageSide = (m_nSelectStageSide + ((CGame::MAPTYPE_MAX / 2) - 1)) % (CGame::MAPTYPE_MAX / 2);
		ChangeSelectStagePolygon(CHARASELECT_MAPTYPE_NUMBER);
	}
	else if (pKey->GetTrigger(DIK_D) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_3) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_19) == true ||
		pKey->GetRepeat(DIK_D) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_3) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_19) == true)
	{// 右方向の入力がされた
		ChangeNoneStagePolygon(CHARASELECT_MAPTYPE_NUMBER);
		m_nSelectStageSide = (m_nSelectStageSide + 1) % (CGame::MAPTYPE_MAX / 2);
		ChangeSelectStagePolygon(CHARASELECT_MAPTYPE_NUMBER);
	}
	else if (pKey->GetTrigger(DIK_W) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_0) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_16) == true ||
		pKey->GetRepeat(DIK_W) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_0) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_16) == true)
	{// 上方向の入力がされた
		ChangeNoneStagePolygon(CHARASELECT_MAPTYPE_NUMBER);
		m_nSelectStageVertical = (m_nSelectStageVertical + 1) % (CGame::MAPTYPE_MAX / 2);
		ChangeSelectStagePolygon(CHARASELECT_MAPTYPE_NUMBER);
	}
	else if (pKey->GetTrigger(DIK_S) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_1) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_17) == true ||
		pKey->GetRepeat(DIK_S) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_1) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_17) == true)
	{// 下方向の入力がされた
		ChangeNoneStagePolygon(CHARASELECT_MAPTYPE_NUMBER);
		m_nSelectStageVertical = (m_nSelectStageVertical + ((CGame::MAPTYPE_MAX / 2) - 1)) % (CGame::MAPTYPE_MAX / 2);
		ChangeSelectStagePolygon(CHARASELECT_MAPTYPE_NUMBER);
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		m_State[nIdx] = STATE_END;
	}
}

//=============================================================================
// キャラセレクトのステージ選択用ポリゴンを選択されていない状態にする
//=============================================================================
void CCharaSelect::ChangeNoneStagePolygon(int nSelect)
{
	if (m_apStage[nSelect] != NULL)
	{
		m_apStage[nSelect]->SetState(CScene2DFlash::STATE_NONE);
		m_apStage[nSelect]->SetScale(CHARASELECT_STAGEPOLYGON_WIDTH_NONE, CHARASELECT_STAGEPOLYGON_HEIGHT_NONE);
		m_apStage[nSelect]->SetCol(CHARASELECT_STAGEPOLYGON_COL_NONE);
		m_apStage[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// キャラセレクトのステージ選択用ポリゴンを選択された状態にする
//=============================================================================
void CCharaSelect::ChangeSelectStagePolygon(int nSelect)
{
	if (m_apStage[nSelect] != NULL)
	{
		m_apStage[nSelect]->SetState(CScene2DFlash::STATE_SELECT);
		m_apStage[nSelect]->SetScale(CHARASELECT_STAGEPOLYGON_WIDTH_SELECT, CHARASELECT_STAGEPOLYGON_HEIGHT_SELECT);
		m_apStage[nSelect]->SetCol(CHARASELECT_STAGEPOLYGON_COL_SELECT);
		m_apStage[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// キャラセレクトの円形にプレイヤーを移動させる処理
//=============================================================================
void CCharaSelect::CircleRotation(int nIdx)
{
	float fAddRot = 0.0f;
	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{
		m_pPlayer[nIdx][nCntType]->SetPos(D3DXVECTOR3(sinf(m_fSelectRot[nIdx] + fAddRot) * CHARASELECT_CIRCLE_RADIUS, 0.0f, cosf(m_fSelectRot[nIdx] + fAddRot) * CHARASELECT_CIRCLE_RADIUS));

		// 回転量加算
		fAddRot += D3DX_PI * 2.0f / m_nNumPlayerData;
		if (fAddRot > D3DX_PI)
		{
			fAddRot -= D3DX_PI * 2.0f;
		}
	}
}

//=============================================================================
// キャラセレクトのプレイヤー描画処理
//=============================================================================
void CCharaSelect::CharaDraw(int nCntTex)
{
	// レンダリングターゲットをテクスチャに設定
	CManager::GetRenderer()->SetRenderTarget(m_apRenderTexture[nCntTex]);

	//ライトマネージャーの生成
	CLightManager *pLightManager = NULL;
	pLightManager = CLightManager::Create(1);
	pLightManager->SettingLight(CDirectionalLight::Create(D3DXVECTOR3(-0.10f, 0.63f, -0.77f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)), 0);

	// プレイヤーの描画(今はポリゴンなのでライティングを外す)
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{// プレイヤーの種類の総数分繰り返し
		if (m_pPlayer[nCntTex] != NULL)
		{
			m_pPlayer[nCntTex][nCntType]->Draw();
		}
	}
	CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_LIGHTING, true);

	// デフォルトのレンダリングターゲットに戻す
	CManager::GetRenderer()->ResetRenderTarget();

	//ライトマネージャーの破棄
	if (pLightManager != NULL)
	{
		pLightManager->Uninit();
		delete pLightManager;
		pLightManager = NULL;
	}
}

//=============================================================================
// キャラセレクトの描画したテクスチャでバックバッファに描画する処理
//=============================================================================
void CCharaSelect::TextureDraw()
{
	// レンダリングターゲットを元に戻す
	CManager::GetRenderer()->ResetRenderTarget();

	// 背景ポリゴンを描画
	if (m_pBg != NULL)
	{
		m_pBg->Draw();
	}

	// ポリゴンを２枚描画する
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPolygonPlayer[nCntPlayer] != NULL)
		{
			m_apPolygonPlayer[nCntPlayer]->Draw();
		}
	}

	// 準備中ポリゴンを描画する
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPreparation[nCntPlayer] != NULL)
		{
			m_apPreparation[nCntPlayer]->Draw();
		}
	}

	// プレイヤー番号表示用ポリゴンを描画する
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_apPlayerNumber[nCntPlayer] != NULL)
		{
			m_apPlayerNumber[nCntPlayer]->Draw();
		}
	}

	// 自分の番号表示用ポリゴン用
	if (m_pYouPolygon != NULL)
	{
		// 自分のクライアント番号によって座標を設定
		if (CManager::GetClient() != NULL)
		{
			D3DXVECTOR3 YouPolygonPos = m_pYouPolygon->GetPos();
			float fPosX = m_fYouPolygonPosXDef + (SCREEN_WIDTH / 2.0f) * CManager::GetClient()->GetClientId();
			YouPolygonPos.x = fPosX;
			m_pYouPolygon->SetPos(YouPolygonPos);
			m_pYouPolygon->SetVtxBuffPos();
		}

		m_pYouPolygon->Draw();
	}

	// ステージ選択用のポリゴンを描画
	for (int nCntType = 0; nCntType < CGame::MAPTYPE_MAX; nCntType++)
	{
		if (m_apStage[nCntType] != NULL)
		{
			m_apStage[nCntType]->Draw();
		}
	}
}

//=============================================================================
// キャラセレクトのシステム情報を読み込む
//=============================================================================
void CCharaSelect::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(CHARASELECT_SYSTEM_FILENAME);
	if (pFileLoader != NULL)
	{
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// 読み込み開始の合図だった
			LoadSystemScript(pFileLoader, aStr);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// キャラセレクトのシステム情報をファイルから読み込む
//=============================================================================
void CCharaSelect::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntPreTex = 0;
	int nCntPlayerNumTex = 0;
	int nCntPre = 0;
	int nCntPlayerNum = 0;
	int nCntPlayerData = 0;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// 読み込むテクスチャの数だった
			CreateTexture(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// 読み込むテクスチャのファイル名だった
			LoadTexFileName(pStr, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_PLAYERDATA) == 0)
		{// 読み込むプレイヤーデータの総数だった
			m_nNumPlayerData = CFunctionLib::ReadInt(pStr, NUM_PLAYERDATA);
			CreatePlayerDataPointer();
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERDATA_FILENAME) == 0)
		{// 読み込むプレイヤーデータのファイル名だった
			LoadPlayerDataFileName(pStr, nCntPlayerData);
			nCntPlayerData++;
		}
		else if (CFunctionLib::Memcmp(pStr, BGPOLYGONSET) == 0)
		{// 背景用ポリゴン情報だった
			LoadBgPolygon(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, PREPARATION_TEX_IDX) == 0)
		{// 準備中ポリゴンの使用するテクスチャ番号情報だった
			m_nPreparationTexIdx[nCntPreTex] = CFunctionLib::ReadInt(pStr, PREPARATION_TEX_IDX);
			nCntPreTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, PREPARATIONSET) == 0)
		{// 準備中ポリゴン情報だった
			LoadPreparation(pFileLoader, pStr, nCntPre);
			nCntPre++;
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERNUMBER_TEX_IDX) == 0)
		{// プレイヤー番号表示ポリゴンの使用するテクスチャ番号情報だった
			m_nPlayerNumberTexIdx[nCntPlayerNumTex] = CFunctionLib::ReadInt(pStr, PLAYERNUMBER_TEX_IDX);
			nCntPlayerNumTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERNUMBERSET) == 0)
		{// プレイヤー番号表示ポリゴン情報だった
			LoadPlayerNumber(pFileLoader, pStr, nCntPlayerNum);
			nCntPlayerNum++;
		}
		else if (CFunctionLib::Memcmp(pStr, YOUPOLYGONSET) == 0)
		{// 自分の番号表示ポリゴン情報だった
			LoadYouPolygon(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// キャラセレクトの読み込むテクスチャファイル名を読み込む
//=============================================================================
void CCharaSelect::LoadTexFileName(char *pStr, int nCntTex)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	char aTexFileName[256] = "\0";
	strcpy(aTexFileName, CFunctionLib::ReadString(pStr, aTexFileName, TEXTURE_FILENAME));

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), aTexFileName, &pTexture);

	// テクスチャ管轄クラスにポインタを設定する
	CTextureManager *pTextureManager = GetTextureManager();
	pTextureManager->SetTexture(pTexture, nCntTex);
	pTextureManager->SetFileName(pStr, nCntTex);
}

//=============================================================================
// キャラセレクトの読み込むプレイヤーデータのファイル名を読み込む
//=============================================================================
void CCharaSelect::LoadPlayerDataFileName(char *pStr, int nCntPlayerData)
{
	strcpy(m_aPlayerDataFileName[nCntPlayerData], CFunctionLib::ReadString(pStr, m_aPlayerDataFileName[nCntPlayerData], PLAYERDATA_FILENAME));
}

//=============================================================================
// キャラセレクトの背景ポリゴンの値を読み込む
//=============================================================================
void CCharaSelect::LoadBgPolygon(CFileLoader *pFileLoader, char *pStr)
{
	int nBgTexIdx = 0;
	D3DXVECTOR3 BgPos = CHARASELECTBG_POS_INI;
	D3DXCOLOR BgCol = CHARASELECTBG_COL_INI;
	float fBgWidth = CHARASELECTBG_WIDTH_INI;
	float fBgHeight = CHARASELECTBG_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nBgTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			BgPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			BgCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fBgWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fBgHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_BGPOLYGONSET) == 0)
		{// 背景ポリゴン情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_pBg = CScene2D::Create(BgPos, BgCol, fBgWidth, fBgHeight);
	if (m_pBg != NULL && GetTextureManager() != NULL)
	{
		m_pBg->BindTexture(GetTextureManager()->GetTexture(nBgTexIdx));
	}
}

//=============================================================================
// キャラセレクトの準備中ポリゴンの値を読み込む
//=============================================================================
void CCharaSelect::LoadPreparation(CFileLoader *pFileLoader, char *pStr, int nCntPre)
{
	D3DXVECTOR3 PreparationPos = CHARASELECTPREPARATION_0_POS_INI;
	D3DXCOLOR PreparationCol = CHARASELECTPREPARATION_0_COL_INI;
	float fPreparationWidth = CHARASELECTPREPARATION_0_WIDTH_INI;
	float fPreparationHeight = CHARASELECTPREPARATION_0_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			PreparationPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			PreparationCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fPreparationWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fPreparationHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PREPARATIONSET) == 0)
		{// 準備中ポリゴン情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_apPreparation[nCntPre] = CScene2D::Create(PreparationPos, PreparationCol, fPreparationWidth, fPreparationHeight);
	if (m_apPreparation[nCntPre] != NULL && GetTextureManager() != NULL)
	{
		m_apPreparation[nCntPre]->BindTexture(GetTextureManager()->GetTexture(m_nPreparationTexIdx[0]));
	}
}

//=============================================================================
// キャラセレクトのプレイヤー番号表示用ポリゴンの値を読み込む
//=============================================================================
void CCharaSelect::LoadPlayerNumber(CFileLoader *pFileLoader, char *pStr, int nCntPlayerNum)
{
	D3DXVECTOR3 PlayerNumberPos = CHARASELECTPLAYERNUMBER_0_POS_INI;
	D3DXCOLOR PlayerNumberCol = CHARASELECTPLAYERNUMBER_0_COL_INI;
	float fPlayerNumberWidth = CHARASELECTPLAYERNUMBER_0_WIDTH_INI;
	float fPlayerNumberHeight = CHARASELECTPLAYERNUMBER_0_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			PlayerNumberPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			PlayerNumberCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fPlayerNumberWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fPlayerNumberHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PLAYERNUMBERSET) == 0)
		{// プレイヤー番号表示ポリゴン情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_apPlayerNumber[nCntPlayerNum] = CScene2D::Create(PlayerNumberPos, PlayerNumberCol, fPlayerNumberWidth, fPlayerNumberHeight);
	if (m_apPlayerNumber[nCntPlayerNum] != NULL && GetTextureManager() != NULL)
	{
		m_apPlayerNumber[nCntPlayerNum]->BindTexture(GetTextureManager()->GetTexture(m_nPlayerNumberTexIdx[nCntPlayerNum]));
	}
}

//=============================================================================
// キャラセレクトの自分の番号表示用ポリゴンの値を読み込む
//=============================================================================
void CCharaSelect::LoadYouPolygon(CFileLoader *pFileLoader, char *pStr)
{
	int nYouPolygonTexIdx = 0;
	D3DXVECTOR3 YouPolygonPos = CHARASELECT_YOUPOLYGON_POS_INI;
	D3DXCOLOR YouPolygonCol = CHARASELECT_YOUPOLYGON_COL_INI;
	float fYouPolygonWidth = CHARASELECT_YOUPOLYGON_WIDTH_INI;
	float fYouPolygonHeight = CHARASELECT_YOUPOLYGON_HEIGHT_INI;
	m_fYouPolygonPosXDef = CHARASELECT_YOUPOLYGON_POS_INI.x;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nYouPolygonTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			YouPolygonPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			YouPolygonCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fYouPolygonWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fYouPolygonHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_YOUPOLYGONSET) == 0)
		{// 背景ポリゴン情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_pYouPolygon = CScene2D::Create(YouPolygonPos, YouPolygonCol, fYouPolygonWidth, fYouPolygonHeight);
	if (m_pYouPolygon != NULL && GetTextureManager() != NULL)
	{
		m_pYouPolygon->BindTexture(GetTextureManager()->GetTexture(nYouPolygonTexIdx));
	}
	m_fYouPolygonPosXDef = YouPolygonPos.x;
}

//=============================================================================
// キャラセレクトの変数を初期化する
//=============================================================================
void CCharaSelect::ClearVariable(void)
{
	m_nStateCounter = 0;
	m_nNumPlayerData = 1;
	m_aPlayerDataFileName = NULL;
	m_pYouPolygon = NULL;
	m_fYouPolygonPosXDef = 0.0f;
	m_nSelectStageSide = 0;
	m_nSelectStageVertical = 0;

	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// プレイヤーの総数分繰り返し
		m_State[nCntPlayer] = STATE_SELECT;
		m_fSelectRot[nCntPlayer] = 0.0f;
		m_fAddSelectRot[nCntPlayer] = 0.0f;
		m_apPolygonPlayer[nCntPlayer] = NULL;
		m_apRenderTexture[nCntPlayer] = NULL;
		m_apPlayerNumber[nCntPlayer] = NULL;
		m_apPreparation[nCntPlayer] = NULL;
		m_nSelectPlayer[nCntPlayer] = 0;
		m_nPreparationTexIdx[nCntPlayer] = 0;
		m_nPlayerNumberTexIdx[nCntPlayer] = 0;
		m_pPlayer[nCntPlayer] = NULL;
	}

	for (int nCntType = 0; nCntType < CGame::MAPTYPE_MAX; nCntType++)
	{// ステージの種類の数だけ繰り返し
		m_apStage[nCntType] = NULL;
	}
}

//=============================================================================
// キャラセレクトの状態を設定する
//=============================================================================
void CCharaSelect::SetState(const STATE state)
{
	m_State[CManager::GetClient()->GetClientId()] = state;
}

//=============================================================================
// キャラセレクトの状態を取得する
//=============================================================================
CCharaSelect::STATE CCharaSelect::GetState(void)
{
	return m_State[CManager::GetClient()->GetClientId()];
}

//=============================================================================
// キャラセレクトの使用するプレイヤーの種類番号を取得する
//=============================================================================
int CCharaSelect::GetPlayerNumber(int nIdx)
{
	return m_nSelectPlayer[nIdx];
}

//=============================================================================
// キャラセレクトの使用するマップの種類番号を取得する
//=============================================================================
int CCharaSelect::GetStageType(void)
{
	return m_nSelectStage;
}