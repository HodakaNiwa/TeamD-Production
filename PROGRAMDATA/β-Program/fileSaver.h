//*****************************************************************************
//
//     ファイル保存の処理[fileSaver.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _FILESAVER_H_
#define _FILESAVER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    ファイル保存クラスの定義
//*****************************************************************************
class CFileSaver
{
public:    // 誰でもアクセス可能
	CFileSaver(char *pSaveFileName);
	~CFileSaver();

	static CFileSaver *Create(char *pSaveFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Print(char *fmt, ...);

	void SetSavefileName(char *pFileName);
	char *GetSavefileName(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	FILE *m_pFile;               // ファイルポインタ
	char m_aSaveFileName[256];   // 保存するファイル名
};


#endif