//*****************************************************************************
//
//     キャラクター管轄の処理[characterManager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "characterManager.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "modelcreate.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "functionlib.h"
#include "object.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
//#define CHARAMANAGER_LOAD_TEXTURE                 // 宣言時テクスチャを読み込む
#define CHARAMANAGER_LOAD_MODEL                   // 宣言時モデルを読み込む
#define CHARAMANAGER_LOAD_CHARACTER               // 宣言時キャラクターデータを読み込む
#define CHARAMANAGER_LOAD_MOTION                  // 宣言時モーションを読み込む
#define CHARAMANAGER_MAX_LOAD_MOTIONDATA (30)     // 読み込めるモーションデータの最大数

// 値読み込みをする際の目印となる文字列
// 共通
#define POS               "POS = "                // 座標を読み取る合図
#define ROT               "ROT = "                // 向きを読み取る合図
#define MOVE              "MOVE = "               // 移動量を読み取る合図

// その他
#define NUM_TEXTURE       "NUM_TEXTURE = "        // 読み込むテクスチャの数
#define TEXTURE_FILENAME  "TEXTURE_FILENAME = "   // 読み込むテクスチャのファイル名
#define NUM_MODEL         "NUM_MODEL = "          // 読み込むモデルの数
#define MODEL_FILENAME    "MODEL_FILENAME = "     // 読み込むモデルのファイル名

// キャラクター情報
#define CHARACTERSET      "CHARACTERSET"          // キャラクター情報読み込み開始の合図
#define END_CHARACTERSET  "END_CHARACTERSET"      // キャラクター情報読み込み終了の合図
#define JUMP              "JUMP = "               // ジャンプ量
#define GRAVITY           "GRAVITY = "            // 重力量
#define INERTIA           "INERTIA = "            // 慣性量
#define RIVISION_ROT      "RIVISION_ROT = "       // 向きを補正する倍率
#define COL_HEIGHT        "COL_HEIGHT = "         // 当たり判定をとる高さ
#define COL_RANGE         "COL_RANGE = "          // 当たり判定をとる範囲
#define NUM_PARTS         "NUM_PARTS = "          // キャラクターのパーツ数

// パーツ情報
#define PARTSSET          "PARTSSET"              // パーツ情報読み込み開始の合図
#define END_PARTSSET      "END_PARTSSET"          // パーツ情報読み込み終了の合図
#define INDEX             "INDEX = "              // 番号を読み取る合図
#define PARENT            "PARENT = "             // 親パーツの番号を読み取る合図

// モーション情報
#define MOTIONSET         "MOTIONSET"             // モーション情報読み込み開始の合図
#define END_MOTIONSET     "END_MOTIONSET"         // モーション情報読み込み終了の合図
#define COLLISIONSET      "COLLISIONSET"          // モーションの当たり判定情報読み込み開始の合図
#define END_COLLISIONSET  "END_COLLISIONSET"      // モーションの当たり判定情報読み込み終了の合図
#define ORBITSET          "ORBITSET"              // モーションの軌跡情報読み込み開始の合図
#define END_ORBITSET      "END_ORBITSET"          // モーションの軌跡情報読み込み終了の合図
#define RANGE             "RANGE = "              // 範囲を読み取る合図
#define XBLOCK            "XBLOCK = "             // 軌跡の横の分割数を読み取る合図
#define ZBLOCK            "ZBLOCK = "             // 軌跡の奥行の分割数を読み取る合図
#define OFFSET            "OFFSET = "             // オフセットを読み取る合図
#define LOOP              "LOOP = "               // モーションがループ再生するかどうかを読み取る合図
#define BLEND             "BLEND = "              // モーションブレンドの倍率を読み取る合図
#define NUM_KEY           "NUM_KEY = "            // モーションのキーフレームの数を読み取る合図

// 攻撃モーション情報
#define ATTACKSET         "ATTACKSET"             // 攻撃モーション情報読み込み開始の合図
#define END_ATTACKSET     "END_ATTACKSET"         // 攻撃モーション情報読み込み終了の合図
#define NEXT_MOTION       "NEXT_MOTION = "        // 次のモーションの番号

// 当たり判定情報
#define NUM_COL           "NUM_COL = "            // モーションの当たり判定情報の数を読み取る合図
#define COLLISION         "COLLISION"             // 当たり判定情報読み込み開始の合図
#define DAMAGE            "DAMAGE = "             // 当たった場合のダメージ
#define END_COLLISION     "END_COLLISION"         // 当たり判定情報読み込み終了の合図

