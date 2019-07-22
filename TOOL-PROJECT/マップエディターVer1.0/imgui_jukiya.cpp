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
#include "boxCollider.h"
#include "editor.h"
#include "input.h"
#include "map.h"
#include "modelcreate.h"
#include "manager.h"
#include "camera.h"
#include "cameraManager.h"
#include "light.h"
#include "lightManager.h"
#include "textureManager.h"
#include "fade.h"
#include "meshfield.h"
#include "headquarters.h"
#include "respawn.h"
#include "fileLoader.h"
#include "sky.h"
#include "effectManager.h"
#include "emitter.h"
#include "billboardObject.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define IMGUI_SAVE_DISP   (60)
#define IMGUI_LOAD_DISP   (60)
#define IMGUI_CREATE_DISP (60)

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
			ChangeModeDebug(nMode);
		}

		// �ǂݍ��ރ}�b�v�̃t�@�C������ݒ�
		LoadFileNameDebug();
		ImGui::Text("");

		// �ǂݍ��ݐ��ۏ��\��
		m_nLoadModeCounter = LoadDisp(m_nLoadModeCounter, m_bLoadModeDisp);
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
// ImGui�̓ǂݍ��ރ}�b�v�̃t�@�C�������`�揈��
//=============================================================================
void CImGui_Jukiya::LoadFileNameDebug(void)
{
	char m_aFileName[256] = "\0";
	strcpy(m_aFileName, CManager::GetLoadMapFileName());

	if (CManager::GetMode() == CManager::MODE_EDITOR)
	{
		ImGui::InputText(u8"�f���v���C���ǂݍ��ރ}�b�v", m_aFileName, IM_ARRAYSIZE(m_aFileName));
	}
	else if (CManager::GetMode() == CManager::MODE_DEMOPLAY)
	{
		ImGui::InputText(u8"�G�f�B�^�[���ǂݍ��ރ}�b�v", m_aFileName, IM_ARRAYSIZE(m_aFileName));
	}

	CManager::SetLoadMapFileName(m_aFileName);
}

