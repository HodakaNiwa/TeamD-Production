//*****************************************************************************
//
//     エフェクト管轄の処理[effectManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "effectManager.h"
#include "emitter.h"
#include "particle.h"
#include "ringEffect.h"
#include "textureManager.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debugproc.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
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
#define RANGE             "RANGE = "               // エフェクトを生成する範囲
#define LIFE              "LIFE = "                // エミッタの寿命
#define LAP               "LAP = "                 // １回の放出で出すエフェクトの個数
#define APPEAR            "APPEAR = "              // エフェクトを出す間隔
#define LOOP              "LOOP = "                // ループするかしないか

// パーティクルデータ
#define NUM_PARTICLE      "NUM_PARTICLE = "        // パーティクルデータの個数
#define PARTICLESET       "PARTICLESET"            // パーティクルデータ読み込み開始の合図
#define END_PARTICLESET   "END_PARTICLESET"        // パーティクルデータ読み込み終了の合図
#define RANGE             "RANGE = "               // 生成する範囲
#define RANGE_MAG         "RANGE_MAG = "           // 生成する範囲の倍率
#define ROT               "ROT = "                 // 生成時の向き[ 左:最大値 右:最小値 ]
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
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMMITER; nCntEmitter++)
	{// エミッタデータの最大数だけ繰り返し
		m_apEmitterData[nCntEmitter] = NULL;   // エミッタデータクラスへのポインタ
	}
	for (int nCntParData = 0; nCntParData < MAX_PARTICLEDATA; nCntParData++)
	{// パーティクルデータの最大数だけ繰り返し
		m_apParData[nCntParData] = NULL;       // パーティクルデータクラスへのポインタ
	}
	for (int nCntRingData = 0; nCntRingData < MAX_RINGEFFECTDATA; nCntRingData++)
	{// リングエフェクトデータの最大数だけ繰り返し
		m_apRingData[nCntRingData] = NULL;       // リングエフェクトデータクラスへのポインタ
	}
	m_pEmitter = NULL;          // 現在生成されているエミッタへのポインタ
	m_nNumEmitterData = 0;      // エミッタデータの個数
	m_nNumParData = 0;          // パーティクルデータの個数
	m_nNumRingEffectData = 0;   // リングエフェクトデータの個数
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
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// ポインタを確保できた
			char *pLine = NULL;                    // 1行分読み取り用
			char *pStrCur = NULL;                  // 現在の先頭ポインタ
			char *pStr = NULL;                     // 先頭ポインタ保存用
			int nNumTexture = 0;                   // 読み込むテクスチャの数
			int nCntTexture = 0;                   // テクスチャを読み込んだ回数
			int nCntEmitter = 0;                   // エミッタデータを読み込んだ回数
			int nCntParticle = 0;                  // パーティクルデータを読み込んだ回数
			int nCntRing = 0;                      // リングエフェクトデータを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture = NULL;    // テクスチャ読み取り用
			if (pLine == NULL && pStr == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				pStr = new char[256];
				strcpy(pLine, "\0");
				strcpy(pStr, "\0");
				if (pLine != NULL && pStr != NULL)
				{// メモリが確保されている
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// テクスチャの個数情報があった
								nNumTexture = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTexture >= 1)
								{// モデル数が1つ以上ある
									m_pTextureManager = CTextureManager::Create(nNumTexture);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// テクスチャのファイルパス名情報があった
								// テクスチャのファイルパス名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

								// テクスチャの読み込み
								D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
								if (pTexture != NULL)
								{// テクスチャを読み込めた
									m_pTextureManager->SetTexture(pTexture, nCntTexture);
									m_pTextureManager->SetFileName(pStr, nCntTexture);
									pTexture = NULL;
									nCntTexture++;
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_EMITTER) == 0)
							{// エミッタデータの個数情報がある
								m_nNumEmitterData = CFunctionLib::ReadInt(pStrCur, NUM_EMITTER);
							}
							else if (CFunctionLib::Memcmp(pStrCur, EMITTERSET) == 0)
							{// エミッタデータ読み込み開始の合図だった
								m_apEmitterData[nCntEmitter] = ReadEmitterData(pLine, pStrCur, pFile, nCntEmitter);
								nCntEmitter++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_PARTICLE) == 0)
							{// パーティクルデータの個数情報がある
								m_nNumParData = CFunctionLib::ReadInt(pStrCur, NUM_PARTICLE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, PARTICLESET) == 0)
							{// パーティクルデータ読み込み開始の合図だった
								m_apParData[nCntParticle] = ReadParticleData(pLine, pStrCur, pFile, nCntParticle);
								nCntParticle++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_RING) == 0)
							{// リングエフェクトデータの個数情報がある
								m_nNumRingEffectData = CFunctionLib::ReadInt(pStrCur, NUM_RING);
							}
							else if (CFunctionLib::Memcmp(pStrCur, RINGSET) == 0)
							{// リングエフェクトデータ読み込み開始の合図だった
								m_apRingData[nCntRing] = ReadRingEffectData(pLine, pStrCur, pFile, nCntRing);
								nCntRing++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// メモリの開放
					if (pStr != NULL)
					{// メモリが確保されている
						delete[] pStr;
						pStr = NULL;
					}
					if (pLine != NULL)
					{// メモリが確保されている
						delete[] pLine;
						pLine = NULL;
					}
				}
			}
			fclose(pFile);   // ファイルを閉じておく
		}
		else
		{// ポインタを確保できなかった
			return E_FAIL;
		}
	}
	else
	{// ファイルポインタを確保できる状態でない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CEffectManager::Uninit(void)
{
	// エフェクトデータを保存する
	//Save();

	// エミッタの開放
	if (m_pEmitter != NULL)
	{
		m_pEmitter = NULL;
	}

	// エミッタデータクラスの開放
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMMITER; nCntEmitter++)
	{// エミッタデータの最大数だけ繰り返し
		if (m_apEmitterData[nCntEmitter] != NULL)
		{// メモリが確保されている
			delete m_apEmitterData[nCntEmitter];
			m_apEmitterData[nCntEmitter] = NULL;
		}
	}

	// パーティクルデータクラスの開放
	for (int nCntParData = 0; nCntParData < MAX_PARTICLEDATA; nCntParData++)
	{// パーティクルデータの最大数だけ繰り返し
		if (m_apParData[nCntParData] != NULL)
		{// メモリが確保されている
			delete m_apParData[nCntParData];
			m_apParData[nCntParData] = NULL;
		}
	}

	// リングエフェクトデータクラスの開放
	for (int nCntRingData = 0; nCntRingData < MAX_RINGEFFECTDATA; nCntRingData++)
	{// リングエフェクトデータの最大数だけ繰り返し
		if (m_apRingData[nCntRingData] != NULL)
		{// メモリが確保されている
			delete m_apRingData[nCntRingData];
			m_apRingData[nCntRingData] = NULL;
		}
	}

	// テクスチャ管轄クラスの開放
	if (m_pTextureManager != NULL)
	{// メモリが確保されている
		m_pTextureManager->Uninit();

		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

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
	CEmitter *pEmitter = NULL;
	// エミッタデータクラスからデータを取得
	if (m_apEmitterData[nType] != NULL)
	{// データクラスが作成されている
		int nEffectType = m_apEmitterData[nType]->GetType();
		int nTexIdx = m_apEmitterData[nType]->GetTexIdx();
		int nEffectIdx = m_apEmitterData[nType]->GetEffectIdx();
		int nLife = m_apEmitterData[nType]->GetLife();
		float fRangeMax = m_apEmitterData[nType]->GetRangeMax();
		float fRangeMin = m_apEmitterData[nType]->GetRangeMin();
		int nLap = m_apEmitterData[nType]->GetLap();
		int nAppear = m_apEmitterData[nType]->GetAppear();
		bool bLoop = m_apEmitterData[nType]->GetLoop();
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		if (nTexIdx != -1)
		{
			pTexture = m_pTextureManager->GetTexture(nTexIdx);
		}

		// エミッタクラスを作成する
		if (nEffectType == 0)
		{// 0番のエフェクトの種類(パーティクル)だったら
			pEmitter = m_pEmitter = CParEmitter::Create(pos, rot, nType, nTexIdx, nEffectIdx, fRangeMax, fRangeMin, nLife, nLap, nAppear, bLoop, m_apParData[nEffectIdx], pTexture, nPriority);
		}
		else if (nEffectType == 1)
		{// 1番のエフェクトの種類(リングエフェクト)だったら
			pEmitter = m_pEmitter = CRingEmitter::Create(pos, rot, nType, nTexIdx, nEffectIdx, fRangeMax, fRangeMin, nLife, nLap, nAppear, bLoop, m_apRingData[nEffectIdx], pTexture, nPriority);
		}
	}

	return pEmitter;
}

//=============================================================================
//    マップ配置用のエフェクトを設定する処理
//=============================================================================
CEmitter *CEffectManager::SetMapEditEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	CEmitter *pSetEmitter = NULL;

	// エミッタデータクラスからデータを取得
	if (m_apEmitterData[nType] != NULL)
	{// データクラスが作成されている
		int nEffectType = m_apEmitterData[nType]->GetType();
		int nTexIdx = m_apEmitterData[nType]->GetTexIdx();
		int nEffectIdx = m_apEmitterData[nType]->GetEffectIdx();
		int nLife = m_apEmitterData[nType]->GetLife();
		float fRangeMax = m_apEmitterData[nType]->GetRangeMax();
		float fRangeMin = m_apEmitterData[nType]->GetRangeMin();
		int nLap = m_apEmitterData[nType]->GetLap();
		int nAppear = m_apEmitterData[nType]->GetAppear();
		bool bLoop = m_apEmitterData[nType]->GetLoop();
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		if (nTexIdx != -1)
		{
			pTexture = m_pTextureManager->GetTexture(nTexIdx);
		}

		// エミッタクラスを作成する
		if (nEffectType == 0)
		{// 0番のエフェクトの種類(パーティクル)だったら
			pSetEmitter = CSetParEmitter::Create(pos, rot, nType, nTexIdx, nEffectIdx, fRangeMax, fRangeMin, nLife, nLap, nAppear, bLoop, m_apParData[nEffectIdx], pTexture, nPriority);
		}
		else if (nEffectType == 1)
		{// 1番のエフェクトの種類(リングエフェクト)だったら
			pSetEmitter = CSetRingEmitter::Create(pos, rot, nType, nTexIdx, nEffectIdx, fRangeMax, fRangeMin, nLife, nLap, nAppear, bLoop, m_apRingData[nEffectIdx], pTexture, nPriority);
		}
	}

	return pSetEmitter;
}

