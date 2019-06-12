//=============================================================================
//
// �G�f�B�^�[�̏��� [editor.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "main.h"
#include "basemode.h"

//�O���錾
class CMeshField;
class CGrid;
class CBlock;
class CFileLoader;
class CFileSaver;
class CModelCreate;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEditor : public CBasemode
{
public:	//�N������A�N�Z�X�\
	//-------------------
	//  �������[�h
	//-------------------
	typedef enum
	{
		MODE_CREATE = 0,
		MODE_DESTROY,
		MODE_MAX
	}MODE;

	//-------------------
	//  �ҏW���[�h
	//-------------------
	typedef enum
	{
		EDITMODE_GAMEFIELD = 0,   // �Q�[���t�B�[���h
		EDITMODE_LIGHT,           // ���C�g
		EDITMODE_OBJECT,          // �z�u��
		EDITMODE_RESPAWN,         // ���X�|�[���ʒu
		EDITMODE_HEADQUARTERS,    // �i�ߕ�
		EDITMODE_MAX
	}EDITMODE;

	//-------------------
	//  �J�������[�h
	//-------------------
	typedef enum
	{
		CAMERAMODE_GAME = 0,   // �Q�[���̃J����(�Œ�J����)
		CAMERAMODE_EDIT,       // �G�f�B�^�[�̃J����(��������J����)
		CAMERAMODE_TOP,        // �g�b�v�r���[�̃J����(�Œ�J����)
		CAMERAMODE_MAX
	}CAMERAMODE;

	//-------------------------------
	//  �z�u����Q�[���t�B�[���h�̎��
	//-------------------------------
	typedef enum
	{
		GAMEFIELDMODE_BLOCK = 0, // �u���b�N
		GAMEFIELDMODE_RIVER,     // ��
		GAMEFIELDMODE_ICE,       // �X
		GAMEFIELDMODE_MAX
	}GAMEFIELDMODE;

	//-------------------------------
	//  �z�u����I�u�W�F�N�g�̎��
	//-------------------------------
	typedef enum
	{
		OBJECTMODE_MODEL = 0,   // ���f��
		OBJECTMODE_BILLBOARD,   // �r���{�[�h
		OBJECTMODE_EFFECT,      // �G�t�F�N�g
		OBJECTMODE_MAX
	}OBJECTMODE;

	//�����o�֐�
	CEditor();
	~CEditor();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetRespawnModel(void);
	void SetMode(const MODE mode);
	void SetEditMode(const EDITMODE editMode);
	void SetCameraMode(const CAMERAMODE cameraMode);
	void SetGameFieldMode(const GAMEFIELDMODE gameFieldMode);
	void SetObjectMode(const OBJECTMODE objMode);
	void SetMeshField(CMeshField *pMeshField);

	MODE GetMode(void);
	EDITMODE GetEditMode(void);
	CAMERAMODE GetCameraMode(void);
	GAMEFIELDMODE GetGameFieldMode(void);
	OBJECTMODE GetObjectMode(void);

	// �G�f�B�^�[�p�֐�
	int GetMasuX(void);
	int GetMasuZ(void);
	int GetBoxSide(void);
	int GetBoxVertical(void);
	int GetStartLeftVerIndex(void);

	// �u���b�N�p�֐�
	void SetBlockType(const int nType);
	void SetBlockModelIdx(const int nModelIdx);
	void SetBlockBreak(const bool bBreak);
	void SetBlockColWidth(const float fWidth);
	void SetBlockColHeight(const float fHeight);
	void SetBlockColDepth(const float fDepth);
	int GetBlockType(void);
	int GetBlockModelIdx(void);
	bool GetBlockBreak(void);
	float GetBlockColWidth(void);
	float GetBlockColHeight(void);
	float GetBlockColDepth(void);

	// �}�b�v���p�֐�
	void SetMapLoadFileName(char *pFileName);
	void SetMapSaveFileName(char *pFileName);
	char *GetMapLoadFileName(void);
	char *GetMapSaveFileName(void);

	// ���f�����X�g���p�֐�
	void SetModelListLoadFileName(char *pFileName);
	void SetModelListSaveFileName(char *pFileName);
	char *GetModelListLoadFileName(void);
	char *GetModelListSaveFileName(void);

	// �e�N�X�`�����X�g���p�֐�
	void SetTexListLoadFileName(char *pFileName);
	void SetTexListSaveFileName(char *pFileName);
	char *GetTexListLoadFileName(void);
	char *GetTexListSaveFileName(void);

	// ���C�g���p�֐�
	void SetLightLoadFileName(char *pFileName);
	void SetLightSaveFileName(char *pFileName);
	char *GetLightLoadFileName(void);
	char *GetLightSaveFileName(void);

	// �Q�[���t�B�[���h�p�֐�
	void SetGameFieldLoadFileName(char *pFileName);
	void SetGameFieldSaveFileName(char *pFileName);
	char *GetGameFieldLoadFileName(void);
	char *GetGameFieldSaveFileName(void);

	// �z�u�����p�֐�
	void SetObjectLoadFileName(char *pFileName);
	void SetObjectSaveFileName(char *pFileName);
	char *GetObjectLoadFileName(void);
	char *GetObjectSaveFileName(void);

