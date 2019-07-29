//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "map.h"
#include "modelcreate.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "input.h"
#include "fade.h"
#include "UI.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "respawn.h"
#include "item.h"
#include "scene2D.h"
#include "server.h"
#include "sound.h"
#include "charaselect.h"
#include "effectManager.h"
#include "characterManager.h"
#include "playerManager.h"
#include "blossoms.h"
#include "title.h"
#include "debugproc.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define GAME_DEBUG                                                    // �錾���f�o�b�O�R�}���h�K�p

#ifdef GAME_DEBUG
#define GAME_DEBUG_STARTMAP (CGame::MAPTYPE_HINAMATSURI)
#endif

#define GAME_SYSTEM_FILENAME              "data/TEXT/MODE/game.ini"    // �������Ɏg�p����V�X�e���t�@�C����
#define GAME_STAGEDISP_TIME               (120)                        // �X�e�[�W�ԍ��\����ԂɂȂ��Ă���ʏ��Ԃɐ؂�ւ��܂ł̎���
#define GAME_RESULT_TIMING                (60)                         // �I����ԂɂȂ��Ă��烊�U���g��Ԃɐ؂�ւ��܂ł̎���
#define GAME_NEXTMODE_TIMING              (180)                        // ���U���g��ԂɂȂ��Ă��玟�̃��[�h�ɐ؂�ւ��܂ł̎���
#define GAME_GAMEOVER_TIMING              (180)                        // �Q�[���I�[�o�[��ԂɂȂ��Ă��玟�̃��[�h�ɐ؂�ւ��܂ł̎���
#define GAME_GAMEOVER_POSUP               (8.0f)                       // �Q�[���I�[�o�[���S���ǂꂭ�炢�グ�邩
#define GAME_GAMEOVER_POSYMAX             (10.0f)                      // �Q�[���I�[�o�[�|���S����Y���W�ő�l(�|���S���̍����ɉ����Ĕ���)
#define GAME_NUMBER_INTERVAL_CUT          (0.8f)                       // �����̊Ԋu�̔{��(�|���S���̕�������ŏ�Z����)
#define GAME_PLAYER_STOCK_INI             (3)                          // �v���C���[�̎c�@�������l
#define GAME_PLAYER_RESPAWN_TIMING        (180)                        // �v���C���[�����X�|�[������܂ł̎���
#define GAME_MAPSTAGE_MAX                 (3)                          // �X�e�[�W�̎�ނ��Ƃ̐�
#define GAME_STAGEPOLYGON_PRIORITY        (6)                          // �X�e�[�W�|���S���̕`��D�揇��
#define GAME_GAMEOVERPOLYGON_PRIORITY     (6)                          // �Q�[���I�[�o�[�|���S���̕`��D�揇��
#define GAME_PLAYER_SPAWN_EFFECT_IDX      (11)                         // �v���C���[�����X�|�[������Ƃ��̃G�t�F�N�g
#define GAME_SE_FANFARE_IDX               (5)                          // �Q�[���ŃX�e�[�W���n�܂�Ƃ��̉�
#define GAME_BGM_HINAMAP_IDX              (3)                          // �Q�[����BGM(�ЂȍՂ�}�b�v)

// �A�C�e���C�x���g�p�}�N��
#define GAME_ITEMEVENT_GRENADESHAKE_TIME  (10)                         // �O���l�[�h����������ɃJ�������ǂꂭ�炢�h�炵�����邩
#define GAME_ITEMEVENT_GRENADESHAKE_VALUE (40.0f)                      // �O���l�[�h����������ɃJ�������ǂꂭ�炢�h�炷��
#define GAME_ITEMEVENT_GRENADESHAKE_CUT   (3.0f)                       // �J�����̗h����ǂꂭ�炢���炵�Ă�����
#define GAME_ITEMEVENT_ENEMYSTOPTIME      (300)                        // �G���ǂꂭ�炢�~�߂Ă�����

// �}�b�v�X�V���ɏ������I�u�W�F�N�g(����p�̃}�N���Ȃ̂ł����ɒǉ�)
#define GAME_DELETE_OBJECT (pScene->GetObjType() == CScene::OBJTYPE_ITEM || pScene->GetObjType() == CScene::OBJTYPE_BULLET || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER || pScene->GetObjType() == CScene::OBJTYPE_EMITTER || pScene->GetObjType() == CScene::OBJTYPE_BLOSSOMS)

// �X�e�[�W�w�i�|���S���������p
#define GAME_STAGEBG_POS_INI             (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGEBG_COL_INI             (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))
#define GAME_STAGEBG_WIDTH_INI           (SCREEN_WIDTH / 2.0f)
#define GAME_STAGEBG_HEIGHT_INI          (SCREEN_HEIGHT / 2.0f)

// �X�e�[�W���S�|���S���������p
#define GAME_STAGELOGO_POS_INI           (D3DXVECTOR3(500.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGELOGO_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_STAGELOGO_WIDTH_INI         (170.0f)
#define GAME_STAGELOGO_HEIGHT_INI        (60.0f)

// �X�e�[�W�ԍ��\���p�|���S���������p
#define GAME_STAGENUMBER_POS_INI         (D3DXVECTOR3(750.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGENUMBER_COL_INI         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_STAGENUMBER_WIDTH_INI       (40.0f)
#define GAME_STAGENUMBER_HEIGHT_INI      (40.0f)

// �Q�[���I�[�o�[���S�|���S���������p
#define GAME_GAMEOVERLOGO_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 900.0f, 0.0f))
#define GAME_GAMEOVERLOGO_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMEOVERLOGO_WIDTH_INI      (80.0f)
#define GAME_GAMEOVERLOGO_HEIGHT_INI     (80.0f)

// ���̉Ԃт�f�[�^�������p
#define GAME_BLOSSOMS_POS_INI            (D3DXVECTOR3(SCREEN_WIDTH - UI_BG_WIDTH_INI + 100.0f, -100.0f, 0.0f))
#define GAME_BLOSSOMS_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_BLOSSOMS_TIME_INI           (300)
#define GAME_BLOSSOMS_APPEAR_INI         (3)
#define GAME_BLOSSOMS_MOVEX_MAX_INI      (15)
#define GAME_BLOSSOMS_MOVEX_MIN_INI      (5)
#define GAME_BLOSSOMS_MOVEY_MAX_INI      (10)
#define GAME_BLOSSOMS_MOVEY_MIN_INI      (2)
#define GAME_BLOSSOMS_WIDTH_MAX_INI      (20)
#define GAME_BLOSSOMS_WIDTH_MIN_INI      (10)
#define GAME_BLOSSOMS_HEIGHT_MAX_INI     (20)
#define GAME_BLOSSOMS_HEIGHT_MIN_INI     (10)
#define GAME_BLOSSOMS_ANGLESPEED_MAX_INI (5)
#define GAME_BLOSSOMS_ANGLESPEED_MIN_INI (0)
#define GAME_BLOSSOMS_ROTSPEED_MAX_INI   (5)
#define GAME_BLOSSOMS_ROTSPEED_MIN_INI   (0)

// �Q�[�������U���g�p�w�i�|���S���������p
#define GAME_GAMERESULT_BG_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_GAMERESULT_BG_COL_INI        (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))
#define GAME_GAMERESULT_BG_WIDTH_INI      (SCREEN_WIDTH / 2.0f)
#define GAME_GAMERESULT_BG_HEIGHT_INI     (SCREEN_HEIGHT / 2.0f)
#define GAME_GAMERESULT_BG_PRIORITY       (6)

// �Q�[�������U���g�p�n�C�X�R�A���S�|���S���������p
#define GAME_GAMERESULT_HIGHSCORELOGO_POS_INI        (D3DXVECTOR3(300.0f, 50.0f, 0.0f))
#define GAME_GAMERESULT_HIGHSCORELOGO_COL_INI        (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define GAME_GAMERESULT_HIGHSCORELOGO_WIDTH_INI      (200.0f)
#define GAME_GAMERESULT_HIGHSCORELOGO_HEIGHT_INI     (30.0f)
#define GAME_GAMERESULT_HIGHSCORELOGO_PRIORITY       (6)

// �Q�[�������U���g�p�X�e�[�W���S�|���S���������p
#define GAME_GAMERESULT_STAGELOGO_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 120.0f, 0.0f))
#define GAME_GAMERESULT_STAGELOGO_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_STAGELOGO_WIDTH_INI      (150.0f)
#define GAME_GAMERESULT_STAGELOGO_HEIGHT_INI     (30.0f)
#define GAME_GAMERESULT_STAGELOGO_PRIORITY       (6)

// �l�ǂݍ��ݗp�̃p�X
// �e�N�X�`���p
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// ���f���p
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// �}�b�v�p
#define NUM_MAP "NUM_MAP = "

// �A�C�e���C�x���g�p
#define ITEMEVENT_FILENAME "ITEMEVENT_FILENAME = "
#define SHAKESET "SHAKESET"
#define END_SHAKESET "END_SHAKESET"
#define TIME "TIME = "
#define VALUE "VALUE = "
#define CUT "CUT = "
#define ENEMY_STOP "ENEMY_STOP = "

// �G�t�F�N�g�p
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// �e�p
#define BULLET_MODEL_IDX "BULLET_MODEL_IDX = "

// �A�C�e���p
#define ITEM_MODEL_IDX "ITEM_MODEL_IDX = "

// �v���C���[�f�[�^�p
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "

// �G�f�[�^�p
#define ENEMY_FILENAME "ENEMY_FILENAME = "

// �w�i�|���S���p
#define BGPOLYGONSET "BGPOLYGONSET"
#define END_BGPOLYGONSET "END_BGPOLYGONSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// �����e�N�X�`���p
#define NUMBER_TEX_IDX "NUMBER_TEX_IDX = "

// �X�e�[�W�w�i�|���S���p
#define STAGEBGSET "STAGEBGSET"
#define END_STAGEBGSET "END_STAGEBGSET"

// �X�e�[�W���S�|���S���p
#define STAGELOGOSET "STAGELOGOSET"
#define END_STAGELOGOSET "END_STAGELOGOSET"

// �X�e�[�W�ԍ��|���S���p
#define STAGENUMBERSET "STAGENUMBERSET"
#define END_STAGENUMBERSET "END_STAGENUMBERSET"

// �Q�[���I�[�o�[���S�|���S���p
#define GAMEOVERPOLYGONSET "GAMEOVERPOLYGONSET"
#define END_GAMEOVERPOLYGONSET "END_GAMEOVERPOLYGONSET"

// �ЂȍՂ�C�x���g�p
#define HINAMATSURI_FILENAME "HINAMATSURI_FILENAME = "

// ���̉Ԃт�f�[�^�p
#define CHERRYBLOSSOMSSET "CHERRYBLOSSOMSSET"
#define END_CHERRYBLOSSOMSSET "END_CHERRYBLOSSOMSSET"
#define TIME "TIME = "
#define APPEAR "APPEAR = "
#define MOVEX "MOVEX = "
#define MOVEY "MOVEY = "
#define ANGLESPEED "ANGLESPEED = "
#define ROTSPEED "ROTSPEED = "

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
	CGame *pGame = NULL;
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
	// �ϐ�������������
	ClearVariable();

	// �}�b�v�̔ԍ���ݒ肷��
#ifdef GAME_DEBUG
	CCharaSelect::SetStageType(GAME_DEBUG_STARTMAP);
#endif
	m_nMapIdx = CCharaSelect::GetStageType() * GAME_MAPSTAGE_MAX;

	// �V�X�e���̏�����
	LoadSystem();

	// �J�����̐���
	CreateCamera();

	// UI�𐶐�
	CreateUI();

	// �v���C���[�̐���
	CreatePlayer();

	// �X�e�[�W�ԍ��̌������v�Z����
	CalcStageDigits();

	// �}�b�v�̐���
	CreateMap();

	// �c�@����ݒ�(��ŊO���t�@�C����)
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nPlayerStock[nCntPlayer] = GAME_PLAYER_STOCK_INI;
	}

	// �t�@���t�@�[�����Đ�
	CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);

	return S_OK;
}

//=============================================================================
// �Q�[���̏I������
//=============================================================================
void CGame::Uninit(void)
{
	// �e��|�C���^�̊J��
	ReleaseUI();
	ReleasePlayer();
	ReleasePlayerManager();
	ReleaseEnemyManager();
	ReleaseGameOverLogo();
	ReleaseStageBg();
	ReleaseStageLogo();
	ReleaseStageNumber();
	ReleaseGameResult();

	// �ǂݍ��ރ}�b�v�̃t�@�C�����̃|�C���^���J��
	ReleaseMapFilePointer();

	// ���ʂ̏I������
	CBasemode::Uninit();

	// �S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();

	// �N���C�A���g���J������
	CManager::ReleaseClient();
}

