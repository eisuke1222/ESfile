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
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "light.h"			// ライト
#include "renderer.h"		// レンダラー
#include "fade.h"			// フェード
#include "title.h"			// タイトル
#include "game.h"			// ゲーム
#include "tutorial.h"		// チュートリアル
#include "result.h"			// リザルト
#include "scene.h"			// シーン
#include "ranking.h"		// ランキング
#include "pause.h"			// ポーズ

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
CInputDIPad			*CManager::m_pInputDIPad	= NULL;					// DIパッド
CInputXPad			*CManager::m_pInputXPad		= NULL;					// Xパッド
CRenderer			*CManager::m_pRenderer		= NULL;					// レンダラー
CCamera				*CManager::m_pCamara		= NULL;					// カメラ
CLight				*CManager::m_pLight			= NULL;					// ライト
CTitle				*CManager::m_pTitle			= NULL;					// タイトル
CGame				*CManager::m_pGame			= NULL;					// ゲーム
CTutorial			*CManager::m_pTutorial		= NULL;					// チュートリアル
CResult				*CManager::m_pResult		= NULL;					// リザルト
CRanking			*CManager::m_pRanking		= NULL;					// ランキング	
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
		{// NULLの場合
			m_pSound = new CSound;		// サウンドメモリ確保
		}
		else
		{// NULL以外の場合
			MessageBox(0, "(manager「Init] : SoundがNULLでない", "警告", MB_OK);
		}

		if (m_pSound != NULL)
		{// NULL以外の場合
		 // サウンド初期化処理
			if (FAILED(m_pSound->Init(hWnd)))
			{
				return -1;
			}
		}
		else
		{// NULLの場合
			MessageBox(0, "(manager「Init] : SoundがNULL", "警告", MB_OK);
		}
	}

	// キーボード
	{
		if (m_pInputKeyboard == NULL)
		{// NULLの場合
			m_pInputKeyboard = new CInputKeyboard;		// キーボードメモリ確保
		}
		else
		{// NULL以外の場合
			MessageBox(0, "(manager「Init] : InputKeyboardがNULLでない", "警告", MB_OK);
		}

		if (m_pInputKeyboard != NULL)
		{// NULL以外の場合
			// キーボード初期化処理
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{
				return -1;
			}
		}
		else
		{// NULLの場合
			MessageBox(0, "(manager「Init] : InputKeyboardがNULL", "警告", MB_OK);
		}
	}
	// DIパッド
	{
		if (m_pInputDIPad == NULL)
		{// NULLの場合
			m_pInputDIPad = new CInputDIPad;		// DIパッドメモリ確保
		}
		else
		{// NULL以外の場合
			MessageBox(0, "(manager「Init] : InputDIPadがNULLでない", "警告", MB_OK);
		}

		if (m_pInputDIPad != NULL)
		{// NULL以外の場合
			// DIパッド初期化処理
			if (FAILED(m_pInputDIPad->Init(hInstance, hWnd)))
			{
				return -1;
			}
		}
		else
		{// NULLの場合
			MessageBox(0, "(manager「Init] : InputDIPadがNULL", "警告", MB_OK);
		}
	}
	// Xパッド
	{
		if (m_pInputXPad == NULL)
		{// NULLの場合
			m_pInputXPad = new CInputXPad;		// DIパッドメモリ確保
		}
		else
		{// NULL以外の場合
			MessageBox(0, "(manager「Init] : InputXPadがNULLでない", "警告", MB_OK);
		}

		if (m_pInputXPad != NULL)
		{// NULL以外の場合
		 // DIパッド初期化処理
			if (FAILED(m_pInputXPad->Init(hInstance, hWnd)))
			{
				return -1;
			}
		}
		else
		{// NULLの場合
			MessageBox(0, "(manager「Init] : InputXPadがNULL", "警告", MB_OK);
		}
	}

	// レンダラー
	{
		if (m_pRenderer == NULL)
		{// NULLの場合
			m_pRenderer = new CRenderer;			// レンダラーメモリ確保
		}
		else
		{// NULL以外の場合
			MessageBox(0, "(manager「Init] : RendererがNULLでない", "警告", MB_OK);
		}

		if (m_pRenderer != NULL)
		{// NULL以外の場合
			// レンダラー初期化処理
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
			{
				return -1;
			}
		}
		else
		{// NULLの場合, 
			MessageBox(0, "(manager「Init] : RendererがNULL", "警告", MB_OK);
		}
	}

	// カメラ
	{
		if (m_pCamara == NULL)
		{// NULLの場合
			m_pCamara = new CCamera;		// カメラメモリ確保
		}
		else
		{// NULL以外の場合
			MessageBox(0, "(manager「Init] : CameraがNULLでない", "警告", MB_OK);
		}

		if (m_pCamara != NULL)
		{// NULL以外の場合
			// カメラ初期化処理
			if (FAILED(m_pCamara->Init(D3DXVECTOR3(0.0f, 80.0f, -150.0f), D3DXVECTOR3(0.0f, 30.0f, 0.0f))))
			{
				return -1;
			}
		}
		else
		{// NULLの場合
			MessageBox(0, "(manager「Init] : CameraがNULL", "警告", MB_OK);
		}
	}

	// ライト
	{
		if (m_pLight == NULL)
		{// NULLの場合
			m_pLight = new CLight;			// ライトメモリ確保
		}
		else
		{// NULL以外の場合
			MessageBox(0, "(manager「Init] : LightがNULLでない", "警告", MB_OK);
		}

		if (m_pLight != NULL)
		{// NULL以外の場合
		 // ライト初期化処理
			if (FAILED(m_pLight->Init()))
			{
				return -1;
			}
		}
		else
		{// NULLの場合
			MessageBox(0, "(manager「Init] : LightがNULL", "警告", MB_OK);
		}
	}

	SetMode(MODE_TITLE);

	return S_OK;
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
			// 終了処理
			m_pSound->Uninit();

			delete m_pSound;	// サウンドメモリ開放
			m_pSound = NULL;	// NULL
		}
	}

	// キーボード
	{
		if (m_pInputKeyboard != NULL)
		{// NULL以外の場合
			// 終了処理
			m_pInputKeyboard->Uninit();

			delete m_pInputKeyboard;	// キーボードメモリ開放
			m_pInputKeyboard = NULL;	// NULL
		}
	}
	// DIパッド
	{
		if (m_pInputDIPad != NULL)
		{// NULL以外の場合
			// 終了処理
			m_pInputDIPad->Uninit();

			delete m_pInputDIPad;	// DIパッドメモリ開放
			m_pInputDIPad = NULL;	// NULL
		}
	}
	// Xパッド
	{
		if (m_pInputXPad != NULL)
		{// NULL以外の場合
		 // 終了処理
			m_pInputXPad->Uninit();

			delete m_pInputXPad;	// Xパッドメモリ開放
			m_pInputXPad = NULL;	// NULL
		}
	}

	switch (m_mode)
	{// モード
	case CManager::MODE_TITLE:		// タイトル
		if (m_pTitle != NULL)
		{// NULL以外の場合
			m_pTitle->Unload();		// アンロード
			m_pTitle->Uninit();		// 終了処理
			delete m_pTitle;
			m_pTitle = NULL;		// NULLへ
		}

		break;
	case CManager::MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
		{// NULL以外の場合
			m_pGame->Uninit();		// 終了処理
			delete m_pGame;
			m_pGame = NULL;			// NULLへ
		}

		break;

	case CManager::MODE_TUTORIAL:	// チュートリアル
		if (m_pTutorial != NULL)
		{	// NULL以外の場合
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;

	case CManager::MODE_RESULT:	// リザルト
		if (m_pResult != NULL)
		{	// NULL以外の場合
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = NULL;
		}
		break;

	case CManager::MODE_RANKING:	// ランキング
		if (m_pRanking != NULL)
		{	// NULL以外の場合
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
		}
		break;

	}

	// レンダラー
	{
		if (m_pRenderer != NULL)
		{// NULL以外の場合
			// 終了処理
			m_pRenderer->Uninit();

			delete m_pRenderer;	// レンダラーメモリ開放
			m_pRenderer = NULL;	// NULL
		}
	}

	// カメラ
	{
		if (m_pCamara != NULL)
		{// NULL以外の場合
			// 終了処理
			m_pCamara->Uninit();

			delete m_pCamara;	// カメラメモリ開放
			m_pCamara = NULL;	// NULL
		}
	}

	// ライト
	{
		if (m_pLight != NULL)
		{// NULL以外の場合
			// 終了処理
			m_pLight->Uninit();

			delete m_pLight;	// ライトメモリ開放
			m_pLight = NULL;	// NULL
		}
	}
}

