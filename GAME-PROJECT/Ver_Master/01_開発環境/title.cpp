//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "title.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "scene2D.h"
#include "number.h"
#include "sky.h"
#include "meshfield.h"
#include "light.h"
#include "lightManager.h"
#include "player.h"
#include "playerManager.h"
#include "effectManager.h"
#include "modelcreate.h"
#include "block.h"
#include "debugproc.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TITLE_SYSTEM_FILENAME               "data/TEXT/MODE/title.ini"    // 初期化に使用するシステムファイル名
#define TITLE_TEAMLOGO_SOUND_TIMING         (1)                           // チームロゴを出している時にサウンドを鳴らすタイミング
#define TITLE_FADE_SPEED                    (0.015f)                      // フェードのスピード
#define TITLE_FADEOUT_TIMING                (180)                         // チームロゴ状態からフェードアウト状態に移る時間
#define TITLE_CHANGEMODE_TIMING             (1200)                        // 通常状態から画面遷移するまでの時間
#define TITLE_BG_SLIDE_UV                   (0.001f)                      // 背景のテクスチャ座標をどれくらい動かすか
#define TITLE_PLAYER_PRIORITY               (6)                           // プレイヤーの処理優先順位
#define TITLE_BGM_IDX                       (0)                           // タイトルのBGMの音番号
#define TITLE_SE_TEAMLOGO_IDX               (7)                           // チームロゴを出すときのSEの音番号
#define TITLE_SE_SELECT_IDX                 (8)                           // 項目を選択している時のSEの音番号
#define TITLE_SE_DECIDE_IDX                 (9)                           // 決定ボタンを押された時のSEの音番号
#define TITLE_DASHEFFECT_IDX                (10)                          // プレイヤーが走っているときのエフェクト

// カメラ用
#define TITLECAMERA_POSV_INI                (D3DXVECTOR3(0.0f, 1000.0f, -320.0f))
#define TITLECAMERA_POSR_UNDER_Y            (60.0f)
#define TITLECAMERA_POSR_INI                (D3DXVECTOR3(0.0f, 1000.0f - TITLECAMERA_POSR_UNDER_Y, 0.0f))
#define TITLECAMERA_POS_DOWN                (3.8f)
#define TITLECAMERA_POSV_MIN_Y              (120.0f)

// チームロゴ用背景ポリゴン初期化用
#define TITLETEAMBG_POS_INI                 (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLETEAMBG_COL_INI                 (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLETEAMBG_WIDTH_INI               (SCREEN_WIDTH / 2.0f)
#define TITLETEAMBG_HEIGHT_INI              (SCREEN_HEIGHT / 2.0f)
#define TITLETEAMBG_PRIORITY                (6)

// フェード用ポリゴン初期化用
#define TITLEFADE_POS_INI                   (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLEFADE_COL_INI                   (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
#define TITLEFADE_WIDTH_INI                 (SCREEN_WIDTH / 2.0f)
#define TITLEFADE_HEIGHT_INI                (SCREEN_HEIGHT / 2.0f)
#define TITLEFADE_PRIORITY                  (6)

// チームロゴ用ポリゴン初期化用
#define TITLETEAMLOGO_POS_INI               (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLETEAMLOGO_COL_INI               (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLETEAMLOGO_WIDTH_INI             (100.0f)
#define TITLETEAMLOGO_HEIGHT_INI            (100.0f)
#define TITLETEAMLOGO_PRIORITY              (6)

// タイトルロゴ初期化用
#define TITLELOGO_POS_INI                   (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 120.0f, 0.0f))
#define TITLELOGO_COL_INI                   (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLELOGO_WIDTH_INI                 (200.0f)
#define TITLELOGO_HEIGHT_INI                (120.0f)
#define TITLELOGO_PRIORITY                  (7)

// プレス待機初期化用
#define TITLEPRESS_POS_INI                  (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 500.0f, 0.0f))
#define TITLEPRESS_COL_INI                  (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEPRESS_WIDTH_INI                (150.0f)
#define TITLEPRESS_HEIGHT_INI               (60.0f)
#define TITLEPRESS_PRIORITY                 (6)

// ゲームモード背景ポリゴン初期化用
#define TITLEGAMEMODEBG_POS_INI             (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLEGAMEMODEBG_COL_INI             (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEGAMEMODEBG_WIDTH_INI           (SCREEN_WIDTH / 2.0f)
#define TITLEGAMEMODEBG_HEIGHT_INI          (SCREEN_HEIGHT / 2.0f)
#define TITLEGAMEMODEBG_PRIORITY            (6)

// ゲームモードロゴポリゴン初期化用
#define TITLEGAMEMODELOGO_POS_INI           (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 200.0f, 0.0f))
#define TITLEGAMEMODELOGO_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEGAMEMODELOGO_WIDTH_INI         (150.0f)
#define TITLEGAMEMODELOGO_HEIGHT_INI        (60.0f)
#define TITLEGAMEMODELOGO_INTERVAL_INI      (100.0f)
#define TITLEGAMEMODELOGO_PRIORITY          (7)

// 決定ポリゴン初期化用
#define TITLEDECIDELOGO_POS_INI             (D3DXVECTOR3(980.0f, 685.0f, 0.0f))
#define TITLEDECIDELOGO_COL_INI             (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEDECIDELOGO_WIDTH_INI           (90.0f)
#define TITLEDECIDELOGO_HEIGHT_INI          (30.0f)
#define TITLEDECIDELOGO_PRIORITY            (7)
#define TITLEDECIDELOGO_TEXIDX              (11)

// 戻るロゴポリゴン初期化用
#define TITLERETURNLOGO_POS_INI             (D3DXVECTOR3(1170.0f, 685.0f, 0.0f))
#define TITLERETURNLOGO_COL_INI             (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLERETURNLOGO_WIDTH_INI           (80.0f)
#define TITLERETURNLOGO_HEIGHT_INI          (28.0f)
#define TITLERETURNLOGO_PRIORITY            (7)
#define TITLERETURNLOGO_TEXIDX              (12)

// タイトルハイスコアロゴ初期化用
#define TITLE_HIGHSCORE_FALSH_COL           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLE_HIGHSCORELOGO_POS_INI         (D3DXVECTOR3(1010.0f, 475.0f, 0.0f))
#define TITLE_HIGHSCORELOGO_COL_INI         (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define TITLE_HIGHSCORELOGO_WIDTH_INI       (135.0f)
#define TITLE_HIGHSCORELOGO_HEIGHT_INI      (30.0f)
#define TITLE_HIGHSCORELOGO_TEXIDX          (13)
#define TITLE_HIGHSCORELOGO_PRIORITY        (7)

// タイトルハイスコア数字初期化用
#define TITLE_HIGHSCORENUMBER_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH - 90.0f, 555.0f, 0.0f))
#define TITLE_HIGHSCORENUMBER_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLE_HIGHSCORENUMBER_WIDTH_INI     (30.0f)
#define TITLE_HIGHSCORENUMBER_HEIGHT_INI    (25.0f)
#define TITLE_HIGHSCORENUMBER_INTERVAL_INI  (D3DXVECTOR3(-55.0f, 0.0f, 0.0f))
#define TITLE_HIGHSCORENUMBER_PRIORITY      (7)

// タイトルアイコン初期化用
#define TITLE_ICON_POS_INI                  (D3DXVECTOR3(230.0f, 560.0f, 0.0f))
#define TITLE_ICON_COL_INI                  (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLE_ICON_WIDTH_INI                (130.0f)
#define TITLE_ICON_HEIGHT_INI               (130.0f)
#define TITLE_ICON_TEXIDX                   (14)
#define TITLE_ICON_PRIORITY                 (7)

// タイトル道なりモデル初期化用
#define TITLE_LOAD_MODEL_INTERVAL           (150.0f)
#define TITLE_LOAD_ZPOS_INI                 (3000.0f)
#define TITLE_LOAD_WIDTH                    (500.0f)
#define TITLE_LOAD_MODELIDX                 (0)
#define TITLE_LOAD_SETMODEL_NUM             (50)


// タイトル配置物初期化用
#define TITLE_OBJECT_MODELIDX               (1)
#define TITLE_OBJECT_NUM_MODELTYPE          (3)
#define TITLE_OBJECT_SET_WIDTH              (1300)
#define TITLE_OBJECT_SET_DEPTH              (3000)
#define TITLE_OBJECT_POS_INI                (D3DXVECTOR3(2000.0f, 560.0f, 2800.0f))
#define TITLE_OBJECT_SETMODEL_NUM           (700)

// 値読み込み用のパス
// テクスチャ用
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// モデル用
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// エフェクト用
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// タイトルロゴ用
#define TITLELOGOSET "TITLELOGOSET"
#define END_TITLELOGOSET "END_TITLELOGOSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// プレス待機用
#define PRESSSET "PRESSSET"
#define END_PRESSSET "END_PRESSSET"

// チームロゴ用
#define TEAMLOGOSET "TEAMLOGOSET"
#define END_TEAMLOGOSET "END_TEAMLOGOSET"

