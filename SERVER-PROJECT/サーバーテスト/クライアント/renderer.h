//*****************************************************************************
//
//     レンダリングの処理[renderer.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************


//*****************************************************************************
//    前方宣言(実体がない宣言)
//*****************************************************************************
class CPause;

//*****************************************************************************
//    レンダリングクラスの定義
//*****************************************************************************
class CRenderer
{
public:     // 誰でもアクセス可能
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	LPDIRECT3D9				m_pD3D;			                        // Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;	                        // Deviceオブジェクト(描画に必要)
};

#endif