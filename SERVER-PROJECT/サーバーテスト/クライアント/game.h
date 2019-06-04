//=============================================================================
//
// ゲームの処理 [game.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "basemode.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPlayer;

//*****************************************************************************
// ゲームクラスの定義
//*****************************************************************************
class CGame : public CBasemode
{
public:	//誰からもアクセス可能
		//メンバ関数
	CGame();
	~CGame();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CPlayer *GetPlayer(int nIdx);

	//静的メンバ関数
	static CGame *Create(void);

private:	//自分だけがアクセス可能
	void CreateCamera(void);
	void CreateMap(void);
	void Debug(void);

	CPlayer *m_pPlayer[2];
};
#endif