// 軌跡情報
#define NUM_ORBIT         "NUM_ORBIT = "          // モーションの軌跡情報の数を読み取る合図
#define ORBIT		      "ORBIT"                 // 軌跡情報読み込み開始の合図
#define END_ORBIT	      "END_ORBIT"             // 軌跡情報読み込み終了の合図
#define TEX_INDEX         "TEX_INDEX = "          // 軌跡に張り付けるテクスチャの番号を読み取る合図
#define OFFSET1           "OFFSET1 = "            // １個目のオフセットを読み取る合図
#define OFFSET2           "OFFSET2 = "            // ２個目のオフセットを読み取る合図
#define OFFSETAMP1        "OFFSETAMP1 = "         // 軌跡のオフセット距離増幅値を読み取る合図(１個目)
#define OFFSETAMP2        "OFFSETAMP2 = "         // 軌跡のオフセット距離増幅値を読み取る合図(２個目)
#define COLUP             "COLUP = "              // 軌跡の頂点上側の色
#define COLDOWN           "COLDOWN = "            // 軌跡の頂点下側の色
#define ALPHA_DECAY_UP    "ALPHA_DECAY_UP = "     // 軌跡の頂点上側の透明度減衰値
#define ALPHA_DECAY_DOWN  "ALPHA_DECAY_DOWN = "   // 軌跡の頂点下側の透明度減衰値
#define START             "START = "              // 軌跡を出し始めるタイミングを読み取る合図
#define FINISH            "FINISH = "             // 軌跡をしまうタイミングを読み取る合図

// キーフレーム情報
#define KEYSET            "KEYSET"                // キーフレーム情報読み込み開始の合図
#define END_KEYSET        "END_KEYSET"            // キーフレーム情報読み込み終了の合図
#define FRAME             "FRAME = "              // キーフレームの再生フレーム数を読み取る合図

