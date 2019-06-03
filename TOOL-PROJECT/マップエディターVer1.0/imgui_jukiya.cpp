//=============================================================================
//
// ImGui�̏��� [imgui_jukiya.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "imgui_jukiya.h"
#include "imconfig.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"

#include "block.h"
#include "editor.h"
#include "input.h"
#include "map.h"
#include "modelcreate.h"
#include "manager.h"
#include "camera.h"
#include "cameraManager.h"
#include "light.h"
#include "lightManager.h"
#include "fade.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define IMGUI_SAVE_DISP (60)
#define IMGUI_LOAD_DISP (60)

//=============================================================================
// ImGui�̃R���X�g���N�^
//=============================================================================
CImGui_Jukiya::CImGui_Jukiya()
{

}

//=============================================================================
// ImGui�̃f�X�g���N�^
//=============================================================================
CImGui_Jukiya::~CImGui_Jukiya()
{
}

//=============================================================================
// ImGui�̏���������
//=============================================================================
void CImGui_Jukiya::Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	// ImGui�p�̃e�L�X�g�f�[�^�𐶐�����
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImFontConfig config;
	//config.MergeMode = true;
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:/Windows/Fonts/meiryo.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());

	// AddFontFromFileTTF�֐�
	// ��P���� : �t�H���g�f�[�^�̃t�@�C���p�X��
	// ��Q���� : �t�H���g�̃T�C�Y
	// ��R���� : NULL��OK
	// ��S���� : ���{��ɂ��������io.Fonts->GetGlyphRangesJapanese�Ŋ�{�I��OK
	io.Fonts->AddFontFromFileTTF("c:/Windows/Fonts/meiryo.ttc", 20.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// ImGui�𐶐�����
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);
	ImGui::StyleColorsClassic();

	// �e��l�̏��������s��
	ClearVariable();
}

