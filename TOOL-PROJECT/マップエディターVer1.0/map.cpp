//*****************************************************************************
//
//     マップの処理[map.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "map.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "modelcreate.h"
#include "light.h"
#include "lightManager.h"
#include "block.h"
#include "boxCollider.h"
#include "scene.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
// オートセーブ用マクロ
#define AUTOSAVE                                                // オートセーブをするかどうか(宣言時 : オートセーブ)
#define AUTOSAVEFILENAME_MAP       "autosave_map.txt"           // マップ情報オートセーブ用のファイル名
#define AUTOSAVEFILENAME_GAMEFIELD "autosave_gamefield.txt"     // ゲームフィールド情報オートセーブ用のファイル名
#define AUTOSAVEFILENAME_MODELLIST "autosave_modellist.txt"     // モデルリスト情報オートセーブ用のファイル名
#define AUTOSAVEFILENAME_TEXLIST   "autosave_texlist.txt"       // テクスチャリスト情報オートセーブ用のファイル名
#define AUTOSAVEFILENAME_LIGHT     "autosave_light.txt"         // ライト情報オートセーブ用のファイル名
#define AUTOSAVEFILENAME_OBJECT    "autosave_object.txt"        // 配置物情報オートセーブ用のファイル名

// スクリプト読み込み用
#define BASE_POS "BASE_POS = "
#define PLAYER_RESPAWN "PLAYER_RESPAWN = "
#define ENEMY_RESPAWN "ENEMY_RESPAWN = "
#define MODELLIST_FILENAME "MODELLIST_FILENAME = "
#define TEXLIST_FILENAME "TEXLIST_FILENAME = "
#define MAPSET "MAPSET"
#define END_MAPSET "END_MAPSET"
#define LIGHT_FILENAME "LIGHT_FILENAME = "
#define GAMEFILED_FILENAME "GAMEFILED_FILENAME = "
#define OBJECT_FILENAME "OBJECT_FILENAME = "

// 共通情報
#define POS "POS = "
#define ROT "ROT = "

// モデルリスト情報
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// テクスチャリスト情報
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// ライト情報
#define NUM_LIGHT "NUM_LIGHT = "
#define DIRECTIONALLIGHTSET "DIRECTIONALLIGHTSET"
#define END_DIRECTIONALLIGHTSET "END_DIRECTIONALLIGHTSET"
#define POINTLIGHTSET "POINTLIGHTSET"
#define END_POINTLIGHTSET "END_POINTLIGHTSET"
#define SPOTLIGHTSET "SPOTLIGHTSET"
#define END_SPOTLIGHTSET "END_SPOTLIGHTSET"
#define DIFFUSE "DIFFUSE = "
#define AMBIENT "AMBIENT = "
#define SPECULAR "SPECULAR = "
#define ATTENUATION1 "ATTENUATION1 = "
#define ATTENUATION2 "ATTENUATION2 = "
#define ATTENUATION3 "ATTENUATION3 = "
#define RANGE "RANGE = "
#define FALLOFF "FALLOFF = "
#define THETA "THETA = "
#define PHI "PHI = "
#define DIR "DIR = "

// ブロック情報
#define BLOCKSET "BLOCKSET"
#define END_BLOCKSET "END_BLOCKSET"
#define BLOCKTYPE "TYPE = "
#define MODELIDX "MODELIDX = "
#define BREAK "BREAK = "
#define COLLISION "COLLISION"
#define END_COLLISION "END_COLLISION"
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "
#define DEPTH "DEPTH = "

// 川情報
#define RIVERSET "RIVERSET"
#define END_RIVERSET "END_RIVERSET"

// 氷情報
#define ICESET "ICESET"
#define END_ICESET "END_ICESET"

// 配置モデル情報
#define MODELSET "MODELSET"
#define END_MODELSET "END_MODELSET"

// 配置ビルボード情報
#define BILLBOARDSET "BILLBOARDSET"
#define END_BILLBOARDSET "END_BILLBOARDSET"

// 配置エフェクト情報
#define EFFECTSET "EFFECTSET"
#define END_EFFECTSET "END_EFFECTSET"

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CMap::CMap()
{
	strcpy(m_aFileName, "\0");             // 読み込むマップデータのファイル名
	strcpy(m_aModelListFileName, "\0");    // 読み込むモデルリスト情報のファイル名
	strcpy(m_aTexListFileName, "\0");      // 読み込むテクスチャリスト情報のファイル名
	strcpy(m_aLightFileName, "\0");        // 読み込むライト情報のファイル名
	strcpy(m_aGameFieldFileName, "\0");    // 読み込むゲームフィールド情報のファイル名
	strcpy(m_aObjectFileName, "\0");       // 読み込む配置物情報のファイル名
	m_pTextureManager = NULL;              // テクスチャ管轄クラスへのポインタ
	m_pModelCreate = NULL;                 // モデル管轄クラスへのポインタ
	m_pLightManager = NULL;                // ライト管轄クラスへのポインタ

}

