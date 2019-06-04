//*****************************************************************************
//
//     �}�b�v�̏���[map.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MAP_H_
#define _MAP_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAP_FILENAME           "MAP_FILENAME = "                // �}�b�v�̃t�@�C������ǂݍ��ލ��}
#define SAVEFILENAME_MAP       "data/TEXT/MAP/"                 // �}�b�v���ۑ���̃p�X��
#define SAVEFILENAME_GAMEFIELD "data/TEXT/MAP/GAMEFIELD/"       // �Q�[���t�B�[���h���ۑ���̃p�X��
#define SAVEFILENAME_MODELLIST "data/TEXT/MAP/MODELLIST/"       // ���f�����X�g���ۑ���̃p�X��
#define SAVEFILENAME_TEXLIST   "data/TEXT/MAP/TEXLIST/"         // �e�N�X�`�����X�g���ۑ���̃p�X��
#define SAVEFILENAME_LIGHT     "data/TEXT/MAP/LIGHT/"           // ���C�g���ۑ���̃p�X��
#define SAVEFILENAME_OBJECT    "data/TEXT/MAP/OBJECT/"          // �z�u�����ۑ���̃p�X��
#define MAX_PLAYER_RESPAWN     (2)                              // �v���C���[�̃��X�|�[���ʒu�̍ő�l
#define MAX_ENEMY_RESPAWN      (3)                              // �G�̃��X�|�[���ʒu�̍ő�l

//*****************************************************************************
//    �O���錾
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
//    �}�b�v�N���X�̒�`
//*****************************************************************************
class CMap
{
public:    // �N�ł��A�N�Z�X�\
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

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
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

	char            m_aFileName[256];                      // �ǂݍ��ރ}�b�v�f�[�^�̃X�N���v�g�t�@�C����
	char            m_aModelListFileName[256];             // �ǂݍ��ރ��f�����X�g���X�N���v�g�t�@�C����
	char            m_aTexListFileName[256];               // �ǂݍ��ރe�N�X�`�����X�g���̃X�N���v�g�t�@�C����
	char            m_aLightFileName[256];                 // �ǂݍ��ރ��C�g���̃X�N���v�g�t�@�C����
	char            m_aGameFieldFileName[256];             // �ǂݍ��ރQ�[���t�B�[���h���̃X�N���v�g�t�@�C����
	char            m_aObjectFileName[256];                // �ǂݍ��ޔz�u�����̃X�N���v�g�t�@�C����
	D3DXVECTOR3     m_BasePos;                             // �i�ߕ��̈ʒu
	D3DXVECTOR3     m_PlayerRespawn[MAX_PLAYER_RESPAWN];   // �v���C���[�̃��X�|�[���ʒu(2�l��)
	D3DXVECTOR3     m_EnemyRespawn[MAX_ENEMY_RESPAWN];     // �G�̃��X�|�[���ʒu(3�ӏ���)
	CTextureManager *m_pTextureManager;                    // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	CModelCreate    *m_pModelCreate;                       // ���f���Ǌ��N���X�ւ̃|�C���^
	CLightManager   *m_pLightManager;                      // ���C�g�Ǌ��N���X�ւ̃|�C���^
};

#endif