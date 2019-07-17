//*****************************************************************************
//
//     飛沫の処理[splash.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SPLASH_H_
#define _SPLASH_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    飛沫クラスの定義
//*****************************************************************************
class CSplash : public CScene2D
{
public:    // 誰でもアクセス可能
	//------------------------
	//  状態
	//------------------------
	typedef enum
	{
		STATE_APPEAR = 0,  // 出現
		STATE_NORMAL,      // 通常
		STATE_VANISH,      // 消える
		STATE_MAX
	}STATE;

	CSplash(int nPriority = 3, OBJTYPE objType = OBJTYPE_SPLASH);
	~CSplash();

	static CSplash *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nTexIdx, D3DXVECTOR3 StartMove, float fPosYDown, float fRollSpeed, float fLengthMax, float fLengthTransition, int nLife, int nPriority = 3, float fRot = 0.0f, float fStartTexU = 0.0f, float fStartTexV = 0.0f, float fAddTexU = 1.0f, float fAddTexV = 1.0f);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE state);
	void SetTexIdx(const int nTexIdx);
	void SetStartMove(const D3DXVECTOR3 StartMove);
	void SetPosYDown(const float fPosYDown);
	void SetRollSpeed(const float fRollSpeed);
	void SetLengthMax(const float fLengthMax);
	void SetLengthTransition(const float fLengthTransition);
	void SetLife(const int nLife);
	void SetCounter(const int nCounter);

	STATE GetState(void);
	int GetTexIdx(void);
	D3DXVECTOR3 GetMove(void);
	float GetPosYDown(void);
	float GetRollSpeed(void);
	float GetLengthMax(void);
	float GetLengthTransition(void);
	int GetLife(void);
	int GetCounter(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void AppearUpdate(void);
	void NormalUpdate(void);
	void VanishUpdate(void);

	STATE m_State;              // 状態
	int m_nTexIdx;              // 使用するテクスチャの番号
	D3DXVECTOR3 m_StartMove;    // 飛沫ポリゴンが出現したときの移動量
	float m_fPosYDown;          // 飛沫ポリゴンが下がるときの移動量
	float m_fRollSpeed;         // 飛沫ポリゴンの回転スピード
	float m_fLengthMax;         // 飛沫ポリゴンの最大の大きさ
	float m_fLengthTransition;  // 飛沫ポリゴンの大きさ変化量
	int   m_nLife;              // 寿命
	int   m_nCounter;           // 飛沫の大きさ変化を管理するカウンター

};

#endif