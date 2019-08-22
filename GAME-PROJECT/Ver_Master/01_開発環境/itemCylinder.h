//*****************************************************************************
//
//     アイテム用の円筒処理[itemCylinder.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ITEMCYLINDER_H_
#define _ITEMCYLINDER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "meshCylinder.h"

//*****************************************************************************
//    アイテム用の円筒クラスの定義
//*****************************************************************************
class CItemCylinder : public CMeshCylinder
{
public:    // 誰でもアクセス可能
	typedef enum
	{
		STATE_NONE = 0,
		STATE_DEATH,
		STATE_MAX
	}STATE;

	CItemCylinder(int nPriority = 3, OBJTYPE objType = OBJTYPE_ITEMCYLINDER);
	~CItemCylinder();

	static CItemCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetState(const STATE state);
	STATE GetState(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Normal(void);
	void Death(void);

	STATE m_State;    // 状態
};

#endif