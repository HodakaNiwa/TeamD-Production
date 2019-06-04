//=============================================================================
//
// �e�̏��� [bullet.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

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
class CBullet : public CObject3D
{
public:	//�N������A�N�Z�X�\
	//=========================================================================
	// �\���̂̒�`
	//=========================================================================
	typedef enum
	{
		BULLET_TYPE_NONE = 0,
		BULLET_TYPE_PLAYER,
		BULLET_TYPE_ENEMY,
		BULLET_TYPE_MAX,
	} BULLET_TYPE;

	//�����o�֐�
	CBullet();
	~CBullet();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosOld(D3DXVECTOR3 posOld);
	void SetMove(D3DXVECTOR3 move);
	void SetBulletType(BULLET_TYPE type);
	void SetScene(CScene *pScene);

	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetMove(void);
	BULLET_TYPE GetBulletType(void);
	CScene *GetScene(void);

	//�ÓI�����o�֐�
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 move, BULLET_TYPE type,CScene *pScene);

private:	//�����������A�N�Z�X�\
	//�����o�֐�
	void Move(void);
	void Collision(void);
	void Destroy(void);

	//�����o�ϐ�
	CModel						*m_pModel;					//���f���ւ̃|�C���^
	D3DXVECTOR3					m_posOld;					//�ߋ��̈ʒu
	D3DXVECTOR3					m_move;						//�ړ���
	BULLET_TYPE					m_type;						//�e�̎��
	CScene						*m_pScene;					//�I�u�W�F�N�g�ւ̃|�C���^���

	//�ÓI�����o�ϐ�
	static LPD3DXMESH			m_pMesh;					//���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat;					//�}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat;					//�}�e���A�����̐�

};
#endif
