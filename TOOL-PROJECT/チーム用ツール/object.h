//=============================================================================
//
// 配置物の処理 [object.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "sceneX.h"

#define MAX_OBJECT (10)

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CObject : public CSceneX
{
public:	//誰からもアクセス可能
	//*************************************************************************
	// 構造体定義
	//*************************************************************************
	typedef struct
	{
		int nType;				//種類
		D3DXVECTOR3 pos;		//位置
		D3DXVECTOR3 rot;		//向き
		D3DXVECTOR3 size;		//大きさ
		bool		bHit;		//当たり判定
		bool		bDestroy;	//破壊できるかどうか
	}OBJECT_INFO;

	//メンバ関数
	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove/*, float fRad*/);
	void SetType(const int nType);
	int GetType(void);

	//静的メンバ関数
	static CObject *Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 size,bool bHit,bool bDestory);

private:	//自分だけがアクセス可能
	//メンバ変数
	int							m_nNumModel;							//オブジェクトの総数
	int							m_nType;								//種類
};
#endif