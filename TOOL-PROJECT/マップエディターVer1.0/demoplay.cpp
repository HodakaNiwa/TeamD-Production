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
#include "playerManager.h"
#include "characterManager.h"
#include "bullet.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "
#define DEMOPLAY_SYSTEM_FILENAME  "data/TEXT/demoplay.ini"    // �������Ɏg�p����V�X�e���t�@�C����

// �ǂݍ��ݗp�̃p�X
// ���f���p
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// �}�b�v�p
#define NUM_MAP "NUM_MAP = "

// �G�t�F�N�g�p
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// �e�p
#define BULLET_MODEL_IDX "BULLET_MODEL_IDX = "

// �A�C�e���p
#define ITEM_MODEL_IDX "ITEM_MODEL_IDX = "

// �A�C�e���C�x���g�p
#define ITEMEVENT_FILENAME "ITEMEVENT_FILENAME = "
#define ENEMY_STOP "ENEMY_STOP = "

// �v���C���[�f�[�^�p
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "

// �G�f�[�^�p
#define ENEMY_FILENAME "ENEMY_FILENAME = "


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
	CreatePlayer();

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

	// �e��N���X�̔j��
	ReleasePlayer();
	ReleasePlayerManager();
	ReleaseEnemyManager();

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


//*****************************************************************************
//
// �����p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
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
// �f���v���C�̃v���C���[��������
//=============================================================================
void CDemoplay::CreatePlayer(void)
{
	// �v���C���[�𐶐�����
	m_pPlayer = m_pPlayerManager->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, 0);
}

//=============================================================================
// �f���v���C�̓G��������
//=============================================================================
CEnemy *CDemoplay::CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData)
{
	CEnemy *pEnemy = m_pEnemyManager[pEnemyData->GetEnemyType()]->SetEnemy(pos, rot, pEnemyData->GetEnemyType());

	if (pEnemy == NULL)return NULL;
	// �A�C�e���̃f�[�^��ݒ肷��
	if (pEnemyData->GetItem() == true)
	{
		pEnemy->SetItemData(pEnemyData->GetItemType());
	}

	// AI�̃f�[�^��ݒ肷��
	pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());

	return pEnemy;
}

//=============================================================================
// �f���v���C�̃A�C�e���𐶐����鏈��
//=============================================================================
void CDemoplay::CreateItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nItemType)
{
	int nModelIdx = 0;
	switch (nItemType)
	{
	case CItem::TYPE_STAR:
		nModelIdx = m_nItemModelIdx[0];
		CItemStar::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_GRENADE:
		nModelIdx = m_nItemModelIdx[1];
		CItemGrenade::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_1UP_TANK:
		nModelIdx = m_nItemModelIdx[2];
		CItem1up::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_SCOOP:
		nModelIdx = m_nItemModelIdx[3];
		CItemScoop::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_CLOCK:
		nModelIdx = m_nItemModelIdx[4];
		CItemClock::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_HELMET:
		nModelIdx = m_nItemModelIdx[5];
		CItemHelmet::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	}
}




//*****************************************************************************
//
// �f���v���C���X�|�[�������p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
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
	CreateEnemy(EnemyPos, INITIALIZE_D3DXVECTOR3, pEnemyData);
}


//*****************************************************************************
//
// �J���p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �f���v���C�̃v���C���[�N���X���J������
//=============================================================================
void CDemoplay::ReleasePlayer(void)
{
	if (m_pPlayer != NULL)
	{
		m_pPlayer->Uninit();
		m_pPlayer = NULL;
	}
}

//=============================================================================
// �f���v���C�̃v���C���[�Ǌ��N���X���J������
//=============================================================================
void CDemoplay::ReleasePlayerManager(void)
{
	if (m_pPlayerManager != NULL)
	{
		m_pPlayerManager->Uninit();
		delete m_pPlayerManager;
		m_pPlayerManager = NULL;
	}
}

//=============================================================================
// �f���v���C�̓G�f�[�^�Ǌ��N���X���J������
//=============================================================================
void CDemoplay::ReleaseEnemyManager(void)
{
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		if (m_pEnemyManager[nCntEnemy] != NULL)
		{
			m_pEnemyManager[nCntEnemy]->Uninit();
			delete m_pEnemyManager[nCntEnemy];
			m_pEnemyManager[nCntEnemy] = NULL;
		}
	}
}