// キー情報
#define KEY               "KEY"                   // キー情報読み込み開始の合図
#define END_KEY           "END_KEY"               // キー情報読み込み終了の合図

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CCharacterManager::CCharacterManager()
{
	// 各種値のクリア
	strcpy(m_aFileName, "\0");    // プレイヤーデータのスクリプトファイル名
	m_nNumMotionData = 0;         // モーションデータを読み込んだ回数
	m_pModelCreate = NULL;        // モデル管轄クラスへのポインタ
	m_pTextureManager = NULL;     // テクスチャ管轄クラスへのポインタ
	m_pParent = NULL;             // 親モデルの番号
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCharacterManager::~CCharacterManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CCharacterManager *CCharacterManager::Create(char *pFileName)
{
	CCharacterManager *pCharacterManager = NULL;  // キャラクター管轄クラス型のポインタ
	if (pCharacterManager == NULL)
	{// メモリが空になっている
		pCharacterManager = new CCharacterManager;
		if (pCharacterManager != NULL)
		{// インスタンスを生成できた
			pCharacterManager->SetFileName(pFileName);
			if (FAILED(pCharacterManager->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pCharacterManager;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CCharacterManager::Init(void)
{
	// ファイルオープン
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = CFileLoader::Create(m_aFileName);
	if (pFileLoader == NULL)
	{
		return E_FAIL;
	}

	// ファイルを読み進める
	strcpy(aStr, pFileLoader->GetString(aStr));
	if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
	{// スクリプト読み込み開始の合図だった
		LoadScript(pFileLoader, aStr);
	}

	// ファイルを閉じる
	if (pFileLoader != NULL)
	{
		pFileLoader->Uninit();
		delete pFileLoader;
		pFileLoader = NULL;
	}


	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CCharacterManager::Uninit(void)
{
	// 親モデルの番号の破棄
	if (m_pParent != NULL)
	{
		delete[] m_pParent;
		m_pParent = NULL;
	}

	// モデル管轄クラスの破棄
	if (m_pModelCreate != NULL)
	{
		m_pModelCreate->Uninit();
		delete m_pModelCreate;
		m_pModelCreate = NULL;
	}

	// モーション管轄クラスの破棄
	if (m_pMotionManager != NULL)
	{
		m_pMotionManager->Uninit();
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// モデルクラスの破棄
	if (m_apModel != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			m_apModel[nCntModel]->Uninit();
			delete m_apModel[nCntModel];
			m_apModel[nCntModel] = NULL;
		}

		delete[] m_apModel;
		m_apModel = NULL;
	}

	// テクスチャ管轄クラスの破棄
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    配置物を作成する処理
//=============================================================================
CObject *CCharacterManager::SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CModel **apModelCpy = NULL;                // モデルクラスコピー用
	CMotionManager *pMotionManagerCpy = NULL;  // モーション管轄クラスコピー用

	// モデルの数だけメモリ確保
	apModelCpy = new CModel*[m_nNumModel];
	if (apModelCpy == NULL) return NULL;
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		// メモリ確保
		apModelCpy[nCntModel] = NULL;
		apModelCpy[nCntModel] = new CModel;

		// 値コピー
		if (apModelCpy[nCntModel] != NULL)
		{
			apModelCpy[nCntModel]->Cpy(m_apModel[nCntModel]);
			if(m_pParent[nCntModel] != -1)
			{
				apModelCpy[nCntModel]->SetParent(apModelCpy[m_pParent[nCntModel]]);
			}
		}
	}

	// モーションデータをコピーする
	if(m_nNumMotionData > 0)
	{
		pMotionManagerCpy = CMotionManager::Create(m_nNumMotionData, m_nNumModel);
		for (int nCntMotion = 0; nCntMotion < m_nNumMotionData; nCntMotion++)
		{
			pMotionManagerCpy->CpyMotion(m_pMotionManager->GetMotion()[nCntMotion], nCntMotion);
		}
	}

	// 配置物を生成する
	CObject *pObject = CObject::Create(pos, rot, nType, nPriority);
	if (pObject != NULL)
	{
		pObject->SetNumModel(m_nNumModel);
		pObject->SetModel(apModelCpy);
		pObject->SetMotionManager(pMotionManagerCpy);
	}

	return pObject;
}

//=============================================================================
//    スクリプトファイルを読み込む処理
//=============================================================================
HRESULT CCharacterManager::LoadScript(CFileLoader *pFileLoader, char *pStr)
{
#ifdef CHARAMANAGER_LOAD_TEXTURE
	// テクスチャを読み込む
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
	{// 読み込むテクスチャ数情報だった
		if (FAILED(LoadTextureSet(pFileLoader, pStr)))
		{
			return E_FAIL;
		}
	}
#endif

#ifdef CHARAMANAGER_LOAD_MODEL
	// モデルを読み込む
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
	{// 読み込むモデル数情報だった
		if (FAILED(LoadModelSet(pFileLoader, pStr)))
		{
			return E_FAIL;
		}
	}
#endif

#ifdef CHARAMANAGER_LOAD_CHARACTER
	// キャラクター情報を読み込む
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, CHARACTERSET) == 0)
	{// キャラクター情報読み込み開始の合図だった
		if (FAILED(LoadCharacterSet(pFileLoader, pStr)))
		{
			return E_FAIL;
		}
	}
#endif

#ifdef CHARAMANAGER_LOAD_MOTION
	// モーション情報を読み込む
	CMotion *apMotion[CHARAMANAGER_MAX_LOAD_MOTIONDATA] = {};
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MOTIONSET) == 0)
		{// モーション情報読み込み開始の合図だった
			if (FAILED(LoadMotionSet(pFileLoader, pStr, apMotion)))
			{
				return E_FAIL;
			}
			m_nNumMotionData++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル読み込み終了の合図だった
			break;
		}
	}

	// モーション管轄クラスを生成
	if (m_nNumMotionData > 0)
	{// モーションデータが読み込まれている
		m_pMotionManager = CMotionManager::Create(m_nNumMotionData, m_CharaData.nNumParts);
		if (m_pMotionManager == NULL)
		{
			delete m_pMotionManager;
			m_pMotionManager = NULL;
			return E_FAIL;
		}

		// モーションへのポインタを設定する
		for (int nCntMotion = 0; nCntMotion < m_nNumMotionData; nCntMotion++)
		{
			m_pMotionManager->CpyMotion(apMotion[nCntMotion], nCntMotion);
		}

		// 読み込み用のメモリを開放
		for (int nCntMotion = 0; nCntMotion < CHARAMANAGER_MAX_LOAD_MOTIONDATA; nCntMotion++)
		{
			if (apMotion[nCntMotion] != NULL)
			{
				apMotion[nCntMotion]->Uninit(m_CharaData.nNumParts);
				delete apMotion[nCntMotion];
				apMotion[nCntMotion] = NULL;
			}
		}
	}
#endif

	return S_OK;
}

//=============================================================================
//    テクスチャ情報を読み込む処理
//=============================================================================
HRESULT CCharacterManager::LoadTextureSet(CFileLoader *pFileLoader, char *pStr)
{
	// 読み込むテクスチャの数を設定
	m_nNumTexture = CFunctionLib::ReadInt(pStr, NUM_TEXTURE);

	// 読み込みに必要なメモリを確保
	m_pTextureManager = CTextureManager::Create(m_nNumTexture);
	if (m_pTextureManager == NULL)
	{
		delete m_pTextureManager;
		m_pTextureManager = NULL;
		return E_FAIL;
	}

	// テクスチャ読み込み開始
	for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// 読み込むテクスチャのファイル情報だった
			LoadTexture(pStr, nCntTex);
		}
	}

	return S_OK;
}

