//*****************************************************************************
//
//     �O���b�h���̏���[grid.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "grid.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define GRID_LINECOLOR (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)) // ���̐F
#define GRID_LINEWIDTH (2.0f)                           // ���̕�

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGrid::CGrid()
{
	// �e��l�̃N���A
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���W
	m_fWidth = 0.0f;                          // ����
	m_fDepth = 0.0f;                          // ���s
	m_nXBlock = 0;                            // ���̕�����
	m_nZBlock = 0;                            // ���s�̕�����
	m_pVecPos = NULL;                         // ���_���W
	m_pIndex = NULL;                          // ���_�ԍ�
	m_pLine = NULL;                           // ���`��p�C���X�^���X�ւ̃|�C���^
	m_nNumVertex = 0;                         // ���_��
	m_nNumIdx = 0;                            // �C���f�b�N�X�ԍ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGrid::~CGrid()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGrid *CGrid::Create(D3DXVECTOR3 pos, float fWidth, float fDepth, int nXBlock, int nZBlock)
{
	CGrid *pGrid = NULL;      // �����蔻��p�����f���N���X�^�̃|�C���^
	if (pGrid == NULL)
	{// ����������ɂȂ��Ă���
		pGrid = new CGrid;
		if (pGrid != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pGrid->SetPos(pos);                 // ���W
			pGrid->SetWidth(fWidth);            // ����
			pGrid->SetDepth(fDepth);            // ���s
			pGrid->SetXBlock(nXBlock);          // ���̕�����
			pGrid->SetZBlock(nZBlock);          // ���s�̕�����

			if (FAILED(pGrid->Init()))
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

	return pGrid;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGrid::Init(void)
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

			// ���_��񐶐�
			MakeVertex();
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGrid::Uninit(void)
{
	if (m_pVecPos != NULL)
	{
		delete[] m_pVecPos;
		m_pVecPos = NULL;
	}

	if (m_pIndex != NULL)
	{
		delete[] m_pIndex;
		m_pIndex = NULL;
	}

	DIRECT_RELEASE(m_pLine);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGrid::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGrid::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �������e�ϊ��s����J��������쐬����
			D3DXVECTOR3 VecScreenPos;
			D3DXMATRIX mtxView;
			D3DXMATRIX mtxProj;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
			// ����`��
			if (m_pVecPos != NULL && m_pIndex != NULL && m_pLine != NULL)
			{
				int nIdx = 0;
				D3DXMATRIX mtxTrans, mtxWorld;
				D3DXMatrixIdentity(&mtxWorld);
				D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxView);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxProj);
				D3DXVECTOR3 Vec[2];
				m_pLine->SetAntialias(TRUE);
				m_pLine->SetWidth(GRID_LINEWIDTH);
				m_pLine->Begin();
				for (int nCnt = 0; nCnt < (m_nXBlock * m_nZBlock); nCnt++)
				{
					Vec[0] = m_pVecPos[m_pIndex[nIdx]];
					Vec[1] = m_pVecPos[m_pIndex[nIdx + 1]];
					m_pLine->DrawTransform(Vec, 2, &mtxWorld, GRID_LINECOLOR);

					Vec[0] = m_pVecPos[m_pIndex[nIdx + 2]];
					Vec[1] = m_pVecPos[m_pIndex[nIdx + 3]];
					m_pLine->DrawTransform(Vec, 2, &mtxWorld, GRID_LINECOLOR);

					Vec[0] = m_pVecPos[m_pIndex[nIdx + 4]];
					Vec[1] = m_pVecPos[m_pIndex[nIdx + 5]];
					m_pLine->DrawTransform(Vec, 2, &mtxWorld, GRID_LINECOLOR);

					Vec[0] = m_pVecPos[m_pIndex[nIdx + 6]];
					Vec[1] = m_pVecPos[m_pIndex[nIdx + 7]];
					m_pLine->DrawTransform(Vec, 2, &mtxWorld, GRID_LINECOLOR);

					nIdx += 8;
				}
				m_pLine->End();
			}
		}
	}
}

//=============================================================================
//    ���_���𐶐����鏈��
//=============================================================================
void CGrid::MakeVertex(void)
{
	// �K�v�Ȑ��������������擾
	m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);
	m_nNumIdx = (m_nXBlock * m_nZBlock) * 8;
	m_pVecPos = new D3DXVECTOR3[m_nNumVertex];
	m_pIndex = new int[m_nNumIdx];

	// ���_���W��ݒ�
	float XPos = (-m_fWidth * m_nXBlock) / 2;
	float ZPos = (m_fDepth * m_nZBlock) / 2;
	int nIdx = 0;
	if (m_pVecPos != NULL)
	{
		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// ���������̕����� + 1�J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				m_pVecPos[nIdx] = D3DXVECTOR3(XPos, 0.0f, ZPos);
				XPos += m_fWidth;
				nIdx++;
			}
			XPos = (-m_fWidth * m_nXBlock) / 2;
			ZPos += -m_fDepth;
		}
	}

	// ���_�ԍ���ݒ�
	int nVerIndex = 0;
	nIdx = 0;
	for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
	{// ���������̕����� + 1�J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
		{// ���������̕����� + 1�����J��Ԃ�
			m_pIndex[nVerIndex] = nIdx;
			m_pIndex[nVerIndex + 1] = nIdx + 1;
			m_pIndex[nVerIndex + 2] = nIdx;
			m_pIndex[nVerIndex + 3] = nIdx + m_nXBlock + 1;
			m_pIndex[nVerIndex + 4] = nIdx + 1;
			m_pIndex[nVerIndex + 5] = nIdx + m_nXBlock + 2;
			m_pIndex[nVerIndex + 6] = nIdx + m_nXBlock + 2;
			m_pIndex[nVerIndex + 7] = nIdx + m_nXBlock + 1;
			nVerIndex += 8;
			nIdx++;
		}
		nIdx++;
	}
}

//=============================================================================
//    ���W�ݒ菈��
//=============================================================================
void CGrid::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ���̉����ݒ菈��
//=============================================================================
void CGrid::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    ���̉��s�ݒ菈��
//=============================================================================
void CGrid::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    ���̕������ݒ菈��
//=============================================================================
void CGrid::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    ���s�̕������ݒ菈��
//=============================================================================
void CGrid::SetZBlock(const int nZBlock)
{
	m_nZBlock = nZBlock;
}

//=============================================================================
//    ���W�擾����
//=============================================================================
D3DXVECTOR3 CGrid::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ���̉����擾����
//=============================================================================
float CGrid::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ���̉��s�擾����
//=============================================================================
float CGrid::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    ���s�̕������擾����
//=============================================================================
int CGrid::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    ���̕������擾����
//=============================================================================
int CGrid::GetZBlock(void)
{
	return m_nZBlock;
}