//=============================================================================
//
// タイトルの処理 [title.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "basemode.h"
#include "scene3D.h"

//前方宣言
class CFileLoader;
class CFileSaver;
class CScene2D;
class CNumber;
class CScene2DFlash;
class CScene2DPress;
class CSky;
class CMeshField;
class CLightManager;
class CPlayer;
class CPlayerManager;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TITLEBG_NUM (2)    // タイトル用背景の数

//*****************************************************************************
// タイトル用ロゴクラスの定義
//*****************************************************************************
class CTitleLogo : public CScene3D
{
public:    // 誰でもアクセス可能
	CTitleLogo(int nPriority = 3, OBJTYPE objtype = OBJTYPE_SCENE3D);
	~CTitleLogo();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetPolypos(const D3DXVECTOR3 Polypos);
	D3DXVECTOR3 GetPolypos(void);

	// 静的メンバ関数
	static CTitleLogo *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_Polypos;
};

//*****************************************************************************
// タイトルクラスの定義
//*****************************************************************************
class CTitle : public CBasemode
{
public:	// 誰からもアクセス可能
	//--------------
	// 状態
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_TEAMLOGO,
		STATE_FADEOUT,
		STATE_FADEIN,
		STATE_NORMAL,
		STATE_GAMEMODE,
		STATE_MAX
	}STATE;

	//--------------
	// 状態
	//--------------
	typedef enum
	{
		GAMEMODE_LOCAL1P = 0,
		GAMEMODE_LOCAL2P,
		GAMEMODE_ONLINE2P,
		GAMEMODE_MAX
	}GAMEMODE;

	// メンバ関数
	CTitle();
	~CTitle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	STATE GetState(void);

	// 静的メンバ関数
	static CTitle *Create(void);
	static int GetGameMode(void);

	static void SetHighScore(const int nHighScore);
	static int GetHighScore(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
	void ClearVariable(void);
	void ClearPress(void);
	void ClearGameModeBg(void);
	void ClearGameModeLogo(void);
	void ClearGameModeLogo_PolyNone(void);
	void ClearGameModeLogo_PolySelect(void);
	void ClearSky(void);
	void ClearField(void);
	void ClearLight(void);
	void ClearPlayer(void);
	void ClearHighScore(void);

	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateFadePolygon(void);
	void CreateTeamBgPolygon(void);
	void CreatePress(void);
	void CreateGameModeBg(void);
	void CreateGameModeLogo(void);
	void CreateSky(void);
	void CreateField(void);
	void CreateLight(void);
	void CreatePlayer(void);
	void CreatePlayerManagerPointer(void);
	void CreatePlayerManager(void);
	void CreatePlayerDataPointer(void);
	void CreateLightDataPointer(void);
	void CreateHighScore(void);

	void ReleaseFadePolygon(void);
	void ReleaseTeamLogoPolygon(void);
	void ReleaseTeamBgPolygon(void);
	void ReleaseTitleLogoPolygon(void);
	void ReleasePress(void);
	void ReleaseGameModeBg(void);
	void ReleaseGameModeLogo(void);
	void ReleaseSky(void);
	void ReleaseField(void);
	void ReleaseLight(void);
	void ReleasePlayer(void);
	void ReleasePlayerManagerPointer(void);
	void ReleasePlayerManager(void);
	void ReleasePlayerDataPointer(void);
	void ReleaseLightDataPointer(void);
	void ReleaseHighScore(void);

	void TeamLogoUpdate(void);
	void FadeOutUpdate(void);
	void FadeInUpdate(void);
	void NormalUpdate(void);
	void GameModeUpdate(void);
	void WaitInputToNormal(void);
	void WaitInputToGameMode(void);
	void SetNextMode_None(void);
	void SetNextMode_Push(void);
	void ChangeNoneGameModeLogoPoly(int nSelect);
	void ChangeSelectGameModeLogoPoly(int nSelect);
	void AddFieldTexUV(void);
	void CreateDashEffect(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadEffectFileName(char *pStr);
	void LoadTitleLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadPress(CFileLoader *pFileLoader, char *pStr);
	void LoadTeamLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadGameModebg(CFileLoader *pFileLoader, char *pStr);
	void LoadGameModeLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadGameModeLogo_PolyNone(CFileLoader *pFileLoader, char *pStr);
	void LoadGameModeLogo_PolySelect(CFileLoader *pFileLoader, char *pStr);
	void LoadPlayerDataFileName(char *pStr, int nCntPlayerData);
	void LoadPlayerData(CFileLoader *pFileLoader, char *pStr, int nCntPlayer);
	void LoadLight(CFileLoader *pFileLoader, char *pStr, int nCntLight);
	void LoadField(CFileLoader *pFileLoader, char *pStr);
	void LoadSky(CFileLoader *pFileLoader, char *pStr);

	static int m_nGameMode;            // ゲームモード(各種プログラムで更新する)
	static int m_nHighScore;           // ゲームのハイスコア(更新はCGameクラスが行う)
	STATE m_State;                     // 状態
	CScene2D *m_pFade;                 // フェードに使用するポリゴン
	CScene2D *m_pTeamLogo;             // チームロゴ用ポリゴンクラス型のポインタ
	CScene2D *m_pTeamBg;               // チームロゴ背景用ポリゴンクラス型のポインタ
	CScene2D *m_pDecide;               // 決定ロゴ用ポリゴンクラス型のポインタ
	CScene2D *m_pRetrun;               // もどるロゴ用ポリゴンクラス型のポインタ
	CTitleLogo *m_pTitleLogo;          // タイトルロゴクラス型のポインタ
	int m_nStateCounter;               // 状態を管理するカウンター

	// ハイスコア用データ
	CScene2D *m_pHighScoreLogo;
	CNumber *m_pHighScore;

	// プレスポリゴン用データ
	typedef struct
	{
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}PRESS_DATA;
	PRESS_DATA m_PressData;
	CScene2DPress *m_pPress;

	// ゲームモード背景ポリゴン用データ
	typedef struct
	{
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}GAMEMODEBG_DATA;
	GAMEMODEBG_DATA m_GameModeBgData;
	CScene2D *m_pGameLogoBg;

	// ゲームモードロゴポリゴン用データ
	typedef struct
	{
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}GAMEMODELOGO_POLYDATA;
	typedef struct
	{
		D3DXVECTOR3 pos;
		int nTexIdx;
		float fInterval;
	}GAMEMODELOGO_DATA;
	GAMEMODELOGO_DATA m_GameModeLogoData;
	GAMEMODELOGO_POLYDATA m_GameModeLogoPoly_NoneData;
	GAMEMODELOGO_POLYDATA m_GameModeLogoPoly_SelectData;
	CScene2DFlash *m_apGameModeLogo[GAMEMODE_MAX];

	// 空用データ
	typedef struct
	{
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXCOLOR col;
		float fRadius;
		int nXBlock;
		int nZBlock;
		float fRollSpeed;
	}SKYDATA;
	SKYDATA m_SkyData;
	CSky *m_pSky;

	// 地面用データ
	typedef struct
	{
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXCOLOR col;
		float fWidth;
		float fDepth;
		int nXBlock;
		int nZBlock;
		int nTexUSplit;
		int nTexVSplit;
		bool bCulling;
		float fAddTex;
	}FIELDDATA;
	FIELDDATA m_FieldData;
	CMeshField *m_pMeshField;

	// ライト用データ
	typedef struct
	{
		D3DXVECTOR3 Dir;
		D3DXCOLOR Diffuse;
		D3DXCOLOR Ambient;
		D3DXCOLOR Specular;
	}LIGHTDATA;
	int m_nNumLight;
	LIGHTDATA *m_pLightData;
	CLightManager *m_pLightManager;

	// プレイヤー用データ
	typedef struct
	{
		int nType;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	}PLAYERDATA;
	int m_nNumPlayerData;
	char **m_aPlayerDataFileName;
	CPlayerManager	**m_pPlayerManager;
	int m_nNumPlayer;
	PLAYERDATA *m_pPlayerData;
	CPlayer **m_pPlayer;
};
#endif