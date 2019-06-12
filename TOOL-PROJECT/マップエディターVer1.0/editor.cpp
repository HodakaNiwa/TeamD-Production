//=============================================================================
//
// エディター処理 [editor.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "editor.h"
#include "scene.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "meshfield.h"
#include "debugproc.h"
#include "fade.h"
#include "map.h"
#include "modelcreate.h"
#include "grid.h"
#include "block.h"
#include "camera.h"
#include "cameraManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "model.h"
#include "respawn.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define EDITOR_SYSTEM_FILENAME            "data/TEXT/editor.ini"    // 初期化に使用するシステムファイル名
#define EDITOR_MAP_SAVEFILENAME_INI       "map.txt"                 // マップ情報を保存するファイル名
#define EDITOR_MODELLIST_SAVEFILENAME_INI "modellist.txt"           // モデルリスト情報を保存するファイル名
#define EDITOR_TEXLIST_SAVEFILENAME_INI   "texlist.txt"             // テクスチャリスト情報を保存するファイル名
#define EDITOR_LIGHT_SAVEFILENAME_INI     "light.txt"               // ライト情報を保存するファイル名
#define EDITOR_GAMEFILED_SAVEFILENAME_INI "gamefield.txt"           // ゲームフィールド情報を保存するファイル名
#define EDITOR_OBJECT_SAVEFILENAME_INI    "object.txt"              // 配置物情報を保存するファイル名
#define EDITOR_PLAYERRESPAWN_FILENAME     "PLAYERRESPAWN_MODEL = "  // プレイヤーのリスポーンモデルのファイル名
#define EDITOR_ENEMYRESPAWN_FILENAME      "ENEMYRESPAWN_MODEL = "   // 敵のリスポーンモデルのファイル名

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================


//=============================================================================
// エディターのコンストラクタ
//=============================================================================
CEditor::CEditor()
{

}

//=============================================================================
// エディターのデストラクタ
//=============================================================================
CEditor::~CEditor()
{
	m_pMeshField = NULL;	//メッシュフィールドを初期化
}

//=============================================================================
// エディターのデストラクタ
//=============================================================================
CEditor *CEditor::Create()
{
	CEditor *pEditor = NULL;	//エディターのポインタ
	if (pEditor == NULL)
	{
		pEditor = new CEditor;	//動的確保
		if (pEditor != NULL)
		{
			pEditor->Init();	//初期化処理
		}
	}
	return pEditor;
}

//=============================================================================
// エディターの初期化処理
//=============================================================================
HRESULT CEditor::Init(void)
{
	// 変数を初期化する
	ClearVariable();

	// システム情報を読み込む
	LoadSystem();

	// カメラを生成する
	CreateCamera();

	//マップを生成
	CreateMap();

	// 地面クラスへのポインタを取得
	m_pMeshField = GetMap()->GetMeshField();

	// グリッド線を生成
	m_pGrid = CGrid::Create(D3DXVECTOR3(0.0f, 1.0f, 0.0f), MASU_SIZE_X_HALF, MASU_SIZE_Z_HALF, MASU_BLOCK_X * 2, MASU_BLOCK_Z * 2);

	// リスポーンモデルを設定
	SetRespawnModel();

	return S_OK;
}

//=============================================================================
// エディターの終了処理
//=============================================================================
void CEditor::Uninit(void)
{
	// 共通の終了処理
	CBasemode::Uninit();

	// グリッド線の破棄
	if (m_pGrid != NULL)
	{
		m_pGrid->Uninit();
		delete m_pGrid;
		m_pGrid = NULL;
	}

	//全てのオブジェクト開放
	CScene::ReleaseAll();
}

//=============================================================================
// エディターの更新処理
//=============================================================================
void CEditor::Update(void)
{
	// カメラの更新処理
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->GetCamera(m_CameraMode)->Update();
	}

	//全ての更新処理
	CScene::UpdateAll();

	// グリッド線の更新
	if (m_pGrid != NULL)
	{
		m_pGrid->Update();
	}

	// 各エディットモードによって処理わけ
	if (m_EditMode == EDITMODE_GAMEFIELD)
	{
		// 地面にマウスのレイが貫通しているか
		CheckFieldPenetration();
	}
}

//=============================================================================
// エディターの描画処理
//=============================================================================
void CEditor::Draw(void)
{
	// カメラの設定
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera(m_CameraMode);
	}

	// シーンクラスの描画処理
	CScene::DrawAll();

	// グリッド線の更新
	if (m_pGrid != NULL)
	{
		m_pGrid->Draw();
	}
}

