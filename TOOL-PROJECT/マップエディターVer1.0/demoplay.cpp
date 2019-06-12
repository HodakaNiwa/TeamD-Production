//=============================================================================
//
// �f���v���C���� [demoplay.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "demoplay.h"
#include "scene.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "meshfield.h"
#include "debugproc.h"
#include "fade.h"
#include "map.h"
#include "modelcreate.h"
#include "camera.h"
#include "cameraManager.h"
#include "player.h"
#include "enemy.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEMOPLAY_SYSTEM_FILENAME  "data/TEXT/demoplay.ini"    // �������Ɏg�p����V�X�e���t�@�C����

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f���v���C�̃R���X�g���N�^
//=============================================================================
CDemoplay::CDemoplay()
{

}

//=============================================================================
// �f���v���C�̃f�X�g���N�^
//=============================================================================
CDemoplay::~CDemoplay()
{

}

//=============================================================================
// �f���v���C�̐�������
//=============================================================================
CDemoplay *CDemoplay::Create()
{
	CDemoplay *pDemoplay = NULL;	//�f���v���C�̃|�C���^
	if (pDemoplay == NULL)
	{
		pDemoplay = new CDemoplay;	//���I�m��
		if (pDemoplay != NULL)
		{
			pDemoplay->Init();	//����������
		}
	}
	return pDemoplay;
}

//=============================================================================
// �f���v���C�̏���������
//=============================================================================
HRESULT CDemoplay::Init(void)
{
	// �V�X�e���̏�����
	LoadSystem();

	// �J�����̐���
	CreateCamera();

	//�}�b�v�𐶐�
	CreateMap();

	//���b�V���t�B�[���h�̐���
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASU_SIZE_X, MASU_SIZE_Z, 19, 15, 1, 1, NULL, true);

	// �v���C���[�̐���
	CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3);

	// �G�̐���
	CEnemy::Create(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), INITIALIZE_D3DXVECTOR3);

	return S_OK;
}

//=============================================================================
// �f���v���C�̏I������
//=============================================================================
void CDemoplay::Uninit(void)
{
	// ���ʂ̏I������
	CBasemode::Uninit();

	//�S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();
}

//=============================================================================
// �f���v���C�̍X�V����
//=============================================================================
void CDemoplay::Update(void)
{
	//�S�Ă̍X�V����
	CScene::UpdateAll();
}

//=============================================================================
// �f���v���C�̕`�揈��
//=============================================================================
void CDemoplay::Draw(void)
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
// �f���v���C�̃J������������
//=============================================================================
void CDemoplay::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager != NULL)
	{
		CCamera *pCamera = CCamera::Create();
		if (pCamera != NULL)
		{
			pCameraManager->SetCamera(pCamera);
		}
		SetCameraManager(pCameraManager);
	}
}

//=============================================================================
// �f���v���C�̃}�b�v��������
//=============================================================================
void CDemoplay::CreateMap(void)
{
	CMap *pMap = CMap::Create(m_aMapFileName);
	SetMap(pMap);
}

//=============================================================================
// �f���v���C�̃V�X�e������ǂݍ���
//=============================================================================
void CDemoplay::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(DEMOPLAY_SYSTEM_FILENAME);
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
// �f���v���C�̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CDemoplay::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// �}�b�v�t�@�C����������
			LoadMapFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �f���v���C�̓ǂݍ��ރ}�b�v�t�@�C������ǂݍ���
//=============================================================================
void CDemoplay::LoadMapFileName(char *pStr)
{
	strcpy(m_aMapFileName, CFunctionLib::ReadString(pStr, m_aMapFileName, MAP_FILENAME));
}

//=============================================================================
// �f���v���C�̃V�X�e������ۑ�����
//=============================================================================
void CDemoplay::SaveSystem(void)
{
	char aStr[256] = "\0";
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(DEMOPLAY_SYSTEM_FILENAME);
	if (pFileSaver != NULL)
	{
		// �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �f���v���C�V�X�e���t�@�C�� [demoplay.ini]\n");
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �ǂݍ��ރ}�b�v�̃t�@�C��������������
		SaveMapFileName(pFileSaver);

		// �X�N���v�g�I���̍��}����������
		pFileSaver->Print("%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}
}

//=============================================================================
// �f���v���C�̓ǂݍ��ރ}�b�v�t�@�C������ۑ�����
//=============================================================================
void CDemoplay::SaveMapFileName(CFileSaver *pFileSaver)
{
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �ǂݍ��ރ}�b�v�̃t�@�C����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("%s%s", MAP_FILENAME, m_aMapFileName);
	pFileSaver->Print("\n");
}