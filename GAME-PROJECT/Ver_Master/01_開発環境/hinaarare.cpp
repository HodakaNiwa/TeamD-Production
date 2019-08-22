//*****************************************************************************
//
//     �ЂȂ����̏���[hinaarare.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "hinaarare.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "boxCollider.h"
#include "basemode.h"
#include "player.h"
#include "effectManager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define HINAARARE_DOWN_SPEED    (-20.0f)   // �ЂȂ���ꂪ������X�s�[�h
#define HINAARARE_MAX_LIFE      (120)      // �ЂȂ����̎����̍ő�l
#define HINAARARE_EFFECT_IDX    (13)       // �ЂȂ���ꂪ�g���G�t�F�N�g�̔ԍ�

//*****************************************************************************
//    �ÓI�����o�ϐ�
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CHinaarare::CHinaarare(int nPriority, OBJTYPE objType) : CObject3D(nPriority, objType)
{
	// �e��l�̃N���A
	m_nLife = 0;
	m_pModel = NULL;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CHinaarare::~CHinaarare()
{

}

//=============================================================================
//    ��������
//=============================================================================
CHinaarare *CHinaarare::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth, int nPriority)
{
	CHinaarare *pHinaarare = NULL;      // �ЂȂ����N���X�^�̃|�C���^
	if (pHinaarare == NULL)
	{// ����������ɂȂ��Ă���
		pHinaarare = new CHinaarare(nPriority);
		if (pHinaarare != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pHinaarare->SetPos(pos);                                     // ���W
			pHinaarare->SetRot(rot);                                     // ����
			pHinaarare->BindModel(pMesh, pBuffMat, nNumMat, pTexture);   // ���f����񊄂蓖��

			// �����蔻��p�����f�����쐬
			pHinaarare->SetColRange(D3DXVECTOR3(fBoxWidth, fBoxHeight, fBoxDepth));
			pHinaarare->CreateBoxCollider(fBoxWidth, fBoxHeight, fBoxDepth);

			if (FAILED(pHinaarare->Init()))
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

	return pHinaarare;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CHinaarare::Init(void)
{
	// ������ݒ�
	m_nLife = HINAARARE_MAX_LIFE;

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CHinaarare::Uninit(void)
{
	// ���f���̔j��
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = NULL;
	}

	// �I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CHinaarare::Update(void)
{
	// �l�̎擾
	D3DXVECTOR3 pos = GetPos();

	// ���ɗ��Ƃ�
	pos.y += HINAARARE_DOWN_SPEED;

	// �G�t�F�N�g���o��
	SetEffect(pos);

	// �l�̐ݒ�
	SetPos(pos);

	// ���������炷
	m_nLife--;
	if (m_nLife <= 0 || pos.y <= 0.0f)
	{// �������Ȃ��Ȃ� OR �n�ʂɂ�
		Uninit();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CHinaarare::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer == NULL) { return; }
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == NULL) { return; }

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetMtxWorld(pDevice);

	// ���f���̕`��
	if (m_pModel != NULL)
	{
		m_pModel->Draw();
	}

	// 3D�I�u�W�F�N�g�̕`��
	CObject3D::Draw();
}

//=============================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//=============================================================================
void CHinaarare::SetMtxWorld(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxWorld;  // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CObject3D::SetMtxWorld(mtxWorld);
}

//=============================================================================
//    �����蔻��p�����f�����쐬���鏈��
//=============================================================================
void CHinaarare::CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = CBoxCollider::Create(GetPos(), fBoxWidth, fBoxHeight, fBoxDepth, bReturnFlag);
	SetBoxCollider(pBoxCollider);
}

//=============================================================================
//    ���f�����ݒ菈��
//=============================================================================
void CHinaarare::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	m_pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, pMesh, pBuffMat,
		nNumMat, pTexture);
}

//=============================================================================
//    �I�u�W�F�N�g�ɓ��������Ƃ��̏���
//=============================================================================
void CHinaarare::Hit(CScene *pScene)
{
	if (pScene->GetObjType() == OBJTYPE_PLAYER)
	{// �v���C���[�Ƀq�b�g����
		Uninit();
	}
}

//=============================================================================
//    �G�t�F�N�g���o������
//=============================================================================
void CHinaarare::SetEffect(D3DXVECTOR3 pos)
{
	// �G�t�F�N�g�Ǌ��N���X���擾
	if (CManager::GetBaseMode() == NULL) { return; }
	CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
	if (pEffectManager == NULL) { return; }

	// �G�t�F�N�g�𐶐�
	pEffectManager->SetEffect(pos, INITIALIZE_D3DXVECTOR3, HINAARARE_EFFECT_IDX);
}