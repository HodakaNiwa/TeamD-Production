//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "title.h"
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
#include "sound.h"
#include "debugproc.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TITLE_SYSTEM_FILENAME    "data/TEXT/MODE/title.ini"    // �������Ɏg�p����V�X�e���t�@�C����
#define TITLE_FADE_SPEED         (0.015f)                      // �t�F�[�h�̃X�s�[�h
#define TITLE_FADEOUT_TIMING     (180)                         // �`�[�����S��Ԃ���t�F�[�h�A�E�g��ԂɈڂ鎞��
#define TITLE_CHANGEMODE_TIMING  (600)                         // �ʏ��Ԃ����ʑJ�ڂ���܂ł̎���
#define TITLE_BG_SLIDE_UV        (0.001f)                      // �w�i�̃e�N�X�`�����W���ǂꂭ�炢��������

// �`�[�����S�p�w�i�|���S���������p
#define TITLETEAMBG_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLETEAMBG_COL_INI      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLETEAMBG_WIDTH_INI    (SCREEN_WIDTH / 2.0f)
#define TITLETEAMBG_HEIGHT_INI   (SCREEN_HEIGHT / 2.0f)

// �t�F�[�h�p�|���S���������p
#define TITLEFADE_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLEFADE_COL_INI        (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
#define TITLEFADE_WIDTH_INI      (SCREEN_WIDTH / 2.0f)
#define TITLEFADE_HEIGHT_INI     (SCREEN_HEIGHT / 2.0f)

// �`�[�����S�p�|���S���������p
#define TITLETEAMLOGO_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLETEAMLOGO_COL_INI    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLETEAMLOGO_WIDTH_INI  (100.0f)
#define TITLETEAMLOGO_HEIGHT_INI (100.0f)

// �w�i�|���S���������p
#define TITLEBG_POS_INI          (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define TITLEBG_COL_INI          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEBG_WIDTH_INI        (SCREEN_WIDTH / 2.0f)
#define TITLEBG_HEIGHT_INI       (SCREEN_HEIGHT / 2.0f)

// �^�C�g�����S�������p
#define TITLELOGO_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 280.0f, 0.0f))
#define TITLELOGO_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLELOGO_WIDTH_INI      (200.0f)
#define TITLELOGO_HEIGHT_INI     (120.0f)

// �v���X�ҋ@�������p
#define TITLEPRESS_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 500.0f, 0.0f))
#define TITLEPRESS_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TITLEPRESS_WIDTH_INI     (150.0f)
#define TITLEPRESS_HEIGHT_INI    (60.0f)

// �l�ǂݍ��ݗp�̃p�X
// �e�N�X�`���p
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// �w�i�|���S���p
#define BGPOLYGONSET "BGPOLYGONSET"
#define END_BGPOLYGONSET "END_BGPOLYGONSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// �^�C�g�����S�p
#define TITLELOGOSET "TITLELOGOSET"
#define END_TITLELOGOSET "END_TITLELOGOSET"

// �v���X�ҋ@�p
#define PRESSSET "PRESSSET"
#define END_PRESSSET "END_PRESSSET"

// �`�[�����S�p
#define TEAMLOGOSET "TEAMLOGOSET"
#define END_TEAMLOGOSET "END_TEAMLOGOSET"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================


//*****************************************************************************
// CTitle�̏���
//*****************************************************************************
//=============================================================================
// �^�C�g���̃R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// �^�C�g���̃f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// �^�C�g���̐�������
//=============================================================================
CTitle *CTitle::Create()
{
	CTitle *pTitle = NULL;
	if (pTitle == NULL)
	{
		pTitle = new CTitle;
		if (pTitle != NULL)
		{
			pTitle->Init();
		}
	}
	return pTitle;
}

//=============================================================================
// �^�C�g���̏���������
//=============================================================================
HRESULT CTitle::Init(void)
{
	// �ϐ�������������
	ClearVariable();

	// �V�X�e���̏�����
	LoadSystem();

	// �J�����̐���
	CreateCamera();

	return S_OK;
}

//=============================================================================
// �^�C�g���̏I������
//=============================================================================
void CTitle::Uninit(void)
{
	// ���ʂ̏I������
	CBasemode::Uninit();

	// �e��|���S�����J��
	ReleaseFadePolygon();
	ReleaseTeamLogoPolygon();
	ReleaseTeamBgPolygon();
	ReleaseBgPolygon();
	ReleaseTitleLogoPolygon();
	ReleasePress();

	// �S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();
}

