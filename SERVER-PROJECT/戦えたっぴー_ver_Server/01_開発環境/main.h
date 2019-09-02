//*****************************************************************************
//
//     メインの処理[main.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
//     ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS         // 標準入力警告対策マクロ
#define _WINSOCK_DEPRECATED_NO_WARNINGS // Winsock警告対策マクロ
#include <stdio.h>                      // 標準入出力処理に必要
#include <stdlib.h>                     // 標準関数処理に必要
#include <stdarg.h>                     // 可変引数を使用するために必要
#include <windows.h>                    // Windows標準処理
#include <process.h>                    // マルチスレッドプログラムに必要
#include <time.h>                       // ランダムな値を出力するために必要
#include "d3dx9.h"                      // DirectX(ver9.0)の処理に必要
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"                     // 入力処理に必要
#include "xaudio2.h"                    // サウンド処理に必要
#include <XInput.h>                     // XInputの処理に必要

#ifdef _DEBUG
// メモリリークチェックに必要
#include <crtdbg.h>
#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__)
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#endif

//*****************************************************************************
//     ライブラリファイルのリンク
//*****************************************************************************
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "XInput.lib")

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define SERVER_MODE            // 宣言時サーバー : 非宣言時 クライアント
#define SCREEN_WIDTH  (1280)   // ウィンドウの幅
#define SCREEN_HEIGHT (720)    // ウィンドウの高さ
#define NUM_POLYGON   (2)      // 描画するポリゴンの数

// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線ベクトル / 頂点カラー / テクスチャ座標)
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// DirectXで作成されたポインタの開放(マクロで楽にしておく)
#define DIRECT_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }


//*****************************************************************************
//     構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;  // 頂点座標
	float rhw;        // 頂点座標変換用(1.0fで固定)
	D3DCOLOR col;     // 頂点カラー
	D3DXVECTOR2 tex;  // テクスチャ画像
} VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;  // 頂点座標
	D3DXVECTOR3 nor;  // 法線ベクトル
	D3DCOLOR col;     // 頂点カラー
	D3DXVECTOR2 tex;  // テクスチャ画像
} VERTEX_3D;

//*****************************************************************************
//     プロトタイプ宣言
//*****************************************************************************
#if _DEBUG
int GetFPSCount(void);
#endif

#endif