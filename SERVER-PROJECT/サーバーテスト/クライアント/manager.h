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
class CBasemode;
class CGame;
class CFade;
class CDebugProc;

//*****************************************************************************
//    �}�l�[�W���N���X�̒�`
//*****************************************************************************
class CManager
{
public:    // �N�ł��A�N�Z�X�\
	typedef enum
	{
		MODE_NONE = 0,
		MODE_GAME,
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void	SetMode(MODE mode);
	static MODE	GetMode(void);
	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick *GetJoyStick(void);
	static CClient *GetClient(void);
	static CBasemode *GetBaseMode(void);
	static CGame *GetGame(void);
	static CFade *GetFade(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateServer(void);
	void CreateClient(void);

	static CRenderer *m_pRenderer;              // �����_�����O�N���X�^�̃|�C���^
	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�g�N���X�^�̃|�C���^
	static CInputMouse *m_pMouse;               // �}�E�X�N���X�^�̃|�C���^
	static CInputJoyStick *m_pInputJoyStick;	// �W���C�p�b�h�N���X�^�̃|�C���^
	static CServer *m_pServer;                  // �T�[�o�[�N���X�^�̃|�C���^
	static CClient *m_pClient;                  // �N���C�A���g�N���X�^�̃|�C���^
	static MODE	m_mode;				            // ���݂̃��[�h
	static CBasemode *m_pBasemode;		        // �x�[�X���[�h�N���X�^�̃|�C���^
	static CFade *m_pFade;			            // �t�F�[�h�N���X�^�̃|�C���^
	static CDebugProc *m_pDebugProc;            // �f�o�b�O���O�N���X�^�̃|�C���^
};

#endif