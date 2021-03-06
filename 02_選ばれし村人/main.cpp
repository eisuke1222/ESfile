//=========================================================================================================================
//
// メイン処理 [main.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"
#include "title.h"		// タイトル
#include "game.h"		// ゲーム
#include "result.h"		// リザルト
#include "ranking.h"	// ランキング
#include "pause.h"		// ポーズ
#include "fade.h"		// フェード
#include "window.h"		// ウィンドウ
#include "sound.h"		// サウンド
#include "font.h"		// フォント
#include "input.h"		// 入力処理

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"選ばれし村人"			// ウインドウのキャプション名

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3D9				g_pD3D			=		NULL;				// Direct3Dオブジェクト
LPDIRECT3DDEVICE9		g_pD3DDevice	=		NULL;				// デバイスオブジェクト(描画に必要)
bool					g_bWindowMode	=		false;				// ウィンドウモード
MODE					g_Mode			=		MODE_TITLE;			// 初期画面状態

//=========================================================================================================================
// メイン関数
//=========================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	// カーソル表示
	//ShowCursor(false);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
							CLASS_NAME,
							WINDOW_NAME,
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							(rect.right - rect.left),
							(rect.bottom - rect.top),
							NULL,
							NULL,
							hInstance,
							NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, g_bWindowMode)))
	{
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=========================================================================================================================
// プロシージャ
//=========================================================================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 変数宣言
	int nID;		// メッセージ選択ID

	switch (uMsg)
	{
	case WM_DESTROY:
		// "WM_QUIT"メッセージを送る
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		// ×ボタン警告
		if (g_bWindowMode == true)
		{// ウィンドウモードの場合
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);	// 終了確認メッセージ

			if (nID == IDYES)
			{// YESを押した場合
			 // ウィンドウを破棄する（WM_DESTOROYメッセージを送る）
				DestroyWindow(hWnd);
			}
		}
		else
		{
			// ウィンドウを破棄する（WM_DESTOROYメッセージを送る）
			DestroyWindow(hWnd);
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:	// ESCキーが押された
			if (g_bWindowMode == true)
			{// ウィンドウモードの場合
				nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

				if (nID == IDYES)
				{// YESを押した場合のみ
				 // ウィンドウを破棄する（WM_DESTOROYメッセージを送る）
					DestroyWindow(hWnd);
				}
			}
			else
			{
				// ウィンドウを破棄する（WM_DESTOROYメッセージを送る）
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を返す
}

//=========================================================================================================================
// 初期化処理
//=========================================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount			 = 1;							// バックバッファの数
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed					 = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	// キーボード初期化処理
	InitKeyboard(hInstance, hWnd);
	InitMouse(hInstance, hWnd);
	InitJoystick(hInstance, hWnd);

	// サウンド初期化処理
	InitSound(hWnd);

	// フェード初期化処理
	InitFade(g_Mode);

	// ウィンドウ初期化処理
	InitWindow();

	// 画面初期化処理
	SetMode(g_Mode);

	// フォント初期化処理
	InitFont();

	return S_OK;
}

//=========================================================================================================================
// 終了処理
//=========================================================================================================================
void Uninit(void)
{
	// キーボード終了処理
	UninitInput();

	// サウンド終了処理
	UninitSound();

	// ウィンドウ終了処理
	UninitWindow();

	// フォント終了処理
	UninitFont();

	// フェード終了処理
	UninitFade();

	if(g_pD3DDevice != NULL)
	{// デバイスオブジェクトの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//=========================================================================================================================
// 更新処理
//=========================================================================================================================
void Update(void)
{
	bool bPause = false;

	// キーボード更新処理
	UpdateKeyboard();
	UpdateMouse();
	UpdateJoystick();

	switch (g_Mode)
	{// 画面更新処理
	case MODE_TITLE:		// タイトル
		UpdateTitle();
		break;
	case MODE_GAME:			// ゲーム
		UpdateGame();
		bPause = GetPauseState();
		break;
	case MODE_RESULT:		// リザルト
		UpdateResult();
		break;
	case MODE_RANKING:		// ランキング
		UpdateRanking();
		break;
	}

	if (bPause == false)
	{// ポーズが使用されている場合
		// ウィンドウ更新処理
		UpdateWindow();

		// フォント更新処理
		UpdateFont();
	}

	// フェード更新処理
	UpdateFade();
}

//=========================================================================================================================
// 描画処理
//=========================================================================================================================
void Draw(void)
{
	bool bPause = false;

	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// ウィンドウ背景描画処理
		DrawBGWindow();

		switch (g_Mode)
		{// 画面描画処理
		case MODE_TITLE:	// タイトル
			DrawTitle();
			break;
		case MODE_GAME:		// ゲーム
			DrawGame();
			bPause = GetPauseState();
			break;
		case MODE_RESULT:	// リザルト
			DrawResult();
			break;
		case MODE_RANKING:	// ランキング
			DrawRanking();
			break;
		}

		// ウィンドウ前景背景描画処理
		DrawTopBGWindow();

		// ウィンドウ前景描画処理
		DrawFrontWindow();

		// フォント描画処理
		DrawFont();

		if (bPause == true)
		{// ポーズが使用されている場合
			// ポーズの描画処理
			DrawPause();
		}

		// ウィンドウ一番前景描画処理
		DrawTopFrontWindow();

		// フェード描画処理
		DrawFade();

		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=========================================================================================================================
// デバイスの取得
//=========================================================================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=========================================================================================================================
// 画面設定処理
//=========================================================================================================================
void SetMode(MODE mode)
{
	switch (g_Mode)
	{// 画面終了処理
	case MODE_TITLE:		// タイトル
		UninitTitle();
		break;
	case MODE_GAME:			// ゲーム
		UninitGame();
		break;
	case MODE_RESULT:		// リザルト
		UninitResult();
		break;
	case MODE_RANKING:		// ランキング
		UninitRanking();
		break;
	}

	switch (mode)
	{// 画面初期化処理
	case MODE_TITLE:		// タイトル
		InitTitle();
		break;
	case MODE_GAME:			// ゲーム
		InitGame();
		break;
	case MODE_RESULT:		// リザルト
		InitResult();
		break;
	case MODE_RANKING:		// ランキング
		InitRanking();
		break;
	}

	g_Mode = mode;			// モードを設定
}

//=========================================================================================================================
// 画面取得処理
//=========================================================================================================================
MODE GetMode(void)
{
	return g_Mode;			// 値を返す
}