//=============================================================================
//
// �f���v���C�̏��� [demoplay.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _DEMOPLAY_H_
#define _DEMOPLAY_H_

#include "main.h"
#include "basemode.h"
#include "item.h"
#include "enemy.h"

//�O���錾
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
class CDemoplay : public CBasemode
{
public:	//�N������A�N�Z�X�\
	//�����o�֐�
	CDemoplay();
	~CDemoplay();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nItemType);

	// �A�C�e���̏������s�p�֐�
	void ItemEvent_Star(int nPlayerNumber);
	void ItemEvent_Grenade(void);
	void ItemEvent_1Up(int nPlayerNumber);
	void ItemEvent_Scoop(void);
	void ItemEvent_Clock(void);
	void ItemEvent_Helmet(int nPlayerNumber);

	void SetBulletModel(CBullet *pBullet);

	//�ÓI�����o�֐�
	static CDemoplay *Create(void);

private:	//�����������A�N�Z�X�\
	void ClearVariable(void);

	void CreateCamera(void);
	void CreateMap(void);
	void CreateUI(void);
	void CreatePlayer(void);
	CEnemy *CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData);

	void ReleasePlayer(void);
	void ReleasePlayerManager(void);
	void ReleaseEnemyManager(void);

	void CheckEnemySpawn(int nTime);
	void EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void CreateTextureManager(int nNumTex);
	void LoadTexture(char *pFileName, int nCntTex);
	void LoadModel(char *pStr, int nCntModel);
	void LoadItemEvent(char *pStr);
	void LoadItemEventScript(CFileLoader *pFileLoader, char *pStr);
	void LoadEffectFileName(char *pStr);
	void LoadPlayerFileName(char *pStr, int nCntPlayer);
	void LoadEnemyFileName(char *pStr, int nCntEnemy);
	void LoadMapFileName(char *pStr);

	char m_aMapFileName[256];             // �ǂݍ��ރ}�b�v�̃t�@�C����
	CUI *m_pUI;                           // UI�N���X�ւ̃|�C���^
	int m_nGameCounter;                   // �Q�[���̎��Ԃ𐔂���J�E���^�[
	int m_nSpawnEnemyCount;               // �X�|�[�������G�̐��𐔂���J�E���^�[
	int m_nNumberTexIdx;                  // �����|���S�����g�p����e�N�X�`���̔ԍ�
	int m_nBulletModelIdx;                // �e���g�p���郂�f���̔ԍ�
	int m_nItemModelIdx[CItem::TYPE_MAX]; // �A�C�e�����g�p���郂�f���̔ԍ�
	bool m_bEnemyMove;                    // �G�������邩�ǂ���
	int m_nEnemyMoveCounter;              // �G�������Ȃ���ԂɂȂ��Ă���̎��Ԃ𐔂���J�E���^�[

	// �v���C���[�p
	CPlayer *m_pPlayer;
	CPlayerManager *m_pPlayerManager;
	int m_nPlayerStock;
	int m_nPlayerRespawnCounter;

	// �G�f�[�^�p
	CCharacterManager *m_pEnemyManager[CEnemy::TYPE_MAX];

	typedef struct
	{// �A�C�e���C�x���g�f�[�^
		int nStop;
	}ITEMEVENT_DATA;
	ITEMEVENT_DATA m_ItemEventData;
};
#endif