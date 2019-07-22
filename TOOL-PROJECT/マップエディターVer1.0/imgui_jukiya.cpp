//=============================================================================
//
// ImGuiの処理 [imgui_jukiya.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "imgui_jukiya.h"
#include "imconfig.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"
#include "imstb_rectpack.h"
#include "imstb_textedit.h"
#include "imstb_truetype.h"

#include "block.h"
#include "boxCollider.h"
#include "editor.h"
#include "input.h"
#include "map.h"
#include "modelcreate.h"
#include "manager.h"
#include "camera.h"
#include "cameraManager.h"
#include "light.h"
#include "lightManager.h"
#include "textureManager.h"
#include "fade.h"
#include "meshfield.h"
#include "headquarters.h"
#include "respawn.h"
#include "fileLoader.h"
#include "sky.h"
#include "effectManager.h"
#include "emitter.h"
#include "billboardObject.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define IMGUI_SAVE_DISP   (60)
#define IMGUI_LOAD_DISP   (60)
#define IMGUI_CREATE_DISP (60)

//=============================================================================
// ImGuiのコンストラクタ
//=============================================================================
CImGui_Jukiya::CImGui_Jukiya()
{

}

//=============================================================================
// ImGuiのデストラクタ
//=============================================================================
CImGui_Jukiya::~CImGui_Jukiya()
{
}

//=============================================================================
// ImGuiの初期化処理
//=============================================================================
void CImGui_Jukiya::Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice)
{
	// ImGui用のテキストデータを生成する
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//ImFontConfig config;
	//config.MergeMode = true;
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:/Windows/Fonts/meiryo.ttc", 18.0f, &config, io.Fonts->GetGlyphRangesJapanese());

	// AddFontFromFileTTF関数
	// 第１引数 : フォントデータのファイルパス名
	// 第２引数 : フォントのサイズ
	// 第３引数 : NULLでOK
	// 第４引数 : 日本語にしたければio.Fonts->GetGlyphRangesJapaneseで基本的にOK
	io.Fonts->AddFontFromFileTTF("c:/Windows/Fonts/meiryo.ttc", 20.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// ImGuiを生成する
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pDevice);
	ImGui::StyleColorsClassic();

	// 各種値の初期化を行う
	ClearVariable();
}