//=============================================================================
// ImGui�̏I������
//=============================================================================
void CImGui_Jukiya::Uninit(void)
{
	// ImGui���I��������
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=============================================================================
// ImGui�̍X�V����
//=============================================================================
void CImGui_Jukiya::Update(void)
{
	// �V�����t���[����p�ӂ���
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// �f�o�b�O�ǂ��E�B���h�E����
	WindowDebug();

	// �E�B���h�E�����J�n
	ImGui::Begin(u8"�}�b�v�G�f�B�^�[ Ver1.0", false);

	// ���݂̃��[�h�ݒ�
	if (CFade::GetFade() == CFade::FADE_NONE)
	{// �t�F�[�h���g�p����Ă��Ȃ�
		int nMode = CManager::GetMode();
		int nModeOld = nMode;
		ImGui::RadioButton(u8"�G�f�B�^�[", &nMode, CManager::MODE_EDITOR); ImGui::SameLine();
		ImGui::RadioButton(u8"�f���v���C", &nMode, CManager::MODE_DEMOPLAY);
		if (nModeOld != nMode)
		{// ���[�h���X�V���ꂽ
			CFade::SetFade((CManager::MODE)nMode, CFade::FADE_OUT);
		}
	}
	else
	{// �t�F�[�h���g�p����Ă���
		ImGui::Text(u8"���[�h�ؑ֒�");
	}

	if (CManager::GetMode() == CManager::MODE_EDITOR)
	{// �G�f�B�^�[���[�h��������
		EditerDebug();
	}
	else if (CManager::GetMode() == CManager::MODE_DEMOPLAY)
	{// �f���v���C���[�h��������
		DemoplayDebug();
	}

	// �t���[�����I��������
	ImGui::EndFrame();
}

//=============================================================================
// ImGui�̕`�揈��
//=============================================================================
void CImGui_Jukiya::Draw(void)
{
	// Update�Ő�������ImGui��\������
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//=============================================================================
// ImGui�̃f�o�b�O���`�揈��
//=============================================================================
void CImGui_Jukiya::WindowDebug(void)
{
	//�J�����̎擾
	CCamera *pCamera = NULL;
	if (CManager::GetBaseMode()->GetCameraManager() != NULL)
	{
		if (CManager::GetMode() == CManager::MODE_EDITOR)
		{
			pCamera = CManager::GetBaseMode()->GetCameraManager()->GetCamera(CManager::GetEditor()->GetCameraMode());
		}
		else if (CManager::GetMode() == CManager::MODE_DEMOPLAY)
		{
			pCamera = CManager::GetBaseMode()->GetCameraManager()->GetCamera();
		}
	}

	// �}�E�X�̎擾
	CInputMouse *pMouse = CManager::GetMouse();

	// �E�B���h�E�����J�n
	ImGui::Begin(u8"�f�o�b�O���");

	// FPS�`��
	ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);

	// �J�����̃f�o�b�O�ݒ�
	ImGui::Text("");
	ImGui::CollapsingHeader(u8"�J�������");
	if (pCamera != NULL)
	{
		bool bHoverWindow = ImGui::IsMouseHoveringAnyWindow();
		if (bHoverWindow == true)
		{
			pCamera->SetChengeLength(false);
			bool bDrag = ImGui::IsMouseDragging();
			if (bDrag == true)
			{// ImGui�̃E�B���h�E�𓮂����Ă���
				pCamera->SetMouseMove(false);
			}
			else
			{// ImGui�̃E�B���h�E�𓮂����Ă��Ȃ�
				pCamera->SetMouseMove(true);
			}
		}
		else
		{
			pCamera->SetChengeLength(true);
		}

		// �J�����̎��_�̕\��
		float aCameraPosV[3] = { pCamera->GetPosV().x,pCamera->GetPosV().y,pCamera->GetPosV().z };
		ImGui::Text(u8"���_ %.2f %.2f %.2f", aCameraPosV[0], aCameraPosV[1], aCameraPosV[2]);

		// �J�����̒����_�̕\��
		float aCameraPosR[3] = { pCamera->GetPosR().x,pCamera->GetPosR().y,pCamera->GetPosR().z };
		ImGui::Text(u8"�����_ %.2f %.2f %.2f", aCameraPosR[0], aCameraPosR[1], aCameraPosR[2]);

		// �J�����̌���
		float aCameraRot[3] = { pCamera->GetRot().x,pCamera->GetRot().y,pCamera->GetRot().z };
		ImGui::Text(u8"���� %.3f %.3f %.3f", aCameraRot[0], aCameraRot[1], aCameraRot[2]);

		// ���_�����_�Ԃ̋���
		float aCameraLength = pCamera->GetLength();
		ImGui::Text(u8"���� %.1f", aCameraLength);
	}


	// �}�E�X�̃f�o�b�O�ݒ�
	ImGui::Text("");
	ImGui::CollapsingHeader(u8"�}�E�X���");
	if (pMouse != NULL)
	{
		// �}�E�X�̃f�o�b�O���\��
		int aMouseScreenPos[2] = { pMouse->GetPoint().x,pMouse->GetPoint().y };
		float aMousePosZn[3] = { pMouse->GetWorldPosZn().x,pMouse->GetWorldPosZn().y,pMouse->GetWorldPosZn().z };
		float aMousePosZf[3] = { pMouse->GetWorldPosZf().x,pMouse->GetWorldPosZf().y,pMouse->GetWorldPosZf().z };
		ImGui::Text(u8"�X�N���[�����W                        %d %d", aMouseScreenPos[0], aMouseScreenPos[1]);
		ImGui::Text(u8"���[���h���W(�J��������߂���) %.1f %.1f %.1f", aMousePosZn[0], aMousePosZn[1], aMousePosZn[2]);
		ImGui::Text(u8"���[���h���W(�J�������牓����) %.1f %.1f %.1f", aMousePosZf[0], aMousePosZf[1], aMousePosZf[2]);
	}

	// �E�B���h�E�����I��
	ImGui::End();

}

//=============================================================================
// ImGui�̃G�f�B�^�[�`�揈��
//=============================================================================
void CImGui_Jukiya::EditerDebug(void)
{
	// �G�f�B�^�[�̎擾
	CEditor *pEditor = CManager::GetEditor();
	if (pEditor != NULL)
	{
		// �I�u�W�F�N�g�𐶐����邩�j�����邩��ݒ肷��
		ImGui::Text("");
		ImGui::CollapsingHeader(u8"�G�f�B�^�[�ݒ�");

		//�}�b�v�̃|�C���^���擾
		CMap *pMap = pEditor->GetMap();

		// �������[�h���j�����[�h����\��
		if (ImGui::TreeNode(u8"���[�h"))
		{
			int nEditerMode = pEditor->GetMode();
			ImGui::RadioButton(u8"����", &nEditerMode, CEditor::MODE_CREATE); ImGui::SameLine();
			ImGui::RadioButton(u8"�j��", &nEditerMode, CEditor::MODE_DESTROY);
			pEditor->SetMode((CEditor::MODE)nEditerMode);
			ImGui::TreePop();
		}

		// �ҏW���[�h��ݒ�
		if (ImGui::TreeNode(u8"�ҏW���[�h"))
		{
			int nEditMode = pEditor->GetEditMode();
			ImGui::RadioButton(u8"�Q�[���t�B�[���h", &nEditMode, CEditor::EDITMODE_GAMEFIELD); ImGui::SameLine();
			ImGui::RadioButton(u8"���C�g", &nEditMode, CEditor::EDITMODE_LIGHT); ImGui::SameLine();
			ImGui::RadioButton(u8"�z�u��", &nEditMode, CEditor::EDITMODE_OBJECT);
			pEditor->SetEditMode((CEditor::EDITMODE)nEditMode);
			ImGui::TreePop();
		}
		ImGui::Text("");

		// �J�������\��
		CameraDebug(pEditor);

		// �}�b�v���\��
		MapDebug(pEditor);
	}

	// �E�B���h�E�����I��
	ImGui::End();

	// �ҏW���[�h���Ƃ̃E�B���h�E�𐶐�
	if (pEditor->GetEditMode() == CEditor::EDITMODE_GAMEFIELD)
	{// �Q�[���t�B�[���h���\��
		GameFieldDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_LIGHT)
	{// ���C�g���\��
		LightDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_OBJECT)
	{// �z�u�����\��
		ObjectDebug(pEditor);
	}
}

//=============================================================================
// ImGui�̃J�������`�揈��
//=============================================================================
void CImGui_Jukiya::CameraDebug(CEditor *pEditor)
{
	// �w�b�_�[�����J�n
	ImGui::CollapsingHeader(u8"�J�����ݒ�");

	//�J�����̎擾
	CCamera *pCamera = NULL;
	if (CManager::GetBaseMode()->GetCameraManager() != NULL)
	{
		pCamera = CManager::GetBaseMode()->GetCameraManager()->GetCamera(CManager::GetEditor()->GetCameraMode());
	}

	// �J�������[�h��ݒ�
	int nCameraMode = pEditor->GetCameraMode();
	if (ImGui::TreeNode(u8"�J�������[�h"))
	{
		ImGui::RadioButton(u8"�Q�[��", &nCameraMode, CEditor::CAMERAMODE_GAME); ImGui::SameLine();
		ImGui::RadioButton(u8"�G�f�B�^�[", &nCameraMode, CEditor::CAMERAMODE_EDIT);
		pEditor->SetCameraMode((CEditor::CAMERAMODE)nCameraMode);
		ImGui::TreePop();
	}

	if (pCamera != NULL)
	{
		// �J��������`�悷��
		if (nCameraMode == CEditor::CAMERAMODE_GAME)
		{// �Q�[���J������������
			GameCameraDebug(pCamera);
		}
		else if (nCameraMode == CEditor::CEditor::CAMERAMODE_EDIT)
		{// �G�f�B�b�g�J������������
			EditorCameraDebug(pCamera);
		}
	}
	ImGui::Text("");
}

//=============================================================================
// ImGui�̃Q�[���J�������`�揈��
//=============================================================================
void CImGui_Jukiya::GameCameraDebug(CCamera *pCamera)
{

}

//=============================================================================
// ImGui�̃G�f�B�b�g�J�������`�揈��
//=============================================================================
void CImGui_Jukiya::EditorCameraDebug(CCamera *pCamera)
{
	// �{�^���������ꂽ��J������������������
	if (ImGui::Button(u8"�J�������Z�b�g"))
	{
		pCamera->Reset();
	}
}

//=============================================================================
// ImGui�̕ۑ����ۏ����`�揈��
//=============================================================================
int CImGui_Jukiya::SaveDisp(int nCounter, bool bDisp)
{
	nCounter--;
	if (nCounter <= 0)
	{
		nCounter = 0;
	}
	else
	{
		ImGui::SameLine();
		if (bDisp == true)
		{
			ImGui::Text(u8"�ۑ����������܂���");
		}
		else
		{
			ImGui::Text(u8"�ۑ��Ɏ��s���܂���");
		}
	}

	return nCounter;
}

//=============================================================================
// ImGui�̓ǂݍ��ݐ��ۏ��`�揈��
//=============================================================================
int CImGui_Jukiya::LoadDisp(int nCounter, bool bDisp)
{
	nCounter--;
	if (nCounter <= 0)
	{
		nCounter = 0;
	}
	else
	{
		ImGui::SameLine();
		if (bDisp == true)
		{
			ImGui::Text(u8"�ǂݍ��݂��������܂���");
		}
		else
		{
			ImGui::Text(u8"�ǂݍ��݂Ɏ��s���܂���");
		}
	}

	return nCounter;
}

//=============================================================================
// ImGui�̃}�b�v���`�揈��
//=============================================================================
void CImGui_Jukiya::MapDebug(CEditor *pEditor)
{
	// �w�b�_�[����
	ImGui::CollapsingHeader(u8"�}�b�v���");

	// �ǂݍ��ރ}�b�v�̃t�@�C������ݒ�
	LoadMapDebug(pEditor, pEditor->GetMap());

	// �ǂݍ��ݐ��ۏ��\��
	m_nLoadMapCounter = LoadDisp(m_nLoadMapCounter, m_bLoadMapDisp);

	// �ۑ�����}�b�v�̃t�@�C������ݒ�
	SaveMapDebug(pEditor, pEditor->GetMap());

	// �ۑ����ۏ��\��
	m_nSaveMapCounter = SaveDisp(m_nSaveMapCounter, m_bSaveMapDisp);
}

//=============================================================================
// ImGui�̃}�b�v�ۑ����`�揈��
//=============================================================================
void CImGui_Jukiya::SaveMapDebug(CEditor *pEditor, CMap *pMap)
{
	char aSaveFileName[256];
	strcpy(aSaveFileName, pEditor->GetMapSaveFileName());

	// �ۑ�����t�@�C�����ݒ�
	ImGui::InputText(u8"�ۑ�����t�@�C����", aSaveFileName, IM_ARRAYSIZE(aSaveFileName));
	pEditor->SetMapSaveFileName(aSaveFileName);

	if (ImGui::Button(u8"�ۑ�����"))
	{// �{�^������
	    // �}�b�v���ۑ�
		pMap->SetGameFieldFileName(pEditor->GetGameFieldSaveFileName());
		pMap->SetModelListFileName(pEditor->GetModelListSaveFileName());
		pMap->SetTexListFileName(pEditor->GetTexListSaveFileName());
		pMap->SetLightFileName(pEditor->GetLightSaveFileName());
		pMap->SetObjectFileName(pEditor->GetObjectSaveFileName());
		if (FAILED(pMap->Save(aSaveFileName)))
		{
			m_bSaveMapDisp = false;
		}
		else
		{
			m_bSaveMapDisp = true;
		}
		m_nSaveMapCounter = IMGUI_SAVE_DISP;
	}
}

//=============================================================================
// ImGui�̃}�b�v�ǂݍ��ݏ��`�揈��
//=============================================================================
void CImGui_Jukiya::LoadMapDebug(CEditor *pEditor, CMap *pMap)
{
	char aLoadFileName[256];
	strcpy(aLoadFileName, pEditor->GetMapLoadFileName());

	// �ǂݍ��ރt�@�C������ݒ�
	ImGui::InputText(u8"�ǂݍ��ރt�@�C����", aLoadFileName, IM_ARRAYSIZE(aLoadFileName));
	pEditor->SetMapLoadFileName(aLoadFileName);

	if (ImGui::Button(u8"�ǂݍ���"))
	{// �{�^������
		strcpy(aLoadFileName, SAVEFILENAME_MAP);
		if (FAILED(pMap->ChangeMap(strcat(aLoadFileName, pEditor->GetMapLoadFileName()))))
		{
			m_bLoadMapDisp = false;
		}
		else
		{
			m_bLoadMapDisp = true;
		}
		m_nLoadMapCounter = IMGUI_LOAD_DISP;
	}
}

//=============================================================================
// ImGui�̃��C�g���`�揈��
//=============================================================================
void CImGui_Jukiya::LightDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// �}�b�v�̎擾
		CMap *pMap = pEditor->GetMap();

		// ���C�g�Ǌ��N���X���擾
		CLightManager *pLightManager = pMap->GetLightManager();

		// �E�B���h�E�����J�n
		ImGui::Begin(u8"���C�g���");

		// �ۑ����\��
		SaveLightDebug(pEditor, pEditor->GetMap());

		// �ۑ����ۏ��\��
		m_nSaveLightCounter = SaveDisp(m_nSaveLightCounter, m_bSaveLightDisp);

		// �ǂݍ��ݏ��\��
		LoadLightDebug(pEditor, pEditor->GetMap());

		// �ǂݍ��ݐ��ۏ��\��
		m_nLoadLightCounter = LoadDisp(m_nLoadLightCounter, m_bLoadLightDisp);
		ImGui::Text("");

		// ���C�g�̑������\��
		pMap->SetLightManager(LightIncreDecreDebug(pLightManager));
		ImGui::Text("");

		// ���C�g���\��
		pLightManager = pMap->GetLightManager();
		LightInfoDebug(pLightManager);

		// �}�b�v�̐ݒ�
		pEditor->SetMap(pMap);

		// �E�B���h�E�����I��
		ImGui::End();
	}
}

