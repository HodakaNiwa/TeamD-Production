//*****************************************************************************
//
//     モーションの処理[motion.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "motion.h"
#include "model.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MOTION_SAVE_FILENAME "data/TEXT/key_info.txt"    // モーションデータを書き出すファイル名

// テキストファイルから読み込む際の目安となるパス名
#define MOTIONSET        "MOTIONSET"
#define LOOP             "LOOP"
#define BLEND            "BLEND"
#define NUM_KEY          "NUM_KEY"
#define KEYSET           "KEYSET"
#define FRAME            "FRAME"
#define KEY              "KEY"
#define POS              "POS"
#define ROT              "ROT"
#define END_KEY          "END_KEY"
#define END_KEYSET       "END_KEYSET"
#define END_MOTIONSET    "END_MOTIONSET"

#define MAX_KEYFRAME     (20)     // 読み込めるキーフレームの最大数
#define MAX_MODEL_NUM    (50)     // 読み込めるモデルの最大数

//*****************************************************************************
//     CMotionAttackの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CMotionAttack::CMotionAttack()
{
	// 各種値のクリア
	m_nNextMotionIdx = 0;      // 次のモーションに派生する番号
	m_nStartTiming = 0;        // 次のモーションに派生できる時間が始まるタイミング
	m_nFinishTiming = 0;       // 次のモーションに派生できる時間が終わるタイミング
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMotionAttack::~CMotionAttack()
{

}


//*****************************************************************************
//     CMotionCollisionの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CMotionCollision::CMotionCollision()
{
	// 各種値のクリア
	m_nModelIdx = 0;                              // 当たり判定を取るモデルの番号
	m_OffSetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 当たり判定を取るモデルからのオフセット距離
	m_fRange = 0.0f;                              // 当たり判定を取る範囲
	m_nStartTiming = 0;                           // 当たり判定を取り始める時間
	m_nFinishTiming = 0;                          // 当たり判定を取り終わる時間
	m_nDamage = 0;                                // 当たった場合のダメージ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMotionCollision::~CMotionCollision()
{

}

//*****************************************************************************
//     CMotionOrbitの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CMotionOrbit::CMotionOrbit()
{
	// 各種値のクリア
	m_nModelIdx = 0;                                           // 軌跡を出すモデルの番号
	m_nTexIdx = 0;                                             // 軌跡に貼るテクスチャの番号
	m_OffSetPos1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);              // 軌跡を出すモデルからのオフセット距離(1番目)
	m_OffSetPos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);              // 軌跡を出すモデルからのオフセット距離(2番目)
	D3DXVECTOR3 m_OffSetAmp1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 軌跡のオフセット増幅値(1番目)
	D3DXVECTOR3 m_OffSetAmp2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 軌跡のオフセット増幅値(2番目)
	m_ColUp = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);                  // 軌跡の頂点上側の色
	m_ColDown = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);             // 軌跡の頂点下側の色
	m_fAlphaDecayUp = 0.0f;                                    // 軌跡の頂点上側の透明度減衰値
	m_fAlphaDecayDown = 0.0f;                                  // 軌跡の頂点上側の透明度減衰値
	m_nXBlock = 0;                                             // 軌跡の横の分割数
	m_nYBlock = 0;                                             // 軌跡の縦の分割数
	m_nStartTiming = 0;                                        // 軌跡を出し始めるタイミング
	m_nFinishTiming = 0;                                       // 軌跡をしまい始めるタイミング
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMotionOrbit::~CMotionOrbit()
{

}