//=============================================================================
// ImGuiの終了処理
//=============================================================================
void CImGui_Jukiya::Uninit(void)
{
	// ImGuiを終了させる
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//=============================================================================
// ImGuiの更新処理
//=============================================================================
void CImGui_Jukiya::Update(void)
{
	// 新しいフレームを用意する
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// デバッグ良いウィンドウ生成
	WindowDebug();

	// ウィンドウ生成開始
	ImGui::Begin(u8"マップエディター Ver1.0", false);

	// 現在のモード設定
	if (CFade::GetFade() == CFade::FADE_NONE)
	{// フェードが使用されていない
		int nMode = CManager::GetMode();
		int nModeOld = nMode;
		ImGui::RadioButton(u8"エディター", &nMode, CManager::MODE_EDITOR); ImGui::SameLine();
		ImGui::RadioButton(u8"デモプレイ", &nMode, CManager::MODE_DEMOPLAY);

		if (nModeOld != nMode)
		{// モードが更新された
			ChangeModeDebug(nMode);
		}

		// 読み込むマップのファイル名を設定
		LoadFileNameDebug();
		ImGui::Text("");

		// 読み込み成否情報表示
		m_nLoadModeCounter = LoadDisp(m_nLoadModeCounter, m_bLoadModeDisp);
	}
	else
	{// フェードが使用されている
		ImGui::Text(u8"モード切替中");
	}

	if (CManager::GetMode() == CManager::MODE_EDITOR)
	{// エディターモードだったら
		EditerDebug();
	}
	else if (CManager::GetMode() == CManager::MODE_DEMOPLAY)
	{// デモプレイモードだったら
		DemoplayDebug();
	}

	// フレームを終了させる
	ImGui::EndFrame();
}

//=============================================================================
// ImGuiの描画処理
//=============================================================================
void CImGui_Jukiya::Draw(void)
{
	// Updateで生成したImGuiを表示する
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

//=============================================================================
// ImGuiの読み込むマップのファイル名情報描画処理
//=============================================================================
void CImGui_Jukiya::LoadFileNameDebug(void)
{
	char m_aFileName[256] = "\0";
	strcpy(m_aFileName, CManager::GetLoadMapFileName());

	if (CManager::GetMode() == CManager::MODE_EDITOR)
	{
		ImGui::InputText(u8"デモプレイが読み込むマップ", m_aFileName, IM_ARRAYSIZE(m_aFileName));
	}
	else if (CManager::GetMode() == CManager::MODE_DEMOPLAY)
	{
		ImGui::InputText(u8"エディターが読み込むマップ", m_aFileName, IM_ARRAYSIZE(m_aFileName));
	}

	CManager::SetLoadMapFileName(m_aFileName);
}

//=============================================================================
// ImGuiのモード切替情報描画処理
//=============================================================================
void CImGui_Jukiya::ChangeModeDebug(int nMode)
{
	char m_aFileName[256] = "\0";
	strcpy(m_aFileName, CManager::GetLoadMapFileName());

	// カウンターセット
	m_nLoadModeCounter = IMGUI_LOAD_DISP;
	m_bLoadModeDisp = true;

	// マップを読み込めるかチェック
	CFileLoader *pFileLoader = CFileLoader::Create(m_aFileName);
	if (pFileLoader == NULL)
	{
		m_bLoadModeDisp = false;
		return;
	}

	// 読み込めるならばメモリ開放してモード切替
	if (pFileLoader != NULL)
	{
		delete pFileLoader;
		pFileLoader = NULL;
	}
	CFade::SetFade((CManager::MODE)nMode, CFade::FADE_OUT);
}

//=============================================================================
// ImGuiのデバッグ情報描画処理
//=============================================================================
void CImGui_Jukiya::WindowDebug(void)
{
	//カメラの取得
	CCamera *pCamera = NULL;
	if (CManager::GetBaseMode()->GetCameraManager() != NULL)
	{
		if (CManager::GetMode() == CManager::MODE_EDITOR)
		{
			pCamera = CManager::GetBaseMode()->GetCameraManager()->GetCamera(CManager::GetEditor()->GetCameraMode());
		}
		else if (CManager::GetMode() == CManager::MODE_DEMOPLAY)
		{
			pCamera = CManager::GetBaseMode()->GetCameraManager()->GetCamera();
		}
	}

	// マウスの取得
	CInputMouse *pMouse = CManager::GetMouse();

	// ウィンドウ生成開始
	ImGui::Begin(u8"デバッグ情報");

	// FPS描画
	ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);

	// カメラのデバッグ設定
	ImGui::Text("");
	ImGui::CollapsingHeader(u8"カメラ情報");
	if (pCamera != NULL)
	{
		bool bHoverWindow = ImGui::IsMouseHoveringAnyWindow();
		if (bHoverWindow == true)
		{
			pCamera->SetChengeLength(false);
			bool bDrag = ImGui::IsMouseDragging();
			if (bDrag == true)
			{// ImGuiのウィンドウを動かしている
				pCamera->SetMouseMove(false);
			}
			else
			{// ImGuiのウィンドウを動かしていない
				pCamera->SetMouseMove(true);
			}
		}
		else
		{
			pCamera->SetChengeLength(true);
		}

		// カメラの視点の表示
		float aCameraPosV[3] = { pCamera->GetPosV().x,pCamera->GetPosV().y,pCamera->GetPosV().z };
		ImGui::Text(u8"視点 %.2f %.2f %.2f", aCameraPosV[0], aCameraPosV[1], aCameraPosV[2]);

		// カメラの注視点の表示
		float aCameraPosR[3] = { pCamera->GetPosR().x,pCamera->GetPosR().y,pCamera->GetPosR().z };
		ImGui::Text(u8"注視点 %.2f %.2f %.2f", aCameraPosR[0], aCameraPosR[1], aCameraPosR[2]);

		// カメラの向き
		float aCameraRot[3] = { pCamera->GetRot().x,pCamera->GetRot().y,pCamera->GetRot().z };
		ImGui::Text(u8"向き %.3f %.3f %.3f", aCameraRot[0], aCameraRot[1], aCameraRot[2]);

		// 視点注視点間の距離
		float aCameraLength = pCamera->GetLength();
		ImGui::Text(u8"距離 %.1f", aCameraLength);
	}


	// マウスのデバッグ設定
	ImGui::Text("");
	ImGui::CollapsingHeader(u8"マウス情報");
	if (pMouse != NULL)
	{
		// マウスのデバッグ情報表示
		int aMouseScreenPos[2] = { pMouse->GetPoint().x,pMouse->GetPoint().y };
		float aMousePosZn[3] = { pMouse->GetWorldPosZn().x,pMouse->GetWorldPosZn().y,pMouse->GetWorldPosZn().z };
		float aMousePosZf[3] = { pMouse->GetWorldPosZf().x,pMouse->GetWorldPosZf().y,pMouse->GetWorldPosZf().z };
		ImGui::Text(u8"スクリーン座標                        %d %d", aMouseScreenPos[0], aMouseScreenPos[1]);
		ImGui::Text(u8"ワールド座標(カメラから近い方) %.1f %.1f %.1f", aMousePosZn[0], aMousePosZn[1], aMousePosZn[2]);
		ImGui::Text(u8"ワールド座標(カメラから遠い方) %.1f %.1f %.1f", aMousePosZf[0], aMousePosZf[1], aMousePosZf[2]);
	}

	// ウィンドウ生成終了
	ImGui::End();

}

//=============================================================================
// ImGuiのエディター描画処理
//=============================================================================
void CImGui_Jukiya::EditerDebug(void)
{
	// エディターの取得
	CEditor *pEditor = CManager::GetEditor();
	if (pEditor != NULL)
	{
		// オブジェクトを生成するか破棄するかを設定する
		ImGui::Text("");
		ImGui::CollapsingHeader(u8"エディター設定");

		//マップのポインタ情報取得
		CMap *pMap = pEditor->GetMap();

		// 生成モードか破棄モードかを表示
		if (ImGui::TreeNode(u8"モード"))
		{
			int nEditerMode = pEditor->GetMode();
			ImGui::RadioButton(u8"生成", &nEditerMode, CEditor::MODE_CREATE); ImGui::SameLine();
			ImGui::RadioButton(u8"破棄", &nEditerMode, CEditor::MODE_DESTROY);
			pEditor->SetMode((CEditor::MODE)nEditerMode);
			ImGui::TreePop();
		}

		// 編集モードを設定
		if (ImGui::TreeNode(u8"編集モード"))
		{
			int nEditMode = pEditor->GetEditMode();
			int nEditModeDef = nEditMode;
			ImGui::RadioButton(u8"ゲームフィールド", &nEditMode, CEditor::EDITMODE_GAMEFIELD); ImGui::SameLine();
			ImGui::RadioButton(u8"ライト", &nEditMode, CEditor::EDITMODE_LIGHT); ImGui::SameLine();
			ImGui::RadioButton(u8"配置物", &nEditMode, CEditor::EDITMODE_OBJECT);
			ImGui::RadioButton(u8"リスポーン", &nEditMode, CEditor::EDITMODE_RESPAWN); ImGui::SameLine();
			ImGui::RadioButton(u8"司令部", &nEditMode, CEditor::EDITMODE_HEADQUARTERS); ImGui::SameLine();
			ImGui::RadioButton(u8"敵の生成情報", &nEditMode, CEditor::EDITMODE_ENEMYLIST);
			ImGui::RadioButton(u8"空", &nEditMode, CEditor::EDITMODE_SKY);
			pEditor->SetEditMode((CEditor::EDITMODE)nEditMode);
			ImGui::TreePop();

			// ベースオブジェクトを破棄するかどうかチェック
			if (nEditModeDef != CEditor::EDITMODE_OBJECT && nEditMode == CEditor::EDITMODE_OBJECT)
			{
				pEditor->CreateBaseObject();
				pEditor->SwitchBaseObject(pEditor->GetObjectMode());
			}
			else if(nEditMode != CEditor::EDITMODE_OBJECT)
			{
				pEditor->ReleaseBaseObject();
			}
		}
		ImGui::Text("");
		ImGui::Text("");

		// カメラ情報表示
		CameraDebug(pEditor);

		// 地面情報表示
		FieldDebug(pEditor);

		// マップ情報表示
		MapDebug(pEditor);
	}

	// ウィンドウ生成終了
	ImGui::End();

	// 編集モードごとのウィンドウを生成
	if (pEditor->GetEditMode() == CEditor::EDITMODE_GAMEFIELD)
	{// ゲームフィールド情報表示
		GameFieldDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_LIGHT)
	{// ライト情報表示
		LightDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_OBJECT)
	{// 配置物情報表示
		ObjectDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_RESPAWN)
	{// リスポーン位置情報表示
		RespawnDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_HEADQUARTERS)
	{// 司令部情報表示
		HeadQuartersDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_ENEMYLIST)
	{// 敵の生成情報表示
		EnemyListDebug(pEditor);
	}
	else if (pEditor->GetEditMode() == CEditor::EDITMODE_SKY)
	{// 空情報表示
		SkyDebug(pEditor);
	}
}

//=============================================================================
// ImGuiのカメラ情報描画処理
//=============================================================================
void CImGui_Jukiya::CameraDebug(CEditor *pEditor)
{
	// ヘッダー生成開始
	ImGui::CollapsingHeader(u8"カメラ設定");

	//カメラの取得
	CCamera *pCamera = NULL;
	if (CManager::GetBaseMode()->GetCameraManager() != NULL)
	{
		pCamera = CManager::GetBaseMode()->GetCameraManager()->GetCamera(CManager::GetEditor()->GetCameraMode());
	}

	// カメラモードを設定
	int nCameraMode = pEditor->GetCameraMode();
	if (ImGui::TreeNode(u8"カメラモード"))
	{
		ImGui::RadioButton(u8"ゲーム", &nCameraMode, CEditor::CAMERAMODE_GAME); ImGui::SameLine();
		ImGui::RadioButton(u8"エディター", &nCameraMode, CEditor::CAMERAMODE_EDIT); ImGui::SameLine();
		ImGui::RadioButton(u8"トップビュー", &nCameraMode, CEditor::CAMERAMODE_TOP);
		pEditor->SetCameraMode((CEditor::CAMERAMODE)nCameraMode);
		ImGui::TreePop();
	}

	if (pCamera != NULL)
	{
		// カメラ情報を描画する
		if (nCameraMode == CEditor::CAMERAMODE_GAME)
		{// ゲームカメラだったら
			GameCameraDebug(pCamera);
		}
		else if (nCameraMode == CEditor::CEditor::CAMERAMODE_EDIT)
		{// エディットカメラだったら
			EditorCameraDebug(pCamera);
		}
		else if (nCameraMode == CEditor::CEditor::CAMERAMODE_TOP)
		{// エディットカメラだったら
			TopViewCameraDebug(pCamera);
		}
	}
	ImGui::Text("");
}

//=============================================================================
// ImGuiのゲームカメラ情報描画処理
//=============================================================================
void CImGui_Jukiya::GameCameraDebug(CCamera *pCamera)
{

}

//=============================================================================
// ImGuiのエディットカメラ情報描画処理
//=============================================================================
void CImGui_Jukiya::EditorCameraDebug(CCamera *pCamera)
{
	// ボタンが押されたらカメラ情報を初期化する
	if (ImGui::Button(u8"カメラリセット"))
	{
		pCamera->Reset();
	}
}

//=============================================================================
// ImGuiのトップビューカメラ情報描画処理
//=============================================================================
void CImGui_Jukiya::TopViewCameraDebug(CCamera *pCamera)
{

}

//=============================================================================
// ImGuiの地面情報描画処理
//=============================================================================
void CImGui_Jukiya::FieldDebug(CEditor *pEditor)
{
	// ヘッダー生成開始
	ImGui::CollapsingHeader(u8"フィールド情報");

	// 地面に張り付けるテクスチャ番号を設定
	int nNumTex = pEditor->GetMap()->GetTextureManager()->GetNumTexture();
	int nFieldTexIdx = pEditor->GetMap()->GetFieldTexIdx();
	ImGui::SliderInt(u8"テクスチャの番号", &nFieldTexIdx, 0, nNumTex - 1);
	pEditor->GetMap()->SetFieldTexIdx(nFieldTexIdx);

	// 地面にテクスチャを設定
	CMeshField *pMeshField = pEditor->GetMap()->GetMeshField();
	pMeshField->BindTexture(pEditor->GetMap()->GetTextureManager()->GetTexture(nFieldTexIdx));

	// グリッド線を描画するかどうか設定
	bool bGridDisp = pEditor->GetGridDisp();
	ImGui::Checkbox(u8"グリッド線描画", &bGridDisp);
	pEditor->SetGridDisp(bGridDisp);

	// 当たり判定の線を描画するかどうか設定
	bool bColRangeDisp = pEditor->GetColRangeDisp();
	ImGui::Checkbox(u8"当たり判定の線描画", &bColRangeDisp);
	pEditor->SetColRangeDisp(bColRangeDisp);

	// 可読性のため改行
	ImGui::Text("");
}

//=============================================================================
// ImGuiの保存成否情報情報描画処理
//=============================================================================
int CImGui_Jukiya::SaveDisp(int nCounter, bool bDisp)
{
	nCounter--;
	if (nCounter <= 0)
	{
		nCounter = 0;
	}
	else
	{
		ImGui::SameLine();
		if (bDisp == true)
		{
			ImGui::Text(u8"保存が完了しました");
		}
		else
		{
			ImGui::Text(u8"保存に失敗しました");
		}
	}

	return nCounter;
}

//=============================================================================
// ImGuiの読み込み成否情報描画処理
//=============================================================================
int CImGui_Jukiya::LoadDisp(int nCounter, bool bDisp)
{
	nCounter--;
	if (nCounter <= 0)
	{
		nCounter = 0;
	}
	else
	{
		ImGui::SameLine();
		if (bDisp == true)
		{
			ImGui::Text(u8"読み込みが完了しました");
		}
		else
		{
			ImGui::Text(u8"読み込みに失敗しました");
		}
	}

	return nCounter;
}

//=============================================================================
// ImGuiのマップ情報描画処理
//=============================================================================
void CImGui_Jukiya::MapDebug(CEditor *pEditor)
{
	// ヘッダー生成
	ImGui::CollapsingHeader(u8"マップ情報");

	// 読み込むマップのファイル名を設定
	LoadMapDebug(pEditor, pEditor->GetMap());

	// 読み込み成否情報表示
	m_nLoadMapCounter = LoadDisp(m_nLoadMapCounter, m_bLoadMapDisp);
	ImGui::Text("");

	// 保存するマップのファイル名を設定
	SaveMapDebug(pEditor, pEditor->GetMap());

	// 保存成否情報表示
	m_nSaveMapCounter = SaveDisp(m_nSaveMapCounter, m_bSaveMapDisp);
}

//=============================================================================
// ImGuiのマップ保存情報描画処理
//=============================================================================
void CImGui_Jukiya::SaveMapDebug(CEditor *pEditor, CMap *pMap)
{
	char aSaveFileName[256];
	char aSaveModelFileName[256];
	char aSaveTexFileName[256];
	char aSaveLightFileName[256];
	char aSaveGameFieldFileName[256];
	char aSaveObjectFileName[256];
	char aSaveEnemyFileName[256];
	strcpy(aSaveFileName, pEditor->GetMapSaveFileName());
	strcpy(aSaveModelFileName, pEditor->GetModelListSaveFileName());
	strcpy(aSaveTexFileName, pEditor->GetTexListSaveFileName());
	strcpy(aSaveLightFileName, pEditor->GetLightSaveFileName());
	strcpy(aSaveGameFieldFileName, pEditor->GetGameFieldSaveFileName());
	strcpy(aSaveObjectFileName, pEditor->GetObjectSaveFileName());
	strcpy(aSaveEnemyFileName, pEditor->GetEnemyListSaveFileName());

	// 保存するファイル名設定
	ImGui::InputText(u8"保存するファイル名", aSaveFileName, IM_ARRAYSIZE(aSaveFileName));
	ImGui::InputText(u8"モデル情報保存先", aSaveModelFileName, IM_ARRAYSIZE(aSaveModelFileName));
	ImGui::InputText(u8"テクスチャリスト情報保存先", aSaveTexFileName, IM_ARRAYSIZE(aSaveTexFileName));
	ImGui::InputText(u8"ライト情報保存先", aSaveLightFileName, IM_ARRAYSIZE(aSaveLightFileName));
	ImGui::InputText(u8"ゲームフィールド情報保存先", aSaveGameFieldFileName, IM_ARRAYSIZE(aSaveGameFieldFileName));
	ImGui::InputText(u8"配置物情報保存先", aSaveObjectFileName, IM_ARRAYSIZE(aSaveObjectFileName));
	ImGui::InputText(u8"敵の生成情報保存先", aSaveEnemyFileName, IM_ARRAYSIZE(aSaveEnemyFileName));
	pEditor->SetMapSaveFileName(aSaveFileName);
	pEditor->SetModelListSaveFileName(aSaveModelFileName);
	pEditor->SetTexListSaveFileName(aSaveTexFileName);
	pEditor->SetLightSaveFileName(aSaveLightFileName);
	pEditor->SetGameFieldSaveFileName(aSaveGameFieldFileName);
	pEditor->SetObjectSaveFileName(aSaveObjectFileName);
	pEditor->SetEnemyListSaveFileName(aSaveEnemyFileName);

	if (ImGui::Button(u8"保存する"))
	{// ボタン押下
	    // マップ情報保存
		pMap->SetModelListFileName(pEditor->GetModelListSaveFileName());
		pMap->SetTexListFileName(pEditor->GetTexListSaveFileName());
		pMap->SetLightFileName(pEditor->GetLightSaveFileName());
		pMap->SetGameFieldFileName(pEditor->GetGameFieldSaveFileName());
		pMap->SetObjectFileName(pEditor->GetObjectSaveFileName());
		pMap->SetEnemyListFileName(pEditor->GetEnemyListSaveFileName());
		if (FAILED(pMap->Save(aSaveFileName)))
		{
			m_bSaveMapDisp = false;
		}
		else
		{
			m_bSaveMapDisp = true;
		}
		m_nSaveMapCounter = IMGUI_SAVE_DISP;
	}
}

//=============================================================================
// ImGuiのマップ読み込み情報描画処理
//=============================================================================
void CImGui_Jukiya::LoadMapDebug(CEditor *pEditor, CMap *pMap)
{
	char aLoadFileName[256];
	strcpy(aLoadFileName, pEditor->GetMapLoadFileName());

	// 読み込むファイル名を設定
	ImGui::InputText(u8"読み込むファイル名", aLoadFileName, IM_ARRAYSIZE(aLoadFileName));
	pEditor->SetMapLoadFileName(aLoadFileName);

	if (ImGui::Button(u8"読み込む"))
	{// ボタン押下
		strcpy(aLoadFileName, SAVEFILENAME_MAP);
		if (FAILED(pMap->ChangeMap(strcat(aLoadFileName, pEditor->GetMapLoadFileName()))))
		{
			m_bLoadMapDisp = false;
		}
		else
		{
			m_bLoadMapDisp = true;
			pEditor->SetRespawnModel();
			pEditor->SetMeshField(pMap->GetMeshField());
			pEditor->SetEditBlock(NULL);
		}
		m_nLoadMapCounter = IMGUI_LOAD_DISP;
	}
}

//=============================================================================
// ImGuiのライト情報描画処理
//=============================================================================
void CImGui_Jukiya::LightDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// マップの取得
		CMap *pMap = pEditor->GetMap();

		// ライト管轄クラスを取得
		CLightManager *pLightManager = pMap->GetLightManager();

		// ウィンドウ生成開始
		ImGui::Begin(u8"ライト情報");

		// ライトの増減情報表示
		pMap->SetLightManager(LightIncreDecreDebug(pLightManager));
		ImGui::Text("");

		// ライト情報表示
		pLightManager = pMap->GetLightManager();
		LightInfoDebug(pLightManager);

		// マップの設定
		pEditor->SetMap(pMap);

		// ウィンドウ生成終了
		ImGui::End();
	}
}

