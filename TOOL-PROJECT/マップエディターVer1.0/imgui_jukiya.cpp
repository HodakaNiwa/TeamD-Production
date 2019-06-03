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
#include "editor.h"
#include "input.h"
#include "map.h"
#include "modelcreate.h"
#include "manager.h"
#include "camera.h"
#include "cameraManager.h"
#include "light.h"
#include "lightManager.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define IMGUI_SAVE_DISP (60)
#define IMGUI_LOAD_DISP (60)

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
			CFade::SetFade((CManager::MODE)nMode, CFade::FADE_OUT);
		}
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
			ImGui::RadioButton(u8"ゲームフィールド", &nEditMode, CEditor::EDITMODE_GAMEFIELD); ImGui::SameLine();
			ImGui::RadioButton(u8"ライト", &nEditMode, CEditor::EDITMODE_LIGHT); ImGui::SameLine();
			ImGui::RadioButton(u8"配置物", &nEditMode, CEditor::EDITMODE_OBJECT);
			pEditor->SetEditMode((CEditor::EDITMODE)nEditMode);
			ImGui::TreePop();
		}
		ImGui::Text("");

		// カメラ情報表示
		CameraDebug(pEditor);

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
		ImGui::RadioButton(u8"エディター", &nCameraMode, CEditor::CAMERAMODE_EDIT);
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
	strcpy(aSaveFileName, pEditor->GetMapSaveFileName());

	// 保存するファイル名設定
	ImGui::InputText(u8"保存するファイル名", aSaveFileName, IM_ARRAYSIZE(aSaveFileName));
	pEditor->SetMapSaveFileName(aSaveFileName);

	if (ImGui::Button(u8"保存する"))
	{// ボタン押下
	    // マップ情報保存
		pMap->SetGameFieldFileName(pEditor->GetGameFieldSaveFileName());
		pMap->SetModelListFileName(pEditor->GetModelListSaveFileName());
		pMap->SetTexListFileName(pEditor->GetTexListSaveFileName());
		pMap->SetLightFileName(pEditor->GetLightSaveFileName());
		pMap->SetObjectFileName(pEditor->GetObjectSaveFileName());
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

		// 保存情報表示
		SaveLightDebug(pEditor, pEditor->GetMap());

		// 保存成否情報表示
		m_nSaveLightCounter = SaveDisp(m_nSaveLightCounter, m_bSaveLightDisp);

		// 読み込み情報表示
		LoadLightDebug(pEditor, pEditor->GetMap());

		// 読み込み成否情報表示
		m_nLoadLightCounter = LoadDisp(m_nLoadLightCounter, m_bLoadLightDisp);
		ImGui::Text("");

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

		// ライト情報を表示
		for (int nCntLight = 0; nCntLight < nNumLight; nCntLight++)
		{// 現在生成されているライトの数だけ繰り返し
			pLightManager = LightInfo(pLightManager, nCntLight);
		}
	}
}

//=============================================================================
// ImGuiのライト情報表示処理
//=============================================================================
CLightManager *CImGui_Jukiya::LightInfo(CLightManager *pLightManager, int nCntLight)
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

	return pLightManager;
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
// ImGuiのライト保存情報描画処理
//=============================================================================
void CImGui_Jukiya::SaveLightDebug(CEditor *pEditor, CMap *pMap)
{
	char aSaveFileName[256];
	strcpy(aSaveFileName, pEditor->GetLightSaveFileName());

	// 保存するファイル名設定
	ImGui::InputText(u8"保存するファイル名", aSaveFileName, IM_ARRAYSIZE(aSaveFileName));
	pEditor->SetLightSaveFileName(aSaveFileName);

	if (ImGui::Button(u8"保存する"))
	{// ボタン押下
		if (FAILED(pMap->SaveLight(aSaveFileName)))
		{
			m_bSaveLightDisp = false;
		}
		else
		{
			m_bSaveLightDisp = true;
		}
		m_nSaveLightCounter = IMGUI_SAVE_DISP;
	}
}