//=============================================================================
// �Q�[���̍X�V����
//=============================================================================
void CGame::Update(void)
{
	// �J�����̍X�V����
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->Update();
	}

	// ���[�J��1P�v���C�Ȃ�΂�����l�̃v���C���[�������Ă���
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		if (m_pPlayer[MAX_NUM_PLAYER - 1] != NULL)
		{
			m_pPlayer[MAX_NUM_PLAYER - 1]->Uninit();
			m_pPlayer[MAX_NUM_PLAYER - 1] = NULL;
		}
	}

	// �G�𓮂����邩�`�F�b�N
	m_nEnemyMoveCounter--;
	if (m_nEnemyMoveCounter <= 0)
	{
		m_nEnemyMoveCounter = 0;
		m_bEnemyMove = true;
	}

	// ��Ԃɂ���ď����킯
	switch (m_State)
	{
	case STATE_STAGE_SELECT:
		StageSelectUpdate();
		CScene::DeathCheck();
		break;
	case STATE_STAGE_DISP:
		StageDispUpdate();
		CScene::DeathCheck();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		CScene::UpdateAll();
		break;
	case STATE_PAUSE:
		PauseUpdate();
		break;
	case STATE_GAMEOVER:
		GameOverUpdate();
		CScene::UpdateAll();
		break;
	case STATE_RESULT:
		ResultUpdate();
		break;
	case STATE_CHANGE_MAP:
		ChangeMapUpdate();
		CScene::UpdateAll();
		break;
	case STATE_PREV_MAP:
		ChangePrevMap();
		CScene::DeathCheck();
		break;
	case STATE_NEXT_MAP:
		ChangeNextMap();
		CScene::DeathCheck();
		break;
	case STATE_END:
		EndUpdate();
		CScene::UpdateAll();
		break;
	}

	// �}�b�v�̏�Ԃɂ���ď����킯
	switch (CCharaSelect::GetStageType())
	{
	case MAPTYPE_CANDY:        // ���َq
		MapEvent_Candy();
		break;
	case MAPTYPE_CHRISTMAS:    // �N���X�}�X
		MapEvent_Christmas();
		break;
	case MAPTYPE_HINAMATSURI:  // �ЂȍՂ�
		MapEvent_Hinamatsuri();
		break;
	}

	// �v���C���[�����X�|�[�������邩�`�F�b�N
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// ���[�J��1P�v���C�Ȃ��
		CheckPlayerResSpawn(0);
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P)
	{// ���[�J��2P�v���C�Ȃ��
		for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			CheckPlayerResSpawn(nCntPlayer);
		}
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C��2P�v���C�Ȃ��
		CheckPlayerResSpawn(CManager::GetClient()->GetClientId());
	}

	// �G�𐶐����邩�`�F�b�N����
	int nIdxClient = 0;
	if (CManager::GetClient() != NULL)
	{
		nIdxClient = CManager::GetClient()->GetClientId();
	}
	if (nIdxClient == 0 && m_State != STATE_STAGE_SELECT && m_State != STATE_STAGE_DISP && m_State != STATE_PAUSE)
	CheckEnemySpawn(m_nGameCounter);

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		// �T�[�o�[�֑���f�[�^��ݒ肷��
		SetDataToServer();

		// �N���C�A���g�̍X�V����
		CManager::ClientUpdate();

		// �T�[�o�[���瑗��ꂽ�f�[�^��ݒ肷��
		GetDataFromServer();
	}

	// �f�[�^�N���A
	m_bDeletePlayerFlag = false;
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");

	CDebugProc::Print(1, "�Q�[�����\n");
	if (CManager::GetClient() != NULL)
	{
		CDebugProc::Print(1, "�N���C�A���g�ԍ�      : %d\n", CManager::GetClient()->GetClientId());
	}
	CDebugProc::Print(1, "�ǂݍ��񂾃}�b�v��    : %d\n", m_nNumMap);
	CDebugProc::Print(1, "�}�b�v�̎�ޔԍ�      : %d\n", CCharaSelect::GetStageType());
	CDebugProc::Print(1, "�v���C���[1�̎�ޔԍ� : %d\n", CCharaSelect::GetPlayerNumber(0));
	CDebugProc::Print(1, "�v���C���[2�̎�ޔԍ� : %d\n\n", CCharaSelect::GetPlayerNumber(1));
	CDebugProc::Print(1, "���݂̃}�b�v�ԍ�      : %d\n", m_nMapIdx);
	CDebugProc::Print(1, "���݂̃X�e�[�W�ԍ�    : %d\n", m_nStageIdx);
	CDebugProc::Print(1, "�Q�[���J�E���^�[      : %d\n", m_nGameCounter);
	CDebugProc::Print(1, "���ݐ������ꂽ�G�̐�  : %d\n", m_nSpawnEnemyCount);
	CDebugProc::Print(1, "�c��̓G�̐�          : %d\n", m_nNumEnemy);
	CDebugProc::Print(1, "�v���C���[1�̎c�@��   : %d\n", m_nPlayerStock[0]);
	CDebugProc::Print(1, "�v���C���[2�̎c�@��   : %d\n", m_nPlayerStock[1]);
	CDebugProc::Print(1, "���X�|�[���J�E���^�[  : %d\n", m_nPlayerRespawnCounter);
	CDebugProc::Print(1, "�v���C���[1�̒e�̐�   : %d\n", CBulletPlayer::GetNumAll_0());
	CDebugProc::Print(1, "�v���C���[2�̒e�̐�   : %d\n", CBulletPlayer::GetNumAll_1());
	CDebugProc::Print(1, "�G�̒e�̐�            : %d\n", CBulletEnemy::GetNumAll());
	CDebugProc::Print(1, "�u���b�N�̐�          : %d\n", CBlock::GetNumAll());
	CDebugProc::Print(1, "�X�|�[�������G�̐�    : %d\n", CEnemy::GetSpawnCounter());
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


//*****************************************************************************
//
// �T�[�o�[�̍X�V�p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̏����u���b�N�̃f�[�^��ݒ�
//=============================================================================
void CGame::DeleteBlock(const int nIdx)
{
	m_nNumDeleteBlock++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteBlock, aData);
	strcat(m_aDeleteBlock, " ");
}

//=============================================================================
// �Q�[���̏����G�̃f�[�^��ݒ�
//=============================================================================
void CGame::DeleteEnemy(const int nIdx)
{
	m_nNumDeleteEnemy++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteEnemy, aData);
	strcat(m_aDeleteEnemy, " ");
}