//=============================================================================
// ImGui�̃��[�h�ؑ֏��`�揈��
//=============================================================================
void CImGui_Jukiya::ChangeModeDebug(int nMode)
{
	char m_aFileName[256] = "\0";
	strcpy(m_aFileName, CManager::GetLoadMapFileName());

	// �J�E���^�[�Z�b�g
	m_nLoadModeCounter = IMGUI_LOAD_DISP;
	m_bLoadModeDisp = true;

	// �}�b�v��ǂݍ��߂邩�`�F�b�N
	CFileLoader *pFileLoader = CFileLoader::Create(m_aFileName);
	if (pFileLoader == NULL)
	{
		m_bLoadModeDisp = false;
		return;
	}

	// �ǂݍ��߂�Ȃ�΃������J�����ă��[�h�ؑ�
	if (pFileLoader != NULL)
	{
		delete pFileLoader;
		pFileLoader = NULL;
	}
	CFade::SetFade((CManager::MODE)nMode, CFade::FADE_OUT);
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
			int nEditModeDef = nEditMode;
			ImGui::RadioButton(u8"�Q�[���t�B�[���h", &nEditMode, CEditor::EDITMODE_GAMEFIELD); ImGui::SameLine();
			ImGui::RadioButton(u8"���C�g", &nEditMode, CEditor::EDITMODE_LIGHT); ImGui::SameLine();
			ImGui::RadioButton(u8"�z�u��", &nEditMode, CEditor::EDITMODE_OBJECT);
			ImGui::RadioButton(u8"���X�|�[��", &nEditMode, CEditor::EDITMODE_RESPAWN); ImGui::SameLine();
			ImGui::RadioButton(u8"�i�ߕ�", &nEditMode, CEditor::EDITMODE_HEADQUARTERS); ImGui::SameLine();
			ImGui::RadioButton(u8"�G�̐������", &nEditMode, CEditor::EDITMODE_ENEMYLIST);
			ImGui::RadioButton(u8"��", &nEditMode, CEditor::EDITMODE_SKY);
			pEditor->SetEditMode((CEditor::EDITMODE)nEditMode);
			ImGui::TreePop();

			// �x�[�X�I�u�W�F�N�g��j�����邩�ǂ����`�F�b�N
			if (nEditModeDef != CEditor::EDITMODE_OBJECT && nEditMode == CEditor::EDITMODE_OBJECT)
			{
				pEditor->CreateBaseObject();
				pEditor->SwitchBaseObject(pEditor->GetObjectMode());
			}
			else if(nEditMode != CEditor::EDITMODE_OBJECT)
			{
				pEditor->ReleaseBaseObject();
			}
		}
		ImGui::Text("");
		ImGui::Text("");

		// �J�������\��
		CameraDebug(pEditor);

		// �n�ʏ��\��
		FieldDebug(pEditor);

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
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_RESPAWN)
	{// ���X�|�[���ʒu���\��
		RespawnDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_HEADQUARTERS)
	{// �i�ߕ����\��
		HeadQuartersDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_ENEMYLIST)
	{// �G�̐������\��
		EnemyListDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_SKY)
	{// ����\��
		SkyDebug(pEditor);
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
		ImGui::RadioButton(u8"�G�f�B�^�[", &nCameraMode, CEditor::CAMERAMODE_EDIT); ImGui::SameLine();
		ImGui::RadioButton(u8"�g�b�v�r���[", &nCameraMode, CEditor::CAMERAMODE_TOP);
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
		else if (nCameraMode == CEditor::CEditor::CAMERAMODE_TOP)
		{// �G�f�B�b�g�J������������
			TopViewCameraDebug(pCamera);
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
// ImGui�̃g�b�v�r���[�J�������`�揈��
//=============================================================================
void CImGui_Jukiya::TopViewCameraDebug(CCamera *pCamera)
{

}

//=============================================================================
// ImGui�̒n�ʏ��`�揈��
//=============================================================================
void CImGui_Jukiya::FieldDebug(CEditor *pEditor)
{
	// �w�b�_�[�����J�n
	ImGui::CollapsingHeader(u8"�t�B�[���h���");

	// �n�ʂɒ���t����e�N�X�`���ԍ���ݒ�
	int nNumTex = pEditor->GetMap()->GetTextureManager()->GetNumTexture();
	int nFieldTexIdx = pEditor->GetMap()->GetFieldTexIdx();
	ImGui::SliderInt(u8"�e�N�X�`���̔ԍ�", &nFieldTexIdx, 0, nNumTex - 1);
	pEditor->GetMap()->SetFieldTexIdx(nFieldTexIdx);

	// �n�ʂɃe�N�X�`����ݒ�
	CMeshField *pMeshField = pEditor->GetMap()->GetMeshField();
	pMeshField->BindTexture(pEditor->GetMap()->GetTextureManager()->GetTexture(nFieldTexIdx));

	// �O���b�h����`�悷�邩�ǂ����ݒ�
	bool bGridDisp = pEditor->GetGridDisp();
	ImGui::Checkbox(u8"�O���b�h���`��", &bGridDisp);
	pEditor->SetGridDisp(bGridDisp);

	// �����蔻��̐���`�悷�邩�ǂ����ݒ�
	bool bColRangeDisp = pEditor->GetColRangeDisp();
	ImGui::Checkbox(u8"�����蔻��̐��`��", &bColRangeDisp);
	pEditor->SetColRangeDisp(bColRangeDisp);

	// �ǐ��̂��߉��s
	ImGui::Text("");
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
	ImGui::Text("");

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
	char aSaveModelFileName[256];
	char aSaveTexFileName[256];
	char aSaveLightFileName[256];
	char aSaveGameFieldFileName[256];
	char aSaveObjectFileName[256];
	char aSaveEnemyFileName[256];
	strcpy(aSaveFileName, pEditor->GetMapSaveFileName());
	strcpy(aSaveModelFileName, pEditor->GetModelListSaveFileName());
	strcpy(aSaveTexFileName, pEditor->GetTexListSaveFileName());
	strcpy(aSaveLightFileName, pEditor->GetLightSaveFileName());
	strcpy(aSaveGameFieldFileName, pEditor->GetGameFieldSaveFileName());
	strcpy(aSaveObjectFileName, pEditor->GetObjectSaveFileName());
	strcpy(aSaveEnemyFileName, pEditor->GetEnemyListSaveFileName());

	// �ۑ�����t�@�C�����ݒ�
	ImGui::InputText(u8"�ۑ�����t�@�C����", aSaveFileName, IM_ARRAYSIZE(aSaveFileName));
	ImGui::InputText(u8"���f�����ۑ���", aSaveModelFileName, IM_ARRAYSIZE(aSaveModelFileName));
	ImGui::InputText(u8"�e�N�X�`�����X�g���ۑ���", aSaveTexFileName, IM_ARRAYSIZE(aSaveTexFileName));
	ImGui::InputText(u8"���C�g���ۑ���", aSaveLightFileName, IM_ARRAYSIZE(aSaveLightFileName));
	ImGui::InputText(u8"�Q�[���t�B�[���h���ۑ���", aSaveGameFieldFileName, IM_ARRAYSIZE(aSaveGameFieldFileName));
	ImGui::InputText(u8"�z�u�����ۑ���", aSaveObjectFileName, IM_ARRAYSIZE(aSaveObjectFileName));
	ImGui::InputText(u8"�G�̐������ۑ���", aSaveEnemyFileName, IM_ARRAYSIZE(aSaveEnemyFileName));
	pEditor->SetMapSaveFileName(aSaveFileName);
	pEditor->SetModelListSaveFileName(aSaveModelFileName);
	pEditor->SetTexListSaveFileName(aSaveTexFileName);
	pEditor->SetLightSaveFileName(aSaveLightFileName);
	pEditor->SetGameFieldSaveFileName(aSaveGameFieldFileName);
	pEditor->SetObjectSaveFileName(aSaveObjectFileName);
	pEditor->SetEnemyListSaveFileName(aSaveEnemyFileName);

	if (ImGui::Button(u8"�ۑ�����"))
	{// �{�^������
	    // �}�b�v���ۑ�
		pMap->SetModelListFileName(pEditor->GetModelListSaveFileName());
		pMap->SetTexListFileName(pEditor->GetTexListSaveFileName());
		pMap->SetLightFileName(pEditor->GetLightSaveFileName());
		pMap->SetGameFieldFileName(pEditor->GetGameFieldSaveFileName());
		pMap->SetObjectFileName(pEditor->GetObjectSaveFileName());
		pMap->SetEnemyListFileName(pEditor->GetEnemyListSaveFileName());
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
			pEditor->SetRespawnModel();
			pEditor->SetMeshField(pMap->GetMeshField());
			pEditor->SetEditBlock(NULL);
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
			m_nLightIdx = 0;
		}
	}

	return pLightManagerNew;
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
		if (m_nLightIdx > nNumLight)
		{
			m_nLightIdx = 0;
		}

		// ���C�g����\��
		ImGui::SliderInt(u8"�\������ԍ�", &m_nLightIdx, 0, nNumLight - 1);
		LightInfo(pLightManager, m_nLightIdx);
	}
}

