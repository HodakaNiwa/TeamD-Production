//=============================================================================
//
// 配置物の処理 [object.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "loadfilefunction.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CObject::CObject(int nPrioriry, OBJTYPE objtype) : CSceneX(nPrioriry, objtype)
{

}
//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
// オブジェクトXの生成
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 size,bool bHit,bool bDestroy)
{
	CObject *pObject = NULL;		//配置物のポインタ

	if (pObject == NULL)
	{
		pObject = new CObject;		//動的確保
		if (pObject != NULL)
		{
			pObject->SetPos(pos);	//位置の設置処理
			pObject->SetRot(rot);	//向きの設置処理
			pObject->SetSize(size);	//大きさの設置処理
			pObject->SetHit(bHit);	//当たり判定の設置処理
			pObject->SetDestroy(bDestroy); //破壊できるかどうかの設置処理
			pObject->Init();		//初期化処理
		}
	}
	return pObject;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject::Init(void)
{
	//初期化処理
	CSceneX::Init();
	//オブジェクトの種類設定
	SetObjType(OBJTYPE_OBJECT);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject::Uninit(void)
{
	CSceneX::Uninit();
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObject::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CObject::Draw(void)
{
	//描画処理
	CSceneX::Draw();
}


//=============================================================================
// 配置物の当たり判定
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pMove/*, float fRad*/)
{
	bool bLand = false;	//乗っているかどうか

	D3DXVECTOR3 pos = GetPos();				//位置の取得
	D3DXVECTOR3 vtxMax = GetVtxMax();		//頂点座標最大値の取得
	D3DXVECTOR3 vtxMin = GetVtxMin();		//頂点座標最小値の取得

	if (pPos->x >= pos.x + vtxMin.x&&
		pPos->x <= pos.x + vtxMax.x &&
		pPos->z >= pos.z + vtxMin.z &&
		pPos->z <= pos.z + vtxMax.z)
	{
		if (pPosOld->x <= pos.x + vtxMin.x&&
			pPos->x >= pos.x + vtxMin.x)
		{//左からめり込んだ場合
			bLand = true;
			pPos->x = pos.x + vtxMin.x;
		}
		else if (pPosOld->x >= pos.x + vtxMax.x&&
			pPos->x <= pos.x + vtxMax.x)
		{//右からめり込んだ
			bLand = true;
			pPos->x = pos.x + vtxMax.x;

		}
		else if (pPosOld->z <= pos.z + vtxMin.z &&
			pPos->z >= pos.z + vtxMin.z)
		{//前からめり込んだ
			bLand = true;
			pPos->z = pos.z + vtxMin.z;

		}
		else if (pPosOld->z >= pos.z + vtxMax.z &&
			pPos->z <= pos.z + vtxMax.z)
		{//後からめり込んだ
			bLand = true;
			pPos->z = pos.z + vtxMax.z;

		}
	}
	return bLand;
}

//=============================================================================
// 種類の設置処理
//=============================================================================
void CObject::SetType(int nType)
{
	m_nType = nType;
}

//=============================================================================
// 種類の取得処理
//=============================================================================
int CObject::GetType(void)
{
	return m_nType;
}
