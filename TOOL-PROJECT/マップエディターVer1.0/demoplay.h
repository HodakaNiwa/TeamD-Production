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

//�O���錾
class CFileLoader;
class CFileSaver;
class CUI;
class CMap;
class CEnemy_ListData;

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

	//�ÓI�����o�֐�
	static CDemoplay *Create(void);

private:	//�����������A�N�Z�X�\
	void ClearVariable(void);

	void CreateCamera(void);
	void CreateMap(void);
	void CreateUI(void);

	void CheckEnemySpawn(int nTime);
	void EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void CreateTextureManager(int nNumTex);
	void LoadTexture(char *pFileName, int nCntTex);
	void LoadEffectFileName(char *pStr);
	void LoadMapFileName(char *pStr);
	void SaveSystem(void);
	void SaveMapFileName(CFileSaver *pFileSaver);

	char m_aMapFileName[256];             // �ǂݍ��ރ}�b�v�̃t�@�C����
	CUI *m_pUI;                           // UI�N���X�ւ̃|�C���^
	int m_nGameCounter;                   // �Q�[���̎��Ԃ𐔂���J�E���^�[
	int m_nSpawnEnemyCount;               // �X�|�[�������G�̐��𐔂���J�E���^�[
};
#endif