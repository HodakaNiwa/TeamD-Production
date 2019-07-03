//=============================================================================
//
// �`���[�g���A���̏��� [tutorial.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "basemode.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CFileLoader;
class CFileSaver;
class CScene2D;
class CScene2DFlash;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTutorial : public CBasemode
{
public:	// �N������A�N�Z�X�\
	//--------------
	// �X�L�b�v�@�\
	//--------------
	typedef enum
	{
		SKIP_NONE = -1,
		SKIP_YES,
		SKIP_NO,
		SKIP_MAX
	}SKIP;

	//--------------
	// ���
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_SKIP_CHECK,
		STATE_NORMAL,
		STATE_GOAL,
		STATE_MAP_CHANGE,
		STATE_END,
		STATE_MAX
	}STATE;

	// �����o�֐�
	CTutorial();
	~CTutorial();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	STATE GetState(void);

	// �ÓI�����o�֐�
	static CTutorial *Create(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
	void ClearVariable(void);

	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateMapFilePointer(void);

	void ReleaseMapFilePointer(void);
	void ReleaseSkipLogo(void);
	void ReleaseSkipCheckLogo(void);

	void SkipCheckUpdate(void);
	void NormalUpdate(void);
	void GoalUpdate(void);
	void MapChangeUpdate(void);
	void EndUpdate(void);
	void ChangeNoneSkipCheckPolygon(int nSelect);
	void ChangeSelectSkipCheckPolygon(int nSelect);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadMapFileName(char *pStr, int nCntMap);
	void LoadSkipLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadSkipCheckLogo(CFileLoader *pFileLoader, char *pStr, int nCntSkipCheck);

	STATE m_State;                              // ���
	int m_nStateCounter;                        // ��Ԃ��Ǘ�����J�E���^�[
	int m_nNumMap;                              // �}�b�v��ǂݍ��ސ�
	char **m_pMapFileName;                      // �ǂݍ��ރ}�b�v�̃t�@�C����
	CScene2D *m_pSkipLogo;                      // �X�L�b�v���邩��\�����郍�S�|���S��
	CScene2DFlash *m_pSkipCheckLogo[SKIP_MAX];  // �X�L�b�v�m�F���� �͂� ������ ��\������p�̃��S�|���S��
	int m_nSelectSkip;                          // �X�L�b�v���邩�ǂ���
};
#endif