//=============================================================================
// �^�C�g���̍X�V����
//=============================================================================
void CTitle::Update(void)
{
	// �w�i�X���C�h
	SlideBgUV();

	// ��Ԃɂ���ď����킯
	switch (m_State)
	{
	case STATE_TEAMLOGO:
		TeamLogoUpdate();
		break;
	case STATE_FADEOUT:
		FadeOutUpdate();
		break;
	case STATE_FADEIN:
		FadeInUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	}

	// �S�Ă̍X�V����
	CScene::UpdateAll();

	CDebugProc::Print(1, "�^�C�g�����\n");
}

//=============================================================================
// �^�C�g���̕`�揈��
//=============================================================================
void CTitle::Draw(void)
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
// �^�C�g���̃e�N�X�`���Ǌ��N���X��������
//=============================================================================
void CTitle::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}


//=============================================================================
// �^�C�g���̃J������������
//=============================================================================
void CTitle::CreateCamera(void)
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
// �^�C�g���̃t�F�[�h�p�|���S����������
//=============================================================================
void CTitle::CreateFadePolygon(void)
{
	m_pFade = CScene2D::Create(TITLEFADE_POS_INI, TITLEFADE_COL_INI, TITLEFADE_WIDTH_INI, TITLEFADE_HEIGHT_INI);
}

//=============================================================================
// �^�C�g���̃`�[�����S�w�i�p�|���S����������
//=============================================================================
void CTitle::CreateTeamBgPolygon(void)
{
	m_pTeamBg = CScene2D::Create(TITLETEAMBG_POS_INI, TITLETEAMBG_COL_INI, TITLETEAMBG_WIDTH_INI, TITLETEAMBG_HEIGHT_INI);
}

//=============================================================================
// �^�C�g���̃v���X�ҋ@�p�|���S����������
//=============================================================================
void CTitle::CreatePress(void)
{
	m_pPress = CScene2DPress::Create(m_PressPos, m_PressCol, m_fPressWidth, m_fPressHeight);
	if (m_pPress != NULL && GetTextureManager() != NULL)
	{
		m_pPress->BindTexture(GetTextureManager()->GetTexture(m_nPressTexIdx));
	}
}

//=============================================================================
// �^�C�g���̃t�F�[�h�p�|���S���J������
//=============================================================================
void CTitle::ReleaseFadePolygon(void)
{
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();
		m_pFade = NULL;
	}
}

//=============================================================================
// �^�C�g���̃`�[�����S�w�i�p�|���S���J������
//=============================================================================
void CTitle::ReleaseTeamLogoPolygon(void)
{
	if (m_pTeamLogo != NULL)
	{
		m_pTeamLogo->Uninit();
		m_pTeamLogo = NULL;
	}
}

//=============================================================================
// �^�C�g���̃`�[�����S�w�i�p�|���S���J������
//=============================================================================
void CTitle::ReleaseTeamBgPolygon(void)
{
	if (m_pTeamBg != NULL)
	{
		m_pTeamBg->Uninit();
		m_pTeamBg = NULL;
	}
}

//=============================================================================
// �^�C�g���̔w�i�p�|���S���J������
//=============================================================================
void CTitle::ReleaseBgPolygon(void)
{
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}
}

//=============================================================================
// �^�C�g���̃^�C�g�����S�p�|���S���J������
//=============================================================================
void CTitle::ReleaseTitleLogoPolygon(void)
{
	if (m_pTitleLogo != NULL)
	{
		m_pTitleLogo->Uninit();
		m_pTitleLogo = NULL;
	}
}

//=============================================================================
// �^�C�g���̃v���X�ҋ@�p�|���S���J������
//=============================================================================
void CTitle::ReleasePress(void)
{
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}
}

//=============================================================================
// �^�C�g���̔w�i�e�N�X�`�����W�𓮂�������
//=============================================================================
void CTitle::SlideBgUV(void)
{
	if (m_pBg != NULL)
	{
		// �l�̎擾
		float fTexU = m_pBg->GetStartTexU();
		float fTexV = m_pBg->GetStartTexV();
		float fAddTexU = m_pBg->GetAddTexU();
		float fAddTexV = m_pBg->GetAddTexV();

		// �e�N�X�`�����W�𓮂���
		fTexU += TITLE_BG_SLIDE_UV;
		fTexV += TITLE_BG_SLIDE_UV;
		fAddTexU += TITLE_BG_SLIDE_UV;
		fAddTexV += TITLE_BG_SLIDE_UV;

		// �l�̐ݒ�
		m_pBg->SetStartTexU(fTexU);
		m_pBg->SetStartTexV(fTexV);
		m_pBg->SetAddTexU(fAddTexU);
		m_pBg->SetAddTexV(fAddTexV);
		m_pBg->SetVtxBuffTex();
	}
}

