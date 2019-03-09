//=============================================================================
//
// ウィンドウ処理 [window.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "window.h"			// ウィンドウ
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "fade.h"			// フェード
#include "texture.h"		// テクスチャ

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
// コンストラクタ										(public)	*** CWindow ***
//=============================================================================
CWindow::CWindow() : CScene(CScene::PRIORITY_UI, CScene::OBJTYPE_WINDOW)
{

}

//=============================================================================
// デストラクタ										(public)	*** CWindow ***
//=============================================================================
CWindow::~CWindow()
{

}

//=============================================================================
// ロード処理										(public)	*** CWindow ***
//=============================================================================
HRESULT CWindow::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CWindow ***
//=============================================================================
void CWindow::Unload(void)
{

}

//=============================================================================
// クリエイト										(public)	*** CWindow ***
//=============================================================================
CWindow *CWindow::Create(WINDOW window)
{
	CWindow *pWindow = NULL;			// ポインタ

	if (pWindow == NULL)
	{// NULLの場合
		if(window == WINDOW_YESNO)		// エフェクト2D
			pWindow = new CWindow_YESNO;	// メモリ確保
	}

	if (pWindow != NULL)
	{// NULL以外の場合
		pWindow->Init();				// 初期化処理
	}

	return pWindow;		// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CWindow ***
//=============================================================================
void CWindow::Uninit(void)
{
	CScene::SetDeath();		// 死亡フラグ
}

//-----------------------------------------------------------------------------

//=============================================================================
// コンストラクタ									(public)	*** CWindow_YESNO ***
//=============================================================================
CWindow_YESNO::CWindow_YESNO() : CWindow()
{
	// 初期値設定
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
		m_pScene2D[nCount] = NULL;		// シーン2D

	m_nSelectNum = 0;
}

//=============================================================================
// デストラクタ									(public)	*** CWindow_YESNO ***
//=============================================================================
CWindow_YESNO::~CWindow_YESNO()
{

}

//=============================================================================
// ロード処理									(public)	*** CWindow_YESNO ***
//=============================================================================
HRESULT CWindow_YESNO::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理									(public)	*** CWindow_YESNO ***
//=============================================================================
void CWindow_YESNO::Unload(void)
{

}

//=============================================================================
// 初期化処理									(public)	*** CWindow_YESNO ***
//=============================================================================
HRESULT CWindow_YESNO::Init(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// カウント
		m_pScene2D[nCount] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_WINDOW);

		if (nCount == TYPE_BG)					// 背景
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pScene2D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
		}
		else if (nCount == TYPE_MESSAGE_BG)		// メッセージ背景
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 30), 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 28, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
		}
		else if (nCount == TYPE_MESSAGE)		// メッセージ
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 30), 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 9, SCREEN_HEIGHT / 28, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pScene2D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_END));
		}
		else if (nCount == TYPE_SELECT_BG)		// 選択背景
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 25), SCREEN_HEIGHT / 2 + (SCREEN_HEIGHT / 8 / 2), 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH / 28, SCREEN_HEIGHT / 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 0.5f));
			//m_pScene2D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_YESNO));
		}
		else if (nCount == TYPE_YES)			// 選択肢（はい）
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 25), SCREEN_HEIGHT / 2 + (SCREEN_HEIGHT / 8 / 2), 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH / 28, SCREEN_HEIGHT / 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
			m_pScene2D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_YESNO));
		}
		else if (nCount == TYPE_NO)				// 選択肢（いいえ）
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2 + (SCREEN_WIDTH / 25), SCREEN_HEIGHT / 2 + (SCREEN_HEIGHT / 8 / 2), 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH / 28, SCREEN_HEIGHT / 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pScene2D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_YESNO));
		}
		// 初期化処理
		m_pScene2D[nCount]->Init();

		if (nCount == TYPE_YES)					// 選択肢（はい）
		{
			m_pScene2D[nCount]->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
		}
		else if (nCount == TYPE_NO)				// 選択肢（いいえ）
		{
			m_pScene2D[nCount]->SetTex(1, 1, D3DXVECTOR2(1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	return S_OK;	// 値を返す
}	

//=============================================================================
// 終了処理										(public)	*** CWindow_YESNO ***
//=============================================================================
void CWindow_YESNO::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// カウント
		m_pScene2D[nCount]->Uninit();	// 終了処理
		m_pScene2D[nCount] = NULL;		// NULLへ
	}

	CWindow::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理										(public)	*** CWindow_YESNO ***
//=============================================================================
void CWindow_YESNO::Update(void)
{
	CInputKeyboard	*pKey = CManager::GetInputKeyboard();		// キーボード取得
	CInputXPad		*pXPad = CManager::GetInputXPad();			// Xパッド取得

	if (pKey->GetTrigger(DIK_A) == true || pKey->GetTrigger(DIK_LEFT) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, 0) == true)
	{// キーボード（A）が押された、Xパッド（Lスティック左）が押された
		m_nSelectNum = (m_nSelectNum + (TYPE_MAX - TYPE_YES) - 1) % (TYPE_MAX - TYPE_YES);
	}
	else if (pKey->GetTrigger(DIK_D) == true || pKey->GetTrigger(DIK_RIGHT) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
	{// キーボード（D）が押された、Xパッド（Lスティック右）が押された
		m_nSelectNum = (m_nSelectNum + 1) % (TYPE_MAX - TYPE_YES);
	}

	for (int nCount = TYPE_YES; nCount < TYPE_MAX; nCount++)
	{// カウント
		if (m_pScene2D[nCount] != NULL)
		{// NULL以外の場合
			if (nCount == m_nSelectNum + TYPE_YES)
			{// 同じだったら
				m_pScene2D[nCount]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				if (m_pScene2D[TYPE_SELECT_BG] != NULL)
				{// NULL以外の場合
					m_pScene2D[TYPE_SELECT_BG]->Setpos(m_pScene2D[nCount]->Getpos());
				}
			}
			else
			{// その他
				m_pScene2D[nCount]->Setcol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			}
		}
	}
}

//=============================================================================
// 描画処理										(public)	*** CWindow_YESNO ***
//=============================================================================
void CWindow_YESNO::Draw(void)
{

}