//*****************************************************************************
//
// マップの処理[map.cpp]
// Auther:Jukiya Hayakawa
//
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "map.h"
#include "object.h"
#include "meshField.h"
#include "loadfilefunction.h"
#include "modelcreate.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_MAPSET				"MAPSET"
#define TEXT_END_MAPSET			"END_MAPSET"
#define TEXT_FILENAME_OBJECT	"FILENAME_OBJECT = "
#define TEXT_FILENAME_FIELD		"FILENAME_FIELD = "

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
CModelCreate *CMap::m_pModelCreate = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CMap::CMap()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CMap *CMap::Create(void)
{
	CMap *pMap = NULL;  // マップポインタ
	if (pMap == NULL)
	{//NULLの場合
		pMap = new CMap;	//動的確保
		if (pMap != NULL)
		{// NULLでない場合
			pMap->Init();	//初期化処理
		}
	}
	return pMap;
}

//=============================================================================
//    初期化処理
//=============================================================================
void CMap::Init(void)
{
	//レンダリングの取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = pRenderer->GetDevice();

	// マップ情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{
		pFile = fopen(TEXT_FILENAME_MAP, "r");
		if (pFile != NULL)
		{
			char *pStrCur;						//文字列の先頭へのポインタ
			char aLine[256];					//文字列の読み込み用
			char aStr[256];						//文字列抜き出し用
			int	 nNumModel = 0;				//モデル総数
			int  nCntModel = 0;					//モデルカウンター
			LPD3DXMESH pMesh = NULL;            //メッシュのポインタ
			LPD3DXBUFFER pBuffMat = NULL;       //マテリアルのポインタ
			DWORD nNumMat = 0;                  //マテリアルの数
			char aFileName[256] = "\0";			//モデルのファイル名

			while (1)
			{
				pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
				int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
				strcpy(&aStr[0], pStrCur);
				aStr[nNum - 1] = '\0';
				if (memcmp(&aStr[0], TEXT_SCRIPT, strlen(TEXT_SCRIPT)) == 0)
				{
					while (1)
					{//モデルファイルを読み込むまで繰り返し
						pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
						//int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
						strcpy(&aStr[0], pStrCur);	//文字列を取り出す
						//aStr[nNum - 1] = '\0';

						if (memcmp(&aStr[0], TEXT_NUM_MODEL, strlen(TEXT_NUM_MODEL)) == 0)
						{
							pStrCur += strlen(TEXT_NUM_MODEL);	//頭出し
							strcpy(&aStr[0], pStrCur);

							nNumModel = atoi(&aStr[0]);	//モデルの総数に代入
							m_pModelCreate = CModelCreate::Create(nNumModel);
						}
						else if (memcmp(&aStr[0], TEXT_FILENAME_MODEL, strlen(TEXT_FILENAME_MODEL)) == 0)
						{// モデルのファイルパス名情報があった
						 // モデルのファイルパス名を読み取る
							pStrCur += strlen(TEXT_FILENAME_MODEL);					//頭出し
							strcpy(&aStr[0], pStrCur);
							int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
							strcpy(&aStr[0], pStrCur);
							aStr[nNum - 1] = '\0';

							D3DXLoadMeshFromX(&aStr[0],
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&pBuffMat,
								NULL,
								&nNumMat,
								&pMesh);

							//モデル生成のメッシュ設置
							m_pModelCreate->SetMesh(pMesh, nCntModel);
							//モデル生成のマテリアル設置
							m_pModelCreate->SetBuffMat(pBuffMat, nCntModel);
							//モデル生成のマテリアル総数設置
							m_pModelCreate->SetNumMat(nNumMat, nCntModel);

							//変数初期化
							pMesh = NULL;
							pBuffMat = NULL;
							nNumMat = 0;

							nCntModel++;  //加算
						}
						else if (memcmp(&aStr[0], TEXT_MAPSET, strlen(TEXT_MAPSET)) == 0)
						{//マップの設置を示すテキストの場合
							while (1)
							{
								pStrCur = CLoadFileFunction::ReadLine(pFile, &aLine[0]);
								//int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
								strcpy(&aStr[0], pStrCur);	//文字列を取り出す

								if (memcmp(&aStr[0], TEXT_FILENAME_FIELD, strlen(TEXT_FILENAME_FIELD)) == 0)
								{//フィールドファイル名を示すテキストの場合
								}
								if (memcmp(&aStr[0], TEXT_FILENAME_OBJECT, strlen(TEXT_FILENAME_OBJECT)) == 0)
								{//オブジェクトファイル名を示すテキストの場合
									pStrCur += strlen(TEXT_FILENAME_OBJECT);					//頭出し
									strcpy(&aStr[0], pStrCur);
									int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
									strcpy(&aStr[0], pStrCur);
									aStr[nNum - 1] = '\0';

									LoadObject(&aStr[0], pStrCur, &aLine[0]);
								}
								if (memcmp(&aStr[0], TEXT_END_MAPSET, strlen(TEXT_END_MAPSET)) == 0)
								{//マップの設置終了を示すテキストの場合
									break;
								}
							}
							break;
						}
					}
				}
				else if (memcmp(&aStr[0], TEXT_END_SCRIPT, strlen(TEXT_END_SCRIPT)) == 0)
				{
					break;
				}
			}
			fclose(pFile);	//ファイルを閉じる
		}
	}
}

