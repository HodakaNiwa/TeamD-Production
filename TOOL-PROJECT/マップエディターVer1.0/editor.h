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
class CMeshCollider;
class CGrid;
class C3DPolygon;
class CObject3D;
class CBlock;
class CRiver;
class CIceField;
class CObject;
class CBillboardObject;
class CEmitter;
class CSceneBillboard;
class CFileLoader;
class CFileSaver;
class CModelCreate;
class CUI;

//*****************************************************************************
// �x�[�X�I�u�W�F�N�g�N���X��`
//*****************************************************************************
class CBaseObject
{
public:    // �N�ł��A�N�Z�X�\
	CBaseObject();
	~CBaseObject();

	static CBaseObject *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void SwitchObject(int nObjMode);
	void SetValueToObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetValueToBillboardObj(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, bool bLighting, bool bDrawAddtive, int nTexIdx);
	void SetValueToEmitter(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	CObject *GetObj(void);
	CBillboardObject *GetBillboardObj(void);
	CEmitter *GetEmitter(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateObject(void);
	void CreateBillboardObj(void);
	void CreateEmitter(void);

	CObject *m_pObject;                 // �z�u���N���X�ւ̃|�C���^
	CBillboardObject *m_pBillboardObj;  // �r���{�[�h�I�u�W�F�N�g�N���X�ւ̃|�C���^
	CEmitter *m_pEmitter;               // �G�~�b�^�N���X�ւ̃|�C���^
	CSceneBillboard *m_pBill;           // �G�~�b�^�̍��W��\�����邽�߂̃r���{�[�h
};

//*****************************************************************************
// �G�f�B�^�[�N���X��`
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
		EDITMODE_ENEMYLIST,       // �G�̐������
		EDITMODE_SKY,             // ��
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
	void ExChangeArea(void);
	void SetRespawnModel(void);
	void SetMode(const MODE mode);
	void SetEditMode(const EDITMODE editMode);
	void SetCameraMode(const CAMERAMODE cameraMode);
	void SetGameFieldMode(const GAMEFIELDMODE gameFieldMode);
	void SetObjectMode(const OBJECTMODE objMode);
	void SetMeshField(CMeshField *pMeshField);
	void ShearchEditBlock(void);
	bool JudgAreaInObject(CObject3D *pObj3D, D3DXVECTOR3 pos, float fWidth, float fDepth);
	CBlock *ShearchBlockInArea(void);
	CBlock *ShearchBlockInArea_Loop(int nCntPriority, bool *pShearch, D3DXVECTOR3 ShearchPos, float fShearchWidth, float fShearchDepth);
	CBlock *ShearchBlock(CBlock *pBlock, bool *pShearch, D3DXVECTOR3 ShearchPos, float fShearchWidth, float fShearchDepth);
	void CreateBaseObject(void);
	void ReleaseBaseObject(void);
	void SwitchBaseObject(int nObjMode);
	void SetValueToBaseObject(void);
	void CreateObject(void);
	void CreateBillboardObj(void);
	void CreateEffect(void);

	MODE GetMode(void);
	EDITMODE GetEditMode(void);
	CAMERAMODE GetCameraMode(void);
	GAMEFIELDMODE GetGameFieldMode(void);
	OBJECTMODE GetObjectMode(void);

	// �G�f�B�^�[�p�֐�
	void SetGridDisp(const bool bDisp);
	void SetColRangeDisp(const bool bDisp);
	bool GetGridDisp(void);
	bool GetColRangeDisp(void);
	int GetMassX(void);
	int GetMassZ(void);
	int GetBoxSide(void);
	int GetBoxVertical(void);
	int GetStartLeftVerIndex(void);
	CBaseObject *GetBaseObject(void);

	// �u���b�N�p�֐�
	void SetEditBlock(CBlock *pBlock);
	void SetBlockType(const int nType);
	void SetBlockModelIdx(const int nModelIdx);
	void SetBlockRot(const D3DXVECTOR3 rot);
	void SetBlockColWidth(const float fWidth);
	void SetBlockColHeight(const float fHeight);
	void SetBlockColDepth(const float fDepth);
	CBlock *GetEditBlock(void);
	int GetBlockType(void);
	int GetBlockModelIdx(void);
	D3DXVECTOR3 GetBlockRot(void);
	float GetBlockColWidth(void);
	float GetBlockColHeight(void);
	float GetBlockColDepth(void);

	// ��p�֐�
	void SetRiverTexIdx(const int nTexIdx);
	int GetRiverTexIdx(void);

	// �X�p�֐�
	void SetIceTexIdx(const int nTexIdx);
	int GetIceTexIdx(void);

	// �z�u���f���p�֐�
	void SetObjModelType(const int nType);
	void SetObjModelPos(const D3DXVECTOR3 pos);
	void SetObjModelRot(const D3DXVECTOR3 rot);
	int GetObjModelType(void);
	D3DXVECTOR3 GetObjModelPos(void);
	D3DXVECTOR3 GetObjModelRot(void);

	// �z�u�r���{�[�h�p�֐�
	void SetObjBillTexIdx(const int nTexIdx);
	void SetObjBillPos(const D3DXVECTOR3 pos);
	void SetObjBillCol(const D3DXCOLOR col);
	void SetObjBillRot(const float fRot);
	void SetObjBillWidth(const float fWidth);
	void SetObjBillHeight(const float fHeight);
	void SetObjBillLighting(const bool bLighting);
	void SetObjBillDrawAddtive(const bool bDrawAddtive);
	int GetObjBillTexIdx(void);
	D3DXVECTOR3 GetObjBillPos(void);
	D3DXCOLOR GetObjBillCol(void);
	float GetObjBillRot(void);
	float GetObjBillWidth(void);
	float GetObjBillHeight(void);
	bool GetObjBillLighting(void);
	bool GetObjBillDrawAddtive(void);

	// �z�u�G�t�F�N�g�p�֐�
	void SetObjEffectType(const int nType);
	void SetObjEffectPos(const D3DXVECTOR3 pos);
	void SetObjEffectRot(const D3DXVECTOR3 rot);
	int GetObjEffectType(void);
	D3DXVECTOR3 GetObjEffectPos(void);
	D3DXVECTOR3 GetObjEffectRot(void);

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

	// �G�̐������p�֐�
	void SetEnemyListLoadFileName(char *pFileName);
	void SetEnemyListSaveFileName(char *pFileName);
	char *GetEnemyListLoadFileName(void);
	char *GetEnemyListSaveFileName(void);

	//�ÓI�����o�֐�
	static CEditor *Create(void);

private:	//�����������A�N�Z�X�\
	void ClearVariable(void);
	void ClearEditorVariable(void);
	void ClearMapVariable(void);
	void ClearBlockVariable(void);
	void ClearRiverVariable(void);
	void ClearIceVariable(void);
	void ClearObjModelVariable(void);
	void ClearObjBillVariable(void);
	void ClearObjEffectVariable(void);
	void ClearModelListVariable(void);
	void ClearTexListVariable(void);
	void ClearLightVariable(void);
	void ClearGameFieldVariable(void);
	void ClearObjectVariable(void);
	void ClearEnemyListVariable(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadPlayerRespawnModel(CModelCreate *pModelCreate, char *pStr);
	void LoadEnemyRespawnModel(CModelCreate *pModelCreate, char *pStr);
	void LoadEffectFileName(char *pStr);
	void LoadMapFileName(char *pStr);

	void CreateCamera(void);
	void CreateMap(void);
	void CreateUI(void);
	void CheckFieldPenetration(void);
	void BoxVisualization(int nMassX, int nMassZ);
	void PenetrationFieldArea(int nMassX, int nMassZ, int nLeftVerIndex);
	void AreaVisualization(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void CreateGameFieldObj(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void BlockCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void RiverCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void IceCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void DestroyGameFieldObj(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void BlockDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void RiverDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void IceDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	bool GameFieldObjDestroyCheck(CObject3D *pObj3D, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);
	bool GameFieldObjDestroyCheck_Half(CObject3D *pObj3D, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);
	void BlockDestroy(CBlock *pBlock, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);
	void RiverDestroy(CRiver *pRiver, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);
	void IceDestroy(CIceField *pIceField, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);
	void ReturnFieldHeight_FromRiver(CRiver *pRiver);
	void ReturnFieldHeight_FromIce(CIceField *pIceField);

	void ExChangeField(void);
	void ExChangeGrid(void);

	// �G�f�B�^�[�p�ϐ�
	char m_aPlayerRespawnFileName[256];   // �v���C���[�̃��X�|�[�����f���̃t�@�C����
	char m_aEnemyRespawnFileName[256];    // �G�̃��X�|�[�����f���̃t�@�C����
	CMeshField *m_pMeshField;             // ���b�V���t�B�[���h�N���X�ւ̃|�C���^
	CMeshCollider *m_pMeshCollider;       // ���b�V���R���C�_�[�N���X�ւ̃|�C���^
	CGrid *m_pGrid;                       // �O���b�h���N���X�ւ̃|�C���^
	C3DPolygon *m_pArea;                  // �G���A�����p�̃|���S���N���X�ւ̃|�C���^
	CUI *m_pUI;                           // UI�N���X�ւ̃|�C���^
	CBaseObject *m_pBaseObject;           // �x�[�X�I�u�W�F�N�g�N���X�ւ̃|�C���^
	bool m_bDispGrid;                     // �O���b�h����`�悷�邩�ǂ���
	bool m_bDispColRange;                 // �����蔻���`�悷�邩�ǂ���
	int m_nLeftVerIdx;                    // ���݃}�E�X�������Ă���G���A�̍��㒸�_�ԍ�
	int m_nMassX;                         // ���݃}�E�X�������Ă���G���A�ԍ�
	int m_nMassZ;                         // ���݃}�E�X�������Ă���G���A�ԍ�
	int m_nBoxSide;                       // �I�u�W�F�N�g��z�u����G���A�̉��̕�����
	int m_nBoxVertical;                   // �I�u�W�F�N�g��z�u����G���A�̉��s�̕�����
	int m_nStartLeftVerIndex;             // �I�u�W�F�N�g��z�u����G���A�̍��㒸�_�ԍ�
	MODE m_Mode;                          // �������邩�j�����邩
	EDITMODE m_EditMode;                  // ���ݕҏW�ł���I�u�W�F�N�g�̎��
	CAMERAMODE m_CameraMode;              // �J�����̃��[�h(�ǂ̃J�������g�������肷��)
	GAMEFIELDMODE m_GameFieldMode;        // �Q�[���t�B�[���h�ɔz�u����I�u�W�F�N�g�̎��
	OBJECTMODE m_ObjMode;                 // �z�u����I�u�W�F�N�g�̎��

	// �u���b�N�p�ϐ�
	CBlock *m_pEditBlock;                 // �ҏW����u���b�N�N���X�ւ̃|�C���^
	int m_nBlockType;                     // �z�u����u���b�N�̎�ޔԍ�
	int m_nBlockModelIdx;                 // �z�u����u���b�N�̎g�p���郂�f���̔ԍ�
	D3DXVECTOR3 m_BlockRot;               // �z�u����u���b�N�̌���
	float m_fBlockColWidth;               // �z�u����u���b�N�̓����蔻�蕝���
	float m_fBlockColHeight;              // �z�u����u���b�N�̓����蔻�荂�����
	float m_fBlockColDepth;               // �z�u����u���b�N�̓����蔻�艜�s���

	// ��p�ϐ�
	int m_nRiverTexIdx;                   // ��Ɏg�p����e�N�X�`���̔ԍ�

	// �X�p�ϐ�
	int m_nIceTexIdx;                     // �X�Ɏg�p����e�N�X�`���̔ԍ�

	// �z�u���f���p�ϐ�
	int m_nObjModelType;                  // �z�u���f���̎��
	D3DXVECTOR3 m_ObjModelPos;            // �z�u���f���̍��W
	D3DXVECTOR3 m_ObjModelRot;            // �z�u���f���̌���

	// �z�u�r���{�[�h�p�ϐ�
	int m_nObjBillTexIdx;                 // �z�u�r���{�[�h���g�p����e�N�X�`���̔ԍ�
	D3DXVECTOR3 m_ObjBillPos;             // �z�u�r���{�[�h�̍��W
	D3DXCOLOR m_ObjBillCol;               // �z�u�r���{�[�h�̐F
	float m_fObjBillRot;                  // �z�u�r���{�[�h�̃|���S���̌���
	float m_fObjBillWidth;                // �z�u�r���{�[�h�̕�
	float m_fObjBillHeight;               // �z�u�r���{�[�h�̍���
	bool m_bObjBillLighting;              // �z�u�r���{�[�h�̃��C�e�B���O�����邩�ǂ���
	bool m_bObjBillDrawAddtive;           // �z�u�r���{�[�h�̉��Z�����ŕ`�悷�邩���Ȃ���

	// �z�u�G�t�F�N�g�p�ϐ�
	int m_nObjEffectType;                 // �z�u�G�t�F�N�g�̎��
	D3DXVECTOR3 m_ObjEffectPos;           // �z�u�G�t�F�N�g�̍��W
	D3DXVECTOR3 m_ObjEffectRot;           // �z�u�G�t�F�N�g�̌���

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
	char m_aObjectLoadFileName[256];      // �z�u������ǂݍ��ރX�N���v�g�t�@�C����
	char m_aObjectSaveFileName[256];      // �z�u������ۑ�����X�N���v�g�t�@�C����

	// �G�̐������p�ϐ�
	char m_aEnemyListLoadFileName[256];   // �G�̐�������ǂݍ��ރX�N���v�g�t�@�C����
	char m_aEnemyListSaveFileName[256];   // �G�̐�������ۑ�����X�N���v�g�t�@�C����
};
#endif