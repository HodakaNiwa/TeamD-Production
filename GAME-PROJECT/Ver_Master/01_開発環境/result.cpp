//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "number.h"
#include "title.h"
#include "sound.h"
#include "debugproc.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define RESULT_SYSTEM_FILENAME               "data/TEXT/MODE/result.ini"    // �������Ɏg�p����V�X�e���t�@�C����
#define RESULT_WAITSTATE_TIME                (60)                           // �ҋ@��Ԃ���ʏ��Ԃɕς��܂ł̎���
#define RESULT_NEXTHIGHSCORE_TIME            (240)                          // �ʏ��Ԃ���n�C�X�R�A��\�������Ԃɕς��܂ł̎���
#define RESULT_CHANGEMODE_TIME               (600)                          // �ʏ��Ԃ���I����Ԃɕς��܂ł̎���
#define RESULT_HIGHSCORE_FALSH_TIME          (2)                            // �n�C�X�R�A���S��_�ł�����^�C�~���O
#define RESULT_BGM_IDX                       (4)                            // ���U���g�ōĐ�����BGM�̔ԍ�

// ���U���g���S�������p
#define RESULT_LOGO_POS_INI                  (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define RESULT_LOGO_COL_INI                  (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_LOGO_WIDTH_INI                (150.0f)
#define RESULT_LOGO_HEIGHT_INI               (100.0f)

// ���U���g�n�C�X�R�A���S�������p
#define RESULT_HIGHSCORE_FALSH_COL           (D3DXCOLOR(0.5f,0.5f,1.0f,1.0f))
#define RESULT_HIGHSCORELOGO_POS_INI         (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 220.0f, 0.0f))
#define RESULT_HIGHSCORELOGO_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_HIGHSCORELOGO_WIDTH_INI       (400.0f)
#define RESULT_HIGHSCORELOGO_HEIGHT_INI      (60.0f)

// ���U���g�n�C�X�R�A���S�������p
#define RESULT_HIGHSCORENUMBER_POS_INI       (D3DXVECTOR3(900.0f, 380.0f, 0.0f))
#define RESULT_HIGHSCORENUMBER_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define RESULT_HIGHSCORENUMBER_WIDTH_INI     (40.0f)
#define RESULT_HIGHSCORENUMBER_HEIGHT_INI    (40.0f)
#define RESULT_HIGHSCORENUMBER_INTERVAL_INI  (D3DXVECTOR3(-90.0f, 0.0f, 0.0f))

// �l�ǂݍ��ݗp�̃p�X
// �e�N�X�`���p
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// ���U���g���S�p
#define RESULTLOGOSET "RESULTLOGOSET"
#define END_RESULTLOGOSET "END_RESULTLOGOSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
bool CResult::m_bHighScore = true;   // �n�C�X�R�A��\�����邩�ǂ���

//=============================================================================
// ���U���g�̃R���X�g���N�^
//=============================================================================
CResult::CResult()
{

}

//=============================================================================
// ���U���g�̃f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ���U���g�̐�������
//=============================================================================
CResult *CResult::Create()
{
	CResult *pResult = NULL;
	if (pResult == NULL)
	{
		pResult = new CResult;
		if (pResult != NULL)
		{
			pResult->Init();
		}
	}
	return pResult;
}

//=============================================================================
// ���U���g�̏���������
//=============================================================================
HRESULT CResult::Init(void)
{
	// �ϐ�������������
	ClearVariable();

	// �V�X�e���̏�����
	LoadSystem();

	// �J�����̐���
	CreateCamera();

	// BGM�̍Đ�
	CManager::GetSound()->PlaySound(RESULT_BGM_IDX);

	return S_OK;
}

//=============================================================================
// ���U���g�̏I������
//=============================================================================
void CResult::Uninit(void)
{
	// ���ʂ̏I������
	CBasemode::Uninit();

	// �e��N���X�̊J��
	ReleaseLogo();
	ReleaseHighScore();

	// �S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();

	// BGM���~
	CManager::GetSound()->StopSound(RESULT_BGM_IDX);

	// �n�C�X�R�A��\�����Ȃ���Ԃ�
	m_bHighScore = false;
}

//=============================================================================
// ���U���g�̍X�V����
//=============================================================================
void CResult::Update(void)
{
	CDebugProc::Print(1, "���U���g���\n");

	// ��Ԃɂ���ď����킯
	switch (m_State)
	{
	case STATE_WAIT:
		WaitUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	case STATE_HIGHSCORE:
		HighScoreUpdate();
		break;
	case STATE_END:
		EndUpdate();
		break;
	}

	// �S�Ă̍X�V����
	CScene::UpdateAll();
}

//=============================================================================
// ���U���g�̕`�揈��
//=============================================================================
void CResult::Draw(void)
{
	// �J�����̐ݒ�
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera();
	}

	// �V�[���N���X�̕`�揈��
	CScene::DrawAll();
}