//=============================================================================
// 地面にマウスのレイが貫通しているかチェックする処理
//=============================================================================
void CEditor::CheckFieldPenetration(void)
{
	// フィールドにマウスのレイが刺さっているかチェック
	int nMasuX = 0, nMasuZ = 0;
	int nLeftVerIndex = 0;
	D3DXVECTOR3 MouseWorldPos1 = CManager::GetMouse()->GetWorldPosZn();
	D3DXVECTOR3 MouseWorldPos2 = CManager::GetMouse()->GetWorldPosZf();
	D3DXVECTOR3 PolyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 PolyNor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 Nor = MouseWorldPos2 - MouseWorldPos1;
	D3DXVec3Normalize(&Nor, &Nor);
	if (Nor.y <= 0.0f && m_pMeshField != NULL)
	{// 地面に向かってレイが作られている && 地面が生成されている
		D3DXVECTOR3 PenePos = MouseWorldPos1 + (D3DXVec3Dot(&-MouseWorldPos1, &PolyNor) / D3DXVec3Dot(&Nor, &PolyNor) * Nor);
		if (m_pMeshField->CheckPenetration(PenePos, &nMasuX, &nMasuZ, &nLeftVerIndex) == true)
		{// 地面にレイが貫通している
			PenetrationFieldArea(nMasuX, nMasuZ, nLeftVerIndex);
			FillinFieldBox(nLeftVerIndex);
		}
	}
}

