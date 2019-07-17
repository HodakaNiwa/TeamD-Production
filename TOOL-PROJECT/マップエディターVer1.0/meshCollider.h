//*****************************************************************************
//
//     ���b�V���R���C�_�[�̏���[meshCollider.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHCOLLIDER_H_
#define _MESHCOLLIDER_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMeshCollider : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMeshCollider(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHFIELD);
	~CMeshCollider();

	static CMeshCollider *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nXBlock, int nZBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetXBlock(void);
	int GetZBlock(void);
	int GetNumVertex(void);
	D3DXMATRIX GetMtxWorld(void);

	float GetPolyHeight(D3DXVECTOR3 pos, bool *pLand, D3DXVECTOR3 *pPolyNormal);
	bool CheckPenetration(D3DXVECTOR3 pos, int *pXBlock = NULL, int *pYBlock = NULL, int *pLeftVerIndex = NULL);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3             m_Pos;                    // ���b�V���t�B�[���h�̍��W
	D3DXVECTOR3             m_Rot;                    // ���b�V���t�B�[���h�̌���
	float                   m_fWidth;                 // 1�u���b�N���̕�
	float                   m_fHeight;                // 1�u���b�N������
	int                     m_nXBlock;                // ���̕�����
	int                     m_nZBlock;                // ���s�̕�����
	int                     m_nNumVertex;             // ���_��
	D3DXMATRIX              m_MtxWorld;               // ���[���h�}�g���b�N�X
};

#endif