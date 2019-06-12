//=============================================================================
//
// �x�[�X���[�h�̏���[basemode.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _BASEMODE_H_
#define _BASEMODE_H_

#include "main.h"

//�O���錾
class CMap;
class CCameraManager;
class CModelCreate;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBasemode
{
public:	//�N������A�N�Z�X�\
		//�����o�֐�
	CBasemode();
	~CBasemode();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetMap(CMap *pMap);
	void SetCameraManager(CCameraManager *pCameraManager);
	void SetModelCreate(CModelCreate *pModelCreate);

	CMap *GetMap(void);
	CCameraManager *GetCameraManager(void);
	CModelCreate *GetModelCreate(void);

private:	// �����������A�N�Z�X�\
	CMap *m_pMap;			            // �}�b�v�̃|�C���^
	CCameraManager *m_pCameraManager;   // �J�����Ǌ��N���X�ւ̃|�C���^
	CModelCreate   *m_pModelCreate;     // ���f���Ǌ��N���X�ւ̃|�C���^
};
#endif