//=============================================================================
//    テクスチャを読み込む処理
//=============================================================================
HRESULT CCharacterManager::LoadTexture(char *pStr, int nCntTex)
{
	// テクスチャ読み込み用変数宣言
	LPDIRECT3DTEXTURE9 pTexture = NULL;   // テクスチャへのポインタ

	// テクスチャのファイルパス名を読み取る
	pStr = CFunctionLib::ReadString(pStr, pStr, TEXTURE_FILENAME);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);

	// テクスチャ管轄クラスに値を設定する
	m_pTextureManager->SetTexture(pTexture, nCntTex);
	m_pTextureManager->SetFileName(pStr, nCntTex);

	return S_OK;
}

//=============================================================================
//    モデル情報を読み込む処理
//=============================================================================
HRESULT CCharacterManager::LoadModelSet(CFileLoader *pFileLoader, char *pStr)
{
	// 読み込むモデル数を設定
	m_nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);

	// 読み込みに必要なメモリを確保
	m_pModelCreate = CModelCreate::Create(m_nNumModel);
	if (m_pModelCreate == NULL)
	{
		delete m_pModelCreate;
		m_pModelCreate = NULL;
		return E_FAIL;
	}

	// モデル読み込み開始
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// 読み込むモデルのファイル情報だった
			LoadModel(pStr, nCntModel);
		}
	}

	return S_OK;
}

//=============================================================================
//    モデルを読み込む処理
//=============================================================================
HRESULT CCharacterManager::LoadModel(char *pStr, int nCntModel)
{
	// モデル読み込み用変数宣言
	LPD3DXMESH pMesh = NULL;       // メッシュへのポインタ
	LPD3DXBUFFER pBuffMat = NULL;  // マテリアル情報へのポインタ
	DWORD nNumMat = 0;             // マテリアル情報の数

	// モデルのファイルパス名を読み取る
	pStr = CFunctionLib::ReadString(pStr, pStr, MODEL_FILENAME);

	// xファイルの読み込み
	D3DXLoadMeshFromX(pStr, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// モデル管轄クラスに値を設定する
	m_pModelCreate->SetMesh(pMesh, nCntModel);
	m_pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);
	m_pModelCreate->SetFileName(pStr, nCntModel);

	return S_OK;
}