//=============================================================================
// ImGui�̃��C�g���\������
//=============================================================================
void CImGui_Jukiya::LightInfo(CLightManager *pLightManager, int nCntLight)
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
// ImGui�̃Q�[���t�B�[���h���`�揈��
//=============================================================================
void CImGui_Jukiya::GameFieldDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// �E�B���h�E�����J�n
		ImGui::Begin(u8"�Q�[���t�B�[���h���");

		// �z�u����I�u�W�F�N�g�̎�ނ�ݒ�
		int nGameFieldMode = pEditor->GetGameFieldMode();
		int nGameFieldModeDef = nGameFieldMode;
		ImGui::RadioButton(u8"�u���b�N", &nGameFieldMode, CEditor::GAMEFIELDMODE_BLOCK); ImGui::SameLine();
		ImGui::RadioButton(u8"��", &nGameFieldMode, CEditor::GAMEFIELDMODE_RIVER); ImGui::SameLine();
		ImGui::RadioButton(u8"�X", &nGameFieldMode, CEditor::GAMEFIELDMODE_ICE);
		pEditor->SetGameFieldMode((CEditor::GAMEFIELDMODE)nGameFieldMode);
		ImGui::Text("");
		if (nGameFieldMode != nGameFieldModeDef)
		{
			pEditor->ExChangeArea();
		}

		// ���݂̃��[�h�ɂ���ď����킯
		if (nGameFieldMode == CEditor::GAMEFIELDMODE_BLOCK)
		{// �u���b�N���\��
			BlockDebug(pEditor);
		}
		else if (nGameFieldMode == CEditor::GAMEFIELDMODE_RIVER)
		{// ����\��
			RiverDebug(pEditor);
		}
		else if (nGameFieldMode == CEditor::GAMEFIELDMODE_ICE)
		{// �X���\��
			IceDebug(pEditor);
		}

		// �E�B���h�E�����I��
		ImGui::End();
	}
}

