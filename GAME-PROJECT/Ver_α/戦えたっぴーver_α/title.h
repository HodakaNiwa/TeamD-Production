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
#include "scene2D.h"

//前方宣言
class CFileLoader;
class CFileSaver;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TITLEBG_NUM (2)    // タイトル用背景の数

//*****************************************************************************
// タイトル用ロゴクラスの定義
//*****************************************************************************
class CTitleLogo : public CScene2D
{
public:    // 誰でもアクセス可能
	CTitleLogo(int nPriority = 3, OBJTYPE objtype = OBJTYPE_SCENE2D);
	~CTitleLogo();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CTitleLogo *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

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
		STATE_MAX
	}STATE;

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

protected: // このクラスと派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
	void ClearVariable(void);
	void ClearPress(void);

	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateFadePolygon(void);
	void CreateTeamBgPolygon(void);
	void CreatePress(void);

	void ReleaseFadePolygon(void);
	void ReleaseTeamLogoPolygon(void);
	void ReleaseTeamBgPolygon(void);
	void ReleaseBgPolygon(void);
	void ReleaseTitleLogoPolygon(void);
	void ReleasePress(void);

	void SlideBgUV(void);
	void TeamLogoUpdate(void);
	void FadeOutUpdate(void);
	void FadeInUpdate(void);
	void NormalUpdate(void);
	void WaitInputToNormal(void);
	void WaitInputToNextMode(void);
	void SetNextMode_None(void);
	void SetNextMode_Push(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadBgPolygon(CFileLoader *pFileLoader, char *pStr);
	void LoadTitleLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadPress(CFileLoader *pFileLoader, char *pStr);
	void LoadTeamLogo(CFileLoader *pFileLoader, char *pStr);

	STATE m_State;                     // 状態
	CScene2D *m_pFade;                 // フェードに使用するポリゴン
	CScene2D *m_pTeamLogo;             // チームロゴ用ポリゴンクラス型のポインタ
	CScene2D *m_pTeamBg;               // チームロゴ背景用ポリゴンクラス型のポインタ
	CScene2D *m_pBg;                   // 背景用ポリゴンクラス型のポインタ
	CTitleLogo *m_pTitleLogo;          // タイトルロゴクラス型のポインタ
	CScene2DPress *m_pPress;           // プレス待機ポリゴンクラス型のポインタ
	int m_nPressTexIdx;                // プレス待機ポリゴンの使用するテクスチャ番号
	D3DXVECTOR3 m_PressPos;            // プレス待機ポリゴンの座標
	D3DXCOLOR m_PressCol;              // プレス待機ポリゴンの色
	float m_fPressWidth;               // プレス待機ポリゴンの幅
	float m_fPressHeight;              // プレス待機ポリゴンの高さ
	int m_nStateCounter;               // 状態を管理するカウンター
};
#endif