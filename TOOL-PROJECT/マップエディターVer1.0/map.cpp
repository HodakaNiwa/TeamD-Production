//*****************************************************************************
//
//     �}�b�v�̏���[map.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "map.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "modelcreate.h"
#include "light.h"
#include "lightManager.h"
#include "block.h"
#include "boxCollider.h"
#include "scene.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
// �I�[�g�Z�[�u�p�}�N��
#define AUTOSAVE                                                // �I�[�g�Z�[�u�����邩�ǂ���(�錾�� : �I�[�g�Z�[�u)
#define AUTOSAVEFILENAME_MAP       "autosave_map.txt"           // �}�b�v���I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_GAMEFIELD "autosave_gamefield.txt"     // �Q�[���t�B�[���h���I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_MODELLIST "autosave_modellist.txt"     // ���f�����X�g���I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_TEXLIST   "autosave_texlist.txt"       // �e�N�X�`�����X�g���I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_LIGHT     "autosave_light.txt"         // ���C�g���I�[�g�Z�[�u�p�̃t�@�C����
#define AUTOSAVEFILENAME_OBJECT    "autosave_object.txt"        // �z�u�����I�[�g�Z�[�u�p�̃t�@�C����

// �X�N���v�g�ǂݍ��ݗp
#define BASE_POS "BASE_POS = "
#define PLAYER_RESPAWN "PLAYER_RESPAWN = "
#define ENEMY_RESPAWN "ENEMY_RESPAWN = "
#define MODELLIST_FILENAME "MODELLIST_FILENAME = "
#define TEXLIST_FILENAME "TEXLIST_FILENAME = "
#define MAPSET "MAPSET"
#define END_MAPSET "END_MAPSET"
#define LIGHT_FILENAME "LIGHT_FILENAME = "
#define GAMEFILED_FILENAME "GAMEFILED_FILENAME = "
#define OBJECT_FILENAME "OBJECT_FILENAME = "

// ���ʏ��
#define POS "POS = "
#define ROT "ROT = "

// ���f�����X�g���
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// �e�N�X�`�����X�g���
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// ���C�g���
#define NUM_LIGHT "NUM_LIGHT = "
#define DIRECTIONALLIGHTSET "DIRECTIONALLIGHTSET"
#define END_DIRECTIONALLIGHTSET "END_DIRECTIONALLIGHTSET"
#define POINTLIGHTSET "POINTLIGHTSET"
#define END_POINTLIGHTSET "END_POINTLIGHTSET"
#define SPOTLIGHTSET "SPOTLIGHTSET"
#define END_SPOTLIGHTSET "END_SPOTLIGHTSET"
#define DIFFUSE "DIFFUSE = "
#define AMBIENT "AMBIENT = "
#define SPECULAR "SPECULAR = "
#define ATTENUATION1 "ATTENUATION1 = "
#define ATTENUATION2 "ATTENUATION2 = "
#define ATTENUATION3 "ATTENUATION3 = "
#define RANGE "RANGE = "
#define FALLOFF "FALLOFF = "
#define THETA "THETA = "
#define PHI "PHI = "
#define DIR "DIR = "

// �u���b�N���
#define BLOCKSET "BLOCKSET"
#define END_BLOCKSET "END_BLOCKSET"
#define BLOCKTYPE "TYPE = "
#define MODELIDX "MODELIDX = "
#define BREAK "BREAK = "
#define COLLISION "COLLISION"
#define END_COLLISION "END_COLLISION"
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "
#define DEPTH "DEPTH = "

// ����
#define RIVERSET "RIVERSET"
#define END_RIVERSET "END_RIVERSET"

// �X���
#define ICESET "ICESET"
#define END_ICESET "END_ICESET"

// �z�u���f�����
#define MODELSET "MODELSET"
#define END_MODELSET "END_MODELSET"

// �z�u�r���{�[�h���
#define BILLBOARDSET "BILLBOARDSET"
#define END_BILLBOARDSET "END_BILLBOARDSET"