// ゲームモード背景ポリゴン用
#define GAMEMODEBGSET "GAMEMODEBGSET"
#define END_GAMEMODEBGSET "END_GAMEMODEBGSET"

// ゲームモード選択ポリゴン用
#define GAMEMODELOGOSET "GAMEMODELOGOSET"
#define END_GAMEMODELOGOSET "END_GAMEMODELOGOSET"
#define INTERVAL "INTERVAL = "
#define NONE "NONE"
#define END_NONE "END_NONE"
#define SELECT "SELECT"
#define END_SELECT "END_SELECT"

// プレイヤーデータ用
#define NUM_PLAYERDATA "NUM_PLAYERDATA = "
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "
#define NUM_PLAYER "NUM_PLAYER = "
#define PLAYERSET "PLAYERSET"
#define END_PLAYERSET "END_PLAYERSET"
#define TYPE "TYPE = "
#define ROT "ROT = "

// ライト用
#define NUM_LIGHT "NUM_LIGHT = "
#define LIGHTSET "LIGHTSET"
#define END_LIGHTSET "END_LIGHTSET"
#define DIR "DIR = "
#define DIFFUSE "DIFFUSE = "
#define AMBIENT "AMBIENT = "
#define SPECULAR "SPECULAR = "

// 地面用
#define FIELDSET "FIELDSET"
#define END_FIELDSET "END_FIELDSET"
#define DEPTH "DEPTH = "
#define XBLOCK "XBLOCK = "
#define ZBLOCK "ZBLOCK = "
#define TEXU_SPLIT "TEXU_SPLIT = "
#define TEXV_SPLIT "TEXV_SPLIT = "
#define CULLING "CULLING = "
#define ADDTEX "ADDTEX = "

// 空用
#define SKYSET "SKYSET"
#define END_SKYSET "END_SKYSET"
#define RADIUS "RADIUS = "
#define ROLLSPEED "ROLLSPEED = "

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CTitle::m_nGameMode = CTitle::GAMEMODE_LOCAL1P;   // ゲームモード
int CTitle::m_nHighScore = 10000;                     // ゲームのハイスコア

//*****************************************************************************
// CTitleの処理
//*****************************************************************************
//=============================================================================
// タイトルのコンストラクタ
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// タイトルのデストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// タイトルの生成処理
//=============================================================================
CTitle *CTitle::Create()
{
	CTitle *pTitle = NULL;
	if (pTitle == NULL)
	{
		pTitle = new CTitle;
		if (pTitle != NULL)
		{
			pTitle->Init();
		}
	}
	return pTitle;
}

//=============================================================================
// タイトルの初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	// 変数を初期化する
	ClearVariable();

	// システムの初期化
	LoadSystem();

	// カメラの生成
	CreateCamera();

	// 空を生成する
	CreateSky();

	// 地面を生成する
	CreateField();

	// ライトを生成する
	CreateLight();

	// プレイヤー管轄クラスを生成する
	CreatePlayerManager();

	// プレイヤークラスを生成する
	CreatePlayer();

	// 走っているようなエフェクトを生成する
	CreateDashEffect();

	// 道なりにブロックを配置する
	CreateLoad();

	// 配置物生成
	CreateObject();

	// ゲームモード初期化
	m_nGameMode = GAMEMODE_LOCAL1P;

	return S_OK;
}

//=============================================================================
// タイトルの終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	// 共通の終了処理
	CBasemode::Uninit();

	// 各種ポリゴンを開放
	ReleaseFadePolygon();
	ReleaseTeamLogoPolygon();
	ReleaseTeamBgPolygon();
	ReleaseTitleLogoPolygon();
	ReleasePress();
	ReleaseGameModeBg();
	ReleaseGameModeLogo();
	ReleaseSky();
	ReleaseField();
	ReleaseLight();
	ReleasePlayer();
	ReleasePlayerManager();
	ReleasePlayerManagerPointer();
	ReleasePlayerDataPointer();
	ReleaseLightDataPointer();
	ReleaseHighScore();

	// 全てのオブジェクト開放
	CScene::ReleaseAll();

	// BGMを停止
	CManager::GetSound()->StopSound(TITLE_BGM_IDX);
}

//=============================================================================
// タイトルの更新処理
//=============================================================================
void CTitle::Update(void)
{
	// 状態によって処理わけ
	switch (m_State)
	{
	case STATE_TEAMLOGO:
		TeamLogoUpdate();
		break;
	case STATE_FADEOUT:
		FadeOutUpdate();
		break;
	case STATE_FADEIN:
		FadeInUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	case STATE_GAMEMODE:
		GameModeUpdate();
		break;
	}

	// 地面のUV座標を進める
	AddFieldTexUV();

	// 全ての更新処理
	CScene::UpdateAll();

	CDebugProc::Print(1, "タイトル画面\n");
}

//=============================================================================
// タイトルの描画処理
//=============================================================================
void CTitle::Draw(void)
{
	// カメラの設定
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera();
	}

	// シーンクラスの描画処理
	CScene::DrawAll();
}

//=============================================================================
// タイトルのテクスチャ管轄クラス生成処理
//=============================================================================
void CTitle::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}


//=============================================================================
// タイトルのカメラ生成処理
//=============================================================================
void CTitle::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager == NULL) return;

	// カメラを生成
	CCamera *pCamera = CCamera::Create();
	if (pCamera != NULL)
	{
		pCamera->SetPosV(TITLECAMERA_POSV_INI);
		pCamera->SetPosR(TITLECAMERA_POSR_INI);
		pCameraManager->SetCamera(pCamera);
	}
	SetCameraManager(pCameraManager);
}

//=============================================================================
// タイトルのフェード用ポリゴン生成処理
//=============================================================================
void CTitle::CreateFadePolygon(void)
{
	m_pFade = CScene2D::Create(TITLEFADE_POS_INI, TITLEFADE_COL_INI, TITLEFADE_WIDTH_INI, TITLEFADE_HEIGHT_INI, TITLEFADE_PRIORITY);
}

//=============================================================================
// タイトルのチームロゴ背景用ポリゴン生成処理
//=============================================================================
void CTitle::CreateTeamBgPolygon(void)
{
	m_pTeamBg = CScene2D::Create(TITLETEAMBG_POS_INI, TITLETEAMBG_COL_INI, TITLETEAMBG_WIDTH_INI, TITLETEAMBG_HEIGHT_INI, TITLETEAMBG_PRIORITY);
}

//=============================================================================
// タイトルのプレス待機用ポリゴン生成処理
//=============================================================================
void CTitle::CreatePress(void)
{
	m_pPress = CScene2DPress::Create(m_PressData.pos, m_PressData.col, m_PressData.fWidth, m_PressData.fHeight, 0.0f,
		80, 100, 4, TITLEPRESS_PRIORITY);
	if (m_pPress != NULL && GetTextureManager() != NULL)
	{
		m_pPress->BindTexture(GetTextureManager()->GetTexture(m_PressData.nTexIdx));
	}
}

//=============================================================================
// タイトルのゲームモード背景用ポリゴン生成処理
//=============================================================================
void CTitle::CreateGameModeBg(void)
{
	m_pGameLogoBg = CScene2D::Create(m_GameModeBgData.pos, m_GameModeBgData.col,
		m_GameModeBgData.fWidth, m_GameModeBgData.fHeight, TITLEGAMEMODEBG_PRIORITY);
	if (m_pGameLogoBg != NULL && GetTextureManager() != NULL)
	{
		m_pGameLogoBg->BindTexture(GetTextureManager()->GetTexture(m_GameModeBgData.nTexIdx));
	}

	m_pDecide = CScene2D::Create(TITLEDECIDELOGO_POS_INI, TITLEDECIDELOGO_COL_INI, TITLEDECIDELOGO_WIDTH_INI,
		TITLEDECIDELOGO_HEIGHT_INI, TITLEDECIDELOGO_PRIORITY);
	if (m_pDecide != NULL)
	{
		m_pDecide->BindTexture(GetTextureManager()->GetTexture(TITLEDECIDELOGO_TEXIDX));
	}

	m_pRetrun = CScene2D::Create(TITLERETURNLOGO_POS_INI, TITLERETURNLOGO_COL_INI, TITLERETURNLOGO_WIDTH_INI,
		TITLERETURNLOGO_HEIGHT_INI, TITLERETURNLOGO_PRIORITY);
	if (m_pRetrun != NULL)
	{
		m_pRetrun->BindTexture(GetTextureManager()->GetTexture(TITLERETURNLOGO_TEXIDX));
	}

	if (m_pIcon == NULL)
	{
		m_pIcon = CScene2D::Create(TITLE_ICON_POS_INI, TITLE_ICON_COL_INI,
			TITLE_ICON_WIDTH_INI, TITLE_ICON_HEIGHT_INI, TITLE_ICON_PRIORITY);
		if (m_pIcon != NULL && GetTextureManager() != NULL)
		{
			m_pIcon->BindTexture(GetTextureManager()->GetTexture(TITLE_ICON_TEXIDX));
		}
	}
}

