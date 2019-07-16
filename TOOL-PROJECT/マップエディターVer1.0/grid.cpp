//*****************************************************************************
//
//     グリッド線の処理[grid.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "grid.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define GRID_LINECOLOR (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)) // 線の色
#define GRID_LINEWIDTH (2.0f)                           // 線の幅

//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CGrid::CGrid()
{
	// 各種値のクリア
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 座標
	m_fWidth = 0.0f;                          // 横幅
	m_fDepth = 0.0f;                          // 奥行
	m_nXBlock = 0;                            // 横の分割数
	m_nZBlock = 0;                            // 奥行の分割数
	m_pVecPos = NULL;                         // 頂点座標
	m_pIndex = NULL;                          // 頂点番号
	m_pLine = NULL;                           // 線描画用インスタンスへのポインタ
	m_nNumVertex = 0;                         // 頂点数
	m_nNumIdx = 0;                            // インデックス番号数
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGrid::~CGrid()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGrid *CGrid::Create(D3DXVECTOR3 pos, float fWidth, float fDepth, int nXBlock, int nZBlock)
{
	CGrid *pGrid = NULL;      // 当たり判定用箱モデルクラス型のポインタ
	if (pGrid == NULL)
	{// メモリが空になっている
		pGrid = new CGrid;
		if (pGrid != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pGrid->SetPos(pos);                 // 座標
			pGrid->SetWidth(fWidth);            // 横幅
			pGrid->SetDepth(fDepth);            // 奥行
			pGrid->SetXBlock(nXBlock);          // 横の分割数
			pGrid->SetZBlock(nZBlock);          // 奥行の分割数

			if (FAILED(pGrid->Init()))
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

	return pGrid;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGrid::Init(void)
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

			// 頂点情報生成
			MakeVertex();
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGrid::Uninit(void)
{
	if (m_pVecPos != NULL)
	{
		delete[] m_pVecPos;
		m_pVecPos = NULL;
	}

	if (m_pIndex != NULL)
	{
		delete[] m_pIndex;
		m_pIndex = NULL;
	}

	DIRECT_RELEASE(m_pLine);
}

//=============================================================================
//    更新処理
//=============================================================================
void CGrid::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CGrid::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 透視投影変換行列をカメラから作成する
			D3DXVECTOR3 VecScreenPos;
			D3DXMATRIX mtxView;
			D3DXMATRIX mtxProj;
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);
			pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
			// 線を描画
			if (m_pVecPos != NULL && m_pIndex != NULL && m_pLine != NULL)
			{
				int nIdx = 0;
				D3DXMATRIX mtxTrans, mtxWorld;
				D3DXMatrixIdentity(&mtxWorld);
				D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxView);
				D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxProj);
				D3DXVECTOR3 Vec[2];
				m_pLine->SetAntialias(TRUE);
				m_pLine->SetWidth(GRID_LINEWIDTH);
				m_pLine->Begin();
				for (int nCnt = 0; nCnt < (m_nXBlock * m_nZBlock); nCnt++)
				{
					Vec[0] = m_pVecPos[m_pIndex[nIdx]];
					Vec[1] = m_pVecPos[m_pIndex[nIdx + 1]];
					m_pLine->DrawTransform(Vec, 2, &mtxWorld, GRID_LINECOLOR);

					Vec[0] = m_pVecPos[m_pIndex[nIdx + 2]];
					Vec[1] = m_pVecPos[m_pIndex[nIdx + 3]];
					m_pLine->DrawTransform(Vec, 2, &mtxWorld, GRID_LINECOLOR);

					Vec[0] = m_pVecPos[m_pIndex[nIdx + 4]];
					Vec[1] = m_pVecPos[m_pIndex[nIdx + 5]];
					m_pLine->DrawTransform(Vec, 2, &mtxWorld, GRID_LINECOLOR);

					Vec[0] = m_pVecPos[m_pIndex[nIdx + 6]];
					Vec[1] = m_pVecPos[m_pIndex[nIdx + 7]];
					m_pLine->DrawTransform(Vec, 2, &mtxWorld, GRID_LINECOLOR);

					nIdx += 8;
				}
				m_pLine->End();
			}
		}
	}
}

//=============================================================================
//    頂点情報を生成する処理
//=============================================================================
void CGrid::MakeVertex(void)
{
	// 必要な数だけメモリを取得
	m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);
	m_nNumIdx = (m_nXBlock * m_nZBlock) * 8;
	m_pVecPos = new D3DXVECTOR3[m_nNumVertex];
	m_pIndex = new int[m_nNumIdx];

	// 頂点座標を設定
	float XPos = (-m_fWidth * m_nXBlock) / 2;
	float ZPos = (m_fDepth * m_nZBlock) / 2;
	int nIdx = 0;
	if (m_pVecPos != NULL)
	{
		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				m_pVecPos[nIdx] = D3DXVECTOR3(XPos, 0.0f, ZPos);
				XPos += m_fWidth;
				nIdx++;
			}
			XPos = (-m_fWidth * m_nXBlock) / 2;
			ZPos += -m_fDepth;
		}
	}

	// 頂点番号を設定
	int nVerIndex = 0;
	nIdx = 0;
	for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
	{// 垂直方向の分割数 + 1繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
		{// 水平方向の分割数 + 1だけ繰り返し
			m_pIndex[nVerIndex] = nIdx;
			m_pIndex[nVerIndex + 1] = nIdx + 1;
			m_pIndex[nVerIndex + 2] = nIdx;
			m_pIndex[nVerIndex + 3] = nIdx + m_nXBlock + 1;
			m_pIndex[nVerIndex + 4] = nIdx + 1;
			m_pIndex[nVerIndex + 5] = nIdx + m_nXBlock + 2;
			m_pIndex[nVerIndex + 6] = nIdx + m_nXBlock + 2;
			m_pIndex[nVerIndex + 7] = nIdx + m_nXBlock + 1;
			nVerIndex += 8;
			nIdx++;
		}
		nIdx++;
	}
}

//=============================================================================
//    座標設定処理
//=============================================================================
void CGrid::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    箱の横幅設定処理
//=============================================================================
void CGrid::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    箱の奥行設定処理
//=============================================================================
void CGrid::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    横の分割数設定処理
//=============================================================================
void CGrid::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    奥行の分割数設定処理
//=============================================================================
void CGrid::SetZBlock(const int nZBlock)
{
	m_nZBlock = nZBlock;
}

//=============================================================================
//    座標取得処理
//=============================================================================
D3DXVECTOR3 CGrid::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    箱の横幅取得処理
//=============================================================================
float CGrid::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    箱の奥行取得処理
//=============================================================================
float CGrid::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    奥行の分割数取得処理
//=============================================================================
int CGrid::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    横の分割数取得処理
//=============================================================================
int CGrid::GetZBlock(void)
{
	return m_nZBlock;
}