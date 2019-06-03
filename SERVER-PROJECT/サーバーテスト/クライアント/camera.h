//=============================================================================
//
// �J�������� [camera.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �J�����N���X�̒�`
//*****************************************************************************
class CCamera
{
public:	//�N������A�N�Z�X�\
	//�����o�֐�
	CCamera();
	~CCamera();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Reset(void);
	void SetCamera(void);

	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetRot(void);
	float GetLength(void);
	bool GetMouseMove(void);
	bool GetChengeLength(void);

	void SetPosV(D3DXVECTOR3 posV);
	void SetPosR(D3DXVECTOR3 posR);
	void SetRot(D3DXVECTOR3 Rot);
	void SetLength(float fLength);
	void SetMouseMove(bool bMouseMove);
	void SetChengeLength(bool bChengeLength);

	// �ÓI�����o�֐�
	static CCamera *Create(void);

private://�����������A�N�Z�X�\
	D3DXVECTOR3 m_posV;				// ���_
	D3DXVECTOR3 m_posR;				// �����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXMATRIX	m_mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	m_mtxView;			// �r���[�}�g���b�N�X
	D3DXVECTOR3 m_rot;				// ����
	D3DXVECTOR3 m_rotDest;			// �ړI�̌���
	D3DXVECTOR3	m_rotDiff;          // ���݂̌����ƖړI�̌����̍���
	float		m_fLength;			// ����
	bool        m_bMouseMove;       // �}�E�X�œ������邩�ǂ���
	bool        m_bChengeLength;    // ������ς����邩�ǂ���
};

//*****************************************************************************
// �G�f�B�^�[�p�J�����N���X�̒�`
//*****************************************************************************
class CEditorCamera : public CCamera
{
public:	//�N������A�N�Z�X�\
	//�����o�֐�
	CEditorCamera();
	~CEditorCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Reset(void);

	// �ÓI�����o�֐�
	static CEditorCamera *Create(void);

private://�����������A�N�Z�X�\
	void MouseMove(void);
};

#endif