//=============================================================================
// ImGuiのライト増減情報描画処理
//=============================================================================
CLightManager *CImGui_Jukiya::LightIncreDecreDebug(CLightManager *pLightManager)
{
	CLightManager *pLightManagerNew = pLightManager;
	if (pLightManager != NULL)
	{
		// ライトの数を取得
		int nNumLight = pLightManager->GetNumLight();
		int nNumLightDef = nNumLight;

		// ライトの数を増減させる
		ImGui::InputInt(u8"ライトの数", &nNumLight);

		// 増減判定
		if (nNumLight != nNumLightDef && nNumLight > 0)
		{// デフォルトの値から変更された
		    // ライト情報をコピーして渡す
			pLightManagerNew = CLightManager::Cpy(pLightManager, nNumLight);
			m_nLightIdx = 0;
		}
	}

	return pLightManagerNew;
}

//=============================================================================
// ImGuiのライト情報描画処理
//=============================================================================
void CImGui_Jukiya::LightInfoDebug(CLightManager *pLightManager)
{
	// ライト管轄クラスを取得
	if (pLightManager != NULL)
	{
		// ヘッダー生成開始
		ImGui::CollapsingHeader(u8"ライト列挙");

		// ライトの数を取得
		int nNumLight = pLightManager->GetNumLight();
		if (m_nLightIdx > nNumLight)
		{
			m_nLightIdx = 0;
		}

		// ライト情報を表示
		ImGui::SliderInt(u8"表示する番号", &m_nLightIdx, 0, nNumLight - 1);
		LightInfo(pLightManager, m_nLightIdx);
	}
}

//=============================================================================
// ImGuiのライト情報表示処理
//=============================================================================
void CImGui_Jukiya::LightInfo(CLightManager *pLightManager, int nCntLight)
{
	ImGui::Text(u8"ライト%d   ", nCntLight);
	// ライト情報を取得
	CLight *pLight = pLightManager->GetLight(nCntLight);

	if (pLight != NULL)
	{
		// ライトの種類を取得
		int nLightType = pLight->GetType();
		int nLightTypeDef = nLightType;
		ImGui::SameLine(); ImGui::RadioButton(u8"ディレクショナル", &nLightType, CLight::TYPE_DIRECTIONAL);
		ImGui::SameLine(); ImGui::RadioButton(u8"ポイント", &nLightType, CLight::TYPE_POINT);
		ImGui::SameLine(); ImGui::RadioButton(u8"スポット", &nLightType, CLight::TYPE_SPOT);

		// ライトの種類が変わっていないか判定
		pLightManager = ChangeLight(pLightManager, nCntLight, pLight, nLightType, nLightTypeDef);
		pLight = pLightManager->GetLight(nCntLight);

		// 現在のライト情報を取得
		if (nLightType == CLight::TYPE_DIRECTIONAL)
		{// ディレクショナルライトなら
			DirectionalLightInfoDebug((CDirectionalLight*)pLight);
		}
		else if (nLightType == CLight::TYPE_POINT)
		{// ポイントライトなら
			PointLightInfoDebug((CPointLight*)pLight);
		}
		else if (nLightType == CLight::TYPE_SPOT)
		{// スポットライトなら
			SpotLightInfoDebug((CSpotLight*)pLight);
		}
		pLightManager->SettingLight(pLight, nCntLight);
		ImGui::Separator();
	}
}

