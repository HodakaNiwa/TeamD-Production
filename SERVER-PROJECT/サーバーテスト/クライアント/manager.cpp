//*****************************************************************************
//
//     �}�l�[�W���[�̏���[manager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "server.h"
#include "fileLoader.h"
#include "functionlib.h"
#include "basemode.h"
#include "game.h"
#include "fade.h"
#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MANAGER_SYSTEM_FILENAME "data/system.ini"
#define SERVER_ADDRESS "SERVER_ADDRESS = "

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
CRenderer *CManager::m_pRenderer = NULL;              // �����_�����O�N���X�^�̃|�C���^
CInputKeyboard *CManager::m_pInputKeyboard = NULL;    // �L�[�{�[�h�N���X�^�̃|�C���^
CInputMouse *CManager::m_pMouse = NULL;               // �}�E�X�N���X�^�̃|�C���^
CInputJoyStick *CManager::m_pInputJoyStick = NULL;	  // �W���C�p�b�h�N���X�^�̃|�C���^
CServer *CManager::m_pServer = NULL;                  // �T�[�o�[�N���X�^�̃|�C���^
CClient *CManager::m_pClient = NULL;                  // �N���C�A���g�N���X�^�̃|�C���^
CManager::MODE CManager::m_mode = MODE::MODE_GAME;    // ���݂̃��[�h
CBasemode *CManager::m_pBasemode = NULL;              // �x�[�X���[�h�N���X�^�̃|�C���^
CFade *CManager::m_pFade = NULL;			          // �t�F�[�h�N���X�^�ւ̃|�C���^
CDebugProc *CManager::m_pDebugProc = NULL;            // �f�o�b�O���O�N���X�^�̃|�C���^

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �T�[�o�[�̃X�^�[�g�A�b�v
	CServer::ServerStartUp();

	// �����_�����O�̍쐬
	if (m_pRenderer == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))  // FALSE�ɂ���ƃt���X�N���[��
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// �L�[�{�[�h�̍쐬
	if (m_pInputKeyboard == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pInputKeyboard = new CInputKeyboard;
		if (m_pInputKeyboard != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
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
	if (m_pInputJoyStick == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pInputJoyStick = new CInputJoyStick(1);
		if (m_pInputJoyStick != NULL)
		{// ���������m�ۂł���
			m_pInputJoyStick->Init(hInstance, hWnd);
		}
	}

	// �f�o�b�O���O�N���X�̍쐬
	if (m_pDebugProc == NULL)
	{
		m_pDebugProc = new CDebugProc;
		if (m_pDebugProc != NULL)
		{
			m_pDebugProc->Init();
		}
	}


#ifdef SERVER_MODE
	CreateServer();
#else
	CreateClient();
#endif

	// �t�F�[�h�̐���
	m_pFade = CFade::Create();
	if (m_pFade != NULL)
	{
		// �t�F�[�h�̐ݒ�
		m_pFade->SetFade(m_mode, m_pFade->FADE_IN);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �L�[�{�[�h�̔j��
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

	// �W���C�p�b�h�̔j��
	if (m_pInputJoyStick != NULL)
	{
		m_pInputJoyStick->Uninit();
		delete m_pInputJoyStick;
		m_pInputJoyStick = NULL;
	}

	// �f�o�b�O���O�̔j��
	if (m_pDebugProc != NULL)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	// �����_�����O�̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// �T�[�o�[�̔j��
	if (m_pServer != NULL)
	{
		m_pServer->Uninit();
		delete m_pServer;
		m_pServer = NULL;
	}

	// �N���C�A���g�̔j��
	if (m_pClient != NULL)
	{
		m_pClient->Uninit();
		delete m_pClient;
		m_pClient = NULL;
	}

	// �T�[�o�[�̃N���[���A�b�v
	CServer::ServerCleanUp();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CManager::Update(void)
{
	// �L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// �}�E�X�̍X�V����
	if (m_pMouse != NULL)
	{
		m_pMouse->Update();
	}

	// �W���C�p�b�h�̍X�V����
	if (m_pInputJoyStick != NULL)
	{
		m_pInputJoyStick->Update();

	}

	// �t�F�[�h�̍X�V����
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

	// �x�[�X���[�h�̍X�V����
	if (m_pBasemode != NULL)
	{
		m_pBasemode->Update();
	}

#ifdef SERVER_MODE
	// �T�[�o�[�̍X�V����
	if (m_pServer != NULL)
	{
		m_pServer->Update();
	}
#else
	// �N���C�A���g�̍X�V����
	if (m_pClient != NULL)
	{
		m_pClient->Update();
	}
#endif

	// �����_�����O�̍X�V����
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// ���������m�ۂ���Ă���
		m_pRenderer->Draw();
	}

	// �f�o�b�O���O�̕�����N���A
	CDebugProc::ClearString();
}

//=============================================================================
//    �T�[�o�[�𐶐�����
//=============================================================================
void CManager::CreateServer(void)
{
	if (m_pServer == NULL)
	{
		m_pServer = CServer::Create();
	}
}

//=============================================================================
//    �N���C�A���g�𐶐�����
//=============================================================================
void CManager::CreateClient(void)
{
	char aStr[256];
	char aServerAddr[256] = "\0";
	CFileLoader *pFileLoader = CFileLoader::Create(MANAGER_SYSTEM_FILENAME);
	if (pFileLoader != NULL)
	{
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SERVER_ADDRESS) == 0)
		{// �T�[�o�[��IP�A�h���X������
			strcpy(aServerAddr, CFunctionLib::ReadString(aStr, aServerAddr, SERVER_ADDRESS));
			if (m_pClient == NULL)
			{
				//m_pClient = CClient::Create("127.0.1.1");  <- ���[�J��PC��̃z�X�g�A�h���X�Ȃ̂ő�PC�Ƃ͒ʐM�ł��Ȃ�!
				m_pClient = CClient::Create(aServerAddr);
			}
		}

		if (pFileLoader != NULL)
		{
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
//    ���݂̃��[�h��ݒ�
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
	case MODE_GAME:    // �Q�[��
		m_pBasemode = CGame::Create();
		break;
	}
}

//=============================================================================
//    ���݂̃��[�h���擾
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

//=============================================================================
//    �����_�����O�N���X�̃|�C���^���擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
//    �L�[�{�[�h�N���X�̃|�C���^���擾
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pInputKeyboard;
}

//=============================================================================
//    �}�E�X�N���X�̃|�C���^���擾
//=============================================================================
CInputMouse *CManager::GetMouse(void)
{
	return m_pMouse;
}

//=============================================================================
//    �W���C�X�e�B�b�N�N���X�̃|�C���^���擾
//=============================================================================
CInputJoyStick *CManager::GetJoyStick(void)
{
	return m_pInputJoyStick;
}

//=============================================================================
//    �N���C�A���g�N���X�̃|�C���^���擾
//=============================================================================
CClient *CManager::GetClient(void)
{
	return m_pClient;
}

//=============================================================================
//    �x�[�X���[�h�N���X�̃|�C���^���擾
//=============================================================================
CBasemode *CManager::GetBaseMode(void)
{
	return m_pBasemode;
}

//=============================================================================
//    �Q�[���N���X�̃|�C���^���擾
//=============================================================================
CGame *CManager::GetGame(void)
{
	return (CGame*)m_pBasemode;
}

//=============================================================================
//    �t�F�[�h�N���X�̃|�C���^���擾
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;
}