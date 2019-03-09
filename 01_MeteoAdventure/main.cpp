//=========================================================================================================================
//
// アクションゲーム処理 [main.cpp]
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
#include "bgwindow.h"	// 背景
#include "fade.h"		// フェード
#include "sound.h"		// サウンド
#include "input.h"		// キーボード

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define CLASS_NAME		"WindowClass"				// ウインドウクラスの名前
#define WINDOW_NAME		"MeteoAdventure"			// ウインドウの名前(キャプション名)

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);		// 初期化処理
void Uninit(void);												// 終了処理
void Update(void);												// 更新処理
void Draw(void);												// 描画処理

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3D9				g_pD3D			= NULL;					// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9		g_pD3DDevice	= NULL;					// Direct3Dデバイスへのポインタ
bool					g_bWindowMode	= true;					// ウィンドウモード
MODE					g_Mode			= MODE_TITLE;			// 初期画面状態

//=========================================================================================================================
// メイン関数
//=========================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,									// 表示するウィンドウのスタイルを設定
		WindowProc,									// ウィンドウプロシージャのアドレスを指定
		0,											// 通常は指定しないため"0"を指定
		0,											// 通常は指定しないため"0"を指定
		hInstance,									// WinMainの引数のインスタンスハンドルを指定
		LoadIcon(NULL, IDI_APPLICATION),			// タスクバーに使用するアイコンを指定
		LoadCursor(NULL, IDC_ARROW),				// 使用するマウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),					// ウィンドウのクライアント領域の背景色を設定
		NULL,										// Windowsにつけるメニューを設定
		CLASS_NAME,									// ウィンドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)				// ファイルアイコンに使用するアイコンを指定
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// クライアント領域の設定
	HWND hWnd;										// ウィンドウハンドル（識別子）
	MSG msg;										// メッセージを格納する変数
	DWORD dwCurrentTime;							// 現在時刻
	DWORD dwExecLastTime;							// 最後に処理した時刻

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの生成
	hWnd = CreateWindowEx
	(
		0,											// 拡張ウィンドウスタイル
		CLASS_NAME,									// ウィンドウクラスの名前
		WINDOW_NAME,								// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,						// ウィンドウスタイル
		CW_USEDEFAULT,								// ウィンドウの左上X座標		/*重なると少しずつずらしてくれる    */
		CW_USEDEFAULT,								// ウィンドウの左上Y座標		/*固定値だと同じ場所に表示され続ける*/
		(rect.right - rect.left),					// ウィンドウの幅
		(rect.bottom - rect.top),					// ウィンドウの高さ
		NULL,										// 親ウィンドウのハンドル
		NULL,										// メニューハンドルまたはウィンドウID
		hInstance,									// インスタンスハンドル
		NULL										// ウィンドウ作成データ
	);

	// 初期化処理(ウィンドウを生成してから行う) ※フルスクリーン変更 FALSE:TRUE?
	if (FAILED(Init(hInstance, hWnd, g_bWindowMode)))
	{
		return -1;
	}

	// ウインドウの表示(初期化処理の後に行う)
	ShowWindow(hWnd, nCmdShow);						// ウィンドウの表示（ウィンドウの表示状態を設定）
	UpdateWindow(hWnd);								// ウィンドウの反映（ウィンドウのクライアント領域の更新）

	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();					// 現在時刻を取得

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)	// メッセージを取得しなかった場合"0"を返す
		{// Windowsの処理
			if (msg.message == WM_QUIT)
			{	// "WM_QUIT"メッセージが送られてきたらループを抜ける
				break;
			}
			else
			{
				// メッセージの翻訳
				TranslateMessage(&msg);				// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);				// ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();			// 現在時刻を取得

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// "60分1秒"経過
				dwExecLastTime = dwCurrentTime;		// 処理した時刻を保持

				// 更新処理
				Update();

				// 描画処理
				Draw();
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
// ウインドウプロシージャ
//=========================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));									// ワークをゼロクリア
	d3dpp.BackBufferWidth				 = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight				 = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat				 = d3ddm.Format;				// バックバッファの形式
	d3dpp.BackBufferCount				 = 1;							// バックバッファの数
	d3dpp.SwapEffect					 = D3DSWAPEFFECT_DISCARD;		// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil		 = TRUE;						// デプスバッファ(Ｚバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat		 = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed						 = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz	 = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート(現在の速度に合わせる)
	d3dpp.PresentationInterval			 = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル(VSyncを待って描画)

	// Direct3Dデバイスの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,					// ディスプレイアダプタ
		D3DDEVTYPE_HAL,													// デバイスタイプ
		hWnd,															// フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,							// デバイス作成制御の組み合わせ
		&d3dpp,															// デバイスのプレゼンテーションパラメータ
		&g_pD3DDevice)))												// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//	キーボード初期化処理
	InitKeyboard(hInstance, hWnd);

	// サウンド初期化処理
	InitSound(hWnd);

	// フェード初期化処理
	InitFade(MODE_TITLE);

	// 背景初期化処理
	InitBGWindow();

	// 画面初期化処理
	SetMode(g_Mode);

	return S_OK;
}

