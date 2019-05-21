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
// カメラの構造体
//*****************************************************************************
class CCamera
{
public:	//誰からもアクセス可能
	//メンバ関数
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetRot(void);

	void SetPosV(D3DXVECTOR3 posV);
	void SetPosR(D3DXVECTOR3 posR);

	//静的メンバ関数
	static CCamera *Create(void);

private://自分だけがアクセス可能
	D3DXVECTOR3 m_posV;				//視点
	D3DXVECTOR3 m_posR;				//注視点
	D3DXVECTOR3 m_vecU;				//上方向ベクトル
	D3DXMATRIX	m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX	m_mtxView;			//ビューマトリックス
	D3DXVECTOR3 m_rot;				//向き
	D3DXVECTOR3 m_rotDest;			//目的の向き
	float		m_rotDiff;
	float		m_fLength;			//距離
	D3DXVECTOR3 m_posVDest;			//目的の視点
	D3DXVECTOR3 m_posRDest;			//目的の注視点
	D3DXVECTOR3 m_DiffAngle;		//差分

};
#endif
