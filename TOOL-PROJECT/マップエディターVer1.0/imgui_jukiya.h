//=============================================================================
//
// ImGuiの処理 [imgui_jukiya.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _IMGUI_JUKIYA_H_
#define _IMGUI_JUKIYA_H_

#include "main.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CEditor;
class CCamera;
class CMap;
class CLightManager;
class CLight;
class CDirectionalLight;
class CPointLight;
class CSpotLight;
class CRespawn;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CImGui_Jukiya
{
public:	//誰からもアクセス可能
		//メンバ関数
	CImGui_Jukiya();
	~CImGui_Jukiya();
	void Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:	//自分だけがアクセス可能
	void ClearVariable(void);
	void ClearMapVariable(void);
	void ClearModelListVariable(void);
	void ClearTexListVariable(void);
	void ClearLightVariable(void);
	void ClearGameFieldVariable(void);
	void ClearObjectVariable(void);
	void ClearRespawnVariable(void);

	CLightManager *ChangeLight(CLightManager *pLightManager, int nCntLight, CLight *pLightOld, int nLightType, int nLightTypeDef);

	void WindowDebug(void);
	void EditerDebug(void);
	void CameraDebug(CEditor *pEditor);
	void GameCameraDebug(CCamera *pCamera);
	void EditorCameraDebug(CCamera *pCamera);
	void TopViewCameraDebug(CCamera *pCamera);
	void FieldDebug(CEditor *pEditor);
	int SaveDisp(int nCounter, bool bDisp);
	int LoadDisp(int nCounter, bool bDisp);
	void MapDebug(CEditor *pEditor);
	void SaveMapDebug(CEditor *pEditor, CMap *pMap);
	void LoadMapDebug(CEditor *pEditor, CMap *pMap);

	void LightDebug(CEditor *pEditor);
	CLightManager *LightIncreDecreDebug(CLightManager *pLightManager);
	void LightInfoDebug(CLightManager *pLightManager);
	void LightInfo(CLightManager *pLightManager, int nCntLight);
	void DirectionalLightInfoDebug(CDirectionalLight *pDirLight);
	void PointLightInfoDebug(CPointLight *pPointLight);
	void SpotLightInfoDebug(CSpotLight *pSpotLight);

	void GameFieldDebug(CEditor *pEditor);
	void BlockDebug(CEditor *pEditor);
	void RiverDebug(CEditor *pEditor);
	void IceDebug(CEditor *pEditor);

	void ObjectDebug(CEditor *pEditor);
	void ObjModelDebug(CEditor *pEditor);
	void ObjBillboardDebug(CEditor *pEditor);
	void ObjEffectDebug(CEditor *pEditor);

	void RespawnDebug(CEditor *pEditor);
	void RespawnInfoDebug(CRespawn *pRespawn);

	void HeadQuartersDebug(CEditor *pEditor);

	void DemoplayDebug(void);


	//マップ情報用変数
	int m_nSaveMapCounter;                // マップ情報保存成否情報表示用カウンター
	bool m_bSaveMapDisp;                  // マップ情報保存成否情報を表示するかどうか
	int m_nLoadMapCounter;                // マップ情報読み込み成否情報表示用カウンター
	bool m_bLoadMapDisp;                  // マップ情報読み込み成否情報を表示するかどうか

	// モデルリスト情報用変数
	int m_nSaveModelListCounter;          // モデルリスト情報保存成否情報表示用カウンター
	bool m_bSaveModelListDisp;            // モデルリスト情報保存成否情報を表示するかどうか
	int m_nLoadModelListCounter;          // モデルリスト情報読み込み成否情報表示用カウンター
	bool m_bLoadModelListDisp;            // モデルリスト情報読み込み成否情報を表示するかどうか

	// テクスチャリスト情報用変数
	int m_nSaveTexListCounter;            // テクスチャリスト情報保存成否情報表示用カウンター
	bool m_bSaveTexListDisp;              // テクスチャリスト情報保存成否情報を表示するかどうか
	int m_nLoadTexListCounter;            // テクスチャリスト情報読み込み成否情報表示用カウンター
	bool m_bLoadTexListDisp;              // テクスチャリスト情報読み込み成否情報を表示するかどうか

	// ライト情報用変数
	int m_nLightIdx;                      // ライトの情報を操作する番号
	int m_nSaveLightCounter;              // ライト情報保存成否情報表示用カウンター
	bool m_bSaveLightDisp;                // ライト情報保存成否情報を表示するかどうか
	int m_nLoadLightCounter;              // ライト情報読み込み成否情報表示用カウンター
	bool m_bLoadLightDisp;                // ライト情報読み込み成否情報を表示するかどうか

	// ゲームフィールド用変数
	int m_nSaveGameFieldCounter;          // ゲームフィールド情報保存成否情報表示用カウンター
	bool m_bSaveGameFieldDisp;            // ゲームフィールド情報保存成否情報を表示するかどうか
	int m_nLoadGameFieldCounter;          // ゲームフィールド情報読み込み成否情報表示用カウンター
	bool m_bLoadGameFieldDisp;            // ゲームフィールド情報読み込み成否情報を表示するかどうか

	// 配置物情報用変数
	int m_nSaveObjectCounter;             // 配置物情報保存成否情報表示用カウンター
	bool m_bSaveObjectDisp;               // 配置物情報保存成否情報を表示するかどうか
	int m_nLoadObjectCounter;             // 配置物情報読み込み成否情報表示用カウンター
	bool m_bLoadObjectDisp;               // 配置物情報読み込み成否情報を表示するかどうか

	// リスポーン用変数
	int m_nPlayerResIdx;
	int m_nEnemyResIdx;
};
#endif