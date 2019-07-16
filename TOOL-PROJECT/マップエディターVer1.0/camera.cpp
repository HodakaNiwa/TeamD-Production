//=============================================================================
//
// �J�������� [camera.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_ANGEL_VIEW      (45.0f)                           // �J�����̉�p
#define CAMERA_ANGLE_SPEED     (0.01f)                           // �J�������}�E�X�ŉ�]������X�s�[�h�{��
#define CAMERA_MOVE_SPEED      (1.3f)                            // �J�������}�E�X�ňړ�������X�s�[�h�{��
#define CAMERA_LENGTH_MIN      (100.0f)                          // ���_�����_�̋����̍ŏ��l
#define CAMERA_LENGTH_MAX      (2000.0f)                         // ���_�����_�̋����̍ő�l

#define CAMERA_LENGTH_INI      (1700.0f)                         // ���_�����_�Ԃ̋���(�����l)
#define CAMERA_ROT_INI         (D3DXVECTOR3(-1.2f,0.0f,0.0f))    // �J�����̌���(�����l)
#define TOPVIEW_CAMERA_ROT_INI (D3DXVECTOR3(-1.57f,0.0f,0.0f))   // �g�b�v�r���[�J�����̌���(�����l)
#define CAMERA_MOVE            (3.0f)                            // �J�����̓����X�s�[�h
#define CAMERA_POS_X           (175.0f)

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************

//*****************************************************************************
// CCamera�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// ��������
//=============================================================================
CCamera *CCamera::Create(void)
{
	CCamera *pCamera = NULL;

	if (pCamera == NULL)
	{
		pCamera = new CCamera;
		pCamera->Init();
	}
	return pCamera;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCamera::Init(void)
{
	// �J�����̏�������ݒ�
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);          // �J�����̏�����x�N�g��
	m_posR = D3DXVECTOR3(CAMERA_POS_X, 0.0f, 0.0f);  // �J�����̒����_
	m_rot = CAMERA_ROT_INI;                          // �J�����̌��݂̌���
	m_rotDest = m_rot;                               // �J�����̖ړI�̌���
	m_rotDiff = INITIALIZE_D3DXVECTOR3;              // ���݂̌����ƖړI�̌����̍���
	m_fLength = CAMERA_LENGTH_INI;                   // ���_�����_�Ԃ̋���
	m_bMouseMove = true;                             // �}�E�X�œ������邩�ǂ���
	m_bChengeLength = true;                          // ���_�����_�Ԃ̋�����ύX�ł��邩�ǂ���

	// �J�����̎��_�̈ʒu���v�Z
	m_posV = D3DXVECTOR3(CAMERA_POS_X, 0.0f, 0.0f);
	m_posV.y = m_posR.y - sinf(m_rot.x) * m_fLength;
	m_posV.z = m_posR.z - cosf(m_rot.x) * m_fLength;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
#ifdef _DEBUG
	//�����_�̕\��
	CDebugProc::Print(1, "���_�̈ړ�    : x[%.1f],y[%.1f],z[%.1f]\n", m_posV.x, m_posV.y, m_posV.z);
	//���_�̕\��
	CDebugProc::Print(1, "�����_�̈ړ�  : x[%.1f],y[%.1f],z[%.1f]\n", m_posR.x, m_posR.y, m_posR.z);
#endif
}

//=============================================================================
// �J�����̃��Z�b�g����
//=============================================================================
void CCamera::Reset(void)
{
	m_rot = CAMERA_ROT_INI;
	m_fLength = CAMERA_LENGTH_INI;

	// ���_�����_�̈ʒu��ݒ�
	m_posR = INITIALIZE_D3DXVECTOR3;
	m_posV.y = m_posR.y - sinf(m_rot.x) * m_fLength;
	m_posV.z = m_posR.z - cosf(m_rot.x) * m_fLength;
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void CCamera::SetCamera(void)
{
	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),						//��p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//��ʔ䗦
		10.0f,										//��O
		20000.0f);									//���s��

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&m_mtxView,
				       &m_posV,
					   &m_posR,
					   &m_vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}
//=============================================================================
// �J�����̎��_�擾
//=============================================================================
D3DXVECTOR3 CCamera::GetPosV(void)
{
	return m_posV;
}

//=============================================================================
// �J�����̒����_�擾
//=============================================================================
D3DXVECTOR3 CCamera::GetPosR(void)
{
	return m_posR;
}

//=============================================================================
// �J�����̌����擾
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// �J�����̒����擾
//=============================================================================
float CCamera::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
// �J�������}�E�X�œ������邩�ǂ����擾
//=============================================================================
bool CCamera::GetMouseMove(void)
{
	return m_bMouseMove;
}

