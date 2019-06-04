//=============================================================================
//
// ビルボードの処理 [billboard.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "billboard.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BILLBOARD (128)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		m_pTexture = NULL;	// テクスチャへのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CBillboard::CBillboard(int nPriority, OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pos = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	m_fWidth = 10.0f;
	m_fHeight = 10.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBillboard::~CBillboard()
{

}

//=============================================================================
// ビルボードの生成
//=============================================================================
CBillboard *CBillboard::Create(void)
{
	CBillboard *pBillboard = NULL;

	if (pBillboard == NULL)
	{
		pBillboard = new CBillboard;
		if (pBillboard != NULL)
		{
			pBillboard->Init();
		}
	}
	return pBillboard;
}
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBillboard::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\grass.png", &m_pTexture);	//テクスチャ読み込み

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth,m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth,m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth,-m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth,-m_fHeight, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBillboard::Uninit(void)
{
	//データの開放
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CBillboard::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

}

//=============================================================================
// 描画処理
//=============================================================================
void CBillboard::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans, mtxView;							//計算用マトリックス

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);			//
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//基準値
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW,
		&mtxView);

	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	//// 回転を反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y,
	//	m_rot.x,
	//	m_rot.z);

	//D3DXMatrixMultiply(&m_mtxWorld,
	//	&m_mtxWorld,
	//	&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// オブジェクトの取得処理
//=============================================================================
void CBillboard::BindTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_pTexture = texture;
}

//=============================================================================
// オブジェクトの位置の設置処理
//=============================================================================
void CBillboard::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// オブジェクトの位置の取得処理
//=============================================================================
D3DXVECTOR3 CBillboard::GetPos(void)
{
	return m_pos;
}

//=============================================================================
// オブジェクトの幅の設置処理
//=============================================================================
void CBillboard::SetWidth(float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
// オブジェクトの高さの設置処理
//=============================================================================
void CBillboard::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
// オブジェクトの向きの取得処理
//=============================================================================
void CBillboard::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// オブジェクトの向きの設置処理
//=============================================================================
D3DXVECTOR3 CBillboard::GetRot(void)
{
	return m_rot;
}