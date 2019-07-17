//*****************************************************************************
//
//     配置物の処理[object.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "object3D.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CModel;
class CMotionManager;

//*****************************************************************************
//    配置物クラスの定義
//*****************************************************************************
class CObject : public CObject3D
{
public:    // 誰でもアクセス可能
	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ModelDraw(void);

	void SetType(const int nType);
	void SetNumModel(const int nNumModel);
	void SetModel(CModel **apModel);
	void SetModel(CModel *pModel, int nIdx);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetAlpha(const float fAlpha);

	int GetType(void);
	int GetNumModel(void);
	CModel **GetModel(void);
	CModel *GetModel(int nIdx);
	CMotionManager *GetMotionManager(void);
	float GetAlpha(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void ReleaseModel(void);
	void ReleaseMotionManager(void);

	int            m_nType;            // 種類番号
	int            m_nNumModel;        // 使用しているモデルの数
	CModel         **m_apModel;        // モデルクラスへのポインタ
	CMotionManager *m_pMotionManager;  // モーション管轄クラスへのポインタ
	float          m_fAlpha;           // 配置物の透明度
};

#endif