//=============================================================================
//
// 弾の処理 [bullet.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

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
class CBullet : public CObject3D
{
public:	//誰からもアクセス可能
	//=========================================================================
	// 構造体の定義
	//=========================================================================
	typedef enum
	{
		BULLET_TYPE_NONE = 0,
		BULLET_TYPE_PLAYER,
		BULLET_TYPE_ENEMY,
		BULLET_TYPE_MAX,
	} BULLET_TYPE;

	//メンバ関数
	CBullet();
	~CBullet();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosOld(D3DXVECTOR3 posOld);
	void SetMove(D3DXVECTOR3 move);
	void SetBulletType(BULLET_TYPE type);
	void SetScene(CScene *pScene);

	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetMove(void);
	BULLET_TYPE GetBulletType(void);
	CScene *GetScene(void);

	//静的メンバ関数
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 move, BULLET_TYPE type,CScene *pScene);

private:	//自分だけがアクセス可能
	//メンバ関数
	void Move(void);
	void Collision(void);
	void Destroy(void);

	//メンバ変数
	CModel						*m_pModel;					//モデルへのポインタ
	D3DXVECTOR3					m_posOld;					//過去の位置
	D3DXVECTOR3					m_move;						//移動量
	BULLET_TYPE					m_type;						//弾の種類
	CScene						*m_pScene;					//オブジェクトへのポインタ情報

	//静的メンバ変数
	static LPD3DXMESH			m_pMesh;					//メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat;					//マテリアル情報へのポインタ
	static DWORD				m_nNumMat;					//マテリアル情報の数

};
#endif
