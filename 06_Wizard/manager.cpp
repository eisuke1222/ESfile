//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "light.h"			// ライト
#include "fade.h"			// フェード
#include "scene.h"			// シーン
#include "imgui_es.h"		// ImGui_ES

#include "text.h"			// テキスト
#include "title.h"			// タイトル
#include "game.h"			// ゲーム

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
CManager::MODE		CManager::m_mode			= CManager::MODE_NONE;	// モード
CSound				*CManager::m_pSound			= NULL;					// サウンド
CInputKeyboard		*CManager::m_pInputKeyboard = NULL;					// キーボード
CInputMouse			*CManager::m_pInputMouse	= NULL;					// マウス
CInputDIPad			*CManager::m_pInputDIPad	= NULL;					// DIパッド
CInputXPad			*CManager::m_pInputXPad		= NULL;					// Xパッド
CRenderer			*CManager::m_pRenderer		= NULL;					// レンダラー
CCamera				*CManager::m_pCamara[CManager::CAMERA_MAX] = {NULL};// カメラ
CLight				*CManager::m_pLight			= NULL;					// ライト

CTitle				*CManager::m_pTitle			= NULL;					// タイトル
CGame				*CManager::m_pGame			= NULL;					// ゲーム
CResult				*CManager::m_pResult		= NULL;					// リザルト

int					CManager::m_nPlayNum		= 0;					// プレイ人数

