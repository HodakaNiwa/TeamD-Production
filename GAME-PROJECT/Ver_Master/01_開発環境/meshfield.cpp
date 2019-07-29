//*****************************************************************************
//
//     メッシュフィールドの処理[meshField.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshfield.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CMeshField::CMeshField(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	strcpy(m_aBinaryFileName, "\0");            // 頂点情報が格納されたバイナリファイル名
	m_pVtxBuff = NULL;                          // 頂点バッファへのポインタ
	m_pIdxBuff = NULL;                          // インデックスバッファへのポインタ
	m_pTexture = NULL;	                        // テクスチャへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュフィールドの座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュフィールドの向き
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // メッシュフィールドの色
	m_fWidth = 0.0f;                            // 1ブロック分の幅
	m_fHeight = 0.0f;                           // 1ブロック分高さ
	m_nXBlock = 0;                              // 横の分割数
	m_nZBlock = 0;                              // 奥行の分割数
	m_nNumVertex = 0;                           // 頂点数
	m_nNumIndex = 0;                            // インデックス数
	m_nNumPolygon = 0;                          // ポリゴン数
	m_bCulling = true;                          // カリングするかどうか
	D3DXMatrixIdentity(&m_MtxWorld);            // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMeshField::~CMeshField()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nZBlock, int nTexSplitU, int nTexSplitV, char *pFileName, bool bCulling, int nPriority)
{
	CMeshField *pMeshField = NULL;         // メッシュフィールドクラス型のポインタ
	if (pMeshField == NULL)
	{// メモリが空になっている
		pMeshField = new CMeshField(nPriority);
		if (pMeshField != NULL)
		{// インスタンスを生成できた
		 // 各種値の代入
			pMeshField->SetPos(pos);                   // メッシュフィールドの座標
			pMeshField->SetRot(rot);                   // メッシュフィールドの向き
			pMeshField->SetCol(col);                   // メッシュフィールドの色
			pMeshField->SetWidth(fWidth);              // 1ブロック分の幅
			pMeshField->SetHeight(fHeight);            // 1ブロック分の高さ
			pMeshField->SetXBlock(nXBlock);            // 横の分割数
			pMeshField->SetZBlock(nZBlock);            // 奥行の分割数
			pMeshField->SetTexSplitU(nTexSplitU);      // テクスチャU座標の分割数
			pMeshField->SetTexSplitV(nTexSplitV);      // テクスチャV座標の分割数
			pMeshField->SetCulling(bCulling);          // カリングするかどうか
			pMeshField->SetBinaryFileName(pFileName);  // 頂点情報が格納されたバイナリファイル名

			if (FAILED(pMeshField->Init()))
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

	return pMeshField;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMeshField::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		 // 作成に必要な頂点数,インデックス数,ポリゴン数を計算
			m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);                                                               // 頂点数を計算
			m_nNumIndex = ((m_nXBlock + 1) * (m_nZBlock + 1)) + (2 * (m_nZBlock - 1)) + (m_nXBlock + 1) * (m_nZBlock - 1);  // インデックス数を設定
			m_nNumPolygon = ((m_nXBlock * m_nZBlock) * 2) + ((m_nZBlock - 1) * 4);                                          // 描画するポリゴン数を設定

																															// 頂点バッファの生成
			MakeVertex(pDevice);

			// インデックスバッファの生成
			MakeIndex(pDevice);
		}
		else
		{// デバイスを取得できなかった
			return E_FAIL;
		}
	}
	else
	{// レンダリングクラスが生成されていない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMeshField::Uninit(void)
{
	// 頂点情報の保存
	//SaveMesh();

	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// インデックスバッファの破棄
	DIRECT_RELEASE(m_pIdxBuff);

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMeshField::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	 // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Culling;

			if (m_bCulling == false)
			{// カリングしない設定だったら
				pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}

			// ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデバイスのデータストリームにバインド
			pDevice->SetIndices(m_pIdxBuff);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

			if (m_bCulling == false)
			{// カリングしない設定だったら
				pDevice->SetRenderState(D3DRS_CULLMODE, Culling);
			}
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CMeshField::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

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
//    頂点バッファ生成処理
//=============================================================================
void CMeshField::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 生成に必要な変数を宣言
	D3DXCOLOR col = GetCol();
	float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
	float ZPos = (m_fHeight * m_nZBlock) / 2;  // Z座標を奥に設定
	float fTexU = 0.0f;                        // テクスチャのU座標を右上に設定
	float fTexV = 0.0f;                        // テクスチャのV座標を右上に設定
	float YPos = 0.0f;                         // Y座標(今はランダムで計算します)

	for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
	{// 垂直方向の分割数 + 1だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数 + 1だけ繰り返し
		 // 頂点座標
			pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

			// 頂点カラー
			pVtx[nCntH].col = col;

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

			XPos += m_fWidth;                              // X座標を横に進める
			fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // テクスチャU座標を進める
														   //YPos = rand() % 20 - 10.0f;                    // 高さをランダムに計算
		}
		XPos -= m_fWidth * (m_nXBlock + 1);                               // X座標を進めていた分戻す
		ZPos -= m_fHeight;                                                // Z座標を手前に進める
		fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // テクスチャU座標を進めていた分戻す
		fTexV += ((1.0f / m_nZBlock) * m_nTexSplitV);                     // テクスチャV座標を進める

		// ポインタを進める
		pVtx += m_nXBlock + 1;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 法線計算処理
	MakeNormal();


	// 頂点情報をファイルから読み取る
	//LoadMesh();
}

//=============================================================================
//    インデックスバッファ生成処理
//=============================================================================
void CMeshField::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // インデックスデータへのポインタ
	int nCntIdx = 0;  // インデックス番号

					  // インデックスバッファをロックし,インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nZBlock; nCntIdxY++)
	{// 縦の分割数分繰り返し
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// 横の分割数+１繰り返し
		 // 上下の二つ分インデックス番号を設定
			pIdx[0] = nCntIdx + m_nXBlock + 1; // 下側
			pIdx[1] = nCntIdx;                 // 上側

			pIdx += 2;  // 2つ分進める
			if (nCntIdxY < m_nZBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , 下側が縦の分割数の最下層ではない
			 // 2 , 横の分割数分設定が終わった
				pIdx[0] = nCntIdx;                       // 上側
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // 次の下側

				pIdx += 2; // 2つ分進める
			}
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    法線計算処理
//=============================================================================
void CMeshField::MakeNormal(void)
{
	VERTEX_3D *pVtx;	                              // 頂点データへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファ

	D3DXVECTOR3 VecVer[3];	    // 法線計算用ベクトル
	D3DXVECTOR3 *pNor = NULL;   // 頂点の法線

	// 必要な数だけ動的に確保する
	pNor = new D3DXVECTOR3[(m_nXBlock * m_nZBlock) * 2];

	// 外積用にベクトルを作る
	if (pNor != NULL)
	{	// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nCntVec1 = 0;                // 番号計算用(左上の頂点番号)
		int nCntVec2 = (m_nXBlock + 1);  // 番号計算用(左下の頂点番号)
		int nCntVec3 = (m_nXBlock + 2);  // 番号計算用(右下の頂点番号)
		int nCntVec4 = 1;                // 番号計算用(右上の頂点番号)
		int nCntNor = 0;                 // 番号計算用(法線の番号)
		for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
		{// 垂直方向の分割数だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
			{// 水平方向の分割数だけ繰り返し
				VecVer[0] = pVtx[nCntVec2].pos - pVtx[nCntVec1].pos;
				VecVer[1] = pVtx[nCntVec3].pos - pVtx[nCntVec1].pos;
				VecVer[2] = pVtx[nCntVec4].pos - pVtx[nCntVec1].pos;

				// 外積を使ってベクトルにする
				D3DXVec3Cross(&pNor[nCntNor], &VecVer[1], &VecVer[0]);
				D3DXVec3Cross(&pNor[nCntNor + 1], &VecVer[2], &VecVer[1]);

				// 出てきたベクトルを正規化して法線にする
				D3DXVec3Normalize(&pNor[nCntNor], &pNor[nCntNor]);
				D3DXVec3Normalize(&pNor[nCntNor + 1], &pNor[nCntNor + 1]);

				// 番号をずらす
				nCntNor += 2;
				nCntVec1++;
				nCntVec2++;
				nCntVec3++;
				nCntVec4++;
			}
			// 右端はもう終わっているので左端になるように番号をそろえる
			nCntVec1++;
			nCntVec2++;
			nCntVec3++;
			nCntVec4++;
		}

		// 法線を代入
		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				if (nCntH == 0)
				{// 右端だった場合
					if (nCntV == 0)
					{// 一番左上の頂点番号の場合
						pVtx[nCntH].nor = (pNor[0] + pNor[1]) / 2;  // OK
					}
					else if (nCntV == m_nZBlock)
					{// 一番左下の頂点番号の場合
						pVtx[nCntH].nor = pNor[((m_nXBlock * 2) * (m_nZBlock - 1))];  // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2))] + pNor[nCntV * (m_nXBlock * 2)] + pNor[nCntV * (m_nXBlock * 2) + 1]) / 3;   // OK
					}
				}
				else if (nCntH == m_nXBlock)
				{// 右端だった場合
					if (nCntV == 0)
					{// 一番右上の頂点番号の場合
						pVtx[nCntH].nor = pNor[(m_nXBlock * 2) - 1];  // OK
					}
					else if (nCntV == m_nZBlock)
					{// 一番右下の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((m_nXBlock * 2) * m_nZBlock) - 1] + pNor[((m_nXBlock * 2) * m_nZBlock) - 2]) / 2;  // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2)] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2) + 1] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2) + 2]) / 3;  // OK
					}
				}
				else
				{// 真ん中の場合
					if (nCntV == 0)
					{// 一番上の頂点番号の場合
						pVtx[nCntH].nor = (pNor[(nCntH * 2) - 1] + pNor[(nCntH * 2)] + pNor[(nCntH * 2) + 1]) / 3;   // OK
					}
					else if (nCntV == m_nZBlock)
					{// 一番下の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nZBlock - 1))] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nZBlock - 1)) + 1] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nZBlock - 1)) + 2]) / 3; // OK
					}
					else
					{// それ以外の頂点番号の場合
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2)] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2) + 1] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2) + 2] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2)] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2) + 1] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2) + 2]) / 6;  // OK
					}
				}
			}
			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		if (pNor != NULL)
		{// メモリが確保されている
			delete[] pNor;
			pNor = NULL;
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    頂点情報が格納されたバイナリファイル名設定処理
//=============================================================================
void CMeshField::SetBinaryFileName(char *pFileName)
{
	if (pFileName != NULL)
	{
		strcpy(m_aBinaryFileName, pFileName);
	}
}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CMeshField::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CMeshField::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    インデックスバッファ設定処理
//=============================================================================
void CMeshField::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    メッシュフィールドの座標設定処理
//=============================================================================
void CMeshField::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    メッシュフィールドの向き設定処理
//=============================================================================
void CMeshField::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    メッシュフィールドの色設定処理
//=============================================================================
void CMeshField::SetCol(const D3DXCOLOR col)
{
	// 色を設定
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	 // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
			 // 頂点カラー
				pVtx[nCntH].col = m_Col;
			}
			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1ブロック分の幅設定処理
//=============================================================================
void CMeshField::SetWidth(const float fWidth)
{
	// 幅を設定
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	 // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float XPos = -(m_fWidth * m_nXBlock) / 2; // X座標を左に設定

		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].pos.x = XPos;

				XPos += m_fWidth;   // X座標を横に進める
			}
			XPos -= m_fWidth * (m_nXBlock + 1); // X座標を進めていた分戻す

												// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1ブロック分の高さ設定処理
