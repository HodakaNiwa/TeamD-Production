//*****************************************************************************
//
//     エフェクト管轄の処理[effectManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MAX_EMMITER         (150)     // エミッタデータの最大数
#define MAX_PARTICLEDATA    (150)     // パーティクルデータの最大数
#define MAX_RINGEFFECTDATA  (150)     // リングエフェクトデータの最大数

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CFileLoader;
class CFileSaver;
class CParData;
class CEmitterData;
class CRingData;
class CTextureManager;
class CEmitter;

//*****************************************************************************
//     エフェクトマネージャークラスの定義
//*****************************************************************************
class CEffectManager : CScene
{
public:   // 誰でもアクセス可能
	typedef enum
	{
		EFFECT_TYPE_PARTICLE = 0,  // パーティクル
		EFFECT_TYPE_RING,          // リング
		EFFECT_TYPE_MAX
	}EFFECT_TYPE;

	CEffectManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_EFFECTMANAGER);
	~CEffectManager();

	static CEffectManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CEmitter *SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

	void SetFileName(char *pFileName);
	void SetEmitter(CEmitter *pEmitter);
	void SetEmitterData(CEmitterData *pEmitterData, const int nIdx);
	void SetParData(CParData *pParData, const int nIdx);
	void SetRingEffectData(CRingData *pRingData, const int nIdx);
	void SetTexManager(CTextureManager *pTexManager);
	void SetNumEmitterData(const int nNumEmitterData);
	void SetNumParData(const int nNumParData);
	void SetNumRingEffectData(const int nNumRingEffectData);

	CEmitter *GetEmitter(void);
	LPDIRECT3DTEXTURE9 GetTexture(const int nIdx);
	CEmitterData **GetEmitterData(void);
	CEmitterData *GetEmitterData(const int nIdx);
	CParData **GetParData(void);
	CParData *GetParData(const int nIdx);
	CRingData **GetRingEffectData(void);
	CRingData *GetRingEffectData(const int nIdx);
	CTextureManager *GetTexManager(void);
	int GetNumEmitterData(void);
	int GetNumParData(void);
	int GetNumRingEffectData(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateTextureManager(const int nNumTex);
	void CreateEmitterDataMemory(const int nNumEmitterData);
	void CreateParticleDataMemory(const int nNumParticleData);
	void CreateRingEffectDataMemory(const int nNumRingEffectData);

	void ReleaseTextureManager(void);
	void ReleaseEmitterData(void);
	void ReleaseParticleData(void);
	void ReleaseRingEffectData(void);

	HRESULT LoadScript(CFileLoader *pFileLoader, char *pStr);
	void LoadTexture(char *pStr, const int nCntTex);
	CEmitterData *LoadEmitterData(CFileLoader *pFileLoader, char *pStr, const int nCntEmitter);
	CParData *LoadParticleData(CFileLoader *pFileLoader, char *pStr, const int nCntParData);
	CRingData *LoadRingEffectData(CFileLoader *pFileLoader, char *pStr, const int nCntRingData);

	void Save(void);
	void SaveTextureData(CFileSaver *pFileSaver);
	void SaveEmitterData(CFileSaver *pFileSaver);
	void SaveParticleData(CFileSaver *pFileSaver);
	void SaveRingEffectData(CFileSaver *pFileSaver);

	char            m_aFileName[256];         // スクリプトファイル名
	CEmitter        *m_pEmitter;              // 最後に生成したエミッタクラスへのポインタ
	int             m_nNumEmitterData;        // エミッタデータの個数
	int             m_nNumParData;            // パーティクルデータの個数
	int             m_nNumRingEffectData;     // リングエフェクトデータの個数
	CEmitterData    **m_apEmitterData;        // エミッタデータクラスへのポインタ
	CParData        **m_apParData;            // パーティクルデータクラスへのポインタ
	CRingData       **m_apRingData;           // リングエフェクトデータクラスへのポインタ
	CTextureManager *m_pTextureManager;       // テクスチャ管理クラスへのポインタ
};

#endif