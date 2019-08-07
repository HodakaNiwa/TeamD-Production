//=============================================================================
//
// �G�̏��� [enemy.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "scene.h"
#include "character.h"
#include "player.h"
#include "block.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemy : public CCharacter
{
public:	//�N������A�N�Z�X�\
	//*****************************************************************************
	// �\���̒�`
	//*****************************************************************************
	typedef enum
	{
		MOVE_UP = 0,
		MOVE_DOWN,
		MOVE_RIGHT,
		MOVE_LEFT,
		MOVE_MAX,
	}MOVE;

	//�����o�֐�
	CEnemy();
	~CEnemy();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(CScene *pScene);

	void SetEnemy(CEnemy *pEnemy);

	CEnemy *GetEnemy(void);

	//�ÓI�����o�֐�
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:	//�����������A�N�Z�X�\
	//�����o�֐�
	void Move(void);
	void Collision(void);

	//�����o�ϐ�
	CModel						*m_pModel;
	CEnemy						*m_pEnemy;
	int							m_nLife;
	MOVE						m_moveType;

	//�ÓI�����o�ϐ�
	static LPD3DXMESH			m_pMesh;					//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat;					//�}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat;					//�}�e���A�����̐�

};
#endif
