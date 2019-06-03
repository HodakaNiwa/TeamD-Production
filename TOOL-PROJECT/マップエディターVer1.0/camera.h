//=============================================================================
//
// カメラ処理 [camera.h]
// Author : Jukiya Hayakawa
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
	//メンバ関数
	CCamera();
	~CCamera();
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Reset(void);
	void SetCamera(void);

	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetRot(void);
	float GetLength(void);
	bool GetMouseMove(void);
	bool GetChengeLength(void);

	void SetPosV(D3DXVECTOR3 posV);
	void SetPosR(D3DXVECTOR3 posR);
	void SetRot(D3DXVECTOR3 Rot);
	void SetLength(float fLength);
	void SetMouseMove(bool bMouseMove);
	void SetChengeLength(bool bChengeLength);

	// 静的メンバ関数
	static CCamera *Create(void);

private://自分だけがアクセス可能
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
};

//*****************************************************************************
// エディター用カメラクラスの定義
//*****************************************************************************
class CEditorCamera : public CCamera
{
public:	//誰からもアクセス可能
	//メンバ関数
	CEditorCamera();
	~CEditorCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Reset(void);

	// 静的メンバ関数
	static CEditorCamera *Create(void);

private://自分だけがアクセス可能
	void MouseMove(void);
};

#endif
