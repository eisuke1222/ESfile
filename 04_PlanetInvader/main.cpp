//=============================================================================
//
// メイン処理 [main.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"Planet Invader "	// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
#ifdef _DEBUG
int						g_nCountFPS;				// FPSカウンタ
#endif
bool					g_bWindowMode = false;

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (g_bWindowMode == true)
	{
		ShowCursor(false);
	}

	CManager *pManager = NULL;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,									// 表示するウィンドウのスタイルを設定
		WndProc,									// ウィンドウプロシージャのアドレスを指定
		0,											// 通常は指定しないため"0"を指定
		0,											// 通常は指定しないため"0"を指定
		hInstance,									// WinMainの引数のインスタンスハンドルを指定
		NULL,										// タスクバーに使用するアイコンを指定
		LoadCursor(NULL, IDC_ARROW),				// 使用するマウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),					// ウィンドウのクライアント領域の背景色を設定
		NULL,										// Windowsにつけるメニューを設定
		CLASS_NAME,									// ウィンドウクラスの名前
		NULL										// ファイルアイコンに使用するアイコンを指定
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };// クライアント領域の設定
	HWND hWnd;										// ウィンドウハンドル（識別子）
	MSG msg;										// メッセージを格納する変数
	DWORD dwCurrentTime;							// 現在時刻
	DWORD dwExecLastTime;							// 最後に処理した時刻
	DWORD dwFrameCount;								// FPS値カウント
	DWORD dwFPSLastTime;							// FPS値最後に処理した時刻

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, true);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,					// ウィンドウクラスの名前
		WINDOW_NAME,								// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,						// ウィンドウスタイル
		CW_USEDEFAULT,								// ウィンドウの左上X座標		/*重なると少しずつずらしてくれる    */
		CW_USEDEFAULT,								// ウィンドウの左上Y座標		/*固定値だと同じ場所に表示され続ける*/
		(rect.right - rect.left),					// ウィンドウの幅
		(rect.bottom - rect.top),					// ウィンドウの高さ
		NULL,										// 親ウィンドウのハンドル
		NULL,										// メニューハンドルまたはウィンドウID
		hInstance,									// インスタンスハンドル
		NULL);										// ウィンドウ作成データ

	srand((unsigned int)time(0));					// ランダム

	if (pManager == NULL)
	{// NULLの場合
		pManager = new CManager;
	}

	if (pManager != NULL)
	{// NULL以外の場合
		if (FAILED(pManager->Init(hInstance, hWnd, g_bWindowMode)))
		{
			return -1;
		}

		// 分解能を設定
		timeBeginPeriod(1);

		// フレームカウント初期化
		dwCurrentTime =
			dwFrameCount = 0;
		dwExecLastTime =
			dwFPSLastTime = timeGetTime();				// 現在時刻を取得

		// ウインドウの表示
		ShowWindow(hWnd, nCmdShow);						// ウィンドウの表示（ウィンドウの表示状態を設定）
		UpdateWindow(hWnd);								// ウィンドウの反映（ウィンドウのクライアント領域の更新）

		// メッセージループ
		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// メッセージを取得しなかった場合"0"を返す
			{// Windowsの処理
				if (msg.message == WM_QUIT)
				{// PostQuitMessage()が呼ばれたらループ終了
					break;
				}
				else
				{
					// メッセージの翻訳とディスパッチ
					TranslateMessage(&msg);				// 仮想キーメッセージを文字メッセージへ変換
					DispatchMessage(&msg);				// ウィンドウプロシージャへメッセージを送出
				}
			}
			else
			{// DirectXの処理
				dwCurrentTime = timeGetTime();		// 現在の時間を取得
				if ((dwCurrentTime - dwFPSLastTime) >= 500)
				{// 0.5秒ごとに実行
#ifdef _DEBUG
					// FPSを算出
					g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
					dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
					dwFrameCount = 0;
				}

				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{// 1/60秒経過
					dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

					// 更新処理
					pManager->Update();

					// 描画処理
					pManager->Draw();

					dwFrameCount++;
				}
			}
		}
	}

	// 終了処理
	if (pManager != NULL)
	{// NULL以外の場合
		pManager->Uninit();

		delete pManager;
		pManager = NULL;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
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

#ifdef _DEBUG
//=============================================================================
// FPS値取得
//=============================================================================
int GetFPS(void)
{
	return g_nCountFPS;
}
#endif