// �z�u�G�t�F�N�g���
#define EFFECTSET "EFFECTSET"
#define END_EFFECTSET "END_EFFECTSET"

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMap::CMap()
{
	strcpy(m_aFileName, "\0");             // �ǂݍ��ރ}�b�v�f�[�^�̃t�@�C����
	strcpy(m_aModelListFileName, "\0");    // �ǂݍ��ރ��f�����X�g���̃t�@�C����
	strcpy(m_aTexListFileName, "\0");      // �ǂݍ��ރe�N�X�`�����X�g���̃t�@�C����
	strcpy(m_aLightFileName, "\0");        // �ǂݍ��ރ��C�g���̃t�@�C����
	strcpy(m_aGameFieldFileName, "\0");    // �ǂݍ��ރQ�[���t�B�[���h���̃t�@�C����
	strcpy(m_aObjectFileName, "\0");       // �ǂݍ��ޔz�u�����̃t�@�C����
	m_pTextureManager = NULL;              // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_pModelCreate = NULL;                 // ���f���Ǌ��N���X�ւ̃|�C���^
	m_pLightManager = NULL;                // ���C�g�Ǌ��N���X�ւ̃|�C���^

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMap *CMap::Create(char *pFileName)
{
	CMap *pMap = NULL;  // �}�b�v�N���X�^�̃|�C���^
	if (pMap == NULL)
	{// ����������ɂȂ��Ă���
		pMap = new CMap;
		if (pMap != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pMap->Init(pFileName)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pMap;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMap::Init(char *pFileName)
{
	// �t�@�C�������R�s�[����
	strcpy(m_aFileName, pFileName);

	// �}�b�v�f�[�^��ǂݍ���
	if (FAILED(Load(m_aFileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMap::Uninit(void)
{
#ifdef AUTOSAVE
	// �I�[�g�Z�[�u�̃t�@�C������ݒ肷��
	strcpy(m_aGameFieldFileName, AUTOSAVEFILENAME_GAMEFIELD);
	strcpy(m_aModelListFileName, AUTOSAVEFILENAME_MODELLIST);
	strcpy(m_aTexListFileName, AUTOSAVEFILENAME_TEXLIST);
	strcpy(m_aLightFileName, AUTOSAVEFILENAME_LIGHT);
	strcpy(m_aObjectFileName, AUTOSAVEFILENAME_OBJECT);

	// �f�[�^��ۑ�����
	Save(AUTOSAVEFILENAME_MAP);
#endif

	// �e�N�X�`���Ǌ��N���X���J������
	ReleaseTextureManager();

	// ���f���Ǌ��N���X���J������
	ReleaseModelManager();

	// ���C�g�Ǌ��N���X���J������
	ReleaseLightManager();
}

//=============================================================================
//    �}�b�v�f�[�^�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::Load(char *pLoadFileName)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pLoadFileName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		char aStr[256];
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadScript(aStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �}�b�v�f�[�^�ۑ�����
//=============================================================================
HRESULT CMap::Save(char *pSaveFileName)
{
	char aStr[256] = SAVEFILENAME_MAP;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aStr, pSaveFileName));
	if (pFileSaver != NULL)
	{// �t�@�C���𐶐��ł���
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �}�b�v���X�N���v�g�t�@�C�� [%s]\n", pSaveFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �i�ߕ��̈ʒu��ۑ�
		SaveBasePos(pFileSaver);

		// �v���C���[�̃��X�|�[���ʒu��ۑ�
		SavePlayerRespawn(pFileSaver);

		// �G�̃��X�|�[���ʒu��ۑ�
		SaveEnemyRespawn(pFileSaver);

		// ���f�����̃X�N���v�g�t�@�C������ۑ�
		SaveModelList(pFileSaver);

		// �e�N�X�`�����̃X�N���v�g�t�@�C������ۑ�
		SaveTexList(pFileSaver);

		// �}�b�v����ۑ�
		SaveMap(pFileSaver);

		// �X�N���v�g�I���̍��}����������
		pFileSaver->Print("%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}

		// �Q�[���t�B�[���h����ۑ�
		SaveGameField(m_aGameFieldFileName);

		// ���f�����X�g����ۑ�
		SaveModel(m_aModelListFileName);

		// �e�N�X�`�����X�g����ۑ�
		SaveTexture(m_aTexListFileName);

		// ���C�g����ۑ�
		SaveLight(m_aLightFileName);

		// �z�u������ۑ�
		SaveObject(m_aObjectFileName);
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �}�b�v�؂�ւ�����
//=============================================================================
HRESULT CMap::ChangeMap(char *pLoadFileName)
{
	// ��������Ă���I�u�W�F�N�g�����ׂĔj������
	DeleteMap();

	// �V���Ƀ}�b�v�f�[�^��ǂݍ���
	strcpy(m_aFileName, pLoadFileName);       // �t�@�C�����R�s�[
	if (FAILED(Load(pLoadFileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}


//=============================================================================
//    �X�N���v�g���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadScript(char *pStr, CFileLoader *pFileLoader)
{
	int nCntLoadPlayerRes = 0;  // �v���C���[�̃��X�|�[���ʒu��ǂݍ��񂾉�
	int nCntLoadEnemyRes = 0;   // �G�̃��X�|�[���ʒu��ǂݍ��񂾉�
	char aStr[256];
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, BASE_POS) == 0)
		{// ��n�̈ʒu��񂾂���
			LoadBasePos(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYER_RESPAWN) == 0)
		{// �v���C���[�̃��X�|�[���ʒu��񂾂���
			LoadPlayerRespawn(pStr, nCntLoadPlayerRes);
			nCntLoadPlayerRes++;
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMY_RESPAWN) == 0)
		{// �G�̃��X�|�[���ʒu��񂾂���
			LoadEnemyRespawn(pStr, nCntLoadEnemyRes);
			nCntLoadEnemyRes++;
		}
		else if (CFunctionLib::Memcmp(pStr, MODELLIST_FILENAME) == 0)
		{// ���f�����X�g��񂾂���
			LoadModelList(CFunctionLib::ReadString(pStr, aStr, MODELLIST_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXLIST_FILENAME) == 0)
		{// �e�N�X�`�����X�g��񂾂���
			LoadTextureList(CFunctionLib::ReadString(pStr, aStr, TEXLIST_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, MAPSET) == 0)
		{// �}�b�v�Z�b�g��񂾂���
			LoadMap(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    ��n�̈ʒu�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadBasePos(char *pStr)
{
	m_BasePos = CFunctionLib::ReadVector3(pStr, BASE_POS);

	return S_OK;
}

//=============================================================================
//    �v���C���[�̃��X�|�[���ʒu(2�l��)�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadPlayerRespawn(char *pStr, int nCntPlayerRes)
{
	m_PlayerRespawn[nCntPlayerRes] = CFunctionLib::ReadVector3(pStr, PLAYER_RESPAWN);

	return S_OK;
}

//=============================================================================
//    �G�̃��X�|�[���ʒu(3�ӏ���)�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadEnemyRespawn(char *pStr, int nCntEnemyRes)
{
	// �G�̃��X�|�[���ʒu��ǂݍ���
	m_EnemyRespawn[nCntEnemyRes] = CFunctionLib::ReadVector3(pStr, ENEMY_RESPAWN);

	return S_OK;
}

//=============================================================================
//    ���f�����X�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadModelList(char *pModelFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pModelFileName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0 && m_pModelCreate == NULL)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadModel(pStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// �t�@�C�������R�s�[���Ă���
		strcpy(m_aModelListFileName, pModelFileName);
	}
	return S_OK;
}

//=============================================================================
//    ���f���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadModel(char *pStr, CFileLoader *pFileLoader)
{
	// ���f���ǂݍ��ݗp�ϐ��錾
	int nCntModel = 0;             // ���ݓǂݍ��񂾃��f����
	LPD3DXMESH pMesh = NULL;       // ���b�V���ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;  // �}�e���A�����ւ̃|�C���^
	DWORD nNumMat = 0;             // �}�e���A�����̐�

	while (1)
	{// ������܂Ń��[�v
		char aStr[256];
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{// �ǂݍ��ރ��f������񂪂�����
			int nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			m_pModelCreate = CModelCreate::Create(nNumModel);
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// �ǂݍ��ރ��f���̃t�@�C������񂪂�����
		    // ���f���̃t�@�C���p�X����ǂݎ��
			pStr = CFunctionLib::ReadString(pStr, aStr, MODEL_FILENAME);

			// x�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(pStr, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
				&pBuffMat, NULL, &nNumMat, &pMesh);

			// ���f���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
			m_pModelCreate->SetMesh(pMesh, nCntModel);
			m_pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

			// �t�@�C������ݒ肷��
			m_pModelCreate->SetFileName(pStr, nCntModel);

			// �g�p�����ϐ������������Ă���
			pMesh = NULL;
			pBuffMat = NULL;
			nNumMat = 0;

			// �J�E���^�[�𑝂₷
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}��������
			break;
		}
	}
	return S_OK;
}

//=============================================================================
//    �e�N�X�`�����X�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadTextureList(char *pTextureFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pTextureFileName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0 && m_pTextureManager == NULL)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadTexture(pStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// �t�@�C�������R�s�[���Ă���
		strcpy(m_aTexListFileName, pTextureFileName);
	}
	return S_OK;
}

//=============================================================================
//    �e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadTexture(char *pStr, CFileLoader *pFileLoader)
{
	// �e�N�X�`���ǂݍ��ݗp�ϐ��錾
	int nCntTex = 0;                      // ���ݓǂݍ��񂾃��f����
	LPDIRECT3DTEXTURE9 pTexture = NULL;   // �e�N�X�`���ւ̃|�C���^

	while (1)
	{// ������܂Ń��[�v
		char aStr[256];
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// �ǂݍ��ރe�N�X�`������񂪂�����
			int nNumTex = CFunctionLib::ReadInt(pStr, NUM_TEXTURE);
			m_pTextureManager = CTextureManager::Create(nNumTex);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// �ǂݍ��ރe�N�X�`���̃t�@�C������񂪂�����
		    // �e�N�X�`���̃t�@�C���p�X����ǂݎ��
			pStr = CFunctionLib::ReadString(pStr, aStr, TEXTURE_FILENAME);

			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);

			// �e�N�X�`���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
			m_pTextureManager->SetTexture(pTexture, nCntTex);

			// �t�@�C������ݒ肷��
			m_pTextureManager->SetFileName(pStr, nCntTex);

			// �g�p�����ϐ������������Ă���
			pTexture = NULL;

			// �J�E���^�[�𑝂₷
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��ݏI���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �}�b�v���ǂݍ��ݏ�����
//=============================================================================
HRESULT CMap::LoadMap(char *pStr, CFileLoader *pFileLoader)
{
	while (1)
	{// ������܂Ń��[�v
		char aStr[256];
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, LIGHT_FILENAME) == 0)
		{// ���C�g��񂾂���
			LoadLight(CFunctionLib::ReadString(pStr, aStr, LIGHT_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEFILED_FILENAME) == 0)
		{// �Q�[���t�B�[���h��񂾂���
			LoadGameField(CFunctionLib::ReadString(pStr, aStr, GAMEFILED_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, OBJECT_FILENAME) == 0)
		{// �z�u����񂾂���
			LoadObject(CFunctionLib::ReadString(pStr, aStr, OBJECT_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_MAPSET) == 0)
		{// �}�b�v�Z�b�g���I���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    ���C�g���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadLight(char *pLightFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pLightFileName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadLightInfo(pStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// �t�@�C�������R�s�[���Ă���
		strcpy(m_aLightFileName, pLightFileName);
	}

	return S_OK;
}

//=============================================================================
//    ���C�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadLightInfo(char *pStr, CFileLoader *pFileLoader)
{
	int nCntLight = 0;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_LIGHT) == 0)
		{// ���C�g�̐���񂾂���
			int nNumLight = CFunctionLib::ReadInt(pStr, NUM_LIGHT);
			m_pLightManager = CLightManager::Create(nNumLight);
		}
		else if (CFunctionLib::Memcmp(pStr, DIRECTIONALLIGHTSET) == 0)
		{// �f�B���N�V���i�����C�g��񂾂���
			LoadDirectionalLight(pStr, pFileLoader, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, POINTLIGHTSET) == 0)
		{// �|�C���g���C�g��񂾂���
			LoadPointLight(pStr, pFileLoader, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, SPOTLIGHTSET) == 0)
		{// �X�|�b�g���C�g��񂾂���
			LoadSpotLight(pStr, pFileLoader, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�I���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �f�B���N�V���i�����C�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadDirectionalLight(char *pStr, CFileLoader *pFileLoader, int nCntLight)
{
	D3DXVECTOR3 LightDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// ���C�g�̊g�U����񂾂���
			LightDiffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// ���C�g�̊�����񂾂���
			LightAmbient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// ���C�g�̔��ˌ���񂾂���
			LightSpecular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, DIR) == 0)
		{// ���C�g�̌�����񂾂���
			LightDir = CFunctionLib::ReadVector3(pStr, DIR);
		}
		else if (CFunctionLib::Memcmp(pStr, END_DIRECTIONALLIGHTSET) == 0)
		{// �f�B���N�V���i�����C�g���I���̍��}��������
			CDirectionalLight *pDirLight = CDirectionalLight::Create(LightDir, LightDiffuse,
				                                                     LightAmbient, LightSpecular);
			m_pLightManager->SettingLight(pDirLight, nCntLight);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �|�C���g���C�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadPointLight(char *pStr, CFileLoader *pFileLoader, int nCntLight)
{
	D3DXVECTOR3 LightPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	float fLightAttenuation0 = 0.0f;
	float fLightAttenuation1 = 0.0f;
	float fLightAttenuation2 = 0.0f;
	float fLightRange = 0.0f;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���C�g�̍��W��񂾂���
			LightPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// ���C�g�̊g�U����񂾂���
			LightDiffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// ���C�g�̊�����񂾂���
			LightAmbient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// ���C�g�̔��ˌ���񂾂���
			LightSpecular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION1) == 0)
		{// ���C�g�̐��`�����W����񂾂���
			fLightAttenuation0 = CFunctionLib::ReadFloat(pStr, ATTENUATION1);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION2) == 0)
		{// ���C�g��2�������W����񂾂���
			fLightAttenuation1 = CFunctionLib::ReadFloat(pStr, ATTENUATION2);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION3) == 0)
		{// ���C�g�̎w�������W����񂾂���
			fLightAttenuation2 = CFunctionLib::ReadFloat(pStr, ATTENUATION3);
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE) == 0)
		{// ���C�g�͈̔͏�񂾂���
			fLightRange = CFunctionLib::ReadFloat(pStr, RANGE);
		}
		else if (CFunctionLib::Memcmp(pStr, END_POINTLIGHTSET) == 0)
		{// �|�C���g���C�g���I���̍��}��������
			CPointLight *pPointLight = CPointLight::Create(LightPos, LightDiffuse, LightAmbient, LightSpecular,
				                                           fLightAttenuation0, fLightAttenuation1,
				                                           fLightAttenuation2, fLightRange);
			m_pLightManager->SettingLight(pPointLight, nCntLight);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �X�|�b�g���C�g�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadSpotLight(char *pStr, CFileLoader *pFileLoader, int nCntLight)
{
	D3DXVECTOR3 LightPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 LightDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	float fLightAttenuation0 = 0.0f;
	float fLightAttenuation1 = 0.0f;
	float fLightAttenuation2 = 0.0f;
	float fLightRange = 0.0f;
	float fLightFalloff = 0.0f;
	float fLightTheta = 0.0f;
	float fLightPhi = 0.0f;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���C�g�̍��W��񂾂���
			LightPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, DIR) == 0)
		{// ���C�g�̌�����񂾂���
			LightDir = CFunctionLib::ReadVector3(pStr, DIR);
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// ���C�g�̊g�U����񂾂���
			LightDiffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// ���C�g�̊�����񂾂���
			LightAmbient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// ���C�g�̔��ˌ���񂾂���
			LightSpecular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION1) == 0)
		{// ���C�g�̐��`�����W����񂾂���
			fLightAttenuation0 = CFunctionLib::ReadFloat(pStr, ATTENUATION1);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION2) == 0)
		{// ���C�g��2�������W����񂾂���
			fLightAttenuation1 = CFunctionLib::ReadFloat(pStr, ATTENUATION2);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION3) == 0)
		{// ���C�g�̎w�������W����񂾂���
			fLightAttenuation2 = CFunctionLib::ReadFloat(pStr, ATTENUATION3);
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE) == 0)
		{// ���C�g�͈̔͏�񂾂���
			fLightRange = CFunctionLib::ReadFloat(pStr, RANGE);
		}
		else if (CFunctionLib::Memcmp(pStr, FALLOFF) == 0)
		{// ���C�g�̃t�H�[���I�t��񂾂���
			fLightFalloff = CFunctionLib::ReadFloat(pStr, FALLOFF);
		}
		else if (CFunctionLib::Memcmp(pStr, THETA) == 0)
		{// ���C�g�̓����R�[���̊p�x��񂾂���
			fLightTheta = CFunctionLib::ReadFloat(pStr, THETA);
			D3DXToRadian(fLightTheta);
		}
		else if (CFunctionLib::Memcmp(pStr, PHI) == 0)
		{// ���C�g�̊O���R�[���̊p�x��񂾂���
			fLightPhi = CFunctionLib::ReadFloat(pStr, PHI);
			D3DXToRadian(fLightPhi);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SPOTLIGHTSET) == 0)
		{// �X�|�b�g���C�g���I���̍��}��������
			CSpotLight *pSpotLight = CSpotLight::Create(LightPos, LightDir, LightDiffuse, LightAmbient,
				                                        LightSpecular, fLightAttenuation0, fLightAttenuation1,
				                                        fLightAttenuation2, fLightRange,
				                                        fLightFalloff, fLightTheta, fLightPhi);
			m_pLightManager->SettingLight(pSpotLight, nCntLight);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �Q�[���t�B�[���h���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadGameField(char *pGameFieldFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pGameFieldFileName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadGameFieldInfo(pStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// �t�@�C�������R�s�[���Ă���
		strcpy(m_aGameFieldFileName, pGameFieldFileName);
	}

	return S_OK;
}

//=============================================================================
//    �Q�[���t�B�[���h�ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadGameFieldInfo(char *pStr, CFileLoader *pFileLoader)
{
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, BLOCKSET) == 0)
		{// �u���b�N��񂾂���
			LoadBlock(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�I���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �u���b�N���ǂݍ��ݏ���
//=============================================================================
void CMap::LoadBlock(char *pStr, CFileLoader *pFileLoader)
{
	int nBlockType = 0;
	int nBlockModelIdx = 0;
	D3DXVECTOR3 BlockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 BlockRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bBlockBreak = true;
	float fBlockColWidth = 0.0f;
	float fBlockColHeight = 0.0f;
	float fBlockColDepth = 0.0f;

	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, BLOCKTYPE) == 0)
		{// ��ޔԍ���񂾂���
			nBlockModelIdx = CFunctionLib::ReadInt(pStr, BLOCKTYPE);
		}
		else if (CFunctionLib::Memcmp(pStr, MODELIDX) == 0)
		{// �g�p���郂�f���̔ԍ���񂾂���
			nBlockModelIdx = CFunctionLib::ReadInt(pStr, MODELIDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ���W��񂾂���
			BlockPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// ������񂾂���
			BlockRot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, BREAK) == 0)
		{// �󂹂邩�ǂ�����񂾂���
			bBlockBreak = CFunctionLib::ReadBool(pStr, BREAK);
		}
		else if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// �����蔻�����񂾂���
			LoadCollision(pStr, pFileLoader, &fBlockColWidth, &fBlockColHeight, &fBlockColDepth);
		}
		else if (CFunctionLib::Memcmp(pStr, END_BLOCKSET) == 0)
		{// �u���b�N���I���̍��}��������
			CBlock::Create(BlockPos, BlockRot, (CBlock::TYPE)nBlockType, nBlockModelIdx, bBlockBreak,
				m_pModelCreate->GetMesh(nBlockModelIdx), m_pModelCreate->GetBuffMat(nBlockModelIdx),
				m_pModelCreate->GetNumMat(nBlockModelIdx), m_pModelCreate->GetTexture(nBlockModelIdx),
				fBlockColWidth, fBlockColHeight, fBlockColDepth);
			break;
		}
	}
}

//=============================================================================
//    �����蔻����ǂݍ��ݏ���
//=============================================================================
void CMap::LoadCollision(char *pStr, CFileLoader *pFileLoader, float *pWidth, float *pHeight, float *pDepth)
{
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �����蔻��̕���񂾂���
			*pWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �����蔻��̍�����񂾂���
			*pHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, DEPTH) == 0)
		{// �����蔻��̉��s��񂾂���
			*pDepth = CFunctionLib::ReadFloat(pStr, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStr, END_COLLISION) == 0)
		{// �u���b�N���I���̍��}��������
			break;
		}
	}
}

