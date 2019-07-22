//=============================================================================
//
// テクスチャマネージャーの処理 [textureManager.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "textureManager.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// テクスチャマネージャーのコンストラクタ
//=============================================================================
CTextureManager::CTextureManager()
{
	m_pTexture = NULL;			//テクスチャのポインタ
	m_pFileName = NULL;	    	//ファイル名のポインタ
	m_nNumTexture = 0;			//テクスチャの総数
}
//=============================================================================
// テクスチャマネージャーのコンストラクタ
//=============================================================================
CTextureManager::~CTextureManager()
{

}
//=============================================================================
// テクスチャマネージャーの生成
//=============================================================================
CTextureManager *CTextureManager::Create(int nNumTexture)
{
	CTextureManager *pTextureManager = NULL;				//弾のポインタ

	if (pTextureManager == NULL)
	{//NULLの場合
		pTextureManager = new CTextureManager;				//動的確保
		if (pTextureManager != NULL)
		{//NULLでない場合
			pTextureManager->SetNumTexture(nNumTexture);	//位置の設置処理
			pTextureManager->Init();						//初期化処理
		}
	}
	return pTextureManager;
}
//=============================================================================
// 初期化処理
//=============================================================================
void CTextureManager::Init(void)
{
	if (m_pTexture == NULL)
	{
		//テクスチャの動的確保
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumTexture];
		for (int nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
		{//文字列の動的確保
			m_pTexture[nCntTexture] = NULL;
		}
	}

	if (m_pFileName == NULL)
	{//ファイル名の動的確保
		m_pFileName = new char*[m_nNumTexture];
		if (m_pFileName != NULL)
		{
			for (int nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
			{//文字列の動的確保
				m_pFileName[nCntTexture] = new char[256];
			}
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void CTextureManager::Uninit(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		for (int nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
		{
			if (m_pTexture[nCntTexture] != NULL)
			{
				m_pTexture[nCntTexture]->Release();
				m_pTexture[nCntTexture] = NULL;
			}
		}
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

	//ファイル名の破棄
	if (m_pFileName != NULL)
	{
		for (int nCntTexture = 0; nCntTexture < m_nNumTexture; nCntTexture++)
		{//テクスチャの総数分繰り返し
			delete[] m_pFileName[nCntTexture];
			m_pFileName[nCntTexture] = NULL;
		}
		delete[] m_pFileName;
		m_pFileName = NULL;
	}
}

//=============================================================================
// テクスチャの設置処理
//=============================================================================
void CTextureManager::SetTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx)
{
	m_pTexture[nIdx] = pTexture;
}

//=============================================================================
// テクスチャの総数設置処理
//=============================================================================
void CTextureManager::SetNumTexture(int nNumTexture)
{
	m_nNumTexture = nNumTexture;
}

//=============================================================================
// ファイル名の設置処理
//=============================================================================
void CTextureManager::SetFileName(char *pFileName,int nIdx)
{
	strcpy(m_pFileName[nIdx], pFileName);
}

//=============================================================================
// ファイル名の取得処理
//=============================================================================
char *CTextureManager::GetFileName(int nIdx)
{
	return m_pFileName[nIdx];
}

//=============================================================================
// テクスチャの総数取得処理
//=============================================================================
int CTextureManager::GetNumTexture(void)
{
	return m_nNumTexture;
}

//=============================================================================
// テクスチャの取得処理
//=============================================================================
LPDIRECT3DTEXTURE9 CTextureManager::GetTexture(int nIdx)
{
	return m_pTexture[nIdx];
}