//=============================================================================
//
// オブジェクト処理 [scene.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "scene.h"
#include "debugproc.h"
//=============================================================================
//	静的メンバ変数宣言
//=============================================================================
int CScene::m_nNumAll = 0;								//オブジェクトの総数
int CScene::m_nNumPriority[NUM_PRIORITY] = {};			//優先順位別の総数
CScene *CScene::m_apTop[NUM_PRIORITY] = {};				//先頭のオブジェクトへのポインタ
CScene *CScene::m_apCur[NUM_PRIORITY] = {};				//現在（最後尾）のオブジェクトへのポインタ

//=============================================================================
// オブジェクトのコンストラクタ
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE objtype)
{
	m_bDeath = false;							//死んでいない状態にする
	m_nPriority = nPriority;					//優先順位の設定

	if (m_apTop[m_nPriority] == NULL)
	{//まだ先頭にオブジェクトがない場合
		m_apTop[m_nPriority] = this;			//先頭に代入
		m_apTop[m_nPriority]->m_pNext = NULL;
		m_apTop[m_nPriority]->m_pPrev = NULL;
	}
	if (m_apCur[m_nPriority] != NULL)
	{//まだ現在にオブジェクトがない場合
		m_apCur[m_nPriority]->m_pNext = this;			//現在の次に今のアドレスを代入
	}
	this->m_pPrev = m_apCur[m_nPriority];				//今の前のアドレスに現在を代入
	this->m_pNext = NULL;								//今の次のアドレスをNULLにする
	m_apCur[m_nPriority] = this;						//現在に今のアドレスを代入
	m_objType = objtype;
	m_nNumPriority[m_nPriority]++;						//指定された優先順位を加算
	m_nNumAll++;										//総数の初期値
}

//=============================================================================
// オブジェクトのデストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// 全てのオブジェクトの開放処理
//=============================================================================
void CScene::ReleaseAll(void)
{
	CScene *pSceneTop;				//今実行する変数
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pSceneTop = m_apTop[nCntPriority];						//今の実行変数に初期位置を代入

		while (pSceneTop != NULL)
		{//NULLになるまで繰り返し
			CScene *pSceneNext;									//次の変数
			pSceneNext = pSceneTop->m_pNext;					//次の変数に今の次のアドレスを代入
			if (pSceneTop->m_objType != OBJTYPE_FADE)
			{
				pSceneTop->Uninit();							//終了処理
			}
			pSceneTop = pSceneNext;								//次に実行するものを今の変数に代入する
		}

		pSceneTop = m_apTop[nCntPriority];						//次の実行変数に初期位置を代入
		while (pSceneTop != NULL)
		{//NULLになるまで繰り返し
			CScene *pSceneNext;									//次の変数
			pSceneNext = pSceneTop->m_pNext;					//次の変数に今の次のアドレスを代入
			if (pSceneTop->m_bDeath == true)
			{//死亡フラグが立っていたら消去処理へ
				pSceneTop->Death();
			}
			pSceneTop = pSceneNext;								//次に実行するものを今の変数に代入する
		}
		m_apTop[nCntPriority] = NULL;
		m_apCur[nCntPriority] = NULL;
	}
	m_nNumAll = 0;		//敵の総数を初期化
}

//=============================================================================
// 全てのオブジェクトの更新処理
//=============================================================================
void CScene::UpdateAll(void)
{
	CScene *pSceneTop;				//今実行する変数
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pSceneTop = m_apTop[nCntPriority];			//今の実行変数に初期位置を代入

		while (pSceneTop != NULL)
		{//NULLになるまで繰り返し
			CScene *pSceneNext;					//次の変数
			pSceneNext = pSceneTop->m_pNext;	//次の変数に今の次のアドレスを代入
			pSceneTop->Update();				//終了処理
			pSceneTop = pSceneNext;				//次に実行するものを今の変数に代入する
		}
	}

	//開放チェック
	DeathCheck();
}

