//=============================================================================
//
// プレイヤーマネージャーの処理 [playerManager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "playerManager.h"
#include "manager.h"
#include "renderer.h"
#include "fileLoader.h"
#include "functionlib.h"
#include "player.h"
#include "model.h"
#include "modelcreate.h"
#include "motion.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "
#define CHARACTERSET "CHARACTERSET"
#define END_CHARACTERSET "END_CHARACTERSET"
#define MOTIONSET "MOTIONSET"
#define END_MOTIONSET "END_MOTIONSET"
#define NUM_PARTS "NUM_PARTS = "
#define PARTSSET "PARTSSET"
#define END_PARTSSET "END_PARTSSET"
#define INDEX "INDEX = "
#define PARENT "PARENT = "
#define POS "POS = "
#define ROT "ROT = "
#define NUM_PARTS "NUM_PARTS = "
#define LOOP "LOOP = "
#define NUM_KEY "NUM_KEY ="
#define KEYSET "KEYSET"
#define END_KEYSET "END_KEYSET"
#define FRAME "FRAME = "
#define KEY "KEY"
#define END_KEY "END_KEY"

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayerManager::CPlayerManager()
{
	m_nNumPlayer = 1;			//プレイヤー総数
	m_pStock = NULL;			//残機数
	m_pScoreInfo = NULL;		//スコア情報
	m_pBarrier = NULL;			//バリア情報
	m_pModelCreate = NULL;		//モデル生成のポインタ
	m_apModel = NULL;			//モデルのポインタ情報
	m_pMotionManager = NULL;	//モーションマネージャーのポインタ情報
	m_nNumParts = 0;			//パーツの総数
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayerManager::~CPlayerManager()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CPlayerManager *CPlayerManager::Create(char *PlayerFileName)
{
	CPlayerManager *pPlayerManager = NULL;  //モデル生成ポインタ
	if (pPlayerManager == NULL)
	{//NULLの場合
		pPlayerManager = new CPlayerManager;		//動的確保
		if (pPlayerManager != NULL)
		{//NULLでない場合
			pPlayerManager->Init(PlayerFileName);	//初期化処理
		}
	}
	return pPlayerManager;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayerManager::Init(char *PlayerFileName)
{
	//残機数をプレイヤー分動的確保
	m_pStock = new int[m_nNumPlayer];

	//スコア情報をプレイヤー分動的確保
	m_pScoreInfo = new int[m_nNumPlayer];

	//バリア情報をプレイヤー分動的確保
	m_pBarrier = new bool[m_nNumPlayer];

	//モデル生成をプレイヤー分動的確保
	m_pModelCreate = new CModelCreate*[m_nNumPlayer];

	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{//プレイヤー分繰り返し
		m_pStock[nCntPlayer] = MAX_STOCK;				//残機数を代入
		m_pScoreInfo[nCntPlayer] = 0;					//スコア情報を初期化
		m_pBarrier[nCntPlayer] = false;					//バリア情報を初期化
		m_pModelCreate[nCntPlayer] = NULL;				//モデル生成をNULLにする
	}

	/*for (int nCntMotion = 0; nCntMotion < CPlayer::MOTION_MAX; nCntMotion++)
	{
		if (m_pMotion[nCntMotion] != NULL)
		{
			m_pMotion[nCntMotion] = NULL;
		}
	}*/
	m_apModel = NULL;	//モデルの初期化処理
	char *pStr = NULL;
	char aStr[256];

	for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
	{//プレイヤー分繰り返し
		CFileLoader *pFileLoader = NULL;	//フォルダ読み込みのポインタ
		if (pFileLoader == NULL)
		{//NULLの場合
		 //プレイヤー選択の処理
			pFileLoader = CFileLoader::Create(PlayerFileName);

			if (pFileLoader != NULL)
			{
				strcpy(aStr, pFileLoader->GetString(aStr));
				if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
				{
					LoadPlayer(aStr, pFileLoader, nCntPlayer);
				}

				//ファイル読み込みの終了処理
				if (pFileLoader != NULL)
				{
					pFileLoader->Uninit();
					delete pFileLoader;
					pFileLoader = NULL;
				}
			}
		}
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayerManager::Uninit(void)
{
	//残機の終了処理
	if (m_pStock != NULL)
	{
		delete m_pStock;
		m_pStock = NULL;
	}

	//残機数の終了処理
	if (m_pScoreInfo != NULL)
	{
		delete m_pScoreInfo;
		m_pScoreInfo = NULL;
	}

	//バリアの終了処理
	if (m_pBarrier != NULL)
	{
		delete m_pBarrier;
		m_pBarrier = NULL;
	}

	//プレイヤー選択番号の終了処理
	if (m_pSelectPlayer != NULL)
	{
		//delete m_pSelectPlayer;
		//m_pSelectPlayer = NULL;
	}

	//モデル生成の終了処理
	if (m_pModelCreate != NULL)
	{
		for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
		{
			if (m_pModelCreate[nCntPlayer] != NULL)
			{
				m_pModelCreate[nCntPlayer]->Uninit();
				delete m_pModelCreate[nCntPlayer];
				m_pModelCreate[nCntPlayer] = NULL;
			}
		}
		delete[] m_pModelCreate;
		m_pModelCreate = NULL;
	}

	/*if (m_pMotionManager != NULL)
	{
		m_pMotionManager->Uninit();
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}*/
	/*if (m_pMotion != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < CPlayer::MOTION_MAX; nCntMotion++)
		{
			if (m_pMotion[nCntMotion] != NULL)
			{
				m_pMotion[nCntMotion]->Uninit(m_nNumParts);
				delete m_pMotion[nCntMotion];
				m_pMotion[nCntMotion] = NULL;
			}
		}
	}*/
}

//=============================================================================
// 残機数の設置処理
//=============================================================================
void CPlayerManager::SetStock(int nStock, int nPlayerIdx)
{
	m_pStock[nPlayerIdx] = nStock;
}

//=============================================================================
// スコア情報の設置処理
//=============================================================================
void CPlayerManager::SetScoreInfo(int nScoreInfo, int nPlayerIdx)
{
	m_pScoreInfo[nPlayerIdx] = nScoreInfo;
}

//=============================================================================
// バリア情報の設置処理
//=============================================================================
void CPlayerManager::SetBarrier(int nPlayerIdx)
{
	m_pBarrier[nPlayerIdx];
}

//=============================================================================
// 選択番号の設置処理
//=============================================================================
void CPlayerManager::SetSelectPlayer(CPlayerManager::SELECT_PLAYER selectPlayer, int nPlayerIdx)
{
	m_pSelectPlayer[nPlayerIdx] = selectPlayer;
}

//=============================================================================
// 残機数の取得処理
//=============================================================================
int CPlayerManager::GetStock(int nPlayerIdx)
{
	return m_pStock[nPlayerIdx];
}


//=============================================================================
// スコア情報の取得処理
//=============================================================================
int CPlayerManager::GetScoreInfo(int nPlayerIdx)
{
	return m_pScoreInfo[nPlayerIdx];
}

//=============================================================================
// バリア情報の取得処理
//=============================================================================
bool CPlayerManager::GetBarrier(int nPlayerIdx)
{
	return m_pBarrier[nPlayerIdx];
}

//=============================================================================
// 選択番号の取得処理
//=============================================================================
CPlayerManager::SELECT_PLAYER CPlayerManager::GetSelectPlayer(int nPlayerIdx)
{
	return m_pSelectPlayer[nPlayerIdx];
}

//=============================================================================
// プレイヤー読み込み処理
//=============================================================================
HRESULT CPlayerManager::LoadPlayer(char *pStr, CFileLoader *pFileLoader, int nPlayerIdx)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	int				nNumModel = 0;						//モデルの総数
	int				nCntModel = 0;						//モデルのカウンター
	int				nCntPlayer = 0;						//プレイヤーのカウンター
	int				nNumParts = 0;						//パーツの総数
	int				*pParent = NULL;					//親モデルの番号
	CMotion			*pMotion[CPlayer::MOTION_MAX] = {};	//モーションのポインタ情報
	LPD3DXMESH		pMesh = NULL;						//メッシュのポインタ情報
	LPD3DXBUFFER	pBuffMat = NULL;					//マテリアルのポインタ情報
	DWORD			nNumMat = NULL;						//マテリアルの数

	int				nCntMotion = 0;						//モーションのカウンター

	while (1)
	{//ループ
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{//モデルの総数だったら
			nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			if (m_pModelCreate[nPlayerIdx] == NULL)
			{
				pParent = new int[nNumModel];									//親モデル総数分動的確保
				m_pModelCreate[nPlayerIdx] = CModelCreate::Create(nNumModel);	//モデル総数分生成
			}
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{//モデルファイル名だったら
			if (m_pModelCreate[nPlayerIdx] != NULL)
			{//モデル生成のポインタがNULLでない場合
				if (nCntModel < nNumModel)
				{//モデルカウンターがモデルの総数より小さい場合
					pStr = CFunctionLib::ReadString(pStr, pStr, MODEL_FILENAME);

					// Xファイルの読み込み
					D3DXLoadMeshFromX(pStr,
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&pBuffMat,
						NULL,
						&nNumMat,
						&pMesh);

					//モデル生成にモデル情報設置処理
					m_pModelCreate[nPlayerIdx]->SetMesh(pMesh, nCntModel);
					m_pModelCreate[nPlayerIdx]->SetMaterial(pDevice, pBuffMat, nNumMat, nCntModel);

					//モデル情報で使用した変数初期化
					pBuffMat = NULL;
					pMesh = NULL;
					nNumMat = 0;

					//モデルカウンターの加算
					nCntModel++;
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStr, CHARACTERSET) == 0)
		{//キャラクター情報の設置だったら
			m_apModel = ReadCharacterSet(pStr, pFileLoader, &nNumParts, pParent, nPlayerIdx);
		}
		else if (CFunctionLib::Memcmp(pStr, MOTIONSET) == 0)
		{//キャラクター情報の設置だったら
			if (nCntMotion < CPlayer::MOTION_MAX)
			{//カウンターがプレイヤーモーションより低い場合
				pMotion[nCntMotion] = ReadMotionSet(pStr, pFileLoader, &nNumParts, pMotion[nCntMotion], nCntMotion);
			}
			nCntMotion++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{//スクリプトの終了だったら
			break;
		}
	}

	//モーションマネージャーにモーションを設置
	if (m_pMotionManager == NULL)
	{//NULLの場合
	 //モーションマネージャーの生成
		m_pMotionManager = CMotionManager::Create(CPlayer::MOTION_MAX, nNumModel);
		if (m_pMotionManager != NULL)
		{//NULLではない場合
			for (int nCntMotion = 0; nCntMotion < CPlayer::MOTION_MAX; nCntMotion++)
			{//モーションの数だけ繰り返し
				if (pMotion[nCntMotion] != NULL)
				{
					//モーションの設置処理
					m_pMotionManager->CpyMotion(pMotion[nCntMotion], nCntMotion);
				}
			}
		}
	}

	//プレイヤー生成の処理
	//CreatePlayer(apModel, m_pModelCreate[nPlayerIdx], pMotionManager, nNumParts, pParent, nPlayerIdx);

	//親子の破棄
	if (pParent != NULL)
	{
		delete[] pParent;
		pParent = NULL;
	}

	//モーションの破棄
	if (pMotion != NULL)
	{
		for (int nCntMotion = 0; nCntMotion < CPlayer::MOTION_MAX; nCntMotion++)
		{
			if (pMotion[nCntMotion] != NULL)
			{
				pMotion[nCntMotion]->Uninit(nNumParts);
				delete pMotion[nCntMotion];
				pMotion[nCntMotion] = NULL;
			}
		}
	}
	return S_OK;
}

//=============================================================================
// キャラクターの読み込み処理
//=============================================================================
CModel ** CPlayerManager::ReadCharacterSet(char * pStr, CFileLoader * pFileLoader, int * pNumParts, int * pParent, int nPlayerIdx)
{
	CModel **apModel = NULL;	//モデルのポインタ
	int	nNumParts = 0;			//パーツの総数
	int nCntParts = 0;			//パーツのカウンター

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_PARTS) == 0)
		{//番号だったら
		 //nNumParts = CFunctionLib::ReadInt(pStr, NUM_PARTS);
			*pNumParts = CFunctionLib::ReadInt(pStr, NUM_PARTS);
			m_nNumParts = CFunctionLib::ReadInt(pStr, NUM_PARTS);
			if (apModel == NULL)
			{
				apModel = new CModel*[*pNumParts];
			}
		}
		else if (CFunctionLib::Memcmp(pStr, PARTSSET) == 0)
		{//パーツの設置だったら
			if (nCntParts < *pNumParts)
			{//パーツカウンターが総数より少ない場合
			 //パーツ読み込み処理
				apModel[nCntParts] = ReadParts(pStr, pFileLoader, pParent, apModel, m_pModelCreate[nPlayerIdx]);
				//パーツカウンター加算
				nCntParts++;
			}
		}
		else if (CFunctionLib::Memcmp(pStr, END_CHARACTERSET) == 0)
		{//キャラクター情報の終了だったら
			break;
		}

	}
	return apModel;
}

//=============================================================================
// パーツの読み込み処理
//=============================================================================
CModel * CPlayerManager::ReadParts(char * pStr, CFileLoader * pFileLoader, int * pParent, CModel **apModel, CModelCreate *pModelCreate)
{
	CModel *pModel = NULL;							//モデルのポインタ情報
	int nIdx = 0;									//番号
	D3DXVECTOR3 pos = INITIALIZE_D3DXVECTOR3;		//位置
	D3DXVECTOR3 rot = INITIALIZE_D3DXVECTOR3;		//向き

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, INDEX) == 0)
		{//番号だったら
			nIdx = CFunctionLib::ReadInt(pStr, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStr, PARENT) == 0)
		{//親子関係だったら
			pParent[nIdx] = CFunctionLib::ReadInt(pStr, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{//位置だったら
			pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{//向きだったら
			rot = CFunctionLib::ReadVector3(pStr, ROT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_PARTSSET) == 0)
		{//パーツの設置終了だったら
			break;
		}
	}

	if (pModel == NULL)
	{//モデルの生成
		pModel = CModel::Create(pos,
			rot,
			pModelCreate->GetMesh(nIdx),
			pModelCreate->GetBuffMat(nIdx),
			pModelCreate->GetNumMat(nIdx),
			pModelCreate->GetTexture(nIdx),
			pModelCreate->GetVtxMax(nIdx),
			pModelCreate->GetVtxMin(nIdx));
		if (pModel != NULL)
		{
			if (pParent[nIdx] != -1)
			{//親情報がある場合
				pModel->SetParent(apModel[pParent[nIdx]]);
			}
		}
	}
	return pModel;
}

//=============================================================================
// モーションの読み込み処理
//=============================================================================
CMotion * CPlayerManager::ReadMotionSet(char *pStr, CFileLoader *pFileLoader, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	bool bLoop = false;				//ループするかどうか
	int nNumKey = false;			//キー総数
	int nCntKeySet = 0;				//キー設置のカウンター
	int *pPlayBackKey = 0;			//再生フレーム数
	D3DXVECTOR3 **pAddPos = NULL;	//位置
	D3DXVECTOR3 **pDestRot = NULL;	//向き

	while (1)
	{//ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, LOOP) == 0)
		{//ループするかどうか
			bLoop = CFunctionLib::ReadBool(pStr, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_KEY) == 0)
		{//キーの総数だったら
			nNumKey = CFunctionLib::ReadInt(pStr, NUM_KEY);

			if (pPlayBackKey == NULL)
			{
				pPlayBackKey = new int[nNumKey];
			}

			if (pAddPos == NULL)
			{//NULLの場合
				pAddPos = new D3DXVECTOR3*[nNumKey];	//キーの総数分動的確保
				if (pAddPos != NULL)
				{//NULLではない場合
					for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
					{//キーの総数分繰り返し
						pAddPos[nCntKey] = new D3DXVECTOR3[*pNumParts];	//パーツ総数分動的確保
					}
				}
			}

			if (pDestRot == NULL)
			{//NULLの場合
				pDestRot = new D3DXVECTOR3*[nNumKey];	//キーの総数分動的確保
				if (pDestRot != NULL)
				{//NULLではない場合
					for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
					{//キーの総数分繰り返し
						pDestRot[nCntKey] = new D3DXVECTOR3[*pNumParts];	//パーツ総数分動的確保
					}
				}
			}

		}
		else if (CFunctionLib::Memcmp(pStr, KEYSET) == 0)
		{//キーの設置だったら
			ReadKeySet(pStr, pFileLoader, nCntKeySet, pPlayBackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_MOTIONSET) == 0)
		{//モーション情報の終了だったら
			break;
		}
	}

	if (pMotion == NULL)
	{// メモリが確保できる状態である
		pMotion = CMotion::Create(*pNumParts, nNumKey, 0.35f, bLoop, pPlayBackKey);
		if (pMotion != NULL)
		{// メモリが確保できた
			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// キーフレームの総数分繰り返し
				for (int nCntParts = 0; nCntParts < *pNumParts; nCntParts++)
				{// キーフレームの総数分繰り返し
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
				}
			}
		}
	}

	//再生フレームの破棄
	if (pPlayBackKey != NULL)
	{
		delete pPlayBackKey;
		pPlayBackKey = NULL;
	}

	//位置の破棄
	if (pAddPos != NULL)
	{
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{
			delete pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	//向きの破棄
	if (pDestRot != NULL)
	{
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{
			delete pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return pMotion;
}

//=============================================================================
// キー設置の処理
//=============================================================================
void CPlayerManager::ReadKeySet(char *pStr, CFileLoader *pFileLoader, int nCntKeySet, int *pPlayBackKey, D3DXVECTOR3 *pAddPos, D3DXVECTOR3 *pDestRot)
{
	int nCntKey = 0;

	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, FRAME) == 0)
		{//フレーム数だったら
			pPlayBackKey[nCntKeySet] = CFunctionLib::ReadInt(pStr, FRAME);
		}
		else if (CFunctionLib::Memcmp(pStr, KEY) == 0)
		{//キーの情報だったら
			ReadKey(pStr, pFileLoader, nCntKey, pAddPos, pDestRot);
			nCntKey++;
		}
		else if (CFunctionLib::Memcmp(pStr, END_KEYSET) == 0)
		{//キーの設置終了だったら
			break;
		}
	}
}

//=============================================================================
// キー情報の処理
//=============================================================================
void CPlayerManager::ReadKey(char *pStr, CFileLoader *pFileLoader, int nCntKey, D3DXVECTOR3 *pAddPos, D3DXVECTOR3 *pDestRot)
{
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));

		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{//位置だったら
			pAddPos[nCntKey] = CFunctionLib::ReadVector3(pStr, POS);
			D3DXVECTOR3 addPos = pAddPos[nCntKey];
		}
		else if (CFunctionLib::Memcmp(pStr, ROT) == 0)
		{//向きだったら
			pDestRot[nCntKey] = CFunctionLib::ReadVector3(pStr, ROT);
			D3DXVECTOR3 addRot = pAddPos[nCntKey];
		}
		else if (CFunctionLib::Memcmp(pStr, END_KEY) == 0)
		{//キーの終了だったら
			break;
		}
	}
}

