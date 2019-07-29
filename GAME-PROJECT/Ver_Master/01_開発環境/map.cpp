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
#include "characterManager.h"
#include "block.h"
#include "river.h"
#include "icefield.h"
#include "boxCollider.h"
#include "object3D.h"
#include "respawn.h"
#include "headquarters.h"
#include "meshfield.h"
#include "object.h"
#include "billboardObject.h"
#include "emitter.h"
#include "effectManager.h"
#include "basemode.h"
#include "sky.h"
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
#define AUTOSAVEFILENAME_ENEMYLIST "autosave_enemylist.txt"     // 敵の生成情報オートセーブ用のファイル名

// スクリプト読み込み用
#define BASE_POS "BASE_POS = "
#define PLAYER_RESPAWN "PLAYER_RESPAWN = "
#define ENEMY_RESPAWN "ENEMY_RESPAWN = "
#define MODELLIST_FILENAME "MODELLIST_FILENAME = "
#define TEXLIST_FILENAME "TEXLIST_FILENAME = "
#define MAPSET "MAPSET"
#define END_MAPSET "END_MAPSET"
#define LIGHT_FILENAME "LIGHT_FILENAME = "
#define GAMEFIELD_FILENAME "GAMEFIELD_FILENAME = "
#define OBJECT_FILENAME "OBJECT_FILENAME = "
#define ENEMYLIST_FILENAME "ENEMYLIST_FILENAME = "

// 共通情報
#define POS "POS = "
#define ROT "ROT = "

// モデルリスト情報
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// テクスチャリスト情報
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// 空情報
#define SKYSET "SKYSET"
#define END_SKYSET "END_SKYSET"
#define ROLLSPEED "ROLLSPEED = "
#define YBLOCK "YBLOCK = "
#define RADIUS "RADIUS = "

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

// 地面情報
#define FIELDSET "FIELDSET"
#define TEX_IDX "TEX_IDX = "
#define XBLOCK "XBLOCK = "
#define ZBLOCK "ZBLOCK = "
#define END_FIELDSET "END_FIELDSET"

// 川情報
#define RIVERSET "RIVERSET"
#define END_RIVERSET "END_RIVERSET"

// 氷情報
#define ICESET "ICESET"
#define END_ICESET "END_ICESET"

// 配置物情報
#define NUM_OBJECT_DATA "NUM_OBJECT_DATA = "
#define OBJECT_FILENAME "OBJECT_FILENAME = "

// 配置モデル情報
#define MODELSET "MODELSET"
#define END_MODELSET "END_MODELSET"

// 配置ビルボード情報
#define BILLBOARDSET "BILLBOARDSET"
#define END_BILLBOARDSET "END_BILLBOARDSET"
#define COL "COL = "
#define LIGHTING "LIGHTING = "
#define DRAW_ADDTIVE "DRAW_ADDTIVE = "

// 配置エフェクト情報
#define EFFECTSET "EFFECTSET"
#define END_EFFECTSET "END_EFFECTSET"

