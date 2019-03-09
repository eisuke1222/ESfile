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
#include "resource.h"	// リソース

#include "font.h"		// フォント
#include "renderer.h"	// レンダラー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"異世界転生したら巨人に追われた件"	// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// ウィンドウプロシージャ

bool openReadFile(void);

bool ScreenShot(char *cBmpName);												// スクリーンショット
void DrawCursor(HDC hdc);														// カーソル描画
bool WriteBitmap(LPTSTR lpszFileName, int nWidth, int nHeight, LPVOID lpBits);	// ビットマップ生成
HBITMAP CreateBackbuffer(int nWidth, int nHeight);								// テクスチャ生成

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// メモリリーク検出
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//ShowCursor(false);							// カーソル表示

	CManager *pManager = NULL;

	HICON hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));	// アイコン読み込み

#ifdef _DEBUG
	LPCSTR lpMenu = "IDR_MENU1";					// メニュー表示
#else
	LPCSTR lpMenu = NULL;							// メニュー非表示
#endif

	lpMenu = NULL;

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),							// WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,									// 表示するウィンドウのスタイルを設定
		WndProc,									// ウィンドウプロシージャのアドレスを指定
		0,											// 通常は指定しないため"0"を指定
		0,											// 通常は指定しないため"0"を指定
		hInstance,									// WinMainの引数のインスタンスハンドルを指定
		hIcon,										// タスクバーに使用するアイコンを指定
		LoadCursor(NULL, IDC_ARROW),				// 使用するマウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1),					// ウィンドウのクライアント領域の背景色を設定
		lpMenu,										// Windowsにつけるメニューを設定
		CLASS_NAME,									// ウィンドウクラスの名前
		hIcon										// ファイルアイコンに使用するアイコンを指定
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
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

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
		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
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
				static int nCntFps = 0;
				dwCurrentTime = timeGetTime();		// 現在の時間を取得
				if ((dwCurrentTime - dwFPSLastTime) >= 500)
				{// 0.5秒ごとに実行
					nCntFps = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
					dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
					dwFrameCount = 0;
				}
			
				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{// 1/60秒経過
					dwExecLastTime = dwCurrentTime;	// 現在の時間を保存
					CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_DEBUG);

					// デバッグ情報表示用フォント(FPS更新)
					if (pFont != NULL)
					{// NULL以外の場合
						pFont->SetMessage(CFont::SETTYPE_NEW, "FPS:%d\n", nCntFps);
					}
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
	int nChackNum = 0;				// 現在の選択メニュー
	UINT uState = 0;				// メニュー情報取得用