//=============================================================================
// ImGuiのライト種類変更処理
//=============================================================================
CLightManager *CImGui_Jukiya::ChangeLight(CLightManager *pLightManager, int nCntLight, CLight *pLightOld, int nLightType, int nLightTypeDef)
{
	CLight *pLight = pLightOld;
	if (nLightType != nLightTypeDef)
	{// 変わっている
		CLight *pLightDef = pLightOld;
		pLight = CLight::ChangeLightType((CLight::TYPE)nLightType);

		pLightManager->SettingLight(pLight, nCntLight);

		pLightDef->Uninit();
		delete pLightDef;
		pLightDef = NULL;
	}

	return pLightManager;
}

//=============================================================================
// ImGuiのディレクショナルライト情報描画処理
//=============================================================================
void CImGui_Jukiya::DirectionalLightInfoDebug(CDirectionalLight *pDirLight)
{
	if (pDirLight != NULL)
	{
		// 各種情報を取得
		float fLightDiffuse[4] = { pDirLight->GetLight().Diffuse.r,pDirLight->GetLight().Diffuse.g,pDirLight->GetLight().Diffuse.b ,pDirLight->GetLight().Diffuse.a };
		float fLightAmbient[4] = { pDirLight->GetLight().Ambient.r,pDirLight->GetLight().Ambient.g,pDirLight->GetLight().Ambient.b ,pDirLight->GetLight().Ambient.a };
		float fLightSpecular[4] = { pDirLight->GetLight().Specular.r,pDirLight->GetLight().Specular.g,pDirLight->GetLight().Specular.b ,pDirLight->GetLight().Specular.a };
		float fLightDir[3] = { pDirLight->GetLight().Direction.x,pDirLight->GetLight().Direction.y,pDirLight->GetLight().Direction.z };

		// 各種情報を編集
		ImGui::ColorEdit3(u8"拡散光", &fLightDiffuse[0]);
		ImGui::ColorEdit3(u8"環境光", &fLightAmbient[0]);
		ImGui::ColorEdit3(u8"反射光", &fLightSpecular[0]);
		ImGui::InputFloat3(u8"向き", &fLightDir[0]);

		// 各種情報を設定
		D3DLIGHT9 Light;
		ZeroMemory(&Light, sizeof(D3DLIGHT9));
		Light.Type = D3DLIGHT_DIRECTIONAL;
		Light.Diffuse = D3DXCOLOR(fLightDiffuse[0], fLightDiffuse[1], fLightDiffuse[2], fLightDiffuse[3]);
		Light.Ambient = D3DXCOLOR(fLightAmbient[0], fLightAmbient[1], fLightAmbient[2], fLightAmbient[3]);
		Light.Specular = D3DXCOLOR(fLightSpecular[0], fLightSpecular[1], fLightSpecular[2], fLightSpecular[3]);
		Light.Direction = D3DXVECTOR3(fLightDir[0], fLightDir[1], fLightDir[2]);
		pDirLight->SetLight(Light);
	}
}

//=============================================================================
// ImGuiのポイントライト情報描画処理
//=============================================================================
void CImGui_Jukiya::PointLightInfoDebug(CPointLight *pPointLight)
{
	if (pPointLight != NULL)
	{
		// 各種情報を取得
		float fLightPos[3] = { pPointLight->GetLight().Position.x,pPointLight->GetLight().Position.y,pPointLight->GetLight().Position.z };
		float fLightDiffuse[4] = { pPointLight->GetLight().Diffuse.r,pPointLight->GetLight().Diffuse.g,pPointLight->GetLight().Diffuse.b ,pPointLight->GetLight().Diffuse.a };
		float fLightAmbient[4] = { pPointLight->GetLight().Ambient.r,pPointLight->GetLight().Ambient.g,pPointLight->GetLight().Ambient.b ,pPointLight->GetLight().Ambient.a };
		float fLightSpecular[4] = { pPointLight->GetLight().Specular.r,pPointLight->GetLight().Specular.g,pPointLight->GetLight().Specular.b ,pPointLight->GetLight().Specular.a };
		float fLightAttenuation0 = pPointLight->GetLight().Attenuation0;
		float fLightAttenuation1 = pPointLight->GetLight().Attenuation1;
		float fLightAttenuation2 = pPointLight->GetLight().Attenuation2;
		float fLightRange = pPointLight->GetLight().Range;

		// 各種情報を編集
		ImGui::InputFloat3(u8"座標", &fLightPos[0]);
		ImGui::ColorEdit3(u8"拡散光", &fLightDiffuse[0]);
		ImGui::ColorEdit3(u8"環境光", &fLightAmbient[0]);
		ImGui::ColorEdit3(u8"反射光", &fLightSpecular[0]);
		ImGui::InputFloat(u8"線形減衰係数", &fLightAttenuation0);
		ImGui::InputFloat(u8"２次減衰係数", &fLightAttenuation1);
		ImGui::InputFloat(u8"指数減衰係数", &fLightAttenuation2);
		ImGui::InputFloat(u8"範囲", &fLightRange);

		// 各種情報を設定
		D3DLIGHT9 Light;
		ZeroMemory(&Light, sizeof(D3DLIGHT9));
		Light.Type = D3DLIGHT_POINT;
		Light.Position = D3DXVECTOR3(fLightPos[0], fLightPos[1], fLightPos[2]);
		Light.Diffuse = D3DXCOLOR(fLightDiffuse[0], fLightDiffuse[1], fLightDiffuse[2], fLightDiffuse[3]);
		Light.Ambient = D3DXCOLOR(fLightAmbient[0], fLightAmbient[1], fLightAmbient[2], fLightAmbient[3]);
		Light.Specular = D3DXCOLOR(fLightSpecular[0], fLightSpecular[1], fLightSpecular[2], fLightSpecular[3]);
		Light.Attenuation0 = fLightAttenuation0;
		Light.Attenuation1 = fLightAttenuation1;
		Light.Attenuation2 = fLightAttenuation2;
		Light.Range = fLightRange;
		pPointLight->SetLight(Light);
	}
}

//=============================================================================
// ImGuiのスポットライト情報描画処理
//=============================================================================
void CImGui_Jukiya::SpotLightInfoDebug(CSpotLight *pSpotLight)
{
	if (pSpotLight != NULL)
	{	// 各種情報を取得
		float fLightPos[3] = { pSpotLight->GetLight().Position.x,pSpotLight->GetLight().Position.y,pSpotLight->GetLight().Position.z };
		float fLightDir[3] = { pSpotLight->GetLight().Direction.x,pSpotLight->GetLight().Direction.y,pSpotLight->GetLight().Direction.z };
		float fLightDiffuse[4] = { pSpotLight->GetLight().Diffuse.r,pSpotLight->GetLight().Diffuse.g,pSpotLight->GetLight().Diffuse.b ,pSpotLight->GetLight().Diffuse.a };
		float fLightAmbient[4] = { pSpotLight->GetLight().Ambient.r,pSpotLight->GetLight().Ambient.g,pSpotLight->GetLight().Ambient.b ,pSpotLight->GetLight().Ambient.a };
		float fLightSpecular[4] = { pSpotLight->GetLight().Specular.r,pSpotLight->GetLight().Specular.g,pSpotLight->GetLight().Specular.b ,pSpotLight->GetLight().Specular.a };
		float fLightAttenuation0 = pSpotLight->GetLight().Attenuation0;
		float fLightAttenuation1 = pSpotLight->GetLight().Attenuation1;
		float fLightAttenuation2 = pSpotLight->GetLight().Attenuation2;
		float fLightRange = pSpotLight->GetLight().Range;
		float fLightFallOff = pSpotLight->GetLight().Falloff;
		float fLightTheta = pSpotLight->GetLight().Theta;
		float fLightPhi = pSpotLight->GetLight().Phi;

		// 各種情報を編集
		ImGui::InputFloat3(u8"座標", &fLightPos[0]);
		ImGui::InputFloat3(u8"向き", &fLightDir[0]);
		ImGui::ColorEdit3(u8"拡散光", &fLightDiffuse[0]);
		ImGui::ColorEdit3(u8"環境光", &fLightAmbient[0]);
		ImGui::ColorEdit3(u8"反射光", &fLightSpecular[0]);
		ImGui::InputFloat(u8"線形減衰係数", &fLightAttenuation0);
		ImGui::InputFloat(u8"２次減衰係数", &fLightAttenuation1);
		ImGui::InputFloat(u8"指数減衰係数", &fLightAttenuation2);
		ImGui::InputFloat(u8"範囲", &fLightRange);
		ImGui::InputFloat(u8"フォールオフ", &fLightFallOff);
		ImGui::InputFloat(u8"内部コーンの角度", &fLightTheta);
		ImGui::InputFloat(u8"外部コーンの角度", &fLightPhi);

		// 各種情報を設定
		D3DLIGHT9 Light;
		ZeroMemory(&Light, sizeof(D3DLIGHT9));
		Light.Type = D3DLIGHT_POINT;
		Light.Position = D3DXVECTOR3(fLightPos[0], fLightPos[1], fLightPos[2]);
		Light.Direction = D3DXVECTOR3(fLightDir[0], fLightDir[1], fLightDir[2]);
		Light.Diffuse = D3DXCOLOR(fLightDiffuse[0], fLightDiffuse[1], fLightDiffuse[2], fLightDiffuse[3]);
		Light.Ambient = D3DXCOLOR(fLightAmbient[0], fLightAmbient[1], fLightAmbient[2], fLightAmbient[3]);
		Light.Specular = D3DXCOLOR(fLightSpecular[0], fLightSpecular[1], fLightSpecular[2], fLightSpecular[3]);
		Light.Attenuation0 = fLightAttenuation0;
		Light.Attenuation1 = fLightAttenuation1;
		Light.Attenuation2 = fLightAttenuation2;
		Light.Range = fLightRange;
		Light.Falloff = fLightFallOff;
		Light.Theta = fLightTheta;
		Light.Phi = fLightPhi;
		pSpotLight->SetLight(Light);
	}
}

