//*****************************************************************************
//
//     UI�̏���[UI.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "UI.h"
#include "scene2D.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
// �������p�}�N��
// �w�i�p
#define UI_BG_HEIGHT_INI            (SCREEN_HEIGHT / 2)
#define UI_BG_POS_INI               (D3DXVECTOR3(SCREEN_WIDTH - UI_BG_WIDTH_INI,SCREEN_HEIGHT / 2,0.0f))
#define UI_BG_COL_INI               (D3DXCOLOR(0.3f,0.3f,0.3f,1.0f))

// �G�A�C�R���p
#define UI_ENEMYICON_WIDTH_INI      (90.0f)
#define UI_ENEMYICON_HEIGHT_INI     (SCREEN_HEIGHT / 2)
#define UI_ENEMYICON_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH ,SCREEN_HEIGHT / 2,0.0f))
#define UI_ENEMYICON_COL_INI        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

// �v���C���[�A�C�R���p
// �P��
#define UI_PLAYERICON_0_WIDTH_INI   (16.0f)
#define UI_PLAYERICON_0_HEIGHT_INI  (16.0f)
#define UI_PLAYERICON_0_POS_INI     (D3DXVECTOR3(SCREEN_WIDTH - 150.0f,410.0f,0.0f))
#define UI_PLAYERICON_0_COL_INI     (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))

// �Q��
#define UI_PLAYERICON_1_WIDTH_INI   (16.0f)
#define UI_PLAYERICON_1_HEIGHT_INI  (16.0f)
#define UI_PLAYERICON_1_POS_INI     (D3DXVECTOR3(SCREEN_WIDTH - 70.0f ,410.0f,0.0f))
#define UI_PLAYERICON_1_COL_INI     (D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))

// �v���C���[�̎c�@���p
#define UI_PLAYERSTOCK_WIDTH_INI    (90.0f)
#define UI_PLAYERSTOCK_HEIGHT_INI   (SCREEN_HEIGHT / 2)
#define UI_PLAYERSTOCK_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH ,SCREEN_HEIGHT / 2,0.0f))
#define UI_PLAYERSTOCK_COL_INI      (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

// �X�e�[�W�A�C�R���p
#define UI_STAGEICON_WIDTH_INI      (25.0f)
#define UI_STAGEICON_HEIGHT_INI     (25.0f)
#define UI_STAGEICON_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH - 112.5f ,550.0f,0.0f))
#define UI_STAGEICON_COL_INI        (D3DXCOLOR(0.0f,1.0f,0.0f,1.0f))

// �X�e�[�W���\���p�|���S���p
#define UI_STAGENUMBER_WIDTH_INI     (20.0f)
#define UI_STAGENUMBER_HEIGHT_INI    (SCREEN_HEIGHT / 2)
#define UI_STAGENUMBER_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH ,SCREEN_HEIGHT / 2,0.0f))
#define UI_STAGENUMBER_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CUI::CUI()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
//    ��������
//=============================================================================
CUI *CUI::Create(CTextureManager *pTextureManager, int nNumEnemy, int nMapIdx)
{
	CUI *pUI = NULL;  // UI�N���X�^�̃|�C���^
	if (pUI == NULL)
	{// ����������ɂȂ��Ă���
		pUI = new CUI;
		if (pUI != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pUI->Init(pTextureManager, nNumEnemy, nMapIdx)))
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

	return pUI;
}


