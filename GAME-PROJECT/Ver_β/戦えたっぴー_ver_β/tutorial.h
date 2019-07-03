//=============================================================================
//
// チュートリアルの処理 [tutorial.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "basemode.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CFileLoader;
class CFileSaver;
class CScene2D;
class CScene2DFlash;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTutorial : public CBasemode
{
public:	// 誰からもアクセス可能
	//--------------
	// スキップ機能
	//--------------
	typedef enum
	{
		SKIP_NONE = -1,
		SKIP_YES,
		SKIP_NO,
		SKIP_MAX
	}SKIP;

	//--------------
	// 状態
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_SKIP_CHECK,
		STATE_NORMAL,
		STATE_GOAL,
		STATE_MAP_CHANGE,
		STATE_END,
		STATE_MAX
	}STATE;

	// メンバ関数
	CTutorial();
	~CTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	STATE GetState(void);

	// 静的メンバ関数
	static CTutorial *Create(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
	void ClearVariable(void);

	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateMapFilePointer(void);

	void ReleaseMapFilePointer(void);
	void ReleaseSkipLogo(void);
	void ReleaseSkipCheckLogo(void);

	void SkipCheckUpdate(void);
	void NormalUpdate(void);
	void GoalUpdate(void);
	void MapChangeUpdate(void);
	void EndUpdate(void);
	void ChangeNoneSkipCheckPolygon(int nSelect);
	void ChangeSelectSkipCheckPolygon(int nSelect);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadMapFileName(char *pStr, int nCntMap);
	void LoadSkipLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadSkipCheckLogo(CFileLoader *pFileLoader, char *pStr, int nCntSkipCheck);

	STATE m_State;                              // 状態
	int m_nStateCounter;                        // 状態を管理するカウンター
	int m_nNumMap;                              // マップを読み込む数
	char **m_pMapFileName;                      // 読み込むマップのファイル名
	CScene2D *m_pSkipLogo;                      // スキップするかを表示するロゴポリゴン
	CScene2DFlash *m_pSkipCheckLogo[SKIP_MAX];  // スキップ確認時に はい いいえ を表示する用のロゴポリゴン
	int m_nSelectSkip;                          // スキップするかどうか
};
#endif