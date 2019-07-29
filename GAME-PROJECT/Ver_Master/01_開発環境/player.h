//=============================================================================
//
// �v���C���[�̏��� [player.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"
#include "character.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CCharacter
{
public:	//�N������A�N�Z�X�\
	//*************************************************************************
	// �\���̂̒�`
	//*************************************************************************
	typedef enum
	{
		MOTION_NEUTAL = 0,
		MOTION_MOVE,
		MOTION_MAX,
	}MOTION;

	typedef enum
	{
		STATE_NOMAL = 0,//�ʏ���
		STATE_STOP,		//��~���
		STATE_RESPAWN,	//���X�|�[�����
		STATE_DEATH,	//���S���
		STATE_MAX,
	}STATE;

	typedef enum
	{
		PLAYER_ABILITY_NOMAL = 0,		//�ʏ�
		PLAYER_ABILITY_SPEEDUP,			//�e�̃X�s�[�h�A�b�v
		PLAYER_ABILITY_DOUBLEBULLET,	//�e�Q�����Ă�
		PLAYER_ABILITY_ALLBLOCKDESTROY,	//�S�Ẵu���b�N������
		PLAYER_ABILITY_MAX,
	}PLAYER_ABILITY;

	//�����o�֐�
	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(CScene *pScene);

	void SetPlayer(CPlayer *pPlayer);
	void SetPlayerIdx(int nPlayerIdx);
	void SetState(STATE state);
	void SetMaxBullet(int nMaxBullet);
	void SetSplash(bool bSplash);

	CPlayer *GetPlayer(void);
	int GetPlayerIdx(void);
	STATE GetState(void);
	int GetMaxBullet(void);
	bool GetSplash(void);

	//�ÓI�����o�֐�
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPlayerIdx, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, int nPriority = 3);

private:	//�����������A�N�Z�X�\
	void ClearVariable(void);

	void InputMove_Game(void);
	void InputMove_Tutorial(void);
	void InputMove(void);
	bool InputMove_Keyboard(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot);
	bool InputMove_Controller(D3DXVECTOR3 *pMove, float *pDiffAngle, D3DXVECTOR3 rot);
	void Move(void);
	void Collision(void);
	void State(void);
	void SetDiffAngle(float fDiffAngle);
	void CreateBullet(void);
	void SwitchAbility(void);

	//�����o�ϐ�
	CPlayer						*m_pPlayer;					//�v���C���[
	int							m_nPlayerIdx;				//�v���C���[�̔ԍ�
	STATE						m_state;					//�v���C���[�̏��
	int							m_nCntState;				//�v���C���[�̏�ԃJ�E���^�[
	int							m_nMaxBullet;				//�ő�e��
	int							m_nCntAbility;				//�\�̓J�E���^�[
	bool						m_bAllBlockDestroy;			//�S�Ẵu���b�N�������邩�ǂ���
	bool						m_bSplash;					//����Ă��邩�ǂ���
	int							m_nCntSplash;				//����J�E���^�[
	MOTION						m_motion;
	PLAYER_ABILITY				m_ability;				//�v���C���[�\��
};
#endif
