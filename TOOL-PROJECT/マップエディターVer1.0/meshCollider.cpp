//*****************************************************************************
//
//     ���b�V���R���C�_�[�̏���[meshCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshCollider.h"
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
CMeshCollider::CMeshCollider(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pVtxBuff = NULL;                          // ���_�o�b�t�@�ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���b�V���t�B�[���h�̍��W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���b�V���t�B�[���h�̌���
	m_fWidth = 0.0f;                            // 1�u���b�N���̕�
	m_fHeight = 0.0f;                           // 1�u���b�N������
	m_nXBlock = 0;                              // ���̕�����
	m_nZBlock = 0;                              // ���s�̕�����
	m_nNumVertex = 0;                           // ���_��
	D3DXMatrixIdentity(&m_MtxWorld);            // ���[���h�}�g���b�N�X
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMeshCollider::~CMeshCollider()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMeshCollider *CMeshCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nXBlock, int nZBlock, int nPriority)
{
	CMeshCollider *pMeshCollder = NULL;   // ���b�V���R���C�_�[�N���X�^�̃|�C���^
	if (pMeshCollder == NULL)
	{// ����������ɂȂ��Ă���
		pMeshCollder = new CMeshCollider(nPriority);
		if (pMeshCollder != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̑��
			pMeshCollder->SetPos(pos);         // ���b�V���R���C�_�[�̍��W
			pMeshCollder->SetRot(rot);         // ���b�V���R���C�_�[�̌���
			pMeshCollder->SetWidth(fWidth);    // 1�u���b�N���̕�
			pMeshCollder->SetHeight(fHeight);  // 1�u���b�N���̍���
			pMeshCollder->SetXBlock(nXBlock);  // ���̕�����
			pMeshCollder->SetZBlock(nZBlock);  // ���s�̕�����

			if (FAILED(pMeshCollder->Init()))
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

	return pMeshCollder;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMeshCollider::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);   // ���_�����v�Z

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
void CMeshCollider::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �����[�X����
	CScene::Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMeshCollider::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMeshCollider::Draw(void)
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
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CMeshCollider::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
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
void CMeshCollider::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �����ɕK�v�ȕϐ���錾
	float XPos = -(m_fWidth * m_nXBlock) / 2;  // X���W�����ɐݒ�
	float ZPos = (m_fHeight * m_nZBlock) / 2;  // Z���W�����ɐݒ�
	float YPos = 0.0f;                         // Y���W

	for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
	{// ���������̕����� + 1�����J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// ���������̕����� + 1�����J��Ԃ�
		    // ���_���W
			pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

			XPos += m_fWidth;  // X���W�����ɐi�߂�
		}
		XPos -= m_fWidth * (m_nXBlock + 1);     // X���W��i�߂Ă������߂�
		ZPos -= m_fHeight;                      // Z���W����O�ɐi�߂�

		// �|�C���^��i�߂�
		pVtx += m_nXBlock + 1;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    ���_�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshCollider::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    ���b�V���t�B�[���h�̍��W�ݒ菈��
//=============================================================================
void CMeshCollider::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ���b�V���t�B�[���h�̌����ݒ菈��
//=============================================================================
void CMeshCollider::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    1�u���b�N���̕��ݒ菈��
//=============================================================================
void CMeshCollider::SetWidth(const float fWidth)
{
	// ����ݒ�
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	 // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float XPos = -(m_fWidth * m_nXBlock) / 2; // X���W�����ɐݒ�

		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].pos.x = XPos;

				XPos += m_fWidth;   // X���W�����ɐi�߂�
			}
			XPos -= m_fWidth * (m_nXBlock + 1); // X���W��i�߂Ă������߂�

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1�u���b�N���̍����ݒ菈��
//=============================================================================
void CMeshCollider::SetHeight(const float fHeight)
{
	// ������ݒ�
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	 // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float ZPos = (m_fHeight * m_nZBlock) / 2; // Z���W�����ɐݒ�

		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].pos.z = ZPos;
			}
			ZPos -= m_fHeight;   // Z���W����O�ɐi�߂�

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ���̕������ݒ菈��
//=============================================================================
void CMeshCollider::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_�o�b�t�@�̔j��
		DIRECT_RELEASE(m_pVtxBuff);

		// �����_�����O�N���X�^�̃|�C���^
		CRenderer *pRenderer = CManager::GetRenderer();

		if (pRenderer != NULL)
		{// �����_�����O�N���X����������Ă���
		    // �f�o�C�X�̎擾
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
			if (pDevice != NULL)
			{// �f�o�C�X���擾�ł���
		    	// �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
				m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1); // ���_�����v�Z

				// ���_�o�b�t�@�̐���
				pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&m_pVtxBuff,
					NULL);

				// ���_���̐ݒ�
				VERTEX_3D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// �����ɕK�v�ȕϐ���錾
				float XPos = -(m_fWidth * m_nXBlock) / 2;  // X���W�����ɐݒ�
				float ZPos = (m_fHeight * m_nZBlock) / 2;  // Z���W�����ɐݒ�
				float YPos = 0.0f;                         // Y���W

				for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
				{// ���������̕����� + 1�����J��Ԃ�
					for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
					{// ���������̕����� + 1�����J��Ԃ�
					    // ���_���W
						pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

						XPos += m_fWidth;  // X���W�����ɐi�߂�
					}
					XPos -= m_fWidth * (m_nXBlock + 1);     // X���W��i�߂Ă������߂�
					ZPos -= m_fHeight;                      // Z���W����O�ɐi�߂�

					// �|�C���^��i�߂�
					pVtx += m_nXBlock + 1;
				}

				// ���_�o�b�t�@���A�����b�N����
				m_pVtxBuff->Unlock();
			}
		}
	}
}