//=============================================================================
// �J�����̎��_�����_�Ԃ̋�����ύX�ł��邩�ǂ����擾
//=============================================================================
bool CCamera::GetChengeLength(void)
{
	return m_bChengeLength;
}

//=============================================================================
// �J�����̎��_�ݒu����
//=============================================================================
void CCamera::SetPosV(D3DXVECTOR3 posV)
{
	m_posV = posV;
}

//=============================================================================
// �J�����̒����_�ݒu����
//=============================================================================
void CCamera::SetPosR(D3DXVECTOR3 posR)
{
	m_posR = posR;
}

//=============================================================================
// �J�����̌����ݒ菈��
//=============================================================================
void CCamera::SetRot(D3DXVECTOR3 Rot)
{
	m_rot = Rot;
}

//=============================================================================
// �J�����̎��_�����_�Ԃ̋����ݒ菈��
//=============================================================================
void CCamera::SetLength(float fLength)
{
	m_fLength = fLength;
}

//=============================================================================
// �J�������}�E�X�œ������邩�ǂ����ݒ�
//=============================================================================
void CCamera::SetMouseMove(bool bMouseMove)
{
	m_bMouseMove = bMouseMove;
}

//=============================================================================
// �J�����̎��_�����_�Ԃ̋�����ύX�ł��邩�ǂ����ݒ�
//=============================================================================
void CCamera::SetChengeLength(bool bChengeLength)
{
	m_bChengeLength = bChengeLength;
}


//*****************************************************************************
// CEditorCamera�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEditorCamera::CEditorCamera()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEditorCamera::~CEditorCamera()
{

}