//=============================================================================
//    �X���ǂݍ��ݏ���
//=============================================================================
void CMap::LoadIce(char *pStr, CFileLoader *pFileLoader)
{

}

//=============================================================================
//    ����ǂݍ��ݏ���
//=============================================================================
void CMap::LoadRiver(char *pStr, CFileLoader *pFileLoader)
{

}

//=============================================================================
//    �z�u�����ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadObject(char *pObjectName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // �t�@�C���ǂݍ��ݗp�N���X�ւ̃|�C���^
	pFileLoader = CFileLoader::Create(pObjectName);
	if (pFileLoader != NULL)
	{// �t�@�C�����ǂݍ��߂�
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// �X�N���v�g�ǂݍ��݊J�n�̍��}��������
			LoadObjectInfo(pStr, pFileLoader);
		}

		// �������̊J��
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// �t�@�C�������R�s�[���Ă���
		strcpy(m_aObjectFileName, pObjectName);
	}

	return S_OK;
}

//=============================================================================
//    �z�u���ǂݍ��ݏ���
//=============================================================================
HRESULT CMap::LoadObjectInfo(char *pStr, CFileLoader *pFileLoader)
{
	while (1)
	{// ������܂Ń��[�v
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g���ǂݍ��ݏI���̍��}��������
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    �z�u���f�����ǂݍ��ݏ���
//=============================================================================
void CMap::LoadObjModel(char *pStr, CFileLoader *pFileLoader)
{

}

//=============================================================================
//    �z�u�r���{�[�h���ǂݍ��ݏ���
//=============================================================================
void CMap::LoadObjBill(char *pStr, CFileLoader *pFileLoader)
{

}

//=============================================================================
//    �z�u�G�t�F�N�g���ǂݍ��ݏ���
//=============================================================================
void CMap::LoadObjEffect(char *pStr, CFileLoader *pFileLoader)
{

}

//=============================================================================
//    ��n�̈ʒu���ۑ�����
//=============================================================================
HRESULT CMap::SaveBasePos(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �i�ߕ��̈ʒu\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �i�ߕ��̈ʒu����������
	pFileSaver->Print("%s%.1f %.1f %.1f\n\n", BASE_POS, m_BasePos.x, m_BasePos.y, m_BasePos.z);

	return S_OK;
}

//=============================================================================
//    �v���C���[�̃��X�|�[���ʒu���ۑ�����
//=============================================================================
HRESULT CMap::SavePlayerRespawn(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �v���C���[�̃��X�|�[���ʒu\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �v���C���[�̃��X�|�[���ʒu����������
	for (int nCntRes = 0; nCntRes < MAX_PLAYER_RESPAWN; nCntRes++)
	{
		pFileSaver->Print("%s%.1f %.1f %.1f\n", PLAYER_RESPAWN, m_PlayerRespawn[nCntRes].x, m_PlayerRespawn[nCntRes].y, m_PlayerRespawn[nCntRes].z);
	}
	pFileSaver->Print("\n");

	return S_OK;
}

//=============================================================================
//    �G�̃��X�|�[���ʒu���ۑ�����
//=============================================================================
HRESULT CMap::SaveEnemyRespawn(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �G�̃��X�|�[���ʒu\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �G�̃��X�|�[���ʒu����������
	for (int nCntRes = 0; nCntRes < MAX_ENEMY_RESPAWN; nCntRes++)
	{
		pFileSaver->Print("%s%.1f %.1f %.1f\n", ENEMY_RESPAWN, m_EnemyRespawn[nCntRes].x, m_EnemyRespawn[nCntRes].y, m_EnemyRespawn[nCntRes].z);
	}
	pFileSaver->Print("\n");

	return S_OK;
}

//=============================================================================
//    ���f�����X�g�̃t�@�C�������ۑ�����
//=============================================================================
HRESULT CMap::SaveModelList(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ���f�����̃X�N���v�g�t�@�C����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// ���f�����̃X�N���v�g�t�@�C��������������
	char aSaveFileName[256];
	strcpy(aSaveFileName, SAVEFILENAME_MODELLIST);
	pFileSaver->Print("%s%s\n\n", MODELLIST_FILENAME, strcat(aSaveFileName, m_aModelListFileName));

	return S_OK;
}

//=============================================================================
//    �e�N�X�`�����X�g�̃t�@�C�������ۑ�����
//=============================================================================
HRESULT CMap::SaveTexList(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �e�N�X�`�����̃X�N���v�g�t�@�C����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �e�N�X�`�����̃X�N���v�g�t�@�C��������������
	char aSaveFileName[256];
	strcpy(aSaveFileName, SAVEFILENAME_TEXLIST);
	pFileSaver->Print("%s%s\n\n", TEXLIST_FILENAME, strcat(aSaveFileName, m_aTexListFileName));

	return S_OK;
}

//=============================================================================
//    �}�b�v�Z�b�g���ۑ�����
//=============================================================================
HRESULT CMap::SaveMap(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �}�b�v���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	char aSaveLightFileName[256];
	char aSaveGameFieldFileName[256];
	char aSaveObjectFileName[256];
	strcpy(aSaveLightFileName, SAVEFILENAME_LIGHT);
	strcpy(aSaveGameFieldFileName, SAVEFILENAME_GAMEFIELD);
	strcpy(aSaveObjectFileName, SAVEFILENAME_OBJECT);
	pFileSaver->Print("%s\n", MAPSET);      // �}�b�v�Z�b�g���ǂݍ��݊J�n�̍��}����������
	pFileSaver->Print("	%s%s					# ���C�g���̃X�N���v�g�t�@�C����\n", LIGHT_FILENAME, strcat(aSaveLightFileName, m_aLightFileName));
	pFileSaver->Print("	%s%s		# �Q�[���t�B�[���h���̃X�N���v�g�t�@�C����\n", GAMEFILED_FILENAME, strcat(aSaveGameFieldFileName, m_aGameFieldFileName));
	pFileSaver->Print("	%s%s				# �z�u�����̃X�N���v�g�t�@�C����\n", OBJECT_FILENAME, strcat(aSaveObjectFileName, m_aObjectFileName));
	pFileSaver->Print("%s\n", END_MAPSET);  // �}�b�v�Z�b�g���ǂݍ��ݏI���̍��}����������

	return S_OK;
}

//=============================================================================
//    ���f�����ۑ�����
//=============================================================================
HRESULT CMap::SaveModel(char *pModelFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_MODELLIST;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pModelFileName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# ���f�����X�g���X�N���v�g�t�@�C�� [%s]\n", pModelFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �ǂݍ��ރ��f��������������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �ǂݍ��ރ��f����\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n\n", NUM_MODEL, m_pModelCreate->GetNumModel());

		// �ǂݍ��ރ��f���̃t�@�C��������������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �ǂݍ��ރ��f���̃t�@�C����\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		for (int nCntModel = 0; nCntModel < m_pModelCreate->GetNumModel(); nCntModel++)
		{// �ǂݍ��񂾃��f���̐������J��Ԃ�
			pFileSaver->Print("%s%s\n", MODEL_FILENAME, m_pModelCreate->GetFileName(nCntModel));
		}
		pFileSaver->Print("\n");

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

	return S_OK;
}

//=============================================================================
//    �e�N�X�`�����ۑ�����
//=============================================================================
HRESULT CMap::SaveTexture(char *pTextureFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_TEXLIST;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pTextureFileName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �e�N�X�`�����X�g���X�N���v�g�t�@�C�� [%s]\n", pTextureFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �ǂݍ��ރe�N�X�`��������������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �ǂݍ��ރe�N�X�`����\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n\n", NUM_TEXTURE, m_pTextureManager->GetNumTexture());

		// �ǂݍ��ރe�N�X�`���̃t�@�C��������������
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# �ǂݍ��ރe�N�X�`���̃t�@�C����\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		for (int nCntTex = 0; nCntTex < m_pTextureManager->GetNumTexture(); nCntTex++)
		{// �ǂݍ��񂾃e�N�X�`���̐������J��Ԃ�
			pFileSaver->Print("%s%s\n", TEXTURE_FILENAME, m_pTextureManager->GetFileName(nCntTex));
		}
		pFileSaver->Print("\n");

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

	return S_OK;
}

//=============================================================================
//    ���C�g���ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
HRESULT CMap::SaveLight(char *pLightFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_LIGHT;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pLightFileName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# ���C�g���X�N���v�g�t�@�C�� [%s]\n", pLightFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// ���C�g�̑���������������
		int nNumLight = 0;
		if (m_pLightManager != NULL)
		{
			nNumLight = m_pLightManager->GetNumLight();
		}
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# ���C�g�̑���\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n\n", NUM_LIGHT, nNumLight);

		// ���C�g������������
		SaveLightInfo(pFileSaver);

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

	return S_OK;
}

//=============================================================================
//    ���C�g���ۑ�����
//=============================================================================
void CMap::SaveLightInfo(CFileSaver *pFileSaver)
{
	CLight *pLight = NULL;
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ���C�g���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	if (m_pLightManager != NULL)
	{
		for (int nCntLight = 0; nCntLight < m_pLightManager->GetNumLight(); nCntLight++)
		{// ���C�g�̑������J��Ԃ�
			pLight = m_pLightManager->GetLight(nCntLight);
			if (pLight->GetType() == CLight::TYPE_DIRECTIONAL)
			{// �f�B���N�V���i�����C�g������
				SaveDirectionalLight((CDirectionalLight*)pLight, pFileSaver);
			}
			else if (pLight->GetType() == CLight::TYPE_POINT)
			{// �|�C���g���C�g������
				SavePointLight((CPointLight*)pLight, pFileSaver);
			}
			else if (pLight->GetType() == CLight::TYPE_SPOT)
			{// �X�|�b�g���C�g������
				SaveSpotLight((CSpotLight*)pLight, pFileSaver);
			}
		}
	}
	pFileSaver->Print("\n");
}

//=============================================================================
//    �f�B���N�V���i�����C�g���ۑ�����
//=============================================================================
void CMap::SaveDirectionalLight(CDirectionalLight *pDirLight, CFileSaver *pFileSaver)
{
	// �e������擾
	D3DXCOLOR LightDiffuse = pDirLight->GetLight().Diffuse;
	D3DXCOLOR LightAmbient = pDirLight->GetLight().Ambient;
	D3DXCOLOR LightSpecular = pDirLight->GetLight().Specular;
	D3DXVECTOR3 LightDir = pDirLight->GetLight().Direction;

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", DIRECTIONALLIGHTSET);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊g�U��\n", DIFFUSE, LightDiffuse.r, LightDiffuse.g, LightDiffuse.b, LightDiffuse.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊���\n", AMBIENT, LightAmbient.r, LightAmbient.g, LightAmbient.b, LightAmbient.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̔��ˌ�\n", SPECULAR, LightSpecular.r, LightSpecular.g, LightSpecular.b, LightSpecular.a);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ���C�g�̌���\n", DIR, LightDir.x, LightDir.y, LightDir.z);
	pFileSaver->Print("%s\n", END_DIRECTIONALLIGHTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �|�C���g���C�g���ۑ�����
//=============================================================================
void CMap::SavePointLight(CPointLight *pPointLight, CFileSaver *pFileSaver)
{
	// �e������擾
	D3DXVECTOR3 LightPos = pPointLight->GetLight().Position;
	D3DXCOLOR LightDiffuse = pPointLight->GetLight().Diffuse;
	D3DXCOLOR LightAmbient = pPointLight->GetLight().Ambient;
	D3DXCOLOR LightSpecular = pPointLight->GetLight().Specular;
	float fLightAttenuation0 = pPointLight->GetLight().Attenuation0;
	float fLightAttenuation1 = pPointLight->GetLight().Attenuation1;
	float fLightAttenuation2 = pPointLight->GetLight().Attenuation2;
	float fLightRange = pPointLight->GetLight().Range;

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", POINTLIGHTSET);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ���C�g�̍��W\n", POS, LightPos.x, LightPos.y, LightPos.z);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊g�U��\n", DIFFUSE, LightDiffuse.r, LightDiffuse.g, LightDiffuse.b, LightDiffuse.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊���\n", AMBIENT, LightAmbient.r, LightAmbient.g, LightAmbient.b, LightAmbient.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̔��ˌ�\n", SPECULAR, LightSpecular.r, LightSpecular.g, LightSpecular.b, LightSpecular.a);
	pFileSaver->Print("	%s%.3f					# ���C�g�̐��`�����W��\n", ATTENUATION1, fLightAttenuation0);
	pFileSaver->Print("	%s%.3f					# ���C�g�̂Q�������W��\n", ATTENUATION2, fLightAttenuation1);
	pFileSaver->Print("	%s%.3f					# ���C�g�̎w�������W��\n", ATTENUATION3, fLightAttenuation2);
	pFileSaver->Print("	%s%.3f							# ���C�g�͈̔�\n", RANGE, fLightRange);
	pFileSaver->Print("%s\n", END_POINTLIGHTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �X�|�b�g���C�g���ۑ�����
//=============================================================================
void CMap::SaveSpotLight(CSpotLight *pSpotLight, CFileSaver *pFileSaver)
{
	// �e������擾
	D3DXVECTOR3 LightPos = pSpotLight->GetLight().Position;
	D3DXVECTOR3 LightDir = pSpotLight->GetLight().Direction;
	D3DXCOLOR LightDiffuse = pSpotLight->GetLight().Diffuse;
	D3DXCOLOR LightAmbient = pSpotLight->GetLight().Ambient;
	D3DXCOLOR LightSpecular = pSpotLight->GetLight().Specular;
	float fLightAttenuation0 = pSpotLight->GetLight().Attenuation0;
	float fLightAttenuation1 = pSpotLight->GetLight().Attenuation1;
	float fLightAttenuation2 = pSpotLight->GetLight().Attenuation2;
	float fLightRange = pSpotLight->GetLight().Range;
	float fLightFalloff = pSpotLight->GetLight().Falloff;
	float fLightTheta = pSpotLight->GetLight().Theta;
	float fLighPhi = pSpotLight->GetLight().Phi;

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", SPOTLIGHTSET);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ���C�g�̍��W\n", POS, LightPos.x, LightPos.y, LightPos.z);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ���C�g�̌���\n", DIR, LightDir.x, LightDir.y, LightDir.z);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊g�U��\n", DIFFUSE, LightDiffuse.r, LightDiffuse.g, LightDiffuse.b, LightDiffuse.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̊���\n", AMBIENT, LightAmbient.r, LightAmbient.g, LightAmbient.b, LightAmbient.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ���C�g�̔��ˌ�\n", SPECULAR, LightSpecular.r, LightSpecular.g, LightSpecular.b, LightSpecular.a);
	pFileSaver->Print("	%s%.3f					# ���C�g�̐��`�����W��\n", ATTENUATION1, fLightAttenuation0);
	pFileSaver->Print("	%s%.3f					# ���C�g�̂Q�������W��\n", ATTENUATION2, fLightAttenuation1);
	pFileSaver->Print("	%s%.3f					# ���C�g�̎w�������W��\n", ATTENUATION3, fLightAttenuation2);
	pFileSaver->Print("	%s%.3f							# ���C�g�͈̔�\n", RANGE, fLightRange);
	pFileSaver->Print("	%s%.3f							# ���C�g�̃t�H�[���I�t\n", FALLOFF, fLightFalloff);
	pFileSaver->Print("	%s%.3f							# ���C�g�̓����R�[���̊p�x\n", THETA, fLightTheta);
	pFileSaver->Print("	%s%.3f							# ���C�g�̊O���R�[���̊p�x\n", PHI, fLighPhi);
	pFileSaver->Print("%s\n", END_SPOTLIGHTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �Q�[���t�B�[���h���ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
HRESULT CMap::SaveGameField(char *pGameFieldFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_GAMEFIELD;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pGameFieldFileName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �Q�[���t�B�[���h���X�N���v�g�t�@�C�� [%s]\n", pGameFieldFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �Q�[���t�B�[���h����ۑ�����
		SaveGameFieldInfo(pFileSaver);

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

	return S_OK;
}

//=============================================================================
//    �Q�[���t�B�[���h����ۑ����鏈��
//=============================================================================
void CMap::SaveGameFieldInfo(CFileSaver *pFileSaver)
{
	// �R�����g��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �u���b�N�z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CBlock *pBlock = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// �u���b�N�N���X������
				SaveBlock((CBlock*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �u���b�N�̏���ۑ����鏈��
//=============================================================================
void CMap::SaveBlock(CBlock *pBlock, CFileSaver *pFileSaver)
{
	// �e������擾
	int nBlockType = pBlock->GetType();
	int nBlockModelIdx = pBlock->GetModelIdx();
	D3DXVECTOR3 BlockPos = pBlock->GetPos();
	D3DXVECTOR3 BlockRot = D3DXToDegree(pBlock->GetRot());
	bool bBlockBreak = pBlock->GetBreak();
	float fBlockColWidth = pBlock->GetBoxCollider()->GetWidth();
	float fBlockColHeight = pBlock->GetBoxCollider()->GetHeight();
	float fBlockColDepth = pBlock->GetBoxCollider()->GetDepth();

	// �e������e�L�X�g�t�@�C���ɕۑ�
	pFileSaver->Print("%s\n", BLOCKSET);
	pFileSaver->Print("	%s%d				# ���\n", BLOCKTYPE, nBlockType);
	pFileSaver->Print("	%s%d			# �g�p���郂�f���̔ԍ�\n", MODELIDX, nBlockType);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ���W\n", POS, BlockPos.x, BlockPos.y, BlockPos.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# ����\n", ROT, BlockRot.x, BlockRot.y, BlockRot.z);
	pFileSaver->Print("	%s%d				# �󂹂邩�ǂ���\n", BREAK, (int)bBlockBreak);
	pFileSaver->Print("	%s\n", COLLISION);
	pFileSaver->Print("		%s%.1f		# �����蔻��̕�\n", WIDTH, fBlockColWidth);
	pFileSaver->Print("		%s%.1f		# �����蔻��̍���\n", HEIGHT, fBlockColHeight);
	pFileSaver->Print("		%s%.1f		# �����蔻��̉��s\n", DEPTH, fBlockColDepth);
	pFileSaver->Print("	%s\n", END_COLLISION);
	pFileSaver->Print("%s\n", END_BLOCKSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    �X���ۑ�����
//=============================================================================
void CMap::SaveIce(CFileSaver *pFileSaver)
{

}

//=============================================================================
//    ����ۑ�����
//=============================================================================
void CMap::SaveRiver(CFileSaver *pFileSaver)
{

}

//=============================================================================
//    �z�u�����ۑ��̂��߂̃t�@�C���I�[�v������
//=============================================================================
HRESULT CMap::SaveObject(char *pObjectName)
{
	char aSaveFileName[256] = SAVEFILENAME_OBJECT;
	CFileSaver *pFileSaver = NULL;  // �t�@�C���ۑ��p�N���X�ւ̃|�C���^
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pObjectName));
	if (pFileSaver != NULL)
	{// �t�@�C�����ǂݍ��߂�
	    // �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �z�u�����X�N���v�g�t�@�C�� [%s]\n", pObjectName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �z�u��������������
		SaveObjectInfo(pFileSaver);

		// �X�N���v�g�ǂݍ��ݏI���̍��}����������
		pFileSaver->Print("%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}
	return S_OK;
}

//=============================================================================
//    �z�u�����ۑ�����
//=============================================================================
void CMap::SaveObjectInfo(CFileSaver *pFileSaver)
{
	// ���f���z�u������������
	SaveObjModel(pFileSaver);

	// �r���{�[�h�z�u������������
	SaveObjBill(pFileSaver);

	// �G�t�F�N�g�z�u������������
	SaveObjEffect(pFileSaver);
}

//=============================================================================
//    �z�u���f�����ۑ�����
//=============================================================================
void CMap::SaveObjModel(CFileSaver *pFileSaver)
{
	// �`��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ���f���z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �z�u���f��������������

	pFileSaver->Print("\n");
}

//=============================================================================
//    �z�u�r���{�[�h���ۑ�����
//=============================================================================
void CMap::SaveObjBill(CFileSaver *pFileSaver)
{
	// �`��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �r���{�[�h�z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �z�u�r���{�[�h������������

	pFileSaver->Print("\n");
}

//=============================================================================
//    �z�u�G�t�F�N�g���ۑ�����
//=============================================================================
void CMap::SaveObjEffect(CFileSaver *pFileSaver)
{
	// �`��������������
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �G�t�F�N�g�z�u���\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// �z�u�G�t�F�N�g������������

	pFileSaver->Print("\n");
}

//=============================================================================
//    �}�b�v��j�����鏈��
//=============================================================================
void CMap::DeleteMap(void)
{
	// ���C�g��j��
	DeleteLight();

	// �Q�[���t�B�[���h�I�u�W�F�N�g��j��
	DeleteGameField();
}

//=============================================================================
//    ���C�g��j�����鏈��
//=============================================================================
void CMap::DeleteLight(void)
{
	if (m_pLightManager != NULL)
	{
		// �`��f�o�C�X�̃��C�g��OFF�ɂ���
		for (int nCntLight = 0; nCntLight < m_pLightManager->GetNumLight(); nCntLight++)
		{
			CManager::GetRenderer()->GetDevice()->LightEnable(nCntLight, false);
		}
	}

	// ���C�g�Ǌ��N���X��j������
	ReleaseLightManager();
}

//=============================================================================
//    �Q�[���t�B�[���h�ɕK�v�ȃI�u�W�F�N�g��j�����鏈��
//=============================================================================
void CMap::DeleteGameField(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CBlock *pBlock = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// �u���b�N�N���X������
				DeleteBlock((CBlock*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �u���b�N��j�����鏈��
//=============================================================================
void CMap::DeleteBlock(CBlock *pBlock)
{
	pBlock->Uninit();
	pBlock = NULL;
}

//=============================================================================
//    �z�u���ɕK�v�ȃI�u�W�F�N�g��j�����鏈��
//=============================================================================
void CMap::DeleteObject(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CBlock *pBlock = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			//if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			//{// �u���b�N�N���X������
			//	DeleteBlock((CBlock*)pScene);
			//}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    �z�u���f����j�����鏈��
//=============================================================================
void CMap::DeleteObjModel(void)
{

}

//=============================================================================
//    �z�u�r���{�[�h��j�����鏈��
//=============================================================================
void CMap::DeleteObjBillboard(void)
{

}

//=============================================================================
//    �z�u�G�t�F�N�g��j�����鏈��
//=============================================================================
void CMap::DeleteObjEffect(void)
{

}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X���J�����鏈��
//=============================================================================
void CMap::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    ���f���Ǌ��N���X���J�����鏈��
//=============================================================================
void CMap::ReleaseModelManager(void)
{
	if (m_pModelCreate != NULL)
	{
		m_pModelCreate->Uninit();
		delete m_pModelCreate;
		m_pModelCreate = NULL;
	}
}

//=============================================================================
//    ���C�g�Ǌ��N���X���J�����鏈��
//=============================================================================
void CMap::ReleaseLightManager(void)
{
	if (m_pLightManager != NULL)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = NULL;
	}
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CTextureManager *CMap::GetTextureManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CModelCreate *CMap::GetModelCreate(void)
{
	return m_pModelCreate;
}

//=============================================================================
//    ���C�g�Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CLightManager *CMap::GetLightManager(void)
{
	return m_pLightManager;
}

//=============================================================================
//    �ǂݍ��ރ��f�����X�g���̃X�N���v�g�t�@�C�������擾����
//=============================================================================
char *CMap::GetModelListFileName(void)
{
	return m_aModelListFileName;
}

//=============================================================================
//    �ǂݍ��ރe�N�X�`�����X�g���̃X�N���v�g�t�@�C�������擾����
//=============================================================================
char *CMap::GetTexListFileName(void)
{
	return m_aTexListFileName;
}

//=============================================================================
//    �ǂݍ��ރ��C�g���̃X�N���v�g�t�@�C�������擾����
//=============================================================================
char *CMap::GetLightFileName(void)
{
	return m_aLightFileName;
}

//=============================================================================
//    �ǂݍ��ރQ�[���t�B�[���h���̃X�N���v�g�t�@�C�������擾����
//=============================================================================
char *CMap::GetGameFieldFileName(void)
{
	return m_aGameFieldFileName;
}

//=============================================================================
//    �ǂݍ��ޔz�u�����̃X�N���v�g�t�@�C�������擾����
//=============================================================================
char *CMap::GetObjectFileName(void)
{
	return m_aObjectFileName;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetTextureManager(CTextureManager *pTextureManager)
{
	m_pTextureManager = pTextureManager;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetModelCreate(CModelCreate *pModelCreate)
{
	m_pModelCreate = pModelCreate;
}

//=============================================================================
//    ���C�g�Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CMap::SetLightManager(CLightManager *pLightManager)
{
	m_pLightManager = pLightManager;
}

//=============================================================================
//    �ǂݍ��ރ��f�����X�g���̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetModelListFileName(char *pFileName)
{
	strcpy(m_aModelListFileName, pFileName);
}

//=============================================================================
//    �ǂݍ��ރe�N�X�`�����X�g���̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetTexListFileName(char *pFileName)
{
	strcpy(m_aTexListFileName, pFileName);
}

//=============================================================================
//    �ǂݍ��ރ��C�g���̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetLightFileName(char *pFileName)
{
	strcpy(m_aLightFileName, pFileName);
}

//=============================================================================
//    �ǂݍ��ރQ�[���t�B�[���h���̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetGameFieldFileName(char *pFileName)
{
	strcpy(m_aGameFieldFileName, pFileName);
}

//=============================================================================
//    �ǂݍ��ޔz�u�����̃X�N���v�g�t�@�C������ݒ肷��
//=============================================================================
void CMap::SetObjectFileName(char *pFileName)
{
	strcpy(m_aObjectFileName, pFileName);
}