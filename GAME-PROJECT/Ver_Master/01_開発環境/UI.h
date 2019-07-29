//*****************************************************************************
//
//     UIの処理[UI.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CTextureManager;
class CScene2D;
class CNumber;

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define UI_PRIORITY     (5)       // UIの処理優先順位
#define UI_BG_WIDTH_INI (90.0f)   // UIの背景幅

//*****************************************************************************
//    UIクラスの定義
//*****************************************************************************
class CUI
{
public:    // 誰でもアクセス可能
	CUI();
	~CUI();

	static CUI *Create(CTextureManager *pTextureManager, int nNumEnemy, int nMapIdx);

	HRESULT Init(CTextureManager *pTextureManager, int nNumEnemy, int nMapIdx);
	void Uninit(void);
	void ReCreateEnemyIcon(int nNumEnemy);
	void CutEnemyIcon(int nIdx);
	void ReCreatePlayerStock(int nStock);
	void ReCreateStageNumber(int nMapIdx);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void ClearVariable(void);

	void CreateBg(void);
	void CreateEnemyIcon(int nNumEnemy);
	void CreatePlayerIcon(void);
	void CreatePlayerStock(void);
	void CreateStageIcon(void);
	void CreateStageNumber(int nMapIdx);

	void ReleaseBg(void);
	void ReleaseEnemyIcon(void);
	void ReleasePlayerIcon(void);
	void ReleasePlayerStock(void);
	void ReleaseStageIcon(void);
	void ReleaseStageNumber(void);

	CTextureManager *m_pTextureManager;         // テクスチャ管轄クラスへのポインタ(ベースモードクラスから取得する)
	CScene2D *m_pBg;                            // 背景に使用するポリゴンクラスへのポインタ
	CScene2D **m_pEnemyIcon;                    // 敵のアイコン表示用ポリゴンクラスへのポインタ
	CScene2D *m_pPlayerIcon[MAX_NUM_PLAYER];    // プレイヤーアイコン表示用ポリゴン
	CNumber **m_pPlayerStock[MAX_NUM_PLAYER];   // プレイヤーの残機数表示用ポリゴン
	CScene2D *m_pStageIcon;                     // ステージアイコン表示用ポリゴン
	CNumber **m_pStageNumber;                   // 現在のステージ数表示用ポリゴン
	int m_nNumEnemy;                            // 敵のアイコンの数
	int m_nCharaStockDigit[MAX_NUM_PLAYER];     // プレイヤーの残機数の桁数
	int m_nStageNumberDigit;                    // 現在のステージ数の桁数
};

#endif