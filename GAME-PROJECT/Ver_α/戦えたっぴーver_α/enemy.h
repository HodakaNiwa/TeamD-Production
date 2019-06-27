//=============================================================================
//
// 敵の処理 [enemy.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "scene.h"
#include "character.h"
#include "player.h"
#include "block.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy : public CCharacter
{
public:	//誰からもアクセス可能
	//*****************************************************************************
	// 構造体定義
	//*****************************************************************************
	typedef enum
	{
		MOVE_UP = 0,
		MOVE_DOWN,
		MOVE_RIGHT,
		MOVE_LEFT,
		MOVE_MAX,
	}MOVE;

	//メンバ関数
	CEnemy();
	~CEnemy();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(CScene *pScene);

	void SetEnemy(CEnemy *pEnemy);
	void SetIdx(const int nIdx);

	CEnemy *GetEnemy(void);
	int GetIdx(void);

	//静的メンバ関数
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static int GetNumAll(void);
	static int GetSpawnCounter(void);
	static void ResetSpawnCounter(void);

private:	//自分だけがアクセス可能
	static int m_nNumAll;
	static int m_nSpawnCounter;

	//メンバ関数
	void Move(void);
	void SetBullet(void);
	void Collision(void);

	//メンバ変数
	CModel						*m_pModel;
	CEnemy						*m_pEnemy;
	int							m_nLife;
	MOVE						m_moveType;
	int                         m_nIdx;

	//静的メンバ変数
	static LPD3DXMESH			m_pMesh;					//メッシュ情報へのポインタ
	static LPD3DXBUFFER			m_pBuffMat;					//マテリアル情報へのポインタ
	static DWORD				m_nNumMat;					//マテリアル情報の数

};
#endif
