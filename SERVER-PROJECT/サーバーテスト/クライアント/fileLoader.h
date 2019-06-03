//*****************************************************************************
//
//     ファイル読み込みの処理[fileLoader.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _FILELOADER_H_
#define _FILELOADER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    ファイル読み込みクラスの定義
//*****************************************************************************
class CFileLoader
{
public:    // 誰でもアクセス可能
	CFileLoader(char *pLoadFileName);
	~CFileLoader();

	static CFileLoader *Create(char *pLoadFileName);

	HRESULT Init(void);
	void Uninit(void);
	char *GetString(char *pSrc);

	void SetLoadfileName(char *pFileName);
	char *GetLoadfileName(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	FILE *m_pFile;               // ファイルポインタ
	char m_aLoadFileName[256];   // 読み込むファイル名
};


#endif