//=============================================================================
// ImGui�̃��C�g���`�揈��
//=============================================================================
void CImGui_Jukiya::LightInfoDebug(CLightManager *pLightManager)
{
	// ���C�g�Ǌ��N���X���擾
	if (pLightManager != NULL)
	{
		// �w�b�_�[�����J�n
		ImGui::CollapsingHeader(u8"���C�g��");

		// ���C�g�̐����擾
		int nNumLight = pLightManager->GetNumLight();

		// ���C�g����\��
		for (int nCntLight = 0; nCntLight < nNumLight; nCntLight++)
		{// ���ݐ�������Ă��郉�C�g�̐������J��Ԃ�
			pLightManager = LightInfo(pLightManager, nCntLight);
		}
	}
}

//=============================================================================
// ImGui�̃��C�g���\������
//=============================================================================
CLightManager *CImGui_Jukiya::LightInfo(CLightManager *pLightManager, int nCntLight)
{
	ImGui::Text(u8"���C�g%d   ", nCntLight);
	// ���C�g�����擾
	CLight *pLight = pLightManager->GetLight(nCntLight);

	if (pLight != NULL)
	{
		// ���C�g�̎�ނ��擾
		int nLightType = pLight->GetType();
		int nLightTypeDef = nLightType;
		ImGui::SameLine(); ImGui::RadioButton(u8"�f�B���N�V���i��", &nLightType, CLight::TYPE_DIRECTIONAL);
		ImGui::SameLine(); ImGui::RadioButton(u8"�|�C���g", &nLightType, CLight::TYPE_POINT);
		ImGui::SameLine(); ImGui::RadioButton(u8"�X�|�b�g", &nLightType, CLight::TYPE_SPOT);

		// ���C�g�̎�ނ��ς���Ă��Ȃ�������
		pLightManager = ChangeLight(pLightManager, nCntLight, pLight, nLightType, nLightTypeDef);
		pLight = pLightManager->GetLight(nCntLight);

		// ���݂̃��C�g�����擾
		if (nLightType == CLight::TYPE_DIRECTIONAL)
		{// �f�B���N�V���i�����C�g�Ȃ�
			DirectionalLightInfoDebug((CDirectionalLight*)pLight);
		}
		else if (nLightType == CLight::TYPE_POINT)
		{// �|�C���g���C�g�Ȃ�
			PointLightInfoDebug((CPointLight*)pLight);
		}
		else if (nLightType == CLight::TYPE_SPOT)
		{// �X�|�b�g���C�g�Ȃ�
			SpotLightInfoDebug((CSpotLight*)pLight);
		}
		pLightManager->SettingLight(pLight, nCntLight);
		ImGui::Separator();
	}

	return pLightManager;
}

