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

#include "object.h"
#include "editor.h"
#include "objectplayer.h"
#include "map.h"
#include "modelcreate.h"
#include "manager.h"
#include "camera.h"

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
	//カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	//オブジェクトプレイヤーのポインタ取得処理
	CObjectPlayer *pObjectPlayer;
	pObjectPlayer = CEditor::GetObjectPlayer();

	//オブジェクトのポインタ情報取得
	CObject *pObject;
	pObject = pObjectPlayer->GetObjectPointer();

	//マップのポインタ情報取得
	CMap *pMap = CEditor::GetMap();

	// 新しいフレームを用意する
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ウィンドウ生成開始
	ImGui::Begin(u8"オブジェクトツール Ver1.1", false);

	// FPS描画
	ImGui::Text("FPS : %.1f", ImGui::GetIO().Framerate);

	if (pCamera != NULL)
	{
		//カメラの視点の取得代入
		float aCameraPosV[3] = {pCamera->GetPosV().x,pCamera->GetPosV().y,pCamera->GetPosV().z};

		//カメラの注視点の取得代入
		float aCameraPosR[3] = { pCamera->GetPosR().x,pCamera->GetPosR().y,pCamera->GetPosR().z };

		// カメラの視点の表示
		ImGui::InputFloat3(u8"カメラの視点", &aCameraPosV[0]);

		// カメラの注視点の表示
		ImGui::InputFloat3(u8"カメラの注視点", &aCameraPosR[0]);

		////視点の設置処理
		//pCamera->SetPosV(D3DXVECTOR3(aCameraPosV[0], aCameraPosV[1], aCameraPosV[2]));

		////注視点の設置処理
		//pCamera->SetPosR(D3DXVECTOR3(aCameraPosR[0], aCameraPosR[1], aCameraPosR[2]));

	}
	//現在の情報を保存する
	if (ImGui::Button(u8"保存する"))
	{
		ImGui::SameLine();
		if (pMap != NULL)
		{//情報がある場合
			pMap->SaveObject();
			ImGui::Text(u8"保存が完了しました。");
		}
		else
		{//情報がない場合
			ImGui::Text(u8"保存が出来ませんでした。");
		}
	}

	// 操作方法の表示
	ImGui::CollapsingHeader(u8"操作方法");

	ImGui::Text(u8"W,A,S,D [オブジェクトの移動]");
	ImGui::Text(u8"上,左,下,右 [カメラの移動]");
	ImGui::Text(u8"T,B,Q,E [カメラ注視点の移動]");
	ImGui::Text(u8"Y,N,Z,C [カメラ視点の移動]");
	ImGui::Text(u8"1,2 [種類の変更]");
	ImGui::Text(u8"Enter [モデルの生成]");
	ImGui::Text(u8"保存した場合、情報はdata/TEXT/MAP/OBJECT/save_object.txtにあります。");

	// ウィンドウ生成終了
	ImGui::End();

	// ウィンドウ生成開始
	ImGui::Begin(u8"オブジェクト", false);

	if (pObjectPlayer != NULL)
	{
		if (pObjectPlayer != NULL)
		{
			if (pObject != NULL)
			{
				//プレイヤーの選択を代入
				int nSelect = pObjectPlayer->GetSelectObject();

				//プレイヤーの位置取得代入
				float aPos[3] = { pObject->GetPos().x,  pObject->GetPos().y , pObject->GetPos().z };

				//プレイヤーの向き取得代入
				float aRot[3] = { pObject->GetRot().x,pObject->GetRot().y ,pObject->GetRot().z };

				//プレイヤーの大きさ取得代入
				float aSize[3] = { pObject->GetSize().x,  pObject->GetSize().y , pObject->GetSize().z };

				//プレイヤーのスピードの取得
				float fSpeed = pObjectPlayer->GetSpeed();

				//プレイヤーの当たり判定取得代入
				bool bHit = pObject->GetHit();

				//プレイヤーの破壊できるかどうか取得代入
				bool bDestroy = pObject->GetDestroy();

				//現在の種類表示
				ImGui::InputInt(u8"モデルの種類", &nSelect);

				//プレイヤーの位置表示
				ImGui::InputFloat3(u8"位置", &aPos[0]);

				//プレイヤーの向き表示
				ImGui::InputFloat3(u8"向き", &aRot[0]);

				//プレイヤーの大きさ表示
				ImGui::InputFloat3(u8"大きさ", &aSize[0]);

				//現在のスピード表示
				ImGui::DragFloat(u8"移動量", &fSpeed);

				//プレイヤーの当たり判定表示
				ImGui::Checkbox(u8"当たり判定", &bHit);

				//プレイヤーの破壊できるかどうか表示
				ImGui::Checkbox(u8"破壊できるかどうか", &bDestroy);

				if (nSelect != pObjectPlayer->GetSelectObject())
				{//現在と過去で種類が違う場合
				 //選択種類の設置処理
					pObjectPlayer->SetSelectObject(nSelect);
				}

				//位置の設置処理
				pObject->SetPos(D3DXVECTOR3(aPos[0], aPos[1], aPos[2]));
				//pObjectPlayer->SetPos(D3DXVECTOR3(aPos[0], aPos[1], aPos[2]));

				//向きの設置処理
				pObject->SetRot(D3DXVECTOR3(aRot[0], aRot[1], aRot[2]));

				//大きさの設置処理
				pObject->SetSize(D3DXVECTOR3(aSize[0], aSize[1], aSize[2]));

				if (fSpeed != pObjectPlayer->GetSpeed())
				{//現在と過去でスピードが違う場合
				 //スピードの代入
					pObjectPlayer->SetSpeed(fSpeed);
				}

				//当たり判定の設置処理
				pObject->SetHit(bHit);

				//破壊できるかどうかの設置処理
				pObject->SetDestroy(bDestroy);


				//モデル生成処理
				if(ImGui::Button(u8"モデルを生成する"))
				{
					if (pMap != NULL)
					{
						CObject *pObject = NULL;
						if (pObject == NULL)
						{
							//オブジェクトの生成
							pObject = CObject::Create(D3DXVECTOR3(aPos[0], aPos[1], aPos[2]),
								D3DXVECTOR3(aRot[0], aRot[1], aRot[2]),
								D3DXVECTOR3(aSize[0], aSize[1], aSize[2]),
								bHit,
								bDestroy);

							//LPD3DXMESH		mesh = pMap->GetCModelCreate()->GetMesh(nSelect);
							//LPD3DXBUFFER;
							//DWORD;
							pObject->BindX(pMap->GetCModelCreate()->GetMesh(nSelect),
								pMap->GetCModelCreate()->GetBuffMat(nSelect),
								pMap->GetCModelCreate()->GetNumMat(nSelect));
							pObject->SetType(nSelect);
						}
					}
				}
			}

		}

	}

	// ウィンドウ生成終了
	ImGui::End();


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