// 敵の生成情報
#define NUM_ENEMYLIST "NUM_ENEMYLIST = "
#define ENEMYLISTSET "ENEMYLISTSET"
#define END_ENEMYLISTSET "END_ENEMYLISTSET"
#define RESPAWN "RESPAWN = "
#define TYPE "TYPE = "
#define TIME "TIME = "
#define ITEM "ITEM"
#define END_ITEM "END_ITEM"
#define AISET "AISET"
#define END_AISET "END_AISET"
#define BULLET "BULLET = "
#define MASS "MASS = "
#define DOWN "DOWN = "

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//*****************************************************************************
//    CMapの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CMap::CMap()
{
	m_nFieldTexIdx = 0;                    // 地面に張り付けるテクスチャの番号
	strcpy(m_aFileName, "\0");             // 読み込むマップデータのファイル名
	strcpy(m_aModelListFileName, "\0");    // 読み込むモデルリスト情報のファイル名
	strcpy(m_aTexListFileName, "\0");      // 読み込むテクスチャリスト情報のファイル名
	strcpy(m_aLightFileName, "\0");        // 読み込むライト情報のファイル名
	strcpy(m_aGameFieldFileName, "\0");    // 読み込むゲームフィールド情報のファイル名
	strcpy(m_aObjectFileName, "\0");       // 読み込む配置物情報のファイル名
	strcpy(m_aEnemyListFileName, "\0");    // 読み込む敵の生成情報のファイル名
	m_pMeshField = NULL;                   // 地面クラスへのポインタ
	m_pSky = NULL;                         // 空クラスへのポインタ
	m_pTextureManager = NULL;              // テクスチャ管轄クラスへのポインタ
	m_pModelCreate = NULL;                 // モデル管轄クラスへのポインタ
	m_pLightManager = NULL;                // ライト管轄クラスへのポインタ
	m_pObjectManager = NULL;               // オブジェクトデータ管轄クラスへのポインタ
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
	strcpy(m_aEnemyListFileName, AUTOSAVEFILENAME_ENEMYLIST);

	// データを保存する
	Save(AUTOSAVEFILENAME_MAP);
#endif

	// マップを削除する
	DeleteMap();

	// テクスチャ管轄クラスの破棄
	ReleaseTextureManager();

	// モデル管轄クラスの破棄
	ReleaseModelManager();

	// 配置物管轄クラスの破棄
	ReleaseObjectManager();
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

		// モデル情報のスクリプトファイル名を保存
		SaveModelList(pFileSaver);

		// テクスチャ情報のスクリプトファイル名を保存
		SaveTexList(pFileSaver);

		// 司令部の位置を保存
		SaveHeadQuartersPos(pFileSaver);

		// プレイヤーのリスポーン位置を保存
		SavePlayerRespawn(pFileSaver);

		// 敵のリスポーン位置を保存
		SaveEnemyRespawn(pFileSaver);

		// 空情報保存
		SaveSky(pFileSaver);

		// マップ情報を保存
		SaveMap(pFileSaver);

		// スクリプト終了の合図を書き込み
		pFileSaver->Print("\n%s		# この行は絶対消さないこと！\n", END_SCRIPT);

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

		// 敵の生成情報を保存
		SaveEnemyList(m_aEnemyListFileName);
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
	// ファイルオープンできるかチェック
	CFileLoader *pFileLoader = CFileLoader::Create(pLoadFileName);
	if (pFileLoader == NULL)
	{// ファイルオープンに失敗(処理を停止)
		return E_FAIL;
	}
	else
	{// ファイルオープンに成功(メモリを開放し処理続行)
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}

	// 生成されているオブジェクトをすべて破棄する
	DeleteMap();

	// 新たにマップデータを読み込む
	m_nFieldTexIdx = 0;                       // 地面のテクスチャ番号リセット
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
	char aStr[512];
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MODELLIST_FILENAME) == 0)
		{// モデルリスト情報だった
			LoadModelList(CFunctionLib::ReadString(pStr, aStr, MODELLIST_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, TEXLIST_FILENAME) == 0)
		{// テクスチャリスト情報だった
			LoadTextureList(CFunctionLib::ReadString(pStr, aStr, TEXLIST_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, BASE_POS) == 0)
		{// 基地の位置情報だった
			LoadHeadQuartersPos(pStr);
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
		else if (CFunctionLib::Memcmp(pStr, SKYSET) == 0)
		{// 空情報だった
			LoadSky(pStr, pFileLoader);
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
//    基地の位置読み込み処理
//=============================================================================
HRESULT CMap::LoadHeadQuartersPos(char *pStr)
{
	// 司令部の位置を設定
	int nWord = 0;
	int nAreaX = CFunctionLib::ReadInt(pStr, BASE_POS);
	pStr = CFunctionLib::HeadPutout(pStr, BASE_POS);
	nWord = CFunctionLib::PopString(pStr, " ");
	pStr += nWord;
	int nAreaZ = CFunctionLib::ReadInt(pStr, "");

	// 司令部を生成
	m_pHeadQuarters = CHeadQuarters::Create(nAreaX, nAreaZ, m_pModelCreate->GetMesh(0), m_pModelCreate->GetBuffMat(0),
		m_pModelCreate->GetNumMat(0), m_pModelCreate->GetTexture(0));

	return S_OK;
}

//=============================================================================
//    プレイヤーのリスポーン位置(2人分)読み込み処理
//=============================================================================
HRESULT CMap::LoadPlayerRespawn(char *pStr, int nCntPlayerRes)
{
	// プレイヤーのリスポーン位置を設定
	int nWord = 0;
	int nAreaX = CFunctionLib::ReadInt(pStr, PLAYER_RESPAWN);
	pStr = CFunctionLib::HeadPutout(pStr, PLAYER_RESPAWN);
	nWord = CFunctionLib::PopString(pStr, " ");
	pStr += nWord;
	int nAreaZ = CFunctionLib::ReadInt(pStr, "");
	D3DXVECTOR3 Pos = CFunctionLib::ReadVector3(pStr, PLAYER_RESPAWN);
	m_pPlayerRespawn[nCntPlayerRes] = CRespawn::Create(nAreaX, nAreaZ);

	return S_OK;
}

//=============================================================================
//    敵のリスポーン位置(3箇所分)読み込み処理
//=============================================================================
HRESULT CMap::LoadEnemyRespawn(char *pStr, int nCntEnemyRes)
{
	// 敵のリスポーン位置を読み込み
	int nWord = 0;
	int nAreaX = CFunctionLib::ReadInt(pStr, ENEMY_RESPAWN);
	pStr = CFunctionLib::HeadPutout(pStr, ENEMY_RESPAWN);
	nWord = CFunctionLib::PopString(pStr, " ");
	pStr += nWord;
	int nAreaZ = CFunctionLib::ReadInt(pStr, "");
	D3DXVECTOR3 Pos = CFunctionLib::ReadVector3(pStr, ENEMY_RESPAWN);
	m_pEnemyRespawn[nCntEnemyRes] = CRespawn::Create(nAreaX, nAreaZ);

	return S_OK;
}

//=============================================================================
//    空情報読み込み処理
//=============================================================================
HRESULT CMap::LoadSky(char *pStr, CFileLoader *pFileLoader)
{
	int nSkyTexIdx = 0;
	D3DXVECTOR3 SkyPos = INITIALIZE_D3DXVECTOR3;
	D3DXVECTOR3 SkyRot = INITIALIZE_D3DXVECTOR3;
	float fSkyRollSpeed = 0.0f;
	int nSkyXBlock = 0;
	int nSkyYBlock = 0;
	float fSkyRadius = 0.0f;

	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 座標情報だった
			nSkyTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// 座標情報だった
			SkyPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// 向き情報だった
			SkyRot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, ROLLSPEED) == 0)
		{// 回転スピード情報だった
			fSkyRollSpeed = CFunctionLib::ReadFloat(pStr, ROLLSPEED);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// 横の分割数情報だった
			nSkyXBlock = CFunctionLib::ReadInt(pStr, YBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, YBLOCK) == 0)
		{// 縦の分割数情報だった
			nSkyYBlock = CFunctionLib::ReadInt(pStr, YBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, RADIUS) == 0)
		{// 半径情報だった
			fSkyRadius = CFunctionLib::ReadFloat(pStr, RADIUS);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SKYSET) == 0)
		{// 空情報終了の合図があった
			m_pSky = CSky::Create(SkyPos, SkyRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), fSkyRadius, nSkyXBlock, nSkyYBlock,
				nSkyTexIdx, fSkyRollSpeed);
			if (m_pSky != NULL) m_pSky->BindTexture(m_pTextureManager->GetTexture(nSkyTexIdx));
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
		else if (CFunctionLib::Memcmp(pStr, GAMEFIELD_FILENAME) == 0)
		{// ゲームフィールド情報だった
			LoadGameField(CFunctionLib::ReadString(pStr, aStr, GAMEFIELD_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, OBJECT_FILENAME) == 0)
		{// 配置物情報だった
			LoadObject(CFunctionLib::ReadString(pStr, aStr, OBJECT_FILENAME), pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMYLIST_FILENAME) == 0)
		{// 敵の生成情報だった
			LoadEnemyList(CFunctionLib::ReadString(pStr, aStr, ENEMYLIST_FILENAME), pStr);
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
		if (CFunctionLib::Memcmp(pStr, FIELDSET) == 0)
		{// 地面情報だった
			LoadField(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, BLOCKSET) == 0)
		{// ブロック情報だった
			LoadBlock(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, RIVERSET) == 0)
		{// 川情報だった
			LoadRiver(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, ICESET) == 0)
		{// 氷情報だった
			LoadIce(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト終了の合図があった
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    地面情報読み込み処理
//=============================================================================
void CMap::LoadField(char *pStr, CFileLoader *pFileLoader)
{
	float fFieldWidth = 0.0f;
	float fFieldDepth = 0.0f;
	int nFieldXBlock = 0;
	int nFieldZBlock = 0;
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャ番号情報だった
			m_nFieldTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// 1マス分の地面の横幅情報だった
			fFieldWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, DEPTH) == 0)
		{// 1マス分の地面の奥行情報だった
			fFieldDepth = CFunctionLib::ReadFloat(pStr, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// 横の分割数情報だった
			nFieldXBlock = CFunctionLib::ReadInt(pStr, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
		{// 奥行の分割数情報だった
			nFieldZBlock = CFunctionLib::ReadInt(pStr, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, END_FIELDSET) == 0)
		{// 地面情報終了の合図があった
			m_pMeshField = CMeshField::Create(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),fFieldWidth, fFieldDepth, nFieldXBlock, nFieldZBlock,
				1, 1, NULL, true);
			m_pMeshField->BindTexture(m_pTextureManager->GetTexture(m_nFieldTexIdx));
			break;
		}
	}
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
	float fBlockColWidth = 0.0f;
	float fBlockColHeight = 0.0f;
	float fBlockColDepth = 0.0f;

	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, BLOCKTYPE) == 0)
		{// 種類番号情報だった
			nBlockType = CFunctionLib::ReadInt(pStr, BLOCKTYPE);
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
		else if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// 当たり判定情報情報だった
			LoadCollision(pStr, pFileLoader, &fBlockColWidth, &fBlockColHeight, &fBlockColDepth);
		}
		else if (CFunctionLib::Memcmp(pStr, END_BLOCKSET) == 0)
		{// ブロック情報終了の合図があった
			CreateBlock(BlockPos, BlockRot, nBlockType, nBlockModelIdx,
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
//    川情報読み込み処理
//=============================================================================
void CMap::LoadRiver(char *pStr, CFileLoader *pFileLoader)
{
	int nRiverTexIdx = 0;
	D3DXVECTOR3 RiverPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nRiverXBlock = 0;
	int nRiverZBlock = 0;
	float fRiverColWidth = 0.0f;
	float fRiverColHeight = 0.0f;
	float fRiverColDepth = 0.0f;

	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号情報だった
			nRiverTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// 座標情報だった
			RiverPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// 横の分割数情報だった
			nRiverXBlock = CFunctionLib::ReadInt(pStr, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
		{// 奥行の分割数情報だった
			nRiverZBlock = CFunctionLib::ReadInt(pStr, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// 当たり判定情報だった
			LoadCollision(pStr, pFileLoader, &fRiverColWidth, &fRiverColHeight, &fRiverColDepth);
		}
		else if (CFunctionLib::Memcmp(pStr, END_RIVERSET) == 0)
		{// 川情報終了の合図があった
			SetRiver(nRiverTexIdx, RiverPos, nRiverXBlock, nRiverZBlock, fRiverColWidth, fRiverColHeight, fRiverColDepth);
			break;
		}
	}
}

//=============================================================================
//    氷情報読み込み処理
//=============================================================================
void CMap::LoadIce(char *pStr, CFileLoader *pFileLoader)
{
	int nIceTexIdx = 0;
	D3DXVECTOR3 IcePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nIceXBlock = 0;
	int nIceZBlock = 0;
	float fIceColWidth = 0.0f;
	float fIceColHeight = 0.0f;
	float fIceColDepth = 0.0f;

	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号情報だった
			nIceTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// 座標情報だった
			IcePos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// 横の分割数情報だった
			nIceXBlock = CFunctionLib::ReadInt(pStr, XBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
		{// 奥行の分割数情報だった
			nIceZBlock = CFunctionLib::ReadInt(pStr, ZBLOCK);
		}
		else if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// 当たり判定情報だった
			LoadCollision(pStr, pFileLoader, &fIceColWidth, &fIceColHeight, &fIceColDepth);
		}
		else if (CFunctionLib::Memcmp(pStr, END_ICESET) == 0)
		{// 氷情報終了の合図があった
			SetIce(nIceTexIdx, IcePos, nIceXBlock, nIceZBlock, fIceColWidth, fIceColHeight, fIceColDepth);
			break;
		}
	}
}

//=============================================================================
//    地面を下げる処理
//=============================================================================
void CMap::FieldDown(D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fFieldDown)
{
	// 川の座標からどのエリアに川があるかを取得
	int nXArea = (int)((pos.x + (MASS_SIZE_X * MASS_BLOCK_X / 2)) / MASS_SIZE_X) + 1;
	int nZArea = (int)((-pos.z + (MASS_SIZE_X* MASS_BLOCK_Z / 2)) / MASS_SIZE_X) + 1;
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
			pVtx[nStartLeftVertex + nCntX].pos.y = fFieldDown;
		}
		nStartLeftVertex += (MASS_BLOCK_X + 1);
	}

	// 頂点バッファをアンロックする
	pVtxBuff->Unlock();
}

//=============================================================================
//    川配置処理
//=============================================================================
void CMap::SetRiver(int nTexIdx, D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth)
{
	// 川の座標から地面の頂点を下げる
	FieldDown(pos, nXBlock, nZBlock, RIVER_DOWN_FIELD);

	// 川を配置
	CRiver::Create(pos, INITIALIZE_D3DXVECTOR3, m_pTextureManager->GetTexture(nTexIdx),
		fBoxColWidth, fBoxColHeight, fBoxColDepth, nXBlock, nZBlock, nTexIdx);
}

//=============================================================================
//    氷配置処理
//=============================================================================
void CMap::SetIce(int nTexIdx, D3DXVECTOR3 pos, int nXBlock, int nZBlock, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth)
{
	// 氷の座標から地面の頂点を下げる
	FieldDown(pos, nXBlock, nZBlock, RIVER_DOWN_FIELD);

	// 氷を配置
	CIceField::Create(pos, INITIALIZE_D3DXVECTOR3, m_pTextureManager->GetTexture(nTexIdx),
		fBoxColWidth, fBoxColHeight, fBoxColDepth, nXBlock, nZBlock, nTexIdx);
}

//=============================================================================
//    配置物読み込み処理
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
//    配置物情報読み込み処理
//=============================================================================
HRESULT CMap::LoadObjectInfo(char *pStr, CFileLoader *pFileLoader)
{
	int nCntObjData = 0;

	// エフェクト管轄クラスを見つけておく
	CEffectManager *pEffectManager = NULL;
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_EFFECTMANAGER)
			{// エフェクト管轄クラスだったクラスだった
				pEffectManager = (CEffectManager*)pScene;
				break;
			}
			pScene = pSceneNext;
		}
	}

	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_OBJECT_DATA) == 0)
		{// オブジェクトデータの数情報だった
			LoadNumObjectData(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, OBJECT_FILENAME) == 0)
		{// オブジェクトデータのファイル名だった
			LoadObjectData(pStr, pFileLoader, nCntObjData);
			nCntObjData++;
		}
		else if (CFunctionLib::Memcmp(pStr, MODELSET) == 0)
		{// 配置物情報だった
			LoadObjModel(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, BILLBOARDSET) == 0)
		{// 配置ビルボード情報だった
			LoadObjBill(pStr, pFileLoader);
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECTSET) == 0)
		{// 配置エフェクト情報だった
			LoadObjEffect(pStr, pFileLoader, pEffectManager);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト情報読み込み終了の合図があった
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    配置物データの数情報読み込み処理
//=============================================================================
void CMap::LoadNumObjectData(char *pStr, CFileLoader *pFileLoader)
{
	// データの数を読み込む
	m_nNumObjectData = CFunctionLib::ReadInt(pStr, NUM_OBJECT_DATA);
	if (m_nNumObjectData <= 0)return;

	// メモリを確保する
	m_pObjectManager = new CCharacterManager*[m_nNumObjectData];
	if (m_pObjectManager == NULL)return;

	// メモリの中身をクリアしておく
	for (int nCntObject = 0; nCntObject < m_nNumObjectData; nCntObject++)
	{
		m_pObjectManager[nCntObject] = NULL;
	}
}

//=============================================================================
//    配置物データ読み込み処理
//=============================================================================
void CMap::LoadObjectData(char *pStr, CFileLoader *pFileLoader, int nCntObjData)
{
	// ファイル名を読み込む
	char aObjFileName[256] = "\0";
	strcpy(aObjFileName, CFunctionLib::ReadString(pStr, aObjFileName, OBJECT_FILENAME));

	// 配置物のデータを読み込む
	if (m_pObjectManager == NULL)return;
	m_pObjectManager[nCntObjData] = CCharacterManager::Create(aObjFileName);
}

//=============================================================================
//    配置モデル情報読み込み処理
//=============================================================================
void CMap::LoadObjModel(char *pStr, CFileLoader *pFileLoader)
{
	int nObjModelType = 0;
	D3DXVECTOR3 ObjModelPos = INITIALIZE_D3DXVECTOR3;
	D3DXVECTOR3 ObjModelRot = INITIALIZE_D3DXVECTOR3;
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// 使用するオブジェクトデータの種類情報だった
			nObjModelType = CFunctionLib::ReadInt(pStr, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// 座標だった
			ObjModelPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// 向きだった
			ObjModelRot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_MODELSET) == 0)
		{// 配置モデル情報終了の合図だった
			break;
		}
	}

	// モデルを配置する
	if (m_pObjectManager != NULL)
	{
		m_pObjectManager[nObjModelType]->SetObject(ObjModelPos, ObjModelRot, nObjModelType);
	}
}