//=============================================================================
// ImGui�̃��C�g��ޕύX����
//=============================================================================
CLightManager *CImGui_Jukiya::ChangeLight(CLightManager *pLightManager, int nCntLight, CLight *pLightOld, int nLightType, int nLightTypeDef)
{
	CLight *pLight = pLightOld;
	if (nLightType != nLightTypeDef)
	{// �ς���Ă���
		CLight *pLightDef = pLightOld;
		pLight = CLight::ChangeLightType((CLight::TYPE)nLightType);

		pLightManager->SettingLight(pLight, nCntLight);

		pLightDef->Uninit();
		delete pLightDef;
		pLightDef = NULL;
	}

	return pLightManager;
}

//=============================================================================
// ImGui�̃f�B���N�V���i�����C�g���`�揈��
//=============================================================================
void CImGui_Jukiya::DirectionalLightInfoDebug(CDirectionalLight *pDirLight)
{
	if (pDirLight != NULL)
	{
		// �e������擾
		float fLightDiffuse[4] = { pDirLight->GetLight().Diffuse.r,pDirLight->GetLight().Diffuse.g,pDirLight->GetLight().Diffuse.b ,pDirLight->GetLight().Diffuse.a };
		float fLightAmbient[4] = { pDirLight->GetLight().Ambient.r,pDirLight->GetLight().Ambient.g,pDirLight->GetLight().Ambient.b ,pDirLight->GetLight().Ambient.a };
		float fLightSpecular[4] = { pDirLight->GetLight().Specular.r,pDirLight->GetLight().Specular.g,pDirLight->GetLight().Specular.b ,pDirLight->GetLight().Specular.a };
		float fLightDir[3] = { pDirLight->GetLight().Direction.x,pDirLight->GetLight().Direction.y,pDirLight->GetLight().Direction.z };

		// �e�����ҏW
		ImGui::ColorEdit3(u8"�g�U��", &fLightDiffuse[0]);
		ImGui::ColorEdit3(u8"����", &fLightAmbient[0]);
		ImGui::ColorEdit3(u8"���ˌ�", &fLightSpecular[0]);
		ImGui::InputFloat3(u8"����", &fLightDir[0]);

		// �e�����ݒ�
		D3DLIGHT9 Light;
		ZeroMemory(&Light, sizeof(D3DLIGHT9));
		Light.Type = D3DLIGHT_DIRECTIONAL;
		Light.Diffuse = D3DXCOLOR(fLightDiffuse[0], fLightDiffuse[1], fLightDiffuse[2], fLightDiffuse[3]);
		Light.Ambient = D3DXCOLOR(fLightAmbient[0], fLightAmbient[1], fLightAmbient[2], fLightAmbient[3]);
		Light.Specular = D3DXCOLOR(fLightSpecular[0], fLightSpecular[1], fLightSpecular[2], fLightSpecular[3]);
		Light.Direction = D3DXVECTOR3(fLightDir[0], fLightDir[1], fLightDir[2]);
		pDirLight->SetLight(Light);
	}
}