//=============================================================================
// ImGui�̃u���b�N���`�揈��
//=============================================================================
void CImGui_Jukiya::BlockDebug(CEditor *pEditor)
{
	// �}�b�v�N���X���擾
	CMap *pMap = pEditor->GetMap();

	// ���f���Ǌ��N���X���擾
	CModelCreate *pModelCreate = pMap->GetModelCreate();

	// �u���b�N���擾
	int nBlockType = pEditor->GetBlockType();
	int nBlockModelIdx = pEditor->GetBlockModelIdx();
	float fBlockRot[3] = { pEditor->GetBlockRot().x,pEditor->GetBlockRot().y ,pEditor->GetBlockRot().z };
	float fBlockColWidth = pEditor->GetBlockColWidth();
	float fBlockColHeight = pEditor->GetBlockColHeight();
	float fBlockColDepth = pEditor->GetBlockColDepth();
	int nBlockTypeDef = nBlockType;
	int nBlockModelIdxDef = nBlockModelIdx;
	float fBlockColWidthDef = fBlockColWidth;
	float fBlockColHeightDef = fBlockColHeight;
	float fBlockColDepthDef = fBlockColDepth;

	// �w�b�_�[�����J�n
	ImGui::CollapsingHeader(u8"�u���b�N�̒l�ݒ�");

	// �u���b�N����\��
	ImGui::InputInt(u8"���", &nBlockType);
	ImGui::InputInt(u8"�g�p���郂�f���̔ԍ�", &nBlockModelIdx);
	ImGui::InputFloat3(u8"����", &fBlockRot[0]);
	ImGui::InputFloat(u8"�����蔻��̕�", &fBlockColWidth);
	ImGui::InputFloat(u8"�����蔻��̍���", &fBlockColHeight);
	ImGui::InputFloat(u8"�����蔻��̉��s", &fBlockColDepth);
	ImGui::Text("");

	// �u���b�N��񔻒�
	if (nBlockType < 0)
	{// �u���b�N��ރf�[�^��0��������Ă���
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
	pEditor->SetBlockRot(D3DXVECTOR3(fBlockRot[0], fBlockRot[1], fBlockRot[2]));
	pEditor->SetBlockColWidth(fBlockColWidth);
	pEditor->SetBlockColHeight(fBlockColHeight);
	pEditor->SetBlockColDepth(fBlockColDepth);

	// ���݃}�E�X�������Ă���G���A�̃u���b�N���`��
	AreaBlockDebug(pEditor);
}

//=============================================================================
// ImGui�̌��݃}�E�X�������Ă���G���A�̃u���b�N���`�揈��
//=============================================================================
void CImGui_Jukiya::AreaBlockDebug(CEditor *pEditor)
{
	// ����\������u���b�N���擾
	CBlock *pEditBlock = pEditor->GetEditBlock();
	if(pEditBlock != NULL)
	{
		// �}�b�v�N���X���擾
		CMap *pMap = pEditor->GetMap();

		// ���f���Ǌ��N���X���擾
		CModelCreate *pModelCreate = pMap->GetModelCreate();

		// �u���b�N���擾
		int nEditBlockType = pEditBlock->GetType();
		int nEditBlockModelIdx = pEditBlock->GetModelIdx();
		float fEditBlockRot[3] = { pEditBlock->GetRot().x,pEditBlock->GetRot().y ,pEditBlock->GetRot().z };
		float fEditBlockColWidth = pEditBlock->GetBoxCollider()->GetWidth();
		float fEditBlockColHeight = pEditBlock->GetBoxCollider()->GetHeight();
		float fEditBlockColDepth = pEditBlock->GetBoxCollider()->GetDepth();
		int nEditBlockTypeDef = nEditBlockType;
		int nEditBlockModelIdxDef = nEditBlockModelIdx;
		float fEditBlockColWidthDef = fEditBlockColWidth;
		float fEditBlockColHeightDef = fEditBlockColHeight;
		float fEditBlockColDepthDef = fEditBlockColDepth;
		int nEditBlockXBlock = 0;
		int nEditBlockZBlock = 0;
		D3DXVECTOR3 EditBlockPos = pEditBlock->GetPos();

		// ���݃u���b�N������G���A���v�Z
		nEditBlockXBlock = (int)((EditBlockPos.x + (MASS_SIZE_X_HALF * MASS_BLOCK_X)) / MASS_SIZE_X_HALF);
		nEditBlockZBlock = (int)((-EditBlockPos.z + (MASS_SIZE_Z_HALF * MASS_BLOCK_Z)) / MASS_SIZE_Z_HALF);
		nEditBlockXBlock++;
		nEditBlockZBlock++;

		// �w�b�_�[�����J�n
		ImGui::CollapsingHeader(u8"�G���A�ɂ���u���b�N�̕ҏW");

		// �u���b�N����\��
		ImGui::SliderInt(u8"���̃G���A_�ҏW", &nEditBlockXBlock, 1, MASS_BLOCK_X * 2);
		ImGui::SliderInt(u8"���s�̃G���A_�ҏW", &nEditBlockZBlock, 1, MASS_BLOCK_Z * 2);
		ImGui::InputInt(u8"���_�ҏW", &nEditBlockType);
		ImGui::InputInt(u8"�g�p���郂�f���̔ԍ�_�ҏW", &nEditBlockModelIdx);
		ImGui::InputFloat3(u8"����_�ҏW", &fEditBlockRot[0]);
		ImGui::InputFloat(u8"�����蔻��̕�_�ҏW", &fEditBlockColWidth);
		ImGui::InputFloat(u8"�����蔻��̍���_�ҏW", &fEditBlockColHeight);
		ImGui::InputFloat(u8"�����蔻��̉��s_�ҏW", &fEditBlockColDepth);

		// �u���b�N��񔻒�
		if (nEditBlockType < 0)
		{// �u���b�N��ރf�[�^��0��������Ă���
			nEditBlockType = nEditBlockTypeDef;
		}
		if (nEditBlockModelIdx > pModelCreate->GetNumModel() - 1 || nEditBlockModelIdx < 0)
		{// �ǂݍ��񂾃��f���̑����𒴂��ē��͂���Ă���
			nEditBlockModelIdx = nEditBlockModelIdxDef;
		}
		if (fEditBlockColWidth < 0.0f)
		{// �����蔻�蕝���0��������Ă���
			fEditBlockColWidth = fEditBlockColWidthDef;
		}
		if (fEditBlockColHeight < 0.0f)
		{// �����蔻�荂�����0��������Ă���
			fEditBlockColHeight = fEditBlockColHeightDef;
		}
		if (fEditBlockColDepth < 0.0f)
		{// �����蔻�艜�s���0��������Ă���
			fEditBlockColDepth = fEditBlockColDepthDef;
		}

		// �{�b�N�X�R���C�_�[����蒼��������
		if (fEditBlockColWidth != fEditBlockColWidthDef || fEditBlockColHeight != fEditBlockColHeightDef || fEditBlockColDepth != fEditBlockColDepthDef)
		{// �l���ύX����Ă���
			pEditBlock->RemakeBoxCollider(fEditBlockColWidth, fEditBlockColHeight, fEditBlockColDepth);
		}

		// ���f�����쐬���Ȃ���������
		if (nEditBlockModelIdx != nEditBlockModelIdxDef)
		{
			pEditBlock->BindModel(pModelCreate->GetMesh(nEditBlockModelIdx), pModelCreate->GetBuffMat(nEditBlockModelIdx),
				pModelCreate->GetNumMat(nEditBlockModelIdx), pModelCreate->GetTexture(nEditBlockModelIdx));
		}

		// �ʒu���v�Z����
		EditBlockPos.x = ((-MASS_SIZE_X_HALF * MASS_BLOCK_X)) + (MASS_SIZE_X_HALF * (nEditBlockXBlock - 1)) + (MASS_SIZE_X_HALF / 2.0f);
		EditBlockPos.z = ((MASS_SIZE_Z_HALF * MASS_BLOCK_Z)) - (MASS_SIZE_Z_HALF * (nEditBlockZBlock - 1)) - (MASS_SIZE_Z_HALF / 2.0f);

		// �u���b�N���ݒ�
		pEditBlock->SetPos(EditBlockPos);
		pEditBlock->SetType(nEditBlockType);
		pEditBlock->SetModelIdx(nEditBlockModelIdx);
		pEditBlock->SetRot(D3DXVECTOR3(fEditBlockRot[0], fEditBlockRot[1], fEditBlockRot[2]));
		pEditBlock->GetBoxCollider()->SetPos(EditBlockPos);
		pEditBlock->GetBoxCollider()->SetWidth(fEditBlockColWidth);
		pEditBlock->GetBoxCollider()->SetHeight(fEditBlockColHeight);
		pEditBlock->GetBoxCollider()->SetDepth(fEditBlockColDepth);

		if (ImGui::Button(u8"�폜"))
		{// �{�^������
			pEditBlock->Uninit();
			pEditBlock = NULL;
			pEditor->SetEditBlock(NULL);
		}
	}
}

//=============================================================================
// ImGui�̐���`�揈��
//=============================================================================
void CImGui_Jukiya::RiverDebug(CEditor *pEditor)
{
	// �w�b�_�[�����J�n
	ImGui::CollapsingHeader(u8"��̒l�ݒ�");

	// ��ɒ���t����e�N�X�`���ԍ���ݒ�
	int nNumTex = pEditor->GetMap()->GetTextureManager()->GetNumTexture();
	int nRiverTexIdx = pEditor->GetRiverTexIdx();
	ImGui::SliderInt(u8"�e�N�X�`���̔ԍ�", &nRiverTexIdx, 0, nNumTex - 1);
	pEditor->SetRiverTexIdx(nRiverTexIdx);
}

//=============================================================================
// ImGui�̕X���`�揈��
//=============================================================================
void CImGui_Jukiya::IceDebug(CEditor *pEditor)
{
	// �w�b�_�[�����J�n
	ImGui::CollapsingHeader(u8"�X�̒l�ݒ�");

	// �X�ɒ���t����e�N�X�`���ԍ���ݒ�
	int nNumTex = pEditor->GetMap()->GetTextureManager()->GetNumTexture();
	int nIceTexIdx = pEditor->GetIceTexIdx();
	ImGui::SliderInt(u8"�e�N�X�`���̔ԍ�", &nIceTexIdx, 0, nNumTex - 1);
	pEditor->SetIceTexIdx(nIceTexIdx);
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

		// �z�u����I�u�W�F�N�g�̎�ނ�ݒ�
		int nObjMode = pEditor->GetObjectMode();
		int nObjModeDef = nObjMode;
		ImGui::RadioButton(u8"���f��", &nObjMode, CEditor::OBJECTMODE_MODEL); ImGui::SameLine();
		ImGui::RadioButton(u8"�r���{�[�h", &nObjMode, CEditor::OBJECTMODE_BILLBOARD); ImGui::SameLine();
		ImGui::RadioButton(u8"�G�t�F�N�g", &nObjMode, CEditor::OBJECTMODE_EFFECT);
		pEditor->SetObjectMode((CEditor::OBJECTMODE)nObjMode);
		ImGui::Text("");

		if (nObjMode != nObjModeDef)
		{
			pEditor->SwitchBaseObject(nObjMode);
		}

		// ���݂̃��[�h�ɂ���ď����킯
		if (nObjMode == CEditor::OBJECTMODE_MODEL)
		{// �z�u���f�����\��
			ObjModelDebug(pEditor);
		}
		else if (nObjMode == CEditor::OBJECTMODE_BILLBOARD)
		{// �z�u�r���{�[�h���\��
			ObjBillboardDebug(pEditor);
		}
		else if (nObjMode == CEditor::OBJECTMODE_EFFECT)
		{// �z�u�G�t�F�N�g���\��
			ObjEffectDebug(pEditor);
		}

		// �E�B���h�E�����I��
		ImGui::End();
	}
}