//=============================================================================
// タイトルのゲームモード選択用ポリゴン生成処理
//=============================================================================
void CTitle::CreateGameModeLogo(void)
{
	int nGameModeTexIdx = m_GameModeLogoData.nTexIdx;
	D3DXVECTOR3 GameModeLogoPos = m_GameModeLogoData.pos;
	D3DXCOLOR GameModeLogoCol;
	float fGameModeLogoWidth;
	float fGameModeLogoHeight;
	CScene2DFlash::STATE GameModeLogoState;
	for (int nCntLogo = 0; nCntLogo < GAMEMODE_MAX; nCntLogo++)
	{
		// 値を設定
		if (nCntLogo == m_nGameMode)
		{// 選択された番号だった
			GameModeLogoCol = m_GameModeLogoPoly_SelectData.col;
			fGameModeLogoWidth = m_GameModeLogoPoly_SelectData.fWidth;
			fGameModeLogoHeight = m_GameModeLogoPoly_SelectData.fHeight;
			GameModeLogoState = CScene2DFlash::STATE_SELECT;
		}
		else
		{// 選択されていない番号だった
			GameModeLogoCol = m_GameModeLogoPoly_NoneData.col;
			fGameModeLogoWidth = m_GameModeLogoPoly_NoneData.fWidth;
			fGameModeLogoHeight = m_GameModeLogoPoly_NoneData.fHeight;
			GameModeLogoState = CScene2DFlash::STATE_NONE;
		}

		// ポリゴンを生成
		m_apGameModeLogo[nCntLogo] = CScene2DFlash::Create(GameModeLogoPos, GameModeLogoCol,
			fGameModeLogoWidth, fGameModeLogoHeight, 0.0f, GameModeLogoState, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f), 0.01f, 4, TITLEGAMEMODELOGO_PRIORITY);
		if (m_apGameModeLogo[nCntLogo] != NULL && GetTextureManager() != NULL)
		{
			m_apGameModeLogo[nCntLogo]->BindTexture(GetTextureManager()->GetTexture(nGameModeTexIdx));
		}
		nGameModeTexIdx++;
		GameModeLogoPos.y += m_GameModeLogoData.fInterval;
	}
}

//=============================================================================
// タイトルの空生成処理
//=============================================================================
void CTitle::CreateSky(void)
{
	m_pSky = CSky::Create(m_SkyData.pos, m_SkyData.rot, m_SkyData.col, m_SkyData.fRadius,
		m_SkyData.nXBlock, m_SkyData.nZBlock, m_SkyData.nTexIdx, m_SkyData.fRollSpeed);
	if (m_pSky != NULL && GetTextureManager() != NULL)
	{
		m_pSky->BindTexture(GetTextureManager()->GetTexture(m_SkyData.nTexIdx));
	}
}

//=============================================================================
// タイトルの地面生成処理
//=============================================================================
void CTitle::CreateField(void)
{
	m_pMeshField = CMeshField::Create(m_FieldData.pos, m_FieldData.rot, m_FieldData.col,
		m_FieldData.fWidth, m_FieldData.fDepth, m_FieldData.nXBlock, m_FieldData.nZBlock,
		m_FieldData.nTexUSplit, m_FieldData.nTexVSplit, NULL, m_FieldData.bCulling);
	if (m_pMeshField != NULL && GetTextureManager() != NULL)
	{
		m_pMeshField->BindTexture(GetTextureManager()->GetTexture(m_FieldData.nTexIdx));
	}
}

//=============================================================================
// タイトルのライト生成処理
//=============================================================================
void CTitle::CreateLight(void)
{
	if (m_nNumLight <= 0) { return; }

	m_pLightManager = CLightManager::Create(m_nNumLight);
	if (m_pLightManager != NULL)
	{
		CLight *pLight = NULL;
		for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
		{
			pLight = CDirectionalLight::Create(m_pLightData[nCntLight].Dir, m_pLightData[nCntLight].Diffuse,
				m_pLightData[nCntLight].Ambient, m_pLightData[nCntLight].Specular);
			m_pLightManager->SettingLight(pLight, nCntLight);
		}
	}
}

//=============================================================================
// タイトルのプレイヤーを生成する処理
//=============================================================================
void CTitle::CreatePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = m_pPlayerManager[m_pPlayerData[nCntPlayer].nType]->SetPlayer(m_pPlayerData[nCntPlayer].pos, m_pPlayerData[nCntPlayer].rot, 0, TITLE_PLAYER_PRIORITY);
	}
}

//=============================================================================
// タイトルのプレイヤー管轄クラス用のポインタを生成する処理
//=============================================================================
void CTitle::CreatePlayerManagerPointer(void)
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

	// プレイヤーマネージャー
	m_pPlayerManager = new CPlayerManager*[m_nNumPlayerData];
	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{
		m_pPlayerManager[nCntType] = NULL;
	}
}

//=============================================================================
// タイトルのプレイヤー管轄クラスを生成する処理
//=============================================================================
void CTitle::CreatePlayerManager(void)
{
	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{
		m_pPlayerManager[nCntType] = CPlayerManager::Create(m_aPlayerDataFileName[nCntType]);
	}
}

//=============================================================================
// タイトルのプレイヤーデータ用のポインタを生成する処理
//=============================================================================
void CTitle::CreatePlayerDataPointer(void)
{
	// プレイヤー
	m_pPlayer = new CPlayer*[m_nNumPlayer];
	for (int nCntType = 0; nCntType < m_nNumPlayer; nCntType++)
	{
		m_pPlayer[nCntType] = NULL;
	}

	// プレイヤーデータ
	m_pPlayerData = new PLAYERDATA[m_nNumPlayer];
	for (int nCntType = 0; nCntType < m_nNumPlayer; nCntType++)
	{
		m_pPlayerData[nCntType].nType = 0;
		m_pPlayerData[nCntType].pos = INITIALIZE_D3DXVECTOR3;
		m_pPlayerData[nCntType].rot = INITIALIZE_D3DXVECTOR3;
	}
}

//=============================================================================
// タイトルのライトデータ用のポインタを生成する処理
//=============================================================================
void CTitle::CreateLightDataPointer(void)
{
	m_pLightData = new LIGHTDATA[m_nNumLight];
	for (int nCntLight = 0; nCntLight < m_nNumLight; nCntLight++)
	{
		m_pLightData[nCntLight].Dir = INITIALIZE_D3DXVECTOR3;
		m_pLightData[nCntLight].Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_pLightData[nCntLight].Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		m_pLightData[nCntLight].Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// タイトルのハイスコアを生成する処理
//=============================================================================
void CTitle::CreateHighScore(void)
{
	// ロゴポリゴン生成
	if (m_pHighScoreLogo == NULL)
	{
		m_pHighScoreLogo = CScene2D::Create(TITLE_HIGHSCORELOGO_POS_INI, TITLE_HIGHSCORELOGO_COL_INI,
			TITLE_HIGHSCORELOGO_WIDTH_INI, TITLE_HIGHSCORELOGO_HEIGHT_INI, TITLE_HIGHSCORELOGO_PRIORITY);
		if (m_pHighScoreLogo != NULL && GetTextureManager() != NULL)
		{
			m_pHighScoreLogo->BindTexture(GetTextureManager()->GetTexture(TITLE_HIGHSCORELOGO_TEXIDX));
		}
	}

	// 数字ポリゴン生成
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(10);
	}
	if (m_pHighScore == NULL)
	{
		m_pHighScore = CNumber::Create(TITLE_HIGHSCORENUMBER_POS_INI, TITLE_HIGHSCORENUMBER_COL_INI,
			TITLE_HIGHSCORENUMBER_WIDTH_INI, TITLE_HIGHSCORENUMBER_HEIGHT_INI, TITLE_HIGHSCORENUMBER_INTERVAL_INI,
			pTexture, CTitle::GetHighScore(), CNumberPolygon::STATE_NONE, 3, 0.0f, TITLE_HIGHSCORENUMBER_PRIORITY);
	}
}

//=============================================================================
// タイトルの道なりモデルを生成する処理
//=============================================================================
void CTitle::CreateLoad(void)
{
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL) { return; }

	D3DXVECTOR3 LoadPos = D3DXVECTOR3(TITLE_LOAD_WIDTH, 0.0f, TITLE_LOAD_ZPOS_INI);

	// 右側
	for (int nCntModel = 0; nCntModel < TITLE_LOAD_SETMODEL_NUM; nCntModel++)
	{
		CBlock::Create(LoadPos, INITIALIZE_D3DXVECTOR3, 0, 0, pModelCreate->GetMesh(TITLE_LOAD_MODELIDX),
			pModelCreate->GetBuffMat(TITLE_LOAD_MODELIDX), pModelCreate->GetNumMat(TITLE_LOAD_MODELIDX),
			pModelCreate->GetTexture(TITLE_LOAD_MODELIDX));
		LoadPos.z -= TITLE_LOAD_MODEL_INTERVAL;
	}

	// 座標を初期値に設定
	LoadPos.x = -TITLE_LOAD_WIDTH;
	LoadPos.z = TITLE_LOAD_ZPOS_INI;

	// 左側
	for (int nCntModel = 0; nCntModel < TITLE_LOAD_SETMODEL_NUM; nCntModel++)
	{
		CBlock::Create(LoadPos, INITIALIZE_D3DXVECTOR3, 0, 0, pModelCreate->GetMesh(TITLE_LOAD_MODELIDX),
			pModelCreate->GetBuffMat(TITLE_LOAD_MODELIDX), pModelCreate->GetNumMat(TITLE_LOAD_MODELIDX),
			pModelCreate->GetTexture(TITLE_LOAD_MODELIDX));
		LoadPos.z -= TITLE_LOAD_MODEL_INTERVAL;
	}
}

