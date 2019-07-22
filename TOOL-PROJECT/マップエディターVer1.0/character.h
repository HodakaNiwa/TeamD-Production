//=============================================================================
//
// キャラクターの処理 [character.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#ifndef _CHARCTER_H_
#define _CHARCTER_H_

#include "main.h"
#include "object3D.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;
class CMotionManager;

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

	void SetNumPart(const int nNumParts);
	void SetModel(CModel **apModel);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetAccel(const float fAccel);
	void SetInertia(const float fInertia);
	void SetRivisionRot(const float fRivisionRot);
	void SetPosOld(D3DXVECTOR3 posOld);
	void SetMove(D3DXVECTOR3 move);
	void SetShoot(bool bHit);
	void SetNowRotInfo(NOW_ROT_INFO nowRotInfo);

	int GetNumPart(void);
	CModel **GetModel(void);
	CMotionManager *GetMotionManager(void);
	float GetAccel(void);
	float GetInertia(void);
	float GetRivisionRot(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetMove(void);
	bool GetShoot(void);
	NOW_ROT_INFO GetNowRotInfo(void);

private:	//自分だけがアクセス可能
	void ClearVariable(void);
	void ReleaseModel(void);
	void ReleaseMotionManager(void);
	void ModelDraw(void);

	// メンバ変数
	int                         m_nNumParts;                        // 必要なパーツ数
	CModel						**m_apModel;						// モデル情報へのポインタ
	CMotionManager              *m_pMotionManager;                  // モーション管轄クラスへのポインタ
	float                       m_fAccel;                           // 移動できるスピード量
	float                       m_fInertia;                         // 慣性量
	float                       m_fRivisionRot;                     // 向きを補正する倍率
	D3DXVECTOR3					m_posOld;							// 前回の位置
	D3DXVECTOR3					m_move;								// 移動量
	bool						m_bShoot;							// 撃っているかどうか
	NOW_ROT_INFO				m_nowRotInfo;						// 現在の向き情報
};
#endif