//=============================================================================
//    終了処理
//=============================================================================
void CMap::Uninit(void)
{
	if (m_pModelCreate != NULL)
	{
		m_pModelCreate->Uninit();
		delete m_pModelCreate;
		m_pModelCreate = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CMap::Update(void)
{
}

//=============================================================================
// 配置物読み込み処理
//=============================================================================
void CMap::LoadObject(char *pObjectFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{//NULLの場合
		pFile = fopen(pObjectFileName, "r");
		if (pFile != NULL)
		{//NULLでない場合
			char aStr[256];													// 文字列抜き出し用
			CObject *pObject = NULL;                                        // 配置物クラスへのポインタ
			int nObjectType = 0;                                            // 配置物のモデル番号
			D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の座標
			D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の向き
			D3DXVECTOR3 ObjectSize = D3DXVECTOR3(1.0f, 1.0f, 1.0f);			// 配置物の大きさ
			bool bObjectHit = true;											// 配置物の当たる判定
			bool bObjectDestroy = false;									// 配置物が壊せるかどうか

			pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);
			int nNum = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
			strcpy(&aStr[0], pStrCur);
			aStr[nNum - 1] = '\0';

			if (memcmp(&aStr[0], TEXT_SCRIPT, strlen(TEXT_SCRIPT)) == 0)
			{
				while (1)
				{
					pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);	// 有効な文字列を見つける
					strcpy(&aStr[0], pStrCur);								//文字列を取り出す

					if (memcmp(&aStr[0], TEXT_MODELSET, strlen(TEXT_MODELSET)) == 0)
					{//
						while (1)
						{
							pStrCur = CLoadFileFunction::ReadLine(pFile, pLine);	// 有効な文字列を見つける
							strcpy(&aStr[0], pStrCur);								//文字列を取り出す

							if (memcmp(&aStr[0], TEXT_TYPE, strlen(TEXT_TYPE)) == 0)
							{// 種類を示すテキストの場合
								pStrCur += strlen(TEXT_TYPE);
								pStrCur = CLoadFileFunction::GetLineTop(pStrCur);
								strcpy(&aStr[0], pStrCur);							//文字列を取り出す
								nObjectType = atoi(&aStr[0]);
							}
							else if (memcmp(pStrCur, TEXT_POS, strlen(TEXT_POS)) == 0)
							{// 位置を示すテキストの場合
								int nWord = 0;
								pStrCur += strlen(TEXT_POS);
								strcpy(&aStr[0], pStrCur);			//文字列を取り出す
								ObjectPos.x = (float)atof(&aStr[0]);

								nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
								pStrCur += nWord;
								strcpy(&aStr[0], pStrCur);			//文字列を取り出す
								ObjectPos.y = (float)atof(&aStr[0]);

								nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
								pStrCur += nWord;
								strcpy(&aStr[0], pStrCur);			//文字列を取り出す
								ObjectPos.z = (float)atof(&aStr[0]);

							}
							else if (memcmp(pStrCur, TEXT_ROT, strlen(TEXT_ROT)) == 0)
							{// 向きを示すテキストの場合
								int nWord = 0;
								pStrCur += strlen(TEXT_ROT);
								strcpy(&aStr[0], pStrCur);			//文字列を取り出す
								ObjectRot.x = (float)atof(&aStr[0]);

								nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
								pStrCur += nWord;
								strcpy(&aStr[0], pStrCur);			//文字列を取り出す
								ObjectRot.y = (float)atof(&aStr[0]);

								nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
								pStrCur += nWord;
								strcpy(&aStr[0], pStrCur);			//文字列を取り出す
								ObjectRot.z = (float)atof(&aStr[0]);
							}
							else if (memcmp(pStrCur, TEXT_SIZE, strlen(TEXT_SIZE)) == 0)
							{// 大きさを示すテキストの場合
								int nWord = 0;
								pStrCur += strlen(TEXT_SIZE);
								strcpy(&aStr[0], pStrCur);			//文字列を取り出す
								ObjectSize.x = (float)atof(&aStr[0]);

								nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
								pStrCur += nWord;
								strcpy(&aStr[0], pStrCur);			//文字列を取り出す
								ObjectSize.y = (float)atof(&aStr[0]);

								nWord = CLoadFileFunction::PopString(pStrCur, &aStr[0]);
								pStrCur += nWord;
								strcpy(&aStr[0], pStrCur);			//文字列を取り出す
								ObjectSize.z = (float)atof(&aStr[0]);
							}
							else if (memcmp(&aStr[0], TEXT_HIT, strlen(TEXT_HIT)) == 0)
							{// 種類を示すテキストの場合
								pStrCur += strlen(TEXT_HIT);
								pStrCur = CLoadFileFunction::GetLineTop(pStrCur);
								strcpy(&aStr[0], pStrCur);							//文字列を取り出す
								int nHit = atoi(&aStr[0]);							//数字格納
								switch (nHit)
								{
								case 0:	//0の場合当たり判定なし
									bObjectHit = false;
									break;
								case 1:	//1の場合当たり判定あり
									bObjectHit = true;
									break;
								}
							}
							else if (memcmp(&aStr[0], TEXT_DESTROY, strlen(TEXT_DESTROY)) == 0)
							{// 種類を示すテキストの場合
								pStrCur += strlen(TEXT_DESTROY);
								pStrCur = CLoadFileFunction::GetLineTop(pStrCur);
								strcpy(&aStr[0], pStrCur);							//文字列を取り出す
								int nDestoroy = atoi(&aStr[0]);							//数字格納
								switch (nDestoroy)
								{
								case 0:	//0の場合当たり判定なし
									bObjectDestroy = false;
									break;
								case 1:	//1の場合当たり判定あり
									bObjectDestroy = true;
									break;
								}
							}
							else if (memcmp(pStrCur, TEXT_END_MODELSET, strlen(TEXT_END_MODELSET)) == 0)
							{// 配置読み込み終了を示すテキストの場合
								pObject = CObject::Create(ObjectPos, ObjectRot,ObjectSize,bObjectHit,bObjectHit);
								if (pObject != NULL)
								{
									pObject->BindX(m_pModelCreate->GetMesh(nObjectType), m_pModelCreate->GetBuffMat(nObjectType), m_pModelCreate->GetNumMat(nObjectType));
									pObject->SetVtxMin(m_pModelCreate->GetVtxMin(nObjectType));
									pObject->SetVtxMax(m_pModelCreate->GetVtxMax(nObjectType));
									pObject->SetType(nObjectType);
								}
								break;  // ループ終了
							}

						}
					}
					else if (memcmp(pStrCur, TEXT_END_SCRIPT, strlen(TEXT_END_SCRIPT)) == 0)
					{
						break;  // ループ終了
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
// 配置物生成の情報取得処理
//=============================================================================
CModelCreate *CMap::GetCModelCreate(void)
{
	return m_pModelCreate;
}

//=============================================================================
// オブジェクトを保存処理
//=============================================================================
void CMap::SaveObject(void)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(TEXT_SAVE_FILENAME_OBJECT, "w");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // コメント部分を書き込み
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#		オブジェクトの配置情報\n");
			fprintf(pFile, "#       Auther : Jukiya Hayakawa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s	#この行は絶対に消さないこと！\n\n", TEXT_SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  オブジェクト配置情報\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// オブジェクト情報を保存
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CObject *pObject = NULL;
			int nHit = 0;			//当たり判定
			int nDestroy = 0;		//破壊できるかどうか

			for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
			{// 優先順位の数だけ繰り返し
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{//
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
					{// オブジェクトの種類だった場合
						pObject = (CObject*)pScene;
						if (pObject != NULL)
						{// オブジェクトへのポインタが取得できた
							// 配置物の情報を書き込み
							fprintf(pFile, "%s\n", TEXT_MODELSET);
							fprintf(pFile, "	%s%d\t\t\t\t#モデルの種類\n", TEXT_TYPE, pObject->GetType());
							fprintf(pFile, "	%s%.1f %.1f %.1f\t\t#モデルの位置\n", TEXT_POS, pObject->GetPos().x, pObject->GetPos().y, pObject->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f\t\t#モデルの向き\n", TEXT_ROT, pObject->GetRot().x, pObject->GetRot().y, pObject->GetRot().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f\t\t#モデルの大きさ\n", TEXT_SIZE, pObject->GetSize().x, pObject->GetSize().y, pObject->GetSize().z);

							//当たり判定別の処理
							switch (pObject->GetHit())
							{
							case false:	//falseの場合当たり判定なし
								nHit = 0;
								break;

							case true:	//trueの場合当たり判定あり
								nHit = 1;
								break;
							}

							//破壊できるかどうか別の処理
							switch (pObject->GetDestroy())
							{
							case false:	//falseの場合破壊できない
								nDestroy = 0;
								break;

							case true:	//trueの場合破壊できる
								nDestroy = 1;
								break;
							}

							fprintf(pFile, "	%s%d\t\t\t\t\t#モデルの当たり判定\n", TEXT_HIT, nHit);
							fprintf(pFile, "	%s%d\t\t\t\t#モデルの破壊できるかどうか\n", TEXT_DESTROY, nDestroy);
							fprintf(pFile, "%s\n", TEXT_END_MODELSET);
							fprintf(pFile, "\n");
						}
					}
					pScene = pSceneNext;
				}
			}
			// コメント部分を書き込み
			fprintf(pFile, "%s	#この行は絶対に消さないこと！\n", TEXT_END_SCRIPT);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}
