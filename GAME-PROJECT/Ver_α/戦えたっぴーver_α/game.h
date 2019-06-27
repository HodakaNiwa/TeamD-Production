//=============================================================================
//
// �Q�[���̏��� [game.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "basemode.h"
#include "item.h"

//�O���錾
class CFileLoader;
class CFileSaver;
class CUI;
class CPlayer;
class CEnemy_ListData;
class CScene2D;
class CNumber;
class CEnemy;
class CBullet;
class CBlock;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CBasemode
{
public:	// �N������A�N�Z�X�\
	//--------------
	// ���
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_STAGE_SELECT,
		STATE_STAGE_DISP,
		STATE_WAIT,
		STATE_NORMAL,
		STATE_GAMEOVER,
		STATE_PREV_MAP,
		STATE_NEXT_MAP,
		STATE_END,
		STATE_MAX
	}STATE;

	// �����o�֐�
	CGame();
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetBulletModel(CBullet *pBullet);
	void SetItemModel(CItem *pItem, int nType);
	void DeletePlayer(CPlayer *pPlayer, const int nIdx);
	void DeleteBlock(const int nIdx);
	void DeleteEnemy(const int nIdx);

	void SetState(const STATE state);
	void SetPlayer(CPlayer *pPlayer, const int nIdx);
	void SetNumEnemy(const int nNumEnemy);

	STATE GetState(void);
	CPlayer *GetPlayer(const int nIdx);
	int GetNumEnemy(void);

	// �ÓI�����o�֐�
	static CGame *Create(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
	void ClearVariable(void);
	void CreateMapFilePointer(void);
	void ReleaseMapFilePointer(void);

	void CreateTexture(int nNumTex);
	void CreateItemModel(int nNumItem);
	void CreateCamera(void);
	void CreateMap(void);
	void CreateUI(void);
	void CreatePlayer(void);
	void CreateGameOverLogo(void);
	void CreateStageBg(void);
	void CreateStageLogo(void);
	void CreateStageNumber(void);

	void ReleaseUI(void);
	void ReleasePlayer(void);
	void ReleaseGameOverLogo(void);
	void ReleaseStageBg(void);
	void ReleaseStageLogo(void);
	void ReleaseStageNumber(void);

	void SetDataToServer(void);
	void SetDataToServerFromPlayer(void);
	void SetDataToServerFromGame(void);
	void SetDataToServerFromEnemy(void);
	void SetDataToServerFromEnemyBullet(void);
	void SetDataToServerFromPlayerBullet(void);
	void SetDataToServerFromDeleteBlock(void);
	void SetDataToServerFromDeleteEnemy(void);
	char *GetDataToEnemy(CEnemy *pEnemy, char *pStr);
	char *GetDataToEnemyBullet(CBullet *pBullet, char *pStr);
	char *GetDataToPlayerBullet(CBullet *pBullet, char *pStr);
	void GetDataFromServer(void);
	char *SetDataToPlayerFromServer(char *pStr);
	char *SetDataToGameFromServer(char *pStr);
	char *SetDataToEnemyFromServer(char *pStr);
	char *SetDataToEnemy(CEnemy *pEnemy, char *pStr);
	char *SetDataToCreateEnemy(char *pStr);
	void ReleaseEnemy(int nNumEnemy);
	void ReleaseCheckEnemy(CEnemy *pEnemy, int *pNumEnemy);
	char *SetDataToPlayerBulletFromServer(char *pStr);
	void ReleasePlayerBullet(int nNumBullet);
	void ReleaseCheckPlayerBullet(CBullet *pBullet, int *pNumBullet);
	char *SetDataToPlayerBullet(CBullet *pBullet, char *pStr, int *pNumBullet);
	char *SetDataToCreatePlayerBullet(char *pStr);
	char *SetDataToEnemyBulletFromServer(char *pStr);
	void ReleaseEnemyBullet(int nNumBullet);
	void ReleaseCheckEnemyBullet(CBullet *pBullet, int *pNumBullet);
	char *SetDataToEnemyBullet(CBullet *pBullet, char *pStr, int *pNumBullet);
	char *SetDataToCreateEnemyBullet(char *pStr);
	char *SetDataToDeleteBlock(char *pStr);
	void ReleaseCheckBlock(CBlock *pBlock, int *pDeleteIdx, int *nNumDeleteBlock);
	char *SetDataToDeleteEnemy(char *pStr);
	void ReleaseCheckDeleteEnemy(CEnemy *pEnemy, int *pDeleteIdx, int *nNumDeleteEnemy);
	void StageSelectUpdate(void);
	void StageDispUpdate(void);
	void NormalUpdate(void);
	void GameOverUpdate(void);
	void GameOverLogoUp(void);
	void EndUpdate(void);

	void SetPlayerPosToSpawn(void);
	void CheckPlayerResSpawn(int nCntPlayer);
	void CheckEnemySpawn(int nTime);
	void EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt);
	void ResetCounter(void);
	void ChangeNextMap(void);
	void ChangePrevMap(void);
	void DeleteGameObject(void);
	void CalcStageDigits(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadModel(char *pStr, int nCntModel);
	void LoadMapFileName(char *pStr, int nCntMap);
	void LoadStageBg(CFileLoader *pFileLoader, char *pStr);
	void LoadStageLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadStageNumber(CFileLoader *pFileLoader, char *pStr);
	void LoadGameOverLogo(CFileLoader *pFileLoader, char *pStr);

	int m_nNumMap;                        // �ǂݍ��ރ}�b�v�̐�
	char **m_pMapFileName;                // �ǂݍ��ރ}�b�v�̃t�@�C����
	int m_nMapIdx;                        // ���݂̃}�b�v�̔ԍ�
	CUI *m_pUI;                           // UI�N���X�ւ̃|�C���^
	STATE m_State;                        // ����̏��
	STATE m_StateOld;                     // �O��̏��
	int m_nStateCounter;                  // ��Ԃ��Ǘ�����J�E���^�[
	CPlayer *m_pPlayer[MAX_NUM_PLAYER];   // �v���C���[�N���X�^�̃|�C���^
	int m_nGameCounter;                   // �Q�[���̎��Ԃ𐔂���J�E���^�[
	int m_nSpawnEnemyCount;               // �X�|�[�������G�̐��𐔂���J�E���^�[
	int m_nNumEnemy;                      // �o������G�̐�
	CScene2D *m_pGameOverLogo;            // �Q�[���I�[�o�[��m�点�郍�S�|���S��
	CScene2D *m_pStageBg;                 // �X�e�[�W�\����Ԃ̎��̔w�i�|���S��
	CScene2D *m_pStageLogo;               // �X�e�[�W���S�p�|���S��
	CNumber **m_pStageNumber;             // �X�e�[�W�ԍ��\���p�|���S��
	int m_nNumberTexIdx;                  // �����|���S�����g�p����e�N�X�`���̔ԍ�
	int m_nNumNumberDigits;               // ���݂̃X�e�[�W�ԍ��̌���
	int m_nPlayerStock[MAX_NUM_PLAYER];   // �v���C���[�̎c�@��
	int m_nPlayerRespawnCounter;          // �v���C���[�̃��X�|�[�����Ǘ�����J�E���^�[
	int m_nBulletModelIdx;                // �e���g�p���郂�f���̔ԍ�
	int m_nItemModelIdx[CItem::TYPE_MAX]; // �A�C�e�����g�p���郂�f���̔ԍ�

	// �u���b�N�̓����ɕK�v
	int m_nNumDeleteBlock;
	char m_aDeleteBlock[2048];

	// �G�̓����ɕK�v
	int m_nNumDeleteEnemy;
	char m_aDeleteEnemy[2048];

	// �e��N���X�����p���
	typedef struct
	{// �X�e�[�W�w�i�f�[�^
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}STAGEBG_DATA;
	STAGEBG_DATA m_StageBgData;

	typedef struct
	{// �X�e�[�W���S�f�[�^
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}STAGELOGO_DATA;
	STAGELOGO_DATA m_StageLogoData;

	typedef struct
	{// �X�e�[�W�ԍ��f�[�^
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}STAGENUMBER_DATA;
	STAGENUMBER_DATA m_StageNumberData;

	typedef struct
	{// �Q�[���I�[�o�[���S�f�[�^
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}GAMEOVERPOLY_DATA;
	GAMEOVERPOLY_DATA m_GameOverPolyData;
};
#endif