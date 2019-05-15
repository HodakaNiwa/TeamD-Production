//*****************************************************************************
//
//     �����蔻��p�����f���̏���[boxCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "boxCollider.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define BOX_COLLIDER_LINEWIDTH (3.0f)    // ���̕�

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBoxCollider::CBoxCollider()
{
	// �e��l�̃N���A
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ����
	D3DXMatrixIdentity(&m_MtxWorld);          // ���[���h�}�g���b�N�X
	m_fWidth = 0.0f;                          // ���̉���
	m_fHeight = 0.0f;                         // ���̍���
	m_fDepth = 0.0f;                          // ���̉��s
	m_bReturnFlag = false;                    // ���������ۂɖ߂����ǂ���
	m_pLine = NULL;                           // ���`��p�C���X�^���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBoxCollider::~CBoxCollider()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBoxCollider *CBoxCollider::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = NULL;      // �����蔻��p�����f���N���X�^�̃|�C���^
	if (pBoxCollider == NULL)
	{// ����������ɂȂ��Ă���
		pBoxCollider = new CBoxCollider;
		if (pBoxCollider != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pBoxCollider->SetPos(pos);                 // ���W
			pBoxCollider->SetWidth(fWidth);            // ���̉���
			pBoxCollider->SetHeight(fHeight);          // ���̍���
			pBoxCollider->SetDepth(fDepth);            // ���̉��s
			pBoxCollider->SetReturnFlag(bReturnFlag);  // ���������ۂɖ߂����ǂ���

			if (FAILED(pBoxCollider->Init()))
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

	return pBoxCollider;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBoxCollider::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���`��p�C���X�^���X�𐶐�
			D3DXCreateLine(pDevice, &m_pLine);
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBoxCollider::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CBoxCollider::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBoxCollider::Draw(void)
{
#ifdef _DEBUG
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���[���h�}�g���b�N�X�̐ݒ菈��
			SetMtxWorld(pDevice);

			// �������e�ϊ��s����J��������쐬����
			D3DXVECTOR3 VecScreenPos;
			D3DXMATRIX mtxView;
			D3DXMATRIX mtxProj;
			D3DXMATRIX mtxViewPort;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
			D3DXMatrixIdentity(&mtxViewPort);
			D3DXMatrixIdentity(&mtxViewPort);
			mtxViewPort._11 = SCREEN_WIDTH / 2;
			mtxViewPort._22 = -SCREEN_HEIGHT / 2;
			mtxViewPort._41 = SCREEN_WIDTH / 2;
			mtxViewPort._42 = SCREEN_HEIGHT / 2;
			mtxViewPort._33 = 1.0f;
			mtxViewPort._44 = 1.0f;

			// �e���C���̒��_���W��ݒ�
			D3DXVECTOR3 VecPos[24] =
			{
				// �P�{��
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				// �Q�{��
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				// �R�{��
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				// �S�{��
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				// �T�{��
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				// �U�{��
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
				// �V�{��
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				// �W�{��
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				// �X�{��
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				// �P�O�{��
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				// �P�P�{��
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
				// �P�Q�{��
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
			};

			// �e���_�����[���h���W����X�N���[�����W�֕ϊ�����
			for (int nCnt = 0; nCnt < 24; nCnt++)
			{
				D3DXVec3TransformCoord(&VecScreenPos, &VecPos[nCnt], &m_MtxWorld);
				D3DXVec3TransformCoord(&VecScreenPos, &VecScreenPos, &mtxView);
				D3DXVec3TransformCoord(&VecScreenPos, &VecScreenPos, &mtxProj);
				D3DXVec3TransformCoord(&VecScreenPos, &VecScreenPos, &mtxViewPort);
				m_VecPos[nCnt] = D3DXVECTOR2(VecScreenPos.x, VecScreenPos.y);
			}

			// ����`��
			D3DXVECTOR2 Vec[2];
			m_pLine->SetAntialias(TRUE);
			m_pLine->SetWidth(BOX_COLLIDER_LINEWIDTH);
			m_pLine->Begin();
			for (int nCnt = 0; nCnt < 12; nCnt++)
			{
				Vec[0] = m_VecPos[(nCnt * 2)];
				Vec[1] = m_VecPos[(nCnt * 2) + 1];
				m_pLine->Draw(Vec, 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			m_pLine->End();
		}
	}

#endif
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CBoxCollider::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxScale, mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CBoxCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand)
{
	bool bHit = false;  // �����������ǂ���

	// Y����̔���
	if (pPos->y < m_Pos.y + m_fHeight && pPos->y + ColRange.y > m_Pos.y)
	{// ���݂̍��������̒��ɂ���
	    // X����̔���
		if (pPos->z + ColRange.z > m_Pos.z - (m_fDepth / 2) && pPos->z - ColRange.z < m_Pos.z + (m_fDepth / 2))
		{// Z����ɂ��邩�ǂ�������
			if (pPosOld->x + ColRange.x <= m_Pos.x - (m_fWidth / 2) && pPos->x + ColRange.x > m_Pos.x - (m_fWidth / 2))
			{// ���̍������������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->x = m_Pos.x - ColRange.x - (m_fWidth / 2);
				}
				bHit = true;
			}
			else if (pPosOld->x - ColRange.x >= m_Pos.x + (m_fWidth / 2) && pPos->x - ColRange.x < m_Pos.x + (m_fWidth / 2))
			{// ���̉E�����������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->x = m_Pos.x + ColRange.x + (m_fWidth / 2);
				}
				bHit = true;
			}
		}
		// Z����̔���
		if (pPos->x + ColRange.x > m_Pos.x - (m_fWidth / 2) && pPos->x - ColRange.x < m_Pos.x + (m_fWidth / 2))
		{// X����ɂ��邩�ǂ�������
			if (pPosOld->z - ColRange.z >= m_Pos.z + (m_fDepth / 2) && pPos->z - ColRange.z < m_Pos.z + (m_fDepth / 2))
			{// ���̉������������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->z = m_Pos.z + ColRange.z + (m_fDepth / 2);
				}
				bHit = true;
			}
			else if (pPosOld->z + ColRange.z <= m_Pos.z - (m_fDepth / 2) && pPos->z + ColRange.z > m_Pos.z - (m_fDepth / 2))
			{// ���̎�O�����������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->z = m_Pos.z - ColRange.z - (m_fDepth / 2);
				}
				bHit = true;
			}
		}
	}

	if (pPosOld->y >= m_Pos.y + m_fHeight && pPos->y < m_Pos.y + m_fHeight)
	{// �������ォ�甠�̓����ɓ����Ă��܂���
		if (pPos->x + ColRange.x > m_Pos.x - (m_fWidth / 2) && pPos->x - ColRange.x < m_Pos.x + (m_fWidth / 2)
			&& pPos->z + ColRange.z > m_Pos.z - (m_fDepth / 2) && pPos->z - ColRange.z < m_Pos.z + (m_fDepth / 2))
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->y = m_Pos.y + m_fHeight;
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + ColRange.y <= m_Pos.y && pPos->y + ColRange.y > m_Pos.y)
	{// �����������甠�̓����ɓ����Ă��܂���
		if (pPos->x + ColRange.x > m_Pos.x - (m_fWidth / 2) && pPos->x - ColRange.x < m_Pos.x + (m_fWidth / 2)
			&& pPos->z + ColRange.z > m_Pos.z - (m_fDepth / 2) && pPos->z - ColRange.z < m_Pos.z + (m_fDepth / 2))
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->y = m_Pos.y - ColRange.y;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    ���W�ݒ菈��
//=============================================================================
void CBoxCollider::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CBoxCollider::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CBoxCollider::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���̉����ݒ菈��
//=============================================================================
void CBoxCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    ���̍����ݒ菈��
//=============================================================================
void CBoxCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    ���̉��s�ݒ菈��
//=============================================================================
void CBoxCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����ݒ菈��
//=============================================================================
void CBoxCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    ���W�擾����
//=============================================================================
D3DXVECTOR3 CBoxCollider::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �����擾����
//=============================================================================
D3DXVECTOR3 CBoxCollider::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CBoxCollider::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ���̉����擾����
//=============================================================================
float CBoxCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ���̍����擾����
//=============================================================================
float CBoxCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���̉��s�擾����
//=============================================================================
float CBoxCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����擾����
//=============================================================================
bool CBoxCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}