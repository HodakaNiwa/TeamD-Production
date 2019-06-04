//*****************************************************************************
//
//     レンダリングの処理[renderer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define RENDERER_WINDOW_COLOR (D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0xff))    // 画面クリア時の背景色

//=============================================================================
//    コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	// 各種値のクリア
	m_pD3DDevice = NULL;        // デバイスへのポインタ
	m_pD3D = NULL;              // Direct3Dオブジェクトへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							            // バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				            // ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				            // ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;			            	// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		                // 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						    // デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デプスバッファとして24bit、ステンシルバッファとして8bitを使う
	d3dpp.Windowed = bWindow;						                // ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	    // インターバル
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;              // ダブルステンシル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);              // 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                      // Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);             // αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                     // ライティングを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);        // αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // αデスティネーションカラーの指定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // アルファブレンディング処理を設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); // ２番目のアルファ引数

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
	// デバイスの破棄
	DIRECT_RELEASE(m_pD3DDevice);

	// Direct3Dオブジェクトの破棄
	DIRECT_RELEASE(m_pD3D);
}

//=============================================================================
//    更新処理
//=============================================================================
void CRenderer::Update(void)
{
}

//=============================================================================
//    描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3Dによる描画の開始(レンダリングターゲットはテクスチャ)
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// デバッグ情報描画処理
		CDebugProc::Draw();

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
//    描画デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}