//*****************************************************************************
//
//     モーションの処理[motion.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
//    モーション用攻撃モーションデータクラスの定義
//*****************************************************************************
class CMotionAttack
{
public:    // 誰でもアクセス可能
	CMotionAttack();
	~CMotionAttack();

	void SetNextMotionIdx(const int nNextMotionIdx) { m_nNextMotionIdx = nNextMotionIdx; }
	void SetStartTiming(const int nStartTiming) { m_nStartTiming = nStartTiming; }
	void SetFinishTiming(const int nFinishTiming) { m_nFinishTiming = nFinishTiming; }

	int GetNextMotionIdx(void) { return m_nNextMotionIdx; }
	int GetStartTiming(void) { return m_nStartTiming; }
	int GetFinishTiming(void) { return m_nFinishTiming; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int m_nNextMotionIdx;    // 次のモーションの番号
	int m_nStartTiming;      // 次のモーションに派生できる時間が始まるタイミング
	int m_nFinishTiming;     // 次のモーションに派生できる時間が終わるタイミング
};

//*****************************************************************************
//    モーション用当たり判定データクラスの定義
//*****************************************************************************
class CMotionCollision
{
public:    // 誰でもアクセス可能
	CMotionCollision();
	~CMotionCollision();

	void SetModelIdx(const int nModelIdx) { m_nModelIdx = nModelIdx; }
	void SetOffsetPos(const D3DXVECTOR3 OffsetPos) { m_OffSetPos = OffsetPos; }
	void SetRange(const float fRange) { m_fRange = fRange; }
	void SetStartTiming(const int nStartTiming) { m_nStartTiming = nStartTiming; }
	void SetFinishTiming(const int nFinishTiming) { m_nFinishTiming = nFinishTiming; }
	void SetDamage(const int nDamage) { m_nDamage = nDamage; }

	int GetModelIdx(void) { return m_nModelIdx; }
	D3DXVECTOR3 GetOffsetPos(void) { return m_OffSetPos; }
	float GetRange(void) { return m_fRange; }
	int GetStartTiming(void) { return m_nStartTiming; }
	int GetFinishTiming(void) { return m_nFinishTiming; }
	int GetDamage(void) { return m_nDamage; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int         m_nModelIdx;      // 当たり判定を取るモデルの番号
	D3DXVECTOR3 m_OffSetPos;      // 当たり判定を取るモデルからのオフセット距離
	float       m_fRange;         // 当たり判定を取る範囲
	int         m_nStartTiming;   // 当たり判定をし始めるタイミング
	int         m_nFinishTiming;  // 当たり判定をなくすタイミング
	int         m_nDamage;        // 当たった場合のダメージ量
};

//*****************************************************************************
//    モーション用軌跡データクラスの定義
//*****************************************************************************
class CMotionOrbit
{
public:    // 誰でもアクセス可能
	CMotionOrbit();
	~CMotionOrbit();

	void SetModelIdx(const int nModelIdx) { m_nModelIdx = nModelIdx; }
	void SetTexIdx(const int nTexIdx) { m_nTexIdx = nTexIdx; }
	void SetOffsetPos1(const D3DXVECTOR3 OffsetPos) { m_OffSetPos1 = OffsetPos; }
	void SetOffsetPos2(const D3DXVECTOR3 OffsetPos) { m_OffSetPos2 = OffsetPos; }
	void SetOffsetAmp1(const D3DXVECTOR3 OffsetAmp) { m_OffSetAmp1 = OffsetAmp; }
	void SetOffsetAmp2(const D3DXVECTOR3 OffsetAmp) { m_OffSetAmp2 = OffsetAmp; }
	void SetColUp(const D3DXCOLOR ColUp) { m_ColUp = ColUp; }
	void SetColDown(const D3DXCOLOR ColDown) { m_ColDown = ColDown; }
	void SetAlphaDecayUp(const float fAlphaDecayUp) { m_fAlphaDecayUp = fAlphaDecayUp; }
	void SetAlphaDecayDown(const float fAlphaDecayDown) { m_fAlphaDecayDown = fAlphaDecayDown; }
	void SetXBlock(const int nXBlock) { m_nXBlock = nXBlock; }
	void SetYBlock(const int nYBlock) { m_nYBlock = nYBlock; }
	void SetStartTiming(const int nStartTiming) { m_nStartTiming = nStartTiming; }
	void SetFinishTiming(const int nFinishTiming) { m_nFinishTiming = nFinishTiming; }

