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
};
#endif