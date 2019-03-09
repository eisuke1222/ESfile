//=============================================================================
//
// レンダラー処理 [renderer.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "renderer.h"		// レンダラー
#include "manager.h"		// マネージャー
#include "scene.h"			// シーン
#include "input.h"			// 入力
#include "font.h"			// フォント

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CFont	*CRenderer::m_pFont[CFont::FONTNUM_MAX] = {};

//=============================================================================
// コンストラクタ								(public)	*** CRenderer ***
//=============================================================================
CRenderer::CRenderer()
{
	// 初期値設定
	m_pD3D			= NULL;			// オブジェクト
	m_pD3DDevice	= NULL;			// デバイス
}

//=============================================================================
// デストラクタ									(public)	*** CRenderer ***
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理									(public)	*** CRenderer ***
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;		//ディスプレイモード
    D3DDISPLAYMODE d3ddm;				//プレゼンテーションパラメータ

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

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// デバッグ情報表示用フォントの生成
	for (int nCntFont = 0; nCntFont < CFont::FONTNUM_MAX; nCntFont++)
	{
		if (m_pFont[nCntFont] == NULL)
		{
			int nHeight = 20;
			int nWidth = 0;
			int nWeight = 0;
			RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			UINT uFmt = DT_LEFT;
			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);;

			switch (nCntFont)
			{
			case CFont::FONTNUM_COMMAND:
				break;
			case CFont::FONTNUM_LOG:
				rect = { 0, 200, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			case CFont::FONTNUM_DEBUG:
				rect = { 300, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			case CFont::FONTNUM_DATA:
				rect = { SCREEN_WIDTH - 350, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			case CFont::FONTNUM_TEST:
				rect = { SCREEN_WIDTH - 640, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			default:
				break;
			}

			m_pFont[nCntFont] = CFont::Create(nHeight, nWidth, nWeight, rect, uFmt, col);
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理										(public)	*** CRenderer ***
//=============================================================================
void CRenderer::Uninit(void)
{
	CScene::ReleaseAll();

	// デバッグ情報表示用フォントの破棄
	for (int nCntFont = 0; nCntFont < CFont::FONTNUM_MAX; nCntFont++)
	{// カウント
		if (m_pFont[nCntFont] == NULL)
		{// NULL以外の場合
			m_pFont[nCntFont]->Uninit();			// 解放
			m_pFont[nCntFont] = NULL;				// NULLへ
		}
	}

	CFont::ReleaseAll();

	// デバイスの破棄
	if(m_pD3DDevice != NULL)
	{// NULL以外の場合
		m_pD3DDevice->Release();	// 解放
		m_pD3DDevice = NULL;		// NULLへ
	}

	// Direct3Dオブジェクトの破棄
	if(m_pD3D != NULL)
	{// NULL以外の場合
		m_pD3D->Release();			// 解放
		m_pD3D = NULL;				// NULLへ
	}
}

//=============================================================================
// 更新処理										(public)	*** CRenderer ***
//=============================================================================
void CRenderer::Update(void)
{
	CScene::UpadteAll();			// 更新処理
}

//=============================================================================
// 描画処理										(public)	*** CRenderer ***
//=============================================================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(100, 149, 237, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CScene::DrawAll();			// 描画処理

		CFont::DrawAll();			// 描画処理

		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}