//=============================================================================
//    キャラクター情報を読み込む処理
//=============================================================================
HRESULT CCharacterManager::LoadCharacterSet(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MOVE) == 0)
		{// 移動量だった
			m_CharaData.fAccel = CFunctionLib::ReadFloat(pStr, MOVE);
		}
		else if (CFunctionLib::Memcmp(pStr, INERTIA) == 0)
		{// 慣性量だった
			m_CharaData.fInertia = CFunctionLib::ReadFloat(pStr, INERTIA);
		}
		else if (CFunctionLib::Memcmp(pStr, RIVISION_ROT) == 0)
		{// 向きを補正する倍率だった
			m_CharaData.fRivisionRot = CFunctionLib::ReadFloat(pStr, RIVISION_ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_PARTS) == 0)
		{// 使用するパーツ数情報だった
			m_CharaData.nNumParts = CFunctionLib::ReadInt(pStr, NUM_PARTS);
			break;
		}
	}

	// パーツ情報を読み込む
	m_apModel = new CModel*[m_CharaData.nNumParts];
	m_pParent = new int[m_CharaData.nNumParts];
	for (int nCntParts = 0; nCntParts < m_CharaData.nNumParts; nCntParts++)
	{
		m_apModel[nCntParts] = NULL;
		m_pParent[nCntParts] = -1;
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, PARTSSET) == 0)
		{// パーツ情報読み込み開始の合図だった
			if (FAILED(LoadPartsSet(pFileLoader, pStr, nCntParts)))
			{
				return E_FAIL;
			}
		}
	}

	// キャラクター読み込み終了
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_CHARACTERSET) != 0)
	{// キャラクター情報読み込み終了の合図だった
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    パーツ情報を読み取る処理
//=============================================================================
HRESULT CCharacterManager::LoadPartsSet(CFileLoader *pFileLoader, char *pStr, int nCntParts)
{
	int nModelIdx = 0;                                     // パーツ番号情報
	D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // モデルのオフセット位置情報
	D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // モデルのオフセット向き情報

	// パーツ番号を読み込む
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, INDEX) == 0)
	{
		nModelIdx = CFunctionLib::ReadInt(pStr, INDEX);
	}

	// 親モデルの番号を読み込む
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, PARENT) == 0)
	{
		m_pParent[nCntParts] = CFunctionLib::ReadInt(pStr, PARENT);
	}

	// オフセット位置情報を読み込む
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, POS) == 0)
	{
		ModelPos = CFunctionLib::ReadVector3(pStr, POS);
	}

	// オフセット向き情報を読み込む
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, ROT) == 0)
	{
		ModelRot = CFunctionLib::ReadVector3(pStr, ROT);
	}

	// モデルを生成する
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_PARTSSET) == 0)
	{// パーツ情報読み込み終了の合図だった
		m_apModel[nCntParts] = CModel::Create(ModelPos, ModelRot, m_pModelCreate->GetMesh(nModelIdx),
			m_pModelCreate->GetBuffMat(nModelIdx), m_pModelCreate->GetNumMat(nModelIdx),
			m_pModelCreate->GetTexture(nModelIdx),
			m_pModelCreate->GetVtxMax(nModelIdx), m_pModelCreate->GetVtxMin(nModelIdx));
		if (m_apModel[nCntParts] != NULL && m_pParent[nCntParts] != -1)
		{// ポインタが確保でき親モデルが存在する
			m_apModel[nCntParts]->SetParent(m_apModel[m_pParent[nCntParts]]);
		}
	}

	return S_OK;
}

