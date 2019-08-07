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
		TYPE_NONE = -1,               // ����
		TYPE_BREAK_BULLET_NOT_DIRTY,  // �e�ŉ󂹂ĉ���Ȃ�
		TYPE_BREAK_TANK_DIRTY,        // �^���N�ŉ󂹂ĉ����
		TYPE_NOT_BREAK_NOT_DIRTY,     // �󂹂Ȃ�������Ȃ�
		TYPE_NOT_BREAK_TANK_DIRTY,    // �󂹂Ȃ����^���N�������
		TYPE_MAX
	}TYPE;

	CBlock(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlock();

	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CreateBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth, bool bReturnFlag = true);
	virtual void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
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

	int GetType(void);
	int GetModelIdx(void);
	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	LPDIRECT3DTEXTURE9 *GetTexture(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetVtxMin(void);
	float GetAlpha(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int                 m_nType;        // ��ޔԍ�
	int                 m_nModelIdx;    // �g�p���郂�f���̔ԍ�
	LPD3DXMESH          m_pMesh;        // ���b�V���ւ̃|�C���^
	LPD3DXBUFFER        m_pBuffMat;     // �}�e���A�����ւ̃|�C���^
	DWORD               m_nNumMat;      // �}�e���A�����̐�
	LPDIRECT3DTEXTURE9  *m_pTexture;    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3         m_VtxMax;       // �ő�̒��_���W
	D3DXVECTOR3         m_VtxMin;       // �ŏ��̒��_���W
	float               m_fAlpha;       // ���f���̓����x
};

//*****************************************************************************
//    �^�C�v0�̃u���b�N�N���X�̒�`
//*****************************************************************************
class CBlockType0 : public CBlock
{
public:    // �N�ł��A�N�Z�X�\
	CBlockType0(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlockType0();

	static CBlockType0 *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
//    �^�C�v1�̃u���b�N�N���X�̒�`
//*****************************************************************************
class CBlockType1 : public CBlock
{
public:    // �N�ł��A�N�Z�X�\
	CBlockType1(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlockType1();

	static CBlockType1 *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateSplash(void);
};

//*****************************************************************************
//    �^�C�v2�̃u���b�N�N���X�̒�`
//*****************************************************************************
class CBlockType2 : public CBlock
{
public:    // �N�ł��A�N�Z�X�\
	CBlockType2(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlockType2();

	static CBlockType2 *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
//    �^�C�v3�̃u���b�N�N���X�̒�`
//*****************************************************************************
class CBlockType3 : public CBlock
{
public:    // �N�ł��A�N�Z�X�\
	CBlockType3(int nPriority = 3, OBJTYPE objType = OBJTYPE_BLOCK);
	~CBlockType3();

	static CBlockType3 *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth = 75.0f, float fBoxHeight = 75.0f, float fBoxDepth = 75.0f, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void RemakeBoxCollider(float fBoxWidth, float fBoxHeight, float fBoxDepth);
	void Hit(CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateSplash(void);
};

#endif