//=============================================================================
// コンストラクタ								(public)	*** CManager ***
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// デストラクタ									(public)	*** CManager ***
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理									(public)	*** CManager ***
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// サウンド
	{
		if (m_pSound == NULL)
			m_pSound = new CSound;					// メモリ確保
		else
			MessageBox(0, "(manager「Init] : SoundがNULLでない", "警告", MB_OK);

		if (m_pSound != NULL)
			if (FAILED(m_pSound->Init(hWnd)))		// 初期化処理
				return -1;		// 値を返す
	}

	// キーボード
	{
		if (m_pInputKeyboard == NULL)
			m_pInputKeyboard = new CInputKeyboard;	// メモリ確保
		else
			MessageBox(0, "(manager「Init] : InputKeyboardがNULLでない", "警告", MB_OK);

		if (m_pInputKeyboard != NULL)
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))	// 初期化処理
			{				
				m_pInputKeyboard->Uninit();			// 終了処理

				delete m_pInputKeyboard;			// メモリ開放
				m_pInputKeyboard = NULL;			// NULL
			}
	}
	// マウス
	{
		if (m_pInputMouse == NULL)
			m_pInputMouse = new CInputMouse;		// メモリ確保
		else
			MessageBox(0, "(manager「Init] : InputMouseがNULLでない", "警告", MB_OK);

		if (m_pInputMouse != NULL)
			if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))		// 初期化処理
			{
				m_pInputMouse->Uninit();			// 終了処理

				delete m_pInputMouse;				// メモリ開放
				m_pInputMouse = NULL;				// NULL
			}
	}
	// DIパッド
	{
		if (m_pInputDIPad == NULL)
			m_pInputDIPad = new CInputDIPad;		// メモリ確保
		else
			MessageBox(0, "(manager「Init] : InputDIPadがNULLでない", "警告", MB_OK);

		if (m_pInputDIPad != NULL)
			if (FAILED(m_pInputDIPad->Init(hInstance, hWnd)))		// 初期化処理
			{				
				m_pInputDIPad->Uninit();			// 終了処理

				delete m_pInputDIPad;				// メモリ開放
				m_pInputDIPad = NULL;				// NULL
			}
	}
	// Xパッド
	{
		if (m_pInputXPad == NULL)
			m_pInputXPad = new CInputXPad;			// メモリ確保
		else
			MessageBox(0, "(manager「Init] : InputXPadがNULLでない", "警告", MB_OK);

		if (m_pInputXPad != NULL)
			if (FAILED(m_pInputXPad->Init(hInstance, hWnd)))
			{
				m_pInputDIPad->Uninit();			// 終了処理

				delete m_pInputDIPad;				// メモリ開放
				m_pInputDIPad = NULL;				// NULL
			}
	}

	// レンダラー
	{
		if (m_pRenderer == NULL)
			m_pRenderer = new CRenderer;			// メモリ確保
		else
			MessageBox(0, "(manager「Init] : RendererがNULLでない", "警告", MB_OK);

		if (m_pRenderer != NULL)
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))	// 初期化処理
				return -1;		// 値を返す
	}

	// カメラ
	{
		for (int nCount = 0; nCount < CAMERA_MAX; nCount++)
		{// カメラ数カウント
			if (m_pCamara[nCount] == NULL)
			{// NULLの場合
				if (nCount == 0)
					m_pCamara[nCount] = new CCameraMain;	// メモリ確保
				else if (nCount == 1)
					m_pCamara[nCount] = new CCameraMap;		// メモリ確保
			}
			else
				MessageBox(0, "(manager「Init] : CameraがNULLでない", "警告", MB_OK);

			if (m_pCamara[nCount] != NULL)
				m_pCamara[nCount]->Init();			// 初期化処理
		}

		// ミニマップ設定
		m_pCamara[CAMERA_GAME_MAP]->SetViewPort(SCREEN_WIDTH - (SCREEN_WIDTH / 6) - (SCREEN_WIDTH / 80), 0, (SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 80), (SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 80));
	}

	// ライト
	{
		if (m_pLight == NULL)
			m_pLight = new CLight;					// メモリ確保
		else
			MessageBox(0, "(manager「Init] : LightがNULLでない", "警告", MB_OK);

		if (m_pLight != NULL)
			m_pLight->Init();						// 初期化処理
	}

	CText::Load();			// テキストロード

	SetMode(MODE_TITLE);	// モード設定

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理										(public)	*** CManager ***
//=============================================================================
void CManager::Uninit(void)
{
	// サウンド
	{
		if (m_pSound != NULL)
		{// NULL以外の場合
			m_pSound->Uninit();			// 終了処理

			delete m_pSound;			// メモリ開放
			m_pSound = NULL;			// NULL
		}
	}

	// キーボード
	{
		if (m_pInputKeyboard != NULL)
		{// NULL以外の場合
			m_pInputKeyboard->Uninit();	// 終了処理

			delete m_pInputKeyboard;	// メモリ開放
			m_pInputKeyboard = NULL;	// NULL
		}
	}
	// マウス
	{
		if (m_pInputMouse != NULL)
		{// NULL以外の場合
			m_pInputMouse->Uninit();	// 終了処理

			delete m_pInputMouse;		// メモリ開放
			m_pInputMouse = NULL;		// NULL
		}
	}
	// DIパッド
	{
		if (m_pInputDIPad != NULL)
		{// NULL以外の場合
			m_pInputDIPad->Uninit();	// 終了処理

			delete m_pInputDIPad;		// メモリ開放
			m_pInputDIPad = NULL;		// NULL
		}
	}
	// Xパッド
	{
		if (m_pInputXPad != NULL)
		{// NULL以外の場合
			m_pInputXPad->Uninit();		// 終了処理

			delete m_pInputXPad;		// メモリ開放
			m_pInputXPad = NULL;		// NULL
		}
	}

	switch (m_mode)
	{// モード
	case CManager::MODE_TITLE:			// タイトル
		if (m_pTitle != NULL)
		{// NULL以外の場合
			m_pTitle->Unload();			// アンロード
			m_pTitle->Uninit();			// 終了処理

			delete m_pTitle;			// メモリ開放
			m_pTitle = NULL;			// NULLへ
		}

		break;
	case CManager::MODE_GAME:			// ゲーム
		if (m_pGame != NULL)
		{// NULL以外の場合
			m_pGame->Unload();			// アンロード
			m_pGame->Uninit();			// 終了処理

			delete m_pGame;				// メモリ開放
			m_pGame = NULL;				// NULLへ
		}

		break;
	case CManager::MODE_RESULT:			// リザルト

		break;
	}

	// レンダラー
	{
		if (m_pRenderer != NULL)
		{// NULL以外の場合
			m_pRenderer->Uninit();		// 終了処理

			delete m_pRenderer;			// レンダラーメモリ開放
			m_pRenderer = NULL;			// NULL
		}
	}

	// カメラ
	{
		for (int nCount = 0; nCount < CAMERA_MAX; nCount++)
		{// カメラ数カウント
			if (m_pCamara[nCount] != NULL)
			{// NULL以外の場合
				m_pCamara[nCount]->Uninit();// 終了処理

				delete m_pCamara[nCount];	// カメラメモリ開放
				m_pCamara[nCount] = NULL;	// NULL
			}
		}
	}

	// ライト
	{
		if (m_pLight != NULL)
		{// NULL以外の場合
			m_pLight->Uninit();			// 終了処理

			delete m_pLight;			// ライトメモリ開放
			m_pLight = NULL;			// NULL
		}
	}

	CText::Unload();	// テキストアンロード
}