//=============================================================================
// タイトルの配置物生成処理
//=============================================================================
void CTitle::CreateObject(void)
{
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL) { return; }

	int nModelIdx = 0;
	D3DXVECTOR3 LoadPos = D3DXVECTOR3(TITLE_LOAD_WIDTH, 0.0f, TITLE_LOAD_ZPOS_INI);

	// 右側
	for (int nCntModel = 0; nCntModel < TITLE_OBJECT_SETMODEL_NUM; nCntModel++)
	{
		// 使用するモデル番号の設定
		nModelIdx = rand() % TITLE_OBJECT_NUM_MODELTYPE + TITLE_OBJECT_MODELIDX;

		// 配置する座標の設定
		LoadPos.x = (float)(-(rand() % TITLE_OBJECT_SET_WIDTH) + TITLE_OBJECT_POS_INI.x);
		LoadPos.z = (float)(-(rand() % TITLE_OBJECT_SET_DEPTH) + TITLE_OBJECT_POS_INI.z);

		// モデル配置
		CBlock::Create(LoadPos, INITIALIZE_D3DXVECTOR3, 0, 0, pModelCreate->GetMesh(nModelIdx),
			pModelCreate->GetBuffMat(nModelIdx), pModelCreate->GetNumMat(nModelIdx),
			pModelCreate->GetTexture(nModelIdx));
	}

	// 左側
	for (int nCntModel = 0; nCntModel < TITLE_OBJECT_SETMODEL_NUM; nCntModel++)
	{
		// 使用するモデル番号の設定
		nModelIdx = rand() % TITLE_OBJECT_NUM_MODELTYPE + TITLE_OBJECT_MODELIDX;

		// 配置する座標の設定
		LoadPos.x = (float)((rand() % TITLE_OBJECT_SET_WIDTH) - TITLE_OBJECT_POS_INI.x);
		LoadPos.z = (float)(-(rand() % TITLE_OBJECT_SET_DEPTH) + TITLE_OBJECT_POS_INI.z);

		// モデル配置
		CBlock::Create(LoadPos, INITIALIZE_D3DXVECTOR3, 0, 0, pModelCreate->GetMesh(nModelIdx),
			pModelCreate->GetBuffMat(nModelIdx), pModelCreate->GetNumMat(nModelIdx),
			pModelCreate->GetTexture(nModelIdx));
	}
}

//=============================================================================
// タイトルのフェード用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseFadePolygon(void)
{
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		m_pFade = NULL;
	}
}

//=============================================================================
// タイトルのチームロゴ背景用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseTeamLogoPolygon(void)
{
	if (m_pTeamLogo != NULL)
	{
		m_pTeamLogo->Uninit();
		m_pTeamLogo = NULL;
	}
}

//=============================================================================
// タイトルのチームロゴ背景用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseTeamBgPolygon(void)
{
	if (m_pTeamBg != NULL)
	{
		m_pTeamBg->Uninit();
		m_pTeamBg = NULL;
	}
}

//=============================================================================
// タイトルのタイトルロゴ用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseTitleLogoPolygon(void)
{
	if (m_pTitleLogo != NULL)
	{
		m_pTitleLogo->Uninit();
		m_pTitleLogo = NULL;
	}
}

//=============================================================================
// タイトルのプレス待機用ポリゴン開放処理
//=============================================================================
void CTitle::ReleasePress(void)
{
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}
}

//=============================================================================
// タイトルのゲームモード背景用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseGameModeBg(void)
{
	if (m_pGameLogoBg != NULL)
	{
		m_pGameLogoBg->Uninit();
		m_pGameLogoBg = NULL;
	}

	if (m_pDecide != NULL)
	{
		m_pDecide->Uninit();
		m_pDecide = NULL;
	}

	if (m_pRetrun != NULL)
	{
		m_pRetrun->Uninit();
		m_pRetrun = NULL;
	}

	if (m_pIcon != NULL)
	{
		m_pIcon->Uninit();
		m_pIcon = NULL;
	}
}

//=============================================================================
// タイトルのライト開放処理
//=============================================================================
void CTitle::ReleaseLight(void)
{
	if (m_pLightManager != NULL)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = NULL;
	}
}

//=============================================================================
// タイトルのゲームモード選択用ポリゴン開放処理
//=============================================================================
void CTitle::ReleaseGameModeLogo(void)
{
	for (int nCntLogo = 0; nCntLogo < GAMEMODE_MAX; nCntLogo++)
	{
		if (m_apGameModeLogo[nCntLogo] != NULL)
		{
			m_apGameModeLogo[nCntLogo]->Uninit();
			m_apGameModeLogo[nCntLogo] = NULL;
		}
	}
}

//=============================================================================
// タイトルの空開放処理
//=============================================================================
void CTitle::ReleaseSky(void)
{
	if (m_pSky != NULL)
	{
		m_pSky->Uninit();
		m_pSky = NULL;
	}
}

//=============================================================================
// タイトルのプレイヤーを開放する処理
//=============================================================================
void CTitle::ReleasePlayer(void)
{
	if (m_pPlayer == NULL) { return; }

	for (int nCntType = 0; nCntType < m_nNumPlayer; nCntType++)
	{
		if (m_pPlayer[nCntType] != NULL)
		{
			m_pPlayer[nCntType]->Uninit();
			m_pPlayer[nCntType] = NULL;
		}
	}
	delete[] m_pPlayer;
	m_pPlayer = NULL;
}

//=============================================================================
// タイトルのプレイヤー管轄クラス用のポインタを開放する処理
//=============================================================================
void CTitle::ReleasePlayerManagerPointer(void)
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
// タイトルのプレイヤー管轄クラスを開放する処理
//=============================================================================
void CTitle::ReleasePlayerManager(void)
{
	if (m_pPlayerManager == NULL) { return; }

	for (int nCntType = 0; nCntType < m_nNumPlayerData; nCntType++)
	{
		if (m_pPlayerManager[nCntType] != NULL)
		{
			m_pPlayerManager[nCntType]->Uninit();
			delete m_pPlayerManager[nCntType];
			m_pPlayerManager[nCntType] = NULL;
		}
	}
	delete[] m_pPlayerManager;
	m_pPlayerManager = NULL;
}

//=============================================================================
// タイトルのプレイヤーデータ用ポインタを開放する処理
//=============================================================================
void CTitle::ReleasePlayerDataPointer(void)
{
	if (m_pPlayerData == NULL) { return; }

	delete[] m_pPlayerData;
	m_pPlayerData = NULL;
}


//=============================================================================
// タイトルのライトデータ用ポインタを開放する処理
//=============================================================================
void CTitle::ReleaseLightDataPointer(void)
{
	if (m_pLightData == NULL) { return; }

	delete[] m_pLightData;
	m_pLightData = NULL;
}

//=============================================================================
// タイトルの地面開放処理
//=============================================================================
void CTitle::ReleaseField(void)
{
	if (m_pMeshField != NULL)
	{
		m_pMeshField->Uninit();
		m_pMeshField = NULL;
	}
}

//=============================================================================
// タイトルのハイスコア開放処理
//=============================================================================
void CTitle::ReleaseHighScore(void)
{
	// ロゴポリゴン開放
	if (m_pHighScoreLogo != NULL)
	{
		m_pHighScoreLogo->Uninit();
		m_pHighScoreLogo = NULL;
	}

	// 数字ポリゴン開放
	if (m_pHighScore != NULL)
	{
		m_pHighScore->Uninit();
		m_pHighScore = NULL;
	}
}

//=============================================================================
// タイトルのチームロゴ状態の更新処理
//=============================================================================
void CTitle::TeamLogoUpdate(void)
{
	CDebugProc::Print(1, "チームロゴ状態\n");
	if (CManager::GetFade()->GetFade() != CFade::FADE_NONE) return;

	m_nStateCounter++;
	CDebugProc::Print(1, "%d\n", m_nStateCounter);
	if (m_nStateCounter >= TITLE_FADEOUT_TIMING)
	{// 一定値を超えた
		m_nStateCounter = 0;
		CreateFadePolygon();
		m_State = STATE_FADEOUT;
	}
	else
	{
		WaitInputToNormal();
	}
	if (m_nStateCounter == TITLE_TEAMLOGO_SOUND_TIMING)
	{
		CManager::GetSound()->PlaySound(TITLE_SE_TEAMLOGO_IDX);
	}
}