//=============================================================================
//    ����������
//=============================================================================
HRESULT CUI::Init(CTextureManager *pTextureManager, int nNumEnemy, int nMapIdx)
{
	// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^��ݒ�
	m_pTextureManager = pTextureManager;

	// �G�̐���ۑ�
	m_nNumEnemy = nNumEnemy;

	// �ϐ����N���A����
	ClearVariable();

	// �w�i���쐬����
	CreateBg();

	// �G�A�C�R�����쐬
	CreateEnemyIcon(nNumEnemy);

	// �v���C���[�̃A�C�R�����쐬
	CreatePlayerIcon();

	// �v���C���[�̎c�@���\���p�|���S�����쐬
	CreatePlayerStock();

	// �X�e�[�W�A�C�R�����쐬
	CreateStageIcon();

	// ���݂̃X�e�[�W���\���p�|���S�����쐬
	CreateStageNumber(nMapIdx);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CUI::Uninit(void)
{
	// �w�i�̔j��
	CreateBg();

	// �G�A�C�R���̔j��
	ReleaseEnemyIcon();

	// �v���C���[�̃A�C�R���̔j��
	ReleasePlayerIcon();

	// �v���C���[�̎c�@���\���p�|���S���̔j��
	ReleasePlayerStock();

	// �X�e�[�W�A�C�R���̔j��
	ReleaseStageIcon();

	// ���݂̃X�e�[�W���\���p�|���S���̔j��
	ReleaseStageNumber();
}

//=============================================================================
//    �G�̃A�C�R�����ēx�������鏈��
//=============================================================================
void CUI::ReCreateEnemyIcon(int nNumEnemy)
{

}

//=============================================================================
//    �G�̃A�C�R��������Ă�������
//=============================================================================
void CUI::CutEnemyIcon(int nIdx)
{

}

//=============================================================================
//    �v���C���[�̎c�@�����ēx�������鏈��
//=============================================================================
void CUI::ReCreatePlayerStock(int nStock)
{

}

//=============================================================================
//    �X�e�[�W���\���p�|���S�����ēx�������鏈��
//=============================================================================
void CUI::ReCreateStageNumber(int nMapIdx)
{

}

//=============================================================================
//    �ϐ����N���A����
//=============================================================================
void CUI::ClearVariable(void)
{
	m_pTextureManager = NULL;     // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^(�x�[�X���[�h�N���X����擾����)
	m_pBg = NULL;                 // �w�i�Ɏg�p����|���S���N���X�ւ̃|�C���^
	m_pEnemyIcon = NULL;          // �G�̃A�C�R���\���p�|���S���N���X�ւ̃|�C���^
	m_pStageIcon = NULL;          // �X�e�[�W�A�C�R���\���p�|���S��
	m_pStageNumber = NULL;        // ���݂̃X�e�[�W���\���p�|���S��
	m_nNumEnemy = 0;              // �G�̃A�C�R���̐�
	m_nStageNumberDigit = 0;      // ���݂̃X�e�[�W���̌���

	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		m_nCharaStockDigit[nCnt] = 0;    // �v���C���[�̎c�@���̌���
		m_pPlayerIcon[nCnt] = NULL;       // �v���C���[�A�C�R���\���p�|���S��
		m_pPlayerStock[nCnt] = NULL;      // �v���C���[�̎c�@���\���p�|���S��
	}
}

//=============================================================================
//    �w�i�𐶐�����
//=============================================================================
void CUI::CreateBg(void)
{
	m_pBg = CScene2D::Create(UI_BG_POS_INI, UI_BG_COL_INI, UI_BG_WIDTH_INI, UI_BG_HEIGHT_INI, UI_PRIORITY);
}

//=============================================================================
//    �G�̃A�C�R���𐶐�����
//=============================================================================
void CUI::CreateEnemyIcon(int nNumEnemy)
{

}

//=============================================================================
//    �v���C���[�A�C�R���𐶐�����
//=============================================================================
void CUI::CreatePlayerIcon(void)
{
	D3DXVECTOR3 PlayerIconPos[MAX_NUM_PLAYER] = { UI_PLAYERICON_0_POS_INI, UI_PLAYERICON_1_POS_INI };
	D3DXCOLOR PlayerIconCol[MAX_NUM_PLAYER] = { UI_PLAYERICON_0_COL_INI, UI_PLAYERICON_1_COL_INI };
	float fPlayerIconWidth[MAX_NUM_PLAYER] = { UI_PLAYERICON_0_WIDTH_INI, UI_PLAYERICON_1_WIDTH_INI };
	float fPlayerIconHeight[MAX_NUM_PLAYER] = { UI_PLAYERICON_0_HEIGHT_INI, UI_PLAYERICON_1_HEIGHT_INI };

	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		m_pPlayerIcon[nCnt] = CScene2D::Create(PlayerIconPos[nCnt], PlayerIconCol[nCnt],
			fPlayerIconWidth[nCnt], fPlayerIconHeight[nCnt], UI_PRIORITY);
	}
}

