//=============================================================================
//
// プレイヤーの処理 [player.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene.h"
#include "character.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPlayer : public CCharacter
{
public:	//誰からもアクセス可能
		//メンバ関数
	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPlayer(CPlayer *pPlayer);

	CPlayer *GetPlayer(void);

	//静的メンバ関数
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:	//自分だけがアクセス可能
	//メンバ関数
	void Move(void);
	void Collision(void);

	//メンバ変数
	CModel						*m_pModel;
	CPlayer						*m_pPlayer;
	//静的メンバ変数
	static LPD3DXMESH			m_pMesh;						//メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat;					//マテリアル情報へのポインタ
	static DWORD				m_nNumMat;					//マテリアル情報の数

};
#endif