//=============================================================================
// タイトルのフェードアウト状態の更新処理
//=============================================================================
void CTitle::FadeOutUpdate(void)
{
	CDebugProc::Print(1, "フェードアウト状態\n");
	D3DXCOLOR FadeCol = m_pFade->GetCol();
	FadeCol.a += TITLE_FADE_SPEED;
	CDebugProc::Print(1, "%.1f\n", FadeCol.a);
	if (FadeCol.a >= 1.0f)
	{
		ReleaseTeamBgPolygon();
		ReleaseTeamLogoPolygon();
		FadeCol.a = 1.0f;
		m_State = STATE_FADEIN;
		CManager::GetSound()->PlaySound(TITLE_BGM_IDX);
	}

	if (m_pFade != NULL)
	{
		m_pFade->SetCol(FadeCol);
		m_pFade->SetVtxBuffCol();
	}
}

//=============================================================================
// タイトルのフェードイン状態の更新処理
//=============================================================================
void CTitle::FadeInUpdate(void)
{
	CDebugProc::Print(1, "フェードイン状態\n");
	D3DXCOLOR FadeCol = m_pFade->GetCol();
	FadeCol.a -= TITLE_FADE_SPEED;
	CDebugProc::Print(1, "%.1f\n", FadeCol.a);
	if (FadeCol.a <= 0.0f)
	{
		ReleaseFadePolygon();
		m_State = STATE_NORMAL;
	}

	if (m_pFade != NULL)
	{
		m_pFade->SetCol(FadeCol);
		m_pFade->SetVtxBuffCol();
	}
}

//=============================================================================
// タイトルの通常状態の更新処理
//=============================================================================
void CTitle::NormalUpdate(void)
{
	CDebugProc::Print(1, "通常状態\n");

	// カメラを下に下げる
	bool bAdvance = false;
	CCameraManager *pCameraManager = GetCameraManager();
	if (pCameraManager == NULL) { return; }
	CCamera *pCamera = pCameraManager->GetCamera();
	if (pCamera != NULL)
	{
		D3DXVECTOR3 CameraPosV = pCamera->GetPosV();
		D3DXVECTOR3 CameraPosR = pCamera->GetPosR();
		CameraPosV.y -= TITLECAMERA_POS_DOWN;
		CameraPosR.y -= TITLECAMERA_POS_DOWN;
		if (CameraPosV.y <= TITLECAMERA_POSV_MIN_Y)
		{
			CameraPosV.y = TITLECAMERA_POSV_MIN_Y;
			CameraPosR.y = TITLECAMERA_POSV_MIN_Y - TITLECAMERA_POSR_UNDER_Y;
			bAdvance = true;

			// プレスポリゴンが作られてなければ生成する
			if (m_pPress == NULL)
			{
				CreatePress();
			}
		}

		// 決定ボタンが押されたらカメラの位置を下にする
		CInputKeyboard *pKeyboard = CManager::GetKeyboard();
		CXInput *pXInput = CManager::GetXInput();
		if (pKeyboard == NULL || pXInput == NULL) { return; }
		if (pKeyboard->GetTrigger(DIK_RETURN) == true)
		{// 決定ボタンが押された
			CameraPosV.y = TITLECAMERA_POSV_MIN_Y;
			CameraPosR.y = TITLECAMERA_POSV_MIN_Y - TITLECAMERA_POSR_UNDER_Y;
		}
		else
		{// キーボードが押されていない
			for (int nCntButton = CXInput::XIJS_BUTTON_8; nCntButton < CXInput::XIJS_BUTTON_16; nCntButton++)
			{// 判定するボタンの数だけ繰り返し
				if (CManager::GetXInput()->GetTrigger(0, (CXInput::XIJS_BUTTON)nCntButton) == true)
				{// 何かボタンが押された
					CameraPosV.y = TITLECAMERA_POSV_MIN_Y;
					CameraPosR.y = TITLECAMERA_POSV_MIN_Y - TITLECAMERA_POSR_UNDER_Y;
				}
			}
		}

		pCamera->SetPosV(CameraPosV);
		pCamera->SetPosR(CameraPosR);
		pCameraManager->SetCamera(pCamera);
	}

	// カメラが下に下がり切っていなかったらこれ以上処理しない
	if (bAdvance == false) { return; }

	// フェードが使用されていたらこれ以上処理しない
	if (CManager::GetFade()->GetFade() != CFade::FADE_NONE) return;

	m_nStateCounter++;
	CDebugProc::Print(1, "%d\n", m_nStateCounter);
	if (m_nStateCounter >= TITLE_CHANGEMODE_TIMING)
	{// 一定値を超えた
		SetNextMode_None();
	}
	else
	{// まだ超えていない
		WaitInputToGameMode();
	}
}

//=============================================================================
// タイトルのゲームモード選択状態の更新処理
//=============================================================================
void CTitle::GameModeUpdate(void)
{
	// フェードが使用されていたらこれ以上処理しない
	if (CManager::GetFade()->GetFade() != CFade::FADE_NONE) return;

	// ゲームモードを選択させる
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	if (pKey == NULL || pXInput == NULL)return;

	if (pKey->GetTrigger(DIK_W) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_16) == true ||
		pKey->GetRepeat(DIK_W) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_0) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_16) == true)
	{// 上方向の入力がされた
		ChangeNoneGameModeLogoPoly(m_nGameMode);
		m_nGameMode = (m_nGameMode + (GAMEMODE_MAX - 1)) % GAMEMODE_MAX;
		ChangeSelectGameModeLogoPoly(m_nGameMode);
		CManager::GetSound()->PlaySound(TITLE_SE_SELECT_IDX);
		if (m_pIcon != NULL)
		{
			m_pIcon->BindTexture(GetTextureManager()->GetTexture(TITLE_ICON_TEXIDX + m_nGameMode));
		}
	}
	else if (pKey->GetTrigger(DIK_S) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_17) == true ||
		pKey->GetRepeat(DIK_S) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_1) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_17) == true)
	{// 下方向の入力がされた
		ChangeNoneGameModeLogoPoly(m_nGameMode);
		m_nGameMode = (m_nGameMode + 1) % GAMEMODE_MAX;
		ChangeSelectGameModeLogoPoly(m_nGameMode);
		CManager::GetSound()->PlaySound(TITLE_SE_SELECT_IDX);
		if (m_pIcon != NULL)
		{
			m_pIcon->BindTexture(GetTextureManager()->GetTexture(TITLE_ICON_TEXIDX + m_nGameMode));
		}
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		SetNextMode_Push();
		CManager::GetSound()->PlaySound(TITLE_SE_DECIDE_IDX);
	}
	else if (pKey->GetTrigger(DIK_BACKSPACE) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_10) == true)
	{// 戻るボタンが押された
		SetState(STATE_NORMAL);
		ReleaseGameModeBg();
		ReleaseGameModeLogo();
		ReleaseHighScore();
		CreatePress();
		CScene::DeathCheck();
	}
}

//=============================================================================
// タイトルの通常状態に行くための入力待機処理
//=============================================================================
void CTitle::WaitInputToNormal(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;

	if (pFade->GetFade() != CFade::FADE_NONE) { return; }

	if (pKey->GetTrigger(DIK_RETURN) == true)
	{// 決定ボタンが押された
		m_nStateCounter = 0;
		CreateFadePolygon();
		m_State = STATE_FADEOUT;
		CManager::GetSound()->StopSound(TITLE_SE_TEAMLOGO_IDX);
	}
	else
	{// キーボードが押されていない
		for (int nCntButton = CXInput::XIJS_BUTTON_8; nCntButton < CXInput::XIJS_BUTTON_16; nCntButton++)
		{// 判定するボタンの数だけ繰り返し
			if (CManager::GetXInput()->GetTrigger(0, (CXInput::XIJS_BUTTON)nCntButton) == true)
			{// 何かボタンが押された
				m_nStateCounter = 0;
				CreateFadePolygon();
				m_State = STATE_FADEOUT;
				CManager::GetSound()->StopSound(TITLE_SE_TEAMLOGO_IDX);
			}
		}
	}
}

//=============================================================================
// タイトルの次のモードに行くための入力待機処理
//=============================================================================
void CTitle::WaitInputToGameMode(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;

	if (pFade->GetFade() != CFade::FADE_NONE) { return; }

	if (pKey->GetTrigger(DIK_RETURN) == true)
	{// 決定ボタンが押された
		m_nStateCounter = 0;
		SetState(STATE_GAMEMODE);
		ReleasePress();
		CreateGameModeBg();
		CreateGameModeLogo();
		CreateHighScore();
		CManager::GetSound()->PlaySound(TITLE_SE_DECIDE_IDX);
	}
	else
	{// キーボードが押されていない
		for (int nCntButton = CXInput::XIJS_BUTTON_8; nCntButton < CXInput::XIJS_BUTTON_16; nCntButton++)
		{// 判定するボタンの数だけ繰り返し
			if (CManager::GetXInput()->GetTrigger(0, (CXInput::XIJS_BUTTON)nCntButton) == true)
			{// 何かボタンが押された
				m_nStateCounter = 0;
				SetState(STATE_GAMEMODE);
				ReleasePress();
				CreateGameModeBg();
				CreateGameModeLogo();
				CreateHighScore();
				CManager::GetSound()->PlaySound(TITLE_SE_DECIDE_IDX);
			}
		}
	}
}