//=============================================================================
// ImGui�̔z�u���f�����`�揈��
//=============================================================================
void CImGui_Jukiya::ObjModelDebug(CEditor *pEditor)
{
	// �w�b�_�[�����J�n
	ImGui::CollapsingHeader(u8"�z�u���f���̒l�ݒ�");

	// �e��f�[�^���擾
	int nObjModelTypeMax = pEditor->GetMap()->GetNumObjectData();
	int nObjModelType = pEditor->GetObjModelType();
	float fObjModelPos[3] = { pEditor->GetObjModelPos().x, pEditor->GetObjModelPos().y ,pEditor->GetObjModelPos().z };
	int nObjModelTypeDef = nObjModelType;

	// ������360�����Z�ɕς���
	D3DXVECTOR3 ObjRot = pEditor->GetObjModelRot();
	ObjRot.x = D3DXToDegree(ObjRot.x);
	ObjRot.y = D3DXToDegree(ObjRot.y);
	ObjRot.z = D3DXToDegree(ObjRot.z);
	float fObjModelRot[3] = { ObjRot.x, ObjRot.y ,ObjRot.z };

	// �e��f�[�^��ҏW
	ImGui::InputInt(u8"�z�u���f���̎��", &nObjModelType);
	ImGui::InputFloat3(u8"�z�u���f���̍��W", &fObjModelPos[0]);
	ImGui::InputFloat3(u8"�z�u���f���̌���", &fObjModelRot[0]);

	// �e��f�[�^�ޔ���
	if (nObjModelType < 0 || nObjModelType > nObjModelTypeMax - 1)
	{
		nObjModelType = nObjModelTypeDef;
	}

	// �������~�������Z�ɕς���
	fObjModelRot[0] = D3DXToRadian(fObjModelRot[0]);
	fObjModelRot[1] = D3DXToRadian(fObjModelRot[1]);
	fObjModelRot[2] = D3DXToRadian(fObjModelRot[2]);

	// �e��f�[�^�ސݒ�
	pEditor->SetObjModelType(nObjModelType);
	pEditor->SetObjModelPos(D3DXVECTOR3(fObjModelPos[0], fObjModelPos[1], fObjModelPos[2]));
	pEditor->SetObjModelRot(D3DXVECTOR3(fObjModelRot[0], fObjModelRot[1], fObjModelRot[2]));

	// �I�u�W�F�N�g�����ւ��邩�ǂ�������
	if (nObjModelType != nObjModelTypeDef)
	{
		pEditor->SwitchBaseObject(CEditor::OBJECTMODE_MODEL);
	}

	// �x�[�X�I�u�W�F�N�g�ɒl��ݒ�
	pEditor->SetValueToBaseObject();

	// �������邩�ǂ����`�F�b�N
	if (ImGui::Button(u8"�z�u���f������"))
	{// �{�^������
		pEditor->CreateObject();
		m_nCreateObjectCounter = IMGUI_CREATE_DISP;
	}

	// �z�u�ł������ǂ����\������
	m_nCreateObjectCounter--;
	if (m_nCreateObjectCounter <= 0)
	{
		m_nCreateObjectCounter = 0;
	}
	else
	{
		ImGui::SameLine(); ImGui::Text(u8"�z�u����!!");
	}
}