	int GetModelIdx(void) { return m_nModelIdx; }
	int GetTexIdx(void) { return m_nTexIdx; }
	D3DXVECTOR3 GetOffsetPos1(void) { return m_OffSetPos1; }
	D3DXVECTOR3 GetOffsetPos2(void) { return m_OffSetPos2; }
	D3DXVECTOR3 GetOffsetAmp1(void) { return m_OffSetAmp1; }
	D3DXVECTOR3 GetOffsetAmp2(void) { return m_OffSetAmp2; }
	D3DXCOLOR GetColUp(void) { return m_ColUp; }
	D3DXCOLOR GetColDown(void) { return m_ColDown; }
	float GetAlphaDecayUp(void) { return m_fAlphaDecayUp; }
	float GetAlphaDecayDown(void) { return m_fAlphaDecayDown; }
	int GetXBlock(void) { return m_nXBlock; }
	int GetYBlock(void) { return m_nYBlock; }
	int GetStartTiming(void) { return m_nStartTiming; }
	int GetFinishTiming(void) { return m_nFinishTiming; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int         m_nModelIdx;      // 軌跡を出すモデルの番号
	int         m_nTexIdx;        // 軌跡に貼るテクスチャの番号
	D3DXVECTOR3 m_OffSetPos1;     // 軌跡を出すモデルからのオフセット距離(1番目)
	D3DXVECTOR3 m_OffSetPos2;     // 軌跡を出すモデルからのオフセット距離(2番目)
	D3DXVECTOR3 m_OffSetAmp1;     // 軌跡のオフセット増幅値(1番目)
	D3DXVECTOR3 m_OffSetAmp2;     // 軌跡のオフセット増幅値(2番目)
	D3DXCOLOR   m_ColUp;          // 軌跡の頂点上側の色
	D3DXCOLOR   m_ColDown;        // 軌跡の頂点下側の色
	float       m_fAlphaDecayUp;  // 軌跡の頂点上側の透明度減衰値
	float       m_fAlphaDecayDown;// 軌跡の頂点上側の透明度減衰値
	int         m_nXBlock;        // 軌跡の横の分割数
	int         m_nYBlock;        // 軌跡の縦の分割数
	int         m_nStartTiming;   // 軌跡を出し始めるタイミング
	int         m_nFinishTiming;  // 軌跡をしまい始めるタイミング
};

//*****************************************************************************
//    キーフレームクラスの定義
//*****************************************************************************
class CKeyframe
{
public:    // 誰でもアクセス可能
	CKeyframe();
	~CKeyframe();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetAddPos(D3DXVECTOR3 pos);
	void SetDestPos(D3DXVECTOR3 pos);
	void SetDestRot(D3DXVECTOR3 rot);
	void SetDiffRot(D3DXVECTOR3 rotDiff);

	D3DXVECTOR3 GetAddPos(void);
	D3DXVECTOR3 GetDestPos(void);
	D3DXVECTOR3 GetDestRot(void);
	D3DXVECTOR3 GetDiffRot(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_AddPos;       // 現在のオフセット位置に加える値
	D3DXVECTOR3 m_DestPos;      // 目的のオフセット位置
	D3DXVECTOR3 m_DestRot;      // 目的のオフセット向き
	D3DXVECTOR3 m_DiffRot;      // 差分の向き
};

//*****************************************************************************
//    モーションクラスの定義
//*****************************************************************************
class CMotion
{
public:    // 誰でもアクセス可能
	//------------------------
	//  モーションの状態
	//------------------------
	typedef enum
	{
		STATE_NORMAL = 0,  // 通常状態
		STATE_STOP,        // 停止状態
		STATE_SWITCH,      // 切り替え状態
		STATE_MAX
	}STATE;

