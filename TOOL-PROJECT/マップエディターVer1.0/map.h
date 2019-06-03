//*****************************************************************************
//
//     マップの処理[map.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MAP_H_
#define _MAP_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MAP_FILENAME           "MAP_FILENAME = "                // マップのファイル名を読み込む合図
#define SAVEFILENAME_MAP       "data/TEXT/MAP/"                 // マップ情報保存先のパス名
#define SAVEFILENAME_GAMEFIELD "data/TEXT/MAP/GAMEFIELD/"       // ゲームフィールド情報保存先のパス名
#define SAVEFILENAME_MODELLIST "data/TEXT/MAP/MODELLIST/"       // モデルリスト情報保存先のパス名
#define SAVEFILENAME_TEXLIST   "data/TEXT/MAP/TEXLIST/"         // テクスチャリスト情報保存先のパス名
#define SAVEFILENAME_LIGHT     "data/TEXT/MAP/LIGHT/"           // ライト情報保存先のパス名
#define SAVEFILENAME_OBJECT    "data/TEXT/MAP/OBJECT/"          // 配置物情報保存先のパス名
#define MAX_PLAYER_RESPAWN     (2)                              // プレイヤーのリスポーン位置の最大値
#define MAX_ENEMY_RESPAWN      (3)                              // 敵のリスポーン位置の最大値

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CFileLoader;
class CFileSaver;
class CTextureManager;
class CModelCreate;
class CLightManager;
class CBlock;
class CDirectionalLight;
class CPointLight;
class CSpotLight;

//*****************************************************************************
//    マップクラスの定義
//*****************************************************************************
class CMap
{
public:    // 誰でもアクセス可能
	CMap();
	~CMap();

	static CMap *Create(char *pFileName);

	HRESULT Init(char *pFileName);
	void Uninit(void);
	HRESULT Load(char *pLoadFileName);
	HRESULT Save(char *pSaveFileName);
	HRESULT ChangeMap(char *pLoadFileName);