//=============================================================================
//    配置ビルボード情報読み込み処理
//=============================================================================
void CMap::LoadObjBill(char *pStr, CFileLoader *pFileLoader)
{
	int nObjBillTexIdx = 0;
	D3DXVECTOR3 ObjBillPos = INITIALIZE_D3DXVECTOR3;
	D3DXCOLOR ObjBillCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float fObjBillRot = 0.0f;
	float fObjBillWidth = 0.0f;
	float fObjBillHeight = 0.0f;
	bool bObjBillLighting = false;
	bool bObjBillDrawAddtive = false;
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号情報だった
			nObjBillTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// 座標だった
			ObjBillPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// 色だった
			ObjBillCol = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// 向きだった
			fObjBillRot = CFunctionLib::ReadFloat(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// 幅だった
			fObjBillWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// 高さだった
			fObjBillHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, LIGHTING) == 0)
		{// ライティングするかどうかだった
			bObjBillLighting = CFunctionLib::ReadBool(pStr, LIGHTING);
		}
		else if (CFunctionLib::Memcmp(pStr, DRAW_ADDTIVE) == 0)
		{// 加算合成で描画するかするかどうかだった
			bObjBillDrawAddtive = CFunctionLib::ReadBool(pStr, DRAW_ADDTIVE);
		}
		else if (CFunctionLib::Memcmp(pStr, END_BILLBOARDSET) == 0)
		{// 配置ビルボード情報終了の合図だった
			break;
		}
	}

	// ビルボードを配置する
	CBillboardObject *pObjBill = CBillboardObject::Create(ObjBillPos, ObjBillCol, fObjBillWidth, fObjBillWidth,
		fObjBillRot, bObjBillLighting, bObjBillDrawAddtive, nObjBillTexIdx);
	if (pObjBill != NULL && m_pTextureManager != NULL)
	{
		pObjBill->BindTexture(m_pTextureManager->GetTexture(nObjBillTexIdx));
	}
}

//=============================================================================
//    配置エフェクト情報読み込み処理
//=============================================================================
void CMap::LoadObjEffect(char *pStr, CFileLoader *pFileLoader, CEffectManager *pEffectManager)
{
	int nObjEffectType = 0;
	D3DXVECTOR3 ObjEffectPos = INITIALIZE_D3DXVECTOR3;
	D3DXVECTOR3 ObjEffectRot = INITIALIZE_D3DXVECTOR3;
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// 使用するオブジェクトデータの種類情報だった
			nObjEffectType = CFunctionLib::ReadInt(pStr, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// 座標だった
			ObjEffectPos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// 向きだった
			ObjEffectRot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_EFFECTSET) == 0)
		{// 配置エフェクト情報終了の合図だった
			break;
		}
	}

	// エフェクトを配置する
	pEffectManager->SetEffect(ObjEffectPos, ObjEffectRot, nObjEffectType);
}