//=============================================================================
// ImGui�̃|�C���g���C�g���`�揈��
//=============================================================================
void CImGui_Jukiya::PointLightInfoDebug(CPointLight *pPointLight)
{
	if (pPointLight != NULL)
	{
		// �e������擾
		float fLightPos[3] = { pPointLight->GetLight().Position.x,pPointLight->GetLight().Position.y,pPointLight->GetLight().Position.z };
		float fLightDiffuse[4] = { pPointLight->GetLight().Diffuse.r,pPointLight->GetLight().Diffuse.g,pPointLight->GetLight().Diffuse.b ,pPointLight->GetLight().Diffuse.a };
		float fLightAmbient[4] = { pPointLight->GetLight().Ambient.r,pPointLight->GetLight().Ambient.g,pPointLight->GetLight().Ambient.b ,pPointLight->GetLight().Ambient.a };
		float fLightSpecular[4] = { pPointLight->GetLight().Specular.r,pPointLight->GetLight().Specular.g,pPointLight->GetLight().Specular.b ,pPointLight->GetLight().Specular.a };
		float fLightAttenuation0 = pPointLight->GetLight().Attenuation0;
		float fLightAttenuation1 = pPointLight->GetLight().Attenuation1;
		float fLightAttenuation2 = pPointLight->GetLight().Attenuation2;
		float fLightRange = pPointLight->GetLight().Range;

		// �e�����ҏW
		ImGui::InputFloat3(u8"���W", &fLightPos[0]);
		ImGui::ColorEdit3(u8"�g�U��", &fLightDiffuse[0]);
		ImGui::ColorEdit3(u8"����", &fLightAmbient[0]);
		ImGui::ColorEdit3(u8"���ˌ�", &fLightSpecular[0]);
		ImGui::InputFloat(u8"���`�����W��", &fLightAttenuation0);
		ImGui::InputFloat(u8"�Q�������W��", &fLightAttenuation1);
		ImGui::InputFloat(u8"�w�������W��", &fLightAttenuation2);
		ImGui::InputFloat(u8"�͈�", &fLightRange);

		// �e�����ݒ�
		D3DLIGHT9 Light;
		ZeroMemory(&Light, sizeof(D3DLIGHT9));
		Light.Type = D3DLIGHT_POINT;
		Light.Position = D3DXVECTOR3(fLightPos[0], fLightPos[1], fLightPos[2]);
		Light.Diffuse = D3DXCOLOR(fLightDiffuse[0], fLightDiffuse[1], fLightDiffuse[2], fLightDiffuse[3]);
		Light.Ambient = D3DXCOLOR(fLightAmbient[0], fLightAmbient[1], fLightAmbient[2], fLightAmbient[3]);
		Light.Specular = D3DXCOLOR(fLightSpecular[0], fLightSpecular[1], fLightSpecular[2], fLightSpecular[3]);
		Light.Attenuation0 = fLightAttenuation0;
		Light.Attenuation1 = fLightAttenuation1;
		Light.Attenuation2 = fLightAttenuation2;
		Light.Range = fLightRange;
		pPointLight->SetLight(Light);
	}
}

//=============================================================================
// ImGui�̃X�|�b�g���C�g���`�揈��
//=============================================================================
void CImGui_Jukiya::SpotLightInfoDebug(CSpotLight *pSpotLight)
{
	if (pSpotLight != NULL)
	{	// �e������擾
		float fLightPos[3] = { pSpotLight->GetLight().Position.x,pSpotLight->GetLight().Position.y,pSpotLight->GetLight().Position.z };
		float fLightDir[3] = { pSpotLight->GetLight().Direction.x,pSpotLight->GetLight().Direction.y,pSpotLight->GetLight().Direction.z };
		float fLightDiffuse[4] = { pSpotLight->GetLight().Diffuse.r,pSpotLight->GetLight().Diffuse.g,pSpotLight->GetLight().Diffuse.b ,pSpotLight->GetLight().Diffuse.a };
		float fLightAmbient[4] = { pSpotLight->GetLight().Ambient.r,pSpotLight->GetLight().Ambient.g,pSpotLight->GetLight().Ambient.b ,pSpotLight->GetLight().Ambient.a };
		float fLightSpecular[4] = { pSpotLight->GetLight().Specular.r,pSpotLight->GetLight().Specular.g,pSpotLight->GetLight().Specular.b ,pSpotLight->GetLight().Specular.a };
		float fLightAttenuation0 = pSpotLight->GetLight().Attenuation0;
		float fLightAttenuation1 = pSpotLight->GetLight().Attenuation1;
		float fLightAttenuation2 = pSpotLight->GetLight().Attenuation2;
		float fLightRange = pSpotLight->GetLight().Range;
		float fLightFallOff = pSpotLight->GetLight().Falloff;
		float fLightTheta = pSpotLight->GetLight().Theta;
		float fLightPhi = pSpotLight->GetLight().Phi;

		// �e�����ҏW
		ImGui::InputFloat3(u8"���W", &fLightPos[0]);
		ImGui::InputFloat3(u8"����", &fLightDir[0]);
		ImGui::ColorEdit3(u8"�g�U��", &fLightDiffuse[0]);
		ImGui::ColorEdit3(u8"����", &fLightAmbient[0]);
		ImGui::ColorEdit3(u8"���ˌ�", &fLightSpecular[0]);
		ImGui::InputFloat(u8"���`�����W��", &fLightAttenuation0);
		ImGui::InputFloat(u8"�Q�������W��", &fLightAttenuation1);
		ImGui::InputFloat(u8"�w�������W��", &fLightAttenuation2);
		ImGui::InputFloat(u8"�͈�", &fLightRange);
		ImGui::InputFloat(u8"�t�H�[���I�t", &fLightFallOff);
		ImGui::InputFloat(u8"�����R�[���̊p�x", &fLightTheta);
		ImGui::InputFloat(u8"�O���R�[���̊p�x", &fLightPhi);

		// �e�����ݒ�
		D3DLIGHT9 Light;
		ZeroMemory(&Light, sizeof(D3DLIGHT9));
		Light.Type = D3DLIGHT_POINT;
		Light.Position = D3DXVECTOR3(fLightPos[0], fLightPos[1], fLightPos[2]);
		Light.Direction = D3DXVECTOR3(fLightDir[0], fLightDir[1], fLightDir[2]);
		Light.Diffuse = D3DXCOLOR(fLightDiffuse[0], fLightDiffuse[1], fLightDiffuse[2], fLightDiffuse[3]);
		Light.Ambient = D3DXCOLOR(fLightAmbient[0], fLightAmbient[1], fLightAmbient[2], fLightAmbient[3]);
		Light.Specular = D3DXCOLOR(fLightSpecular[0], fLightSpecular[1], fLightSpecular[2], fLightSpecular[3]);
		Light.Attenuation0 = fLightAttenuation0;
		Light.Attenuation1 = fLightAttenuation1;
		Light.Attenuation2 = fLightAttenuation2;
		Light.Range = fLightRange;
		Light.Falloff = fLightFallOff;
		Light.Theta = fLightTheta;
		Light.Phi = fLightPhi;
		pSpotLight->SetLight(Light);
	}
}

