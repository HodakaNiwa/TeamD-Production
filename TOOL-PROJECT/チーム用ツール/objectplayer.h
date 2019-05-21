//=============================================================================
//
// オブジェクトプレイヤー処理 [objectplayer.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _OBJECTPLAYER_H_
#define _OBJECTPLAYER_H_

#include "main.h"
#include "scene.h"
#include "object.h"

//前方宣言
class CModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CObjectPlayer : public CObject
{
public:	//誰からもアクセス可能
	//メンバ関数
	CObjectPlayer();
	~CObjectPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetSelectObject(void);
	float GetSpeed(void);
	void SetSelectObject(int nSelect);
	void SetSpeed(float fSpeed);

	//静的メンバ関数
	static CObjectPlayer *Create(D3DXVECTOR3 pos);
	static CObject *GetObjectPointer(void);

private:	//自分だけがアクセス可能
	//メンバ関数
	void Move(void);

	//メンバ変数
	D3DXVECTOR3 m_pos;											//位置
	D3DXVECTOR3 m_rot;											//向き
	D3DXVECTOR3 m_move;											//移動量
	float		m_fSpeed;										//移動の速さ
	D3DXMATRIX	m_mtxWorld;										//ワールドマトリックス
	int			m_nNumModel;									//モデル総数
	D3DXVECTOR3	m_scale;										//大きさ
	static CObject		*m_pObject;								//オブジェクトのポインタ
	CObject		*m_pCreateObject;								//オブジェクトの生成ポインタ
	int			m_nSeletObject;									//オブジェクトの選択

};
#endif