	CMotion();
	~CMotion();

	static CMotion *Create(int nNumModel, int nNumKey, float fBlendMag, bool bLoop, int *pPlaybackKey);
	HRESULT Init(int nNumModel, int nNumKey, float fBlendMag, bool bLoop, int *pPlaybackKey);
	void Uninit(int nNumModel);
	void Update(CModel **pModel, int nNumModel, int *pKey, int *pCounter, int *pBlendCounter, int *pAllCounter);

	CMotionAttack *GetAttack(void);
	CMotionCollision **GetCollision(void);
	int GetNumColData(void);
	CMotionOrbit **GetOrbit(void);
	int GetNumOrbitData(void);
	CKeyframe ***GetKeyframe(void);
	STATE GetState(void);
	int GetNumKey(void);
	bool GetLoop(void);
	int GetPlayBackKey(int nIdx);
	int *GetPlayBackKey(void);
	int GetAllPlayBackKey(void);
	float GetBlendMag(void);

	void SetAttack(CMotionAttack *pAttack);
	void SetCollision(CMotionCollision **pCollision);
	void SetNumColData(const int nNumColData);
	void SetOrbit(CMotionOrbit **pOrbit);
	void SetNumOrbitData(const int nNumOrbitData);
	void SetKeyFrame(const int nIdxModel, const int nIdxKey, CKeyframe *pKeyFrame);
	void SetState(STATE state);
	void SetNumKey(const int nNumKey);
	void SetLoop(const bool bLoop);
	void SetPlayBackKey(const int nPlayBackKey, int nIdx);
	void SetPlayBackKey(int *pPlayBackKey);
	void SetAllPlayBackKey(int nAllPlayBackKey);
	void SetBlendMag(const float fBlendMag);
	void SwitchKey(CModel **pModel, int nNumModel, int nKey);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CMotionAttack    *m_pMotionAttack;    // モーション攻撃モーションデータクラスへのポインタ
	CMotionCollision **m_pCollision;      // モーションの当たり判定データクラスへのポインタ
	int              m_nNumColData;       // 当たり判定データの数
	CMotionOrbit     **m_pOrbit;          // モーションの軌跡データクラスへのポインタ
	int              m_nNumOrbitData;     // 軌跡データの数
	CKeyframe        ***m_pKeyframe;      // キーフレームの情報(そのモーションのキーフレームの数とモデルの数分動的に確保)
	STATE            m_State;             // モーションの状態
	int              *m_pPlaybackKey;     // 再生フレームの総数
	int              m_nAllPlaybackKey;   // 再生フレームの総数
	int              m_nNumKey;           // キーフレームの数
	float            m_fBlendMag;         // モーションブレンド倍率
	bool             m_bLoop;             // ループするかしないか
};

//*****************************************************************************
//    モーション管理クラスの定義
//*****************************************************************************
class CMotionManager
{
public:    // 誰でもアクセス可能
	CMotionManager();
	~CMotionManager();

	static CMotionManager *Create(int nNumMotion, int nNumModel);
	HRESULT Init(int nNumMotion, int nNumModel);
	void Uninit(void);
	void Update(CModel **pModel);

	void SetMotion(CMotion *pMotion, int nIdx);
	void SetKey(int nKey);
	void SetAllCounter(int nAllCounter);
	void SetNumber(int nNumber);

	CMotion **GetMotion(void);
	int GetKey(void);
	int GetCounter(void);
	int GetAllCounter(void);
	int GetNumModel(void);
	int GetNumber(void);

	void SwitchMotion(CModel **pModel, int nNumber);

	void Save(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CMotion **m_pMotion;     // モーションクラスへのポインタ
	int     m_nKey;          // 現在のキーフレーム
	int     m_nCounter;      // 現在の再生フレーム数
	int     m_nAllCounter;   // 現在の総再生フレーム数
	int     m_nBlendCounter; // ブレンドを管理するカウンター
	int     m_nNumAll;       // モーションの総数
	int     m_nNumber;       // 現在のモーション番号
	int     m_nNumberOld;    // 前回のモーション番号
	int     m_nNumModel;     // モデルの数
};

#endif