//=============================================================================
// ImGui�̃��C�g�ۑ����`�揈��
//=============================================================================
void CImGui_Jukiya::SaveLightDebug(CEditor *pEditor, CMap *pMap)
{
	char aSaveFileName[256];
	strcpy(aSaveFileName, pEditor->GetLightSaveFileName());

	// �ۑ�����t�@�C�����ݒ�
	ImGui::InputText(u8"�ۑ�����t�@�C����", aSaveFileName, IM_ARRAYSIZE(aSaveFileName));
	pEditor->SetLightSaveFileName(aSaveFileName);

	if (ImGui::Button(u8"�ۑ�����"))
	{// �{�^������
		if (FAILED(pMap->SaveLight(aSaveFileName)))
		{
			m_bSaveLightDisp = false;
		}
		else
		{
			m_bSaveLightDisp = true;
		}
		m_nSaveLightCounter = IMGUI_SAVE_DISP;
	}
}

//=============================================================================
// ImGui�̃��C�g�ۑ����`�揈��
//=============================================================================
void CImGui_Jukiya::LoadLightDebug(CEditor *pEditor, CMap *pMap)
{
	char aLoadFileName[256];
	strcpy(aLoadFileName, pEditor->GetLightLoadFileName());

	// �ǂݍ��ރt�@�C������ݒ�
	ImGui::InputText(u8"�ǂݍ��ރt�@�C����", aLoadFileName, IM_ARRAYSIZE(aLoadFileName));
	pEditor->SetLightLoadFileName(aLoadFileName);

	if (ImGui::Button(u8"�ǂݍ���"))
	{// �{�^������
		// ���C�g��j������
		pMap->DeleteLight();

		char aStr[256];
		strcpy(aLoadFileName, SAVEFILENAME_LIGHT);
		if (FAILED(pMap->LoadLight(strcat(aLoadFileName, pEditor->GetLightLoadFileName()), aStr)))
		{
			m_bLoadLightDisp = false;
		}
		else
		{
			m_bLoadLightDisp = true;
		}
		m_nLoadLightCounter = IMGUI_LOAD_DISP;
	}
}

//=============================================================================
// ImGui�̃��C�g�������`�揈��
//=============================================================================
CLightManager *CImGui_Jukiya::LightIncreDecreDebug(CLightManager *pLightManager)
{
	CLightManager *pLightManagerNew = pLightManager;
	if (pLightManager != NULL)
	{
		// ���C�g�̐����擾
		int nNumLight = pLightManager->GetNumLight();
		int nNumLightDef = nNumLight;

		// ���C�g�̐��𑝌�������
		ImGui::InputInt(u8"���C�g�̐�", &nNumLight);

		// ��������
		if (nNumLight != nNumLightDef && nNumLight > 0)
		{// �f�t�H���g�̒l����ύX���ꂽ
			// ���C�g�����R�s�[���ēn��
			pLightManagerNew = CLightManager::Cpy(pLightManager, nNumLight);
		}
	}

	return pLightManagerNew;
}

//=============================================================================
// ImGui�̃Q�[���t�B�[���h���`�揈��
//=============================================================================
void CImGui_Jukiya::GameFieldDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// �E�B���h�E�����J�n
		ImGui::Begin(u8"�Q�[���t�B�[���h���");

		// �ۑ����\��
		SaveGameFieldDebug(pEditor, pEditor->GetMap());

		// �ۑ����ۏ��\��
		m_nSaveGameFieldCounter = SaveDisp(m_nSaveGameFieldCounter, m_bSaveGameFieldDisp);

		// �ǂݍ��ݏ��\��
		LoadGameFieldDebug(pEditor, pEditor->GetMap());

		// �ǂݍ��ݐ��ۏ��\��
		m_nLoadGameFieldCounter = LoadDisp(m_nLoadGameFieldCounter, m_bLoadGameFieldDisp);
		ImGui::Text("");

		// �u���b�N���\��
		BlockDebug(pEditor);

		// �E�B���h�E�����I��
		ImGui::End();
	}
}