//=============================================================================
// ImGuiのライト保存情報描画処理
//=============================================================================
void CImGui_Jukiya::LoadLightDebug(CEditor *pEditor, CMap *pMap)
{
	char aLoadFileName[256];
	strcpy(aLoadFileName, pEditor->GetLightLoadFileName());

	// 読み込むファイル名を設定
	ImGui::InputText(u8"読み込むファイル名", aLoadFileName, IM_ARRAYSIZE(aLoadFileName));
	pEditor->SetLightLoadFileName(aLoadFileName);

	if (ImGui::Button(u8"読み込む"))
	{// ボタン押下
		// ライトを破棄する
		pMap->DeleteLight();

		char aStr[256];
		strcpy(aLoadFileName, SAVEFILENAME_LIGHT);
		if (FAILED(pMap->LoadLight(strcat(aLoadFileName, pEditor->GetLightLoadFileName()), aStr)))
		{
			m_bLoadLightDisp = false;
		}
		else
		{
			m_bLoadLightDisp = true;
		}
		m_nLoadLightCounter = IMGUI_LOAD_DISP;
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
		}
	}

	return pLightManagerNew;
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

		// 保存情報表示
		SaveGameFieldDebug(pEditor, pEditor->GetMap());

		// 保存成否情報表示
		m_nSaveGameFieldCounter = SaveDisp(m_nSaveGameFieldCounter, m_bSaveGameFieldDisp);

		// 読み込み情報表示
		LoadGameFieldDebug(pEditor, pEditor->GetMap());

		// 読み込み成否情報表示
		m_nLoadGameFieldCounter = LoadDisp(m_nLoadGameFieldCounter, m_bLoadGameFieldDisp);
		ImGui::Text("");

		// ブロック情報表示
		BlockDebug(pEditor);

		// ウィンドウ生成終了
		ImGui::End();
	}
}

//=============================================================================
// ImGuiのゲームフィールド保存情報描画処理
//=============================================================================
void CImGui_Jukiya::SaveGameFieldDebug(CEditor *pEditor, CMap *pMap)
{
	char aSaveFileName[256];
	strcpy(aSaveFileName, pEditor->GetGameFieldSaveFileName());

	// 保存するファイル名を設定
	ImGui::InputText(u8"保存するファイル名", aSaveFileName, IM_ARRAYSIZE(aSaveFileName));
	pEditor->SetGameFieldSaveFileName(aSaveFileName);

	if (ImGui::Button(u8"保存する"))
	{// ボタン押下
		if (FAILED(pMap->SaveGameField(aSaveFileName)))
		{
			m_bSaveGameFieldDisp = false;
		}
		else
		{
			m_bSaveGameFieldDisp = true;
		}
		m_nSaveGameFieldCounter = IMGUI_SAVE_DISP;
	}
}

//=============================================================================
// ImGuiのゲームフィールド読み込み情報描画処理
//=============================================================================
void CImGui_Jukiya::LoadGameFieldDebug(CEditor *pEditor, CMap *pMap)
{
	char aLoadFileName[256];
	strcpy(aLoadFileName, pEditor->GetGameFieldLoadFileName());

	// 読み込むファイル名を設定
	ImGui::InputText(u8"読み込むファイル名", aLoadFileName, IM_ARRAYSIZE(aLoadFileName));
	pEditor->SetGameFieldLoadFileName(aLoadFileName);

	if (ImGui::Button(u8"読み込む"))
	{// ボタン押下
		// ゲームフィールドを破棄する
		pMap->DeleteGameField();

		char aStr[256];
		strcpy(aLoadFileName, SAVEFILENAME_GAMEFIELD);
		if (FAILED(pMap->LoadGameField(strcat(aLoadFileName, pEditor->GetGameFieldLoadFileName()), aStr)))
		{
			m_bLoadGameFieldDisp = false;
		}
		else
		{
			m_bLoadGameFieldDisp = true;
		}
		m_nLoadGameFieldCounter = IMGUI_LOAD_DISP;
	}
}

