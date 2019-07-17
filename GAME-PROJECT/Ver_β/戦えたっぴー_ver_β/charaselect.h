//=============================================================================
//
// キャラセレクトの処理 [charaselect.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _CHARASELECT_H_
#define _CHARASELECT_H_

#include "main.h"
#include "basemode.h"
#include "game.h"

//前方宣言
class CFileLoader;
class CFileSaver;
class CScene2D;
class CScene2DFlash;

// 後にプレイヤーになる
class CScene3D;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_CHARASELECT_PREPARATION_TEX (2)   // 準備中テクスチャの数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCharaSelect : public CBasemode
{
public:	// 誰からもアクセス可能
	//--------------
	// 状態
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_SELECT,
		STATE_CHARACHANGE_TO_LEFT,
		STATE_CHARACHANGE_TO_RIGHT,
		STATE_WAIT_PARTNER,
		STATE_STAGE_SELECT,
		STATE_WAIT_STAGESELECT,
		STATE_END,
		STATE_MAX
	}STATE;

	// メンバ関数
	CCharaSelect();
	~CCharaSelect();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	STATE GetState(void);

	// 静的メンバ関数
	static CCharaSelect *Create(void);
	static int GetPlayerNumber(int nIdx);
	static int GetStageType(void);
	static void SetStageType(const int nType);

protected: // このクラスと派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
	void ClearVariable(void);

	// 生成用関数
	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreatePlayer(void);
	void CreateRenderTexture(void);
	void CreateStagePolygon(void);
	void CreatePlayerDataPointer(void);

	// 開放用関数
	void ReleaseBg(void);
	void ReleasePlayer(void);
	void ReleaseRenderTexture(void);
	void ReleaseRenderPolygon(void);
	void ReleasePreparation(void);
	void ReleasePlayerNumber(void);
	void ReleaseYouPolygon(void);
	void ReleaseStagePolygon(void);
	void ReleasePlayerDataPointer(void);

	// サーバーとの交信用関数
	void SetDataToServer(void);
	void GetDataFromServer(void);

	// 状態による処理分け用関数
	void SelectUpdate(int nIdx);
	void CharaChangeToLeftUpdate(int nIdx);
	void CharaChangeToRightUpdate(int nIdx);
	void WaitPartnerUpdate(int nIdx);
	void StageSelectUpdate(int nIdx);
	void WaitStageSelectUpdate(int nIdx);
	void EndUpdate(void);
	void WaitInputToChangeChara(int nIdx);
	void WaitInputToStageSelect(int nIdx);
	void ChangeNoneStagePolygon(int nSelect);
	void ChangeSelectStagePolygon(int nSelect);
	void CircleRotation(int nIdx);
	void ChangeState_WaitPartnerToStageSelect(int nIdx);

	// 描画用関数
	void CharaDraw(int nCntTex);
	void TextureDraw(void);

	// スクリプト読み込み用関数
	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadPlayerDataFileName(char *pStr, int nCntPlayerData);
	void LoadBgPolygon(CFileLoader *pFileLoader, char *pStr);
	void LoadPreparation(CFileLoader *pFileLoader, char *pStr, int nCntPre);
	void LoadPlayerNumber(CFileLoader *pFileLoader, char *pStr, int nCntPlayerNum);
	void LoadYouPolygon(CFileLoader *pFileLoader, char *pStr);

	// メンバ変数
	STATE m_State[MAX_NUM_PLAYER];                              // 状態
	int m_nStateCounter;                                        // 状態を管理するカウンター
	LPDIRECT3DTEXTURE9 m_apRenderTexture[MAX_NUM_PLAYER];       // レンダリングに使用するテクスチャへのポインタ
	float m_fSelectRot[MAX_NUM_PLAYER];                         // 円形回転に使用するベクトルの角度
	float m_fAddSelectRot[MAX_NUM_PLAYER];                      // 円形回転がどれくらい進んだか
	CScene2D *m_pBg;                                            // 背景用ポリゴン
	CScene2D *m_apPolygonPlayer[MAX_NUM_PLAYER];                // プレイヤーを描画したテクスチャを描画するのに使用
	CScene2D *m_apPlayerNumber[MAX_NUM_PLAYER];                 // プレイヤー番号を表示するポリゴン
	CScene2D *m_apPreparation[MAX_NUM_PLAYER];                  // 準備中かどうか表示するポリゴン
	CScene2D *m_pYouPolygon;                                    // 自分のプレイヤー番号を表示するポリゴン
	CScene2DFlash *m_apStage[CGame::MAPTYPE_MAX];               // マップの種類を表示するポリゴン
	int m_nSelectStageSide;                                     // 現在選ばれているマップの種類番号(横方向)
	int m_nSelectStageVertical;                                 // 現在選ばれているマップの種類番号(縦方向)
	float m_fYouPolygonPosXDef;                                 // 自分のプレイヤー番号を表示するポリゴンのデフォルトX座標
	int m_nPreparationTexIdx[MAX_CHARASELECT_PREPARATION_TEX];  // 準備中ポリゴンの使用するテクスチャ番号
	int m_nPlayerNumberTexIdx[MAX_NUM_PLAYER];                  // プレイヤー番号を表示するポリゴンの使用するテクスチャ番号
	int m_nNumPlayerData;                                       // 読み込むプレイヤーの種類の数
	char **m_aPlayerDataFileName;                               // 読み込むプレイヤーデータのファイル名

	// 静的メンバ変数
	static int m_nSelectPlayer[MAX_NUM_PLAYER];                 // 何番のプレイヤーを選んでいるか
	static int m_nSelectStage;                                  // 使用するマップの種類

	// 後でプレイヤークラスに入れ替え
	CScene3D **m_pPlayer[MAX_NUM_PLAYER];   // プレイヤーの選択用のポリゴン
};
#endif