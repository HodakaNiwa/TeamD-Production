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
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS			//警告対処
#define _WINSOCK_DEPRECATED_NO_WARNINGS // Winsock警告対策マクロ
#include <windows.h>
#include <process.h>                    // マルチスレッドプログラムに必要
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "d3dx9.h"						//描画処理に必要
#include "xaudio2.h"					//音楽処理に必要
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"						//入力処理に必要
#include <XInput.h>

//デバッグ処理に必要
#include <crtdbg.h>
#ifdef _DEBUG
#define _malloc_dbg(X, _NORMAL_BLOCK, __FILE__, __LINE__)
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#endif // _DEBUG

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment (lib, "ws2_32.lib")     // winsockに必要
#pragma comment(lib,"d3d9.lib")			// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")		// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")		// DirectXコンポーネント（部品）使用に必要
#pragma comment(lib,"winmm.lib")		// システム時刻取得に必要
#pragma comment (lib,"dinput8.lib")		// 入力処理に必要
#pragma comment (lib, "XInput.lib")     // XInputに必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	 (1280)		           // ウインドウの幅
#define SCREEN_HEIGHT	 (720)		           // ウインドウの高さ
#define MASS_BLOCK_X     (19)                  // マスの分割数(横)
#define MASS_BLOCK_Z     (15)                  // マスの分割数(縦)
#define MASS_SIZE_X      (75.0f)               // マスのサイズ(横)
#define MASS_SIZE_Z      (75.0f)               // マスのサイズ(縦)
#define MASS_SIZE_X_HALF (MASS_SIZE_X / 2.0f)  // マスのサイズの半分(横)
#define MASS_SIZE_Z_HALF (MASS_SIZE_Z / 2.0f)  // マスのサイズの半分(縦)
#define MAX_NUM_PLAYER   (2)                   // プレイヤーの総数
#define INITIALIZE_D3DXVECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f))

// 2Dポリゴン用頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D]/ 頂点法線 / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// DirectXで作成されたポインタの開放(マクロで楽にしておく)
#define DIRECT_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

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
