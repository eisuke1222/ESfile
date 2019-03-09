//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "fade.h"			// フェード
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MC_FADE_TIME	(60.0f)						// フェード時間
#define MC_ADDCOLOR_A	(1.0f / MC_FADE_TIME)		// 透明度加算量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CFade::FADE			CFade::m_fade		= CFade::FADE_NONE;		// フェード状態

//=============================================================================
// コンストラクタ										(public)	*** CFade ***
//=============================================================================
CFade::CFade() : CScene2D(CScene::PRIORITY_FADE, CScene::OBJTYPE_FADE)
{
	// 初期値設定
	m_fadeNum	= 0;							// フェード番号
	m_modeNext	= CManager::MODE_NONE;			// 次のモード
	m_gameNext	= CGame::GAME_NONE;				// 次のゲーム
	m_nCntTimer = 0;							// 時間カウント
}

//=============================================================================
// デストラクタ										(public)	*** CFade ***
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// ロード処理										(public)	*** CFade ***
//=============================================================================
HRESULT CFade::Load(void)
{
	return S_OK;
}

//=============================================================================
// アンロード処理										(public)	*** CFade ***
//=============================================================================
void CFade::Unload(void)
{

}

//=============================================================================
// クリエイト										(public)	*** CFade ***
//=============================================================================
CFade *CFade::Create(CManager::MODE mode, FADE fade)
{
	CFade *pFade = NULL;					// ポインタ

	if (m_fade == FADE_NONE)
	{// フェードしていない
		if (pFade == NULL)
		{// NULLの場合
			pFade = new CFade;				// メモリ確保
		}

		if (pFade != NULL)
		{// NULL以外の場合
			pFade->Init(mode, fade);		// 初期化処理
		}

		return pFade;	// 値を返す
	}

	return NULL;	// 値を返す
}
CFade *CFade::Create(CGame::GAME game, FADE fade)
{
	CFade *pFade = NULL;					// ポインタ

	if (m_fade == FADE_NONE)
	{// フェードしていない
		if (pFade == NULL)
		{// NULLの場合
			pFade = new CFade;				// メモリ確保
		}

		if (pFade != NULL)
		{// NULL以外の場合
			pFade->Init(game, fade);		// 初期化処理
		}

		return pFade;	// 値を返す
	}

	return NULL;	// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CFade ***
//=============================================================================
HRESULT CFade::Init(CManager::MODE mode, FADE fade)
{
	// 初期値設定
	if (fade == FADE_OUT)
	{// フェードアウトの場合
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	}
	else
	{// その他
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	}
	
	CScene2D::Init();		// 初期化処理

	m_fadeNum = 0;
	m_fade = FADE_OUT;		// フェードアウト
	m_modeNext = mode;		// 次モード
	m_nCntTimer = 0;		// カウント

	return S_OK;	// 値を返す
}
HRESULT CFade::Init(CGame::GAME game, FADE fade)
{
	// 初期値設定
	if (fade == FADE_OUT)
	{// フェードアウトの場合
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	}
	else
	{// その他
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	}

	CScene2D::Init();		// 初期化処理

	m_fadeNum = 1;
	m_fade = FADE_OUT;		// フェードアウト
	m_gameNext = game;		// 次ゲーム
	m_nCntTimer = 0;		// カウント

	return S_OK;	// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CFade ***
//=============================================================================
void CFade::Uninit(void)
{
	CScene2D::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CFade ***
//=============================================================================
void CFade::Update(void)
{	
	D3DXCOLOR col = CScene2D::Getcol();		// 色を取得
	bool bDelete = false;					// 破棄する

	if (m_fade != FADE_NONE)
	{// フェード使われていたら
		if (m_fade == FADE_IN)
		{// フェードイン
			col.a -= MC_ADDCOLOR_A;			// 透明に

			if (col.a <= 0.0f)
			{// 透明
				col.a = 0.0f;				// 透明度を調整
				m_fade = FADE_NONE;			// 何もしていない状態
				m_nCntTimer = 0;			// カウント初期化

				bDelete = true;				// 破棄する
			}
		}
		else if (m_fade == FADE_OUT)
		{// フェードアウト
			col.a += MC_ADDCOLOR_A;			// 不透明に

			if (col.a >= 1.0f)
			{// 不透明
				col.a = 1.0f;				// 透明度を調整

				m_nCntTimer++;				// カウンタを加算
				if (m_nCntTimer > (int)(MC_FADE_TIME * 0.5f))
				{// 一定フレーム後
					m_fade = FADE_IN;		// フェードイン
				}
				else if (m_nCntTimer == (int)(MC_FADE_TIME * 0.25f))
				{// フレームの中間
					if (m_fadeNum == 0)
					{
						CManager::SetMode(m_modeNext);				// モード変更
					}
					else
					{
						CManager::GetGame()->SetGame(m_gameNext);	// ゲーム変更
					}

					CScene::SetStartUp(0);				// 更新範囲を初期化
					CScene::SetUpdateTime(1);			// 更新カウントを初期化
				}
			}
		}
	}

	CScene2D::Setcol(col);					// 色を設定
	CScene2D::Update();						// 更新処理

	if (bDelete == true)
	{// 破棄する
		Uninit();							// 終了処理
	}
}

//=============================================================================
// 描画処理											(public)	*** CFade ***
//=============================================================================
void CFade::Draw(void)
{
	CScene2D::Draw();						// 描画処理
}