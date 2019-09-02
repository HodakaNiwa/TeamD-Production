//*****************************************************************************
//
//     サーバー用関数の処理[serverlib.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "serverlib.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define STRING_TAB       "\t"                     // Tabの文字列
#define STRING_ENTER     "\n"                     // 改行コード
#define STRING_SPACE     " "                      // 空白の文字列
#define STRING_COMMENT   "#"                      // コメントが開始される文字

//*****************************************************************************
//    静的メンバ変数
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CServerLib::CServerLib()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CServerLib::~CServerLib()
{

}

//=============================================================================
//    指定の文字列が同じ文字列であるか比較する処理
//=============================================================================
int CServerLib::Memcmp(char *pSrc, char *pComp)
{
	return memcmp(pSrc, pComp, strlen(pComp));  // 0なら同じ文字列であると判定
}

//=============================================================================
//    tabやスペースを引っこ抜いて文字列を先頭に持っていく処理
//=============================================================================
char *CServerLib::GetLineTop(char *pSrc)
{
	while (1)
	{// 有効な文字が出てくるまで繰り返し
		if (*pSrc == *STRING_SPACE)
		{// 先頭の文字が空白行だったら
			pSrc++;
		}
		else if (*pSrc == *STRING_TAB)
		{// 先頭の文字がTabだったら
			pSrc += strlen(STRING_TAB);
		}
		else
		{// 違う文字列が出てきた
			break;
		}
	}

	return pSrc;
}

//=============================================================================
//    読み取った数字や文字をどれくらい削るのかを返す処理
//=============================================================================
int CServerLib::PopString(char *pSrc, char *pDest)
{
	int nCntWord = 0;   // 読み取りに邪魔な文字数
	while (1)
	{// 文字列が先頭に来るように返す
		nCntWord++;   // 文字数を加算する
		pSrc++;
		if (*pSrc == *STRING_SPACE || *pSrc == *STRING_TAB || *pSrc == *STRING_ENTER)
		{// 空白かタブが先頭にきた
			nCntWord++;                      // SPACEの分を考慮して文字数加算
			break;
		}
	}
	return nCntWord;
}

//=============================================================================
//    先頭の文字を消して頭出しをする処理
//=============================================================================
char *CServerLib::HeadPutout(char *pDest, char *pHead)
{
	pDest = GetLineTop(pDest);  // 無効な文字を消す
	pDest += strlen(pHead);     // データ抜き取りに不要な文字列を消す
	return pDest;
}

//=============================================================================
//    文字列からint型のデータを返す処理
//=============================================================================
int CServerLib::ReadInt(char *pSrc, char *pHead)
{
	int nData = 0;
	pSrc = HeadPutout(pSrc, pHead);
	nData = atoi(pSrc);
	return nData;
}

//=============================================================================
//    文字列からfloat型のデータを返す処理
//=============================================================================
float CServerLib::ReadFloat(char *pSrc, char *pHead)
{
	float fData = 0;
	pSrc = HeadPutout(pSrc, pHead);
	fData = (float)atof(pSrc);
	return fData;
}

//=============================================================================
//    文字列からfloat3型のデータを返す処理
//=============================================================================
char *CServerLib::ReadFloat3(char *pSrc, char *pHead, float *pData)
{
	int nWord = 0;
	float fData = 0;
	pSrc = HeadPutout(pSrc, pHead);
	fData = (float)atof(pSrc);
	pData[0] = fData;
	nWord = CServerLib::PopString(pSrc, pSrc);
	pSrc += nWord;

	fData = (float)atof(pSrc);
	pData[1] = fData;
	nWord = CServerLib::PopString(pSrc, pSrc);
	pSrc += nWord;

	fData = (float)atof(pSrc);
	pData[2] = fData;
	nWord = CServerLib::PopString(pSrc, pSrc);
	pSrc += nWord;

	return pSrc;
}

//=============================================================================
//    文字列からbool型のデータを返す処理
//=============================================================================
bool CServerLib::ReadBool(char *pSrc, char *pHead)
{
	bool bBool = false;
	int nBool = ReadInt(pSrc, pHead);
	if (nBool == 1)
	{// ループする番号だった
		bBool = true;
	}
	else
	{// ループしない番号だった
		bBool = false;
	}

	return bBool;
}

//=============================================================================
//    文字列からchar型のデータを返す処理
//=============================================================================
char *CServerLib::ReadString(char *pSrc, char *pDest, char *pHead)
{
	int nWord = 0;
	// 文字列を抜き出す
	pSrc = HeadPutout(pSrc, pHead);
	nWord = PopString(pSrc, pDest);
	strcpy(pDest, "\0");
	strcpy(pDest, pSrc);
	strcpy(&pDest[nWord - 1], "\0");

	return pDest;
}

//=============================================================================
//    int型のデータからchar型のデータを返す処理
//=============================================================================
char *CServerLib::WriteInt(int nData, char *pDest)
{
	sprintf(pDest, "%d", nData);
	return pDest;
}

//=============================================================================
//    float型のデータからchar型のデータを返す処理
//=============================================================================
char *CServerLib::WriteFloat(float fData, char *pDest)
{
	sprintf(pDest, "%.1f", fData);
	return pDest;
}


//=============================================================================
//    float3型のデータからchar型のデータを返す処理
//=============================================================================
char *CServerLib::WriteFloat3(float *pData, char *pDest)
{
	sprintf(pDest, "%.1f %.1f %.1f", pData[0], pData[1], pData[2]);
	return pDest;
}