//=============================================================================
// ImGuiのゲームフィールド情報描画処理
//=============================================================================
void CImGui_Jukiya::GameFieldDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// ウィンドウ生成開始
		ImGui::Begin(u8"ゲームフィールド情報");

		// 配置するオブジェクトの種類を設定
		int nGameFieldMode = pEditor->GetGameFieldMode();
		int nGameFieldModeDef = nGameFieldMode;
		ImGui::RadioButton(u8"ブロック", &nGameFieldMode, CEditor::GAMEFIELDMODE_BLOCK); ImGui::SameLine();
		ImGui::RadioButton(u8"川", &nGameFieldMode, CEditor::GAMEFIELDMODE_RIVER); ImGui::SameLine();
		ImGui::RadioButton(u8"氷", &nGameFieldMode, CEditor::GAMEFIELDMODE_ICE);
		pEditor->SetGameFieldMode((CEditor::GAMEFIELDMODE)nGameFieldMode);
		ImGui::Text("");
		if (nGameFieldMode != nGameFieldModeDef)
		{
			pEditor->ExChangeArea();
		}

		// 現在のモードによって処理わけ
		if (nGameFieldMode == CEditor::GAMEFIELDMODE_BLOCK)
		{// ブロック情報表示
			BlockDebug(pEditor);
		}
		else if (nGameFieldMode == CEditor::GAMEFIELDMODE_RIVER)
		{// 川情報表示
			RiverDebug(pEditor);
		}
		else if (nGameFieldMode == CEditor::GAMEFIELDMODE_ICE)
		{// 氷情報表示
			IceDebug(pEditor);
		}

		// ウィンドウ生成終了
		ImGui::End();
	}
}

//=============================================================================
// ImGuiのブロック情報描画処理
//=============================================================================
void CImGui_Jukiya::BlockDebug(CEditor *pEditor)
{
	// マップクラスを取得
	CMap *pMap = pEditor->GetMap();

	// モデル管轄クラスを取得
	CModelCreate *pModelCreate = pMap->GetModelCreate();

	// ブロック情報取得
	int nBlockType = pEditor->GetBlockType();
	int nBlockModelIdx = pEditor->GetBlockModelIdx();
	float fBlockRot[3] = { pEditor->GetBlockRot().x,pEditor->GetBlockRot().y ,pEditor->GetBlockRot().z };
	float fBlockColWidth = pEditor->GetBlockColWidth();
	float fBlockColHeight = pEditor->GetBlockColHeight();
	float fBlockColDepth = pEditor->GetBlockColDepth();
	int nBlockTypeDef = nBlockType;
	int nBlockModelIdxDef = nBlockModelIdx;
	float fBlockColWidthDef = fBlockColWidth;
	float fBlockColHeightDef = fBlockColHeight;
	float fBlockColDepthDef = fBlockColDepth;

	// ヘッダー生成開始
	ImGui::CollapsingHeader(u8"ブロックの値設定");

	// ブロック情報を表示
	ImGui::InputInt(u8"種類", &nBlockType);
	ImGui::InputInt(u8"使用するモデルの番号", &nBlockModelIdx);
	ImGui::InputFloat3(u8"向き", &fBlockRot[0]);
	ImGui::InputFloat(u8"当たり判定の幅", &fBlockColWidth);
	ImGui::InputFloat(u8"当たり判定の高さ", &fBlockColHeight);
	ImGui::InputFloat(u8"当たり判定の奥行", &fBlockColDepth);
	ImGui::Text("");

	// ブロック情報判定
	if (nBlockType < 0)
	{// ブロック種類データが0を下回っている
		nBlockType = nBlockTypeDef;
	}
	if (nBlockModelIdx > pModelCreate->GetNumModel() - 1 || nBlockModelIdx < 0)
	{// 読み込んだモデルの総数を超えて入力されている
		nBlockModelIdx = nBlockModelIdxDef;
	}
	if (fBlockColWidth < 0.0f)
	{// 当たり判定幅情報が0を下回っている
		fBlockColWidth = fBlockColWidthDef;
	}
	if (fBlockColHeight < 0.0f)
	{// 当たり判定高さ情報が0を下回っている
		fBlockColHeight = fBlockColHeightDef;
	}
	if (fBlockColDepth < 0.0f)
	{// 当たり判定奥行情報が0を下回っている
		fBlockColDepth = fBlockColDepthDef;
	}

	// ブロック情報設定
	pEditor->SetBlockType(nBlockType);
	pEditor->SetBlockModelIdx(nBlockModelIdx);
	pEditor->SetBlockRot(D3DXVECTOR3(fBlockRot[0], fBlockRot[1], fBlockRot[2]));
	pEditor->SetBlockColWidth(fBlockColWidth);
	pEditor->SetBlockColHeight(fBlockColHeight);
	pEditor->SetBlockColDepth(fBlockColDepth);

	// 現在マウスがさしているエリアのブロック情報描画
	AreaBlockDebug(pEditor);
}

//=============================================================================
// ImGuiの現在マウスがさしているエリアのブロック情報描画処理
//=============================================================================
void CImGui_Jukiya::AreaBlockDebug(CEditor *pEditor)
{
	// 情報を表示するブロックを取得
	CBlock *pEditBlock = pEditor->GetEditBlock();
	if(pEditBlock != NULL)
	{
		// マップクラスを取得
		CMap *pMap = pEditor->GetMap();

		// モデル管轄クラスを取得
		CModelCreate *pModelCreate = pMap->GetModelCreate();

		// ブロック情報取得
		int nEditBlockType = pEditBlock->GetType();
		int nEditBlockModelIdx = pEditBlock->GetModelIdx();
		float fEditBlockRot[3] = { pEditBlock->GetRot().x,pEditBlock->GetRot().y ,pEditBlock->GetRot().z };
		float fEditBlockColWidth = pEditBlock->GetBoxCollider()->GetWidth();
		float fEditBlockColHeight = pEditBlock->GetBoxCollider()->GetHeight();
		float fEditBlockColDepth = pEditBlock->GetBoxCollider()->GetDepth();
		int nEditBlockTypeDef = nEditBlockType;
		int nEditBlockModelIdxDef = nEditBlockModelIdx;
		float fEditBlockColWidthDef = fEditBlockColWidth;
		float fEditBlockColHeightDef = fEditBlockColHeight;
		float fEditBlockColDepthDef = fEditBlockColDepth;
		int nEditBlockXBlock = 0;
		int nEditBlockZBlock = 0;
		D3DXVECTOR3 EditBlockPos = pEditBlock->GetPos();

		// 現在ブロックがあるエリアを計算
		nEditBlockXBlock = (int)((EditBlockPos.x + (MASS_SIZE_X_HALF * MASS_BLOCK_X)) / MASS_SIZE_X_HALF);
		nEditBlockZBlock = (int)((-EditBlockPos.z + (MASS_SIZE_Z_HALF * MASS_BLOCK_Z)) / MASS_SIZE_Z_HALF);
		nEditBlockXBlock++;
		nEditBlockZBlock++;

		// ヘッダー生成開始
		ImGui::CollapsingHeader(u8"エリアにあるブロックの編集");

		// ブロック情報を表示
		ImGui::SliderInt(u8"横のエリア_編集", &nEditBlockXBlock, 1, MASS_BLOCK_X * 2);
		ImGui::SliderInt(u8"奥行のエリア_編集", &nEditBlockZBlock, 1, MASS_BLOCK_Z * 2);
		ImGui::InputInt(u8"種類_編集", &nEditBlockType);
		ImGui::InputInt(u8"使用するモデルの番号_編集", &nEditBlockModelIdx);
		ImGui::InputFloat3(u8"向き_編集", &fEditBlockRot[0]);
		ImGui::InputFloat(u8"当たり判定の幅_編集", &fEditBlockColWidth);
		ImGui::InputFloat(u8"当たり判定の高さ_編集", &fEditBlockColHeight);
		ImGui::InputFloat(u8"当たり判定の奥行_編集", &fEditBlockColDepth);

		// ブロック情報判定
		if (nEditBlockType < 0)
		{// ブロック種類データが0を下回っている
			nEditBlockType = nEditBlockTypeDef;
		}
		if (nEditBlockModelIdx > pModelCreate->GetNumModel() - 1 || nEditBlockModelIdx < 0)
		{// 読み込んだモデルの総数を超えて入力されている
			nEditBlockModelIdx = nEditBlockModelIdxDef;
		}
		if (fEditBlockColWidth < 0.0f)
		{// 当たり判定幅情報が0を下回っている
			fEditBlockColWidth = fEditBlockColWidthDef;
		}
		if (fEditBlockColHeight < 0.0f)
		{// 当たり判定高さ情報が0を下回っている
			fEditBlockColHeight = fEditBlockColHeightDef;
		}
		if (fEditBlockColDepth < 0.0f)
		{// 当たり判定奥行情報が0を下回っている
			fEditBlockColDepth = fEditBlockColDepthDef;
		}

		// ボックスコライダーを作り直すか判定
		if (fEditBlockColWidth != fEditBlockColWidthDef || fEditBlockColHeight != fEditBlockColHeightDef || fEditBlockColDepth != fEditBlockColDepthDef)
		{// 値が変更されている
			pEditBlock->RemakeBoxCollider(fEditBlockColWidth, fEditBlockColHeight, fEditBlockColDepth);
		}

		// モデルを作成しなおすか判定
		if (nEditBlockModelIdx != nEditBlockModelIdxDef)
		{
			pEditBlock->BindModel(pModelCreate->GetMesh(nEditBlockModelIdx), pModelCreate->GetBuffMat(nEditBlockModelIdx),
				pModelCreate->GetNumMat(nEditBlockModelIdx), pModelCreate->GetTexture(nEditBlockModelIdx));
		}

		// 位置を計算する
		EditBlockPos.x = ((-MASS_SIZE_X_HALF * MASS_BLOCK_X)) + (MASS_SIZE_X_HALF * (nEditBlockXBlock - 1)) + (MASS_SIZE_X_HALF / 2.0f);
		EditBlockPos.z = ((MASS_SIZE_Z_HALF * MASS_BLOCK_Z)) - (MASS_SIZE_Z_HALF * (nEditBlockZBlock - 1)) - (MASS_SIZE_Z_HALF / 2.0f);

		// ブロック情報設定
		pEditBlock->SetPos(EditBlockPos);
		pEditBlock->SetType(nEditBlockType);
		pEditBlock->SetModelIdx(nEditBlockModelIdx);
		pEditBlock->SetRot(D3DXVECTOR3(fEditBlockRot[0], fEditBlockRot[1], fEditBlockRot[2]));
		pEditBlock->GetBoxCollider()->SetPos(EditBlockPos);
		pEditBlock->GetBoxCollider()->SetWidth(fEditBlockColWidth);
		pEditBlock->GetBoxCollider()->SetHeight(fEditBlockColHeight);
		pEditBlock->GetBoxCollider()->SetDepth(fEditBlockColDepth);

		if (ImGui::Button(u8"削除"))
		{// ボタン押下
			pEditBlock->Uninit();
			pEditBlock = NULL;
			pEditor->SetEditBlock(NULL);
		}
	}
}