//=============================================================================
// �Q�[���̃v���C���[����ɂ��鏈��
//=============================================================================
void CGame::DeletePlayer(CPlayer *pPlayer, const int nIdx)
{
	SetPlayer(pPlayer, nIdx);
	if (m_nPlayerStock[nIdx] <= 0)
	{
		SetState(STATE_GAMEOVER);
	}

	// �z�X�g����Ȃ�������
	if (CManager::GetClient() == NULL)return;
	if (CManager::GetClient()->GetClientId() == 0 && nIdx != 0)
	{
		m_bDeletePlayerFlag = true;
	}
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServer(void)
{
	if (CManager::GetClient() == NULL || CManager::GetClient()->GetConnected() == false)return;

	// �v���C���[�̃f�[�^��ݒ�
	SetDataToServerFromPlayer();

	if (CManager::GetClient()->GetClientId() == 0)
	{// �������z�X�g�Ȃ�
	    // �Q�[���̃f�[�^��ݒ�
		SetDataToServerFromGame();
		// �G�̃f�[�^��ݒ�
		SetDataToServerFromEnemy();
		// �G�̒e�̃f�[�^��ݒ�
		SetDataToServerFromEnemyBullet();
		// �����u���b�N�̃f�[�^��ݒ�
		SetDataToServerFromDeleteBlock();
	}

	// �v���C���[�̒e����ݒ�
	SetDataToServerFromPlayerBullet();

	if (CManager::GetClient()->GetClientId() != 0)
	{// �������z�X�g�łȂ��Ȃ�
		// �����u���b�N�̃f�[�^��ݒ�
		SetDataToServerFromDeleteBlock();

		// �����G�̃f�[�^��ݒ�
		SetDataToServerFromDeleteEnemy();
	}
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���v���C���[�f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromPlayer(void)
{
	// �v���C���[�̐l����ݒ�
	if (m_pPlayer[CManager::GetClient()->GetClientId()] == NULL)
	{
		CManager::GetClient()->Print("0");
		CManager::GetClient()->Print(" ");
		if (CManager::GetClient()->GetClientId() == 0)
		{
			CManager::GetClient()->Print("%d", (int)m_bDeletePlayerFlag);
			CManager::GetClient()->Print(" ");
		}
		return;
	}
	else if(m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
	{
		CManager::GetClient()->Print("1");
		CManager::GetClient()->Print(" ");

		// �v���C���[�̍��W��ݒ�
		D3DXVECTOR3 PlayerPos = m_pPlayer[CManager::GetClient()->GetClientId()]->GetPos();
		CManager::GetClient()->Print("%.1f %.1f %.1f", PlayerPos.x, PlayerPos.y, PlayerPos.z);
		CManager::GetClient()->Print(" ");

		// �v���C���[�̌�����ݒ�
		D3DXVECTOR3 PlayerRot = m_pPlayer[CManager::GetClient()->GetClientId()]->GetRot();
		CManager::GetClient()->Print("%.1f %.1f %.1f", PlayerRot.x, PlayerRot.y, PlayerRot.z);
		CManager::GetClient()->Print(" ");

		// �v���C���[�̏�Ԃ�ݒ�
		int nPlayerState = m_pPlayer[CManager::GetClient()->GetClientId()]->GetState();
		CManager::GetClient()->Print("%d", nPlayerState);
		CManager::GetClient()->Print(" ");
	}

	// �z�X�g��������
	if (CManager::GetClient()->GetClientId() == 0)
	{
		CManager::GetClient()->Print("%d", (int)m_bDeletePlayerFlag);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���Q�[���f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromGame(void)
{
	// ��Ԃ�ݒ�
	CManager::GetClient()->Print("%d", m_State);
	CManager::GetClient()->Print(" ");

	// �}�b�v�̔ԍ���ݒ�
	CManager::GetClient()->Print("%d", m_nMapIdx);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���G�f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromEnemy(void)
{
	int nNumEnemy = 0;
	char aData[64] = "\0";
	char aEnemyData[MAX_SERVER_DATA] = "\0";

	// �G�̃f�[�^��ݒ�
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �G�N���X������
				strcat(aEnemyData, GetDataToEnemy((CEnemy*)pScene, aData));
				strcpy(aData, "\0");
				nNumEnemy++;
			}
			pScene = pSceneNext;
		}
	}

	// �G�̐���ݒ�
	CManager::GetClient()->Print("%d", nNumEnemy);
	CManager::GetClient()->Print(" ");

	// �G�̃f�[�^��ݒ�
	CManager::GetClient()->Print("%s", aEnemyData);
}

//=============================================================================
// �G�̃f�[�^�𕶎���ɕϊ����鏈��
//=============================================================================
char *CGame::GetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	char aData[64] = "\0";

	// �G�̍��W��ݒ�
	D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
	sprintf(aData, "%.1f %.1f %.1f", EnemyPos.x, EnemyPos.y, EnemyPos.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �G�̌�����ݒ�
	D3DXVECTOR3 EnemyRot = pEnemy->GetRot();
	sprintf(aData, "%.1f %.1f %.1f", EnemyRot.x, EnemyRot.y, EnemyRot.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �G�̎��
	int nEnemyType = pEnemy->GetType();
	sprintf(aData, "%d", nEnemyType);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// �G�̔ԍ�
	int nEnemyIdx = pEnemy->GetIdx();
	sprintf(aData, "%d", nEnemyIdx);
	strcat(pStr, aData);
	strcat(pStr, " ");

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���v���C���[�̒e�f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromPlayerBullet(void)
{
	int nNumBullet = 0;
	char aData[64] = "\0";
	char aBulletData[MAX_SERVER_DATA] = "\0";

	// ���݂̃v���C���[�̒e�̐���ݒ�
	if (CManager::GetClient()->GetClientId() == 0)
	{
		nNumBullet = CBulletPlayer::GetNumAll_0();
	}
	else
	{
		nNumBullet = CBulletPlayer::GetNumAll_1();
	}

	// �e�̐���ݒ�
	CManager::GetClient()->Print("%d", nNumBullet);
	CManager::GetClient()->Print(" ");
	if (nNumBullet < 1) return;

	// �e�̃f�[�^��ݒ肷��
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// �e�N���X������
				strcat(aBulletData, GetDataToPlayerBullet((CBullet*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// �e�̃f�[�^��ݒ�
	CManager::GetClient()->Print("%s", aBulletData);
}

//=============================================================================
// �v���C���[�̒e�̃f�[�^�𕶎���ɕϊ����鏈��
//=============================================================================
char *CGame::GetDataToPlayerBullet(CBullet *pBullet, char *pStr)
{
	if (pBullet->GetType() == CManager::GetClient()->GetClientId())
	{// �����̃v���C���[���������e�̏�����������
		char aData[64] = "\0";

		// �e�̍��W��ݒ�
		D3DXVECTOR3 BulletPos = pBullet->GetPos();
		sprintf(aData, "%.1f %.1f %.1f", BulletPos.x, BulletPos.y, BulletPos.z);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̔ԍ���ݒ�
		int nBulletIdx = pBullet->GetIdx();
		sprintf(aData, "%d", nBulletIdx);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̎�ނ�ݒ�
		int nBulletType = pBullet->GetType();
		sprintf(aData, "%d", nBulletType);
		strcat(pStr, aData);
		strcat(pStr, " ");
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ���G�̒e�f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromEnemyBullet(void)
{
	int nNumBullet = 0;
	char aData[64] = "\0";
	char aBulletData[MAX_SERVER_DATA] = "\0";

	// �G�̒e�̐���ݒ�
	nNumBullet = CBulletEnemy::GetNumAll();

	// �e�̐���ݒ�
	CManager::GetClient()->Print("%d", nNumBullet);
	CManager::GetClient()->Print(" ");
	if (nNumBullet < 1) return;

	// �e�̃f�[�^��ݒ肷��
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// �e�N���X������
				strcat(aBulletData, GetDataToEnemyBullet((CBullet*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// �e�̃f�[�^��ݒ�
	CManager::GetClient()->Print("%s", aBulletData);
}

//=============================================================================
// �G�̒e�̃f�[�^�𕶎���ɕϊ����鏈��
//=============================================================================
char *CGame::GetDataToEnemyBullet(CBullet *pBullet, char *pStr)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{// �G�̒e�f�[�^��ݒ肷��
		char aData[64] = "\0";

		// �e�̍��W��ݒ�
		D3DXVECTOR3 BulletPos = pBullet->GetPos();
		sprintf(aData, "%.1f %.1f %.1f", BulletPos.x, BulletPos.y, BulletPos.z);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̔ԍ���ݒ�
		int nBulletIdx = pBullet->GetIdx();
		sprintf(aData, "%d", nBulletIdx);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// �e�̎�ނ�ݒ�
		int nBulletType = pBullet->GetType();
		sprintf(aData, "%d", nBulletType);
		strcat(pStr, aData);
		strcat(pStr, " ");
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ�������u���b�N�̃f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromDeleteBlock(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteBlock);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteBlock > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteBlock);
		CManager::GetClient()->Print(" ");
	}
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
}

//=============================================================================
// �Q�[���̃T�[�o�[�ɑ�������G�̃f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::SetDataToServerFromDeleteEnemy(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteEnemy);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteEnemy > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteEnemy);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^��ݒ肷�鏈��
//=============================================================================
void CGame::GetDataFromServer(void)
{
	// ����ꂽ���b�Z�[�W���擾
	if (CManager::GetClient() == NULL)return;
	char *pStr = CManager::GetClient()->GetReceiveMessage();
	if (pStr == NULL || CManager::GetClient()->GetConnected() == false || *pStr == *"???")
	{// ���b�Z�[�W��M�Ɏ��s || �T�[�o�[�ɐڑ��ł��Ă��Ȃ�
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL && m_State == STATE_NORMAL)
		{// �v���C���[�������Ă���
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->Uninit();
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] = NULL;
			m_nPlayerStock[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]--;
		}
		return;
	}

	// �v���C���[�̃f�[�^��ݒ�
	pStr = SetDataToPlayerFromServer(pStr);

	if (CManager::GetClient()->GetClientId() != 0)
	{// �z�X�g����Ȃ�������
	    // �Q�[���̏�Ԃ�ݒ�
		pStr = SetDataToGameFromServer(pStr);
		// �G�̃f�[�^��ݒ�
		pStr = SetDataToEnemyFromServer(pStr);
		// �G�̒e�̃f�[�^��ݒ�
		pStr = SetDataToEnemyBulletFromServer(pStr);
		// �����u���b�N�̃f�[�^��ݒ�
		pStr = SetDataToDeleteBlock(pStr);
	}

	// �v���C���[�̒e�̃f�[�^��ݒ�
	pStr = SetDataToPlayerBulletFromServer(pStr);

	if (CManager::GetClient()->GetClientId() == 0)
	{// �z�X�g��������
		// �����u���b�N�̃f�[�^��ݒ�
		pStr = SetDataToDeleteBlock(pStr);

		// �����G�̃f�[�^��ݒ�
		pStr = SetDataToDeleteEnemy(pStr);
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^���v���C���[�ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToPlayerFromServer(char *pStr)
{
	int nWord = 0;
	int nNumPlayer = 0;

	// �v���C���[�̐l����ǂݎ��
	nNumPlayer = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumPlayer == 0)
	{// �v���C���[�����݂��Ȃ�
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL && m_State == STATE_NORMAL)
		{// �v���C���[�������Ă���
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->Uninit();
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] = NULL;
			m_nPlayerStock[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]--;
		}
	}
	else if(nNumPlayer == 1)
	{// �v���C���[�����݂���
		CMap *pMap = GetMap();
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] == NULL && pMap != NULL)
		{// ���������m�ۂ���Ă��Ȃ�
			int nNumber = (CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER;
			m_pPlayer[nNumber] = m_pPlayer[nNumber] = m_pPlayerManager[nNumber]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nNumber);
		}
		if(m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL)
		{// ���������m�ۂł��Ă���
			// �v���C���[�̍��W��ǂݎ��
			float fGetPlayerPosX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̌�����ǂݎ��
			float fGetPlayerRotX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�̏�Ԃ�ǂݎ��
			int nGetPlayerState = CFunctionLib::ReadInt(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// �v���C���[�ɒl��ݒ�
			if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL)
			{
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetPos(D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ));
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetRot(D3DXVECTOR3(fGetPlayerRotX, fGetPlayerRotY, fGetPlayerRotZ));
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetState((CPlayer::STATE)nGetPlayerState);
			}
		}
	}


	// �z�X�g����Ȃ�������
	if (CManager::GetClient()->GetClientId() != 0)
	{
		int nDeletePlayerFlag = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		if (nDeletePlayerFlag == 1 && m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->Uninit();
			m_pPlayer[CManager::GetClient()->GetClientId()] = NULL;
		}
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^���Q�[���ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToGameFromServer(char *pStr)
{
	int nGameState = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	int nMapIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// ��Ԃ�ݒ�
	m_StateOld = m_State;
	m_State = (STATE)nGameState;
	if (m_State != STATE_END && m_State != STATE_GAMEOVER && m_State != STATE_RESULT)
	{// �I����Ԃł��Q�[���I�[�o�[��Ԃł��Q�[�������U���g��Ԃł��Ȃ�
		m_nStateCounter = 0;
	}

	// �}�b�v�̔ԍ���ݒ�
	m_nMapIdx = nMapIdx;

	// ��Ԃɂ���ď����킯
	if (m_State == STATE_STAGE_DISP && GetMap() == NULL)
	{
		CreateMap();
		CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);
	}
	else if (m_State == STATE_STAGE_SELECT && GetMap() != NULL)
	{
		ReleaseMap();
		DeleteGameObject();
	}
	else if (m_State == STATE_STAGE_DISP && m_StateOld == STATE_NORMAL)
	{
		ReleaseMap();
		DeleteGameObject();
	}
	else if (m_State == STATE_NORMAL && m_StateOld == STATE_STAGE_DISP)
	{
		ReleaseStageBg();
		ReleaseStageLogo();
		ReleaseStageNumber();
		SetPlayerPosToSpawn();
		CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX);
	}
	else if (m_State == STATE_STAGE_DISP || m_State == STATE_STAGE_SELECT)
	{
		ReleaseStageNumber();
		CalcStageDigits();
		CreateStageNumber();
	}
	else if (m_State == STATE_RESULT && m_StateOld == STATE_END)
	{
		CreateGameResult();
		CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX);
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^��G�ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToEnemyFromServer(char *pStr)
{
	int nWord = 0;
	int nNumEnemy = 0;

	// ��������Ă���G�̐���ǂݎ��
	nNumEnemy = CFunctionLib::ReadInt(pStr, "");
	m_nNumEnemy = nNumEnemy;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �����킹
	ReleaseEnemy(nNumEnemy);
	if (nNumEnemy == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �G�N���X������
				pStr = SetDataToEnemy((CEnemy*)pScene, pStr);
				nNumEnemy--;
			}
			pScene = pSceneNext;
		}
	}

	// ����Ȃ��G�͐V���ɐ������f�[�^��ݒ�
	for (int nCntEnemy = 0; nCntEnemy < nNumEnemy; nCntEnemy++)
	{
		pStr = SetDataToCreateEnemy(pStr);
	}

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ����G�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyType = 0;
	int nEnemyIdx = 0;

	// �G�̍��W��ǂݎ��
	fEnemyPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̌�����ǂݎ��
	fEnemyRotX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̎�ޔԍ�
	nEnemyType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̔ԍ�
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�Ƀf�[�^��ݒ�
	pEnemy->SetPos(D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ));
	pEnemy->SetRot(D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ));
	pEnemy->SetIdx(nEnemyIdx);

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ������������G�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToCreateEnemy(char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyType = 0;
	int nEnemyIdx = 0;

	// �G�̍��W��ǂݎ��
	fEnemyPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̌�����ǂݎ��
	fEnemyRotX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̎�ޔԍ�
	nEnemyType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�̔ԍ�
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CEnemy_ListData *pEnemyData = GetMap()->GetEnemyListData(nEnemyIdx);
	D3DXVECTOR3 pos = D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ);
	D3DXVECTOR3 rot = D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ);
	CEnemy *pEnemy = m_pEnemyManager[nEnemyType]->SetEnemy(pos, rot, nEnemyType);

	if (pEnemy == NULL)return NULL;
	// �G�̃f�[�^��ݒ肷��
	if (pEnemyData->GetItem() == true)
	{
		pEnemy->SetItemData(pEnemyData->GetItemType());
	}
	pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());
	pEnemy->SetIdx(nEnemyIdx);

	return pStr;
}

//=============================================================================
// �v���C���[�̒e�̐������킹�鏈��
//=============================================================================
void CGame::ReleaseEnemy(int nNumEnemy)
{
	// ���݂̓G�̐���ݒ�
	int nAllEnemy = 0;
	nAllEnemy = CEnemy::GetNumAll();
	if (nAllEnemy < nNumEnemy)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && nAllEnemy > nNumEnemy)
			{// �e�N���X������
				ReleaseCheckEnemy((CEnemy*)pScene, &nAllEnemy);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
// �G��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckEnemy(CEnemy *pEnemy, int *pNumEnemy)
{
	pEnemy->Uninit();
	pEnemy = NULL;
	*pNumEnemy = *pNumEnemy - 1;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^���v���C���[�̒e�ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToPlayerBulletFromServer(char *pStr)
{
	int nWord = 0;
	int nNumBullet = 0;

	// ��������Ă���e�̐���ǂݎ��
	nNumBullet = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �����킹
	ReleasePlayerBullet(nNumBullet);
	if (nNumBullet == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// �e�N���X������
				pStr = SetDataToPlayerBullet((CBullet*)pScene, pStr, &nNumBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ����Ȃ��e�͐V���ɐ������f�[�^��ݒ�
	for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
	{
		pStr = SetDataToCreatePlayerBullet(pStr);
	}

	return pStr;
}

//=============================================================================
// �v���C���[�̒e�̐������킹�鏈��
//=============================================================================
void CGame::ReleasePlayerBullet(int nNumBullet)
{
	// ���݂̃v���C���[�̒e�̐���ݒ�
	int nAllBullet = 0;
	if (CManager::GetClient()->GetClientId() == 0)
	{
		nAllBullet = CBulletPlayer::GetNumAll_1();
	}
	else
	{
		nAllBullet = CBulletPlayer::GetNumAll_0();
	}
	if (nAllBullet < nNumBullet)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nAllBullet > nNumBullet)
			{// �e�N���X������
				ReleaseCheckPlayerBullet((CBullet*)pScene, &nAllBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
// �v���C���[�̒e��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckPlayerBullet(CBullet *pBullet, int *pNumBullet)
{
	if (pBullet->GetType() != CBullet::TYPE_ENEMY &&
		pBullet->GetType() != CManager::GetClient()->GetClientId())
	{
		pBullet->Uninit();
		pBullet = NULL;
		*pNumBullet = *pNumBullet - 1;
	}
}

//=============================================================================
// ��������f�[�^�ɕϊ����v���C���[�̒e�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToPlayerBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY ||
		pBullet->GetType() == CManager::GetClient()->GetClientId()) return pStr;

	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̍��W��ǂݎ��
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̔ԍ���ǂݎ��
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̎�ނ�ǂݎ��
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �v���C���[�̒e�Ƀf�[�^��ݒ�
	pBullet->SetPos(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ));
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);
	*pNumBullet = *pNumBullet - 1;

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ������������v���C���[�̒e�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToCreatePlayerBullet(char *pStr)
{
	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̍��W��ǂݎ��
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̔ԍ���ǂݎ��
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̎�ނ�ǂݎ��
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CBulletPlayer *pBullet = CBulletPlayer::Create(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ), INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
		(CBullet::TYPE)((CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER), m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]);
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);

	return pStr;
}

//=============================================================================
// �Q�[���̃T�[�o�[���瑗��ꂽ�f�[�^��G�̒e�ɐݒ肷�鏈��
//=============================================================================
char *CGame::SetDataToEnemyBulletFromServer(char *pStr)
{
	int nWord = 0;
	int nNumBullet = 0;

	// ��������Ă���e�̐���ǂݎ��
	nNumBullet = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �����킹
	ReleaseEnemyBullet(nNumBullet);
	if (nNumBullet == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nNumBullet > 0)
			{// �e�N���X������
				pStr = SetDataToEnemyBullet((CBullet*)pScene, pStr, &nNumBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ����Ȃ��e�͐V���ɐ������f�[�^��ݒ�
	for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
	{
		pStr = SetDataToCreateEnemyBullet(pStr);
	}

	return pStr;
}

//=============================================================================
// �G�̒e�̐������킹�鏈��
//=============================================================================
void CGame::ReleaseEnemyBullet(int nNumBullet)
{
	// ���݂̓G�̒e�̐���ݒ�
	int nAllBullet = 0;
	nAllBullet = CBulletEnemy::GetNumAll();
	if (nAllBullet < nNumBullet)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nAllBullet > nNumBullet)
			{// �e�N���X������
				ReleaseCheckEnemyBullet((CBullet*)pScene, &nAllBullet);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
// �G�̒e��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckEnemyBullet(CBullet *pBullet, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{
		pBullet->Uninit();
		pBullet = NULL;
		*pNumBullet = *pNumBullet - 1;
	}
}

//=============================================================================
// ��������f�[�^�ɕϊ����e�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToEnemyBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{
		int nWord = 0;
		float fBulletPosX = 0.0f;
		float fBulletPosY = 0.0f;
		float fBulletPosZ = 0.0f;
		int nBulletIdx = 0;
		int nBulletType = 0;

		// �e�̍��W��ǂݎ��
		fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// �e�̔ԍ���ǂݎ��
		nBulletIdx = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// �e�̎�ނ�ǂݎ��
		nBulletType = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// �e�Ƀf�[�^��ݒ�
		pBullet->SetPos(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ));
		pBullet->SetIdx(nBulletIdx);
		pBullet->SetType((CBullet::TYPE)nBulletType);
		*pNumBullet = *pNumBullet - 1;
	}

	return pStr;
}

//=============================================================================
// ��������f�[�^�ɕϊ������������G�̒e�ɐݒ肷��
//=============================================================================
char *CGame::SetDataToCreateEnemyBullet(char *pStr)
{
	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// �e�̍��W��ǂݎ��
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̔ԍ���ǂݎ��
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�̎�ނ�ǂݎ��
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �e�𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CBulletEnemy *pBullet = CBulletEnemy::Create(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ), INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, NULL);
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);

	return pStr;
}

//=============================================================================
// �Q�[���̏����u���b�N�̃f�[�^��ݒ�
//=============================================================================
char *CGame::SetDataToDeleteBlock(char *pStr)
{
	int nWord = 0;
	int nNumDeleteBlock = 0;
	int *pDeleteIdx = NULL;

	// �����u���b�N�̐���ǂݎ��
	nNumDeleteBlock = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteBlock == 0) return pStr;

	// �������������������m��
	pDeleteIdx = new int[nNumDeleteBlock];
	for (int nCnt = 0; nCnt < nNumDeleteBlock; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// �����������u���b�N���J������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && m_State == STATE_NORMAL)
			{// �u���b�N�N���X������
				ReleaseCheckBlock((CBlock*)pScene, pDeleteIdx, &nNumDeleteBlock);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();

	// �������̊J��
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// �Q�[���̃u���b�N��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckBlock(CBlock *pBlock, int *pDeleteIdx, int *nNumDeleteBlock)
{
	int nIdx = pBlock->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteBlock; nCntDeleteNum++)
	{// ���������J��Ԃ�
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pBlock != NULL)
		{// �ԍ���v
			pBlock->Uninit();
			pBlock = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̏����G�̃f�[�^��ݒ�
//=============================================================================
char *CGame::SetDataToDeleteEnemy(char *pStr)
{
	int nWord = 0;
	int nNumDeleteEnemy = 0;
	int nCntDelete = 0;
	int *pDeleteIdx = NULL;

	// �����G�̐���ǂݎ��
	nNumDeleteEnemy = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteEnemy == 0) return pStr;

	// �������������������m��
	pDeleteIdx = new int[nNumDeleteEnemy];
	for (int nCnt = 0; nCnt < nNumDeleteEnemy; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// �����������G���J������
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && m_State == STATE_NORMAL)
			{// �G�N���X������
				ReleaseCheckDeleteEnemy((CEnemy*)pScene, pDeleteIdx, &nNumDeleteEnemy);
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();

	// �������̊J��
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// �Q�[���̓G��j�����邩�`�F�b�N���鏈��
//=============================================================================
void CGame::ReleaseCheckDeleteEnemy(CEnemy *pEnemy, int *pDeleteIdx, int *nNumDeleteEnemy)
{
	int nIdx = pEnemy->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteEnemy; nCntDeleteNum++)
	{// ���������J��Ԃ�
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pEnemy != NULL)
		{// �ԍ���v
			pEnemy->Uninit();
			pEnemy = NULL;
		}
	}
}


//*****************************************************************************
//
// ��Ԃɂ�鏈�������p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̃X�e�[�W�Z���N�g��Ԃ̍X�V����
//=============================================================================
void CGame::StageSelectUpdate(void)
{
	// �w�i����
	if (m_pStageBg == NULL)
	{
		CreateStageBg();
	}

	// �X�e�[�W���S����
	if (m_pStageLogo == NULL)
	{
		CreateStageLogo();
	}

	// �X�e�[�W�ԍ�����
	if (m_pStageNumber == NULL)
	{
		CreateStageNumber();
	}


	if (CManager::GetClient()->GetClientId() != 0)
	{
		ReleaseStageNumber();
		CalcStageDigits();
		CreateStageNumber();
	}

	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL || CManager::GetClient()->GetClientId() != 0) return;

	if (pKey->GetTrigger(DIK_W) == true || pKey->GetRepeat(DIK_W) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_0) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_0) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_16) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_16) == true)
	{// ������̓��͂����ꂽ
		if (m_nStageIdx < GAME_MAPSTAGE_MAX - 1)
		{
			m_nStageIdx++;
			m_nMapIdx++;
			ReleaseStageNumber();
			CalcStageDigits();
			CreateStageNumber();
		}
	}
	else if (pKey->GetTrigger(DIK_S) == true || pKey->GetRepeat(DIK_S) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_1) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_1) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_17) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_17) == true)
	{// �������̓��͂����ꂽ
		if (m_nStageIdx > 0)
		{
			m_nStageIdx--;
			m_nMapIdx--;
			ReleaseStageNumber();
			CalcStageDigits();
			CreateStageNumber();
		}
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// ����{�^���������ꂽ
		SetState(STATE_STAGE_DISP);
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ���\�������Ԃ̍X�V����
//=============================================================================
void CGame::StageDispUpdate(void)
{
	// �Q�[�������U���g�ɕK�v�Ȃ��̂�j������
	ReleaseGameResult();

	// �w�i����
	if (m_pStageBg == NULL)
	{
		CreateStageBg();
	}

	// �X�e�[�W���S����
	if (m_pStageLogo == NULL)
	{
		CreateStageLogo();
	}

	// �X�e�[�W�ԍ�����
	if (m_pStageNumber == NULL)
	{
		CreateStageNumber();
	}

	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_STAGEDISP_TIME == 0)
	{// �X�e�[�W�ԍ����o���Ă��炠����x������
		SetState(STATE_NORMAL);
		ReleaseStageBg();
		ReleaseStageLogo();
		ReleaseStageNumber();
		CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX);
	}
}

//=============================================================================
// �Q�[���̒ʏ��Ԃ̍X�V����
//=============================================================================
void CGame::NormalUpdate(void)
{
#ifdef GAME_DEBUG  // �f�o�b�O�R�}���h�K�p��

	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL) return;

	// �|�[�YON/OFF�؂�ւ�
	if (pKey->GetTrigger(DIK_P) == true)
	{
		m_State = STATE_PAUSE;
	}

	// �X�e�[�W�Z���N�g�f�o�b�O
	int nIdxClient = 0;
	if (CManager::GetClient() != NULL)
	{
		nIdxClient = CManager::GetClient()->GetClientId();
	}
	if (nIdxClient == 0)
	{
		if (pKey->GetTrigger(DIK_1) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_PREV_MAP);
		}
		else if (pKey->GetTrigger(DIK_2) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_NEXT_MAP);
		}
		else if (pKey->GetTrigger(DIK_3) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_STAGE_SELECT);
		}
	}

	// ��ԃ`�F�b�N�f�o�b�O
	if (pKey->GetTrigger(DIK_4) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		SetState(STATE_GAMEOVER);
	}
	else if (pKey->GetTrigger(DIK_5) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		SetState(STATE_END);
	}

	// �v���C���[��|���f�o�b�O
	if (pKey->GetTrigger(DIK_6) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		if (m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->Uninit();
			m_pPlayer[CManager::GetClient()->GetClientId()] = NULL;
			m_nPlayerStock[CManager::GetClient()->GetClientId()]--;
		}
	}


	// �A�C�e���C�x���g�f�o�b�O
	if (pKey->GetTrigger(DIK_F1) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Star(0);
	}
	else if (pKey->GetTrigger(DIK_F2) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Star(1);
	}
	else if (pKey->GetTrigger(DIK_F3) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Grenade();
	}
	else if (pKey->GetTrigger(DIK_F4) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_1Up(0);
	}
	else if (pKey->GetTrigger(DIK_F5) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_1Up(1);
	}
	else if (pKey->GetTrigger(DIK_F6) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Scoop();
	}
	else if (pKey->GetTrigger(DIK_F7) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Clock();
	}
	else if (pKey->GetTrigger(DIK_F8) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Helmet(0);
	}
	else if (pKey->GetTrigger(DIK_F9) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Helmet(1);
	}

	// �X�e�[�W�C�x���g�f�o�b�O
	if (CManager::GetKeyboard()->GetTrigger(DIK_1) == true && CManager::GetKeyboard()->GetPress(DIK_RSHIFT) == true)
	{
		m_HinaEvent = HINAEVENT_CHERRY_BLOSSOMS;
		m_nEventCounter = 0;
	}

	CDebugProc::Print(1, "�f�o�b�O�R�}���h\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [1]�L�[        : �O�̃X�e�[�W�ɂ���\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [2]�L�[        : ���̃X�e�[�W�ɂ���\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [3]�L�[        : �X�e�[�W�Z���N�g��Ԃɂ���\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [4]�L�[        : �Q�[���I�[�o�[��Ԃɂ���\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [5]�L�[        : �I����Ԃɂ���\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [6]�L�[        : �v���C���[��|���Ďc�@�����炷\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F1]�`[F2]�L�[ : �X�^�[�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F3]�L�[       : �O���l�[�h�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F4]�`[F5]�L�[ : 1Up�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F6]�L�[       : �X�R�b�v�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F7]�L�[       : ���v�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[LSHIFT]�L�[ + [F8]�`[F9]�L�[ : �w�����b�g�A�C�e���C�x���g\n");
	CDebugProc::Print(1, "[RSHIFT]�L�[ + [1]�L�[        : �Ԃт畑���U��\n\n");

