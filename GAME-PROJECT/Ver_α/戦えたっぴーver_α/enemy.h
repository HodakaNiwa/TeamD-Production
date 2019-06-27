//=============================================================================
//
// �G�̏��� [enemy.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
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
	void SetIdx(const int nIdx);

	CEnemy *GetEnemy(void);
	int GetIdx(void);

	//�ÓI�����o�֐�
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static int GetNumAll(void);
	static int GetSpawnCounter(void);
	static void ResetSpawnCounter(void);

private:	//�����������A�N�Z�X�\
	static int m_nNumAll;
	static int m_nSpawnCounter;

	//�����o�֐�
	void Move(void);
	void SetBullet(void);
	void Collision(void);

	//�����o�ϐ�
	CModel						*m_pModel;
	CEnemy						*m_pEnemy;
	int							m_nLife;
	MOVE						m_moveType;
	int                         m_nIdx;

	//�ÓI�����o�ϐ�
	static LPD3DXMESH			m_pMesh;					//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat;					//�}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat;					//�}�e���A�����̐�

};
#endif