//=============================================================================
//    モーション情報を読み取る処理
//=============================================================================
HRESULT CCharacterManager::LoadMotionSet(CFileLoader *pFileLoader, char *pStr, CMotion **apMotion)
{
	int nCntKeySet = 0;                           // キーフレームフレーム情報を読み込んだ回数
	CMotionAttack *pMotionAttack = NULL;          // 攻撃モーションデータクラスへのポインタ
	CMotionCollision **pMotionCollision = NULL;   // モーションの当たり判定データクラスへのポインタ
	int nNumColData = 0;                          // 当たり判定データの数
	CMotionOrbit **pMotionOrbit = NULL;           // モーションの軌跡データクラスへのポインタ
	int nNumOrbitData = 0;                        // 軌跡データの数
	bool bLoop = false;                           // ループするかしないか
	float fBlendMag = 0.0f;                       // ブレンド倍率
	int nNumKey = 0;                              // キーフレームの総数
	int *pPlaybackKey = NULL;                     // 再生フレーム数(複数あるので動的確保)
	D3DXVECTOR3 **pAddPos = NULL;                 // モデルの座標
	D3DXVECTOR3 **pDestRot = NULL;                // モデルの向き

	// ループするかどうかを読み込み
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, LOOP) == 0)
	{// ループするかしないかの情報がある
		bLoop = CFunctionLib::ReadBool(pStr, LOOP);
	}

	// ブレンド倍率を読み込み
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, BLEND) == 0)
	{// ブレンド倍率の情報がある
		fBlendMag = CFunctionLib::ReadFloat(pStr, BLEND);
	}

	// キーフレーム数を読み込み
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, NUM_KEY) == 0)
	{// キーフレームの総数の情報がある
		nNumKey = (CFunctionLib::ReadInt(pStr, NUM_KEY));
	}

	// 読み込みに必要なメモリを確保
	if (nNumKey >= 1)
	{// 1個以上キーフレームがある
	    // 必要な変数を動的に確保する
	    // 再生フレーム数
		pPlaybackKey = new int[nNumKey];

		// 座標
		if (pAddPos == NULL)
		{// メモリが確保できる状態である
			pAddPos = new D3DXVECTOR3*[nNumKey];
			if (pAddPos != NULL)
			{// メモリが確保できた
				for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
				{// キーフレームの総数分繰り返し
					pAddPos[nCntKey] = new D3DXVECTOR3[m_CharaData.nNumParts];
				}
			}
		}

		if (pDestRot == NULL)
		{// メモリが確保できる状態である
		 // 向き
			pDestRot = new D3DXVECTOR3*[nNumKey];
			if (pDestRot != NULL)
			{// メモリが確保できた
				for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
				{// キーフレームの総数分繰り返し
					pDestRot[nCntKey] = new D3DXVECTOR3[m_CharaData.nNumParts];
				}
			}
		}
	}


	// モーションデータを読み込み
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, ATTACKSET) == 0)
		{// 当たり判定情報読み込み開始の合図だった
			pMotionAttack = LoadMotionAttackSet(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, COLLISIONSET) == 0)
		{// 当たり判定情報読み込み開始の合図だった
			pMotionCollision = LoadMotionCollisionSet(pFileLoader, pStr, &nNumColData);
		}
		else if (CFunctionLib::Memcmp(pStr, ORBITSET) == 0)
		{// 軌跡情報読み込み開始の合図だった
			pMotionOrbit = LoadMotionOrbitSet(pFileLoader, pStr, &nNumOrbitData);
		}
		else if (CFunctionLib::Memcmp(pStr, KEYSET) == 0)
		{// キーフレーム情報読み込み開始の合図だった
			LoadKeySet(pFileLoader, pStr, nCntKeySet, pPlaybackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_MOTIONSET) == 0)
		{// モーション情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	apMotion[m_nNumMotionData] = CMotion::Create(m_CharaData.nNumParts, nNumKey, fBlendMag, bLoop, pPlaybackKey);
	if (apMotion[m_nNumMotionData] != NULL)
	{// メモリが確保できた
	    // 攻撃モーションデータクラスを設定する
		apMotion[m_nNumMotionData]->SetAttack(pMotionAttack);

		// 当たり判定データクラスを設定する
		apMotion[m_nNumMotionData]->SetCollision(pMotionCollision);
		apMotion[m_nNumMotionData]->SetNumColData(nNumColData);

		// 軌跡データクラスを設定する
		apMotion[m_nNumMotionData]->SetOrbit(pMotionOrbit);
		apMotion[m_nNumMotionData]->SetNumOrbitData(nNumOrbitData);

		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// キーフレームの総数分繰り返し
			for (int nCntParts = 0; nCntParts < m_CharaData.nNumParts; nCntParts++)
			{// キーフレームの総数分繰り返し
				apMotion[m_nNumMotionData]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
				apMotion[m_nNumMotionData]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
			}
		}
	}

	// メモリを確保した分開放する
	// 再生フレーム数
	if (pPlaybackKey != NULL)
	{// メモリが確保されている
		delete[] pPlaybackKey;
		pPlaybackKey = NULL;
	}

	// 座標
	if (pAddPos != NULL)
	{// メモリが確保できた
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// キーフレームの総数分繰り返し
			delete[] pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	// 向き
	if (pDestRot != NULL)
	{// メモリが確保できた
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// キーフレームの総数分繰り返し
			delete[] pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return S_OK;
}

//=============================================================================
//    モーションの攻撃モーション情報を読み取る処理
//=============================================================================
CMotionAttack *CCharacterManager::LoadMotionAttackSet(CFileLoader *pFileLoader, char *pStr)
{
	CMotionAttack *pMotionAttack = NULL;   // 攻撃モーションデータクラスへのポインタ
	pMotionAttack = new CMotionAttack;     // メモリを確保する
	if (pMotionAttack == NULL)
	{
		delete pMotionAttack;
		return NULL;
	}

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NEXT_MOTION) == 0)
		{// 次のモーションの番号情報だった
			pMotionAttack->SetNextMotionIdx(CFunctionLib::ReadInt(pStr, NEXT_MOTION));
		}
		else if (CFunctionLib::Memcmp(pStr, START) == 0)
		{// 軌跡を出し始めるタイミング情報だった
			pMotionAttack->SetStartTiming(CFunctionLib::ReadInt(pStr, START));
		}
		else if (CFunctionLib::Memcmp(pStr, FINISH) == 0)
		{// 軌跡をしまうタイミング情報だった
			pMotionAttack->SetFinishTiming(CFunctionLib::ReadInt(pStr, FINISH));
		}
		else if (CFunctionLib::Memcmp(pStr, END_ATTACKSET) == 0)
		{// 攻撃モーションデータ読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pMotionAttack;
}

