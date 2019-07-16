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
#include "blossoms.h"
#include "debugproc.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define GAME_DEBUG                                                    // �錾���f�o�b�O�R�}���h�K�p
#ifdef GAME_DEBUG
#define GAME_DEBUG_STARTMAP (3)
#endif

#define GAME_SYSTEM_FILENAME             "data/TEXT/MODE/game.ini"    // �������Ɏg�p����V�X�e���t�@�C����
#define GAME_STAGEDISP_TIME              (120)                        // �X�e�[�W�ԍ��\����ԂɂȂ��Ă���ʏ��Ԃɐ؂�ւ��܂ł̎���
#define GAME_NEXTMODE_TIMING             (60)                         // �I����ԂɂȂ��Ă��玟�̃��[�h�ɐ؂�ւ��܂ł̎���
#define GAME_GAMEOVER_TIMING             (180)                        // �Q�[���I�[�o�[��ԂɂȂ��Ă��玟�̃��[�h�ɐ؂�ւ��܂ł̎���
#define GAME_GAMEOVER_POSUP              (8.0f)                       // �Q�[���I�[�o�[���S���ǂꂭ�炢�グ�邩
#define GAME_GAMEOVER_POSYMAX            (10.0f)                      // �Q�[���I�[�o�[�|���S����Y���W�ő�l(�|���S���̍����ɉ����Ĕ���)
#define GAME_NUMBER_INTERVAL_CUT         (0.8f)                       // �����̊Ԋu�̔{��(�|���S���̕�������ŏ�Z����)
#define GAME_PLAYER_STOCK_INI            (3)                          // �v���C���[�̎c�@�������l
#define GAME_PLAYER_RESPAWN_TIMING       (180)                        // �v���C���[�����X�|�[������܂ł̎���
#define GAME_MAPSTAGE_MAX                (3)                          // �X�e�[�W�̎�ނ��Ƃ̐�
#define GAME_STAGEPOLYGON_PRIORITY       (6)                          // �X�e�[�W�|���S���̕`��D�揇��
#define GAME_GAMEOVERPOLYGON_PRIORITY    (6)                          // �Q�[���I�[�o�[�|���S���̕`��D�揇��

// �A�C�e���C�x���g�p�}�N��
#define GAME_ITEMEVENT_ENEMYSTOPTIME     (300)                        // �G���ǂꂭ�炢�~�߂Ă�����

// �}�b�v�X�V���ɏ������I�u�W�F�N�g(����p�̃}�N���Ȃ̂ł����ɒǉ�)
#define GAME_DELETE_OBJECT (pScene->GetObjType() == CScene::OBJTYPE_BULLET || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER || pScene->GetObjType() == CScene::OBJTYPE_EMITTER || pScene->GetObjType() == CScene::OBJTYPE_BLOSSOMS)

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
#define ENEMY_STOP "ENEMY_STOP = "

// �G�t�F�N�g�p
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// �e�p
#define BULLET_MODEL_IDX "BULLET_MODEL_IDX = "

// �A�C�e���p
#define ITEM_MODEL_IDX "ITEM_MODEL_IDX = "

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
	ReleaseGameOverLogo();
	ReleaseStageBg();
	ReleaseStageLogo();
	ReleaseStageNumber();

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
	case STATE_GAMEOVER:
		GameOverUpdate();
		CScene::UpdateAll();
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
	case MAPTYPE_NEWYEAR:      // ������
		MapEvent_NewYear();
		break;
	case MAPTYPE_HINAMATSURI:  // �ЂȍՂ�
		MapEvent_Hinamatsuri();
		break;
	}

	// �v���C���[�����X�|�[�������邩�`�F�b�N
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (CManager::GetClient()->GetClientId() == nCntPlayer)
		{
			CheckPlayerResSpawn(nCntPlayer);
		}
	}

	// �G�𐶐����邩�`�F�b�N����
	if (CManager::GetClient()->GetClientId() == 0 && m_State != STATE_STAGE_SELECT && m_State != STATE_STAGE_DISP)
	CheckEnemySpawn(m_nGameCounter);

	// �T�[�o�[�֑���f�[�^��ݒ肷��
	SetDataToServer();

	// �N���C�A���g�̍X�V����
	CManager::ClientUpdate();

	// �T�[�o�[���瑗��ꂽ�f�[�^��ݒ肷��
	GetDataFromServer();

	// �f�[�^�N���A
	m_bDeletePlayerFlag = false;
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");


	if (CManager::GetClient() == NULL)return;
	CDebugProc::Print(1, "�Q�[�����\n");
	CDebugProc::Print(1, "�N���C�A���g�ԍ�      : %d\n", CManager::GetClient()->GetClientId());
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
	char *pStr = CManager::GetClient()->GetReceiveMessage();
	if (pStr == NULL) return;
	if (*pStr == *"???") return;

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
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] =
				CPlayer::Create(INITIALIZE_D3DXVECTOR3,
					INITIALIZE_D3DXVECTOR3, (CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER);
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

			// �v���C���[�ɒl��ݒ�
			if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL)
			{
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetPos(D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ));
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetRot(D3DXVECTOR3(fGetPlayerRotX, fGetPlayerRotY, fGetPlayerRotZ));
			}
		}
	}


	// �z�X�g����Ȃ�������
	if (CManager::GetClient()->GetClientId() != 0)
	{
		nNumPlayer = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		if (nNumPlayer == 1 && m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
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
	if (m_State != STATE_END && m_State != STATE_GAMEOVER)
	{// �I����Ԃł��Q�[���I�[�o�[��Ԃł��Ȃ�
		m_nStateCounter = 0;
	}

	// �}�b�v�̔ԍ���ݒ�
	m_nMapIdx = nMapIdx;

	// ��Ԃɂ���ď����킯
	if (m_State == STATE_STAGE_DISP && GetMap() == NULL)
	{
		CreateMap();
		CManager::GetSound()->PlaySound(1);
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
	}
	else if (m_State == STATE_STAGE_DISP || m_State == STATE_STAGE_SELECT)
	{
		ReleaseStageNumber();
		CalcStageDigits();
		CreateStageNumber();
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

	// �G�̔ԍ�
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// �G�𐶐�����(�����ɕK�v�ȃf�[�^��ݒ�)
	CEnemy *pEnemy = CEnemy::Create(D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ), D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ));
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
// ��Ԃɂ�鏈���킯�p�֐�
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
		CManager::GetSound()->PlaySound(1);
	}
}

