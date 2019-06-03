//*****************************************************************************
//
//     共通で使う関数の処理[functionlib.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _FUNKTIONLIB_H_
#define _FUNKTIONLIB_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// 共通で使う文字の定義
#define STRING_TAB       "\t"                     // Tabの文字列
#define STRING_ENTER     "\n"                     // 改行コード
#define STRING_SPACE     " "                      // 空白の文字列
#define STRING_COMMENT   "#"                      // コメントが開始される文字
#define SCRIPT           "SCRIPT"                 // スクリプト読み込み開始の合図
#define END_SCRIPT       "END_SCRIPT"             // スクリプト読み込み終了の合図

//*****************************************************************************
//     共通関数クラスの定義
//*****************************************************************************
class CFunctionLib
{
public:    // 誰でもアクセス可能
	CFunctionLib();
	~CFunctionLib();

	static char *ReadLine(FILE *pFile, char *pDest);
	static int Memcmp(char *pSrc, char *pComp);
	static char *GetLineTop(char *pSrc);
	static int PopString(char *pSrc, char *pDest);
	static char *HeadPutout(char *pDest, char *pHead);
	static int ReadInt(char *pSrc, char *pHead);
	static float ReadFloat(char *pSrc, char *pHead);
	static char *ReadString(char *pSrc, char *pDest, char *pHead);
	static bool ReadBool(char *pSrc, char *pHead);
	static D3DXVECTOR3 ReadVector3(char *pSrc, char *pHead);
	static D3DXCOLOR ReadVector4(char *pSrc, char *pHead);
	static float RotationVectorX(float fRot, float X, float Y);
	static float RotationVectorY(float fRot, float X, float Y);
	static float D3DXVec2Cross(D3DXVECTOR2* Vec1, D3DXVECTOR2* Vec2);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

#endif