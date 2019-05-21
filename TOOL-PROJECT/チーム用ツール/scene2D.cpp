//=============================================================================
//
// オブジェクト2D処理 [scene2D.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

#define TEXTURE_NAME "data\\TEXTURE\\enemy010.png"			//テクスチャ名

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CScene2D::CScene2D(int nPriority,OBJTYPE objtype) : CScene(nPriority, objtype)
{
	m_pTexture = NULL;					// テクスチャへのポインタ
	m_pVtxBuff = NULL;					// 頂点バッファへのポインタ
	m_pos = D3DXVECTOR3(0, 0, 0);		// 位置
	m_rot = 0;							//向き
	m_fAddLength = 1;					//加算数
	m_fLength = 0;						//拡大
	m_fWidth = 50;						//幅
	m_fHeight = 50;						//高さ
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色

}
//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
//オブジェクトの生成
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, float fWight, float fHeight)
{
	CScene2D *pScene2D = NULL;	//CScene2Dのポインタ

	if (pScene2D == NULL)
	{//NULLの場合
		pScene2D = new CScene2D;	//シーンの動的確保
		if (pScene2D != NULL)
		{
			pScene2D->m_pos = pos;			//位置の代入
			pScene2D->m_fWidth = fWight;	//幅の代入
			pScene2D->m_fHeight = fHeight;	//高さの代入
			pScene2D->Init();				//初期化処理
		}
	}
	return pScene2D;			//値を返す
}

//=============================================================================
// オブジェクトの初期化処理
//=============================================================================
HRESULT CScene2D::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//ポインタの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME,
		&m_pTexture);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)& pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//種類の設定
	SetObjType(OBJTYPE_SCENE2D);

	return S_OK;
}

//=============================================================================
// オブジェクトの終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// テクスチャの破棄
	/*if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}*/

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// オブジェクトの更新処理
//=============================================================================
void CScene2D::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();
	/*m_rot.x += 0.01f;*/

	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		//Uninit();
	}
}

//=============================================================================
// オブジェクトの描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// オブジェクトの取得処理
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;	//値を返す
}

//=============================================================================
// オブジェクトの設置処理
//=============================================================================
void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;	//位置の代入
}

//=============================================================================
// 頂点情報の設置処理
//=============================================================================
void CScene2D::SetVtxPos(D3DXVECTOR3 pos)
{
	m_pos = pos;	//位置の代入

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点情報の設置処理
//=============================================================================
void CScene2D::SetVtxLenghtPos(D3DXVECTOR3 pos)
{
	m_pos = pos;	//位置の代入

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y - m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fLength, m_pos.y + m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fLength, m_pos.y + m_fLength, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// オブジェクトの設置処理
//=============================================================================
void CScene2D::SetVtxRotPos(D3DXVECTOR3 pos)
{
	m_pos = pos;	//位置の代入

	VERTEX_2D*pVtx;	//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(-D3DX_PI * 0.75f - m_rot) * m_fLength, m_pos.y + cosf(-D3DX_PI * 0.75f - m_rot) * m_fLength, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * 0.75f - m_rot) * m_fLength, m_pos.y + cosf(D3DX_PI * 0.75f - m_rot) * m_fLength, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(-D3DX_PI * 0.25f - m_rot) * m_fLength, m_pos.y + cosf(-D3DX_PI * 0.25f - m_rot) * m_fLength, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(D3DX_PI * 0.25f - m_rot) * m_fLength, m_pos.y + cosf(D3DX_PI * 0.25f - m_rot) * m_fLength, 0.0f);


	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
//=============================================================================
// 幅の設置処理
//=============================================================================
void CScene2D::SetWidth(float width)
{
	m_fWidth = width;
}
//=============================================================================
// 高さの設置処理
//=============================================================================
void CScene2D::SetHeight(float height)
{
	m_fHeight = height;
}
//=============================================================================
// 拡大の設置処理
//=============================================================================
void CScene2D::SetLength(float fLength)
{
	m_fLength = fLength;
}
//=============================================================================
// 幅の取得処理
//=============================================================================
float CScene2D::GetWidth(void)
{
	return m_fWidth;
}
//=============================================================================
// 高さの設置処理
//=============================================================================
float CScene2D::GetHeight(void)
{
	return m_fHeight;
}
//=============================================================================
// 拡大の設置処理
//=============================================================================
float CScene2D::GetfLength(void)
{
	return m_fLength;
}

//=============================================================================
// オブジェクトテクスチャの設置処理
//=============================================================================
void CScene2D::SetCol(D3DXCOLOR col)
{
	m_col = col;
	VERTEX_2D*pVtx;	//頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// オブジェクトの取得処理
//=============================================================================
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 texture)
{
	m_pTexture = texture;
}

//=============================================================================
// 頂点バッファの取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetVtx(void)
{
	return m_pVtxBuff;
}

//=============================================================================
// 頂点バッファの取得処理
//=============================================================================
D3DXCOLOR CScene2D::GetCol(void)
{
	return m_col;
}