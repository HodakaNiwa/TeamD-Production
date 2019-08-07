//*****************************************************************************
//
//     共通で使う関数の処理[functionlib.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "functionlib.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CFunctionLib::CFunctionLib()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CFunctionLib::~CFunctionLib()
{

}

//=============================================================================
//    テキストから一列分文字列を読み進める処理
//=============================================================================
char *CFunctionLib::ReadLine(FILE *pFile, char *pDest)
{
	// 文字列を読み進める
	fgets(pDest, 256, pFile);
	while (1)
	{// 有効な行が見つかるまでループ
		if (*pDest == *STRING_COMMENT)
		{// 先頭の文字が有効な文字列ではない
			fgets(pDest, 256, pFile);
		}
		else if (*pDest == *STRING_ENTER)
		{// 先頭の文字が改行コードだったら
			fgets(pDest, 256, pFile);
		}
		else if (*pDest == *STRING_SPACE)
		{// 先頭の文字が空白行だったら
			pDest = GetLineTop(pDest);
		}
		else if (*pDest == *STRING_TAB)
		{// 先頭の文字がTabだったら
			pDest = GetLineTop(pDest);
		}
		else
		{// 有効な文字列が発見された
			break;
		}
	}

	return pDest;  // 文字列のアドレスを返す
}

//=============================================================================
//    指定の文字列が同じ文字列であるか比較する処理
//=============================================================================
int CFunctionLib::Memcmp(char *pSrc, char *pComp)
{
	return memcmp(pSrc, pComp, strlen(pComp));  // 0なら同じ文字列であると判定
}

//=============================================================================
//    tabやスペースを引っこ抜いて文字列を先頭に持っていく処理
//=============================================================================
char *CFunctionLib::GetLineTop(char *pSrc)
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
int CFunctionLib::PopString(char *pSrc, char *pDest)
{
	int nCntWord = 0;   // 読み取りに邪魔な文字数
	char aStr[256];     // 文字列保存用

	strcpy(aStr, pSrc); // 文字列を保存しておく
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
char *CFunctionLib::HeadPutout(char *pDest, char *pHead)
{
	pDest = GetLineTop(pDest);  // 無効な文字を消す
	pDest += strlen(pHead);     // データ抜き取りに不要な文字列を消す
	return pDest;
}

//=============================================================================
//    文字列を整数型に変換して返す処理
//=============================================================================
int CFunctionLib::ReadInt(char *pSrc, char *pHead)
{
	int nData = 0;
	pSrc = HeadPutout(pSrc, pHead);
	nData = atoi(pSrc);
	return nData;
}

//=============================================================================
//    文字列を浮動小数点型に変換して返す処理
//=============================================================================
float CFunctionLib::ReadFloat(char *pSrc, char *pHead)
{
	float fData = 0;
	pSrc = HeadPutout(pSrc, pHead);
	fData = (float)atof(pSrc);
	return fData;
}

//=============================================================================
//    文字列を必要な部分だけ抜き出して返す処理
//=============================================================================
char *CFunctionLib::ReadString(char *pSrc, char *pDest, char *pHead)
{
	int nWord = 0;
	// 文字列を抜き出す
	pSrc = CFunctionLib::HeadPutout(pSrc, pHead);
	nWord = CFunctionLib::PopString(pSrc, pDest);
	strcpy(pDest, "\0");
	strcpy(pDest, pSrc);
	strcpy(&pDest[nWord - 1], "\0");

	return pDest;
}

//=============================================================================
//    文字列をBool型に変換して返す処理
//=============================================================================
bool CFunctionLib::ReadBool(char *pSrc, char *pHead)
{
	bool bBool = false;
	int nBool = CFunctionLib::ReadInt(pSrc, pHead);
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
//    文字列を３次元ベクトル型に変換して返す処理
//=============================================================================
D3DXVECTOR3 CFunctionLib::ReadVector3(char *pSrc, char *pHead)
{
	int nWord = 0;
	char aStr[256];
	D3DXVECTOR3 Vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// X
	pSrc = CFunctionLib::HeadPutout(pSrc, pHead);
	Vector.x = (float)atof(pSrc);

	// Y
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Vector.y = (float)atof(pSrc);

	// Z
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Vector.z = (float)atof(pSrc);

	return Vector;
}

//=============================================================================
//    文字列を４次元ベクトル型に変換して返す処理
//=============================================================================
D3DXCOLOR CFunctionLib::ReadVector4(char *pSrc, char *pHead)
{
	int nWord = 0;
	char aStr[256];

	D3DXCOLOR Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	// r
	pSrc = CFunctionLib::HeadPutout(pSrc, pHead);
	Color.r = (float)atof(pSrc);

	// g
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Color.g = (float)atof(pSrc);

	// b
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Color.b = (float)atof(pSrc);

	// a
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Color.a = (float)atof(pSrc);

	return Color;
}

//=============================================================================
//    ベクトルを回転させる処理(X方向)
//=============================================================================
float CFunctionLib::RotationVectorX(float fRot, float X, float Y)
{
	return cosf(fRot) * X - sinf(fRot) * Y;
}

//=============================================================================
//    ベクトルを回転させる処理(Y方向)
//=============================================================================
float CFunctionLib::RotationVectorY(float fRot, float X, float Y)
{
	return sinf(fRot) * X + cosf(fRot) * Y;
}

//=============================================================================
//    2つのベクトルの外積を出す
//=============================================================================
float CFunctionLib::D3DXVec2Cross(D3DXVECTOR2* Vec1, D3DXVECTOR2* Vec2)
{
	return Vec1->x * Vec2->y - Vec1->y * Vec2->x;
}