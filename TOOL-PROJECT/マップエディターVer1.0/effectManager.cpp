//*****************************************************************************
//
//     エフェクト管轄の処理[effectManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "effectManager.h"
#include "manager.h"
#include "renderer.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "emitter.h"
#include "particle.h"
#include "ringEffect.h"
#include "textureManager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define EFFECT_AUTO_SAVE                                  // 宣言時 : オートセーブ
#define EFFECT_SAVE_FILENAME "data/TEXT/save_effect.txt"  // エフェクトデータを保存する外部ファイルパス名

// 値読み込みをする際の目印となる文字列
// テクスチャ
#define NUM_TEXTURE       "NUM_TEXTURE = "         // 読み込むテクスチャの数
#define TEXTURE_FILENAME  "TEXTURE_FILENAME = "    // 読み込むテクスチャのファイル名

// エミッタデータ
#define NUM_EMITTER       "NUM_EMITTER = "         // エミッタデータの個数
#define EMITTERSET        "EMITTERSET"             // エミッタデータ読み込み開始の合図
#define END_EMITTERSET    "END_EMITTERSET"         // エミッタデータ読み込み終了の合図
#define TYPE              "TYPE = "                // 使用するエフェクトの種類
#define TEX_TYPE          "TEX_TYPE = "            // 使用するテクスチャの番号
#define EFFECT_TYPE       "EFFECT_TYPE = "         // 使用するエフェクトの番号
#define RANGE_MIN         "RANGE_MIN = "           // エフェクトを生成する範囲の最小値
#define RANGE_MAX         "RANGE_MAX = "           // エフェクトを生成する範囲の最大値
#define LIFE              "LIFE = "                // エミッタの寿命
#define LAP               "LAP = "                 // １回の放出で出すエフェクトの個数
#define APPEAR            "APPEAR = "              // エフェクトを出す間隔
#define LOOP              "LOOP = "                // ループするかしないか

// パーティクルデータ
#define NUM_PARTICLE      "NUM_PARTICLE = "        // パーティクルデータの個数
#define PARTICLESET       "PARTICLESET"            // パーティクルデータ読み込み開始の合図
#define END_PARTICLESET   "END_PARTICLESET"        // パーティクルデータ読み込み終了の合図
#define ROT               "ROT = "                 // 向き
#define MAX_MOVE          "MAX_MOVE = "            // 移動量の最大値
#define MIN_MOVE          "MIN_MOVE = "            // 移動量の最小値
#define CHANGE_MOVE       "CHANGE_MOVE = "         // 移動量の変化量
#define INIT_COL          "INIT_COL = "            // 生成時の色
#define CHANGE_COL        "CHANGE_COL = "          // 毎フレームの色の変化量
#define MAX_SIZE          "MAX_SIZE = "            // 大きさの最大値
#define MIN_SIZE          "MIN_SIZE = "            // 大きさの最小値
#define SPREAD            "SPREAD = "              // 大きさの変化量
#define MAX_LIFE          "MAX_LIFE = "            // 寿命の最大値
#define MIN_LIFE          "MIN_LIFE = "            // 寿命の最小値
#define GRAVITY           "GRAVITY = "             // 毎フレームかける重力
#define BOUNCING          "BOUNCING = "            // バウンド量
#define ROT_PATTERN       "ROT_PATTERN = "         // 回転の種類[ 0:時計回り 1:反時計回り 2:ランダム ]
#define ROT_SPEED         "ROT_SPEED = "           // 回転するスピード
#define COLLISION         "COLLISION = "           // 当たり判定するかしないか[ 0:なし 1:あり ]
#define DRAW_ADDTIVE      "DRAW_ADDTIVE = "        // 加算合成するかしないか[ 0:なし 1:あり ]
#define LIGHTING          "LIGHTING = "            // ライティングするかしないか[ 0:なし 1:あり ]

