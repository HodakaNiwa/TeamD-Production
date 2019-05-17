//*****************************************************************************
//
//     2Dポリゴンの処理[scene2D.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    2Dポリゴンクラスの定義
//*****************************************************************************
class CScene2D : public CScene
{
public:     // 誰でもアクセス可能
	CScene2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScene2D();

	static CScene2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetRot(const float fRot);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetAngle(const float fAngle);
	void SetLength(const float fLength);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetRot(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetAngle(void);
	float GetLength(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	  // テクスチャへのポインタ
	D3DXVECTOR3				m_Pos;	      // ポリゴンの位置
	D3DXCOLOR               m_Col;        // ポリゴンの色
	float                   m_fRot;       // ポリゴンの向き
	float                   m_fAngle;     // ポリゴンを出す角度
	float                   m_fWidth;     // ポリゴンの幅
	float                   m_fHeight;    // ポリゴンの高さ
	float                   m_fLength;    // ポリゴンの長さ
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

	CScene2DFlash(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
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

	CScene2DPress(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
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

//*****************************************************************************
//    数字クラスの定義
//*****************************************************************************
class CNumber : public CScene2D
{
public:     // 誰でもアクセス可能
	typedef enum
	{
		STATE_NONE = 0,  // 通常状態
		STATE_FLASH,     // 点滅表示させる状態
		STATE_MAX
	}STATE;

	CNumber(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CNumber();

	static CNumber *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		int nNumber = 0, int nFlashTiming = 3, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		int nNumber = 0, int nFlashTiming = 3);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetNumber(const int nNumber);
	void SetState(const STATE State);

	int GetNumber(void);
	STATE GetState(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Flash(void);

	STATE m_State;         // 状態
	int   m_nNumber;       // 表示する数字
	int   m_nFlashTiming;  // 点滅させるタイミング
	int   m_nCounter;      // 描画を管理するカウンター
};

#endif