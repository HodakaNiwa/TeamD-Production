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

//前方宣言
class CFileLoader;
class CFileSaver;
class CScene2D;

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
	void SetSelectPlayer(const int nSelectPlayer, const int nIdx);
	STATE GetState(void);
	int GetSelectPlayer(const int nIdx);

	// 静的メンバ関数
	static CCharaSelect *Create(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:	// 自分だけがアクセス可能
	void ClearVariable(void);

	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreatePlayer(void);
	void CreateRenderTexture(void);
	void CreatePlayerDataPointer(void);

	void ReleaseBg(void);
	void ReleasePlayer(void);
	void ReleaseRenderTexture(void);
	void ReleaseRenderPolygon(void);
	void ReleasePreparation(void);
	void ReleasePlayerNumber(void);
	void ReleaseYouPolygon(void);
	void ReleasePlayerDataPointer(void);

	void SetDataToServer(void);
	void GetDataFromServer(void);
	void SelectUpdate(int nIdx);
	void CharaChangeToLeftUpdate(int nIdx);
	void CharaChangeToRightUpdate(int nIdx);
	void WaitPartnerUpdate(void);
	void EndUpdate(void);
	void WaitInputToChangeChara(int nIdx);
	void WaitInputToNextMode(void);
	void CircleRotation(int nIdx);

	void CharaDraw(int nCntTex);
	void TextureDraw(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadPlayerDataFileName(char *pStr, int nCntPlayerData);
	void LoadBgPolygon(CFileLoader *pFileLoader, char *pStr);
	void LoadPreparation(CFileLoader *pFileLoader, char *pStr, int nCntPre);
	void LoadPlayerNumber(CFileLoader *pFileLoader, char *pStr, int nCntPlayerNum);
	void LoadYouPolygon(CFileLoader *pFileLoader, char *pStr);

	STATE m_State[MAX_NUM_PLAYER];                              // 状態
	int m_nStateCounter;                                        // 状態を管理するカウンター
	LPDIRECT3DTEXTURE9 m_apRenderTexture[MAX_NUM_PLAYER];       // レンダリングに使用するテクスチャへのポインタ
	int m_nSelectPlayer[MAX_NUM_PLAYER];                        // 何番のプレイヤーを選んでいるか
	float m_fSelectRot[MAX_NUM_PLAYER];                         // 円形回転に使用するベクトルの角度
	float m_fAddSelectRot[MAX_NUM_PLAYER];                      // 円形回転がどれくらい進んだか
	CScene2D *m_pBg;                                            // 背景用ポリゴン
	CScene2D *m_apPolygonPlayer[MAX_NUM_PLAYER];                // プレイヤーを描画したテクスチャを描画するのに使用
	CScene2D *m_apPlayerNumber[MAX_NUM_PLAYER];                 // プレイヤー番号を表示するポリゴン
	CScene2D *m_apPreparation[MAX_NUM_PLAYER];                  // 準備中かどうか表示するポリゴン
	CScene2D *m_pYouPolygon;                                    // 自分のプレイヤー番号を表示するポリゴン
	float m_fYouPolygonPosXDef;                                 // 自分のプレイヤー番号を表示するポリゴンのデフォルトX座標
	int m_nPreparationTexIdx[MAX_CHARASELECT_PREPARATION_TEX];  // 準備中ポリゴンの使用するテクスチャ番号
	int m_nPlayerNumberTexIdx[MAX_NUM_PLAYER];                  // プレイヤー番号を表示するポリゴンの使用するテクスチャ番号
	int m_nNumPlayerData;                                       // 読み込むプレイヤーの種類の数
	char **m_aPlayerDataFileName;                               // 読み込むプレイヤーデータのファイル名

	// 後でプレイヤークラスに入れ替え
	CScene3D **m_pPlayer[MAX_NUM_PLAYER];   // プレイヤーの選択用のポリゴン
};
#endif