//=============================================================================
//    デストラクタ
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMap *CMap::Create(char *pFileName)
{
	CMap *pMap = NULL;  // マップクラス型のポインタ
	if (pMap == NULL)
	{// メモリが空になっている
		pMap = new CMap;
		if (pMap != NULL)
		{// インスタンスを生成できた
			if (FAILED(pMap->Init(pFileName)))
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

	return pMap;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMap::Init(char *pFileName)
{
	// ファイル名をコピーする
	strcpy(m_aFileName, pFileName);

	// マップデータを読み込む
	if (FAILED(Load(m_aFileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMap::Uninit(void)
{
#ifdef AUTOSAVE
	// オートセーブのファイル名を設定する
	strcpy(m_aGameFieldFileName, AUTOSAVEFILENAME_GAMEFIELD);
	strcpy(m_aModelListFileName, AUTOSAVEFILENAME_MODELLIST);
	strcpy(m_aTexListFileName, AUTOSAVEFILENAME_TEXLIST);
	strcpy(m_aLightFileName, AUTOSAVEFILENAME_LIGHT);
	strcpy(m_aObjectFileName, AUTOSAVEFILENAME_OBJECT);

	// データを保存する
	Save(AUTOSAVEFILENAME_MAP);
#endif

	// テクスチャ管轄クラスを開放する
	ReleaseTextureManager();

	// モデル管轄クラスを開放する
	ReleaseModelManager();

	// ライト管轄クラスを開放する
	ReleaseLightManager();
}

//=============================================================================
//    マップデータ読み込み処理
//=============================================================================
HRESULT CMap::Load(char *pLoadFileName)
{
	CFileLoader *pFileLoader = NULL;  // ファイル読み込み用クラスへのポインタ
	pFileLoader = CFileLoader::Create(pLoadFileName);
	if (pFileLoader != NULL)
	{// ファイルが読み込めた
		char aStr[256];
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図があった
			LoadScript(aStr, pFileLoader);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    マップデータ保存処理
//=============================================================================
HRESULT CMap::Save(char *pSaveFileName)
{
	char aStr[256] = SAVEFILENAME_MAP;
	CFileSaver *pFileSaver = NULL;  // ファイル保存用クラスへのポインタ
	pFileSaver = CFileSaver::Create(strcat(aStr, pSaveFileName));
	if (pFileSaver != NULL)
	{// ファイルを生成できた
	    // ファイルの冒頭分を書き込み
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# マップ情報スクリプトファイル [%s]\n", pSaveFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対消さないこと！\n", SCRIPT);
		pFileSaver->Print("\n");

		// 司令部の位置を保存
		SaveBasePos(pFileSaver);

		// プレイヤーのリスポーン位置を保存
		SavePlayerRespawn(pFileSaver);

		// 敵のリスポーン位置を保存
		SaveEnemyRespawn(pFileSaver);

		// モデル情報のスクリプトファイル名を保存
		SaveModelList(pFileSaver);

		// テクスチャ情報のスクリプトファイル名を保存
		SaveTexList(pFileSaver);

		// マップ情報を保存
		SaveMap(pFileSaver);

		// スクリプト終了の合図を書き込み
		pFileSaver->Print("%s		# この行は絶対消さないこと！\n", END_SCRIPT);

		// メモリの開放
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}

		// ゲームフィールド情報を保存
		SaveGameField(m_aGameFieldFileName);

		// モデルリスト情報を保存
		SaveModel(m_aModelListFileName);

		// テクスチャリスト情報を保存
		SaveTexture(m_aTexListFileName);

		// ライト情報を保存
		SaveLight(m_aLightFileName);

		// 配置物情報を保存
		SaveObject(m_aObjectFileName);
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    マップ切り替え処理
//=============================================================================
HRESULT CMap::ChangeMap(char *pLoadFileName)
{
	// 生成されているオブジェクトをすべて破棄する
	DeleteMap();

	// 新たにマップデータを読み込む
	strcpy(m_aFileName, pLoadFileName);       // ファイル名コピー
	if (FAILED(Load(pLoadFileName)))
	{
		return E_FAIL;
	}

	return S_OK;
}


//=============================================================================
//    スクリプト情報読み込み処理
//=============================================================================
HRESULT CMap::LoadScript(char *pStr, CFileLoader *pFileLoader)
{
	int nCntLoadPlayerRes = 0;  // プレイヤーのリスポーン位置を読み込んだ回数
	int nCntLoadEnemyRes = 0;   // 敵のリスポーン位置を読み込んだ回数
	char aStr[256];
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, BASE_POS) == 0)
		{// 基地の位置情報だった
			LoadBasePos(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYER_RESPAWN) == 0)
		{// プレイヤーのリスポーン位置情報だった
			LoadPlayerRespawn(pStr, nCntLoadPlayerRes);
			nCntLoadPlayerRes++;
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMY_RESPAWN) == 0)
		{// 敵のリスポーン位置情報だった
			LoadEnemyRespawn(pStr, nCntLoadEnemyRes);
			nCntLoadEnemyRes++;
		}
		else if (CFunctionLib::Memcmp(pStr, MODELLIST_FILENAME) == 0)
		{// モデルリスト情報だった
			LoadModelList(CFunctionLib::ReadString(pStr, aStr, MODELLIST_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXLIST_FILENAME) == 0)
		{// テクスチャリスト情報だった
			LoadTextureList(CFunctionLib::ReadString(pStr, aStr, TEXLIST_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, MAPSET) == 0)
		{// マップセット情報だった
			LoadMap(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図があった
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    基地の位置読み込み処理
//=============================================================================
HRESULT CMap::LoadBasePos(char *pStr)
{
	m_BasePos = CFunctionLib::ReadVector3(pStr, BASE_POS);

	return S_OK;
}

//=============================================================================
//    プレイヤーのリスポーン位置(2人分)読み込み処理
//=============================================================================
HRESULT CMap::LoadPlayerRespawn(char *pStr, int nCntPlayerRes)
{
	m_PlayerRespawn[nCntPlayerRes] = CFunctionLib::ReadVector3(pStr, PLAYER_RESPAWN);

	return S_OK;
}

//=============================================================================
//    敵のリスポーン位置(3箇所分)読み込み処理
//=============================================================================
HRESULT CMap::LoadEnemyRespawn(char *pStr, int nCntEnemyRes)
{
	// 敵のリスポーン位置を読み込み
	m_EnemyRespawn[nCntEnemyRes] = CFunctionLib::ReadVector3(pStr, ENEMY_RESPAWN);

	return S_OK;
}

//=============================================================================
//    モデルリスト読み込み処理
//=============================================================================
HRESULT CMap::LoadModelList(char *pModelFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // ファイル読み込み用クラスへのポインタ
	pFileLoader = CFileLoader::Create(pModelFileName);
	if (pFileLoader != NULL)
	{// ファイルが読み込めた
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0 && m_pModelCreate == NULL)
		{// スクリプト読み込み開始の合図があった
			LoadModel(pStr, pFileLoader);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// ファイル名をコピーしておく
		strcpy(m_aModelListFileName, pModelFileName);
	}
	return S_OK;
}

//=============================================================================
//    モデル読み込み処理
//=============================================================================
HRESULT CMap::LoadModel(char *pStr, CFileLoader *pFileLoader)
{
	// モデル読み込み用変数宣言
	int nCntModel = 0;             // 現在読み込んだモデル数
	LPD3DXMESH pMesh = NULL;       // メッシュへのポインタ
	LPD3DXBUFFER pBuffMat = NULL;  // マテリアル情報へのポインタ
	DWORD nNumMat = 0;             // マテリアル情報の数

	while (1)
	{// 抜けるまでループ
		char aStr[256];
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{// 読み込むモデル数情報があった
			int nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			m_pModelCreate = CModelCreate::Create(nNumModel);
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// 読み込むモデルのファイル名情報があった
		    // モデルのファイルパス名を読み取る
			pStr = CFunctionLib::ReadString(pStr, aStr, MODEL_FILENAME);

			// xファイルの読み込み
			D3DXLoadMeshFromX(pStr, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
				&pBuffMat, NULL, &nNumMat, &pMesh);

			// モデル管轄クラスにポインタを設定する
			m_pModelCreate->SetMesh(pMesh, nCntModel);
			m_pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

			// ファイル名を設定する
			m_pModelCreate->SetFileName(pStr, nCntModel);

			// 使用した変数を初期化しておく
			pMesh = NULL;
			pBuffMat = NULL;
			nNumMat = 0;

			// カウンターを増やす
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図があった
			break;
		}
	}
	return S_OK;
}

//=============================================================================
//    テクスチャリスト読み込み処理
//=============================================================================
HRESULT CMap::LoadTextureList(char *pTextureFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // ファイル読み込み用クラスへのポインタ
	pFileLoader = CFileLoader::Create(pTextureFileName);
	if (pFileLoader != NULL)
	{// ファイルが読み込めた
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0 && m_pTextureManager == NULL)
		{// スクリプト読み込み開始の合図があった
			LoadTexture(pStr, pFileLoader);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// ファイル名をコピーしておく
		strcpy(m_aTexListFileName, pTextureFileName);
	}
	return S_OK;
}

//=============================================================================
//    テクスチャ読み込み処理
//=============================================================================
HRESULT CMap::LoadTexture(char *pStr, CFileLoader *pFileLoader)
{
	// テクスチャ読み込み用変数宣言
	int nCntTex = 0;                      // 現在読み込んだモデル数
	LPDIRECT3DTEXTURE9 pTexture = NULL;   // テクスチャへのポインタ

	while (1)
	{// 抜けるまでループ
		char aStr[256];
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// 読み込むテクスチャ数情報があった
			int nNumTex = CFunctionLib::ReadInt(pStr, NUM_TEXTURE);
			m_pTextureManager = CTextureManager::Create(nNumTex);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// 読み込むテクスチャのファイル名情報があった
		    // テクスチャのファイルパス名を読み取る
			pStr = CFunctionLib::ReadString(pStr, aStr, TEXTURE_FILENAME);

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);

			// テクスチャ管轄クラスにポインタを設定する
			m_pTextureManager->SetTexture(pTexture, nCntTex);

			// ファイル名を設定する
			m_pTextureManager->SetFileName(pStr, nCntTex);

			// 使用した変数を初期化しておく
			pTexture = NULL;

			// カウンターを増やす
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図があった
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    マップ情報読み込み処処理
//=============================================================================
HRESULT CMap::LoadMap(char *pStr, CFileLoader *pFileLoader)
{
	while (1)
	{// 抜けるまでループ
		char aStr[256];
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, LIGHT_FILENAME) == 0)
		{// ライト情報だった
			LoadLight(CFunctionLib::ReadString(pStr, aStr, LIGHT_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEFILED_FILENAME) == 0)
		{// ゲームフィールド情報だった
			LoadGameField(CFunctionLib::ReadString(pStr, aStr, GAMEFILED_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, OBJECT_FILENAME) == 0)
		{// 配置物情報だった
			LoadObject(CFunctionLib::ReadString(pStr, aStr, OBJECT_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_MAPSET) == 0)
		{// マップセット情報終了の合図があった
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    ライト情報読み込み処理
//=============================================================================
HRESULT CMap::LoadLight(char *pLightFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // ファイル読み込み用クラスへのポインタ
	pFileLoader = CFileLoader::Create(pLightFileName);
	if (pFileLoader != NULL)
	{// ファイルが読み込めた
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図があった
			LoadLightInfo(pStr, pFileLoader);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// ファイル名をコピーしておく
		strcpy(m_aLightFileName, pLightFileName);
	}

	return S_OK;
}

//=============================================================================
//    ライト読み込み処理
//=============================================================================
HRESULT CMap::LoadLightInfo(char *pStr, CFileLoader *pFileLoader)
{
	int nCntLight = 0;

	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_LIGHT) == 0)
		{// ライトの数情報だった
			int nNumLight = CFunctionLib::ReadInt(pStr, NUM_LIGHT);
			m_pLightManager = CLightManager::Create(nNumLight);
		}
		else if (CFunctionLib::Memcmp(pStr, DIRECTIONALLIGHTSET) == 0)
		{// ディレクショナルライト情報だった
			LoadDirectionalLight(pStr, pFileLoader, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, POINTLIGHTSET) == 0)
		{// ポイントライト情報だった
			LoadPointLight(pStr, pFileLoader, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, SPOTLIGHTSET) == 0)
		{// スポットライト情報だった
			LoadSpotLight(pStr, pFileLoader, nCntLight);
			nCntLight++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト終了の合図があった
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    ディレクショナルライト読み込み処理
//=============================================================================
HRESULT CMap::LoadDirectionalLight(char *pStr, CFileLoader *pFileLoader, int nCntLight)
{
	D3DXVECTOR3 LightDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// ライトの拡散光情報だった
			LightDiffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// ライトの環境光情報だった
			LightAmbient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// ライトの反射光情報だった
			LightSpecular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, DIR) == 0)
		{// ライトの向き情報だった
			LightDir = CFunctionLib::ReadVector3(pStr, DIR);
		}
		else if (CFunctionLib::Memcmp(pStr, END_DIRECTIONALLIGHTSET) == 0)
		{// ディレクショナルライト情報終了の合図があった
			CDirectionalLight *pDirLight = CDirectionalLight::Create(LightDir, LightDiffuse,
				                                                     LightAmbient, LightSpecular);
			m_pLightManager->SettingLight(pDirLight, nCntLight);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    ポイントライト読み込み処理
//=============================================================================
HRESULT CMap::LoadPointLight(char *pStr, CFileLoader *pFileLoader, int nCntLight)
{
	D3DXVECTOR3 LightPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	float fLightAttenuation0 = 0.0f;
	float fLightAttenuation1 = 0.0f;
	float fLightAttenuation2 = 0.0f;
	float fLightRange = 0.0f;

	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ライトの座標情報だった
			LightPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// ライトの拡散光情報だった
			LightDiffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// ライトの環境光情報だった
			LightAmbient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// ライトの反射光情報だった
			LightSpecular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION1) == 0)
		{// ライトの線形減衰係数情報だった
			fLightAttenuation0 = CFunctionLib::ReadFloat(pStr, ATTENUATION1);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION2) == 0)
		{// ライトの2次減衰係数情報だった
			fLightAttenuation1 = CFunctionLib::ReadFloat(pStr, ATTENUATION2);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION3) == 0)
		{// ライトの指数減衰係数情報だった
			fLightAttenuation2 = CFunctionLib::ReadFloat(pStr, ATTENUATION3);
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE) == 0)
		{// ライトの範囲情報だった
			fLightRange = CFunctionLib::ReadFloat(pStr, RANGE);
		}
		else if (CFunctionLib::Memcmp(pStr, END_POINTLIGHTSET) == 0)
		{// ポイントライト情報終了の合図があった
			CPointLight *pPointLight = CPointLight::Create(LightPos, LightDiffuse, LightAmbient, LightSpecular,
				                                           fLightAttenuation0, fLightAttenuation1,
				                                           fLightAttenuation2, fLightRange);
			m_pLightManager->SettingLight(pPointLight, nCntLight);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    スポットライト読み込み処理
//=============================================================================
HRESULT CMap::LoadSpotLight(char *pStr, CFileLoader *pFileLoader, int nCntLight)
{
	D3DXVECTOR3 LightPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 LightDir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightAmbient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	D3DXCOLOR LightSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	float fLightAttenuation0 = 0.0f;
	float fLightAttenuation1 = 0.0f;
	float fLightAttenuation2 = 0.0f;
	float fLightRange = 0.0f;
	float fLightFalloff = 0.0f;
	float fLightTheta = 0.0f;
	float fLightPhi = 0.0f;

	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ライトの座標情報だった
			LightPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, DIR) == 0)
		{// ライトの向き情報だった
			LightDir = CFunctionLib::ReadVector3(pStr, DIR);
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSE) == 0)
		{// ライトの拡散光情報だった
			LightDiffuse = CFunctionLib::ReadVector4(pStr, DIFFUSE);
		}
		else if (CFunctionLib::Memcmp(pStr, AMBIENT) == 0)
		{// ライトの環境光情報だった
			LightAmbient = CFunctionLib::ReadVector4(pStr, AMBIENT);
		}
		else if (CFunctionLib::Memcmp(pStr, SPECULAR) == 0)
		{// ライトの反射光情報だった
			LightSpecular = CFunctionLib::ReadVector4(pStr, SPECULAR);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION1) == 0)
		{// ライトの線形減衰係数情報だった
			fLightAttenuation0 = CFunctionLib::ReadFloat(pStr, ATTENUATION1);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION2) == 0)
		{// ライトの2次減衰係数情報だった
			fLightAttenuation1 = CFunctionLib::ReadFloat(pStr, ATTENUATION2);
		}
		else if (CFunctionLib::Memcmp(pStr, ATTENUATION3) == 0)
		{// ライトの指数減衰係数情報だった
			fLightAttenuation2 = CFunctionLib::ReadFloat(pStr, ATTENUATION3);
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE) == 0)
		{// ライトの範囲情報だった
			fLightRange = CFunctionLib::ReadFloat(pStr, RANGE);
		}
		else if (CFunctionLib::Memcmp(pStr, FALLOFF) == 0)
		{// ライトのフォールオフ情報だった
			fLightFalloff = CFunctionLib::ReadFloat(pStr, FALLOFF);
		}
		else if (CFunctionLib::Memcmp(pStr, THETA) == 0)
		{// ライトの内部コーンの角度情報だった
			fLightTheta = CFunctionLib::ReadFloat(pStr, THETA);
			D3DXToRadian(fLightTheta);
		}
		else if (CFunctionLib::Memcmp(pStr, PHI) == 0)
		{// ライトの外部コーンの角度情報だった
			fLightPhi = CFunctionLib::ReadFloat(pStr, PHI);
			D3DXToRadian(fLightPhi);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SPOTLIGHTSET) == 0)
		{// スポットライト情報終了の合図があった
			CSpotLight *pSpotLight = CSpotLight::Create(LightPos, LightDir, LightDiffuse, LightAmbient,
				                                        LightSpecular, fLightAttenuation0, fLightAttenuation1,
				                                        fLightAttenuation2, fLightRange,
				                                        fLightFalloff, fLightTheta, fLightPhi);
			m_pLightManager->SettingLight(pSpotLight, nCntLight);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    ゲームフィールド情報読み込み処理
//=============================================================================
HRESULT CMap::LoadGameField(char *pGameFieldFileName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // ファイル読み込み用クラスへのポインタ
	pFileLoader = CFileLoader::Create(pGameFieldFileName);
	if (pFileLoader != NULL)
	{// ファイルが読み込めた
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図があった
			LoadGameFieldInfo(pStr, pFileLoader);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// ファイル名をコピーしておく
		strcpy(m_aGameFieldFileName, pGameFieldFileName);
	}

	return S_OK;
}

//=============================================================================
//    ゲームフィールド読み込み処理
//=============================================================================
HRESULT CMap::LoadGameFieldInfo(char *pStr, CFileLoader *pFileLoader)
{
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, BLOCKSET) == 0)
		{// ブロック情報だった
			LoadBlock(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト終了の合図があった
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    ブロック情報読み込み処理
//=============================================================================
void CMap::LoadBlock(char *pStr, CFileLoader *pFileLoader)
{
	int nBlockType = 0;
	int nBlockModelIdx = 0;
	D3DXVECTOR3 BlockPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 BlockRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bBlockBreak = true;
	float fBlockColWidth = 0.0f;
	float fBlockColHeight = 0.0f;
	float fBlockColDepth = 0.0f;

	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, BLOCKTYPE) == 0)
		{// 種類番号情報だった
			nBlockModelIdx = CFunctionLib::ReadInt(pStr, BLOCKTYPE);
		}
		else if (CFunctionLib::Memcmp(pStr, MODELIDX) == 0)
		{// 使用するモデルの番号情報だった
			nBlockModelIdx = CFunctionLib::ReadInt(pStr, MODELIDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// 座標情報だった
			BlockPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// 向き情報だった
			BlockRot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, BREAK) == 0)
		{// 壊せるかどうか情報だった
			bBlockBreak = CFunctionLib::ReadBool(pStr, BREAK);
		}
		else if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// 当たり判定情報情報だった
			LoadCollision(pStr, pFileLoader, &fBlockColWidth, &fBlockColHeight, &fBlockColDepth);
		}
		else if (CFunctionLib::Memcmp(pStr, END_BLOCKSET) == 0)
		{// ブロック情報終了の合図があった
			CBlock::Create(BlockPos, BlockRot, (CBlock::TYPE)nBlockType, nBlockModelIdx, bBlockBreak,
				m_pModelCreate->GetMesh(nBlockModelIdx), m_pModelCreate->GetBuffMat(nBlockModelIdx),
				m_pModelCreate->GetNumMat(nBlockModelIdx), m_pModelCreate->GetTexture(nBlockModelIdx),
				fBlockColWidth, fBlockColHeight, fBlockColDepth);
			break;
		}
	}
}

//=============================================================================
//    当たり判定情報読み込み処理
//=============================================================================
void CMap::LoadCollision(char *pStr, CFileLoader *pFileLoader, float *pWidth, float *pHeight, float *pDepth)
{
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// 当たり判定の幅情報だった
			*pWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// 当たり判定の高さ情報だった
			*pHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, DEPTH) == 0)
		{// 当たり判定の奥行情報だった
			*pDepth = CFunctionLib::ReadFloat(pStr, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStr, END_COLLISION) == 0)
		{// ブロック情報終了の合図があった
			break;
		}
	}
}

//=============================================================================
//    氷情報読み込み処理
//=============================================================================
void CMap::LoadIce(char *pStr, CFileLoader *pFileLoader)
{

}

//=============================================================================
//    川情報読み込み処理
//=============================================================================
void CMap::LoadRiver(char *pStr, CFileLoader *pFileLoader)
{

}

//=============================================================================
//    配置物情報読み込み処理
//=============================================================================
HRESULT CMap::LoadObject(char *pObjectName, char *pStr)
{
	CFileLoader *pFileLoader = NULL;  // ファイル読み込み用クラスへのポインタ
	pFileLoader = CFileLoader::Create(pObjectName);
	if (pFileLoader != NULL)
	{// ファイルが読み込めた
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// スクリプト読み込み開始の合図があった
			LoadObjectInfo(pStr, pFileLoader);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}

		// ファイル名をコピーしておく
		strcpy(m_aObjectFileName, pObjectName);
	}

	return S_OK;
}

//=============================================================================
//    配置物読み込み処理
//=============================================================================
HRESULT CMap::LoadObjectInfo(char *pStr, CFileLoader *pFileLoader)
{
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト情報読み込み終了の合図があった
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    配置モデル情報読み込み処理
//=============================================================================
void CMap::LoadObjModel(char *pStr, CFileLoader *pFileLoader)
{

}

//=============================================================================
//    配置ビルボード情報読み込み処理
//=============================================================================
void CMap::LoadObjBill(char *pStr, CFileLoader *pFileLoader)
{

}

//=============================================================================
//    配置エフェクト情報読み込み処理
//=============================================================================
void CMap::LoadObjEffect(char *pStr, CFileLoader *pFileLoader)
{

}

//=============================================================================
//    基地の位置情報保存処理
//=============================================================================
HRESULT CMap::SaveBasePos(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 司令部の位置\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 司令部の位置を書き込み
	pFileSaver->Print("%s%.1f %.1f %.1f\n\n", BASE_POS, m_BasePos.x, m_BasePos.y, m_BasePos.z);

	return S_OK;
}

//=============================================================================
//    プレイヤーのリスポーン位置情報保存処理
//=============================================================================
HRESULT CMap::SavePlayerRespawn(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# プレイヤーのリスポーン位置\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// プレイヤーのリスポーン位置を書き込み
	for (int nCntRes = 0; nCntRes < MAX_PLAYER_RESPAWN; nCntRes++)
	{
		pFileSaver->Print("%s%.1f %.1f %.1f\n", PLAYER_RESPAWN, m_PlayerRespawn[nCntRes].x, m_PlayerRespawn[nCntRes].y, m_PlayerRespawn[nCntRes].z);
	}
	pFileSaver->Print("\n");

	return S_OK;
}

//=============================================================================
//    敵のリスポーン位置情報保存処理
//=============================================================================
HRESULT CMap::SaveEnemyRespawn(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 敵のリスポーン位置\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 敵のリスポーン位置を書き込み
	for (int nCntRes = 0; nCntRes < MAX_ENEMY_RESPAWN; nCntRes++)
	{
		pFileSaver->Print("%s%.1f %.1f %.1f\n", ENEMY_RESPAWN, m_EnemyRespawn[nCntRes].x, m_EnemyRespawn[nCntRes].y, m_EnemyRespawn[nCntRes].z);
	}
	pFileSaver->Print("\n");

	return S_OK;
}

//=============================================================================
//    モデルリストのファイル名情報保存処理
//=============================================================================
HRESULT CMap::SaveModelList(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# モデル情報のスクリプトファイル名\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// モデル情報のスクリプトファイル名を書き込み
	char aSaveFileName[256];
	strcpy(aSaveFileName, SAVEFILENAME_MODELLIST);
	pFileSaver->Print("%s%s\n\n", MODELLIST_FILENAME, strcat(aSaveFileName, m_aModelListFileName));

	return S_OK;
}

//=============================================================================
//    テクスチャリストのファイル名情報保存処理
//=============================================================================
HRESULT CMap::SaveTexList(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# テクスチャ情報のスクリプトファイル名\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// テクスチャ情報のスクリプトファイル名を書き込み
	char aSaveFileName[256];
	strcpy(aSaveFileName, SAVEFILENAME_TEXLIST);
	pFileSaver->Print("%s%s\n\n", TEXLIST_FILENAME, strcat(aSaveFileName, m_aTexListFileName));

	return S_OK;
}

//=============================================================================
//    マップセット情報保存処理
//=============================================================================
HRESULT CMap::SaveMap(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# マップ情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	char aSaveLightFileName[256];
	char aSaveGameFieldFileName[256];
	char aSaveObjectFileName[256];
	strcpy(aSaveLightFileName, SAVEFILENAME_LIGHT);
	strcpy(aSaveGameFieldFileName, SAVEFILENAME_GAMEFIELD);
	strcpy(aSaveObjectFileName, SAVEFILENAME_OBJECT);
	pFileSaver->Print("%s\n", MAPSET);      // マップセット情報読み込み開始の合図を書き込み
	pFileSaver->Print("	%s%s					# ライト情報のスクリプトファイル名\n", LIGHT_FILENAME, strcat(aSaveLightFileName, m_aLightFileName));
	pFileSaver->Print("	%s%s		# ゲームフィールド情報のスクリプトファイル名\n", GAMEFILED_FILENAME, strcat(aSaveGameFieldFileName, m_aGameFieldFileName));
	pFileSaver->Print("	%s%s				# 配置物情報のスクリプトファイル名\n", OBJECT_FILENAME, strcat(aSaveObjectFileName, m_aObjectFileName));
	pFileSaver->Print("%s\n", END_MAPSET);  // マップセット情報読み込み終了の合図を書き込み

	return S_OK;
}

//=============================================================================
//    モデル情報保存処理
//=============================================================================
HRESULT CMap::SaveModel(char *pModelFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_MODELLIST;
	CFileSaver *pFileSaver = NULL;  // ファイル保存用クラスへのポインタ
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pModelFileName));
	if (pFileSaver != NULL)
	{// ファイルが読み込めた
	    // ファイルの冒頭分を書き込み
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# モデルリスト情報スクリプトファイル [%s]\n", pModelFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対消さないこと！\n", SCRIPT);
		pFileSaver->Print("\n");

		// 読み込むモデル数を書き込み
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# 読み込むモデル数\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n\n", NUM_MODEL, m_pModelCreate->GetNumModel());

		// 読み込むモデルのファイル名を書き込み
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# 読み込むモデルのファイル名\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		for (int nCntModel = 0; nCntModel < m_pModelCreate->GetNumModel(); nCntModel++)
		{// 読み込んだモデルの数だけ繰り返し
			pFileSaver->Print("%s%s\n", MODEL_FILENAME, m_pModelCreate->GetFileName(nCntModel));
		}
		pFileSaver->Print("\n");

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

	return S_OK;
}

//=============================================================================
//    テクスチャ情報保存処理
//=============================================================================
HRESULT CMap::SaveTexture(char *pTextureFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_TEXLIST;
	CFileSaver *pFileSaver = NULL;  // ファイル保存用クラスへのポインタ
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pTextureFileName));
	if (pFileSaver != NULL)
	{// ファイルが読み込めた
	    // ファイルの冒頭分を書き込み
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# テクスチャリスト情報スクリプトファイル [%s]\n", pTextureFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対消さないこと！\n", SCRIPT);
		pFileSaver->Print("\n");

		// 読み込むテクスチャ数を書き込み
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# 読み込むテクスチャ数\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n\n", NUM_TEXTURE, m_pTextureManager->GetNumTexture());

		// 読み込むテクスチャのファイル名を書き込み
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# 読み込むテクスチャのファイル名\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		for (int nCntTex = 0; nCntTex < m_pTextureManager->GetNumTexture(); nCntTex++)
		{// 読み込んだテクスチャの数だけ繰り返し
			pFileSaver->Print("%s%s\n", TEXTURE_FILENAME, m_pTextureManager->GetFileName(nCntTex));
		}
		pFileSaver->Print("\n");

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

	return S_OK;
}

//=============================================================================
//    ライト情報保存のためのファイルオープン処理
//=============================================================================
HRESULT CMap::SaveLight(char *pLightFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_LIGHT;
	CFileSaver *pFileSaver = NULL;  // ファイル保存用クラスへのポインタ
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pLightFileName));
	if (pFileSaver != NULL)
	{// ファイルが読み込めた
	    // ファイルの冒頭分を書き込み
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# ライト情報スクリプトファイル [%s]\n", pLightFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対消さないこと！\n", SCRIPT);
		pFileSaver->Print("\n");

		// ライトの総数情報を書き込み
		int nNumLight = 0;
		if (m_pLightManager != NULL)
		{
			nNumLight = m_pLightManager->GetNumLight();
		}
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# ライトの総数\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n\n", NUM_LIGHT, nNumLight);

		// ライト情報を書き込み
		SaveLightInfo(pFileSaver);

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

	return S_OK;
}