//=============================================================================
// タイトルの次のモードに設定する処理(入力されなかった場合)
//=============================================================================
void CTitle::SetNextMode_None(void)
{
	CManager::GetFade()->SetFade(CManager::MODE_TITLE);
}

//=============================================================================
// タイトルの次のモードに設定する処理(入力された場合)
//=============================================================================
void CTitle::SetNextMode_Push(void)
{
	CManager::GetFade()->SetFade(CManager::MODE_CHARASELECT);
}

//=============================================================================
// タイトルのゲームモード用ポリゴンを選択されていない状態にする
//=============================================================================
void CTitle::ChangeNoneGameModeLogoPoly(int nSelect)
{
	D3DXCOLOR GameModeLogoColLogo = m_GameModeLogoPoly_NoneData.col;
	float fGameModeLogoWidthLogo = m_GameModeLogoPoly_NoneData.fWidth;
	float fGameModeLogoHeightLogo = m_GameModeLogoPoly_NoneData.fHeight;
	if (m_apGameModeLogo[nSelect] != NULL)
	{
		m_apGameModeLogo[nSelect]->SetState(CScene2DFlash::STATE_NONE);
		m_apGameModeLogo[nSelect]->SetScale(fGameModeLogoWidthLogo, fGameModeLogoHeightLogo);
		m_apGameModeLogo[nSelect]->SetCol(GameModeLogoColLogo);
		m_apGameModeLogo[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// タイトルのゲームモード用ポリゴンを選択された状態にする
//=============================================================================
void CTitle::ChangeSelectGameModeLogoPoly(int nSelect)
{
	D3DXCOLOR GameModeLogoColLogo = m_GameModeLogoPoly_SelectData.col;
	float fGameModeLogoWidthLogo = m_GameModeLogoPoly_SelectData.fWidth;
	float fGameModeLogoHeightLogo = m_GameModeLogoPoly_SelectData.fHeight;

	if (m_apGameModeLogo[nSelect] != NULL)
	{
		m_apGameModeLogo[nSelect]->SetState(CScene2DFlash::STATE_SELECT);
		m_apGameModeLogo[nSelect]->SetScale(fGameModeLogoWidthLogo, fGameModeLogoHeightLogo);
		m_apGameModeLogo[nSelect]->SetCol(GameModeLogoColLogo);
		m_apGameModeLogo[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// タイトルの地面のUV値を進める処理
//=============================================================================
void CTitle::AddFieldTexUV(void)
{
	if (m_pMeshField == NULL) { return; }

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nXBlock = m_pMeshField->GetXBlock();
		int nZBlock = m_pMeshField->GetZBlock();

		for (int nCntV = 0; nCntV < nZBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].tex.y += m_FieldData.fAddTex;
			}
			// ポインタを進める
			pVtx += nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
// タイトルの地面を走るエフェクトを生成する処理
//=============================================================================
void CTitle::CreateDashEffect(void)
{
	// エフェクト管轄クラスを取得
	CEffectManager *pEffectManager = GetEffectManager();
	if (pEffectManager == NULL) { return; }

	// エフェクトを出す
	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{
		D3DXVECTOR3 EffectPos = m_pPlayerData[nCntPlayer].pos;
		EffectPos.x += 20.0f + (m_pPlayerData[nCntPlayer].pos.x * 0.05f);
		EffectPos.z += 40.0f;
		pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, TITLE_DASHEFFECT_IDX);
		EffectPos.x -= 40.0f - (m_pPlayerData[nCntPlayer].pos.x * 0.05f);
		pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, TITLE_DASHEFFECT_IDX);
	}
}

//=============================================================================
// タイトルのシステム情報を読み込む
//=============================================================================
void CTitle::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(TITLE_SYSTEM_FILENAME);
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
// タイトルのシステム情報をファイルから読み込む
//=============================================================================
void CTitle::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntModel = 0;
	int nCntPlayerData = 0;
	int nCntPlayer = 0;
	int nCntLight = 0;
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
		else if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{// 読み込むモデルの数だった
			int nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			CModelCreate *pModelCreate = CModelCreate::Create(nNumModel);
			SetModelCreate(pModelCreate);
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// 読み込むモデルのファイル名だった
			LoadModel(pStr, nCntModel);
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_FILENAME) == 0)
		{// 読み込むエフェクトのファイル名だった
			LoadEffectFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TITLELOGOSET) == 0)
		{// タイトルロゴ情報だった
			LoadTitleLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, PRESSSET) == 0)
		{// プレス待機情報だった
			LoadPress(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TEAMLOGOSET) == 0)
		{// チームロゴ情報だった
			LoadTeamLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEMODEBGSET) == 0)
		{// ゲームモード用背景情報だった
			LoadGameModebg(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEMODELOGOSET) == 0)
		{// ゲームモード選択用ポリゴン情報だった
			LoadGameModeLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_PLAYERDATA) == 0)
		{// 読み込むプレイヤーデータの総数だった
			m_nNumPlayerData = CFunctionLib::ReadInt(pStr, NUM_PLAYERDATA);
			CreatePlayerManagerPointer();
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERDATA_FILENAME) == 0)
		{// 読み込むプレイヤーデータのファイル名だった
			LoadPlayerDataFileName(pStr, nCntPlayerData);
			nCntPlayerData++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_PLAYER) == 0)
		{// 読み込むプレイヤー情報の総数だった
			m_nNumPlayer = CFunctionLib::ReadInt(pStr, NUM_PLAYER);
			CreatePlayerDataPointer();
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERSET) == 0)
		{// 読み込むプレイヤー情報だった
			LoadPlayerData(pFileLoader, pStr, nCntPlayer);
			nCntPlayer++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_LIGHT) == 0)
		{// 読み込むプレイヤー情報の総数だった
			m_nNumLight = CFunctionLib::ReadInt(pStr, NUM_LIGHT);
			CreateLightDataPointer();
		}
		else if (CFunctionLib::Memcmp(pStr, LIGHTSET) == 0)
		{// 読み込むライト情報だった
			LoadLight(pFileLoader, pStr, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, FIELDSET) == 0)
		{// 読み込む地面情報だった
			LoadField(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, SKYSET) == 0)
		{// 読み込む空情報だった
			LoadSky(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルの読み込むテクスチャファイル名を読み込む
//=============================================================================
void CTitle::LoadTexFileName(char *pStr, int nCntTex)
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
// タイトルのモデルを読み込む
//=============================================================================
void CTitle::LoadModel(char *pStr, int nCntModel)
{
	// モデルのファイルパス名を読み取る
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat = 0;
	char aModelFileName[256] = "\0";
	strcpy(aModelFileName, CFunctionLib::ReadString(pStr, aModelFileName, MODEL_FILENAME));

	// xファイルの読み込み
	D3DXLoadMeshFromX(aModelFileName, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// モデル管轄クラスに値を設定する
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL)return;
	pModelCreate->SetMesh(pMesh, nCntModel);
	pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);
	pModelCreate->SetFileName(pStr, nCntModel);
}

//=============================================================================
// タイトルの読み込むエフェクトファイル名を読み込む
//=============================================================================
void CTitle::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// エフェクト管轄クラスを生成
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
}

//=============================================================================
// タイトルのタイトルロゴポリゴンの値を読み込む
//=============================================================================
void CTitle::LoadTitleLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nLogoTexIdx = 0;
	D3DXVECTOR3 LogoPos = TITLELOGO_POS_INI;
	D3DXCOLOR LogoCol = TITLELOGO_COL_INI;
	float fLogoWidth = TITLELOGO_WIDTH_INI;
	float fLogoHeight = TITLELOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			LogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			LogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_TITLELOGOSET) == 0)
		{// タイトルロゴ情報終了の合図だった
			break;
		}
	}

	// ポリゴン生成
	m_pTitleLogo = CTitleLogo::Create(LogoPos, LogoCol, fLogoWidth, fLogoHeight, TITLELOGO_PRIORITY);
	if (m_pTitleLogo != NULL && GetTextureManager() != NULL)
	{
		m_pTitleLogo->BindTexture(GetTextureManager()->GetTexture(nLogoTexIdx));
	}
}

