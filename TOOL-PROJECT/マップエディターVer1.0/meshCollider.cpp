//*****************************************************************************
//
//     メッシュコライダーの処理[meshCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshCollider.h"
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
CMeshCollider::CMeshCollider(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pVtxBuff = NULL;                          // 頂点バッファへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュフィールドの座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // メッシュフィールドの向き
	m_fWidth = 0.0f;                            // 1ブロック分の幅
	m_fHeight = 0.0f;                           // 1ブロック分高さ
	m_nXBlock = 0;                              // 横の分割数
	m_nZBlock = 0;                              // 奥行の分割数
	m_nNumVertex = 0;                           // 頂点数
	D3DXMatrixIdentity(&m_MtxWorld);            // ワールドマトリックス
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMeshCollider::~CMeshCollider()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMeshCollider *CMeshCollider::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nXBlock, int nZBlock, int nPriority)
{
	CMeshCollider *pMeshCollder = NULL;   // メッシュコライダークラス型のポインタ
	if (pMeshCollder == NULL)
	{// メモリが空になっている
		pMeshCollder = new CMeshCollider(nPriority);
		if (pMeshCollder != NULL)
		{// インスタンスを生成できた
		    // 各種値の代入
			pMeshCollder->SetPos(pos);         // メッシュコライダーの座標
			pMeshCollder->SetRot(rot);         // メッシュコライダーの向き
			pMeshCollder->SetWidth(fWidth);    // 1ブロック分の幅
			pMeshCollder->SetHeight(fHeight);  // 1ブロック分の高さ
			pMeshCollder->SetXBlock(nXBlock);  // 横の分割数
			pMeshCollder->SetZBlock(nZBlock);  // 奥行の分割数

			if (FAILED(pMeshCollder->Init()))
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

	return pMeshCollder;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMeshCollider::Init(void)
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
			m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);   // 頂点数を計算

			// 頂点バッファの生成
			MakeVertex(pDevice);
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
void CMeshCollider::Uninit(void)
{
	// 頂点バッファの破棄
	DIRECT_RELEASE(m_pVtxBuff);

	// リリース処理
	CScene::Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CMeshCollider::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CMeshCollider::Draw(void)
{
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
		}
	}
}

//=============================================================================
//    ワールドマトリックス計算処理
//=============================================================================
void CMeshCollider::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
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
void CMeshCollider::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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
	float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
	float ZPos = (m_fHeight * m_nZBlock) / 2;  // Z座標を奥に設定
	float YPos = 0.0f;                         // Y座標

	for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
	{// 垂直方向の分割数 + 1だけ繰り返し
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// 水平方向の分割数 + 1だけ繰り返し
		    // 頂点座標
			pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

			XPos += m_fWidth;  // X座標を横に進める
		}
		XPos -= m_fWidth * (m_nXBlock + 1);     // X座標を進めていた分戻す
		ZPos -= m_fHeight;                      // Z座標を手前に進める

		// ポインタを進める
		pVtx += m_nXBlock + 1;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    頂点バッファ設定処理
//=============================================================================
void CMeshCollider::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    メッシュフィールドの座標設定処理
//=============================================================================
void CMeshCollider::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    メッシュフィールドの向き設定処理
//=============================================================================
void CMeshCollider::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    1ブロック分の幅設定処理
//=============================================================================
void CMeshCollider::SetWidth(const float fWidth)
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
void CMeshCollider::SetHeight(const float fHeight)
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
void CMeshCollider::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点バッファの破棄
		DIRECT_RELEASE(m_pVtxBuff);

		// レンダリングクラス型のポインタ
		CRenderer *pRenderer = CManager::GetRenderer();

		if (pRenderer != NULL)
		{// レンダリングクラスが生成されている
		    // デバイスの取得
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
			if (pDevice != NULL)
			{// デバイスが取得できた
		    	// 作成に必要な頂点数,インデックス数,ポリゴン数を計算
				m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1); // 頂点数を計算

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
				float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
				float ZPos = (m_fHeight * m_nZBlock) / 2;  // Z座標を奥に設定
				float YPos = 0.0f;                         // Y座標

				for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
				{// 垂直方向の分割数 + 1だけ繰り返し
					for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
					{// 水平方向の分割数 + 1だけ繰り返し
					    // 頂点座標
						pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

						XPos += m_fWidth;  // X座標を横に進める
					}
					XPos -= m_fWidth * (m_nXBlock + 1);     // X座標を進めていた分戻す
					ZPos -= m_fHeight;                      // Z座標を手前に進める

					// ポインタを進める
					pVtx += m_nXBlock + 1;
				}

				// 頂点バッファをアンロックする
				m_pVtxBuff->Unlock();
			}
		}
	}
}

//=============================================================================
//    奥行の分割数設定処理
//=============================================================================
void CMeshCollider::SetZBlock(const int nZBlock)
{
	m_nZBlock = nZBlock;

	if (m_pVtxBuff != NULL)
	{// 頂点バッファが取得できている
	    // 頂点バッファの破棄
		DIRECT_RELEASE(m_pVtxBuff);

		// レンダリングクラス型のポインタ
		CRenderer *pRenderer = CManager::GetRenderer();

		if (pRenderer != NULL)
		{// レンダリングクラスが生成されている
		    // デバイスの取得
			LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
			if (pDevice != NULL)
			{// デバイスが取得できた
			    // 作成に必要な頂点数,インデックス数,ポリゴン数を計算
				m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);     // 頂点数を計算

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
				float XPos = -(m_fWidth * m_nXBlock) / 2;  // X座標を左に設定
				float ZPos = (m_fHeight * m_nZBlock) / 2;  // Z座標を奥に設定
				float YPos = 0.0f;                         // Y座標

				for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
				{// 垂直方向の分割数 + 1だけ繰り返し
					for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
					{// 水平方向の分割数 + 1だけ繰り返し
					    // 頂点座標
						pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

						XPos += m_fWidth;   // X座標を横に進める
					}
					XPos -= m_fWidth * (m_nXBlock + 1);   // X座標を進めていた分戻す
					ZPos -= m_fHeight;                    // Z座標を手前に進める

					// ポインタを進める
					pVtx += m_nXBlock + 1;
				}

				// 頂点バッファをアンロックする
				m_pVtxBuff->Unlock();
			}
		}
	}
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CMeshCollider::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    頂点バッファ取得処理
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshCollider::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    メッシュコライダーの座標取得処理
//=============================================================================
D3DXVECTOR3 CMeshCollider::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    メッシュコライダーの向き取得処理
//=============================================================================
D3DXVECTOR3 CMeshCollider::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    1ブロック分の幅取得処理
//=============================================================================
float CMeshCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1ブロック分の高さ取得処理
//=============================================================================
float CMeshCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    横の分割数取得処理
//=============================================================================
int CMeshCollider::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    奥行の分割数取得処理
//=============================================================================
int CMeshCollider::GetZBlock(void)
{
	return m_nZBlock;
}

//=============================================================================
//    頂点数取得処理
//=============================================================================
int CMeshCollider::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CMeshCollider::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    メッシュフィールドのワールド座標における高さ取得する処理
//=============================================================================
float CMeshCollider::GetPolyHeight(D3DXVECTOR3 pos, bool *pLand, D3DXVECTOR3 *pPolyNormal)
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
//    地面に貫通しているか判定する処理
//=============================================================================
bool CMeshCollider::CheckPenetration(D3DXVECTOR3 pos, int *pXBlock, int *pYBlock, int *pLeftVerIndex)
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