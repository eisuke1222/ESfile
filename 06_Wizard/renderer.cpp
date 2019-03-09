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
#include "camera.h"			// カメラ
#include "font.h"			// フォント
#include "texture.h"		// テクスチャ

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
CFont	*CRenderer::m_pFont[CFont::FONTNUM_MAX] = {};				// フォント
bool	CRenderer::m_bImGuiWindow				= false;			// ImGui表示
bool	CRenderer::m_bDrawAll					= true;				// 全描画
bool	CRenderer::m_bDrawMap					= true;				// マップ描画

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
	D3DPRESENT_PARAMETERS	d3dpp;		//ディスプレイモード
    D3DDISPLAYMODE			d3ddm;		//プレゼンテーションパラメータ

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
		return E_FAIL;

	// 現在のディスプレイモードを取得
    if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		return E_FAIL;

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

	// Setup Dear ImGui binding		（ImGui初期化）
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(m_pD3DDevice);

	// Setup style		（初期の色変更できる）
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();

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
	{// フォント数カウント
		if (m_pFont[nCntFont] == NULL)
		{// NULLの場合
			// 初期値設定
			int			nHeight = 20;										// 高さ
			int			nWidth	= 0;										// 横幅
			int			nWeight = 0;										// 幅
			RECT		rect	= { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// 位置
			UINT		uFmt	= DT_LEFT;									// 段落
			D3DXCOLOR	col		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);;		// 色

			switch (nCntFont)
			{// カウント番号
			case CFont::FONTNUM_COMMAND:	// コマンド
				break;
			case CFont::FONTNUM_LOG:		// ログ
				rect = { 0, 200, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			case CFont::FONTNUM_DEBUG:		// デバッグ
				rect = { 300, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			case CFont::FONTNUM_DATA:		// データ
				rect = { SCREEN_WIDTH - 350, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			case CFont::FONTNUM_TEST:		// テスト
				rect = { SCREEN_WIDTH - 640, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				break;
			}

			// フォント生成
			m_pFont[nCntFont] = CFont::Create(nHeight, nWidth, nWeight, rect, uFmt, col);
		}
	}

#ifdef _DEBUG
	m_bImGuiWindow = true;		// 表示
#else
	m_bImGuiWindow = false;		// 非表示
#endif // _DEBUG

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CRenderer ***
//=============================================================================
void CRenderer::Uninit(void)
{
	CTexture::ReleaseAll();			// テクスチャ破棄
	CScene::ReleaseAll();			// 全シーン破棄

	// デバッグ情報表示用フォントの破棄
	for (int nCntFont = 0; nCntFont < CFont::FONTNUM_MAX; nCntFont++)
	{// フォント数カウント
		if (m_pFont[nCntFont] == NULL)
		{// NULL以外の場合
			m_pFont[nCntFont]->Uninit();	// 終了処理
			m_pFont[nCntFont] = NULL;		// NULLへ
		}
	}

	CFont::ReleaseAll();			// 全フォント終了処理

	// ImGui終了
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

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
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F3))
	{// キーボード（F3）キーを押された
		m_bImGuiWindow = m_bImGuiWindow ? false : true;		// ImGui表示切り替え
	}

	if (m_bImGuiWindow == true)
	{// 表示する
		// Start the Dear ImGui frame	（書かないと動かない）
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		// ESウィンドウ
		ImGui::ShowESWindow();

		ImGui::EndFrame();

		//ShowCursor(true);
	}
	else
	{
		/*HWND hWnd = GetForegroundWindow();
		RECT rect;

		GetWindowRect(hWnd, (LPRECT)&rect);

		SetCursorPos((rect.right - rect.left) / 2, (rect.bottom - rect.top) / 2);

		ShowCursor(false);*/
		
	}

	CScene::UpdateAll();			// 全シーン更新処理
}

//=============================================================================
// 描画処理										(public)	*** CRenderer ***
//=============================================================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(0.4f, 0.6f, 0.9f, 1.0f), 1.0f, 0);

	CCamera *pCamera = NULL;	// カメラ
	CManager::MODE mode = CManager::GetMode();

	D3DVIEWPORT9 viewPort;
	m_pD3DDevice->GetViewport(&viewPort);

	// Direct3Dによる描画の開始
	if(SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// カメラ取得（メイン）
		pCamera = CManager::GetCamera(CManager::CAMERA_MAIN);
		pCamera->SetCamera();

		if (m_bDrawAll == true)
		{// 表示する
			CScene::DrawAll();		// 全シーン描画処理
		}

		if (m_bImGuiWindow == true)
		{// 表示する
			// フィルモード設定
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

			// ImGui描画
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}

		CFont::DrawAll();			// 全フォント描画処理

		if (CScene::GetDrawUi() == true)
		{
			if (m_bDrawMap == true)
			{
				// バックバッファ＆Ｚバッファのクリア
				m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DXCOLOR(0.4f, 0.6f, 0.9f, 1.0f), 1.0f, 0);

				// カメラ取得（マップ）
				pCamera = CManager::GetCamera(CManager::CAMERA_GAME_MAP);
				pCamera->SetCamera();

				if (m_bDrawAll == true)
				{// 表示する
					CScene::DrawAllMap();		// 全シーン描画マップ処理
				}
			}
		}
		
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}
	m_pD3DDevice->SetViewport(&viewPort);

	// バックバッファとフロントバッファの入れ替え
	HRESULT result = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

	D3DPRESENT_PARAMETERS d3dpp;		//ディスプレイモード

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && m_pD3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		m_pD3DDevice->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}