//	HDC         hdc;
//	PAINTSTRUCT ps;

	switch(uMsg)
	{
	case WM_CREATE:	
		// 表示メニュー初期化
		CheckMenuRadioItem(GetMenu(hWnd), MENU_ITEM2_1, MENU_ITEM2_2, LOWORD(MENU_ITEM2_1), MF_BYCOMMAND);

		break;
	case WM_DESTROY:				// ×ボタンが押された
		PostQuitMessage(0);			// "WM_QUIT"メッセージを送る

		break;
	case WM_KEYDOWN:				// キーが押された
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
		
			break;
		
		case VK_F9:				// [ESC]キーが押された
			ScreenShot("capture");

			break;
		}

		break;
	case WM_PAINT:
		/*hdc = BeginPaint(hWnd, &ps);
		
		EndPaint(hWnd, &ps);*/
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case MENU_ITEM1_1:
			//DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する

			if (openReadFile() == FALSE) PostMessage(hWnd, WM_CLOSE, 0, 0);

			break;
		case MENU_ITEM1_2:
			//DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する

			if(ScreenShot("capture") == true)
			{
				MessageBox(NULL, TEXT("ファイルを作成しました。"), TEXT("OK"), MB_OK);
			}
			else
			{
				MessageBox(NULL, TEXT("ファイルの作成に失敗しました。"), NULL, MB_ICONWARNING);
			}

			break;
		case MENU_ITEM2_1:
		case MENU_ITEM2_2:
			for (int nCntMenu = MENU_ITEM2_1; nCntMenu < MENU_ITEM2_2; nCntMenu++)
			{// カウント
				uState = GetMenuState(GetMenu(hWnd), nCntMenu, MF_BYCOMMAND);	// 状態取得

				if (uState == 520)
				{// 選択されている
					nChackNum = nCntMenu;		// 番号保存
					break;
				}
			}

			CheckMenuRadioItem(GetMenu(hWnd), MENU_ITEM2_1, MENU_ITEM2_2, LOWORD(wParam), MF_BYCOMMAND);	// 選択メニュー更新

			if (nChackNum != wParam)
			{// 番号が違う場合

			}

			break;
		case MENU_ITEM3_1:			
			uState = GetMenuState(GetMenu(hWnd), MENU_ITEM3_1, MF_BYCOMMAND);	// 状態取得

			if (uState & MFS_CHECKED)
			{//チェックされてたからチェックはずす
				CheckMenuItem(GetMenu(hWnd), MENU_ITEM3_1, MF_BYCOMMAND | MFS_UNCHECKED);
			}
			else
			{//チェックされてなかったからチェックする
				CheckMenuItem(GetMenu(hWnd), MENU_ITEM3_1, MF_BYCOMMAND | MFS_CHECKED);
			}

			break;
		case MENU_ITEM3_2:
			uState = GetMenuState(GetMenu(hWnd), MENU_ITEM3_2, MF_BYCOMMAND);	// 状態取得

			if (uState & MFS_CHECKED)
			{//チェックされてたからチェックはずす
				CheckMenuItem(GetMenu(hWnd), MENU_ITEM3_2, MF_BYCOMMAND | MFS_UNCHECKED);
			}
			else
			{//チェックされてなかったからチェックする
				CheckMenuItem(GetMenu(hWnd), MENU_ITEM3_2, MF_BYCOMMAND | MFS_CHECKED);
			}
			break;
		}

		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を返す
}

//=============================================================================
// スクリーンショット
//=============================================================================
bool ScreenShot(char *cBmpName)
{
	HDC     hdc;
	HWND    hwndDesk;
	RECT    rc;
	BITMAP  bm;
	HBITMAP hbmp;
	HBITMAP hbmpPrev;
	bool bSuccess = false;				// 生成したか

	hwndDesk = GetForegroundWindow();	// フォアグラウンドのウィンドウハンドル取得
	GetClientRect(hwndDesk, &rc);		// クライアントのウィンドウサイズ取得

	hdc = CreateCompatibleDC(NULL);

	hbmp = CreateBackbuffer((rc.right - rc.left), (rc.bottom - rc.top));

	hbmpPrev = (HBITMAP)SelectObject(hdc, hbmp);

	BitBlt(hdc, 0, 0, rc.right, rc.bottom, GetDC(hwndDesk), 0, 0, SRCCOPY);			// ビットマップ生成
	//DrawCursor(hdc);

	GetObject(hbmp, sizeof(BITMAP), &bm);

	FILE *pFile = NULL;					// ファイルポインタ
	char cName[128];					// 名前管理
	int nNum = 0;						// 名前番号

	while (1)
	{
		wsprintf(cName, "data\\TEXTURE\\ScreenShot\\%s_%d.bmp", cBmpName, nNum);	// 名前作成
		pFile = fopen(cName, "r");		// ファイルを開く

		if (pFile == NULL)
		{// 存在しない
			break;
		}
		else
		{// 存在する
			fclose(pFile);				// ファイルを閉じる
			nNum++;						// 番号を進める
		}
	}

	bSuccess = (bool)WriteBitmap(TEXT(cName), rc.right, rc.bottom, bm.bmBits);		// テクスチャ生成

	SelectObject(hdc, hbmpPrev);
	DeleteObject(hbmp);
	DeleteDC(hdc);
	
	return bSuccess;
}