//=============================================================================
// 全てのオブジェクトの描画処理
//=============================================================================
void CScene::DrawAll(void)
{
	CScene *pSceneTop;						//今実行する変数
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pSceneTop = m_apTop[nCntPriority];					//今の実行変数に初期位置を代入

		while (pSceneTop != NULL)
		{//NULLになるまで繰り返し
			CScene *pSceneNext;					//次の変数
			pSceneNext = pSceneTop->m_pNext;	//次の変数に今の次のアドレスを代入
			pSceneTop->Draw();					//終了処理
			pSceneTop = pSceneNext;				//次に実行するものを今の変数に代入する
		}
	}

}

//=============================================================================
// 全てのオブジェクトの開放チェック処理
//=============================================================================
void CScene::DeathCheck(void)
{
	CScene *pSceneTop;				//今実行する変数
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pSceneTop = m_apTop[nCntPriority];						//次の実行変数に初期位置を代入
		while (pSceneTop != NULL)
		{//NULLになるまで繰り返し
			CScene *pSceneNext;					//次の変数
			pSceneNext = pSceneTop->m_pNext;	//次の変数に今の次のアドレスを代入
			if (pSceneTop->m_bDeath == true)
			{//死亡フラグが立っていたら消去処理へ
				pSceneTop->Death();
			}
			pSceneTop = pSceneNext;				//次に実行するものを今の変数に代入する
		}
	}
}

//=============================================================================
// 指定された優先順位のオブジェクトの開放チェック処理
//=============================================================================
void CScene::DeathCheck(int nPriority)
{
	CScene *pSceneTop;				//今実行する変数
	CScene *pSceneNext;					//次の変数
	pSceneTop = m_apTop[nPriority];						//次の実行変数に初期位置を代入
	while (pSceneTop != NULL)
	{//NULLになるまで繰り返し
		pSceneNext = pSceneTop->m_pNext;	//次の変数に今の次のアドレスを代入
		if (pSceneTop->m_bDeath == true)
		{//死亡フラグが立っていたら消去処理へ
			pSceneTop->Death();
		}
		pSceneTop = pSceneNext;				//次に実行するものを今の変数に代入する
	}
}

//=============================================================================
// オブジェクトの先頭取得処理
//=============================================================================
CScene * CScene::GetTop(int nPriority)
{
	return m_apTop[nPriority];
}

//=============================================================================
// オブジェクトの破棄
//=============================================================================
void CScene::Release(void)
{
	//フラグを立てる
	m_bDeath = true;
}

//=============================================================================
// オブジェクトの死亡処理
//=============================================================================
void CScene::Death(void)
{
	if (m_pPrev != NULL)
	{//自分の前の次のアドレスに自分の次のアドレスを代入
		m_pPrev->m_pNext = m_pNext;
	}
	if (m_pNext != NULL)
	{//自分の次の前のアドレスに自分の前のアドレスを代入
		m_pNext->m_pPrev = m_pPrev;
	}
	if (this == m_apTop[m_nPriority])
	{//先頭と同じアドレスの場合先頭の次のアドレスに先頭を移動
		m_apTop[m_nPriority] = m_apTop[m_nPriority]->m_pNext;
	}
	if (this == m_apCur[m_nPriority])
	{//現在と同じアドレスの場合現在の次のアドレスに現在を移動
		m_apCur[m_nPriority] = m_apCur[m_nPriority]->m_pPrev;
	}

	m_nNumPriority[m_nPriority]--;		//指定された優先順位の減算
	delete this;						//今のアドレスを開放
	m_nNumAll--;						//総数の減算
}

//=============================================================================
// 種類の設置
//=============================================================================
void CScene::SetObjType(OBJTYPE objType)
{
	m_objType = objType;
}

//=============================================================================
// 種類の取得
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}

//=============================================================================
// 次のオブジェクトのアドレス取得処理
//=============================================================================
CScene * CScene::GetNext(void)
{
	return m_pNext;
}

//=============================================================================
// 前のオブジェクトのアドレス取得処理
//=============================================================================
CScene * CScene::GetPrev(void)
{
	return m_pPrev;
}

//=============================================================================
// 処理の優先順位取得処理
//=============================================================================
int CScene::GetPriority(void)
{
	return m_nPriority;
}