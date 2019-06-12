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
class CGrid;
class CBlock;
class CFileLoader;
class CFileSaver;
class CModelCreate;

//*****************************************************************************
// クラス定義
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
	void SetRespawnModel(void);
	void SetMode(const MODE mode);
	void SetEditMode(const EDITMODE editMode);
	void SetCameraMode(const CAMERAMODE cameraMode);
	void SetGameFieldMode(const GAMEFIELDMODE gameFieldMode);
	void SetObjectMode(const OBJECTMODE objMode);
	void SetMeshField(CMeshField *pMeshField);

	MODE GetMode(void);
	EDITMODE GetEditMode(void);
	CAMERAMODE GetCameraMode(void);
	GAMEFIELDMODE GetGameFieldMode(void);
	OBJECTMODE GetObjectMode(void);

	// エディター用関数
	int GetMasuX(void);
	int GetMasuZ(void);
	int GetBoxSide(void);
	int GetBoxVertical(void);
	int GetStartLeftVerIndex(void);

	// ブロック用関数
	void SetBlockType(const int nType);
	void SetBlockModelIdx(const int nModelIdx);
	void SetBlockBreak(const bool bBreak);
	void SetBlockColWidth(const float fWidth);
	void SetBlockColHeight(const float fHeight);
	void SetBlockColDepth(const float fDepth);
	int GetBlockType(void);
	int GetBlockModelIdx(void);
	bool GetBlockBreak(void);
	float GetBlockColWidth(void);
	float GetBlockColHeight(void);
	float GetBlockColDepth(void);

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

	//静的メンバ関数
	static CEditor *Create(void);

private:	//自分だけがアクセス可能
	void ClearVariable(void);
	void ClearEditorVariable(void);
	void ClearMapVariable(void);
	void ClearBlockVariable(void);
	void ClearModelListVariable(void);
	void ClearTexListVariable(void);
	void ClearLightVariable(void);
	void ClearGameFieldVariable(void);
	void ClearObjectVariable(void);

	void LoadSystem(void);
	void LoadSystemScript(CFileLoader *pFileLoader, char *pStr);
	void LoadPlayerRespawnModel(CModelCreate *pModelCreate, char *pStr);
	void LoadEnemyRespawnModel(CModelCreate *pModelCreate, char *pStr);
	void LoadMapFileName(char *pStr);
	void SaveSystem(void);
	void SavePlayerRespawnModel(CFileSaver *pFileSaver);
	void SaveEnemyRespawnModel(CFileSaver *pFileSaver);
	void SaveMapFileName(CFileSaver *pFileSaver);

	void CreateCamera(void);
	void CreateMap(void);
	void CheckFieldPenetration(void);
	void PenetrationFieldArea(int nMasuX, int nMasuZ, int nLeftVerIndex);
	void FillinFieldArea(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void FillinFieldBox(int nLeftVerIndex);
	void BlockCreate(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void BlockDestroy(int nSwitchX, int nSwitchZ, bool bSwitchX, bool bSwitchZ);
	void BlockDestroyCheck(CBlock *pBlock, D3DXVECTOR3 DestroyPos, float fDestroyWidth, float fDestroyDepth, bool bSwitchX, bool bSwitchZ);

	// エディター用変数
	char m_aPlayerRespawnFileName[256];   // プレイヤーのリスポーンモデルのファイル名
	char m_aEnemyRespawnFileName[256];    // 敵のリスポーンモデルのファイル名
	CMeshField *m_pMeshField;             // メッシュフィールドクラスへのポインタ
	CGrid *m_pGrid;                       // グリッド線クラスへのポインタ
	int m_nMasuX;                         // 現在マウスがさしているエリア番号
	int m_nMasuZ;                         // 現在マウスがさしているエリア番号
	int m_nBoxSide;                       // オブジェクトを配置するエリアの横の分割数
	int m_nBoxVertical;                   // オブジェクトを配置するエリアの奥行の分割数
	int m_nStartLeftVerIndex;             // 現在マウスがさしているエリアの左上頂点番号
	MODE m_Mode;                          // 生成するか破棄するか
	EDITMODE m_EditMode;                  // 現在編集できるオブジェクトの種類
	CAMERAMODE m_CameraMode;              // カメラのモード(どのカメラを使うか決定する)
	GAMEFIELDMODE m_GameFieldMode;        // ゲームフィールドに配置するオブジェクトの種類
	OBJECTMODE m_ObjMode;                 // 配置するオブジェクトの種類

	// ブロック用変数
	int m_nBlockType;                     // 配置するブロックの種類番号
	int m_nBlockModelIdx;                 // 配置するブロックの使用するモデルの番号
	bool m_bBlockBreak;                   // 配置するブロックを壊せるかどうか
	float m_fBlockColWidth;               // 配置するブロックの当たり判定幅情報
	float m_fBlockColHeight;              // 配置するブロックの当たり判定高さ情報
	float m_fBlockColDepth;               // 配置するブロックの当たり判定奥行情報

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
	int m_nObjModelIdx;                   // 配置するオブジェクトの使用するモデル番号
	int m_nObjTexIdx;                     // 配置するオブジェクトの使用するテクスチャ番号
	D3DXVECTOR3 m_ObjPos;                 // 配置するオブジェクトの位置
	D3DXVECTOR3 m_ObjRot;                 // 配置するオブジェクトの向き
	char m_aObjectLoadFileName[256];      // 配置物情報を読み込むスクリプトファイル名
	char m_aObjectSaveFileName[256];      // 配置物情報を保存するスクリプトファイル名
};
#endif