//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "tutorial.h"
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
#include "map.h"
#include "debugproc.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TUTORIAL_SYSTEM_FILENAME  "data/TEXT/MODE/tutorial.ini"  // �������Ɏg�p����V�X�e���t�@�C����
#define TUTORIAL_WAITSTATE_TIME   (60)                           // �ҋ@��Ԃ���ʏ��Ԃɕς��܂ł̎���
#define TUTORIAL_CHANGEMODE_TIME  (600)                          // �ʏ��Ԃ���I����Ԃɕς��܂ł̎���


// �X�L�b�v���S�������p
#define TUTORIAL_SKIPLOGO_POS_INI              (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 3.0f, 0.0f))
#define TUTORIAL_SKIPLOGO_COL_INI              (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define TUTORIAL_SKIPLOGO_WIDTH_INI            (300.0f)
#define TUTORIAL_SKIPLOGO_HEIGHT_INI           (90.0f)

// �X�L�b�v�`�F�b�N���S�������p
// �P��
#define TUTORIAL_SKIPCHECKLOGO_0_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 3.0f * 2.0f, 0.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_COL_NONE      (D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE    (80.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE   (40.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_COL_SELECT    (D3DXCOLOR(0.9f,0.9f,0.9f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_0_WIDTH_SELECT  (120.0f)
#define TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_SELECT (60.0f)

// �Q��
#define TUTORIAL_SKIPCHECKLOGO_1_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 4.0f * 3.0f, SCREEN_HEIGHT / 3.0f * 2.0f, 0.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_COL_NONE      (D3DXCOLOR(0.2f,0.2f,0.2f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE    (80.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE   (40.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_COL_SELECT    (D3DXCOLOR(0.9f,0.9f,0.9f,1.0f))
#define TUTORIAL_SKIPCHECKLOGO_1_WIDTH_SELECT  (120.0f)
#define TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_SELECT (60.0f)

// �l�ǂݍ��ݗp�̃p�X
// �e�N�X�`���p
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// �}�b�v�p
#define NUM_MAP "NUM_MAP = "

// �X�L�b�v���S�p
#define SKIPLOGOSET "SKIPLOGOSET"
#define END_SKIPLOGOSET "END_SKIPLOGOSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// �X�L�b�v�`�F�b�N���S�p
#define SKIPCHECKLOGOSET "SKIPCHECKLOGOSET"
#define END_SKIPCHECKLOGOSET "END_SKIPCHECKLOGOSET"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �`���[�g���A���̃R���X�g���N�^
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// �`���[�g���A���̃f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// �`���[�g���A���̐�������
//=============================================================================
CTutorial *CTutorial::Create()
{
	CTutorial *pTutorial = NULL;
	if (pTutorial == NULL)
	{
		pTutorial = new CTutorial;
		if (pTutorial != NULL)
		{
			pTutorial->Init();
		}
	}
	return pTutorial;
}

//=============================================================================
// �`���[�g���A���̏���������
//=============================================================================
HRESULT CTutorial::Init(void)
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
// �`���[�g���A���̏I������
//=============================================================================
void CTutorial::Uninit(void)
{
	// ���ʂ̏I������
	CBasemode::Uninit();

	// �e��N���X�̊J��
	ReleaseSkipLogo();
	ReleaseSkipCheckLogo();
	ReleaseMapFilePointer();

	// �S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();
}

//=============================================================================
// �`���[�g���A���̍X�V����
//=============================================================================
void CTutorial::Update(void)
{
	CDebugProc::Print(1, "�`���[�g���A�����\n");

	// ��Ԃɂ���ď����킯
	switch (m_State)
	{
	case STATE_SKIP_CHECK:
		SkipCheckUpdate();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		break;
	case STATE_GOAL:
		GoalUpdate();
		break;
	case STATE_MAP_CHANGE:
		MapChangeUpdate();
		break;
	case STATE_END:
		EndUpdate();
		break;
	}

	// �S�Ă̍X�V����
	CScene::UpdateAll();
}