//=============================================================================
// マウスが触れているフィールドのエリアを算出する処理
//=============================================================================
void CEditor::PenetrationFieldArea(int nMasuX, int nMasuZ, int nLeftVerIndex)
{
	CInputMouse *pMouse = CManager::GetMouse();
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_2) == true)
	{// マウスの中クリックが押された
		// 押されたときのマス目を記録
		m_nMasuX = nMasuX;
		m_nMasuZ = nMasuZ;
		m_nStartLeftVerIndex = nLeftVerIndex;
	}

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_2) == true)
	{// マウスの中クリックが押されている
		// 横の分割数を計算
		int nSwitchX = 0;
		bool bSwitchX = false;
		nSwitchX = m_nMasuX - nMasuX;
		if (nSwitchX > 0)
		{
			bSwitchX = true;
		}

		// 縦の分割数を計算
		int nSwitchZ = 0;
		bool bSwitchZ = false;
		nSwitchZ = m_nMasuZ - nMasuZ;
		if (nSwitchZ > 0)
		{
			bSwitchZ = true;
		}

		// エリアの分割数を縦横それぞれ計算
		m_nBoxSide = abs(m_nMasuX - nMasuX) + 1;
		m_nBoxVertical = abs(m_nMasuZ - nMasuZ) + 1;

		// 指定されたエリアを塗りつぶす処理
		FillinFieldArea(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}

	if (pMouse->GetRelease(CInputMouse::DIMS_BUTTON_2) == true)
	{// マウスの中クリックが離された
		// 最終的な分割数を保存
		m_nBoxSide = abs(m_nMasuX - nMasuX) + 1;
		m_nBoxVertical = abs(m_nMasuZ - nMasuZ) + 1;

		// 横の分割数を計算
		int nSwitchX = 0;
		bool bSwitchX = false;
		nSwitchX = m_nMasuX - nMasuX;
		if (nSwitchX > 0)
		{
			bSwitchX = true;
		}

		// 縦の分割数を計算
		int nSwitchZ = 0;
		bool bSwitchZ = false;
		nSwitchZ = m_nMasuZ - nMasuZ;
		if (nSwitchZ > 0)
		{
			bSwitchZ = true;
		}

		// オブジェクトを配置 OR オブジェクトを破棄
		if (m_Mode == MODE_CREATE)
		{
			BlockCreate(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
		}
		else if(m_Mode == MODE_DESTROY)
		{
			BlockDestroy(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
		}
	}
}

//=============================================================================
// 指定された地面のエリアを塗りつぶす処理
//=============================================================================
void CEditor::FillinFieldArea(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// 頂点情報の設定
	int nXBlock = m_pMeshField->GetXBlock();
	int nZBlock = m_pMeshField->GetZBlock();
	int nCntVertexIndex = m_nStartLeftVerIndex;
	int nData = nCntVertexIndex;
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = m_nMasuX; nCntV < (m_nMasuX + m_nBoxVertical); nCntV++)
	{// 奥行の分割数分繰り返し
		for (int nCntH = m_nMasuZ; nCntH < (m_nMasuZ + m_nBoxSide); nCntH++)
		{// 横の分割数分繰り返し
			pVtx[nCntVertexIndex].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[nCntVertexIndex + 1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[nCntVertexIndex + (nXBlock + 1) + 1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[nCntVertexIndex + (nXBlock + 1)].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			// 頂点番号をずらす
			if (bSwitchX == false)
			{
				nCntVertexIndex++;
			}
			else
			{
				nCntVertexIndex--;
			}
		}

		// 頂点番号をずらす
		if (bSwitchZ == false)
		{
			nCntVertexIndex = (nData + nXBlock + 1);
			nData = nCntVertexIndex;
		}
		else
		{
			nCntVertexIndex = (nData - nXBlock - 1);
			nData = nCntVertexIndex;
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// マウスが触れているエリアを塗りつぶす処理
//=============================================================================
void CEditor::FillinFieldBox(int nLeftVerIndex)
{
	CInputMouse *pMouse = CManager::GetMouse();
	int nXBlock = m_pMeshField->GetXBlock();

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_2) == false)
	{// マウスの中クリックが押されていない
		VERTEX_3D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

		// 頂点バッファをロックし,頂点データへのポインタを取得
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[nLeftVerIndex].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[nLeftVerIndex + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[nLeftVerIndex + (nXBlock + 1) + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);
		pVtx[nLeftVerIndex + (nXBlock + 1)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);

		// 頂点バッファをアンロックする
		pVtxBuff->Unlock();
	}
}

//=============================================================================
// 指定されたエリアにオブジェクトを配置する処理
//=============================================================================
void CEditor::BlockCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	CModelCreate *pModelCreate = GetMap()->GetModelCreate();  // モデル管轄クラス
	CBlock *pBlock = NULL;                                    // オブジェクトクラスへのポインタ
	D3DXVECTOR3 ObjPos;                                       // オブジェクトを配置する位置
	D3DXVECTOR3 FieldPos = m_pMeshField->GetPos();            // 地面の座標
	int nXBlock = m_pMeshField->GetXBlock();                  // 地面の横の分割数
	int nZBlock = m_pMeshField->GetZBlock();                  // 地面の縦の分割数
	int nCntVertexIndex = m_nStartLeftVerIndex;               // 現在の左上の頂点番号
	int nData = nCntVertexIndex;                              // 頂点番号保存用
	int nSelect = m_nBlockModelIdx;                           // 配置するモデルの番号

	// 頂点バッファの取得
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = m_nMasuX; nCntV < (m_nMasuX + m_nBoxVertical); nCntV++)
	{// 奥行の分割数分繰り返し
		for (int nCntH = m_nMasuZ; nCntH < (m_nMasuZ + m_nBoxSide); nCntH++)
		{// 横の分割数分繰り返し
			// オブジェクトを配置する位置を計算
			ObjPos = pVtx[nCntVertexIndex].pos;
			ObjPos.x += (MASU_SIZE_X_HALF / 2);
			ObjPos.y = 0.0f;
			ObjPos.z -= (MASU_SIZE_Z_HALF / 2);

			// オブジェクトを配置する
			pBlock = CBlock::Create(ObjPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), (CBlock::TYPE)m_nBlockType, m_nBlockModelIdx,
				m_bBlockBreak, pModelCreate->GetMesh(m_nBlockModelIdx), pModelCreate->GetBuffMat(m_nBlockModelIdx),
				pModelCreate->GetNumMat(m_nBlockModelIdx), pModelCreate->GetTexture(m_nBlockModelIdx),
				m_fBlockColWidth, m_fBlockColHeight, m_fBlockColDepth);

			// 頂点番号をずらす
			if (bSwitchX == false)
			{
				nCntVertexIndex++;
			}
			else
			{
				nCntVertexIndex--;
			}
		}

		// 頂点番号をずらす
		if (bSwitchZ == false)
		{
			nCntVertexIndex = (nData + nXBlock + 1);
			nData = nCntVertexIndex;
		}
		else
		{
			nCntVertexIndex = (nData - nXBlock - 1);
			nData = nCntVertexIndex;
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 指定されたエリアのオブジェクトを破棄する処理
//=============================================================================
void CEditor::BlockDestroy(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	CBlock *pBlock = NULL;                              // ブロッククラスへのポインタ
	D3DXVECTOR3 DestroyPos;                             // オブジェクトを消す左上座標
	int nCntVertexIndex = m_nStartLeftVerIndex;         // 現在の左上の頂点番号
	int nData = nCntVertexIndex;                        // 頂点番号保存用

    // 消す範囲を先に計算
	float fDestroyWidth = (float)(m_nBoxSide * MASU_SIZE_X_HALF);
	float fDestroyDepth = (float)(m_nBoxVertical * MASU_SIZE_Z_HALF);

	// 頂点バッファの取得
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// オブジェクトを消す範囲の左上座標を設定
	DestroyPos = pVtx[nCntVertexIndex].pos;

	// オブジェクトを消す
	CScene *pScene = NULL;               // シーンクラスへのポインタ
	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// オブジェクトへのポインタだった
				pBlock = (CBlock*)pScene;
				BlockDestroyCheck(pBlock, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ);
			}

			pScene = pSceneNext;
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// オブジェクトを破棄するかチェックする処理
//=============================================================================
void CEditor::BlockDestroyCheck(CBlock *pBlock, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	D3DXVECTOR3 ObjPos = pBlock->GetPos();
	if (bSwitchX == true)
	{
		DestroyPos.x -= fDestroyWidth - MASU_SIZE_X_HALF - (MASU_SIZE_X_HALF / 2);
		if (ObjPos.x < DestroyPos.x + fDestroyWidth && ObjPos.x > DestroyPos.x - MASU_SIZE_X_HALF)
		{// X軸の判定
			if (bSwitchZ == true)
			{
				DestroyPos.z += fDestroyDepth - MASU_SIZE_Z_HALF - (MASU_SIZE_Z_HALF / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASU_SIZE_Z_HALF)
				{// Z軸の判定
					pBlock->Uninit();
					pBlock = NULL;
				}
			}
			else if(bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z軸の判定
					pBlock->Uninit();
					pBlock = NULL;
				}
			}
		}
	}
	else if(bSwitchX == false)
	{
		if (ObjPos.x < DestroyPos.x + fDestroyWidth && ObjPos.x > DestroyPos.x)
		{// X軸の判定
			if (bSwitchZ == true)
			{
				DestroyPos.z += fDestroyDepth - MASU_SIZE_Z_HALF - (MASU_SIZE_Z_HALF / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASU_SIZE_Z_HALF)
				{// Z軸の判定
					pBlock->Uninit();
					pBlock = NULL;
				}
			}
			else if (bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z軸の判定
					pBlock->Uninit();
					pBlock = NULL;
				}
			}
		}
	}
}

//=============================================================================
// エディターの変数をクリアする
//=============================================================================
void CEditor::ClearVariable(void)
{
	// 各種類ごとの変数をクリアする
	ClearEditorVariable();       // エディター用
	ClearBlockVariable();        // ブロック用
	ClearMapVariable();          // マップ用
	ClearModelListVariable();    // モデルリスト用
	ClearTexListVariable();      // テクスチャリスト用
	ClearLightVariable();        // ライト用
	ClearGameFieldVariable();    // ゲームフィールド用
	ClearObjectVariable();       // 配置物用
}

//=============================================================================
// エディターのエディター用変数をクリアする
//=============================================================================
void CEditor::ClearEditorVariable(void)
{
	m_pGrid = NULL;
	m_pMeshField = NULL;
	m_nMasuX = 0;
	m_nMasuZ = 0;
	m_nBoxSide = 0;
	m_nBoxVertical = 0;
	m_nStartLeftVerIndex = 0;
	m_Mode = MODE_CREATE;
	m_EditMode = EDITMODE_GAMEFIELD;
	m_CameraMode = CAMERAMODE_GAME;
	m_GameFieldMode = GAMEFIELDMODE_BLOCK;
	m_ObjMode = OBJECTMODE_MODEL;
}

//=============================================================================
// エディターのブロック用変数をクリアする
//=============================================================================
void CEditor::ClearBlockVariable(void)
{
	m_nBlockType = 0;
	m_nBlockModelIdx = 0;
	m_bBlockBreak = true;
	m_fBlockColWidth = MASU_SIZE_X_HALF;
	m_fBlockColHeight = MASU_SIZE_Z_HALF;
	m_fBlockColDepth = MASU_SIZE_Z_HALF;
}

//=============================================================================
// エディターのマップ用変数をクリアする
//=============================================================================
void CEditor::ClearMapVariable(void)
{
	strcpy(m_aMapFileName, "\0");
	strcpy(m_aMapSaveFileName, EDITOR_MAP_SAVEFILENAME_INI);
	strcpy(m_aMapLoadFileName, EDITOR_MAP_SAVEFILENAME_INI);
}

//=============================================================================
// エディターのモデルリスト用変数をクリアする
//=============================================================================
void CEditor::ClearModelListVariable(void)
{
	strcpy(m_aModelListSaveFileName, EDITOR_MODELLIST_SAVEFILENAME_INI);
	strcpy(m_aModelListLoadFileName, EDITOR_MODELLIST_SAVEFILENAME_INI);
}

//=============================================================================
// エディターのテクスチャリスト用変数をクリアする
//=============================================================================
void CEditor::ClearTexListVariable(void)
{
	strcpy(m_aTexListSaveFileName, EDITOR_TEXLIST_SAVEFILENAME_INI);
	strcpy(m_aTexListLoadFileName, EDITOR_TEXLIST_SAVEFILENAME_INI);
}

//=============================================================================
// エディターのライト用変数をクリアする
//=============================================================================
void CEditor::ClearLightVariable(void)
{
	strcpy(m_aLightSaveFileName, EDITOR_LIGHT_SAVEFILENAME_INI);
	strcpy(m_aLightLoadFileName, EDITOR_LIGHT_SAVEFILENAME_INI);
}

//=============================================================================
// エディターのゲームフィールド用変数をクリアする
//=============================================================================
void CEditor::ClearGameFieldVariable(void)
{
	strcpy(m_aGameFieldSaveFileName, EDITOR_GAMEFILED_SAVEFILENAME_INI);
	strcpy(m_aGameFieldLoadFileName, EDITOR_GAMEFILED_SAVEFILENAME_INI);
}

//=============================================================================
// エディターの配置物用変数をクリアする
//=============================================================================
void CEditor::ClearObjectVariable(void)
{
	strcpy(m_aObjectSaveFileName, EDITOR_OBJECT_SAVEFILENAME_INI);
	strcpy(m_aObjectLoadFileName, EDITOR_OBJECT_SAVEFILENAME_INI);
}

//=============================================================================
// エディターのカメラ生成処理
//=============================================================================
void CEditor::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create(CAMERAMODE_MAX);
	if (pCameraManager != NULL)
	{
		CCamera *pCamera = CCamera::Create();
		if (pCamera != NULL)
		{
			pCameraManager->SetCamera(pCamera, CAMERAMODE_GAME);
		}
		CEditorCamera *pEditCamera = CEditorCamera::Create();
		if (pEditCamera != NULL)
		{
			pCameraManager->SetCamera(pEditCamera, CAMERAMODE_EDIT);
		}
		CTopViewCamera *pTopViewCamera = CTopViewCamera::Create();
		if (pTopViewCamera != NULL)
		{
			pCameraManager->SetCamera(pTopViewCamera, CAMERAMODE_TOP);
		}
		SetCameraManager(pCameraManager);
	}
}

//=============================================================================
// エディターのマップを生成する
//=============================================================================
void CEditor::CreateMap(void)
{
	CMap *pMap = CMap::Create(m_aMapFileName);
	SetMap(pMap);
}

//=============================================================================
// エディターのリスポーンモデルを設定する
//=============================================================================
void CEditor::SetRespawnModel(void)
{
	// プレイヤーのリスポーンモデル
	CModel *pModel = NULL;
	CRespawn *pRespawn = NULL;
	for (int nCntRes = 0; nCntRes < MAX_PLAYER_RESPAWN; nCntRes++)
	{
		pRespawn = GetMap()->GetPlayerRespawn(nCntRes);
		pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, GetModelCreate()->GetMesh(0),
			GetModelCreate()->GetBuffMat(0), GetModelCreate()->GetNumMat(0), GetModelCreate()->GetTexture(0));
		pRespawn->SetModel(pModel);
	}

	// 敵のリスポーンモデル
	for (int nCntRes = 0; nCntRes < MAX_ENEMY_RESPAWN; nCntRes++)
	{
		pRespawn = GetMap()->GetEnemyRespawn(nCntRes);
		pModel = CModel::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, GetModelCreate()->GetMesh(1),
			GetModelCreate()->GetBuffMat(1), GetModelCreate()->GetNumMat(1), GetModelCreate()->GetTexture(1));
		pRespawn->SetModel(pModel);
	}
}