//=============================================================================
// ImGuiの川情報描画処理
//=============================================================================
void CImGui_Jukiya::RiverDebug(CEditor *pEditor)
{
	// ヘッダー生成開始
	ImGui::CollapsingHeader(u8"川の値設定");

	// 川に張り付けるテクスチャ番号を設定
	int nNumTex = pEditor->GetMap()->GetTextureManager()->GetNumTexture();
	int nRiverTexIdx = pEditor->GetRiverTexIdx();
	ImGui::SliderInt(u8"テクスチャの番号", &nRiverTexIdx, 0, nNumTex - 1);
	pEditor->SetRiverTexIdx(nRiverTexIdx);
}

//=============================================================================
// ImGuiの氷情報描画処理
//=============================================================================
void CImGui_Jukiya::IceDebug(CEditor *pEditor)
{
	// ヘッダー生成開始
	ImGui::CollapsingHeader(u8"氷の値設定");

	// 氷に張り付けるテクスチャ番号を設定
	int nNumTex = pEditor->GetMap()->GetTextureManager()->GetNumTexture();
	int nIceTexIdx = pEditor->GetIceTexIdx();
	ImGui::SliderInt(u8"テクスチャの番号", &nIceTexIdx, 0, nNumTex - 1);
	pEditor->SetIceTexIdx(nIceTexIdx);
}

//=============================================================================
// ImGuiの配置物情報描画処理
//=============================================================================
void CImGui_Jukiya::ObjectDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// ウィンドウ生成開始
		ImGui::Begin(u8"配置物情報");

		// 配置するオブジェクトの種類を設定
		int nObjMode = pEditor->GetObjectMode();
		int nObjModeDef = nObjMode;
		ImGui::RadioButton(u8"モデル", &nObjMode, CEditor::OBJECTMODE_MODEL); ImGui::SameLine();
		ImGui::RadioButton(u8"ビルボード", &nObjMode, CEditor::OBJECTMODE_BILLBOARD); ImGui::SameLine();
		ImGui::RadioButton(u8"エフェクト", &nObjMode, CEditor::OBJECTMODE_EFFECT);
		pEditor->SetObjectMode((CEditor::OBJECTMODE)nObjMode);
		ImGui::Text("");

		if (nObjMode != nObjModeDef)
		{
			pEditor->SwitchBaseObject(nObjMode);
		}

		// 現在のモードによって処理わけ
		if (nObjMode == CEditor::OBJECTMODE_MODEL)
		{// 配置モデル情報表示
			ObjModelDebug(pEditor);
		}
		else if (nObjMode == CEditor::OBJECTMODE_BILLBOARD)
		{// 配置ビルボード情報表示
			ObjBillboardDebug(pEditor);
		}
		else if (nObjMode == CEditor::OBJECTMODE_EFFECT)
		{// 配置エフェクト情報表示
			ObjEffectDebug(pEditor);
		}

		// ウィンドウ生成終了
		ImGui::End();
	}
}

//=============================================================================
// ImGuiの配置モデル情報描画処理
//=============================================================================
void CImGui_Jukiya::ObjModelDebug(CEditor *pEditor)
{
	// ヘッダー生成開始
	ImGui::CollapsingHeader(u8"配置モデルの値設定");

	// 各種データを取得
	int nObjModelTypeMax = pEditor->GetMap()->GetNumObjectData();
	int nObjModelType = pEditor->GetObjModelType();
	float fObjModelPos[3] = { pEditor->GetObjModelPos().x, pEditor->GetObjModelPos().y ,pEditor->GetObjModelPos().z };
	int nObjModelTypeDef = nObjModelType;

	// 向きを360°換算に変える
	D3DXVECTOR3 ObjRot = pEditor->GetObjModelRot();
	ObjRot.x = D3DXToDegree(ObjRot.x);
	ObjRot.y = D3DXToDegree(ObjRot.y);
	ObjRot.z = D3DXToDegree(ObjRot.z);
	float fObjModelRot[3] = { ObjRot.x, ObjRot.y ,ObjRot.z };

	// 各種データを編集
	ImGui::InputInt(u8"配置モデルの種類", &nObjModelType);
	ImGui::InputFloat3(u8"配置モデルの座標", &fObjModelPos[0]);
	ImGui::InputFloat3(u8"配置モデルの向き", &fObjModelRot[0]);

	// 各種データ類判定
	if (nObjModelType < 0 || nObjModelType > nObjModelTypeMax - 1)
	{
		nObjModelType = nObjModelTypeDef;
	}

	// 向きを円周率換算に変える
	fObjModelRot[0] = D3DXToRadian(fObjModelRot[0]);
	fObjModelRot[1] = D3DXToRadian(fObjModelRot[1]);
	fObjModelRot[2] = D3DXToRadian(fObjModelRot[2]);

	// 各種データ類設定
	pEditor->SetObjModelType(nObjModelType);
	pEditor->SetObjModelPos(D3DXVECTOR3(fObjModelPos[0], fObjModelPos[1], fObjModelPos[2]));
	pEditor->SetObjModelRot(D3DXVECTOR3(fObjModelRot[0], fObjModelRot[1], fObjModelRot[2]));

	// オブジェクトを作り替えるかどうか判定
	if (nObjModelType != nObjModelTypeDef)
	{
		pEditor->SwitchBaseObject(CEditor::OBJECTMODE_MODEL);
	}

	// ベースオブジェクトに値を設定
	pEditor->SetValueToBaseObject();

	// 生成するかどうかチェック
	if (ImGui::Button(u8"配置モデル生成"))
	{// ボタン押下
		pEditor->CreateObject();
		m_nCreateObjectCounter = IMGUI_CREATE_DISP;
	}

	// 配置できたかどうか表示する
	m_nCreateObjectCounter--;
	if (m_nCreateObjectCounter <= 0)
	{
		m_nCreateObjectCounter = 0;
	}
	else
	{
		ImGui::SameLine(); ImGui::Text(u8"配置完了!!");
	}
}