	HRESULT LoadScript(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadBasePos(char *pStr);
	HRESULT LoadPlayerRespawn(char *pStr, int nCntPlayerRes);
	HRESULT LoadEnemyRespawn(char *pStr, int nCntEnemyRes);
	HRESULT LoadModelList(char *pModelFileName, char *pStr);
	HRESULT LoadModel(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadTextureList(char *pTextureFileName, char *pStr);
	HRESULT LoadTexture(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadMap(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadLight(char *pLightFileName, char *pStr);
	HRESULT LoadLightInfo(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadDirectionalLight(char *pStr, CFileLoader *pFileLoader, int nCntLight);
	HRESULT LoadPointLight(char *pStr, CFileLoader *pFileLoader, int nCntLight);
	HRESULT LoadSpotLight(char *pStr, CFileLoader *pFileLoader, int nCntLight);
	HRESULT LoadGameField(char *pGameFieldFileName, char *pStr);
	HRESULT LoadGameFieldInfo(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadObject(char *pObjectName, char *pStr);
	HRESULT LoadObjectInfo(char *pStr, CFileLoader *pFileLoader);

	HRESULT SaveBasePos(CFileSaver *pFileSaver);
	HRESULT SavePlayerRespawn(CFileSaver *pFileSaver);
	HRESULT SaveEnemyRespawn(CFileSaver *pFileSaver);
	HRESULT SaveModelList(CFileSaver *pFileSaver);
	HRESULT SaveTexList(CFileSaver *pFileSaver);
	HRESULT SaveMap(CFileSaver *pFileSaver);
	HRESULT SaveModel(char *pModelFileName);
	HRESULT SaveTexture(char *pTextureFileName);
	HRESULT SaveLight(char *pLightFileName);
	HRESULT SaveGameField(char *pGameFieldFileName);
	HRESULT SaveObject(char *pObjectName);

	void DeleteMap(void);
	void DeleteLight(void);
	void DeleteGameField(void);
	void DeleteObject(void);

	CTextureManager *GetTextureManager(void);
	CModelCreate *GetModelCreate(void);
	CLightManager *GetLightManager(void);
	char *GetModelListFileName(void);
	char *GetTexListFileName(void);
	char *GetLightFileName(void);
	char *GetGameFieldFileName(void);
	char *GetObjectFileName(void);

	void SetTextureManager(CTextureManager *pTextureManager);
	void SetModelCreate(CModelCreate *pModelCreate);
	void SetLightManager(CLightManager *pLightManager);
	void SetModelListFileName(char *pFileName);
	void SetTexListFileName(char *pFileName);
	void SetLightFileName(char *pFileName);
	void SetGameFieldFileName(char *pFileName);
	void SetObjectFileName(char *pFileName);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void LoadBlock(char *pStr, CFileLoader *pFileLoader);
	void LoadCollision(char *pStr, CFileLoader *pFileLoader, float *pWidth, float *pHeight, float *pDepth);
	void LoadIce(char *pStr, CFileLoader *pFileLoader);
	void LoadRiver(char *pStr, CFileLoader *pFileLoader);
	void LoadObjModel(char *pStr, CFileLoader *pFileLoader);
	void LoadObjBill(char *pStr, CFileLoader *pFileLoader);
	void LoadObjEffect(char *pStr, CFileLoader *pFileLoader);

	void SaveGameFieldInfo(CFileSaver *pFileSaver);
	void SaveBlock(CBlock *pBlock, CFileSaver *pFileSaver);
	void SaveIce(CFileSaver *pFileSaver);
	void SaveRiver(CFileSaver *pFileSaver);
	void SaveLightInfo(CFileSaver *pFileSaver);
	void SaveDirectionalLight(CDirectionalLight *pDirLight, CFileSaver *pFileSaver);
	void SavePointLight(CPointLight *pPointLight, CFileSaver *pFileSaver);
	void SaveSpotLight(CSpotLight *pSpotLight, CFileSaver *pFileSaver);
	void SaveObjectInfo(CFileSaver *pFileSaver);
	void SaveObjModel(CFileSaver *pFileSaver);
	void SaveObjBill(CFileSaver *pFileSaver);
	void SaveObjEffect(CFileSaver *pFileSaver);

	void DeleteBlock(CBlock *pBlock);
	void DeleteObjModel(void);
	void DeleteObjBillboard(void);
	void DeleteObjEffect(void);

	void ReleaseTextureManager(void);
	void ReleaseModelManager(void);
	void ReleaseLightManager(void);

	char            m_aFileName[256];                      // 読み込むマップデータのスクリプトファイル名
	char            m_aModelListFileName[256];             // 読み込むモデルリスト情報スクリプトファイル名
	char            m_aTexListFileName[256];               // 読み込むテクスチャリスト情報のスクリプトファイル名
	char            m_aLightFileName[256];                 // 読み込むライト情報のスクリプトファイル名
	char            m_aGameFieldFileName[256];             // 読み込むゲームフィールド情報のスクリプトファイル名
	char            m_aObjectFileName[256];                // 読み込む配置物情報のスクリプトファイル名
	D3DXVECTOR3     m_BasePos;                             // 司令部の位置
	D3DXVECTOR3     m_PlayerRespawn[MAX_PLAYER_RESPAWN];   // プレイヤーのリスポーン位置(2人分)
	D3DXVECTOR3     m_EnemyRespawn[MAX_ENEMY_RESPAWN];     // 敵のリスポーン位置(3箇所分)
	CTextureManager *m_pTextureManager;                    // テクスチャ管轄クラスへのポインタ
	CModelCreate    *m_pModelCreate;                       // モデル管轄クラスへのポインタ
	CLightManager   *m_pLightManager;                      // ライト管轄クラスへのポインタ
};

#endif