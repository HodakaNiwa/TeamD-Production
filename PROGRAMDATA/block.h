//*****************************************************************************
//
//     �u���b�N�̏���[block.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    �u���b�N�N���X�̒�`
//*****************************************************************************
class CBlock : public CObject3D
{
public:    // �N�ł��A�N�Z�X�\
	CBlock(int nPriority = 3, OBJTYPE objType = OBJTYPE_3D);
	~CBlock();

	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax = INITIALIZE_D3DXVECTOR3, D3DXVECTOR3 VtxMin = INITIALIZE_D3DXVECTOR3);

	void SetMesh(const LPD3DXMESH pMesh);
	void SetBuffMat(const LPD3DXBUFFER pBuffMat);
	void SetNumMat(const DWORD nNumMat);
	void SetTexture(LPDIRECT3DTEXTURE9 *pTexture);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetAlpha(const float fAlpha);

	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	LPDIRECT3DTEXTURE9 *GetTexture(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);
	float GetAlpha(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPD3DXMESH          m_pMesh;        // ���b�V���ւ̃|�C���^
	LPD3DXBUFFER        m_pBuffMat;     // �}�e���A�����ւ̃|�C���^
 	DWORD               m_nNumMat;      // �}�e���A�����̐�
	LPDIRECT3DTEXTURE9  *m_pTexture;    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3         m_VtxMax;       // �ő�̒��_���W
	D3DXVECTOR3         m_VtxMin;       // �ŏ��̒��_���W
	float               m_fAlpha;       // ���f���̓����x
};

#endif