// リングエフェクトデータ
#define NUM_RING          "NUM_RING = "            // リングエフェクトデータの個数
#define RINGSET           "RINGSET"                // リングエフェクトデータ読み込み開始の合図
#define MAX_ROT           "MAX_ROT = "             // 向きの最大値
#define MIN_ROT           "MIN_ROT = "             // 向きの最大値
#define HEIGHT            "HEIGHT = "              // 高さ
#define RADIUS            "RADIUS = "              // 半径
#define DIFFUSION         "DIFFUSION = "           // 幅
#define CHANGE_HEIGHT     "CHANGE_HEIGHT = "       // 高さを毎フレームどれくらい変化させるか
#define CHANGE_RADIUS     "CHANGE_RADIUS = "       // 半径を毎フレームどれくらい変化させるか
#define CHANGE_DIFFUSION  "CHANGE_DIFFUSION = "    // 幅を毎フレームどれくらい変化させるか
#define XBLOCK            "XBLOCK = "              // 横の分割数
#define YBLOCK            "YBLOCK = "              // 縦の分割数
#define CULLING           "CULLING = "             // カリングするかしないか[ 0:なし 1:あり ]
#define END_RINGSET       "END_RINGSET"            // リングエフェクトデータ読み込み終了の合図

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CEffectManager::CEffectManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	strcpy(m_aFileName, "\0");  // 読み込むスクリプトファイル名
	m_apEmitterData = NULL;     // エミッタデータクラスへのポインタ
	m_apParData = NULL;         // パーティクルデータクラスへのポインタ
	m_apRingData = NULL;        // リングエフェクトデータクラスへのポインタ
	m_nNumEmitterData = 0;      // エミッタデータの個数
	m_nNumParData = 0;          // パーティクルデータの個数
	m_nNumRingEffectData = 0;   // リングエフェクトデータの個数
	m_pTextureManager = NULL;   // テクスチャ管轄クラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEffectManager::~CEffectManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CEffectManager *CEffectManager::Create(char *pFileName)
{
	CEffectManager *pEffectManager = NULL;   // エフェクト管轄クラス型のポインタ
	if (pEffectManager == NULL)
	{// メモリが空になっている
		pEffectManager = new CEffectManager;
		if (pEffectManager != NULL)
		{// インスタンスを生成できた
			pEffectManager->SetFileName(pFileName);
			if (FAILED(pEffectManager->Init()))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}

	return pEffectManager;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CEffectManager::Init(void)
{
	// スクリプトファイルが読み込めるかチェック
	CFileLoader *pFileLoader = CFileLoader::Create(m_aFileName);
	if (pFileLoader == NULL) { return E_FAIL; }

	// スクリプトファイルを読み込む
	char aStr[256] = "\0";
	strcpy(aStr, pFileLoader->GetString(aStr));
	if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
	{// スクリプト読み込み開始の合図だった
		if (FAILED(LoadScript(pFileLoader, aStr)))
		{
			return E_FAIL;
		}
	}

	// メモリの開放
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
void CEffectManager::Uninit(void)
{
#ifdef EFFECT_AUTO_SAVE
	// エフェクトデータを保存する
	Save();
#endif

	// エミッタデータクラスの開放
	ReleaseEmitterData();

	// パーティクルデータクラスの開放
	ReleaseParticleData();

	// リングエフェクトデータクラスの開放
	ReleaseRingEffectData();

	// テクスチャ管轄クラスの開放
	ReleaseTextureManager();

	// 自身のポインタを削除
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CEffectManager::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CEffectManager::Draw(void)
{

}

//=============================================================================
//    エフェクトを設定する処理
//=============================================================================
CEmitter *CEffectManager::SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	// エミッタデータクラスからデータを取得
	CEmitter *pEmitter = NULL;
	if (m_apEmitterData[nType] != NULL)
	{// データクラスが作成されている
		int nEffectType = m_apEmitterData[nType]->GetType();
		int nTexIdx = m_apEmitterData[nType]->GetTexIdx();
		int nEffectIdx = m_apEmitterData[nType]->GetEffectIdx();
		int nLife = m_apEmitterData[nType]->GetLife();
		D3DXVECTOR3 RangeMax = m_apEmitterData[nType]->GetRangeMax();
		D3DXVECTOR3 RangeMin = m_apEmitterData[nType]->GetRangeMin();
		int nLap = m_apEmitterData[nType]->GetLap();
		int nAppear = m_apEmitterData[nType]->GetAppear();
		bool bLoop = m_apEmitterData[nType]->GetLoop();

		// エミッタクラスを作成する
		if (nEffectType == 0)
		{// 0番のエフェクトの種類(パーティクル)だったら
			pEmitter = CParEmitter::Create(pos, rot, nTexIdx, nEffectIdx, RangeMax, RangeMin, nLife, nLap, nAppear, bLoop, m_apParData[nEffectIdx], m_pTextureManager->GetTexture(nTexIdx), nType, nPriority);
		}
		else if (nEffectType == 1)
		{// 1番のエフェクトの種類(リングエフェクト)だったら
			pEmitter = CRingEmitter::Create(pos, rot, nTexIdx, nEffectIdx, RangeMax, RangeMin, nLife, nLap, nAppear, bLoop, m_apRingData[nEffectIdx], m_pTextureManager->GetTexture(nTexIdx), nType, nPriority);
		}
	}

	m_pEmitter = pEmitter;
	return pEmitter;
}

//=============================================================================
//    スクリプトファイルを読み込む
//=============================================================================
HRESULT CEffectManager::LoadScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntTex = 0;
	int nCntEmitter = 0;
	int nCntParticle = 0;
	int nCntRing = 0;

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// テクスチャの個数情報があった
			CreateTextureManager(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// テクスチャのファイルパス名情報があった
			LoadTexture(pStr, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_EMITTER) == 0)
		{// エミッタデータの個数情報がある
			CreateEmitterDataMemory(CFunctionLib::ReadInt(pStr, NUM_EMITTER));
		}
		else if (CFunctionLib::Memcmp(pStr, EMITTERSET) == 0)
		{// エミッタデータ読み込み開始の合図だった
			m_apEmitterData[nCntEmitter] = LoadEmitterData(pFileLoader, pStr, nCntEmitter);
			nCntEmitter++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_PARTICLE) == 0)
		{// パーティクルデータの個数情報がある
			CreateParticleDataMemory(CFunctionLib::ReadInt(pStr, NUM_PARTICLE));
		}
		else if (CFunctionLib::Memcmp(pStr, PARTICLESET) == 0)
		{// パーティクルデータ読み込み開始の合図だった
			m_apParData[nCntParticle] = LoadParticleData(pFileLoader, pStr, nCntParticle);
			nCntParticle++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_RING) == 0)
		{// リングエフェクトデータの個数情報がある
			CreateRingEffectDataMemory(CFunctionLib::ReadInt(pStr, NUM_RING));
		}
		else if (CFunctionLib::Memcmp(pStr, RINGSET) == 0)
		{// リングエフェクトデータ読み込み開始の合図だった
			m_apRingData[nCntRing] = LoadRingEffectData(pFileLoader, pStr, nCntRing);
			nCntRing++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプト読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return S_OK;
}

//=============================================================================
//    テクスチャを読み込む処理
//=============================================================================
void CEffectManager::LoadTexture(char *pStr, const int nCntTex)
{
	// テクスチャのファイルパス名を読み取る
	char aTexFileName[256];
	strcpy(aTexFileName, CFunctionLib::ReadString(pStr, aTexFileName, TEXTURE_FILENAME));

	// テクスチャの読み込み
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), aTexFileName, &pTexture);
	if (pTexture != NULL)
	{// テクスチャを読み込めた
		m_pTextureManager->SetTexture(pTexture, nCntTex);
		m_pTextureManager->SetFileName(aTexFileName, nCntTex);
	}
}