//=============================================================================
// ImGuiのブロック情報描画処理
//=============================================================================
void CImGui_Jukiya::BlockDebug(CEditor *pEditor)
{
	if (pEditor != NULL)
	{
		// マップクラスを取得
		CMap *pMap = pEditor->GetMap();

		// モデル管轄クラスを取得
		CModelCreate *pModelCreate = pMap->GetModelCreate();

		// ブロック情報取得
		int nBlockType = pEditor->GetBlockType();
		int nBlockModelIdx = pEditor->GetBlockModelIdx();
		bool bBlockBreak = pEditor->GetBlockBreak();
		float fBlockColWidth = pEditor->GetBlockColWidth();
		float fBlockColHeight = pEditor->GetBlockColHeight();
		float fBlockColDepth = pEditor->GetBlockColDepth();
		int nBlockTypeDef = nBlockType;
		int nBlockModelIdxDef = nBlockModelIdx;
		float fBlockColWidthDef = fBlockColWidth;
		float fBlockColHeightDef = fBlockColHeight;
		float fBlockColDepthDef = fBlockColDepth;

		// ヘッダー生成開始
		ImGui::CollapsingHeader(u8"配置するブロックの値設定");

		// ブロック情報を表示
		ImGui::InputInt(u8"種類", &nBlockType);
		ImGui::InputInt(u8"使用するモデルの番号", &nBlockModelIdx);
		ImGui::Checkbox(u8"壊せるかどうか", &bBlockBreak);
		ImGui::InputFloat(u8"当たり判定の幅", &fBlockColWidth);
		ImGui::InputFloat(u8"当たり判定の高さ", &fBlockColHeight);
		ImGui::InputFloat(u8"当たり判定の奥行", &fBlockColDepth);

		// ブロック情報判定
		if (nBlockType > CBlock::TYPE_MAX - 1 || nBlockType < 0)
		{// ブロックの種類の総数を超えて入力されている
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
		pEditor->SetBlockBreak(bBlockBreak);
		pEditor->SetBlockColWidth(fBlockColWidth);
		pEditor->SetBlockColHeight(fBlockColHeight);
		pEditor->SetBlockColDepth(fBlockColDepth);
	}
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

		// 保存情報表示
		SaveObjectDebug(pEditor, pEditor->GetMap());

		// 保存成否情報表示
		m_nSaveObjectCounter = SaveDisp(m_nSaveObjectCounter, m_bSaveObjectDisp);

		// 読み込み情報表示
		LoadObjectDebug(pEditor, pEditor->GetMap());

		// 読み込み成否情報表示
		m_nLoadObjectCounter = LoadDisp(m_nLoadObjectCounter, m_bLoadObjectDisp);
		ImGui::Text("");

		// ウィンドウ生成終了
		ImGui::End();
	}
}

//=============================================================================
// ImGuiの配置物保存情報描画処理
//=============================================================================
void CImGui_Jukiya::SaveObjectDebug(CEditor *pEditor, CMap *pMap)
{
	char aSaveFileName[256];
	strcpy(aSaveFileName, pEditor->GetObjectSaveFileName());

	// 保存するファイル名を設定
	ImGui::InputText(u8"保存するファイル名", aSaveFileName, IM_ARRAYSIZE(aSaveFileName));
	pEditor->SetObjectSaveFileName(aSaveFileName);

	if (ImGui::Button(u8"保存する"))
	{// ボタン押下
		if (FAILED(pMap->SaveObject(aSaveFileName)))
		{
			m_bSaveObjectDisp = false;
		}
		else
		{
			m_bSaveObjectDisp = true;
		}
		m_nSaveObjectCounter = IMGUI_SAVE_DISP;
	}
}

//=============================================================================
// ImGuiの配置物読み込み情報描画処理
//=============================================================================
void CImGui_Jukiya::LoadObjectDebug(CEditor *pEditor, CMap *pMap)
{
	char aLoadFileName[256];
	strcpy(aLoadFileName, pEditor->GetObjectLoadFileName());

	// 読み込むファイル名を設定
	ImGui::InputText(u8"読み込むファイル名", aLoadFileName, IM_ARRAYSIZE(aLoadFileName));
	pEditor->SetObjectLoadFileName(aLoadFileName);

	if (ImGui::Button(u8"読み込む"))
	{// ボタン押下
	    // 配置物を破棄する
		pMap->DeleteObject();

		char aStr[256];
		strcpy(aLoadFileName, SAVEFILENAME_OBJECT);
		if (FAILED(pMap->LoadGameField(strcat(aLoadFileName, pEditor->GetObjectLoadFileName()), aStr)))
		{
			m_bLoadObjectDisp = false;
		}
		else
		{
			m_bLoadObjectDisp = true;
		}
		m_nLoadObjectCounter = IMGUI_LOAD_DISP;
	}
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
	ClearModelListVariable();    // モデルリスト用
	ClearTexListVariable();      // テクスチャリスト用
	ClearLightVariable();        // ライト用
	ClearGameFieldVariable();    // ゲームフィールド用
	ClearObjectVariable();       // 配置物用
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
}