//=============================================================================
// 更新処理										(public)	*** CManager ***
//=============================================================================
void CManager::Update(void)
{
	// キーボード
	{
		if (m_pInputKeyboard != NULL)
		{// NULL以外の場合
			// キーボード更新処理
			m_pInputKeyboard->Update();
		}
	}
	// DIパッド
	{
		if (m_pInputDIPad != NULL)
		{// NULL以外の場合
			// DIパッド更新処理
			m_pInputDIPad->Update();
		}
	}
	// Xパッド
	{
		if (m_pInputXPad != NULL)
		{// NULL以外の場合
			// Xパッド更新処理
			m_pInputXPad->Update();
		}
	}

	DebugFont();

	switch (m_mode)
	{// モード
	case CManager::MODE_TITLE:		// タイトル
		if (m_pTitle != NULL)
		{// NULL以外の場合
			m_pTitle->Update();		// 更新処理
		}

		break;
	case CManager::MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
		{// NULL以外の場合
			m_pGame->Update();		// 更新処理
		}

		break;

	case CManager::MODE_TUTORIAL:	// チュートリアル
		if (m_pTutorial != NULL)
		{	// NULL以外の場合
			m_pTutorial->Update();
		}
		break;

	case CManager::MODE_RESULT:	// リザルト
		if (m_pResult != NULL)
		{	// NULL以外の場合
			m_pResult->Update();
		}
		break;

	case CManager::MODE_RANKING:	// ランキング
		if (m_pRanking != NULL)
		{	// NULL以外の場合
			m_pRanking->Update();
		}
		break;
	}

	// レンダラー
	{
		if (m_pRenderer != NULL)
		{// NULL以外の場合
			// レンダラー更新処理
			m_pRenderer->Update();
		}
	}

	// カメラ
	{
		if (m_pCamara != NULL)
		{// NULL以外の場合
			// カメラ更新処理
			m_pCamara->Update();
		}
	}

	// ライト
	{
		if (m_pLight != NULL)
		{// NULL以外の場合
			// ライト更新処理
			m_pLight->Update();
		}
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
		{// NULL以外の場合
			// レンダラー描画処理
			m_pRenderer->Draw();
		}
	}

	// カメラ
	{
		if (m_pCamara != NULL)
		{// NULL以外の場合
			// カメラ描画処理
			m_pCamara->SetCamera();
		}
	}
}

