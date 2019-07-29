//*****************************************************************************
//
//     空の処理[sky.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SKY_H_
#define _SKY_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "meshSphere.h"

//*****************************************************************************
//    空クラスの定義
//*****************************************************************************
class CSky : public CMeshSphere
{
public:    // 誰でもアクセス可能
	CSky(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHSPHERE);
	~CSky();
	static CSky *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nTexIdx, float fRollSpeed, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTexIdx(const int nTexIdx);
	void SetRollSpeed(const float fRollSpeed);

	int GetTexIdx(void);
	float GetRollSpeed(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int m_nTexIdx;       // 使用しているテクスチャ番号
	float m_fRollSpeed;  // 回転するスピード
};

#endif