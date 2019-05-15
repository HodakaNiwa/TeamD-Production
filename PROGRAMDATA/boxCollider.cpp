//*****************************************************************************
//
//     当たり判定用箱モデルの処理[boxCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "boxCollider.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define BOX_COLLIDER_LINEWIDTH (3.0f)    // 線の幅

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CBoxCollider::CBoxCollider()
{
	// 各種値のクリア
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 向き
	D3DXMatrixIdentity(&m_MtxWorld);          // ワールドマトリックス
	m_fWidth = 0.0f;                          // 箱の横幅
	m_fHeight = 0.0f;                         // 箱の高さ
	m_fDepth = 0.0f;                          // 箱の奥行
	m_bReturnFlag = false;                    // 当たった際に戻すかどうか
	m_pLine = NULL;                           // 線描画用インスタンスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBoxCollider::~CBoxCollider()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBoxCollider *CBoxCollider::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag)
{
	CBoxCollider *pBoxCollider = NULL;      // 当たり判定用箱モデルクラス型のポインタ
	if (pBoxCollider == NULL)
	{// メモリが空になっている
		pBoxCollider = new CBoxCollider;
		if (pBoxCollider != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pBoxCollider->SetPos(pos);                 // 座標
			pBoxCollider->SetWidth(fWidth);            // 箱の横幅
			pBoxCollider->SetHeight(fHeight);          // 箱の高さ
			pBoxCollider->SetDepth(fDepth);            // 箱の奥行
			pBoxCollider->SetReturnFlag(bReturnFlag);  // 当たった際に戻すかどうか

			if (FAILED(pBoxCollider->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// インスタンスを生成できなかった
		return NULL;
	}

	return pBoxCollider;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBoxCollider::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// 線描画用インスタンスを生成
			D3DXCreateLine(pDevice, &m_pLine);
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBoxCollider::Uninit(void)
{

}

//=============================================================================
//    更新処理
//=============================================================================
void CBoxCollider::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBoxCollider::Draw(void)
{
#ifdef _DEBUG
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 透視投影変換行列をカメラから作成する
			D3DXVECTOR3 VecScreenPos;
			D3DXMATRIX mtxView;
			D3DXMATRIX mtxProj;
			D3DXMATRIX mtxViewPort;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
			D3DXMatrixIdentity(&mtxViewPort);
			D3DXMatrixIdentity(&mtxViewPort);
			mtxViewPort._11 = SCREEN_WIDTH / 2;
			mtxViewPort._22 = -SCREEN_HEIGHT / 2;
			mtxViewPort._41 = SCREEN_WIDTH / 2;
			mtxViewPort._42 = SCREEN_HEIGHT / 2;
			mtxViewPort._33 = 1.0f;
			mtxViewPort._44 = 1.0f;

			// 各ラインの頂点座標を設定
			D3DXVECTOR3 VecPos[24] =
			{
				// １本目
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				// ２本目
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				// ３本目
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				// ４本目
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				// ５本目
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				// ６本目
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
				// ７本目
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				// ８本目
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				// ９本目
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				// １０本目
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  -(m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  -(m_fDepth / 2)),
				// １１本目
				D3DXVECTOR3(-(m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				D3DXVECTOR3(-(m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
				// １２本目
				D3DXVECTOR3((m_fWidth / 2), m_fHeight,  (m_fDepth / 2)),
				D3DXVECTOR3((m_fWidth / 2), 0.0f,  (m_fDepth / 2)),
			};

			// 各頂点をワールド座標からスクリーン座標へ変換する
			for (int nCnt = 0; nCnt < 24; nCnt++)
			{
				D3DXVec3TransformCoord(&VecScreenPos, &VecPos[nCnt], &m_MtxWorld);
				D3DXVec3TransformCoord(&VecScreenPos, &VecScreenPos, &mtxView);
				D3DXVec3TransformCoord(&VecScreenPos, &VecScreenPos, &mtxProj);
				D3DXVec3TransformCoord(&VecScreenPos, &VecScreenPos, &mtxViewPort);
				m_VecPos[nCnt] = D3DXVECTOR2(VecScreenPos.x, VecScreenPos.y);
			}

			// 線を描画
			D3DXVECTOR2 Vec[2];
			m_pLine->SetAntialias(TRUE);
			m_pLine->SetWidth(BOX_COLLIDER_LINEWIDTH);
			m_pLine->Begin();
			for (int nCnt = 0; nCnt < 12; nCnt++)
			{
				Vec[0] = m_VecPos[(nCnt * 2)];
				Vec[1] = m_VecPos[(nCnt * 2) + 1];
				m_pLine->Draw(Vec, 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
			m_pLine->End();
		}
	}

#endif
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CBoxCollider::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxScale, mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CBoxCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand)
{
	bool bHit = false;  // 当たったかどうか

	// Y軸上の判定
	if (pPos->y < m_Pos.y + m_fHeight && pPos->y + ColRange.y > m_Pos.y)
	{// 現在の高さが箱の中にいる
	    // X軸上の判定
		if (pPos->z + ColRange.z > m_Pos.z - (m_fDepth / 2) && pPos->z - ColRange.z < m_Pos.z + (m_fDepth / 2))
		{// Z軸上にいるかどうか判定
			if (pPosOld->x + ColRange.x <= m_Pos.x - (m_fWidth / 2) && pPos->x + ColRange.x > m_Pos.x - (m_fWidth / 2))
			{// 箱の左側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->x = m_Pos.x - ColRange.x - (m_fWidth / 2);
				}
				bHit = true;
			}
			else if (pPosOld->x - ColRange.x >= m_Pos.x + (m_fWidth / 2) && pPos->x - ColRange.x < m_Pos.x + (m_fWidth / 2))
			{// 箱の右側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->x = m_Pos.x + ColRange.x + (m_fWidth / 2);
				}
				bHit = true;
			}
		}
		// Z軸上の判定
		if (pPos->x + ColRange.x > m_Pos.x - (m_fWidth / 2) && pPos->x - ColRange.x < m_Pos.x + (m_fWidth / 2))
		{// X軸上にいるかどうか判定
			if (pPosOld->z - ColRange.z >= m_Pos.z + (m_fDepth / 2) && pPos->z - ColRange.z < m_Pos.z + (m_fDepth / 2))
			{// 箱の奥側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->z = m_Pos.z + ColRange.z + (m_fDepth / 2);
				}
				bHit = true;
			}
			else if (pPosOld->z + ColRange.z <= m_Pos.z - (m_fDepth / 2) && pPos->z + ColRange.z > m_Pos.z - (m_fDepth / 2))
			{// 箱の手前側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->z = m_Pos.z - ColRange.z - (m_fDepth / 2);
				}
				bHit = true;
			}
		}
	}

	if (pPosOld->y >= m_Pos.y + m_fHeight && pPos->y < m_Pos.y + m_fHeight)
	{// 高さが上から箱の内側に入ってしまった
		if (pPos->x + ColRange.x > m_Pos.x - (m_fWidth / 2) && pPos->x - ColRange.x < m_Pos.x + (m_fWidth / 2)
			&& pPos->z + ColRange.z > m_Pos.z - (m_fDepth / 2) && pPos->z - ColRange.z < m_Pos.z + (m_fDepth / 2))
		{// 箱の内側にいる判定だった
			if (m_bReturnFlag == true)
			{// 位置を戻すならば
				pPos->y = m_Pos.y + m_fHeight;
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + ColRange.y <= m_Pos.y && pPos->y + ColRange.y > m_Pos.y)
	{// 高さが下から箱の内側に入ってしまった
		if (pPos->x + ColRange.x > m_Pos.x - (m_fWidth / 2) && pPos->x - ColRange.x < m_Pos.x + (m_fWidth / 2)
			&& pPos->z + ColRange.z > m_Pos.z - (m_fDepth / 2) && pPos->z - ColRange.z < m_Pos.z + (m_fDepth / 2))
		{// 箱の内側にいる判定だった
			if (m_bReturnFlag == true)
			{// 位置を戻すならば
				pPos->y = m_Pos.y - ColRange.y;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    座標設定処理
//=============================================================================
void CBoxCollider::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    向き設定処理
//=============================================================================
void CBoxCollider::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CBoxCollider::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    箱の横幅設定処理
//=============================================================================
void CBoxCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    箱の高さ設定処理
//=============================================================================
void CBoxCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    箱の奥行設定処理
//=============================================================================
void CBoxCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    当たった際に戻すかどうか設定処理
//=============================================================================
void CBoxCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    座標取得処理
//=============================================================================
D3DXVECTOR3 CBoxCollider::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    向き取得処理
//=============================================================================
D3DXVECTOR3 CBoxCollider::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CBoxCollider::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    箱の横幅取得処理
//=============================================================================
float CBoxCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    箱の高さ取得処理
//=============================================================================
float CBoxCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    箱の奥行取得処理
//=============================================================================
float CBoxCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    当たった際に戻すかどうか取得処理
//=============================================================================
bool CBoxCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}