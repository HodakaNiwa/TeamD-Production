//*****************************************************************************
//
//     �u���b�N�̏���[block.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "block.h"
#include "manager.h"
#include "boxCollider.h"
#include "renderer.h"
#include "splash.h"
#include "basemode.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ�
//*****************************************************************************


//*****************************************************************************
//    CBlock�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlock::CBlock(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	// �e��l�̃N���A
	m_pMesh = NULL;                      // ���b�V���ւ̃|�C���^
	m_pBuffMat = NULL;                   // �}�e���A�����ւ̃|�C���^
	m_nNumMat = 0;                       // �}�e���A�����̐�
	m_VtxMax = INITIALIZE_D3DXVECTOR3;   // �ő�̒��_���W
	m_VtxMin = INITIALIZE_D3DXVECTOR3;   // �ŏ��̒��_���W
	m_fAlpha = 0.0f;                     // ���f���̓����x
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlock::~CBlock()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlock *pBlock = NULL;      // �u���b�N�N���X�^�̃|�C���^
	if (pBlock == NULL)
	{// ����������ɂȂ��Ă���
		pBlock = new CBlock(nPriority);
		if (pBlock != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pBlock->SetPos(pos);                                     // ���W
			pBlock->SetRot(rot);                                     // ����
			pBlock->SetType(nType);                                  // ��ޔԍ�
			pBlock->SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
			pBlock->SetAlpha(1.0f);                                  // ���f���̓����x
			pBlock->BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

			// �����蔻��p�����f�����쐬
			pBlock->SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
			pBlock->CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

			if (FAILED(pBlock->Init()))
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

	return pBlock;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlock::Init(void)
{
	// ��ނ̐ݒu����
	SetObjType(OBJTYPE_BLOCK);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlock::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlock::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlock::Draw(void)
{
	D3DMATERIAL9 matDef;     // ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;      // �}�e���A���f�[�^�ւ̃|�C���^
	float fAlphaDef = 0.0f;  // �f�t�H���g�̓����x

	//�����_�����O�̎擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���[���h�}�g���b�N�X�ݒ菈��
			SetMtxWorld(pDevice);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			if (m_pBuffMat != NULL && m_pMesh != NULL)
			{// X�t�@�C�����烂�f���f�[�^���ǂݍ��߂Ă���
			    // �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
				{// �ݒ肳��Ă����}�e���A���̐������J��Ԃ�
				    // �����x�̐ݒ�
					fAlphaDef = pMat[nCntMat].MatD3D.Diffuse.a;
					pMat[nCntMat].MatD3D.Diffuse.a = m_fAlpha;

					// �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					if (m_pTexture != NULL)
					{// �e�N�X�`����񂪈����o���Ă���
					    // �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, m_pTexture[nCntMat]);
					}
					else
					{// �e�N�X�`����񂪈����o���Ă��Ȃ�
					    // �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, NULL);
					}

					// ���f��(�p�[�c)�̕`��
					m_pMesh->DrawSubset(nCntMat);

					// �����x��߂�
					pMat[nCntMat].MatD3D.Diffuse.a = fAlphaDef;
				}
			}

			// �ۑ����Ă����}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}

	// 3D�I�u�W�F�N�g�̕`��
	CObject3D::Draw();
}

//=============================================================================
//    �����蔻��p�����f�����쐬���鏈��
//=============================================================================
void CBlock::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, bReturnFlag);
	SetBoxCollider(pBoxCollider);
}


//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlock::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    �I�u�W�F�N�g�ɓ��������Ƃ��̏���
//=============================================================================
void CBlock::Hit(CScene *pScene)
{
	Uninit();
}

//=============================================================================
//    ���f�����ݒ菈��
//=============================================================================
void CBlock::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	m_pMesh = pMesh;
	m_pBuffMat = pBuffMat;
	m_nNumMat = nNumMat;
	m_pTexture = pTexture;
	m_VtxMax = VtxMax;
	m_VtxMin = VtxMin;
}

//=============================================================================
//    ��ޔԍ��ݒ菈��
//=============================================================================
void CBlock::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    �g�p���郂�f���̔ԍ��ݒ菈��
//=============================================================================
void CBlock::SetModelIdx(const int nModelIdx)
{
	m_nModelIdx = nModelIdx;
}

