//=============================================================================
//
// �v���C���[�̏��� [player.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"
#include "character.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CCharacter
{
public:	//�N������A�N�Z�X�\
		//�����o�֐�
	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPlayer(CPlayer *pPlayer);

	CPlayer *GetPlayer(void);

	//�ÓI�����o�֐�
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:	//�����������A�N�Z�X�\
	//�����o�֐�
	void Move(void);
	void Collision(void);

	//�����o�ϐ�
	CModel						*m_pModel;
	CPlayer						*m_pPlayer;
	//�ÓI�����o�ϐ�
	static LPD3DXMESH			m_pMesh;						//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat;					//�}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat;					//�}�e���A�����̐�

};
#endif