//=============================================================================
// ImGuiの配置ビルボード情報描画処理
//=============================================================================
void CImGui_Jukiya::ObjBillboardDebug(CEditor *pEditor)
{
	// ヘッダー生成開始
	ImGui::CollapsingHeader(u8"配置ビルボードの値設定");

	// 各種データを取得
	int nObjBillTexIdxMax = pEditor->GetMap()->GetTextureManager()->GetNumTexture();
	int nObjBillTexIdx = pEditor->GetObjBillTexIdx();
	float fObjBillPos[3] = { pEditor->GetObjBillPos().x, pEditor->GetObjBillPos().y ,pEditor->GetObjBillPos().z };
	float fObjBillCol[4] = { pEditor->GetObjBillCol().r, pEditor->GetObjBillCol().g ,pEditor->GetObjBillCol().b ,pEditor->GetObjBillCol().a };
	float fObjBillWidth = pEditor->GetObjBillWidth();
	float fObjBillHeight = pEditor->GetObjBillHeight();
	bool bObjBillLighting = pEditor->GetObjBillLighting();
	bool bObjBillDrawAddtive = pEditor->GetObjBillDrawAddtive();
	int nObjBillTexIdxDef = nObjBillTexIdx;

	// 向きを360°換算に変える
	float fObjBillRot = pEditor->GetObjBillRot();
	fObjBillRot = D3DXToDegree(fObjBillRot);

	// 各種データを編集
	ImGui::InputInt(u8"配置ビルボードの使用するテクスチャの番号", &nObjBillTexIdx);
	ImGui::InputFloat3(u8"配置ビルボードの座標", &fObjBillPos[0]);
	ImGui::InputFloat(u8"配置ビルボードの向き", &fObjBillRot);
	ImGui::ColorEdit4(u8"配置ビルボードの色", &fObjBillCol[0]);
	ImGui::InputFloat(u8"配置ビルボードの幅", &fObjBillWidth);
	ImGui::InputFloat(u8"配置ビルボードの高さ", &fObjBillHeight);
	ImGui::Checkbox(u8"配置ビルボードのライティングするかどうか", &bObjBillLighting);
	ImGui::Checkbox(u8"配置ビルボードの加算合成で描画するかどうか", &bObjBillDrawAddtive);

	// 各種データ類判定
	if (nObjBillTexIdx < 0 || nObjBillTexIdx > nObjBillTexIdxMax - 1)
	{
		nObjBillTexIdx = nObjBillTexIdxDef;
	}

	// 向きを円周率換算に変える
	fObjBillRot = D3DXToRadian(fObjBillRot);

	// 各種データ類設定
	pEditor->SetObjBillTexIdx(nObjBillTexIdx);
	pEditor->SetObjBillPos(D3DXVECTOR3(fObjBillPos[0], fObjBillPos[1], fObjBillPos[2]));
	pEditor->SetObjBillRot(fObjBillRot);
	pEditor->SetObjBillCol(D3DXCOLOR(fObjBillCol[0], fObjBillCol[1], fObjBillCol[2], fObjBillCol[3]));
	pEditor->SetObjBillWidth(fObjBillWidth);
	pEditor->SetObjBillHeight(fObjBillHeight);
	pEditor->SetObjBillLighting(bObjBillLighting);
	pEditor->SetObjBillDrawAddtive(bObjBillDrawAddtive);

	// テクスチャを張り替えるか判定
	if (nObjBillTexIdx != nObjBillTexIdxDef)
	{
		CBillboardObject *pBill = pEditor->GetBaseObject()->GetBillboardObj();
		pBill->BindTexture(pEditor->GetMap()->GetTextureManager()->GetTexture(nObjBillTexIdx));
	}

	// ベースオブジェクトに値を設定
	pEditor->SetValueToBaseObject();

	// 生成するかどうかチェック
	if (ImGui::Button(u8"配置ビルボード生成"))
	{// ボタン押下
		pEditor->CreateBillboardObj();
		m_nCreateBillObjCounter = IMGUI_CREATE_DISP;
	}

	// 配置できたかどうか表示する
	m_nCreateBillObjCounter--;
	if (m_nCreateBillObjCounter <= 0)
	{
		m_nCreateBillObjCounter = 0;
	}
	else
	{
		ImGui::SameLine(); ImGui::Text(u8"配置完了!!");
	}
}

//=============================================================================
// ImGuiのエフェクト情報描画処理
//=============================================================================
void CImGui_Jukiya::ObjEffectDebug(CEditor *pEditor)
{
	// ヘッダー生成開始
	ImGui::CollapsingHeader(u8"配置エフェクトの値設定");

	// 各種データを取得
	int nObjEffectTypeMax = CManager::GetBaseMode()->GetEffectManager()->GetNumEmitterData();
	int nObjEffectType = pEditor->GetObjEffectType();
	float fObjEffectPos[3] = { pEditor->GetObjEffectPos().x, pEditor->GetObjEffectPos().y ,pEditor->GetObjEffectPos().z };
	int nObjEffectTypeDef = nObjEffectType;

	// 向きを360°換算に変える
	D3DXVECTOR3 ObjRot = pEditor->GetObjEffectRot();
	ObjRot.x = D3DXToDegree(ObjRot.x);
	ObjRot.y = D3DXToDegree(ObjRot.y);
	ObjRot.z = D3DXToDegree(ObjRot.z);
	float fObjEffectRot[3] = { ObjRot.x, ObjRot.y ,ObjRot.z };

	// 各種データを編集
	ImGui::InputInt(u8"配置エフェクトの種類", &nObjEffectType);
	ImGui::InputFloat3(u8"配置エフェクトの座標", &fObjEffectPos[0]);
	ImGui::InputFloat3(u8"配置エフェクトの向き", &fObjEffectRot[0]);

	// 各種データ類判定
	if (nObjEffectType < 0 || nObjEffectType > nObjEffectTypeMax - 1)
	{
		nObjEffectType = nObjEffectTypeDef;
	}

	// 向きを円周率換算に変える
	fObjEffectRot[0] = D3DXToRadian(fObjEffectRot[0]);
	fObjEffectRot[1] = D3DXToRadian(fObjEffectRot[1]);
	fObjEffectRot[2] = D3DXToRadian(fObjEffectRot[2]);

	// 各種データ類設定
	pEditor->SetObjEffectType(nObjEffectType);
	pEditor->SetObjEffectPos(D3DXVECTOR3(fObjEffectPos[0], fObjEffectPos[1], fObjEffectPos[2]));
	pEditor->SetObjEffectRot(D3DXVECTOR3(fObjEffectRot[0], fObjEffectRot[1], fObjEffectRot[2]));

	// オブジェクトを作り替えるかどうか判定
	if (nObjEffectType != nObjEffectTypeDef)
	{
		bool bEffectLoop = pEditor->GetEffectManager()->GetEmitterData(nObjEffectType)->GetLoop();
		if (bEffectLoop == true)
		{
			pEditor->SwitchBaseObject(CEditor::OBJECTMODE_EFFECT);
		}
		else
		{
			nObjEffectType = nObjEffectTypeDef;
			pEditor->SetObjEffectType(nObjEffectType);
		}
	}

	// ベースオブジェクトに値を設定
	pEditor->SetValueToBaseObject();

	// 生成するかどうかチェック
	if (ImGui::Button(u8"配置エフェクト生成"))
	{// ボタン押下
		pEditor->CreateEffect();
		m_nCreateEffectCounter = IMGUI_CREATE_DISP;
	}

	// 配置できたかどうか表示する
	m_nCreateEffectCounter--;
	if (m_nCreateEffectCounter <= 0)
	{
		m_nCreateEffectCounter = 0;
	}
	else
	{
		ImGui::SameLine(); ImGui::Text(u8"配置完了!!");
	}
}

//=============================================================================
// ImGuiのリスポーン情報描画処理
//=============================================================================
void CImGui_Jukiya::RespawnDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// ウィンドウ生成開始
		ImGui::Begin(u8"プレイヤーのリスポーン情報");

		// プレイヤーのリスポーン情報描画
		ImGui::SliderInt(u8"情報を操作する番号", &m_nPlayerResIdx, 0, MAX_PLAYER_RESPAWN - 1);
		RespawnInfoDebug(pEditor->GetMap()->GetPlayerRespawn(m_nPlayerResIdx));

		// ウィンドウ生成終了
		ImGui::End();

		// ウィンドウ生成開始
		ImGui::Begin(u8"敵のリスポーン情報");

		// 敵のリスポーン情報描画
		ImGui::SliderInt(u8"情報を操作する番号", &m_nEnemyResIdx, 0, MAX_ENEMY_RESPAWN - 1);
		RespawnInfoDebug(pEditor->GetMap()->GetEnemyRespawn(m_nEnemyResIdx));

		// ウィンドウ生成終了
		ImGui::End();
	}
}

//=============================================================================
// ImGuiのプレイヤーのリスポーン情報描画処理
//=============================================================================
void CImGui_Jukiya::RespawnInfoDebug(CRespawn *pRespawn)
{
	if (pRespawn != NULL)
	{
		// 今いるエリア番号を取得
		int nResAreaX = pRespawn->GetAreaX();
		int nResAreaZ = pRespawn->GetAreaZ();

		// エリア番号編集
		ImGui::SliderInt(u8"横のエリア", &nResAreaX, 1, MASS_BLOCK_X);
		ImGui::SliderInt(u8"奥行のエリア", &nResAreaZ, 1, MASS_BLOCK_Z);

		// エリア番号設定
		pRespawn->SetArea(nResAreaX, nResAreaZ);
	}
}

//=============================================================================
// ImGuiの司令部情報描画処理
//=============================================================================
void CImGui_Jukiya::HeadQuartersDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// ウィンドウ生成開始
		ImGui::Begin(u8"司令部情報");

		// 司令部の情報描画処理
		CHeadQuarters *pHeadQuarters = pEditor->GetMap()->GetHeadQuarters();
		if (pHeadQuarters != NULL)
		{
			// 今いるエリア番号を取得
			int nHeadAreaX = pHeadQuarters->GetAreaX();
			int nHeadAreaZ = pHeadQuarters->GetAreaZ();

			// エリア番号編集
			ImGui::SliderInt(u8"横のエリア", &nHeadAreaX, 1, MASS_BLOCK_X);
			ImGui::SliderInt(u8"奥行のエリア", &nHeadAreaZ, 1, MASS_BLOCK_Z);

			// エリア番号設定
			pHeadQuarters->SetArea(nHeadAreaX, nHeadAreaZ);
		}

		// ウィンドウ生成終了
		ImGui::End();
	}
}


