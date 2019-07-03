//=============================================================================
//
// ���U���g�̏��� [result.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"
#include "basemode.h"

//�O���錾
class CFileLoader;
class CFileSaver;
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CResult : public CBasemode
{
public:	// �N������A�N�Z�X�\
	//--------------
	// ���
	//--------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_WAIT,
		STATE_NORMAL,
		STATE_END,
		STATE_MAX
	}STATE;

	// �����o�֐�
	CResult();
	~CResult();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	STATE GetState(void);

	// �ÓI�����o�֐�
	static CResult *Create(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:	// �����������A�N�Z�X�\
	void ClearVariable(void);

	void CreateTexture(int nNumTex);
	void CreateCamera(void);
	void CreateLogo(void);

	void ReleaseLogo(void);

	void WaitUpdate(void);
	void NormalUpdate(void);
	void EndUpdate(void);
	void WaitInputToNextMode(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexFileName(char *pStr, int nCntTex);
	void LoadLogo(CFileLoader *pFileLoader, char *pStr);

	STATE m_State;        // ���
	int m_nStateCounter;  // ��Ԃ��Ǘ�����J�E���^�[
	CScene2D *m_pLogo;    // ���U���g���S�\���p�|���S��


	// �e��N���X�����p���
	typedef struct
	{// ���U���g���S�f�[�^
		int nTexIdx;
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
		float fWidth;
		float fHeight;
	}RESULTLOGO_DATA;
	RESULTLOGO_DATA m_ResultLogoData;
};
#endif