//*****************************************************************************
//
// �A�C�e���C�x���g�����p�֐�
// Auther : Hodaka Niwa & Jukiya Hayakawa
//
//*****************************************************************************
//=============================================================================
// �`���[�g���A���̃X�^�[�A�C�e���̃C�x���g����
//=============================================================================
void CDemoplay::ItemEvent_Star(int nPlayerNumber)
{
	// �v���C���[�̃p�����[�^���p���[�A�b�v������

}

//=============================================================================
// �`���[�g���A���̃O���l�[�h�A�C�e���̃C�x���g����
//=============================================================================
void CDemoplay::ItemEvent_Grenade(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �G������
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �`���[�g���A����1Up�A�C�e���̃C�x���g����
//=============================================================================
void CDemoplay::ItemEvent_1Up(int nPlayerNumber)
{
	// �v���C���[�̎c�@���𑝂₷
	m_nPlayerStock++;
}

//=============================================================================
// �`���[�g���A���̃X�R�b�v�A�C�e���̃C�x���g����
//=============================================================================
void CDemoplay::ItemEvent_Scoop(void)
{
	// �i�ߕ��̎���ɉ󂹂Ȃ��u���b�N��u��
}

//=============================================================================
// �`���[�g���A���̎��v�A�C�e���̃C�x���g����
//=============================================================================
void CDemoplay::ItemEvent_Clock(void)
{
	// �G�������Ȃ���Ԃɂ���
	m_bEnemyMove = false;
	m_nEnemyMoveCounter = m_ItemEventData.nStop;
}

//=============================================================================
// �`���[�g���A���̃w�����b�g�A�C�e���̃C�x���g����
//=============================================================================
void CDemoplay::ItemEvent_Helmet(int nPlayerNumber)
{
	// �v���C���[�𖳓G��Ԃɂ���
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
	int nCntMap = 0;
	int nCntTex = 0;
	int nCntModel = 0;
	int nCntItemModel = 0;
	int nCntEnemy = 0;
	int nCntPlayer = 0;

	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// �ǂݍ��ރe�N�X�`���̐�������
			int nNumTex = CFunctionLib::ReadInt(pStr, NUM_TEXTURE);
			CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
			SetTextureManager(pTextureManager);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// �ǂݍ��ރe�N�X�`���̃t�@�C����������
			LoadTexture(pStr, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{// �ǂݍ��ރ��f���̐�������
			int nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			CModelCreate *pModelCreate = CModelCreate::Create(nNumModel);
			SetModelCreate(pModelCreate);
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// �ǂݍ��ރ��f���̃t�@�C����������
			LoadModel(pStr, nCntModel);
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_FILENAME) == 0)
		{// �ǂݍ��ރG�t�F�N�g�̃t�@�C����������
			LoadEffectFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// �ǂݍ��ރ}�b�v�t�@�C����������
			strcpy(m_aMapFileName, CFunctionLib::ReadString(pStr, m_aMapFileName, MAP_FILENAME));
		}
		else if (CFunctionLib::Memcmp(pStr, ITEMEVENT_FILENAME) == 0)
		{// �A�C�e���C�x���g�p�X�N���v�g�t�@�C����������
			LoadItemEvent(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, BULLET_MODEL_IDX) == 0)
		{// �e���g�p���郂�f���̔ԍ�������
			m_nBulletModelIdx = CFunctionLib::ReadInt(pStr, BULLET_MODEL_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, ITEM_MODEL_IDX) == 0)
		{// �A�C�e�����g�p���郂�f���̔ԍ�������
			m_nItemModelIdx[nCntItemModel] = CFunctionLib::ReadInt(pStr, ITEM_MODEL_IDX);
			nCntItemModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERDATA_FILENAME) == 0)
		{// �v���C���[�f�[�^�̃X�N���v�g�t�@�C����������
			LoadPlayerFileName(pStr, nCntPlayer);
			nCntPlayer++;
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMY_FILENAME) == 0)
		{// �G�f�[�^�̃X�N���v�g�t�@�C����������
			LoadEnemyFileName(pStr, nCntEnemy);
			nCntEnemy++;
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
// �f���v���C�̓ǂݍ��ރA�C�e�����f����ǂݍ���
//=============================================================================
void CDemoplay::LoadModel(char *pStr, int nCntModel)
{
	// ���f���̃t�@�C���p�X����ǂݎ��
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat = 0;
	char aModelFileName[256] = "\0";
	strcpy(aModelFileName, CFunctionLib::ReadString(pStr, aModelFileName, MODEL_FILENAME));

	// x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(aModelFileName, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// ���f���Ǌ��N���X�ɒl��ݒ肷��
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL)return;
	pModelCreate->SetMesh(pMesh, nCntModel);
	pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);
	pModelCreate->SetFileName(pStr, nCntModel);
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
// �Q�[���̓ǂݍ��ރv���C���[�f�[�^�t�@�C������ǂݍ���
//=============================================================================
void CDemoplay::LoadPlayerFileName(char *pStr, int nCntPlayer)
{
	char aPlayerFileName[256] = "\0";
	strcpy(aPlayerFileName, CFunctionLib::ReadString(pStr, aPlayerFileName, PLAYERDATA_FILENAME));

	if (m_pPlayerManager != NULL)return;
	m_pPlayerManager = CPlayerManager::Create(aPlayerFileName);
}

//=============================================================================
// �Q�[���̓ǂݍ��ޓG�f�[�^�t�@�C������ǂݍ���
//=============================================================================
void CDemoplay::LoadEnemyFileName(char *pStr, int nCntEnemy)
{
	char aEnemyFileName[256] = "\0";
	strcpy(aEnemyFileName, CFunctionLib::ReadString(pStr, aEnemyFileName, ENEMY_FILENAME));

	// �L�����N�^�[�Ǌ��N���X���쐬
	m_pEnemyManager[nCntEnemy] = CCharacterManager::Create(aEnemyFileName);
}

//=============================================================================
// �f���v���C�̓ǂݍ��ރ}�b�v�t�@�C������ǂݍ���
//=============================================================================
void CDemoplay::LoadMapFileName(char *pStr)
{
	strcpy(m_aMapFileName, CFunctionLib::ReadString(pStr, m_aMapFileName, MAP_FILENAME));
}

//*****************************************************************************
//
// �A�C�e���C�x���g�p�X�N���v�g�ǂݍ��ݏ���
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �f���v���C�̃A�C�e���C�x���g�X�N���v�g��ǂݍ���
//=============================================================================
void CDemoplay::LoadItemEvent(char *pStr)
{
	char aFileName[256] = "\0";
	strcpy(aFileName, CFunctionLib::ReadString(pStr, aFileName, ITEMEVENT_FILENAME));
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(aFileName);
	if (pFileLoader != NULL)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// �ǂݍ��݊J�n�̍��}������
			LoadItemEventScript(pFileLoader, pStr);
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
// �f���v���C�̃A�C�e���C�x���g�����t�@�C������ǂݍ���
//=============================================================================
void CDemoplay::LoadItemEventScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, ENEMY_STOP) == 0)
		{// �G���ǂꂭ�炢�~�߂邩��񂾂���
			m_ItemEventData.nStop = CFunctionLib::ReadInt(pStr, ENEMY_STOP);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}


//*****************************************************************************
//
// �ݒ�A�擾���F�X�֐�(�O���Ƃ̑������܂߂Ă����ɏ����Ă܂�)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �f���v���C�̕ϐ����N���A����
//=============================================================================
void CDemoplay::ClearVariable(void)
{
	m_pUI = NULL;
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;
	m_nNumberTexIdx = 0;
	m_nPlayerRespawnCounter = 0;
	m_bEnemyMove = true;
	m_nEnemyMoveCounter = 0;
	m_pPlayer = NULL;
	m_pPlayerManager = NULL;
	m_nPlayerStock = 0;
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		m_pEnemyManager[nCntEnemy] = NULL;
	}
}


//=============================================================================
// �f���v���C�̒e���f����ݒ肷�鏈��
//=============================================================================
void CDemoplay::SetBulletModel(CBullet *pBullet)
{
	pBullet->BindModel(GetModelCreate()->GetMesh(m_nBulletModelIdx), GetModelCreate()->GetBuffMat(m_nBulletModelIdx),
		GetModelCreate()->GetNumMat(m_nBulletModelIdx), GetModelCreate()->GetTexture(m_nBulletModelIdx));
}