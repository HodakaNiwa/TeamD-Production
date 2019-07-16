//=============================================================================
//
// �G�f�B�^�[���� [editor.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "editor.h"
#include "scene.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "meshfield.h"
#include "meshCollider.h"
#include "debugproc.h"
#include "fade.h"
#include "map.h"
#include "modelcreate.h"
#include "textureManager.h"
#include "grid.h"
#include "block.h"
#include "river.h"
#include "icefield.h"
#include "camera.h"
#include "cameraManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "model.h"
#include "respawn.h"
#include "3DPolygon.h"
#include "UI.h"
#include "effectManager.h"
#include "object.h"
#include "characterManager.h"
#include "billboardObject.h"
#include "emitter.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define EDITOR_SYSTEM_FILENAME            "data/TEXT/editor.ini"    // �������Ɏg�p����V�X�e���t�@�C����
#define EDITOR_MAP_SAVEFILENAME_INI       "map.txt"                 // �}�b�v����ۑ�����t�@�C����
#define EDITOR_MODELLIST_SAVEFILENAME_INI "modellist.txt"           // ���f�����X�g����ۑ�����t�@�C����
#define EDITOR_TEXLIST_SAVEFILENAME_INI   "texlist.txt"             // �e�N�X�`�����X�g����ۑ�����t�@�C����
#define EDITOR_LIGHT_SAVEFILENAME_INI     "light.txt"               // ���C�g����ۑ�����t�@�C����
#define EDITOR_GAMEFILED_SAVEFILENAME_INI "gamefield.txt"           // �Q�[���t�B�[���h����ۑ�����t�@�C����
#define EDITOR_OBJECT_SAVEFILENAME_INI    "object.txt"              // �z�u������ۑ�����t�@�C����
#define EDITOR_ENEMYLIST_SAVEFILENAME_INI "enemylist.txt"           // �G�̐�������ۑ�����t�@�C����
#define EDITOR_PLAYERRESPAWN_FILENAME     "PLAYERRESPAWN_MODEL = "  // �v���C���[�̃��X�|�[�����f���̃t�@�C����
#define EDITOR_ENEMYRESPAWN_FILENAME      "ENEMYRESPAWN_MODEL = "   // �G�̃��X�|�[�����f���̃t�@�C����
#define EDITOR_POLY_HEIGHT                (1.0f)                    // �G���A�����p�̃|���S���̍���
#define EDITOR_LOAD_MAPFILENAME_DEF       "data/TEXT/MAP/map.txt"   // �ǂݍ��ރ}�b�v�̃t�@�C�����f�t�H���g�l


// �ǂݍ��ݗp�̃p�X
#define EFFECT_FILENAME "EFFECT_FILENAME = "

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================


//=============================================================================
// �G�f�B�^�[�̃R���X�g���N�^
//=============================================================================
CEditor::CEditor()
{

}

//=============================================================================
// �G�f�B�^�[�̃f�X�g���N�^
//=============================================================================
CEditor::~CEditor()
{

}

//=============================================================================
// �G�f�B�^�[�̐�������
//=============================================================================
CEditor *CEditor::Create()
{
	CEditor *pEditor = NULL;
	if (pEditor == NULL)
	{
		pEditor = new CEditor;
		if (pEditor != NULL)
		{
			pEditor->Init();
		}
	}
	return pEditor;
}

//=============================================================================
// �G�f�B�^�[�̏���������
//=============================================================================
HRESULT CEditor::Init(void)
{
	// �ϐ�������������
	ClearVariable();

	// �V�X�e������ǂݍ���
	LoadSystem();

	// ���łɋN�����Ă���Ȃ�}�b�v�̃t�@�C���������߂Đݒ�
	if (CManager::GetStartUp() == true)
	{
		strcpy(m_aMapFileName, CManager::GetLoadMapFileName());
	}

	// �J�����𐶐�����
	CreateCamera();

	//�}�b�v�𐶐�
	CreateMap();

	// UI�𐶐�
	CreateUI();

	// �n�ʃN���X�ւ̃|�C���^���擾
	m_pMeshField = GetMap()->GetMeshField();

	// ���b�V���R���C�_�[�N���X�ւ̃|�C���^���擾
	m_pMeshCollider = CMeshCollider::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_BLOCK_X * 2, MASS_BLOCK_Z * 2);

	// �O���b�h���𐶐�
	m_pGrid = CGrid::Create(D3DXVECTOR3(0.0f, 2.0f, 0.0f), MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_BLOCK_X * 2, MASS_BLOCK_Z * 2);

	// �G���A�����p�̃|���S���𐶐�
	m_pArea = C3DPolygon::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF);

	// ���X�|�[�����f����ݒ�
	SetRespawnModel();

	return S_OK;
}

//=============================================================================
// �G�f�B�^�[�̏I������
//=============================================================================
void CEditor::Uninit(void)
{
	// UI�̔j��
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = NULL;
	}

	// �O���b�h���̔j��
	if (m_pGrid != NULL)
	{
		m_pGrid->Uninit();
		delete m_pGrid;
		m_pGrid = NULL;
	}

	// �G���A�����p�̃|���S���̔j��
	if (m_pArea != NULL)
	{
		m_pArea->Uninit();
		delete m_pArea;
		m_pArea = NULL;
	}

	// �x�[�X�I�u�W�F�N�g�̔j��
	ReleaseBaseObject();

	// ���ʂ̏I������
	CBasemode::Uninit();
}

//=============================================================================
// �G�f�B�^�[�̍X�V����
//=============================================================================
void CEditor::Update(void)
{
	// �J�����̍X�V����
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->GetCamera(m_CameraMode)->Update();
	}

	//�S�Ă̍X�V����
	CScene::UpdateAll();

	// �O���b�h���̍X�V
	if (m_pGrid != NULL)
	{
		m_pGrid->Update();
	}

	// �e�G�f�B�b�g���[�h�ɂ���ď����킯
	if (m_EditMode == EDITMODE_GAMEFIELD)
	{
		// �n�ʂɃ}�E�X�̃��C���ђʂ��Ă��邩
		CheckFieldPenetration();
	}
}

//=============================================================================
// �G�f�B�^�[�̕`�揈��
//=============================================================================
void CEditor::Draw(void)
{
	// �ҏW����u���b�N��T��
	ShearchEditBlock();

	// �J�����̐ݒ�
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera(m_CameraMode);
	}

	// �V�[���N���X�̕`�揈��
	CScene::DrawAll();

	// �G���A�����p�̃|���S���̕`��
	if (m_pArea != NULL)
	{
		// ���C�e�B���O�̐ݒ���O��
		DWORD Lighting;
		CManager::GetRenderer()->GetDevice()->GetRenderState(D3DRS_LIGHTING, &Lighting);
		CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		m_pArea->Draw();
		CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_LIGHTING, Lighting);
	}

	// �O���b�h���̕`��
	if (m_pGrid != NULL && m_bDispGrid == true)
	{
		m_pGrid->Draw();
	}
}

//=============================================================================
// �G���A���������鏈��
//=============================================================================
void CEditor::ExChangeArea(void)
{
	// �n��
	ExChangeField();

	// �O���b�h��
	ExChangeGrid();
}

//=============================================================================
// �n�ʂ��������鏈��
//=============================================================================
void CEditor::ExChangeField(void)
{
	// ���̒n�ʂ�j��
	if (m_pMeshCollider != NULL)
	{
		m_pMeshCollider->Uninit();
		m_pMeshCollider = NULL;
	}

	// �V�����n�ʂ𐶐�
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{// �u���b�N��u�����[�h��������
		m_pMeshCollider = CMeshCollider::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
			MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_BLOCK_X * 2, MASS_BLOCK_Z * 2);
	}
	else
	{// ����ȊO
		m_pMeshCollider = CMeshCollider::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
			MASS_SIZE_X, MASS_SIZE_Z, MASS_BLOCK_X, MASS_BLOCK_Z);
	}
}

//=============================================================================
// �O���b�h�����������鏈��
//=============================================================================
void CEditor::ExChangeGrid(void)
{
	// ���̃O���b�h����j��
	if (m_pGrid != NULL)
	{
		m_pGrid->Uninit();
		delete m_pGrid;
		m_pGrid = NULL;
	}

	// �V�����O���b�h���𐶐�
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{// �u���b�N��u�����[�h��������
		m_pGrid = CGrid::Create(D3DXVECTOR3(0.0f, 1.0f, 0.0f), MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_BLOCK_X * 2, MASS_BLOCK_Z * 2);
	}
	else
	{// ����ȊO
		m_pGrid = CGrid::Create(D3DXVECTOR3(0.0f, 1.0f, 0.0f), MASS_SIZE_X, MASS_SIZE_Z, MASS_BLOCK_X, MASS_BLOCK_Z);
	}
}

//=============================================================================
// �n�ʂɃ}�E�X�̃��C���ђʂ��Ă��邩�`�F�b�N���鏈��
//=============================================================================
void CEditor::CheckFieldPenetration(void)
{
	// �t�B�[���h�Ƀ}�E�X�̃��C���h�����Ă��邩�`�F�b�N
	int nMASSX = 0, nMASSZ = 0;
	D3DXVECTOR3 MouseWorldPos1 = CManager::GetMouse()->GetWorldPosZn();
	D3DXVECTOR3 MouseWorldPos2 = CManager::GetMouse()->GetWorldPosZf();
	D3DXVECTOR3 PolyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 PolyNor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 Nor = MouseWorldPos2 - MouseWorldPos1;
	D3DXVec3Normalize(&Nor, &Nor);
	if (Nor.y <= 0.0f && m_pMeshField != NULL)
	{// �n�ʂɌ������ă��C������Ă��� && �n�ʂ���������Ă���
		D3DXVECTOR3 PenePos = MouseWorldPos1 + (D3DXVec3Dot(&-MouseWorldPos1, &PolyNor) / D3DXVec3Dot(&Nor, &PolyNor) * Nor);
		if (m_pMeshCollider->CheckPenetration(PenePos, &nMASSX, &nMASSZ, &m_nLeftVerIdx) == true)
		{// �n�ʂɃ��C���ђʂ��Ă���
		    // �G���A���킩��₷�����邽�߂̃|���S����z�u
			BoxVisualization(nMASSX, nMASSZ);
			PenetrationFieldArea(nMASSX, nMASSZ, m_nLeftVerIdx);
		}
	}
}