//=============================================================================
// ��������
//=============================================================================
CEditorCamera *CEditorCamera::Create(void)
{
	CEditorCamera *pEditCamera = NULL;

	if (pEditCamera == NULL)
	{
		pEditCamera = new CEditorCamera;
		pEditCamera->Init();
	}
	return pEditCamera;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEditorCamera::Init(void)
{
	CCamera::Init();
	Reset();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEditorCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEditorCamera::Update(void)
{
	// �}�E�X�̃J��������
	if (GetMouseMove() == true)
	{
		MouseMove();
	}
}

//=============================================================================
// �G�f�B�b�g�J�����̃��Z�b�g����
//=============================================================================
void CEditorCamera::Reset(void)
{
	// �l�̎擾
	float fLength = GetLength();
	D3DXVECTOR3 Rot = GetRot();
	D3DXVECTOR3 PosV = GetPosV();
	D3DXVECTOR3 PosR = GetPosR();

	Rot = CAMERA_ROT_INI;
	fLength = CAMERA_LENGTH_INI;

	// ���_�����_�̈ʒu��ݒ�
	PosR = INITIALIZE_D3DXVECTOR3;
	PosV.x = PosR.x - sinf(Rot.y) * fLength;
	PosV.y = PosR.y + cosf(Rot.z) * fLength;
	PosV.z = PosR.z - cosf(Rot.y) * fLength;

	// �l�̐ݒ�
	SetLength(fLength);
	SetRot(Rot);
	SetPosV(PosV);
	SetPosR(PosR);
}

//=============================================================================
// �}�E�X�̃J�������쏈��
//=============================================================================
void CEditorCamera::MouseMove(void)
{
	CInputMouse *pMouse = CManager::GetMouse();
	float fLength = GetLength();
	D3DXVECTOR3 Rot = GetRot();
	D3DXVECTOR3 PosV = GetPosV();
	D3DXVECTOR3 PosR = GetPosR();
	if (pMouse != NULL)
	{// �}�E�X���擾�ł���
		if (pMouse->GetMouseAxisZ() != 0 && GetChengeLength() == true)
		{// �}�E�X�̃z�C�[���{�^�����X�N���[�����ꂽ
		    fLength += 0.1f * pMouse->GetMouseAxisZ();
			if (fLength < CAMERA_LENGTH_MIN)
			{// ���������̒l���߂��Ȃ���
				fLength = CAMERA_LENGTH_MIN;
			}
			else if (fLength > CAMERA_LENGTH_MAX)
			{// ���������̒l��艓���Ȃ���
				fLength = CAMERA_LENGTH_MAX;
			}

			// ���_�̌v�Z
			PosV.x = PosR.x - sinf(Rot.y) * fLength;
			PosV.y = PosR.y + cosf(Rot.z) * fLength;
			PosV.z = PosR.z - cosf(Rot.y) * fLength;
		}

		if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
		{// �}�E�X�̍��{�^���ƉE�{�^���������ɉ����ꂽ
			if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
			{// �}�E�X����������Ă���
			 // �����ɉ����ꂽ�Ƃ��̈ʒu���獡�̃}�E�X�J�[�\���̈ʒu���王�_�����_���ړ�������������v�Z
				float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
				fAngle -= Rot.y;
				if (fAngle < -D3DX_PI)
				{// �������~�����𒴂���
					fAngle += D3DX_PI * 2.0f;
				}
				if (fAngle > D3DX_PI)
				{// �������~�����𒴂���
					fAngle -= D3DX_PI * 2.0f;
				}

				// �}�E�X��X��,Y������ړ��ʂ��v�Z
				float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
				fMovement *= CAMERA_MOVE_SPEED;

				// �����_���ړ�������
				PosR.x += sinf(fAngle) * fMovement;
				PosR.z -= cosf(fAngle) * fMovement;

				// ���_���ړ�������
				PosV.x = PosR.x - sinf(Rot.y) * fLength;
				PosV.z = PosR.z - cosf(Rot.y) * fLength;
			}
		}
		else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && CManager::GetKeyboard()->GetPress(DIK_LALT) == false)
		{// �}�E�X�̍��N���b�N�݂̂������ꂽ
		    // �������}�E�X�̑��x�ɍ��킹�ĉ�]������
		    // �c��
			Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
			if (Rot.y > D3DX_PI)
			{// �������~�����𒴂���
				Rot.y -= D3DX_PI * 2.0f;
			}
			if (Rot.y < -D3DX_PI)
			{// �������~�����𒴂���
				Rot.y += D3DX_PI * 2.0f;
			}

			// ����
			Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
			if (Rot.z < -D3DX_PI)
			{// �������~�����𒴂���
				Rot.z = -D3DX_PI;
			}
			if (Rot.z > 0.0f)
			{// �������~�����𒴂���
				Rot.z = 0.0f;
			}

			// ���_�̈ʒu���v�Z����
			PosV.x = PosR.x - sinf(Rot.y) * fLength;
			PosV.y = PosR.y + cosf(Rot.z) * fLength;
			PosV.z = PosR.z - cosf(Rot.y) * fLength;
		}
		else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
		{// �}�E�X�̉E�N���b�N�݂̂������ꂽ
		    // �������}�E�X�̑��x�ɍ��킹�ĉ�]������
		    // �c��
			Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
			if (Rot.y > D3DX_PI)
			{// �������~�����𒴂���
				Rot.y -= D3DX_PI * 2.0f;
			}
			if (Rot.y < -D3DX_PI)
			{// �������~�����𒴂���
				Rot.y += D3DX_PI * 2.0f;
			}

			// ����
			Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
			if (Rot.z < -D3DX_PI)
			{// �������~�����𒴂���
				Rot.z = -D3DX_PI;
			}
			if (Rot.z > 0.0f)
			{// �������~�����𒴂���
				Rot.z = 0.0f;
			}

			// �����_�̈ʒu���v�Z����
			PosR.x = PosV.x + sinf(Rot.y) * fLength;
			PosR.y = PosV.y - cosf(Rot.z) * fLength;
			PosR.z = PosV.z + cosf(Rot.y) * fLength;
		}
	}

	// �l�̐ݒ�
	SetLength(fLength);
	SetRot(Rot);
	SetPosV(PosV);
	SetPosR(PosR);
}

//*****************************************************************************
// CTopViewCamera�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTopViewCamera::CTopViewCamera()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTopViewCamera::~CTopViewCamera()
{

}

//=============================================================================
// ��������
//=============================================================================
CTopViewCamera *CTopViewCamera::Create(void)
{
	CTopViewCamera *pTopViewCamera = NULL;

	if (pTopViewCamera == NULL)
	{
		pTopViewCamera = new CTopViewCamera;
		pTopViewCamera->Init();
	}
	return pTopViewCamera;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTopViewCamera::Init(void)
{
	CCamera::Init();
	Reset();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTopViewCamera::Uninit(void)
{
	CCamera::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTopViewCamera::Update(void)
{

}

//=============================================================================
// ���Z�b�g����
//=============================================================================
void CTopViewCamera::Reset(void)
{
		// �l�̎擾
	float fLength = GetLength();
	D3DXVECTOR3 Rot = GetRot();
	D3DXVECTOR3 PosV = GetPosV();
	D3DXVECTOR3 PosR = GetPosR();

	Rot = TOPVIEW_CAMERA_ROT_INI;
	fLength = CAMERA_LENGTH_INI;

	// ���_�����_�̈ʒu��ݒ�
	PosR = INITIALIZE_D3DXVECTOR3 + D3DXVECTOR3(CAMERA_POS_X,0.0f,1.0f);
	PosV.x = CAMERA_POS_X;
	PosV.y = PosR.y - sinf(Rot.x) * fLength;
	PosV.z = PosR.z - cosf(Rot.x) * fLength;

	// �l�̐ݒ�
	SetLength(fLength);
	SetRot(Rot);
	SetPosV(PosV);
	SetPosR(PosR);
}