//=============================================================================
// ImGui�̔z�u�r���{�[�h���`�揈��
//=============================================================================
void CImGui_Jukiya::ObjBillboardDebug(CEditor *pEditor)
{
	// �w�b�_�[�����J�n
	ImGui::CollapsingHeader(u8"�z�u�r���{�[�h�̒l�ݒ�");

	// �e��f�[�^���擾
	int nObjBillTexIdxMax = pEditor->GetMap()->GetTextureManager()->GetNumTexture();
	int nObjBillTexIdx = pEditor->GetObjBillTexIdx();
	float fObjBillPos[3] = { pEditor->GetObjBillPos().x, pEditor->GetObjBillPos().y ,pEditor->GetObjBillPos().z };
	float fObjBillCol[4] = { pEditor->GetObjBillCol().r, pEditor->GetObjBillCol().g ,pEditor->GetObjBillCol().b ,pEditor->GetObjBillCol().a };
	float fObjBillWidth = pEditor->GetObjBillWidth();
	float fObjBillHeight = pEditor->GetObjBillHeight();
	bool bObjBillLighting = pEditor->GetObjBillLighting();
	bool bObjBillDrawAddtive = pEditor->GetObjBillDrawAddtive();
	int nObjBillTexIdxDef = nObjBillTexIdx;

	// ������360�����Z�ɕς���
	float fObjBillRot = pEditor->GetObjBillRot();
	fObjBillRot = D3DXToDegree(fObjBillRot);

	// �e��f�[�^��ҏW
	ImGui::InputInt(u8"�z�u�r���{�[�h�̎g�p����e�N�X�`���̔ԍ�", &nObjBillTexIdx);
	ImGui::InputFloat3(u8"�z�u�r���{�[�h�̍��W", &fObjBillPos[0]);
	ImGui::InputFloat(u8"�z�u�r���{�[�h�̌���", &fObjBillRot);
	ImGui::ColorEdit4(u8"�z�u�r���{�[�h�̐F", &fObjBillCol[0]);
	ImGui::InputFloat(u8"�z�u�r���{�[�h�̕�", &fObjBillWidth);
	ImGui::InputFloat(u8"�z�u�r���{�[�h�̍���", &fObjBillHeight);
	ImGui::Checkbox(u8"�z�u�r���{�[�h�̃��C�e�B���O���邩�ǂ���", &bObjBillLighting);
	ImGui::Checkbox(u8"�z�u�r���{�[�h�̉��Z�����ŕ`�悷�邩�ǂ���", &bObjBillDrawAddtive);

	// �e��f�[�^�ޔ���
	if (nObjBillTexIdx < 0 || nObjBillTexIdx > nObjBillTexIdxMax - 1)
	{
		nObjBillTexIdx = nObjBillTexIdxDef;
	}

	// �������~�������Z�ɕς���
	fObjBillRot = D3DXToRadian(fObjBillRot);

	// �e��f�[�^�ސݒ�
	pEditor->SetObjBillTexIdx(nObjBillTexIdx);
	pEditor->SetObjBillPos(D3DXVECTOR3(fObjBillPos[0], fObjBillPos[1], fObjBillPos[2]));
	pEditor->SetObjBillRot(fObjBillRot);
	pEditor->SetObjBillCol(D3DXCOLOR(fObjBillCol[0], fObjBillCol[1], fObjBillCol[2], fObjBillCol[3]));
	pEditor->SetObjBillWidth(fObjBillWidth);
	pEditor->SetObjBillHeight(fObjBillHeight);
	pEditor->SetObjBillLighting(bObjBillLighting);
	pEditor->SetObjBillDrawAddtive(bObjBillDrawAddtive);

	// �e�N�X�`���𒣂�ւ��邩����
	if (nObjBillTexIdx != nObjBillTexIdxDef)
	{
		CBillboardObject *pBill = pEditor->GetBaseObject()->GetBillboardObj();
		pBill->BindTexture(pEditor->GetMap()->GetTextureManager()->GetTexture(nObjBillTexIdx));
	}

	// �x�[�X�I�u�W�F�N�g�ɒl��ݒ�
	pEditor->SetValueToBaseObject();

	// �������邩�ǂ����`�F�b�N
	if (ImGui::Button(u8"�z�u�r���{�[�h����"))
	{// �{�^������
		pEditor->CreateBillboardObj();
		m_nCreateBillObjCounter = IMGUI_CREATE_DISP;
	}

	// �z�u�ł������ǂ����\������
	m_nCreateBillObjCounter--;
	if (m_nCreateBillObjCounter <= 0)
	{
		m_nCreateBillObjCounter = 0;
	}
	else
	{
		ImGui::SameLine(); ImGui::Text(u8"�z�u����!!");
	}
}

//=============================================================================
// ImGui�̃G�t�F�N�g���`�揈��
//=============================================================================
void CImGui_Jukiya::ObjEffectDebug(CEditor *pEditor)
{
	// �w�b�_�[�����J�n
	ImGui::CollapsingHeader(u8"�z�u�G�t�F�N�g�̒l�ݒ�");

	// �e��f�[�^���擾
	int nObjEffectTypeMax = CManager::GetBaseMode()->GetEffectManager()->GetNumEmitterData();
	int nObjEffectType = pEditor->GetObjEffectType();
	float fObjEffectPos[3] = { pEditor->GetObjEffectPos().x, pEditor->GetObjEffectPos().y ,pEditor->GetObjEffectPos().z };
	int nObjEffectTypeDef = nObjEffectType;

	// ������360�����Z�ɕς���
	D3DXVECTOR3 ObjRot = pEditor->GetObjEffectRot();
	ObjRot.x = D3DXToDegree(ObjRot.x);
	ObjRot.y = D3DXToDegree(ObjRot.y);
	ObjRot.z = D3DXToDegree(ObjRot.z);
	float fObjEffectRot[3] = { ObjRot.x, ObjRot.y ,ObjRot.z };

	// �e��f�[�^��ҏW
	ImGui::InputInt(u8"�z�u�G�t�F�N�g�̎��", &nObjEffectType);
	ImGui::InputFloat3(u8"�z�u�G�t�F�N�g�̍��W", &fObjEffectPos[0]);
	ImGui::InputFloat3(u8"�z�u�G�t�F�N�g�̌���", &fObjEffectRot[0]);

	// �e��f�[�^�ޔ���
	if (nObjEffectType < 0 || nObjEffectType > nObjEffectTypeMax - 1)
	{
		nObjEffectType = nObjEffectTypeDef;
	}

	// �������~�������Z�ɕς���
	fObjEffectRot[0] = D3DXToRadian(fObjEffectRot[0]);
	fObjEffectRot[1] = D3DXToRadian(fObjEffectRot[1]);
	fObjEffectRot[2] = D3DXToRadian(fObjEffectRot[2]);

	// �e��f�[�^�ސݒ�
	pEditor->SetObjEffectType(nObjEffectType);
	pEditor->SetObjEffectPos(D3DXVECTOR3(fObjEffectPos[0], fObjEffectPos[1], fObjEffectPos[2]));
	pEditor->SetObjEffectRot(D3DXVECTOR3(fObjEffectRot[0], fObjEffectRot[1], fObjEffectRot[2]));

	// �I�u�W�F�N�g�����ւ��邩�ǂ�������
	if (nObjEffectType != nObjEffectTypeDef)
	{
		bool bEffectLoop = pEditor->GetEffectManager()->GetEmitterData(nObjEffectType)->GetLoop();
		if (bEffectLoop == true)
		{
			pEditor->SwitchBaseObject(CEditor::OBJECTMODE_EFFECT);
		}
		else
		{
			nObjEffectType = nObjEffectTypeDef;
			pEditor->SetObjEffectType(nObjEffectType);
		}
	}

	// �x�[�X�I�u�W�F�N�g�ɒl��ݒ�
	pEditor->SetValueToBaseObject();

	// �������邩�ǂ����`�F�b�N
	if (ImGui::Button(u8"�z�u�G�t�F�N�g����"))
	{// �{�^������
		pEditor->CreateEffect();
		m_nCreateEffectCounter = IMGUI_CREATE_DISP;
	}

	// �z�u�ł������ǂ����\������
	m_nCreateEffectCounter--;
	if (m_nCreateEffectCounter <= 0)
	{
		m_nCreateEffectCounter = 0;
	}
	else
	{
		ImGui::SameLine(); ImGui::Text(u8"�z�u����!!");
	}
}

