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

//=============================================================================
// �}�N����`
//=============================================================================
#define DEMOPLAY_MAP_FILENAME "data/TEXT/MAP/map000.txt"


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
	CMap *pMap = CMap::Create(DEMOPLAY_MAP_FILENAME);
	SetMap(pMap);
}