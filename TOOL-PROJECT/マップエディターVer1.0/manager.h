//=============================================================================
//
// �}�l�[�W������ [manager.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//�O���錾
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CSound;
class CCamera;
class CLight;
class CDebugProc;
class CBasemode;
class CEditor;
class CDemoplay;
class CFade;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CManager
{
public:	//�N������A�N�Z�X�\
	typedef enum
	{
		MODE_NONE = 0,
		MODE_EDITOR,
		MODE_DEMOPLAY,
		MODE_MAX,
	}MODE;

	//�����o�֐�
	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ÓI�����o�֐�
	static CRenderer		*GetRenderer(void);
	static CInputKeyboard	*GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick		*GetJoypad(void);
	static CSound			*GetSound(void);
	static CBasemode        *GetBaseMode(void);
	static CEditor          *GetEditor(void);
	static CDemoplay        *GetDemoplay(void);
	static MODE				GetMode(void);
	static void				SetMode(MODE mode);
	static CFade			*GetFade(void);

private:	//�����������A�N�Z�X�\
	//�ÓI�����o�ϐ�
	static CRenderer		*m_pRenderer;		//�����_�����O�̃|�C���^
	static CInputKeyboard	*m_pInputKeyboard;	//�L�[�{�[�g�̃|�C���^
	static CInputMouse       *m_pMouse;         // �}�E�X�N���X�^�̃|�C���^
	static CInputJoyStick   *m_pInputJoypad;	//�W���C�p�b�h�̃|�C���^
	static CSound			*m_pSound;			//�T�E���h�̃|�C���^
	static CDebugProc		*m_pDebugproc;		//�f�o�b�O�̃|�C���^
	static MODE				m_mode;				//���[�h�̏��
	static CBasemode		*m_pBasemode;		//�x�[�X���[�h�̃|�C���^
	static CFade			*m_pFade;			//�t�F�[�h�̃|�C���^

protected:	//�����Ɣh���N���X�������A�N�Z�X�\
};
#endif