//=============================================================================
//    �v���C���[�̎c�@���\���p�|���S���𐶐�����
//=============================================================================
void CUI::CreatePlayerStock(void)
{

}

//=============================================================================
//    �X�e�[�W�A�C�R���𐶐�����
//=============================================================================
void CUI::CreateStageIcon(void)
{
	m_pStageIcon = CScene2D::Create(UI_STAGEICON_POS_INI, UI_STAGEICON_COL_INI,
		UI_STAGEICON_WIDTH_INI, UI_STAGEICON_HEIGHT_INI, UI_PRIORITY);
}

//=============================================================================
//    ���݂̃X�e�[�W���\���p�|���S���𐶐�����
//=============================================================================
void CUI::CreateStageNumber(int nMapIdx)
{

}

//=============================================================================
//    �w�i���J������
//=============================================================================
void CUI::ReleaseBg(void)
{
	if (m_pBg != NULL)
	{
		m_pBg->Uninit();
		m_pBg = NULL;
	}
}

//=============================================================================
//    �G�̃A�C�R�����J������
//=============================================================================
void CUI::ReleaseEnemyIcon(void)
{
	if (m_pEnemyIcon != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumEnemy; nCnt++)
		{
			if (m_pEnemyIcon[nCnt] != NULL)
			{
				m_pEnemyIcon[nCnt]->Uninit();
				m_pEnemyIcon[nCnt] = NULL;
			}
		}
		delete[] m_pEnemyIcon;
		m_pEnemyIcon = NULL;
	}
}

//=============================================================================
//    �v���C���[�A�C�R�����J������
//=============================================================================
void CUI::ReleasePlayerIcon(void)
{
	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (m_pPlayerIcon[nCnt] != NULL)
		{
			m_pPlayerIcon[nCnt]->Uninit();
			m_pPlayerIcon[nCnt] = NULL;
		}
	}
}

//=============================================================================
//    �v���C���[�̎c�@���\���p�|���S�����J������
//=============================================================================
void CUI::ReleasePlayerStock(void)
{
	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (m_pPlayerStock[nCnt] != NULL)
		{
			for (int nCntDigit = 0; nCntDigit < m_nCharaStockDigit[nCnt]; nCntDigit++)
			{
				if (m_pPlayerStock[nCnt][nCntDigit] != NULL)
				{
					m_pPlayerStock[nCnt][nCntDigit]->Uninit();
					m_pPlayerStock[nCnt][nCntDigit] = NULL;
				}
			}
			delete[] m_pPlayerStock[nCnt];
			m_pPlayerStock[nCnt] = NULL;
		}
	}
}

//=============================================================================
//    �X�e�[�W�A�C�R�����J������
//=============================================================================
void CUI::ReleaseStageIcon(void)
{
	if (m_pStageIcon != NULL)
	{
		m_pStageIcon->Uninit();
		m_pStageIcon = NULL;
	}
}

//=============================================================================
//    ���݂̃X�e�[�W���\���p�|���S�����J������
//=============================================================================
void CUI::ReleaseStageNumber(void)
{
	if (m_pStageNumber != NULL)
	{
		for (int nCnt = 0; nCnt < m_nStageNumberDigit; nCnt++)
		{
			if (m_pStageNumber[nCnt] != NULL)
			{
				m_pStageNumber[nCnt]->Uninit();
				m_pStageNumber[nCnt] = NULL;
			}
		}
		delete[] m_pStageNumber;
		m_pStageNumber = NULL;
	}
}