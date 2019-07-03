//=============================================================================
//
// レンダリング処理 [renderer.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "debugproc.h"
#include "fade.h"
#include "basemode.h"

//=============================================================================
// レンダリングのコンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;			// Direct3Dオブジェクト
	m_pD3DDevice = NULL;	// Deviceオブジェクト(描画に必要)
	m_pRenderOrg = NULL;    // デフォルトのレンダリングターゲットへのポインタ
	m_pBuffOrg = NULL;      // デフォルトの各種バッファへのポインタ
	m_pTextureMT = NULL;    // マルチレンダリングに使用するテクスチャへのポインタ
	m_pRenderMT = NULL;     // マルチレンダリングに使用するレンダリングターゲットへのポインタ
	m_pBuffMT = NULL;       // マルチレンダリングに使用する各種バッファへのポインタ
}
//=============================================================================
// レンダリングのデストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	// 全ての開放処理
	CScene::ReleaseAll();

	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount			 = 1;							// バックバッファの数
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed					 = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF, hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1 , D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// 現在の情報を保存
	m_pD3DDevice->GetRenderTarget(0, &m_pRenderOrg);
	m_pD3DDevice->GetDepthStencilSurface(&m_pBuffOrg);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
	// マルチレンダリングに使用する情報を開放
	ReleaseMultRenderer();

	// レンダリングターゲットの破棄
	if (m_pRenderOrg != NULL)
	{
		m_pRenderOrg->Release();
		m_pRenderOrg = NULL;
	}

	// 各種バッファを破棄
	if (m_pBuffOrg != NULL)
	{
		m_pBuffOrg->Release();
		m_pBuffOrg = NULL;
	}

	// デバイスの破棄
	if(m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if(m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	CFade *pFade;
	pFade = CManager::GetFade();

	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0xff), 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		if (CManager::GetBaseMode() != NULL)
		{
			CManager::GetBaseMode()->Draw();
		}

		pFade->Draw();

		//デバッグの表示
		CDebugProc::Draw();

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}

//=============================================================================
// レンダリングターゲットを入れ替える処理
//=============================================================================
void CRenderer::SetRenderTarget(const LPDIRECT3DTEXTURE9 pTexture)
{
	// テクスチャのサーフェイス情報を取得
	pTexture->GetSurfaceLevel(0, &m_pRenderMT);

	// テクスチャにレンダリングするときに使うバッファを生成
	m_pD3DDevice->CreateDepthStencilSurface(SCREEN_WIDTH, SCREEN_HEIGHT,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pBuffMT, NULL);

	// テクスチャのサーフェイス情報を設定
	m_pD3DDevice->SetRenderTarget(0, m_pRenderMT);
	m_pD3DDevice->SetDepthStencilSurface(m_pBuffMT);

	// サーフェイス情報をクリアする
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);
}

//=============================================================================
// レンダリングターゲットをデフォルトに戻す処理
//=============================================================================
void CRenderer::ResetRenderTarget(void)
{
	// マルチレンダリングが使用されていれば開放する
	ReleaseMultRenderer();

	m_pD3DDevice->SetRenderTarget(0, m_pRenderOrg);
	m_pD3DDevice->SetDepthStencilSurface(m_pBuffOrg);
}

//=============================================================================
// マルチレンダリングに使用していたものを開放する
//=============================================================================
void CRenderer::ReleaseMultRenderer(void)
{
	// レンダリングターゲット
	if (m_pRenderMT != NULL)
	{
		m_pRenderMT->Release();
		m_pRenderMT = NULL;
	}

	// 各種バッファ類へのポインタ
	if (m_pBuffMT != NULL)
	{
		m_pBuffMT->Release();
		m_pBuffMT = NULL;
	}

	// テクスチャへのポインタを空にする
	m_pTextureMT = NULL;
}