//=============================================================================
// �`���[�g���A���̕`�揈��
//=============================================================================
void CTutorial::Draw(void)
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
// �`���[�g���A���̃e�N�X�`���Ǌ��N���X��������
//=============================================================================
void CTutorial::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// �`���[�g���A���̃J������������
//=============================================================================
void CTutorial::CreateCamera(void)
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
// �`���[�g���A���̓ǂݍ��ރ}�b�v�t�@�C�����̃|�C���^�𐶐�����
//=============================================================================
void CTutorial::CreateMapFilePointer(void)
{
	m_pMapFileName = new char*[m_nNumMap];
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			m_pMapFileName[nCnt] = new char[256];
			strcpy(m_pMapFileName[nCnt], "\0");
		}
	}
}

//=============================================================================
// �`���[�g���A���̓ǂݍ��ރ}�b�v�t�@�C�����̃|�C���^���J������
//=============================================================================
void CTutorial::ReleaseMapFilePointer(void)
{
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			delete[] m_pMapFileName[nCnt];
			m_pMapFileName[nCnt] = NULL;
		}

		delete[] m_pMapFileName;
		m_pMapFileName = NULL;
	}
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v���S�|���S�����J������
//=============================================================================
void CTutorial::ReleaseSkipLogo(void)
{
	if (m_pSkipLogo != NULL)
	{
		m_pSkipLogo->Uninit();
		m_pSkipLogo = NULL;
	}
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v�m�F���S�p�|���S�����J������
//=============================================================================
void CTutorial::ReleaseSkipCheckLogo(void)
{
	for (int nCntSkipCheck = 0; nCntSkipCheck < SKIP_MAX; nCntSkipCheck++)
	{
		if (m_pSkipCheckLogo[nCntSkipCheck] != NULL)
		{
			m_pSkipCheckLogo[nCntSkipCheck]->Uninit();
			m_pSkipCheckLogo[nCntSkipCheck] = NULL;
		}
	}
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v�m�F��Ԃ̍X�V����
//=============================================================================
void CTutorial::SkipCheckUpdate(void)
{
	CDebugProc::Print(1, "�X�L�b�v�m�F���\n");
	CDebugProc::Print(1, "�I��ԍ� : %d\n", m_nSelectSkip);

	// ���̓N���X���擾
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	if (pKey == NULL || pXInput == NULL)return;

	if (pKey->GetTrigger(DIK_A) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetPress(0, CXInput::XIJS_BUTTON_18) == true ||
		pKey->GetRepeat(DIK_A) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_2) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_18) == true)
	{// �������̓��͂����ꂽ
		ChangeNoneSkipCheckPolygon(m_nSelectSkip);
		m_nSelectSkip = (m_nSelectSkip + (SKIP_MAX - 1)) % SKIP_MAX;
		ChangeSelectSkipCheckPolygon(m_nSelectSkip);
	}
	else if (pKey->GetTrigger(DIK_D) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_19) == true ||
		pKey->GetRepeat(DIK_D) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_3) == true ||
		pXInput->GetRepeat(0, CXInput::XIJS_BUTTON_19) == true)
	{// �E�����̓��͂����ꂽ
		ChangeNoneSkipCheckPolygon(m_nSelectSkip);
		m_nSelectSkip = (m_nSelectSkip + 1) % SKIP_MAX;
		ChangeSelectSkipCheckPolygon(m_nSelectSkip);
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		if (m_nSelectSkip == SKIP_YES)
		{// �͂���������
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
		}
		else if(m_nSelectSkip == SKIP_NO)
		{// ��������������
			SetState(STATE_NORMAL);
			ReleaseSkipLogo();
			ReleaseSkipCheckLogo();
			CScene::DeathCheck();
		}
	}
}

//=============================================================================
// �`���[�g���A���̒ʏ��Ԃ̍X�V����
//=============================================================================
void CTutorial::NormalUpdate(void)
{
	CDebugProc::Print(1, "�ʏ���\n");
}

//=============================================================================
// �`���[�g���A���̃S�[����Ԃ̍X�V����
//=============================================================================
void CTutorial::GoalUpdate(void)
{
	CDebugProc::Print(1, "�S�[�����\n");
}