//=============================================================================
// ImGui�̃��X�|�[�����`�揈��
//=============================================================================
void CImGui_Jukiya::RespawnDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// �E�B���h�E�����J�n
		ImGui::Begin(u8"�v���C���[�̃��X�|�[�����");

		// �v���C���[�̃��X�|�[�����`��
		ImGui::SliderInt(u8"���𑀍삷��ԍ�", &m_nPlayerResIdx, 0, MAX_PLAYER_RESPAWN - 1);
		RespawnInfoDebug(pEditor->GetMap()->GetPlayerRespawn(m_nPlayerResIdx));

		// �E�B���h�E�����I��
		ImGui::End();

		// �E�B���h�E�����J�n
		ImGui::Begin(u8"�G�̃��X�|�[�����");

		// �G�̃��X�|�[�����`��
		ImGui::SliderInt(u8"���𑀍삷��ԍ�", &m_nEnemyResIdx, 0, MAX_ENEMY_RESPAWN - 1);
		RespawnInfoDebug(pEditor->GetMap()->GetEnemyRespawn(m_nEnemyResIdx));

		// �E�B���h�E�����I��
		ImGui::End();
	}
}

//=============================================================================
// ImGui�̃v���C���[�̃��X�|�[�����`�揈��
//=============================================================================
void CImGui_Jukiya::RespawnInfoDebug(CRespawn *pRespawn)
{
	if (pRespawn != NULL)
	{
		// ������G���A�ԍ����擾
		int nResAreaX = pRespawn->GetAreaX();
		int nResAreaZ = pRespawn->GetAreaZ();

		// �G���A�ԍ��ҏW
		ImGui::SliderInt(u8"���̃G���A", &nResAreaX, 1, MASS_BLOCK_X);
		ImGui::SliderInt(u8"���s�̃G���A", &nResAreaZ, 1, MASS_BLOCK_Z);

		// �G���A�ԍ��ݒ�
		pRespawn->SetArea(nResAreaX, nResAreaZ);
	}
}

//=============================================================================
// ImGui�̎i�ߕ����`�揈��
//=============================================================================
void CImGui_Jukiya::HeadQuartersDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// �E�B���h�E�����J�n
		ImGui::Begin(u8"�i�ߕ����");

		// �i�ߕ��̏��`�揈��
		CHeadQuarters *pHeadQuarters = pEditor->GetMap()->GetHeadQuarters();
		if (pHeadQuarters != NULL)
		{
			// ������G���A�ԍ����擾
			int nHeadAreaX = pHeadQuarters->GetAreaX();
			int nHeadAreaZ = pHeadQuarters->GetAreaZ();

			// �G���A�ԍ��ҏW
			ImGui::SliderInt(u8"���̃G���A", &nHeadAreaX, 1, MASS_BLOCK_X);
			ImGui::SliderInt(u8"���s�̃G���A", &nHeadAreaZ, 1, MASS_BLOCK_Z);

			// �G���A�ԍ��ݒ�
			pHeadQuarters->SetArea(nHeadAreaX, nHeadAreaZ);
		}

		// �E�B���h�E�����I��
		ImGui::End();
	}
}


