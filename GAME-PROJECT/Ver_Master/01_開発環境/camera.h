//=============================================================================
//
// カメラ処理 [camera.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// カメラクラスの定義
//*****************************************************************************
class CCamera
{
public:	//誰からもアクセス可能
	//---------------------
	// 種類
	//---------------------
	typedef enum
	{
		TYPE_NONE = -1,
		TYPE_CHARASELECT,
		TYPE_GAME,
		TYPE_MAX
	}TYPE;

	//メンバ関数
	CCamera();
	~CCamera();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Reset(void);
	void SetCamera(void);

	TYPE GetType(void);
	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetRot(void);
	float GetLength(void);
	bool GetMouseMove(void);
	bool GetChengeLength(void);
	float GetAngleOfView(void);

	void SetType(TYPE type);
	void SetPosV(D3DXVECTOR3 posV);
	void SetPosR(D3DXVECTOR3 posR);
	void SetRot(D3DXVECTOR3 Rot);
	void SetLength(float fLength);
	void SetMouseMove(bool bMouseMove);
	void SetChengeLength(bool bChengeLength);
	void SetAngleOfView(float fAngleOfView);

	// 静的メンバ関数
	static CCamera *Create(void);

private://自分だけがアクセス可能
	TYPE        m_Type;             // 種類
	D3DXVECTOR3 m_posV;				// 視点
	D3DXVECTOR3 m_posR;				// 注視点
	D3DXVECTOR3 m_vecU;				// 上方向ベクトル
	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;			// ビューマトリックス
	D3DXVECTOR3 m_rot;				// 向き
	D3DXVECTOR3 m_rotDest;			// 目的の向き
	D3DXVECTOR3	m_rotDiff;          // 現在の向きと目的の向きの差分
	float		m_fLength;			// 距離
	bool        m_bMouseMove;       // マウスで動かせるかどうか
	bool        m_bChengeLength;    // 距離を変えられるかどうか
	float       m_fAngleOfView;     // カメラの画角
};

//*****************************************************************************
// キャラセレクト用カメラクラスの定義
//*****************************************************************************
class CCharaSelectCamera : public CCamera
{
public:	//誰からもアクセス可能
	//メンバ関数
	CCharaSelectCamera();
	~CCharaSelectCamera();
	HRESULT Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR);
	void Uninit(void);
	void Update(void);

	// 静的メンバ関数
	static CCharaSelectCamera *Create(D3DXVECTOR3 posV, D3DXVECTOR3 posR);

private://自分だけがアクセス可能

};

//*****************************************************************************
// ゲーム用カメラクラスの定義
//*****************************************************************************
class CGameCamera : public CCamera
{
public:	//誰からもアクセス可能
	//---------------------
	// 状態
	//---------------------
	typedef enum
	{
		STATE_NONE = -1,
		STATE_NORMAL,
		STATE_SHAKE,
		STATE_MAX
	}STATE;

	//メンバ関数
	CGameCamera();
	~CGameCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void StartShake(int nTimer, float fShake, float fShakeCut);

	void SetState(const STATE state);
	STATE GetState(void);

	// 静的メンバ関数
	static CGameCamera *Create(void);

private://自分だけがアクセス可能
	void Normal(void);
	void Shake(void);

	D3DXVECTOR3 m_PosVDef;    // デフォルトの視点
	D3DXVECTOR3 m_PosRDef;    // デフォルトの注視点
	STATE m_State;            // 状態
	int m_nShakeTimer;        // 揺れる時間を計測するタイマー
	float m_fShakeValue;      // どれくらい揺れるか
	float m_fShakeCutValue;   // 揺れをどれくらい減らしていくか
};

#endif
