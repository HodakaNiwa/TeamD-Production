//*****************************************************************************
//
//     アイテム用の円筒処理[itemCylinder.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "itemCylinder.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define ITEMCYLINDER_ALPHA_DECAY (-0.012f)
#define ITEMCYLINDER_ALPHA_DECAY_CUT (1.15f)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CItemCylinder::CItemCylinder(int nPriority, OBJTYPE objType) : CMeshCylinder(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CItemCylinder::~CItemCylinder()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CItemCylinder *CItemCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	CItemCylinder *pItemCylinder = NULL;   // アイテム用円筒クラス型のポインタ
	if (pItemCylinder == NULL)
	{// メモリが空になっている
		pItemCylinder = new CItemCylinder(nPriority);
		if (pItemCylinder != NULL)
		{// インスタンスを生成できた
			if (FAILED(pItemCylinder->Init(pos, rot, col, fHeight, fRadius, nXBlock, nYBlock)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pItemCylinder;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CItemCylinder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock)
{
	// 各種値の代入
	SetPos(pos);            // メッシュシリンダーの座標
	SetRot(rot);            // メッシュシリンダーの向き
	SetCol(col);            // メッシュシリンダーの色
	SetHeight(fHeight);     // メッシュシリンダーの高さ
	SetRadius(fRadius);     // メッシュシリンダーの半径
	SetXBlock(nXBlock);     // 横の分割数
	SetYBlock(nYBlock);     // 縦の分割数
	m_State = STATE_NONE;   // 状態を初期化

	// 共通の初期化処理
	if (FAILED(CMeshCylinder::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CItemCylinder::Uninit(void)
{
	// 共通の終了処理
	CMeshCylinder::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CItemCylinder::Update(void)
{
	switch (m_State)
	{// 状態によって処理わけ
	case STATE_NONE:
		Normal();
		break;
	case STATE_DEATH:
		Death();
		break;
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CItemCylinder::Draw(void)
{
	// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Lighting;
			DWORD Culling;

			// カリングをしない設定に
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// ライティングの設定
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// 共通の描画処理
			CMeshCylinder::Draw();

			// カリングの設定を元に戻す
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

			// ライティングを戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    頂点バッファ生成処理
//=============================================================================
void CItemCylinder::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * GetNumVertex(),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;   // 角度
	float fRadius = 0;  // 半径
	D3DXVECTOR3 pos;    // 座標
	D3DXCOLOR col;      // 色
	float Utex;         // テクスチャU座標
	float Vtex;         // テクスチャV座標

	fAngle = 0.0f;                          // 角度は0に
	fRadius = GetRadius();                  // 半径を設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 座標を設定
	col = GetCol();                         // 色の設定
	Utex = 0.0f;                            // テクスチャU座標を設定
	Vtex = 0.0f;                            // テクスチャV座標を設定

	for (int nCntV = 0; nCntV < GetYBlock() + 1; nCntV++)
	{// 垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
		{// 水平方向の分割数だけ繰り返し
			pos.x = sinf(fAngle) * fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// 頂点座標
			pVtx[nCntH].pos = pos;

			// 頂点カラー
			pVtx[nCntH].col = col;

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

			fAngle -= D3DX_PI / (GetXBlock() / 2.0f);  // 角度を進める
			Utex += 1.0f / (GetXBlock() / 2.0f);       // テクスチャU座標を進める

			if (fAngle <= -D3DX_PI)
			{// 角度が円周率を超えた
				fAngle += D3DX_PI * 2.0f;
			}
		}
		pos.y += GetHeight() / (GetYBlock() + 1);  // 座標を上げる
		fAngle = 0.0f;                             // 角度は0に戻す
		Utex = 0.0f;                               // テクスチャU座標を戻す
		Vtex -= 1.0f / GetYBlock();                // テクスチャV座標を進める
		pVtx += GetXBlock() + 1;                   // 頂点バッファの番号を横の分割数分進める
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    状態の設定
//=============================================================================
void CItemCylinder::SetState(const STATE state)
{
	m_State = state;
}

//=============================================================================
//    状態の取得
//=============================================================================
CItemCylinder::STATE CItemCylinder::GetState(void)
{
	return m_State;
}

//=============================================================================
//    通常状態の処理
//=============================================================================
void CItemCylinder::Normal(void)
{

}

//=============================================================================
//    死亡状態の処理
//=============================================================================
void CItemCylinder::Death(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff == NULL) { return; }
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の透明度を上げる
	D3DXCOLOR col;
	float fAlphaDecay = ITEMCYLINDER_ALPHA_DECAY;
	bool bDeath = true;
	for (int nCntV = 0; nCntV < GetYBlock() + 1; nCntV++)
	{// 垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
		{// 水平方向の分割数だけ繰り返し
			// 頂点カラーを取得
			col = pVtx[nCntH].col;

			// 透明度を上げる
			col.a += fAlphaDecay;
			if (col.a > 0.0f)
			{
				bDeath = false;
			}
			else
			{
				col.a = 0.0f;
			}

			// 頂点カラーの設定
			pVtx[nCntH].col = col;
		}
		// 透明度を下げる量を減らす
		fAlphaDecay *= ITEMCYLINDER_ALPHA_DECAY_CUT;
		pVtx += GetXBlock() + 1;                   // 頂点バッファの番号を横の分割数分進める
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 全ての頂点が透明になった
	if (bDeath == true)
	{
		Uninit();
	}
}