//=============================================================================
// タイトルのプレス待機ポリゴンの値を読み込む
//=============================================================================
void CTitle::LoadPress(CFileLoader *pFileLoader, char *pStr)
{
	m_PressData.nTexIdx = 0;
	m_PressData.pos = TITLEPRESS_POS_INI;
	m_PressData.col = TITLEPRESS_COL_INI;
	m_PressData.fWidth = TITLEPRESS_WIDTH_INI;
	m_PressData.fHeight = TITLEPRESS_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_PressData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_PressData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_PressData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_PressData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_PressData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PRESSSET) == 0)
		{// プレス待機情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルのチームロゴ用ポリゴンの値を読み込む
//=============================================================================
void CTitle::LoadTeamLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nTeamLogoTexIdx = 0;
	D3DXVECTOR3 TeamLogoPos = TITLETEAMLOGO_POS_INI;
	D3DXCOLOR TeamLogoCol = TITLETEAMLOGO_COL_INI;
	float fTeamLogoWidth = TITLETEAMLOGO_WIDTH_INI;
	float fTeamLogoHeight = TITLETEAMLOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			nTeamLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			TeamLogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			TeamLogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			fTeamLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			fTeamLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_TEAMLOGOSET) == 0)
		{// チームロゴ情報終了の合図だった
			break;
		}
	}

	// チームロゴ用背景ポリゴンの生成
	CreateTeamBgPolygon();

	// ポリゴン生成
	m_pTeamLogo = CScene2D::Create(TeamLogoPos, TeamLogoCol, fTeamLogoWidth, fTeamLogoHeight, TITLETEAMLOGO_PRIORITY);
	if (m_pTeamLogo != NULL && GetTextureManager() != NULL)
	{
		m_pTeamLogo->BindTexture(GetTextureManager()->GetTexture(nTeamLogoTexIdx));
	}
}

//=============================================================================
// タイトルのゲームモード背景用ポリゴンの値を読み込む
//=============================================================================
void CTitle::LoadGameModebg(CFileLoader *pFileLoader, char *pStr)
{
	m_GameModeBgData.nTexIdx = 0;
	m_GameModeBgData.pos = TITLEGAMEMODEBG_POS_INI;
	m_GameModeBgData.col = TITLEGAMEMODEBG_COL_INI;
	m_GameModeBgData.fWidth = TITLEGAMEMODEBG_WIDTH_INI;
	m_GameModeBgData.fHeight = TITLEGAMEMODEBG_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_GameModeBgData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_GameModeBgData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_GameModeBgData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_GameModeBgData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_GameModeBgData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_GAMEMODEBGSET) == 0)
		{// ゲームモード背景情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルのゲームモード選択用ポリゴンの値を読み込む
//=============================================================================
void CTitle::LoadGameModeLogo(CFileLoader *pFileLoader, char *pStr)
{
	m_GameModeLogoData.nTexIdx = 0;
	m_GameModeLogoData.pos = TITLEGAMEMODELOGO_POS_INI;
	m_GameModeLogoData.fInterval = TITLEGAMEMODELOGO_INTERVAL_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_GameModeLogoData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_GameModeLogoData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, INTERVAL) == 0)
		{// ポリゴンとポリゴンの間隔だった
			m_GameModeLogoData.fInterval = CFunctionLib::ReadFloat(pStr, INTERVAL);
		}
		else if (CFunctionLib::Memcmp(pStr, NONE) == 0)
		{// 選択されていないポリゴンのデータだった
			LoadGameModeLogo_PolyNone(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, SELECT) == 0)
		{// 選択されているポリゴンのデータだった
			LoadGameModeLogo_PolySelect(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_GAMEMODELOGOSET) == 0)
		{// ゲームモード選択用ポリゴン情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルのゲームモード選択用ポリゴンの値を読み込む(選択されてない時)
//=============================================================================
void CTitle::LoadGameModeLogo_PolyNone(CFileLoader *pFileLoader, char *pStr)
{
	m_GameModeLogoPoly_NoneData.col = TITLEGAMEMODELOGO_COL_INI;
	m_GameModeLogoPoly_NoneData.fWidth = TITLEGAMEMODELOGO_WIDTH_INI;
	m_GameModeLogoPoly_NoneData.fHeight = TITLEGAMEMODELOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_GameModeLogoPoly_NoneData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_GameModeLogoPoly_NoneData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_GameModeLogoPoly_NoneData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_NONE) == 0)
		{// 選択されていないゲームモード選択用ポリゴン情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルのゲームモード選択用ポリゴンの値を読み込む(選択されている時)
//=============================================================================
void CTitle::LoadGameModeLogo_PolySelect(CFileLoader *pFileLoader, char *pStr)
{
	m_GameModeLogoPoly_SelectData.col = TITLEGAMEMODELOGO_COL_INI;
	m_GameModeLogoPoly_SelectData.fWidth = TITLEGAMEMODELOGO_WIDTH_INI;
	m_GameModeLogoPoly_SelectData.fHeight = TITLEGAMEMODELOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_GameModeLogoPoly_SelectData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_GameModeLogoPoly_SelectData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_GameModeLogoPoly_SelectData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SELECT) == 0)
		{// 選択されているゲームモード選択用ポリゴン情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルの読み込むプレイヤーデータのファイル名を読み込む
//=============================================================================
void CTitle::LoadPlayerDataFileName(char *pStr, int nCntPlayerData)
{
	strcpy(m_aPlayerDataFileName[nCntPlayerData], CFunctionLib::ReadString(pStr, m_aPlayerDataFileName[nCntPlayerData], PLAYERDATA_FILENAME));
}

//=============================================================================
// タイトルの読み込むプレイヤーデータのファイル名を読み込む
//=============================================================================
void CTitle::LoadPlayerData(CFileLoader *pFileLoader, char *pStr, int nCntPlayer)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// 使用するテクスチャの番号だった
			m_pPlayerData[nCntPlayer].nType = CFunctionLib::ReadInt(pStr, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// 位置だった
			m_pPlayerData[nCntPlayer].pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// 向きだった
			m_pPlayerData[nCntPlayer].rot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PLAYERSET) == 0)
		{// プレイヤー情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルのライト情報を読み込む
//=============================================================================
void CTitle::LoadLight(CFileLoader *pFileLoader, char *pStr, int nCntLight)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, DIR) == 0)
		{// 向きだった
			m_pLightData[nCntLight].Dir = CFunctionLib::ReadVector3(pStr, DIR);
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// 拡散光だった
			m_pLightData[nCntLight].Diffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// 環境光だった
			m_pLightData[nCntLight].Ambient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// 反射光だった
			m_pLightData[nCntLight].Specular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, END_LIGHTSET) == 0)
		{// ライト情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルの地面情報を読み込む
//=============================================================================
void CTitle::LoadField(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_FieldData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// 座標だった
			m_FieldData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// 向きだった
			m_FieldData.rot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// 色だった
			m_FieldData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// 1マス分の幅だった
			m_FieldData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, DEPTH) == 0)
		{// 1マス分の奥行だった
			m_FieldData.fDepth = CFunctionLib::ReadFloat(pStr, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// 横の分割数だった
			m_FieldData.nXBlock = CFunctionLib::ReadInt(pStr, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
		{// 奥行の分割数だった
			m_FieldData.nZBlock = CFunctionLib::ReadInt(pStr, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXU_SPLIT) == 0)
		{// テクスチャ座標の横の分割数だった
			m_FieldData.nTexUSplit = CFunctionLib::ReadInt(pStr, TEXU_SPLIT);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXV_SPLIT) == 0)
		{// テクスチャ座標の縦の分割数だった
			m_FieldData.nTexVSplit = CFunctionLib::ReadInt(pStr, TEXV_SPLIT);
		}
		else if (CFunctionLib::Memcmp(pStr, CULLING) == 0)
		{// カリングするかしないかだった
			m_FieldData.bCulling = CFunctionLib::ReadBool(pStr, CULLING);
		}
		else if (CFunctionLib::Memcmp(pStr, ADDTEX) == 0)
		{// どれくらいテクスチャ座標を進めるかだった
			m_FieldData.fAddTex = CFunctionLib::ReadFloat(pStr, ADDTEX);
		}
		else if (CFunctionLib::Memcmp(pStr, END_FIELDSET) == 0)
		{// 地面情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルの空情報を読み込む
//=============================================================================
void CTitle::LoadSky(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_SkyData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// 座標だった
			m_SkyData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// 向きだった
			m_SkyData.rot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// 色だった
			m_SkyData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, RADIUS) == 0)
		{// 半径だった
			m_SkyData.fRadius = CFunctionLib::ReadFloat(pStr, RADIUS);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// 横の分割数だった
			m_SkyData.nXBlock = CFunctionLib::ReadInt(pStr, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
		{// 奥行の分割数だった
			m_SkyData.nZBlock = CFunctionLib::ReadInt(pStr, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ROLLSPEED) == 0)
		{// 回転させるスピードだった
			m_SkyData.fRollSpeed = CFunctionLib::ReadFloat(pStr, ROLLSPEED);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKYSET) == 0)
		{// 空情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// タイトルの変数を初期化する
//=============================================================================
void CTitle::ClearVariable(void)
{
	m_State = STATE_TEAMLOGO;
	m_pFade = NULL;
	m_pTeamLogo = NULL;
	m_pTeamBg = NULL;
	m_pTitleLogo = NULL;
	m_nStateCounter = 0;
	ClearPress();
	ClearGameModeBg();
	ClearGameModeLogo();
	ClearGameModeLogo_PolyNone();
	ClearGameModeLogo_PolySelect();
	ClearPlayer();
	ClearHighScore();
}

//=============================================================================
// タイトルのプレス待機用変数を初期化する
//=============================================================================
void CTitle::ClearPress(void)
{
	m_PressData.nTexIdx = 0;
	m_PressData.pos = INITIALIZE_D3DXVECTOR3;
	m_PressData.col = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	m_PressData.fWidth = 0.0f;
	m_PressData.fHeight = 0.0f;
	m_pPress = NULL;
}

//=============================================================================
// タイトルのゲームモード背景用変数を初期化する
//=============================================================================
void CTitle::ClearGameModeBg(void)
{
	m_GameModeBgData.nTexIdx = 0;
	m_GameModeBgData.pos = INITIALIZE_D3DXVECTOR3;
	m_GameModeBgData.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_GameModeBgData.fWidth = 0.0f;
	m_GameModeBgData.fHeight = 0.0f;
	m_pGameLogoBg = NULL;
	m_pDecide = NULL;
	m_pRetrun = NULL;
	m_pIcon = NULL;
}

//=============================================================================
// タイトルのゲームモードロゴ用変数を初期化する
//=============================================================================
void CTitle::ClearGameModeLogo(void)
{
	m_GameModeLogoData.nTexIdx = 0;
	m_GameModeLogoData.pos = INITIALIZE_D3DXVECTOR3;
	m_GameModeLogoData.fInterval = 0.0f;
	for (int nCntLogo = 0; nCntLogo < GAMEMODE_MAX; nCntLogo++)
	{
		m_apGameModeLogo[nCntLogo] = NULL;
	}
}

//=============================================================================
// タイトルの選択されていないゲームモードロゴポリゴン用変数を初期化する
//=============================================================================
void CTitle::ClearGameModeLogo_PolyNone(void)
{
	m_GameModeLogoPoly_NoneData.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_GameModeLogoPoly_NoneData.fWidth = 0.0f;
	m_GameModeLogoPoly_NoneData.fHeight = 0.0f;
}

//=============================================================================
// タイトルの選択されているゲームモードロゴポリゴン用変数を初期化する
//=============================================================================
void CTitle::ClearGameModeLogo_PolySelect(void)
{
	m_GameModeLogoPoly_SelectData.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_GameModeLogoPoly_SelectData.fWidth = 0.0f;
	m_GameModeLogoPoly_SelectData.fHeight = 0.0f;
}

//=============================================================================
// タイトルの空用変数を初期化する
//=============================================================================
void CTitle::ClearSky(void)
{
	m_SkyData.nTexIdx = 0;
	m_SkyData.pos = INITIALIZE_D3DXVECTOR3;
	m_SkyData.rot = INITIALIZE_D3DXVECTOR3;
	m_SkyData.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_SkyData.nXBlock = 0;
	m_SkyData.nZBlock = 0;
	m_SkyData.fRadius = 0.0f;
	m_SkyData.fRollSpeed = 0.0f;
	m_pSky = NULL;
}

//=============================================================================
// タイトルの地面用変数を初期化する
//=============================================================================
void CTitle::ClearField(void)
{
	m_FieldData.nTexIdx = 0;
	m_FieldData.pos = INITIALIZE_D3DXVECTOR3;
	m_FieldData.rot = INITIALIZE_D3DXVECTOR3;
	m_FieldData.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_FieldData.fWidth = 0.0f;
	m_FieldData.fDepth = 0.0f;
	m_FieldData.nXBlock = 0;
	m_FieldData.nZBlock = 0;
	m_FieldData.nTexUSplit = 0;
	m_FieldData.nTexVSplit = 0;
	m_FieldData.bCulling = false;
	m_FieldData.fAddTex = 0.0f;
	m_pMeshField = NULL;
}

//=============================================================================
// タイトルのライト用変数を初期化する
//=============================================================================
void CTitle::ClearLight(void)
{
	m_nNumLight = 0;
	m_pLightData = NULL;
	m_pLightManager = NULL;
}

//=============================================================================
// タイトルのプレイヤー用変数を初期化する
//=============================================================================
void CTitle::ClearPlayer(void)
{
	m_nNumPlayerData = 0;
	m_nNumPlayer = 0;
	m_pPlayerManager = NULL;
	m_pPlayerData = NULL;
	m_pPlayer = NULL;
	m_aPlayerDataFileName = NULL;
}

//=============================================================================
// タイトルのハイスコア用変数を初期化する
//=============================================================================
void CTitle::ClearHighScore(void)
{
	m_pHighScore = NULL;
	m_pHighScoreLogo = NULL;
}

//=============================================================================
// タイトルの状態を設定する
//=============================================================================
void CTitle::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
// タイトルの状態を取得する
//=============================================================================
CTitle::STATE CTitle::GetState(void)
{
	return m_State;
}

//=============================================================================
// タイトルのゲームモードを取得する
//=============================================================================
int CTitle::GetGameMode(void)
{
	return m_nGameMode;
}

//=============================================================================
// タイトルのハイスコアを設定する
//=============================================================================
void CTitle::SetHighScore(const int nHighScore)
{
	m_nHighScore = nHighScore;
}

//=============================================================================
// タイトルのハイスコアを取得する
//=============================================================================
int CTitle::GetHighScore(void)
{
	return m_nHighScore;
}



//*****************************************************************************
// CTitleLogoの処理
//*****************************************************************************
//=============================================================================
// タイトルロゴのコンストラクタ
//=============================================================================
CTitleLogo::CTitleLogo(int nPriority, OBJTYPE objtype) : CScene3D(nPriority, objtype)
{

}

//=============================================================================
// タイトルロゴのデストラクタ
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
//タイトルロゴの生成
//=============================================================================
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CTitleLogo *pTitleLogo = NULL;	// CTitleLogoクラス型のポインタ
	if (pTitleLogo == NULL)
	{
		pTitleLogo = new CTitleLogo(nPriority);
		if (pTitleLogo != NULL)
		{
			// 値の設定
			pTitleLogo->SetPos(INITIALIZE_D3DXVECTOR3);
			pTitleLogo->SetPolypos(pos);
			pTitleLogo->SetCol(col);
			pTitleLogo->SetWidth(fWidth);
			pTitleLogo->SetHeight(fHeight);

			if (FAILED(pTitleLogo->Init()))
			{// 初期化に失敗
				return NULL;
			}
		}
	}
	return pTitleLogo;
}

//=============================================================================
// タイトルロゴの初期化処理
//=============================================================================
HRESULT CTitleLogo::Init(void)
{
	// 共通の初期化処理
	if (FAILED(CScene3D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// タイトルロゴの終了処理
//=============================================================================
void CTitleLogo::Uninit(void)
{
	// 共通の終了処理
	CScene3D::Uninit();
}

//=============================================================================
// タイトルロゴの更新処理
//=============================================================================
void CTitleLogo::Update(void)
{

}

//=============================================================================
// タイトルロゴの描画処理
//=============================================================================
void CTitleLogo::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Lighting;
			D3DXMATRIX mtxProjOrg;
			D3DXMATRIX mtxProj;

			// 現在の情報を保存する
			pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjOrg);

			// プロジェクションマトリックスの初期化
			D3DXMatrixIdentity(&mtxProj);

			// プロジェクションマトリックスを作成
			D3DXMatrixOrthoLH(&mtxProj,
				(float)SCREEN_WIDTH,
				(float)SCREEN_HEIGHT,
				1.0f,
				3000.0f);

			// プロジェクションマトリックスの設定
			pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);

			// ライティングの設定を外す
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// Zテストをしない設定に
			pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		    // 共通の描画処理
			CScene3D::Draw();

			// Zテストの設定をデフォルトに戻す
			pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

			// ライティングの設定を戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// 描画領域の設定を戻す
			pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjOrg);
		}
	}
}