//=============================================================================
// �`���[�g���A���̃}�b�v�ύX��Ԃ̍X�V����
//=============================================================================
void CTutorial::MapChangeUpdate(void)
{
	CDebugProc::Print(1, "�}�b�v�ύX���\n");
}

//=============================================================================
// �`���[�g���A���̏I����Ԃ̍X�V����
//=============================================================================
void CTutorial::EndUpdate(void)
{
	CDebugProc::Print(1, "�I�����\n");
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v�m�F�p�|���S����I������Ă��Ȃ���Ԃɂ���
//=============================================================================
void CTutorial::ChangeNoneSkipCheckPolygon(int nSelect)
{
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_NONE,  TUTORIAL_SKIPCHECKLOGO_1_COL_NONE };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE };
	if (m_pSkipCheckLogo[nSelect] != NULL)
	{
		m_pSkipCheckLogo[nSelect]->SetState(CScene2DFlash::STATE_NONE);
		m_pSkipCheckLogo[nSelect]->SetScale(fSkipCheckWidthLogo[nSelect], fSkipCheckHeightLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetCol(SkipCheckColLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v�m�F�p�|���S����I�����ꂽ��Ԃɂ���
//=============================================================================
void CTutorial::ChangeSelectSkipCheckPolygon(int nSelect)
{
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_COL_SELECT };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_SELECT };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_SELECT,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_SELECT };

	if (m_pSkipCheckLogo[nSelect] != NULL)
	{
		m_pSkipCheckLogo[nSelect]->SetState(CScene2DFlash::STATE_SELECT);
		m_pSkipCheckLogo[nSelect]->SetScale(fSkipCheckWidthLogo[nSelect], fSkipCheckHeightLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetCol(SkipCheckColLogo[nSelect]);
		m_pSkipCheckLogo[nSelect]->SetVtxBuffValue();
	}
}

//=============================================================================
// �`���[�g���A���̃V�X�e������ǂݍ���
//=============================================================================
void CTutorial::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(TUTORIAL_SYSTEM_FILENAME);
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
// �`���[�g���A���̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CTutorial::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntMap = 0;
	int nCntSkipCheck = 0;

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
		else if (CFunctionLib::Memcmp(pStr, NUM_MAP) == 0)
		{// �ǂݍ��ރ}�b�v�̐�������
			m_nNumMap = CFunctionLib::ReadInt(pStr, NUM_MAP);
			CreateMapFilePointer();
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// �ǂݍ��ރ}�b�v�t�@�C����������
			LoadMapFileName(pStr, nCntMap);
			nCntMap++;
		}
		else if (CFunctionLib::Memcmp(pStr, SKIPLOGOSET) == 0)
		{// �X�L�b�v���S�|���S����񂾂���
			LoadSkipLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, SKIPCHECKLOGOSET) == 0)
		{// �X�L�b�v�m�F���S�p�|���S����񂾂���
			LoadSkipCheckLogo(pFileLoader, pStr, nCntSkipCheck);
			nCntSkipCheck++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �`���[�g���A���̓ǂݍ��ރe�N�X�`���t�@�C������ǂݍ���
//=============================================================================
void CTutorial::LoadTexFileName(char *pStr, int nCntTex)
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
// �`���[�g���A���̓ǂݍ��ރ}�b�v�t�@�C������ǂݍ���
//=============================================================================
void CTutorial::LoadMapFileName(char *pStr, int nCntMap)
{
	strcpy(m_pMapFileName[nCntMap], CFunctionLib::ReadString(pStr, m_pMapFileName[nCntMap], MAP_FILENAME));
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v���S�|���S������ǂݍ���
//=============================================================================
void CTutorial::LoadSkipLogo(CFileLoader *pFileLoader, char *pStr)
{
	int nSkipLogoTexIdx = 0;
	D3DXVECTOR3 SkipLogoPos = TUTORIAL_SKIPLOGO_POS_INI;
	D3DXCOLOR SkipLogoCol = TUTORIAL_SKIPLOGO_COL_INI;
	float fSkipLogoWidth = TUTORIAL_SKIPLOGO_WIDTH_INI;
	float fSkipLogoHeight = TUTORIAL_SKIPLOGO_HEIGHT_INI;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nSkipLogoTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			SkipLogoPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			SkipLogoCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fSkipLogoWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fSkipLogoHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKIPLOGOSET) == 0)
		{// �X�L�b�v���S�|���S�����I���̍��}������
			break;
		}
	}

	// �|���S������
	m_pSkipLogo = CScene2D::Create(SkipLogoPos, SkipLogoCol, fSkipLogoWidth, fSkipLogoHeight);

	// �e�N�X�`�����蓖��
	if (m_pSkipLogo != NULL && GetTextureManager() != NULL)
	{
		m_pSkipLogo->BindTexture(GetTextureManager()->GetTexture(nSkipLogoTexIdx));
	}
}