//=============================================================================
//    ���b�V���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CBlock::SetMesh(const LPD3DXMESH pMesh)
{
	m_pMesh = pMesh;
}

//=============================================================================
//    �}�e���A�����ւ̃|�C���^�ݒ菈��
//=============================================================================
void CBlock::SetBuffMat(const LPD3DXBUFFER pBuffMat)
{
	m_pBuffMat = pBuffMat;
}

//=============================================================================
//    �}�e���A�����̐��ݒ菈��
//=============================================================================
void CBlock::SetNumMat(const DWORD nNumMat)
{
	m_nNumMat = nNumMat;
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CBlock::SetTexture(LPDIRECT3DTEXTURE9 *pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    �ő�̒��_���W�ݒ菈��
//=============================================================================
void CBlock::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    �ŏ��̒��_���W�ݒ菈��
//=============================================================================
void CBlock::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    ���f���̓����x�ݒ菈��
//=============================================================================
void CBlock::SetAlpha(const float fAlpha)
{
	m_fAlpha = fAlpha;
}

//=============================================================================
//    ��ޔԍ��擾����
//=============================================================================
int CBlock::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    �g�p���郂�f���̔ԍ��擾����
//=============================================================================
int CBlock::GetModelIdx(void)
{
	return m_nModelIdx;
}

//=============================================================================
//    ���b�V���ւ̃|�C���^�擾����
//=============================================================================
LPD3DXMESH CBlock::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    �}�e���A�����ւ̃|�C���^�擾����
//=============================================================================
LPD3DXBUFFER CBlock::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    �}�e���A�����̐��擾����
//=============================================================================
DWORD CBlock::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�擾����
//=============================================================================
LPDIRECT3DTEXTURE9 *CBlock::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    �ő�̒��_���W�擾����
//=============================================================================
D3DXVECTOR3 CBlock::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    �ŏ��̒��_���W�擾����
//=============================================================================
D3DXVECTOR3 CBlock::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    ���f���̓����x�擾����
//=============================================================================
float CBlock::GetAlpha(void)
{
	return m_fAlpha;
}