//=============================================================================
// �^�C�g���̃`�[�����S��Ԃ̍X�V����
//=============================================================================
void CTitle::TeamLogoUpdate(void)
{
	CDebugProc::Print(1, "�`�[�����S���\n");
	if (CManager::GetFade()->GetFade() != CFade::FADE_NONE) return;

	m_nStateCounter++;
	CDebugProc::Print(1, "%d\n", m_nStateCounter);
	if (m_nStateCounter >= TITLE_FADEOUT_TIMING)
	{// ���l�𒴂���
		m_nStateCounter = 0;
		CreateFadePolygon();
		m_State = STATE_FADEOUT;
	}
	else
	{
		WaitInputToNormal();
	}
	if (m_nStateCounter == 5)
	{
		CManager::GetSound()->PlaySound(3);
	}
}

//=============================================================================
// �^�C�g���̃t�F�[�h�A�E�g��Ԃ̍X�V����
//=============================================================================
void CTitle::FadeOutUpdate(void)
{
	CDebugProc::Print(1, "�t�F�[�h�A�E�g���\n");
	D3DXCOLOR FadeCol = m_pFade->GetCol();
	FadeCol.a += TITLE_FADE_SPEED;
	CDebugProc::Print(1, "%.1f\n", FadeCol.a);
	if (FadeCol.a >= 1.0f)
	{
		ReleaseTeamBgPolygon();
		ReleaseTeamLogoPolygon();
		FadeCol.a = 1.0f;
		m_State = STATE_FADEIN;
	}

	if (m_pFade != NULL)
	{
		m_pFade->SetCol(FadeCol);
		m_pFade->SetVtxBuffCol();
	}
}

//=============================================================================
// �^�C�g���̃t�F�[�h�C����Ԃ̍X�V����
//=============================================================================
void CTitle::FadeInUpdate(void)
{
	CDebugProc::Print(1, "�t�F�[�h�C�����\n");
	D3DXCOLOR FadeCol = m_pFade->GetCol();
	FadeCol.a -= TITLE_FADE_SPEED;
	CDebugProc::Print(1, "%.1f\n", FadeCol.a);
	if (FadeCol.a <= 0.0f)
	{
		ReleaseFadePolygon();
		CreatePress();
		m_State = STATE_NORMAL;
	}

	if (m_pFade != NULL)
	{
		m_pFade->SetCol(FadeCol);
		m_pFade->SetVtxBuffCol();
	}
}

//=============================================================================
// �^�C�g���̒ʏ��Ԃ̍X�V����
//=============================================================================
void CTitle::NormalUpdate(void)
{
	CDebugProc::Print(1, "�ʏ���\n");
	if (CManager::GetFade()->GetFade() != CFade::FADE_NONE) return;

	m_nStateCounter++;
	CDebugProc::Print(1, "%d\n", m_nStateCounter);
	if (m_nStateCounter >= TITLE_CHANGEMODE_TIMING)
	{// ���l�𒴂���
		SetNextMode_None();
	}
	else
	{// �܂������Ă��Ȃ�
		WaitInputToNextMode();
	}
}

//=============================================================================
// �^�C�g���̒ʏ��Ԃɍs�����߂̓��͑ҋ@����
//=============================================================================
void CTitle::WaitInputToNormal(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;

	if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		m_nStateCounter = 0;
		CreateFadePolygon();
		m_State = STATE_FADEOUT;
	}
}