//=============================================================================
// エディターのシステム情報を読み込む
//=============================================================================
void CEditor::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(EDITOR_SYSTEM_FILENAME);
	if (pFileLoader != NULL)
	{
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// 読み込み開始の合図だった
			LoadSystemScript(pFileLoader, aStr);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// エディターのシステム情報をファイルから読み込む
//=============================================================================
void CEditor::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	CModelCreate *pModelCreate = CModelCreate::Create(2);
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, EDITOR_PLAYERRESPAWN_FILENAME) == 0)
		{// プレイヤーのリスポーンモデルファイル名だった
			LoadPlayerRespawnModel(pModelCreate, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, EDITOR_ENEMYRESPAWN_FILENAME) == 0)
		{// 敵のリスポーンモデルファイル名だった
			LoadEnemyRespawnModel(pModelCreate, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// マップファイル名だった
			LoadMapFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			SetModelCreate(pModelCreate);
			break;
		}
	}
}

//=============================================================================
// エディターのプレイヤーのリスポーンモデルファイル名を読み込む
//=============================================================================
void CEditor::LoadPlayerRespawnModel(CModelCreate *pModelCreate, char *pStr)
{
	// ファイル名を読み込み
	strcpy(m_aPlayerRespawnFileName, CFunctionLib::ReadString(pStr, m_aPlayerRespawnFileName, EDITOR_PLAYERRESPAWN_FILENAME));

	// Xファイルからモデルを読み込み
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat = 0;
	D3DXLoadMeshFromX(m_aPlayerRespawnFileName, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// モデル管轄クラスにポインタを設定する
	pModelCreate->SetMesh(pMesh, 0);
	pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, 0);
	pModelCreate->SetFileName(m_aPlayerRespawnFileName, 0);
}