//*****************************************************************************
//    CBlockType0�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlockType0::CBlockType0(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlockType0::~CBlockType0()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlockType0 *CBlockType0::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType0 *pBlockType0 = NULL;      // �^�C�v0�̃u���b�N�N���X�^�̃|�C���^
	if (pBlockType0 == NULL)
	{// ����������ɂȂ��Ă���
		pBlockType0 = new CBlockType0(nPriority);
		if (pBlockType0 != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pBlockType0->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType0;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlockType0::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// �e��l�̐ݒ�
	SetPos(pos);                                     // ���W
	SetRot(rot);                                     // ����
	SetType(nType);                                  // ��ޔԍ�
	SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
	SetAlpha(1.0f);                                  // ���f���̓����x
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

	// �����蔻��p�����f�����쐬
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth, true);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlockType0::Uninit(void)
{
	// ���ʂ̏I������
	CBlock::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlockType0::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlockType0::Draw(void)
{
	// ���ʂ̕`�揈��
	CBlock::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlockType0::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    ���������Ƃ��̏���
//=============================================================================
void CBlockType0::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_BULLET)
	{// �e������
		Uninit();
	}
}

//*****************************************************************************
//    CBlockType1�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlockType1::CBlockType1(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlockType1::~CBlockType1()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlockType1 *CBlockType1::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType1 *pBlockType1 = NULL;      // �^�C�v1�̃u���b�N�N���X�^�̃|�C���^
	if (pBlockType1 == NULL)
	{// ����������ɂȂ��Ă���
		pBlockType1 = new CBlockType1(nPriority);
		if (pBlockType1 != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pBlockType1->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType1;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlockType1::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// �e��l�̐ݒ�
	SetPos(pos);                                     // ���W
	SetRot(rot);                                     // ����
	SetType(nType);                                  // ��ޔԍ�
	SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
	SetAlpha(1.0f);                                  // ���f���̓����x
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

	// �����蔻��p�����f�����쐬
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth, false);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlockType1::Uninit(void)
{
	// ���ʂ̏I������
	CBlock::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlockType1::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlockType1::Draw(void)
{
	// ���ʂ̕`�揈��
	CBlock::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlockType1::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth, false);
}

//=============================================================================
//    ���������Ƃ��̏���
//=============================================================================
void CBlockType1::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{// �v���C���[������
		// �򖗃|���S������
		CreateSplash();

		// �I������
		Uninit();
	}
	else if (pScene->GetObjType() == OBJTYPE_ENEMY)
	{// �G������
		// �GAI�p�^�[���ω�

		// �I������
		Uninit();
	}
}

//=============================================================================
//    �򖗃|���S���𐶐����鏈��
//=============================================================================
void CBlockType1::CreateSplash(void)
{
	CSplash::Create(D3DXVECTOR3(130.0f, 260.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(1.0f, -1.0f, 0.0f), 0.0f, 0.005f, 250.0f, -1.0f, 180);
	CSplash::Create(D3DXVECTOR3(350.0f, 490.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(1.0f, -1.0f, 0.0f), 0.0f, 0.005f, 240.0f, -1.0f, 180);
	CSplash::Create(D3DXVECTOR3(600.0f, 160.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(1.0f, -1.0f, 0.0f), 0.0f, 0.005f, 265.0f, -1.0f, 180);
	CSplash::Create(D3DXVECTOR3(890.0f, 560.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10.0f, 0,
		D3DXVECTOR3(1.0f, -1.0f, 0.0f), 0.0f, 0.005f, 230.0f, -1.0f, 180);
}

//*****************************************************************************
//    CBlockType2�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlockType2::CBlockType2(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlockType2::~CBlockType2()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlockType2 *CBlockType2::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType2 *pBlockType2 = NULL;      // �^�C�v2�̃u���b�N�N���X�^�̃|�C���^
	if (pBlockType2 == NULL)
	{// ����������ɂȂ��Ă���
		pBlockType2 = new CBlockType2(nPriority);
		if (pBlockType2 != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pBlockType2->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType2;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlockType2::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// �e��l�̐ݒ�
	SetPos(pos);                                     // ���W
	SetRot(rot);                                     // ����
	SetType(nType);                                  // ��ޔԍ�
	SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
	SetAlpha(1.0f);                                  // ���f���̓����x
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

	// �����蔻��p�����f�����쐬
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlockType2::Uninit(void)
{
	// ���ʂ̏I������
	CBlock::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlockType2::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlockType2::Draw(void)
{
	// ���ʂ̕`�揈��
	CBlock::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlockType2::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    ���������Ƃ��̏���
//=============================================================================
void CBlockType2::Hit(CScene *pScene)
{

}

//*****************************************************************************
//    CBlockType3�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBlockType3::CBlockType3(int nPriority, OBJTYPE objType) : CBlock(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBlockType3::~CBlockType3()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBlockType3 *CBlockType3::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CBlockType3 *pBlockType3 = NULL;      // �^�C�v3�̃u���b�N�N���X�^�̃|�C���^
	if (pBlockType3 == NULL)
	{// ����������ɂȂ��Ă���
		pBlockType3 = new CBlockType3(nPriority);
		if (pBlockType3 != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pBlockType3->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pBlockType3;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBlockType3::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// �e��l�̐ݒ�
	SetPos(pos);                                     // ���W
	SetRot(rot);                                     // ����
	SetType(nType);                                  // ��ޔԍ�
	SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
	SetAlpha(1.0f);                                  // ���f���̓����x
	BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

	// �����蔻��p�����f�����쐬
	SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBlockType3::Uninit(void)
{
	// ���ʂ̏I������
	CBlock::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBlockType3::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBlockType3::Draw(void)
{
	// ���ʂ̕`�揈��
	CBlock::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CBlockType3::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
{
	// ���݂̓����蔻��p�����f�����J������
	CBoxCollider *pBoxCollider = GetBoxCollider();
	if (pBoxCollider != NULL)
	{
		pBoxCollider->Uninit();
		delete pBoxCollider;
		pBoxCollider = NULL;
	}

	// �����蔻��p�����f���𐶐����Ȃ���
	CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);
}

//=============================================================================
//    ���������Ƃ��̏���
//=============================================================================
void CBlockType3::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{// �v���C���[������
		// �򖗃|���S������
	}
	else if (pScene->GetObjType() == OBJTYPE_ENEMY)
	{// �G������
		// �GAI�p�^�[���ω�
	}
}