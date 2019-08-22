//*****************************************************************************
//
//     数字の処理[number.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    数字ポリゴンクラスの定義
//*****************************************************************************
class CNumberPolygon : public CScene2D
{
public:     // 誰でもアクセス可能
	typedef enum
	{
		STATE_NONE = 0,  // 通常状態
		STATE_FLASH,     // 点滅表示させる状態
		STATE_MAX
	}STATE;

	CNumberPolygon(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCENE2D);
	~CNumberPolygon();

	static CNumberPolygon *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
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

//*****************************************************************************
//    数字クラスの定義
//*****************************************************************************
class CNumber : CScene
{
public:     // 誰でもアクセス可能
	CNumber(int nPriority = 3, OBJTYPE objType = OBJTYPE_NUMBER);
	~CNumber();

	static CNumber *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight,
		D3DXVECTOR3 fInterval, LPDIRECT3DTEXTURE9 pNumberTex, int nNumber = 0,
		CNumberPolygon::STATE State = CNumberPolygon::STATE_NONE, int nFlashTiming = 3,
		float fRot = 0.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetNumber(const int nNumber);
	void SetNumberVtxBuffCol();

	void SetNumberValue(const int nNumber);
	void SetNumberTexture(const LPDIRECT3DTEXTURE9 pNumberTex);
	void SetNumberPos(const D3DXVECTOR3 NumberPos);
	void SetNumberCol(const D3DXCOLOR NumberCol);
	void SetNumberWidth(const float fNumberWidth);
	void SetNumberHeight(const float fNumberHeight);
	void SetNumberInterval(const D3DXVECTOR3 NumberInterval);
	void SetNumberRot(const float fNumberRot);
	void SetNumberState(const CNumberPolygon::STATE state);
	void SetNumberFlashTiming(const int nNumberFlashTiming);

	int GetNumber(void);
	int GetNumberDigit(void);
	CNumberPolygon **GetNumberPolygon(void);
	LPDIRECT3DTEXTURE9 GetNumberTexture(void);
	D3DXVECTOR3 GetNumberPos(void);
	D3DXCOLOR GetNumberCol(void);
	float GetNumberWidth(void);
	float GetNumberHeight(void);
	D3DXVECTOR3 GetNumberInterval(void);
	float GetNumberRot(void);
	CNumberPolygon::STATE GetNumberState(void);
	int GetNumberFlashTiming(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CalcNumberDigit(void);
	void CreateNumberPolygon(void);
	void ReleaseNumberPolygon(int nDigit);

	int                   m_nNumber;             // 現在表示している数字
	int                   m_nNumberDigit;        // 表示している数字の桁数
	CNumberPolygon        **m_pNumberPolygon;    // 数字ポリゴンクラスへのポインタ
	LPDIRECT3DTEXTURE9    m_pNumberTexture;      // 数字ポリゴンに使用しているテクスチャへのポインタ
	D3DXVECTOR3           m_NumberPos;           // 数字ポリゴンの位置
	D3DXCOLOR             m_NumberCol;           // 数字ポリゴンの色
	float                 m_fNumberWidth;        // 数字ポリゴンの幅
	float                 m_fNumberHeight;       // 数字ポリゴンの高さ
	D3DXVECTOR3           m_NumberInterval;      // 数字ポリゴン同士の間隔
	float                 m_fNumberRot;          // 数字ポリゴンの向き
	CNumberPolygon::STATE m_NumberState;         // 数字ポリゴンの状態
	int                   m_nNumberFlashTiming;  // 数字ポリゴンを点滅させるのに使う
};

#endif