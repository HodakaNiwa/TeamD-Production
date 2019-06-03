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

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPlayer;

//*****************************************************************************
// �Q�[���N���X�̒�`
//*****************************************************************************
class CGame : public CBasemode
{
public:	//�N������A�N�Z�X�\
		//�����o�֐�
	CGame();
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CPlayer *GetPlayer(int nIdx);

	//�ÓI�����o�֐�
	static CGame *Create(void);

private:	//�����������A�N�Z�X�\
	void CreateCamera(void);
	void CreateMap(void);
	void Debug(void);

	CPlayer *m_pPlayer[2];
};
#endif