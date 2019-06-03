//=============================================================================
//
// キャラクターの処理 [character.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _CHARCTER_H_
#define _CHARCTER_H_

#include "main.h"
#include "scene.h"
#include "object3D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CCharacter : public CObject3D
{
public:	//誰からもアクセス可能
	typedef enum
	{
		ROT_UP = 0,
		ROT_DOWN,
		ROT_RIGHT,
		ROT_LEFT,
		ROT_MAX,
	}NOW_ROT_INFO;

	//メンバ関数
	CCharacter(int nPriority = 3, OBJTYPE objtype = OBJTYPE_CHARACTER);
	~CCharacter();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosOld(D3DXVECTOR3 posOld);
	void SetMove(D3DXVECTOR3 move);
	void SetShoot(bool bHit);
	void SetNowRotInfo(NOW_ROT_INFO nowRotInfo);

	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetMove(void);
	bool GetShoot(void);
	NOW_ROT_INFO GetNowRotInfo(void);

private:	//自分だけがアクセス可能
	//メンバ変数
	D3DXMATRIX					m_mtxWorld;							// ワールドマトリックス
	CModel						*m_pModel;							//モデル情報へのポインタ
	D3DXVECTOR3					m_posOld;							// 過去の位置
	D3DXVECTOR3					m_move;								// 移動量
	bool						m_bShoot;							// 撃っているかどうか
	NOW_ROT_INFO				m_nowRotInfo;							// 現在の向き情報
	//静的メンバ変数
};
#endif