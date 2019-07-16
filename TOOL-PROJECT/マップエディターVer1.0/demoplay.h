//=============================================================================
//
// デモプレイの処理 [demoplay.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _DEMOPLAY_H_
#define _DEMOPLAY_H_

#include "main.h"
#include "basemode.h"

//前方宣言
class CFileLoader;
class CFileSaver;
class CUI;
class CMap;
class CEnemy_ListData;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDemoplay : public CBasemode
{
public:	//誰からもアクセス可能
	//メンバ関数
	CDemoplay();
	~CDemoplay();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CDemoplay *Create(void);

private:	//自分だけがアクセス可能
	void ClearVariable(void);

	void CreateCamera(void);
	void CreateMap(void);
	void CreateUI(void);

	void CheckEnemySpawn(int nTime);
	void EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void CreateTextureManager(int nNumTex);
	void LoadTexture(char *pFileName, int nCntTex);
	void LoadEffectFileName(char *pStr);
	void LoadMapFileName(char *pStr);
	void SaveSystem(void);
	void SaveMapFileName(CFileSaver *pFileSaver);

	char m_aMapFileName[256];             // 読み込むマップのファイル名
	CUI *m_pUI;                           // UIクラスへのポインタ
	int m_nGameCounter;                   // ゲームの時間を数えるカウンター
	int m_nSpawnEnemyCount;               // スポーンした敵の数を数えるカウンター
};
#endif