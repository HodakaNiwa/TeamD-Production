//*****************************************************************************
//
//     リングエフェクトの処理[ringEffect.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RINGEFFECT_H_
#define _RINGEFFECT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "meshRing.h"

//*****************************************************************************
//    リングエフェクトデータクラスの定義
//*****************************************************************************
class CRingData
{
public:    // 誰でもアクセス可能
	CRingData();
	~CRingData();

	void SetMaxRot(const D3DXVECTOR3 MaxRot) { m_MaxRot = MaxRot; }
	void SetMinRot(const D3DXVECTOR3 MinRot) { m_MinRot = MinRot; }
	void SetMaxMove(const D3DXVECTOR3 MaxMove) { m_MaxMove = MaxMove; }
	void SetMinMove(const D3DXVECTOR3 MinMove) { m_MinMove = MinMove; }
	void SetChangeMove(const D3DXVECTOR3 ChangeMove) { m_ChangeMove = ChangeMove; }
	void SetInitCol(const D3DXCOLOR InitCol) { m_InitCol = InitCol; }
	void SetChangeCol(const D3DXCOLOR ChangeCol) { m_ChangeCol = ChangeCol; }
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }
	void SetDiffusion(const float fDiffusion) { m_fDiffusion = fDiffusion; }
	void SetChangeHeight(const float fChangeHeight) { m_fChangeHeight = fChangeHeight; }
	void SetChangeRadius(const float fChangeRadius) { m_fChangeRadius = fChangeRadius; }
	void SetChangeDiffusion(const float fChangeDiffusion) { m_fChangeDiffusion = fChangeDiffusion; }
	void SetMaxLife(const int nMaxLife) { m_nMaxLife = nMaxLife; }
	void SetMinLife(const int nMinLife) { m_nMinLife = nMinLife; }
	void SetXBlock(const int nXBlock) { m_nXBlock = nXBlock; }
	void SetYBlock(const int nYBlock) { m_nYBlock = nYBlock; }
	void SetRotPattern(const int nRotPattern) { m_nRotPattern = nRotPattern; }
	void SetRotSpeed(const D3DXVECTOR3 RotSpeed) { m_RotSpeed = RotSpeed; }
	void SetCulling(const bool bCulling) { m_bCulling = bCulling; }
	void SetDrawAddtive(const bool bDrawAddtive) { m_bDrawAddtive = bDrawAddtive; }

	D3DXVECTOR3 GetMaxRot(void) { return m_MaxRot; }
	D3DXVECTOR3 GetMinRot(void) { return m_MinRot; }
	D3DXVECTOR3 GetMaxMove(void) { return m_MaxMove; }
	D3DXVECTOR3 GetMinMove(void) { return m_MinMove; }
	D3DXVECTOR3 GetChangeMove(void) { return m_ChangeMove; }
	D3DXCOLOR GetInitCol(void) { return m_InitCol; }
	D3DXCOLOR GetChangeCol(void) { return m_ChangeCol; }
	float GetHeight(void) { return m_fHeight; }
	float GetRadius(void) { return m_fRadius; }
	float GetDiffusion(void) { return m_fDiffusion; }
	float GetChangeHeight(void) { return m_fChangeHeight; }
	float GetChangeRadius(void) { return m_fChangeRadius; }
	float GetChangeDiffusion(void) { return m_fChangeDiffusion; }
	int GetMaxLife(void) { return m_nMaxLife; }
	int GetMinLife(void) { return m_nMinLife; }
	int GetXBlock(void) { return m_nXBlock; }
	int GetYBlock(void) { return m_nYBlock; }
	int GetRotPattern(void) { return m_nRotPattern; }
	D3DXVECTOR3 GetRotSpeed(void) { return m_RotSpeed; }
	bool GetCulling(void) { return m_bCulling; }
	bool GetDrawAddtive(void) { return m_bDrawAddtive; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_MaxRot;            // 向きの最大値
	D3DXVECTOR3 m_MinRot;            // 向きの最小値
	D3DXVECTOR3 m_MaxMove;           // 移動量の最大値
	D3DXVECTOR3 m_MinMove;           // 移動量の最小値
	D3DXVECTOR3 m_ChangeMove;        // 移動量の変化量
	D3DXCOLOR   m_InitCol;           // 生成時の色
	D3DXCOLOR   m_ChangeCol;         // フレーム毎の色の変化量
	float       m_fHeight;           // 高さ
	float       m_fRadius;           // 半径
	float       m_fDiffusion;        // 幅
	float       m_fChangeHeight;     // 高さを毎フレームどれくらい変化させるか
	float       m_fChangeRadius;     // 半径をどれくらい毎フレーム変化させるか
	float       m_fChangeDiffusion;  // リングの幅をどれくらい毎フレーム変化させるか
	int         m_nXBlock;           // 横の分割数
	int         m_nYBlock;           // 縦の分割数
	int         m_nMaxLife;          // 寿命の最大値
	int         m_nMinLife;          // 寿命の最小値
	int         m_nRotPattern;       // 回転の種類[ 0:時計回り 1:反時計回り 2:ランダム ]
	D3DXVECTOR3 m_RotSpeed;          // 回転するスピード
	bool        m_bCulling;          // カリングするかどうか[ 0:なし 1:あり ]
	bool        m_bDrawAddtive;      // 加算合成するかしないか[ 0:なし 1:あり ]
};

//*****************************************************************************
//    リングエフェクトクラスの定義
//*****************************************************************************
class CRingEffect : public CMeshRing
{
public:    // 誰でもアクセス可能
	CRingEffect(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHRING);
	~CRingEffect();
	static CRingEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
		int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
		float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
		int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
		float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeVertex(void);

	void SetLife(const int nLife);
	void SetRotSpeed(const D3DXVECTOR3 RotSpeed);
	void SetRotPattern(const int nRotPattern);
	void SetMove(const D3DXVECTOR3 Move);
	void SetChangeMove(const D3DXVECTOR3 ChangeMove);
	void SetChangeCol(const D3DXCOLOR ChangeCol);
	void SetChangeHeight(const float fChangeHeight);
	void SetChangeRadius(const float fChangeRadisu);
	void SetChangeDiffusion(const float fChangeDiffusion);
	void SetCulling(const bool bCulling);
	void SetDrawAddtive(const bool bDrawAddtive);

	int GetLife(void);
	D3DXVECTOR3 GetRotSpeed(void);
	int GetRotPattern(void);
	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetChangeMove(void);
	D3DXCOLOR GetChangeCol(void);
	float GetChangeHeight(void);
	float GetChangeRadius(void);
	float GetChangeDiffusion(void);
	bool GetCulling(void);
	bool GetDrawAddtive(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int         m_nLife;             // 寿命
	D3DXVECTOR3 m_RotSpeed;          // 回転量
	int         m_nRotPattern;       // 回転のパターン
	D3DXVECTOR3 m_Move;              // 移動量
	D3DXVECTOR3 m_ChangeMove;        // 移動量を毎フレームどれくらい変化させるか
	D3DXCOLOR   m_ChangeCol;         // 色を毎フレームどれくらい変化させるか
	float       m_fChangeHeight;     // 高さを毎フレームどれくらい変化させるか
	float       m_fChangeRadius;     // 半径をどれくらい毎フレーム変化させるか
	float       m_fChangeDiffusion;  // リングの幅をどれくらい毎フレーム変化させるか
	bool        m_bCulling;          // カリングするかどうか
	bool        m_bDrawAddtive;      // 加算合成で描画するかしないか
};

#endif