//=============================================================================
// ���U���g�̃e�N�X�`���Ǌ��N���X��������
//=============================================================================
void CResult::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// ���U���g�̃J������������
//=============================================================================
void CResult::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager == NULL) return;

	// �J�����𐶐�
	CCamera *pCamera = CCamera::Create();
	if (pCamera != NULL)
	{
		pCameraManager->SetCamera(pCamera);
	}
	SetCameraManager(pCameraManager);
}

//=============================================================================
// ���U���g�̃��S��������
//=============================================================================
void CResult::CreateLogo(void)
{
	m_pLogo = CScene2D::Create(m_ResultLogoData.pos, m_ResultLogoData.col, m_ResultLogoData.fWidth, m_ResultLogoData.fHeight);
	if (m_pLogo != NULL && GetTextureManager() != NULL)
	{
		m_pLogo->BindTexture(GetTextureManager()->GetTexture(m_ResultLogoData.nTexIdx));
	}
}

//=============================================================================
// ���U���g�̃��S��������
//=============================================================================
void CResult::CreateHighScore(void)
{
	// ���S�|���S������
	if (m_pHighScoreLogo == NULL)
	{
		m_pHighScoreLogo = CScene2D::Create(RESULT_HIGHSCORELOGO_POS_INI, RESULT_HIGHSCORELOGO_COL_INI,
			RESULT_HIGHSCORELOGO_WIDTH_INI, RESULT_HIGHSCORELOGO_HEIGHT_INI);
		if (m_pHighScoreLogo != NULL && GetTextureManager() != NULL)
		{
			m_pHighScoreLogo->BindTexture(GetTextureManager()->GetTexture(m_ResultLogoData.nTexIdx));
		}
	}

	// �����|���S������
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(1);
	}
	if (m_pHighScore == NULL)
	{
		m_pHighScore = CNumber::Create(RESULT_HIGHSCORENUMBER_POS_INI, RESULT_HIGHSCORENUMBER_COL_INI,
			RESULT_HIGHSCORENUMBER_WIDTH_INI, RESULT_HIGHSCORENUMBER_HEIGHT_INI, RESULT_HIGHSCORENUMBER_INTERVAL_INI,
			pTexture, CTitle::GetHighScore());
	}
}

//=============================================================================
// ���U���g�̃��S�J������
//=============================================================================
void CResult::ReleaseLogo(void)
{
	if (m_pLogo != NULL)
	{
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}
}

//=============================================================================
// ���U���g�̃n�C�X�R�A�J������
//=============================================================================
void CResult::ReleaseHighScore(void)
{
	// ���S�|���S���J��
	if (m_pHighScoreLogo != NULL)
	{
		m_pHighScoreLogo->Uninit();
		m_pHighScoreLogo = NULL;
	}

	// �����|���S���J��
	if (m_pHighScore != NULL)
	{
		m_pHighScore->Uninit();
		m_pHighScore = NULL;
	}
}

//=============================================================================
// ���U���g�̑ҋ@��Ԃ̍X�V����
//=============================================================================
void CResult::WaitUpdate(void)
{
	CDebugProc::Print(1, "�ҋ@���\n");

	m_nStateCounter++;
	if (m_nStateCounter % RESULT_WAITSTATE_TIME == 0)
	{// �J�E���^�[�����l�ɒB����
		SetState(STATE_NORMAL);
		m_nStateCounter = 0;
	}

	CDebugProc::Print(1, "%d\n", m_nStateCounter);
}

//=============================================================================
// ���U���g�̒ʏ��Ԃ̍X�V����
//=============================================================================
void CResult::NormalUpdate(void)
{
	CDebugProc::Print(1, "�ʏ���\n");

	// �J�E���^�[���Z
	m_nStateCounter++;

	if (m_bHighScore == true)
	{// �n�C�X�R�A���X�V����Ă���
		if (m_nStateCounter % RESULT_NEXTHIGHSCORE_TIME == 0)
		{// �J�E���^�[�����l�ɒB����
			// �n�C�X�R�A��\�������Ԃ�
			SetState(STATE_HIGHSCORE);
			m_nStateCounter = 0;

			// ���U���g���S���J��
			ReleaseLogo();

			// �n�C�X�R�A�\���ɕK�v�Ȃ��̂𐶐�
			CreateHighScore();
		}

		// ���̏�ԑҋ@����
		WaitInputToHighScore();
	}
	else
	{// �n�C�X�R�A���X�V����Ă��Ȃ�
		if (m_nStateCounter % RESULT_CHANGEMODE_TIME == 0)
		{// �J�E���^�[�����l�ɒB����
			SetState(STATE_END);
			m_nStateCounter = 0;
		}

		// ���̃��[�h�ҋ@����
		WaitInputToNextMode();
	}

	CDebugProc::Print(1, "%d\n", m_nStateCounter);
}

