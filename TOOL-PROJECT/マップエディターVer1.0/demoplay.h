//=============================================================================
//
// デモプレイの処理 [demoplay.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _DEMOPLAY_H_
#define _DEMOPLAY_H_

#include "main.h"
#include "basemode.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CDemoplay : public CBasemode
{
public:	//誰からもアクセス可能
	//メンバ関数
	CDemoplay();
	~CDemoplay();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CDemoplay *Create(void);

private:	//自分だけがアクセス可能
	void CreateCamera(void);
	void CreateMap(void);
};
#endif