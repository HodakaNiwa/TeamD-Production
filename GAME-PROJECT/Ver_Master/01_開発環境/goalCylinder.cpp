//*****************************************************************************
//
//     ゴール用の円筒処理[goalCylinder.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "goalCylinder.h"
#include "manager.h"
#include "renderer.h"
#include "tutorial.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define GOALCYLINDER_COL_RANGE          (0.2f)  // 当たり判定に使用する長さの倍率
#define GOALCYLINDER_COL_RANGE_TUTORIAL (5.0f)  // ゴールに近いレスポンスを返すのに使用する長さの倍率

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CGoalCylinder::CGoalCylinder(int nPriority, OBJTYPE objType) : CMeshCylinder(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CGoalCylinder::~CGoalCylinder()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGoalCylinder *CGoalCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, float fDecay, float fMag, float fUpValue, int nPriority)
{
	CGoalCylinder *pGoalCylinder = NULL;   // ゴール用円筒クラス型のポインタ
	if (pGoalCylinder == NULL)
	{// メモリが空になっている
		pGoalCylinder = new CGoalCylinder(nPriority);
		if (pGoalCylinder != NULL)
		{// インスタンスを生成できた
			if (FAILED(pGoalCylinder->Init(pos, rot, col, fHeight, fRadius, nXBlock, nYBlock, fDecay, fMag, fUpValue)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pGoalCylinder;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGoalCylinder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, float fDecay, float fMag, float fUpValue)
{
	// 各種値の代入
	SetPos(pos);            // メッシュシリンダーの座標
	SetRot(rot);            // メッシュシリンダーの向き
	SetCol(col);            // メッシュシリンダーの色
	SetHeight(fHeight);     // メッシュシリンダーの高さ
	SetRadius(fRadius);     // メッシュシリンダーの半径
	SetXBlock(nXBlock);     // 横の分割数
	SetYBlock(nYBlock);     // 縦の分割数
	SetDecay(fDecay);       // 減衰値
	SetMag(fMag);           // 倍率
	SetUpValue(fUpValue);   // 倍率に加算する量

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
void CGoalCylinder::Uninit(void)
{
	// 共通の終了処理
	CMeshCylinder::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGoalCylinder::Update(void)
{
	// 色を取得
	D3DXCOLOR col = GetCol();

	// 倍率に加算する
	m_fMag += m_fUpValue;
	if (m_fMag > D3DX_PI)
	{
		m_fMag -= D3DX_PI;
	}

	// 透明度を倍率によって変更する
	col.a = sinf(m_fMag) * 1.0f;

	// 頂点の色を倍率によってずらす
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが取得できた
	    // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 倍率計算用
		float fMag = m_fMag;

		for (int nCntV = 0; nCntV < GetYBlock() + 1; nCntV++)
		{// 垂直方向の分割数だけ繰り返し
			for (int nCntH = 0; nCntH < GetXBlock() + 1; nCntH++)
			{// 水平方向の分割数だけ繰り返し
			    // 頂点カラー
				pVtx[nCntH].col = col;
			}
			pVtx += GetXBlock() + 1;   // 頂点バッファの番号を横の分割数分進める
			if (nCntV % 2 == 0)
			{
				// 透明度の変更
				fMag += m_fDecay;
				if (fMag > D3DX_PI)
				{
					fMag -= D3DX_PI;
				}
				col.a = sinf(fMag);
			}
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CGoalCylinder::Draw(void)
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
void CGoalCylinder::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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

	// 透明度を倍率によって変更する
	col.a = sinf(m_fMag) * 1.0f;

	// 倍率計算用
	float fMag = m_fMag;

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

		// 透明度の変更
		if (nCntV % 2 == 0)
		{
			fMag += m_fDecay;
			if (fMag > D3DX_PI)
			{
				fMag -= D3DX_PI;
			}
			col.a = sinf(fMag);
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CGoalCylinder::Collision(D3DXVECTOR3 *pPos)
{
	// 距離を求める
	D3DXVECTOR3 pos = GetPos();
	float fLength = sqrtf((pPos->x - pos.x) * (pPos->x - pos.x) + (pPos->y - pos.y) * (pPos->y - pos.y) + (pPos->z - pos.z) * (pPos->z - pos.z));

	if (fLength <= GetRadius() * GOALCYLINDER_COL_RANGE)
	{// 距離が判定する長さより短い
		return true;
	}

	if (fLength <= GetRadius() * GOALCYLINDER_COL_RANGE_TUTORIAL && CManager::GetMode() == CManager::MODE_TUTORIAL)
	{// チュートリアル画面でレスポンスを返す距離にプレイヤーがいる
		CTutorial *pTutorial = CManager::GetTutorial();
		if (pTutorial != NULL)
		{
			pTutorial->CreateNearGoalInfo();
		}
	}

	return false;
}

//=============================================================================
//    減衰値を設定する処理
//=============================================================================
void CGoalCylinder::SetDecay(const float fDecay)
{
	m_fDecay = fDecay;
}

//=============================================================================
//    倍率を設定する処理
//=============================================================================
void CGoalCylinder::SetMag(const float fMag)
{
	m_fMag = fMag;
}

//=============================================================================
//    倍率を加算する量を設定処理
//=============================================================================
void CGoalCylinder::SetUpValue(const float fUpValue)
{
	m_fUpValue = fUpValue;
}

//=============================================================================
//    減衰値を取得する処理
//=============================================================================
float CGoalCylinder::GetDecay(void)
{
	return m_fDecay;
}

//=============================================================================
//    倍率を取得する処理
//=============================================================================
float CGoalCylinder::GetMag(void)
{
	return m_fMag;
}

//=============================================================================
//    倍率を加算する量を取得する処理
//=============================================================================
float CGoalCylinder::GetUpValue(void)
{
	return m_fUpValue;
}