//=============================================================================
// �}�E�X���G��Ă���t�B�[���h�̃G���A���������鏈��
//=============================================================================
void CEditor::BoxVisualization(int nMassX, int nMassZ)
{
	// �n�ʂ��璸�_���W���擾����
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	D3DXVECTOR3 VerPos = INITIALIZE_D3DXVECTOR3;
	pVtxBuff = m_pMeshCollider->GetVtxBuff();
	float fWidth = 0.0f;
	float fDepth = 0.0f;
	float fAddWidth = 0.0f;
	float fAddDepth = 0.0f;
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{
		fAddWidth = ((MASS_SIZE_X_HALF * MASS_BLOCK_X * 2) / 2) + (MASS_SIZE_X_HALF / 2);
		fAddDepth = ((MASS_SIZE_Z_HALF * MASS_BLOCK_Z * 2) / 2) + (MASS_SIZE_Z_HALF / 2);
		fWidth = MASS_SIZE_X_HALF;
		fDepth = MASS_SIZE_Z_HALF;
	}
	else
	{
		fAddWidth = (MASS_SIZE_X * MASS_BLOCK_X) / 2 + (MASS_SIZE_X / 2);
		fAddDepth = (MASS_SIZE_Z * MASS_BLOCK_Z) / 2 + (MASS_SIZE_Z / 2);
		fWidth = MASS_SIZE_X;
		fDepth = MASS_SIZE_Z;
	}

	// ���݂̃G���A�ԍ�������W���v�Z
	VerPos.x = (nMassX * fWidth) - fAddWidth;
	VerPos.z = -(nMassZ * fDepth) + fAddDepth;

	// �G���A�����p�̃|���S���̒��_�𓮂���
	pVtxBuff = m_pArea->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �|���S���̈ʒu��ݒ�
	m_pArea->SetPos(VerPos);

	// ���_���W��ݒ�
	pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, EDITOR_POLY_HEIGHT, fDepth / 2);
	pVtx[1].pos = D3DXVECTOR3(fWidth / 2, EDITOR_POLY_HEIGHT, fDepth / 2);
	pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, EDITOR_POLY_HEIGHT, -fDepth / 2);
	pVtx[3].pos = D3DXVECTOR3(fWidth / 2, EDITOR_POLY_HEIGHT, -fDepth / 2);

	// �F��ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