//=============================================================================
// プレイヤー生成処理
//=============================================================================
void CPlayerManager::CreatePlayer(CModel **apModel, CModelCreate *pModelCreate, CMotionManager *pMotionManager, int nNumParts, int *pParent, int nPlayer)
{
	CModel **apModelCopy = NULL;					//モデル複製のポインタ情報
	CMotionManager *pMotionManagerCopy = NULL;		//モーションマネージャー複製のポインタ情報
	CPlayer *pPlayer = NULL;						//プレイヤー複製のポインタ情報
	CMotion *pMotionCopy[CPlayer::MOTION_MAX] = {};	//モーション複製のポインタ情報

	if (apModelCopy == NULL)
	{
		apModelCopy = new CModel*[nNumParts];		//パーツ分動的確保
		if (apModelCopy != NULL)
		{
			for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
			{//パーツの総数分繰り返し
				apModelCopy[nCntParts] = NULL;
				if (apModelCopy[nCntParts] == NULL)
				{
					//モデル生成の処理
					apModelCopy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(),
						apModel[nCntParts]->GetRot(),
						pModelCreate->GetMesh(nCntParts),
						pModelCreate->GetBuffMat(nCntParts),
						pModelCreate->GetNumMat(nCntParts),
						pModelCreate->GetTexture(nCntParts),
						pModelCreate->GetVtxMax(nCntParts),
						pModelCreate->GetVtxMin(nCntParts));

					if (apModelCopy[nCntParts] != NULL)
					{
						if (pParent[nCntParts] != -1)
						{//親情報がある場合
							apModelCopy[nCntParts]->SetParent(apModelCopy[pParent[nCntParts]]);
						}
					}
				}
			}
		}
	}

	if (pMotionManagerCopy == NULL)
	{
		//モーションマネージャーの生成
		pMotionManagerCopy = CMotionManager::Create(CPlayer::MOTION_MAX, nNumParts);
		if (pMotionManagerCopy != NULL)
		{
			for (int nCntMotion = 0; nCntMotion < CPlayer::MOTION_MAX; nCntMotion++)
			{//モーションの生成
				pMotionCopy[nCntMotion] = CMotion::Create(nNumParts,
					pMotionManager->GetMotion()[nCntMotion]->GetNumKey(),
					pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(),
					pMotionManager->GetMotion()[nCntMotion]->GetLoop(),
					pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());

				if (pMotionCopy != NULL)
				{
					for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
					{
						for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
						{
							pMotionCopy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
							pMotionCopy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
						}
					}
					//pMotionManagerCopy->SetMotion(pMotionCopy[nCntMotion], nCntMotion);
					pMotionManagerCopy->CpyMotion(pMotionCopy[nCntMotion], nCntMotion);
				}
			}
		}
	}
	m_apModel = apModelCopy;				//モデルポインタの代入
	m_pMotionManager = pMotionManagerCopy;	//モーションマネージャーの代入
	m_nNumParts = nNumParts;				//パーツの代入
}

//=============================================================================
// モデル情報の取得処理
//=============================================================================
CModel **CPlayerManager::GetModel(void)
{
	return m_apModel;
}
//=============================================================================
// モーションマネージャーの取得処理
//=============================================================================
CMotionManager *CPlayerManager::GetMotionManager(void)
{
	return m_pMotionManager;
}

//=============================================================================
// パーツ数の取得処理
//=============================================================================
int CPlayerManager::GetParts(void)
{
	return m_nNumParts;
}