//=============================================================================
// 更新処理										(public)	*** CManager ***
//=============================================================================
void CManager::Update(void)
{
	// キーボード
	{
		if (m_pInputKeyboard != NULL)
			m_pInputKeyboard->Update();	// 更新処理
	}
	// マウス
	{
		if (m_pInputMouse != NULL)
			m_pInputMouse->Update();	// 更新処理
	}
	// DIパッド
	{
		if (m_pInputDIPad != NULL)
			m_pInputDIPad->Update();	// 更新処理
	}
	// Xパッド
	{
		if (m_pInputXPad != NULL)
			m_pInputXPad->Update();		// 更新処理
	}

	DebugFont();	// デバッグフォント

	switch (m_mode)
	{// モード
	case CManager::MODE_TITLE:		// タイトル
		if (m_pTitle != NULL)
			m_pTitle->Update();		// 更新処理

		break;
	case CManager::MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
			m_pGame->Update();		// 更新処理

		break;
	case CManager::MODE_RESULT:		// リザルト

		break;
	}

	// レンダラー
	{
		if (m_pRenderer != NULL)
			m_pRenderer->Update();	// 更新処理
	}

	// カメラ
	{
		for (int nCount = 0; nCount < CAMERA_MAX; nCount++)
			if (m_pCamara[nCount] != NULL)
				m_pCamara[nCount]->Update();	// 更新処理
	}

	// ライト
	{
		if (m_pLight != NULL)
			m_pLight->Update();		// 更新処理
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7))
	{
		CManager::SetMode(CManager::MODE_GAME);
	}
}

//=============================================================================
// 描画処理										(public)	*** CManager ***
//=============================================================================
void CManager::Draw(void)
{
	// レンダラー
	{
		if (m_pRenderer != NULL)
			m_pRenderer->Draw();	// 描画処理
	}

	// カメラ
	{
		/*for (int nCount = 0; nCount < CAMERA_MAX; nCount++)
			if (m_pCamara[nCount] != NULL)
				m_pCamara[nCount]->SetCamera();	// 描画処理
		*/
	}
}