// �}�E�X���G��Ă���t�B�[���h�̃G���A���Z�o���鏈��
//=============================================================================
void CEditor::PenetrationFieldArea(int nMassX, int nMassZ, int nLeftVerIndex)
{
	CInputMouse *pMouse = CManager::GetMouse();
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_2) == true
		|| pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true && pKeyboard->GetPress(DIK_LALT) == true)
	{// �}�E�X�̒��N���b�N�������ꂽ
		// �����ꂽ�Ƃ��̃}�X�ڂ��L�^
		m_nMassX = nMassX;
		m_nMassZ = nMassZ;
		m_nStartLeftVerIndex = nLeftVerIndex;
	}

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_2) == true
		|| pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == true && pKeyboard->GetPress(DIK_LALT) == true)
	{// �}�E�X�̒��N���b�N��������Ă���
		// ���̕��������v�Z
		int nSwitchX = 0;
		bool bSwitchX = false;
		nSwitchX = m_nMassX - nMassX;
		if (nSwitchX > 0)
		{
			bSwitchX = true;
		}

		// �c�̕��������v�Z
		int nSwitchZ = 0;
		bool bSwitchZ = false;
		nSwitchZ = m_nMassZ - nMassZ;
		if (nSwitchZ > 0)
		{
			bSwitchZ = true;
		}

		// �G���A�̕��������c�����ꂼ��v�Z
		m_nBoxSide = abs(m_nMassX - nMassX) + 1;
		m_nBoxVertical = abs(m_nMassZ - nMassZ) + 1;

		// �G���A���킩��₷�����邽�߂̃|���S����z�u
		AreaVisualization(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}

	if (pMouse->GetRelease(CInputMouse::DIMS_BUTTON_2) == true
		|| pMouse->GetRelease(CInputMouse::DIMS_BUTTON_0) == true && pKeyboard->GetPress(DIK_LALT) == true)
	{// �}�E�X�̒��N���b�N�������ꂽ
		// �ŏI�I�ȕ�������ۑ�
		m_nBoxSide = abs(m_nMassX - nMassX) + 1;
		m_nBoxVertical = abs(m_nMassZ - nMassZ) + 1;

		// ���̕��������v�Z
		int nSwitchX = 0;
		bool bSwitchX = false;
		nSwitchX = m_nMassX - nMassX;
		if (nSwitchX > 0)
		{
			bSwitchX = true;
		}

		// �c�̕��������v�Z
		int nSwitchZ = 0;
		bool bSwitchZ = false;
		nSwitchZ = m_nMassZ - nMassZ;
		if (nSwitchZ > 0)
		{
			bSwitchZ = true;
		}

		// �I�u�W�F�N�g��z�u OR �I�u�W�F�N�g��j��
		if (m_Mode == MODE_CREATE)
		{
			CreateGameFieldObj(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
		}
		else if(m_Mode == MODE_DESTROY)
		{
			DestroyGameFieldObj(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g��z�u����G���A���������鏈��
//=============================================================================
void CEditor::AreaVisualization(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// �n�ʂ��璸�_���W���擾����
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	D3DXVECTOR3 VerPos = INITIALIZE_D3DXVECTOR3;
	pVtxBuff = m_pMeshCollider->GetVtxBuff();
	int nXBlock = m_pMeshCollider->GetXBlock();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (bSwitchX == true && bSwitchZ == false)
	{// �����Ɍ����ē����Ă���
		VerPos = pVtx[m_nStartLeftVerIndex - nSwitchX].pos;
	}
	else if (bSwitchX == true && bSwitchZ == true)
	{// ����Ɍ����ē����Ă���
		VerPos = pVtx[m_nLeftVerIdx].pos;
	}
	else if (bSwitchX == false && bSwitchZ == true)
	{// �E��Ɍ����ē����Ă���
		VerPos = pVtx[m_nStartLeftVerIndex - ((nXBlock + 1) * (m_nBoxVertical - 1))].pos;
	}
	else
	{// �ʏ펞&�E���Ɍ����ē����Ă���
		VerPos = pVtx[m_nStartLeftVerIndex].pos;
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// �G���A�����p�̃|���S���̒��_�𓮂���
	pVtxBuff = m_pArea->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = 0.0f;
	float fDepth = 0.0f;
	float fAddWidth = 0.0f;
	float fAddDepth = 0.0f;
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{
		fAddWidth = MASS_SIZE_X_HALF;
		fAddDepth = MASS_SIZE_Z_HALF;
		if (bSwitchX == true)
		{
			fWidth = MASS_SIZE_X_HALF;
		}
		else
		{
			fWidth = -MASS_SIZE_X_HALF;
		}
		if (bSwitchZ == true)
		{
			fDepth = MASS_SIZE_Z_HALF;
		}
		else
		{
			fDepth = -MASS_SIZE_Z_HALF;
		}
	}
	else
	{
		fAddWidth = MASS_SIZE_X;
		fAddDepth = MASS_SIZE_Z;
		if (bSwitchX == true)
		{
			fWidth = MASS_SIZE_X;
		}
		else
		{
			fWidth = -MASS_SIZE_X;
		}
		if (bSwitchZ == true)
		{
			fDepth = MASS_SIZE_Z;
		}
		else
		{
			fDepth = -MASS_SIZE_Z;
		}
	}

	// �|���S���̈ʒu��ݒ�
	m_pArea->SetPos(INITIALIZE_D3DXVECTOR3);

	// ���_���W��ݒ�
	pVtx[0].pos = VerPos + D3DXVECTOR3(0.0f, EDITOR_POLY_HEIGHT, 0.0f);
	pVtx[1].pos = VerPos + D3DXVECTOR3(fWidth * nSwitchX + fAddWidth, EDITOR_POLY_HEIGHT, 0.0f);
	pVtx[2].pos = VerPos + D3DXVECTOR3(0.0f, EDITOR_POLY_HEIGHT, -fDepth * nSwitchZ - fAddDepth);
	pVtx[3].pos = VerPos + D3DXVECTOR3(fWidth * nSwitchX + fAddWidth, EDITOR_POLY_HEIGHT, -fDepth * nSwitchZ - fAddDepth);

	// �F��ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
// �w�肳�ꂽ�G���A�ɃQ�[���t�B�[���h�p�I�u�W�F�N�g��z�u���鏈��
//=============================================================================
void CEditor::CreateGameFieldObj(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// ���݂̃Q�[���t�B�[���h���[�h�ɂ���ď����킯
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{
		BlockCreate(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
	else if(m_GameFieldMode == GAMEFIELDMODE_RIVER)
	{
		RiverCreate(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
	else if (m_GameFieldMode == GAMEFIELDMODE_ICE)
	{
		IceCreate(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
}

//=============================================================================
// �w�肳�ꂽ�G���A�Ƀu���b�N��z�u���鏈��
//=============================================================================
void CEditor::BlockCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	CModelCreate *pModelCreate = GetMap()->GetModelCreate();  // ���f���Ǌ��N���X
	CBlock *pBlock = NULL;                                    // �I�u�W�F�N�g�N���X�ւ̃|�C���^
	D3DXVECTOR3 ObjPos;                                       // �I�u�W�F�N�g��z�u����ʒu
	D3DXVECTOR3 FieldPos = m_pMeshCollider->GetPos();         // �n�ʂ̍��W
	int nXBlock = m_pMeshCollider->GetXBlock();               // �n�ʂ̉��̕�����
	int nZBlock = m_pMeshCollider->GetZBlock();               // �n�ʂ̏c�̕�����
	int nCntVertexIndex = m_nStartLeftVerIndex;               // ���݂̍���̒��_�ԍ�
	int nData = nCntVertexIndex;                              // ���_�ԍ��ۑ��p
	int nSelect = m_nBlockModelIdx;                           // �z�u���郂�f���̔ԍ�

	// ���_�o�b�t�@�̎擾
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshCollider->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = m_nMassX; nCntV < (m_nMassX + m_nBoxVertical); nCntV++)
	{// ���s�̕��������J��Ԃ�
		for (int nCntH = m_nMassZ; nCntH < (m_nMassZ + m_nBoxSide); nCntH++)
		{// ���̕��������J��Ԃ�
			// �I�u�W�F�N�g��z�u����ʒu���v�Z
			ObjPos = pVtx[nCntVertexIndex].pos;
			ObjPos.x += (MASS_SIZE_X_HALF / 2);
			ObjPos.y = 0.0f;
			ObjPos.z -= (MASS_SIZE_Z_HALF / 2);

			// �I�u�W�F�N�g��z�u����
			GetMap()->CreateBlock(ObjPos, m_BlockRot, m_nBlockType, m_nBlockModelIdx,
				pModelCreate->GetMesh(m_nBlockModelIdx), pModelCreate->GetBuffMat(m_nBlockModelIdx),
				pModelCreate->GetNumMat(m_nBlockModelIdx), pModelCreate->GetTexture(m_nBlockModelIdx),
				m_fBlockColWidth, m_fBlockColHeight, m_fBlockColDepth);

			// ���_�ԍ������炷
			if (bSwitchX == false)
			{
				nCntVertexIndex++;
			}
			else
			{
				nCntVertexIndex--;
			}
		}

		// ���_�ԍ������炷
		if (bSwitchZ == false)
		{
			nCntVertexIndex = (nData + nXBlock + 1);
			nData = nCntVertexIndex;
		}
		else
		{
			nCntVertexIndex = (nData - nXBlock - 1);
			nData = nCntVertexIndex;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
// �w�肳�ꂽ�G���A�ɐ��z�u���鏈��
//=============================================================================
void CEditor::RiverCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// ���_���̐ݒ�
	int nXBlock = m_pMeshField->GetXBlock();
	int nZBlock = m_pMeshField->GetZBlock();
	int nCntVertexIndex = m_nStartLeftVerIndex;
	int nData = nCntVertexIndex;
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();
	D3DXVECTOR3 RiverPos = D3DXVECTOR3(0.0f, RIVER_DOWN_FIELD + (-RIVER_DOWN_FIELD / 2), 0.0f);

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = m_nMassX; nCntV < (m_nMassX + m_nBoxVertical); nCntV++)
	{// ���s�̕��������J��Ԃ�
		for (int nCntH = m_nMassZ; nCntH < (m_nMassZ + m_nBoxSide); nCntH++)
		{// ���̕��������J��Ԃ�
			pVtx[nCntVertexIndex].pos.y = RIVER_DOWN_FIELD;
			pVtx[nCntVertexIndex + 1].pos.y = RIVER_DOWN_FIELD;
			pVtx[nCntVertexIndex + (nXBlock + 1) + 1].pos.y = RIVER_DOWN_FIELD;
			pVtx[nCntVertexIndex + (nXBlock + 1)].pos.y = RIVER_DOWN_FIELD;

			// ���_�ԍ������炷
			if (bSwitchX == false)
			{
				nCntVertexIndex++;
			}
			else
			{
				nCntVertexIndex--;
			}
		}

		// ���_�ԍ������炷
		if (bSwitchZ == false)
		{
			nCntVertexIndex = (nData + nXBlock + 1);
			nData = nCntVertexIndex;
		}
		else
		{
			nCntVertexIndex = (nData - nXBlock - 1);
			nData = nCntVertexIndex;
		}
	}

	// ��̍��W��ݒ�
	RiverPos.x = pVtx[m_nStartLeftVerIndex].pos.x;
	RiverPos.z = pVtx[m_nStartLeftVerIndex].pos.z;

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();


	// ��𐶐�
	CTextureManager *pTexManager = GetMap()->GetTextureManager();
	if (bSwitchX == true)
	{
		RiverPos.x -= (MASS_SIZE_X_HALF * m_nBoxSide) - MASS_SIZE_X;
	}
	else
	{
		RiverPos.x += (MASS_SIZE_X_HALF * m_nBoxSide);
	}
	if (bSwitchZ == true)
	{
		RiverPos.z += (MASS_SIZE_Z_HALF * m_nBoxVertical) - MASS_SIZE_Z;
	}
	else
	{
		RiverPos.z -= (MASS_SIZE_Z_HALF * m_nBoxVertical);
	}

	CRiver::Create(RiverPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pTexManager->GetTexture(m_nRiverTexIdx),
		MASS_SIZE_X * m_nBoxSide, 75.0f, MASS_SIZE_Z * m_nBoxVertical, m_nBoxSide, m_nBoxVertical, m_nRiverTexIdx);
}

//=============================================================================
// �w�肳�ꂽ�G���A�ɕX��z�u���鏈��
//=============================================================================
void CEditor::IceCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// ���_���̐ݒ�
	int nXBlock = m_pMeshField->GetXBlock();
	int nZBlock = m_pMeshField->GetZBlock();
	int nCntVertexIndex = m_nStartLeftVerIndex;
	int nData = nCntVertexIndex;
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();
	D3DXVECTOR3 IcePos = D3DXVECTOR3(0.0f, ICE_DOWN_FIELD + (-ICE_DOWN_FIELD / 2), 0.0f);

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = m_nMassX; nCntV < (m_nMassX + m_nBoxVertical); nCntV++)
	{// ���s�̕��������J��Ԃ�
		for (int nCntH = m_nMassZ; nCntH < (m_nMassZ + m_nBoxSide); nCntH++)
		{// ���̕��������J��Ԃ�
			pVtx[nCntVertexIndex].pos.y = ICE_DOWN_FIELD;
			pVtx[nCntVertexIndex + 1].pos.y = ICE_DOWN_FIELD;
			pVtx[nCntVertexIndex + (nXBlock + 1) + 1].pos.y = ICE_DOWN_FIELD;
			pVtx[nCntVertexIndex + (nXBlock + 1)].pos.y = ICE_DOWN_FIELD;

			// ���_�ԍ������炷
			if (bSwitchX == false)
			{
				nCntVertexIndex++;
			}
			else
			{
				nCntVertexIndex--;
			}
		}

		// ���_�ԍ������炷
		if (bSwitchZ == false)
		{
			nCntVertexIndex = (nData + nXBlock + 1);
			nData = nCntVertexIndex;
		}
		else
		{
			nCntVertexIndex = (nData - nXBlock - 1);
			nData = nCntVertexIndex;
		}
	}

	// ��̍��W��ݒ�
	IcePos.x = pVtx[m_nStartLeftVerIndex].pos.x;
	IcePos.z = pVtx[m_nStartLeftVerIndex].pos.z;

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();


	// �X�𐶐�
	CTextureManager *pTexManager = GetMap()->GetTextureManager();
	if (bSwitchX == true)
	{
		IcePos.x -= (MASS_SIZE_X_HALF * m_nBoxSide) - MASS_SIZE_X;
	}
	else
	{
		IcePos.x += (MASS_SIZE_X_HALF * m_nBoxSide);
	}
	if (bSwitchZ == true)
	{
		IcePos.z += (MASS_SIZE_Z_HALF * m_nBoxVertical) - MASS_SIZE_Z;
	}
	else
	{
		IcePos.z -= (MASS_SIZE_Z_HALF * m_nBoxVertical);
	}
	CIceField::Create(IcePos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pTexManager->GetTexture(m_nIceTexIdx),
		MASS_SIZE_X * m_nBoxSide, 75.0f, MASS_SIZE_Z * m_nBoxVertical, m_nBoxSide, m_nBoxVertical, m_nIceTexIdx);
}

//=============================================================================
// �w�肳�ꂽ�G���A�̃I�u�W�F�N�g��j�����鏈��
//=============================================================================
void CEditor::DestroyGameFieldObj(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// ���݂̃Q�[���t�B�[���h���[�h�ɂ���ď����킯
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{
		BlockDestroyCheck(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
	else if (m_GameFieldMode == GAMEFIELDMODE_RIVER)
	{
		RiverDestroyCheck(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
	else if (m_GameFieldMode == GAMEFIELDMODE_ICE)
	{
		IceDestroyCheck(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
}

//=============================================================================
// �w�肳�ꂽ�G���A�̃u���b�N��j�����鏈��
//=============================================================================
void CEditor::BlockDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	D3DXVECTOR3 DestroyPos;                             // �I�u�W�F�N�g������������W
	int nCntVertexIndex = m_nStartLeftVerIndex;         // ���݂̍���̒��_�ԍ�
	int nData = nCntVertexIndex;                        // ���_�ԍ��ۑ��p

    // �����͈͂��Ɍv�Z
	float fDestroyWidth = (float)(m_nBoxSide * MASS_SIZE_X_HALF);
	float fDestroyDepth = (float)(m_nBoxVertical * MASS_SIZE_Z_HALF);

	// ���_�o�b�t�@�̎擾
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshCollider->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �I�u�W�F�N�g�������͈͂̍�����W��ݒ�
	DestroyPos = pVtx[nCntVertexIndex].pos;

	// �I�u�W�F�N�g������
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// �I�u�W�F�N�g�ւ̃|�C���^������
				BlockDestroy((CBlock*)pScene, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ);
			}

			pScene = pSceneNext;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
// �w�肳�ꂽ�G���A�̐��j�����鏈��
//=============================================================================
void CEditor::RiverDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	D3DXVECTOR3 DestroyPos;                             // �I�u�W�F�N�g������������W
	int nCntVertexIndex = m_nStartLeftVerIndex;         // ���݂̍���̒��_�ԍ�
	int nData = nCntVertexIndex;                        // ���_�ԍ��ۑ��p

	// �����͈͂��Ɍv�Z
	float fDestroyWidth = (float)(m_nBoxSide * MASS_SIZE_X);
	float fDestroyDepth = (float)(m_nBoxVertical * MASS_SIZE_Z);

	// ���_�o�b�t�@�̎擾
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �I�u�W�F�N�g�������͈͂̍�����W��ݒ�
	DestroyPos = pVtx[nCntVertexIndex].pos;

	// �I�u�W�F�N�g������
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_RIVER)
			{// �I�u�W�F�N�g�ւ̃|�C���^������
				RiverDestroy((CRiver*)pScene, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ);
			}

			pScene = pSceneNext;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
// �w�肳�ꂽ�G���A�̕X��j�����鏈��
//=============================================================================
void CEditor::IceDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	D3DXVECTOR3 DestroyPos;                             // �I�u�W�F�N�g������������W
	int nCntVertexIndex = m_nStartLeftVerIndex;         // ���݂̍���̒��_�ԍ�
	int nData = nCntVertexIndex;                        // ���_�ԍ��ۑ��p

	// �����͈͂��Ɍv�Z
	float fDestroyWidth = (float)(m_nBoxSide * MASS_SIZE_X);
	float fDestroyDepth = (float)(m_nBoxVertical * MASS_SIZE_Z);

	// ���_�o�b�t�@�̎擾
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �I�u�W�F�N�g�������͈͂̍�����W��ݒ�
	DestroyPos = pVtx[nCntVertexIndex].pos;

	// �I�u�W�F�N�g������
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ICEFIELD)
			{// �I�u�W�F�N�g�ւ̃|�C���^������
				IceDestroy((CIceField*)pScene, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ);
			}

			pScene = pSceneNext;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
// �I�u�W�F�N�g��j�����邩�`�F�b�N���鏈��
//=============================================================================
bool CEditor::GameFieldObjDestroyCheck(CObject3D *pObj3D, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	bool bDeath = false;
	D3DXVECTOR3 ObjPos = pObj3D->GetPos();
	if (bSwitchX == true)
	{
		DestroyPos.x -= fDestroyWidth - MASS_SIZE_X - (MASS_SIZE_X / 2);
		if (ObjPos.x < DestroyPos.x + fDestroyWidth && ObjPos.x > DestroyPos.x - MASS_SIZE_X)
		{// X���̔���
			if (bSwitchZ == true)
			{
				DestroyPos.z += fDestroyDepth - MASS_SIZE_Z - (MASS_SIZE_Z / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASS_SIZE_Z)
				{// Z���̔���
					bDeath = true;
				}
			}
			else if (bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z���̔���
					bDeath = true;
				}
			}
		}
	}
	else if (bSwitchX == false)
	{
		if (ObjPos.x < DestroyPos.x + fDestroyWidth && ObjPos.x > DestroyPos.x)
		{// X���̔���
			if (bSwitchZ == true)
			{
				DestroyPos.z += fDestroyDepth - MASS_SIZE_Z - (MASS_SIZE_Z / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASS_SIZE_Z)
				{// Z���̔���
					bDeath = true;
				}
			}
			else if (bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z���̔���
					bDeath = true;
				}
			}
		}
	}

	return bDeath;
}

//=============================================================================
// �I�u�W�F�N�g��j�����邩�`�F�b�N���鏈��(����������)
//=============================================================================
bool CEditor::GameFieldObjDestroyCheck_Half(CObject3D *pObj3D, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	bool bDeath = false;
	D3DXVECTOR3 ObjPos = pObj3D->GetPos();
	if (bSwitchX == true)
	{
		DestroyPos.x -= fDestroyWidth - MASS_SIZE_X_HALF - (MASS_SIZE_X_HALF / 2);
		if (ObjPos.x < DestroyPos.x + fDestroyWidth && ObjPos.x > DestroyPos.x - MASS_SIZE_X_HALF)
		{// X���̔���
			if (bSwitchZ == true)
			{
				DestroyPos.z += fDestroyDepth - MASS_SIZE_Z_HALF - (MASS_SIZE_Z_HALF / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASS_SIZE_Z_HALF)
				{// Z���̔���
					bDeath = true;
				}
			}
			else if(bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z���̔���
					bDeath = true;
				}
			}
		}
	}
	else if(bSwitchX == false)
	{
		if (ObjPos.x < DestroyPos.x + fDestroyWidth && ObjPos.x > DestroyPos.x)
		{// X���̔���
			if (bSwitchZ == true)
			{
				DestroyPos.z += fDestroyDepth - MASS_SIZE_Z_HALF - (MASS_SIZE_Z_HALF / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASS_SIZE_Z_HALF)
				{// Z���̔���
					bDeath = true;
				}
			}
			else if (bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z���̔���
					bDeath = true;
				}
			}
		}
	}

	if (bDeath == true && pObj3D == m_pEditBlock)
	{
		m_pEditBlock = NULL;
	}

	return bDeath;
}

//=============================================================================
// �u���b�N��j�����鏈��
//=============================================================================
void CEditor::BlockDestroy(CBlock *pBlock, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	if (GameFieldObjDestroyCheck_Half(pBlock, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ) == true)
	{// �����͈͓��ł���
		pBlock->Uninit();
		pBlock = NULL;
	}
}

//=============================================================================
// ���j�����鏈��
//=============================================================================
void CEditor::RiverDestroy(CRiver *pRiver, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	if (GameFieldObjDestroyCheck(pRiver, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ) == true)
	{// �����͈͓��ł���
		// �n�ʂ̍�����߂�
		ReturnFieldHeight_FromRiver(pRiver);

		// ���j��
		pRiver->Uninit();
		pRiver = NULL;
	}
}

//=============================================================================
// �X��j�����鏈��
//=============================================================================
void CEditor::IceDestroy(CIceField *pIceField, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	if (GameFieldObjDestroyCheck(pIceField, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ) == true)
	{// �����͈͓��ł���
	    // �n�ʂ̍�����߂�
		ReturnFieldHeight_FromIce(pIceField);

		// �X��j��
		pIceField->Uninit();
		pIceField = NULL;
	}
}

//=============================================================================
// �n�ʂ̍�����߂�����(���)
//=============================================================================
void CEditor::ReturnFieldHeight_FromRiver(CRiver *pRiver)
{
	// ���������擾
	int nXBlock = pRiver->GetMeshField()->GetXBlock();
	int nZBlock = pRiver->GetMeshField()->GetZBlock();

	// ��̍��W����ǂ̃G���A�ɐ삪���邩���擾
	D3DXVECTOR3 RiverPos = pRiver->GetPos();
	int nXArea = (int)((RiverPos.x + (MASS_SIZE_X * MASS_BLOCK_X / 2)) / MASS_SIZE_X) + 1;
	int nZArea = (int)((-RiverPos.z + (MASS_SIZE_Z * MASS_BLOCK_Z / 2)) / MASS_SIZE_Z) + 1;
	int nStartLeftVertex = (nXArea - 1) + ((MASS_BLOCK_X + 1) * (nZArea - 1));

	// ���������l�����Ė߂�
	nStartLeftVertex -= (nXBlock / 2);
	nStartLeftVertex -= ((nZBlock / 2)) * (MASS_BLOCK_X + 1);

	// ���_�o�b�t�@�̎擾
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < nZBlock + 1; nCntZ++)
	{// �c�̕�����+1���J��Ԃ�
		for (int nCntX = 0; nCntX < nXBlock + 1; nCntX++)
		{// ���̕�����+1���J��Ԃ�
			pVtx[nStartLeftVertex + nCntX].pos.y = 0.0f;
		}
		nStartLeftVertex += (MASS_BLOCK_X + 1);
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
// �n�ʂ̍�����߂�����(�X��)
//=============================================================================
void CEditor::ReturnFieldHeight_FromIce(CIceField *pIceField)
{
	// ���������擾
	int nXBlock = pIceField->GetMeshField()->GetXBlock();
	int nZBlock = pIceField->GetMeshField()->GetZBlock();

	// �X�̍��W����ǂ̃G���A�ɐ삪���邩���擾
	D3DXVECTOR3 RiverPos = pIceField->GetPos();
	int nX = (int)((RiverPos.x + (MASS_SIZE_X * MASS_BLOCK_X / 2)) / MASS_SIZE_X) + 1;
	int nZ = (int)((-RiverPos.z + (MASS_SIZE_Z * MASS_BLOCK_Z / 2)) / MASS_SIZE_Z) + 1;
	int nStartLeftVertex = (nX - 1) + ((MASS_BLOCK_X + 1) * (nZ - 1));

	// ���������l�����Ė߂�
	nStartLeftVertex -= (nXBlock / 2);
	nStartLeftVertex -= ((nZBlock / 2)) * (MASS_BLOCK_X + 1);

	// ���_�o�b�t�@�̎擾
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < nZBlock + 1; nCntZ++)
	{// �c�̕�����+1���J��Ԃ�
		for (int nCntX = 0; nCntX < nXBlock + 1; nCntX++)
		{// ���̕�����+1���J��Ԃ�
			pVtx[nStartLeftVertex + nCntX].pos.y = 0.0f;
		}
		nStartLeftVertex += (MASS_BLOCK_X + 1);
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}

//=============================================================================
// �f�[�^��ҏW����u���b�N��T��
//=============================================================================
void CEditor::ShearchEditBlock(void)
{
	if (CManager::GetMouse()->GetTrigger(CInputMouse::DIMS_BUTTON_1) == true)
	{
		CBlock *pBlock = ShearchBlockInArea();
		if (pBlock != NULL)
		{
			m_pEditBlock = pBlock;
		}
		else
		{
			m_pEditBlock = NULL;
		}

		// �킩��₷�������ɂ��Ă���
		if (m_pEditBlock != NULL)
		{
			m_pEditBlock->SetAlpha(0.2f);
		}
	}
}

//=============================================================================
// ���݃}�E�X�������Ă���G���A�ɃI�u�W�F�N�g�����邩���肷��
//=============================================================================
bool CEditor::JudgAreaInObject(CObject3D *pObj3D, D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	bool bCol = false;
	D3DXVECTOR3 ObjPos = pObj3D->GetPos();

	// �͈͓��ɂ��邩����
	if (ObjPos.x < pos.x + fWidth && ObjPos.x > pos.x - fWidth
		&& ObjPos.z < pos.z + fDepth && ObjPos.z > pos.z - fDepth)
	{
		bCol = true;
	}

	return bCol;
}

//=============================================================================
// ���݃}�E�X�������Ă���G���A�Ƀu���b�N�����邩�T��
//=============================================================================
CBlock *CEditor::ShearchBlockInArea(void)
{
	// �������j�����J�n����Ă���ꍇ��̃A�h���X��Ԃ�
	if (CManager::GetMouse()->GetPress(CInputMouse::DIMS_BUTTON_2) == true
		|| CManager::GetMouse()->GetRelease(CInputMouse::DIMS_BUTTON_0) == true && CManager::GetKeyboard()->GetPress(DIK_LALT) == true)
		return NULL;

	// ���݃}�E�X�������Ă���G���A���v�Z
	D3DXVECTOR3 ShearchPos = m_pArea->GetPos();
	float fShearchWidth = MASS_SIZE_X_HALF - 1.0f;
	float fShearchDepth = MASS_SIZE_Z_HALF - 1.0f;

	bool bShearch = false;
	CBlock *pBlock = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		if (bShearch == false)
		{
			pBlock = ShearchBlockInArea_Loop(nCntPriority, &bShearch, ShearchPos, fShearchWidth, fShearchDepth);
		}
	}

	return pBlock;
}

//=============================================================================
// ���݃}�E�X�������Ă���G���A�Ƀu���b�N�����邩�T��(���[�v)
//=============================================================================
CBlock *CEditor::ShearchBlockInArea_Loop(int nCntPriority, bool *pShearch, D3DXVECTOR3 ShearchPos, float fShearchWidth, float fShearchDepth)
{
	float fAlpha = 0.0f;
	CBlock *pBlock = NULL;
	CBlock *pBlockJudg = NULL;
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	pScene = CScene::GetTop(nCntPriority);
	while (pScene != NULL)
	{// �|�C���^����ɂȂ�܂�
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
		{// �u���b�N�N���X������
			pBlockJudg = (CBlock*)pScene;
			pBlockJudg->SetAlpha(1.0f);
			// �͈͓��Ƀu���b�N�����邩����
			if (*pShearch == false)
			{
				pBlock = ShearchBlock(pBlockJudg, pShearch, ShearchPos, fShearchWidth, fShearchDepth);
			}
		}
		pScene = pSceneNext;
	}

	return pBlock;
}

//=============================================================================
// ���݃}�E�X�������Ă���G���A�Ƀu���b�N�����邩����
//=============================================================================
CBlock *CEditor::ShearchBlock(CBlock *pBlock, bool *pShearch, D3DXVECTOR3 ShearchPos, float fShearchWidth, float fShearchDepth)
{
	if (JudgAreaInObject(pBlock, ShearchPos, fShearchWidth, fShearchDepth) == true)
	{// �G���A���ɂ���
		*pShearch = true;
	}
	else
	{// �G���A���ɂ��Ȃ�
		pBlock = NULL;
	}

	return pBlock;
}

//=============================================================================
// �G�f�B�^�[�̃x�[�X�I�u�W�F�N�g�N���X�𐶐����鏈��
//=============================================================================
void CEditor::CreateBaseObject(void)
{
	m_pBaseObject = CBaseObject::Create();
}

//=============================================================================
// �G�f�B�^�[�̃x�[�X�I�u�W�F�N�g�N���X��j�����鏈��
//=============================================================================
void CEditor::ReleaseBaseObject(void)
{
	if (m_pBaseObject != NULL)
	{
		m_pBaseObject->Uninit();
		delete m_pBaseObject;
		m_pBaseObject = NULL;
	}
}

//=============================================================================
// �G�f�B�^�[�̃x�[�X�I�u�W�F�N�g��ύX���鏈��
//=============================================================================
void CEditor::SwitchBaseObject(int nObjMode)
{
	if (m_pBaseObject != NULL)
	{
		m_pBaseObject->SwitchObject(nObjMode);
	}
}

//=============================================================================
// �G�f�B�^�[�̃x�[�X�I�u�W�F�N�g�ɒl��ݒ肷�鏈��
//=============================================================================
void CEditor::SetValueToBaseObject(void)
{
	// ���ݐ�������Ă����ނɂ���ď����킯
	switch (m_ObjMode)
	{
	case CEditor::OBJECTMODE_MODEL:
		m_pBaseObject->SetValueToObject(m_ObjModelPos, m_ObjModelRot);
		break;
	case CEditor::OBJECTMODE_BILLBOARD:
		m_pBaseObject->SetValueToBillboardObj(m_ObjBillPos, m_ObjBillCol, m_fObjBillRot, m_fObjBillWidth, m_fObjBillHeight, m_bObjBillLighting, m_bObjBillDrawAddtive, m_nObjBillTexIdx);
		break;
	case CEditor::OBJECTMODE_EFFECT:
		m_pBaseObject->SetValueToEmitter(m_ObjEffectPos, m_ObjEffectRot);
		break;
	}
}

//=============================================================================
// �z�u���𐶐����鏈��
//=============================================================================
void CEditor::CreateObject(void)
{
	CMap *pMap = GetMap();
	if (pMap == NULL)return;
	pMap->GetObjectManager()[m_nObjModelType]->SetObject(m_ObjModelPos, m_ObjModelRot, m_nObjModelType);
}

//=============================================================================
// �r���{�[�h�I�u�W�F�N�g�𐶐����鏈��
//=============================================================================
void CEditor::CreateBillboardObj(void)
{
	CMap *pMap = GetMap();
	if (pMap == NULL)return;

	CBillboardObject *pBillObj = CBillboardObject::Create(m_ObjBillPos, m_ObjBillCol, m_fObjBillWidth, m_fObjBillHeight,
		m_fObjBillRot, m_bObjBillLighting, m_bObjBillDrawAddtive, m_nObjBillTexIdx);
	if (pBillObj != NULL)
	{
		pBillObj->BindTexture(pMap->GetTextureManager()->GetTexture(m_nObjBillTexIdx));
	}
}

//=============================================================================
// �G�t�F�N�g�𐶐����鏈��
//=============================================================================
void CEditor::CreateEffect(void)
{
	CEffectManager *pEffectManager = GetEffectManager();
	if (pEffectManager == NULL) return;
	pEffectManager->SetEffect(m_ObjEffectPos, m_ObjEffectRot, m_nObjEffectType);
}

//=============================================================================
// �G�f�B�^�[�̕ϐ����N���A����
//=============================================================================
void CEditor::ClearVariable(void)
{
	// �e��ނ��Ƃ̕ϐ����N���A����
	ClearEditorVariable();       // �G�f�B�^�[�p
	ClearBlockVariable();        // �u���b�N�p
	ClearMapVariable();          // �}�b�v�p
	ClearModelListVariable();    // ���f�����X�g�p
	ClearTexListVariable();      // �e�N�X�`�����X�g�p
	ClearLightVariable();        // ���C�g�p
	ClearGameFieldVariable();    // �Q�[���t�B�[���h�p
	ClearObjectVariable();       // �z�u���p
	ClearEnemyListVariable();    // �G�̐������p
	ClearObjModelVariable();     // �z�u���f���p
	ClearObjBillVariable();      // �z�u�r���{�[�h�p
	ClearObjEffectVariable();    // �z�u�G�t�F�N�g�p
}

//=============================================================================
// �G�f�B�^�[�̃G�f�B�^�[�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearEditorVariable(void)
{
	m_pGrid = NULL;
	m_pMeshField = NULL;
	m_pBaseObject = NULL;
	m_bDispGrid = true;
	m_bDispColRange = true;
	m_nLeftVerIdx = 0;
	m_nMassX = 0;
	m_nMassZ = 0;
	m_nBoxSide = 0;
	m_nBoxVertical = 0;
	m_nStartLeftVerIndex = 0;
	m_Mode = MODE_CREATE;
	m_EditMode = EDITMODE_GAMEFIELD;
	m_CameraMode = CAMERAMODE_GAME;
	m_GameFieldMode = GAMEFIELDMODE_BLOCK;
	m_ObjMode = OBJECTMODE_MODEL;
}

//=============================================================================
// �G�f�B�^�[�̃u���b�N�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearBlockVariable(void)
{
	m_pEditBlock = NULL;
	m_nBlockType = 0;
	m_nBlockModelIdx = 0;
	m_BlockRot = INITIALIZE_D3DXVECTOR3;
	m_fBlockColWidth = MASS_SIZE_X_HALF;
	m_fBlockColHeight = MASS_SIZE_Z_HALF;
	m_fBlockColDepth = MASS_SIZE_Z_HALF;
}

//=============================================================================
// �G�f�B�^�[�̐�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearRiverVariable(void)
{
	m_nRiverTexIdx = 0;
}

//=============================================================================
// �G�f�B�^�[�̕X�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearIceVariable(void)
{
	m_nIceTexIdx = 0;
}

//=============================================================================
// �G�f�B�^�[�̔z�u���f���p�ϐ����N���A����
//=============================================================================
void CEditor::ClearObjModelVariable(void)
{
	m_nObjModelType = 0;
	m_ObjModelPos = INITIALIZE_D3DXVECTOR3;
	m_ObjModelRot = INITIALIZE_D3DXVECTOR3;
}

//=============================================================================
// �G�f�B�^�[�̔z�u�r���{�[�h�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearObjBillVariable(void)
{
	m_nObjBillTexIdx = 0;
	m_ObjBillPos = INITIALIZE_D3DXVECTOR3;
	m_ObjBillCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fObjBillRot = 0.0f;
	m_fObjBillWidth = 30.0f;
	m_fObjBillHeight = 30.0f;
	m_bObjBillLighting = true;
	m_bObjBillDrawAddtive = false;
}

//=============================================================================
// �G�f�B�^�[�̔z�u�G�t�F�N�g�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearObjEffectVariable(void)
{
	m_nObjEffectType = 0;
	m_ObjEffectPos = INITIALIZE_D3DXVECTOR3;
	m_ObjEffectRot = INITIALIZE_D3DXVECTOR3;
}

//=============================================================================
// �G�f�B�^�[�̃}�b�v�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearMapVariable(void)
{
	strcpy(m_aMapFileName, "\0");
	strcpy(m_aMapSaveFileName, EDITOR_MAP_SAVEFILENAME_INI);
	strcpy(m_aMapLoadFileName, EDITOR_MAP_SAVEFILENAME_INI);
}

//=============================================================================
// �G�f�B�^�[�̃��f�����X�g�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearModelListVariable(void)
{
	strcpy(m_aModelListSaveFileName, EDITOR_MODELLIST_SAVEFILENAME_INI);
	strcpy(m_aModelListLoadFileName, EDITOR_MODELLIST_SAVEFILENAME_INI);
}

//=============================================================================
// �G�f�B�^�[�̃e�N�X�`�����X�g�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearTexListVariable(void)
{
	strcpy(m_aTexListSaveFileName, EDITOR_TEXLIST_SAVEFILENAME_INI);
	strcpy(m_aTexListLoadFileName, EDITOR_TEXLIST_SAVEFILENAME_INI);
}

//=============================================================================
// �G�f�B�^�[�̃��C�g�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearLightVariable(void)
{
	strcpy(m_aLightSaveFileName, EDITOR_LIGHT_SAVEFILENAME_INI);
	strcpy(m_aLightLoadFileName, EDITOR_LIGHT_SAVEFILENAME_INI);
}

//=============================================================================
// �G�f�B�^�[�̃Q�[���t�B�[���h�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearGameFieldVariable(void)
{
	strcpy(m_aGameFieldSaveFileName, EDITOR_GAMEFILED_SAVEFILENAME_INI);
	strcpy(m_aGameFieldLoadFileName, EDITOR_GAMEFILED_SAVEFILENAME_INI);
}

//=============================================================================
// �G�f�B�^�[�̔z�u���p�ϐ����N���A����
//=============================================================================
void CEditor::ClearObjectVariable(void)
{
	strcpy(m_aObjectSaveFileName, EDITOR_OBJECT_SAVEFILENAME_INI);
	strcpy(m_aObjectLoadFileName, EDITOR_OBJECT_SAVEFILENAME_INI);
}

//=============================================================================
// �G�f�B�^�[�̓G�̐������p�ϐ����N���A����
//=============================================================================
void CEditor::ClearEnemyListVariable(void)
{
	strcpy(m_aEnemyListSaveFileName, EDITOR_ENEMYLIST_SAVEFILENAME_INI);
	strcpy(m_aEnemyListLoadFileName, EDITOR_ENEMYLIST_SAVEFILENAME_INI);
}

//=============================================================================
// �G�f�B�^�[�̃J������������
//=============================================================================
void CEditor::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create(CAMERAMODE_MAX);
	if (pCameraManager != NULL)
	{
		CCamera *pCamera = CCamera::Create();
		if (pCamera != NULL)
		{
			pCameraManager->SetCamera(pCamera, CAMERAMODE_GAME);
		}
		CEditorCamera *pEditCamera = CEditorCamera::Create();
		if (pEditCamera != NULL)
		{
			pCameraManager->SetCamera(pEditCamera, CAMERAMODE_EDIT);
		}
		CTopViewCamera *pTopViewCamera = CTopViewCamera::Create();
		if (pTopViewCamera != NULL)
		{
			pCameraManager->SetCamera(pTopViewCamera, CAMERAMODE_TOP);
		}
		SetCameraManager(pCameraManager);
	}
}

//=============================================================================
// �G�f�B�^�[�̃}�b�v�𐶐�����
//=============================================================================
void CEditor::CreateMap(void)
{
	CMap *pMap = CMap::Create(m_aMapFileName);
	SetMap(pMap);
}

//=============================================================================
// �G�f�B�^�[��UI�𐶐�����
//=============================================================================
void CEditor::CreateUI(void)
{
	m_pUI = CUI::Create();
}

//=============================================================================
// �G�f�B�^�[�̃��X�|�[�����f����ݒ肷��
//=============================================================================
void CEditor::SetRespawnModel(void)
{
	// �v���C���[�̃��X�|�[�����f��
	CModel *pModel = NULL;
	CRespawn *pRespawn = NULL;
	for (int nCntRes = 0; nCntRes < MAX_PLAYER_RESPAWN; nCntRes++)
	{
		pRespawn = GetMap()->GetPlayerRespawn(nCntRes);
		pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, GetModelCreate()->GetMesh(0),
			GetModelCreate()->GetBuffMat(0), GetModelCreate()->GetNumMat(0), GetModelCreate()->GetTexture(0));
		pRespawn->SetModel(pModel);
	}

	// �G�̃��X�|�[�����f��
	for (int nCntRes = 0; nCntRes < MAX_ENEMY_RESPAWN; nCntRes++)
	{
		pRespawn = GetMap()->GetEnemyRespawn(nCntRes);
		pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, GetModelCreate()->GetMesh(1),
			GetModelCreate()->GetBuffMat(1), GetModelCreate()->GetNumMat(1), GetModelCreate()->GetTexture(1));
		pRespawn->SetModel(pModel);
	}
}

//=============================================================================
// �G�f�B�^�[�̃V�X�e������ǂݍ���
//=============================================================================
void CEditor::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(EDITOR_SYSTEM_FILENAME);
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
// �G�f�B�^�[�̃V�X�e�������t�@�C������ǂݍ���
//=============================================================================
void CEditor::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	CModelCreate *pModelCreate = CModelCreate::Create(2);
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, EDITOR_PLAYERRESPAWN_FILENAME) == 0)
		{// �v���C���[�̃��X�|�[�����f���t�@�C����������
			LoadPlayerRespawnModel(pModelCreate, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, EDITOR_ENEMYRESPAWN_FILENAME) == 0)
		{// �G�̃��X�|�[�����f���t�@�C����������
			LoadEnemyRespawnModel(pModelCreate, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_FILENAME) == 0)
		{// �G�t�F�N�g�t�@�C����������
			LoadEffectFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// �}�b�v�t�@�C����������
			LoadMapFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// �X�N���v�g�t�@�C���I���̍��}������
			SetModelCreate(pModelCreate);
			break;
		}
	}
}

//=============================================================================
// �G�f�B�^�[�̃v���C���[�̃��X�|�[�����f���t�@�C������ǂݍ���
//=============================================================================
void CEditor::LoadPlayerRespawnModel(CModelCreate *pModelCreate, char *pStr)
{
	// �t�@�C������ǂݍ���
	strcpy(m_aPlayerRespawnFileName, CFunctionLib::ReadString(pStr, m_aPlayerRespawnFileName, EDITOR_PLAYERRESPAWN_FILENAME));

	// X�t�@�C�����烂�f����ǂݍ���
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat = 0;
	D3DXLoadMeshFromX(m_aPlayerRespawnFileName, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// ���f���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
	pModelCreate->SetMesh(pMesh, 0);
	pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, 0);
	pModelCreate->SetFileName(m_aPlayerRespawnFileName, 0);
}

//=============================================================================
// �G�f�B�^�[�̓G�̃��X�|�[�����f���t�@�C������ǂݍ���
//=============================================================================
void CEditor::LoadEnemyRespawnModel(CModelCreate *pModelCreate, char *pStr)
{
	// �t�@�C������ǂݍ���
	strcpy(m_aEnemyRespawnFileName, CFunctionLib::ReadString(pStr, m_aEnemyRespawnFileName, EDITOR_ENEMYRESPAWN_FILENAME));

	// X�t�@�C�����烂�f����ǂݍ���
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat = 0;
	D3DXLoadMeshFromX(m_aEnemyRespawnFileName, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// ���f���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
	pModelCreate->SetMesh(pMesh, 1);
	pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, 1);
	pModelCreate->SetFileName(m_aEnemyRespawnFileName, 1);
}

//=============================================================================
// �G�f�B�^�[�̓ǂݍ��ރG�t�F�N�g�t�@�C������ǂݍ���
//=============================================================================
void CEditor::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// �G�t�F�N�g�Ǌ��N���X�̐���
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
}

//=============================================================================
// �G�f�B�^�[�̓ǂݍ��ރ}�b�v�t�@�C������ǂݍ���
//=============================================================================
void CEditor::LoadMapFileName(char *pStr)
{
	strcpy(m_aMapFileName, CFunctionLib::ReadString(pStr, m_aMapFileName, MAP_FILENAME));
}

//=============================================================================
// ���[�h��ݒ肷��
//=============================================================================
void CEditor::SetMode(const MODE mode)
{
	m_Mode = mode;
}

//=============================================================================
// �ҏW���[�h��ݒ肷��
//=============================================================================
void CEditor::SetEditMode(const EDITMODE editMode)
{
	m_EditMode = editMode;
}

//=============================================================================
// �J�������[�h��ݒ肷��
//=============================================================================
void CEditor::SetCameraMode(const CAMERAMODE cameraMode)
{
	m_CameraMode = cameraMode;
}

//=============================================================================
// �Q�[���t�B�[���h�ɔz�u����I�u�W�F�N�g�̎�ނ�ݒ肷��
//=============================================================================
void CEditor::SetGameFieldMode(const GAMEFIELDMODE gameFieldMode)
{
	m_GameFieldMode = gameFieldMode;
}

//=============================================================================
// �z�u����I�u�W�F�N�g�̎�ނ�ݒ肷��
//=============================================================================
void CEditor::SetObjectMode(const OBJECTMODE objMode)
{
	m_ObjMode = objMode;
}

//=============================================================================
// �n�ʃN���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CEditor::SetMeshField(CMeshField *pMeshField)
{
	m_pMeshField = pMeshField;
}

//=============================================================================
// �O���b�h����`�悷�邩�ǂ����ݒ肷��
//=============================================================================
void CEditor::SetGridDisp(const bool bDisp)
{
	m_bDispGrid = bDisp;
}

//=============================================================================
// �����蔻��̐���`�悷�邩�ǂ����ݒ肷��
//=============================================================================
void CEditor::SetColRangeDisp(const bool bDisp)
{
	m_bDispColRange = bDisp;
}

//=============================================================================
// �O���b�h����`�悷�邩�ǂ����擾����
//=============================================================================
bool CEditor::GetGridDisp(void)
{
	return m_bDispGrid;
}

//=============================================================================
// �����蔻��̐���`�悷�邩�ǂ����擾����
//=============================================================================
bool CEditor::GetColRangeDisp(void)
{
	return m_bDispColRange;
}

//=============================================================================
// ���݂̉��̕�����
//=============================================================================
int CEditor::GetMassX(void)
{
	return m_nMassX;
}

//=============================================================================
// ���݂̏c�̕�����
//=============================================================================
int CEditor::GetMassZ(void)
{
	return m_nMassZ;
}

//=============================================================================
// �G���A�̏c�̕������擾
//=============================================================================
int CEditor::GetBoxSide(void)
{
	return m_nBoxSide;
}

//=============================================================================
// �G���A�̏c�̕������擾
//=============================================================================
int CEditor::GetBoxVertical(void)
{
	return m_nBoxVertical;
}

//=============================================================================
// �n�ʂ̍��㒸�_�ԍ��擾
//=============================================================================
int CEditor::GetStartLeftVerIndex(void)
{
	return m_nStartLeftVerIndex;
}

//=============================================================================
// ���݂̃��[�h���擾
//=============================================================================
CEditor::MODE CEditor::GetMode(void)
{
	return m_Mode;
}

//=============================================================================
// �ҏW���[�h���擾����
//=============================================================================
CEditor::EDITMODE CEditor::GetEditMode(void)
{
	return m_EditMode;
}

//=============================================================================
// �J�������[�h���擾����
//=============================================================================
CEditor::CAMERAMODE CEditor::GetCameraMode(void)
{
	return m_CameraMode;
}

//=============================================================================
// �Q�[���t�B�[���h�ɔz�u����I�u�W�F�N�g�̎�ނ��擾����
//=============================================================================
CEditor::GAMEFIELDMODE CEditor::GetGameFieldMode(void)
{
	return m_GameFieldMode;
}

//=============================================================================
// �z�u����I�u�W�F�N�g�̎�ނ��擾����
//=============================================================================
CEditor::OBJECTMODE CEditor::GetObjectMode(void)
{
	return m_ObjMode;
}

//=============================================================================
// �x�[�X�I�u�W�F�N�g�N���X�ւ̃|�C���^���擾����
//=============================================================================
CBaseObject *CEditor::GetBaseObject(void)
{
	return m_pBaseObject;
}

//=============================================================================
// �ҏW����u���b�N�N���X�ւ̃|�C���^��ݒ�
//=============================================================================
void CEditor::SetEditBlock(CBlock *pBlock)
{
	m_pEditBlock = pBlock;
}

//=============================================================================
// �u���b�N�̎�ޔԍ���ݒ�
//=============================================================================
void CEditor::SetBlockType(const int nType)
{
	m_nBlockType = nType;
}

//=============================================================================
// �u���b�N�̎g�p���郂�f���̔ԍ���ݒ�
//=============================================================================
void CEditor::SetBlockModelIdx(const int nModelIdx)
{
	m_nBlockModelIdx = nModelIdx;
}

//=============================================================================
// �u���b�N�̌�����ݒ�
//=============================================================================
void CEditor::SetBlockRot(const D3DXVECTOR3 rot)
{
	m_BlockRot = rot;
}

//=============================================================================
// �u���b�N�̎�ޔԍ���ݒ�
//=============================================================================
void CEditor::SetBlockColWidth(const float fWidth)
{
	m_fBlockColWidth = fWidth;
}

//=============================================================================
// �u���b�N�̎�ޔԍ���ݒ�
//=============================================================================
void CEditor::SetBlockColHeight(const float fHeight)
{
	m_fBlockColHeight = fHeight;
}

//=============================================================================
// �u���b�N�̎�ޔԍ���ݒ�
//=============================================================================
void CEditor::SetBlockColDepth(const float fDepth)
{
	m_fBlockColDepth = fDepth;
}

//=============================================================================
// �ҏW����u���b�N�N���X�ւ̃|�C���^���擾
//=============================================================================
CBlock *CEditor::GetEditBlock(void)
{
	return m_pEditBlock;
}

//=============================================================================
// �u���b�N�̎�ޔԍ����擾
//=============================================================================
int CEditor::GetBlockType(void)
{
	return m_nBlockType;
}

//=============================================================================
// �u���b�N�̎g�p���郂�f���̔ԍ����擾
//=============================================================================
int CEditor::GetBlockModelIdx(void)
{
	return m_nBlockModelIdx;
}

//=============================================================================
// �u���b�N�̌������擾
//=============================================================================
D3DXVECTOR3 CEditor::GetBlockRot(void)
{
	return m_BlockRot;
}

//=============================================================================
// �u���b�N�̓����蔻�荂�������擾
//=============================================================================
float CEditor::GetBlockColWidth(void)
{
	return m_fBlockColWidth;
}

//=============================================================================
// �u���b�N�̓����蔻�荂�������擾
//=============================================================================
float CEditor::GetBlockColHeight(void)
{
	return m_fBlockColHeight;
}

//=============================================================================
// �u���b�N�̓����蔻�艜�s�����擾
//=============================================================================
float CEditor::GetBlockColDepth(void)
{
	return m_fBlockColDepth;
}

//=============================================================================
// ��̎g�p����e�N�X�`���̔ԍ���ݒ肷��
//=============================================================================
void CEditor::SetRiverTexIdx(const int nTexIdx)
{
	m_nRiverTexIdx = nTexIdx;
}

//=============================================================================
// ��̎g�p����e�N�X�`���̔ԍ����擾����
//=============================================================================
int CEditor::GetRiverTexIdx(void)
{
	return m_nRiverTexIdx;
}

//=============================================================================
// �X�̎g�p����e�N�X�`���̔ԍ���ݒ肷��
//=============================================================================
void CEditor::SetIceTexIdx(const int nTexIdx)
{
	m_nIceTexIdx = nTexIdx;
}

//=============================================================================
// �X�̎g�p����e�N�X�`���̔ԍ����擾����
//=============================================================================
int CEditor::GetIceTexIdx(void)
{
	return m_nIceTexIdx;
}

//=============================================================================
// �z�u���f���̍��W��ݒ肷��
//=============================================================================
void CEditor::SetObjModelType(const int nType)
{
	m_nObjModelType = nType;
}

//=============================================================================
// �z�u���f���̍��W��ݒ肷��
//=============================================================================
void CEditor::SetObjModelPos(const D3DXVECTOR3 pos)
{
	m_ObjModelPos = pos;
}

//=============================================================================
// �z�u���f���̍��W��ݒ肷��
//=============================================================================
void CEditor::SetObjModelRot(const D3DXVECTOR3 rot)
{
	m_ObjModelRot = rot;
}

//=============================================================================
// �z�u���f���̎�ނ��擾����
//=============================================================================
int CEditor::GetObjModelType(void)
{
	return m_nObjModelType;
}

//=============================================================================
// �z�u���f���̍��W���擾����
//=============================================================================
D3DXVECTOR3 CEditor::GetObjModelPos(void)
{
	return m_ObjModelPos;
}

//=============================================================================
// �z�u���f���̌������擾����
//=============================================================================
D3DXVECTOR3 CEditor::GetObjModelRot(void)
{
	return m_ObjModelRot;
}

//=============================================================================
// �z�u�r���{�[�h�̎g�p����e�N�X�`���̔ԍ����擾����
//=============================================================================
void CEditor::SetObjBillTexIdx(const int nTexIdx)
{
	m_nObjBillTexIdx = nTexIdx;
}

//=============================================================================
// �z�u�r���{�[�h�̍��W���擾����
//=============================================================================
void CEditor::SetObjBillPos(const D3DXVECTOR3 pos)
{
	m_ObjBillPos = pos;
}

//=============================================================================
// �z�u�r���{�[�h�̐F���擾����
//=============================================================================
void CEditor::SetObjBillCol(const D3DXCOLOR col)
{
	m_ObjBillCol = col;
}

//=============================================================================
// �z�u�r���{�[�h�̌������擾����
//=============================================================================
void CEditor::SetObjBillRot(const float fRot)
{
	m_fObjBillRot = fRot;
}

//=============================================================================
// �z�u�r���{�[�h�̕����擾����
//=============================================================================
void CEditor::SetObjBillWidth(const float fWidth)
{
	m_fObjBillWidth = fWidth;
}

//=============================================================================
// �z�u�r���{�[�h�̍������擾����
//=============================================================================
void CEditor::SetObjBillHeight(const float fHeight)
{
	m_fObjBillHeight = fHeight;
}

//=============================================================================
// �z�u�r���{�[�h�̃��C�e�B���O���邩�ǂ������擾����
//=============================================================================
void CEditor::SetObjBillLighting(const bool bLighting)
{
	m_bObjBillLighting = bLighting;
}

//=============================================================================
// �z�u�r���{�[�h�̉��Z�����ŕ`�悷�邩�ǂ������擾����
//=============================================================================
void CEditor::SetObjBillDrawAddtive(const bool bDrawAddtive)
{
	m_bObjBillDrawAddtive = bDrawAddtive;
}

//=============================================================================
// �z�u�r���{�[�h�̎g�p����e�N�X�`���̔ԍ����擾����
//=============================================================================
int CEditor::GetObjBillTexIdx(void)
{
	return m_nObjBillTexIdx;
}

//=============================================================================
// �z�u�r���{�[�h�̍��W���擾����
//=============================================================================
D3DXVECTOR3 CEditor::GetObjBillPos(void)
{
	return m_ObjBillPos;
}

//=============================================================================
// �z�u�r���{�[�h�̐F���擾����
//=============================================================================
D3DXCOLOR CEditor::GetObjBillCol(void)
{
	return m_ObjBillCol;
}

//=============================================================================
// �z�u�r���{�[�h�̌������擾����
//=============================================================================
float CEditor::GetObjBillRot(void)
{
	return m_fObjBillRot;
}

//=============================================================================
// �z�u�r���{�[�h�̕����擾����
//=============================================================================
float CEditor::GetObjBillWidth(void)
{
	return m_fObjBillWidth;
}

//=============================================================================
// �z�u�r���{�[�h�̍������擾����
//=============================================================================
float CEditor::GetObjBillHeight(void)
{
	return m_fObjBillHeight;
}

//=============================================================================
// �z�u�r���{�[�h�̃��C�e�B���O���邩�ǂ������擾����
//=============================================================================
bool CEditor::GetObjBillLighting(void)
{
	return m_bObjBillLighting;
}

//=============================================================================
// �z�u�r���{�[�h�̉��Z�����ŕ`�悷�邩�ǂ������擾����
//=============================================================================
bool CEditor::GetObjBillDrawAddtive(void)
{
	return m_bObjBillDrawAddtive;
}

//=============================================================================
// �z�u�G�t�F�N�g�̎�ނ�ݒ肷��
//=============================================================================
void CEditor::SetObjEffectType(const int nType)
{
	m_nObjEffectType = nType;
}

//=============================================================================
// �z�u�G�t�F�N�g�̍��W��ݒ肷��
//=============================================================================
void CEditor::SetObjEffectPos(const D3DXVECTOR3 pos)
{
	m_ObjEffectPos = pos;
}

//=============================================================================
// �z�u�G�t�F�N�g�̌�����ݒ肷��
//=============================================================================
void CEditor::SetObjEffectRot(const D3DXVECTOR3 rot)
{
	m_ObjEffectRot = rot;
}

//=============================================================================
// �z�u�G�t�F�N�g�̎�ނ��擾����
//=============================================================================
int CEditor::GetObjEffectType(void)
{
	return m_nObjEffectType;
}

//=============================================================================
// �z�u�G�t�F�N�g�̍��W���擾����
//=============================================================================
D3DXVECTOR3 CEditor::GetObjEffectPos(void)
{
	return m_ObjEffectPos;
}

//=============================================================================
// �z�u�G�t�F�N�g�̌������擾����
//=============================================================================
D3DXVECTOR3 CEditor::GetObjEffectRot(void)
{
	return m_ObjEffectRot;
}

//=============================================================================
// �}�b�v����ǂݍ��ރX�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetMapLoadFileName(char *pFileName)
{
	strcpy(m_aMapLoadFileName, pFileName);
}

//=============================================================================
// �}�b�v����ۑ�����X�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetMapSaveFileName(char *pFileName)
{
	strcpy(m_aMapSaveFileName, pFileName);
}

//=============================================================================
// �}�b�v����ǂݍ��ރX�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetMapLoadFileName(void)
{
	return m_aMapLoadFileName;
}

//=============================================================================
// �}�b�v����ۑ�����X�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetMapSaveFileName(void)
{
	return m_aMapSaveFileName;
}

//=============================================================================
// ���f�����X�g����ǂݍ��ރX�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetModelListLoadFileName(char *pFileName)
{
	strcpy(m_aModelListLoadFileName, pFileName);
}

//=============================================================================
// ���f�����X�g����ۑ�����X�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetModelListSaveFileName(char *pFileName)
{
	strcpy(m_aModelListSaveFileName, pFileName);
}

//=============================================================================
// ���f�����X�g����ǂݍ��ރX�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetModelListLoadFileName(void)
{
	return m_aModelListLoadFileName;
}

//=============================================================================
// ���f�����X�g����ۑ�����X�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetModelListSaveFileName(void)
{
	return m_aModelListSaveFileName;
}

//=============================================================================
// �e�N�X�`�����X�g����ǂݍ��ރX�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetTexListLoadFileName(char *pFileName)
{
	strcpy(m_aTexListLoadFileName, pFileName);
}

//=============================================================================
// �e�N�X�`�����X�g����ۑ�����X�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetTexListSaveFileName(char *pFileName)
{
	strcpy(m_aTexListSaveFileName, pFileName);
}

//=============================================================================
// �e�N�X�`�����X�g����ǂݍ��ރX�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetTexListLoadFileName(void)
{
	return m_aTexListLoadFileName;
}

//=============================================================================
// �e�N�X�`�����X�g����ۑ�����X�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetTexListSaveFileName(void)
{
	return m_aTexListSaveFileName;
}

//=============================================================================
// ���C�g����ǂݍ��ރX�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetLightLoadFileName(char *pFileName)
{
	strcpy(m_aLightLoadFileName, pFileName);
}

//=============================================================================
// ���C�g����ۑ�����X�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetLightSaveFileName(char *pFileName)
{
	strcpy(m_aLightSaveFileName, pFileName);
}

//=============================================================================
// ���C�g����ǂݍ��ރX�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetLightLoadFileName(void)
{
	return m_aLightLoadFileName;
}

//=============================================================================
// ���C�g����ۑ�����X�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetLightSaveFileName(void)
{
	return m_aLightSaveFileName;
}

//=============================================================================
// �Q�[���t�B�[���h����ǂݍ��ރX�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetGameFieldLoadFileName(char *pFileName)
{
	strcpy(m_aGameFieldLoadFileName, pFileName);
}

//=============================================================================
// �Q�[���t�B�[���h����ۑ�����X�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetGameFieldSaveFileName(char *pFileName)
{
	strcpy(m_aGameFieldSaveFileName, pFileName);
}

//=============================================================================
// �Q�[���t�B�[���h����ǂݍ��ރX�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetGameFieldLoadFileName(void)
{
	return m_aGameFieldLoadFileName;
}

//=============================================================================
// �Q�[���t�B�[���h����ۑ�����X�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetGameFieldSaveFileName(void)
{
	return m_aGameFieldSaveFileName;
}

//=============================================================================
// �z�u������ۑ�����X�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetObjectLoadFileName(char *pFileName)
{
	strcpy(m_aObjectLoadFileName, pFileName);
}

//=============================================================================
// �z�u������ǂݍ��ރX�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetObjectSaveFileName(char *pFileName)
{
	strcpy(m_aObjectSaveFileName, pFileName);
}

//=============================================================================
// �z�u������ۑ�����X�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetObjectLoadFileName(void)
{
	return m_aObjectLoadFileName;
}

//=============================================================================
// �z�u������ǂݍ��ރX�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetObjectSaveFileName(void)
{
	return m_aObjectSaveFileName;
}

//=============================================================================
// �G�̐�������ǂݍ��ރX�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetEnemyListLoadFileName(char *pFileName)
{
	strcpy(m_aEnemyListLoadFileName, pFileName);
}

//=============================================================================
// �G�̐�������ۑ�����X�N���v�g�t�@�C��������ݒ�
//=============================================================================
void CEditor::SetEnemyListSaveFileName(char *pFileName)
{
	strcpy(m_aEnemyListSaveFileName, pFileName);
}

//=============================================================================
// �G�̐�������ǂݍ��ރX�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetEnemyListLoadFileName(void)
{
	return m_aEnemyListLoadFileName;
}

//=============================================================================
// �G�̐�������ۑ�����X�N���v�g�t�@�C���������擾
//=============================================================================
char *CEditor::GetEnemyListSaveFileName(void)
{
	return m_aEnemyListSaveFileName;
}



//*****************************************************************************
// CBaseObject�̏���
//*****************************************************************************
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBaseObject::CBaseObject()
{
	// �e��l�̃N���A
	m_pObject = NULL;
	m_pBillboardObj = NULL;
	m_pEmitter = NULL;
	m_pBill = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBaseObject::~CBaseObject()
{

}

//=============================================================================
// ��������
//=============================================================================
CBaseObject *CBaseObject::Create(void)
{
	CBaseObject *pBaseObject = NULL;
	if (pBaseObject == NULL)
	{
		pBaseObject = new CBaseObject;
		if (pBaseObject != NULL)
		{
			pBaseObject->Init();
		}
	}
	return pBaseObject;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBaseObject::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBaseObject::Uninit(void)
{
	// �z�u���N���X�̊J��
	if (m_pObject != NULL)
	{
		m_pObject->Uninit();
		m_pObject = NULL;
	}

	// �r���{�[�h�I�u�W�F�N�g�N���X�̊J��
	if (m_pBillboardObj != NULL)
	{
		m_pBillboardObj->Uninit();
		m_pBillboardObj = NULL;
	}

	// �G�~�b�^�N���X�̊J��
	if (m_pEmitter != NULL)
	{
		m_pEmitter->Uninit();
		m_pEmitter = NULL;
	}
	if (m_pBill != NULL)
	{
		m_pBill->Uninit();
		m_pBill = NULL;
	}
}

//=============================================================================
// �x�[�X�I�u�W�F�N�g��؂�ւ��鏈��
//=============================================================================
void CBaseObject::SwitchObject(int nObjMode)
{
	// �f�t�H���g�̃I�u�W�F�N�g��j������
	Uninit();

	// �I�u�W�F�N�g�𐶐����Ȃ���
	switch (nObjMode)
	{
	case CEditor::OBJECTMODE_MODEL:
		CreateObject();
		break;
	case CEditor::OBJECTMODE_BILLBOARD:
		CreateBillboardObj();
		break;
	case CEditor::OBJECTMODE_EFFECT:
		CreateEmitter();
		break;
	}
}

//=============================================================================
// �z�u���N���X�𐶐�����
//=============================================================================
void CBaseObject::CreateObject(void)
{
	// �G�f�B�^�[�N���X���擾
	CEditor *pEditor = CManager::GetEditor();
	if (pEditor == NULL) return;

	// �G�f�B�^�[����l���擾
	int nObjModelType = pEditor->GetObjModelType();
	D3DXVECTOR3 ObjModelPos = pEditor->GetObjModelPos();
	D3DXVECTOR3 ObjModelRot = pEditor->GetObjModelRot();

	// �}�b�v�N���X���擾
	CMap *pMap = CManager::GetEditor()->GetMap();
	if (pMap == NULL) return;

	// �L�����N�^�[�Ǌ��N���X���g���Đ������|�C���^���擾
	m_pObject = pMap->GetObjectManager()[nObjModelType]->SetObject(ObjModelPos, ObjModelRot, nObjModelType);
	if (m_pObject != NULL)
	{
		m_pObject->SetAlpha(0.3f);
	}
}

//=============================================================================
// �r���{�[�h�I�u�W�F�N�g�𐶐�����
//=============================================================================
void CBaseObject::CreateBillboardObj(void)
{
	// �G�f�B�^�[�N���X���擾
	CEditor *pEditor = CManager::GetEditor();
	if (pEditor == NULL) return;

	// �G�f�B�^�[����l���擾
	int nObjBillTexIdx = pEditor->GetObjEffectType();
	D3DXVECTOR3 ObjBillPos = pEditor->GetObjEffectPos();
	D3DXCOLOR ObjBillCol = pEditor->GetObjBillCol();
	float fObjBillRot = pEditor->GetObjBillRot();
	float fObjBillWidth = pEditor->GetObjBillWidth();
	float fObjBillHeight = pEditor->GetObjBillHeight();
	bool bObjBillLighting = pEditor->GetObjBillLighting();
	bool bObjBillDrawAddtive = pEditor->GetObjBillDrawAddtive();

	// �I�u�W�F�N�g�𐶐�
	m_pBillboardObj = CBillboardObject::Create(ObjBillPos, ObjBillCol, fObjBillWidth, fObjBillHeight,
		fObjBillRot, bObjBillLighting, bObjBillDrawAddtive, nObjBillTexIdx);
	if (m_pBillboardObj != NULL)
	{
		m_pBillboardObj->BindTexture(pEditor->GetMap()->GetTextureManager()->GetTexture(nObjBillTexIdx));
	}

	// �G�t�F�N�g�̍��W�\���p�|���S���𐶐�
	m_pBill = CSceneBillboard::Create(ObjBillPos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, 8.0f, 8.0f);
}

//=============================================================================
// �G�~�b�^�N���X�𐶐�����
//=============================================================================
void CBaseObject::CreateEmitter(void)
{
	// �G�f�B�^�[�N���X���擾
	CEditor *pEditor = CManager::GetEditor();
	if (pEditor == NULL) return;

	// �G�f�B�^�[����l���擾
	int nObjEffectType = pEditor->GetObjEffectType();
	D3DXVECTOR3 ObjEffectPos = pEditor->GetObjEffectPos();
	D3DXVECTOR3 ObjEffectRot = pEditor->GetObjEffectRot();

	// �G�t�F�N�g�Ǌ��N���X���琶�����|�C���^���擾
	m_pEmitter = pEditor->GetEffectManager()->SetEffect(ObjEffectPos, ObjEffectRot, nObjEffectType);

	// �G�t�F�N�g�̍��W�\���p�|���S���𐶐�
	m_pBill = CSceneBillboard::Create(ObjEffectPos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, 8.0f, 8.0f, false, EFFECT_PRIORITY + 1);
}

//=============================================================================
// �z�u���N���X�ɒl��ݒ肷�鏈��
//=============================================================================
void CBaseObject::SetValueToObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pObject == NULL)return;
	m_pObject->SetPos(pos);
	m_pObject->SetRot(rot);

}

//=============================================================================
// �r���{�[�h�I�u�W�F�N�g�N���X�ɒl��ݒ肷�鏈��
//=============================================================================
void CBaseObject::SetValueToBillboardObj(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, bool bLighting, bool bDrawAddtive, int nTexIdx)
{
	if (m_pBillboardObj == NULL)return;
	m_pBillboardObj->SetPos(pos);
	m_pBillboardObj->SetCol(col);
	m_pBillboardObj->SetRot(fRot);
	m_pBillboardObj->SetWidth(fWidth);
	m_pBillboardObj->SetHeight(fHeight);
	m_pBillboardObj->SetLighting(bLighting);
	m_pBillboardObj->SetDrawAddtive(bDrawAddtive);
	m_pBillboardObj->SetTexIdx(nTexIdx);
	m_pBillboardObj->SetVtxBuffValue();

	if (m_pBill == NULL)return;
	m_pBill->SetPos(pos);
}

//=============================================================================
// �G�~�b�^�N���X�ɒl��ݒ肷�鏈��
//=============================================================================
void CBaseObject::SetValueToEmitter(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pEmitter == NULL)return;
	m_pEmitter->SetPos(pos);
	m_pEmitter->SetRot(rot);

	if (m_pBill == NULL)return;
	m_pBill->SetPos(pos);
}

//=============================================================================
// �z�u���N���X�ւ̃|�C���^���擾����
//=============================================================================
CObject *CBaseObject::GetObj(void)
{
	return m_pObject;
}

//=============================================================================
// �r���{�[�h�I�u�W�F�N�g�N���X�ւ̃|�C���^���擾����
//=============================================================================
CBillboardObject *CBaseObject::GetBillboardObj(void)
{
	return m_pBillboardObj;
}

//=============================================================================
// �G�~�b�^�N���X�ւ̃|�C���^���擾����
//=============================================================================
CEmitter *CBaseObject::GetEmitter(void)
{
	return m_pEmitter;
}