//=============================================================================
//
// テクスチャマネージャーの処理 [textureManager.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//前方宣言
#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTextureManager
{
public:	//誰からもアクセス可能
		//メンバ関数
	CTextureManager();
	~CTextureManager();
	void Init(void);
	void Uninit(void);

	void SetTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx);
	void SetNumTexture(int nNumTexture);
	void SetFileName(char *pFileName, int nIdx);

	LPDIRECT3DTEXTURE9 GetTexture(int nIdx);
	int GetNumTexture(void);
	char *GetFileName(int nIdx);

	//静的メンバ関数
	static CTextureManager *Create(int nNumTexture);

private:	//自分だけがアクセス可能
	//メンバ変数
	LPDIRECT3DTEXTURE9		*m_pTexture;	//テクスチャのポインタ
	char					**m_pFileName;	//ファイル名
	int						m_nNumTexture;	//テクスチャの総数

};
#endif
