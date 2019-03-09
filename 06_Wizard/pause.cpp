//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "pause.h"			// ポーズ
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "sound.h"			// サウンド
#include "fade.h"			// フェード
#include "texture.h"		// テクスチャ

#include "scene2d.h"		// シーン2D
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

//=============================================================================
// コンストラクタ									(public)	*** CPause ***
//=============================================================================
CPause::CPause() : CScene(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_PAUSE)
{
	// 初期値設定
	m_nSelectNum	= 0;		// 選択番号
	m_nCntTimer		= 0;		// 時間カウンタ

	for (int nCntType= 0; nCntType < TYPE_MAX; nCntType++)
		m_apPauseBg[nCntType]	= NULL;		// 背景

	for (int nCntSelect= 0; nCntSelect < SELECT_MAX; nCntSelect++)
		m_apSelect[nCntSelect]	= NULL;		// 選択
}

//=============================================================================
// デストラクタ										(public)	*** CPause ***
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ロード処理										(public)	*** CPause ***
//=============================================================================
HRESULT CPause::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CPause ***
//=============================================================================
void CPause::Unload(void)
{

}

//=============================================================================
// クリエイト										(public)	*** CPause ***
//=============================================================================
CPause *CPause::Create(void)
{
	CPause *pPause = NULL;		// ポインタ

	if (pPause == NULL)
	{// NULLの場合
		pPause = new CPause;	// メモリ確保
	}

	if (pPause != NULL)
	{// NULL以外の場合
		pPause->Init();			// 初期化処理
	}

	return pPause;		// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CPause ***
//=============================================================================
HRESULT CPause::Init(void)
{
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// 種類カウント
		if (m_apPauseBg[nCntType] == NULL)
		{// NULLの場合
			// メモリ確保
			m_apPauseBg[nCntType] = new CScene2D(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_PAUSE);
			// 初期値設定
			// テクスチャ反映
			if (nCntType == TYPE_BG)			// 背景
			{
				m_apPauseBg[nCntType]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
			}
			else if (nCntType == TYPE_FRAME)	// 枠
			{
				m_apPauseBg[nCntType]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.48f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH * 0.18f, SCREEN_HEIGHT * 0.35f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
				m_apPauseBg[nCntType]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
			}
			else if (nCntType == TYPE_LINE)		// 線
			{
				m_apPauseBg[nCntType]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.35f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.001f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			}
			else if (nCntType == TYPE_MENU)		// メニュー
			{
				m_apPauseBg[nCntType]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.25f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.075f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_apPauseBg[nCntType]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_PAUSE_LOGO));
			}
			// 初期化処理
			m_apPauseBg[nCntType]->Init();
		}
	}

	if (m_pSelectChange == NULL)
	{// NULLの場合
		// メモリ確保
		m_pSelectChange = new CScene2D(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_PAUSE);
		// 初期値設定
		m_pSelectChange->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.58f - SCREEN_HEIGHT * 0.12f, 0.0f),
			D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.05f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0714f, 1.0f));
		// 初期化処理
		m_pSelectChange->Init();
		// テクスチャ反映
		m_pSelectChange->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_PAUSE_BG));
		m_pSelectChange->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.333f), D3DXVECTOR2(0.0f, 0.0f));
	}

	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{// 選択カウント
		if (m_apSelect[nCntSelect] == NULL)
		{// NULLの場合
			// メモリ確保
			m_apSelect[nCntSelect] = new CScene2D(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_PAUSE);
			// 初期値設定
			m_apSelect[nCntSelect]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.58f - SCREEN_HEIGHT * 0.12f + (SCREEN_HEIGHT * 0.12f * nCntSelect), 0.0f), 
				D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.05f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXVECTOR2(0.0714f, 1.0f));
			// 初期化処理
			m_apSelect[nCntSelect]->Init();
			// テクスチャ反映
			m_apSelect[nCntSelect]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_PAUSE_MENU));
			m_apSelect[nCntSelect]->SetTex(1, nCntSelect, D3DXVECTOR2(1.0f, 0.333f), D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	for (int nCntMark = 0; nCntMark < MARK_MAX; nCntMark++)
	{// マークカウント
		if (m_apMark[nCntMark] == NULL)
		{// NULLの場合
		 // メモリ確保
			m_apMark[nCntMark] = new CScene2D(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_PAUSE);
			// 初期値設定
			m_apMark[nCntMark]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.135f) + ((SCREEN_WIDTH * 0.135f * 2.0f) * nCntMark), SCREEN_HEIGHT * 0.58f - SCREEN_HEIGHT * 0.12f, 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH * 0.06f, SCREEN_WIDTH * 0.06f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.333f));
			// 初期化処理
			m_apMark[nCntMark]->Init();
			// テクスチャ反映
			m_apMark[nCntMark]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
			m_apMark[nCntMark]->SetTex(14, 7, D3DXVECTOR2(0.0714f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CPause ***
//=============================================================================
void CPause::Uninit(void)
{
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// 種類カウント
		if (m_apPauseBg[nCntType] != NULL)
		{// NULL以外の場合
			m_apPauseBg[nCntType]->Uninit();	// 終了処理
			m_apPauseBg[nCntType] = NULL;		// NULLへ
		}
	}

	if (m_pSelectChange != NULL)
	{// NULL以外の場合
		m_pSelectChange->Uninit();				// 終了処理
		m_pSelectChange = NULL;					// NULLへ
	}

	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{// 選択カウント
		if (m_apSelect[nCntSelect] != NULL)
		{// NULL以外の場合
			m_apSelect[nCntSelect]->Uninit();	// 終了処理
			m_apSelect[nCntSelect] = NULL;		// NULLへ
		}
	}

	for (int nCntMark = 0; nCntMark < MARK_MAX; nCntMark++)
	{// マークカウント
		if (m_apMark[nCntMark] != NULL)
		{// NULL以外の場合
			m_apMark[nCntMark]->Uninit();		// 終了処理
			m_apMark[nCntMark] = NULL;			// NULLへ
		}
	}
	
	CScene::SetDeath();		// 死亡フラグ
}

//=============================================================================
// 更新処理											(public)	*** CPause ***
//=============================================================================
void CPause::Update(void)
{
	CSound *pSound = CManager::GetSound();

	CInputKeyboard *pKey	= CManager::GetInputKeyboard();	// キーボード取得
	CInputXPad		*pXPad	= CManager::GetInputXPad();			// Xパッド取得
	CFade::FADE		fade	= CFade::GetFade();				// フェード取得

	if (fade == CFade::FADE_NONE)
	{// フェードしていない
		if (pKey->GetTrigger(DIK_W) == true || pKey->GetTrigger(DIK_UP) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, 0) == true)
		{// キーボード（W）が押された、Xパッド（Lスティック上）が押された
			m_nSelectNum = (m_nSelectNum + SELECT_MAX - 1) % SELECT_MAX;

			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SERECT000);
			}
		}
		else if (pKey->GetTrigger(DIK_S) == true || pKey->GetTrigger(DIK_DOWN) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, 0) == true)
		{// キーボード（S）が押された、Xパッド（Lスティック下）が押された
			m_nSelectNum = (m_nSelectNum + 1) % SELECT_MAX;

			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SERECT000);
			}
		}
	}

	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{// 選択カウント
		if (m_apSelect[nCntSelect] != NULL)
		{// NULL以外の場合
			if (nCntSelect == m_nSelectNum)
			{// 選択番号と一致
				m_apSelect[nCntSelect]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// 色変更
				
				if (m_pSelectChange != NULL)
				{// NULL以外の場合
					m_pSelectChange->Setpos(m_apSelect[nCntSelect]->Getpos());
					m_pSelectChange->SetTex(1, nCntSelect, D3DXVECTOR2(1.0f, 0.333f), D3DXVECTOR2(0.0f, 0.0f));
				}

				for (int nCntMark = 0; nCntMark < MARK_MAX; nCntMark++)
				{// マークカウント
					if (m_apMark[nCntMark] != NULL)
					{// NULL以外の場合
						m_apMark[nCntMark]->Setpos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.135f) + ((SCREEN_WIDTH * 0.135f * 2.0f) * nCntMark), m_apSelect[nCntSelect]->Getpos().y, 0.0f));
						m_apMark[nCntMark]->Setrot(m_apMark[nCntMark]->Getrot() + 0.05f);
					}
				}
			}
			else
			{// 選択番号以外
				m_apSelect[nCntSelect]->Setcol(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));	// 色変更
			}
		}
	}

	if (fade == CFade::FADE_NONE)
	{// フェードしていない
		if (pKey->GetTrigger(DIK_RETURN) == true || pKey->GetTrigger(DIK_E) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
		{// キーボード（ENTER）が押された、Xパッド（B、4）が押された
			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
			}

			switch (m_nSelectNum)
			{// 選択番号
			case SELECT_CONTINUE:	// コンテニュー
				CManager::GetGame()->PauseSeting();		// ポーズ解除
				break;
			case SELECT_RETRY:		// リトライ
				CFade::Create(CManager::MODE_GAME);		// フェード生成（ゲームモードへ）
				break;
			case SELECT_QUIT:		// クイット
				CFade::Create(CManager::MODE_TITLE);	// フェード生成（タイトルモードへ）
				break;
			}			
		}
	}
}

//=============================================================================
// 描画処理											(public)	*** CPause ***
//=============================================================================
void CPause::Draw(void)
{

}