//=============================================================================
// ���U���g�̃n�C�X�R�A��\�������Ԃ̍X�V����
//=============================================================================
void CResult::HighScoreUpdate(void)
{
	CDebugProc::Print(1, "�n�C�X�R�A�\�����\n");

	// �X�R�A��_�ł�����
	if (m_nStateCounter % RESULT_HIGHSCORE_FALSH_TIME == 0)
	{
		// �_�ł̐F��ݒ�
		if (m_FalshCol.r < 1.0f)
		{
			m_FalshCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			m_FalshCol = RESULT_HIGHSCORE_FALSH_COL;
		}

		// �|���S���̐F��ݒ�
		// ���S�|���S��
		if (m_pHighScoreLogo != NULL)
		{
			m_pHighScoreLogo->SetCol(m_FalshCol);
			m_pHighScoreLogo->SetVtxBuffCol();
		}

		// �����|���S��
		if (m_pHighScore != NULL)
		{
			m_pHighScore->SetNumberCol(m_FalshCol);
			m_pHighScore->SetNumberVtxBuffCol();
		}
	}

	// �J�E���^�[���Z
	m_nStateCounter++;

	if (m_nStateCounter % RESULT_CHANGEMODE_TIME == 0)
	{// �J�E���^�[�����l�ɒB����
		SetState(STATE_END);
		m_nStateCounter = 0;
	}

	CDebugProc::Print(1, "%d\n", m_nStateCounter);

	// ���̃��[�h�ҋ@����
	WaitInputToNextMode();
}

//=============================================================================
// ���U���g�̏I����Ԃ̍X�V����
//=============================================================================
void CResult::EndUpdate(void)
{
	CDebugProc::Print(1, "�I�����\n");

	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) return;

	if (pFade->GetFade() == CFade::FADE_NONE)
	{// �t�F�[�h���g�p����Ă��Ȃ�
		pFade->SetFade(CManager::MODE_TITLE);
	}
}

//=============================================================================
// ���U���g�̃n�C�X�R�A��\�������Ԃɍs�����߂̑ҋ@����
//=============================================================================
void CResult::WaitInputToHighScore(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL) return;

	if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		// ��Ԃ�ݒ�
		SetState(STATE_HIGHSCORE);
		m_nStateCounter = 0;

		// ���U���g���S���J��
		ReleaseLogo();

		// �n�C�X�R�A�\���ɕK�v�Ȃ��̂𐶐�
		CreateHighScore();
	}
}

//=============================================================================
// ���U���g�̎��̃��[�h�ɍs�����߂̑ҋ@����
//=============================================================================
void CResult::WaitInputToNextMode(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL) return;

	if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		SetState(STATE_END);
	}
}

//=============================================================================
// ���U���g�̃V�X�e������ǂݍ���
//=============================================================================
void CResult::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(RESULT_SYSTEM_FILENAME);
	if (pFileLoader != NULL)
	{
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// �ǂݍ��݊J�n�̍��}������
			LoadSystemScript(pFileLoader, aStr);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// ���U���g�̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CResult::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// �ǂݍ��ރe�N�X�`���̐�������
			CreateTexture(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// �ǂݍ��ރe�N�X�`���̃t�@�C����������
			LoadTexFileName(pStr, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, RESULTLOGOSET) == 0)
		{// ���U���g���S��񂾂���
			LoadLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// ���U���g�̓ǂݍ��ރe�N�X�`���t�@�C������ǂݍ���
//=============================================================================
void CResult::LoadTexFileName(char *pStr, int nCntTex)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	char aTexFileName[256] = "\0";
	strcpy(aTexFileName, CFunctionLib::ReadString(pStr, aTexFileName, TEXTURE_FILENAME));

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), aTexFileName, &pTexture);

	// �e�N�X�`���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
	CTextureManager *pTextureManager = GetTextureManager();
	pTextureManager->SetTexture(pTexture, nCntTex);
	pTextureManager->SetFileName(pStr, nCntTex);
}

//=============================================================================
// ���U���g�̃��S�|���S������ǂݍ���
//=============================================================================
void CResult::LoadLogo(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_ResultLogoData.nTexIdx = 0;
	m_ResultLogoData.pos = RESULT_LOGO_POS_INI;
	m_ResultLogoData.col = RESULT_LOGO_COL_INI;
	m_ResultLogoData.fWidth = RESULT_LOGO_WIDTH_INI;
	m_ResultLogoData.fHeight = RESULT_LOGO_HEIGHT_INI;

	// �f�[�^�ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_ResultLogoData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_ResultLogoData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_ResultLogoData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_ResultLogoData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_ResultLogoData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_RESULTLOGOSET) == 0)
		{// ���U���g���S�|���S�����I���̍��}������
			break;
		}
	}

	// �|���S������
	CreateLogo();
}

//=============================================================================
// ���U���g�̕ϐ�������������
//=============================================================================
void CResult::ClearVariable(void)
{
	m_State = STATE_WAIT;
	m_nStateCounter = 0;
	m_pLogo = NULL;
	m_pHighScore = NULL;
	m_pHighScoreLogo = NULL;
}

//=============================================================================
// ���U���g�̏�Ԃ�ݒ肷��
//=============================================================================
void CResult::SetState(const STATE state)
{
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// ���U���g�̃n�C�X�R�A���o�����ǂ�����ݒ肷��
//=============================================================================
void CResult::SetHighScore(void)
{
	m_bHighScore = true;
}

//=============================================================================
// ���U���g�̏�Ԃ��擾����
//=============================================================================
CResult::STATE CResult::GetState(void)
{
	return m_State;
}