#endif

	CDebugProc::Print(1, "�ʏ���\n");
}

//=============================================================================
// �Q�[���̃|�[�Y��Ԃ̍X�V����
//=============================================================================
void CGame::PauseUpdate(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL) return;

	// �|�[�YON/OFF�؂�ւ�
	if (pKey->GetTrigger(DIK_P) == true)
	{
		m_State = STATE_NORMAL;
	}
}

//=============================================================================
// �Q�[���̃Q�[���I�[�o�[��Ԃ̍X�V����
//=============================================================================
void CGame::GameOverUpdate(void)
{
	// �Q�[���I�[�o�[�̃��S���o��
	if (m_pGameOverLogo == NULL)
	{
		CreateGameOverLogo();
		return;
	}

	// BGM���~�߂�
	CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX);

	// �Q�[���I�[�o�[���S�|���S�����グ��
	GameOverLogoUp();

	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_GAMEOVER_TIMING == 0)
	{
		CFade *pFade = CManager::GetFade();
		if (pFade == NULL) return;
		if (pFade->GetFade() != CFade::FADE_NONE) return;

		// ���̃��[�h�ɐݒ�
		pFade->SetFade(CManager::MODE_TITLE);
	}

	CDebugProc::Print(1, "�Q�[���I�[�o�[���\n");
}

