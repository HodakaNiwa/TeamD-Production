//*****************************************************************************
//
//     エミッタの処理[emitter.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EMITTER_H_
#define _EMITTER_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     マクロ宣言
//*****************************************************************************
#define EMITTER_MOVEMENT   (6.0f)    // エミッタの移動量
#define EMITTER_ROTATION   (0.2f)    // エミッタの回転量

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CParData;
class CRingData;

//*****************************************************************************
//     エミッタデータクラスの定義
//*****************************************************************************
class CEmitterData
{
public:    // 誰でもアクセス可能
	CEmitterData();
	~CEmitterData();

	void SetType(const int nType) { m_nType = nType; }
	void SetTexIdx(const int nTexIdx) { m_nTexIdx = nTexIdx; }
	void SetEffectIdx(const int nEffectIdx) { m_nEffectIdx = nEffectIdx; }
	void SetRangeMax(const float fRangeMax) { m_fRangeMax = fRangeMax; }
	void SetRangeMin(const float fRangeMin) { m_fRangeMin = fRangeMin; }
	void SetLife(const int nLife) { m_nLife = nLife; }
	void SetLap(const int nLap) { m_nLap = nLap; }
	void SetAppear(const int nAppear) { m_nAppear = nAppear; }
	void SetLoop(const bool bLoop) { m_bLoop = bLoop; }

	int GetType(void) { return m_nType; }
	int GetTexIdx(void) { return m_nTexIdx; }
	int GetEffectIdx(void) { return m_nEffectIdx; }
	float GetRangeMax(void) { return m_fRangeMax; }
	float GetRangeMin(void) { return m_fRangeMin; }
	int GetLife(void) { return m_nLife; }
	int GetLap(void) { return m_nLap; }
	int GetAppear(void) { return m_nAppear; }
	bool GetLoop(void) { return m_bLoop; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int   m_nType;       // 使用するエフェクトの種類
	int   m_nTexIdx;     // 使用するテクスチャの番号
	int   m_nEffectIdx;  // 使用するエフェクトデータの番号
	float m_fRangeMax;   // エフェクトを生成する範囲の最大値
	float m_fRangeMin;   // エフェクトを生成する範囲の最小値
	int   m_nLife;       // 寿命
	int   m_nLap;        // 1回の放出で出すエフェクトの個数
	int   m_nAppear;     // エフェクトを出す間隔
	bool  m_bLoop;       // ループするかしないか
};

//*****************************************************************************
//     エミッタクラスの定義
//*****************************************************************************
class CEmitter : public CScene
{
public:    // 誰でもアクセス可能
	CEmitter(int nPriority = 1, OBJTYPE objType = OBJTYPE_EMITTER);
	~CEmitter();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetType(const int nType);
	void SetTexIdx(const int nTexIdx);
	void SetEffectIdx(const int nEffectIdx);
	void SetRangeMax(const float fRangeMax);
	void SetRangeMin(const float fRangeMin);
	void SetLife(const int nLife);
	void SetLap(const int nLap);
	void SetAppear(const int nAppear);
	void SetLoop(const bool bLoop);

	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	int GetType(void);
	int GetTexIdx(void);
	int GetEffectIdx(void);
	float GetRangeMax(void);
	float GetRangeMin(void);
	int GetLife(void);
	int GetLap(void);
	int GetAppear(void);
	bool GetLoop(void);

protected: // このクラスと派生クラスだけがアクセス可能
	virtual void SetEffect(void) = 0;

private:   // このクラスだけがアクセス可能
	void Movement(void);

	LPDIRECT3DTEXTURE9 m_pTexture;   // テクスチャへのポインタ
	D3DXVECTOR3        m_Pos;        // 座標
	D3DXVECTOR3        m_Rot;        // 向き
	D3DXMATRIX         m_MtxWorld;   // ワールドマトリックス
	int                m_nType;      // 使用するエフェクトの種類
	int                m_nTexIdx;    // 使用するテクスチャ番号
	int                m_nEffectIdx; // 使用するエフェクトの番号
	float              m_fRangeMax;  // エフェクトを生成する範囲の最大値
	float              m_fRangeMin;  // エフェクトを生成する範囲の最小値
	int                m_nLife;      // エミッタの寿命
	int                m_nLap;       // 1回の放出タイミングに出すエフェクトの個数
	int                m_nAppear;    // 放出する間隔
	int                m_nCounter;   // 放出タイミングを管理するカウンター
	bool               m_bLoop;      // ループするかしないか
};

//*****************************************************************************
//     パーティクルエミッタクラスの定義
//*****************************************************************************
class CParEmitter : public CEmitter
{
public:    // 誰でもアクセス可能
	CParEmitter(int nPriority = 1, OBJTYPE objType = OBJTYPE_PAREMITTER);
	~CParEmitter();

	static CParEmitter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, float fRangeMax, float fRangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParData(CParData *pParData);
	CParData *GetParData(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void SetEffect(void);

private:   // このクラスだけがアクセス可能
	CParData  *m_pParData;  // パーティクルデータへのポインタ
};

//*****************************************************************************
//     リングエフェクトエミッタクラスの定義
//*****************************************************************************
class CRingEmitter : public CEmitter
{
public:    // 誰でもアクセス可能
	CRingEmitter(int nPriority = 3, OBJTYPE objType = OBJTYPE_RINGEMITTER);
	~CRingEmitter();

	static CRingEmitter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, float fRangeMax, float fRangeMin, int nLife, int nLap, int nAppear, bool bLoop, CRingData *pRingData, LPDIRECT3DTEXTURE9 pTexture, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRingData(CRingData *pRingData);
	CRingData *GetRingData(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void SetEffect(void);

private:   // このクラスだけがアクセス可能
	CRingData  *m_pRingData;  // リングエフェクトデータへのポインタ
};

#endif