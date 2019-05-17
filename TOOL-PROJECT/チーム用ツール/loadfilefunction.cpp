//=============================================================================
//
// ファイルを読み込む際の関数 [loadfilefunction.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "loadfilefunction.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CLoadFileFunction::CLoadFileFunction()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CLoadFileFunction::~CLoadFileFunction()
{

}

//=============================================================================
// 行の読み込み処理
//=============================================================================
char *CLoadFileFunction::ReadLine(FILE *pFile, char *pDst)
{
	fgets(pDst, 256, pFile);
	while (1)
	{//情報がなくなるまで繰り返し

		if (memcmp(pDst, "#", strlen("#")) == 0)
		{//コメントを示す行の場合
			fgets(pDst, 256, pFile);
		}
		else if (memcmp(pDst, "\n", strlen("\n")) == 0)
		{//改行を示す行の場合
			fgets(pDst, 256, pFile);
		}
		else if (memcmp(pDst, "\t", strlen("\t")) == 0)
		{//タブを示す行の場合
			pDst += strlen("\t");	//頭出し

			while (1)
			{
				if (memcmp(pDst, "#", 1) == 0)
				{//コメントを示す行の場合
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\n", 2) == 0)
				{//改行を示す行の場合
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\t", 2) == 0)
				{//タブを示す行の場合
					pDst = GetLineTop(pDst);
				}
				else if (memcmp(pDst, " ", 2) == 0)
				{//スペースを示す行の場合
					pDst = GetLineTop(pDst);
				}
				else
				{
					break;
				}
			}
		}
		else if (memcmp(pDst, " ", 1) == 0)
		{//スペースを示す行の場合
			while (1)
			{
				if (memcmp(pDst, "#", 1) == 0)
				{//コメントを示す行の場合
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\n", 2) == 0)
				{//改行を示す行の場合
					fgets(pDst, 256, pFile);
					break;
				}
				else if (memcmp(pDst, "\t", 2) == 0)
				{//タブを示す行の場合
					pDst = GetLineTop(pDst);
				}
				else if (memcmp(pDst, " ", 1) == 0)
				{//スペースを示す行の場合
					pDst = GetLineTop(pDst);
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
	return pDst;
}

//=============================================================================
// 空白の処理
//=============================================================================
char *CLoadFileFunction::GetLineTop(char *pScr)
{
	while (1)
	{
		if (memcmp(pScr, " ", strlen(" ")) == 0)
		{//スペースを示す行の場合
			pScr += strlen(" ");
		}
		else if (memcmp(pScr, "\t", strlen("\t")) == 0)
		{//タブを示す行の場合
			pScr += strlen("\t");
		}
		else
		{
			break;
		}
	}

	return pScr;
}

//=============================================================================
// 文字数カウント処理
//=============================================================================
int CLoadFileFunction::PopString(char *pScr, char *pDst)
{
	int nCount = 0;
	while (1)
	{
		nCount += 1;	//カウンターを加算
		pScr += 1;		//頭出し
		if (memcmp(pScr, " ", strlen(" ")) == 0)
		{//スペースを示す行の場合
			pScr = "\0";	//NULLにする
			nCount += 1;	//カウンターを加算
			break;
		}
		else if (memcmp(pScr, "\t", strlen("\t")) == 0)
		{//スペースを示す行の場合
			pScr = "\0";	//NULLにする
			nCount += 1;	//カウンターを加算する
			break;
		}
	}
	strcpy(pDst, pScr);
	return  nCount;
}