//=============================================================================
// �`���[�g���A���̃X�L�b�v�m�F���S�p�|���S������ǂݍ���
//=============================================================================
void CTutorial::LoadSkipCheckLogo(CFileLoader *pFileLoader, char *pStr, int nCntSkipCheck)
{
	int nSkipCheckTexIdx[SKIP_MAX] = {};
	D3DXVECTOR3 SkipCheckPosLogo[SKIP_MAX] = {TUTORIAL_SKIPCHECKLOGO_0_POS_INI,  TUTORIAL_SKIPCHECKLOGO_1_POS_INI };
	D3DXCOLOR SkipCheckColLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_COL_NONE,  TUTORIAL_SKIPCHECKLOGO_1_COL_NONE };
	float fSkipCheckWidthLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_WIDTH_NONE,  TUTORIAL_SKIPCHECKLOGO_1_WIDTH_NONE };
	float fSkipCheckHeightLogo[SKIP_MAX] = { TUTORIAL_SKIPCHECKLOGO_0_HEIGHT_NONE,  TUTORIAL_SKIPCHECKLOGO_1_HEIGHT_NONE };

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			nSkipCheckTexIdx[nCntSkipCheck] = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			SkipCheckPosLogo[nCntSkipCheck] = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			SkipCheckColLogo[nCntSkipCheck] = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			fSkipCheckWidthLogo[nCntSkipCheck] = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			fSkipCheckHeightLogo[nCntSkipCheck] = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKIPCHECKLOGOSET) == 0)
		{// �X�L�b�v�m�F���S�p�|���S�����I���̍��}������
			break;
		}
	}

	// �|���S������
	m_pSkipCheckLogo[nCntSkipCheck] = CScene2DFlash::Create(SkipCheckPosLogo[nCntSkipCheck],
		SkipCheckColLogo[nCntSkipCheck], fSkipCheckWidthLogo[nCntSkipCheck],
		fSkipCheckHeightLogo[nCntSkipCheck], 0.0f, CScene2DFlash::STATE_NONE);
	if (nCntSkipCheck == 0)m_pSkipCheckLogo[nCntSkipCheck]->SetState(CScene2DFlash::STATE_SELECT);

	// �e�N�X�`�����蓖��
	if (m_pSkipCheckLogo[nCntSkipCheck] != NULL && GetTextureManager() != NULL)
	{
		m_pSkipCheckLogo[nCntSkipCheck]->BindTexture(GetTextureManager()->GetTexture(nSkipCheckTexIdx[nCntSkipCheck]));
	}
}

//=============================================================================
// �`���[�g���A���̕ϐ�������������
//=============================================================================
void CTutorial::ClearVariable(void)
{
	m_State = STATE_SKIP_CHECK;
	m_nStateCounter = 0;
	m_nSelectSkip = 0;
	m_pSkipLogo = NULL;
	m_nNumMap = 0;
	m_pMapFileName = NULL;
	for (int nCntSkip = 0; nCntSkip < SKIP_MAX; nCntSkip++)
	{
		m_pSkipCheckLogo[nCntSkip] = NULL;
	}
}

//=============================================================================
// �`���[�g���A���̏�Ԃ�ݒ肷��
//=============================================================================
void CTutorial::SetState(const STATE state)
{
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// �`���[�g���A���̏�Ԃ��擾����
//=============================================================================
CTutorial::STATE CTutorial::GetState(void)
{
	return m_State;
}