//=============================================================================
// ImGui�̃Q�[���t�B�[���h�ۑ����`�揈��
//=============================================================================
void CImGui_Jukiya::SaveGameFieldDebug(CEditor *pEditor, CMap *pMap)
{
	char aSaveFileName[256];
	strcpy(aSaveFileName, pEditor->GetGameFieldSaveFileName());

	// �ۑ�����t�@�C������ݒ�
	ImGui::InputText(u8"�ۑ�����t�@�C����", aSaveFileName, IM_ARRAYSIZE(aSaveFileName));
	pEditor->SetGameFieldSaveFileName(aSaveFileName);

	if (ImGui::Button(u8"�ۑ�����"))
	{// �{�^������
		if (FAILED(pMap->SaveGameField(aSaveFileName)))
		{
			m_bSaveGameFieldDisp = false;
		}
		else
		{
			m_bSaveGameFieldDisp = true;
		}
		m_nSaveGameFieldCounter = IMGUI_SAVE_DISP;
	}
}

//=============================================================================
// ImGui�̃Q�[���t�B�[���h�ǂݍ��ݏ��`�揈��
//=============================================================================
void CImGui_Jukiya::LoadGameFieldDebug(CEditor *pEditor, CMap *pMap)
{
	char aLoadFileName[256];
	strcpy(aLoadFileName, pEditor->GetGameFieldLoadFileName());

	// �ǂݍ��ރt�@�C������ݒ�
	ImGui::InputText(u8"�ǂݍ��ރt�@�C����", aLoadFileName, IM_ARRAYSIZE(aLoadFileName));
	pEditor->SetGameFieldLoadFileName(aLoadFileName);

	if (ImGui::Button(u8"�ǂݍ���"))
	{// �{�^������
		// �Q�[���t�B�[���h��j������
		pMap->DeleteGameField();

		char aStr[256];
		strcpy(aLoadFileName, SAVEFILENAME_GAMEFIELD);
		if (FAILED(pMap->LoadGameField(strcat(aLoadFileName, pEditor->GetGameFieldLoadFileName()), aStr)))
		{
			m_bLoadGameFieldDisp = false;
		}
		else
		{
			m_bLoadGameFieldDisp = true;
		}
		m_nLoadGameFieldCounter = IMGUI_LOAD_DISP;
	}
}

//=============================================================================
// ImGui�̃u���b�N���`�揈��
//=============================================================================
void CImGui_Jukiya::BlockDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// �}�b�v�N���X���擾
		CMap *pMap = pEditor->GetMap();

		// ���f���Ǌ��N���X���擾
		CModelCreate *pModelCreate = pMap->GetModelCreate();

		// �u���b�N���擾
		int nBlockType = pEditor->GetBlockType();
		int nBlockModelIdx = pEditor->GetBlockModelIdx();
		bool bBlockBreak = pEditor->GetBlockBreak();
		float fBlockColWidth = pEditor->GetBlockColWidth();
		float fBlockColHeight = pEditor->GetBlockColHeight();
		float fBlockColDepth = pEditor->GetBlockColDepth();
		int nBlockTypeDef = nBlockType;
		int nBlockModelIdxDef = nBlockModelIdx;
		float fBlockColWidthDef = fBlockColWidth;
		float fBlockColHeightDef = fBlockColHeight;
		float fBlockColDepthDef = fBlockColDepth;

		// �w�b�_�[�����J�n
		ImGui::CollapsingHeader(u8"�z�u����u���b�N�̒l�ݒ�");

		// �u���b�N����\��
		ImGui::InputInt(u8"���", &nBlockType);
		ImGui::InputInt(u8"�g�p���郂�f���̔ԍ�", &nBlockModelIdx);
		ImGui::Checkbox(u8"�󂹂邩�ǂ���", &bBlockBreak);
		ImGui::InputFloat(u8"�����蔻��̕�", &fBlockColWidth);
		ImGui::InputFloat(u8"�����蔻��̍���", &fBlockColHeight);
		ImGui::InputFloat(u8"�����蔻��̉��s", &fBlockColDepth);

		// �u���b�N��񔻒�
		if (nBlockType > CBlock::TYPE_MAX - 1 || nBlockType < 0)
		{// �u���b�N�̎�ނ̑����𒴂��ē��͂���Ă���
			nBlockType = nBlockTypeDef;
		}
		if (nBlockModelIdx > pModelCreate->GetNumModel() - 1 || nBlockModelIdx < 0)
		{// �ǂݍ��񂾃��f���̑����𒴂��ē��͂���Ă���
			nBlockModelIdx = nBlockModelIdxDef;
		}
		if (fBlockColWidth < 0.0f)
		{// �����蔻�蕝���0��������Ă���
			fBlockColWidth = fBlockColWidthDef;
		}
		if (fBlockColHeight < 0.0f)
		{// �����蔻�荂�����0��������Ă���
			fBlockColHeight = fBlockColHeightDef;
		}
		if (fBlockColDepth < 0.0f)
		{// �����蔻�艜�s���0��������Ă���
			fBlockColDepth = fBlockColDepthDef;
		}

		// �u���b�N���ݒ�
		pEditor->SetBlockType(nBlockType);
		pEditor->SetBlockModelIdx(nBlockModelIdx);
		pEditor->SetBlockBreak(bBlockBreak);
		pEditor->SetBlockColWidth(fBlockColWidth);
		pEditor->SetBlockColHeight(fBlockColHeight);
		pEditor->SetBlockColDepth(fBlockColDepth);
	}
}