//=============================================================================
//    エミッタデータを読み込む処理
//=============================================================================
CEmitterData *CEffectManager::LoadEmitterData(CFileLoader *pFileLoader, char *pStr, const int nCntEmitter)
{
	// エミッタデータ用のメモリを確保する
	CEmitterData *pEmitterData = NULL;  // エミッタデータクラスへのポインタ
	pEmitterData = new CEmitterData;
	if (pEmitterData == NULL) { return NULL; }

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TYPE) == 0)
		{// エフェクトの種類情報がある
			pEmitterData->SetType(CFunctionLib::ReadInt(pStr, TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEX_TYPE) == 0)
		{// テクスチャ番号情報がある
			pEmitterData->SetTexIdx(CFunctionLib::ReadInt(pStr, TEX_TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_TYPE) == 0)
		{// 種類情報がある
			pEmitterData->SetEffectIdx(CFunctionLib::ReadInt(pStr, EFFECT_TYPE));
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE_MIN) == 0)
		{// エフェクトを生成する範囲の最小値情報がある
			pEmitterData->SetRangeMin(CFunctionLib::ReadVector3(pStr, RANGE_MIN));
		}
		else if (CFunctionLib::Memcmp(pStr, RANGE_MAX) == 0)
		{// エフェクトを生成する範囲の最大値情報がある
			pEmitterData->SetRangeMax(CFunctionLib::ReadVector3(pStr, RANGE_MAX));
		}
		else if (CFunctionLib::Memcmp(pStr, LIFE) == 0)
		{// 寿命情報がある
			pEmitterData->SetLife(CFunctionLib::ReadInt(pStr, LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, LAP) == 0)
		{// 1回の放出で生成する個数情報がある
			pEmitterData->SetLap(CFunctionLib::ReadInt(pStr, LAP));
		}
		else if (CFunctionLib::Memcmp(pStr, APPEAR) == 0)
		{// 放出する間隔情報がある
			pEmitterData->SetAppear(CFunctionLib::ReadInt(pStr, APPEAR));
		}
		else if (CFunctionLib::Memcmp(pStr, LOOP) == 0)
		{// ループするかしないか情報がある
			pEmitterData->SetLoop(CFunctionLib::ReadBool(pStr, LOOP));
		}
		else if (CFunctionLib::Memcmp(pStr, END_EMITTERSET) == 0)
		{// エミッタデータ読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pEmitterData;
}

//=============================================================================
//    パーティクルデータを読み込む処理
//=============================================================================
CParData *CEffectManager::LoadParticleData(CFileLoader *pFileLoader, char *pStr, const int nCntParData)
{
	// パーティクルデータ用のメモリを確保する
	CParData *pParData = NULL;
	pParData = new CParData;
	if (pParData == NULL) { return NULL; }
	char aStr[256];

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{// 生成時の向き情報がある
			pParData->SetMaxRot(CFunctionLib::ReadFloat(pStr, ROT));
			pStr = CFunctionLib::HeadPutout(pStr, ROT);
			int nWord = CFunctionLib::PopString(pStr, aStr);
			pStr += nWord;
			pParData->SetMinRot(CFunctionLib::ReadFloat(pStr, ""));
		}
		else if (CFunctionLib::Memcmp(pStr, MAX_MOVE) == 0)
		{// 移動量の最大値情報がある
			pParData->SetMaxMove(CFunctionLib::ReadVector3(pStr, MAX_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_MOVE) == 0)
		{// 移動量の最小値情報がある
			pParData->SetMinMove(CFunctionLib::ReadVector3(pStr, MIN_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_MOVE) == 0)
		{// 移動量の変化量情報がある
			pParData->SetChangeMove(CFunctionLib::ReadVector3(pStr, CHANGE_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, INIT_COL) == 0)
		{// 生成時の色情報がある
			pParData->SetInitCol(CFunctionLib::ReadVector4(pStr, INIT_COL));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_COL) == 0)
		{// フレーム毎の色の変化量情報がある
			pParData->SetChangeCol(CFunctionLib::ReadVector4(pStr, CHANGE_COL));
		}
		else if (CFunctionLib::Memcmp(pStr, MAX_SIZE) == 0)
		{// 大きさの最大値情報がある
			pParData->SetMaxWidth(CFunctionLib::ReadFloat(pStr, MAX_SIZE));
			pStr = CFunctionLib::HeadPutout(pStr, MAX_SIZE);
			int nWord = CFunctionLib::PopString(pStr, aStr);
			pStr += nWord;
			pParData->SetMaxHeight(CFunctionLib::ReadFloat(pStr, ""));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_SIZE) == 0)
		{// 大きさの最小値情報がある
			pParData->SetMinWidth(CFunctionLib::ReadFloat(pStr, MIN_SIZE));
			pStr = CFunctionLib::HeadPutout(pStr, MIN_SIZE);
			int nWord = CFunctionLib::PopString(pStr, aStr);
			pStr += nWord;
			pParData->SetMinHeight(CFunctionLib::ReadFloat(pStr, ""));
		}
		else if (CFunctionLib::Memcmp(pStr, SPREAD) == 0)
		{// 大きさの変化量情報がある
			pParData->SetSpread(CFunctionLib::ReadFloat(pStr, SPREAD));
		}
		else if (CFunctionLib::Memcmp(pStr, MAX_LIFE) == 0)
		{// 寿命の最大値情報がある
			pParData->SetMaxLife(CFunctionLib::ReadInt(pStr, MAX_LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_LIFE) == 0)
		{// 寿命の最小値情報がある
			pParData->SetMinLife(CFunctionLib::ReadInt(pStr, MIN_LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, BOUNCING) == 0)
		{// バウンド量情報がある
			pParData->SetBouncing(CFunctionLib::ReadFloat(pStr, BOUNCING));
		}
		else if (CFunctionLib::Memcmp(pStr, ROT_PATTERN) == 0)
		{// 回転の種類情報がある
			pParData->SetRotPattern(CFunctionLib::ReadInt(pStr, ROT_PATTERN));
		}
		else if (CFunctionLib::Memcmp(pStr, ROT_SPEED) == 0)
		{// 回転のスピード情報がある
			pParData->SetRotSpeed(CFunctionLib::ReadFloat(pStr, ROT_SPEED));
		}
		else if (CFunctionLib::Memcmp(pStr, COLLISION) == 0)
		{// 当たり判定をするかしないか情報がある
			pParData->SetCollision(CFunctionLib::ReadBool(pStr, COLLISION));
		}
		else if (CFunctionLib::Memcmp(pStr, DRAW_ADDTIVE) == 0)
		{// 加算合成するかしないか情報がある
			pParData->SetDrawAddtive(CFunctionLib::ReadBool(pStr, DRAW_ADDTIVE));
		}
		else if (CFunctionLib::Memcmp(pStr, LIGHTING) == 0)
		{// ライティングするかしないか情報がある
			pParData->SetLighting(CFunctionLib::ReadBool(pStr, LIGHTING));
		}
		else if (CFunctionLib::Memcmp(pStr, END_PARTICLESET) == 0)
		{// パーティクルデータ読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pParData;
}

//=============================================================================
//    リングエフェクトデータを読み込む処理
//=============================================================================
CRingData *CEffectManager::LoadRingEffectData(CFileLoader *pFileLoader, char *pStr, const int nCntRingEffectData)
{
	// リングエフェクトデータ用のメモリを確保する
	CRingData *pRingData = NULL;
	pRingData = new CRingData;
	if (pRingData == NULL) { return NULL; }

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MAX_ROT) == 0)
		{// 向きの最大値情報がある
			pRingData->SetMaxRot(CFunctionLib::ReadVector3(pStr, MAX_ROT));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_ROT) == 0)
		{// 向きの最小値情報がある
			pRingData->SetMinRot(CFunctionLib::ReadVector3(pStr, MIN_ROT));
		}
		else if (CFunctionLib::Memcmp(pStr, MAX_MOVE) == 0)
		{// 移動量の最大値情報がある
			pRingData->SetMaxMove(CFunctionLib::ReadVector3(pStr, MAX_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_MOVE) == 0)
		{// 移動量の最小値情報がある
			pRingData->SetMinMove(CFunctionLib::ReadVector3(pStr, MIN_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_MOVE) == 0)
		{// 移動量の変化量情報がある
			pRingData->SetChangeMove(CFunctionLib::ReadVector3(pStr, CHANGE_MOVE));
		}
		else if (CFunctionLib::Memcmp(pStr, INIT_COL) == 0)
		{// 生成時の色情報がある
			pRingData->SetInitCol(CFunctionLib::ReadVector4(pStr, INIT_COL));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_COL) == 0)
		{// フレーム毎の色の変化量情報がある
			pRingData->SetChangeCol(CFunctionLib::ReadVector4(pStr, CHANGE_COL));
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// 高さ情報がある
			pRingData->SetHeight(CFunctionLib::ReadFloat(pStr, HEIGHT));
		}
		else if (CFunctionLib::Memcmp(pStr, RADIUS) == 0)
		{// 半径情報がある
			pRingData->SetRadius(CFunctionLib::ReadFloat(pStr, RADIUS));
		}
		else if (CFunctionLib::Memcmp(pStr, DIFFUSION) == 0)
		{// 幅情報がある
			pRingData->SetDiffusion(CFunctionLib::ReadFloat(pStr, DIFFUSION));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_HEIGHT) == 0)
		{// 高さを毎フレームどれくらい変化させる情報がある
			pRingData->SetChangeHeight(CFunctionLib::ReadFloat(pStr, CHANGE_HEIGHT));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_RADIUS) == 0)
		{// 半径を毎フレームどれくらい変化させる情報がある
			pRingData->SetChangeRadius(CFunctionLib::ReadFloat(pStr, CHANGE_RADIUS));
		}
		else if (CFunctionLib::Memcmp(pStr, CHANGE_DIFFUSION) == 0)
		{// 幅を毎フレームどれくらい変化させる情報がある
			pRingData->SetChangeDiffusion(CFunctionLib::ReadFloat(pStr, CHANGE_DIFFUSION));
		}
		else if (CFunctionLib::Memcmp(pStr, MAX_LIFE) == 0)
		{// 寿命の最大値情報がある
			pRingData->SetMaxLife(CFunctionLib::ReadInt(pStr, MAX_LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, MIN_LIFE) == 0)
		{// 寿命の最小値情報がある
			pRingData->SetMinLife(CFunctionLib::ReadInt(pStr, MIN_LIFE));
		}
		else if (CFunctionLib::Memcmp(pStr, XBLOCK) == 0)
		{// 横の分割数情報がある
			pRingData->SetXBlock(CFunctionLib::ReadInt(pStr, XBLOCK));
		}
		else if (CFunctionLib::Memcmp(pStr, YBLOCK) == 0)
		{// 縦の分割数情報がある
			pRingData->SetYBlock(CFunctionLib::ReadInt(pStr, YBLOCK));
		}
		else if (CFunctionLib::Memcmp(pStr, ROT_PATTERN) == 0)
		{// 回転の種類情報がある
			pRingData->SetRotPattern(CFunctionLib::ReadInt(pStr, ROT_PATTERN));
		}
		else if (CFunctionLib::Memcmp(pStr, ROT_SPEED) == 0)
		{// 回転のスピード情報がある
			pRingData->SetRotSpeed(CFunctionLib::ReadVector3(pStr, ROT_SPEED));
		}
		else if (CFunctionLib::Memcmp(pStr, CULLING) == 0)
		{// カリングをするかしないか情報がある
			pRingData->SetCulling(CFunctionLib::ReadBool(pStr, CULLING));
		}
		else if (CFunctionLib::Memcmp(pStr, DRAW_ADDTIVE) == 0)
		{// 加算合成するかしないか情報がある
			pRingData->SetDrawAddtive(CFunctionLib::ReadBool(pStr, DRAW_ADDTIVE));
		}
		else if (CFunctionLib::Memcmp(pStr, END_RINGSET) == 0)
		{// リングエフェクトデータ読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pRingData;
}

//=============================================================================
//    エフェクトデータを保存する処理
//=============================================================================
void CEffectManager::Save(void)
{
	CFileSaver *pFileSaver = CFileSaver::Create(EFFECT_SAVE_FILENAME);
	if (pFileSaver != NULL)
	{// ファイルポインタが確保できた
	    // ファイルの冒頭部分を作成
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("# エフェクトデータ [%s]\n", EFFECT_SAVE_FILENAME);
		pFileSaver->Print("# Author : HODAKA NIWA\n");
		pFileSaver->Print("#\n");
		pFileSaver->Print("#==============================================================================\n");
		pFileSaver->Print("%s			# この行は絶対に消さないこと！\n\n", SCRIPT);

		// テクスチャデータの保存
		SaveTextureData(pFileSaver);

		// エミッタデータの保存
		SaveEmitterData(pFileSaver);

		// パーティクルデータの保存
		SaveParticleData(pFileSaver);

		// リングエフェクトデータの保存
		SaveRingEffectData(pFileSaver);

		// コメント部分を作成
		pFileSaver->Print("%s			# この行は絶対に消さないこと！\n\n", END_SCRIPT);

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
//    テクスチャデータを保存する処理
//=============================================================================
void CEffectManager::SaveTextureData(CFileSaver *pFileSaver)
{
	if (m_pTextureManager == NULL) { return; }

	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  使用するテクスチャ数\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// テクスチャ数を保存
	pFileSaver->Print("%s%d\n\n", NUM_TEXTURE, m_pTextureManager->GetNumTexture());

	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  使用するテクスチャのファイルパス名\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// テクスチャへのファイルパス名を保存
	for (int nCntTex = 0; nCntTex < m_pTextureManager->GetNumTexture(); nCntTex++)
	{// 読み込んだテクスチャの数だけ繰り返し
		pFileSaver->Print("%s%s\n", TEXTURE_FILENAME, m_pTextureManager->GetFileName(nCntTex));
	}
	pFileSaver->Print("\n");
}

//=============================================================================
//    エミッタデータを保存する処理
//=============================================================================
void CEffectManager::SaveEmitterData(CFileSaver *pFileSaver)
{
	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  エミッタ数\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// エミッタ数を保存
	pFileSaver->Print("%s%d\n\n", NUM_EMITTER, m_nNumEmitterData);

	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  エミッタのデータ\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// エミッタのデータを作成した数だけ保存
	if (m_apEmitterData == NULL) { return; }
	for (int nCntEmitter = 0; nCntEmitter < m_nNumEmitterData; nCntEmitter++)
	{// 作成したエミッタの数だけ繰り返し
		if (m_apEmitterData[nCntEmitter] != NULL)
		{// エミッタデータが作成されている
			D3DXVECTOR3 RangeMin = m_apEmitterData[nCntEmitter]->GetRangeMin();
			D3DXVECTOR3 RangeMax = m_apEmitterData[nCntEmitter]->GetRangeMax();

			pFileSaver->Print("%s\n", EMITTERSET);
			pFileSaver->Print("	%s%d					# 使用するエフェクトの種類\n", TYPE, m_apEmitterData[nCntEmitter]->GetType());
			pFileSaver->Print("	%s%d				# テクスチャの番号\n", TEX_TYPE, m_apEmitterData[nCntEmitter]->GetTexIdx());
			pFileSaver->Print("	%s%d				# 使用するエフェクトの番号\n", EFFECT_TYPE, m_apEmitterData[nCntEmitter]->GetEffectIdx());
			pFileSaver->Print("	%s%.1f %.1f %.1f		# エフェクトを放出する範囲の最小値\n", RANGE_MIN, RangeMin.x, RangeMin.y, RangeMin.z);
			pFileSaver->Print("	%s%.1f %.1f %.1f		# エフェクトを放出する範囲の最大値\n", RANGE_MAX, RangeMax.x, RangeMax.y, RangeMax.z);
			pFileSaver->Print("	%s%d					# エミッタの寿命\n", LIFE, m_apEmitterData[nCntEmitter]->GetLife());
			pFileSaver->Print("	%s%d						# １回の放出で出すエフェクトの個数\n", LAP, m_apEmitterData[nCntEmitter]->GetLap());
			pFileSaver->Print("	%s%d					# エフェクトを出す間隔\n", APPEAR, m_apEmitterData[nCntEmitter]->GetAppear());
			pFileSaver->Print("	%s%d					# ループするかしないか[ 0:なし 1:あり ]\n", LOOP, (int)m_apEmitterData[nCntEmitter]->GetLoop());
			pFileSaver->Print("%s\n\n", END_EMITTERSET);
		}
	}
}

//=============================================================================
//    パーティクルデータを保存する処理
//=============================================================================
void CEffectManager::SaveParticleData(CFileSaver *pFileSaver)
{
	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  パーティクルデータの数\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// パーティクルデータ数を保存
	pFileSaver->Print("%s%d\n\n", NUM_PARTICLE, m_nNumParData);

	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  パーティクルのデータ\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// パーティクルのデータを作成した数だけ保存
	if (m_apParData == NULL) { return; }
	for (int nCntParticle = 0; nCntParticle < m_nNumParData; nCntParticle++)
	{// 作成したパーティクルのデータの数だけ繰り返し
		pFileSaver->Print("%s\n", PARTICLESET);
		pFileSaver->Print("	%s%.4f %.4f							# 生成時の向き[ 左:最大値 右:最小値 ]\n", ROT, m_apParData[nCntParticle]->GetMaxRot(), m_apParData[nCntParticle]->GetMinRot());
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 移動量の最大値\n", MAX_MOVE, m_apParData[nCntParticle]->GetMaxMove().x, m_apParData[nCntParticle]->GetMaxMove().y, m_apParData[nCntParticle]->GetMaxMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f				# 移動量の最小値\n", MIN_MOVE, m_apParData[nCntParticle]->GetMinMove().x, m_apParData[nCntParticle]->GetMinMove().y, m_apParData[nCntParticle]->GetMinMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 移動量の変化量\n", CHANGE_MOVE, m_apParData[nCntParticle]->GetChangeMove().x, m_apParData[nCntParticle]->GetChangeMove().y, m_apParData[nCntParticle]->GetChangeMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f %.4f			# 生成時の色\n", INIT_COL, m_apParData[nCntParticle]->GetInitCol().r, m_apParData[nCntParticle]->GetInitCol().g, m_apParData[nCntParticle]->GetInitCol().b, m_apParData[nCntParticle]->GetInitCol().a);
		pFileSaver->Print("	%s%.4f %.4f %.4f %.4f		# フレーム毎の色の変化量\n", CHANGE_COL, m_apParData[nCntParticle]->GetChangeCol().r, m_apParData[nCntParticle]->GetChangeCol().g, m_apParData[nCntParticle]->GetChangeCol().b, m_apParData[nCntParticle]->GetChangeCol().a);
		pFileSaver->Print("	%s%.4f %.4f				# 大きさの最大値\n", MAX_SIZE, m_apParData[nCntParticle]->GetMaxWidth(), m_apParData[nCntParticle]->GetMaxHeight());
		pFileSaver->Print("	%s%.4f %.4f				# 大きさの最小値\n", MIN_SIZE, m_apParData[nCntParticle]->GetMinWidth(), m_apParData[nCntParticle]->GetMinHeight());
		pFileSaver->Print("	%s%f						# 大きさの変化量\n", SPREAD, m_apParData[nCntParticle]->GetSpread());
		pFileSaver->Print("	%s%d						# 寿命の最大値\n", MAX_LIFE, m_apParData[nCntParticle]->GetMaxLife());
		pFileSaver->Print("	%s%d						# 寿命の最小値\n", MIN_LIFE, m_apParData[nCntParticle]->GetMinLife());
		pFileSaver->Print("	%s%.4f						# バウンド量\n", BOUNCING, m_apParData[nCntParticle]->GetBouncing());
		pFileSaver->Print("	%s%d						# 回転のパターン[ 0:時計回り 1:反時計回り 2:ランダム ]\n", ROT_PATTERN, m_apParData[nCntParticle]->GetRotPattern());
		pFileSaver->Print("	%s%.4f						# 回転のスピード\n", ROT_SPEED, m_apParData[nCntParticle]->GetRotSpeed());
		pFileSaver->Print("	%s%d						# 当たり判定をするかしないか[ 0:なし 1:あり ]\n", COLLISION, (int)m_apParData[nCntParticle]->GetCollision());
		pFileSaver->Print("	%s%d					# 加算合成するかしないか[ 0:なし 1:あり ]\n", DRAW_ADDTIVE, (int)m_apParData[nCntParticle]->GetDrawAddtive());
		pFileSaver->Print("	%s%d						# ライティングするかしないか[ 0:なし 1:あり ]\n", LIGHTING, (int)m_apParData[nCntParticle]->GetLighting());
		pFileSaver->Print("%s\n\n", END_PARTICLESET);
	}
}

//=============================================================================
//    リングエフェクトデータを保存する処理
//=============================================================================
void CEffectManager::SaveRingEffectData(CFileSaver *pFileSaver)
{
	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  リングエフェクトデータの数\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// リングエフェクトデータ数を保存
	pFileSaver->Print("%s%d\n\n", NUM_RING, m_nNumRingEffectData);

	// コメント部分を作成
	pFileSaver->Print("#------------------------------------------------------------------------------\n");
	pFileSaver->Print("#  リングエフェクトのデータ\n");
	pFileSaver->Print("#------------------------------------------------------------------------------\n");

	// リングエフェクトのデータを作成した数だけ保存
	if (m_apRingData == NULL) { return; }
	for (int nCntRing = 0; nCntRing < m_nNumRingEffectData; nCntRing++)
	{// 作成したリングエフェクトのデータの数だけ繰り返し
		pFileSaver->Print("%s\n", RINGSET);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 向きの最大値\n", MAX_ROT, m_apRingData[nCntRing]->GetMaxRot().x, m_apRingData[nCntRing]->GetMaxRot().y, m_apRingData[nCntRing]->GetMaxRot().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 向きの最小値\n", MIN_ROT, m_apRingData[nCntRing]->GetMinRot().x, m_apRingData[nCntRing]->GetMinRot().y, m_apRingData[nCntRing]->GetMinRot().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 移動量の最大値\n", MAX_MOVE, m_apRingData[nCntRing]->GetMaxMove().x, m_apRingData[nCntRing]->GetMaxMove().y, m_apRingData[nCntRing]->GetMaxMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f				# 移動量の最小値\n", MIN_MOVE, m_apRingData[nCntRing]->GetMinMove().x, m_apRingData[nCntRing]->GetMinMove().y, m_apRingData[nCntRing]->GetMinMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 移動量の変化量\n", CHANGE_MOVE, m_apRingData[nCntRing]->GetChangeMove().x, m_apRingData[nCntRing]->GetChangeMove().y, m_apRingData[nCntRing]->GetChangeMove().z);
		pFileSaver->Print("	%s%.4f %.4f %.4f %.4f			# 生成時の色\n", INIT_COL, m_apRingData[nCntRing]->GetInitCol().r, m_apRingData[nCntRing]->GetInitCol().g, m_apRingData[nCntRing]->GetInitCol().b, m_apParData[nCntRing]->GetInitCol().a);
		pFileSaver->Print("	%s%.4f %.4f %.4f %.4f		# フレーム毎の色の変化量\n", CHANGE_COL, m_apRingData[nCntRing]->GetChangeCol().r, m_apRingData[nCntRing]->GetChangeCol().g, m_apRingData[nCntRing]->GetChangeCol().b, m_apRingData[nCntRing]->GetChangeCol().a);
		pFileSaver->Print("	%s%f						# 高さ\n", HEIGHT, m_apRingData[nCntRing]->GetHeight());
		pFileSaver->Print("	%s%f						# 半径\n", RADIUS, m_apRingData[nCntRing]->GetRadius());
		pFileSaver->Print("	%s%f						# 幅\n", DIFFUSION, m_apRingData[nCntRing]->GetDiffusion());
		pFileSaver->Print("	%s%f						# 高さを毎フレームどれくらい変化させる\n", CHANGE_HEIGHT, m_apRingData[nCntRing]->GetChangeHeight());
		pFileSaver->Print("	%s%f						# 半径を毎フレームどれくらい変化させる\n", CHANGE_RADIUS, m_apRingData[nCntRing]->GetChangeRadius());
		pFileSaver->Print("	%s%f						# 幅を毎フレームどれくらい変化させる\n", CHANGE_DIFFUSION, m_apRingData[nCntRing]->GetChangeDiffusion());
		pFileSaver->Print("	%s%d						# 寿命の最大値\n", MAX_LIFE, m_apRingData[nCntRing]->GetMaxLife());
		pFileSaver->Print("	%s%d						# 寿命の最小値\n", MIN_LIFE, m_apRingData[nCntRing]->GetMinLife());
		pFileSaver->Print("	%s%d						# 横の分割数\n", XBLOCK, m_apRingData[nCntRing]->GetXBlock());
		pFileSaver->Print("	%s%d						# 縦の分割数\n", YBLOCK, m_apRingData[nCntRing]->GetYBlock());
		pFileSaver->Print("	%s%d						# 回転のパターン[ 0:時計回り 1:反時計回り 2:ランダム ]\n", ROT_PATTERN, m_apRingData[nCntRing]->GetRotPattern());
		pFileSaver->Print("	%s%.4f %.4f %.4f			# 回転のスピード\n", ROT_SPEED, m_apRingData[nCntRing]->GetRotSpeed().x, m_apRingData[nCntRing]->GetRotSpeed().y, m_apRingData[nCntRing]->GetRotSpeed().z);
		pFileSaver->Print("	%s%d						# カリングするかしないか[ 0:なし 1:あり ]\n", CULLING, (int)m_apRingData[nCntRing]->GetCulling());
		pFileSaver->Print("	%s%d					# 加算合成するかしないか[ 0:なし 1:あり ]\n", DRAW_ADDTIVE, (int)m_apRingData[nCntRing]->GetDrawAddtive());
		pFileSaver->Print("%s\n\n", END_RINGSET);
	}
}

//=============================================================================
//    テクスチャ管轄クラスを生成する処理
//=============================================================================
void CEffectManager::CreateTextureManager(const int nNumTex)
{
	if (nNumTex >= 1)
	{// テクスチャ数が1つ以上ある
		m_pTextureManager = CTextureManager::Create(nNumTex);
	}
}

//=============================================================================
//    エミッタデータ用のメモリを生成する処理
//=============================================================================
void CEffectManager::CreateEmitterDataMemory(const int nNumEmitterData)
{
	// 生成数を保存
	m_nNumEmitterData = nNumEmitterData;

	// メモリを確保する
	if (nNumEmitterData == 0 || m_apEmitterData != NULL) { return; }
	m_apEmitterData = new CEmitterData*[nNumEmitterData];

	// 確保したメモリを初期化しておく
	if (m_apEmitterData == NULL) { return; }
	for (int nCntEmitter = 0; nCntEmitter < nNumEmitterData; nCntEmitter++)
	{// エミッタデータ数だけ繰り返し
		m_apEmitterData[nCntEmitter] = NULL;
	}
}

//=============================================================================
//    パーティクルデータ用のメモリを生成する処理
//=============================================================================
void CEffectManager::CreateParticleDataMemory(const int nNumParticleData)
{
	// 生成数を保存
	m_nNumParData = nNumParticleData;

	// メモリを確保する
	if (m_nNumParData == 0 || m_apRingData != NULL) { return; }
	m_apParData = new CParData*[nNumParticleData];

	// 確保したメモリを初期化しておく
	if (m_apParData == NULL) { return; }
	for (int nCntParData = 0; nCntParData < nNumParticleData; nCntParData++)
	{// パーティクルデータ数だけ繰り返し
		m_apParData[nCntParData] = NULL;
	}
}

//=============================================================================
//    リングエフェクトデータ用のメモリを生成する処理
//=============================================================================
void CEffectManager::CreateRingEffectDataMemory(const int nNumRingEffectData)
{
	// 生成数を保存
	m_nNumRingEffectData = nNumRingEffectData;

	// メモリを確保する
	if (nNumRingEffectData == 0 || m_apRingData != NULL) { return; }
	m_apRingData = new CRingData*[nNumRingEffectData];

	// 確保したメモリを初期化しておく
	if (m_apRingData == NULL) { return; }
	for (int nCntRingData = 0; nCntRingData < nNumRingEffectData; nCntRingData++)
	{// リングエフェクトデータ数だけ繰り返し
		m_apRingData[nCntRingData] = NULL;
	}
}

//=============================================================================
//    テクスチャ管轄クラスを開放する処理
//=============================================================================
void CEffectManager::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{// メモリが確保されている
		m_pTextureManager->Uninit();

		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    エミッタデータを開放する処理
//=============================================================================
void CEffectManager::ReleaseEmitterData(void)
{
	// メモリが確保されていなければ処理しない
	if (m_apEmitterData == NULL) { return; }

	for (int nCntEmitter = 0; nCntEmitter < m_nNumEmitterData; nCntEmitter++)
	{// エミッタデータ数だけ繰り返し
		if (m_apEmitterData[nCntEmitter] != NULL)
		{// メモリが確保されている
			delete m_apEmitterData[nCntEmitter];
			m_apEmitterData[nCntEmitter] = NULL;
		}
	}
	delete[] m_apEmitterData;
	m_apEmitterData = NULL;
}

//=============================================================================
//    パーティクルデータを開放する処理
//=============================================================================
void CEffectManager::ReleaseParticleData(void)
{
	// メモリが確保されていなければ処理しない
	if (m_apParData == NULL) { return; }

	for (int nCntParData = 0; nCntParData < m_nNumParData; nCntParData++)
	{// パーティクルデータ数だけ繰り返し
		if (m_apParData[nCntParData] != NULL)
		{// メモリが確保されている
			delete m_apParData[nCntParData];
			m_apParData[nCntParData] = NULL;
		}
	}
	delete[] m_apParData;
	m_apParData = NULL;
}

//=============================================================================
//    リングエフェクトデータを開放する処理
//=============================================================================
void CEffectManager::ReleaseRingEffectData(void)
{
	// メモリが確保されていなければ処理しない
	if (m_apRingData == NULL) { return; }

	for (int nCntRingData = 0; nCntRingData < m_nNumRingEffectData; nCntRingData++)
	{// リングエフェクトデータ数だけ繰り返し
		if (m_apRingData[nCntRingData] != NULL)
		{// メモリが確保されている
			delete m_apRingData[nCntRingData];
			m_apRingData[nCntRingData] = NULL;
		}
	}
	delete[] m_apRingData;
	m_apRingData = NULL;
}

//=============================================================================
//    スクリプトファイル名を設定する処理
//=============================================================================
void CEffectManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    最後に生成したエミッタクラスへのポインタを設定する処理
//=============================================================================
void CEffectManager::SetEmitter(CEmitter *pEmitter)
{
	m_pEmitter = pEmitter;
}

//=============================================================================
//    エミッタデータを設定する処理
//=============================================================================
void CEffectManager::SetEmitterData(CEmitterData *pEmitterData, const int nIdx)
{
	m_apEmitterData[nIdx] = pEmitterData;
}

//=============================================================================
//    パーティクルデータを設定する処理
//=============================================================================
void CEffectManager::SetParData(CParData *pParData, const int nIdx)
{
	m_apParData[nIdx] = pParData;
}

//=============================================================================
//    リングエフェクトデータを設定する処理
//=============================================================================
void CEffectManager::SetRingEffectData(CRingData *pRingData, const int nIdx)
{
	m_apRingData[nIdx] = pRingData;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを設定する処理
//=============================================================================
void CEffectManager::SetTexManager(CTextureManager *pTexManager)
{
	m_pTextureManager = pTexManager;
}

//=============================================================================
//    エミッタデータの数を設定する処理
//=============================================================================
void CEffectManager::SetNumEmitterData(const int nNumEmitterData)
{
	m_nNumEmitterData = nNumEmitterData;
}

//=============================================================================
//    パーティクルデータの数を設定する処理
//=============================================================================
void CEffectManager::SetNumParData(const int nNumParData)
{
	m_nNumParData = nNumParData;
}

//=============================================================================
//    リングエフェクトデータの数を設定する処理
//=============================================================================
void CEffectManager::SetNumRingEffectData(const int nNumRingEffectData)
{
	m_nNumRingEffectData = nNumRingEffectData;
}

//=============================================================================
//    最後に生成したエミッタクラスへのポインタを取得する処理
//=============================================================================
CEmitter *CEffectManager::GetEmitter(void)
{
	return m_pEmitter;
}

//=============================================================================
//    テクスチャを取得する処理
//=============================================================================
LPDIRECT3DTEXTURE9 CEffectManager::GetTexture(const int nIdx)
{
	return m_pTextureManager->GetTexture(nIdx);
}

//=============================================================================
//    エミッタデータを取得する処理
//=============================================================================
CEmitterData **CEffectManager::GetEmitterData(void)
{
	return m_apEmitterData;
}
CEmitterData *CEffectManager::GetEmitterData(const int nIdx)
{
	return m_apEmitterData[nIdx];
}

//=============================================================================
//    パーティクルデータを取得する処理
//=============================================================================
CParData **CEffectManager::GetParData(void)
{
	return m_apParData;
}
CParData *CEffectManager::GetParData(const int nIdx)
{
	return m_apParData[nIdx];
}

//=============================================================================
//    リングエフェクトデータを取得する処理
//=============================================================================
CRingData **CEffectManager::GetRingEffectData(void)
{
	return m_apRingData;
}
CRingData *CEffectManager::GetRingEffectData(const int nIdx)
{
	return m_apRingData[nIdx];
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを取得する処理
//=============================================================================
CTextureManager *CEffectManager::GetTexManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    エミッタデータの数を取得する処理
//=============================================================================
int CEffectManager::GetNumEmitterData(void)
{
	return m_nNumEmitterData;
}

//=============================================================================
//    パーティクルデータの数を取得する処理
//=============================================================================
int CEffectManager::GetNumParData(void)
{
	return m_nNumParData;
}

//=============================================================================
//    リングエフェクトデータの数を取得する処理
//=============================================================================
int CEffectManager::GetNumRingEffectData(void)
{
	return m_nNumRingEffectData;
}