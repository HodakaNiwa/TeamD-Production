//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRenderer
{
public:	//誰からもアクセス可能
	//メンバ関数
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetRenderTarget(const LPDIRECT3DTEXTURE9 pTexture);
	void ResetRenderTarget(void);
	void ReleaseMultRenderer(void);
	LPDIRECT3DDEVICE9 GetDevice(void);

private:	//自分だけがアクセス可能
	LPDIRECT3D9        m_pD3D;        // Direct3Dオブジェクト
	LPDIRECT3DDEVICE9  m_pD3DDevice;  // Deviceオブジェクト(描画に必要)
	LPDIRECT3DSURFACE9 m_pRenderOrg;  // デフォルトのレンダリングターゲットへのポインタ
	LPDIRECT3DSURFACE9 m_pBuffOrg;    // デフォルトの各種バッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTextureMT;  // マルチレンダリングに使用するテクスチャへのポインタ
	LPDIRECT3DSURFACE9 m_pRenderMT;   // マルチレンダリングに使用するレンダリングターゲットへのポインタ
	LPDIRECT3DSURFACE9 m_pBuffMT;     // マルチレンダリングに使用する各種バッファへのポインタ
};
#endif