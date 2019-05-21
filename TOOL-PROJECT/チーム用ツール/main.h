//=============================================================================
//
// メイン処理 [main.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//前方宣言
class CScene;
class CRenderer;
class CManager;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int GetFPS(void);

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS			//警告対処
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "d3dx9.h"						//描画処理に必要
#include "xaudio2.h"					//音楽処理に必要
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"						//入力処理に必要

//デバッグ処理に必要
#include <crtdbg.h>
#ifdef _DEBUG
//#define _malloc_dbg(X, _NORMAL_BLOCK, __FILE__, __LINE__)
//#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#endif // _DEBUG

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")			//描画処理に必要
#pragma comment(lib,"d3dx9.lib")		//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")		//DirectXコンポーネント（部品）使用に必要
#pragma comment(lib,"winmm.lib")		//システム時刻取得に必要
#pragma comment (lib,"dinput8.lib")		// 入力処理に必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)		// ウインドウの幅
#define SCREEN_HEIGHT	(720)		// ウインドウの高さ
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// ３Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//1.0fで固定
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
} VERTEX_2D;

// ３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ
} VERTEX_3D;

#endif