//=============================================================================
void CMeshField::SetHeight(const float fHeight)
{
	// 高さを設定
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	 // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float ZPos = (m_fHeight * m_nZBlock) / 2; // Z座標を奥に設定

		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].pos.z = ZPos;
			}
			ZPos -= m_fHeight;   // Z座標を手前に進める

								 // ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    横の分割数設定処理
//=============================================================================
void CMeshField::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点バッファの破棄
		DIRECT_RELEASE(m_pVtxBuff);

		// インデックスバッファの破棄
		DIRECT_RELEASE(m_pIdxBuff);

		// レンダリングクラス型のポインタ
		CRenderer *pRenderer = CManager::GetRenderer();

		if (pRenderer != NULL)
		{// レンダリングクラスが生成されている
		 // デバイスの取得
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
			if (pDevice != NULL)
			{// デバイスが取得できた
			 // 作成に必要な頂点数,インデックス数,ポリゴン数を計算
				m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);                                                               // 頂点数を計算
				m_nNumIndex = ((m_nXBlock + 1) * (m_nZBlock + 1)) + (2 * (m_nZBlock - 1)) + (m_nXBlock + 1) * (m_nZBlock - 1);  // インデックス数を設定
				m_nNumPolygon = ((m_nXBlock * m_nZBlock) * 2) + ((m_nZBlock - 1) * 4);                                          // 描画するポリゴン数を設定

																																// 頂点バッファの生成
				pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&m_pVtxBuff,
					NULL);

				// 頂点情報の設定
				VERTEX_3D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 生成に必要な変数を宣言
				D3DXCOLOR col = GetCol();
				float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
				float ZPos = (m_fHeight * m_nZBlock) / 2;  // Z座標を奥に設定
				float fTexU = 0.0f;                        // テクスチャのU座標を右上に設定
				float fTexV = 0.0f;                        // テクスチャのV座標を右上に設定
				float YPos = 0.0f;                         // Y座標(今はランダムで計算します)

				for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
				{// 垂直方向の分割数 + 1だけ繰り返し
					for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
					{// 水平方向の分割数 + 1だけ繰り返し
					 // 頂点座標
						pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

						// 頂点カラー
						pVtx[nCntH].col = col;

						// テクスチャ座標
						pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

						XPos += m_fWidth;                              // X座標を横に進める
						fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // テクスチャU座標を進める
					}
					XPos -= m_fWidth * (m_nXBlock + 1);                               // X座標を進めていた分戻す
					ZPos -= m_fHeight;                                                // Z座標を手前に進める
					fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // テクスチャU座標を進めていた分戻す
					fTexV += ((1.0f / m_nZBlock) * m_nTexSplitV);                     // テクスチャV座標を進める

																					  // ポインタを進める
					pVtx += m_nXBlock + 1;
				}

				// 頂点バッファをアンロックする
				m_pVtxBuff->Unlock();

				// 法線計算処理
				MakeNormal();

				// インデックスバッファの生成
				MakeIndex(pDevice);
			}
		}
	}
}