//=============================================================================
//    敵の生成情報読み込みのためのファイルオープン処理
//=============================================================================
HRESULT CMap::LoadEnemyList(char *pEnemylistName, char *pStr)
{
	int nCntEnemyList = 0;            // 敵の生成情報を読み込んだ回数
	CFileLoader *pFileLoader = NULL;  // ファイル読み込み用クラスへのポインタ

	// ファイルオープン
	pFileLoader = CFileLoader::Create(pEnemylistName);
	if (pFileLoader == NULL) return E_FAIL;

	// ファイル読み込み開始
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
	{// スクリプト読み込み開始の合図があった
		while (1)
		{// 抜けるまでループ
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, NUM_ENEMYLIST) == 0)
			{// 敵の生成情報の総数情報だった
				m_nNumEnemyListData = CFunctionLib::ReadInt(pStr, NUM_ENEMYLIST);
				CreateEnemyListData();
			}
			else if (CFunctionLib::Memcmp(pStr, ENEMYLISTSET) == 0)
			{// 敵の生成情報だった
				LoadEnemyListInfo(pStr, pFileLoader, nCntEnemyList);
				nCntEnemyList++;
			}
			else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
			{// スクリプト情報読み込み終了の合図があった
				break;
			}
		}
	}

	// メモリの開放
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}

	// ファイル名をコピーしておく
	strcpy(m_aEnemyListFileName, pEnemylistName);

	return S_OK;
}

//=============================================================================
//    敵の生成情報読み込み処理
//=============================================================================
HRESULT CMap::LoadEnemyListInfo(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList)
{
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, RESPAWN) == 0)
		{// リスポーン位置の番号情報だった
			m_pEnemyListData[nCntEnemyList]->SetRespawnIdx(CFunctionLib::ReadInt(pStr, RESPAWN));
		}
		else if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// 敵の種類の番号情報だった
			m_pEnemyListData[nCntEnemyList]->SetEnemyType(CFunctionLib::ReadInt(pStr, TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// 敵を出現させるタイミング情報だった
			m_pEnemyListData[nCntEnemyList]->SetRespawnTime(CFunctionLib::ReadInt(pStr, TIME));
		}
		else if (CFunctionLib::Memcmp(pStr, ITEM) == 0)
		{// アイテム情報だった
			m_pEnemyListData[nCntEnemyList]->SetItem(true);
			LoadEnemyItem(pStr, pFileLoader, nCntEnemyList);
		}
		else if (CFunctionLib::Memcmp(pStr, AISET) == 0)
		{// 敵のAI情報だった
			LoadEnemyAI(pStr, pFileLoader, nCntEnemyList);
		}
		else if (CFunctionLib::Memcmp(pStr, END_ENEMYLISTSET) == 0)
		{// 敵の生成情報読み込み終了の合図だった
			break;
		}
	}

	return S_OK;
}

//=============================================================================
//    敵のアイテム生成情報読み込み処理
//=============================================================================
void CMap::LoadEnemyItem(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList)
{
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// 出現させるアイテムの種類番号だった
			m_pEnemyListData[nCntEnemyList]->SetItemType(CFunctionLib::ReadInt(pStr, TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, END_ITEM) == 0)
		{// 敵のアイテム生成情報読み込み終了の合図があった
			break;
		}
	}
}

