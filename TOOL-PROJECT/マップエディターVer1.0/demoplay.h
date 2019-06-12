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
	void CreateCamera(void);
	void CreateMap(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadMapFileName(char *pStr);
	void SaveSystem(void);
	void SaveMapFileName(CFileSaver *pFileSaver);

	char m_aMapFileName[256];     // �ǂݍ��ރ}�b�v�̃t�@�C����
};
#endif