//=============================================================================
//
// �Q�[���̏��� [game.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "game.h"
#include "scene.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "meshfield.h"
#include "debuglog.h"
#include "fade.h"
#include "map.h"
#include "modelcreate.h"
#include "camera.h"
#include "cameraManager.h"
#include "player.h"


#include "server.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define GAME_MAP_FILENAME "data/TEXT/MAP/map000.txt"


//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================


//=============================================================================
// �Q�[���̃R���X�g���N�^
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// �Q�[���̃f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// �Q�[���̐�������
//=============================================================================
CGame *CGame::Create()
{
	CGame *pGame = NULL;	// �Q�[���̃|�C���^
	if (pGame == NULL)
	{
		pGame = new CGame;
		if (pGame != NULL)
		{
			pGame->Init();
		}
	}
	return pGame;
}

//=============================================================================
// �Q�[���̏���������
//=============================================================================
HRESULT CGame::Init(void)
{
	// �J�����̐���
	CreateCamera();

	//�}�b�v�𐶐�
	CreateMap();

	//���b�V���t�B�[���h�̐���
	CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASU_SIZE_X, MASU_SIZE_Z, 19, 15, 1, 1, NULL, true);

	// �v���C���[�̐���
	m_pPlayer[0] = CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, 0);
	m_pPlayer[1] = CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, 1);

	return S_OK;
}

//=============================================================================
// �Q�[���̏I������
//=============================================================================
void CGame::Uninit(void)
{
	// ���ʂ̏I������
	CBasemode::Uninit();

	//�S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();
}

//=============================================================================
// �Q�[���̍X�V����
//=============================================================================
void CGame::Update(void)
{
	CClient *pClient = CManager::GetClient();
	int nClientId = pClient->GetClientId();

	CScene::UpdateAll();

	// �v���C���[�̍��W��ݒ�
	m_pPlayer[(nClientId + 1) % 2]->SetPos(D3DXVECTOR3(pClient->GetPlayerPos(0), pClient->GetPlayerPos(1), pClient->GetPlayerPos(2)));

	// �f�o�b�O���\��
	Debug();
}

//=============================================================================
// �Q�[���̕`�揈��
//=============================================================================
void CGame::Draw(void)
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
// �Q�[���̃J������������
//=============================================================================
void CGame::CreateCamera(void)
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
// �Q�[���̃}�b�v��������
//=============================================================================
void CGame::CreateMap(void)
{
	CMap *pMap = CMap::Create(GAME_MAP_FILENAME);
	SetMap(pMap);
}

//=============================================================================
// �Q�[���̃f�o�b�O��񏈗�
//=============================================================================
void CGame::Debug(void)
{
	CClient *pClient = CManager::GetClient();
	int nClientId = pClient->GetClientId();

	// �N���C�A���g�ԍ��\��
	CDebugProc::Print("�����̃N���C�A���g�ԍ� : %d\n\n", nClientId);

	// �v���C���[�̏��\��
	D3DXVECTOR3 PlayerPos;
	D3DXVECTOR3 PlayerRot;
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		PlayerPos = m_pPlayer[nCnt]->GetPos();
		PlayerRot = m_pPlayer[nCnt]->GetRot();

		CDebugProc::Print("%d�Ԃ̃v���C���[�̈ʒu : %.1f %.1f %.1f\n", nCnt, PlayerPos.x, PlayerPos.y, PlayerPos.z);
		CDebugProc::Print("%d�Ԃ̃v���C���[�̌��� : %.1f %.1f %.1f\n", nCnt, PlayerRot.x, PlayerRot.y, PlayerRot.z);
	}
}

//=============================================================================
// �Q�[���̃v���C���[�擾
//=============================================================================
CPlayer *CGame::GetPlayer(int nIdx)
{
	return m_pPlayer[nIdx];
}