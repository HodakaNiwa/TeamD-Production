//*****************************************************************************
//
//     サーバー用関数の処理[serverlib.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SERVERLIB_H_
#define _SERVERLIB_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     サーバー用ライブラリクラスの定義
//*****************************************************************************
class CServerLib
{
public:    // 誰でもアクセス可能
	CServerLib();
	~CServerLib();

	static int Memcmp(char *pSrc, char *pComp);
	static char *GetLineTop(char *pSrc);
	static int PopString(char *pSrc, char *pDest);
	static char *HeadPutout(char *pDest, char *pHead);
	static int ReadInt(char *pStr, char *pHead);
	static float ReadFloat(char *pStr, char *pHead);
	static bool ReadBool(char *pStr, char *pHead);
	static char *ReadString(char *pStr, char *pDest, char *pHead);
	static char *WriteInt(int nData, char *pDest);
	static char *WriteFloat(float fData, char *pDest);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

#endif