//=============================================================================
// ビットマップ生成
//=============================================================================
HBITMAP CreateBackbuffer(int nWidth, int nHeight)
{
	LPVOID           lp;
	BITMAPINFO       bmi;
	BITMAPINFOHEADER bmiHeader;

	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = nWidth;
	bmiHeader.biHeight = nHeight;
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;

	bmi.bmiHeader = bmiHeader;

	return CreateDIBSection(NULL, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS, &lp, NULL, 0);
}

//=============================================================================
// カーソル描画
//=============================================================================
void DrawCursor(HDC hdc)
{
	int        x, y;
	CURSORINFO cursorInfo;
	ICONINFO   iconInfo;

	cursorInfo.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&cursorInfo);

	GetIconInfo(cursorInfo.hCursor, &iconInfo);

	x = cursorInfo.ptScreenPos.x - iconInfo.xHotspot;
	y = cursorInfo.ptScreenPos.y - iconInfo.yHotspot;
	DrawIcon(hdc, x, y, cursorInfo.hCursor);
}

//=============================================================================
// テクスチャ作成
//=============================================================================
bool WriteBitmap(LPTSTR lpszFileName, int nWidth, int nHeight, LPVOID lpBits)
{
	HANDLE           hFile;
	DWORD            dwResult;
	DWORD            dwSizeImage;
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;

	hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	dwSizeImage = (nHeight) * ((3 * (nWidth) + 3) / 4) * 4;

	ZeroMemory(&bmfHeader, sizeof(BITMAPFILEHEADER));
	bmfHeader.bfType = *(LPWORD)"BM";
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwSizeImage;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	WriteFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwResult, NULL);

	ZeroMemory(&bmiHeader, sizeof(BITMAPINFOHEADER));
	bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmiHeader.biWidth = (nWidth);
	bmiHeader.biHeight = (nHeight);
	bmiHeader.biPlanes = 1;
	bmiHeader.biBitCount = 24;
	bmiHeader.biSizeImage = dwSizeImage;
	bmiHeader.biCompression = BI_RGB;

	WriteFile(hFile, &bmiHeader, sizeof(BITMAPINFOHEADER), &dwResult, NULL);

	WriteFile(hFile, lpBits, dwSizeImage, &dwResult, NULL);

	CloseHandle(hFile);

	return true;
}

bool openReadFile()
{
	char    buf[1024];

	OPENFILENAME    ofn;
	char            filename[256], msg[300];
	HANDLE          hFile;
	DWORD           dwBytes;

	char Path[256];
	GetModuleFileName(NULL, Path, MAX_PATH);

	char drive[MAX_PATH + 1]
		, dir[MAX_PATH + 1]
		, fname[MAX_PATH + 1]
		, ext[MAX_PATH + 1];

	_splitpath(Path, drive, dir, fname, ext);//パス名を構成要素に分解します
	
	char cPassDir[MAX_PATH + 1];
	wsprintf(cPassDir, "%s%s", drive, dir);

	filename[0] = '\0';														//忘れるとデフォルトファイル名に変な文字列が表示される
	memset(&ofn, 0, sizeof(OPENFILENAME));									//構造体を0でクリア
	ofn.lStructSize = sizeof(OPENFILENAME);									//ファイルのサイズ
	ofn.lpstrInitialDir = cPassDir;
	ofn.lpstrFilter = "text file(*.txt)\0*.txt\0all file(*.*)\0*.*\0\0";	//開くファイルの種類
	ofn.lpstrFile = filename;												//検索するファイル名
	ofn.nMaxFile = sizeof(filename);										//開けるファイル文字の最大数
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";

	if (GetOpenFileName(&ofn) != TRUE)
		return FALSE;

	hFile = CreateFile(filename, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		wsprintf(msg, "%s failed to open.", filename);
		MessageBox(NULL, msg, "Section5.2 open file dialog", MB_OK);
		return FALSE;
	}
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	ReadFile(hFile, buf, sizeof(buf) - 1, &dwBytes, NULL);
	*((u_char*)buf + dwBytes) = 0;  //読み終わりに'\0'を挿入
	CloseHandle(hFile);

	return TRUE;
}