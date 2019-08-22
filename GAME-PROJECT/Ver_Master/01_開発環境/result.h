//=============================================================================
//
// リザルトの処理 [result.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "basemode.h"

//前方宣言
class CFileLoader;
class CFileSaver;
class CScene2D;
class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CResult : public CBasemode
{
public:	// 誰からもアクセス可能
	//--------------
	// 状態
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_WAIT,
		STATE_NORMAL,
		STATE_HIGHSCORE,
		STATE_END,
		STATE_MAX
	}STATE;

	// メンバ関数
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	STATE GetState(void);

	// 静的メンバ関数
	static CResult *Create(void);
	static void SetHighScore(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
	void ClearVariable(void);

	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateLogo(void);
	void CreateHighScore(void);

	void ReleaseLogo(void);
	void ReleaseHighScore(void);

	void WaitUpdate(void);
	void NormalUpdate(void);
	void HighScoreUpdate(void);
	void EndUpdate(void);
	void WaitInputToHighScore(void);
	void WaitInputToNextMode(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadLogo(CFileLoader *pFileLoader, char *pStr);

	STATE m_State;        // 状態
	int m_nStateCounter;  // 状態を管理するカウンター
	CScene2D *m_pLogo;    // リザルトロゴ表示用ポリゴン

	// ハイスコア表示用データ
	static bool m_bHighScore;
	CScene2D *m_pHighScoreLogo;
	CNumber *m_pHighScore;
	D3DXCOLOR m_FalshCol;

	// 各種クラス生成用情報
	typedef struct
	{// リザルトロゴデータ
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}RESULTLOGO_DATA;
	RESULTLOGO_DATA m_ResultLogoData;
};
#endif