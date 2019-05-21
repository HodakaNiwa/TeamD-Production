//=============================================================================
//
// メッシュフィールドの処理 [meshField.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "meshField.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESH_SIZE (50.0f)
#define MESH_VERTEX ((MESH_X + 1) * (MESH_Z + 1))	//頂点数
#define MESH_NUMBER (((MESH_X * MESH_Z) * 2) + ((MESH_Z - 1) * 4))
#define MESH_INDEX ((((MESH_X + 1) * 2) * (MESH_Z)) + ((MESH_Z - 1) * 2))

//=============================================================================
// コンストラクタ
//=============================================================================
CMeshField::CMeshField(int nPrioriry, OBJTYPE objtype) : CScene(nPrioriry, objtype)
{
	// 位置・向きの初期設定
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期設定
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きの初期設定
	m_pTexture = NULL;
}
//=============================================================================
// デストラクタ
//=============================================================================
CMeshField::~CMeshField()
{

}
//=============================================================================
// オブジェクトXの生成
//=============================================================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos)
{
	CMeshField *pMeshField = NULL;

	if (pMeshField == NULL)
	{
		pMeshField = new CMeshField;
		pMeshField->Init();
		pMeshField->m_pos = pos;
	}
	return pMeshField;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMeshField::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\field000.jpg"/*NULL*/, &m_pTexture);

	//頂点数の計算
	m_aVartex = (MESH_X + 1) * (MESH_Z + 1) + (MESH_X + 1) * (MESH_Z - 1) + (2 * (MESH_Z - 1));

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MESH_VERTEX,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * MESH_INDEX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,		//インデックスの情報を2バイトにする
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPolygon = 0;
	for (int nCntVtxZ = 0; nCntVtxZ < MESH_Z + 1; nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < MESH_X + 1; nCntVtxX++)
		{
			//pVtx[(nCntVtxZ * (MESH_X + 1)) + (nCntVtxX)].pos = D3DXVECTOR3(-((MESH_SIZE / 2.0f) * MESH_X) + (MESH_SIZE * nCntVtxX)/* + (MESH_SIZE * fCntVtxX)*/, 0.0f, ((MESH_SIZE / 2.0f) * MESH_Z) - (MESH_SIZE * (nCntVtxZ)));
			pVtx[(nCntVtxZ + nCntVtxX) + nCntPolygon].pos = D3DXVECTOR3((-MESH_SIZE + (nCntVtxX * MESH_SIZE)),0.0f,(MESH_SIZE - (nCntVtxZ * MESH_SIZE)));
			pVtx[(nCntVtxZ + nCntVtxX) + nCntPolygon].tex = D3DXVECTOR2(1.0f * (nCntVtxX % (MESH_X + 1)), 1.0f * (nCntVtxZ));
		}
		nCntPolygon += MESH_X;
	}

	nCntPolygon = 0;
	int nCntNor = 0;
	D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;
	D3DXVECTOR3 vec0, vec1, vec2;
	D3DXVECTOR3 nor;

	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X; nCntX++)
		{//Xの数だけ繰り返し
		 //位置の代入
			pPos0 = &pVtx[(nCntZ + nCntX + nCntPolygon)].pos;
			pPos1 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 1].pos;
			pPos2 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 2].pos;
			pPos3 = &pVtx[(nCntZ + nCntX + nCntPolygon) + 1].pos;

			//ベクトル情報取得
			vec0 = *pPos1 - *pPos0;
			vec1 = *pPos2 - *pPos0;
			vec2 = *pPos3 - *pPos0;

			D3DXVec3Cross(&nor, &vec1, &vec0);
			D3DXVec3Normalize(&nor, &nor);		//正規化する
												//法線を保管
			m_aNor[((nCntZ * 2) + (nCntX * 2) + nCntNor)] = nor;
			D3DXVec3Cross(&nor, &vec2, &vec1);
			D3DXVec3Normalize(&nor, &nor);		//正規化する
												//法線を保管
			m_aNor[((nCntZ * 2) + (nCntX * 2) + nCntNor + 1)] = nor;
		}
		nCntPolygon += MESH_X;			//ポリゴンのカウンター加算
		nCntNor += (MESH_X * 2) - 2;	//法線のカウンター加算
	}
	for (int nCntZ = 0; nCntZ < MESH_Z + 1; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X + 1; nCntX++)
		{//Xの数だけ繰り返し
			if (nCntZ == 0)
			{//上の場合
				if (nCntX == 0)
				{//(左端)の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[0] + m_aNor[1]) / 2;
				}
				else if (nCntX == MESH_X)
				{//(右端)の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[(MESH_X * 2) - 1]);
				}
				else
				{//(内側)それ以外の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[(MESH_X * nCntX - 1)]
						+ m_aNor[(MESH_X * nCntX)]
						+ m_aNor[(MESH_X * nCntX + 1)]) / 3;
				}
			}
			else if (nCntZ == MESH_Z)
			{//下の場合
				if (nCntX == 0)
				{//(左端)Xの場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = m_aNor[(2 * MESH_X * (nCntZ - 1))];
				}
				else if (nCntX == MESH_X)
				{//(右端)の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[((MESH_X * MESH_Z) * 2 - 1)]
						+ m_aNor[(MESH_X * MESH_Z) * 2 - 2]) / 2;
				}
				else
				{
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2) - 2]
						+ m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2) - 1]
						+ m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2)]) / 3;
				}
			}
			else if (nCntZ > 0 && nCntZ < MESH_Z)
			{
				if (nCntX == 0)
				{//(左端)Xの場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[(2 * MESH_X)*(nCntZ - 1)]
						+ m_aNor[(2 * MESH_X)*(nCntZ)]
						+ m_aNor[(2 * MESH_X)*(nCntZ)+1]) / 3;
				}
				else if (nCntX == MESH_X)
				{//(右端)の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[(MESH_X * nCntZ) * 2 - 1]
						+ m_aNor[(MESH_X * nCntZ) * 2 - 2]
						+ m_aNor[(MESH_X * nCntZ + 1) * 2 - 1]
						+ m_aNor[(MESH_X * nCntZ + 1) * 2 - 2]) / 4;
				}
				else
				{//それ以外の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2) - 2]
						+ m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2) - 1]
						+ m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2)]
						+ m_aNor[((2 * MESH_X) * (nCntZ)) + (nCntX * 2)]
						+ m_aNor[((2 * MESH_X) * (nCntZ)) + (nCntX * 2) - 1]
						+ m_aNor[((2 * MESH_X) * (nCntZ)) + (nCntX * 2) - 2]) / 6;
				}
			}

		}
	}

	for (int nCntCol = 0; nCntCol < MESH_VERTEX; nCntCol++)
	{
		pVtx[nCntCol].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}


	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	WORD *pIdx;	//インデックスデータへのポインタ

	//インデックスバッファをロックし、インデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	int nCntMesh = 0;

	for (int nCntVtxZ = 0; nCntVtxZ < MESH_Z; nCntVtxZ++)
	{
		for (int nCntVtxX = 0; nCntVtxX < MESH_X + 1; nCntVtxX++)
		{
			//インデックスの設定
			pIdx[(nCntVtxZ * (MESH_X + 1) * 2) + (nCntVtxX * 2) + nCntMesh] = ((nCntVtxZ + 1) * (MESH_X + 1)) + nCntVtxX;
			pIdx[(nCntVtxZ * (MESH_X + 1) * 2) + (nCntVtxX * 2) + 1 + nCntMesh] = (nCntVtxZ  * (MESH_X + 1)) + nCntVtxX;

			if (MESH_Z > 1 && nCntVtxZ != MESH_Z - 1 && nCntVtxX == MESH_X)
			{
				pIdx[(nCntVtxZ * (MESH_X + 1) * 2) + (nCntVtxX * 2) + 1 + nCntMesh + 1] = (nCntVtxZ  * (MESH_X + 1)) + nCntVtxX;
				pIdx[(nCntVtxZ * (MESH_X + 1) * 2) + (nCntVtxX * 2) + 1 + nCntMesh + 2] = ((nCntVtxZ + 2) * (MESH_X + 1));
				nCntMesh += 2;
			}
		}
	}

	//インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMeshField::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//インデックスの開放
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CMeshField::Update(void)
{
	//キーボードの取得
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetKeyboard();

	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pInputKeyboard->GetPress(DIK_0) == true)
	{
		pVtx[0].pos.y += 1.0f;
		pVtx[3].pos.y += 1.0f;
		pVtx[5].pos.y += 1.0f;
		pVtx[8].pos.y += 1.0f;
		pVtx[10].pos.y += 1.0f;
		pVtx[13].pos.y += 1.0f;
		pVtx[17].pos.y += 1.0f;
		pVtx[20].pos.y += 1.0f;
	}
	if (pInputKeyboard->GetPress(DIK_L) == true)
	{

	}
	if (pInputKeyboard->GetPress(DIK_K) == true)
	{
		pVtx[8].pos.y -= 1.0f;
	}

	int nCntPolygon = 0;						//ポリゴンのカウンター
	int nCntNor = 0;							//法線のカウンター
	D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;	//位置
	D3DXVECTOR3 vec0, vec1, vec2;				//ベクトル
	D3DXVECTOR3 nor;							//法線
	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X; nCntX++)
		{//Xの数だけ繰り返し
		 //位置の代入
			pPos0 = &pVtx[(nCntZ + nCntX + nCntPolygon)].pos;
			pPos1 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 1].pos;
			pPos2 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 2].pos;
			pPos3 = &pVtx[(nCntZ + nCntX + nCntPolygon) + 1].pos;

			//ベクトル情報取得
			vec0 = *pPos1 - *pPos0;
			vec1 = *pPos2 - *pPos0;
			vec2 = *pPos3 - *pPos0;

			D3DXVec3Cross(&nor, &vec1, &vec0);
			D3DXVec3Normalize(&nor, &nor);		//正規化する
												//法線を保管
			m_aNor[((nCntZ * 2) + (nCntX * 2) + nCntNor)] = nor;
			D3DXVec3Cross(&nor, &vec2, &vec1);
			D3DXVec3Normalize(&nor, &nor);		//正規化する
												//法線を保管
			m_aNor[((nCntZ * 2) + (nCntX * 2) + nCntNor + 1)] = nor;
		}
		nCntPolygon += MESH_X;			//ポリゴンのカウンター加算
		nCntNor += (MESH_X * 2) - 2;	//法線のカウンター加算
	}
	for (int nCntZ = 0; nCntZ < MESH_Z + 1; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X + 1; nCntX++)
		{//Xの数だけ繰り返し
			if (nCntZ == 0)
			{//上の場合
				if (nCntX == 0)
				{//(左端)の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[0] + m_aNor[1]) / 2;
				}
				else if (nCntX == MESH_X)
				{//(右端)の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[(MESH_X * 2) - 1]);
				}
				else
				{//(内側)それ以外の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[(MESH_X * nCntX - 1)]
						+ m_aNor[(MESH_X * nCntX)]
						+ m_aNor[(MESH_X * nCntX + 1)]) / 3;
				}
			}
			else if (nCntZ == MESH_Z)
			{//下の場合
				if (nCntX == 0)
				{//(左端)Xの場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = m_aNor[(2 * MESH_X * (nCntZ - 1))];
				}
				else if (nCntX == MESH_X)
				{//(右端)の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[((MESH_X * MESH_Z) * 2 - 1)]
						+ m_aNor[(MESH_X * MESH_Z) * 2 - 2]) / 2;
				}
				else
				{
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2) - 2]
						+ m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2) - 1]
						+ m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2)]) / 3;
				}
			}
			else
			{
				if (nCntX == 0)
				{//(左端)Xの場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[(2 * MESH_X)*(nCntZ - 1)]
						+ m_aNor[(2 * MESH_X)*(nCntZ)]
						+ m_aNor[(2 * MESH_X)*(nCntZ)+1]) / 3;
				}
				else if (nCntX == MESH_X)
				{//(右端)の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[(MESH_X * nCntZ) * 2 - 1]
						+ m_aNor[(MESH_X * nCntZ) * 2 - 2]
						+ m_aNor[(MESH_X * nCntZ + 1) * 2 - 1]
						+ m_aNor[(MESH_X * nCntZ + 1) * 2 - 2]) / 4;
				}
				else
				{//それ以外の場合
					pVtx[(nCntZ * (MESH_Z + 1)) + (nCntX)].nor = (m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2) - 2]
						+ m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2) - 1]
						+ m_aNor[((2 * MESH_X) * (nCntZ - 1)) + (nCntX * 2)]
						+ m_aNor[((2 * MESH_X) * (nCntZ)) + (nCntX * 2)]
						+ m_aNor[((2 * MESH_X) * (nCntZ)) + (nCntX * 2) - 1]
						+ m_aNor[((2 * MESH_X) * (nCntZ)) + (nCntX * 2) - 2]) / 6;
				}
			}

		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	//デバッグ表示
	//CDebugProc::Print(1, "メッシュの起伏[0]\n");
}

