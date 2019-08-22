//=============================================================================
//
// オブジェクト2D処理 [scene2D.h]
// Author : Jukiya Hayakawa & Hodaka Niwa
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CScene2D : public CScene
{
public:    // 誰でもアクセス可能
	//メンバ関数
	CScene2D(int nPriority = 3, OBJTYPE objtype = OBJTYPE_SCENE2D);
	~CScene2D();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetScale(const float fWidth, const float fHeight);
	void SetVtxBuffValue(void);
	void SetVtxBuffPos(void);
	void SetVtxBuffCol(void);
	void SetVtxBuffTex(void);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetRot(const float fRot);
	void SetAngle(const float fAngle);
	void SetLength(const float fLength);
	void SetStartTexU(const float fStartTexU);
	void SetStartTexV(const float fStartTexV);
	void SetAddTexU(const float fAddTexU);
	void SetAddTexV(const float fAddTexV);

	LPDIRECT3DTEXTURE9 GetTexture(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetRot(void);
	float GetAngle(void);
	float GetLength(void);
	float GetStartTexU(void);
	float GetStartTexV(void);
	float GetAddTexU(void);
	float GetAddTexV(void);

	// 静的メンバ関数
	static CScene2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9			m_pTexture = NULL;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	D3DXVECTOR3					m_pos;				// ポリゴンの位置
	D3DXCOLOR					m_col;				// ポリゴンの色
	float						m_fWidth;			// ポリゴンの幅
	float						m_fHeight;			// ポリゴンの高さ
	float						m_fRot;				// ポリゴンの向き
	float                       m_fAngle;           // ベクトルの角度
	float						m_fLength;			// ベクトルの長さ
	float                       m_fStartTexU;       // テクスチャU座標(始点)
	float                       m_fStartTexV;       // テクスチャV座標(始点)
	float                       m_fAddTexU;         // テクスチャU座標に加える値
	float                       m_fAddTexV;         // テクスチャV座標に加える値
};

//*****************************************************************************
//    2Dポリゴン点滅クラスの定義
//*****************************************************************************
class CScene2DFlash : public CScene2D
{
public:     // 誰でもアクセス可能
	typedef enum
	{
		STATE_NONE = 0,  // 通常状態
		STATE_SELECT,    // 選択された状態
		STATE_PUSH,      // 押された状態
		STATE_MAX,
	}STATE;

	CScene2DFlash(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);
	~CScene2DFlash();

	static CScene2DFlash *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		D3DXCOLOR ColMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR ColMin = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f),
		float fColChange = 0.01f, int nPushTiming = 4, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		D3DXCOLOR ColMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR ColMin = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f),
		float fColChange = 0.01f, int nPushTiming = 4);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE State);
	void SetColMax(const D3DXCOLOR ColMax);
	void SetColMin(const D3DXCOLOR ColMin);
	void SetColChange(const float fColChange);

	STATE GetState(void);
	D3DXCOLOR GetColMax(void);
	D3DXCOLOR GetColMin(void);
	float GetColChange(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Select(void);

	STATE     m_State;        // 状態
	D3DXCOLOR m_ColMax;       // 色の最大値
	D3DXCOLOR m_ColMin;       // 色の最小値
	float     m_fColChange;   // 色の変化量
	int       m_nPushTiming;  // 描画を切り替えるタイミング(押された状態の時)
	int       m_nCounter;     // 描画を管理するカウンター
};

//*****************************************************************************
//    2Dポリゴンプレスクラスの定義
//*****************************************************************************
class CScene2DPress : public CScene2D
{
public:     // 誰でもアクセス可能
	typedef enum
	{
		STATE_NONE = 0,  // 通常状態
		STATE_PUSH,      // 押された状態
		STATE_MAX,
	}STATE;

	CScene2DPress(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);
	~CScene2DPress();

	static CScene2DPress *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f,
		int nStopTiming = 80, int nAppearTiming = 100, int nPushTiming = 4, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f,
		int nStopTiming = 80, int nAppearTiming = 100, int nPushTiming = 4);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void FlashStart(void);

	void SetState(const STATE State);
	void SetAppearTiming(const int nAppearTiming);
	void SetStopTiming(const int nStopTiming);
	void SetCounter(const int nCounter);

	STATE GetState(void);
	int GetAppearTiming(void);
	int GetStopTiming(void);
	int GetCounter(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void None(void);

	STATE m_State;          // 状態
	int   m_nStopTiming;    // 描画を一時停止するタイミング
	int   m_nAppearTiming;  // 描画を再開するタイミング
	int   m_nCounter;       // 描画を管理するカウンター
	int   m_nPushTiming;    // 描画を切り替えるタイミング(押された状態の時)
};

#endif