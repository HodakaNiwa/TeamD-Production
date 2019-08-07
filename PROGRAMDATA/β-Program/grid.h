//*****************************************************************************
//
//     �O���b�h���̏���[grid.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GRID_H_
#define _GRID_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���b�h���N���X�̒�`
//*****************************************************************************
class CGrid
{
public:    // �N�ł��A�N�Z�X�\
	CGrid();
	~CGrid();

	static CGrid *Create(D3DXVECTOR3 pos, float fWidth, float fDepth, int nXBlock, int nZBlock);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(const D3DXVECTOR3 pos);
	void SetWidth(const float fWidth);
	void SetDepth(const float fDepth);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);

	D3DXVECTOR3 GetPos(void);
	float GetWidth(void);
	float GetDepth(void);
	int GetXBlock(void);
	int GetZBlock(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void MakeVertex(void);

	D3DXVECTOR3             m_Pos;          // ���W
	float                   m_fWidth;       // 1�}�X���̉���
	float                   m_fDepth;       // 1�}�X���̉��s
	int                     m_nXBlock;      // ���̕�����
	int                     m_nZBlock;      // ���s�̕�����
	D3DXVECTOR3             *m_pVecPos;     // ���_���W
	int                     *m_pIndex;      // ���_�ԍ�
	ID3DXLine*              m_pLine;        // ���`��p�C���X�^���X�ւ̃|�C���^
	int                     m_nNumVertex;   // ���_���W�̐�
	int                     m_nNumIdx;      // �C���f�b�N�X�ԍ���
};

#endif