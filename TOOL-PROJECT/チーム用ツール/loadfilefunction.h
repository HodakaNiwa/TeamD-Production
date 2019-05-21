//=============================================================================
//
// ファイルを読み込む際の関数[loadfilefunction.h]
// Auther:Jukiya Hayakawa
//
//=============================================================================
#ifndef _LOADFILEFUNCTION_H_
#define _LOADFILEFUNCTION_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_NUM_MODEL		"NUM_MODEL = "
#define TEXT_FILENAME_MODEL		"MODEL_FILENAME = "
#define TEXT_NUM_PARTS		"NUM_PARTS = "
#define TEXT_CHARASET		"CHARACTERSET"
#define TEXT_END_CHARASET	"END_CHARACTERSET"
#define TEXT_PARTSSET		"PARTSSET"
#define TEXT_END_PARTSSET	"END_PARTSSET"
#define TEXT_INDEX			"INDEX = "
#define TEXT_PARENT			"PARENT = "
#define TEXT_POS			"POS = "
#define TEXT_ROT			"ROT = "
#define TEXT_MOTIONSET		"MOTIONSET"
#define TEXT_END_MOTIONSET	"END_MOTIONSET"
#define TEXT_LOOP			"LOOP = "
#define TEXT_FRAME			"FRAME = "
#define TEXT_NUM_KEY		"NUM_KEY = "
#define TEXT_KEYSET			"KEYSET"
#define TEXT_END_KEYSET		"END_KEYSET"
#define TEXT_KEY			"KEY"
#define TEXT_END_KEY		"END_KEY"
#define TEXT_MODELSET		"MODELSET"
#define TEXT_END_MODELSET	"END_MODELSET"
#define TEXT_TYPE			"TYPE = "
#define TEXT_SIZE			"SIZE = "
#define TEXT_TAB			"\t"
#define TEXT_ENTER			"\n"
#define TEXT_SPACE			" "
#define TEXT_COMMENT		"#"
#define TEXT_SCRIPT          "SCRIPT"
#define TEXT_END_SCRIPT      "END_SCRIPT"
#define TEXT_HIT			 "HIT = "
#define TEXT_DESTROY		 "DESTROY = "
//*****************************************************************************
// クラスの定義
//*****************************************************************************
class CLoadFileFunction
{
public:    // 誰からもアクセス可能
	CLoadFileFunction();
	~CLoadFileFunction();

	static char *ReadLine(FILE *pFile, char *pDst);
	static char *GetLineTop(char *pScr);
	static int PopString(char *pScr, char *pDst);
};

#endif