//=============================================================================
//    ���s�̕������ݒ菈��
//=============================================================================
void CMeshCollider::SetZBlock(const int nZBlock)
{
	m_nZBlock = nZBlock;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_�o�b�t�@�̔j��
		DIRECT_RELEASE(m_pVtxBuff);

		// �����_�����O�N���X�^�̃|�C���^
		CRenderer *pRenderer = CManager::GetRenderer();

		if (pRenderer != NULL)
		{// �����_�����O�N���X����������Ă���
		    // �f�o�C�X�̎擾
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
			if (pDevice != NULL)
			{// �f�o�C�X���擾�ł���
			    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
				m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);     // ���_�����v�Z

				// ���_�o�b�t�@�̐���
				pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&m_pVtxBuff,
					NULL);

				// ���_���̐ݒ�
				VERTEX_3D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// �����ɕK�v�ȕϐ���錾
				float XPos = -(m_fWidth * m_nXBlock) / 2;  // X���W�����ɐݒ�
				float ZPos = (m_fHeight * m_nZBlock) / 2;  // Z���W�����ɐݒ�
				float YPos = 0.0f;                         // Y���W

				for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
				{// ���������̕����� + 1�����J��Ԃ�
					for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
					{// ���������̕����� + 1�����J��Ԃ�
					    // ���_���W
						pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

						XPos += m_fWidth;   // X���W�����ɐi�߂�
					}
					XPos -= m_fWidth * (m_nXBlock + 1);   // X���W��i�߂Ă������߂�
					ZPos -= m_fHeight;                    // Z���W����O�ɐi�߂�

					// �|�C���^��i�߂�
					pVtx += m_nXBlock + 1;
				}

				// ���_�o�b�t�@���A�����b�N����
				m_pVtxBuff->Unlock();
			}
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CMeshCollider::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���_�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshCollider::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    ���b�V���R���C�_�[�̍��W�擾����
//=============================================================================
D3DXVECTOR3 CMeshCollider::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ���b�V���R���C�_�[�̌����擾����
//=============================================================================
D3DXVECTOR3 CMeshCollider::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    1�u���b�N���̕��擾����
//=============================================================================
float CMeshCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1�u���b�N���̍����擾����
//=============================================================================
float CMeshCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���̕������擾����
//=============================================================================
int CMeshCollider::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    ���s�̕������擾����
//=============================================================================
int CMeshCollider::GetZBlock(void)
{
	return m_nZBlock;
}

//=============================================================================
//    ���_���擾����
//=============================================================================
int CMeshCollider::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CMeshCollider::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ���b�V���t�B�[���h�̃��[���h���W�ɂ����鍂���擾���鏈��
//=============================================================================
float CMeshCollider::GetPolyHeight(D3DXVECTOR3 pos, bool *pLand, D3DXVECTOR3 *pPolyNormal)
{
	float fHeight = 0.0f;               // ����(�ŏI�I�ɂ��̒l��Ԃ�)
	float fRivision = 0.0f;             // �����̕␳�l(�@��������o���̂�0.0�̍����ɂ��낦�邽��)
	int nVertexIdxStart = 0;            // ����̒��_�ԍ�(���[�v���邽�тɂ���Ă���)
	D3DXMATRIX mtxRot, mtxTrans;        // �v�Z�p���[���h�}�g���b�N�X
	D3DXMATRIX MtxWorld;                // �n�ʂ̃��[���h�}�g���b�N�X
	D3DXMATRIX VertexMtxWorld[4];       // ���_�̃��[���h�}�g���b�N�X4��(��],�ʒu���܂߂Čv�Z����)
	D3DXVECTOR3 VertexPos[4];           // ���_�̍��W4��(���̍��W�̓��[�J�����W�Ȃ̂Ōv�Z�ɂ͎g��Ȃ�)
	D3DXVECTOR3 VertexWorldPos[4];      // ���_�̃��[���h���W4��(���[���h�}�g���b�N�X���甲���o��)
	D3DXVECTOR3 VertexLeftWorldPos[3];  // �����̃|���S���̒��_�̃��[���h���W3��(���[���h�}�g���b�N�X���甲���o��)
	D3DXVECTOR3 VertexRightWorldPos[3]; // �E���̃|���S���̒��_�̃��[���h���W3��(���[���h�}�g���b�N�X���甲���o��)
	D3DXVECTOR3 PolygonVerPos[3];       // �|���S���̍��W(����p�̖@���𓱂����߂ɕK�v)
	D3DXVECTOR3 PolygonNormal;          // �|���S���̖@��(����p�̖@���Ȃ̂Ō����ڂ̖@���Ƃ͈قȂ�)
	D3DXVECTOR3 PolygonVector[2];       // �|���S���̖@��������o�����߂̃x�N�g��
	D3DXVECTOR3 VecPosToPolygon;        // �����̍��W�ƃ|���S���Ƃ̃x�N�g��
	D3DXVECTOR3 VectorVerToVer;         // �O�σx�N�g��1�{��(���_���璸�_�ւ̃x�N�g��)
	D3DXVECTOR3 VectorVerToPos;         // �O�σx�N�g��2�{��(���_��������̍��W�ւ̃x�N�g��)
	int nCntLeftPolygon = 0;            // �����̃|���S���̃x�N�g�������ʂ�����
	int nCntRightPolygon = 0;           // �E���̃|���S���̃x�N�g�������ʂ�����

										// �t�B�[���h���ɂ��邩�ǂ�������
	if (pos.x <= GetPos().x - (GetWidth() * GetXBlock()) || pos.x >= GetPos().x + (GetWidth() * GetXBlock())
		&& pos.z <= GetPos().z - (GetHeight() * GetZBlock()) || pos.z >= GetPos().z + (GetHeight() * GetZBlock()))
	{
		if (pLand != NULL)
		{
			*pLand = false;
		}
		return 0.0f;
	}

	// �n�ʂ̃��[���h�}�g���b�N�X���擾���Ă���
	MtxWorld = GetMtxWorld();

	// ���_�f�[�^�ւ̃|�C���^��錾
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł���
	 // ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
		{// ���s�̕��������J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
			{// ���̕��������J��Ԃ�
			 // ���_�̃��[�J�����W�𔲂��o��
				VertexPos[0] = pVtx[nVertexIdxStart].pos;
				VertexPos[1] = pVtx[nVertexIdxStart + 1].pos;
				VertexPos[2] = pVtx[nVertexIdxStart + (m_nXBlock + 1)].pos;
				VertexPos[3] = pVtx[nVertexIdxStart + (m_nXBlock + 1) + 1].pos;

				// ���_�̃��[���h���W�𔲂��o��
				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// 1�u���b�N���̒��_�����J��Ԃ�
					D3DXVec3TransformCoord(&VertexWorldPos[nCntVer], &VertexPos[nCntVer], &MtxWorld);
				}

				// �����̃|���S���̔���J�n
				// �����̃|���S���̍��W��ݒ�
				VertexLeftWorldPos[0] = VertexWorldPos[0];
				VertexLeftWorldPos[1] = VertexWorldPos[3];
				VertexLeftWorldPos[2] = VertexWorldPos[2];
				for (int nCntLeft = 0; nCntLeft < 3; nCntLeft++)
				{// �|���S���̒��_�����J��Ԃ�
				 // �O�ς̃x�N�g�������o��
					VectorVerToVer = VertexLeftWorldPos[(nCntLeft + 1) % 3] - VertexLeftWorldPos[nCntLeft];
					VectorVerToPos = pos - VertexLeftWorldPos[nCntLeft];

					if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
					{// �����̍��W�����_�ƒ��_�̃x�N�g���̉E���ɂ���
						nCntLeftPolygon++;  // �����ʂ����񐔂�i�߂�
					}
				}
				if (nCntLeftPolygon == 3)
				{// �O�ς̔����S�Ēʂ���
				 // ����p�̒��_���W�����
					PolygonVerPos[0] = D3DXVECTOR3(VertexLeftWorldPos[2].x, VertexLeftWorldPos[2].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[2].z);
					PolygonVerPos[1] = D3DXVECTOR3(VertexLeftWorldPos[1].x, VertexLeftWorldPos[1].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[1].z);
					PolygonVerPos[2] = D3DXVECTOR3(VertexLeftWorldPos[0].x, VertexLeftWorldPos[0].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[0].z);

					// �O�σx�N�g�������
					PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
					PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

					// �O�σx�N�g������@��������o��
					D3DXVec3Cross(&PolygonNormal, &PolygonVector[0], &PolygonVector[1]);
					D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

					// ����̎d����0�ɖ߂��������Ōv�Z���Ă���̂ŕ␳�����邽�߂ɒl��ێ�
					fRivision = VertexLeftWorldPos[2].y;

					// �����̍��W�ƒ��_���W�̃x�N�g�����v�Z
					VecPosToPolygon = pos - PolygonVerPos[0];

					// ���ς̊T�O�𗘗p���č���������o��
					float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X�����̓��ς��v�Z
					float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z�����̓��ς��v�Z
					float Dot = -DotX - DotZ;                               // X������Z�����̓��ϓ��m������
					if (PolygonNormal.y != 0.0f)
					{
						fHeight = (Dot / PolygonNormal.y) + fRivision;          // ���������̂�Y�����̖@���Ŋ����ĕ␳�l�𑫂��Ă�����
					}
					else
					{
						return 0.0f;
					}

					// ��������p�x�����߂�
					//float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
					//float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
					//float DotY = (PolygonNormal.y * fHeight);
					//float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
					//float fAngle = acosf(cosSita);

					if (pLand != NULL)
					{
						*pLand = true;
					}
					if (pPolyNormal != NULL)
					{
						*pPolyNormal = PolygonNormal;
					}
				}

				// �E���̃|���S���̔���J�n
				// �E���̃|���S���̍��W��ݒ�
				VertexRightWorldPos[0] = VertexWorldPos[0];
				VertexRightWorldPos[1] = VertexWorldPos[1];
				VertexRightWorldPos[2] = VertexWorldPos[3];
				for (int nCntRight = 0; nCntRight < 3; nCntRight++)
				{// �|���S���̒��_�����J��Ԃ�
				 // �O�ς̃x�N�g�������o��
					VectorVerToVer = VertexRightWorldPos[(nCntRight + 1) % 3] - VertexRightWorldPos[nCntRight];
					VectorVerToPos = pos - VertexRightWorldPos[nCntRight];

					if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
					{// �����̍��W�����_�ƒ��_�̃x�N�g���̉E���ɂ���
						nCntRightPolygon++;  // �����ʂ����񐔂�i�߂�
					}
				}
				if (nCntRightPolygon == 3)
				{// �O�ς̔����S�Ēʂ���
				 // ����p�̒��_���W�����
					PolygonVerPos[0] = D3DXVECTOR3(VertexRightWorldPos[1].x, VertexRightWorldPos[1].y - VertexRightWorldPos[1].y, VertexRightWorldPos[1].z);
					PolygonVerPos[1] = D3DXVECTOR3(VertexRightWorldPos[0].x, VertexRightWorldPos[0].y - VertexRightWorldPos[1].y, VertexRightWorldPos[0].z);
					PolygonVerPos[2] = D3DXVECTOR3(VertexRightWorldPos[2].x, VertexRightWorldPos[2].y - VertexRightWorldPos[1].y, VertexRightWorldPos[2].z);

					// �O�σx�N�g�������
					PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
					PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

					// �O�σx�N�g������@��������o��
					D3DXVec3Cross(&PolygonNormal, &PolygonVector[1], &PolygonVector[0]);
					D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

					// ����̎d����0�ɖ߂��������Ōv�Z���Ă���̂ŕ␳�����邽�߂ɒl��ێ�
					fRivision = VertexRightWorldPos[1].y;

					// �����̍��W�ƒ��_���W�̃x�N�g�����v�Z
					VecPosToPolygon = pos - PolygonVerPos[0];

					// ���ς̊T�O�𗘗p���č���������o��
					float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X�����̓��ς��v�Z
					float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z�����̓��ς��v�Z
					float Dot = -DotX - DotZ;                               // X������Z�����̓��ϓ��m������
					if (PolygonNormal.y != 0.0f)
					{
						fHeight = (Dot / PolygonNormal.y) + fRivision;          // ���������̂�Y�����̖@���Ŋ����ĕ␳�l�𑫂��Ă�����
					}
					else
					{
						return 0.0f;
					}

					// ��������p�x�����߂�
					//float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
					//float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
					//float DotY = (PolygonNormal.y * fHeight);
					//float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
					//float fAngle = acosf(cosSita);

					if (pLand != NULL)
					{
						*pLand = true;
					}
					if (pPolyNormal != NULL)
					{
						*pPolyNormal = PolygonNormal;
					}
				}

				// ����p�̕ϐ������������Ă���
				if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
				{// �|���S���ɏ���Ă���Ɣ��肳��Ă���
					break;
				}
				else
				{// �܂��|���S���ɏ���Ă��Ȃ��Ɣ��肳��Ă���
					nCntLeftPolygon = 0;
					nCntRightPolygon = 0;
					nVertexIdxStart++;
				}
			}
			if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
			{// �|���S���ɏ���Ă���Ɣ��肳��Ă���
				break;
			}
			else
			{// �܂��|���S���ɏ���Ă��Ȃ��Ɣ��肳��Ă���
			 // �E�[�̃u���b�N�ɔ��肷��ׂ��|���S���͑��݂��Ȃ��̂Ŕԍ������炵�Ă���
				nVertexIdxStart++;
			}
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}

	return fHeight;
}

//=============================================================================
//    �n�ʂɊђʂ��Ă��邩���肷�鏈��
//=============================================================================
bool CMeshCollider::CheckPenetration(D3DXVECTOR3 pos, int *pXBlock, int *pYBlock, int *pLeftVerIndex)
{
	bool bPene = false;               // �ђʂ��Ă��邩�ǂ���
	int nCntVertexIndex = 0;          // ����̒��_�ԍ�(���[�v���邽�тɑ���)
	D3DXVECTOR3 PolygonPos[4];        // �|���S���̒��_���W
	D3DXVECTOR3 PolygonWorldPos[4];   // �|���S���̃��[���h��Ԃɂ����钸�_���W
	D3DXVECTOR3 VecA, VecB;           // �O�όv�Z�p�x�N�g��
	int nCntCheck = 0;                // �����ʂ����񐔂𐔂���
	D3DXCOLOR VerCol;                 // ���_�J���[�ۑ��p

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtxBuff != NULL)
	{
		for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
		{// ���s�̕��������J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
			{// ���̕��������J��Ԃ�
				pVtx[nCntVertexIndex].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[nCntVertexIndex + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[nCntVertexIndex + (m_nXBlock + 1) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[nCntVertexIndex + (m_nXBlock + 1)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


				// �|���S���̃��[�J�����W��c��
				PolygonPos[0] = pVtx[nCntVertexIndex].pos;
				PolygonPos[1] = pVtx[nCntVertexIndex + 1].pos;
				PolygonPos[2] = pVtx[nCntVertexIndex + (m_nXBlock + 1) + 1].pos;
				PolygonPos[3] = pVtx[nCntVertexIndex + (m_nXBlock + 1)].pos;

				// �|���S���̃��[���h��Ԃɂ����钸�_���W�����߂�
				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// �|���S���̒��_�����J��Ԃ�
					D3DXVec3TransformCoord(&PolygonWorldPos[nCntVer], &PolygonPos[nCntVer], &m_MtxWorld);
				}

				// �|���S�������Ɋђʓ_�����邩�O�όv�Z�Ŕ���
				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// �|���S���̒��_�����J��Ԃ�
					VecA = pos - PolygonPos[nCntVer];
					VecB = PolygonPos[(nCntVer + 1) % 4] - PolygonPos[nCntVer];
					if ((VecA.x * VecB.z) - (VecA.z * VecB.x) > 0)
					{// ���Ɋђʓ_������
						nCntCheck++;
						if (nCntCheck == 4)
						{// �S�Ă̔����ʉ߂���
							bPene = true;
							if (pXBlock != NULL)
							{
								*pXBlock = nCntH + 1;
							}
							if (pYBlock != NULL)
							{
								*pYBlock = nCntV + 1;
							}
							if (pLeftVerIndex != NULL)
							{
								*pLeftVerIndex = nCntVertexIndex;
							}
						}
					}
				}
				nCntVertexIndex++;
				nCntCheck = 0;
			}
			nCntVertexIndex++;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();


	return bPene;
}