//=============================================================================
// エディターの敵のリスポーンモデルファイル名を読み込む
//=============================================================================
void CEditor::LoadEnemyRespawnModel(CModelCreate *pModelCreate, char *pStr)
{
	// ファイル名を読み込み
	strcpy(m_aEnemyRespawnFileName, CFunctionLib::ReadString(pStr, m_aEnemyRespawnFileName, EDITOR_ENEMYRESPAWN_FILENAME));

	// Xファイルからモデルを読み込み
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat = 0;
	D3DXLoadMeshFromX(m_aEnemyRespawnFileName, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// モデル管轄クラスにポインタを設定する
	pModelCreate->SetMesh(pMesh, 1);
	pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, 1);
	pModelCreate->SetFileName(m_aEnemyRespawnFileName, 1);
}

//=============================================================================
// エディターの読み込むマップファイル名を読み込む
//=============================================================================
void CEditor::LoadMapFileName(char *pStr)
{
	strcpy(m_aMapFileName, CFunctionLib::ReadString(pStr, m_aMapFileName, MAP_FILENAME));
}

//=============================================================================
// エディターのシステム情報を保存する
//=============================================================================
void CEditor::SaveSystem(void)
{
	char aStr[256] = "\0";
	CFileSaver *pFileSaver = NULL;
	pFileSaver = CFileSaver::Create(EDITOR_SYSTEM_FILENAME);
	if (pFileSaver != NULL)
	{
		// ファイルの冒頭分を書き込み
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# エディターシステムファイル [editor.ini]\n");
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対消さないこと！\n", SCRIPT);
		pFileSaver->Print("\n");

		// プレイヤーのリスポーンモデルファイル名を書き込み
		SavePlayerRespawnModel(pFileSaver);

		// 敵のリスポーンモデルファイル名を書き込み
		SaveEnemyRespawnModel(pFileSaver);

		// 読み込むマップのファイル名を書き込み
		SaveMapFileName(pFileSaver);

		// スクリプト終了の合図を書き込み
		pFileSaver->Print("%s		# この行は絶対消さないこと！\n", END_SCRIPT);

		// メモリの開放
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}
}

