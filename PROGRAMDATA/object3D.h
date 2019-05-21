//=============================================================================
//
// �I�u�W�F�N�g3D���� [object3D.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBoxCollider;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CObject3D : public CScene
{
public:	//�N������A�N�Z�X�\
	//=========================================================================
	// �\���̂̒�`
	//=========================================================================
	typedef enum
	{
		OBJ3DTYPE_NONE = 0,
		OBJ3DTYPE_MAX,
	} OBJ3DTYPE;

	//�����o�֐�
	CObject3D(int nPriority = 3,OBJTYPE type = OBJTYPE_3D);
	~CObject3D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot);
	void SetBoxCollider(CBoxCollider *pBoxCollider);
	void SetMtxWorld(LPDIRECT3DDEVICE9 pDevice);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	CBoxCollider *GetBoxCollider(void);

	virtual	void Hit(CScene *pScene);

	//�ÓI�����o�֐�
	static CObject3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:	//�����������A�N�Z�X�\
	//�����o�ϐ�
	D3DXVECTOR3					m_pos;								// �ʒu
	D3DXVECTOR3					m_rot;								// ����

	D3DXMATRIX					m_mtxWorld;							// �}�g���b�N�X
	CBoxCollider				*m_pBoxCollider;					// �����蔻�蔠�ւ̃|�C���^
};
#endif
