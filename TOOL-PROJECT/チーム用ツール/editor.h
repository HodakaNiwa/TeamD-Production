//=============================================================================
//
// エディターの処理 [editor.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "main.h"

//前方宣言
class CMeshField;
class CCommand;
class CMap;
class CObjectPlayer;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEditor
{
public:	//誰からもアクセス可能
	//メンバ関数
	CEditor();
	~CEditor();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CEditor			*Create(void);
	static CMeshField		*GetMeshField(void);
	static CObjectPlayer	*GetObjectPlayer(void);
	static CMap				*GetMap(void);

private:	//自分だけがアクセス可能
	//静的メンバ変数
	static CMeshField		*m_pMeshField;		//メッシュフィールドのポインタ
	static CMap				*m_pMap;			//マップのポインタ
	static CObjectPlayer	*m_pObjectPlayer;	//オブジェクトプレイヤーのポインタ
};
#endif