//=============================================================================
// エディターのプレイヤーのリスポーンモデルファイル名を保存する
//=============================================================================
void CEditor::SavePlayerRespawnModel(CFileSaver *pFileSaver)
{
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# プレイヤーのリスポーンモデルファイル名\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("%s%s", EDITOR_PLAYERRESPAWN_FILENAME, m_aPlayerRespawnFileName);
	pFileSaver->Print("\n");
}

//=============================================================================
// エディターの敵のリスポーンモデルファイル名を保存する
//=============================================================================
void CEditor::SaveEnemyRespawnModel(CFileSaver *pFileSaver)
{
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 敵のリスポーンモデルファイル名\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("%s%s", EDITOR_ENEMYRESPAWN_FILENAME, m_aEnemyRespawnFileName);
	pFileSaver->Print("\n");
}

//=============================================================================
// エディターの読み込むマップファイル名を保存する
//=============================================================================
void CEditor::SaveMapFileName(CFileSaver *pFileSaver)
{
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 読み込むマップのファイル名\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("%s%s", MAP_FILENAME, m_aMapFileName);
	pFileSaver->Print("\n");
}

//=============================================================================
// モードを設定する
//=============================================================================
void CEditor::SetMode(const MODE mode)
{
	m_Mode = mode;
}

//=============================================================================
// 編集モードを設定する
//=============================================================================
void CEditor::SetEditMode(const EDITMODE editMode)
{
	m_EditMode = editMode;
}