//=============================================================================
// モード設定処理								(public)	*** CManager ***
//=============================================================================
void CManager::SetMode(CManager::MODE mode)
{
	switch (m_mode)
	{// モード（現在）
	case CManager::MODE_TITLE:			// タイトル
		if (m_pTitle != NULL)
		{// NULL以外の場合
			if (m_mode != mode)			// モードが違ったら
				m_pTitle->Unload();		// アンロード
			m_pTitle->Uninit();			// 終了処理

			delete m_pTitle;			// メモリ開放
			m_pTitle = NULL;			// NULLへ
		}

		break;
	case CManager::MODE_GAME:			// ゲーム
		if (m_pGame != NULL)
		{// NULL以外の場合
			if (m_mode != mode)			// モードが違ったら
				m_pGame->Unload();		// アンロード
			m_pGame->Uninit();			// 終了処理

			delete m_pGame;				// メモリ開放
			m_pGame = NULL;				// NULLへ
		}

		break;
	case CManager::MODE_RESULT:			// リザルト

		break;
	}

	char cMode[32];						// モード名
	ConMode(mode, cMode);				// モード文字変換

	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);		// フォント取得

	// フォント設定
	if (pFont != NULL)
	{// NULL以外の場合
		pFont->SetMessage(CFont::SETTYPE_LOG, "\n");
		pFont->SetMessage(CFont::SETTYPE_LOG, "Mode Change[%s]-----\n", cMode);
	}

	// ImGuiLog設定
	CImgui_ES::SetMessage("\n");
	CImgui_ES::SetMessage("Mode Change[%s]-----\n", cMode);

	if (m_pCamara[CAMERA_MAIN] != NULL)
		m_pCamara[CAMERA_MAIN]->Uninit();
	if (m_pCamara[CAMERA_GAME_MAP] != NULL)
		m_pCamara[CAMERA_GAME_MAP]->Uninit();

	int modeOld = m_mode;				// 前回モードを記憶
	m_mode = mode;						// モード変更

	switch (mode)
	{// モード（次回）
	case CManager::MODE_TITLE:			// タイトル
		if (m_pTitle == NULL)
		{// NULLの場合
			m_pTitle = new CTitle;		// メモリ確保

			if (m_pTitle != NULL)
			{// NULL以外の場合
				if (modeOld != mode)	// モードが違ったら
					m_pTitle->Load();	// ロード
				m_pTitle->Init();		// 初期化処理
			}
		}

		break;
	case CManager::MODE_GAME:			// ゲーム
		if (m_pGame == NULL)
		{// NULLの場合
			m_pGame = new CGame;		// メモリ確保

			if (m_pGame != NULL)
			{// NULL以外の場合
				if (modeOld != mode)	// モードが違ったら
					m_pGame->Load();	// ロード
				m_pGame->Init();		// 初期化処理
			}
		}

		break;
	case CManager::MODE_RESULT:			// リザルト

		break;
	}
}

void CManager::ConMode(CManager::MODE mode, char *cMode)
{
	switch (mode)
	{// モード（次回）
	case CManager::MODE_TITLE:			// タイトル
		wsprintf(cMode, "Title");
		break;
	case CManager::MODE_GAME:			// ゲーム
		wsprintf(cMode, "Game");
		break;
	case CManager::MODE_RESULT:			// リザルト
		wsprintf(cMode, "Result");
		break;
	}
}

//=============================================================================
// デバッグフォント処理									(public)	*** CManager ***
//=============================================================================
void CManager::DebugFont(void)
{
	LPDIRECT3DDEVICE9 pDevice = m_pRenderer->GetDevice();

	CFont *pFont[CFont::FONTNUM_MAX] = {};

	// 初期化
	for (int nCntFont = 0; nCntFont < CFont::FONTNUM_MAX; nCntFont++)
	{
		pFont[nCntFont] = CRenderer::GetFont(nCntFont);
	}

	// デバッグ情報表示用フォント(FPS更新)
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_TAB) == true)
	{
		if (pFont[CFont::FONTNUM_LOG] != NULL)
		{// NULL以外の場合
			pFont[CFont::FONTNUM_LOG]->SetMessage(CFont::SETTYPE_NEW, "");
		}
	}

	if (pFont[CFont::FONTNUM_DATA] != NULL)
	{
		pFont[CFont::FONTNUM_DATA]->SetMessage(CFont::SETTYPE_NEW, "");
	}

	if (pFont[CFont::FONTNUM_TEST] != NULL)
	{
		pFont[CFont::FONTNUM_TEST]->SetMessage(CFont::SETTYPE_NEW, "");
	}
}