	//�ÓI�����o�֐�
	static CEditor *Create(void);

private:	//�����������A�N�Z�X�\
	void ClearVariable(void);
	void ClearEditorVariable(void);
	void ClearMapVariable(void);
	void ClearBlockVariable(void);
	void ClearModelListVariable(void);
	void ClearTexListVariable(void);
	void ClearLightVariable(void);
	void ClearGameFieldVariable(void);
	void ClearObjectVariable(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadPlayerRespawnModel(CModelCreate *pModelCreate, char *pStr);
	void LoadEnemyRespawnModel(CModelCreate *pModelCreate, char *pStr);
	void LoadMapFileName(char *pStr);
	void SaveSystem(void);
	void SavePlayerRespawnModel(CFileSaver *pFileSaver);
	void SaveEnemyRespawnModel(CFileSaver *pFileSaver);
	void SaveMapFileName(CFileSaver *pFileSaver);

	void CreateCamera(void);
	void CreateMap(void);
	void CheckFieldPenetration(void);
	void PenetrationFieldArea(int nMasuX, int nMasuZ, int nLeftVerIndex);
	void FillinFieldArea(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void FillinFieldBox(int nLeftVerIndex);
	void BlockCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void BlockDestroy(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void BlockDestroyCheck(CBlock *pBlock, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);

	// �G�f�B�^�[�p�ϐ�
	char m_aPlayerRespawnFileName[256];   // �v���C���[�̃��X�|�[�����f���̃t�@�C����
	char m_aEnemyRespawnFileName[256];    // �G�̃��X�|�[�����f���̃t�@�C����
	CMeshField *m_pMeshField;             // ���b�V���t�B�[���h�N���X�ւ̃|�C���^
	CGrid *m_pGrid;                       // �O���b�h���N���X�ւ̃|�C���^
	int m_nMasuX;                         // ���݃}�E�X�������Ă���G���A�ԍ�
	int m_nMasuZ;                         // ���݃}�E�X�������Ă���G���A�ԍ�
	int m_nBoxSide;                       // �I�u�W�F�N�g��z�u����G���A�̉��̕�����
	int m_nBoxVertical;                   // �I�u�W�F�N�g��z�u����G���A�̉��s�̕�����
	int m_nStartLeftVerIndex;             // ���݃}�E�X�������Ă���G���A�̍��㒸�_�ԍ�
	MODE m_Mode;                          // �������邩�j�����邩
	EDITMODE m_EditMode;                  // ���ݕҏW�ł���I�u�W�F�N�g�̎��
	CAMERAMODE m_CameraMode;              // �J�����̃��[�h(�ǂ̃J�������g�������肷��)
	GAMEFIELDMODE m_GameFieldMode;        // �Q�[���t�B�[���h�ɔz�u����I�u�W�F�N�g�̎��
	OBJECTMODE m_ObjMode;                 // �z�u����I�u�W�F�N�g�̎��

	// �u���b�N�p�ϐ�
	int m_nBlockType;                     // �z�u����u���b�N�̎�ޔԍ�
	int m_nBlockModelIdx;                 // �z�u����u���b�N�̎g�p���郂�f���̔ԍ�
	bool m_bBlockBreak;                   // �z�u����u���b�N���󂹂邩�ǂ���
	float m_fBlockColWidth;               // �z�u����u���b�N�̓����蔻�蕝���
	float m_fBlockColHeight;              // �z�u����u���b�N�̓����蔻�荂�����
	float m_fBlockColDepth;               // �z�u����u���b�N�̓����蔻�艜�s���

	// �}�b�v���p�ϐ�
	char m_aMapFileName[256];             // �ǂݍ��ރ}�b�v�̃t�@�C����
	char m_aMapLoadFileName[256];         // �}�b�v����ǂݍ��ރX�N���v�g�t�@�C����
	char m_aMapSaveFileName[256];         // �}�b�v����ۑ�����X�N���v�g�t�@�C����

	// ���f�����X�g���p�ϐ�
	char m_aModelListLoadFileName[256];   // ���f�����X�g����ǂݍ��ރX�N���v�g�t�@�C����
	char m_aModelListSaveFileName[256];   // ���f�����X�g����ۑ�����X�N���v�g�t�@�C����

	// �e�N�X�`�����X�g���p�ϐ�
	char m_aTexListLoadFileName[256];     // �e�N�X�`�����X�g����ǂݍ��ރX�N���v�g�t�@�C����
	char m_aTexListSaveFileName[256];     // �e�N�X�`�����X�g����ۑ�����X�N���v�g�t�@�C����

	// ���C�g���p�ϐ�
	char m_aLightLoadFileName[256];       // ���C�g����ǂݍ��ރX�N���v�g�t�@�C����
	char m_aLightSaveFileName[256];       // ���C�g����ۑ�����X�N���v�g�t�@�C����

	// �Q�[���t�B�[���h�p�ϐ�
	char m_aGameFieldLoadFileName[256];   // �Q�[���t�B�[���h����ǂݍ��ރX�N���v�g�t�@�C����
	char m_aGameFieldSaveFileName[256];   // �Q�[���t�B�[���h����ۑ�����X�N���v�g�t�@�C����

	// �z�u�����p�ϐ�
	int m_nObjModelIdx;                   // �z�u����I�u�W�F�N�g�̎g�p���郂�f���ԍ�
	int m_nObjTexIdx;                     // �z�u����I�u�W�F�N�g�̎g�p����e�N�X�`���ԍ�
	D3DXVECTOR3 m_ObjPos;                 // �z�u����I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3 m_ObjRot;                 // �z�u����I�u�W�F�N�g�̌���
	char m_aObjectLoadFileName[256];      // �z�u������ǂݍ��ރX�N���v�g�t�@�C����
	char m_aObjectSaveFileName[256];      // �z�u������ۑ�����X�N���v�g�t�@�C����
};
#endif