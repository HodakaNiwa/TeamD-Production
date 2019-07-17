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
#include "meshCollider.h"
#include "debugproc.h"
#include "fade.h"
#include "map.h"
#include "modelcreate.h"
#include "textureManager.h"
#include "grid.h"
#include "block.h"
#include "river.h"
#include "icefield.h"
#include "camera.h"
#include "cameraManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "model.h"
#include "respawn.h"
#include "3DPolygon.h"
#include "UI.h"
#include "effectManager.h"
#include "object.h"
#include "characterManager.h"
#include "billboardObject.h"
#include "emitter.h"

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
#define EDITOR_ENEMYLIST_SAVEFILENAME_INI "enemylist.txt"           // 敵の生成情報を保存するファイル名
#define EDITOR_PLAYERRESPAWN_FILENAME     "PLAYERRESPAWN_MODEL = "  // プレイヤーのリスポーンモデルのファイル名
#define EDITOR_ENEMYRESPAWN_FILENAME      "ENEMYRESPAWN_MODEL = "   // 敵のリスポーンモデルのファイル名
#define EDITOR_POLY_HEIGHT                (1.0f)                    // エリア可視化用のポリゴンの高さ
#define EDITOR_LOAD_MAPFILENAME_DEF       "data/TEXT/MAP/map.txt"   // 読み込むマップのファイル名デフォルト値


// 読み込み用のパス
#define EFFECT_FILENAME "EFFECT_FILENAME = "

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

}

//=============================================================================
// エディターの生成処理
//=============================================================================
CEditor *CEditor::Create()
{
	CEditor *pEditor = NULL;
	if (pEditor == NULL)
	{
		pEditor = new CEditor;
		if (pEditor != NULL)
		{
			pEditor->Init();
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

	// すでに起動しているならマップのファイル名を改めて設定
	if (CManager::GetStartUp() == true)
	{
		strcpy(m_aMapFileName, CManager::GetLoadMapFileName());
	}

	// カメラを生成する
	CreateCamera();

	//マップを生成
	CreateMap();

	// UIを生成
	CreateUI();

	// 地面クラスへのポインタを取得
	m_pMeshField = GetMap()->GetMeshField();

	// メッシュコライダークラスへのポインタを取得
	m_pMeshCollider = CMeshCollider::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_BLOCK_X * 2, MASS_BLOCK_Z * 2);

	// グリッド線を生成
	m_pGrid = CGrid::Create(D3DXVECTOR3(0.0f, 2.0f, 0.0f), MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_BLOCK_X * 2, MASS_BLOCK_Z * 2);

	// エリア可視化用のポリゴンを生成
	m_pArea = C3DPolygon::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF);

	// リスポーンモデルを設定
	SetRespawnModel();

	return S_OK;
}

//=============================================================================
// エディターの終了処理
//=============================================================================
void CEditor::Uninit(void)
{
	// UIの破棄
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = NULL;
	}

	// グリッド線の破棄
	if (m_pGrid != NULL)
	{
		m_pGrid->Uninit();
		delete m_pGrid;
		m_pGrid = NULL;
	}

	// エリア可視化用のポリゴンの破棄
	if (m_pArea != NULL)
	{
		m_pArea->Uninit();
		delete m_pArea;
		m_pArea = NULL;
	}

	// ベースオブジェクトの破棄
	ReleaseBaseObject();

	// 共通の終了処理
	CBasemode::Uninit();
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
	// 編集するブロックを探す
	ShearchEditBlock();

	// カメラの設定
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera(m_CameraMode);
	}

	// シーンクラスの描画処理
	CScene::DrawAll();

	// エリア可視化用のポリゴンの描画
	if (m_pArea != NULL)
	{
		// ライティングの設定を外す
		DWORD Lighting;
		CManager::GetRenderer()->GetDevice()->GetRenderState(D3DRS_LIGHTING, &Lighting);
		CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
		m_pArea->Draw();
		CManager::GetRenderer()->GetDevice()->SetRenderState(D3DRS_LIGHTING, Lighting);
	}

	// グリッド線の描画
	if (m_pGrid != NULL && m_bDispGrid == true)
	{
		m_pGrid->Draw();
	}
}

//=============================================================================
// エリアを交換する処理
//=============================================================================
void CEditor::ExChangeArea(void)
{
	// 地面
	ExChangeField();

	// グリッド線
	ExChangeGrid();
}

//=============================================================================
// 地面を交換する処理
//=============================================================================
void CEditor::ExChangeField(void)
{
	// 今の地面を破棄
	if (m_pMeshCollider != NULL)
	{
		m_pMeshCollider->Uninit();
		m_pMeshCollider = NULL;
	}

	// 新しい地面を生成
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{// ブロックを置くモードだったら
		m_pMeshCollider = CMeshCollider::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
			MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_BLOCK_X * 2, MASS_BLOCK_Z * 2);
	}
	else
	{// それ以外
		m_pMeshCollider = CMeshCollider::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
			MASS_SIZE_X, MASS_SIZE_Z, MASS_BLOCK_X, MASS_BLOCK_Z);
	}
}

//=============================================================================
// グリッド線を交換する処理
//=============================================================================
void CEditor::ExChangeGrid(void)
{
	// 今のグリッド線を破棄
	if (m_pGrid != NULL)
	{
		m_pGrid->Uninit();
		delete m_pGrid;
		m_pGrid = NULL;
	}

	// 新しいグリッド線を生成
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{// ブロックを置くモードだったら
		m_pGrid = CGrid::Create(D3DXVECTOR3(0.0f, 1.0f, 0.0f), MASS_SIZE_X_HALF, MASS_SIZE_Z_HALF, MASS_BLOCK_X * 2, MASS_BLOCK_Z * 2);
	}
	else
	{// それ以外
		m_pGrid = CGrid::Create(D3DXVECTOR3(0.0f, 1.0f, 0.0f), MASS_SIZE_X, MASS_SIZE_Z, MASS_BLOCK_X, MASS_BLOCK_Z);
	}
}

//=============================================================================
// 地面にマウスのレイが貫通しているかチェックする処理
//=============================================================================
void CEditor::CheckFieldPenetration(void)
{
	// フィールドにマウスのレイが刺さっているかチェック
	int nMASSX = 0, nMASSZ = 0;
	D3DXVECTOR3 MouseWorldPos1 = CManager::GetMouse()->GetWorldPosZn();
	D3DXVECTOR3 MouseWorldPos2 = CManager::GetMouse()->GetWorldPosZf();
	D3DXVECTOR3 PolyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 PolyNor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 Nor = MouseWorldPos2 - MouseWorldPos1;
	D3DXVec3Normalize(&Nor, &Nor);
	if (Nor.y <= 0.0f && m_pMeshField != NULL)
	{// 地面に向かってレイが作られている && 地面が生成されている
		D3DXVECTOR3 PenePos = MouseWorldPos1 + (D3DXVec3Dot(&-MouseWorldPos1, &PolyNor) / D3DXVec3Dot(&Nor, &PolyNor) * Nor);
		if (m_pMeshCollider->CheckPenetration(PenePos, &nMASSX, &nMASSZ, &m_nLeftVerIdx) == true)
		{// 地面にレイが貫通している
		    // エリアをわかりやすくするためのポリゴンを配置
			BoxVisualization(nMASSX, nMASSZ);
			PenetrationFieldArea(nMASSX, nMASSZ, m_nLeftVerIdx);
		}
	}
}

