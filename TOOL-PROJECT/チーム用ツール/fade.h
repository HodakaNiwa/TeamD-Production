//=============================================================================
//
// フェードの処理 [fade.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "scene.h"
#include "manager.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFade
{
public:	//誰からもアクセス可能
	//*************************************
	// フェードの状態
	//*************************************
	typedef enum
	{
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	} FADE;

	//メンバ関数
	CFade();
	~CFade();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetFade(CManager::MODE modeNext, FADE fade);

	//静的メンバ関数
	static CFade *Create(void);

private:	//自分だけがアクセス可能
	//メンバ変数
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;	// テクスチャへのポインタ

	//静的メンバ変数
	static FADE					m_fade;				//フェード
	static CManager::MODE		m_modeNext;			//次のモードの情報
	static D3DXCOLOR			m_colorFade;		//フェードの色
};
#endif