//=============================================================================
// �Q�[���̃Q�[���I�[�o�[���S����ɂ����鏈��
//=============================================================================
void CGame::GameOverLogoUp(void)
{
	if (m_pGameOverLogo != NULL)
	{
		D3DXVECTOR3 GameOverLogoPos = m_pGameOverLogo->GetPos();
		GameOverLogoPos.y -= GAME_GAMEOVER_POSUP;
		if (GameOverLogoPos.y <= m_pGameOverLogo->GetHeight() + GAME_GAMEOVER_POSYMAX)
		{
			GameOverLogoPos.y += GAME_GAMEOVER_POSUP;
		}

		m_pGameOverLogo->SetPos(GameOverLogoPos);
		m_pGameOverLogo->SetVtxBuffPos();
	}
}

//=============================================================================
// �Q�[���̃��U���g�\����Ԃ̍X�V����
//=============================================================================
void CGame::ResultUpdate(void)
{
	CDebugProc::Print(1, "�Q�[�������U���g���\n");

	// �J�E���^�[���Z
	m_nStateCounter++;
	CDebugProc::Print(1, "%d\n", m_nStateCounter);
	if (m_nStateCounter % GAME_NEXTMODE_TIMING == 0)
	{
		if (m_nStageIdx >= GAME_MAPSTAGE_MAX - 1)
		{// ����ȏ�X�e�[�W���Ȃ�
			CFade *pFade = CManager::GetFade();
			if (pFade == NULL) return;
			if (pFade->GetFade() != CFade::FADE_NONE) return;

			// ���̃��[�h�ɐݒ�
			pFade->SetFade(CManager::MODE_RESULT);
		}
		else
		{// �܂����̃}�b�v�����݂���
			// �N���C�A���g�ԍ����擾
			int nIdxClient = 0;
			if (CManager::GetClient() != NULL)
			{
				nIdxClient = CManager::GetClient()->GetClientId();
			}
			if (nIdxClient == 0)
			{// �z�X�g������
				SetState(STATE_CHANGE_MAP);
			}
		}
	}
}

//=============================================================================
// �Q�[���̃}�b�v��ς����Ԃ̍X�V����
//=============================================================================
void CGame::ChangeMapUpdate(void)
{
	// �N���C�A���g�ԍ����擾
	int nIdxClient = 0;
	if (CManager::GetClient() != NULL)
	{
		nIdxClient = CManager::GetClient()->GetClientId();
	}
	if (nIdxClient == 0)
	{// �z�X�g��������
		m_nMapIdx++;
		m_nStageIdx++;
		CalcStageDigits();
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
		SetState(STATE_STAGE_DISP);
		CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);
	}
	else
	{// �z�X�g����Ȃ�������
		m_nMapIdx++;
		m_nStageIdx++;
		CalcStageDigits();
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
		SetState(STATE_STAGE_DISP);
		CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);
	}

	// �f�[�^�N���A
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
}

//=============================================================================
// �Q�[���̏I����Ԃ̍X�V����
//=============================================================================
void CGame::EndUpdate(void)
{
	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_RESULT_TIMING == 0)
	{// ������x���Ԃ�������
		SetState(STATE_RESULT);
		CreateGameResult();
		CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX);
	}

	CDebugProc::Print(1, "�I�����\n");
}



//*****************************************************************************
//
// �A�C�e���C�x���g�����p�֐�
// Auther : Hodaka Niwa & Jukiya Hayakawa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̃X�^�[�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_Star(int nPlayerNumber)
{
	// �v���C���[�̃p�����[�^���p���[�A�b�v������

}

//=============================================================================
// �Q�[���̃O���l�[�h�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_Grenade(void)
{
	// ����������Ă���G������
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

	// �J������h�炷
	ItemEvent_Granade_Shake();
}

//=============================================================================
// �Q�[���̃O���l�[�h����������ɃJ������h�炷����
//=============================================================================
void CGame::ItemEvent_Granade_Shake(void)
{
	// �J������h�炷��Ԃɂ���
	if (GetCameraManager() == NULL) { return; }
	CCamera *pCamera = GetCameraManager()->GetCamera();
	if (pCamera == NULL) { return; }

	if (pCamera->GetType() == CCamera::TYPE_GAME)
	{// �Q�[���Ŏg�p���Ă���J�����ł���
		CGameCamera *pGameCamera = (CGameCamera*)pCamera;
		pGameCamera->StartShake(m_ItemEventData.nShakeTime, m_ItemEventData.fShake, m_ItemEventData.fShakeCut);
	}
}

//=============================================================================
// �Q�[����1Up�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_1Up(int nPlayerNumber)
{
	// �v���C���[�̎c�@���𑝂₷
	m_nPlayerStock[nPlayerNumber]++;
}

//=============================================================================
// �Q�[���̃X�R�b�v�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_Scoop(void)
{
	// �i�ߕ��̎���ɉ󂹂Ȃ��u���b�N��u��
}

//=============================================================================
// �Q�[���̎��v�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_Clock(void)
{
	// �G�������Ȃ���Ԃɂ���
	m_bEnemyMove = false;
	m_nEnemyMoveCounter = m_ItemEventData.nStop;
}

//=============================================================================
// �Q�[���̃w�����b�g�A�C�e���̃C�x���g����
//=============================================================================
void CGame::ItemEvent_Helmet(int nPlayerNumber)
{
	// �v���C���[�𖳓G��Ԃɂ���
}


//*****************************************************************************
//
// �}�b�v�C�x���g�p�֐�
// Auther : Hodaka Niwa & Jukiya Hayakawa & Kobayashi Shouhei
//
//*****************************************************************************
//=============================================================================
// �Q�[���̂��َq�̃}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Candy(void)
{

}

//=============================================================================
// �Q�[���̃N���X�}�X�̃}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Christmas(void)
{

}

//=============================================================================
// �Q�[���̂ЂȍՂ�̃}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Hinamatsuri(void)
{
	CDebugProc::Print(1, "�ЂȍՂ�C�x���g\n");

	switch (m_HinaEvent)
	{
	case HINAEVENT_NORMAL:
		MapEvent_Hinamatsuri_Normal();
		break;
	case HINAEVENT_CHERRY_BLOSSOMS:
		MapEvent_Hinamatsuri_CherryBlossoms();
		break;
	case HINAEVENT_DROP_ITEM:
		MapEvent_Hinamatsuri_DropItem();
		break;
	}
}

//=============================================================================
// �Q�[���̂ЂȍՂ�̒ʏ�̃}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Hinamatsuri_Normal(void)
{
	CDebugProc::Print(1, "�ʏ�C�x���g\n");
}

//=============================================================================
// �Q�[���̂ЂȍՂ�̍��������U��}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Hinamatsuri_CherryBlossoms(void)
{
	CDebugProc::Print(1, "�������U��C�x���g\n");
	if (m_State != STATE_NORMAL) { return; }

	// ���̉Ԃт琶��
	for (int nCnt = 0; nCnt < m_CherryBlossomsData.nAppear; nCnt++)
	{
		CreateBlossoms();
	}

	// �C�x���g�J�E���^�[�i�s
	m_nEventCounter++;
	if (m_nEventCounter >= m_CherryBlossomsData.nTime)
	{// �J�E���^�[�����l�ɂȂ���
		m_nEventCounter = 0;
		m_HinaEvent = HINAEVENT_NORMAL;
	}
}

//=============================================================================
// �Q�[���̂ЂȍՂ�̃A�C�e���𓊉�����}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_Hinamatsuri_DropItem(void)
{
	CDebugProc::Print(1, "�A�C�e���h���b�v�C�x���g\n");
}


//*****************************************************************************
//
// �Q�[�����X�|�[�������p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̃v���C���[�����X�|�[���ʒu�ɂ��炷����
//=============================================================================
void CGame::SetPlayerPosToSpawn(void)
{
	// �v���C���[�̃��X�|�[���ʒu���擾
	D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
	CMap *pMap = GetMap();
	if (pMap == NULL) { return; }

	// �v���C���[�̈ʒu�����X�|�[���ʒu�ɐݒ�
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// �I�����C���v���C�Ȃ��
		PlayerPos = pMap->GetPlayerRespawn(CManager::GetClient()->GetClientId())->GetPos();
		if (m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetPos(PlayerPos);
		}
	}
	else
	{// ���[�J���v���C�Ȃ��
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{// �v���C���[�̐l�����J��Ԃ�
			PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			if (m_pPlayer[nCntPlayer] != NULL)
			{
				m_pPlayer[nCntPlayer]->SetPos(PlayerPos);
			}
		}
	}
}

//=============================================================================
// �Q�[���̃v���C���[�����X�|�[�������邩�`�F�b�N����
//=============================================================================
void CGame::CheckPlayerResSpawn(int nCntPlayer)
{
	if (m_pPlayer[nCntPlayer] == NULL && m_nPlayerStock[nCntPlayer] >= 1)
	{
		m_nPlayerRespawnCounter++;
		if (m_nPlayerRespawnCounter % GAME_PLAYER_RESPAWN_TIMING == 0)
		{
			m_nPlayerRespawnCounter = 0;
			m_nPlayerStock[nCntPlayer]--;
			m_pPlayer[nCntPlayer] = m_pPlayerManager[nCntPlayer]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);

			// �v���C���[�̃��X�|�[���ʒu���擾
			D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
			CMap *pMap = GetMap();
			if (pMap != NULL)
			{
				PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			}

			// �v���C���[�̈ʒu�����炷
			m_pPlayer[nCntPlayer]->SetPos(PlayerPos);

			// �G�t�F�N�g���o��
			D3DXVECTOR3 EffectPos = PlayerPos;
			CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
			if (pEffectManager != NULL)
			{
				pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, GAME_PLAYER_SPAWN_EFFECT_IDX);
				pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, GAME_PLAYER_SPAWN_EFFECT_IDX + 1);
			}
		}
	}
}

//=============================================================================
// �Q�[���̓G�̐�������^�C�~���O���ǂ������肷��
//=============================================================================
void CGame::CheckEnemySpawn(int nTime)
{
	CMap *pMap = GetMap();
	if (pMap == NULL)return;

	int nNumEnemyListData = pMap->GetNumEnemyListData();
	CEnemy_ListData *pEnemyData = NULL;

	// �����G���o�Ȃ��̂ł���΂��̎��_�ŏ����I��
	if (m_nSpawnEnemyCount == nNumEnemyListData) return;

	// �J�E���^�[����
	m_nGameCounter++;
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
// �Q�[���̓G���X�|�[��������
//=============================================================================
void CGame::EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt)
{
	// �G�̃��X�|�[���ʒu���擾
	D3DXVECTOR3 EnemyPos = pMap->GetEnemyRespawn(pEnemyData->GetRespawnIdx())->GetPos();

	// �G�̐���
	CreateEnemy(EnemyPos, INITIALIZE_D3DXVECTOR3, pEnemyData);
}


//*****************************************************************************
//
// �}�b�v��ς���p�֐�(�f�o�b�O�p�̊֐�����)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̊e��J�E���^�[������������
//=============================================================================
void CGame::ResetCounter(void)
{
	m_nSpawnEnemyCount = 0;
	m_nGameCounter = 0;
	m_nStateCounter = 0;
	m_nEventCounter = 0;
	m_nEnemyMoveCounter = 0;
}

//=============================================================================
// �Q�[���̎��̃}�b�v�ɐ؂�ւ��鏈��
//=============================================================================
void CGame::ChangeNextMap(void)
{
	// ���݂̃}�b�v��j��
	ReleaseMap();

	// �Q�[���Ɏg�p���Ă���I�u�W�F�N�g��j��
	DeleteGameObject();

	// �}�b�v�ԍ��𑝂₵�}�b�v��ǂݍ���
	m_nMapIdx++;
	m_nStageIdx++;
	if (m_nStageIdx >= GAME_MAPSTAGE_MAX)
	{
		m_nMapIdx--;
		m_nStageIdx--;
	}
	CreateMap();

	// �}�b�v�ԍ��̌������v�Z���Ă���
	CalcStageDigits();

	// �v���C���[�̈ʒu���X�|�[���ʒu��ݒ�
	SetPlayerPosToSpawn();

	// �e��J�E���^�[������
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;

	// ��Ԃ�ݒ�
	SetState(STATE_STAGE_DISP);
}

//=============================================================================
// �Q�[���̑O�̃}�b�v�ɐ؂�ւ��鏈��
//=============================================================================
void CGame::ChangePrevMap(void)
{
	// ���݂̃}�b�v��j��
	ReleaseMap();

	// �Q�[���Ɏg�p���Ă���I�u�W�F�N�g��j��
	DeleteGameObject();

	// �}�b�v�ԍ������炵�}�b�v��ǂݍ���
	m_nMapIdx--;
	m_nStageIdx--;
	if (m_nStageIdx < 0)
	{
		m_nMapIdx++;
		m_nStageIdx++;
	}
	CreateMap();

	// �}�b�v�ԍ��̌������v�Z���Ă���
	CalcStageDigits();

	// �v���C���[�̈ʒu���X�|�[���ʒu�ɐݒ�
	SetPlayerPosToSpawn();

	// �e��J�E���^�[������
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;

	// ��Ԃ�ݒ�
	SetState(STATE_STAGE_DISP);
}