//=============================================================================
//    ライト情報保存処理
//=============================================================================
void CMap::SaveLightInfo(CFileSaver *pFileSaver)
{
	CLight *pLight = NULL;
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ライト情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	if (m_pLightManager != NULL)
	{
		for (int nCntLight = 0; nCntLight < m_pLightManager->GetNumLight(); nCntLight++)
		{// ライトの総数分繰り返し
			pLight = m_pLightManager->GetLight(nCntLight);
			if (pLight->GetType() == CLight::TYPE_DIRECTIONAL)
			{// ディレクショナルライトだった
				SaveDirectionalLight((CDirectionalLight*)pLight, pFileSaver);
			}
			else if (pLight->GetType() == CLight::TYPE_POINT)
			{// ポイントライトだった
				SavePointLight((CPointLight*)pLight, pFileSaver);
			}
			else if (pLight->GetType() == CLight::TYPE_SPOT)
			{// スポットライトだった
				SaveSpotLight((CSpotLight*)pLight, pFileSaver);
			}
		}
	}
	pFileSaver->Print("\n");
}

//=============================================================================
//    ディレクショナルライト情報保存処理
//=============================================================================
void CMap::SaveDirectionalLight(CDirectionalLight *pDirLight, CFileSaver *pFileSaver)
{
	// 各種情報を取得
	D3DXCOLOR LightDiffuse = pDirLight->GetLight().Diffuse;
	D3DXCOLOR LightAmbient = pDirLight->GetLight().Ambient;
	D3DXCOLOR LightSpecular = pDirLight->GetLight().Specular;
	D3DXVECTOR3 LightDir = pDirLight->GetLight().Direction;

	// 各種情報をテキストファイルに保存
	pFileSaver->Print("%s\n", DIRECTIONALLIGHTSET);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ライトの拡散光\n", DIFFUSE, LightDiffuse.r, LightDiffuse.g, LightDiffuse.b, LightDiffuse.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ライトの環境光\n", AMBIENT, LightAmbient.r, LightAmbient.g, LightAmbient.b, LightAmbient.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ライトの反射光\n", SPECULAR, LightSpecular.r, LightSpecular.g, LightSpecular.b, LightSpecular.a);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ライトの向き\n", DIR, LightDir.x, LightDir.y, LightDir.z);
	pFileSaver->Print("%s\n", END_DIRECTIONALLIGHTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    ポイントライト情報保存処理
//=============================================================================
void CMap::SavePointLight(CPointLight *pPointLight, CFileSaver *pFileSaver)
{
	// 各種情報を取得
	D3DXVECTOR3 LightPos = pPointLight->GetLight().Position;
	D3DXCOLOR LightDiffuse = pPointLight->GetLight().Diffuse;
	D3DXCOLOR LightAmbient = pPointLight->GetLight().Ambient;
	D3DXCOLOR LightSpecular = pPointLight->GetLight().Specular;
	float fLightAttenuation0 = pPointLight->GetLight().Attenuation0;
	float fLightAttenuation1 = pPointLight->GetLight().Attenuation1;
	float fLightAttenuation2 = pPointLight->GetLight().Attenuation2;
	float fLightRange = pPointLight->GetLight().Range;

	// 各種情報をテキストファイルに保存
	pFileSaver->Print("%s\n", POINTLIGHTSET);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ライトの座標\n", POS, LightPos.x, LightPos.y, LightPos.z);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ライトの拡散光\n", DIFFUSE, LightDiffuse.r, LightDiffuse.g, LightDiffuse.b, LightDiffuse.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ライトの環境光\n", AMBIENT, LightAmbient.r, LightAmbient.g, LightAmbient.b, LightAmbient.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ライトの反射光\n", SPECULAR, LightSpecular.r, LightSpecular.g, LightSpecular.b, LightSpecular.a);
	pFileSaver->Print("	%s%.3f					# ライトの線形減衰係数\n", ATTENUATION1, fLightAttenuation0);
	pFileSaver->Print("	%s%.3f					# ライトの２次減衰係数\n", ATTENUATION2, fLightAttenuation1);
	pFileSaver->Print("	%s%.3f					# ライトの指数減衰係数\n", ATTENUATION3, fLightAttenuation2);
	pFileSaver->Print("	%s%.3f							# ライトの範囲\n", RANGE, fLightRange);
	pFileSaver->Print("%s\n", END_POINTLIGHTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    スポットライト情報保存処理
//=============================================================================
void CMap::SaveSpotLight(CSpotLight *pSpotLight, CFileSaver *pFileSaver)
{
	// 各種情報を取得
	D3DXVECTOR3 LightPos = pSpotLight->GetLight().Position;
	D3DXVECTOR3 LightDir = pSpotLight->GetLight().Direction;
	D3DXCOLOR LightDiffuse = pSpotLight->GetLight().Diffuse;
	D3DXCOLOR LightAmbient = pSpotLight->GetLight().Ambient;
	D3DXCOLOR LightSpecular = pSpotLight->GetLight().Specular;
	float fLightAttenuation0 = pSpotLight->GetLight().Attenuation0;
	float fLightAttenuation1 = pSpotLight->GetLight().Attenuation1;
	float fLightAttenuation2 = pSpotLight->GetLight().Attenuation2;
	float fLightRange = pSpotLight->GetLight().Range;
	float fLightFalloff = pSpotLight->GetLight().Falloff;
	float fLightTheta = pSpotLight->GetLight().Theta;
	float fLighPhi = pSpotLight->GetLight().Phi;

	// 各種情報をテキストファイルに保存
	pFileSaver->Print("%s\n", SPOTLIGHTSET);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ライトの座標\n", POS, LightPos.x, LightPos.y, LightPos.z);
	pFileSaver->Print("	%s%.2f %.2f %.2f					# ライトの向き\n", DIR, LightDir.x, LightDir.y, LightDir.z);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ライトの拡散光\n", DIFFUSE, LightDiffuse.r, LightDiffuse.g, LightDiffuse.b, LightDiffuse.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ライトの環境光\n", AMBIENT, LightAmbient.r, LightAmbient.g, LightAmbient.b, LightAmbient.a);
	pFileSaver->Print("	%s%.3f %.3f %.3f %.3f		# ライトの反射光\n", SPECULAR, LightSpecular.r, LightSpecular.g, LightSpecular.b, LightSpecular.a);
	pFileSaver->Print("	%s%.3f					# ライトの線形減衰係数\n", ATTENUATION1, fLightAttenuation0);
	pFileSaver->Print("	%s%.3f					# ライトの２次減衰係数\n", ATTENUATION2, fLightAttenuation1);
	pFileSaver->Print("	%s%.3f					# ライトの指数減衰係数\n", ATTENUATION3, fLightAttenuation2);
	pFileSaver->Print("	%s%.3f							# ライトの範囲\n", RANGE, fLightRange);
	pFileSaver->Print("	%s%.3f							# ライトのフォールオフ\n", FALLOFF, fLightFalloff);
	pFileSaver->Print("	%s%.3f							# ライトの内部コーンの角度\n", THETA, fLightTheta);
	pFileSaver->Print("	%s%.3f							# ライトの外部コーンの角度\n", PHI, fLighPhi);
	pFileSaver->Print("%s\n", END_SPOTLIGHTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    ゲームフィールド情報保存のためのファイルオープン処理
//=============================================================================
HRESULT CMap::SaveGameField(char *pGameFieldFileName)
{
	char aSaveFileName[256] = SAVEFILENAME_GAMEFIELD;
	CFileSaver *pFileSaver = NULL;  // ファイル保存用クラスへのポインタ
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pGameFieldFileName));
	if (pFileSaver != NULL)
	{// ファイルが読み込めた
	    // ファイルの冒頭分を書き込み
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# ゲームフィールド情報スクリプトファイル [%s]\n", pGameFieldFileName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対消さないこと！\n", SCRIPT);
		pFileSaver->Print("\n");

		// ゲームフィールド情報を保存する
		SaveGameFieldInfo(pFileSaver);

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

	return S_OK;
}

//=============================================================================
//    ゲームフィールド情報を保存する処理
//=============================================================================
void CMap::SaveGameFieldInfo(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ブロック配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CBlock *pBlock = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// ブロッククラスだった
				SaveBlock((CBlock*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    ブロックの情報を保存する処理
//=============================================================================
void CMap::SaveBlock(CBlock *pBlock, CFileSaver *pFileSaver)
{
	// 各種情報を取得
	int nBlockType = pBlock->GetType();
	int nBlockModelIdx = pBlock->GetModelIdx();
	D3DXVECTOR3 BlockPos = pBlock->GetPos();
	D3DXVECTOR3 BlockRot = D3DXToDegree(pBlock->GetRot());
	bool bBlockBreak = pBlock->GetBreak();
	float fBlockColWidth = pBlock->GetBoxCollider()->GetWidth();
	float fBlockColHeight = pBlock->GetBoxCollider()->GetHeight();
	float fBlockColDepth = pBlock->GetBoxCollider()->GetDepth();

	// 各種情報をテキストファイルに保存
	pFileSaver->Print("%s\n", BLOCKSET);
	pFileSaver->Print("	%s%d				# 種類\n", BLOCKTYPE, nBlockType);
	pFileSaver->Print("	%s%d			# 使用するモデルの番号\n", MODELIDX, nBlockType);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, BlockPos.x, BlockPos.y, BlockPos.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 向き\n", ROT, BlockRot.x, BlockRot.y, BlockRot.z);
	pFileSaver->Print("	%s%d				# 壊せるかどうか\n", BREAK, (int)bBlockBreak);
	pFileSaver->Print("	%s\n", COLLISION);
	pFileSaver->Print("		%s%.1f		# 当たり判定の幅\n", WIDTH, fBlockColWidth);
	pFileSaver->Print("		%s%.1f		# 当たり判定の高さ\n", HEIGHT, fBlockColHeight);
	pFileSaver->Print("		%s%.1f		# 当たり判定の奥行\n", DEPTH, fBlockColDepth);
	pFileSaver->Print("	%s\n", END_COLLISION);
	pFileSaver->Print("%s\n", END_BLOCKSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    氷情報保存処理
//=============================================================================
void CMap::SaveIce(CFileSaver *pFileSaver)
{

}

//=============================================================================
//    川情報保存処理
//=============================================================================
void CMap::SaveRiver(CFileSaver *pFileSaver)
{

}

//=============================================================================
//    配置物情報保存のためのファイルオープン処理
//=============================================================================
HRESULT CMap::SaveObject(char *pObjectName)
{
	char aSaveFileName[256] = SAVEFILENAME_OBJECT;
	CFileSaver *pFileSaver = NULL;  // ファイル保存用クラスへのポインタ
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pObjectName));
	if (pFileSaver != NULL)
	{// ファイルが読み込めた
	    // ファイルの冒頭分を書き込み
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# 配置物情報スクリプトファイル [%s]\n", pObjectName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対消さないこと！\n", SCRIPT);
		pFileSaver->Print("\n");

		// 配置物情報を書き込み
		SaveObjectInfo(pFileSaver);

		// スクリプト読み込み終了の合図を書き込み
		pFileSaver->Print("%s		# この行は絶対消さないこと！\n", END_SCRIPT);

		// メモリの開放
		if (pFileSaver != NULL)
		{
			pFileSaver->Uninit();
			delete pFileSaver;
			pFileSaver = NULL;
		}
	}
	return S_OK;
}

//=============================================================================
//    配置物情報保存処理
//=============================================================================
void CMap::SaveObjectInfo(CFileSaver *pFileSaver)
{
	// モデル配置情報を書き込み
	SaveObjModel(pFileSaver);

	// ビルボード配置情報を書き込み
	SaveObjBill(pFileSaver);

	// エフェクト配置情報を書き込み
	SaveObjEffect(pFileSaver);
}

//=============================================================================
//    配置モデル情報保存処理
//=============================================================================
void CMap::SaveObjModel(CFileSaver *pFileSaver)
{
	// 冒頭分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# モデル配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 配置モデル情報を書き込み

	pFileSaver->Print("\n");
}

//=============================================================================
//    配置ビルボード情報保存処理
//=============================================================================
void CMap::SaveObjBill(CFileSaver *pFileSaver)
{
	// 冒頭分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ビルボード配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 配置ビルボード情報を書き込み

	pFileSaver->Print("\n");
}

//=============================================================================
//    配置エフェクト情報保存処理
//=============================================================================
void CMap::SaveObjEffect(CFileSaver *pFileSaver)
{
	// 冒頭分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# エフェクト配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 配置エフェクト情報を書き込み

	pFileSaver->Print("\n");
}

//=============================================================================
//    マップを破棄する処理
//=============================================================================
void CMap::DeleteMap(void)
{
	// ライトを破棄
	DeleteLight();

	// ゲームフィールドオブジェクトを破棄
	DeleteGameField();
}

//=============================================================================
//    ライトを破棄する処理
//=============================================================================
void CMap::DeleteLight(void)
{
	if (m_pLightManager != NULL)
	{
		// 描画デバイスのライトをOFFにする
		for (int nCntLight = 0; nCntLight < m_pLightManager->GetNumLight(); nCntLight++)
		{
			CManager::GetRenderer()->GetDevice()->LightEnable(nCntLight, false);
		}
	}

	// ライト管轄クラスを破棄する
	ReleaseLightManager();
}

//=============================================================================
//    ゲームフィールドに必要なオブジェクトを破棄する処理
//=============================================================================
void CMap::DeleteGameField(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CBlock *pBlock = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// ブロッククラスだった
				DeleteBlock((CBlock*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    ブロックを破棄する処理
//=============================================================================
void CMap::DeleteBlock(CBlock *pBlock)
{
	pBlock->Uninit();
	pBlock = NULL;
}

//=============================================================================
//    配置物に必要なオブジェクトを破棄する処理
//=============================================================================
void CMap::DeleteObject(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CBlock *pBlock = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			//if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			//{// ブロッククラスだった
			//	DeleteBlock((CBlock*)pScene);
			//}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    配置モデルを破棄する処理
//=============================================================================
void CMap::DeleteObjModel(void)
{

}

//=============================================================================
//    配置ビルボードを破棄する処理
//=============================================================================
void CMap::DeleteObjBillboard(void)
{

}

//=============================================================================
//    配置エフェクトを破棄する処理
//=============================================================================
void CMap::DeleteObjEffect(void)
{

}

//=============================================================================
//    テクスチャ管轄クラスを開放する処理
//=============================================================================
void CMap::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    モデル管轄クラスを開放する処理
//=============================================================================
void CMap::ReleaseModelManager(void)
{
	if (m_pModelCreate != NULL)
	{
		m_pModelCreate->Uninit();
		delete m_pModelCreate;
		m_pModelCreate = NULL;
	}
}

//=============================================================================
//    ライト管轄クラスを開放する処理
//=============================================================================
void CMap::ReleaseLightManager(void)
{
	if (m_pLightManager != NULL)
	{
		m_pLightManager->Uninit();
		delete m_pLightManager;
		m_pLightManager = NULL;
	}
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを取得する
//=============================================================================
CTextureManager *CMap::GetTextureManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する
//=============================================================================
CModelCreate *CMap::GetModelCreate(void)
{
	return m_pModelCreate;
}

//=============================================================================
//    ライト管轄クラスへのポインタを取得する
//=============================================================================
CLightManager *CMap::GetLightManager(void)
{
	return m_pLightManager;
}

//=============================================================================
//    読み込むモデルリスト情報のスクリプトファイル名を取得する
//=============================================================================
char *CMap::GetModelListFileName(void)
{
	return m_aModelListFileName;
}

//=============================================================================
//    読み込むテクスチャリスト情報のスクリプトファイル名を取得する
//=============================================================================
char *CMap::GetTexListFileName(void)
{
	return m_aTexListFileName;
}

//=============================================================================
//    読み込むライト情報のスクリプトファイル名を取得する
//=============================================================================
char *CMap::GetLightFileName(void)
{
	return m_aLightFileName;
}

//=============================================================================
//    読み込むゲームフィールド情報のスクリプトファイル名を取得する
//=============================================================================
char *CMap::GetGameFieldFileName(void)
{
	return m_aGameFieldFileName;
}

//=============================================================================
//    読み込む配置物情報のスクリプトファイル名を取得する
//=============================================================================
char *CMap::GetObjectFileName(void)
{
	return m_aObjectFileName;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを設定する
//=============================================================================
void CMap::SetTextureManager(CTextureManager *pTextureManager)
{
	m_pTextureManager = pTextureManager;
}

//=============================================================================
//    モデル管轄クラスへのポインタを設定する
//=============================================================================
void CMap::SetModelCreate(CModelCreate *pModelCreate)
{
	m_pModelCreate = pModelCreate;
}

//=============================================================================
//    ライト管轄クラスへのポインタを設定する
//=============================================================================
void CMap::SetLightManager(CLightManager *pLightManager)
{
	m_pLightManager = pLightManager;
}

//=============================================================================
//    読み込むモデルリスト情報のスクリプトファイル名を設定する
//=============================================================================
void CMap::SetModelListFileName(char *pFileName)
{
	strcpy(m_aModelListFileName, pFileName);
}

//=============================================================================
//    読み込むテクスチャリスト情報のスクリプトファイル名を設定する
//=============================================================================
void CMap::SetTexListFileName(char *pFileName)
{
	strcpy(m_aTexListFileName, pFileName);
}

//=============================================================================
//    読み込むライト情報のスクリプトファイル名を設定する
//=============================================================================
void CMap::SetLightFileName(char *pFileName)
{
	strcpy(m_aLightFileName, pFileName);
}

//=============================================================================
//    読み込むゲームフィールド情報のスクリプトファイル名を設定する
//=============================================================================
void CMap::SetGameFieldFileName(char *pFileName)
{
	strcpy(m_aGameFieldFileName, pFileName);
}

//=============================================================================
//    読み込む配置物情報のスクリプトファイル名を設定する
//=============================================================================
void CMap::SetObjectFileName(char *pFileName)
{
	strcpy(m_aObjectFileName, pFileName);
}