//=============================================================================
// �Q�[���̃X�e�[�W�ԍ���\�������Ԃ̍X�V����
//=============================================================================
void CGame::StageDispUpdate(void)
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

	// �J�E���^�[���Z
	m_nStateCounter++;
	if (m_nStateCounter % GAME_STAGEDISP_TIME == 0)
	{// �X�e�[�W�ԍ����o���Ă��炠����x������
		SetState(STATE_NORMAL);
		ReleaseStageBg();
		ReleaseStageLogo();
		ReleaseStageNumber();
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

	// �X�e�[�W�Z���N�g�f�o�b�O
	if (CManager::GetClient()->GetClientId() == 0)
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
// �Q�[���̃Q�[���N���A��Ԃ̍X�V����
//=============================================================================
void CGame::GameClearUpdate(void)
{

}

//=============================================================================
// �Q�[���̃}�b�v��ς����Ԃ̍X�V����
//=============================================================================
void CGame::ChangeMapUpdate(void)
{
	if (CManager::GetClient()->GetClientId() == 0)
	{// �z�X�g��������
		m_nMapIdx++;
		m_nStageIdx++;
		CalcStageDigits();
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
		SetState(STATE_STAGE_DISP);
		CManager::GetSound()->PlaySound(1);
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
		CManager::GetSound()->PlaySound(1);
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
	m_nStateCounter++;
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
			if (CManager::GetClient()->GetClientId() == 0)
			{// �z�X�g������
				SetState(STATE_CHANGE_MAP);
			}
		}
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
// Auther : Hodaka Niwa & Jukiya Hayakawa & Kobayashi Shouhei & Gokan Akane
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
// �Q�[���̂������̃}�b�v�C�x���g����
//=============================================================================
void CGame::MapEvent_NewYear(void)
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
	if (pMap != NULL)
	{
		PlayerPos = pMap->GetPlayerRespawn(CManager::GetClient()->GetClientId())->GetPos();
	}

	if (m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
	{
		m_pPlayer[CManager::GetClient()->GetClientId()]->SetPos(PlayerPos);
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
			m_pPlayer[nCntPlayer] = CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);

			// �v���C���[�̃��X�|�[���ʒu���擾
			D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
			CMap *pMap = GetMap();
			if (pMap != NULL)
			{
				PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			}

			// �v���C���[�̈ʒu�����炷
			m_pPlayer[nCntPlayer]->SetPos(PlayerPos);
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
// �Q�[���̓G�̐�������
//=============================================================================
void CGame::EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt)
{
	// �G�̃��X�|�[���ʒu���擾
	D3DXVECTOR3 EnemyPos = pMap->GetEnemyRespawn(pEnemyData->GetRespawnIdx())->GetPos();

	// �G�̐���(�����͌�Ŏ�ޕ������Ă���)
	CEnemy::Create(EnemyPos, INITIALIZE_D3DXVECTOR3);
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
		CCamera *pCamera = CCamera::Create();
		if (pCamera != NULL)
		{
			pCameraManager->SetCamera(pCamera);
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
// �Q�[���̃��f���Ǌ��N���X��������
//=============================================================================
void CGame::CreateItemModel(int nNumItem)
{
	CModelCreate *pModelCreate = CModelCreate::Create(nNumItem);
	SetModelCreate(pModelCreate);
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
		m_pPlayer[nCntPlayer] = CPlayer::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);
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
		m_nPlayerStock[nCntPlayer] = 0;
	}
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
// �Q�[���̃A�C�e�����f����ݒ肷�鏈��
//=============================================================================
void CGame::SetItemModel(CItem *pItem, int nType)
{
	pItem->BindModel(GetModelCreate()->GetMesh(m_nItemModelIdx[nType]), GetModelCreate()->GetBuffMat(m_nItemModelIdx[nType]),
		GetModelCreate()->GetNumMat(m_nItemModelIdx[nType]), GetModelCreate()->GetTexture(m_nItemModelIdx[nType]));
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