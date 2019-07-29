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
#include "enemy.h"

// �O���錾
class CFileLoader;
class CFileSaver;
class CUI;
class CPlayer;
class CPlayerManager;
class CEnemy_ListData;
class CScene2D;
class CNumber;
class CBullet;
class CBlock;
class CCharacterManager;

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
		STATE_PAUSE,
		STATE_GAMEOVER,
		STATE_RESULT,
		STATE_CHANGE_MAP,
		STATE_PREV_MAP,
		STATE_NEXT_MAP,
		STATE_END,
		STATE_MAX
	}STATE;

	//--------------
	// �}�b�v�̎��
	//--------------
	typedef enum
	{
		MAPTYPE_NONE = -1,   // ����
		MAPTYPE_CANDY,       // ���َq
		MAPTYPE_CHRISTMAS,   // �N���X�}�X
		MAPTYPE_HINAMATSURI, // �ЂȍՂ�
		MAPTYPE_MAX
	}MAPTYPE;

	//---------------------
	// �ЂȍՂ�̃C�x���g
	//---------------------
	typedef enum
	{
		HINAEVENT_NONE = -1,
		HINAEVENT_NORMAL,
		HINAEVENT_CHERRY_BLOSSOMS,
		HINAEVENT_DROP_ITEM,
		HINAEVENT_MAX
	}HINAEVENT;

	// �����o�֐�
	CGame();
	~CGame();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nItemType);
	void DeletePlayer(CPlayer *pPlayer, const int nIdx);
	void DeleteBlock(const int nIdx);
	void DeleteEnemy(const int nIdx);

	// �A�C�e���̏������s�p�֐�
	void ItemEvent_Star(int nPlayerNumber);
	void ItemEvent_Grenade(void);
	void ItemEvent_Granade_Shake(void);
	void ItemEvent_1Up(int nPlayerNumber);
	void ItemEvent_Scoop(void);
	void ItemEvent_Clock(void);
	void ItemEvent_Helmet(int nPlayerNumber);


	void SetBulletModel(CBullet *pBullet);
	void SetState(const STATE state);
	void SetPlayer(CPlayer *pPlayer, const int nIdx);
	void SetNumEnemy(const int nNumEnemy);

	STATE GetState(void);
	CPlayer *GetPlayer(const int nIdx);
	int GetNumEnemy(void);
	bool GetEnemyMove(void);

	// �ÓI�����o�֐�
	static CGame *Create(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
	void ClearVariable(void);

	// ���������p�֐�
	void CreateMapFilePointer(void);
	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateMap(void);
	void CreateUI(void);
	void CreatePlayer(void);
	CEnemy *CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData);
	void CreateGameOverLogo(void);
	void CreateStageBg(void);
	void CreateStageLogo(void);
	void CreateStageNumber(void);
	void CreateBlossoms(void);
	void CreateGameResult(void);
	void CreateGameResult_Bg(void);
	void CreateGameResult_Stage(void);
	void CreateGameResult_HighScore(void);
	void CreateGameResult_PlayerScore(void);
	void CreateGameResult_EnemyScore_Logo(void);
	void CreateGameResult_EnemyScore_Number(void);
	void CreateGameResult_TortalScore_Logo(void);
	void CreateGameResult_TortalScore_Number(void);
	void CreateGameResult_BonusScore(void);


	// �J�������p�֐�
	void ReleaseMapFilePointer(void);
	void ReleaseUI(void);
	void ReleasePlayer(void);
	void ReleasePlayerManager(void);
	void ReleaseEnemyManager(void);
	void ReleaseGameOverLogo(void);
	void ReleaseStageBg(void);
	void ReleaseStageLogo(void);
	void ReleaseStageNumber(void);
	void ReleaseGameResult(void);
	void ReleaseGameResult_Bg(void);
	void ReleaseGameResult_Stage(void);
	void ReleaseGameResult_HighScore(void);
	void ReleaseGameResult_PlayerScore(void);
	void ReleaseGameResult_EnemyScore(void);
	void ReleaseGameResult_TortalScore(void);
	void ReleaseGameResult_BonusScore(void);


	// �T�[�o�[�Ƃ̃��b�Z�[�W�����p�֐�
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


	// ��Ԃɂ��X�V���������p�֐�
	void StageSelectUpdate(void);
	void StageDispUpdate(void);
	void NormalUpdate(void);
	void PauseUpdate(void);
	void GameOverUpdate(void);
	void GameOverLogoUp(void);
	void ResultUpdate(void);
	void ChangeMapUpdate(void);
	void EndUpdate(void);


	// �}�b�v�C�x���g�p�֐�(�݂�Ȃ������� OR �ǉ�����Ȃ炱���I�I ����ȊO�͕ʓr���k���āE�E�E)
	void MapEvent_Candy(void);
	void MapEvent_Christmas(void);
	void MapEvent_Hinamatsuri(void);
	void MapEvent_Hinamatsuri_Normal(void);
	void MapEvent_Hinamatsuri_CherryBlossoms(void);
	void MapEvent_Hinamatsuri_DropItem(void);


	// �Q�[�����X�|�[�������p�֐�
	void SetPlayerPosToSpawn(void);
	void CheckPlayerResSpawn(int nCntPlayer);
	void CheckEnemySpawn(int nTime);
	void EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt);


	// �}�b�v��ς���p�֐�(�f�o�b�O�p�̊֐�����)
	void ResetCounter(void);
	void ChangeNextMap(void);
	void ChangePrevMap(void);
	void DeleteGameObject(void);
	void CalcStageDigits(void);


	// �X�N���v�g�ǂݍ��ݗp�֐�(�}�b�v�C�x���g�p�ɃX�N���v�g�����Ȃ炱���ɁI�I)
	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadModel(char *pStr, int nCntModel);
	void LoadMapFileName(char *pStr, int nCntMap);
	void LoadItemEvent(char *pStr);
	void LoadItemEventScript(CFileLoader *pFileLoader, char *pStr);
	void LoadItemEvent_Shake(CFileLoader *pFileLoader, char *pStr);
	void LoadEffectFileName(char *pStr);
	void LoadPlayerFileName(char *pStr, int nCntPlayer);
	void LoadEnemyFileName(char *pStr, int nCntEnemy);
	void LoadStageBg(CFileLoader *pFileLoader, char *pStr);
	void LoadStageLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadStageNumber(CFileLoader *pFileLoader, char *pStr);
	void LoadGameOverLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadHinamatsuriEvent(char *pStr);
	void LoadHinamatsuriEventScript(CFileLoader *pFileLoader, char *pStr);
	void LoadCherryBlossomsData(CFileLoader *pFileLoader, char *pStr);


	// �����o�ϐ�
	int m_nNumMap;                        // �ǂݍ��ރ}�b�v�̐�
	char **m_pMapFileName;                // �ǂݍ��ރ}�b�v�̃t�@�C����
	int m_nMapIdx;                        // ���݂̃}�b�v�̔ԍ�
	int m_nStageIdx;                      // ���݂̃X�e�[�W�ԍ�
	CUI *m_pUI;                           // UI�N���X�ւ̃|�C���^
	STATE m_State;                        // ����̏��
	STATE m_StateOld;                     // �O��̏��
	int m_nStateCounter;                  // ��Ԃ��Ǘ�����J�E���^�[
	int m_nGameCounter;                   // �Q�[���̎��Ԃ𐔂���J�E���^�[
	int m_nSpawnEnemyCount;               // �X�|�[�������G�̐��𐔂���J�E���^�[
	int m_nNumEnemy;                      // �o������G�̐�
	CScene2D *m_pGameOverLogo;            // �Q�[���I�[�o�[��m�点�郍�S�|���S��
	CScene2D *m_pStageBg;                 // �X�e�[�W�\����Ԃ̎��̔w�i�|���S��
	CScene2D *m_pStageLogo;               // �X�e�[�W���S�p�|���S��
	CNumber **m_pStageNumber;             // �X�e�[�W�ԍ��\���p�|���S��
	int m_nNumberTexIdx;                  // �����|���S�����g�p����e�N�X�`���̔ԍ�
	int m_nNumNumberDigits;               // ���݂̃X�e�[�W�ԍ��̌���
	int m_nBulletModelIdx;                // �e���g�p���郂�f���̔ԍ�
	int m_nItemModelIdx[CItem::TYPE_MAX]; // �A�C�e�����g�p���郂�f���̔ԍ�
	bool m_bEnemyMove;                    // �G�������邩�ǂ���
	int m_nEnemyMoveCounter;              // �G�������Ȃ���ԂɂȂ��Ă���̎��Ԃ𐔂���J�E���^�[
	HINAEVENT m_HinaEvent;                // �ЂȍՂ�}�b�v�̃C�x���g�𕪂���ϐ�
	int m_nEventCounter;                  // �C�x���g�J�E���^�[

	// �v���C���[�p
	CPlayer *m_pPlayer[MAX_NUM_PLAYER];
	CPlayerManager *m_pPlayerManager[MAX_NUM_PLAYER];
	int m_nPlayerStock[MAX_NUM_PLAYER];
	int m_nPlayerRespawnCounter;

	// �G�f�[�^�p
	CCharacterManager *m_pEnemyManager[CEnemy::TYPE_MAX];

	// �v���C���[�̓����ɕK�v
	bool m_bDeletePlayerFlag;

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

	typedef struct
	{// �A�C�e���C�x���g�f�[�^
		int nShakeTime;
		float fShake;
		float fShakeCut;
		int nStop;
	}ITEMEVENT_DATA;
	ITEMEVENT_DATA m_ItemEventData;

	typedef struct
	{// ���̉Ԃт�f�[�^
		int nTime;
		int nAppear;
		int nTexIdx;
		int nMoveXMax;
		int nMoveXMin;
		int nMoveYMax;
		int nMoveYMin;
		int nWidthMax;
		int nWidthMin;
		int nHeightMax;
		int nHeightMin;
		int nAngleSpeedMax;
		int nAngleSpeedMin;
		int nRotSpeedMax;
		int nRotSpeedMin;
	}CHERRYBLOSSOMS_DATA;
	CHERRYBLOSSOMS_DATA m_CherryBlossomsData;

	// �Q�[�������U���g�p�f�[�^
	// �w�i�p
	CScene2D *m_pGameResultBg;

	// �X�e�[�W�\���p
	CScene2D *m_pGameResultStage;
	CNumber **m_apGameResultStageNumber;

	// �n�C�X�R�A�p
	CNumber **m_apHighScore;
	int m_nNumHighScoreDigit;
	CScene2D *m_pHighScoreLogo;

	// �v���C���[�X�R�A�p
	int m_nScore[MAX_NUM_PLAYER];
	CNumber **m_apScore[MAX_NUM_PLAYER];
	int m_nScoreDigit[MAX_NUM_PLAYER];
	CScene2D *m_apPlayerLogo[MAX_NUM_PLAYER];

	// �G�l�~�[�X�R�A�p
	int m_nNumBreakEnemy[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	CNumber **m_apNumBreakEnemy[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	int m_nNumBreakEnemyDigit[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	CNumber **m_apEnemyScore[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	int m_nNumEnemyScoreDigit[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	CScene2D *m_apEnemyScorePointLogo[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	CScene2D *m_apEnemyScoreArrow[MAX_NUM_PLAYER][CEnemy::TYPE_MAX];
	CScene2D *m_apEnemyScoreIcon[CEnemy::TYPE_MAX];

	// �g�[�^���X�R�A�p
	int m_nNumAllBreakEnemy[MAX_NUM_PLAYER];
	CNumber **m_apNumAllBreakEnemy[MAX_NUM_PLAYER];
	int m_nNumAllBreakEnemyDigit[MAX_NUM_PLAYER];
	CScene2D *m_pTortalLogo;
	CScene2D *m_pTortalLine;

	// �{�[�i�X�X�R�A�p
	CNumber **m_apBonus;
	int m_nBonusDigit;
	CScene2D *m_pBonusPointLogo;
	CScene2D *m_pBonusScoreLogo;
};
#endif