//=============================================================================
// タイトルロゴの頂点バッファ生成処理
//=============================================================================
void CTitleLogo::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// オフセットの長さを求める
	float fWidth = GetWidth();
	float fHeight = GetHeight();
	float fLength = sqrtf((fWidth * fWidth) + (fHeight * fHeight));
	SetLength(fLength);

	// オフセットの角度を求める
	float fAngle = atan2f(fWidth, fHeight);
	SetAngle(fAngle);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, cosf(fAngle) * fLength, 2.0f) + m_Polypos;
	pVtx[1].pos = D3DXVECTOR3(sinf(fAngle) * fLength, cosf(fAngle) * fLength, 2.0f) + m_Polypos;
	pVtx[2].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, -cosf(fAngle) * fLength, 2.0f) + m_Polypos;
	pVtx[3].pos = D3DXVECTOR3(sinf(fAngle) * fLength, -cosf(fAngle) * fLength, 2.0f) + m_Polypos;

	// 頂点カラー
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファの設定
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
// タイトルロゴのポリゴン座標設定理
//=============================================================================
void CTitleLogo::SetPolypos(const D3DXVECTOR3 Polypos)
{
	m_Polypos = Polypos;
}

//=============================================================================
// タイトルロゴのポリゴン座標取得処理
//=============================================================================
D3DXVECTOR3 CTitleLogo::GetPolypos(void)
{
	return m_Polypos;
}