//=============================================================================
// ImGui�̔z�u�����`�揈��
//=============================================================================
void CImGui_Jukiya::ObjectDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// �E�B���h�E�����J�n
		ImGui::Begin(u8"�z�u�����");

		// �ۑ����\��
		SaveObjectDebug(pEditor, pEditor->GetMap());

		// �ۑ����ۏ��\��
		m_nSaveObjectCounter = SaveDisp(m_nSaveObjectCounter, m_bSaveObjectDisp);

		// �ǂݍ��ݏ��\��
		LoadObjectDebug(pEditor, pEditor->GetMap());

		// �ǂݍ��ݐ��ۏ��\��
		m_nLoadObjectCounter = LoadDisp(m_nLoadObjectCounter, m_bLoadObjectDisp);
		ImGui::Text("");

		// �E�B���h�E�����I��
		ImGui::End();
	}
}

//=============================================================================
// ImGui�̔z�u���ۑ����`�揈��
//=============================================================================
void CImGui_Jukiya::SaveObjectDebug(CEditor *pEditor, CMap *pMap)
{
	char aSaveFileName[256];
	strcpy(aSaveFileName, pEditor->GetObjectSaveFileName());

	// �ۑ�����t�@�C������ݒ�
	ImGui::InputText(u8"�ۑ�����t�@�C����", aSaveFileName, IM_ARRAYSIZE(aSaveFileName));
	pEditor->SetObjectSaveFileName(aSaveFileName);

	if (ImGui::Button(u8"�ۑ�����"))
	{// �{�^������
		if (FAILED(pMap->SaveObject(aSaveFileName)))
		{
			m_bSaveObjectDisp = false;
		}
		else
		{
			m_bSaveObjectDisp = true;
		}
		m_nSaveObjectCounter = IMGUI_SAVE_DISP;
	}
}

//=============================================================================
// ImGui�̔z�u���ǂݍ��ݏ��`�揈��
//=============================================================================
void CImGui_Jukiya::LoadObjectDebug(CEditor *pEditor, CMap *pMap)
{
	char aLoadFileName[256];
	strcpy(aLoadFileName, pEditor->GetObjectLoadFileName());

	// �ǂݍ��ރt�@�C������ݒ�
	ImGui::InputText(u8"�ǂݍ��ރt�@�C����", aLoadFileName, IM_ARRAYSIZE(aLoadFileName));
	pEditor->SetObjectLoadFileName(aLoadFileName);

	if (ImGui::Button(u8"�ǂݍ���"))
	{// �{�^������
	    // �z�u����j������
		pMap->DeleteObject();

		char aStr[256];
		strcpy(aLoadFileName, SAVEFILENAME_OBJECT);
		if (FAILED(pMap->LoadGameField(strcat(aLoadFileName, pEditor->GetObjectLoadFileName()), aStr)))
		{
			m_bLoadObjectDisp = false;
		}
		else
		{
			m_bLoadObjectDisp = true;
		}
		m_nLoadObjectCounter = IMGUI_LOAD_DISP;
	}
}

//=============================================================================
// ImGui�̃f���v���C�`�揈��
//=============================================================================
void CImGui_Jukiya::DemoplayDebug(void)
{
	ImGui::Text("");
	ImGui::CollapsingHeader(u8"�f���v���C�ݒ�");

	// �E�B���h�E�����I��
	ImGui::End();
}

//=============================================================================
// ImGui�̕ϐ����N���A����
//=============================================================================
void CImGui_Jukiya::ClearVariable(void)
{
	// �e��ނ��Ƃ̕ϐ����N���A����
	ClearMapVariable();          // �}�b�v�p
	ClearModelListVariable();    // ���f�����X�g�p
	ClearTexListVariable();      // �e�N�X�`�����X�g�p
	ClearLightVariable();        // ���C�g�p
	ClearGameFieldVariable();    // �Q�[���t�B�[���h�p
	ClearObjectVariable();       // �z�u���p
}

//=============================================================================
// ImGui�̃}�b�v�p�ϐ����N���A����
//=============================================================================
void CImGui_Jukiya::ClearMapVariable(void)
{
	m_nSaveMapCounter = 0;
	m_bSaveMapDisp = false;
	m_nLoadMapCounter = 0;
	m_bLoadMapDisp = false;
}

//=============================================================================
// ImGui�̃��f�����X�g�p�ϐ����N���A����
//=============================================================================
void CImGui_Jukiya::ClearModelListVariable(void)
{
	m_nSaveModelListCounter = 0;
	m_bSaveModelListDisp = false;
	m_nLoadModelListCounter = 0;
	m_bLoadModelListDisp = false;
}

//=============================================================================
// ImGui�̃e�N�X�`�����X�g�p�ϐ����N���A����
//=============================================================================
void CImGui_Jukiya::ClearTexListVariable(void)
{
	m_nSaveTexListCounter = 0;
	m_bSaveTexListDisp = false;
	m_nLoadTexListCounter = 0;
	m_bLoadTexListDisp = false;
}

//=============================================================================
// ImGui�̃��C�g�p�ϐ����N���A����
//=============================================================================
void CImGui_Jukiya::ClearLightVariable(void)
{
	m_nSaveLightCounter = 0;
	m_bSaveLightDisp = false;
	m_nLoadLightCounter = 0;
	m_bLoadLightDisp = false;
}

//=============================================================================
// ImGui�̃Q�[���t�B�[���h�p�ϐ����N���A����
//=============================================================================
void CImGui_Jukiya::ClearGameFieldVariable(void)
{
	m_nSaveGameFieldCounter = 0;
	m_bSaveGameFieldDisp = false;
	m_nLoadGameFieldCounter = 0;
	m_bLoadGameFieldDisp = false;
}

//=============================================================================
// ImGui�̔z�u���p�ϐ����N���A����
//=============================================================================
void CImGui_Jukiya::ClearObjectVariable(void)
{
	m_nSaveObjectCounter = 0;
	m_bSaveObjectDisp = false;
	m_nLoadObjectCounter = 0;
	m_bLoadObjectDisp = false;
}