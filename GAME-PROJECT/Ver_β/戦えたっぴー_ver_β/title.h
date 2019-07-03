//=============================================================================
//
// �^�C�g���̏��� [title.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "basemode.h"
#include "scene2D.h"

//�O���錾
class CFileLoader;
class CFileSaver;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TITLEBG_NUM (2)    // �^�C�g���p�w�i�̐�

//*****************************************************************************
// �^�C�g���p���S�N���X�̒�`
//*****************************************************************************
class CTitleLogo : public CScene2D
{
public:    // �N�ł��A�N�Z�X�\
	CTitleLogo(int nPriority = 3, OBJTYPE objtype = OBJTYPE_SCENE2D);
	~CTitleLogo();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	static CTitleLogo *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
// �^�C�g���N���X�̒�`
//*****************************************************************************
class CTitle : public CBasemode
{
public:	// �N������A�N�Z�X�\
	//--------------
	// ���
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_TEAMLOGO,
		STATE_FADEOUT,
		STATE_FADEIN,
		STATE_NORMAL,
		STATE_MAX
	}STATE;

	// �����o�֐�
	CTitle();
	~CTitle();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	STATE GetState(void);

	// �ÓI�����o�֐�
	static CTitle *Create(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
	void ClearVariable(void);
	void ClearPress(void);

	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateFadePolygon(void);
	void CreateTeamBgPolygon(void);
	void CreatePress(void);

	void ReleaseFadePolygon(void);
	void ReleaseTeamLogoPolygon(void);
	void ReleaseTeamBgPolygon(void);
	void ReleaseBgPolygon(void);
	void ReleaseTitleLogoPolygon(void);
	void ReleasePress(void);

	void SlideBgUV(void);
	void TeamLogoUpdate(void);
	void FadeOutUpdate(void);
	void FadeInUpdate(void);
	void NormalUpdate(void);
	void WaitInputToNormal(void);
	void WaitInputToNextMode(void);
	void SetNextMode_None(void);
	void SetNextMode_Push(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadBgPolygon(CFileLoader *pFileLoader, char *pStr);
	void LoadTitleLogo(CFileLoader *pFileLoader, char *pStr);
	void LoadPress(CFileLoader *pFileLoader, char *pStr);
	void LoadTeamLogo(CFileLoader *pFileLoader, char *pStr);

	STATE m_State;                     // ���
	CScene2D *m_pFade;                 // �t�F�[�h�Ɏg�p����|���S��
	CScene2D *m_pTeamLogo;             // �`�[�����S�p�|���S���N���X�^�̃|�C���^
	CScene2D *m_pTeamBg;               // �`�[�����S�w�i�p�|���S���N���X�^�̃|�C���^
	CScene2D *m_pBg;                   // �w�i�p�|���S���N���X�^�̃|�C���^
	CTitleLogo *m_pTitleLogo;          // �^�C�g�����S�N���X�^�̃|�C���^
	CScene2DPress *m_pPress;           // �v���X�ҋ@�|���S���N���X�^�̃|�C���^
	int m_nPressTexIdx;                // �v���X�ҋ@�|���S���̎g�p����e�N�X�`���ԍ�
	D3DXVECTOR3 m_PressPos;            // �v���X�ҋ@�|���S���̍��W
	D3DXCOLOR m_PressCol;              // �v���X�ҋ@�|���S���̐F
	float m_fPressWidth;               // �v���X�ҋ@�|���S���̕�
	float m_fPressHeight;              // �v���X�ҋ@�|���S���̍���
	int m_nStateCounter;               // ��Ԃ��Ǘ�����J�E���^�[
};
#endif