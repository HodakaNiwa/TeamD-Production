//=============================================================================
//
// 弾の処理 [bullet.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "billboard.h"

#define TEXTURE_NAME001		"data\\TEXTURE\\bullet000.png"			//テクスチャ名
#define MAX_BULLET			(3)
#define BULLET_HIT			(30)								// 当たり判定

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBullet : public CBillboard
{
public:
	//メンバ関数
	CBullet();
	~CBullet();
	static HRESULT Load(void);
	static void Unload(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Collision(void);
	//静的メンバ関数
	static CBullet *Create(D3DXVECTOR3 pos,float fWidth,float fHeight,D3DXVECTOR3 move);

private:
	//メンバ変数
	int m_nLife;						//体力
	D3DXVECTOR3 m_move;					//移動量
	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;
};
#endif