//=============================================================================
//    モーションの当たり判定情報を読み取る処理
//=============================================================================
CMotionCollision **CCharacterManager::LoadMotionCollisionSet(CFileLoader *pFileLoader, char *pStr, int *pNumColData)
{
	CMotionCollision **pMotionCollision = NULL;  // モーションの当たり判定データクラスへのポインタ

	// 当たり判定情報の数を読み込み
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, NUM_COL) == 0)
	{// 当たり判定データの数がある
		*pNumColData = CFunctionLib::ReadInt(pStr, NUM_COL);
	}

	// データ読み込みに必要なメモリを確保
	if (*pNumColData >= 1)
	{// 当たり判定データが１個以上ある
		pMotionCollision = new CMotionCollision*[*pNumColData];
		if (pMotionCollision != NULL)
		{// メモリが確保できた
			for (int nCntCol = 0; nCntCol < *pNumColData; nCntCol++)
			{// 当たり判定データの数だけ繰り返し
				pMotionCollision[nCntCol] = NULL;
				pMotionCollision[nCntCol] = new CMotionCollision;
			}
		}
	}

	// データ読み込み開始
	for (int nCntCol = 0; nCntCol < *pNumColData; nCntCol++)
	{// 当たり判定データの数だけ繰り返し
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// 当たり判定データ読み込み開始の合図だった
			// 当たり判定を取るモデルの番号を読み込む
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, INDEX) == 0)
			{
				pMotionCollision[nCntCol]->SetModelIdx(CFunctionLib::ReadInt(pStr, INDEX));
			}

			// 当たり判定を取るモデルからのオフセット距離情報を読み込む
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, OFFSET) == 0)
			{// 当たり判定を取るモデルからのオフセット距離情報だった
				pMotionCollision[nCntCol]->SetOffsetPos(CFunctionLib::ReadVector3(pStr, OFFSET));
			}

			// 当たり判定を取る範囲情報読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, RANGE) == 0)
			{
				pMotionCollision[nCntCol]->SetRange(CFunctionLib::ReadFloat(pStr, RANGE));
			}

			// 当たり判定をし始めるタイミング情報読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, START) == 0)
			{
				pMotionCollision[nCntCol]->SetStartTiming(CFunctionLib::ReadInt(pStr, START));
			}

			// 当たり判定を終了するタイミング情報読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, FINISH) == 0)
			{
				pMotionCollision[nCntCol]->SetFinishTiming(CFunctionLib::ReadInt(pStr, FINISH));
			}

			// 当たった時のダメージ量情報読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, DAMAGE) == 0)
			{
				pMotionCollision[nCntCol]->SetDamage(CFunctionLib::ReadInt(pStr, DAMAGE));
			}

			// 当たり判定データ読み込み終了の合図チェック
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, END_COLLISION) != 0)
			{
				return NULL;
			}
		}
	}

	// 当たり判定情報読み込み終了の合図チェック
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_COLLISIONSET) != 0)return NULL;

	return pMotionCollision;
}

