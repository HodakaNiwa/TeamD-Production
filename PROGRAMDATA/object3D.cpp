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
CObject3D::CObject3D(int nPriority) : CScene(nPriority,OBJTYPE_3D)
{
	m_pos = INITIALIZE_D3DXVECTOR3;				//位置
	m_rot = INITIALIZE_D3DXVECTOR3;				//向き
	m_pBoxCollider = NULL;
}
//=============================================================================
// オブジェクト3Dのデストラクタ
//=============================================================================
CObject3D::~CObject3D()
{

}
//=============================================================================
// ポリゴンの生成
//=============================================================================
CObject3D *CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CObject3D *pObject3D = NULL;		//プレイヤーのポインタ

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
		m_pBoxCollider = CBoxCollider::Create(pos, 25.0f, 25.0f, 25.0f,true);
	}

	//種類の設置処理
	SetObjType(OBJTYPE_3D);
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
	CDebugProc::Print(1, "位置  : x[%.1f],y[%.1f],z[%.1f]\n", m_pos.x, m_pos.y, m_pos.z);
}

//=============================================================================
// 描画処理
//=============================================================================
void CObject3D::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス
	D3DMATERIAL9 matDef;							//現在のマテリアル保存用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

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