//=============================================================================
//    エミッタデータを読み込む処理
//=============================================================================
CEmitterData *CEffectManager::ReadEmitterData(char *pLine, char *pStrCur, FILE *pFile, int nCntEmitter)
{
	CEmitterData *pEmitterData = NULL;  // エミッタデータクラスへのポインタ
	char aStr[256];
	if (pEmitterData == NULL)
	{// メモリが確保できる状態である
		pEmitterData = new CEmitterData;
		if (pEmitterData != NULL)
		{// メモリが確保できた
			while (1)
			{// ループ開始
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
				if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
				{// エフェクトの種類情報がある
					pEmitterData->SetType(CFunctionLib::ReadInt(pStrCur, TYPE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
				{// テクスチャ番号情報がある
					pEmitterData->SetTexIdx(CFunctionLib::ReadInt(pStrCur, TEX_TYPE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, EFFECT_TYPE) == 0)
				{// 種類情報がある
					pEmitterData->SetEffectIdx(CFunctionLib::ReadInt(pStrCur, EFFECT_TYPE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RANGE) == 0)
				{// エフェクトを生成する範囲情報がある
					pEmitterData->SetRangeMax(CFunctionLib::ReadFloat(pStrCur, RANGE));
					pStrCur = CFunctionLib::HeadPutout(pStrCur, RANGE);
					int nWord = CFunctionLib::PopString(pStrCur, aStr);
					pStrCur += nWord;
					pEmitterData->SetRangeMin(CFunctionLib::ReadFloat(pStrCur, ""));
				}
				else if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
				{// 寿命情報がある
					pEmitterData->SetLife(CFunctionLib::ReadInt(pStrCur, LIFE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, LAP) == 0)
				{// 1回の放出で生成する個数情報がある
					pEmitterData->SetLap(CFunctionLib::ReadInt(pStrCur, LAP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, APPEAR) == 0)
				{// 放出する間隔情報がある
					pEmitterData->SetAppear(CFunctionLib::ReadInt(pStrCur, APPEAR));
				}
				else if (CFunctionLib::Memcmp(pStrCur, LOOP) == 0)
				{// ループするかしないか情報がある
					pEmitterData->SetLoop(CFunctionLib::ReadBool(pStrCur, LOOP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_EMITTERSET) == 0)
				{// エミッタデータ読み込み終了の合図だった
					break;  // ループ終了
				}
			}
		}
	}

	return pEmitterData;
}

//=============================================================================
//    パーティクルデータを読み込む処理
//=============================================================================
CParData *CEffectManager::ReadParticleData(char *pLine, char *pStrCur, FILE *pFile, int nCntParData)
{
	CParData *pParData = NULL;  // パーティクルデータクラスへのポインタ
	char aStr[256];

	if (pParData == NULL)
	{// メモリが確保できる状態である
		pParData = new CParData;
		if (pParData != NULL)
		{// メモリが確保できた
			while (1)
			{// ループ開始
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
				if (CFunctionLib::Memcmp(pStrCur, RANGE) == 0)
				{// 生成する範囲情報がある
					pParData->SetRange(CFunctionLib::ReadVector3(pStrCur, RANGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RANGE_MAG) == 0)
				{// 生成する範囲の倍率情報がある
					pParData->SetRangeMag(CFunctionLib::ReadFloat(pStrCur, RANGE_MAG));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
				{// 生成時の向き情報がある
					pParData->SetMaxRot(CFunctionLib::ReadFloat(pStrCur, ROT));
					pStrCur = CFunctionLib::HeadPutout(pStrCur, ROT);
					int nWord = CFunctionLib::PopString(pStrCur, aStr);
					pStrCur += nWord;
					pParData->SetMinRot(CFunctionLib::ReadFloat(pStrCur, ""));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MAX_MOVE) == 0)
				{// 移動量の最大値情報がある
					pParData->SetMaxMove(CFunctionLib::ReadVector3(pStrCur, MAX_MOVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MIN_MOVE) == 0)
				{// 移動量の最小値情報がある
					pParData->SetMinMove(CFunctionLib::ReadVector3(pStrCur, MIN_MOVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, CHANGE_MOVE) == 0)
				{// 移動量の変化量情報がある
					pParData->SetChangeMove(CFunctionLib::ReadVector3(pStrCur, CHANGE_MOVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, INIT_COL) == 0)
				{// 生成時の色情報がある
					pParData->SetInitCol(CFunctionLib::ReadVector4(pStrCur, INIT_COL));
				}
				else if (CFunctionLib::Memcmp(pStrCur, CHANGE_COL) == 0)
				{// フレーム毎の色の変化量情報がある
					pParData->SetChangeCol(CFunctionLib::ReadVector4(pStrCur, CHANGE_COL));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MAX_SIZE) == 0)
				{// 大きさの最大値情報がある
					pParData->SetMaxWidth(CFunctionLib::ReadFloat(pStrCur, MAX_SIZE));
					pStrCur = CFunctionLib::HeadPutout(pStrCur, MAX_SIZE);
					int nWord = CFunctionLib::PopString(pStrCur, aStr);
					pStrCur += nWord;
					pParData->SetMaxHeight(CFunctionLib::ReadFloat(pStrCur, ""));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MIN_SIZE) == 0)
				{// 大きさの最小値情報がある
					pParData->SetMinWidth(CFunctionLib::ReadFloat(pStrCur, MIN_SIZE));
					pStrCur = CFunctionLib::HeadPutout(pStrCur, MIN_SIZE);
					int nWord = CFunctionLib::PopString(pStrCur, aStr);
					pStrCur += nWord;
					pParData->SetMinHeight(CFunctionLib::ReadFloat(pStrCur, ""));
				}
				else if (CFunctionLib::Memcmp(pStrCur, SPREAD) == 0)
				{// 大きさの変化量情報がある
					pParData->SetSpread(CFunctionLib::ReadFloat(pStrCur, SPREAD));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MAX_LIFE) == 0)
				{// 寿命の最大値情報がある
					pParData->SetMaxLife(CFunctionLib::ReadInt(pStrCur, MAX_LIFE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MIN_LIFE) == 0)
				{// 寿命の最小値情報がある
					pParData->SetMinLife(CFunctionLib::ReadInt(pStrCur, MIN_LIFE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
				{// 毎フレームかける重力情報がある
					pParData->SetGravity(CFunctionLib::ReadFloat(pStrCur, GRAVITY));
				}
				else if (CFunctionLib::Memcmp(pStrCur, BOUNCING) == 0)
				{// バウンド量情報がある
					pParData->SetBouncing(CFunctionLib::ReadFloat(pStrCur, BOUNCING));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ROT_PATTERN) == 0)
				{// 回転の種類情報がある
					pParData->SetRotPattern(CFunctionLib::ReadInt(pStrCur, ROT_PATTERN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ROT_SPEED) == 0)
				{// 回転のスピード情報がある
					pParData->SetRotSpeed(CFunctionLib::ReadFloat(pStrCur, ROT_SPEED));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
				{// 当たり判定をするかしないか情報がある
					pParData->SetCollision(CFunctionLib::ReadBool(pStrCur, COLLISION));
				}
				else if (CFunctionLib::Memcmp(pStrCur, DRAW_ADDTIVE) == 0)
				{// 加算合成するかしないか情報がある
					pParData->SetDrawAddtive(CFunctionLib::ReadBool(pStrCur, DRAW_ADDTIVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, LIGHTING) == 0)
				{// ライティングするかしないか情報がある
					pParData->SetLighting(CFunctionLib::ReadBool(pStrCur, LIGHTING));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_PARTICLESET) == 0)
				{// パーティクルデータ読み込み終了の合図だった
					break;  // ループ終了
				}
			}
		}
	}

	return pParData;
}

//=============================================================================
//    リングエフェクトデータを読み込む処理
//=============================================================================
CRingData *CEffectManager::ReadRingEffectData(char *pLine, char *pStrCur, FILE *pFile, int nCntRingEffectData)
{
	CRingData *pRingData = NULL;  // エフェクトデータクラスへのポインタ

	if (pRingData == NULL)
	{// メモリが確保できる状態である
		pRingData = new CRingData;
		if (pRingData != NULL)
		{// メモリが確保できた
			while (1)
			{// ループ開始
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
				if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
				{// 生成時の向き情報がある
					pRingData->SetRot(CFunctionLib::ReadVector3(pStrCur, ROT));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MAX_MOVE) == 0)
				{// 移動量の最大値情報がある
					pRingData->SetMaxMove(CFunctionLib::ReadVector3(pStrCur, MAX_MOVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MIN_MOVE) == 0)
				{// 移動量の最小値情報がある
					pRingData->SetMinMove(CFunctionLib::ReadVector3(pStrCur, MIN_MOVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, CHANGE_MOVE) == 0)
				{// 移動量の変化量情報がある
					pRingData->SetChangeMove(CFunctionLib::ReadVector3(pStrCur, CHANGE_MOVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, INIT_COL) == 0)
				{// 生成時の色情報がある
					pRingData->SetInitCol(CFunctionLib::ReadVector4(pStrCur, INIT_COL));
				}
				else if (CFunctionLib::Memcmp(pStrCur, CHANGE_COL) == 0)
				{// フレーム毎の色の変化量情報がある
					pRingData->SetChangeCol(CFunctionLib::ReadVector4(pStrCur, CHANGE_COL));
				}
				else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
				{// 高さ情報がある
					pRingData->SetHeight(CFunctionLib::ReadFloat(pStrCur, HEIGHT));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RADIUS) == 0)
				{// 半径情報がある
					pRingData->SetRadius(CFunctionLib::ReadFloat(pStrCur, RADIUS));
				}
				else if (CFunctionLib::Memcmp(pStrCur, DIFFUSION) == 0)
				{// 幅情報がある
					pRingData->SetDiffusion(CFunctionLib::ReadFloat(pStrCur, DIFFUSION));
				}
				else if (CFunctionLib::Memcmp(pStrCur, CHANGE_HEIGHT) == 0)
				{// 高さを毎フレームどれくらい変化させる情報がある
					pRingData->SetChangeHeight(CFunctionLib::ReadFloat(pStrCur, CHANGE_HEIGHT));
				}
				else if (CFunctionLib::Memcmp(pStrCur, CHANGE_RADIUS) == 0)
				{// 半径を毎フレームどれくらい変化させる情報がある
					pRingData->SetChangeRadius(CFunctionLib::ReadFloat(pStrCur, CHANGE_RADIUS));
				}
				else if (CFunctionLib::Memcmp(pStrCur, CHANGE_DIFFUSION) == 0)
				{// 幅を毎フレームどれくらい変化させる情報がある
					pRingData->SetChangeDiffusion(CFunctionLib::ReadFloat(pStrCur, CHANGE_DIFFUSION));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MAX_LIFE) == 0)
				{// 寿命の最大値情報がある
					pRingData->SetMaxLife(CFunctionLib::ReadInt(pStrCur, MAX_LIFE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MIN_LIFE) == 0)
				{// 寿命の最小値情報がある
					pRingData->SetMinLife(CFunctionLib::ReadInt(pStrCur, MIN_LIFE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
				{// 横の分割数情報がある
					pRingData->SetXBlock(CFunctionLib::ReadInt(pStrCur, XBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
				{// 縦の分割数情報がある
					pRingData->SetYBlock(CFunctionLib::ReadInt(pStrCur, YBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ROT_PATTERN) == 0)
				{// 回転の種類情報がある
					pRingData->SetRotPattern(CFunctionLib::ReadInt(pStrCur, ROT_PATTERN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ROT_SPEED) == 0)
				{// 回転のスピード情報がある
					pRingData->SetRotSpeed(CFunctionLib::ReadVector3(pStrCur, ROT_SPEED));
				}
				else if (CFunctionLib::Memcmp(pStrCur, CULLING) == 0)
				{// カリングをするかしないか情報がある
					pRingData->SetCulling(CFunctionLib::ReadBool(pStrCur, CULLING));
				}
				else if (CFunctionLib::Memcmp(pStrCur, DRAW_ADDTIVE) == 0)
				{// 加算合成するかしないか情報がある
					pRingData->SetDrawAddtive(CFunctionLib::ReadBool(pStrCur, DRAW_ADDTIVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_RINGSET) == 0)
				{// リングエフェクトデータ読み込み終了の合図だった
					break;  // ループ終了
				}
			}
		}
	}

	return pRingData;
}

//=============================================================================
//    エフェクトデータを保存する処理
//=============================================================================
void CEffectManager::Save(void)
{
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタが確保できる状態である
		pFile = fopen(EFFECT_SAVE_FILENAME, "w");
		if (pFile != NULL)
		{// ファイルポインタが確保できた
			// ファイルの冒頭部分を作成
			fprintf(pFile, "#==============================================================================\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "# エフェクトデータ [%s]\n", EFFECT_SAVE_FILENAME);
			fprintf(pFile, "# Author : HODAKA NIWA\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#==============================================================================\n");
			fprintf(pFile, "%s			#この行は絶対に消さないこと！\n\n", SCRIPT);

			// テクスチャデータの保存
			SaveTextureData(pFile);

			// エミッタデータの保存
			SaveEmitterData(pFile);

			// パーティクルデータの保存
			SaveParticleData(pFile);

			// リングエフェクトデータの保存
			SaveRingEffectData(pFile);

			// コメント部分を作成
			fprintf(pFile, "%s			#この行は絶対に消さないこと！\n\n", END_SCRIPT);

			fclose(pFile);   // ファイルを閉じておく
		}
	}
}

//=============================================================================
//    テクスチャデータを保存する処理
//=============================================================================
void CEffectManager::SaveTextureData(FILE *pFile)
{
	// コメント部分を作成
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  使用するテクスチャ数\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// テクスチャ数を保存
	fprintf(pFile, "%s%d\n\n", NUM_TEXTURE, m_pTextureManager->GetNumTexture());

	// コメント部分を作成
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  使用するテクスチャのファイルパス名\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// テクスチャへのファイルパス名を保存
	for (int nCntTex = 0; nCntTex < m_pTextureManager->GetNumTexture(); nCntTex++)
	{// 読み込んだテクスチャの数だけ繰り返し
		fprintf(pFile, "%s%s\n", TEXTURE_FILENAME, m_pTextureManager->GetFileName(nCntTex));
	}
	fprintf(pFile, "\n");
}

//=============================================================================
//    エミッタデータを保存する処理
//=============================================================================
void CEffectManager::SaveEmitterData(FILE *pFile)
{
	// コメント部分を作成
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  エミッタ数\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// エミッタ数を保存
	fprintf(pFile, "%s%d\n\n", NUM_EMITTER, m_nNumEmitterData);

	// コメント部分を作成
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  エミッタのデータ\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// エミッタのデータを作成した数だけ保存
	for (int nCntEmitter = 0; nCntEmitter < m_nNumEmitterData; nCntEmitter++)
	{// 作成したエミッタの数だけ繰り返し
		if (m_apEmitterData[nCntEmitter] != NULL)
		{// エミッタデータが作成されている
			fprintf(pFile, "%s\n", EMITTERSET);
			fprintf(pFile, "	%s%d		#テクスチャの番号\n", TEX_TYPE, m_apEmitterData[nCntEmitter]->GetTexIdx());
			fprintf(pFile, "	%s%d		#使用するエフェクトの番号\n", EFFECT_TYPE, m_apEmitterData[nCntEmitter]->GetEffectIdx());
			fprintf(pFile, "	%s%d %d		#エフェクトを放出する範囲[ 左 :最大値 右:最小値 ]\n", RANGE, (int)m_apEmitterData[nCntEmitter]->GetRangeMax(), (int)m_apEmitterData[nCntEmitter]->GetRangeMin());
			fprintf(pFile, "	%s%d			#エミッタの寿命\n", LIFE, m_apEmitterData[nCntEmitter]->GetLife());
			fprintf(pFile, "	%s%d				#１回の放出で出すエフェクトの個数\n", LAP, m_apEmitterData[nCntEmitter]->GetLap());
			fprintf(pFile, "	%s%d			#エフェクトを出す間隔\n", APPEAR, m_apEmitterData[nCntEmitter]->GetAppear());
			fprintf(pFile, "	%s%d			#ループするかしないか[ 0:なし 1:あり ]\n", LOOP, (int)m_apEmitterData[nCntEmitter]->GetLoop());
			fprintf(pFile, "%s\n\n", END_EMITTERSET);
		}
	}
}

//=============================================================================
//    パーティクルデータを保存する処理
//=============================================================================
void CEffectManager::SaveParticleData(FILE *pFile)
{
	// コメント部分を作成
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  パーティクルデータの数\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// パーティクルデータ数を保存
	fprintf(pFile, "%s%d\n\n", NUM_PARTICLE, m_nNumParData);

	// コメント部分を作成
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  パーティクルのデータ\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// パーティクルのデータを作成した数だけ保存
	for (int nCntParticle = 0; nCntParticle < m_nNumParData; nCntParticle++)
	{// 作成したパーティクルのデータの数だけ繰り返し
		fprintf(pFile, "%s\n", PARTICLESET);
		fprintf(pFile, "	%s%.4f %.4f %.4f			#生成する範囲\n", RANGE, m_apParData[nCntParticle]->GetRange().x, m_apParData[nCntParticle]->GetRange().y, m_apParData[nCntParticle]->GetRange().z);
		fprintf(pFile, "	%s%.4f						#生成する範囲の倍率\n", RANGE_MAG, m_apParData[nCntParticle]->GetRangeMag());
		fprintf(pFile, "	%s%.4f %.4f							#生成時の向き[ 左:最大値 右:最小値 ]\n", ROT, m_apParData[nCntParticle]->GetMaxRot(), m_apParData[nCntParticle]->GetMinRot());
		fprintf(pFile, "	%s%.4f %.4f %.4f			#移動量の最大値\n", MAX_MOVE, m_apParData[nCntParticle]->GetMaxMove().x, m_apParData[nCntParticle]->GetMaxMove().y, m_apParData[nCntParticle]->GetMaxMove().z);
		fprintf(pFile, "	%s%.4f %.4f %.4f				#移動量の最小値\n", MIN_MOVE, m_apParData[nCntParticle]->GetMinMove().x, m_apParData[nCntParticle]->GetMinMove().y, m_apParData[nCntParticle]->GetMinMove().z);
		fprintf(pFile, "	%s%.4f %.4f %.4f			#移動量の変化量\n", CHANGE_MOVE, m_apParData[nCntParticle]->GetChangeMove().x, m_apParData[nCntParticle]->GetChangeMove().y, m_apParData[nCntParticle]->GetChangeMove().z);
		fprintf(pFile, "	%s%.4f %.4f %.4f %.4f			#生成時の色\n", INIT_COL, m_apParData[nCntParticle]->GetInitCol().r, m_apParData[nCntParticle]->GetInitCol().g, m_apParData[nCntParticle]->GetInitCol().b, m_apParData[nCntParticle]->GetInitCol().a);
		fprintf(pFile, "	%s%.4f %.4f %.4f %.4f		#フレーム毎の色の変化量\n", CHANGE_COL, m_apParData[nCntParticle]->GetChangeCol().r, m_apParData[nCntParticle]->GetChangeCol().g, m_apParData[nCntParticle]->GetChangeCol().b, m_apParData[nCntParticle]->GetChangeCol().a);
		fprintf(pFile, "	%s%.4f %.4f				#大きさの最大値\n", MAX_SIZE, m_apParData[nCntParticle]->GetMaxWidth(), m_apParData[nCntParticle]->GetMaxHeight());
		fprintf(pFile, "	%s%.4f %.4f				#大きさの最小値\n", MIN_SIZE, m_apParData[nCntParticle]->GetMinWidth(), m_apParData[nCntParticle]->GetMinHeight());
		fprintf(pFile, "	%s%f						#大きさの変化量\n", SPREAD, m_apParData[nCntParticle]->GetSpread());
		fprintf(pFile, "	%s%d						#寿命の最大値\n", MAX_LIFE, m_apParData[nCntParticle]->GetMaxLife());
		fprintf(pFile, "	%s%d						#寿命の最小値\n", MIN_LIFE, m_apParData[nCntParticle]->GetMinLife());
		fprintf(pFile, "	%s%.4f						#毎フレームかける重力\n", GRAVITY, m_apParData[nCntParticle]->GetGravity());
		fprintf(pFile, "	%s%.4f						#バウンド量\n", BOUNCING, m_apParData[nCntParticle]->GetBouncing());
		fprintf(pFile, "	%s%d						#回転のパターン[ 0:時計回り 1:反時計回り 2:ランダム ]\n", ROT_PATTERN, m_apParData[nCntParticle]->GetRotPattern());
		fprintf(pFile, "	%s%.4f						#回転のスピード\n", ROT_SPEED, m_apParData[nCntParticle]->GetRotSpeed());
		fprintf(pFile, "	%s%d						#当たり判定をするかしないか[ 0:なし 1:あり ]\n", COLLISION, (int)m_apParData[nCntParticle]->GetCollision());
		fprintf(pFile, "	%s%d					#加算合成するかしないか[ 0:なし 1:あり ]\n", DRAW_ADDTIVE, (int)m_apParData[nCntParticle]->GetDrawAddtive());
		fprintf(pFile, "	%s%d						#ライティングするかしないか[ 0:なし 1:あり ]\n", LIGHTING, (int)m_apParData[nCntParticle]->GetLighting());
		fprintf(pFile, "%s\n\n", END_PARTICLESET);
	}
}

//=============================================================================
//    リングエフェクトデータを保存する処理
//=============================================================================
void CEffectManager::SaveRingEffectData(FILE *pFile)
{
	// コメント部分を作成
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  リングエフェクトデータの数\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// リングエフェクトデータ数を保存
	fprintf(pFile, "%s%d\n\n", NUM_RING, m_nNumRingEffectData);

	// コメント部分を作成
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  リングエフェクトのデータ\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// リングエフェクトのデータを作成した数だけ保存
	for (int nCntRing = 0; nCntRing < m_nNumRingEffectData; nCntRing++)
	{// 作成したリングエフェクトのデータの数だけ繰り返し
		fprintf(pFile, "%s\n", RINGSET);
		fprintf(pFile, "	%s%.4f %.4f %.4f			#向き\n", ROT, m_apRingData[nCntRing]->GetRot().x, m_apRingData[nCntRing]->GetRot().y, m_apRingData[nCntRing]->GetRot().z);
		fprintf(pFile, "	%s%.4f %.4f %.4f			#移動量の最大値\n", MAX_MOVE, m_apRingData[nCntRing]->GetMaxMove().x, m_apRingData[nCntRing]->GetMaxMove().y, m_apRingData[nCntRing]->GetMaxMove().z);
		fprintf(pFile, "	%s%.4f %.4f %.4f				#移動量の最小値\n", MIN_MOVE, m_apRingData[nCntRing]->GetMinMove().x, m_apRingData[nCntRing]->GetMinMove().y, m_apRingData[nCntRing]->GetMinMove().z);
		fprintf(pFile, "	%s%.4f %.4f %.4f			#移動量の変化量\n", CHANGE_MOVE, m_apRingData[nCntRing]->GetChangeMove().x, m_apRingData[nCntRing]->GetChangeMove().y, m_apRingData[nCntRing]->GetChangeMove().z);
		fprintf(pFile, "	%s%.4f %.4f %.4f %.4f			#生成時の色\n", INIT_COL, m_apRingData[nCntRing]->GetInitCol().r, m_apRingData[nCntRing]->GetInitCol().g, m_apRingData[nCntRing]->GetInitCol().b, m_apParData[nCntRing]->GetInitCol().a);
		fprintf(pFile, "	%s%.4f %.4f %.4f %.4f		#フレーム毎の色の変化量\n", CHANGE_COL, m_apRingData[nCntRing]->GetChangeCol().r, m_apRingData[nCntRing]->GetChangeCol().g, m_apRingData[nCntRing]->GetChangeCol().b, m_apRingData[nCntRing]->GetChangeCol().a);
		fprintf(pFile, "	%s%f						#高さ\n", HEIGHT, m_apRingData[nCntRing]->GetHeight());
		fprintf(pFile, "	%s%f						#半径\n", RADIUS, m_apRingData[nCntRing]->GetRadius());
		fprintf(pFile, "	%s%f						#幅\n", DIFFUSION, m_apRingData[nCntRing]->GetDiffusion());
		fprintf(pFile, "	%s%f						#高さを毎フレームどれくらい変化させる\n", CHANGE_HEIGHT, m_apRingData[nCntRing]->GetChangeHeight());
		fprintf(pFile, "	%s%f						#半径を毎フレームどれくらい変化させる\n", CHANGE_RADIUS, m_apRingData[nCntRing]->GetChangeRadius());
		fprintf(pFile, "	%s%f						#幅を毎フレームどれくらい変化させる\n", CHANGE_DIFFUSION, m_apRingData[nCntRing]->GetChangeDiffusion());
		fprintf(pFile, "	%s%d						#寿命の最大値\n", MAX_LIFE, m_apRingData[nCntRing]->GetMaxLife());
		fprintf(pFile, "	%s%d						#寿命の最小値\n", MIN_LIFE, m_apRingData[nCntRing]->GetMinLife());
		fprintf(pFile, "	%s%d						#横の分割数\n", XBLOCK, m_apRingData[nCntRing]->GetXBlock());
		fprintf(pFile, "	%s%d						#縦の分割数\n", YBLOCK, m_apRingData[nCntRing]->GetYBlock());
		fprintf(pFile, "	%s%d						#回転のパターン[ 0:時計回り 1:反時計回り 2:ランダム ]\n", ROT_PATTERN, m_apRingData[nCntRing]->GetRotPattern());
		fprintf(pFile, "	%s%.4f %.4f %.4f			#回転のスピード\n", ROT_SPEED, m_apRingData[nCntRing]->GetRotSpeed().x, m_apRingData[nCntRing]->GetRotSpeed().y, m_apRingData[nCntRing]->GetRotSpeed().z);
		fprintf(pFile, "	%s%d						#カリングするかしないか[ 0:なし 1:あり ]\n", CULLING, (int)m_apRingData[nCntRing]->GetCulling());
		fprintf(pFile, "	%s%d					#加算合成するかしないか[ 0:なし 1:あり ]\n", DRAW_ADDTIVE, (int)m_apRingData[nCntRing]->GetDrawAddtive());
		fprintf(pFile, "%s\n\n", END_RINGSET);
	}
}

//=============================================================================
//    スクリプトファイル名を設定する処理
//=============================================================================
void CEffectManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    エミッタへのポインタを設定する処理
//=============================================================================
void CEffectManager::SetEmitter(CEmitter *pEmitter)
{
	m_pEmitter = pEmitter;
}

//=============================================================================
//    エミッタデータを設定する処理
//=============================================================================
void CEffectManager::SetEmitterData(CEmitterData *pEmitterData, int nIdx)
{
	m_apEmitterData[nIdx] = pEmitterData;
}

//=============================================================================
//    パーティクルデータを設定する処理
//=============================================================================
void CEffectManager::SetParData(CParData *pParData, int nIdx)
{
	m_apParData[nIdx] = pParData;
}

//=============================================================================
//    リングエフェクトデータを設定する処理
//=============================================================================
void CEffectManager::SetRingEffectData(CRingData *pRingData, int nIdx)
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
void CEffectManager::SetNumEmitterData(int nNumEmitterData)
{
	m_nNumEmitterData = nNumEmitterData;
}

//=============================================================================
//    パーティクルデータの数を設定する処理
//=============================================================================
void CEffectManager::SetNumParData(int nNumParData)
{
	m_nNumParData = nNumParData;
}

//=============================================================================
//    リングエフェクトデータの数を設定する処理
//=============================================================================
void CEffectManager::SetNumRingEffectData(int nNumRingEffectData)
{
	m_nNumRingEffectData = nNumRingEffectData;
}

//=============================================================================
//    テクスチャを取得する処理
//=============================================================================
LPDIRECT3DTEXTURE9 CEffectManager::BindTexture(int nIdx)
{
	return m_pTextureManager->GetTexture(nIdx);
}

//=============================================================================
//    エミッタクラスへのポインタを取得する処理
//=============================================================================
CEmitter *CEffectManager::GetEmitter(void)
{
	return m_pEmitter;
}

//=============================================================================
//    エミッタデータを取得する処理
//=============================================================================
CEmitterData *CEffectManager::GetEmitterData(int nIdx)
{
	return m_apEmitterData[nIdx];
}

//=============================================================================
//    パーティクルデータを取得する処理
//=============================================================================
CParData *CEffectManager::GetParData(int nIdx)
{
	return m_apParData[nIdx];
}

//=============================================================================
//    リングエフェクトデータを取得する処理
//=============================================================================
CRingData *CEffectManager::GetRingEffectData(int nIdx)
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