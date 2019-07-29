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
#define SAVEFILENAME_ENEMYLIST "data/TEXT/MAP/ENEMYLIST/"       // 敵の生成情報保存先のパス名
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
class CCharacterManager;
class CEffectManager;
class CBlock;
class CRiver;
class CIceField;
class CDirectionalLight;
class CPointLight;
class CSpotLight;
class CMeshField;
class CRespawn;
class CHeadQuarters;
class CSky;
class CObject;
class CBillboardObject;
class CEmitter;

//*****************************************************************************
//    敵の生成情報データクラスの定義
//*****************************************************************************
class CEnemy_ListData
{
public:    // 誰でもアクセス可能
	CEnemy_ListData();
	~CEnemy_ListData();

	void Cpy(CEnemy_ListData EnemyData);

	void SetRespawnIdx(const int nRespawnIdx);
	void SetEnemyType(const int nEnemyType);
	void SetRespawnTime(const int nRespawnTime);
	void SetItem(const bool bItem);
	void SetItemType(const int nItemType);
	void SetAI_BulletEva(const int nAI_BulletEva);
	void SetAI_MassEva(const int nAI_MassEva);
	void SetAI_DownEva(const int nAI_DownEva);

	int GetRespawnIdx(void);
	int GetEnemyType(void);
	int GetRespawnTime(void);
	bool GetItem(void);
	int GetItemType(void);
	int GetAI_BulletEva(void);
	int GetAI_MassEva(void);
	int GetAI_DownEva(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int m_nRespawnIdx;     // リスポーン位置の番号
	int m_nEnemyType;      // 敵の種類
	int m_nRespawnTime;    // 敵を出現させる時間(60FPS上で換算)
	bool m_bItem;          // 倒した時にアイテムを出現させるか
	int m_nItemType;       // 出現するアイテムの種類番号
	int m_nAI_BulletEva;   // 弾を打つ際のランダム評価値
	int m_nAI_MassEva;     // マスを移動した際に向きを変えさせるか判定するときのランダム評価値
	int m_nAI_DownEva;     // 下向きに動かすかどうかを判定する際のランダム評価値
};

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
	HRESULT LoadModelList(char *pModelFileName, char *pStr);
	HRESULT LoadModel(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadTextureList(char *pTextureFileName, char *pStr);
	HRESULT LoadTexture(char *pStr, CFileLoader *pFileLoader);
	HRESULT LoadHeadQuartersPos(char *pStr);
	HRESULT LoadPlayerRespawn(char *pStr, int nCntPlayerRes);
	HRESULT LoadEnemyRespawn(char *pStr, int nCntEnemyRes);
	HRESULT LoadSky(char *pStr, CFileLoader *pFileLoader);
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
	HRESULT LoadEnemyList(char *pEnemylistName, char *pStr);
	HRESULT LoadEnemyListInfo(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList);

	HRESULT SaveModelList(CFileSaver *pFileSaver);
	HRESULT SaveTexList(CFileSaver *pFileSaver);
	HRESULT SavePlayerRespawn(CFileSaver *pFileSaver);
	HRESULT SaveEnemyRespawn(CFileSaver *pFileSaver);
	HRESULT SaveHeadQuartersPos(CFileSaver *pFileSaver);
	HRESULT SaveSky(CFileSaver *pFileSaver);
	HRESULT SaveMap(CFileSaver *pFileSaver);
	HRESULT SaveModel(char *pModelFileName);
	HRESULT SaveTexture(char *pTextureFileName);
	HRESULT SaveLight(char *pLightFileName);
	HRESULT SaveGameField(char *pGameFieldFileName);
	HRESULT SaveObject(char *pObjectName);
	HRESULT SaveEnemyList(char *pEnemyListName);

	void CreateBlock(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth);

	void DeleteMap(void);
	void DeleteLight(void);
	void DeleteGameField(void);
	void DeleteObject(void);
	void DeleteEnemy(void);

	int GetFieldTexIdx(void);
	CMeshField *GetMeshField(void);
	CSky *GetSky(void);
	CTextureManager *GetTextureManager(void);
	CModelCreate *GetModelCreate(void);
	CLightManager *GetLightManager(void);
	CCharacterManager **GetObjectManager(void);
	CCharacterManager *GetObjectManager(int nIdx);
	char *GetModelListFileName(void);
	char *GetTexListFileName(void);
	char *GetLightFileName(void);
	char *GetGameFieldFileName(void);
	char *GetObjectFileName(void);
	CHeadQuarters *GetHeadQuarters(void);
	CRespawn *GetPlayerRespawn(int nIdx);
	CRespawn *GetEnemyRespawn(int nIdx);
	CEnemy_ListData *GetEnemyListData(int nIdx);
	int GetNumEnemyListData(void);

	void SetFieldTexIdx(const int nFieldTexIdx);
	void SetMeshField(CMeshField *pMeshField);
	void SetSky(CSky *pSky);
	void SetTextureManager(CTextureManager *pTextureManager);
	void SetModelCreate(CModelCreate *pModelCreate);
	void SetLightManager(CLightManager *pLightManager);
	void SetObjectManager(CCharacterManager **pObjectManager);
	void SetObjectManager(CCharacterManager *pObjectManager, int nIdx);
	void SetModelListFileName(char *pFileName);
	void SetTexListFileName(char *pFileName);
	void SetLightFileName(char *pFileName);
	void SetGameFieldFileName(char *pFileName);
	void SetObjectFileName(char *pFileName);
	void SetEnemyListFileName(char *pFileName);
	void SetHeadQuarters(CHeadQuarters *pHeadQuarters);
	void SetPlayerRespawn(CRespawn *pRespawn, int nIdx);
	void SetEnemyRespawn(CRespawn *pRespawn, int nIdx);
	void SetEnemyListData(CEnemy_ListData EnemyData, int nIdx);
	void SetNumEnemyListData(const int nNumEnemyData);
	void SetNumObjectData(const int nObjectData);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void LoadField(char *pStr, CFileLoader *pFileLoader);
	void LoadBlock(char *pStr, CFileLoader *pFileLoader);
	void LoadCollision(char *pStr, CFileLoader *pFileLoader, float *pWidth, float *pHeight, float *pDepth);
	void LoadRiver(char *pStr, CFileLoader *pFileLoader);
	void LoadIce(char *pStr, CFileLoader *pFileLoader);
	void LoadNumObjectData(char *pStr, CFileLoader *pFileLoader);
	void LoadObjectData(char *pStr, CFileLoader *pFileLoader, int nCntObjData);
	void LoadObjModel(char *pStr, CFileLoader *pFileLoader);
	void LoadObjBill(char *pStr, CFileLoader *pFileLoader);
	void LoadObjEffect(char *pStr, CFileLoader *pFileLoader, CEffectManager *pEffectManager);
	void LoadEnemyItem(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList);
	void LoadEnemyAI(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList);

	void FieldDown(D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fFieldDown);
	void SetRiver(int nTexIdx, D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth);
	void SetIce(int nTexIdx, D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth);

	void SaveGameFieldInfo(CFileSaver *pFileSaver);
	void SaveField(CFileSaver *pFileSaver);
	void SaveBlock(CFileSaver *pFileSaver);
	void SaveBlockInfo(CBlock *pBlock, CFileSaver *pFileSaver);
	void SaveRiver(CFileSaver *pFileSaver);
	void SaveRiverInfo(CRiver *pRiver, CFileSaver *pFileSaver);
	void SaveIce(CFileSaver *pFileSaver);
	void SaveIceInfo(CIceField *pIce, CFileSaver *pFileSaver);
	void SaveLightInfo(CFileSaver *pFileSaver);
	void SaveDirectionalLight(CDirectionalLight *pDirLight, CFileSaver *pFileSaver);
	void SavePointLight(CPointLight *pPointLight, CFileSaver *pFileSaver);
	void SaveSpotLight(CSpotLight *pSpotLight, CFileSaver *pFileSaver);
	void SaveObjectInfo(CFileSaver *pFileSaver);
	void SaveObjModel(CFileSaver *pFileSaver);
	void SaveObjModelInfo(CObject *pObject, CFileSaver *pFileSaver);
	void SaveObjBill(CFileSaver *pFileSaver);
	void SaveObjBillInfo(CBillboardObject *pBillObj, CFileSaver *pFileSaver);
	void SaveObjEffect(CFileSaver *pFileSaver);
	void SaveObjEffectInfo(CEmitter *pEmitter, CFileSaver *pFileSaver);
	void SaveEnemyListInfo(CFileSaver *pFileSaver);

	void CreateEnemyListData(void);

	void DeleteBlock(CBlock *pBlock);
	void DeleteRiver(CRiver *pRiver);
	void DeleteIce(CIceField *pIceField);
	void DeleteObjModel(CObject *pObject);
	void DeleteObjBillboard(CBillboardObject *pBillboard);
	void DeleteObjEffect(CEmitter *pEmitter);

	void ReleaseTextureManager(void);
	void ReleaseModelManager(void);
	void ReleaseLightManager(void);
	void ReleaseObjectManager(void);
	void ReleaseMeshField(void);
	void ReleaseSky(void);
	void ReleaseHeadQuarters(void);
	void ReleasePlayerRespawn(void);
	void ReleaseEnemyRespawn(void);
	void ReleaseEnemyListData(void);

	int             m_nFieldTexIdx;                          // 地面に張り付けるテクスチャの番号
	char            m_aFileName[256];                        // 読み込むマップデータのスクリプトファイル名
	char            m_aModelListFileName[256];               // 読み込むモデルリスト情報スクリプトファイル名
	char            m_aTexListFileName[256];                 // 読み込むテクスチャリスト情報のスクリプトファイル名
	char            m_aLightFileName[256];                   // 読み込むライト情報のスクリプトファイル名
	char            m_aGameFieldFileName[256];               // 読み込むゲームフィールド情報のスクリプトファイル名
	char            m_aObjectFileName[256];                  // 読み込む配置物情報のスクリプトファイル名
	char            m_aEnemyListFileName[256];               // 読み込む敵の生成情報のスクリプトファイル名

	CMeshField        *m_pMeshField;                         // 地面クラスへのポインタ
	CSky              *m_pSky;                               // 空クラスへのポインタ
	CHeadQuarters     *m_pHeadQuarters;                      // 司令部クラス型のポインタ
	CRespawn          *m_pPlayerRespawn[MAX_PLAYER_RESPAWN]; // プレイヤーのリスポーン位置(2人分)
	CRespawn          *m_pEnemyRespawn[MAX_ENEMY_RESPAWN];   // 敵のリスポーン位置(3箇所分)
	CTextureManager   *m_pTextureManager;                    // テクスチャ管轄クラスへのポインタ
	CModelCreate      *m_pModelCreate;                       // モデル管轄クラスへのポインタ
	CLightManager     *m_pLightManager;                      // ライト管轄クラスへのポインタ
	CCharacterManager **m_pObjectManager;                    // 配置物のデータ管轄クラスへのポインタ
	int               m_nNumObjectData;                      // 配置物のデータの数
	CEnemy_ListData   **m_pEnemyListData;                    // 敵の生成情報クラスへのポインタ
	int               m_nNumEnemyListData;                   // 敵の生成情報の数
};

#endif