//=============================================================================
//    奥行の分割数設定処理
//=============================================================================
void CMeshField::SetZBlock(const int nZBlock)
{
	m_nZBlock = nZBlock;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	 // 頂点バッファの破棄
		DIRECT_RELEASE(m_pVtxBuff);

		// インデックスバッファの破棄
		DIRECT_RELEASE(m_pIdxBuff);

		// レンダリングクラス型のポインタ
		CRenderer *pRenderer = CManager::GetRenderer();

		if (pRenderer != NULL)
		{// レンダリングクラスが生成されている
		 // デバイスの取得
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
			if (pDevice != NULL)
			{// デバイスが取得できた
			 // 作成に必要な頂点数,インデックス数,ポリゴン数を計算
				m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);                                                               // 頂点数を計算
				m_nNumIndex = ((m_nXBlock + 1) * (m_nZBlock + 1)) + (2 * (m_nZBlock - 1)) + (m_nXBlock + 1) * (m_nZBlock - 1);  // インデックス数を設定
				m_nNumPolygon = ((m_nXBlock * m_nZBlock) * 2) + ((m_nZBlock - 1) * 4);                                          // 描画するポリゴン数を設定

																																// 頂点バッファの生成
				pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&m_pVtxBuff,
					NULL);

				// 頂点情報の設定
				VERTEX_3D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// 生成に必要な変数を宣言
				D3DXCOLOR col = GetCol();
				float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
				float ZPos = (m_fHeight * m_nZBlock) / 2;  // Z座標を奥に設定
				float fTexU = 0.0f;                        // テクスチャのU座標を右上に設定
				float fTexV = 0.0f;                        // テクスチャのV座標を右上に設定
				float YPos = 0.0f;                         // Y座標(今はランダムで計算します)

				for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
				{// 垂直方向の分割数 + 1だけ繰り返し
					for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
					{// 水平方向の分割数 + 1だけ繰り返し
					 // 頂点座標
						pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

						// 頂点カラー
						pVtx[nCntH].col = col;

						// テクスチャ座標
						pVtx[nCntH].tex = D3DXVECTOR2(fTexU, fTexV);

						XPos += m_fWidth;                              // X座標を横に進める
						fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;  // テクスチャU座標を進める
					}
					XPos -= m_fWidth * (m_nXBlock + 1);                               // X座標を進めていた分戻す
					ZPos -= m_fHeight;                                                // Z座標を手前に進める
					fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1); // テクスチャU座標を進めていた分戻す
					fTexV += ((1.0f / m_nZBlock) * m_nTexSplitV);                     // テクスチャV座標を進める

																					  // ポインタを進める
					pVtx += m_nXBlock + 1;
				}

				// 頂点バッファをアンロックする
				m_pVtxBuff->Unlock();

				// 法線計算処理
				MakeNormal();

				// インデックスバッファの生成
				MakeIndex(pDevice);
			}
		}
	}
}

