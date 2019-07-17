//*****************************************************************************
//
//     �A�C�e���̏���[item.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "item.h"
#include "manager.h"
#include "boxCollider.h"
#include "renderer.h"
#include "basemode.h"
#include "game.h"
#include "player.h"

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
CItem::CItem(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
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
CItem::~CItem()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItem *pItem = NULL;      // �A�C�e���N���X�^�̃|�C���^
	if (pItem == NULL)
	{// ����������ɂȂ��Ă���
		pItem = new CItem(nPriority);
		if (pItem != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pItem->SetPos(pos);                                     // ���W
			pItem->SetRot(rot);                                     // ����
			pItem->SetType(nType);                                  // ��ޔԍ�
			pItem->SetModelIdx(nModelIdx);                          // �g�p���郂�f���̔ԍ�
			pItem->SetAlpha(1.0f);                                  // ���f���̓����x
			pItem->BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

			// �����蔻��p�����f�����쐬
			pItem->SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
			pItem->CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

			if (FAILED(pItem->Init()))
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

	return pItem;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItem::Init(void)
{
	// �A�C�e���̃��f����ݒ�
	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CGame *pGame = CManager::GetGame();
		pGame->SetItemModel(this, m_nType);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CItem::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItem::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItem::Draw(void)
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
void CItem::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, bReturnFlag);
	SetBoxCollider(pBoxCollider);
}


//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItem::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItem::Hit(CScene *pScene)
{
	Uninit();
}

//=============================================================================
//    ���f�����ݒ菈��
//=============================================================================
void CItem::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
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
void CItem::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    �g�p���郂�f���̔ԍ��ݒ菈��
//=============================================================================
void CItem::SetModelIdx(const int nModelIdx)
{
	m_nModelIdx = nModelIdx;
}

//=============================================================================
//    ���b�V���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CItem::SetMesh(const LPD3DXMESH pMesh)
{
	m_pMesh = pMesh;
}

//=============================================================================
//    �}�e���A�����ւ̃|�C���^�ݒ菈��
//=============================================================================
void CItem::SetBuffMat(const LPD3DXBUFFER pBuffMat)
{
	m_pBuffMat = pBuffMat;
}

//=============================================================================
//    �}�e���A�����̐��ݒ菈��
//=============================================================================
void CItem::SetNumMat(const DWORD nNumMat)
{
	m_nNumMat = nNumMat;
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CItem::SetTexture(LPDIRECT3DTEXTURE9 *pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    �ő�̒��_���W�ݒ菈��
//=============================================================================
void CItem::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    �ŏ��̒��_���W�ݒ菈��
//=============================================================================
void CItem::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    ���f���̓����x�ݒ菈��
//=============================================================================
void CItem::SetAlpha(const float fAlpha)
{
	m_fAlpha = fAlpha;
}

//=============================================================================
//    ��ޔԍ��擾����
//=============================================================================
int CItem::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    �g�p���郂�f���̔ԍ��擾����
//=============================================================================
int CItem::GetModelIdx(void)
{
	return m_nModelIdx;
}

//=============================================================================
//    ���b�V���ւ̃|�C���^�擾����
//=============================================================================
LPD3DXMESH CItem::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    �}�e���A�����ւ̃|�C���^�擾����
//=============================================================================
LPD3DXBUFFER CItem::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    �}�e���A�����̐��擾����
//=============================================================================
DWORD CItem::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�擾����
//=============================================================================
LPDIRECT3DTEXTURE9 *CItem::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    �ő�̒��_���W�擾����
//=============================================================================
D3DXVECTOR3 CItem::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    �ŏ��̒��_���W�擾����
//=============================================================================
D3DXVECTOR3 CItem::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    ���f���̓����x�擾����
//=============================================================================
float CItem::GetAlpha(void)
{
	return m_fAlpha;
}

//*****************************************************************************
//    CItemStar�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemStar::CItemStar(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemStar::~CItemStar()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemStar *CItemStar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemStar *pItemStar = NULL;      // �A�C�e��(�X�^�[)�N���X�^�̃|�C���^
	if (pItemStar == NULL)
	{// ����������ɂȂ��Ă���
		pItemStar = new CItemStar(nPriority);
		if (pItemStar != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemStar->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemStar;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemStar::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemStar::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemStar::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemStar::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItemStar::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemStar::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �v���C���[�N���X�̃|�C���^�ɃL���X�g
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �A�C�e���̃C�x���g���N����
		int nPlayerNumber = pPlayer->GetIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Star(nPlayerNumber);
		}

		// �A�C�e��������
		Uninit();
	}
}

//*****************************************************************************
//    CItemGrenade�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemGrenade::CItemGrenade(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemGrenade::~CItemGrenade()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemGrenade *CItemGrenade::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemGrenade *pItemGrenade = NULL;      // �A�C�e��(��֒e)�N���X�^�̃|�C���^
	if (pItemGrenade == NULL)
	{// ����������ɂȂ��Ă���
		pItemGrenade = new CItemGrenade(nPriority);
		if (pItemGrenade != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemGrenade->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemGrenade;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemGrenade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemGrenade::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemGrenade::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemGrenade::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItemGrenade::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemGrenade::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �A�C�e���̃C�x���g���N����
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Grenade();
		}

		// �A�C�e��������
		Uninit();
	}
}

//*****************************************************************************
//    CItem1up�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItem1up::CItem1up(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItem1up::~CItem1up()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItem1up *CItem1up::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItem1up *pItem1Up = NULL;      // �A�C�e��(�c�@UP)�N���X�^�̃|�C���^
	if (pItem1Up == NULL)
	{// ����������ɂȂ��Ă���
		pItem1Up = new CItem1up(nPriority);
		if (pItem1Up != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItem1Up->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItem1Up;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItem1up::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItem1up::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItem1up::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItem1up::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItem1up::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItem1up::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �v���C���[�N���X�̃|�C���^�ɃL���X�g
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �A�C�e���̃C�x���g���N����
		int nPlayerNumber = pPlayer->GetIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_1Up(nPlayerNumber);
		}

		// �A�C�e��������
		Uninit();
	}
}

//*****************************************************************************
//    CItemScoop�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemScoop::CItemScoop(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemScoop::~CItemScoop()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemScoop *CItemScoop::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemScoop *pItemScoop = NULL;      // �A�C�e��(�X�R�b�v)�N���X�^�̃|�C���^
	if (pItemScoop == NULL)
	{// ����������ɂȂ��Ă���
		pItemScoop = new CItemScoop(nPriority);
		if (pItemScoop != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemScoop->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemScoop;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemScoop::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemScoop::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemScoop::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemScoop::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItemScoop::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemScoop::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �A�C�e���̃C�x���g���N����
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Scoop();
		}

		// �A�C�e��������
		Uninit();
	}
}