//=============================================================================
//    モーションの軌跡情報を読み取る処理
//=============================================================================
CMotionOrbit **CCharacterManager::LoadMotionOrbitSet(CFileLoader *pFileLoader, char *pStr, int *pNumOrbitData)
{
	CMotionOrbit **pMotionOrbit = NULL;  // モーションの軌跡データクラスへのポインタ

	// 軌跡データの数を読み込む
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, NUM_ORBIT) == 0)
	{// 軌跡データの数がある
		*pNumOrbitData = CFunctionLib::ReadInt(pStr, NUM_ORBIT);
	}

	// 読み込みに必要なメモリを確保
	if (*pNumOrbitData >= 1)
	{// 当たり判定データが１個以上ある
		pMotionOrbit = new CMotionOrbit*[*pNumOrbitData];
		if (pMotionOrbit != NULL)
		{// メモリが確保できた
			for (int nCntOrbit = 0; nCntOrbit < *pNumOrbitData; nCntOrbit++)
			{// 当たり判定データの数だけ繰り返し
				pMotionOrbit[nCntOrbit] = NULL;
				pMotionOrbit[nCntOrbit] = new CMotionOrbit;
			}
		}
	}

	// データ読み込み開始
	for (int nCntOrbit = 0; nCntOrbit < *pNumOrbitData; nCntOrbit++)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, ORBIT) == 0)
		{// 当たり判定データ読み込み開始の合図だった
		    // 軌跡を出すモデルの番号情報読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, INDEX) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetModelIdx(CFunctionLib::ReadInt(pStr, INDEX));
			}

			// 軌跡に張り付けるテクスチャの番号情報読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, TEX_INDEX) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetTexIdx(CFunctionLib::ReadInt(pStr, TEX_INDEX));
			}

			// 軌跡を出すモデルからのオフセット距離情報(１個目)読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, OFFSET1) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetOffsetPos1(CFunctionLib::ReadVector3(pStr, OFFSET1));
			}

			// 軌跡を出すモデルからのオフセット距離情報(２個目)読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, OFFSET2) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetOffsetPos2(CFunctionLib::ReadVector3(pStr, OFFSET2));
			}

			// 軌跡のオフセット距離増幅値(1個目)読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, OFFSETAMP1) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetOffsetAmp1(CFunctionLib::ReadVector3(pStr, OFFSETAMP1));
			}

			// 軌跡のオフセット距離増幅値(2個目)読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, OFFSETAMP2) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetOffsetAmp2(CFunctionLib::ReadVector3(pStr, OFFSETAMP2));
			}

			// 軌跡の頂点上側の色読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, COLUP) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetColUp(CFunctionLib::ReadVector4(pStr, COLUP));
			}

			// 軌跡の頂点下側の色読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, COLDOWN) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetColDown(CFunctionLib::ReadVector4(pStr, COLDOWN));
			}

			// 軌跡の頂点上側の透明度減衰値読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, ALPHA_DECAY_UP) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(CFunctionLib::ReadFloat(pStr, ALPHA_DECAY_UP));
			}

			// 軌跡の頂点下側の透明度減衰値読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, ALPHA_DECAY_DOWN) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(CFunctionLib::ReadFloat(pStr, ALPHA_DECAY_DOWN));
			}

			// 軌跡の横の分割数情報読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetXBlock(CFunctionLib::ReadInt(pStr, XBLOCK));
			}

			// 軌跡の縦の分割数情報読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, ZBLOCK) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetYBlock(CFunctionLib::ReadInt(pStr, ZBLOCK));
			}

			// 軌跡を出し始めるタイミング情報読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, START) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetStartTiming(CFunctionLib::ReadInt(pStr, START));
			}

			// 軌跡をしまうタイミング情報読み込み
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, FINISH) == 0)
			{
				pMotionOrbit[nCntOrbit]->SetFinishTiming(CFunctionLib::ReadInt(pStr, FINISH));
			}

			// 軌跡データ読み込み終了の合図チェック
			strcpy(pStr, pFileLoader->GetString(pStr));
			if (CFunctionLib::Memcmp(pStr, END_ORBIT) != 0) return NULL;
		}
	}

	// 軌跡情報読み込み終了の合図チェック
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_ORBITSET) != 0) return NULL;

	return pMotionOrbit;
}

//=============================================================================
//    キーフレーム情報を読み取る処理
//=============================================================================
HRESULT CCharacterManager::LoadKeySet(CFileLoader *pFileLoader, char *pStr, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	int nCntKey = 0;

	// 再生フレーム数情報読み込み
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, FRAME) == 0)
	{// 再生フレーム数がある
		pPlaybackKey[nCntKeySet] = CFunctionLib::ReadInt(pStr, FRAME);
	}

	// キー情報読み込み
	for (int nCntParts = 0; nCntParts < m_CharaData.nNumParts; nCntParts++)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, KEY) == 0)
		{// キー情報読み込み開始の合図だった
			LoadKey(pFileLoader, pStr, nCntParts, AddPos, DestRot);
		}
	}

	// キーフレーム情報読み込み終了の合図チェック
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_KEYSET) != 0) return E_FAIL;

	return S_OK;
}

//=============================================================================
//    キー情報を読み取る処理
//=============================================================================
HRESULT CCharacterManager::LoadKey(CFileLoader *pFileLoader, char *pStr, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	// オフセット位置に加える値を読み込み
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, POS) == 0)
	{
		AddPos[nCntKey] = CFunctionLib::ReadVector3(pStr, POS);
	}

	// 目的の向きを読み込み
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, ROT) == 0)
	{
		DestRot[nCntKey] = CFunctionLib::ReadVector3(pStr, ROT);
	}

	// キー情報読み込み終了の合図チェック
	strcpy(pStr, pFileLoader->GetString(pStr));
	if (CFunctionLib::Memcmp(pStr, END_KEY) != 0) return E_FAIL;


	return S_OK;
}

//=============================================================================
//    キャラクターデータのスクリプトファイル名を設定する処理
//=============================================================================
void CCharacterManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    モデル管轄クラスへのポインタを設定する処理
//=============================================================================
void CCharacterManager::SetModelCreate(CModelCreate *pModelCreate)
{
	m_pModelCreate = pModelCreate;
}

//=============================================================================
//    キャラクターデータのスクリプトファイル名を取得する処理
//=============================================================================
char *CCharacterManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する処理
//=============================================================================
CModelCreate *CCharacterManager::GetModelCreate(void)
{
	return m_pModelCreate;
}
