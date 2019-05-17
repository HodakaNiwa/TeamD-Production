//=============================================================================
//
// ImGuiの処理 [imgui_jukiya.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _IMGUI_JUKIYA_H_
#define _IMGUI_JUKIYA_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CImGui_Jukiya
{
public:	//誰からもアクセス可能
		//メンバ関数
	CImGui_Jukiya();
	~CImGui_Jukiya();
	void Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	void Uninit(void);
	void Update(void);
	void Draw(void);
private:	//自分だけがアクセス可能
};
#endif
