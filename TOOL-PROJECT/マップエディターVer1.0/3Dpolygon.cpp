//*****************************************************************************
//
//     3D�|���S���̏���[3Dpolygon.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "3Dpolygon.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
C3DPolygon::C3DPolygon()
{
	// �e��l�̃N���A
	m_pTexture = NULL;                         // �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;                         // ���_�o�b�t�@�ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �|���S���̍��W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �|���S���̌���
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �|���S���̐F
	m_fWidth = 0.0f;                           // �|���S���̕�
	m_fHeight = 0.0f;                          // �|���S���̍���
	m_fAngle = 0.0f;                           // �|���S���̊p�x
	m_fLength = 0.0f;                          // �|���S���̒���
	D3DXMatrixIdentity(&m_MtxWorld);           // ���[���h�}�g���b�N�X
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
C3DPolygon::~C3DPolygon()
{

}

//=============================================================================
//    ��������
//=============================================================================
C3DPolygon *C3DPolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight)
{
	C3DPolygon *p3DPolygon = NULL; // 3D�|���S���N���X�^�̃|�C���^
	if (p3DPolygon == NULL)
	{// ����������ɂȂ��Ă���
		p3DPolygon = new C3DPolygon;
		if (p3DPolygon != NULL)
		{// �C���X�^���X�𐶐��ł���
		 // �e��l�̑��
			p3DPolygon->SetPos(pos);         // �|���S���̍��W
			p3DPolygon->SetRot(rot);         // �|���S���̌���
			p3DPolygon->SetCol(col);         // �|���S���̐F
			p3DPolygon->SetWidth(fWidth);    // �|���S���̕�
			p3DPolygon->SetHeight(fHeight);  // �|���S���̍���

			if (FAILED(p3DPolygon->Init()))
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

	return p3DPolygon;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT C3DPolygon::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		 // ���_�o�b�t�@�̐���
			MakeVertex(pDevice);
		}
		else
		{// �f�o�C�X���擾�ł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �����_�����O�N���X����������Ă��Ȃ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void C3DPolygon::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);
}

//=============================================================================
//    �X�V����
//=============================================================================
void C3DPolygon::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void C3DPolygon::Draw(void)
{
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

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void C3DPolygon::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

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
//    ���_�o�b�t�@��������
//=============================================================================
void C3DPolygon::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �I�t�Z�b�g�̒��������߂�
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	// �I�t�Z�b�g�̊p�x�����߂�
	m_fAngle = atan2f(m_fWidth, m_fHeight);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

	// �@���v�Z�p�x�N�g��
	D3DXVECTOR3 vecVer[4];

	// ���_�̖@��
	D3DXVECTOR3 nor1, nor2;

	// �O�ϗp�Ƀx�N�g�������
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// �x�N�g�����O�ςŌv�Z���Ė@�����o��
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// �@������
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

	// ���_�J���[
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    �e�N�X�`���ݒ菈��
//=============================================================================
void C3DPolygon::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���_�o�b�t�@�ݒ菈��
//=============================================================================
void C3DPolygon::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    �|���S���̍��W�ݒ菈��
//=============================================================================
void C3DPolygon::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �|���S���̌����ݒ菈��
//=============================================================================
void C3DPolygon::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    �|���S���̐F�ݒ菈��
//=============================================================================
void C3DPolygon::SetCol(const D3DXCOLOR col)
{
	// �F��ݒ�
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	 // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[
		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �|���S���̕��ݒ菈��
//=============================================================================
void C3DPolygon::SetWidth(const float fWidth)
{
	// ����ݒ�
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	 // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �I�t�Z�b�g�̒��������߂�
		m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

		// �I�t�Z�b�g�̊p�x�����߂�
		m_fAngle = atan2f(m_fWidth, m_fHeight);

		// ���_���W
		pVtx[0].pos.x = -sinf(m_fAngle) * m_fLength;
		pVtx[1].pos.x = sinf(m_fAngle) * m_fLength;
		pVtx[2].pos.x = -sinf(m_fAngle) * m_fLength;
		pVtx[3].pos.x = sinf(m_fAngle) * m_fLength;

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �|���S���̍����ݒ菈��
//=============================================================================
void C3DPolygon::SetHeight(const float fHeight)
{
	// ������ݒ�
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	 // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �I�t�Z�b�g�̒��������߂�
		m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

		// �I�t�Z�b�g�̊p�x�����߂�
		m_fAngle = atan2f(m_fWidth, m_fHeight);

		// ���_���W
		pVtx[0].pos.y = cosf(m_fAngle) * m_fLength;
		pVtx[1].pos.y = cosf(m_fAngle) * m_fLength;
		pVtx[2].pos.y = -cosf(m_fAngle) * m_fLength;
		pVtx[3].pos.y = -cosf(m_fAngle) * m_fLength;

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �|���S���̊p�x�ݒ菈��
//=============================================================================
void C3DPolygon::SetAngle(const float fAngle)
{
	// �p�x��ݒ�
	m_fAngle = fAngle;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	 // ���_���
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �|���S���̒����ݒ菈��
//=============================================================================
void C3DPolygon::SetLength(const float fLength)
{
	// ������ݒ�
	m_fLength = fLength;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	 // ���_���
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void C3DPolygon::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���_�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 C3DPolygon::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    �e�N�X�`���擾����
//=============================================================================
LPDIRECT3DTEXTURE9 C3DPolygon::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    �|���S���̍��W�擾����
//=============================================================================
D3DXVECTOR3 C3DPolygon::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �|���S���̌����擾����
//=============================================================================
D3DXVECTOR3 C3DPolygon::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    �|���S���̐F�擾����
//=============================================================================
D3DXCOLOR C3DPolygon::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    �|���S���̕��擾����
//=============================================================================
float C3DPolygon::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    �|���S���̍����擾����
//=============================================================================
float C3DPolygon::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    �|���S���̊p�x�擾����
//=============================================================================
float C3DPolygon::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
//    �|���S���̒����擾����
//=============================================================================
float C3DPolygon::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX C3DPolygon::GetMtxWorld(void)
{
	return m_MtxWorld;
}