//=============================================================================
// �^�C�g���̎��̃��[�h�ɍs�����߂̓��͑ҋ@����
//=============================================================================
void CTitle::WaitInputToNextMode(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();
	if (pKey == NULL || pFade == NULL) return;


	if (pFade->GetFade() == CFade::FADE_NONE)
	{// �t�F�[�h���g�p����Ă��Ȃ�
		if (pKey->GetTrigger(DIK_RETURN) == true ||
			CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
		{// ����{�^���������ꂽ
			SetNextMode_Push();
		}
	}
}

//=============================================================================
// �^�C�g���̎��̃��[�h�ɐݒ肷�鏈��(���͂���Ȃ������ꍇ)
//=============================================================================
void CTitle::SetNextMode_None(void)
{
	CManager::GetFade()->SetFade(CManager::MODE_TITLE);
}

//=============================================================================
// �^�C�g���̎��̃��[�h�ɐݒ肷�鏈��(���͂��ꂽ�ꍇ)
//=============================================================================
void CTitle::SetNextMode_Push(void)
{
	CManager::GetFade()->SetFade(CManager::MODE_CHARASELECT);
	m_pPress->SetState(CScene2DPress::STATE_PUSH);
}

//=============================================================================
// �^�C�g���̃V�X�e������ǂݍ���
//=============================================================================
void CTitle::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(TITLE_SYSTEM_FILENAME);
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
// �^�C�g���̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CTitle::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
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
		else if (CFunctionLib::Memcmp(pStr, BGPOLYGONSET) == 0)
		{// �w�i�p�|���S����񂾂���
			LoadBgPolygon(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TITLELOGOSET) == 0)
		{// �^�C�g�����S��񂾂���
			LoadTitleLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, PRESSSET) == 0)
		{// �v���X�ҋ@��񂾂���
			LoadPress(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TEAMLOGOSET) == 0)
		{// �`�[�����S��񂾂���
			LoadTeamLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̓ǂݍ��ރe�N�X�`���t�@�C������ǂݍ���
//=============================================================================
void CTitle::LoadTexFileName(char *pStr, int nCntTex)
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
// �^�C�g���̔w�i�|���S���̒l��ǂݍ���
//=============================================================================
void CTitle::LoadBgPolygon(CFileLoader *pFileLoader, char *pStr)
{
	int nBgTexIdx = 0;
	D3DXVECTOR3 BgPos = TITLEBG_POS_INI;
	D3DXCOLOR BgCol = TITLEBG_COL_INI;
	float fBgWidth = TITLEBG_WIDTH_INI;
	float fBgHeight = TITLEBG_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nBgTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			BgPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			BgCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fBgWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fBgHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_BGPOLYGONSET) == 0)
		{// �w�i�|���S�����I���̍��}������
			break;
		}
	}

	// �|���S������
	m_pBg = CScene2D::Create(BgPos, BgCol, fBgWidth, fBgHeight);
	if (m_pBg != NULL && GetTextureManager() != NULL)
	{
		m_pBg->BindTexture(GetTextureManager()->GetTexture(nBgTexIdx));
	}

	// �e�N�X�`�����W��ݒ�
	m_pBg->SetStartTexU(0.0f);
	m_pBg->SetStartTexV(0.0f);
	m_pBg->SetAddTexU(0.5f);
	m_pBg->SetAddTexV(0.5f);
	m_pBg->SetVtxBuffTex();
}

//=============================================================================
// �^�C�g���̃^�C�g�����S�|���S���̒l��ǂݍ���
//=============================================================================
void CTitle::LoadTitleLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nLogoTexIdx = 0;
	D3DXVECTOR3 LogoPos = TITLELOGO_POS_INI;
	D3DXCOLOR LogoCol = TITLELOGO_COL_INI;
	float fLogoWidth = TITLELOGO_WIDTH_INI;
	float fLogoHeight = TITLELOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			LogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			LogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_TITLELOGOSET) == 0)
		{// �^�C�g�����S���I���̍��}������
			break;
		}
	}

	// �|���S������
	m_pTitleLogo = CTitleLogo::Create(LogoPos, LogoCol, fLogoWidth, fLogoHeight);
	if (m_pTitleLogo != NULL && GetTextureManager() != NULL)
	{
		m_pTitleLogo->BindTexture(GetTextureManager()->GetTexture(nLogoTexIdx));
	}
}

