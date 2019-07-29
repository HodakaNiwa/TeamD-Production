//*****************************************************************************
//
//     UI�̏���[UI.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CTextureManager;
class CScene2D;
class CNumber;

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define UI_PRIORITY     (5)       // UI�̏����D�揇��
#define UI_BG_WIDTH_INI (90.0f)   // UI�̔w�i��

//*****************************************************************************
//    UI�N���X�̒�`
//*****************************************************************************
class CUI
{
public:    // �N�ł��A�N�Z�X�\
	CUI();
	~CUI();

	static CUI *Create(CTextureManager *pTextureManager, int nNumEnemy, int nMapIdx);

	HRESULT Init(CTextureManager *pTextureManager, int nNumEnemy, int nMapIdx);
	void Uninit(void);
	void ReCreateEnemyIcon(int nNumEnemy);
	void CutEnemyIcon(int nIdx);
	void ReCreatePlayerStock(int nStock);
	void ReCreateStageNumber(int nMapIdx);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void ClearVariable(void);

	void CreateBg(void);
	void CreateEnemyIcon(int nNumEnemy);
	void CreatePlayerIcon(void);
	void CreatePlayerStock(void);
	void CreateStageIcon(void);
	void CreateStageNumber(int nMapIdx);

	void ReleaseBg(void);
	void ReleaseEnemyIcon(void);
	void ReleasePlayerIcon(void);
	void ReleasePlayerStock(void);
	void ReleaseStageIcon(void);
	void ReleaseStageNumber(void);

	CTextureManager *m_pTextureManager;         // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^(�x�[�X���[�h�N���X����擾����)
	CScene2D *m_pBg;                            // �w�i�Ɏg�p����|���S���N���X�ւ̃|�C���^
	CScene2D **m_pEnemyIcon;                    // �G�̃A�C�R���\���p�|���S���N���X�ւ̃|�C���^
	CScene2D *m_pPlayerIcon[MAX_NUM_PLAYER];    // �v���C���[�A�C�R���\���p�|���S��
	CNumber **m_pPlayerStock[MAX_NUM_PLAYER];   // �v���C���[�̎c�@���\���p�|���S��
	CScene2D *m_pStageIcon;                     // �X�e�[�W�A�C�R���\���p�|���S��
	CNumber **m_pStageNumber;                   // ���݂̃X�e�[�W���\���p�|���S��
	int m_nNumEnemy;                            // �G�̃A�C�R���̐�
	int m_nCharaStockDigit[MAX_NUM_PLAYER];     // �v���C���[�̎c�@���̌���
	int m_nStageNumberDigit;                    // ���݂̃X�e�[�W���̌���
};

#endif