//=============================================================================
// カメラモードを設定する
//=============================================================================
void CEditor::SetCameraMode(const CAMERAMODE cameraMode)
{
	m_CameraMode = cameraMode;
}

//=============================================================================
// ゲームフィールドに配置するオブジェクトの種類を設定する
//=============================================================================
void CEditor::SetGameFieldMode(const GAMEFIELDMODE gameFieldMode)
{
	m_GameFieldMode = gameFieldMode;
}

//=============================================================================
// 配置するオブジェクトの種類を設定する
//=============================================================================
void CEditor::SetObjectMode(const OBJECTMODE objMode)
{
	m_ObjMode = objMode;
}

//=============================================================================
// 地面クラスへのポインタを設定する
//=============================================================================
void CEditor::SetMeshField(CMeshField *pMeshField)
{
	m_pMeshField = pMeshField;
}

//=============================================================================
// 現在の横の分割数
//=============================================================================
int CEditor::GetMasuX(void)
{
	return m_nMasuX;
}

//=============================================================================
// 現在の縦の分割数
//=============================================================================
int CEditor::GetMasuZ(void)
{
	return m_nMasuZ;
}

//=============================================================================
// エリアの縦の分割数取得
//=============================================================================
int CEditor::GetBoxSide(void)
{
	return m_nBoxSide;
}

//=============================================================================
// エリアの縦の分割数取得
//=============================================================================
int CEditor::GetBoxVertical(void)
{
	return m_nBoxVertical;
}

//=============================================================================
// 地面の左上頂点番号取得
//=============================================================================
int CEditor::GetStartLeftVerIndex(void)
{
	return m_nStartLeftVerIndex;
}

//=============================================================================
// 現在のモードを取得
//=============================================================================
CEditor::MODE CEditor::GetMode(void)
{
	return m_Mode;
}

//=============================================================================
// 編集モードを取得する
//=============================================================================
CEditor::EDITMODE CEditor::GetEditMode(void)
{
	return m_EditMode;
}

//=============================================================================
// カメラモードを取得する
//=============================================================================
CEditor::CAMERAMODE CEditor::GetCameraMode(void)
{
	return m_CameraMode;
}

//=============================================================================
// ゲームフィールドに配置するオブジェクトの種類を取得する
//=============================================================================
CEditor::GAMEFIELDMODE CEditor::GetGameFieldMode(void)
{
	return m_GameFieldMode;
}

//=============================================================================
// 配置するオブジェクトの種類を取得する
//=============================================================================
CEditor::OBJECTMODE CEditor::GetObjectMode(void)
{
	return m_ObjMode;
}

//=============================================================================
// ブロックの種類番号を設定
//=============================================================================
void CEditor::SetBlockType(const int nType)
{
	m_nBlockType = nType;
}

//=============================================================================
// ブロックの使用するモデルの番号を設定
//=============================================================================
void CEditor::SetBlockModelIdx(const int nModelIdx)
{
	m_nBlockModelIdx = nModelIdx;
}

//=============================================================================
// ブロックの種類番号を設定
//=============================================================================
void CEditor::SetBlockBreak(const bool bBreak)
{
	m_bBlockBreak = bBreak;
}

//=============================================================================
// ブロックの種類番号を設定
//=============================================================================
void CEditor::SetBlockColWidth(const float fWidth)
{
	m_fBlockColWidth = fWidth;
}

//=============================================================================
// ブロックの種類番号を設定
//=============================================================================
void CEditor::SetBlockColHeight(const float fHeight)
{
	m_fBlockColHeight = fHeight;
}

//=============================================================================
// ブロックの種類番号を設定
//=============================================================================
void CEditor::SetBlockColDepth(const float fDepth)
{
	m_fBlockColDepth = fDepth;
}

//=============================================================================
// ブロックの種類番号を取得
//=============================================================================
int CEditor::GetBlockType(void)
{
	return m_nBlockType;
}

//=============================================================================
// ブロックの使用するモデルの番号を取得
//=============================================================================
int CEditor::GetBlockModelIdx(void)
{
	return m_nBlockModelIdx;
}