//=============================================================================
// ImGuiの敵の生成情報描画処理
//=============================================================================
void CImGui_Jukiya::EnemyListDebug(CEditor *pEditor)
{
	if (pEditor == NULL) return;
	if (pEditor->GetMap() == NULL) return;

	// ウィンドウ生成開始
	ImGui::Begin(u8"敵の生成情報");

	// 生成する敵の数を取得
	int nNumEnemyList = pEditor->GetMap()->GetNumEnemyListData();
	int nNumEnemyListDef = nNumEnemyList;
	ImGui::InputInt(u8"生成する敵の数", &nNumEnemyList);
	ImGui::Text("");

	// 敵の数判定
	if (nNumEnemyList < 1)
	{
		nNumEnemyList = nNumEnemyListDef;
	}
	if (nNumEnemyList != nNumEnemyListDef)
	{
		// 敵の生成情報データクラスを作り直す
		pEditor->GetMap()->ReCreateEnemyData(nNumEnemyList);

		// データを表示する番号を直す
		m_nEnemyListIdx = 0;
	}


	// 敵の生成情報表示
	ImGui::CollapsingHeader(u8"敵の生成情報の値設定");

	// 表示する敵の生成情報の番号を設定
	m_nEnemyListIdx++;
	ImGui::SliderInt(u8"表示する番号", &m_nEnemyListIdx, 1, nNumEnemyList);
	ImGui::Text("");
	m_nEnemyListIdx--;


	CEnemy_ListData **pEnemyData = pEditor->GetMap()->GetEnemyListData();
	int nEnemyListRespawnIdx = pEnemyData[m_nEnemyListIdx]->GetRespawnIdx();
	int nEnemyListEnemyType = pEnemyData[m_nEnemyListIdx]->GetEnemyType();
	int nEnemyListRespawnTime = pEnemyData[m_nEnemyListIdx]->GetRespawnTime();
	bool bEnemyListItem = pEnemyData[m_nEnemyListIdx]->GetItem();
	int nEnemyListItemType = pEnemyData[m_nEnemyListIdx]->GetItemType();
	int nEnemyListAI_Bullet = pEnemyData[m_nEnemyListIdx]->GetAI_BulletEva();
	int nEnemyListAI_Mass = pEnemyData[m_nEnemyListIdx]->GetAI_MassEva();
	int nEnemyListAI_Down = pEnemyData[m_nEnemyListIdx]->GetAI_DownEva();
	ImGui::SliderInt(u8"リスポーン位置の番号", &nEnemyListRespawnIdx, 0, MAX_ENEMY_RESPAWN - 1);
	ImGui::InputInt(u8"種類", &nEnemyListEnemyType);
	ImGui::InputInt(u8"リスポーンする時間", &nEnemyListRespawnTime);
	ImGui::Checkbox(u8"倒した時にアイテムを生成するかどうか", &bEnemyListItem);
	if (bEnemyListItem == true)
	{
		ImGui::InputInt(u8"出現させるアイテムの番号", &nEnemyListItemType);
	}

	ImGui::InputInt(u8"弾を打つ際のランダム評価値", &nEnemyListAI_Bullet);
	ImGui::InputInt(u8"向きを変えさせるランダム評価値", &nEnemyListAI_Mass);
	ImGui::InputInt(u8"下向きに動かせるランダム評価値", &nEnemyListAI_Down);

	// データ判定
	if (nEnemyListRespawnTime < 0)
	{
		nEnemyListRespawnTime = 0;
	}
	if (nEnemyListEnemyType < 0)
	{
		nEnemyListEnemyType = 0;
	}
	if (nEnemyListItemType < 0)
	{
		nEnemyListItemType = 0;
	}

	// AIのデータ
	if (nEnemyListAI_Bullet < 1)
	{
		nEnemyListAI_Bullet = 1;
	}
	if (nEnemyListAI_Mass < 1)
	{
		nEnemyListAI_Mass = 1;
	}
	if (nEnemyListAI_Down < 1)
	{
		nEnemyListAI_Down = 1;
	}

	// データ設定
	CEnemy_ListData EnemyData;
	EnemyData.SetRespawnIdx(nEnemyListRespawnIdx);
	EnemyData.SetEnemyType(nEnemyListEnemyType);
	EnemyData.SetRespawnTime(nEnemyListRespawnTime);
	EnemyData.SetItem(bEnemyListItem);
	EnemyData.SetItemType(nEnemyListItemType);
	EnemyData.SetAI_BulletEva(nEnemyListAI_Bullet);
	EnemyData.SetAI_MassEva(nEnemyListAI_Mass);
	EnemyData.SetAI_DownEva(nEnemyListAI_Down);
	pEditor->GetMap()->GetEnemyListData(m_nEnemyListIdx)->Cpy(&EnemyData);

	// ウィンドウ生成終了
	ImGui::End();
}

//=============================================================================
// ImGuiの敵の生成情報描画処理
//=============================================================================
void CImGui_Jukiya::SkyDebug(CEditor *pEditor)
{
	if (pEditor == NULL) return;
	if (pEditor->GetMap() == NULL) return;

	// ウィンドウ生成開始
	ImGui::Begin(u8"空情報");

	// 川に張り付けるテクスチャ番号を設定
	CSky *pSky = pEditor->GetMap()->GetSky();
	int nNumTex = pEditor->GetMap()->GetTextureManager()->GetNumTexture();
	int nSkyTexIdx = pSky->GetTexIdx();
	ImGui::SliderInt(u8"テクスチャの番号", &nSkyTexIdx, 0, nNumTex - 1);
	pSky->SetTexIdx(nSkyTexIdx);
	pSky->BindTexture(pEditor->GetMap()->GetTextureManager()->GetTexture(nSkyTexIdx));

	// ウィンドウ生成終了
	ImGui::End();
}

//=============================================================================
// ImGuiのデモプレイ描画処理
//=============================================================================
void CImGui_Jukiya::DemoplayDebug(void)
{
	ImGui::Text("");
	ImGui::CollapsingHeader(u8"デモプレイ設定");

	// ウィンドウ生成終了
	ImGui::End();
}

//=============================================================================
// ImGuiの変数をクリアする
//=============================================================================
void CImGui_Jukiya::ClearVariable(void)
{
	// 各種類ごとの変数をクリアする
	ClearMapVariable();          // マップ用
	ClearModeVariable();         // 画面切り替え用
	ClearModelListVariable();    // モデルリスト用
	ClearTexListVariable();      // テクスチャリスト用
	ClearLightVariable();        // ライト用
	ClearGameFieldVariable();    // ゲームフィールド用
	ClearObjectVariable();       // 配置物用
	ClearRespawnVariable();      // リスポーン用
	ClearEnemyListVariable();    // 敵の生成情報用
}

//=============================================================================
// ImGuiの画面切り替え用変数をクリアする
//=============================================================================
void CImGui_Jukiya::ClearModeVariable(void)
{
	m_nLoadModeCounter = 0;
	m_bLoadModeDisp = false;
}

//=============================================================================
// ImGuiのマップ用変数をクリアする
//=============================================================================
void CImGui_Jukiya::ClearMapVariable(void)
{
	m_nSaveMapCounter = 0;
	m_bSaveMapDisp = false;
	m_nLoadMapCounter = 0;
	m_bLoadMapDisp = false;
}

//=============================================================================
// ImGuiのモデルリスト用変数をクリアする
//=============================================================================
void CImGui_Jukiya::ClearModelListVariable(void)
{
	m_nSaveModelListCounter = 0;
	m_bSaveModelListDisp = false;
	m_nLoadModelListCounter = 0;
	m_bLoadModelListDisp = false;
}

//=============================================================================
// ImGuiのテクスチャリスト用変数をクリアする
//=============================================================================
void CImGui_Jukiya::ClearTexListVariable(void)
{
	m_nSaveTexListCounter = 0;
	m_bSaveTexListDisp = false;
	m_nLoadTexListCounter = 0;
	m_bLoadTexListDisp = false;
}

//=============================================================================
// ImGuiのライト用変数をクリアする
//=============================================================================
void CImGui_Jukiya::ClearLightVariable(void)
{
	m_nLightIdx = 0;
	m_nSaveLightCounter = 0;
	m_bSaveLightDisp = false;
	m_nLoadLightCounter = 0;
	m_bLoadLightDisp = false;
}

//=============================================================================
// ImGuiのゲームフィールド用変数をクリアする
//=============================================================================
void CImGui_Jukiya::ClearGameFieldVariable(void)
{
	m_nSaveGameFieldCounter = 0;
	m_bSaveGameFieldDisp = false;
	m_nLoadGameFieldCounter = 0;
	m_bLoadGameFieldDisp = false;
}

//=============================================================================
// ImGuiの配置物用変数をクリアする
//=============================================================================
void CImGui_Jukiya::ClearObjectVariable(void)
{
	m_nSaveObjectCounter = 0;
	m_bSaveObjectDisp = false;
	m_nLoadObjectCounter = 0;
	m_bLoadObjectDisp = false;
	m_nCreateObjectCounter = 0;
	m_nCreateBillObjCounter = 0;
	m_nCreateEffectCounter = 0;
}

//=============================================================================
// ImGuiのリスポーン用変数をクリアする
//=============================================================================
void CImGui_Jukiya::ClearRespawnVariable(void)
{
	m_nPlayerResIdx = 0;
	m_nEnemyResIdx = 0;
}

//=============================================================================
// ImGuiの敵の生成情報用変数をクリアする
//=============================================================================
void CImGui_Jukiya::ClearEnemyListVariable(void)
{
	m_nEnemyListIdx = 0;
}