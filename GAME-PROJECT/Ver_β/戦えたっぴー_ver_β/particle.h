//*****************************************************************************
//
//     パーティクルの処理[particle.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************


//*****************************************************************************
//    パーティクルデータクラスの定義
//*****************************************************************************
class CParData
{
public:    // 誰でもアクセス可能
	CParData();
	~CParData();

	void SetRange(const D3DXVECTOR3 Range) { m_Range = Range; }
	void SetRangeMag(const float fRangeMag) { m_fRangeMag = fRangeMag; }
	void SetMaxRot(const float fMaxRot) { m_fMaxRot = fMaxRot; }
	void SetMinRot(const float fMinRot) { m_fMinRot = fMinRot; }
	void SetMaxMove(const D3DXVECTOR3 MaxMove) { m_MaxMove = MaxMove; }
	void SetMinMove(const D3DXVECTOR3 MinMove) { m_MinMove = MinMove; }
	void SetChangeMove(const D3DXVECTOR3 ChangeMove) { m_ChangeMove = ChangeMove; }
	void SetInitCol(const D3DXCOLOR InitCol) { m_InitCol = InitCol; }
	void SetChangeCol(const D3DXCOLOR ChangeCol) { m_ChangeCol = ChangeCol; }
	void SetMaxWidth(const float fMaxWidth) { m_fMaxWidth = fMaxWidth; }
	void SetMinWidth(const float fMinWidth) { m_fMinWidth = fMinWidth; }
	void SetMaxHeight(const float fMaxHeight) { m_fMaxHeight = fMaxHeight; }
	void SetMinHeight(const float fMinHeight) { m_fMinHeight = fMinHeight; }
	void SetSpread(const float fSpread) { m_fSpread = fSpread; }
	void SetMaxLife(const int nMaxLife) { m_nMaxLife = nMaxLife; }
	void SetMinLife(const int nMinLife) { m_nMinLife = nMinLife; }
	void SetGravity(const float fGravity) { m_fGravity = fGravity; }
	void SetBouncing(const float fBouncing) { m_fBouncing = fBouncing; }
	void SetRotPattern(const int nRotPattern) { m_nRotPattern = nRotPattern; }
	void SetRotSpeed(const float fRotSpeed) { m_fRotSpeed = fRotSpeed; }
	void SetCollision(const bool bCollision) { m_bCollision = bCollision; }
	void SetDrawAddtive(const bool bDrawAddtive) { m_bDrawAddtive = bDrawAddtive; }
	void SetLighting(const bool bLighting) { m_bLighting = bLighting; }