//*****************************************************************************
//     CMotionManagerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CMotionManager::CMotionManager()
{
	// 各種値のクリア
	m_pMotion = NULL;        // モーションクラスへのポインタ
	m_nNumber = 0;           // 現在のモーション番号
	m_nNumberOld = 0;        // 前回のモーション番号
	m_nNumAll = 0;           // モーションの総数
	m_nKey = 0;              // 現在のキーフレーム
	m_nNumModel = 0;         // モデル数
	m_nBlendCounter = 0;     // ブレンドを管理するカウンター
	m_nCounter = 0;          // モーション管理カウンター
	m_nAllCounter = 0;       // 現在の総再生フレーム数
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMotionManager::~CMotionManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMotionManager *CMotionManager::Create(int nNumMotion, int nNumModel)
{
	CMotionManager *pMotionManager = NULL;   // モーション管理クラス型のポインタ
	if (pMotionManager == NULL)
	{// メモリが確保できる状態にある
		pMotionManager = new CMotionManager; // インスタンスを生成
		if (pMotionManager != NULL)
		{// メモリが確保できた
			if (FAILED(pMotionManager->Init(nNumMotion, nNumModel)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// メモリが確保できなかった
			return NULL;
		}
	}
	else
	{// メモリが確保できる状態にない
		return NULL;
	}

	return pMotionManager;  // ポインタを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMotionManager::Init(int nNumMotion, int nNumModel)
{
	// 各種値の代入
	m_nNumAll = nNumMotion;    // モーションの総数
	m_nNumModel = nNumModel;   // モデルの総数
	m_nNumber = 0;             // 現在のモーション番号
	m_nNumberOld = m_nNumber;  // 前回のモーション番号

	if (m_pMotion == NULL)
	{// メモリを確保できる状態である
		m_pMotion = new CMotion*[nNumMotion];
		if (m_pMotion != NULL)
		{// メモリが確保できた
			for (int nCntMotion = 0; nCntMotion < nNumMotion; nCntMotion++)
			{// モーションの総数だけ繰り返し
				m_pMotion[nCntMotion] = NULL;
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMotionManager::Uninit(void)
{
	if (m_pMotion != NULL)
	{// メモリが確保されている
		for (int nCntMotion = 0; nCntMotion < m_nNumAll; nCntMotion++)
		{// モーションの総数分繰り返し
			if (m_pMotion[nCntMotion] != NULL)
			{// メモリが確保されている
				// モーションの終了処理
				m_pMotion[nCntMotion]->Uninit(m_nNumModel);

				// メモリの開放
				delete m_pMotion[nCntMotion];
				m_pMotion[nCntMotion] = NULL;
			}
		}
		// メモリの開放
		delete[] m_pMotion;
		m_pMotion = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CMotionManager::Update(CModel**pModel)
{
	if (m_pMotion[m_nNumber] != NULL)
	{// メモリが確保されている
		// モーションの更新処理
		m_pMotion[m_nNumber]->Update(pModel, m_nNumModel, &m_nKey, &m_nCounter, &m_nBlendCounter, &m_nAllCounter);
	}
}

//=============================================================================
//    モーションの切り替え処理
//=============================================================================
void CMotionManager::SwitchMotion(CModel **pModel, int nNumber)
{
	m_nCounter = 0;      // カウンターを戻す
	m_nBlendCounter = 0; // ブレンドカウンターを戻す
	m_nAllCounter = 0;   // 総再生フレーム数を戻す

	// 前回のモーション番号を記憶し状態を戻す
	m_pMotion[m_nNumberOld]->SetState(CMotion::STATE_NORMAL);
	m_nNumberOld = m_nNumber;

	// モーションの番号を合わせる
	m_nNumber = nNumber;  // 番号を合わせる
	m_nKey = 0;           // キーフレームを戻す
	m_pMotion[m_nNumber]->SetState(CMotion::STATE_SWITCH);  // 切り替え状態に

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{// モデルの数だけ繰り返し
		D3DXVECTOR3 DiffRot;  // 差分の向き
		D3DXVECTOR3 DestPos;  // 目的の位置

		// モーションのオフセット目的の位置を計算
		DestPos.x = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetAddPos().x - pModel[nCntModel]->GetAddPos().x;
		DestPos.y = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetAddPos().y - pModel[nCntModel]->GetAddPos().y;
		DestPos.z = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetAddPos().z - pModel[nCntModel]->GetAddPos().z;

		// X軸の向きの差分を求める
		DiffRot.x = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetDestRot().x - pModel[nCntModel]->GetRot().x;
		if (DiffRot.x > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			DiffRot.x -= D3DX_PI * 2.0f;
		}
		if (DiffRot.x < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			DiffRot.x += D3DX_PI * 2.0f;
		}

		// Y軸の向きの差分を求める
		DiffRot.y = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetDestRot().y - pModel[nCntModel]->GetRot().y;
		if (DiffRot.y > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			DiffRot.y -= D3DX_PI * 2.0f;
		}
		if (DiffRot.y < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			DiffRot.y += D3DX_PI * 2.0f;
		}

		// Z軸の向きの差分を求める
		DiffRot.z = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetDestRot().z - pModel[nCntModel]->GetRot().z;
		if (DiffRot.z > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			DiffRot.z -= D3DX_PI * 2.0f;
		}
		if (DiffRot.z < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			DiffRot.z += D3DX_PI * 2.0f;
		}

		// 差分の向きを設定する
		m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->SetDiffRot(DiffRot);
		// 目的の位置を設定する
		m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->SetDestPos(DestPos);
	}
}

//=============================================================================
//    モーションクラスへのポインタを設定
//=============================================================================
void CMotionManager::SetMotion(CMotion *pMotion, int nIdx)
{
	m_pMotion[nIdx] = pMotion;
}

//=============================================================================
//    現在のキーフレームを設定
//=============================================================================
void CMotionManager::SetKey(int nKey)
{
	m_nKey = nKey;
}

//=============================================================================
//    現在の総再生フレーム数を設定
//=============================================================================
void CMotionManager::SetAllCounter(int nAllCounter)
{
	m_nAllCounter = nAllCounter;
}

//=============================================================================
//    現在のモーション番号を設定
//=============================================================================
void CMotionManager::SetNumber(int nNumber)
{
	m_nNumber = nNumber;
}

//=============================================================================
//    現在のモーション番号を取得
//=============================================================================
int CMotionManager::GetNumber(void)
{
	return m_nNumber;
}

//=============================================================================
//    モデルの総数を取得
//=============================================================================
int CMotionManager::GetNumModel(void)
{
	return m_nNumModel;
}

//=============================================================================
//    現在のキーフレームを取得
//=============================================================================
int CMotionManager::GetKey(void)
{
	return m_nKey;
}

//=============================================================================
//    現在の再生フレームを取得
//=============================================================================
int CMotionManager::GetCounter(void)
{
	return m_nCounter;
}

//=============================================================================
//    現在の総再生フレームを取得
//=============================================================================
int CMotionManager::GetAllCounter(void)
{
	return m_nAllCounter;
}

//=============================================================================
//    モーションクラスへのポインタ取得処理
//=============================================================================
CMotion **CMotionManager::GetMotion(void)
{
	return m_pMotion;
}

//=============================================================================
//    モーションのキーフレーム情報保存処理
//=============================================================================
void CMotionManager::Save(void)
{
	FILE *pFile = NULL;   // ファイルポインタ

	if (pFile == NULL)
	{// ファイル確保できる状態である
		pFile = fopen(MOTION_SAVE_FILENAME, "w");   // ファイルオープン
		if (pFile != NULL)
		{// ファイルが開けた
			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "# モーション情報\n");
			fprintf(pFile, "#  [ モーション番号 : %d ]\n", m_nNumber);
			fprintf(pFile, "#\n");
			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "\n");

			// モーションの情報保存開始
			fprintf(pFile, "%s\n", MOTIONSET);
			fprintf(pFile, "	%s = %d		# ループするかどうか[0:ループしない / 1:ループする]\n", LOOP, m_pMotion[m_nNumber]->GetLoop());
			fprintf(pFile, "	%s = %.2f	# ブレンドの倍率\n", BLEND, m_pMotion[m_nNumber]->GetBlendMag());
			fprintf(pFile, "	%s = %d		# キーフレームの総数\n", NUM_KEY, m_pMotion[m_nNumber]->GetNumKey());

			for (int nCntKey = 0; nCntKey < m_pMotion[m_nNumber]->GetNumKey(); nCntKey++)
			{// キーフレームの総数分繰り返し
			    // キーフレームの情報を保存する
				fprintf(pFile, "\n	%s			# --- << KEY : %d / %d >> ---\n", KEYSET, nCntKey, m_pMotion[m_nNumber]->GetNumKey());
				fprintf(pFile, "		%s = %d\n", FRAME, m_pMotion[m_nNumber]->GetPlayBackKey(nCntKey));
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{// モデルの数だけ繰り返し
					// モデルごとのオフセット情報を保存
					fprintf(pFile, "		%s # ----- [ %d ] -----\n", KEY, nCntModel);
					fprintf(pFile, "			%s = %.2f %.2f %.2f\n", POS, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetAddPos().x, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetAddPos().y, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetAddPos().z);
					fprintf(pFile, "			%s = %.2f %.2f %.2f\n", ROT, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetDestRot().x, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetDestRot().y, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetDestRot().z);
					fprintf(pFile, "		%s\n\n", END_KEY);
				}
				fprintf(pFile, "	%s\n", END_KEYSET);
			}

			fprintf(pFile, "%s\n", END_MOTIONSET);
			fclose(pFile);   // ファイルを閉じる
		}
	}
}


//*****************************************************************************
//     CMotionの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CMotion::CMotion()
{
	// 各種値のクリア
	m_pMotionAttack = NULL;    // 攻撃モーションデータクラスへのポインタ
	m_pCollision = NULL;       // 当たり判定データクラスへのポインタ
	m_nNumColData = 0;         // 当たり判定データクラスの数
	m_pOrbit = 0;              // 軌跡データクラスへのポインタ
	m_nNumOrbitData = 0;       // 軌跡データの数
	m_pKeyframe = NULL;        // キーフレームの情報
	m_pPlaybackKey = NULL;     // 再生フレーム数へのポインタ
	m_nAllPlaybackKey = 0;     // 総再生フレーム数
	m_State = STATE_NORMAL;    // 状態
	m_nNumKey = 0;             // キーフレームの数
	m_fBlendMag = 0.0f;        // モーションブレンド倍率
	m_bLoop = false;           // ループするかしないか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMotion *CMotion::Create(int nNumModel, int nNumKey, float fBlendMag, bool bLoop, int *pPlaybackKey)
{
	CMotion *pMotion = NULL;   // モーションクラス型のポインタ
	if (pMotion == NULL)
	{// メモリが確保できる状態にある
		pMotion = new CMotion; // インスタンスを生成
		if (pMotion != NULL)
		{// メモリが確保できた
			if (FAILED(pMotion->Init(nNumModel, nNumKey, fBlendMag, bLoop, pPlaybackKey)))
			{// 初期化に失敗した
				return NULL;
			}
		}
		else
		{// メモリが確保できなかった
			return NULL;
		}
	}
	else
	{// メモリが確保できる状態にない
		return NULL;
	}

	return pMotion;  // ポインタを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CMotion::Init(int nNumModel, int nNumKey, float fBlendMag, bool bLoop, int *pPlaybackKey)
{
	// キーフレームの情報をモデルの数とキーフレーム数分動的に確保
	if (nNumKey >= 1)
	{// 総フレーム数が１個以上ある
		if (m_pKeyframe == NULL)
		{// キーフレームへのポインタが空になっている
			m_pKeyframe = new CKeyframe**[nNumKey];  // キーフレーム情報へのポインタのポインタをキーフレーム数分確保
			if (m_pKeyframe != NULL)
			{// メモリが確保できた
				for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
				{// キーフレームの数だけ繰り返し
					m_pKeyframe[nCntKey] = new CKeyframe*[nNumModel];  // キーフレーム情報へのポインタをモデルの数分を確保
					if (m_pKeyframe[nCntKey] != NULL)
					{// メモリが確保できた
						for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
						{// キーフレームの数だけ繰り返し
							m_pKeyframe[nCntKey][nCntModel] = new CKeyframe; // キーフレームの情報のインスタンスを生成
							if (m_pKeyframe[nCntKey][nCntModel] != NULL)
							{// メモリが確保できた
								// キーフレームの情報を初期化する
								m_pKeyframe[nCntKey][nCntModel]->Init();
							}
						}
					}
				}
			}
		}

		if (m_pPlaybackKey == NULL)
		{// メモリが確保できる状態にある
			m_pPlaybackKey = new int[nNumKey];  // キーフレームの数だけ再生フレーム数の情報をインスタンス生成
			if (m_pPlaybackKey != NULL)
			{// メモリが確保できた
				for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
				{// キーフレームの数だけ繰り返し
					m_pPlaybackKey[nCntKey] = pPlaybackKey[nCntKey];
					m_nAllPlaybackKey += pPlaybackKey[nCntKey];
				}
			}
		}
	}

	m_nNumKey = nNumKey;      // キーフレームの数
	m_fBlendMag = fBlendMag;  // ブレンドの倍率
	m_bLoop = bLoop;          // ループするかしないか

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CMotion::Uninit(int nNumModel)
{
	// キーフレームクラスの開放
	if (m_pKeyframe != NULL)
	{// メモリが確保されている
		for (int nCntKey = 0; nCntKey < m_nNumKey; nCntKey++)
		{// キーフレームの数だけ繰り返し
			if (m_pKeyframe[nCntKey] != NULL)
			{// メモリが確保されている
				for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
				{// モデルの数だけ繰り返し
					if (m_pKeyframe[nCntKey][nCntModel] != NULL)
					{// メモリが空になっていない
						// 終了処理
						m_pKeyframe[nCntKey][nCntModel]->Uninit();

						// メモリの開放
						delete m_pKeyframe[nCntKey][nCntModel];
						m_pKeyframe[nCntKey][nCntModel] = NULL;
					}
				}
				// メモリの開放
				delete[] m_pKeyframe[nCntKey];
				m_pKeyframe[nCntKey] = NULL;
			}
			if (m_pPlaybackKey!= NULL)
			{// メモリが確保されている
				delete[] m_pPlaybackKey;
				m_pPlaybackKey = NULL;
			}
		}
		// メモリの開放
		delete[] m_pKeyframe;
		m_pKeyframe = NULL;
	}

	// 攻撃モーションデータの開放
	if (m_pMotionAttack != NULL)
	{
		delete m_pMotionAttack;
		m_pMotionAttack = NULL;
	}

	// 当たり判定データの開放
	if (m_pCollision != NULL)
	{
		for (int nCntCol = 0; nCntCol < m_nNumColData; nCntCol++)
		{// 当たり判定データの数だけ繰り返し
			if (m_pCollision[nCntCol] != NULL)
			{
				delete m_pCollision[nCntCol];
				m_pCollision[nCntCol] = NULL;
			}
		}
		delete[] m_pCollision;
		m_pCollision = NULL;
	}

	// 軌跡データの開放
	if (m_pOrbit != NULL)
	{
		for (int nCntOrbit = 0; nCntOrbit < m_nNumOrbitData; nCntOrbit++)
		{// 当たり判定データの数だけ繰り返し
			if (m_pOrbit[nCntOrbit] != NULL)
			{
				delete m_pOrbit[nCntOrbit];
				m_pOrbit[nCntOrbit] = NULL;
			}
		}
		delete[] m_pOrbit;
		m_pOrbit = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CMotion::Update(CModel **pModel, int nNumModel, int *pKey, int *pCounter, int *pBlendCounter, int *pAllCounter)
{
	D3DXVECTOR3 ModelRot;     // モデルの現在のオフセット向き情報
	D3DXVECTOR3 ModelAddPos;  // モデルのオフセットに加える座標情報

	// モーションカウンターを進める
	if (m_State == STATE_NORMAL)
	{// モーションを再生する状態だったら
		*pCounter = *pCounter + 1;  // モーションカウンターを進める
		*pAllCounter += 1;
		if (*pCounter >= m_pPlaybackKey[*pKey])
		{// モーションカウンターが再生フレーム数を上回った
			*pCounter = 0;        // モーションカウンターを戻す
			*pKey = *pKey + 1;    // キーフレームを進める
			if (*pKey >= m_nNumKey)
			{// 現在のキーフレームがキーフレームの総数を上回った
				if (m_bLoop == true)
				{// ループする
					*pKey = 0;  // 現在のキーフレーム数を戻す
				}
				else
				{// ループしない
					m_State = STATE_STOP;  // 停止状態に
				}
			}

			// キーフレームの切り替え処理
			if (m_State == STATE_NORMAL)
			{// 通常状態ならば
				SwitchKey(pModel, nNumModel, *pKey);
			}
		}

		if (m_State == STATE_NORMAL)
		{// 通常状態ならば
			for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
			{// モデルの数だけ繰り返し
				if (pModel[nCntModel] != NULL)
				{// モデルのポインタが空ではない
					ModelRot = pModel[nCntModel]->GetRot();        // モデルの現在の向きを取得
					ModelAddPos = pModel[nCntModel]->GetAddPos();  // モデルのオフセットに加える位置を取得

					// モデルのオフセットに加える値を動かす
					ModelAddPos.x += m_pKeyframe[*pKey][nCntModel]->GetDestPos().x / m_pPlaybackKey[*pKey];
					ModelAddPos.y += m_pKeyframe[*pKey][nCntModel]->GetDestPos().y / m_pPlaybackKey[*pKey];
					ModelAddPos.z += m_pKeyframe[*pKey][nCntModel]->GetDestPos().z / m_pPlaybackKey[*pKey];

					// X軸の差分を加える
					ModelRot.x += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().x / m_pPlaybackKey[*pKey];
					if (ModelRot.x > D3DX_PI)
					{// 現在の向きがD3DX_PIを超えた
						ModelRot.x -= D3DX_PI * 2.0f;
					}
					if (ModelRot.x < -D3DX_PI)
					{// 現在の向きが-D3DX_PIを超えた
						ModelRot.x += D3DX_PI * 2.0f;
					}

					// Y軸の差分を加える
					ModelRot.y += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().y / m_pPlaybackKey[*pKey];
					if (ModelRot.y > D3DX_PI)
					{// 現在の向きがD3DX_PIを超えた
						ModelRot.y -= D3DX_PI * 2.0f;
					}
					if (ModelRot.y < -D3DX_PI)
					{// 現在の向きが-D3DX_PIを超えた
						ModelRot.y += D3DX_PI * 2.0f;
					}

					// Z軸の差分を加える
					ModelRot.z += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().z / m_pPlaybackKey[*pKey];
					if (ModelRot.z > D3DX_PI)
					{// 現在の向きがD3DX_PIを超えた
						ModelRot.z -= D3DX_PI * 2.0f;
					}
					if (ModelRot.z < -D3DX_PI)
					{// 現在の向きが-D3DX_PIを超えた
						ModelRot.z += D3DX_PI * 2.0f;
					}

					// モデルにオフセット情報を設定
					pModel[nCntModel]->SetRot(ModelRot);        // 向き
					pModel[nCntModel]->SetAddPos(ModelAddPos);  // オフセットに加える位置
				}
			}
		}
	}
	else if(m_State == STATE_SWITCH)
	{// 切り替え状態ならば
		*pBlendCounter = *pBlendCounter + 1;  // モーションカウンターを進める
		*pAllCounter += 1;
		for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
		{// モデルの数だけ繰り返し
			if (pModel[nCntModel] != NULL)
			{// モデルのポインタが空ではない
				ModelRot = pModel[nCntModel]->GetRot();        // モデルの現在の向きを取得
				ModelAddPos = pModel[nCntModel]->GetAddPos();  // モデルの現在の位置を取得

				// モデルのオフセット位置を動かす
				ModelAddPos.x += m_pKeyframe[*pKey][nCntModel]->GetDestPos().x / (m_pPlaybackKey[*pKey] * m_fBlendMag);
				ModelAddPos.y += m_pKeyframe[*pKey][nCntModel]->GetDestPos().y / (m_pPlaybackKey[*pKey] * m_fBlendMag);
				ModelAddPos.z += m_pKeyframe[*pKey][nCntModel]->GetDestPos().z / (m_pPlaybackKey[*pKey] * m_fBlendMag);

				// X軸の差分を加える
				ModelRot.x += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().x / (m_pPlaybackKey[*pKey] * m_fBlendMag);
				if (ModelRot.x > D3DX_PI)
				{// 現在の向きがD3DX_PIを超えた
					ModelRot.x -= D3DX_PI * 2.0f;
				}
				if (ModelRot.x < -D3DX_PI)
				{// 現在の向きが-D3DX_PIを超えた
					ModelRot.x += D3DX_PI * 2.0f;
				}

				// Y軸の差分を加える
				ModelRot.y += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().y / (m_pPlaybackKey[*pKey] * m_fBlendMag);
				if (ModelRot.y > D3DX_PI)
				{// 現在の向きがD3DX_PIを超えた
					ModelRot.y -= D3DX_PI * 2.0f;
				}
				if (ModelRot.y < -D3DX_PI)
				{// 現在の向きが-D3DX_PIを超えた
					ModelRot.y += D3DX_PI * 2.0f;
				}

				// Z軸の差分を加える
				ModelRot.z += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().z / (m_pPlaybackKey[*pKey] * m_fBlendMag);
				if (ModelRot.z > D3DX_PI)
				{// 現在の向きがD3DX_PIを超えた
					ModelRot.z -= D3DX_PI * 2.0f;
				}
				if (ModelRot.z < -D3DX_PI)
				{// 現在の向きが-D3DX_PIを超えた
					ModelRot.z += D3DX_PI * 2.0f;
				}

				// モデルに向きを設定
				pModel[nCntModel]->SetRot(ModelRot);        // 向き
				pModel[nCntModel]->SetAddPos(ModelAddPos);  // オフセットに加える位置
			}
		}

		if (*pBlendCounter >= (m_pPlaybackKey[*pKey] * m_fBlendMag))
		{// ブレンドカウンターが再生フレーム数を上回った
			*pBlendCounter = 0;     // カウンターを戻す
			*pKey = *pKey + 1;      // キーフレームを進める
			if (*pKey >= m_nNumKey)
			{// 現在のキーフレームがキーフレームの総数を上回った
				if (m_bLoop == true)
				{// ループする
					*pKey = 0;  // 現在のキーフレーム数を戻す
				}
				else
				{// ループしない
					m_State = STATE_STOP;  // 停止状態に
				}
			}
			m_State = STATE_NORMAL; // 通常状態に戻す

			// キーフレームの切り替え
			SwitchKey(pModel, nNumModel, *pKey);
		}
	}
}

//=============================================================================
//    モーションの攻撃モーションデータクラスへのポインタ取得処理
//=============================================================================
CMotionAttack *CMotion::GetAttack(void)
{
	return m_pMotionAttack;
}

//=============================================================================
//    モーションの当たり判定データクラスへのポインタ取得処理
//=============================================================================
CMotionCollision **CMotion::GetCollision(void)
{
	return m_pCollision;
}

//=============================================================================
//    当たり判定データの数を取得する処理
//=============================================================================
int CMotion::GetNumColData(void)
{
	return m_nNumColData;
}

//=============================================================================
//    モーションの軌跡データクラスへのポインタを取得する処理
//=============================================================================
CMotionOrbit **CMotion::GetOrbit(void)
{
	return m_pOrbit;
}

//=============================================================================
//    軌跡データの数を設定する処理
//=============================================================================
int CMotion::GetNumOrbitData(void)
{
	return m_nNumOrbitData;
}

//=============================================================================
//    状態を取得
//=============================================================================
CMotion::STATE CMotion::GetState(void)
{
	return m_State;
}

//=============================================================================
//    キーフレームの総数を取得
//=============================================================================
int CMotion::GetNumKey(void)
{
	return m_nNumKey;
}

//=============================================================================
//    キーフレームの情報を取得する
//=============================================================================
CKeyframe ***CMotion::GetKeyframe(void)
{
	return m_pKeyframe;
}

//=============================================================================
//    ループするかしないかを取得する
//=============================================================================
bool CMotion::GetLoop(void)
{
	return m_bLoop;
}

//=============================================================================
//    キーフレームの再生フレーム数を取得する
//=============================================================================
int CMotion::GetPlayBackKey(int nIdx)
{
	return m_pPlaybackKey[nIdx];
}

//=============================================================================
//    キーフレームの再生フレーム数を取得する(ポインタ)
//=============================================================================
int *CMotion::GetPlayBackKey(void)
{
	return m_pPlaybackKey;
}

//=============================================================================
//    モーションの総再生フレーム数を取得する
//=============================================================================
int CMotion::GetAllPlayBackKey(void)
{
	return m_nAllPlaybackKey;
}

//=============================================================================
//    モーションのブレンド倍率を取得する
//=============================================================================
float CMotion::GetBlendMag(void)
{
	return m_fBlendMag;
}

//=============================================================================
//    攻撃モーションデータクラスへのポインタ設定処理
//=============================================================================
void CMotion::SetAttack(CMotionAttack *pAttack)
{
	m_pMotionAttack = pAttack;
}

//=============================================================================
//    当たり判定データクラスへのポインタ設定処理
//=============================================================================
void CMotion::SetCollision(CMotionCollision **pCollision)
{
	m_pCollision = pCollision;
}

//=============================================================================
//    当たり判定データの数設定処理
//=============================================================================
void CMotion::SetNumColData(const int nNumColData)
{
	m_nNumColData = nNumColData;
}

//=============================================================================
//    モーションの軌跡データクラスへのポインタ設定処理
//=============================================================================
void CMotion::SetOrbit(CMotionOrbit **pOrbit)
{
	m_pOrbit = pOrbit;
}

//=============================================================================
//    軌跡データの数設定処理
//=============================================================================
void CMotion::SetNumOrbitData(const int nNumOrbitData)
{
	m_nNumOrbitData = nNumOrbitData;
}

//=============================================================================
//    キーフレーム情報を設定する
//=============================================================================
void CMotion::SetKeyFrame(const int nIdxModel, const int nIdxKey, CKeyframe *pKeyFrame)
{
	m_pKeyframe[nIdxKey][nIdxModel] = pKeyFrame;
}

//=============================================================================
//    状態を設定
//=============================================================================
void CMotion::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
//    キーフレーム数を設定
//=============================================================================
void CMotion::SetNumKey(const int nNumKey)
{
	m_nNumKey = nNumKey;
}

//=============================================================================
//    ループするかしないかを設定
//=============================================================================
void CMotion::SetLoop(const bool bLoop)
{
	m_bLoop = bLoop;
}

//=============================================================================
//    再生フレーム数を設定
//=============================================================================
void CMotion::SetPlayBackKey(const int nPlayBackKey, int nIdx)
{
	m_pPlaybackKey[nIdx] = nPlayBackKey;
}

//=============================================================================
//    再生フレーム数を設定(ポインタ)
//=============================================================================
void CMotion::SetPlayBackKey(int *pPlayBackKey)
{
	m_pPlaybackKey = pPlayBackKey;
}

//=============================================================================
//    総再生フレーム数を設定
//=============================================================================
void CMotion::SetAllPlayBackKey(int nAllPlayBackKey)
{
	m_nAllPlaybackKey = nAllPlayBackKey;
}

//=============================================================================
//    ブレンド倍率を設定
//=============================================================================
void CMotion::SetBlendMag(const float fBlendMag)
{
	m_fBlendMag = fBlendMag;
}

//=============================================================================
//    キーフレーム切り替え処理
//=============================================================================
void CMotion::SwitchKey(CModel **pModel, int nNumModel, int nKey)
{
	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{// モデルの数だけ繰り返し
		D3DXVECTOR3 DiffRot;  // 差分の向き
		D3DXVECTOR3 DestPos;  // 目的の位置

		// モーションのオフセット目的の位置を計算
		DestPos.x = m_pKeyframe[nKey][nCntModel]->GetAddPos().x - pModel[nCntModel]->GetAddPos().x;
		DestPos.y = m_pKeyframe[nKey][nCntModel]->GetAddPos().y - pModel[nCntModel]->GetAddPos().y;
		DestPos.z = m_pKeyframe[nKey][nCntModel]->GetAddPos().z - pModel[nCntModel]->GetAddPos().z;

		// X軸の向きの差分を求める
		DiffRot.x = m_pKeyframe[nKey][nCntModel]->GetDestRot().x - pModel[nCntModel]->GetRot().x;
		if (DiffRot.x > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			DiffRot.x -= D3DX_PI * 2.0f;
		}
		if (DiffRot.x < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			DiffRot.x += D3DX_PI * 2.0f;
		}

		// Y軸の向きの差分を求める
		DiffRot.y = m_pKeyframe[nKey][nCntModel]->GetDestRot().y - pModel[nCntModel]->GetRot().y;
		if (DiffRot.y > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			DiffRot.y -= D3DX_PI * 2.0f;
		}
		if (DiffRot.y < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			DiffRot.y += D3DX_PI * 2.0f;
		}

		// Z軸の向きの差分を求める
		DiffRot.z = m_pKeyframe[nKey][nCntModel]->GetDestRot().z - pModel[nCntModel]->GetRot().z;
		if (DiffRot.z > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			DiffRot.z -= D3DX_PI * 2.0f;
		}
		if (DiffRot.z < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			DiffRot.z += D3DX_PI * 2.0f;
		}

		// 差分の向きを設定する
		m_pKeyframe[nKey][nCntModel]->SetDiffRot(DiffRot);
		// 目的の位置を設定する
		m_pKeyframe[nKey][nCntModel]->SetDestPos(DestPos);
	}
}

//*****************************************************************************
//     CKeyframeの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CKeyframe::CKeyframe()
{
	// 各種値のクリア
	m_AddPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 現在のオフセット位置に加える値
	m_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 目的のオフセット位置
	m_DestRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 目的のオフセット向き
	m_DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 差分の向き
}

//=============================================================================
//    デストラクタ
//=============================================================================
CKeyframe::~CKeyframe()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CKeyframe::Init(void)
{
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CKeyframe::Uninit(void)
{

}

//=============================================================================
//    更新処理
//=============================================================================
void CKeyframe::Update(void)
{

}

//=============================================================================
//    現在のオフセット位置に加える値を設定する
//=============================================================================
void CKeyframe::SetAddPos(D3DXVECTOR3 pos)
{
	m_AddPos = pos;
}

//=============================================================================
//    目的のオフセット位置を設定する
//=============================================================================
void CKeyframe::SetDestPos(D3DXVECTOR3 pos)
{
	m_DestPos = pos;
}

//=============================================================================
//    目的のオフセット向きを設定する
//=============================================================================
void CKeyframe::SetDestRot(D3DXVECTOR3 rot)
{
	m_DestRot = rot;
}

//=============================================================================
//    差分の向きを設定する
//=============================================================================
void CKeyframe::SetDiffRot(D3DXVECTOR3 rotDiff)
{
	m_DiffRot = rotDiff;
}

//=============================================================================
//    現在のオフセット位置に加える値を取得する
//=============================================================================
D3DXVECTOR3 CKeyframe::GetAddPos(void)
{
	return m_AddPos;
}

//=============================================================================
//    目的のオフセット位置を取得する
//=============================================================================
D3DXVECTOR3 CKeyframe::GetDestPos(void)
{
	return m_DestPos;
}

//=============================================================================
//    目的のオフセット向きを取得する
//=============================================================================
D3DXVECTOR3 CKeyframe::GetDestRot(void)
{
	return m_DestRot;
}

//=============================================================================
//    差分の向きを取得する
//=============================================================================
D3DXVECTOR3 CKeyframe::GetDiffRot(void)
{
	return m_DiffRot;
}