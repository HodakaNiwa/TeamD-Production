//*****************************************************************************
//
//     �}�l�[�W���[�̏���[manager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾(���̂��Ȃ��錾)
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CServer;
class CClient;
class CDebugProc;

//*****************************************************************************
//    �}�l�[�W���N���X�̒�`
//*****************************************************************************
class CManager
{
public:    // �N�ł��A�N�Z�X�\
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick *GetJoyStick(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateServer(void);
	void CreateClient(void);

	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�g�N���X�^�ւ̃|�C���^
	static CInputMouse *m_pMouse;               // �}�E�X�N���X�^�ւ̃|�C���^
	static CInputJoyStick *m_pInputJoyStick;	// �W���C�p�b�h�N���X�^�ւ̃|�C���^
	static CServer *m_pServer;                  // �T�[�o�[�N���X�^�ւ̃|�C���^
	static CClient *m_pClient;                  // �N���C�A���g�N���X�^�ւ̃|�C���^
	static CRenderer *m_pRenderer;              // �����_�����O�N���X�^�ւ̃|�C���^
	static CDebugProc *m_pDebugProc;            // �f�o�b�O���O�N���X�ւ̃|�C���^
};

#endif