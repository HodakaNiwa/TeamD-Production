//=============================================================================
//
// プレイヤーマネージャーの処理 [playerManager.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PLAYERMANAGER_H_
#define _PLAYERMANAGER_H_

#include "main.h"
#include "scene.h"
#include "player.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CFileLoader;
class CModelCreate;
class CMotionManager;
class CMotion;
class CPlayer;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_STOCK (1)	//プレイヤーの残機数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayerManager
{
public:	//誰からもアクセス可能
	//*************************************************************************
	// 構造体の定義
	//*************************************************************************
	typedef enum
	{
		SELECT_PLAYER_0 = 0,
		SELECT_PLAYER_1,
		SELECT_PLAYER_2,
		SELECT_PLAYER_3,
		SELECT_PLAYER_MAX,
	}SELECT_PLAYER;

	//メンバ関数
	CPlayerManager();
	~CPlayerManager();
	HRESULT Init(char *PlayerFileName);
	void Uninit(void);
	CPlayer *SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nIdx);

	CModel **GetModel(void);
	CMotionManager *GetMotionManager(void);
	int GetParts(void);

	//静的メンバ関数
	static CPlayerManager *Create(char *PlayerFileName);

private:	//自分だけがアクセス可能
	//メンバ関数
	HRESULT LoadPlayer(char *pStr, CFileLoader *pFileLoader,int nPlayerIdx);
	CModel **ReadCharacterSet(char *pStr, CFileLoader *pFileLoader, int *pNumParts, int *pParent,int nPlayerIdx);
	CModel *ReadParts(char *pStr, CFileLoader *pFileLoader, int *pParent, CModel **apModel,CModelCreate *pModelCreate);
	CMotion *ReadMotionSet(char *pStr, CFileLoader *pFileLoader, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pStr, CFileLoader *pFileLoader, int nCntKeySet, int *pPlayBackKey, D3DXVECTOR3 *pAddPos, D3DXVECTOR3 *pDestRot);
	void ReadKey(char *pStr, CFileLoader *pFileLoader, int nCntKey, D3DXVECTOR3 *pAddPos, D3DXVECTOR3 *pDestPos);
	void CreatePlayer(CModel **apModel, CModelCreate *pModelCreate, CMotionManager *pMotionManager,int nNumParts, int *pParent,int nPlayer);

	//メンバ変数
	int m_nNumPlayer;
	CModelCreate **m_pModelCreate;				//モデル生成のポインタ情報
	int *m_pParent;                             // 親モデルの番号(後で配置するために使う)
	CModel **m_apModel;							//モデルのポインタ情報
	CMotionManager *m_pMotionManager;			//モーションマネージャーのポインタ情報
	//CMotion *m_pMotion[CPlayer::MOTION_MAX];	//モーションのポインタ情報
	int m_nNumParts;
};
#endif
