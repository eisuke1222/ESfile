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
#include "title.h"			// タイトル
#include "tutorial.h"		// チュートリアル
#include "game.h"			// ゲーム
#include "fade.h"			// フェード

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
CTutorial			*CManager::m_pTutorial		= NULL;					// チュートリアル
CGame				*CManager::m_pGame			= NULL;					// ゲーム
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

	CFade::Create(MODE_TITLE, CFade::FADE_IN);		// フェードクリエイト

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
			m_pTitle = NULL;		// NULLへ
		}

		break;
	case CManager::MODE_TUTORIAL:	// チュートリアル
		if (m_pTutorial != NULL)
		{// NULL以外の場合
			m_pTutorial->Uninit();	// 終了処理
			m_pTutorial = NULL;		// NULLへ
		}

		break;
	case CManager::MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
		{// NULL以外の場合
			m_pGame->Uninit();		// 終了処理
			m_pGame = NULL;			// NULLへ
		}

		break;
	}

	CScene::ReleaseAll();

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

	switch (m_mode)
	{// モード
	case CManager::MODE_TITLE:		// タイトル
		if (m_pTitle != NULL)
		{// NULL以外の場合
			m_pTitle->Update();		// 更新処理
		}

		break;
	case CManager::MODE_TUTORIAL:	// ゲーム
		if (m_pTutorial != NULL)
		{// NULL以外の場合
			m_pTutorial->Update();	// 更新処理
		}

		break;
	case CManager::MODE_GAME:		// ゲーム
		if (m_pGame != NULL)
		{// NULL以外の場合
			m_pGame->Update();		// 更新処理
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
			m_pTitle = NULL;			// NULLへ
		}

		m_pSound->StopSound(CSound::SOUND_LABEL_BGM000);

		break;
	case CManager::MODE_TUTORIAL:		// チュートリアル
		if (m_pTutorial != NULL)
		{// NULL以外の場合
			if (m_mode != mode)
			{// モードが違ったら
				m_pTutorial->Unload();	// アンロード
			}
			m_pTutorial->Uninit();		// 終了処理
			m_pTutorial = NULL;			// NULLへ
		}

		m_pSound->StopSound(CSound::SOUND_LABEL_BGM001);

		break;
	case CManager::MODE_GAME:			// ゲーム
		if (m_pGame != NULL)
		{// NULL以外の場合
			if (m_mode != mode)
			{// モードが違ったら
				m_pGame->Unload();		// アンロード
			}
			m_pGame->Uninit();			// 終了処理
			m_pGame = NULL;				// NULLへ
		}

		m_pSound->StopSound(CSound::SOUND_LABEL_BGM001);

		break;
	}

	m_pSound->StopSound();

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
	case CManager::MODE_TUTORIAL:		// チュートリアル
		if (m_pTutorial == NULL)
		{// NULLの場合
			m_pTutorial = new CTutorial;	// メモリ確保

			if (m_pTutorial != NULL)
			{// NULL以外の場合
				if (m_mode != mode)
				{// モードが違ったら
					m_pTutorial->Load();	// ロード
				}
				m_pTutorial->Init();		// 初期化処理
			}
		}

		m_pSound->PlaySoundA(CSound::SOUND_LABEL_BGM001);

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
	}

	m_mode = mode;			// モード変更
}

//=============================================================================
// モード取得処理								(public)	*** CManager ***
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;		// 値を返す
}

//=============================================================================
// プレイ人数取得処理								(public)	*** CManager ***
//=============================================================================
int CManager::GetPlayNum(void)
{
	return m_nPlayNum;	// 値を返す
}