//=============================================================================
// �^�C�g���̃v���X�ҋ@�|���S���̒l��ǂݍ���
//=============================================================================
void CTitle::LoadPress(CFileLoader *pFileLoader, char *pStr)
{
	m_nPressTexIdx = 0;
	m_PressPos = TITLEPRESS_POS_INI;
	m_PressCol = TITLEPRESS_COL_INI;
	m_fPressWidth = TITLEPRESS_WIDTH_INI;
	m_fPressHeight = TITLEPRESS_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_nPressTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_PressPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_PressCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_fPressWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_fPressHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PRESSSET) == 0)
		{// �v���X�ҋ@���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �^�C�g���̃`�[�����S�p�|���S���̒l��ǂݍ���
//=============================================================================
void CTitle::LoadTeamLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nTeamLogoTexIdx = 0;
	D3DXVECTOR3 TeamLogoPos = TITLETEAMLOGO_POS_INI;
	D3DXCOLOR TeamLogoCol = TITLETEAMLOGO_COL_INI;
	float fTeamLogoWidth = TITLETEAMLOGO_WIDTH_INI;
	float fTeamLogoHeight = TITLETEAMLOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nTeamLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			TeamLogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			TeamLogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fTeamLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fTeamLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_TEAMLOGOSET) == 0)
		{// �`�[�����S���I���̍��}������
			break;
		}
	}

	// �`�[�����S�p�w�i�|���S���̐���
	CreateTeamBgPolygon();

	// �|���S������
	m_pTeamLogo = CScene2D::Create(TeamLogoPos, TeamLogoCol, fTeamLogoWidth, fTeamLogoHeight);
	if (m_pTeamLogo != NULL && GetTextureManager() != NULL)
	{
		m_pTeamLogo->BindTexture(GetTextureManager()->GetTexture(nTeamLogoTexIdx));
	}
}

//=============================================================================
// �^�C�g���̕ϐ�������������
//=============================================================================
void CTitle::ClearVariable(void)
{
	m_State = STATE_TEAMLOGO;
	m_pFade = NULL;
	m_pTeamLogo = NULL;
	m_pTeamBg = NULL;
	m_pBg = NULL;
	m_pTitleLogo = NULL;
	m_pPress = NULL;
	m_nStateCounter = 0;
	ClearPress();
}

//=============================================================================
// �^�C�g���̃v���X�ҋ@�p�ϐ�������������
//=============================================================================
void CTitle::ClearPress(void)
{
	m_nPressTexIdx = 0;
	m_PressPos = INITIALIZE_D3DXVECTOR3;
	m_PressCol = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
	m_fPressWidth = 0.0f;
	m_fPressHeight = 0.0f;
}

//=============================================================================
// �^�C�g���̏�Ԃ�ݒ肷��
//=============================================================================
void CTitle::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
// �^�C�g���̏�Ԃ��擾����
//=============================================================================
CTitle::STATE CTitle::GetState(void)
{
	return m_State;
}

//*****************************************************************************
// CTitleLogo�̏���
//*****************************************************************************
//=============================================================================
// �I�u�W�F�N�g�̃R���X�g���N�^
//=============================================================================
CTitleLogo::CTitleLogo(int nPriority, OBJTYPE objtype) : CScene2D(nPriority, objtype)
{

}
//=============================================================================
// �I�u�W�F�N�g�̃f�X�g���N�^
//=============================================================================
CTitleLogo::~CTitleLogo()
{

}

//=============================================================================
//�I�u�W�F�N�g�̐���
//=============================================================================
CTitleLogo *CTitleLogo::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority, float fRot, float fStartTexU, float fStartTexV, float fAddTexU, float fAddTexV)
{
	CTitleLogo *pTitleLogo = NULL;	// CTitleLogo�N���X�^�̃|�C���^
	if (pTitleLogo == NULL)
	{
		pTitleLogo = new CTitleLogo(nPriority);
		if (pTitleLogo != NULL)
		{
			// �l�̐ݒ�
			pTitleLogo->SetPos(pos);
			pTitleLogo->SetCol(col);
			pTitleLogo->SetWidth(fWidth);
			pTitleLogo->SetHeight(fHeight);
			pTitleLogo->SetRot(fRot);
			pTitleLogo->SetStartTexU(fStartTexU);
			pTitleLogo->SetStartTexV(fStartTexV);
			pTitleLogo->SetAddTexU(fAddTexU);
			pTitleLogo->SetAddTexV(fAddTexV);

			if (FAILED(pTitleLogo->Init()))
			{// �������Ɏ��s
				return NULL;
			}
		}
	}
	return pTitleLogo;
}

//=============================================================================
// �I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CTitleLogo::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CScene2D::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// �I�u�W�F�N�g�̏I������
//=============================================================================
void CTitleLogo::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
// �I�u�W�F�N�g�̍X�V����
//=============================================================================
void CTitleLogo::Update(void)
{

}

//=============================================================================
// �^�C�g�����S�̕`�揈��
//=============================================================================
void CTitleLogo::Draw(void)
{
	// ���ʂ̕`�揈��
	CScene2D::Draw();
}