//=============================================================================
//    テクスチャU座標の分割数処理
//=============================================================================
void CMeshField::SetTexSplitU(const int nTexSplitU)
{
	m_nTexSplitU = nTexSplitU;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	 // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexU = 0.0f;   // テクスチャU座標

		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].tex.x = fTexU;
				fTexU += (1.0f / (m_nXBlock)) * m_nTexSplitU;
			}
			fTexU -= ((1.0f / (m_nXBlock)) * m_nTexSplitU) * (m_nXBlock + 1);

			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    テクスチャV座標の分割数設定処理
//=============================================================================
void CMeshField::SetTexSplitV(const int nTexSplitV)
{
	m_nTexSplitV = nTexSplitV;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	 // 頂点情報の設定
		VERTEX_3D *pVtx;

		// 頂点バッファをロックし,頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fTexV = 0.0f;   // テクスチャV座標

		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// 垂直方向の分割数 + 1だけ繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// 水平方向の分割数 + 1だけ繰り返し
				pVtx[nCntH].tex.y = fTexV;
			}
			fTexV += ((1.0f / m_nZBlock) * m_nTexSplitV);

			// ポインタを進める
			pVtx += m_nXBlock + 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    カリングするかどうか設定処理
//=============================================================================
void CMeshField::SetCulling(const bool bCulling)
{
	m_bCulling = bCulling;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CMeshField::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    頂点情報が格納されたバイナリファイル名取得処理
//=============================================================================
char *CMeshField::GetBinaryFileName(void)
{
	return m_aBinaryFileName;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshField::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    インデックスバッファ取得処理
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshField::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    テクスチャ取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshField::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    メッシュフィールドの座標取得処理
//=============================================================================
D3DXVECTOR3 CMeshField::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    メッシュフィールドの向き取得処理
//=============================================================================
D3DXVECTOR3 CMeshField::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    メッシュフィールドの色取得処理
//=============================================================================
D3DXCOLOR CMeshField::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    1ブロック分の幅取得処理
//=============================================================================
float CMeshField::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1ブロック分の高さ取得処理
//=============================================================================
float CMeshField::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    横の分割数取得処理
//=============================================================================
int CMeshField::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    奥行の分割数取得処理
//=============================================================================
int CMeshField::GetZBlock(void)
{
	return m_nZBlock;
}

//=============================================================================
//    テクスチャU座標の分割数取得処理
//=============================================================================
int CMeshField::GetTexSplitU(void)
{
	return m_nTexSplitU;
}

//=============================================================================
//    テクスチャV座標の分割数取得処理
//=============================================================================
int CMeshField::GetTexSplitV(void)
{
	return m_nTexSplitV;
}

//=============================================================================
//    頂点数取得処理
//=============================================================================
int CMeshField::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    インデックス数取得処理
//=============================================================================
int CMeshField::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    ポリゴン数取得処理
//=============================================================================
int CMeshField::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    カリングするかどうか取得処理
//=============================================================================
bool CMeshField::GetCulling(void)
{
	return m_bCulling;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CMeshField::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    メッシュフィールドのワールド座標における高さ取得する処理
//=============================================================================
float CMeshField::GetPolyHeight(D3DXVECTOR3 pos, bool *pLand, D3DXVECTOR3 *pPolyNormal)
{
	float fHeight = 0.0f;               // 高さ(最終的にこの値を返す)
	float fRivision = 0.0f;             // 高さの補正値(法線を割り出すのに0.0の高さにそろえるため)
	int nVertexIdxStart = 0;            // 左上の頂点番号(ループするたびにずれていく)
	D3DXMATRIX mtxRot, mtxTrans;        // 計算用ワールドマトリックス
	D3DXMATRIX MtxWorld;                // 地面のワールドマトリックス
	D3DXMATRIX VertexMtxWorld[4];       // 頂点のワールドマトリックス4つ分(回転,位置を含めて計算する)
	D3DXVECTOR3 VertexPos[4];           // 頂点の座標4つ分(この座標はローカル座標なので計算には使わない)
	D3DXVECTOR3 VertexWorldPos[4];      // 頂点のワールド座標4つ分(ワールドマトリックスから抜き出す)
	D3DXVECTOR3 VertexLeftWorldPos[3];  // 左側のポリゴンの頂点のワールド座標3つ分(ワールドマトリックスから抜き出す)
	D3DXVECTOR3 VertexRightWorldPos[3]; // 右側のポリゴンの頂点のワールド座標3つ分(ワールドマトリックスから抜き出す)
	D3DXVECTOR3 PolygonVerPos[3];       // ポリゴンの座標(判定用の法線を導くために必要)
	D3DXVECTOR3 PolygonNormal;          // ポリゴンの法線(判定用の法線なので見た目の法線とは異なる)
	D3DXVECTOR3 PolygonVector[2];       // ポリゴンの法線を割り出すためのベクトル
	D3DXVECTOR3 VecPosToPolygon;        // 引数の座標とポリゴンとのベクトル
	D3DXVECTOR3 VectorVerToVer;         // 外積ベクトル1本目(頂点から頂点へのベクトル)
	D3DXVECTOR3 VectorVerToPos;         // 外積ベクトル2本目(頂点から引数の座標へのベクトル)
	int nCntLeftPolygon = 0;            // 左側のポリゴンのベクトル判定を通った回数
	int nCntRightPolygon = 0;           // 右側のポリゴンのベクトル判定を通った回数

										// フィールド内にいるかどうか判定
	if (pos.x <= GetPos().x - (GetWidth() * GetXBlock()) || pos.x >= GetPos().x + (GetWidth() * GetXBlock())
		&& pos.z <= GetPos().z - (GetHeight() * GetZBlock()) || pos.z >= GetPos().z + (GetHeight() * GetZBlock()))
	{
		if (pLand != NULL)
		{
			*pLand = false;
		}
		return 0.0f;
	}

	// 地面のワールドマトリックスを取得しておく
	MtxWorld = GetMtxWorld();

	// 頂点データへのポインタを宣言
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// 頂点バッファが取得できた
	 // 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
		{// 奥行の分割数分繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
			{// 横の分割数分繰り返し
			 // 頂点のローカル座標を抜き出す
				VertexPos[0] = pVtx[nVertexIdxStart].pos;
				VertexPos[1] = pVtx[nVertexIdxStart + 1].pos;
				VertexPos[2] = pVtx[nVertexIdxStart + (m_nXBlock + 1)].pos;
				VertexPos[3] = pVtx[nVertexIdxStart + (m_nXBlock + 1) + 1].pos;

				// 頂点のワールド座標を抜き出す
				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// 1ブロック分の頂点数分繰り返し
					D3DXVec3TransformCoord(&VertexWorldPos[nCntVer], &VertexPos[nCntVer], &MtxWorld);
				}

				// 左側のポリゴンの判定開始
				// 左側のポリゴンの座標を設定
				VertexLeftWorldPos[0] = VertexWorldPos[0];
				VertexLeftWorldPos[1] = VertexWorldPos[3];
				VertexLeftWorldPos[2] = VertexWorldPos[2];
				for (int nCntLeft = 0; nCntLeft < 3; nCntLeft++)
				{// ポリゴンの頂点数分繰り返し
				 // 外積のベクトルを作り出す
					VectorVerToVer = VertexLeftWorldPos[(nCntLeft + 1) % 3] - VertexLeftWorldPos[nCntLeft];
					VectorVerToPos = pos - VertexLeftWorldPos[nCntLeft];

					if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
					{// 引数の座標が頂点と頂点のベクトルの右側にいる
						nCntLeftPolygon++;  // 判定を通った回数を進める
					}
				}
				if (nCntLeftPolygon == 3)
				{// 外積の判定を全て通った
				 // 判定用の頂点座標を作る
					PolygonVerPos[0] = D3DXVECTOR3(VertexLeftWorldPos[2].x, VertexLeftWorldPos[2].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[2].z);
					PolygonVerPos[1] = D3DXVECTOR3(VertexLeftWorldPos[1].x, VertexLeftWorldPos[1].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[1].z);
					PolygonVerPos[2] = D3DXVECTOR3(VertexLeftWorldPos[0].x, VertexLeftWorldPos[0].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[0].z);

					// 外積ベクトルを作る
					PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
					PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

					// 外積ベクトルから法線を割り出す
					D3DXVec3Cross(&PolygonNormal, &PolygonVector[0], &PolygonVector[1]);
					D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

					// 判定の仕方が0に戻したうえで計算しているので補正をするために値を保持
					fRivision = VertexLeftWorldPos[2].y;

					// 引数の座標と頂点座標のベクトルを計算
					VecPosToPolygon = pos - PolygonVerPos[0];

					// 内積の概念を利用して高さを割り出す
					float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X成分の内積を計算
					float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z成分の内積を計算
					float Dot = -DotX - DotZ;                               // X成分とZ成分の内積同士を引く
					if (PolygonNormal.y != 0.0f)
					{
						fHeight = (Dot / PolygonNormal.y) + fRivision;          // 引いたものをY成分の法線で割って補正値を足してあげる
					}
					else
					{
						return 0.0f;
					}

					// 高さから角度を求める
					//float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
					//float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
					//float DotY = (PolygonNormal.y * fHeight);
					//float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
					//float fAngle = acosf(cosSita);

					if (pLand != NULL)
					{
						*pLand = true;
					}
					if (pPolyNormal != NULL)
					{
						*pPolyNormal = PolygonNormal;
					}
				}

				// 右側のポリゴンの判定開始
				// 右側のポリゴンの座標を設定
				VertexRightWorldPos[0] = VertexWorldPos[0];
				VertexRightWorldPos[1] = VertexWorldPos[1];
				VertexRightWorldPos[2] = VertexWorldPos[3];
				for (int nCntRight = 0; nCntRight < 3; nCntRight++)
				{// ポリゴンの頂点数分繰り返し
				 // 外積のベクトルを作り出す
					VectorVerToVer = VertexRightWorldPos[(nCntRight + 1) % 3] - VertexRightWorldPos[nCntRight];
					VectorVerToPos = pos - VertexRightWorldPos[nCntRight];

					if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
					{// 引数の座標が頂点と頂点のベクトルの右側にいる
						nCntRightPolygon++;  // 判定を通った回数を進める
					}
				}
				if (nCntRightPolygon == 3)
				{// 外積の判定を全て通った
				 // 判定用の頂点座標を作る
					PolygonVerPos[0] = D3DXVECTOR3(VertexRightWorldPos[1].x, VertexRightWorldPos[1].y - VertexRightWorldPos[1].y, VertexRightWorldPos[1].z);
					PolygonVerPos[1] = D3DXVECTOR3(VertexRightWorldPos[0].x, VertexRightWorldPos[0].y - VertexRightWorldPos[1].y, VertexRightWorldPos[0].z);
					PolygonVerPos[2] = D3DXVECTOR3(VertexRightWorldPos[2].x, VertexRightWorldPos[2].y - VertexRightWorldPos[1].y, VertexRightWorldPos[2].z);

					// 外積ベクトルを作る
					PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
					PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

					// 外積ベクトルから法線を割り出す
					D3DXVec3Cross(&PolygonNormal, &PolygonVector[1], &PolygonVector[0]);
					D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

					// 判定の仕方が0に戻したうえで計算しているので補正をするために値を保持
					fRivision = VertexRightWorldPos[1].y;

					// 引数の座標と頂点座標のベクトルを計算
					VecPosToPolygon = pos - PolygonVerPos[0];

					// 内積の概念を利用して高さを割り出す
					float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X成分の内積を計算
					float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z成分の内積を計算
					float Dot = -DotX - DotZ;                               // X成分とZ成分の内積同士を引く
					if (PolygonNormal.y != 0.0f)
					{
						fHeight = (Dot / PolygonNormal.y) + fRivision;          // 引いたものをY成分の法線で割って補正値を足してあげる
					}
					else
					{
						return 0.0f;
					}

					// 高さから角度を求める
					//float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
					//float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
					//float DotY = (PolygonNormal.y * fHeight);
					//float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
					//float fAngle = acosf(cosSita);

					if (pLand != NULL)
					{
						*pLand = true;
					}
					if (pPolyNormal != NULL)
					{
						*pPolyNormal = PolygonNormal;
					}
				}

				// 判定用の変数を初期化しておく
				if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
				{// ポリゴンに乗っていると判定されている
					break;
				}
				else
				{// まだポリゴンに乗っていないと判定されている
					nCntLeftPolygon = 0;
					nCntRightPolygon = 0;
					nVertexIdxStart++;
				}
			}
			if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
			{// ポリゴンに乗っていると判定されている
				break;
			}
			else
			{// まだポリゴンに乗っていないと判定されている
			 // 右端のブロックに判定するべきポリゴンは存在しないので番号をずらしておく
				nVertexIdxStart++;
			}
		}

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}

	return fHeight;
}

//=============================================================================
//    地面の面の高さ設定処理
//=============================================================================
void CMeshField::SetPolyHeight(const D3DXVECTOR3 pos, const float fValue, const float fRange)
{
	// 頂点情報の設定
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	VERTEX_3D *pVtx;
	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
	{// 垂直方向の分割数 + 1だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数 + 1だけ繰り返し
			D3DXVECTOR3 WorldVerPos = pVtx[nCntH].pos;
			D3DXVec3TransformCoord(&WorldVerPos, &WorldVerPos, &m_MtxWorld);

			// 引数の座標から頂点までの距離を出す
			float fLength = sqrtf((WorldVerPos.x - pos.x) * (WorldVerPos.x - pos.x) + (WorldVerPos.z - pos.z) * (WorldVerPos.z - pos.z));

			if (fLength < fRange)
			{// 距離が半径の範囲内である
			 // 距離が半径内のどれくらいの比率に当たるかを計算
				float fRivision = fLength / fRange;

				// コサインカーブと比率を使ってどれくらい高くするかを計算
				float fHeight = cosf((D3DX_PI * 0.5f) * fRivision) * fValue;
				pVtx[nCntH].pos.y += fHeight;
			}
		}
		pVtx += m_nXBlock + 1;
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// 頂点バッファを設定する
	SetVtxBuff(pVtxBuff);

	// 法線計算処理
	MakeNormal();
}

//=============================================================================
//    地面に貫通しているか判定する処理
//=============================================================================
bool CMeshField::CheckPenetration(D3DXVECTOR3 pos, int *pXBlock, int *pYBlock, int *pLeftVerIndex)
{
	bool bPene = false;               // 貫通しているかどうか
	int nCntVertexIndex = 0;          // 左上の頂点番号(ループするたびに増加)
	D3DXVECTOR3 PolygonPos[4];        // ポリゴンの頂点座標
	D3DXVECTOR3 PolygonWorldPos[4];   // ポリゴンのワールド空間における頂点座標
	D3DXVECTOR3 VecA, VecB;           // 外積計算用ベクトル
	int nCntCheck = 0;                // 判定を通った回数を数える
	D3DXCOLOR VerCol;                 // 頂点カラー保存用

	// 頂点情報の設定
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtxBuff != NULL)
	{
		for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
		{// 奥行の分割数分繰り返し
			for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
			{// 横の分割数分繰り返し
				pVtx[nCntVertexIndex].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[nCntVertexIndex + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[nCntVertexIndex + (m_nXBlock + 1) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[nCntVertexIndex + (m_nXBlock + 1)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


				// ポリゴンのローカル座標を把握
				PolygonPos[0] = pVtx[nCntVertexIndex].pos;
				PolygonPos[1] = pVtx[nCntVertexIndex + 1].pos;
				PolygonPos[2] = pVtx[nCntVertexIndex + (m_nXBlock + 1) + 1].pos;
				PolygonPos[3] = pVtx[nCntVertexIndex + (m_nXBlock + 1)].pos;

				// ポリゴンのワールド空間における頂点座標を求める
				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// ポリゴンの頂点数分繰り返し
					D3DXVec3TransformCoord(&PolygonWorldPos[nCntVer], &PolygonPos[nCntVer], &m_MtxWorld);
				}

				// ポリゴン内部に貫通点があるか外積計算で判定
				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// ポリゴンの頂点数分繰り返し
					VecA = pos - PolygonPos[nCntVer];
					VecB = PolygonPos[(nCntVer + 1) % 4] - PolygonPos[nCntVer];
					if ((VecA.x * VecB.z) - (VecA.z * VecB.x) > 0)
					{// 中に貫通点がある
						nCntCheck++;
						if (nCntCheck == 4)
						{// 全ての判定を通過した
							bPene = true;
							if (pXBlock != NULL)
							{
								*pXBlock = nCntH + 1;
							}
							if (pYBlock != NULL)
							{
								*pYBlock = nCntV + 1;
							}
							if (pLeftVerIndex != NULL)
							{
								*pLeftVerIndex = nCntVertexIndex;
							}
						}
					}
				}
				nCntVertexIndex++;
				nCntCheck = 0;
			}
			nCntVertexIndex++;
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();


	return bPene;
}

//=============================================================================
//    地面の頂点情報外部ファイル保存処理
//=============================================================================
void CMeshField::SaveMesh(void)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(m_aBinaryFileName, "wb");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // 頂点情報の取得
			VERTEX_3D *pVtx;
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファ生成用

															  // 頂点バッファをロックし,頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// バイナリファイルにフィールドの情報を書き込む
			fwrite(pVtx, sizeof(VERTEX_3D), GetNumVertex(), pFile);

			// ファイルを閉じておく
			fclose(pFile);

			// 頂点バッファをアンロックする
			pVtxBuff->Unlock();
		}
	}
}

//=============================================================================
//    地面の地面の頂点情報外部ファイル読み込み処理
//=============================================================================
void CMeshField::LoadMesh(void)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(m_aBinaryFileName, "rb");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // 頂点情報の取得
			VERTEX_3D *pVtx;
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // 頂点バッファ生成用

			// 頂点バッファをロックし,頂点データへのポインタを取得
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// バイナリファイルからフィールドの情報を読み込む
			fread(pVtx, sizeof(VERTEX_3D), GetNumVertex(), pFile);

			// 頂点バッファをアンロックする
			pVtxBuff->Unlock();

			// ファイルを閉じておく
			fclose(pFile);

			// 法線計算処理
			MakeNormal();
		}
	}
}