//*****************************************************************************
//
//     キャラクター管轄の処理[characterManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CHARACTERMANAGER_H_
#define _CHARACTERMANAGER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CFileLoader;
class CMotionAttack;
class CMotionCollision;
class CMotionOrbit;
class CMotion;
class CMotionManager;
class CModelCreate;
class CModel;
class CTextureManager;
class CObject;
class CEnemy;

//*****************************************************************************
//    キャラクター管轄クラスの定義
//*****************************************************************************
class CCharacterManager
{
public:    // 誰でもアクセス可能
	CCharacterManager();
	~CCharacterManager();

	static CCharacterManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	CObject *SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);
	CEnemy *SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

	void SetFileName(char *pFileName);
	void SetModelCreate(CModelCreate *pModelCreate);

	char *GetFileName(void);
	CModelCreate *GetModelCreate(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	HRESULT LoadScript(CFileLoader *pFileLoader, char *pStr);
	HRESULT LoadTextureSet(CFileLoader *pFileLoader, char *pStr);
	HRESULT LoadTexture(char *pStr, int nCntTex);
	HRESULT LoadModelSet(CFileLoader *pFileLoader, char *pStr);
	HRESULT LoadModel(char *pStr, int nCntModel);
	HRESULT LoadCharacterSet(CFileLoader *pFileLoader, char *pStr);
	HRESULT LoadPartsSet(CFileLoader *pFileLoader, char *pStr, int nCntParts);
	HRESULT LoadMotionSet(CFileLoader *pFileLoader, char *pStr, CMotion **apMotion);
	CMotionAttack *LoadMotionAttackSet(CFileLoader *pFileLoader, char *pStr);
	CMotionCollision **LoadMotionCollisionSet(CFileLoader *pFileLoader, char *pStr, int *pNumColData);
	CMotionOrbit **LoadMotionOrbitSet(CFileLoader *pFileLoader, char *pStr, int *pNumOrbitData);
	HRESULT LoadKeySet(CFileLoader *pFileLoader, char *pStr, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	HRESULT LoadKey(CFileLoader *pFileLoader, char *pStr, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char            m_aFileName[256];    // キャラクターデータのスクリプトファイル名
	int             m_nNumMotionData;    // 読み込んだモーションデータの数
	int             m_nNumModel;         // 読み込むモデル数
	int             m_nNumTexture;       // 読み込むテクスチャの数
	int             *m_pParent;          // 親モデルの番号(後で配置するために使う)
	CModel          **m_apModel;         // モデルクラスへのポインタ
	CMotionManager  *m_pMotionManager;   // モーション管理クラスへのポインタ
	CModelCreate    *m_pModelCreate;     // モデル管轄クラスへのポインタ
	CTextureManager *m_pTextureManager;  // テクスチャ管轄クラスへのポインタ

	typedef struct
	{
		float fAccel;
		float fInertia;
		float fRivisionRot;
		int nNumParts;
	}CHARA_DATA;
	CHARA_DATA      m_CharaData;         // キャラクターデータ
};

#endif