//=============================================================================
// ブロックを壊せるかどうか取得
//=============================================================================
bool CEditor::GetBlockBreak(void)
{
	return m_bBlockBreak;
}

//=============================================================================
// ブロックの当たり判定高さ情報を取得
//=============================================================================
float CEditor::GetBlockColWidth(void)
{
	return m_fBlockColWidth;
}

//=============================================================================
// ブロックの当たり判定高さ情報を取得
//=============================================================================
float CEditor::GetBlockColHeight(void)
{
	return m_fBlockColHeight;
}

//=============================================================================
// ブロックの当たり判定奥行情報を取得
//=============================================================================
float CEditor::GetBlockColDepth(void)
{
	return m_fBlockColDepth;
}

//=============================================================================
// マップ情報を読み込むスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetMapLoadFileName(char *pFileName)
{
	strcpy(m_aMapLoadFileName, pFileName);
}

//=============================================================================
// マップ情報を保存するスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetMapSaveFileName(char *pFileName)
{
	strcpy(m_aMapSaveFileName, pFileName);
}

//=============================================================================
// マップ情報を読み込むスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetMapLoadFileName(void)
{
	return m_aMapLoadFileName;
}

//=============================================================================
// マップ情報を保存するスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetMapSaveFileName(void)
{
	return m_aMapSaveFileName;
}

//=============================================================================
// モデルリスト情報を読み込むスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetModelListLoadFileName(char *pFileName)
{
	strcpy(m_aModelListLoadFileName, pFileName);
}

//=============================================================================
// モデルリスト情報を保存するスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetModelListSaveFileName(char *pFileName)
{
	strcpy(m_aModelListSaveFileName, pFileName);
}

//=============================================================================
// モデルリスト情報を読み込むスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetModelListLoadFileName(void)
{
	return m_aModelListLoadFileName;
}

//=============================================================================
// モデルリスト情報を保存するスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetModelListSaveFileName(void)
{
	return m_aModelListSaveFileName;
}

//=============================================================================
// テクスチャリスト情報を読み込むスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetTexListLoadFileName(char *pFileName)
{
	strcpy(m_aTexListLoadFileName, pFileName);
}

//=============================================================================
// テクスチャリスト情報を保存するスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetTexListSaveFileName(char *pFileName)
{
	strcpy(m_aTexListSaveFileName, pFileName);
}

//=============================================================================
// テクスチャリスト情報を読み込むスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetTexListLoadFileName(void)
{
	return m_aTexListLoadFileName;
}

//=============================================================================
// テクスチャリスト情報を保存するスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetTexListSaveFileName(void)
{
	return m_aTexListSaveFileName;
}

//=============================================================================
// ライト情報を読み込むスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetLightLoadFileName(char *pFileName)
{
	strcpy(m_aLightLoadFileName, pFileName);
}

//=============================================================================
// ライト情報を保存するスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetLightSaveFileName(char *pFileName)
{
	strcpy(m_aLightSaveFileName, pFileName);
}

//=============================================================================
// ライト情報を読み込むスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetLightLoadFileName(void)
{
	return m_aLightLoadFileName;
}

//=============================================================================
// ライト情報を保存するスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetLightSaveFileName(void)
{
	return m_aLightSaveFileName;
}

//=============================================================================
// ゲームフィールド情報を読み込むスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetGameFieldLoadFileName(char *pFileName)
{
	strcpy(m_aGameFieldLoadFileName, pFileName);
}

//=============================================================================
// ゲームフィールド情報を保存するスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetGameFieldSaveFileName(char *pFileName)
{
	strcpy(m_aGameFieldSaveFileName, pFileName);
}

//=============================================================================
// ゲームフィールド情報を読み込むスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetGameFieldLoadFileName(void)
{
	return m_aGameFieldLoadFileName;
}

//=============================================================================
// ゲームフィールド情報を保存するスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetGameFieldSaveFileName(void)
{
	return m_aGameFieldSaveFileName;
}

//=============================================================================
// 配置物情報を保存するスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetObjectLoadFileName(char *pFileName)
{
	strcpy(m_aObjectLoadFileName, pFileName);
}

//=============================================================================
// 配置物情報を読み込むスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetObjectSaveFileName(char *pFileName)
{
	strcpy(m_aObjectSaveFileName, pFileName);
}

//=============================================================================
// 配置物情報を保存するスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetObjectLoadFileName(void)
{
	return m_aObjectLoadFileName;
}

//=============================================================================
// 配置物情報を読み込むスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetObjectSaveFileName(void)
{
	return m_aObjectSaveFileName;
}