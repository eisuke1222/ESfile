//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "main.h"
#include "light.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "title.h"
#include "story.h"
#include "titleanim.h"
#include "game.h"
#include "questclear.h"
#include "result.h"
#include "resultover.h"
#include "rank.h"
#include "BG.h"
#include "flontpolygon.h"
#include "titleanim.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"リペア・メモリー"	// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9				g_pD3D			= NULL;				// Direct3Dオブジェクト
LPDIRECT3DDEVICE9		g_pD3DDevice	= NULL;				// デバイスオブジェクト(描画に必要)
bool					g_bWindowMode	= true;				// ウィンドウモード
MODE					g_mode			= MODE_TITLE;		// ゲームのモード設定

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
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
	RECT rect = {0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT};
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

//=============================================================================
// プロシージャ
//=============================================================================
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

//=============================================================================
// 初期化処理
//=============================================================================
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
	d3dpp.BackBufferWidth			 = (UINT)SCREEN_WIDTH;			// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			 = (UINT)SCREEN_HEIGHT;			// ゲーム画面サイズ(高さ)
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

	//初期化処理
	InitSound(hWnd);				//サウンド
	InitKeyboard(hInstance, hWnd);	//キーボード
	InitMouse(hInstance, hWnd);		//マウス
	InitJoystick(hInstance, hWnd);	//ジョイコン
	InitLight();					// ライトの初期化処理
	InitBG();						// 背景の処理
	InitFade(g_mode);				//フェード
	InitTitleAnim();
	SetMode(g_mode);				//画面のモード

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	//終了処理
	UninitSound();		//サウンド
	UninitInput();		//キーボード
	UninitLight();		//ライトの終了処理
	UninitBG();			//背景の処理
	UninitFade();		//フェード

	UninitTitleAnim();

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

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	//更新処理
	UpdateKeyboard();		//キーボード
	UpdateMouse();			//マウス
	UpdateJoystick();		//ジョイコン
	UpdateLight();			// ライトの更新処理
	UpdateBG();				// 背景の処理

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();			//タイトル画面
		break;

	case MODE_STORY:
		UpdateStory();			//ストーリー画面
		break;

	case MODE_GAME:
		UpdateGame();			//ゲーム画面
		break;

	case MODE_QUESTCLEAR:
		UpdateQuestClear();		//クエストクリア画面
		break;

	case MODE_GAMECLEAR:
		UpdateResult();			//リザルト画面
		break;

	case MODE_GAMEOVER:
		UpdateResultOver();		//リザルト[ ゲームオーバー ]画面
		break;
	}

	UpdateTitleAnim();

	UpdateFade();				//フェード
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// カメラの設定
		DrawBG();					//背景の処理
		//DrawFlontPolygon();			//フロントポリゴン

		//描画処理
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();			//タイトル画面
			break;

		case MODE_STORY:
			DrawStory();			//ストーリー画面
			break;

		case MODE_GAME:
			DrawGame();				//ゲーム画面
			break;

		case MODE_QUESTCLEAR:
			DrawQuestClear();		//クエストクリア画面
			break;

		case MODE_GAMECLEAR:
			DrawResult();			//リザルト画面
			break;

		case MODE_GAMEOVER:
			DrawResultOver();		//リザルト[ ゲームオーバー ]画面
			break;
		}

		DrawTitleAnim();

		DrawFade();					//フェード

		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=============================================================================
//	モードの設定処理
//=============================================================================
void SetMode(MODE mode)
{
	//画面を消す
	switch (g_mode)
	{
	case MODE_TITLE:
		StopSound(SOUND_LABEL_BGM_TITLE);
		DeleteAllFlontPolygon();				//フロントポリゴンを消す
		UninitTitle();							//タイトル画面
		break;

	case MODE_STORY:
		StopSound(SOUND_LABEL_BGM_GAMECLEAR);	//サウンド再生停止
		UninitStory();							//ストーリー画面
		break;

	case MODE_GAME:
		UninitGame();							//ゲーム画面
		break;

	case MODE_QUESTCLEAR:
		UninitQuestClear();						//クエストクリア画面
		break;

	case MODE_GAMECLEAR:
		UninitResult();							//リザルト画面
		StopSound(SOUND_LABEL_BGM_GAMECLEAR);	//サウンド再生停止
		break;

	case MODE_GAMEOVER:
		DeleteAllFlontPolygon();				//フロントポリゴンを消す
		UninitResultOver();						//リザルト[ ゲームオーバー ]画面
		StopSound(SOUND_LABEL_BGM_GAMEOVER);	//サウンド再生停止
		break;
	}

	//画面を生成する
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();							//タイトル画面
		PlaySound(SOUND_LABEL_BGM_TITLE);		//サウンド
		break;

	case MODE_STORY:
		PlaySound(SOUND_LABEL_BGM_GAMECLEAR);	//サウンド
		InitStory();							//ストーリー画面
		break;

	case MODE_GAME:
		InitGame();								//ゲーム画面
		//PlaySound(SOUND_LABEL_BGM001);		//サウンド
		break;

	case MODE_QUESTCLEAR:
		InitQuestClear();						//クエストクリア画面
		break;

	case MODE_GAMECLEAR:
		InitResult();							//リザルト画面
		PlaySound(SOUND_LABEL_BGM_GAMECLEAR);	//サウンド
		break;

	case MODE_GAMEOVER:
		InitResultOver();						//リザルト[ ゲームオーバー ]画面
		PlaySound(SOUND_LABEL_BGM_GAMEOVER);	//サウンド
		break;
	}

	//代入
	g_mode = mode;
}

//=============================================================================
//	モードの取得
//============================================================================= 
MODE GetMode(void)
{
	return g_mode;
}