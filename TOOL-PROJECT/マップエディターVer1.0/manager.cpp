//=============================================================================
//
// �}�l�[�W������ [manager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "manager.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "basemode.h"
#include "editor.h"
#include "demoplay.h"
#include "fade.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CManager::MODE	CManager::m_mode = MODE::MODE_EDITOR;		//���[�h�̏��
CRenderer		*CManager::m_pRenderer = NULL;				//�����_�����O�|�C���^
CInputKeyboard	*CManager::m_pInputKeyboard = NULL;			//�L�[�{�[�h�|�C���^
CInputMouse     *CManager::m_pMouse = NULL;                 // �}�E�X�N���X�^�̃|�C���^
CInputJoyStick	*CManager::m_pInputJoypad = NULL;			//�W���C�p�b�h�|�C���^
CSound			*CManager::m_pSound = NULL;					//�T�E���h�̃|�C���^
CDebugProc		*CManager::m_pDebugproc = NULL;				//�f�o�b�O�̃|�C���^
CBasemode		*CManager::m_pBasemode = NULL;				//�x�[�X���[�h�̃|�C���^
CFade			*CManager::m_pFade = NULL;					//�t�F�[�h�̃|�C���^

//=============================================================================
// �}�l�[�W���̃R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// �}�l�[�W���̃f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// �}�l�[�W���̏���������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//�����_�����O�̓��I�m��
	if (m_pRenderer == NULL)
	{
		m_pRenderer = new CRenderer;			//�����_�����O�̓��I�m��
	}

	//�L�[�{�[�h�̓��I�m��
	if (m_pInputKeyboard == NULL)
	{
		m_pInputKeyboard = new CInputKeyboard;	//�L�[�{�[�h�̓��I�m��
	}

	// �}�E�X�̍쐬
	if (m_pMouse == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pMouse = new CInputMouse;
		if (m_pMouse != NULL)
		{// ���������m�ۂł���
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	// �W���C�X�e�B�b�N�̍쐬
	if (m_pInputJoypad == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pInputJoypad = new CInputJoyStick(1);
		if (m_pInputJoypad != NULL)
		{// ���������m�ۂł���
			m_pInputJoypad->Init(hInstance, hWnd);
		}
	}

	//�T�E���h�̓��I�m��
	//if (m_pSound == NULL)
	//{
	//	m_pSound = new CSound;					//�T�E���h�̓��I�m��
	//}

	//�f�o�b�O�̓��I�m��
	if (m_pDebugproc == NULL)
	{
		m_pDebugproc = new CDebugProc;
	}

	//�����_�����O�̏���������
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Init(hWnd, TRUE);
	}

	//�L�[�{�[�h�̏���������
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Init(hInstance,hWnd);
	}

	//�T�E���h�̏���������
	/*if (m_pSound != NULL)
	{
		m_pSound->InitSound(hWnd);
	}*/

	//�f�o�b�O�̏���������
	if (m_pDebugproc != NULL)
	{
		m_pDebugproc->Init();
	}

	//�t�F�[�h�̐���
	m_pFade = CFade::Create();
	//�t�F�[�h�̐ݒu����
	m_pFade->SetFade(m_mode, m_pFade->FADE_IN);

	//���[�h�̐ݒu
	//SetMode(m_mode);

	return S_OK;
}

//=============================================================================
// �}�l�[�W���̏I������
//=============================================================================
void CManager::Uninit(void)
{
	//�L�[�{�[�h�̏I������
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �}�E�X�̔j��
	if (m_pMouse != NULL)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = NULL;
	}

	//�W���C�p�b�h�̏I������
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	//�����_�����O�̏I������
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//�t�F�[�h�̏I������
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = NULL;
	}
	//�T�E���h�̏I������
	//if (m_pSound != NULL)
	//{
	//	m_pSound->UninitSound();
	//	delete m_pSound;
	//	m_pSound = NULL;
	//}

	//�f�o�b�O�̏I������
	if (m_pDebugproc != NULL)
	{
		m_pDebugproc->Uninit();
		delete m_pDebugproc;
		m_pDebugproc = NULL;
	}

	if (m_pBasemode != NULL)
	{
		// �x�[�X���[�h�̏I������
		m_pBasemode->Uninit();
		// �������̊J��
		delete m_pBasemode;
		m_pBasemode = NULL;
	}

	//�S�Ă̊J������
	CScene::ReleaseAll();
}

//=============================================================================
// �}�l�[�W���̍X�V����
//=============================================================================
void CManager::Update(void)
{
	//�L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	if (m_pMouse != NULL)
	{// ���������m�ۂ���Ă���
		m_pMouse->Update();
	}

	//�W���C�p�b�h�̍X�V����
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();

	}

	//�t�F�[�h�̍X�V����
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}
	//�����_�����O�̍X�V����
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// �x�[�X���[�h�̍X�V����
	if (m_pBasemode != NULL)
	{
		m_pBasemode->Update();
	}
}

//=============================================================================
// �}�l�[�W���̕`�揈��
//=============================================================================
void CManager::Draw(void)
{
	//�����_�����O�̕`�揈��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
// �����_�����O�̎擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;	//�l��Ԃ�
}

//=============================================================================
// �L�[�{�[�h�̎擾
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pInputKeyboard;	//�l��Ԃ�
}

//=============================================================================
//    �}�E�X�N���X�̃|�C���^���擾
//=============================================================================
CInputMouse *CManager::GetMouse(void)
{
	return m_pMouse;
}

//=============================================================================
// �W���C�p�b�h�̎擾
//=============================================================================
CInputJoyStick *CManager::GetJoypad(void)
{
	return m_pInputJoypad;	//�l��Ԃ�
}

//=============================================================================
// �T�E���h�̎擾
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;				//�l��Ԃ�
}

//=============================================================================
// �x�[�X���[�h�̎擾
//=============================================================================
CBasemode *CManager::GetBaseMode(void)
{
	return m_pBasemode;				//�l��Ԃ�
}

//=============================================================================
// �G�f�B�^�[�̎擾
//=============================================================================
CEditor *CManager::GetEditor(void)
{
	return (CEditor*)m_pBasemode;
}

//=============================================================================
// �f���v���C�̎擾
//=============================================================================
CDemoplay *CManager::GetDemoplay(void)
{
	return (CDemoplay*)m_pBasemode;
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;				//�l��Ԃ�
}
//=============================================================================
// ���[�h�̎擾
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;				//�l��Ԃ�
}

//=============================================================================
// ���[�h�̐ݒu
//=============================================================================
void CManager::SetMode(MODE mode)
{
	// �O��̃��[�h��j��
	if (m_pBasemode != NULL)
	{
		m_pBasemode->Uninit();
		delete m_pBasemode;
		m_pBasemode = NULL;
	}

	m_mode = mode;	//���

	switch (mode)
	{
	case MODE_EDITOR:					//�G�f�B�^�[�̏ꍇ
		if (m_pBasemode == NULL)
		{
			//�G�f�B�^�[�̐���
			m_pBasemode = CEditor::Create();
		}
		break;

	case MODE_DEMOPLAY:                  // �f���v���C�̏ꍇ
		if (m_pBasemode == NULL)
		{
			//�f���v���C�̐���
			m_pBasemode = CDemoplay::Create();
		}
		break;
	}
}