	D3DXVECTOR3 GetRange(void) { return m_Range; }
	float GetRangeMag(void) { return m_fRangeMag; }
	float GetMaxRot(void) { return m_fMaxRot; }
	float GetMinRot(void) { return m_fMinRot; }
	D3DXVECTOR3 GetMaxMove(void) { return m_MaxMove; }
	D3DXVECTOR3 GetMinMove(void) { return m_MinMove; }
	D3DXVECTOR3 GetChangeMove(void) { return m_ChangeMove; }
	D3DXCOLOR GetInitCol(void) { return m_InitCol; }
	D3DXCOLOR GetChangeCol(void) { return m_ChangeCol; }
	float GetMaxWidth(void) { return m_fMaxWidth; }
	float GetMinWidth(void) { return m_fMinWidth; }
	float GetMaxHeight(void) { return m_fMaxHeight; }
	float GetMinHeight(void) { return m_fMinHeight; }
	float GetSpread(void) { return m_fSpread; }
	int GetMaxLife(void) { return m_nMaxLife; }
	int GetMinLife(void) { return m_nMinLife; }
	float GetGravity(void) { return m_fGravity; }
	float GetBouncing(void) { return m_fBouncing; }
	int GetRotPattern(void) { return m_nRotPattern; }
	float GetRotSpeed(void) { return m_fRotSpeed; }
	bool GetCollision(void) { return m_bCollision; }
	bool GetDrawAddtive(void) { return m_bDrawAddtive; }
	bool GetLighting(void) { return m_bLighting; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_Range;         // 生成する範囲(オフセット)
	float       m_fRangeMag;     // 生成する範囲の倍率
	float       m_fMaxRot;       // 生成時の向き(最大値)
	float       m_fMinRot;       // 生成時の向き(最小値)
	D3DXVECTOR3 m_MaxMove;       // 移動量の最大値
	D3DXVECTOR3 m_MinMove;       // 移動量の最小値
	D3DXVECTOR3 m_ChangeMove;    // 移動量の変化量
	D3DXCOLOR   m_InitCol;       // 生成時の色
	D3DXCOLOR   m_ChangeCol;     // フレーム毎の色の変化量
	float       m_fMaxWidth;     // 幅の最大値
	float       m_fMinWidth;     // 幅の最小値
	float       m_fMaxHeight;    // 高さの最大値
	float       m_fMinHeight;    // 高さの最小値
	float       m_fSpread;       // 大きさの変化量
	int         m_nMaxLife;      // 寿命の最大値
	int         m_nMinLife;      // 寿命の最小値
	float       m_fGravity;      // 毎フレームかける重力
	float       m_fBouncing;     // バウンド量
	int         m_nRotPattern;   // 回転の種類[ 0:時計回り 1:反時計回り 2:ランダム ]
	float       m_fRotSpeed;     // 回転するスピード
	bool        m_bCollision;    // 当たり判定をするかどうか
	bool        m_bDrawAddtive;  // 加算合成するかしないか[ 0:なし 1:あり ]
	bool        m_bLighting;     // ライティングするかしないか[ 0:なし 1:あり ]
};

//*****************************************************************************
//    パーティクルクラスの定義
//*****************************************************************************
class CParticle : public CSceneBillboard
{
public:    // 誰でもアクセス可能
	CParticle(int nPriority = 3, OBJTYPE objType = OBJTYPE_PARTICLE);
	~CParticle();

	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col, float fWidth,
		float fHeight, float fRot, D3DXCOLOR ChangeCol, float fGravity, float fSpread,
		float fChangeRot, float fBouncing, int nLife, int nRotPattern,
		bool bCol, bool bDrawAddtive, bool bLighting, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
		float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fGravity, float fSpread,
		float fChangeRot, float fBouncing, int nLife, int nRotPattern,
		bool bCol, bool bDrawAddtive, bool bLighting);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void ChangeVertex(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fLength);

	void SetMove(const D3DXVECTOR3 move);
	void SetChangeMove(const D3DXVECTOR3 ChangeMove);
	void SetChangeCol(const D3DXCOLOR ChangeCol);
	void SetLife(const int nLife);
	void SetRotPattern(const int nRotPattern);
	void SetGravity(const float fGravity);
	void SetSpread(const float fSpread);
	void SetChangeRot(const float fChangeRot);
	void SetBouncing(const float fBouncing);
	void SetCollision(const bool bCollision);
	void SetDrawAddtive(const bool bDrawAddtive);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetChangeMove(void);
	D3DXCOLOR GetChangeCol(void);
	int GetLife(void);
	int GetRotPattern(void);
	float GetGravity(void);
	float GetSpread(void);
	float GetChangeRot(void);
	float GetBouncing(void);
	bool GetCollision(void);
	bool GetDrawAddtive(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_Move;           // 移動量
	D3DXVECTOR3 m_ChangeMove;     // 移動量の変化量
	D3DXCOLOR   m_ChangeCol;      // 色の変化量
	int         m_nLife;          // 寿命
	int         m_nRotPattern;    // 回転の種類
	float       m_fGravity;       // 重力
	float       m_fSpread;        // 大きさの変化量
	float       m_fChangeRot;     // 回転の変化量
	float       m_fBouncing;      // バウンド量
	bool        m_bCollision;     // 当たり判定を取るかどうか
	bool        m_bDrawAddtive;   // 加算合成で描画するかしないか
};

#endif