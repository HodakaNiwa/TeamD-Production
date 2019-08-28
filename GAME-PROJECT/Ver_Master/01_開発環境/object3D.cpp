//=============================================================================
//
// オブジェクト3D処理 [object3D.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "object3D.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "model.h"
#include "boxCollider.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
//CBoxCollider *CObject3D::m_pBoxCollider = NULL;

//=============================================================================
// オブジェクト3Dのコンストラクタ
//=============================================================================
CObject3D::CObject3D(int nPriority,OBJTYPE type) : CScene(nPriority,type)
{
	m_pos = INITIALIZE_D3DXVECTOR3;				//位置
	m_rot = INITIALIZE_D3DXVECTOR3;				//向き
	m_colRange = INITIALIZE_D3DXVECTOR3;		//当たり判定の大きさ
	m_bReturnFlag = true;						//当たった際に戻すかどうか
	m_pBoxCollider = NULL;
}
//=============================================================================
// オブジェクト3Dのデストラクタ
//=============================================================================
CObject3D::~CObject3D()
{

}
//=============================================================================
// オブジェクト3Dの生成
//=============================================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObject3D *pObject3D = NULL;

	if (pObject3D == NULL)
	{//NULLの場合
		pObject3D = new CObject3D;	//動的確保
		if (pObject3D != NULL)
		{//NULLでない場合
			pObject3D->SetPos(pos);			//位置の設置処理
			pObject3D->SetRot(rot);			//向きの設置処理
			pObject3D->Init();				//初期化処理
		}
	}
	return pObject3D;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject3D::Init()
{
	//位置の取得処理
	D3DXVECTOR3 pos = GetPos();

	//向きの取得処理
	D3DXVECTOR3 rot = GetRot();

	//当たり判定箱の生成処理
	if (m_pBoxCollider == NULL)
	{
		m_pBoxCollider = CBoxCollider::Create(pos, m_colRange.x, m_colRange.y, m_colRange.z,m_bReturnFlag);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CObject3D::Uninit(void)
{
	//当たり判定箱の終了処理
	if (m_pBoxCollider != NULL)
	{
		m_pBoxCollider->Uninit();
		delete m_pBoxCollider;
		m_pBoxCollider = NULL;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObject3D::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CObject3D::Draw(void)
{
	//当たり判定箱の描画処理
	if (m_pBoxCollider != NULL)
	{
		m_pBoxCollider->Draw();
	}
}

//=============================================================================
// 位置の設置処理
//=============================================================================
void CObject3D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// 向きの設置処理
//=============================================================================
void CObject3D::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// 当たり判定箱の設置処理
//=============================================================================
void CObject3D::SetBoxCollider(CBoxCollider *pBoxCollider)
{
	m_pBoxCollider = pBoxCollider;
}

//=============================================================================
// 当たり判定の大きさの設置処理
//=============================================================================
void CObject3D::SetColRange(D3DXVECTOR3 colRange)
{
	m_colRange = colRange;
}

//=============================================================================
// 当たった際戻すかどうかの設置処理
//=============================================================================
void CObject3D::SetReturnFlag(bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
// ワールドマトリックスの計算処理
//=============================================================================
void CObject3D::SetMtxWorld(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転行列を作成(D3DXMatrixRotationYawPitchRoll参照)
	float fSinPitch = sinf(m_rot.x);
	float fCosPitch = cosf(m_rot.x);
	float fSinYaw = sinf(m_rot.y);
	float fCosYaw = cosf(m_rot.y);
	float fSinRoll = sinf(m_rot.z);
	float fCosRoll = cosf(m_rot.z);
	mtxRot._11 = fSinRoll * fSinPitch * fSinYaw + fCosRoll * fCosYaw;
	mtxRot._12 = fSinRoll * fCosPitch;
	mtxRot._13 = fSinRoll * fSinPitch * fCosYaw - fCosRoll * fSinYaw;
	mtxRot._21 = fCosRoll * fSinPitch * fSinYaw - fSinRoll * fCosYaw;
	mtxRot._22 = fCosRoll * fCosPitch;
	mtxRot._23 = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fSinYaw;
	mtxRot._31 = fCosPitch * fSinYaw;
	mtxRot._32 = -fSinPitch;
	mtxRot._33 = fCosPitch * fCosYaw;

	// 回転を反映する
	m_mtxWorld._11 = mtxRot._11;
	m_mtxWorld._12 = mtxRot._12;
	m_mtxWorld._13 = mtxRot._13;
	m_mtxWorld._21 = mtxRot._21;
	m_mtxWorld._22 = mtxRot._22;
	m_mtxWorld._23 = mtxRot._23;
	m_mtxWorld._31 = mtxRot._31;
	m_mtxWorld._32 = mtxRot._32;
	m_mtxWorld._33 = mtxRot._33;

	// オフセット位置を反映
	m_mtxWorld._41 = m_pos.x;
	m_mtxWorld._42 = m_pos.y;
	m_mtxWorld._43 = m_pos.z;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=============================================================================
// ワールドマトリックスの設定処理
//=============================================================================
void CObject3D::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_mtxWorld = mtxWorld;
}

//=============================================================================
// 位置の取得処理
//=============================================================================
D3DXVECTOR3 CObject3D::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// 向きの取得処理
//=============================================================================
D3DXVECTOR3 CObject3D::GetRot(void)
{
	return m_rot;
}

//=============================================================================
// 当たり判定の大きさの取得処理
//=============================================================================
D3DXVECTOR3 CObject3D::GetColRange(void)
{
	return m_colRange;
}

//=============================================================================
// 当たった際戻すかどうかの取得処理
//=============================================================================
bool CObject3D::GetReturnFlag(void)
{
	return m_bReturnFlag;
}

//=============================================================================
// 当たり判定の処理
//=============================================================================
void CObject3D::Hit(CScene *pScene)
{

}

//=============================================================================
// 当たり判定箱の取得処理
//=============================================================================
CBoxCollider * CObject3D::GetBoxCollider(void)
{
	return m_pBoxCollider;
}