//=============================================================================
// マウスが触れているフィールドのエリアを可視化する処理
//=============================================================================
void CEditor::BoxVisualization(int nMassX, int nMassZ)
{
	// 地面から頂点座標を取得する
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	D3DXVECTOR3 VerPos = INITIALIZE_D3DXVECTOR3;
	pVtxBuff = m_pMeshCollider->GetVtxBuff();
	float fWidth = 0.0f;
	float fDepth = 0.0f;
	float fAddWidth = 0.0f;
	float fAddDepth = 0.0f;
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{
		fAddWidth = ((MASS_SIZE_X_HALF * MASS_BLOCK_X * 2) / 2) + (MASS_SIZE_X_HALF / 2);
		fAddDepth = ((MASS_SIZE_Z_HALF * MASS_BLOCK_Z * 2) / 2) + (MASS_SIZE_Z_HALF / 2);
		fWidth = MASS_SIZE_X_HALF;
		fDepth = MASS_SIZE_Z_HALF;
	}
	else
	{
		fAddWidth = (MASS_SIZE_X * MASS_BLOCK_X) / 2 + (MASS_SIZE_X / 2);
		fAddDepth = (MASS_SIZE_Z * MASS_BLOCK_Z) / 2 + (MASS_SIZE_Z / 2);
		fWidth = MASS_SIZE_X;
		fDepth = MASS_SIZE_Z;
	}

	// 現在のエリア番号から座標を計算
	VerPos.x = (nMassX * fWidth) - fAddWidth;
	VerPos.z = -(nMassZ * fDepth) + fAddDepth;

	// エリア可視化用のポリゴンの頂点を動かす
	pVtxBuff = m_pArea->GetVtxBuff();

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの位置を設定
	m_pArea->SetPos(VerPos);

	// 頂点座標を設定
	pVtx[0].pos = D3DXVECTOR3(-fWidth / 2, EDITOR_POLY_HEIGHT, fDepth / 2);
	pVtx[1].pos = D3DXVECTOR3(fWidth / 2, EDITOR_POLY_HEIGHT, fDepth / 2);
	pVtx[2].pos = D3DXVECTOR3(-fWidth / 2, EDITOR_POLY_HEIGHT, -fDepth / 2);
	pVtx[3].pos = D3DXVECTOR3(fWidth / 2, EDITOR_POLY_HEIGHT, -fDepth / 2);

	// 色を設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// マウスが触れているフィールドのエリアを算出する処理
//=============================================================================
void CEditor::PenetrationFieldArea(int nMassX, int nMassZ, int nLeftVerIndex)
{
	CInputMouse *pMouse = CManager::GetMouse();
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	if (pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_2) == true
		|| pMouse->GetTrigger(CInputMouse::DIMS_BUTTON_0) == true && pKeyboard->GetPress(DIK_LALT) == true)
	{// マウスの中クリックが押された
		// 押されたときのマス目を記録
		m_nMassX = nMassX;
		m_nMassZ = nMassZ;
		m_nStartLeftVerIndex = nLeftVerIndex;
	}

	if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_2) == true
		|| pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == true && pKeyboard->GetPress(DIK_LALT) == true)
	{// マウスの中クリックが押されている
		// 横の分割数を計算
		int nSwitchX = 0;
		bool bSwitchX = false;
		nSwitchX = m_nMassX - nMassX;
		if (nSwitchX > 0)
		{
			bSwitchX = true;
		}

		// 縦の分割数を計算
		int nSwitchZ = 0;
		bool bSwitchZ = false;
		nSwitchZ = m_nMassZ - nMassZ;
		if (nSwitchZ > 0)
		{
			bSwitchZ = true;
		}

		// エリアの分割数を縦横それぞれ計算
		m_nBoxSide = abs(m_nMassX - nMassX) + 1;
		m_nBoxVertical = abs(m_nMassZ - nMassZ) + 1;

		// エリアをわかりやすくするためのポリゴンを配置
		AreaVisualization(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}

	if (pMouse->GetRelease(CInputMouse::DIMS_BUTTON_2) == true
		|| pMouse->GetRelease(CInputMouse::DIMS_BUTTON_0) == true && pKeyboard->GetPress(DIK_LALT) == true)
	{// マウスの中クリックが離された
		// 最終的な分割数を保存
		m_nBoxSide = abs(m_nMassX - nMassX) + 1;
		m_nBoxVertical = abs(m_nMassZ - nMassZ) + 1;

		// 横の分割数を計算
		int nSwitchX = 0;
		bool bSwitchX = false;
		nSwitchX = m_nMassX - nMassX;
		if (nSwitchX > 0)
		{
			bSwitchX = true;
		}

		// 縦の分割数を計算
		int nSwitchZ = 0;
		bool bSwitchZ = false;
		nSwitchZ = m_nMassZ - nMassZ;
		if (nSwitchZ > 0)
		{
			bSwitchZ = true;
		}

		// オブジェクトを配置 OR オブジェクトを破棄
		if (m_Mode == MODE_CREATE)
		{
			CreateGameFieldObj(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
		}
		else if(m_Mode == MODE_DESTROY)
		{
			DestroyGameFieldObj(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
		}
	}
}

//=============================================================================
// オブジェクトを配置するエリアを可視化する処理
//=============================================================================
void CEditor::AreaVisualization(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// 地面から頂点座標を取得する
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	D3DXVECTOR3 VerPos = INITIALIZE_D3DXVECTOR3;
	pVtxBuff = m_pMeshCollider->GetVtxBuff();
	int nXBlock = m_pMeshCollider->GetXBlock();

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (bSwitchX == true && bSwitchZ == false)
	{// 左下に向けて動いている
		VerPos = pVtx[m_nStartLeftVerIndex - nSwitchX].pos;
	}
	else if (bSwitchX == true && bSwitchZ == true)
	{// 左上に向けて動いている
		VerPos = pVtx[m_nLeftVerIdx].pos;
	}
	else if (bSwitchX == false && bSwitchZ == true)
	{// 右上に向けて動いている
		VerPos = pVtx[m_nStartLeftVerIndex - ((nXBlock + 1) * (m_nBoxVertical - 1))].pos;
	}
	else
	{// 通常時&右下に向けて動いている
		VerPos = pVtx[m_nStartLeftVerIndex].pos;
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();

	// エリア可視化用のポリゴンの頂点を動かす
	pVtxBuff = m_pArea->GetVtxBuff();

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fWidth = 0.0f;
	float fDepth = 0.0f;
	float fAddWidth = 0.0f;
	float fAddDepth = 0.0f;
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{
		fAddWidth = MASS_SIZE_X_HALF;
		fAddDepth = MASS_SIZE_Z_HALF;
		if (bSwitchX == true)
		{
			fWidth = MASS_SIZE_X_HALF;
		}
		else
		{
			fWidth = -MASS_SIZE_X_HALF;
		}
		if (bSwitchZ == true)
		{
			fDepth = MASS_SIZE_Z_HALF;
		}
		else
		{
			fDepth = -MASS_SIZE_Z_HALF;
		}
	}
	else
	{
		fAddWidth = MASS_SIZE_X;
		fAddDepth = MASS_SIZE_Z;
		if (bSwitchX == true)
		{
			fWidth = MASS_SIZE_X;
		}
		else
		{
			fWidth = -MASS_SIZE_X;
		}
		if (bSwitchZ == true)
		{
			fDepth = MASS_SIZE_Z;
		}
		else
		{
			fDepth = -MASS_SIZE_Z;
		}
	}

	// ポリゴンの位置を設定
	m_pArea->SetPos(INITIALIZE_D3DXVECTOR3);

	// 頂点座標を設定
	pVtx[0].pos = VerPos + D3DXVECTOR3(0.0f, EDITOR_POLY_HEIGHT, 0.0f);
	pVtx[1].pos = VerPos + D3DXVECTOR3(fWidth * nSwitchX + fAddWidth, EDITOR_POLY_HEIGHT, 0.0f);
	pVtx[2].pos = VerPos + D3DXVECTOR3(0.0f, EDITOR_POLY_HEIGHT, -fDepth * nSwitchZ - fAddDepth);
	pVtx[3].pos = VerPos + D3DXVECTOR3(fWidth * nSwitchX + fAddWidth, EDITOR_POLY_HEIGHT, -fDepth * nSwitchZ - fAddDepth);

	// 色を設定
	pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 指定されたエリアにゲームフィールド用オブジェクトを配置する処理
//=============================================================================
void CEditor::CreateGameFieldObj(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// 現在のゲームフィールドモードによって処理わけ
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{
		BlockCreate(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
	else if(m_GameFieldMode == GAMEFIELDMODE_RIVER)
	{
		RiverCreate(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
	else if (m_GameFieldMode == GAMEFIELDMODE_ICE)
	{
		IceCreate(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
}

//=============================================================================
// 指定されたエリアにブロックを配置する処理
//=============================================================================
void CEditor::BlockCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	CModelCreate *pModelCreate = GetMap()->GetModelCreate();  // モデル管轄クラス
	CBlock *pBlock = NULL;                                    // オブジェクトクラスへのポインタ
	D3DXVECTOR3 ObjPos;                                       // オブジェクトを配置する位置
	D3DXVECTOR3 FieldPos = m_pMeshCollider->GetPos();         // 地面の座標
	int nXBlock = m_pMeshCollider->GetXBlock();               // 地面の横の分割数
	int nZBlock = m_pMeshCollider->GetZBlock();               // 地面の縦の分割数
	int nCntVertexIndex = m_nStartLeftVerIndex;               // 現在の左上の頂点番号
	int nData = nCntVertexIndex;                              // 頂点番号保存用
	int nSelect = m_nBlockModelIdx;                           // 配置するモデルの番号

	// 頂点バッファの取得
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshCollider->GetVtxBuff();

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = m_nMassX; nCntV < (m_nMassX + m_nBoxVertical); nCntV++)
	{// 奥行の分割数分繰り返し
		for (int nCntH = m_nMassZ; nCntH < (m_nMassZ + m_nBoxSide); nCntH++)
		{// 横の分割数分繰り返し
			// オブジェクトを配置する位置を計算
			ObjPos = pVtx[nCntVertexIndex].pos;
			ObjPos.x += (MASS_SIZE_X_HALF / 2);
			ObjPos.y = 0.0f;
			ObjPos.z -= (MASS_SIZE_Z_HALF / 2);

			// オブジェクトを配置する
			GetMap()->CreateBlock(ObjPos, m_BlockRot, m_nBlockType, m_nBlockModelIdx,
				pModelCreate->GetMesh(m_nBlockModelIdx), pModelCreate->GetBuffMat(m_nBlockModelIdx),
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
// 指定されたエリアに川を配置する処理
//=============================================================================
void CEditor::RiverCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// 頂点情報の設定
	int nXBlock = m_pMeshField->GetXBlock();
	int nZBlock = m_pMeshField->GetZBlock();
	int nCntVertexIndex = m_nStartLeftVerIndex;
	int nData = nCntVertexIndex;
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();
	D3DXVECTOR3 RiverPos = D3DXVECTOR3(0.0f, RIVER_DOWN_FIELD + (-RIVER_DOWN_FIELD / 2), 0.0f);

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = m_nMassX; nCntV < (m_nMassX + m_nBoxVertical); nCntV++)
	{// 奥行の分割数分繰り返し
		for (int nCntH = m_nMassZ; nCntH < (m_nMassZ + m_nBoxSide); nCntH++)
		{// 横の分割数分繰り返し
			pVtx[nCntVertexIndex].pos.y = RIVER_DOWN_FIELD;
			pVtx[nCntVertexIndex + 1].pos.y = RIVER_DOWN_FIELD;
			pVtx[nCntVertexIndex + (nXBlock + 1) + 1].pos.y = RIVER_DOWN_FIELD;
			pVtx[nCntVertexIndex + (nXBlock + 1)].pos.y = RIVER_DOWN_FIELD;

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

	// 川の座標を設定
	RiverPos.x = pVtx[m_nStartLeftVerIndex].pos.x;
	RiverPos.z = pVtx[m_nStartLeftVerIndex].pos.z;

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();


	// 川を生成
	CTextureManager *pTexManager = GetMap()->GetTextureManager();
	if (bSwitchX == true)
	{
		RiverPos.x -= (MASS_SIZE_X_HALF * m_nBoxSide) - MASS_SIZE_X;
	}
	else
	{
		RiverPos.x += (MASS_SIZE_X_HALF * m_nBoxSide);
	}
	if (bSwitchZ == true)
	{
		RiverPos.z += (MASS_SIZE_Z_HALF * m_nBoxVertical) - MASS_SIZE_Z;
	}
	else
	{
		RiverPos.z -= (MASS_SIZE_Z_HALF * m_nBoxVertical);
	}

	CRiver::Create(RiverPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pTexManager->GetTexture(m_nRiverTexIdx),
		MASS_SIZE_X * m_nBoxSide, 75.0f, MASS_SIZE_Z * m_nBoxVertical, m_nBoxSide, m_nBoxVertical, m_nRiverTexIdx);
}

//=============================================================================
// 指定されたエリアに氷を配置する処理
//=============================================================================
void CEditor::IceCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// 頂点情報の設定
	int nXBlock = m_pMeshField->GetXBlock();
	int nZBlock = m_pMeshField->GetZBlock();
	int nCntVertexIndex = m_nStartLeftVerIndex;
	int nData = nCntVertexIndex;
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();
	D3DXVECTOR3 IcePos = D3DXVECTOR3(0.0f, ICE_DOWN_FIELD + (-ICE_DOWN_FIELD / 2), 0.0f);

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = m_nMassX; nCntV < (m_nMassX + m_nBoxVertical); nCntV++)
	{// 奥行の分割数分繰り返し
		for (int nCntH = m_nMassZ; nCntH < (m_nMassZ + m_nBoxSide); nCntH++)
		{// 横の分割数分繰り返し
			pVtx[nCntVertexIndex].pos.y = ICE_DOWN_FIELD;
			pVtx[nCntVertexIndex + 1].pos.y = ICE_DOWN_FIELD;
			pVtx[nCntVertexIndex + (nXBlock + 1) + 1].pos.y = ICE_DOWN_FIELD;
			pVtx[nCntVertexIndex + (nXBlock + 1)].pos.y = ICE_DOWN_FIELD;

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

	// 川の座標を設定
	IcePos.x = pVtx[m_nStartLeftVerIndex].pos.x;
	IcePos.z = pVtx[m_nStartLeftVerIndex].pos.z;

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();


	// 氷を生成
	CTextureManager *pTexManager = GetMap()->GetTextureManager();
	if (bSwitchX == true)
	{
		IcePos.x -= (MASS_SIZE_X_HALF * m_nBoxSide) - MASS_SIZE_X;
	}
	else
	{
		IcePos.x += (MASS_SIZE_X_HALF * m_nBoxSide);
	}
	if (bSwitchZ == true)
	{
		IcePos.z += (MASS_SIZE_Z_HALF * m_nBoxVertical) - MASS_SIZE_Z;
	}
	else
	{
		IcePos.z -= (MASS_SIZE_Z_HALF * m_nBoxVertical);
	}
	CIceField::Create(IcePos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pTexManager->GetTexture(m_nIceTexIdx),
		MASS_SIZE_X * m_nBoxSide, 75.0f, MASS_SIZE_Z * m_nBoxVertical, m_nBoxSide, m_nBoxVertical, m_nIceTexIdx);
}

//=============================================================================
// 指定されたエリアのオブジェクトを破棄する処理
//=============================================================================
void CEditor::DestroyGameFieldObj(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	// 現在のゲームフィールドモードによって処理わけ
	if (m_GameFieldMode == GAMEFIELDMODE_BLOCK)
	{
		BlockDestroyCheck(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
	else if (m_GameFieldMode == GAMEFIELDMODE_RIVER)
	{
		RiverDestroyCheck(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
	else if (m_GameFieldMode == GAMEFIELDMODE_ICE)
	{
		IceDestroyCheck(nSwitchX, nSwitchZ, bSwitchX, bSwitchZ);
	}
}

//=============================================================================
// 指定されたエリアのブロックを破棄する処理
//=============================================================================
void CEditor::BlockDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	D3DXVECTOR3 DestroyPos;                             // オブジェクトを消す左上座標
	int nCntVertexIndex = m_nStartLeftVerIndex;         // 現在の左上の頂点番号
	int nData = nCntVertexIndex;                        // 頂点番号保存用

    // 消す範囲を先に計算
	float fDestroyWidth = (float)(m_nBoxSide * MASS_SIZE_X_HALF);
	float fDestroyDepth = (float)(m_nBoxVertical * MASS_SIZE_Z_HALF);

	// 頂点バッファの取得
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshCollider->GetVtxBuff();

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
				BlockDestroy((CBlock*)pScene, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ);
			}

			pScene = pSceneNext;
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 指定されたエリアの川を破棄する処理
//=============================================================================
void CEditor::RiverDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	D3DXVECTOR3 DestroyPos;                             // オブジェクトを消す左上座標
	int nCntVertexIndex = m_nStartLeftVerIndex;         // 現在の左上の頂点番号
	int nData = nCntVertexIndex;                        // 頂点番号保存用

	// 消す範囲を先に計算
	float fDestroyWidth = (float)(m_nBoxSide * MASS_SIZE_X);
	float fDestroyDepth = (float)(m_nBoxVertical * MASS_SIZE_Z);

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
			if (pScene->GetObjType() == CScene::OBJTYPE_RIVER)
			{// オブジェクトへのポインタだった
				RiverDestroy((CRiver*)pScene, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ);
			}

			pScene = pSceneNext;
		}
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 指定されたエリアの氷を破棄する処理
//=============================================================================
void CEditor::IceDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ)
{
	D3DXVECTOR3 DestroyPos;                             // オブジェクトを消す左上座標
	int nCntVertexIndex = m_nStartLeftVerIndex;         // 現在の左上の頂点番号
	int nData = nCntVertexIndex;                        // 頂点番号保存用

	// 消す範囲を先に計算
	float fDestroyWidth = (float)(m_nBoxSide * MASS_SIZE_X);
	float fDestroyDepth = (float)(m_nBoxVertical * MASS_SIZE_Z);

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
			if (pScene->GetObjType() == CScene::OBJTYPE_ICEFIELD)
			{// オブジェクトへのポインタだった
				IceDestroy((CIceField*)pScene, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ);
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
bool CEditor::GameFieldObjDestroyCheck(CObject3D *pObj3D, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	bool bDeath = false;
	D3DXVECTOR3 ObjPos = pObj3D->GetPos();
	if (bSwitchX == true)
	{
		DestroyPos.x -= fDestroyWidth - MASS_SIZE_X - (MASS_SIZE_X / 2);
		if (ObjPos.x < DestroyPos.x + fDestroyWidth && ObjPos.x > DestroyPos.x - MASS_SIZE_X)
		{// X軸の判定
			if (bSwitchZ == true)
			{
				DestroyPos.z += fDestroyDepth - MASS_SIZE_Z - (MASS_SIZE_Z / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASS_SIZE_Z)
				{// Z軸の判定
					bDeath = true;
				}
			}
			else if (bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z軸の判定
					bDeath = true;
				}
			}
		}
	}
	else if (bSwitchX == false)
	{
		if (ObjPos.x < DestroyPos.x + fDestroyWidth && ObjPos.x > DestroyPos.x)
		{// X軸の判定
			if (bSwitchZ == true)
			{
				DestroyPos.z += fDestroyDepth - MASS_SIZE_Z - (MASS_SIZE_Z / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASS_SIZE_Z)
				{// Z軸の判定
					bDeath = true;
				}
			}
			else if (bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z軸の判定
					bDeath = true;
				}
			}
		}
	}

	return bDeath;
}

//=============================================================================
// オブジェクトを破棄するかチェックする処理(分割数半分)
//=============================================================================
bool CEditor::GameFieldObjDestroyCheck_Half(CObject3D *pObj3D, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	bool bDeath = false;
	D3DXVECTOR3 ObjPos = pObj3D->GetPos();
	if (bSwitchX == true)
	{
		DestroyPos.x -= fDestroyWidth - MASS_SIZE_X_HALF - (MASS_SIZE_X_HALF / 2);
		if (ObjPos.x < DestroyPos.x + fDestroyWidth && ObjPos.x > DestroyPos.x - MASS_SIZE_X_HALF)
		{// X軸の判定
			if (bSwitchZ == true)
			{
				DestroyPos.z += fDestroyDepth - MASS_SIZE_Z_HALF - (MASS_SIZE_Z_HALF / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASS_SIZE_Z_HALF)
				{// Z軸の判定
					bDeath = true;
				}
			}
			else if(bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z軸の判定
					bDeath = true;
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
				DestroyPos.z += fDestroyDepth - MASS_SIZE_Z_HALF - (MASS_SIZE_Z_HALF / 2);
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z + MASS_SIZE_Z_HALF)
				{// Z軸の判定
					bDeath = true;
				}
			}
			else if (bSwitchZ == false)
			{
				if (ObjPos.z > DestroyPos.z - fDestroyDepth && ObjPos.z < DestroyPos.z)
				{// Z軸の判定
					bDeath = true;
				}
			}
		}
	}

	if (bDeath == true && pObj3D == m_pEditBlock)
	{
		m_pEditBlock = NULL;
	}

	return bDeath;
}

//=============================================================================
// ブロックを破棄する処理
//=============================================================================
void CEditor::BlockDestroy(CBlock *pBlock, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	if (GameFieldObjDestroyCheck_Half(pBlock, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ) == true)
	{// 消す範囲内である
		pBlock->Uninit();
		pBlock = NULL;
	}
}

//=============================================================================
// 川を破棄する処理
//=============================================================================
void CEditor::RiverDestroy(CRiver *pRiver, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	if (GameFieldObjDestroyCheck(pRiver, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ) == true)
	{// 消す範囲内である
		// 地面の高さを戻す
		ReturnFieldHeight_FromRiver(pRiver);

		// 川を破棄
		pRiver->Uninit();
		pRiver = NULL;
	}
}

//=============================================================================
// 氷を破棄する処理
//=============================================================================
void CEditor::IceDestroy(CIceField *pIceField, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ)
{
	if (GameFieldObjDestroyCheck(pIceField, DestroyPos, fDestroyWidth, fDestroyDepth, bSwitchX, bSwitchZ) == true)
	{// 消す範囲内である
	    // 地面の高さを戻す
		ReturnFieldHeight_FromIce(pIceField);

		// 氷を破棄
		pIceField->Uninit();
		pIceField = NULL;
	}
}

//=============================================================================
// 地面の高さを戻す処理(川版)
//=============================================================================
void CEditor::ReturnFieldHeight_FromRiver(CRiver *pRiver)
{
	// 分割数を取得
	int nXBlock = pRiver->GetMeshField()->GetXBlock();
	int nZBlock = pRiver->GetMeshField()->GetZBlock();

	// 川の座標からどのエリアに川があるかを取得
	D3DXVECTOR3 RiverPos = pRiver->GetPos();
	int nXArea = (int)((RiverPos.x + (MASS_SIZE_X * MASS_BLOCK_X / 2)) / MASS_SIZE_X) + 1;
	int nZArea = (int)((-RiverPos.z + (MASS_SIZE_Z * MASS_BLOCK_Z / 2)) / MASS_SIZE_Z) + 1;
	int nStartLeftVertex = (nXArea - 1) + ((MASS_BLOCK_X + 1) * (nZArea - 1));

	// 分割数を考慮して戻す
	nStartLeftVertex -= (nXBlock / 2);
	nStartLeftVertex -= ((nZBlock / 2)) * (MASS_BLOCK_X + 1);

	// 頂点バッファの取得
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < nZBlock + 1; nCntZ++)
	{// 縦の分割数+1分繰り返し
		for (int nCntX = 0; nCntX < nXBlock + 1; nCntX++)
		{// 横の分割数+1分繰り返し
			pVtx[nStartLeftVertex + nCntX].pos.y = 0.0f;
		}
		nStartLeftVertex += (MASS_BLOCK_X + 1);
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// 地面の高さを戻す処理(氷版)
//=============================================================================
void CEditor::ReturnFieldHeight_FromIce(CIceField *pIceField)
{
	// 分割数を取得
	int nXBlock = pIceField->GetMeshField()->GetXBlock();
	int nZBlock = pIceField->GetMeshField()->GetZBlock();

	// 氷の座標からどのエリアに川があるかを取得
	D3DXVECTOR3 RiverPos = pIceField->GetPos();
	int nX = (int)((RiverPos.x + (MASS_SIZE_X * MASS_BLOCK_X / 2)) / MASS_SIZE_X) + 1;
	int nZ = (int)((-RiverPos.z + (MASS_SIZE_Z * MASS_BLOCK_Z / 2)) / MASS_SIZE_Z) + 1;
	int nStartLeftVertex = (nX - 1) + ((MASS_BLOCK_X + 1) * (nZ - 1));

	// 分割数を考慮して戻す
	nStartLeftVertex -= (nXBlock / 2);
	nStartLeftVertex -= ((nZBlock / 2)) * (MASS_BLOCK_X + 1);

	// 頂点バッファの取得
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pMeshField->GetVtxBuff();

	// 頂点バッファをロックし,頂点データへのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntZ = 0; nCntZ < nZBlock + 1; nCntZ++)
	{// 縦の分割数+1分繰り返し
		for (int nCntX = 0; nCntX < nXBlock + 1; nCntX++)
		{// 横の分割数+1分繰り返し
			pVtx[nStartLeftVertex + nCntX].pos.y = 0.0f;
		}
		nStartLeftVertex += (MASS_BLOCK_X + 1);
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
// データを編集するブロックを探す
//=============================================================================
void CEditor::ShearchEditBlock(void)
{
	if (CManager::GetMouse()->GetTrigger(CInputMouse::DIMS_BUTTON_1) == true)
	{
		CBlock *pBlock = ShearchBlockInArea();
		if (pBlock != NULL)
		{
			m_pEditBlock = pBlock;
		}
		else
		{
			m_pEditBlock = NULL;
		}

		// わかりやすく透明にしておく
		if (m_pEditBlock != NULL)
		{
			m_pEditBlock->SetAlpha(0.2f);
		}
	}
}

//=============================================================================
// 現在マウスがさしているエリアにオブジェクトがあるか判定する
//=============================================================================
bool CEditor::JudgAreaInObject(CObject3D *pObj3D, D3DXVECTOR3 pos, float fWidth, float fDepth)
{
	bool bCol = false;
	D3DXVECTOR3 ObjPos = pObj3D->GetPos();

	// 範囲内にいるか判定
	if (ObjPos.x < pos.x + fWidth && ObjPos.x > pos.x - fWidth
		&& ObjPos.z < pos.z + fDepth && ObjPos.z > pos.z - fDepth)
	{
		bCol = true;
	}

	return bCol;
}

//=============================================================================
// 現在マウスがさしているエリアにブロックがあるか探す
//=============================================================================
CBlock *CEditor::ShearchBlockInArea(void)
{
	// 生成か破棄が開始されている場合空のアドレスを返す
	if (CManager::GetMouse()->GetPress(CInputMouse::DIMS_BUTTON_2) == true
		|| CManager::GetMouse()->GetRelease(CInputMouse::DIMS_BUTTON_0) == true && CManager::GetKeyboard()->GetPress(DIK_LALT) == true)
		return NULL;

	// 現在マウスがさしているエリアを計算
	D3DXVECTOR3 ShearchPos = m_pArea->GetPos();
	float fShearchWidth = MASS_SIZE_X_HALF - 1.0f;
	float fShearchDepth = MASS_SIZE_Z_HALF - 1.0f;

	bool bShearch = false;
	CBlock *pBlock = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		if (bShearch == false)
		{
			pBlock = ShearchBlockInArea_Loop(nCntPriority, &bShearch, ShearchPos, fShearchWidth, fShearchDepth);
		}
	}

	return pBlock;
}

//=============================================================================
// 現在マウスがさしているエリアにブロックがあるか探す(ループ)
//=============================================================================
CBlock *CEditor::ShearchBlockInArea_Loop(int nCntPriority, bool *pShearch, D3DXVECTOR3 ShearchPos, float fShearchWidth, float fShearchDepth)
{
	float fAlpha = 0.0f;
	CBlock *pBlock = NULL;
	CBlock *pBlockJudg = NULL;
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	pScene = CScene::GetTop(nCntPriority);
	while (pScene != NULL)
	{// ポインタが空になるまで
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
		{// ブロッククラスだった
			pBlockJudg = (CBlock*)pScene;
			pBlockJudg->SetAlpha(1.0f);
			// 範囲内にブロックがあるか判定
			if (*pShearch == false)
			{
				pBlock = ShearchBlock(pBlockJudg, pShearch, ShearchPos, fShearchWidth, fShearchDepth);
			}
		}
		pScene = pSceneNext;
	}

	return pBlock;
}

//=============================================================================
// 現在マウスがさしているエリアにブロックがあるか判定
//=============================================================================
CBlock *CEditor::ShearchBlock(CBlock *pBlock, bool *pShearch, D3DXVECTOR3 ShearchPos, float fShearchWidth, float fShearchDepth)
{
	if (JudgAreaInObject(pBlock, ShearchPos, fShearchWidth, fShearchDepth) == true)
	{// エリア内にいた
		*pShearch = true;
	}
	else
	{// エリア内にいない
		pBlock = NULL;
	}

	return pBlock;
}

//=============================================================================
// エディターのベースオブジェクトクラスを生成する処理
//=============================================================================
void CEditor::CreateBaseObject(void)
{
	m_pBaseObject = CBaseObject::Create();
}

//=============================================================================
// エディターのベースオブジェクトクラスを破棄する処理
//=============================================================================
void CEditor::ReleaseBaseObject(void)
{
	if (m_pBaseObject != NULL)
	{
		m_pBaseObject->Uninit();
		delete m_pBaseObject;
		m_pBaseObject = NULL;
	}
}

//=============================================================================
// エディターのベースオブジェクトを変更する処理
//=============================================================================
void CEditor::SwitchBaseObject(int nObjMode)
{
	if (m_pBaseObject != NULL)
	{
		m_pBaseObject->SwitchObject(nObjMode);
	}
}

//=============================================================================
// エディターのベースオブジェクトに値を設定する処理
//=============================================================================
void CEditor::SetValueToBaseObject(void)
{
	// 現在生成されている種類によって処理わけ
	switch (m_ObjMode)
	{
	case CEditor::OBJECTMODE_MODEL:
		m_pBaseObject->SetValueToObject(m_ObjModelPos, m_ObjModelRot);
		break;
	case CEditor::OBJECTMODE_BILLBOARD:
		m_pBaseObject->SetValueToBillboardObj(m_ObjBillPos, m_ObjBillCol, m_fObjBillRot, m_fObjBillWidth, m_fObjBillHeight, m_bObjBillLighting, m_bObjBillDrawAddtive, m_nObjBillTexIdx);
		break;
	case CEditor::OBJECTMODE_EFFECT:
		m_pBaseObject->SetValueToEmitter(m_ObjEffectPos, m_ObjEffectRot);
		break;
	}
}

//=============================================================================
// 配置物を生成する処理
//=============================================================================
void CEditor::CreateObject(void)
{
	CMap *pMap = GetMap();
	if (pMap == NULL)return;
	pMap->GetObjectManager()[m_nObjModelType]->SetObject(m_ObjModelPos, m_ObjModelRot, m_nObjModelType);
}

//=============================================================================
// ビルボードオブジェクトを生成する処理
//=============================================================================
void CEditor::CreateBillboardObj(void)
{
	CMap *pMap = GetMap();
	if (pMap == NULL)return;

	CBillboardObject *pBillObj = CBillboardObject::Create(m_ObjBillPos, m_ObjBillCol, m_fObjBillWidth, m_fObjBillHeight,
		m_fObjBillRot, m_bObjBillLighting, m_bObjBillDrawAddtive, m_nObjBillTexIdx);
	if (pBillObj != NULL)
	{
		pBillObj->BindTexture(pMap->GetTextureManager()->GetTexture(m_nObjBillTexIdx));
	}
}

//=============================================================================
// エフェクトを生成する処理
//=============================================================================
void CEditor::CreateEffect(void)
{
	CEffectManager *pEffectManager = GetEffectManager();
	if (pEffectManager == NULL) return;
	pEffectManager->SetEffect(m_ObjEffectPos, m_ObjEffectRot, m_nObjEffectType);
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
	ClearEnemyListVariable();    // 敵の生成情報用
	ClearObjModelVariable();     // 配置モデル用
	ClearObjBillVariable();      // 配置ビルボード用
	ClearObjEffectVariable();    // 配置エフェクト用
}

//=============================================================================
// エディターのエディター用変数をクリアする
//=============================================================================
void CEditor::ClearEditorVariable(void)
{
	m_pGrid = NULL;
	m_pMeshField = NULL;
	m_pBaseObject = NULL;
	m_bDispGrid = true;
	m_bDispColRange = true;
	m_nLeftVerIdx = 0;
	m_nMassX = 0;
	m_nMassZ = 0;
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
	m_pEditBlock = NULL;
	m_nBlockType = 0;
	m_nBlockModelIdx = 0;
	m_BlockRot = INITIALIZE_D3DXVECTOR3;
	m_fBlockColWidth = MASS_SIZE_X_HALF;
	m_fBlockColHeight = MASS_SIZE_Z_HALF;
	m_fBlockColDepth = MASS_SIZE_Z_HALF;
}

//=============================================================================
// エディターの川用変数をクリアする
//=============================================================================
void CEditor::ClearRiverVariable(void)
{
	m_nRiverTexIdx = 0;
}

//=============================================================================
// エディターの氷用変数をクリアする
//=============================================================================
void CEditor::ClearIceVariable(void)
{
	m_nIceTexIdx = 0;
}

//=============================================================================
// エディターの配置モデル用変数をクリアする
//=============================================================================
void CEditor::ClearObjModelVariable(void)
{
	m_nObjModelType = 0;
	m_ObjModelPos = INITIALIZE_D3DXVECTOR3;
	m_ObjModelRot = INITIALIZE_D3DXVECTOR3;
}

//=============================================================================
// エディターの配置ビルボード用変数をクリアする
//=============================================================================
void CEditor::ClearObjBillVariable(void)
{
	m_nObjBillTexIdx = 0;
	m_ObjBillPos = INITIALIZE_D3DXVECTOR3;
	m_ObjBillCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fObjBillRot = 0.0f;
	m_fObjBillWidth = 30.0f;
	m_fObjBillHeight = 30.0f;
	m_bObjBillLighting = true;
	m_bObjBillDrawAddtive = false;
}

//=============================================================================
// エディターの配置エフェクト用変数をクリアする
//=============================================================================
void CEditor::ClearObjEffectVariable(void)
{
	m_nObjEffectType = 0;
	m_ObjEffectPos = INITIALIZE_D3DXVECTOR3;
	m_ObjEffectRot = INITIALIZE_D3DXVECTOR3;
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
// エディターの敵の生成情報用変数をクリアする
//=============================================================================
void CEditor::ClearEnemyListVariable(void)
{
	strcpy(m_aEnemyListSaveFileName, EDITOR_ENEMYLIST_SAVEFILENAME_INI);
	strcpy(m_aEnemyListLoadFileName, EDITOR_ENEMYLIST_SAVEFILENAME_INI);
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
// エディターのUIを生成する
//=============================================================================
void CEditor::CreateUI(void)
{
	m_pUI = CUI::Create();
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
		else if (CFunctionLib::Memcmp(pStr, EFFECT_FILENAME) == 0)
		{// エフェクトファイル名だった
			LoadEffectFileName(pStr);
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
// エディターの読み込むエフェクトファイル名を読み込む
//=============================================================================
void CEditor::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// エフェクト管轄クラスの生成
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
}

//=============================================================================
// エディターの読み込むマップファイル名を読み込む
//=============================================================================
void CEditor::LoadMapFileName(char *pStr)
{
	strcpy(m_aMapFileName, CFunctionLib::ReadString(pStr, m_aMapFileName, MAP_FILENAME));
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
// グリッド線を描画するかどうか設定する
//=============================================================================
void CEditor::SetGridDisp(const bool bDisp)
{
	m_bDispGrid = bDisp;
}

//=============================================================================
// 当たり判定の線を描画するかどうか設定する
//=============================================================================
void CEditor::SetColRangeDisp(const bool bDisp)
{
	m_bDispColRange = bDisp;
}

//=============================================================================
// グリッド線を描画するかどうか取得する
//=============================================================================
bool CEditor::GetGridDisp(void)
{
	return m_bDispGrid;
}

//=============================================================================
// 当たり判定の線を描画するかどうか取得する
//=============================================================================
bool CEditor::GetColRangeDisp(void)
{
	return m_bDispColRange;
}

//=============================================================================
// 現在の横の分割数
//=============================================================================
int CEditor::GetMassX(void)
{
	return m_nMassX;
}

//=============================================================================
// 現在の縦の分割数
//=============================================================================
int CEditor::GetMassZ(void)
{
	return m_nMassZ;
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
// ベースオブジェクトクラスへのポインタを取得する
//=============================================================================
CBaseObject *CEditor::GetBaseObject(void)
{
	return m_pBaseObject;
}

//=============================================================================
// 編集するブロッククラスへのポインタを設定
//=============================================================================
void CEditor::SetEditBlock(CBlock *pBlock)
{
	m_pEditBlock = pBlock;
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
// ブロックの向きを設定
//=============================================================================
void CEditor::SetBlockRot(const D3DXVECTOR3 rot)
{
	m_BlockRot = rot;
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
// 編集するブロッククラスへのポインタを取得
//=============================================================================
CBlock *CEditor::GetEditBlock(void)
{
	return m_pEditBlock;
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
// ブロックの向きを取得
//=============================================================================
D3DXVECTOR3 CEditor::GetBlockRot(void)
{
	return m_BlockRot;
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
// 川の使用するテクスチャの番号を設定する
//=============================================================================
void CEditor::SetRiverTexIdx(const int nTexIdx)
{
	m_nRiverTexIdx = nTexIdx;
}

//=============================================================================
// 川の使用するテクスチャの番号を取得する
//=============================================================================
int CEditor::GetRiverTexIdx(void)
{
	return m_nRiverTexIdx;
}

//=============================================================================
// 氷の使用するテクスチャの番号を設定する
//=============================================================================
void CEditor::SetIceTexIdx(const int nTexIdx)
{
	m_nIceTexIdx = nTexIdx;
}

//=============================================================================
// 氷の使用するテクスチャの番号を取得する
//=============================================================================
int CEditor::GetIceTexIdx(void)
{
	return m_nIceTexIdx;
}

//=============================================================================
// 配置モデルの座標を設定する
//=============================================================================
void CEditor::SetObjModelType(const int nType)
{
	m_nObjModelType = nType;
}

//=============================================================================
// 配置モデルの座標を設定する
//=============================================================================
void CEditor::SetObjModelPos(const D3DXVECTOR3 pos)
{
	m_ObjModelPos = pos;
}

//=============================================================================
// 配置モデルの座標を設定する
//=============================================================================
void CEditor::SetObjModelRot(const D3DXVECTOR3 rot)
{
	m_ObjModelRot = rot;
}

//=============================================================================
// 配置モデルの種類を取得する
//=============================================================================
int CEditor::GetObjModelType(void)
{
	return m_nObjModelType;
}

//=============================================================================
// 配置モデルの座標を取得する
//=============================================================================
D3DXVECTOR3 CEditor::GetObjModelPos(void)
{
	return m_ObjModelPos;
}

//=============================================================================
// 配置モデルの向きを取得する
//=============================================================================
D3DXVECTOR3 CEditor::GetObjModelRot(void)
{
	return m_ObjModelRot;
}

//=============================================================================
// 配置ビルボードの使用するテクスチャの番号を取得する
//=============================================================================
void CEditor::SetObjBillTexIdx(const int nTexIdx)
{
	m_nObjBillTexIdx = nTexIdx;
}

//=============================================================================
// 配置ビルボードの座標を取得する
//=============================================================================
void CEditor::SetObjBillPos(const D3DXVECTOR3 pos)
{
	m_ObjBillPos = pos;
}

//=============================================================================
// 配置ビルボードの色を取得する
//=============================================================================
void CEditor::SetObjBillCol(const D3DXCOLOR col)
{
	m_ObjBillCol = col;
}

//=============================================================================
// 配置ビルボードの向きを取得する
//=============================================================================
void CEditor::SetObjBillRot(const float fRot)
{
	m_fObjBillRot = fRot;
}

//=============================================================================
// 配置ビルボードの幅を取得する
//=============================================================================
void CEditor::SetObjBillWidth(const float fWidth)
{
	m_fObjBillWidth = fWidth;
}

//=============================================================================
// 配置ビルボードの高さを取得する
//=============================================================================
void CEditor::SetObjBillHeight(const float fHeight)
{
	m_fObjBillHeight = fHeight;
}

//=============================================================================
// 配置ビルボードのライティングするかどうかを取得する
//=============================================================================
void CEditor::SetObjBillLighting(const bool bLighting)
{
	m_bObjBillLighting = bLighting;
}

//=============================================================================
// 配置ビルボードの加算合成で描画するかどうかを取得する
//=============================================================================
void CEditor::SetObjBillDrawAddtive(const bool bDrawAddtive)
{
	m_bObjBillDrawAddtive = bDrawAddtive;
}

//=============================================================================
// 配置ビルボードの使用するテクスチャの番号を取得する
//=============================================================================
int CEditor::GetObjBillTexIdx(void)
{
	return m_nObjBillTexIdx;
}

//=============================================================================
// 配置ビルボードの座標を取得する
//=============================================================================
D3DXVECTOR3 CEditor::GetObjBillPos(void)
{
	return m_ObjBillPos;
}

//=============================================================================
// 配置ビルボードの色を取得する
//=============================================================================
D3DXCOLOR CEditor::GetObjBillCol(void)
{
	return m_ObjBillCol;
}

//=============================================================================
// 配置ビルボードの向きを取得する
//=============================================================================
float CEditor::GetObjBillRot(void)
{
	return m_fObjBillRot;
}

//=============================================================================
// 配置ビルボードの幅を取得する
//=============================================================================
float CEditor::GetObjBillWidth(void)
{
	return m_fObjBillWidth;
}

//=============================================================================
// 配置ビルボードの高さを取得する
//=============================================================================
float CEditor::GetObjBillHeight(void)
{
	return m_fObjBillHeight;
}

//=============================================================================
// 配置ビルボードのライティングするかどうかを取得する
//=============================================================================
bool CEditor::GetObjBillLighting(void)
{
	return m_bObjBillLighting;
}

//=============================================================================
// 配置ビルボードの加算合成で描画するかどうかを取得する
//=============================================================================
bool CEditor::GetObjBillDrawAddtive(void)
{
	return m_bObjBillDrawAddtive;
}

//=============================================================================
// 配置エフェクトの種類を設定する
//=============================================================================
void CEditor::SetObjEffectType(const int nType)
{
	m_nObjEffectType = nType;
}

//=============================================================================
// 配置エフェクトの座標を設定する
//=============================================================================
void CEditor::SetObjEffectPos(const D3DXVECTOR3 pos)
{
	m_ObjEffectPos = pos;
}

//=============================================================================
// 配置エフェクトの向きを設定する
//=============================================================================
void CEditor::SetObjEffectRot(const D3DXVECTOR3 rot)
{
	m_ObjEffectRot = rot;
}

//=============================================================================
// 配置エフェクトの種類を取得する
//=============================================================================
int CEditor::GetObjEffectType(void)
{
	return m_nObjEffectType;
}

//=============================================================================
// 配置エフェクトの座標を取得する
//=============================================================================
D3DXVECTOR3 CEditor::GetObjEffectPos(void)
{
	return m_ObjEffectPos;
}

//=============================================================================
// 配置エフェクトの向きを取得する
//=============================================================================
D3DXVECTOR3 CEditor::GetObjEffectRot(void)
{
	return m_ObjEffectRot;
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

//=============================================================================
// 敵の生成情報を読み込むスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetEnemyListLoadFileName(char *pFileName)
{
	strcpy(m_aEnemyListLoadFileName, pFileName);
}

//=============================================================================
// 敵の生成情報を保存するスクリプトファイル名情報を設定
//=============================================================================
void CEditor::SetEnemyListSaveFileName(char *pFileName)
{
	strcpy(m_aEnemyListSaveFileName, pFileName);
}

//=============================================================================
// 敵の生成情報を読み込むスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetEnemyListLoadFileName(void)
{
	return m_aEnemyListLoadFileName;
}

//=============================================================================
// 敵の生成情報を保存するスクリプトファイル名情報を取得
//=============================================================================
char *CEditor::GetEnemyListSaveFileName(void)
{
	return m_aEnemyListSaveFileName;
}



//*****************************************************************************
// CBaseObjectの処理
//*****************************************************************************
//=============================================================================
// コンストラクタ
//=============================================================================
CBaseObject::CBaseObject()
{
	// 各種値のクリア
	m_pObject = NULL;
	m_pBillboardObj = NULL;
	m_pEmitter = NULL;
	m_pBill = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBaseObject::~CBaseObject()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CBaseObject *CBaseObject::Create(void)
{
	CBaseObject *pBaseObject = NULL;
	if (pBaseObject == NULL)
	{
		pBaseObject = new CBaseObject;
		if (pBaseObject != NULL)
		{
			pBaseObject->Init();
		}
	}
	return pBaseObject;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBaseObject::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBaseObject::Uninit(void)
{
	// 配置物クラスの開放
	if (m_pObject != NULL)
	{
		m_pObject->Uninit();
		m_pObject = NULL;
	}

	// ビルボードオブジェクトクラスの開放
	if (m_pBillboardObj != NULL)
	{
		m_pBillboardObj->Uninit();
		m_pBillboardObj = NULL;
	}

	// エミッタクラスの開放
	if (m_pEmitter != NULL)
	{
		m_pEmitter->Uninit();
		m_pEmitter = NULL;
	}
	if (m_pBill != NULL)
	{
		m_pBill->Uninit();
		m_pBill = NULL;
	}
}

//=============================================================================
// ベースオブジェクトを切り替える処理
//=============================================================================
void CBaseObject::SwitchObject(int nObjMode)
{
	// デフォルトのオブジェクトを破棄する
	Uninit();

	// オブジェクトを生成しなおす
	switch (nObjMode)
	{
	case CEditor::OBJECTMODE_MODEL:
		CreateObject();
		break;
	case CEditor::OBJECTMODE_BILLBOARD:
		CreateBillboardObj();
		break;
	case CEditor::OBJECTMODE_EFFECT:
		CreateEmitter();
		break;
	}
}

//=============================================================================
// 配置物クラスを生成する
//=============================================================================
void CBaseObject::CreateObject(void)
{
	// エディタークラスを取得
	CEditor *pEditor = CManager::GetEditor();
	if (pEditor == NULL) return;

	// エディターから値を取得
	int nObjModelType = pEditor->GetObjModelType();
	D3DXVECTOR3 ObjModelPos = pEditor->GetObjModelPos();
	D3DXVECTOR3 ObjModelRot = pEditor->GetObjModelRot();

	// マップクラスを取得
	CMap *pMap = CManager::GetEditor()->GetMap();
	if (pMap == NULL) return;

	// キャラクター管轄クラスを使って生成しポインタを取得
	m_pObject = pMap->GetObjectManager()[nObjModelType]->SetObject(ObjModelPos, ObjModelRot, nObjModelType);
	if (m_pObject != NULL)
	{
		m_pObject->SetAlpha(0.3f);
	}
}

//=============================================================================
// ビルボードオブジェクトを生成する
//=============================================================================
void CBaseObject::CreateBillboardObj(void)
{
	// エディタークラスを取得
	CEditor *pEditor = CManager::GetEditor();
	if (pEditor == NULL) return;

	// エディターから値を取得
	int nObjBillTexIdx = pEditor->GetObjEffectType();
	D3DXVECTOR3 ObjBillPos = pEditor->GetObjEffectPos();
	D3DXCOLOR ObjBillCol = pEditor->GetObjBillCol();
	float fObjBillRot = pEditor->GetObjBillRot();
	float fObjBillWidth = pEditor->GetObjBillWidth();
	float fObjBillHeight = pEditor->GetObjBillHeight();
	bool bObjBillLighting = pEditor->GetObjBillLighting();
	bool bObjBillDrawAddtive = pEditor->GetObjBillDrawAddtive();

	// オブジェクトを生成
	m_pBillboardObj = CBillboardObject::Create(ObjBillPos, ObjBillCol, fObjBillWidth, fObjBillHeight,
		fObjBillRot, bObjBillLighting, bObjBillDrawAddtive, nObjBillTexIdx);
	if (m_pBillboardObj != NULL)
	{
		m_pBillboardObj->BindTexture(pEditor->GetMap()->GetTextureManager()->GetTexture(nObjBillTexIdx));
	}

	// エフェクトの座標表示用ポリゴンを生成
	m_pBill = CSceneBillboard::Create(ObjBillPos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, 8.0f, 8.0f);
}

//=============================================================================
// エミッタクラスを生成する
//=============================================================================
void CBaseObject::CreateEmitter(void)
{
	// エディタークラスを取得
	CEditor *pEditor = CManager::GetEditor();
	if (pEditor == NULL) return;

	// エディターから値を取得
	int nObjEffectType = pEditor->GetObjEffectType();
	D3DXVECTOR3 ObjEffectPos = pEditor->GetObjEffectPos();
	D3DXVECTOR3 ObjEffectRot = pEditor->GetObjEffectRot();

	// エフェクト管轄クラスから生成しポインタを取得
	m_pEmitter = pEditor->GetEffectManager()->SetEffect(ObjEffectPos, ObjEffectRot, nObjEffectType);

	// エフェクトの座標表示用ポリゴンを生成
	m_pBill = CSceneBillboard::Create(ObjEffectPos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, 8.0f, 8.0f, false, EFFECT_PRIORITY + 1);
}

//=============================================================================
// 配置物クラスに値を設定する処理
//=============================================================================
void CBaseObject::SetValueToObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pObject == NULL)return;
	m_pObject->SetPos(pos);
	m_pObject->SetRot(rot);

}

//=============================================================================
// ビルボードオブジェクトクラスに値を設定する処理
//=============================================================================
void CBaseObject::SetValueToBillboardObj(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, bool bLighting, bool bDrawAddtive, int nTexIdx)
{
	if (m_pBillboardObj == NULL)return;
	m_pBillboardObj->SetPos(pos);
	m_pBillboardObj->SetCol(col);
	m_pBillboardObj->SetRot(fRot);
	m_pBillboardObj->SetWidth(fWidth);
	m_pBillboardObj->SetHeight(fHeight);
	m_pBillboardObj->SetLighting(bLighting);
	m_pBillboardObj->SetDrawAddtive(bDrawAddtive);
	m_pBillboardObj->SetTexIdx(nTexIdx);
	m_pBillboardObj->SetVtxBuffValue();

	if (m_pBill == NULL)return;
	m_pBill->SetPos(pos);
}

//=============================================================================
// エミッタクラスに値を設定する処理
//=============================================================================
void CBaseObject::SetValueToEmitter(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (m_pEmitter == NULL)return;
	m_pEmitter->SetPos(pos);
	m_pEmitter->SetRot(rot);

	if (m_pBill == NULL)return;
	m_pBill->SetPos(pos);
}

//=============================================================================
// 配置物クラスへのポインタを取得する
//=============================================================================
CObject *CBaseObject::GetObj(void)
{
	return m_pObject;
}

//=============================================================================
// ビルボードオブジェクトクラスへのポインタを取得する
//=============================================================================
CBillboardObject *CBaseObject::GetBillboardObj(void)
{
	return m_pBillboardObj;
}

//=============================================================================
// エミッタクラスへのポインタを取得する
//=============================================================================
CEmitter *CBaseObject::GetEmitter(void)
{
	return m_pEmitter;
}