//=========================================================================================================================
// 終了処理
//=========================================================================================================================
void Uninit(void)
{
	// キーボード終了処理
	UninitKeyboard();

	// サウンド終了処理
	UninitSound();

	// 背景終了処理
	UninitBGWindow();

	// 画面終了処理
	UninitTitle();		// タイトル
	UninitGame();		// ゲーム
	UninitResult();		// リザルト

	// フェード終了処理
	UninitFade();

	// Direct3Dデバイスの開放
	if (g_pD3DDevice != NULL)
	{// NULL以外の場合
		g_pD3DDevice->Release();	// 開放
		g_pD3DDevice = NULL;		// NULLへ
	}

	// Direct3Dオブジェクトの開放
	if (g_pD3D != NULL)
	{// NULL以外の場合
		g_pD3D->Release();			// 開放
		g_pD3D = NULL;				// NULLへ
	}
}

//========================================================================================================================
// 更新処理
//========================================================================================================================
void Update(void)
{
	// キーボード更新処理
	UpdateKeyboard();

	// 背景更新処理
	UpdateBGWindow();

	switch (g_Mode)
	{// 画面更新処理
	case MODE_TITLE:		// タイトル
		UpdateTitle();
		break;;
	case MODE_GAME:			// ゲーム
		UpdateGame();
		break;
	case MODE_RESULT:		// リザルト
		UpdateResult();
		break;;
	}

	// フェード更新処理
	UpdateFade();
}

//=========================================================================================================================
// 描画処理
//=========================================================================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear
	(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0
	);

	// 描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// 背景描画処理
		DrawBGWindow();

		switch (g_Mode)
		{// 画面描画処理
		case MODE_TITLE:	// タイトル
			DrawTitle();
			break;
		case MODE_GAME:		// ゲーム
			DrawGame();
			break;
		case MODE_RESULT:	// リザルト
			DrawResult();
			break;
		}

		// フェード描画処理
		DrawFade();

		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=========================================================================================================================
// デバイスの取得処理
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
		StopSound(SOUND_LABEL_BGM_TITLE);
		break;
	case MODE_GAME:			// ゲーム
		UninitGame();
		break;
	case MODE_RESULT:		// リザルト
		UninitResult();
		break;
	}

	// 背景破棄処理
	DeleteBGWindow();

	switch (mode)
	{// 画面初期化処理
	case MODE_TITLE:		// タイトル
		InitTitle();
		PlaySound(SOUND_LABEL_BGM_TITLE);
		break;
	case MODE_GAME:			// ゲーム
		InitGame();
		break;
	case MODE_RESULT:		// リザルト
		InitResult();
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