//=============================================================================
// モード設定処理								(public)	*** CManager ***
//=============================================================================
void CManager::SetMode(CManager::MODE mode)
{
	CGame::MODEGAME pGame = m_pGame->GetGame();		// ゲームの状態取得
	switch (m_mode)
	{// モード（現在）
	case CManager::MODE_TITLE:			// タイトル
		if (m_pTitle != NULL)
		{// NULL以外の場合
			if (m_mode != mode)
			{// モードが違ったら
				m_pTitle->Unload();		// アンロード
			}
			m_pTitle->Uninit();			// 終了処理
			delete m_pTitle;
			m_pTitle = NULL;			// NULLへ
		}

		m_pSound->StopSound(CSound::SOUND_LABEL_BGM000);

		break;
	case CManager::MODE_GAME:			// ゲーム
		if (m_pGame != NULL)
		{// NULL以外の場合
			if (m_mode != mode)
			{// モードが違ったら
				m_pGame->Unload();		// アンロード
			}
			m_pGame->Uninit();			// 終了処理
			delete m_pGame;
			m_pGame = NULL;				// NULLへ
		}

		m_pSound->StopSound(CSound::SOUND_LABEL_BGM001);

		break;

	case CManager::MODE_TUTORIAL:		// チュートリアル
		if (m_pTutorial != NULL)
		{	// NULL以外の場合
			if (m_mode != mode)
			{	// モードが違ったら
				m_pTutorial->Unload();	// アンロード
			}
			m_pTutorial->Uninit();		// 終了処理
			delete m_pTutorial;
			m_pTutorial = NULL;			// NULLへ
		}

		m_pSound->StopSound(CSound::SOUND_LABEL_BGM005);

		break;

	case CManager::MODE_RESULT:		// リザルト
		if (m_pResult != NULL)
		{	// NULL以外の場合
			if (m_mode != mode)
			{	// モードが違ったら
				m_pResult->Unload();	// アンロード
			}
			m_pResult->Uninit();		// 終了処理
			delete m_pResult;
			m_pResult = NULL;			// NULLへ
		}
		break;

	case CManager::MODE_RANKING:		// ランキング
		if (m_pRanking != NULL)
		{	// NULL以外の場合
			m_pRanking->Uninit();		// 終了処理
			delete m_pRanking;
			m_pRanking = NULL;			// NULLへ
		}

		m_pSound->StopSound();
		break;
	}

	//m_pSound->StopSound();

	char cName[32];

	switch (mode)
	{// モード（次回）
	case CManager::MODE_TITLE:			// タイトル
		wsprintf(cName, "Title");
		break;
	case CManager::MODE_GAME:			// ゲーム
		wsprintf(cName, "Game");
		break;
	case CManager::MODE_TUTORIAL:			// タイトル
		wsprintf(cName, "Tutorial");
		break;
	case CManager::MODE_RESULT:			// ゲーム
		wsprintf(cName, "Result");
		break;
	case CManager::MODE_RANKING:			// タイトル
		wsprintf(cName, "Ranking");
		break;
	}

	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);

	if (pFont != NULL)
	{
		pFont->SetMessage(CFont::SETTYPE_LOG, "\n");
		pFont->SetMessage(CFont::SETTYPE_LOG, "Mode Change[%s]-----\n", cName);
	}

	switch (mode)
	{// モード（次回）
	case CManager::MODE_TITLE:			// タイトル
		if (m_pTitle == NULL)
		{// NULLの場合
			m_pTitle = new CTitle;		// メモリ確保

			if (m_pTitle != NULL)
			{// NULL以外の場合
				if (m_mode != mode)
				{// モードが違ったら
					m_pTitle->Load();	// ロード
				}
				m_pTitle->Init();		// 初期化処理
				SetPlayNum(0);			// プレイヤー数初期化
			}
		}

		m_pSound->PlaySoundA(CSound::SOUND_LABEL_BGM000);

		break;
	case CManager::MODE_GAME:			// ゲーム
		if (m_pGame == NULL)
		{// NULLの場合
			m_pGame = new CGame;		// メモリ確保

			if (m_pGame != NULL)
			{// NULL以外の場合
				if (m_mode != mode)
				{// モードが違ったら
					m_pGame->Load();	// ロード
				}
				m_pGame->Init();		// 初期化処理
			}
		}

		m_pSound->PlaySoundA(CSound::SOUND_LABEL_BGM001);

		break;

	case CManager::MODE_TUTORIAL:		// チュートリアル
		if (m_pTutorial == NULL)
		{	// NULLの場合
			m_pTutorial = new CTutorial;	// メモリ確保

			if (m_pTutorial != NULL)
			{
				if (m_mode != mode)
				{	// NULL以外の場合
					m_pTutorial->Load();	// ロード
				}
				m_pTutorial->Init();		// 初期化処理
			}
		}

		m_pSound->PlaySoundA(CSound::SOUND_LABEL_BGM005);

		break;

	case CManager::MODE_RESULT:		// リザルト
		if (m_pResult == NULL)
		{	// NULLの場合
			m_pResult = CResult::Create(pGame);	// メモリ確保

			if (m_pResult != NULL)
			{
				if (m_mode != mode)
				{	// NULL以外の場合
					m_pResult->Load();	// ロード
				}
				//m_pResult->SetGameMODE(pGame);	// ゲームの状態を設定
				m_pResult->Init();		// 初期化処理
			}
		}
		break;

	case CManager::MODE_RANKING:		// ランキング
		if (m_pRanking == NULL)
		{	// NULLの場合
			m_pRanking = new CRanking;	// メモリ確保
			m_pRanking->Init(D3DXVECTOR3(1000.0f, 260.0f, 0.0f));		// 初期化処理
		}
		break;
	}

	m_mode = mode;			// モード変更
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

	//----------------------------------------------------------------------------

	// デバッグシステム

	static int nSelectNum = 0;
	static int nSelectMax = 4;
	static bool bMain = false;
	static bool bLog = false;
	static bool bDebug = false;
	static bool bDraw = false;
	static bool bData = false;

	static int bDebugNum = -1;
	static bool bDataDips[CScene::OBJTYPE_MAX] = { false };
	static int nFillMode = 1;

	if (CFont::GetDispAll() == true)
	{
		if (m_pInputKeyboard->GetTrigger(DIK_1))
		{
			nSelectNum = (nSelectNum + nSelectMax - 1) % nSelectMax;
		}
		else if (m_pInputKeyboard->GetTrigger(DIK_2))
		{
			nSelectNum = (nSelectNum + 1) % nSelectMax;
		}

		// デバッグ情報表示用フォント(FPS更新)
		if (pFont[CFont::FONTNUM_COMMAND] != NULL)
		{// NULL以外の場合
			pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_NEW, "command->");

			if (bMain == false) pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "\n");
			else if (bLog == true) pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "log\n");
			else if (bDebug == true) pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "debug\n");
			else if (bDraw == true) pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "draw\n");
			else if (bData == true) pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "data\n");

			if (nSelectNum == 0 && bMain == false)
			{
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "〇");
			}
			if (bMain == false)
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　log\n");
			if (bLog == true)
			{
				if (nSelectNum == 0)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　disp [bool:%d]\n", pFont[CFont::FONTNUM_LOG]->GetDisp());
				if (nSelectNum == 1)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　add line\n");
				if (nSelectNum == 2)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　reset\n");
				if (nSelectNum == 3)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　exit\n");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "\n");
			}

			if ((nSelectNum == 1 && bMain == false))
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "〇");
			if (bMain == false)
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　debug\n");
			if (bDebug == true)
			{
				if (nSelectNum == 0)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　mode\n");
				if (nSelectNum == 1)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　unknow\n");
				if (nSelectNum == 2)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　exit\n");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "\n");
			}

			if ((nSelectNum == 2 && bMain == false))
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "〇");
			if (bMain == false)
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　draw\n");
			if (bDraw == true)
			{
				if (nSelectNum == 0)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　wire [bool:%d]\n", nFillMode ^ 1);
				if (nSelectNum == 1)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　unknow\n");
				if (nSelectNum == 2)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　exit\n");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "\n");
			}

			if ((nSelectNum == 3 && bMain == false))
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "〇");
			if (bMain == false)
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　data\n");
			if (bData == true)
			{
				if (nSelectNum == 0)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　disp [bool:%d]\n", pFont[CFont::FONTNUM_DATA]->GetDisp());
				if (nSelectNum == 1)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　player\n");
				if (nSelectNum == 2)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　camera\n");
				if (nSelectNum == 3)pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　〇");
				else pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "　exit\n");
				pFont[CFont::FONTNUM_COMMAND]->SetMessage(CFont::SETTYPE_ADD, "\n");
			}
		}

		if (m_pInputKeyboard->GetTrigger(DIK_3))
		{
			if (bMain == false)
			{// メインメニュー状態
				switch (nSelectNum)
				{// メニュー番号
				case 0:					
					bLog = true;
					nSelectMax = 4;
					break;
				case 1:
					bDebug = true;
					nSelectMax = 3;
					break;
				case 2:
					bDraw = true;
					nSelectMax = 3;
					break;
				case 3:
					bData = true;
					nSelectMax = 4;
					break;
				}

				bMain = true;
				nSelectNum = 0;		// 選択番号初期化
			}
			else if (bLog == true)
			{// デバッグメニュー
				switch (nSelectNum)
				{
				case 0:
					pFont[CFont::FONTNUM_LOG]->SetDisp(pFont[CFont::FONTNUM_LOG]->GetDisp() ? false : true);
					break;
				case 1:
					pFont[CFont::FONTNUM_LOG]->SetMessage(CFont::SETTYPE_LOG, "\n");
					break;
				case 2:
					pFont[CFont::FONTNUM_LOG]->SetMessage(CFont::SETTYPE_NEW, "");
					break;
				case 3:
					bLog = false;
					bMain = false;
					nSelectNum = 0;
					nSelectMax = 4;
					break;
				}
			}
			else if (bDebug == true)
			{// デバッグメニュー
				switch (nSelectNum)
				{
				case 0:
					bDebugNum = 0;
					break;
				case 1:
					bDebugNum = 1;
					break;
				case 2:
					bDebugNum = -1;
					bDebug = false;
					bMain = false;
					nSelectNum = 0;
					nSelectMax = 4;
					break;
				}
			}
			else if (bDraw == true)
			{// 描画メニュー
				switch (nSelectNum)
				{
				case 0:
					nFillMode = (nFillMode + 1) % 2;
					pDevice->SetRenderState(D3DRS_FILLMODE, (nFillMode + 2));			// ワイヤーフレーム
					break;
				case 1:
					break;
				case 2:
					bDraw = false;
					bMain = false;
					nSelectNum = 0;
					nSelectMax = 4;
					break;
				}
			}
			else if (bData == true)
			{// データメニュー
				switch (nSelectNum)
				{
				case 0:
					pFont[CFont::FONTNUM_DATA]->SetDisp(pFont[CFont::FONTNUM_DATA]->GetDisp() ? false : true);
					break;
				case 1:
					bDataDips[CScene::OBJTYPE_PLAYER] = bDataDips[CScene::OBJTYPE_PLAYER] ? false : true;
					break;
				case 2:
					bDataDips[CScene::OBJTYPE_CAMERA] = bDataDips[CScene::OBJTYPE_CAMERA] ? false : true;
					break;
				case 3:
					bData = false;
					bMain = false;
					nSelectNum = 0;
					nSelectMax = 4;
					break;
				}
			}
		}

		if (bDebug == true)
		{// デバッグメニュー
			switch (bDebugNum)
			{
			case 0:
				pFont[CFont::FONTNUM_DEBUG]->SetMessage(CFont::SETTYPE_ADD, "Mode[Title] : (DIK_4)\n");
				pFont[CFont::FONTNUM_DEBUG]->SetMessage(CFont::SETTYPE_ADD, "Mode[Game] : (DIK_5)\n");

				if (CFade::GetFade() == CFade::FADE_NONE)
				{
					if (CManager::GetInputKeyboard()->GetTrigger(DIK_4))		CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
					else if (CManager::GetInputKeyboard()->GetTrigger(DIK_5))	CFade::Create(CManager::MODE_GAME, CFade::FADE_OUT);
				}
				break;
			}
		}

		for (int nCntObj = 0; nCntObj < CScene::OBJTYPE_MAX; nCntObj++)
		{
			if (bDataDips[nCntObj] == true)
			{
				pFont[CFont::FONTNUM_DATA]->SetMessage(CFont::SETTYPE_ADD, "%s", CFont::GetMessageData((CScene::OBJTYPE)nCntObj));				
			}
		}
	}

	for (int nCntObj = 0; nCntObj < CScene::OBJTYPE_MAX; nCntObj++)
	{
		CFont::SetMessageData((CScene::OBJTYPE)nCntObj, CFont::SETTYPE_NEW, "");
	}
}