//=============================================================================
//
// エディターの処理 [editor.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "main.h"
#include "basemode.h"

//前方宣言
class CMeshField;
class CMeshCollider;
class CGrid;
class C3DPolygon;
class CObject3D;
class CBlock;
class CRiver;
class CIceField;
class CObject;
class CBillboardObject;
class CEmitter;
class CSceneBillboard;
class CFileLoader;
class CFileSaver;
class CModelCreate;
class CUI;

//*****************************************************************************
// ベースオブジェクトクラス定義
//*****************************************************************************
class CBaseObject
{
public:    // 誰でもアクセス可能
	CBaseObject();
	~CBaseObject();

	static CBaseObject *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void SwitchObject(int nObjMode);
	void SetValueToObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetValueToBillboardObj(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, bool bLighting, bool bDrawAddtive, int nTexIdx);
	void SetValueToEmitter(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	CObject *GetObj(void);
	CBillboardObject *GetBillboardObj(void);
	CEmitter *GetEmitter(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateObject(void);
	void CreateBillboardObj(void);
	void CreateEmitter(void);

	CObject *m_pObject;                 // 配置物クラスへのポインタ
	CBillboardObject *m_pBillboardObj;  // ビルボードオブジェクトクラスへのポインタ
	CEmitter *m_pEmitter;               // エミッタクラスへのポインタ
	CSceneBillboard *m_pBill;           // エミッタの座標を表示するためのビルボード
};

//*****************************************************************************
// エディタークラス定義
//*****************************************************************************
class CEditor : public CBasemode
{
public:	//誰からもアクセス可能
	//-------------------
	//  生成モード
	//-------------------
	typedef enum
	{
		MODE_CREATE = 0,
		MODE_DESTROY,
		MODE_MAX
	}MODE;

	//-------------------
	//  編集モード
	//-------------------
	typedef enum
	{
		EDITMODE_GAMEFIELD = 0,   // ゲームフィールド
		EDITMODE_LIGHT,           // ライト
		EDITMODE_OBJECT,          // 配置物
		EDITMODE_RESPAWN,         // リスポーン位置
		EDITMODE_HEADQUARTERS,    // 司令部
		EDITMODE_ENEMYLIST,       // 敵の生成情報
		EDITMODE_SKY,             // 空
		EDITMODE_MAX
	}EDITMODE;

	//-------------------
	//  カメラモード
	//-------------------
	typedef enum
	{
		CAMERAMODE_GAME = 0,   // ゲームのカメラ(固定カメラ)
		CAMERAMODE_EDIT,       // エディターのカメラ(動かせるカメラ)
		CAMERAMODE_TOP,        // トップビューのカメラ(固定カメラ)
		CAMERAMODE_MAX
	}CAMERAMODE;

	//-------------------------------
	//  配置するゲームフィールドの種類
	//-------------------------------
	typedef enum
	{
		GAMEFIELDMODE_BLOCK = 0, // ブロック
		GAMEFIELDMODE_RIVER,     // 川
		GAMEFIELDMODE_ICE,       // 氷
		GAMEFIELDMODE_MAX
	}GAMEFIELDMODE;

	//-------------------------------
	//  配置するオブジェクトの種類
	//-------------------------------
	typedef enum
	{
		OBJECTMODE_MODEL = 0,   // モデル
		OBJECTMODE_BILLBOARD,   // ビルボード
		OBJECTMODE_EFFECT,      // エフェクト
		OBJECTMODE_MAX
	}OBJECTMODE;

	//メンバ関数
	CEditor();
	~CEditor();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ExChangeArea(void);
	void SetRespawnModel(void);
	void SetMode(const MODE mode);
	void SetEditMode(const EDITMODE editMode);
	void SetCameraMode(const CAMERAMODE cameraMode);
	void SetGameFieldMode(const GAMEFIELDMODE gameFieldMode);
	void SetObjectMode(const OBJECTMODE objMode);
	void SetMeshField(CMeshField *pMeshField);
	void ShearchEditBlock(void);
	bool JudgAreaInObject(CObject3D *pObj3D, D3DXVECTOR3 pos, float fWidth, float fDepth);
	CBlock *ShearchBlockInArea(void);
	CBlock *ShearchBlockInArea_Loop(int nCntPriority, bool *pShearch, D3DXVECTOR3 ShearchPos, float fShearchWidth, float fShearchDepth);
	CBlock *ShearchBlock(CBlock *pBlock, bool *pShearch, D3DXVECTOR3 ShearchPos, float fShearchWidth, float fShearchDepth);
	void CreateBaseObject(void);
	void ReleaseBaseObject(void);
	void SwitchBaseObject(int nObjMode);
	void SetValueToBaseObject(void);
	void CreateObject(void);
	void CreateBillboardObj(void);
	void CreateEffect(void);

	MODE GetMode(void);
	EDITMODE GetEditMode(void);
	CAMERAMODE GetCameraMode(void);
	GAMEFIELDMODE GetGameFieldMode(void);
	OBJECTMODE GetObjectMode(void);

	// エディター用関数
	void SetGridDisp(const bool bDisp);
	void SetColRangeDisp(const bool bDisp);
	bool GetGridDisp(void);
	bool GetColRangeDisp(void);
	int GetMassX(void);
	int GetMassZ(void);
	int GetBoxSide(void);
	int GetBoxVertical(void);
	int GetStartLeftVerIndex(void);
	CBaseObject *GetBaseObject(void);

	// ブロック用関数
	void SetEditBlock(CBlock *pBlock);
	void SetBlockType(const int nType);
	void SetBlockModelIdx(const int nModelIdx);
	void SetBlockRot(const D3DXVECTOR3 rot);
	void SetBlockColWidth(const float fWidth);
	void SetBlockColHeight(const float fHeight);
	void SetBlockColDepth(const float fDepth);
	CBlock *GetEditBlock(void);
	int GetBlockType(void);
	int GetBlockModelIdx(void);
	D3DXVECTOR3 GetBlockRot(void);
	float GetBlockColWidth(void);
	float GetBlockColHeight(void);
	float GetBlockColDepth(void);

	// 川用関数
	void SetRiverTexIdx(const int nTexIdx);
	int GetRiverTexIdx(void);

	// 氷用関数
	void SetIceTexIdx(const int nTexIdx);
	int GetIceTexIdx(void);

	// 配置モデル用関数
	void SetObjModelType(const int nType);
	void SetObjModelPos(const D3DXVECTOR3 pos);
	void SetObjModelRot(const D3DXVECTOR3 rot);
	int GetObjModelType(void);
	D3DXVECTOR3 GetObjModelPos(void);
	D3DXVECTOR3 GetObjModelRot(void);

	// 配置ビルボード用関数
	void SetObjBillTexIdx(const int nTexIdx);
	void SetObjBillPos(const D3DXVECTOR3 pos);
	void SetObjBillCol(const D3DXCOLOR col);
	void SetObjBillRot(const float fRot);
	void SetObjBillWidth(const float fWidth);
	void SetObjBillHeight(const float fHeight);
	void SetObjBillLighting(const bool bLighting);
	void SetObjBillDrawAddtive(const bool bDrawAddtive);
	int GetObjBillTexIdx(void);
	D3DXVECTOR3 GetObjBillPos(void);
	D3DXCOLOR GetObjBillCol(void);
	float GetObjBillRot(void);
	float GetObjBillWidth(void);
	float GetObjBillHeight(void);
	bool GetObjBillLighting(void);
	bool GetObjBillDrawAddtive(void);

	// 配置エフェクト用関数
	void SetObjEffectType(const int nType);
	void SetObjEffectPos(const D3DXVECTOR3 pos);
	void SetObjEffectRot(const D3DXVECTOR3 rot);
	int GetObjEffectType(void);
	D3DXVECTOR3 GetObjEffectPos(void);
	D3DXVECTOR3 GetObjEffectRot(void);

	// マップ情報用関数
	void SetMapLoadFileName(char *pFileName);
	void SetMapSaveFileName(char *pFileName);
	char *GetMapLoadFileName(void);
	char *GetMapSaveFileName(void);

	// モデルリスト情報用関数
	void SetModelListLoadFileName(char *pFileName);
	void SetModelListSaveFileName(char *pFileName);
	char *GetModelListLoadFileName(void);
	char *GetModelListSaveFileName(void);

	// テクスチャリスト情報用関数
	void SetTexListLoadFileName(char *pFileName);
	void SetTexListSaveFileName(char *pFileName);
	char *GetTexListLoadFileName(void);
	char *GetTexListSaveFileName(void);

	// ライト情報用関数
	void SetLightLoadFileName(char *pFileName);
	void SetLightSaveFileName(char *pFileName);
	char *GetLightLoadFileName(void);
	char *GetLightSaveFileName(void);

	// ゲームフィールド用関数
	void SetGameFieldLoadFileName(char *pFileName);
	void SetGameFieldSaveFileName(char *pFileName);
	char *GetGameFieldLoadFileName(void);
	char *GetGameFieldSaveFileName(void);

	// 配置物情報用関数
	void SetObjectLoadFileName(char *pFileName);
	void SetObjectSaveFileName(char *pFileName);
	char *GetObjectLoadFileName(void);
	char *GetObjectSaveFileName(void);

	// 敵の生成情報用関数
	void SetEnemyListLoadFileName(char *pFileName);
	void SetEnemyListSaveFileName(char *pFileName);
	char *GetEnemyListLoadFileName(void);
	char *GetEnemyListSaveFileName(void);

	//静的メンバ関数
	static CEditor *Create(void);

private:	//自分だけがアクセス可能
	void ClearVariable(void);
	void ClearEditorVariable(void);
	void ClearMapVariable(void);
	void ClearBlockVariable(void);
	void ClearRiverVariable(void);
	void ClearIceVariable(void);
	void ClearObjModelVariable(void);
	void ClearObjBillVariable(void);
	void ClearObjEffectVariable(void);
	void ClearModelListVariable(void);
	void ClearTexListVariable(void);
	void ClearLightVariable(void);
	void ClearGameFieldVariable(void);
	void ClearObjectVariable(void);
	void ClearEnemyListVariable(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadPlayerRespawnModel(CModelCreate *pModelCreate, char *pStr);
	void LoadEnemyRespawnModel(CModelCreate *pModelCreate, char *pStr);
	void LoadEffectFileName(char *pStr);
	void LoadMapFileName(char *pStr);

	void CreateCamera(void);
	void CreateMap(void);
	void CreateUI(void);
	void CheckFieldPenetration(void);
	void BoxVisualization(int nMassX, int nMassZ);
	void PenetrationFieldArea(int nMassX, int nMassZ, int nLeftVerIndex);
	void AreaVisualization(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void CreateGameFieldObj(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void BlockCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void RiverCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void IceCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void DestroyGameFieldObj(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void BlockDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void RiverDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void IceDestroyCheck(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	bool GameFieldObjDestroyCheck(CObject3D *pObj3D, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);
	bool GameFieldObjDestroyCheck_Half(CObject3D *pObj3D, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);
	void BlockDestroy(CBlock *pBlock, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);
	void RiverDestroy(CRiver *pRiver, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);
	void IceDestroy(CIceField *pIceField, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);
	void ReturnFieldHeight_FromRiver(CRiver *pRiver);
	void ReturnFieldHeight_FromIce(CIceField *pIceField);

	void ExChangeField(void);
	void ExChangeGrid(void);

	// エディター用変数
	char m_aPlayerRespawnFileName[256];   // プレイヤーのリスポーンモデルのファイル名
	char m_aEnemyRespawnFileName[256];    // 敵のリスポーンモデルのファイル名
	CMeshField *m_pMeshField;             // メッシュフィールドクラスへのポインタ
	CMeshCollider *m_pMeshCollider;       // メッシュコライダークラスへのポインタ
	CGrid *m_pGrid;                       // グリッド線クラスへのポインタ
	C3DPolygon *m_pArea;                  // エリア可視化用のポリゴンクラスへのポインタ
	CUI *m_pUI;                           // UIクラスへのポインタ
	CBaseObject *m_pBaseObject;           // ベースオブジェクトクラスへのポインタ
	bool m_bDispGrid;                     // グリッド線を描画するかどうか
	bool m_bDispColRange;                 // 当たり判定を描画するかどうか
	int m_nLeftVerIdx;                    // 現在マウスがさしているエリアの左上頂点番号
	int m_nMassX;                         // 現在マウスがさしているエリア番号
	int m_nMassZ;                         // 現在マウスがさしているエリア番号
	int m_nBoxSide;                       // オブジェクトを配置するエリアの横の分割数
	int m_nBoxVertical;                   // オブジェクトを配置するエリアの奥行の分割数
	int m_nStartLeftVerIndex;             // オブジェクトを配置するエリアの左上頂点番号
	MODE m_Mode;                          // 生成するか破棄するか
	EDITMODE m_EditMode;                  // 現在編集できるオブジェクトの種類
	CAMERAMODE m_CameraMode;              // カメラのモード(どのカメラを使うか決定する)
	GAMEFIELDMODE m_GameFieldMode;        // ゲームフィールドに配置するオブジェクトの種類
	OBJECTMODE m_ObjMode;                 // 配置するオブジェクトの種類

	// ブロック用変数
	CBlock *m_pEditBlock;                 // 編集するブロッククラスへのポインタ
	int m_nBlockType;                     // 配置するブロックの種類番号
	int m_nBlockModelIdx;                 // 配置するブロックの使用するモデルの番号
	D3DXVECTOR3 m_BlockRot;               // 配置するブロックの向き
	float m_fBlockColWidth;               // 配置するブロックの当たり判定幅情報
	float m_fBlockColHeight;              // 配置するブロックの当たり判定高さ情報
	float m_fBlockColDepth;               // 配置するブロックの当たり判定奥行情報

	// 川用変数
	int m_nRiverTexIdx;                   // 川に使用するテクスチャの番号

	// 氷用変数
	int m_nIceTexIdx;                     // 氷に使用するテクスチャの番号

	// 配置モデル用変数
	int m_nObjModelType;                  // 配置モデルの種類
	D3DXVECTOR3 m_ObjModelPos;            // 配置モデルの座標
	D3DXVECTOR3 m_ObjModelRot;            // 配置モデルの向き

	// 配置ビルボード用変数
	int m_nObjBillTexIdx;                 // 配置ビルボードが使用するテクスチャの番号
	D3DXVECTOR3 m_ObjBillPos;             // 配置ビルボードの座標
	D3DXCOLOR m_ObjBillCol;               // 配置ビルボードの色
	float m_fObjBillRot;                  // 配置ビルボードのポリゴンの向き
	float m_fObjBillWidth;                // 配置ビルボードの幅
	float m_fObjBillHeight;               // 配置ビルボードの高さ
	bool m_bObjBillLighting;              // 配置ビルボードのライティングをするかどうか
	bool m_bObjBillDrawAddtive;           // 配置ビルボードの加算合成で描画するかしないか

	// 配置エフェクト用変数
	int m_nObjEffectType;                 // 配置エフェクトの種類
	D3DXVECTOR3 m_ObjEffectPos;           // 配置エフェクトの座標
	D3DXVECTOR3 m_ObjEffectRot;           // 配置エフェクトの向き

	// マップ情報用変数
	char m_aMapFileName[256];             // 読み込むマップのファイル名
	char m_aMapLoadFileName[256];         // マップ情報を読み込むスクリプトファイル名
	char m_aMapSaveFileName[256];         // マップ情報を保存するスクリプトファイル名

	// モデルリスト情報用変数
	char m_aModelListLoadFileName[256];   // モデルリスト情報を読み込むスクリプトファイル名
	char m_aModelListSaveFileName[256];   // モデルリスト情報を保存するスクリプトファイル名

	// テクスチャリスト情報用変数
	char m_aTexListLoadFileName[256];     // テクスチャリスト情報を読み込むスクリプトファイル名
	char m_aTexListSaveFileName[256];     // テクスチャリスト情報を保存するスクリプトファイル名

	// ライト情報用変数
	char m_aLightLoadFileName[256];       // ライト情報を読み込むスクリプトファイル名
	char m_aLightSaveFileName[256];       // ライト情報を保存するスクリプトファイル名

	// ゲームフィールド用変数
	char m_aGameFieldLoadFileName[256];   // ゲームフィールド情報を読み込むスクリプトファイル名
	char m_aGameFieldSaveFileName[256];   // ゲームフィールド情報を保存するスクリプトファイル名

	// 配置物情報用変数
	char m_aObjectLoadFileName[256];      // 配置物情報を読み込むスクリプトファイル名
	char m_aObjectSaveFileName[256];      // 配置物情報を保存するスクリプトファイル名

	// 敵の生成情報用変数
	char m_aEnemyListLoadFileName[256];   // 敵の生成情報を読み込むスクリプトファイル名
	char m_aEnemyListSaveFileName[256];   // 敵の生成情報を保存するスクリプトファイル名
};
#endif