//=============================================================================
//
// ImGui�̏��� [imgui_jukiya.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _IMGUI_JUKIYA_H_
#define _IMGUI_JUKIYA_H_

#include "main.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEditor;
class CCamera;
class CMap;
class CLightManager;
class CLight;
class CDirectionalLight;
class CPointLight;
class CSpotLight;
class CRespawn;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CImGui_Jukiya
{
public:	//�N������A�N�Z�X�\
		//�����o�֐�
	CImGui_Jukiya();
	~CImGui_Jukiya();
	void Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	//�����������A�N�Z�X�\
	void ClearVariable(void);
	void ClearMapVariable(void);
	void ClearModelListVariable(void);
	void ClearTexListVariable(void);
	void ClearLightVariable(void);
	void ClearGameFieldVariable(void);
	void ClearObjectVariable(void);
	void ClearRespawnVariable(void);

	CLightManager *ChangeLight(CLightManager *pLightManager, int nCntLight, CLight *pLightOld, int nLightType, int nLightTypeDef);

	void WindowDebug(void);
	void EditerDebug(void);
	void CameraDebug(CEditor *pEditor);
	void GameCameraDebug(CCamera *pCamera);
	void EditorCameraDebug(CCamera *pCamera);
	void TopViewCameraDebug(CCamera *pCamera);
	void FieldDebug(CEditor *pEditor);
	int SaveDisp(int nCounter, bool bDisp);
	int LoadDisp(int nCounter, bool bDisp);
	void MapDebug(CEditor *pEditor);
	void SaveMapDebug(CEditor *pEditor, CMap *pMap);
	void LoadMapDebug(CEditor *pEditor, CMap *pMap);

	void LightDebug(CEditor *pEditor);
	CLightManager *LightIncreDecreDebug(CLightManager *pLightManager);
	void LightInfoDebug(CLightManager *pLightManager);
	void LightInfo(CLightManager *pLightManager, int nCntLight);
	void DirectionalLightInfoDebug(CDirectionalLight *pDirLight);
	void PointLightInfoDebug(CPointLight *pPointLight);
	void SpotLightInfoDebug(CSpotLight *pSpotLight);

	void GameFieldDebug(CEditor *pEditor);
	void BlockDebug(CEditor *pEditor);
	void RiverDebug(CEditor *pEditor);
	void IceDebug(CEditor *pEditor);

	void ObjectDebug(CEditor *pEditor);
	void ObjModelDebug(CEditor *pEditor);
	void ObjBillboardDebug(CEditor *pEditor);
	void ObjEffectDebug(CEditor *pEditor);

	void RespawnDebug(CEditor *pEditor);
	void RespawnInfoDebug(CRespawn *pRespawn);

	void HeadQuartersDebug(CEditor *pEditor);

	void DemoplayDebug(void);


	//�}�b�v���p�ϐ�
	int m_nSaveMapCounter;                // �}�b�v���ۑ����ۏ��\���p�J�E���^�[
	bool m_bSaveMapDisp;                  // �}�b�v���ۑ����ۏ���\�����邩�ǂ���
	int m_nLoadMapCounter;                // �}�b�v���ǂݍ��ݐ��ۏ��\���p�J�E���^�[
	bool m_bLoadMapDisp;                  // �}�b�v���ǂݍ��ݐ��ۏ���\�����邩�ǂ���

	// ���f�����X�g���p�ϐ�
	int m_nSaveModelListCounter;          // ���f�����X�g���ۑ����ۏ��\���p�J�E���^�[
	bool m_bSaveModelListDisp;            // ���f�����X�g���ۑ����ۏ���\�����邩�ǂ���
	int m_nLoadModelListCounter;          // ���f�����X�g���ǂݍ��ݐ��ۏ��\���p�J�E���^�[
	bool m_bLoadModelListDisp;            // ���f�����X�g���ǂݍ��ݐ��ۏ���\�����邩�ǂ���

	// �e�N�X�`�����X�g���p�ϐ�
	int m_nSaveTexListCounter;            // �e�N�X�`�����X�g���ۑ����ۏ��\���p�J�E���^�[
	bool m_bSaveTexListDisp;              // �e�N�X�`�����X�g���ۑ����ۏ���\�����邩�ǂ���
	int m_nLoadTexListCounter;            // �e�N�X�`�����X�g���ǂݍ��ݐ��ۏ��\���p�J�E���^�[
	bool m_bLoadTexListDisp;              // �e�N�X�`�����X�g���ǂݍ��ݐ��ۏ���\�����邩�ǂ���

	// ���C�g���p�ϐ�
	int m_nLightIdx;                      // ���C�g�̏��𑀍삷��ԍ�
	int m_nSaveLightCounter;              // ���C�g���ۑ����ۏ��\���p�J�E���^�[
	bool m_bSaveLightDisp;                // ���C�g���ۑ����ۏ���\�����邩�ǂ���
	int m_nLoadLightCounter;              // ���C�g���ǂݍ��ݐ��ۏ��\���p�J�E���^�[
	bool m_bLoadLightDisp;                // ���C�g���ǂݍ��ݐ��ۏ���\�����邩�ǂ���

	// �Q�[���t�B�[���h�p�ϐ�
	int m_nSaveGameFieldCounter;          // �Q�[���t�B�[���h���ۑ����ۏ��\���p�J�E���^�[
	bool m_bSaveGameFieldDisp;            // �Q�[���t�B�[���h���ۑ����ۏ���\�����邩�ǂ���
	int m_nLoadGameFieldCounter;          // �Q�[���t�B�[���h���ǂݍ��ݐ��ۏ��\���p�J�E���^�[
	bool m_bLoadGameFieldDisp;            // �Q�[���t�B�[���h���ǂݍ��ݐ��ۏ���\�����邩�ǂ���

	// �z�u�����p�ϐ�
	int m_nSaveObjectCounter;             // �z�u�����ۑ����ۏ��\���p�J�E���^�[
	bool m_bSaveObjectDisp;               // �z�u�����ۑ����ۏ���\�����邩�ǂ���
	int m_nLoadObjectCounter;             // �z�u�����ǂݍ��ݐ��ۏ��\���p�J�E���^�[
	bool m_bLoadObjectDisp;               // �z�u�����ǂݍ��ݐ��ۏ���\�����邩�ǂ���

	// ���X�|�[���p�ϐ�
	int m_nPlayerResIdx;
	int m_nEnemyResIdx;
};
#endif