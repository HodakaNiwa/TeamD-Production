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
#include "debugproc.h"
#include "fade.h"
#include "map.h"
#include "modelcreate.h"
#include "grid.h"
#include "block.h"
#include "camera.h"
#include "cameraManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "model.h"
#include "respawn.h"

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
#define EDITOR_PLAYERRESPAWN_FILENAME     "PLAYERRESPAWN_MODEL = "  // �v���C���[�̃��X�|�[�����f���̃t�@�C����
#define EDITOR_ENEMYRESPAWN_FILENAME      "ENEMYRESPAWN_MODEL = "   // �G�̃��X�|�[�����f���̃t�@�C����

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
	m_pMeshField = NULL;	//���b�V���t�B�[���h��������
}

//=============================================================================
// �G�f�B�^�[�̃f�X�g���N�^
//=============================================================================
CEditor *CEditor::Create()
{
	CEditor *pEditor = NULL;	//�G�f�B�^�[�̃|�C���^
	if (pEditor == NULL)
	{
		pEditor = new CEditor;	//���I�m��
		if (pEditor != NULL)
		{
			pEditor->Init();	//����������
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

	// �J�����𐶐�����
	CreateCamera();

	//�}�b�v�𐶐�
	CreateMap();

	// �n�ʃN���X�ւ̃|�C���^���擾
	m_pMeshField = GetMap()->GetMeshField();

	// �O���b�h���𐶐�
	m_pGrid = CGrid::Create(D3DXVECTOR3(0.0f, 1.0f, 0.0f), MASU_SIZE_X_HALF, MASU_SIZE_Z_HALF, MASU_BLOCK_X * 2, MASU_BLOCK_Z * 2);

	// ���X�|�[�����f����ݒ�
	SetRespawnModel();

	return S_OK;
}

//=============================================================================
// �G�f�B�^�[�̏I������
//=============================================================================
void CEditor::Uninit(void)
{
	// ���ʂ̏I������
	CBasemode::Uninit();

	// �O���b�h���̔j��
	if (m_pGrid != NULL)
	{
		m_pGrid->Uninit();
		delete m_pGrid;
		m_pGrid = NULL;
	}

	//�S�ẴI�u�W�F�N�g�J��
	CScene::ReleaseAll();
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
	// �J�����̐ݒ�
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera(m_CameraMode);
	}

	// �V�[���N���X�̕`�揈��
	CScene::DrawAll();

	// �O���b�h���̍X�V
	if (m_pGrid != NULL)
	{
		m_pGrid->Draw();
	}
}

//=============================================================================
// �n�ʂɃ}�E�X�̃��C���ђʂ��Ă��邩�`�F�b�N���鏈��
//=============================================================================
void CEditor::CheckFieldPenetration(void)
{
	// �t�B�[���h�Ƀ}�E�X�̃��C���h�����Ă��邩�`�F�b�N
	int nMasuX = 0, nMasuZ = 0;
	int nLeftVerIndex = 0;
	D3DXVECTOR3 MouseWorldPos1 = CManager::GetMouse()->GetWorldPosZn();
	D3DXVECTOR3 MouseWorldPos2 = CManager::GetMouse()->GetWorldPosZf();
	D3DXVECTOR3 PolyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 PolyNor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 Nor = MouseWorldPos2 - MouseWorldPos1;
	D3DXVec3Normalize(&Nor, &Nor);
	if (Nor.y <= 0.0f && m_pMeshField != NULL)
	{// �n�ʂɌ������ă��C������Ă��� && �n�ʂ���������Ă���
		D3DXVECTOR3 PenePos = MouseWorldPos1 + (D3DXVec3Dot(&-MouseWorldPos1, &PolyNor) / D3DXVec3Dot(&Nor, &PolyNor) * Nor);
		if (m_pMeshField->CheckPenetration(PenePos, &nMasuX, &nMasuZ, &nLeftVerIndex) == true)
		{// �n�ʂɃ��C���ђʂ��Ă���
			PenetrationFieldArea(nMasuX, nMasuZ, nLeftVerIndex);
			FillinFieldBox(nLeftVerIndex);
		}
	}
}

//=============================================================================
// �}�E�X���G��Ă���t�B�[���h�̃G���A���Z�o���鏈��
//=============================================================================
void CEditor::PenetrationFieldArea(int nMasuX, int nMasuZ, int nLeftVerIndex)
{
	CInputMouse *pMouse = CManager::GetMouse();
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_2) == true)
	{// �}�E�X�̒��N���b�N�������ꂽ
		// �����ꂽ�Ƃ��̃}�X�ڂ��L�^
		m_nMasuX = nMasuX;
		m_nMasuZ = nMasuZ;
		m_nStartLeftVerIndex = nLeftVerIndex;
	}

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_2) == true)
	{// �}�E�X�̒��N���b�N��������Ă���
		// ���̕��������v�Z
		int nSwitchX = 0;
		bool bSwitchX = false;
		nSwitchX = m_nMasuX - nMasuX;
		if (nSwitchX > 0)
		{
			bSwitchX = true;
		}

		// �c�̕��������v�Z
		int nSwitchZ = 0;
		bool bSwitchZ = false;
		nSwitchZ = m_nMasuZ - nMasuZ;
		if (nSwitchZ > 0)
		{
			bSwitchZ = true;
		}

		// �G���A�̕��������c�����ꂼ��v�Z
		m_nBoxSide = abs(m_nMasuX - nMasuX) + 1;
		m_nBoxVertical = abs(m_nMasuZ - nMasuZ) + 1;

		// �w�肳�ꂽ�G���A��h��Ԃ�����
		FillinFieldArea(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}

	if (pMouse->GetRelease(CInputMouse::DIMS_BUTTON_2) == true)
	{// �}�E�X�̒��N���b�N�������ꂽ
		// �ŏI�I�ȕ�������ۑ�
		m_nBoxSide = abs(m_nMasuX - nMasuX) + 1;
		m_nBoxVertical = abs(m_nMasuZ - nMasuZ) + 1;

		// ���̕��������v�Z
		int nSwitchX = 0;
		bool bSwitchX = false;
		nSwitchX = m_nMasuX - nMasuX;
		if (nSwitchX > 0)
		{
			bSwitchX = true;
		}

		// �c�̕��������v�Z
		int nSwitchZ = 0;
		bool bSwitchZ = false;
		nSwitchZ = m_nMasuZ - nMasuZ;
		if (nSwitchZ > 0)
		{
			bSwitchZ = true;
		}

		// �I�u�W�F�N�g��z�u OR �I�u�W�F�N�g��j��
		if (m_Mode == MODE_CREATE)
		{
			BlockCreate(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
		}
		else if(m_Mode == MODE_DESTROY)
		{
			BlockDestroy(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
		}
	}
}

//=============================================================================
// �w�肳�ꂽ�n�ʂ̃G���A��h��Ԃ�����
//=============================================================================
void CEditor::FillinFieldArea(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// ���_���̐ݒ�
	int nXBlock = m_pMeshField->GetXBlock();
	int nZBlock = m_pMeshField->GetZBlock();
	int nCntVertexIndex = m_nStartLeftVerIndex;
	int nData = nCntVertexIndex;
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = m_nMasuX; nCntV < (m_nMasuX + m_nBoxVertical); nCntV++)
	{// ���s�̕��������J��Ԃ�
		for (int nCntH = m_nMasuZ; nCntH < (m_nMasuZ + m_nBoxSide); nCntH++)
		{// ���̕��������J��Ԃ�
			pVtx[nCntVertexIndex].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[nCntVertexIndex + 1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[nCntVertexIndex + (nXBlock + 1) + 1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[nCntVertexIndex + (nXBlock + 1)].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

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
// �}�E�X���G��Ă���G���A��h��Ԃ�����
//=============================================================================
void CEditor::FillinFieldBox(int nLeftVerIndex)
{
	CInputMouse *pMouse = CManager::GetMouse();
	int nXBlock = m_pMeshField->GetXBlock();

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_2) == false)
	{// �}�E�X�̒��N���b�N��������Ă��Ȃ�
		VERTEX_3D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[nLeftVerIndex].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[nLeftVerIndex + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[nLeftVerIndex + (nXBlock + 1) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[nLeftVerIndex + (nXBlock + 1)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
// �w�肳�ꂽ�G���A�ɃI�u�W�F�N�g��z�u���鏈��
//=============================================================================
void CEditor::BlockCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	CModelCreate *pModelCreate = GetMap()->GetModelCreate();  // ���f���Ǌ��N���X
	CBlock *pBlock = NULL;                                    // �I�u�W�F�N�g�N���X�ւ̃|�C���^
	D3DXVECTOR3 ObjPos;                                       // �I�u�W�F�N�g��z�u����ʒu
	D3DXVECTOR3 FieldPos = m_pMeshField->GetPos();            // �n�ʂ̍��W
	int nXBlock = m_pMeshField->GetXBlock();                  // �n�ʂ̉��̕�����
	int nZBlock = m_pMeshField->GetZBlock();                  // �n�ʂ̏c�̕�����
	int nCntVertexIndex = m_nStartLeftVerIndex;               // ���݂̍���̒��_�ԍ�
	int nData = nCntVertexIndex;                              // ���_�ԍ��ۑ��p
	int nSelect = m_nBlockModelIdx;                           // �z�u���郂�f���̔ԍ�

	// ���_�o�b�t�@�̎擾
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = m_nMasuX; nCntV < (m_nMasuX + m_nBoxVertical); nCntV++)
	{// ���s�̕��������J��Ԃ�
		for (int nCntH = m_nMasuZ; nCntH < (m_nMasuZ + m_nBoxSide); nCntH++)
		{// ���̕��������J��Ԃ�
			// �I�u�W�F�N�g��z�u����ʒu���v�Z
			ObjPos = pVtx[nCntVertexIndex].pos;
			ObjPos.x += (MASU_SIZE_X_HALF / 2);
			ObjPos.y = 0.0f;
			ObjPos.z -= (MASU_SIZE_Z_HALF / 2);

			// �I�u�W�F�N�g��z�u����
			pBlock = CBlock::Create(ObjPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), (CBlock::TYPE)m_nBlockType, m_nBlockModelIdx,
				m_bBlockBreak, pModelCreate->GetMesh(m_nBlockModelIdx), pModelCreate->GetBuffMat(m_nBlockModelIdx),
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
// �w�肳�ꂽ�G���A�̃I�u�W�F�N�g��j�����鏈��
//=============================================================================
void CEditor::BlockDestroy(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	CBlock *pBlock = NULL;                              // �u���b�N�N���X�ւ̃|�C���^
	D3DXVECTOR3 DestroyPos;                             // �I�u�W�F�N�g������������W
	int nCntVertexIndex = m_nStartLeftVerIndex;         // ���݂̍���̒��_�ԍ�
	int nData = nCntVertexIndex;                        // ���_�ԍ��ۑ��p

    // �����͈͂��Ɍv�Z
	float fDestroyWidth = (float)(m_nBoxSide * MASU_SIZE_X_HALF);
	float fDestroyDepth = (float)(m_nBoxVertical * MASU_SIZE_Z_HALF);

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
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// �I�u�W�F�N�g�ւ̃|�C���^������
				pBlock = (CBlock*)pScene;
				BlockDestroyCheck(pBlock, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ);
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
void CEditor::BlockDestroyCheck(CBlock *pBlock, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	D3DXVECTOR3 ObjPos = pBlock->GetPos();
	if (bSwitchX == true)
	{
		DestroyPos.x -= fDestroyWidth - MASU_SIZE_X_HALF - (MASU_SIZE_X_HALF / 2);
		if (ObjPos.x < DestroyPos.x + fDestroyWidth && ObjPos.x > DestroyPos.x - MASU_SIZE_X_HALF)
		{// X���̔���
			if (bSwitchZ == true)
			{
				DestroyPos.z += fDestroyDepth - MASU_SIZE_Z_HALF - (MASU_SIZE_Z_HALF / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASU_SIZE_Z_HALF)
				{// Z���̔���
					pBlock->Uninit();
					pBlock = NULL;
				}
			}
			else if(bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z���̔���
					pBlock->Uninit();
					pBlock = NULL;
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
				DestroyPos.z += fDestroyDepth - MASU_SIZE_Z_HALF - (MASU_SIZE_Z_HALF / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASU_SIZE_Z_HALF)
				{// Z���̔���
					pBlock->Uninit();
					pBlock = NULL;
				}
			}
			else if (bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z���̔���
					pBlock->Uninit();
					pBlock = NULL;
				}
			}
		}
	}
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
}

//=============================================================================
// �G�f�B�^�[�̃G�f�B�^�[�p�ϐ����N���A����
//=============================================================================
void CEditor::ClearEditorVariable(void)
{
	m_pGrid = NULL;
	m_pMeshField = NULL;
	m_nMasuX = 0;
	m_nMasuZ = 0;
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
	m_nBlockType = 0;
	m_nBlockModelIdx = 0;
	m_bBlockBreak = true;
	m_fBlockColWidth = MASU_SIZE_X_HALF;
	m_fBlockColHeight = MASU_SIZE_Z_HALF;
	m_fBlockColDepth = MASU_SIZE_Z_HALF;
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
// �G�f�B�^�[�̓ǂݍ��ރ}�b�v�t�@�C������ǂݍ���
//=============================================================================
void CEditor::LoadMapFileName(char *pStr)
{
	strcpy(m_aMapFileName, CFunctionLib::ReadString(pStr, m_aMapFileName, MAP_FILENAME));
}

//=============================================================================
// �G�f�B�^�[�̃V�X�e������ۑ�����
//=============================================================================
void CEditor::SaveSystem(void)
{
	char aStr[256] = "\0";
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(EDITOR_SYSTEM_FILENAME);
	if (pFileSaver != NULL)
	{
		// �t�@�C���̖`��������������
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# �G�f�B�^�[�V�X�e���t�@�C�� [editor.ini]\n");
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# ���̍s�͐�Ώ����Ȃ����ƁI\n", SCRIPT);
		pFileSaver->Print("\n");

		// �v���C���[�̃��X�|�[�����f���t�@�C��������������
		SavePlayerRespawnModel(pFileSaver);

		// �G�̃��X�|�[�����f���t�@�C��������������
		SaveEnemyRespawnModel(pFileSaver);

		// �ǂݍ��ރ}�b�v�̃t�@�C��������������
		SaveMapFileName(pFileSaver);

		// �X�N���v�g�I���̍��}����������
		pFileSaver->Print("%s		# ���̍s�͐�Ώ����Ȃ����ƁI\n", END_SCRIPT);

		// �������̊J��
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}
}

//=============================================================================
// �G�f�B�^�[�̃v���C���[�̃��X�|�[�����f���t�@�C������ۑ�����
//=============================================================================
void CEditor::SavePlayerRespawnModel(CFileSaver *pFileSaver)
{
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �v���C���[�̃��X�|�[�����f���t�@�C����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("%s%s", EDITOR_PLAYERRESPAWN_FILENAME, m_aPlayerRespawnFileName);
	pFileSaver->Print("\n");
}

//=============================================================================
// �G�f�B�^�[�̓G�̃��X�|�[�����f���t�@�C������ۑ�����
//=============================================================================
void CEditor::SaveEnemyRespawnModel(CFileSaver *pFileSaver)
{
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �G�̃��X�|�[�����f���t�@�C����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("%s%s", EDITOR_ENEMYRESPAWN_FILENAME, m_aEnemyRespawnFileName);
	pFileSaver->Print("\n");
}

//=============================================================================
// �G�f�B�^�[�̓ǂݍ��ރ}�b�v�t�@�C������ۑ�����
//=============================================================================
void CEditor::SaveMapFileName(CFileSaver *pFileSaver)
{
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# �ǂݍ��ރ}�b�v�̃t�@�C����\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("%s%s", MAP_FILENAME, m_aMapFileName);
	pFileSaver->Print("\n");
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
// ���݂̉��̕�����
//=============================================================================
int CEditor::GetMasuX(void)
{
	return m_nMasuX;
}

//=============================================================================
// ���݂̏c�̕�����
//=============================================================================
int CEditor::GetMasuZ(void)
{
	return m_nMasuZ;
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
// �u���b�N�̎�ޔԍ���ݒ�
//=============================================================================
void CEditor::SetBlockBreak(const bool bBreak)
{
	m_bBlockBreak = bBreak;
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
// �u���b�N���󂹂邩�ǂ����擾
//=============================================================================
bool CEditor::GetBlockBreak(void)
{
	return m_bBlockBreak;
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