//=============================================================================
// �Q�[���̃Q�[���Ɏg�p���Ă���I�u�W�F�N�g��j�����鏈��
//=============================================================================
void CGame::DeleteGameObject(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂�
			pSceneNext = pScene->GetNext();
			if (GAME_DELETE_OBJECT)
			{// �Q�[���Ɏg�p���Ă���I�u�W�F�N�g�N���X������
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

	// ���S�t���O�`�F�b�N
	CScene::DeathCheck();
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ��̌������v�Z���鏈��
//=============================================================================
void CGame::CalcStageDigits(void)
{
	m_nNumNumberDigits = (int)log10f((float)(m_nStageIdx + 1)) + 1;	// ���������߂�
	if (m_nNumNumberDigits <= 0) { m_nNumNumberDigits = 1; }	    // 0�ȉ��̂Ƃ�1�ɂ���
}



//*****************************************************************************
//
// �����p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̓ǂݍ��ރ}�b�v�t�@�C�����̃|�C���^�𐶐�����
//=============================================================================
void CGame::CreateMapFilePointer(void)
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
// �Q�[���̃J������������
//=============================================================================
void CGame::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager != NULL)
	{
		CGameCamera *pGameCamera = CGameCamera::Create();
		if (pGameCamera != NULL)
		{
			pCameraManager->SetCamera(pGameCamera);
		}
		SetCameraManager(pCameraManager);
	}
}

//=============================================================================
// �Q�[���̃e�N�X�`���Ǌ��N���X��������
//=============================================================================
void CGame::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// �Q�[���̃}�b�v��������
//=============================================================================
void CGame::CreateMap(void)
{
	CMap *pMap = CMap::Create(m_pMapFileName[m_nMapIdx]);
	SetMap(pMap);
	m_nNumEnemy = pMap->GetNumEnemyListData();
	ResetCounter();
	CEnemy::ResetSpawnCounter();
	SetPlayerPosToSpawn();
	CheckEnemySpawn(m_nGameCounter);
}

//=============================================================================
// �Q�[����UI��������
//=============================================================================
void CGame::CreateUI(void)
{
	m_pUI = CUI::Create(GetTextureManager(), 0, m_nMapIdx);
}

//=============================================================================
// �Q�[���̃v���C���[��������
//=============================================================================
void CGame::CreatePlayer(void)
{
	// �v���C���[�𐶐�����
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = m_pPlayerManager[nCntPlayer]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);
	}
}

//=============================================================================
// �Q�[���̓G��������
//=============================================================================
CEnemy *CGame::CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData)
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
// �Q�[���̃A�C�e���𐶐����鏈��
//=============================================================================
void CGame::CreateItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nItemType)
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

