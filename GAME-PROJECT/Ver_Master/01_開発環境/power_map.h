//=============================================================================
//
// 勢力図の処理 [power_map.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _POWER_MAP_H_
#define _POWER_MAP_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;
class CTextureManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPowerMap : public CScene
{
public:    // 誰でもアクセス可能
	//メンバ関数
	CPowerMap(int nPriority = 3, OBJTYPE objtype = OBJTYPE_POWERMAP);
	~CPowerMap();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddGauge(void);
	void ResetGauge(void);

	void SetPlayerIcon(CScene2D *pPlayerIcon);
	void SetEnemyIcon(CScene2D *pEnemyIcon);
	void SetStraw(CScene2D *pStraw);
	void SetStrawContents(CScene2D *pStrawContents);
	void SetTextureManager(CTextureManager *pTextureManager);

	CScene2D *GetPlayerIcon(void);
	CScene2D *GetEnemyIcon(void);
	CScene2D *GetStraw(void);
	CScene2D *GetStrawContents(void);
	CTextureManager *GetTextureManager(void);

	// 静的メンバ関数
	static CPowerMap *Create(CTextureManager *pTextureManager, int nPriority = 3);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void ClearVariable(void);

	void CreatePlayerIcon(void);
	void CreateEnemyIcon(void);
	void CreateStraw(void);
	void CreateStrawContents(void);

	void ReleasePlayerIcon(void);
	void ReleaseEnemyIcon(void);
	void ReleaseStraw(void);
	void ReleaseStrawContents(void);

	CScene2D        *m_pPlayerIcon;
	CScene2D        *m_pEnemyIcon;
	CScene2D        *m_pStraw;
	CScene2D        *m_pStrawContents;
	CTextureManager *m_pTextureManager;
	float           m_fPlayerIconDestPosX;
};

#endif