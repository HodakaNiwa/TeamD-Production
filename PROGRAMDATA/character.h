//=============================================================================
//
// �L�����N�^�[�̏��� [character.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _CHARCTER_H_
#define _CHARCTER_H_

#include "main.h"
#include "scene.h"
#include "object3D.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCharacter : public CObject3D
{
public:	//�N������A�N�Z�X�\
	typedef enum
	{
		ROT_UP = 0,
		ROT_DOWN,
		ROT_RIGHT,
		ROT_LEFT,
		ROT_MAX,
	}NOW_ROT_INFO;

	//�����o�֐�
	CCharacter(int nPriority = 3, OBJTYPE objtype = OBJTYPE_CHARACTER);
	~CCharacter();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosOld(D3DXVECTOR3 posOld);
	void SetMove(D3DXVECTOR3 move);
	void SetShoot(bool bHit);
	void SetNowRotInfo(NOW_ROT_INFO nowRotInfo);

	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetMove(void);
	bool GetShoot(void);
	NOW_ROT_INFO GetNowRotInfo(void);

private:	//�����������A�N�Z�X�\
	//�����o�ϐ�
	D3DXMATRIX					m_mtxWorld;							// ���[���h�}�g���b�N�X
	CModel						*m_pModel;							//���f�����ւ̃|�C���^
	D3DXVECTOR3					m_posOld;							// �ߋ��̈ʒu
	D3DXVECTOR3					m_move;								// �ړ���
	bool						m_bShoot;							// �����Ă��邩�ǂ���
	NOW_ROT_INFO				m_nowRotInfo;							// ���݂̌������
	//�ÓI�����o�ϐ�
};
#endif