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
	//-------------------
	//   ���
	//-------------------
	typedef enum
	{
		TYPE_NONE = -1,     // ����
		TYPE_MAX
	}TYPE;

	CBlock(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlock();

	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, bool bBreak, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(CScene *pScene);
	void BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	void SetType(const int nType);
	void SetModelIdx(const int nModelIdx);
	void SetMesh(const LPD3DXMESH pMesh);
	void SetBuffMat(const LPD3DXBUFFER pBuffMat);
	void SetNumMat(const DWORD nNumMat);
	void SetTexture(LPDIRECT3DTEXTURE9 *pTexture);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetAlpha(const float fAlpha);
	void SetBreak(const bool bBreak);

	int GetType(void);
	int GetModelIdx(void);
	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	LPDIRECT3DTEXTURE9 *GetTexture(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);
	float GetAlpha(void);
	bool GetBreak(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);

	int                 m_nType;        // ��ޔԍ�
	int                 m_nModelIdx;    // �g�p���郂�f���̔ԍ�
	LPD3DXMESH          m_pMesh;        // ���b�V���ւ̃|�C���^
	LPD3DXBUFFER        m_pBuffMat;     // �}�e���A�����ւ̃|�C���^
	DWORD               m_nNumMat;      // �}�e���A�����̐�
	LPDIRECT3DTEXTURE9  *m_pTexture;    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3         m_VtxMax;       // �ő�̒��_���W
	D3DXVECTOR3         m_VtxMin;       // �ŏ��̒��_���W
	float               m_fAlpha;       // ���f���̓����x
	bool                m_bBreak;       // �󂹂邩�ǂ���
};
#endif