//=============================================================================
// 描画処理
//=============================================================================
void CMeshField::Draw(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y,
		m_rot.x,
		m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, MESH_VERTEX, 0, MESH_NUMBER);
}

//=============================================================================
// 高さの取得
//=============================================================================
float CMeshField::GetHeight(D3DXVECTOR3 pos)
{
	bool bVtxX[MESH_X];	//X座標
	bool bVtxZ[MESH_Z];	//Y座標

	for (int nCntX = 0; nCntX < MESH_X; nCntX++)
	{
		bVtxX[nCntX] = false;
	}
	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{
		bVtxZ[nCntZ] = false;
	}

	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X; nCntX++)
		{//Xの数だけ繰り返し
			if (pos.x >= -MESH_SIZE + (nCntX * MESH_SIZE) && pos.x <= (nCntX * MESH_SIZE))
			{//左側２つのポリゴンの内側にいる場合
				bVtxX[nCntX] = true;
			}
			else
			{//右側２つのポリゴンの内側にいる場合
				bVtxX[nCntX] = false;
			}

			if (pos.z <= MESH_SIZE + (nCntZ * -MESH_SIZE) && pos.z >= MESH_SIZE + ((nCntZ+ 1) * -MESH_SIZE))
			{//上の辺より下にいて下の辺より上にいる場合
				bVtxZ[nCntZ] = true;
			}
			else
			{//上の辺より上にいて下の辺より下にいる場合
				bVtxZ[nCntZ] = false;
			}
		}
	}

	int nCntPolygon = 0;							//ポリゴンのカウンター
	int nCntNor = 0;								//法線のカウンター
	D3DXVECTOR3 *pPos0, *pPos1, *pPos2, *pPos3;		//位置
	D3DXVECTOR3 vec0, vec1, vec2,vec3;				//ベクトル
	D3DXVECTOR3 nor0, nor1;
	float fData;

	VERTEX_3D*pVtx;	//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X; nCntX++)
		{//Xの数だけ繰り返し
			if (bVtxX[nCntX] == true && bVtxZ[nCntZ] == true)
			{
				//位置の代入
				pPos0 = &pVtx[(nCntZ + nCntX + nCntPolygon)].pos;
				pPos1 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 1].pos;
				pPos2 = &pVtx[(nCntZ + nCntX + nCntPolygon) + MESH_X + 2].pos;
				pPos3 = &pVtx[(nCntZ + nCntX + nCntPolygon) + 1].pos;

				//ベクトル情報取得
				vec0 = *pPos1 - *pPos0;
				vec1 = *pPos2 - *pPos0;
				vec2 = *pPos3 - *pPos0;
				vec3 = pos - *pPos0;

				//２つのベクトルから法線を算出
				D3DXVec3Cross(&nor0, &vec0, &vec1);
				//正規化する
				D3DXVec3Normalize(&nor0, &nor0);

				//２つのベクトルから法線を算出
				D3DXVec3Cross(&nor1, &vec1, &vec2);
				//正規化する
				D3DXVec3Normalize(&nor1, &nor1);

				//２つの法線から平均値を求める
				fData = ((vec1.z * vec3.x) - (vec1.x * vec3.z));

				if (fData >= 0)
				{
					pos.y = ((nor0.x *(pos.x - pPos0->x) + nor0.z * (pos.z - pPos0->z)) / - nor0.y) + pPos0->y;
				}
				else
				{
					pos.y = ((nor1.x *(pos.x - pPos2->x) + nor1.z * (pos.z - pPos2->z)) / - nor1.y) + pPos2->y;
				}
			}
		}
		nCntPolygon += MESH_X;			//ポリゴンのカウンター加算
		nCntNor += (MESH_X * 2) - 2;	//法線のカウンター加算
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
	return pos.y;
}

//=============================================================================
// 高さの設置処理
//=============================================================================
void CMeshField::SetHeight(D3DXVECTOR3 pos, float fValue, float fRange)
{
	VERTEX_3D*pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < MESH_Z; nCntZ++)
	{//Zの数だけ繰り返し
		for (int nCntX = 0; nCntX < MESH_X; nCntX++)
		{//Xの数だけ繰り返し
			//プレイヤーの位置と頂点の距離を計算
			float fLength = sqrtf((pos.x - pVtx->pos.x) * (pos.x - pVtx->pos.x)
								+ (pos.z - pVtx->pos.z) * (pos.z - pVtx->pos.z));

			if (fLength < fRange)
			{//対象の頂点が範囲内の場合
				float fHeight = cosf((D3DX_PI / 2) *  (fLength / fRange))  * fValue;
				pVtx->pos.y += fHeight;
			}
			pVtx++;
		}
	}
	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}