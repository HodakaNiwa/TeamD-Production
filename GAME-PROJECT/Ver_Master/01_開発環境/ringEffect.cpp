//*****************************************************************************
//
//     リングエフェクトの処理[ringEffect.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ringEffect.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//*****************************************************************************
//    CRingDataの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CRingData::CRingData()
{
	m_MaxRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // 向きの最大値
	m_MinRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // 向きの最小値
	m_MaxMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 移動量の最大値
	m_MinMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 移動量の最小値
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 移動量の変化量
	m_InitCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // 生成時の色
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // フレーム毎の色の変化量
	m_nMaxLife = 0;                                  // 寿命の最大値
	m_nMinLife = 0;                                  // 寿命の最小値
	m_fHeight = 0.0f;                                // 高さ
	m_fRadius = 0.0f;                                // 半径
	m_fDiffusion = 0.0f;                             // 幅
	m_fChangeHeight = 0.0f;                          // 高さを毎フレームどれくらい変化させるか
	m_fChangeRadius = 0.0f;                          // 半径をどれくらい毎フレーム変化させるか
	m_fChangeDiffusion = 0.0f;                       // リングの幅をどれくらい毎フレーム変化させるか
	m_nMaxLife = 0;                                  // 寿命の最大値
	m_nMinLife = 0;                                  // 寿命の最小値
	m_nXBlock = 0;                                   // 横の分割数
	m_nYBlock = 0;                                   // 縦の分割数
	m_nRotPattern = 0;                               // 回転の種類[ 0:時計回り 1:反時計回り 2:ランダム ]
	m_RotSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 回転するスピード
	m_bCulling = false;                              // カリングするかどうか[ 0:なし 1:あり ]
	m_bDrawAddtive = false;                          // 加算合成するかしないか[ 0:なし 1:あり ]
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRingData::~CRingData()
{

}

//*****************************************************************************
//    CRingEffectの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CRingEffect::CRingEffect(int nPriority, OBJTYPE objType) : CMeshRing(nPriority, objType)
{
	// 各種値のクリア
	m_nLife = 0;                                      // 寿命
	m_RotSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);         // 回転量
	m_nRotPattern = 0;                                // 回転のパターン
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);           // 移動量
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 移動量を毎フレームどれくらい変化させるか
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 色を毎フレームどれくらい変化させるか
	m_fChangeHeight = 0.0f;                           // 高さを毎フレームどれくらい変化させるか
	m_fChangeRadius = 0.0f;                           // 大きさをどれくらい毎フレーム変化させるか
	m_fChangeDiffusion = 0.0f;                        // リングの幅をどれくらい毎フレーム変化させるか
	m_bCulling = false;                               // カリングするかどうか
	m_bDrawAddtive = false;                           // 加算合成で描画するかしないか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRingEffect::~CRingEffect()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRingEffect *CRingEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
	int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
	float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive, int nPriority)
{
	CRingEffect *pRingEffect = NULL;       // リングエフェクトの型のポインタ
	if (pRingEffect == NULL)
	{// メモリが空になっている
		pRingEffect = new CRingEffect(nPriority);
		if (pRingEffect != NULL)
		{// インスタンスを生成できた
			if (FAILED(pRingEffect->Init(pos, rot, col, fHeight, fRadius, fDiffusion, nXBlock, nYBlock, nLife, RotSpeed, nRotPattern, Move, ChangeMove, ChangeCol, fChangeHeight, fChangeRadius, fChangeDiffusion, bCulling, bDrawAddtive)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pRingEffect;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRingEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
	int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
	float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive)
{
	// 各種値の設定
	SetPos(pos);                           // リングエフェクトの座標
	SetRot(rot);                           // リングエフェクトの向き
	SetCol(col);                           // リングエフェクトの色
	SetHeight(fHeight);                    // リングエフェクトの高さ
	SetRadius(fRadius);                    // リングエフェクトの半径
	SetDiffusion(fDiffusion);              // リングエフェクトの高さ
	SetXBlock(nXBlock);                    // 横の分割数
	SetYBlock(nYBlock);                    // 縦の分割数
	SetLife(nLife);                        // 寿命
	SetRotSpeed(RotSpeed);                 // 回転量
	SetRotPattern(nRotPattern);            // 回転の種類
	SetMove(Move);                         // 移動量
	SetChangeMove(ChangeMove);             // 移動量を毎フレームどれくらい変化させるか
	SetChangeCol(ChangeCol);               // 色を毎フレームどれくらい変化させるか
	SetChangeHeight(fChangeHeight);        // 高さを毎フレームどれくらい変化させるか
	SetChangeRadius(fChangeRadius);        // 半径を毎フレームどれくらい変化させるか
	SetChangeDiffusion(fChangeDiffusion);  // 幅を毎フレームどれくらい変化させるか
	SetCulling(bCulling);                  // カリングするかしないか
	SetDrawAddtive(bDrawAddtive);          // 加算合成で描画するかしないか

	// 共通の初期化処理
	if (FAILED(CMeshRing::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRingEffect::Uninit(void)
{
	// 共通の終了処理
	CMeshRing::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRingEffect::Update(void)
{
	// 寿命を減らす
	m_nLife--;

	if (m_nLife <= 0)
	{// 寿命がなくなった
		Uninit();
	}
	else
	{// 寿命がまだある
		// 各種値の取得
		D3DXVECTOR3 pos = GetPos();        // 座標
		D3DXVECTOR3 rot = GetRot();        // 向き
		D3DXCOLOR col = GetCol();          // 色
		float fHeight = GetHeight();       // 高さ
		float fRadius = GetRadius();       // 半径
		float fDiffusion = GetDiffusion(); // 幅

		// 移動量を加算する
		pos += m_Move;

		// 移動量を変化させる
		m_Move += m_ChangeMove;

		// 色を変化させる
		col += m_ChangeCol;

		// 高さを変化させる
		fHeight += m_fChangeHeight;

		// 半径を変化させる
		fRadius += m_fChangeRadius;

		// 幅を変化させる
		fDiffusion += m_fChangeDiffusion;

		// 向きを変化させる
		if (m_nRotPattern == 0)
		{// 0のパターン(時計回り)なら
			rot -= m_RotSpeed;
		}
		else if (m_nRotPattern == 1)
		{// 1のパターン(反時計回り)なら
			rot += m_RotSpeed;
		}
		else if (m_nRotPattern == 2)
		{// 2のパターン(ランダム)なら
			int nRandom = rand() % 2;
			if (nRandom == 0)
			{
				rot += m_RotSpeed;
			}
			else
			{
				rot -= m_RotSpeed;
			}
		}

		// 向きの補正
		// X軸
		if (rot.x > D3DX_PI)
		{
			rot.x -= D3DX_PI * 2.0f;
		}
		if (rot.x < -D3DX_PI)
		{
			rot.x += D3DX_PI * 2.0f;
		}
		// Y軸
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		// Z軸
		if (rot.z > D3DX_PI)
		{
			rot.z -= D3DX_PI * 2.0f;
		}
		if (rot.z < -D3DX_PI)
		{
			rot.z += D3DX_PI * 2.0f;
		}


		// 各種値の設定
		SetPos(pos);              // 座標
		SetRot(rot);              // 向き
		SetCol(col);              // 色
		SetHeight(fHeight);       // 高さ
		SetRadius(fRadius);       // 半径
		SetDiffusion(fDiffusion); // 幅

		// 頂点情報を書き換える
		ChangeVertex();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CRingEffect::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Culling;   // カリングの設定

			if (m_bCulling == false)
			{// カリングしない設定ならば
				pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			if (m_bDrawAddtive == true)
			{// 加算合成で描画するかならば
			    // αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); // 深度バッファの書き込み設定を無効に
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);// すべて描画する

			// 共通の描画処理
			CMeshRing::Draw();

			if (m_bCulling == false)
			{// カリングしない設定ならば
				pDevice->SetRenderState(D3DRS_CULLMODE, Culling);
			}
			if (m_bDrawAddtive == true)
			{// 加算合成で描画するかならば
			    // αブレンディングを元に戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}

			// Zテストを有効にする(デフォルトの値に)
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      // 深度バッファへの書き込みを有効に
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL); // 以下のものを描画する
		}
	}
}

//=============================================================================
//    頂点情報書き換えの処理
//=============================================================================
void CRingEffect::ChangeVertex(void)
{
	// 頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	if (pVtxBuff != NULL)
	{// 頂点バッファが生成されている
		// 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fAngle = 0;                    // 角度
		float fRadius = 0;                   // 半径
		D3DXVECTOR3 pos;                     // 座標
		D3DXCOLOR col = GetCol();            // 色
		int nXBlock = GetXBlock();           // 横の分割数
		int nYBlock = GetYBlock();           // 横の分割数
		float fHeight = GetHeight();         // 高さ
		float fDiffusion = GetDiffusion();   // 拡散値
		float Utex;                          // テクスチャU座標
		float Vtex;                          // テクスチャV座標

		fAngle = 0.0f;                          // 角度は0に
		fRadius = GetRadius();                  // 半径を設定
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 座標を設定
		Utex = 0.0f;                            // テクスチャU座標を設定
		Vtex = 0.0f;                            // テクスチャV座標を設定

		for (int nCntV = 0; nCntV < nYBlock + 1; nCntV++)
		{// 垂直方向の分割数だけ繰り返し
			for (int nCntH = 0; nCntH < nXBlock + 1; nCntH++)
			{// 水平方向の分割数だけ繰り返し
				pos.x = sinf(fAngle) * fRadius;
				pos.z = cosf(fAngle) * fRadius;

				// 頂点座標
				pVtx[nCntH].pos = pos;

				// 頂点カラー
				pVtx[nCntH].col = col;

				// テクスチャ座標
				pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

				fAngle -= D3DX_PI / (nXBlock / 2.0f);  // 角度を進める
				Utex += 1.0f / (nXBlock / 2.0f);       // テクスチャU座標を進める

				if (fAngle <= -D3DX_PI)
				{// 角度が円周率を超えた
					fAngle += D3DX_PI * 2.0f;
				}
			}
			col.a -= 0.2f;             // 透明度を下げる
			pos.y += fHeight;          // 座標を上にあげる
			fRadius += fDiffusion;     // 半径を広げる
			fAngle = 0.0f;             // 角度は0に戻す
			Utex = 0.0f;               // テクスチャU座標を戻す
			Vtex += 1.0f / nYBlock;    // テクスチャV座標を進める
			pVtx += nXBlock + 1;       // 頂点バッファの番号を横の分割数分進める
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    寿命設定処理
//=============================================================================
void CRingEffect::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    回転量設定処理
//=============================================================================
void CRingEffect::SetRotSpeed(const D3DXVECTOR3 RotSpeed)
{
	m_RotSpeed = RotSpeed;
}

//=============================================================================
//    回転の種類設定処理
//=============================================================================
void CRingEffect::SetRotPattern(const int nRotPattern)
{
	m_nRotPattern = nRotPattern;
}

//=============================================================================
//    移動量設定処理
//=============================================================================
void CRingEffect::SetMove(const D3DXVECTOR3 Move)
{
	m_Move = Move;
}

//=============================================================================
//    移動量を毎フレームどれくらい変化させるか設定処理
//=============================================================================
void CRingEffect::SetChangeMove(const D3DXVECTOR3 ChangeMove)
{
	m_ChangeMove = ChangeMove;
}

//=============================================================================
//    色を毎フレームどれくらい変化させるか設定処理
//=============================================================================
void CRingEffect::SetChangeCol(const D3DXCOLOR ChangeCol)
{
	m_ChangeCol = ChangeCol;
}

//=============================================================================
//    高さを毎フレームどれくらい変化させるか設定処理
//=============================================================================
void CRingEffect::SetChangeHeight(const float fChangeHeight)
{
	m_fChangeHeight = fChangeHeight;
}

//=============================================================================
//    大きさを毎フレームどれくらい変化させるか設定処理
//=============================================================================
void CRingEffect::SetChangeRadius(const float fChangeRadius)
{
	m_fChangeRadius = fChangeRadius;
}

//=============================================================================
//    幅を毎フレームどれくらい変化させるか設定処理
//=============================================================================
void CRingEffect::SetChangeDiffusion(const float fChangeDiffusion)
{
	m_fChangeDiffusion = fChangeDiffusion;
}

//=============================================================================
//    カリングするかどうか設定処理
//=============================================================================
void CRingEffect::SetCulling(const bool bCulling)
{
	m_bCulling = bCulling;
}

//=============================================================================
//    加算合成で描画するかしないか設定処理
//=============================================================================
void CRingEffect::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    寿命取得処理
//=============================================================================
int CRingEffect::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    回転量取得処理
//=============================================================================
D3DXVECTOR3 CRingEffect::GetRotSpeed(void)
{
	return m_RotSpeed;
}

//=============================================================================
//    回転の種類取得処理
//=============================================================================
int CRingEffect::GetRotPattern(void)
{
	return m_nRotPattern;
}

//=============================================================================
//    移動量取得処理
//=============================================================================
D3DXVECTOR3 CRingEffect::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    移動量を毎フレームどれくらい変化させるか取得処理
//=============================================================================
D3DXVECTOR3 CRingEffect::GetChangeMove(void)
{
	return m_ChangeMove;
}

//=============================================================================
//    色を毎フレームどれくらい変化させるか取得処理
//=============================================================================
D3DXCOLOR CRingEffect::GetChangeCol(void)
{
	return m_ChangeCol;
}

//=============================================================================
//    高さを毎フレームどれくらい変化させるか取得処理
//=============================================================================
float CRingEffect::GetChangeHeight(void)
{
	return m_fChangeHeight;
}

//=============================================================================
//    大きさを毎フレームどれくらい変化させるか取得処理
//=============================================================================
float CRingEffect::GetChangeRadius(void)
{
	return m_fChangeRadius;
}

//=============================================================================
//    幅を毎フレームどれくらい変化させるか取得処理
//=============================================================================
float CRingEffect::GetChangeDiffusion(void)
{
	return m_fChangeDiffusion;
}

//=============================================================================
//    カリングするかどうか取得処理
//=============================================================================
bool CRingEffect::GetCulling(void)
{
	return m_bCulling;
}

//=============================================================================
//    加算合成で描画するかしないか取得処理
//=============================================================================
bool CRingEffect::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}