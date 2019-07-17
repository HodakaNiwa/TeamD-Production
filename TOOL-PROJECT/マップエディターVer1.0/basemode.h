//=============================================================================
//
// ベースモードの処理[basemode.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _BASEMODE_H_
#define _BASEMODE_H_

#include "main.h"

//前方宣言
class CMap;
class CCameraManager;
class CModelCreate;
class CTextureManager;
class CEffectManager;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBasemode
{
public:	//誰からもアクセス可能
		//メンバ関数
	CBasemode();
	~CBasemode();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetMap(CMap *pMap);
	void SetCameraManager(CCameraManager *pCameraManager);
	void SetModelCreate(CModelCreate *pModelCreate);
	void SetTextureManager(CTextureManager *pTextureManager);
	void SetEffectManager(CEffectManager *pEffectManager);

	CMap *GetMap(void);
	CCameraManager *GetCameraManager(void);
	CModelCreate *GetModelCreate(void);
	CTextureManager *GetTextureManager(void);
	CEffectManager *GetEffectManager(void);

private:	// 自分だけがアクセス可能
	CMap *m_pMap;			            // マップのポインタ
	CCameraManager *m_pCameraManager;   // カメラ管轄クラスへのポインタ
	CModelCreate   *m_pModelCreate;     // モデル管轄クラスへのポインタ
	CTextureManager *m_pTextureManager; // テクスチャ管轄クラスへのポインタ
	CEffectManager *m_pEffectManager;   // エフェクト管轄クラスへのポインタ
};
#endif