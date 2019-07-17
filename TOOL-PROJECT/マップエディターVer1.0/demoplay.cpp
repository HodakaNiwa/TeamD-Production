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
#include "textureManager.h"
#include "camera.h"
#include "cameraManager.h"
#include "player.h"
#include "enemy.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "UI.h"
#include "respawn.h"
#include "effectManager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "
#define DEMOPLAY_SYSTEM_FILENAME  "data/TEXT/demoplay.ini"    // �������Ɏg�p����V�X�e���t�@�C����

// �ǂݍ��ݗp�̃p�X
#define EFFECT_FILENAME "EFFECT_FILENAME = "

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
	CDemoplay *pDemoplay = NULL;
	if (pDemoplay == NULL)
	{
		pDemoplay = new CDemoplay;
		if (pDemoplay != NULL)
		{
			pDemoplay->Init();
		}
	}
	return pDemoplay;
}

//=============================================================================
// �f���v���C�̏���������
//=============================================================================
HRESULT CDemoplay::Init(void)
{
	// �ϐ��̏�����
	ClearVariable();

	// �V�X�e���̏�����
	LoadSystem();

	// ���łɋN�����Ă���Ȃ�}�b�v�̃t�@�C���������߂Đݒ�
	if (CManager::GetStartUp() == true)
	{
		strcpy(m_aMapFileName, CManager::GetLoadMapFileName());
	}

	// �J�����̐���
	CreateCamera();

	//�}�b�v�𐶐�
	CreateMap();

	// UI�𐶐�
	CreateUI();

	// �v���C���[�̐���
	CPlayer *pPlayer = CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3);
	pPlayer->SetPos(GetMap()->GetPlayerRespawn(0)->GetPos());

	// �G�𐶐����邩�`�F�b�N����
	CheckEnemySpawn(m_nGameCounter);

	return S_OK;
}

//=============================================================================
// �f���v���C�̏I������
//=============================================================================
void CDemoplay::Uninit(void)
{
	// UI�̔j��
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = NULL;
	}

	// ���ʂ̏I������
	CBasemode::Uninit();
}

//=============================================================================
// �f���v���C�̍X�V����
//=============================================================================
void CDemoplay::Update(void)
{
	// �J�E���^�[����
	m_nGameCounter++;

	// �S�Ă̍X�V����
	CScene::UpdateAll();

	// �G�𐶐����邩�`�F�b�N����
	CheckEnemySpawn(m_nGameCounter);
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
// �f���v���C�̕ϐ����N���A����
//=============================================================================
void CDemoplay::ClearVariable(void)
{
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;
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
// �f���v���C��UI��������
//=============================================================================
void CDemoplay::CreateUI(void)
{
	m_pUI = CUI::Create();
}

//=============================================================================
// �f���v���C�̓G�̐�������^�C�~���O���ǂ������肷��
//=============================================================================
void CDemoplay::CheckEnemySpawn(int nTime)
{
	CMap *pMap = GetMap();
	int nNumEnemyListData = pMap->GetNumEnemyListData();
	CEnemy_ListData *pEnemyData = NULL;

	// �����G���o�Ȃ��̂ł���΂��̎��_�ŏ����I��
	if (m_nSpawnEnemyCount == nNumEnemyListData) return;

	for (int nCnt = 0; nCnt < nNumEnemyListData; nCnt++)
	{// �G�𐶐����鐔���J��Ԃ�
		pEnemyData = pMap->GetEnemyListData(nCnt);
		if (pEnemyData->GetRespawnTime() == nTime)
		{// ���X�|�[������^�C�~���O�ł���
			EnemySpawn(pMap, pEnemyData, nCnt);
			m_nSpawnEnemyCount++;
		}
	}
}

//=============================================================================
// �f���v���C�̓G�̐�������
//=============================================================================
void CDemoplay::EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt)
{
	// �G�̃��X�|�[���ʒu���擾
	D3DXVECTOR3 EnemyPos = pMap->GetEnemyRespawn(pEnemyData->GetRespawnIdx())->GetPos();

	// �G�̐���(�����͌�Ŏ�ޕ������Ă���)
	CEnemy::Create(EnemyPos, INITIALIZE_D3DXVECTOR3);
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
	int nCntTex = 0;  // �e�N�X�`����ǂݍ��񂾐�

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// �ǂݍ��ރe�N�X�`���̐�������
			CreateTextureManager(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// �e�N�X�`���̃t�@�C����������
			LoadTexture(CFunctionLib::ReadString(pStr, pStr, TEXTURE_FILENAME), nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_FILENAME) == 0)
		{// �G�t�F�N�g�t�@�C����������
			LoadEffectFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
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
// �f���v���C�̃e�N�X�`���Ǌ��N���X�𐶐�����
//=============================================================================
void CDemoplay::CreateTextureManager(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// �f���v���C�̃e�N�X�`����ǂݍ���
//=============================================================================
void CDemoplay::LoadTexture(char *pFileName, int nCntTex)
{
	// �e�N�X�`����ǂݍ���
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pFileName, &pTexture);

	// �e�N�X�`���Ǌ��N���X�Ƀ|�C���^��ݒ�
	CTextureManager *pTextureManager = GetTextureManager();
	if (pTextureManager != NULL && pTexture != NULL)
	{
		pTextureManager->SetTexture(pTexture, nCntTex);
	}
}

//=============================================================================
// �G�f�B�^�[�̓ǂݍ��ރG�t�F�N�g�t�@�C������ǂݍ���
//=============================================================================
void CDemoplay::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// �G�t�F�N�g�Ǌ��N���X�̐���
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
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