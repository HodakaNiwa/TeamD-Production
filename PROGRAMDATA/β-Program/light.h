//*****************************************************************************
//
//     ライトの処理[light.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    ライトクラスの定義
//*****************************************************************************
class CLight
{
public:    // 誰でもアクセス可能
	typedef enum
	{
		TYPE_NONE = -1,    // 無し
		TYPE_DIRECTIONAL,  // ディレクショナル
		TYPE_POINT,        // ポイント
		TYPE_SPOT,         // スポット
		TYPE_MAX
	}TYPE;

	static CLight *ChangeLightType(TYPE type);

	CLight();
	virtual ~CLight();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;

	void SetType(const TYPE type);
	void SetLight(const D3DLIGHT9 Light);
	void SetSwitch(const bool bSwitch);

	TYPE GetType(void);
	D3DLIGHT9 GetLight(void);
	bool GetSwitch(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	TYPE      m_Type;    // ライトの種類
	D3DLIGHT9 m_Light;   // ライトの情報
	bool      m_bSwitch; // ライトがONになっているかどうか
};

//*****************************************************************************
//    ディレクショナルライトクラスの定義
//*****************************************************************************
class CDirectionalLight : public CLight
{
public:    // 誰でもアクセス可能
	CDirectionalLight();
	~CDirectionalLight();

	static CDirectionalLight *Create(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, bool bSwitch = true);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateLight(D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, bool bSwitch);
};

//*****************************************************************************
//    ポイントライトクラスの定義
//*****************************************************************************
class CPointLight : public CLight
{
public:    // 誰でもアクセス可能
	CPointLight();
	~CPointLight();

	static CPointLight *Create(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, bool bSwitch = true);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateLight(D3DXVECTOR3 pos, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, bool bSwitch);
};

//*****************************************************************************
//    スポットライトクラスの定義
//*****************************************************************************
class CSpotLight : public CLight
{
public:    // 誰でもアクセス可能
	CSpotLight();
	~CSpotLight();

	static CSpotLight *Create(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch = true);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateLight(D3DXVECTOR3 pos, D3DXVECTOR3 Dir, D3DXCOLOR Diffuse, D3DXCOLOR Ambient, D3DXCOLOR Specular, float fAttenuation0, float fAttenuation1, float fAttenuation2, float fRange, float fFalloff, float fTheta, float fPhi, bool bSwitch);
};

#endif