//*****************************************************************************
//    CItemClock�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemClock::CItemClock(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemClock::~CItemClock()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemClock *CItemClock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemClock *pItemClock = NULL;      // �A�C�e��(���v)�N���X�^�̃|�C���^
	if (pItemClock == NULL)
	{// ����������ɂȂ��Ă���
		pItemClock = new CItemClock(nPriority);
		if (pItemClock != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemClock->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemClock;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemClock::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemClock::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemClock::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemClock::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItemClock::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemClock::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �A�C�e���̃C�x���g���N����
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Clock();
		}

		// �A�C�e��������
		Uninit();
	}
}

//*****************************************************************************
//    CItemHelmet�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CItemHelmet::CItemHelmet(int nPriority, OBJTYPE objType) : CItem(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CItemHelmet::~CItemHelmet()
{

}

//=============================================================================
//    ��������
//=============================================================================
CItemHelmet *CItemHelmet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CItemHelmet *pItemHelmet = NULL;      // �A�C�e��(�w�����b�g)�N���X�^�̃|�C���^
	if (pItemHelmet == NULL)
	{// ����������ɂȂ��Ă���
		pItemHelmet = new CItemHelmet(nPriority);
		if (pItemHelmet != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pItemHelmet->Init(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxWidth, fBoxHeight, fBoxDepth)))
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

	return pItemHelmet;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CItemHelmet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemHelmet::Uninit(void)
{
	// ���ʂ̏I������
	CItem::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CItemHelmet::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CItemHelmet::Draw(void)
{
	// ���ʂ̕`�揈��
	CItem::Draw();
}

//=============================================================================
//    �����蔻��p�����f������蒼������
//=============================================================================
void CItemHelmet::RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth)
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
void CItemHelmet::Hit(CScene *pScene)
{
	// �������Ă����I�u�W�F�N�g�̎�ނŏ����킯
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{
		// �v���C���[�N���X�̃|�C���^�ɃL���X�g
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �A�C�e���̃C�x���g���N����
		int nPlayerNumber = pPlayer->GetIdx();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{
			CGame *pGame = CManager::GetGame();
			pGame->ItemEvent_Helmet(nPlayerNumber);
		}

		// �A�C�e��������
		Uninit();
	}
}