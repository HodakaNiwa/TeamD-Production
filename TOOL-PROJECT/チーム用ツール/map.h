//=============================================================================
//
// マップの処理[map.h]
// Auther:Jukiya Hayakawa
//
//=============================================================================
#ifndef _MAP_H_
#define _MAP_H_

#include "main.h"

//前方宣言
class CModelCreate;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_FILENAME_MAP			"data\\TEXT\\MAP\\map.txt"
#define TEXT_SAVE_FILENAME_OBJECT	"data\\TEXT\\MAP\\OBJECT\\save_object.txt"
class CObject;
//*****************************************************************************
// クラス定義
//*****************************************************************************
class CMap
{
public:    // 誰からもアクセス可能
	CMap();
	~CMap();
	void Init(void);
	void Uninit(void);
	void Update(void);

	//静的メンバ関数
	static CMap *Create(void);
	static CModelCreate *GetCModelCreate(void);
	void SaveObject(void);
	void FieldLoad(char *pFieldFileName, char *pStrCur, char *pLine);

private:   //自分だけがアクセス可能
	void LoadObject(char *pFileNameObject, char *pStrCur, char *pLine);

	static CModelCreate		*m_pModelCreate;		// モデル生成のポインタ
};

#endif