//=============================================================================
// �Q�[���̃Q�[���I�[�o�[���S�\���p�|���S���𐶐�����
//=============================================================================
void CGame::CreateGameOverLogo(void)
{
	m_pGameOverLogo = CScene2D::Create(m_GameOverPolyData.pos, m_GameOverPolyData.col,
		m_GameOverPolyData.fWidth, m_GameOverPolyData.fHeight, GAME_GAMEOVERPOLYGON_PRIORITY);
	if (m_pGameOverLogo != NULL, GetTextureManager() != NULL)
	{
		m_pGameOverLogo->BindTexture(GetTextureManager()->GetTexture(m_GameOverPolyData.nTexIdx));
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�w�i�p�|���S���𐶐�����
//=============================================================================
void CGame::CreateStageBg(void)
{
	m_pStageBg = CScene2D::Create(m_StageBgData.pos, m_StageBgData.col, m_StageBgData.fWidth,
		m_StageBgData.fHeight, GAME_STAGEPOLYGON_PRIORITY);
}

//=============================================================================
// �Q�[���̃X�e�[�W���S�\���p�|���S���𐶐�����
//=============================================================================
void CGame::CreateStageLogo(void)
{
	m_pStageLogo = CScene2D::Create(m_StageLogoData.pos, m_StageLogoData.col,
		m_StageLogoData.fWidth, m_StageLogoData.fHeight, GAME_STAGEPOLYGON_PRIORITY);
	if (m_pStageLogo != NULL && GetTextureManager() != NULL)
	{
		m_pStageLogo->BindTexture(GetTextureManager()->GetTexture(m_StageLogoData.nTexIdx));
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ��\���p�|���S���𐶐�����
//=============================================================================
void CGame::CreateStageNumber(void)
{
	int nStageNumber = 0;
	D3DXVECTOR3 NumberPos = m_StageNumberData.pos;
	m_pStageNumber = new CNumber*[m_nNumNumberDigits];
	if (m_pStageNumber != NULL)
	{
		for (int nCntNumber = 0; nCntNumber < m_nNumNumberDigits; nCntNumber++)
		{// ���݂̃X�e�[�W�ԍ��̌������J��Ԃ�
			m_pStageNumber[nCntNumber] = NULL;
			m_pStageNumber[nCntNumber] = CNumber::Create(NumberPos, m_StageNumberData.col,
				m_StageNumberData.fWidth, m_StageNumberData.fHeight, 0.0f, CNumber::STATE_NONE,
				0, 3, GAME_STAGEPOLYGON_PRIORITY);
			if (m_pStageNumber[nCntNumber] != NULL && GetTextureManager() != NULL)
			{// �����ł���
				nStageNumber = (m_nStageIdx + 1) % ((int)powf(10.0f, (float)nCntNumber) * 10) / (int)powf(10.0f, (float)nCntNumber);
				m_pStageNumber[nCntNumber]->SetNumber(nStageNumber);
				m_pStageNumber[nCntNumber]->BindTexture(GetTextureManager()->GetTexture(m_nNumberTexIdx));

			}
			NumberPos.x -= m_StageNumberData.fWidth + (m_StageNumberData.fWidth * GAME_NUMBER_INTERVAL_CUT);
		}
	}
}

//=============================================================================
// �Q�[���̍��̉Ԃт�𐶐����鏈��
//=============================================================================
void CGame::CreateBlossoms(void)
{
	// �e��l�̌v�Z
	// �ړ���
	D3DXVECTOR3 Move = INITIALIZE_D3DXVECTOR3;
	Move.x = (float)((rand() % (m_CherryBlossomsData.nMoveXMax - m_CherryBlossomsData.nMoveXMin)) + m_CherryBlossomsData.nMoveXMin);
	Move.x *= -1.0f;
	Move.y = (float)((rand() % (m_CherryBlossomsData.nMoveYMax - m_CherryBlossomsData.nMoveYMin)) + m_CherryBlossomsData.nMoveYMin);

	// �p�x�̉�]�X�s�[�h
	float fAngleSpeed = (float)(rand() % (m_CherryBlossomsData.nAngleSpeedMax - m_CherryBlossomsData.nAngleSpeedMin) + m_CherryBlossomsData.nAngleSpeedMin);

	// �����̉�]�X�s�[�h
	float fRotSpeed = (float)(rand() % (m_CherryBlossomsData.nRotSpeedMax - m_CherryBlossomsData.nRotSpeedMin) + m_CherryBlossomsData.nRotSpeedMin);

	// ��
	float fWidth = (float)((rand() % (m_CherryBlossomsData.nWidthMax - m_CherryBlossomsData.nWidthMin)) + m_CherryBlossomsData.nWidthMin);

	// ����
	float fHeight = (float)((rand() % (m_CherryBlossomsData.nHeightMax - m_CherryBlossomsData.nHeightMin)) + m_CherryBlossomsData.nHeightMin);

	// �Ԃт�|���S������
	CBlossoms *pBlossoms = CBlossoms::Create(Move, D3DXToRadian(fAngleSpeed), D3DXToRadian(fRotSpeed), GAME_BLOSSOMS_POS_INI, GAME_BLOSSOMS_COL_INI, fWidth, fHeight);
	if (pBlossoms != NULL && GetTextureManager() != NULL)
	{
		pBlossoms->BindTexture(GetTextureManager()->GetTexture(m_CherryBlossomsData.nTexIdx));
	}
}

//=============================================================================
// �Q�[���̃Q�[�������U���g�ɕK�v�Ȃ��̂𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult(void)
{
	// �w�i���쐬
	CreateGameResult_Bg();

	// �X�e�[�W�ԍ��\���p�|���S�����쐬
	CreateGameResult_Stage();

	// �n�C�X�R�A�p�|���S�����쐬
	CreateGameResult_HighScore();

	// �v���C���[�X�R�A�p�|���S�����쐬
	CreateGameResult_PlayerScore();

	// �G�̃X�R�A�p���S�|���S�����쐬
	CreateGameResult_EnemyScore_Logo();

	// �g�[�^���X�R�A�p���S�|���S�����쐬
	CreateGameResult_TortalScore_Logo();
}

//=============================================================================
// �Q�[���̃Q�[�������U���g�p�w�i�𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_Bg(void)
{
	if (m_pGameResultBg == NULL)
	{
		m_pGameResultBg = CScene2D::Create(GAME_GAMERESULT_BG_POS_INI, GAME_GAMERESULT_BG_COL_INI,
			GAME_GAMERESULT_BG_WIDTH_INI, GAME_GAMERESULT_BG_HEIGHT_INI, GAME_GAMERESULT_BG_PRIORITY);
	}
}

//=============================================================================
// �Q�[���̃Q�[�������U���g�X�e�[�W�ԍ��\���p�|���S���𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_Stage(void)
{
	// ���S�|���S������
	if (m_pGameResultStage == NULL)
	{
		m_pGameResultStage = CScene2D::Create(GAME_GAMERESULT_STAGELOGO_POS_INI, GAME_GAMERESULT_STAGELOGO_COL_INI,
			GAME_GAMERESULT_STAGELOGO_WIDTH_INI, GAME_GAMERESULT_STAGELOGO_HEIGHT_INI, GAME_GAMERESULT_STAGELOGO_PRIORITY);
	}

	// �ԍ��\���p�|���S������
}

//=============================================================================
// �Q�[���̃n�C�X�R�A�𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_HighScore(void)
{

}

//=============================================================================
// �Q�[���̃v���C���[�X�R�A�𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_PlayerScore(void)
{

}

//=============================================================================
// �Q�[���̓G�̃X�R�A�p���S�|���S���𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_EnemyScore_Logo(void)
{

}

//=============================================================================
// �Q�[���̓G�̃X�R�A�p�����|���S���𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_EnemyScore_Number(void)
{

}

//=============================================================================
// �Q�[���̃g�[�^���X�R�A�p���S�|���S���𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_TortalScore_Logo(void)
{

}

//=============================================================================
// �Q�[���̃g�[�^���X�R�A�p�����|���S���𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_TortalScore_Number(void)
{

}

//=============================================================================
// �Q�[���̃{�[�i�X�X�R�A�𐶐����鏈��
//=============================================================================
void CGame::CreateGameResult_BonusScore(void)
{

}



//*****************************************************************************
//
// �J���p�֐�
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̓ǂݍ��ރ}�b�v�t�@�C�����̃|�C���^���J������
//=============================================================================
void CGame::ReleaseMapFilePointer(void)
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
// �Q�[����UI�N���X���J������
//=============================================================================
void CGame::ReleaseUI(void)
{
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = NULL;
	}
}

//=============================================================================
// �Q�[���̃v���C���[�N���X���J������
//=============================================================================
void CGame::ReleasePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̃v���C���[�Ǌ��N���X���J������
//=============================================================================
void CGame::ReleasePlayerManager(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayerManager[nCntPlayer] != NULL)
		{
			m_pPlayerManager[nCntPlayer]->Uninit();
			delete m_pPlayerManager[nCntPlayer];
			m_pPlayerManager[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̓G�f�[�^�Ǌ��N���X���J������
//=============================================================================
void CGame::ReleaseEnemyManager(void)
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

//=============================================================================
// �Q�[���̃Q�[���I�[�o�[���S�\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameOverLogo(void)
{
	if (m_pGameOverLogo != NULL)
	{
		m_pGameOverLogo->Uninit();
		m_pGameOverLogo = NULL;
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�w�i�\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseStageBg(void)
{
	if (m_pStageBg != NULL)
	{
		m_pStageBg->Uninit();
		m_pStageBg = NULL;
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W���S�\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseStageLogo(void)
{
	if (m_pStageLogo != NULL)
	{
		m_pStageLogo->Uninit();
		m_pStageLogo = NULL;
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ��\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseStageNumber(void)
{
	if (m_pStageNumber != NULL)
	{
		for (int nCntNumber = 0; nCntNumber < m_nNumNumberDigits; nCntNumber++)
		{
			if (m_pStageNumber[nCntNumber] != NULL)
			{
				m_pStageNumber[nCntNumber]->Uninit();
				m_pStageNumber[nCntNumber] = NULL;
			}
		}
		delete[] m_pStageNumber;
		m_pStageNumber = NULL;
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ��\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult(void)
{
	ReleaseGameResult_Bg();
	ReleaseGameResult_Stage();
	ReleaseGameResult_HighScore();
	ReleaseGameResult_PlayerScore();
	ReleaseGameResult_EnemyScore();
	ReleaseGameResult_TortalScore();
	ReleaseGameResult_BonusScore();
}

//=============================================================================
// �Q�[���̃Q�[�����U���g�w�i�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_Bg(void)
{
	if (m_pGameResultBg != NULL)
	{
		m_pGameResultBg->Uninit();
		m_pGameResultBg = NULL;
	}
}

//=============================================================================
// �Q�[���̃Q�[�������U���g�X�e�[�W�\���p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_Stage(void)
{
	// �X�e�[�W���S
	if (m_pGameResultStage != NULL)
	{
		m_pGameResultStage->Uninit();
		m_pGameResultStage = NULL;
	}

	// �X�e�[�W�ԍ��\���p�|���S��
	if (m_apGameResultStageNumber != NULL)
	{
		for (int nCntDigit = 0; nCntDigit < m_nNumNumberDigits; nCntDigit++)
		{
			if (m_apGameResultStageNumber[nCntDigit] != NULL)
			{
				m_apGameResultStageNumber[nCntDigit]->Uninit();
				m_apGameResultStageNumber[nCntDigit] = NULL;
			}
		}
		delete[] m_apGameResultStageNumber;
		m_apGameResultStageNumber = NULL;
	}
}

//=============================================================================
// �Q�[���̃n�C�X�R�A�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_HighScore(void)
{
	// �����|���S��
	if (m_apHighScore != NULL)
	{
		for (int nCntDigit = 0; nCntDigit < m_nNumHighScoreDigit; nCntDigit++)
		{
			if (m_apHighScore[nCntDigit] != NULL)
			{
				m_apHighScore[nCntDigit]->Uninit();
				m_apHighScore[nCntDigit] = NULL;
			}
		}
		delete[] m_apHighScore;
		m_apHighScore = NULL;
	}

	// �n�C�X�R�A���S
	if (m_pHighScoreLogo != NULL)
	{
		m_pHighScoreLogo->Uninit();
		m_pHighScoreLogo = NULL;
	}
}

//=============================================================================
// �Q�[���̃v���C���[�X�R�A�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_PlayerScore(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// �v���C���[�̑������J��Ԃ�
	    // �����|���S��
		if (m_apScore[nCntPlayer] != NULL)
		{
			for (int nCntDigit = 0; nCntDigit < m_nScoreDigit[nCntPlayer]; nCntDigit++)
			{
				if (m_apScore[nCntPlayer][nCntDigit] != NULL)
				{
					m_apScore[nCntPlayer][nCntDigit]->Uninit();
					m_apScore[nCntPlayer][nCntDigit] = NULL;
				}
			}
			delete[] m_apScore[nCntPlayer];
			m_apScore[nCntPlayer] = NULL;
		}

		// �v���C���[���S
		if (m_apPlayerLogo[nCntPlayer] != NULL)
		{
			m_apPlayerLogo[nCntPlayer]->Uninit();
			m_apPlayerLogo[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̃G�l�~�[�X�R�A�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_EnemyScore(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// �v���C���[�̑������J��Ԃ�
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{// �G�̎�ޕ��J��Ԃ�
			// �G��|�������|���S��
			if (m_apNumBreakEnemy[nCntPlayer][nCntEnemy] != NULL)
			{
				for (int nCntDigit = 0; nCntDigit < m_nNumBreakEnemyDigit[nCntPlayer][nCntEnemy]; nCntDigit++)
				{
					if (m_apNumBreakEnemy[nCntPlayer][nCntEnemy][nCntDigit] != NULL)
					{
						m_apNumBreakEnemy[nCntPlayer][nCntEnemy][nCntDigit]->Uninit();
						m_apNumBreakEnemy[nCntPlayer][nCntEnemy][nCntDigit] = NULL;
					}
				}
				delete[] m_apNumBreakEnemy[nCntPlayer][nCntEnemy];
				m_apNumBreakEnemy[nCntPlayer][nCntEnemy] = NULL;
			}

			// �G�̃X�R�A�\���p�|���S��
			if (m_apEnemyScore[nCntPlayer][nCntEnemy] != NULL)
			{
				for (int nCntDigit = 0; nCntDigit < m_nNumEnemyScoreDigit[nCntPlayer][nCntEnemy]; nCntDigit++)
				{
					if (m_apEnemyScore[nCntPlayer][nCntEnemy][nCntDigit] != NULL)
					{
						m_apEnemyScore[nCntPlayer][nCntEnemy][nCntDigit]->Uninit();
						m_apEnemyScore[nCntPlayer][nCntEnemy][nCntDigit] = NULL;
					}
				}
				delete[] m_apEnemyScore[nCntPlayer][nCntEnemy];
				m_apEnemyScore[nCntPlayer][nCntEnemy] = NULL;
			}

			// �|�C���g���S�\���p�|���S��
			if (m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy] != NULL)
			{
				m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy]->Uninit();
				m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy] = NULL;
			}

			// �X�R�A���\���p�|���S��
			if (m_apEnemyScoreArrow[nCntPlayer][nCntEnemy] != NULL)
			{
				m_apEnemyScoreArrow[nCntPlayer][nCntEnemy]->Uninit();
				m_apEnemyScoreArrow[nCntPlayer][nCntEnemy] = NULL;
			}
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{// �G�̎�ޕ��J��Ԃ�
		// �G�A�C�R���\���p�|���S��
		if (m_apEnemyScoreIcon[nCntEnemy] != NULL)
		{
			m_apEnemyScoreIcon[nCntEnemy]->Uninit();
			m_apEnemyScoreIcon[nCntEnemy] = NULL;
		}
	}
}

//=============================================================================
// �Q�[���̃g�[�^���X�R�A�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_TortalScore(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// �v���C���[�̑������J��Ԃ�
	    // �����|���S��
		if (m_apNumAllBreakEnemy[nCntPlayer] != NULL)
		{
			for (int nCntDigit = 0; nCntDigit < m_nNumAllBreakEnemyDigit[nCntPlayer]; nCntDigit++)
			{
				if (m_apNumAllBreakEnemy[nCntPlayer][nCntDigit] != NULL)
				{
					m_apNumAllBreakEnemy[nCntPlayer][nCntDigit]->Uninit();
					m_apNumAllBreakEnemy[nCntPlayer][nCntDigit] = NULL;
				}
			}
			delete[] m_apNumAllBreakEnemy[nCntPlayer];
			m_apNumAllBreakEnemy[nCntPlayer] = NULL;
		}
	}

	// �g�[�^�����S�p�|���S��
	if (m_pTortalLogo != NULL)
	{
		m_pTortalLogo->Uninit();
		m_pTortalLogo = NULL;
	}

	// �g�[�^�����C���p�|���S��
	if (m_pTortalLine != NULL)
	{
		m_pTortalLine->Uninit();
		m_pTortalLine = NULL;
	}
}

//=============================================================================
// �Q�[���̃{�[�i�X�X�R�A�p�|���S�����J������
//=============================================================================
void CGame::ReleaseGameResult_BonusScore(void)
{
	// �����|���S��
	if (m_apBonus != NULL)
	{
		for (int nCntDigit = 0; nCntDigit < m_nBonusDigit; nCntDigit++)
		{
			if (m_apBonus[nCntDigit] != NULL)
			{
				m_apBonus[nCntDigit]->Uninit();
				m_apBonus[nCntDigit] = NULL;
			}
		}
		delete[] m_apBonus;
		m_apBonus = NULL;
	}

	// �{�[�i�X�X�R�A�|�C���g���S�\���p�|���S��
	if (m_pBonusPointLogo != NULL)
	{
		m_pBonusPointLogo->Uninit();
		m_pBonusPointLogo = NULL;
	}

	// �{�[�i�X�X�R�A���S�\���p�|���S��
	if (m_pBonusScoreLogo != NULL)
	{
		m_pBonusScoreLogo->Uninit();
		m_pBonusScoreLogo = NULL;
	}
}

//*****************************************************************************
//
// �X�N���v�g�ǂݍ��ݗp�֐�(�}�b�v�C�x���g�p�ɃX�N���v�g�����Ȃ炱���ɁI�I)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̃V�X�e������ǂݍ���
//=============================================================================
void CGame::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(GAME_SYSTEM_FILENAME);
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
// �Q�[���̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
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
			CreateTexture(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// �ǂݍ��ރe�N�X�`���̃t�@�C����������
			LoadTexFileName(pStr, nCntTex);
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
		else if (CFunctionLib::Memcmp(pStr, ITEMEVENT_FILENAME) == 0)
		{// �A�C�e���C�x���g�p�X�N���v�g�t�@�C����������
			LoadItemEvent(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, NUMBER_TEX_IDX) == 0)
		{// �����̎g�p����e�N�X�`���ԍ�������
			m_nNumberTexIdx = CFunctionLib::ReadInt(pStr, NUMBER_TEX_IDX);
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
		else if (CFunctionLib::Memcmp(pStr, STAGEBGSET) == 0)
		{// �X�e�[�W�w�i�p�|���S����񂾂���
			LoadStageBg(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, STAGELOGOSET) == 0)
		{// �X�e�[�W���S�p�|���S����񂾂���
			LoadStageLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, STAGENUMBERSET) == 0)
		{// �X�e�[�W�ԍ��\���p�|���S����񂾂���
			LoadStageNumber(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEOVERPOLYGONSET) == 0)
		{// �Q�[���I�[�o�[���S�|���S����񂾂���
			LoadGameOverLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, HINAMATSURI_FILENAME) == 0)
		{// �ЂȍՂ�C�x���g�p�X�N���v�g�t�@�C����������
			LoadHinamatsuriEvent(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̓ǂݍ��ރe�N�X�`���t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadTexFileName(char *pStr, int nCntTex)
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
// �Q�[���̓ǂݍ��ރA�C�e�����f����ǂݍ���
//=============================================================================
void CGame::LoadModel(char *pStr, int nCntModel)
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
// �Q�[���̓ǂݍ��ރ}�b�v�t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadMapFileName(char *pStr, int nCntMap)
{
	strcpy(m_pMapFileName[nCntMap], CFunctionLib::ReadString(pStr, m_pMapFileName[nCntMap], MAP_FILENAME));
}

//=============================================================================
// �Q�[���̓ǂݍ��ރG�t�F�N�g�t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// �G�t�F�N�g�Ǌ��N���X�𐶐�
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
}

//=============================================================================
// �Q�[���̓ǂݍ��ރv���C���[�f�[�^�t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadPlayerFileName(char *pStr, int nCntPlayer)
{
	char aPlayerFileName[256] = "\0";
	strcpy(aPlayerFileName, CFunctionLib::ReadString(pStr, aPlayerFileName, PLAYERDATA_FILENAME));

	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (nCntPlayer == CCharaSelect::GetPlayerNumber(nCnt))
		{
			m_pPlayerManager[nCnt] = CPlayerManager::Create(aPlayerFileName);
		}
	}
}

//=============================================================================
// �Q�[���̓ǂݍ��ޓG�f�[�^�t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadEnemyFileName(char *pStr, int nCntEnemy)
{
	char aEnemyFileName[256] = "\0";
	strcpy(aEnemyFileName, CFunctionLib::ReadString(pStr, aEnemyFileName, ENEMY_FILENAME));

	// �L�����N�^�[�Ǌ��N���X���쐬
	m_pEnemyManager[nCntEnemy] = CCharacterManager::Create(aEnemyFileName);
}

//=============================================================================
// �Q�[���̃X�e�[�W�w�i�|���S������ǂݍ���
//=============================================================================
void CGame::LoadStageBg(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_StageBgData.pos = GAME_STAGEBG_POS_INI;
	m_StageBgData.col = GAME_STAGEBG_COL_INI;
	m_StageBgData.fWidth = GAME_STAGEBG_WIDTH_INI;
	m_StageBgData.fHeight = GAME_STAGEBG_HEIGHT_INI;

	// �f�[�^�ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_StageBgData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_StageBgData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_StageBgData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_StageBgData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGEBGSET) == 0)
		{// �X�e�[�W�w�i�|���S�����I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W���S�|���S������ǂݍ���
//=============================================================================
void CGame::LoadStageLogo(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_StageLogoData.nTexIdx = 0;
	m_StageLogoData.pos = GAME_STAGELOGO_POS_INI;
	m_StageLogoData.col = GAME_STAGELOGO_COL_INI;
	m_StageLogoData.fWidth = GAME_STAGELOGO_WIDTH_INI;
	m_StageLogoData.fHeight = GAME_STAGELOGO_HEIGHT_INI;

	// �f�[�^�ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_StageLogoData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_StageLogoData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_StageLogoData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_StageLogoData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_StageLogoData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGELOGOSET) == 0)
		{// �X�e�[�W���S�|���S�����I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ��\���p�|���S������ǂݍ���
//=============================================================================
void CGame::LoadStageNumber(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_StageNumberData.pos = GAME_STAGENUMBER_POS_INI;
	m_StageNumberData.col = GAME_STAGENUMBER_COL_INI;
	m_StageNumberData.fWidth = GAME_STAGENUMBER_WIDTH_INI;
	m_StageNumberData.fHeight = GAME_STAGENUMBER_HEIGHT_INI;

	// �f�[�^�ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_StageNumberData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_StageNumberData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_StageNumberData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_StageNumberData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGENUMBERSET) == 0)
		{// �X�e�[�W�ԍ��\���p�|���S�����I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̃Q�[���I�[�o�[���S�|���S������ǂݍ���
//=============================================================================
void CGame::LoadGameOverLogo(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_GameOverPolyData.nTexIdx = 0;
	m_GameOverPolyData.pos = GAME_GAMEOVERLOGO_POS_INI;
	m_GameOverPolyData.col = GAME_GAMEOVERLOGO_COL_INI;
	m_GameOverPolyData.fWidth = GAME_GAMEOVERLOGO_WIDTH_INI;
	m_GameOverPolyData.fHeight = GAME_GAMEOVERLOGO_HEIGHT_INI;

	// �f�[�^�ǂݍ���
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_GameOverPolyData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// �|���S���̈ʒu������
			m_GameOverPolyData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// �|���S���̐F������
			m_GameOverPolyData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_GameOverPolyData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_GameOverPolyData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_GAMEOVERPOLYGONSET) == 0)
		{// �Q�[���I�[�o�[���S�|���S�����I���̍��}������
			break;
		}
	}
}


//*****************************************************************************
//
// �A�C�e���C�x���g�p�X�N���v�g�ǂݍ��ݏ���
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̃A�C�e���C�x���g�X�N���v�g��ǂݍ���
//=============================================================================
void CGame::LoadItemEvent(char *pStr)
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
// �Q�[���̃A�C�e���C�x���g�����t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadItemEventScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SHAKESET) == 0)
		{// �J�����h��C�x���g���ǂݍ��݊J�n�̍��}������
			LoadItemEvent_Shake(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMY_STOP) == 0)
		{// �G���ǂꂭ�炢�~�߂邩��񂾂���
			m_ItemEventData.nStop = CFunctionLib::ReadInt(pStr, ENEMY_STOP);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̃J�����h��C�x���g�����t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadItemEvent_Shake(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// �h�炷���Ԃ�����
			m_ItemEventData.nShakeTime = CFunctionLib::ReadInt(pStr, TIME);
		}
		else if (CFunctionLib::Memcmp(pStr, VALUE) == 0)
		{// �ǂꂭ�炢�h�炷��������
			m_ItemEventData.fShake = CFunctionLib::ReadFloat(pStr, VALUE);
		}
		else if (CFunctionLib::Memcmp(pStr, CUT) == 0)
		{// �ǂꂭ�炢�h������炷��������
			m_ItemEventData.fShakeCut = CFunctionLib::ReadFloat(pStr, CUT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SHAKESET) == 0)
		{// �h��C�x���g�I���̍��}������
			break;
		}
	}
}


//*****************************************************************************
//
// �ЂȍՂ�C�x���g�p�X�N���v�g�ǂݍ��ݏ���
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// �Q�[���̂ЂȍՂ�C�x���g�X�N���v�g��ǂݍ���
//=============================================================================
void CGame::LoadHinamatsuriEvent(char *pStr)
{
	char aFileName[256] = "\0";
	strcpy(aFileName, CFunctionLib::ReadString(pStr, aFileName, HINAMATSURI_FILENAME));
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(aFileName);
	if (pFileLoader != NULL)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// �ǂݍ��݊J�n�̍��}������
			LoadHinamatsuriEventScript(pFileLoader, pStr);
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
// �Q�[���̂ЂȍՂ�C�x���g�����t�@�C������ǂݍ���
//=============================================================================
void CGame::LoadHinamatsuriEventScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ���[�v�J�n
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, CHERRYBLOSSOMSSET) == 0)
		{// ���̉Ԃт��񂾂���
			LoadCherryBlossomsData(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			break;
		}
	}
}

//=============================================================================
// �Q�[���̍��̉Ԃт�f�[�^����ǂݍ���
//=============================================================================
void CGame::LoadCherryBlossomsData(CFileLoader *pFileLoader, char *pStr)
{
	// �f�[�^������
	m_CherryBlossomsData.nTime = GAME_BLOSSOMS_TIME_INI;
	m_CherryBlossomsData.nAppear = GAME_BLOSSOMS_APPEAR_INI;
	m_CherryBlossomsData.nTexIdx = 0;
	m_CherryBlossomsData.nMoveXMax = GAME_BLOSSOMS_MOVEX_MAX_INI;
	m_CherryBlossomsData.nMoveXMin = GAME_BLOSSOMS_MOVEX_MIN_INI;
	m_CherryBlossomsData.nMoveYMax = GAME_BLOSSOMS_MOVEY_MAX_INI;
	m_CherryBlossomsData.nMoveYMin = GAME_BLOSSOMS_MOVEY_MIN_INI;
	m_CherryBlossomsData.nWidthMax = GAME_BLOSSOMS_WIDTH_MAX_INI;
	m_CherryBlossomsData.nWidthMin = GAME_BLOSSOMS_WIDTH_MIN_INI;
	m_CherryBlossomsData.nHeightMax = GAME_BLOSSOMS_HEIGHT_MAX_INI;
	m_CherryBlossomsData.nHeightMin = GAME_BLOSSOMS_HEIGHT_MIN_INI;
	m_CherryBlossomsData.nAngleSpeedMax = GAME_BLOSSOMS_ANGLESPEED_MAX_INI;
	m_CherryBlossomsData.nAngleSpeedMin = GAME_BLOSSOMS_ANGLESPEED_MIN_INI;
	m_CherryBlossomsData.nRotSpeedMax = GAME_BLOSSOMS_ROTSPEED_MAX_INI;
	m_CherryBlossomsData.nRotSpeedMin = GAME_BLOSSOMS_ROTSPEED_MIN_INI;

	// �f�[�^�ǂݍ���
	int nWord = 0;
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// ���̉Ԃт炪�~�蒍������
			m_CherryBlossomsData.nTime = CFunctionLib::ReadInt(pStr, TIME);
		}
		if (CFunctionLib::Memcmp(pStr, APPEAR) == 0)
		{// ���̉Ԃт��1�t���[�����ɂǂꂭ�炢�o����
			m_CherryBlossomsData.nAppear = CFunctionLib::ReadInt(pStr, APPEAR);
		}
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// �g�p����e�N�X�`���̔ԍ�������
			m_CherryBlossomsData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, MOVEX) == 0)
		{// X�����̈ړ��ʂ�����
			m_CherryBlossomsData.nMoveXMax = CFunctionLib::ReadInt(pStr, MOVEX);
			pStr = CFunctionLib::HeadPutout(pStr, MOVEX);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nMoveXMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, MOVEY) == 0)
		{// Y�����̈ړ��ʂ�����
			m_CherryBlossomsData.nMoveYMax = CFunctionLib::ReadInt(pStr, MOVEY);
			pStr = CFunctionLib::HeadPutout(pStr, MOVEY);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nMoveYMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// �|���S���̕�������
			m_CherryBlossomsData.nWidthMax = CFunctionLib::ReadInt(pStr, WIDTH);
			pStr = CFunctionLib::HeadPutout(pStr, WIDTH);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nWidthMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// �|���S���̍���������
			m_CherryBlossomsData.nHeightMax = CFunctionLib::ReadInt(pStr, HEIGHT);
			pStr = CFunctionLib::HeadPutout(pStr, HEIGHT);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nHeightMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, ANGLESPEED) == 0)
		{// �p�x����]������X�s�[�h������
			m_CherryBlossomsData.nRotSpeedMax = CFunctionLib::ReadInt(pStr, ANGLESPEED);
			pStr = CFunctionLib::HeadPutout(pStr, ANGLESPEED);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nRotSpeedMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, ROTSPEED) == 0)
		{// ��������]������X�s�[�h������
			m_CherryBlossomsData.nRotSpeedMax = CFunctionLib::ReadInt(pStr, ROTSPEED);
			pStr = CFunctionLib::HeadPutout(pStr, ROTSPEED);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nRotSpeedMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, END_CHERRYBLOSSOMSSET) == 0)
		{// ���̉Ԃт�f�[�^�I���̍��}������
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
// �Q�[���̕ϐ�������������
//=============================================================================
void CGame::ClearVariable(void)
{
	m_State = STATE_STAGE_DISP;
	m_pUI = NULL;
	m_pMapFileName = NULL;
	m_nStateCounter = 0;
	m_nMapIdx = 0;
	m_nStageIdx = 0;
	m_nNumMap = 0;
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;
	m_nNumEnemy = 0;
	m_pGameOverLogo = NULL;
	m_pStageBg = NULL;
	m_pStageLogo = NULL;
	m_pStageNumber = NULL;
	m_nNumberTexIdx = 0;
	m_nNumNumberDigits = NULL;
	m_nPlayerRespawnCounter = 0;
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_bDeletePlayerFlag = 0;
	m_bEnemyMove = true;
	m_nEnemyMoveCounter = 0;
	m_HinaEvent = HINAEVENT_CHERRY_BLOSSOMS;
	m_nEventCounter = 0;
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;
		m_pPlayerManager[nCntPlayer] = NULL;
		m_nPlayerStock[nCntPlayer] = 0;
	}
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		m_pEnemyManager[nCntEnemy] = NULL;
	}



	// �Q�[�������U���g�p�f�[�^
	// �w�i�p
	m_pGameResultBg = NULL;

	// �X�e�[�W�ԍ��\���p
	m_pGameResultStage = NULL;
	m_apGameResultStageNumber = NULL;

	// �n�C�X�R�A�p
	m_apHighScore = NULL;
	m_nNumHighScoreDigit = 0;
	m_pHighScoreLogo = NULL;

	// �v���C���[�X�R�A�p
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nScore[nCntPlayer] = 0;
		m_apScore[nCntPlayer] = NULL;
		m_nScoreDigit[nCntPlayer] = NULL;
		m_apPlayerLogo[nCntPlayer] = NULL;
	}

	// �G�l�~�[�X�R�A�p
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{
			m_nNumBreakEnemy[nCntPlayer][nCntEnemy] = 0;
			m_apNumBreakEnemy[nCntPlayer][nCntEnemy] = NULL;
			m_nNumBreakEnemyDigit[nCntPlayer][nCntEnemy] = 0;
			m_apEnemyScore[nCntPlayer][nCntEnemy] = NULL;
			m_nNumEnemyScoreDigit[nCntPlayer][nCntEnemy] = 0;
			m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy] = NULL;
			m_apEnemyScoreArrow[nCntPlayer][nCntEnemy] = NULL;
		}
	}
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		m_apEnemyScoreIcon[nCntEnemy] = NULL;
	}

	// �g�[�^���X�R�A�p
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nNumAllBreakEnemy[nCntPlayer] = 0;
		m_apNumAllBreakEnemy[nCntPlayer] = NULL;
		m_nNumAllBreakEnemyDigit[nCntPlayer] = 0;
		m_nScoreDigit[nCntPlayer] = NULL;
		m_apPlayerLogo[nCntPlayer] = NULL;
	}
	m_pTortalLogo = NULL;
	m_pTortalLine = NULL;

	// �{�[�i�X�X�R�A�p
	m_apBonus = NULL;
	m_nBonusDigit = 0;
	m_pBonusPointLogo = NULL;
	m_pBonusScoreLogo = NULL;
}

//=============================================================================
// �Q�[���̒e���f����ݒ肷�鏈��
//=============================================================================
void CGame::SetBulletModel(CBullet *pBullet)
{
	pBullet->BindModel(GetModelCreate()->GetMesh(m_nBulletModelIdx), GetModelCreate()->GetBuffMat(m_nBulletModelIdx),
		GetModelCreate()->GetNumMat(m_nBulletModelIdx), GetModelCreate()->GetTexture(m_nBulletModelIdx));
}

//=============================================================================
// �Q�[���̏�Ԃ�ݒ肷��
//=============================================================================
void CGame::SetState(const STATE state)
{
	m_StateOld = m_State;
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// �Q�[���̃v���C���[�N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CGame::SetPlayer(CPlayer *pPlayer, const int nIdx)
{
	m_pPlayer[nIdx] = pPlayer;
}

//=============================================================================
// �Q�[���̏o������G�̐���ݒ肷��
//=============================================================================
void CGame::SetNumEnemy(const int nNumEnemy)
{
	m_nNumEnemy = nNumEnemy;
}

//=============================================================================
// �Q�[���̏�Ԃ��擾����
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
// �Q�[���̃v���C���[�N���X�ւ̃|�C���^���擾����
//=============================================================================
CPlayer *CGame::GetPlayer(const int nIdx)
{
	return m_pPlayer[nIdx];
}

//=============================================================================
// �Q�[���̏o������G�̐����擾����
//=============================================================================
int CGame::GetNumEnemy(void)
{
	return m_nNumEnemy;
}

//=============================================================================
// �Q�[���̓G�𓮂����邩�ǂ����擾����
//=============================================================================
bool CGame::GetEnemyMove(void)
{
	return m_bEnemyMove;
}