//=============================================================================
// ImGui�̓G�̐������`�揈��
//=============================================================================
void CImGui_Jukiya::EnemyListDebug(CEditor *pEditor)
{
	if (pEditor == NULL) return;
	if (pEditor->GetMap() == NULL) return;

	// �E�B���h�E�����J�n
	ImGui::Begin(u8"�G�̐������");

	// ��������G�̐����擾
	int nNumEnemyList = pEditor->GetMap()->GetNumEnemyListData();
	int nNumEnemyListDef = nNumEnemyList;
	ImGui::InputInt(u8"��������G�̐�", &nNumEnemyList);
	ImGui::Text("");

	// �G�̐�����
	if (nNumEnemyList < 1)
	{
		nNumEnemyList = nNumEnemyListDef;
	}
	if (nNumEnemyList != nNumEnemyListDef)
	{
		// �G�̐������f�[�^�N���X����蒼��
		pEditor->GetMap()->ReCreateEnemyData(nNumEnemyList);

		// �f�[�^��\������ԍ��𒼂�
		m_nEnemyListIdx = 0;
	}


	// �G�̐������\��
	ImGui::CollapsingHeader(u8"�G�̐������̒l�ݒ�");

	// �\������G�̐������̔ԍ���ݒ�
	m_nEnemyListIdx++;
	ImGui::SliderInt(u8"�\������ԍ�", &m_nEnemyListIdx, 1, nNumEnemyList);
	ImGui::Text("");
	m_nEnemyListIdx--;


	CEnemy_ListData **pEnemyData = pEditor->GetMap()->GetEnemyListData();
	int nEnemyListRespawnIdx = pEnemyData[m_nEnemyListIdx]->GetRespawnIdx();
	int nEnemyListEnemyType = pEnemyData[m_nEnemyListIdx]->GetEnemyType();
	int nEnemyListRespawnTime = pEnemyData[m_nEnemyListIdx]->GetRespawnTime();
	bool bEnemyListItem = pEnemyData[m_nEnemyListIdx]->GetItem();
	int nEnemyListItemType = pEnemyData[m_nEnemyListIdx]->GetItemType();
	int nEnemyListAI_Bullet = pEnemyData[m_nEnemyListIdx]->GetAI_BulletEva();
	int nEnemyListAI_Mass = pEnemyData[m_nEnemyListIdx]->GetAI_MassEva();
	int nEnemyListAI_Down = pEnemyData[m_nEnemyListIdx]->GetAI_DownEva();
	ImGui::SliderInt(u8"���X�|�[���ʒu�̔ԍ�", &nEnemyListRespawnIdx, 0, MAX_ENEMY_RESPAWN - 1);
	ImGui::InputInt(u8"���", &nEnemyListEnemyType);
	ImGui::InputInt(u8"���X�|�[�����鎞��", &nEnemyListRespawnTime);
	ImGui::Checkbox(u8"�|�������ɃA�C�e���𐶐����邩�ǂ���", &bEnemyListItem);
	if (bEnemyListItem == true)
	{
		ImGui::InputInt(u8"�o��������A�C�e���̔ԍ�", &nEnemyListItemType);
	}

	ImGui::InputInt(u8"�e��łۂ̃����_���]���l", &nEnemyListAI_Bullet);
	ImGui::InputInt(u8"������ς������郉���_���]���l", &nEnemyListAI_Mass);
	ImGui::InputInt(u8"�������ɓ������郉���_���]���l", &nEnemyListAI_Down);

	// �f�[�^����
	if (nEnemyListRespawnTime < 0)
	{
		nEnemyListRespawnTime = 0;
	}
	if (nEnemyListEnemyType < 0)
	{
		nEnemyListEnemyType = 0;
	}
	if (nEnemyListItemType < 0)
	{
		nEnemyListItemType = 0;
	}

	// AI�̃f�[�^
	if (nEnemyListAI_Bullet < 1)
	{
		nEnemyListAI_Bullet = 1;
	}
	if (nEnemyListAI_Mass < 1)
	{
		nEnemyListAI_Mass = 1;
	}
	if (nEnemyListAI_Down < 1)
	{
		nEnemyListAI_Down = 1;
	}

	// �f�[�^�ݒ�
	CEnemy_ListData EnemyData;
	EnemyData.SetRespawnIdx(nEnemyListRespawnIdx);
	EnemyData.SetEnemyType(nEnemyListEnemyType);
	EnemyData.SetRespawnTime(nEnemyListRespawnTime);
	EnemyData.SetItem(bEnemyListItem);
	EnemyData.SetItemType(nEnemyListItemType);
	EnemyData.SetAI_BulletEva(nEnemyListAI_Bullet);
	EnemyData.SetAI_MassEva(nEnemyListAI_Mass);
	EnemyData.SetAI_DownEva(nEnemyListAI_Down);
	pEditor->GetMap()->GetEnemyListData(m_nEnemyListIdx)->Cpy(&EnemyData);

	// �E�B���h�E�����I��
	ImGui::End();
}

//=============================================================================
// ImGui�̓G�̐������`�揈��
//=============================================================================
void CImGui_Jukiya::SkyDebug(CEditor *pEditor)
{
	if (pEditor == NULL) return;
	if (pEditor->GetMap() == NULL) return;

	// �E�B���h�E�����J�n
	ImGui::Begin(u8"����");

	// ��ɒ���t����e�N�X�`���ԍ���ݒ�
	CSky *pSky = pEditor->GetMap()->GetSky();
	int nNumTex = pEditor->GetMap()->GetTextureManager()->GetNumTexture();
	int nSkyTexIdx = pSky->GetTexIdx();
	ImGui::SliderInt(u8"�e�N�X�`���̔ԍ�", &nSkyTexIdx, 0, nNumTex - 1);
	pSky->SetTexIdx(nSkyTexIdx);
	pSky->BindTexture(pEditor->GetMap()->GetTextureManager()->GetTexture(nSkyTexIdx));

	// �E�B���h�E�����I��
	ImGui::End();
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
	ClearModeVariable();         // ��ʐ؂�ւ��p
	ClearModelListVariable();    // ���f�����X�g�p
	ClearTexListVariable();      // �e�N�X�`�����X�g�p
	ClearLightVariable();        // ���C�g�p
	ClearGameFieldVariable();    // �Q�[���t�B�[���h�p
	ClearObjectVariable();       // �z�u���p
	ClearRespawnVariable();      // ���X�|�[���p
	ClearEnemyListVariable();    // �G�̐������p
}

//=============================================================================
// ImGui�̉�ʐ؂�ւ��p�ϐ����N���A����
//=============================================================================
void CImGui_Jukiya::ClearModeVariable(void)
{
	m_nLoadModeCounter = 0;
	m_bLoadModeDisp = false;
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
	m_nLightIdx = 0;
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
	m_nCreateObjectCounter = 0;
	m_nCreateBillObjCounter = 0;
	m_nCreateEffectCounter = 0;
}

//=============================================================================
// ImGui�̃��X�|�[���p�ϐ����N���A����
//=============================================================================
void CImGui_Jukiya::ClearRespawnVariable(void)
{
	m_nPlayerResIdx = 0;
	m_nEnemyResIdx = 0;
}

//=============================================================================
// ImGui�̓G�̐������p�ϐ����N���A����
//=============================================================================
void CImGui_Jukiya::ClearEnemyListVariable(void)
{
	m_nEnemyListIdx = 0;
}