//=============================================================================
//    敵のAI情報を読み込む処理
//=============================================================================
void CMap::LoadEnemyAI(char *pStr, CFileLoader *pFileLoader, int nCntEnemyList)
{
	while (1)
	{// 抜けるまでループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, BULLET) == 0)
		{// 弾を打つ際のランダム評価値だった
			m_pEnemyListData[nCntEnemyList]->SetAI_BulletEva(CFunctionLib::ReadInt(pStr, BULLET));
		}
		else if (CFunctionLib::Memcmp(pStr, MASS) == 0)
		{// 向きを変えさせるランダム評価値だった
			m_pEnemyListData[nCntEnemyList]->SetAI_MassEva(CFunctionLib::ReadInt(pStr, MASS));
		}
		else if (CFunctionLib::Memcmp(pStr, DOWN) == 0)
		{// 下向きに動かせるランダム評価値だった
			m_pEnemyListData[nCntEnemyList]->SetAI_DownEva(CFunctionLib::ReadInt(pStr, DOWN));
		}
		else if (CFunctionLib::Memcmp(pStr, END_AISET) == 0)
		{// 敵のAI情報読み込み終了の合図があった
			break;
		}
	}
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
//    基地の位置情報保存処理
//=============================================================================
HRESULT CMap::SaveHeadQuartersPos(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 司令部の位置(横 : 奥行)\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 司令部の位置を書き込み
	pFileSaver->Print("%s%d %d\n\n", BASE_POS, m_pHeadQuarters->GetAreaX(), m_pHeadQuarters->GetAreaZ());

	return S_OK;
}

//=============================================================================
//    プレイヤーのリスポーン位置情報保存処理
//=============================================================================
HRESULT CMap::SavePlayerRespawn(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# プレイヤーのリスポーン位置(横 : 奥行)\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// プレイヤーのリスポーン位置を書き込み
	for (int nCntRes = 0; nCntRes < MAX_PLAYER_RESPAWN; nCntRes++)
	{
		pFileSaver->Print("%s%d %d\n", PLAYER_RESPAWN, m_pPlayerRespawn[nCntRes]->GetAreaX(), m_pPlayerRespawn[nCntRes]->GetAreaZ());
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
	pFileSaver->Print("# 敵のリスポーン位置(横 : 奥行)\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 敵のリスポーン位置を書き込み
	for (int nCntRes = 0; nCntRes < MAX_ENEMY_RESPAWN; nCntRes++)
	{
		pFileSaver->Print("%s%d %d\n", ENEMY_RESPAWN, m_pEnemyRespawn[nCntRes]->GetAreaX(), m_pEnemyRespawn[nCntRes]->GetAreaZ());
	}
	pFileSaver->Print("\n");

	return S_OK;
}


//=============================================================================
//    空保存処理
//=============================================================================
HRESULT CMap::SaveSky(CFileSaver *pFileSaver)
{
	if (m_pSky == NULL) return E_FAIL;

	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 空情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	int nSkyTexIdx = m_pSky->GetTexIdx();
	D3DXVECTOR3 SkyPos = m_pSky->GetPos();
	D3DXVECTOR3 SkyRot = m_pSky->GetRot();
	float fSkyRollSpeed = m_pSky->GetRollSpeed();
	int nSkyXBlock = m_pSky->GetXBlock();
	int nSkyYBlock = m_pSky->GetYBlock();
	float fSkyRadius = m_pSky->GetRadius();

	pFileSaver->Print("%s\n", SKYSET);
	pFileSaver->Print("	%s%d						# 種類\n", TEX_IDX, nSkyTexIdx);
	pFileSaver->Print("	%s%.1f %.1f %.1f	# 座標\n", POS, SkyPos.x, SkyPos.y, SkyPos.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f				# 向き\n", ROT, SkyRot.x, SkyRot.y, SkyRot.z);
	pFileSaver->Print("	%s%f			# 回転スピード\n", ROLLSPEED, fSkyRollSpeed);
	pFileSaver->Print("	%s%d						# 横の分割数\n", XBLOCK, nSkyXBlock);
	pFileSaver->Print("	%s%d						# 縦の分割数\n", YBLOCK, nSkyYBlock);
	pFileSaver->Print("	%s%.1f				# 半径\n", RADIUS, fSkyRadius);
	pFileSaver->Print("%s\n", END_SKYSET);
	pFileSaver->Print("\n");

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
	char aSaveEnemyListFileName[256];
	strcpy(aSaveLightFileName, SAVEFILENAME_LIGHT);
	strcpy(aSaveGameFieldFileName, SAVEFILENAME_GAMEFIELD);
	strcpy(aSaveObjectFileName, SAVEFILENAME_OBJECT);
	strcpy(aSaveEnemyListFileName, SAVEFILENAME_ENEMYLIST);
	pFileSaver->Print("%s\n", MAPSET);      // マップセット情報読み込み開始の合図を書き込み
	pFileSaver->Print("	%s%s				# ライト情報のスクリプトファイル名\n", LIGHT_FILENAME, strcat(aSaveLightFileName, m_aLightFileName));
	pFileSaver->Print("	%s%s	# ゲームフィールド情報のスクリプトファイル名\n", GAMEFIELD_FILENAME, strcat(aSaveGameFieldFileName, m_aGameFieldFileName));
	pFileSaver->Print("	%s%s			# 配置物情報のスクリプトファイル名\n", OBJECT_FILENAME, strcat(aSaveObjectFileName, m_aObjectFileName));
	pFileSaver->Print("	%s%s	# 敵の生成情報情報のスクリプトファイル名\n", ENEMYLIST_FILENAME, strcat(aSaveEnemyListFileName, m_aEnemyListFileName));
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
	pFileSaver->Print("	%s%.3f								# ライトの外部コーンの角度\n", PHI, fLighPhi);
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
		pFileSaver->Print("\n%s		# この行は絶対消さないこと！\n", END_SCRIPT);

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
	// 地面情報を書き込み
	SaveField(pFileSaver);

	// ブロック情報を書き込み
	SaveBlock(pFileSaver);

	// 川情報を書き込み
	SaveRiver(pFileSaver);

	// 氷情報を書き込み
	SaveIce(pFileSaver);
}

//=============================================================================
//    地面の情報を保存する処理
//=============================================================================
void CMap::SaveField(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 地面情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 各種情報を取得
	float fFieldWidth = m_pMeshField->GetWidth();
	float fFieldDepth = m_pMeshField->GetHeight();
	int nFieldXBlock = m_pMeshField->GetXBlock();
	int nFieldZBlock = m_pMeshField->GetZBlock();

	// 各種情報をテキストファイルに書き込み
	pFileSaver->Print("%s\n", FIELDSET);
	pFileSaver->Print("	%s%d		# 使用するテクスチャの番号(テクスチャリストの番号と照合)\n", TEX_IDX, m_nFieldTexIdx);
	pFileSaver->Print("	%s%.1f	# 1マス分の横幅\n", WIDTH, fFieldWidth);
	pFileSaver->Print("	%s%.1f	# 1マス分の奥行\n", DEPTH, fFieldDepth);
	pFileSaver->Print("	%s%d		# 横の分割数\n", XBLOCK, nFieldXBlock);
	pFileSaver->Print("	%s%d		# 奥行の分割数\n", ZBLOCK, nFieldZBlock);
	pFileSaver->Print("%s\n\n", END_FIELDSET);
}

//=============================================================================
//    ブロックを保存する処理
//=============================================================================
void CMap::SaveBlock(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ブロック配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
			{// ブロッククラスだった
				SaveBlockInfo((CBlock*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}

	// 可読性のため改行
	pFileSaver->Print("\n");
}

//=============================================================================
//    ブロックの情報を保存する処理
//=============================================================================
void CMap::SaveBlockInfo(CBlock *pBlock, CFileSaver *pFileSaver)
{
	// 各種情報を取得
	int nBlockType = pBlock->GetType();
	int nBlockModelIdx = pBlock->GetModelIdx();
	D3DXVECTOR3 BlockPos = pBlock->GetPos();
	D3DXVECTOR3 BlockRot = D3DXToDegree(pBlock->GetRot());
	float fBlockColWidth = pBlock->GetBoxCollider()->GetWidth();
	float fBlockColHeight = pBlock->GetBoxCollider()->GetHeight();
	float fBlockColDepth = pBlock->GetBoxCollider()->GetDepth();

	// 各種情報をテキストファイルに保存
	pFileSaver->Print("%s\n", BLOCKSET);
	pFileSaver->Print("	%s%d				# 種類\n", BLOCKTYPE, nBlockType);
	pFileSaver->Print("	%s%d			# 使用するモデルの番号\n", MODELIDX, nBlockModelIdx);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, BlockPos.x, BlockPos.y, BlockPos.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 向き\n", ROT, BlockRot.x, BlockRot.y, BlockRot.z);
	pFileSaver->Print("	%s\n", COLLISION);
	pFileSaver->Print("		%s%.1f		# 当たり判定の幅\n", WIDTH, fBlockColWidth);
	pFileSaver->Print("		%s%.1f		# 当たり判定の高さ\n", HEIGHT, fBlockColHeight);
	pFileSaver->Print("		%s%.1f		# 当たり判定の奥行\n", DEPTH, fBlockColDepth);
	pFileSaver->Print("	%s\n", END_COLLISION);
	pFileSaver->Print("%s\n", END_BLOCKSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    川保存処理
//=============================================================================
void CMap::SaveRiver(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 川配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_RIVER)
			{// 川クラスだった
				SaveRiverInfo((CRiver*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    川情報保存処理
//=============================================================================
void CMap::SaveRiverInfo(CRiver *pRiver, CFileSaver *pFileSaver)
{
	// 各種情報を取得
	int nRiverTexIdx = pRiver->GetTexIdx();
	D3DXVECTOR3 RiverPos = pRiver->GetPos();
	int nRiverXBlock = pRiver->GetMeshField()->GetXBlock();
	int nRiverZBlock = pRiver->GetMeshField()->GetZBlock();
	float fRiverColWidth = pRiver->GetBoxCollider()->GetWidth();
	float fRiverColHeight = pRiver->GetBoxCollider()->GetHeight();
	float fRiverColDepth = pRiver->GetBoxCollider()->GetDepth();

	// 各種情報をテキストファイルに保存
	pFileSaver->Print("%s\n", RIVERSET);
	pFileSaver->Print("	%s%d					# 使用するテクスチャの番号\n", TEX_IDX, nRiverTexIdx);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, RiverPos.x, RiverPos.y, RiverPos.z);
	pFileSaver->Print("	%s%d					# 横の分割数\n", XBLOCK, nRiverXBlock);
	pFileSaver->Print("	%s%d					# 奥行の分割数\n", ZBLOCK, nRiverZBlock);
	pFileSaver->Print("	%s\n", COLLISION);
	pFileSaver->Print("		%s%.1f			# 当たり判定の幅\n", WIDTH, fRiverColWidth);
	pFileSaver->Print("		%s%.1f			# 当たり判定の高さ\n", HEIGHT, fRiverColHeight);
	pFileSaver->Print("		%s%.1f			# 当たり判定の奥行\n", DEPTH, fRiverColDepth);
	pFileSaver->Print("	%s\n", END_COLLISION);
	pFileSaver->Print("%s\n", END_RIVERSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    氷保存処理
//=============================================================================
void CMap::SaveIce(CFileSaver *pFileSaver)
{
	// コメント部分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 氷配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ICEFIELD)
			{// 氷クラスだった
				SaveIceInfo((CIceField*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    氷情報保存処理
//=============================================================================
void CMap::SaveIceInfo(CIceField *pIce, CFileSaver *pFileSaver)
{
	// 各種情報を取得
	int nIceTexIdx = pIce->GetTexIdx();
	D3DXVECTOR3 IcePos = pIce->GetPos();
	int nIceXBlock = pIce->GetMeshField()->GetXBlock();
	int nIceZBlock = pIce->GetMeshField()->GetZBlock();
	float fIceColWidth = pIce->GetBoxCollider()->GetWidth();
	float fIceColHeight = pIce->GetBoxCollider()->GetHeight();
	float fIceColDepth = pIce->GetBoxCollider()->GetDepth();

	// 各種情報をテキストファイルに保存
	pFileSaver->Print("%s\n", ICESET);
	pFileSaver->Print("	%s%d					# 使用するテクスチャの番号\n", TEX_IDX, nIceTexIdx);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, IcePos.x, IcePos.y, IcePos.z);
	pFileSaver->Print("	%s%d					# 横の分割数\n", XBLOCK, nIceXBlock);
	pFileSaver->Print("	%s%d					# 奥行の分割数\n", ZBLOCK, nIceZBlock);
	pFileSaver->Print("	%s\n", COLLISION);
	pFileSaver->Print("		%s%.1f			# 当たり判定の幅\n", WIDTH, fIceColWidth);
	pFileSaver->Print("		%s%.1f			# 当たり判定の高さ\n", HEIGHT, fIceColHeight);
	pFileSaver->Print("		%s%.1f			# 当たり判定の奥行\n", DEPTH, fIceColDepth);
	pFileSaver->Print("	%s\n", END_COLLISION);
	pFileSaver->Print("%s\n", END_ICESET);
	pFileSaver->Print("\n");
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

		// 配置物データの数を書き込み
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# 読み込むオブジェクトデータの数\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("%s%d\n", NUM_OBJECT_DATA, m_nNumObjectData);
		pFileSaver->Print("\n");

		// 配置物データのファイル名を書き込み
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		pFileSaver->Print("# 読み込むオブジェクトデータのファイル名\n");
		pFileSaver->Print("#------------------------------------------------------------------------------\n");
		for (int nCntObject = 0; nCntObject < m_nNumObjectData; nCntObject++)
		{
			pFileSaver->Print("%s%s\n", OBJECT_FILENAME, m_pObjectManager[nCntObject]->GetFileName());
		}
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
//    配置モデル保存処理
//=============================================================================
void CMap::SaveObjModel(CFileSaver *pFileSaver)
{
	// 冒頭分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# モデル配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 配置モデル情報を書き込み
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
			{// 配置物クラスだった
				SaveObjModelInfo((CObject*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    配置モデル情報保存処理
//=============================================================================
void CMap::SaveObjModelInfo(CObject *pObject, CFileSaver *pFileSaver)
{
	// 各種情報を取得
	int nObjModelType = pObject->GetType();
	D3DXVECTOR3 ObjModelPos = pObject->GetPos();
	D3DXVECTOR3 ObjModelRot = pObject->GetRot();

	// 各種情報をテキストファイルに保存
	pFileSaver->Print("%s\n", MODELSET);
	pFileSaver->Print("	%s%d					# 種類\n", TYPE, nObjModelType);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, ObjModelPos.x, ObjModelPos.y, ObjModelPos.z);
	pFileSaver->Print("	%s%.2f %.2f %.2f		# 向き\n", ROT, ObjModelRot.x, ObjModelRot.y, ObjModelRot.z);
	pFileSaver->Print("%s\n", END_MODELSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    配置ビルボード保存処理
//=============================================================================
void CMap::SaveObjBill(CFileSaver *pFileSaver)
{
	// 冒頭分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# ビルボード配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 配置ビルボード情報を書き込み
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJBILLBOARD)
			{// 配置ビルボードクラスだった
				SaveObjBillInfo((CBillboardObject*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    配置ビルボード情報保存処理
//=============================================================================
void CMap::SaveObjBillInfo(CBillboardObject *pBillObj, CFileSaver *pFileSaver)
{
	// 各種情報を取得
	int nObjBillTexIdx = pBillObj->GetTexIdx();
	D3DXVECTOR3 ObjBillPos = pBillObj->GetPos();
	D3DXCOLOR ObjBillCol = pBillObj->GetCol();
	float fObjBillRot = pBillObj->GetRot();
	float fObjBillWidth = pBillObj->GetWidth();
	float fObjBillHeight = pBillObj->GetHeight();
	bool bObjBillLighting = pBillObj->GetLighting();
	bool bObjBillDrawAddtive = pBillObj->GetDrawAddtive();

	// 各種情報をテキストファイルに保存
	pFileSaver->Print("%s\n", BILLBOARDSET);
	pFileSaver->Print("	%s%d					# 使用するテクスチャの番号\n", TEX_IDX, nObjBillTexIdx);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, ObjBillPos.x, ObjBillPos.y, ObjBillPos.z);
	pFileSaver->Print("	%s%.1f %.1f %.1f %.1f		# 色\n", COL, ObjBillCol.r, ObjBillCol.g, ObjBillCol.b, ObjBillCol.a);
	pFileSaver->Print("	%s%.1f					# 向き\n", ROT, fObjBillRot);
	pFileSaver->Print("	%s%.1f				# 幅\n", WIDTH, fObjBillWidth);
	pFileSaver->Print("	%s%.1f				# 高さ\n", HEIGHT, fObjBillHeight);
	pFileSaver->Print("	%s%d				# ライティングするかどうか\n", LIGHTING, (int)bObjBillLighting);
	pFileSaver->Print("	%s%d			# 加算合成で描画するかどうか\n", DRAW_ADDTIVE, (int)bObjBillDrawAddtive);
	pFileSaver->Print("%s\n", END_BILLBOARDSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    配置エフェクト保存処理
//=============================================================================
void CMap::SaveObjEffect(CFileSaver *pFileSaver)
{
	// 冒頭分を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# エフェクト配置情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// 配置エフェクト情報を書き込み
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER)
			{// エミッタクラスだった
				SaveObjEffectInfo((CEmitter*)pScene, pFileSaver);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    配置エフェクト情報保存処理
//=============================================================================
void CMap::SaveObjEffectInfo(CEmitter *pEmitter, CFileSaver *pFileSaver)
{
	// 各種情報を取得
	int nObjEffectType = pEmitter->GetType();
	D3DXVECTOR3 ObjEffectPos = pEmitter->GetPos();
	D3DXVECTOR3 ObjEffectRot = pEmitter->GetRot();

	// 各種情報をテキストファイルに保存
	pFileSaver->Print("%s\n", EFFECTSET);
	pFileSaver->Print("	%s%d					# 種類\n", TYPE, nObjEffectType);
	pFileSaver->Print("	%s%.1f %.1f %.1f		# 座標\n", POS, ObjEffectPos.x, ObjEffectPos.y, ObjEffectPos.z);
	pFileSaver->Print("	%s%.2f %.2f %.2f		# 向き\n", ROT, ObjEffectRot.x, ObjEffectRot.y, ObjEffectRot.z);
	pFileSaver->Print("%s\n", END_EFFECTSET);
	pFileSaver->Print("\n");
}

//=============================================================================
//    敵の生成情報を保存する
//=============================================================================
HRESULT CMap::SaveEnemyList(char *pEnemyListName)
{
	char aSaveFileName[256] = SAVEFILENAME_ENEMYLIST;
	CFileSaver *pFileSaver = NULL;  // ファイル保存用クラスへのポインタ
	pFileSaver = CFileSaver::Create(strcat(aSaveFileName, pEnemyListName));
	if (pFileSaver != NULL)
	{// ファイルが読み込めた
	    // ファイルの冒頭分を書き込み
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# 敵の生成情報スクリプトファイル [%s]\n", pEnemyListName);
		pFileSaver->Print("# Author : Hodaka Niwa\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対消さないこと！\n", SCRIPT);
		pFileSaver->Print("\n");

		// 敵の生成情報を書き込み
		SaveEnemyListInfo(pFileSaver);

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
//    敵の生成情報をファイルに保存する
//=============================================================================
void CMap::SaveEnemyListInfo(CFileSaver *pFileSaver)
{
	// 生成する敵の数を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 生成する敵の数\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("%s%d\n", NUM_ENEMYLIST, m_nNumEnemyListData);
	pFileSaver->Print("\n");

	// 敵の生成情報を書き込み
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("# 敵の生成情報\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	for (int nCnt = 0; nCnt < m_nNumEnemyListData; nCnt++)
	{// 生成する敵の数分繰り返し
		pFileSaver->Print("%s\n", ENEMYLISTSET);
		pFileSaver->Print("	%s%d			# リスポーン位置の番号\n", RESPAWN, m_pEnemyListData[nCnt]->GetRespawnIdx());
		pFileSaver->Print("	%s%d			# 種類\n", TYPE, m_pEnemyListData[nCnt]->GetEnemyType());
		pFileSaver->Print("	%s%d			# 出現させる時間\n", TIME, m_pEnemyListData[nCnt]->GetRespawnTime());
		if (m_pEnemyListData[nCnt]->GetItem() == true)
		{// アイテムを出現させるなら
			pFileSaver->Print("	%s\n", ITEM);
			pFileSaver->Print("		%s%d		# 出現させるアイテムの種類\n", TYPE, m_pEnemyListData[nCnt]->GetItemType());
			pFileSaver->Print("	%s\n", END_ITEM);
		}
		pFileSaver->Print("	%s\n", AISET);
		pFileSaver->Print("		%s%d	# 弾を打つ際のランダム評価値\n", BULLET, m_pEnemyListData[nCnt]->GetAI_BulletEva());
		pFileSaver->Print("		%s%d		# 向きを変えさせるランダム評価値\n", MASS, m_pEnemyListData[nCnt]->GetAI_MassEva());
		pFileSaver->Print("		%s%d		# 下向きに動かせるランダム評価値\n", DOWN, m_pEnemyListData[nCnt]->GetAI_DownEva());
		pFileSaver->Print("	%s\n", END_AISET);
		pFileSaver->Print("%s\n", END_ENEMYLISTSET);
		pFileSaver->Print("\n");
	}

	pFileSaver->Print("\n");
}

//=============================================================================
//    ブロックを生成する処理
//=============================================================================
void CMap::CreateBlock(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nModelIdx, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, LPDIRECT3DTEXTURE9 *pTexture, float fBoxColWidth, float fBoxColHeight, float fBoxColDepth)
{
	switch (nType)
	{// 種類によって処理わけ
	case CBlock::TYPE_BREAK_BULLET_NOT_DIRTY:
		CBlockType0::Create(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxColWidth, fBoxColHeight, fBoxColDepth);
		break;
	case CBlock::TYPE_BREAK_TANK_DIRTY:
		CBlockType1::Create(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxColWidth, fBoxColHeight, fBoxColDepth);
		break;
	case CBlock::TYPE_NOT_BREAK_NOT_DIRTY:
		CBlockType2::Create(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxColWidth, fBoxColHeight, fBoxColDepth);
		break;
	case CBlock::TYPE_NOT_BREAK_TANK_DIRTY:
		CBlockType3::Create(pos, rot, nType, nModelIdx, pMesh, pBuffMat, nNumMat, pTexture, fBoxColWidth, fBoxColHeight, fBoxColDepth);
		break;
	}
}

//=============================================================================
//    敵の生成情報クラスを生成する
//=============================================================================
void CMap::CreateEnemyListData(void)
{
	if (m_nNumEnemyListData > 0)
	{// 1個以上生成する
		m_pEnemyListData = new CEnemy_ListData*[m_nNumEnemyListData];
		for (int nCnt = 0; nCnt < m_nNumEnemyListData; nCnt++)
		{
			m_pEnemyListData[nCnt] = new CEnemy_ListData;
		}
	}
}

//=============================================================================
//    マップを破棄する処理
//=============================================================================
void CMap::DeleteMap(void)
{
	// 地面を破棄
	ReleaseMeshField();

	// 空を破棄
	ReleaseSky();

	// 司令部を破棄
	ReleaseHeadQuarters();

	// プレイヤーのリスポーン位置を破棄
	ReleasePlayerRespawn();

	// 敵のリスポーン位置を破棄
	ReleaseEnemyRespawn();

	// 敵の生成情報を破棄
	ReleaseEnemyListData();

	// キャラクター情報を破棄
	ReleaseObjectManager();

	// ライトを破棄
	DeleteLight();

	// ゲームフィールドオブジェクトを破棄
	DeleteGameField();

	// 配置物を破棄
	DeleteObject();

	// 敵を破棄
	DeleteEnemy();

	// 全てのオブジェクトの開放チェック
	CScene::DeathCheck();
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
			else if (pScene->GetObjType() == CScene::OBJTYPE_RIVER)
			{// 川クラスだった
				DeleteRiver((CRiver*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_ICEFIELD)
			{// 氷クラスだった
				DeleteIce((CIceField*)pScene);
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
//    川を破棄する処理
//=============================================================================
void CMap::DeleteRiver(CRiver *pRiver)
{
	pRiver->Uninit();
	pRiver = NULL;
}

//=============================================================================
//    氷を破棄する処理
//=============================================================================
void CMap::DeleteIce(CIceField *pIceField)
{
	pIceField->Uninit();
	pIceField = NULL;
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
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
			{// ブロッククラスだった
				DeleteObjModel((CObject*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJBILLBOARD)
			{// ビルボードクラスだった
				DeleteObjBillboard((CBillboardObject*)pScene);
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_EMITTER || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER)
			{// エミッタクラスだった
				DeleteObjEffect((CEmitter*)pScene);
			}
			pScene = pSceneNext;
		}
	}
}

//=============================================================================
//    配置モデルを破棄する処理
//=============================================================================
void CMap::DeleteObjModel(CObject *pObject)
{
	pObject->Uninit();
	pObject = NULL;
}

//=============================================================================
//    配置ビルボードを破棄する処理
//=============================================================================
void CMap::DeleteObjBillboard(CBillboardObject *pBillboard)
{
	pBillboard->Uninit();
	pBillboard = NULL;
}

//=============================================================================
//    配置エフェクトを破棄する処理
//=============================================================================
void CMap::DeleteObjEffect(CEmitter *pEmitter)
{
	pEmitter->Uninit();
	pEmitter = NULL;
}

//=============================================================================
//    敵を破棄する処理
//=============================================================================
void CMap::DeleteEnemy(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// 敵クラスだった
				pScene->Uninit();
			}
			pScene = pSceneNext;
		}
	}
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
//    配置物のデータ管轄クラスを開放する処理
//=============================================================================
void CMap::ReleaseObjectManager(void)
{
	if (m_pObjectManager != NULL)
	{
		for (int nCntObject = 0; nCntObject < m_nNumObjectData; nCntObject++)
		{
			if (m_pObjectManager[nCntObject] != NULL)
			{
				m_pObjectManager[nCntObject]->Uninit();
				delete m_pObjectManager[nCntObject];
				m_pObjectManager[nCntObject] = NULL;
			}
		}
		delete[] m_pObjectManager;
		m_pObjectManager = NULL;
	}
}

//=============================================================================
//    地面を開放する
//=============================================================================
void CMap::ReleaseMeshField(void)
{
	if (m_pMeshField != NULL)
	{
		m_pMeshField->Uninit();
		m_pMeshField = NULL;
	}
}

//=============================================================================
//    空を開放する
//=============================================================================
void CMap::ReleaseSky(void)
{
	if (m_pSky != NULL)
	{
		m_pSky->Uninit();
		m_pSky = NULL;
	}
}

//=============================================================================
//    司令部を開放する
//=============================================================================
void CMap::ReleaseHeadQuarters(void)
{
	if (m_pHeadQuarters != NULL)
	{
		m_pHeadQuarters->Uninit();
		m_pHeadQuarters = NULL;
	}
}

//=============================================================================
//    プレイヤーのリスポーン位置を開放する
//=============================================================================
void CMap::ReleasePlayerRespawn(void)
{
	for (int nCntRes = 0; nCntRes < MAX_PLAYER_RESPAWN; nCntRes++)
	{
		if (m_pPlayerRespawn[nCntRes] != NULL)
		{
			m_pPlayerRespawn[nCntRes]->Uninit();
			m_pPlayerRespawn[nCntRes] = NULL;
		}
	}
}

//=============================================================================
//    敵のリスポーン位置を開放する
//=============================================================================
void CMap::ReleaseEnemyRespawn(void)
{
	for (int nCntRes = 0; nCntRes < MAX_ENEMY_RESPAWN; nCntRes++)
	{
		if (m_pEnemyRespawn[nCntRes] != NULL)
		{
			m_pEnemyRespawn[nCntRes]->Uninit();
			m_pEnemyRespawn[nCntRes] = NULL;
		}
	}
}

//=============================================================================
//    敵の生成情報を開放する
//=============================================================================
void CMap::ReleaseEnemyListData(void)
{
	if (m_pEnemyListData != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumEnemyListData; nCnt++)
		{
			delete m_pEnemyListData[nCnt];
			m_pEnemyListData[nCnt] = NULL;
		}

		delete[] m_pEnemyListData;
		m_pEnemyListData = NULL;
	}
}

//=============================================================================
//    地面に張り付けるテクスチャの番号を取得する
//=============================================================================
int CMap::GetFieldTexIdx(void)
{
	return m_nFieldTexIdx;
}

//=============================================================================
//    地面クラスへのポインタを取得する
//=============================================================================
CMeshField *CMap::GetMeshField(void)
{
	return m_pMeshField;
}

//=============================================================================
//    空クラスへのポインタを取得する
//=============================================================================
CSky *CMap::GetSky(void)
{
	return m_pSky;
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
//    配置物データ管轄クラスへのポインタを取得する
//=============================================================================
CCharacterManager **CMap::GetObjectManager(void)
{
	return m_pObjectManager;
}
CCharacterManager *CMap::GetObjectManager(int nIdx)
{
	return m_pObjectManager[nIdx];
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
//    司令部クラスへのポインタを取得する
//=============================================================================
CHeadQuarters *CMap::GetHeadQuarters(void)
{
	return m_pHeadQuarters;
}

//=============================================================================
//    プレイヤーのリスポーンクラスへのポインタを取得する
//=============================================================================
CRespawn *CMap::GetPlayerRespawn(int nIdx)
{
	return m_pPlayerRespawn[nIdx];
}

//=============================================================================
//    敵のリスポーンクラスへのポインタを取得する
//=============================================================================
CRespawn *CMap::GetEnemyRespawn(int nIdx)
{
	return m_pEnemyRespawn[nIdx];
}

//=============================================================================
//    敵の生成情報データクラスへのポインタを取得する
//=============================================================================
CEnemy_ListData *CMap::GetEnemyListData(int nIdx)
{
	return m_pEnemyListData[nIdx];
}

//=============================================================================
//    敵の生成情報の総数を取得する
//=============================================================================
int CMap::GetNumEnemyListData(void)
{
	return m_nNumEnemyListData;
}

//=============================================================================
//    地面に張り付けるテクスチャの番号を設定する
//=============================================================================
void CMap::SetFieldTexIdx(const int nFieldTexIdx)
{
	m_nFieldTexIdx = nFieldTexIdx;
}

//=============================================================================
//    地面クラスへのポインタを設定する
//=============================================================================
void CMap::SetMeshField(CMeshField *pMeshField)
{
	m_pMeshField = pMeshField;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを設定する
//=============================================================================
void CMap::SetTextureManager(CTextureManager *pTextureManager)
{
	m_pTextureManager = pTextureManager;
}

//=============================================================================
//    空クラスへのポインタを設定する
//=============================================================================
void CMap::SetSky(CSky *pSky)
{
	m_pSky = pSky;
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
//    配置物データ管轄クラスへのポインタを設定する
//=============================================================================
void CMap::SetObjectManager(CCharacterManager **pObjectManager)
{
	m_pObjectManager = pObjectManager;
}
void CMap::SetObjectManager(CCharacterManager *pObjectManager, int nIdx)
{
	m_pObjectManager[nIdx] = pObjectManager;
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

//=============================================================================
//    読み込む敵の生成情報のスクリプトファイル名を設定する
//=============================================================================
void CMap::SetEnemyListFileName(char *pFileName)
{
	strcpy(m_aEnemyListFileName, pFileName);
}

//=============================================================================
//    司令部クラス型のポインタを設定する
//=============================================================================
void CMap::SetHeadQuarters(CHeadQuarters *pHeadQuarters)
{
	m_pHeadQuarters = pHeadQuarters;
}

//=============================================================================
//    プレイヤーのリスポーンクラス型のポインタを設定する
//=============================================================================
void CMap::SetPlayerRespawn(CRespawn *pRespawn, int nIdx)
{
	m_pPlayerRespawn[nIdx] = pRespawn;
}

//=============================================================================
//    敵のリスポーンクラス型のポインタを設定する
//=============================================================================
void CMap::SetEnemyRespawn(CRespawn *pRespawn, int nIdx)
{
	m_pEnemyRespawn[nIdx] = pRespawn;
}

//=============================================================================
//    敵の生成情報データを設定する
//=============================================================================
void CMap::SetEnemyListData(CEnemy_ListData EnemyData, int nIdx)
{
	m_pEnemyListData[nIdx]->Cpy(EnemyData);
}

//=============================================================================
//    敵の生成情報の総数を設定する
//=============================================================================
void CMap::SetNumEnemyListData(const int nNumEnemyData)
{
	m_nNumEnemyListData = nNumEnemyData;
}

//=============================================================================
//    オブジェクトデータの総数を設定する
//=============================================================================
void CMap::SetNumObjectData(const int nObjectData)
{
	m_nNumObjectData = nObjectData;
}


//*****************************************************************************
//    CEnemy_ListDataの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CEnemy_ListData::CEnemy_ListData()
{
	m_nRespawnIdx = 0;
	m_nEnemyType = 0;
	m_nRespawnTime = 0;
	m_bItem = false;
	m_nItemType = 0;
	m_nAI_BulletEva = 0;
	m_nAI_MassEva = 0;
	m_nAI_DownEva = 0;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEnemy_ListData::~CEnemy_ListData()
{

}

//=============================================================================
//    データをコピーする処理
//=============================================================================
void CEnemy_ListData::Cpy(CEnemy_ListData EnemyData)
{
	m_nRespawnIdx = EnemyData.GetRespawnIdx();
	m_nEnemyType = EnemyData.GetEnemyType();
	m_nRespawnTime = EnemyData.GetRespawnTime();
	m_bItem = EnemyData.GetItem();
	m_nItemType = EnemyData.GetItemType();
	m_nAI_BulletEva = EnemyData.GetAI_BulletEva();
	m_nAI_MassEva = EnemyData.GetAI_MassEva();
	m_nAI_DownEva = EnemyData.GetAI_DownEva();
}

//=============================================================================
//    敵のリスポーン位置の番号を設定する
//=============================================================================
void CEnemy_ListData::SetRespawnIdx(const int nRespawnIdx)
{
	m_nRespawnIdx = nRespawnIdx;
}

//=============================================================================
//    敵の種類番号を設定する
//=============================================================================
void CEnemy_ListData::SetEnemyType(const int nEnemyType)
{
	m_nEnemyType = nEnemyType;
}

//=============================================================================
//    敵を出現させるタイミングを設定する
//=============================================================================
void CEnemy_ListData::SetRespawnTime(const int nRespawnTime)
{
	m_nRespawnTime = nRespawnTime;
}

//=============================================================================
//    倒した時にアイテムを出現させるかどうか設定する
//=============================================================================
void CEnemy_ListData::SetItem(const bool bItem)
{
	m_bItem = bItem;
}

//=============================================================================
//    出現させるアイテムの種類番号を設定する
//=============================================================================
void CEnemy_ListData::SetItemType(const int nItemType)
{
	m_nItemType = nItemType;
}

//=============================================================================
//    弾を打つ際のランダム評価値を設定する
//=============================================================================
void CEnemy_ListData::SetAI_BulletEva(const int nAI_BulletEva)
{
	m_nAI_BulletEva = nAI_BulletEva;
}

//=============================================================================
//    マスを移動した際に向きを変えさせるか判定する時のランダム評価値を設定する
//=============================================================================
void CEnemy_ListData::SetAI_MassEva(const int nAI_MassEva)
{
	m_nAI_MassEva = nAI_MassEva;
}

//=============================================================================
//    下向きに動かすかどうかを判定する際のランダム評価値を設定する
//=============================================================================
void CEnemy_ListData::SetAI_DownEva(const int nAI_DownEva)
{
	m_nAI_DownEva = nAI_DownEva;
}

//=============================================================================
//    敵のリスポーン位置の番号を取得する
//=============================================================================
int CEnemy_ListData::GetRespawnIdx(void)
{
	return m_nRespawnIdx;
}

//=============================================================================
//    敵の種類番号を取得する
//=============================================================================
int CEnemy_ListData::GetEnemyType(void)
{
	return m_nEnemyType;
}

//=============================================================================
//    敵を出現させるタイミングを取得する
//=============================================================================
int CEnemy_ListData::GetRespawnTime(void)
{
	return m_nRespawnTime;
}

//=============================================================================
//    倒した時にアイテムを出現させるかどうか取得する
//=============================================================================
bool CEnemy_ListData::GetItem(void)
{
	return m_bItem;
}

//=============================================================================
//    出現させるアイテムの種類番号を取得する
//=============================================================================
int CEnemy_ListData::GetItemType(void)
{
	return m_nItemType;
}

//=============================================================================
//    弾を打つ際のランダム評価値を取得する
//=============================================================================
int CEnemy_ListData::GetAI_BulletEva(void)
{
	return m_nAI_BulletEva;
}

//=============================================================================
//    マスを移動した際に向きを変えさせるか判定する時のランダム評価値を取得する
//=============================================================================
int CEnemy_ListData::GetAI_MassEva(void)
{
	return m_nAI_MassEva;
}

//=============================================================================
//    下向きに動かすかどうかを判定する際のランダム評価値を取得する
//=============================================================================
int CEnemy_ListData::GetAI_DownEva(void)
{
	return m_nAI_DownEva;
}