//=============================================================================
//
// UI処理 [ui.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ui.h"				// Ui
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "fade.h"			// フェード
#include "texture.h"		// テクスチャ
#include "title.h"			// タイトル
#include "gauge.h"			// ゲージ
#include "ranking.h"		// ランキング

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
// コンストラクタ										(public)	*** CUi ***
//=============================================================================
CUi::CUi() : CScene(CScene::PRIORITY_UI, CScene::OBJTYPE_UI)
{

}

//=============================================================================
// デストラクタ										(public)	*** CUi ***
//=============================================================================
CUi::~CUi()
{

}

//=============================================================================
// ロード処理										(public)	*** CUi ***
//=============================================================================
HRESULT CUi::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CUi ***
//=============================================================================
void CUi::Unload(void)
{

}

//=============================================================================
// クリエイト										(public)	*** CUi ***
//=============================================================================
CUi *CUi::Create(UI ui)
{
	CUi *pUi = NULL;				// ポインタ

	if (pUi == NULL)
	{// NULLの場合
		if(ui == UI_TITLE)			// メモリ確保
			pUi = new CUi_Title;		// UIタイトル
		else if (ui == UI_GAME)		// メモリ確保
			pUi = new CUi_Game;			// UIゲーム
		else if (ui == UI_RESULT)	// メモリ確保
			pUi = new CUi_Result;		// UIリザルト
	}

	if (pUi != NULL)
	{// NULL以外の場合
		pUi->Init();				// 初期化処理
	}

	return pUi;	// 値を返す
}

//=============================================================================
// 終了処理											(public)	*** CUi ***
//=============================================================================
void CUi::Uninit(void)
{
	CScene::SetDeath();		// 死亡フラグ
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
bool CUi_Title::m_bUiEdit = false;		// Ui変更中か

//=============================================================================
// コンストラクタ										(public)	*** CUi_Title ***
//=============================================================================
CUi_Title::CUi_Title() : CUi()
{
	m_bUiEdit = false;		// Ui変更中か

	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
		m_pUi[nCount] = NULL;		// UI

	m_pPress = NULL;		// プレス
	m_nCntPress = 0;		// プレスカウント
	
	m_nCntTitle = 0;		// タイトルカウント
}

//=============================================================================
// デストラクタ										(public)	*** CUi_Title ***
//=============================================================================
CUi_Title::~CUi_Title()
{

}

//=============================================================================
// ロード処理										(public)	*** CUi_Title ***
//=============================================================================
HRESULT CUi_Title::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CUi_Title ***
//=============================================================================
void CUi_Title::Unload(void)
{

}

//=============================================================================
// 初期化処理										(public)	*** CUi_Title ***
//=============================================================================
HRESULT CUi_Title::Init(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// 種類カウント
		if (m_pUi[nCount] == NULL)
		{// NULLの場合		
			m_pUi[nCount] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);		// メモリ確保

			// 初期値設定
			// テクスチャ反映
			if (nCount == TYPE_TITLELOGO_BG)			// タイトルロゴ背景
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 2) - ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)), (SCREEN_WIDTH / 6), 0.0f), D3DXVECTOR3(0.0f, (SCREEN_WIDTH / 12), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_TITLE_LOGO_BG));
			}
			else if (nCount == TYPE_TITLELOGO)			// タイトルロゴ
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_WIDTH / 6), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25), (SCREEN_WIDTH / 12), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_TITLE_LOGO));
			}
			else if (nCount == TYPE_MENU_SELECT)		// 選択
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.32f - SCREEN_HEIGHT * 0.18f + (SCREEN_HEIGHT * 0.18f * (nCount - TYPE_MENU_SELECT)), 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.08f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.25f));
			}
			else if (nCount == TYPE_MENU_SELECT_MARK)	// 選択マーク
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.8f - SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.32f - SCREEN_HEIGHT * 0.18f + (SCREEN_HEIGHT * 0.18f * (nCount - TYPE_MENU_SELECT_MARK)), 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH * 0.08f, SCREEN_WIDTH * 0.08f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0714f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
			}
			else										// 選択肢
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.84f, SCREEN_HEIGHT * 0.32f - SCREEN_HEIGHT * 0.18f + (SCREEN_HEIGHT * 0.18f * (nCount - TYPE_MENU_GAME)), 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.08f, 0.0f), D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.0f), D3DXVECTOR2(1.0f, 0.25f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_TITLE_MENU));
			}

			// 初期化処理
			m_pUi[nCount]->Init();
			
			if (nCount == TYPE_MENU_SELECT_MARK)		// 選択マーク
			{
				m_pUi[nCount]->SetTex(14, 7, D3DXVECTOR2(0.0714f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if(TYPE_MENU_SELECT < nCount)			// 選択肢
			{
				m_pUi[nCount]->SetTex(1, (nCount - TYPE_MENU_GAME), D3DXVECTOR2(1.0f, 0.25f), D3DXVECTOR2(0.0f, 0.0f));
			}

			// 描画編集
			m_pUi[nCount]->DrawEdit(false, false, false);
		}
	}

	// プレス
	if (m_pPress == NULL)
	{// NULLの場合
		m_pPress = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
		m_pPress->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 6, SCREEN_HEIGHT / 14, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
		m_pPress->Init();
		m_pPress->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_PRESS));
	}

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CUi_Title ***
//=============================================================================
void CUi_Title::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// 種類カウント
		if (m_pUi[nCount] != NULL)
		{// NULL以外の場合
			m_pUi[nCount]->Uninit();	// 終了処理
			m_pUi[nCount] = NULL;		// NULLへ
		}
	}

	if (m_pPress != NULL)
	{// NULL以外の場合
		m_pPress->Uninit();		// 終了処理
		m_pPress = NULL;		// NULLへ
	}

	CUi::Uninit();		// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CUi_Title ***
//=============================================================================
void CUi_Title::Update(void)
{
	bool bEdit = false;											// 変更した
	bool bTitle = CManager::GetTitle()->GetTitle();				// タイトル取得
	int nSelectNum = CManager::GetTitle()->GetSelectNum();		// 選択番号取得
	bool bSelectSet = CManager::GetTitle()->GetSelectSet();		// 選択取得

	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// 種類カウント
		if (m_pUi[nCount] != NULL)
		{// NULL以外の場合
			float fcol_a = m_pUi[nCount]->Getcol().a;

			if (nCount == TYPE_TITLELOGO_BG || nCount == TYPE_TITLELOGO)
			{// タイトルロゴ
				if (bTitle == true)
				{// タイトル表示
					if (fcol_a != 1.0f)
					{// 不透明以外
						fcol_a = m_pUi[nCount]->Getcol().a + 0.03f;
						if (1.0f < fcol_a)	fcol_a = 1.0f;
						bEdit = true;
					}
				}
				else
				{// タイトル非表示
					if (fcol_a != 0.0f)
					{// 透明以外
						fcol_a = m_pUi[nCount]->Getcol().a - 0.03f;
						if (fcol_a < 0.0f)	fcol_a = 0.0f;
						bEdit = true;
					}
				}

				m_pUi[nCount]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, fcol_a));	// 色変更

				if (nCount == TYPE_TITLELOGO_BG)
				{// タイトルロゴ背景
					if (bTitle == true)
					{// タイトル表示
						if (m_nCntTitle < 60)
						{// 一定フレームまで
							D3DXVECTOR3 pos = m_pUi[nCount]->Getpos();
							pos.x += ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)) / 60.0f;
							m_pUi[nCount]->Setpos(pos);

							D3DXVECTOR3 size = m_pUi[nCount]->Getsize();
							size.x += ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)) / 60.0f;
							m_pUi[nCount]->Setsize(size);

							D3DXVECTOR2 TexUV = m_pUi[nCount]->GetTexUV();
							TexUV.x += 1.0f / 60.0f;
							m_pUi[nCount]->SetTex(0, 0, TexUV, D3DXVECTOR2(0.0f, 0.0f));
						}
						else
						{// その他
							D3DXVECTOR3 pos = m_pUi[nCount]->Getpos();
							pos.x += ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)) / 60.0f;
							m_pUi[nCount]->Setpos(pos);

							D3DXVECTOR3 size = m_pUi[nCount]->Getsize();
							size.x -= ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)) / 60.0f;
							m_pUi[nCount]->Setsize(size);

							D3DXVECTOR2 TexUV = m_pUi[nCount]->GetTexUV();
							TexUV.x -= 1.0f / 60.0f;
							m_pUi[nCount]->SetTex(0, 0, TexUV, D3DXVECTOR2(1.0f - TexUV.x, 0.0f));
						}

						m_nCntTitle++;		// カウント

						if (m_nCntTitle == 240)
						{// 一定フレームだったら
							m_pUi[nCount]->Setpos(D3DXVECTOR3((SCREEN_WIDTH / 2) - ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)), (SCREEN_WIDTH / 6), 0.0f));
							m_pUi[nCount]->Setsize(D3DXVECTOR3(0.0f, (SCREEN_WIDTH / 12), 0.0f));
							m_pUi[nCount]->SetTex(0, 0, D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
							m_nCntTitle = 0;
						}
					}
					else
					{// タイトル非表示
						if (m_nCntTitle != 0)
						{
							m_pUi[nCount]->Setpos(D3DXVECTOR3((SCREEN_WIDTH / 2) - ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)), (SCREEN_WIDTH / 6), 0.0f));
							m_pUi[nCount]->Setsize(D3DXVECTOR3(0.0f, (SCREEN_WIDTH / 12), 0.0f));
							m_pUi[nCount]->SetTex(0, 0, D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
							m_nCntTitle = 0;
						}
					}
				}
			}
			else if (nCount == TYPE_MENU_SELECT || nCount == TYPE_MENU_SELECT_MARK)
			{// 選択、選択マークのみ
				float fMaxcol_a = 1.0f;

				if (nCount == TYPE_MENU_SELECT)
					fMaxcol_a = 0.7f;
				else
					fMaxcol_a = 1.0f;

				if (bSelectSet == true)
				{// タイトル表示
					if (fcol_a != fMaxcol_a)
					{// 
						fcol_a = m_pUi[nCount]->Getcol().a + 0.03f;
						if (fMaxcol_a < fcol_a)	fcol_a = fMaxcol_a;
						bEdit = true;
					}
				}
				else
				{// タイトル非表示
					if (fcol_a != 0.0f)
					{
						fcol_a = m_pUi[nCount]->Getcol().a - 0.03f;
						if (fcol_a < 0.0f)	fcol_a = 0.0f;
						bEdit = true;
					}
				}

				m_pUi[nCount]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, fcol_a));	// 色変更

				if (bTitle == true || (bTitle == false && fcol_a != 0.0f))
				{// タイトル表示
					D3DXVECTOR3 pos = m_pUi[nCount]->Getpos();
					D3DXVECTOR3 posSelect = m_pUi[(nSelectNum + TYPE_MENU_GAME)]->Getpos();
					m_pUi[nCount]->Setpos(D3DXVECTOR3(pos.x, posSelect.y, 0.0f));

					if (nCount == TYPE_MENU_SELECT_MARK)
						m_pUi[nCount]->Setrot(m_pUi[nCount]->Getrot() + 0.05f);
				}
			}
			else
			{// メニュー表示
				if (bSelectSet == true)
				{
					if (fcol_a != 1.0f)
					{
						fcol_a = m_pUi[nCount]->Getcol().a + 0.03f;
						if (1.0f < fcol_a)	fcol_a = 1.0f;
						bEdit = true;

						m_pUi[nCount]->Setpos(D3DXVECTOR3(SCREEN_WIDTH * 0.84f, SCREEN_HEIGHT * 0.32f - SCREEN_HEIGHT * 0.18f + (SCREEN_HEIGHT * 0.18f * (nCount - TYPE_MENU_GAME)), 0.0f));
					}
				}
				else
				{
					if (fcol_a != 0.0f)
					{
						fcol_a = m_pUi[nCount]->Getcol().a - 0.03f;
						if (fcol_a < 0.0f)	fcol_a = 0.0f;
						bEdit = true;
					}
					else
					{
						m_pUi[nCount]->Setpos(D3DXVECTOR3(500.0f + SCREEN_WIDTH * 0.84f, SCREEN_HEIGHT * 0.32f - SCREEN_HEIGHT * 0.18f + (SCREEN_HEIGHT * 0.18f * (nCount - TYPE_MENU_GAME)), 0.0f));
					}
				}

				if (nCount == (nSelectNum + TYPE_MENU_GAME))
				{// 選択番号と一致
					m_pUi[nCount]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, fcol_a));	// 色変更
				}
				else
				{// 選択番号以外
					m_pUi[nCount]->Setcol(D3DXCOLOR(0.5f, 0.5f, 0.5f, fcol_a));	// 色変更
				}
			}
		}
	}

	if (m_pPress != NULL)
	{// NULL以外の場合
		D3DXCOLOR col = m_pPress->Getcol();

		if (bTitle == true)
		{// タイトル表示
			if (bEdit == true)
			{
				if (col.a != 1.0f)
				{
					col.a += 0.03f;
					if (1.0f < col.a)	col.a = 1.0f;
					bEdit = true;
				}
			}
			else
			{
				if (m_nCntPress == 0)
				{
					m_pPress->SetcolType(90, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
				}
				else if (m_nCntPress == 90)
				{
					m_pPress->SetcolType(90, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				m_nCntPress++;

				if (m_nCntPress == 180)
				{
					m_nCntPress = 0;
				}
			}
		}
		else
		{// メニュー表示
			if (col.a != 0.0f)
			{
				col.a -= 0.03f;
				if (col.a < 0.0f)	col.a = 0.0f;
				bEdit = true;
			}

			m_nCntPress = 0;
		}

		m_pPress->Setcol(col);	// 色変更
	}

	// Ui編集したか
	if (bEdit == true)
		m_bUiEdit = true;	// した
	else
		m_bUiEdit = false;	// していない
}

//=============================================================================
// 描画処理											(public)	*** CUi_Title ***
//=============================================================================
void CUi_Title::Draw(void)
{

}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
int		CUi_Game::m_nEnemyName = 0;				// 敵名前番号

//=============================================================================
// コンストラクタ										(public)	*** CUi_Game ***
//=============================================================================
CUi_Game::CUi_Game() : CUi()
{
	for(int nCount = 0; nCount < TYPE_MAX; nCount++)
		m_pUi[nCount] = NULL;		// UI

	for (int nCount = 0; nCount < MESSAGE_MAX; nCount++)
		m_Message[nCount] = NULL;	// メッセージ

	m_TutorialUV = D3DXVECTOR2(0.0f, 0.0f);			// チュートリアルUV
	m_nCntTutorial = 0;				// チュートリアルカウント

	for (int nCount = 0; nCount < GAUGE_MAX; nCount++)
		m_pGauge[nCount] = NULL;	// ゲージ

	m_nEnemyName = 0;
}

//=============================================================================
// デストラクタ										(public)	*** CUi_Game ***
//=============================================================================
CUi_Game::~CUi_Game()
{

}

//=============================================================================
// ロード処理										(public)	*** CUi_Game ***
//=============================================================================
HRESULT CUi_Game::Load(void)
{
	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CUi_Game ***
//=============================================================================
void CUi_Game::Unload(void)
{

}

//=============================================================================
// 初期化処理										(public)	*** CUi_Game ***
//=============================================================================
HRESULT CUi_Game::Init(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// 種類カウント
		if (m_pUi[nCount] == NULL)
		{// NULLの場合
			m_pUi[nCount] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);		// メモリ確保

			// 初期値設定
			// テクスチャ反映				
			if (nCount == TYPE_ENEMY_DATA)				// 敵データ
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_WIDTH / 24), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_WIDTH / 24), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
			}
			else if (nCount == TYPE_PLAYER_DATA)		// プレイヤーデータ
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 24), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_WIDTH / 24), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
			}
			else if (nCount == TYPE_MAP)				// マップ
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 11) + 2.0f, (SCREEN_WIDTH / 11) - 2.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 12) + (SCREEN_WIDTH / 180) + 2.0f, (SCREEN_WIDTH / 12) + (SCREEN_WIDTH / 180) + 2.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_MAP));
			}
			else if (nCount == TYPE_LOG)				// ログ
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 10), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_WIDTH / 10), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
			}
			else if (nCount == TYPE_WEAPON)				// 武器
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (SCREEN_WIDTH / 28), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 5), (SCREEN_WIDTH / 28), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
			}
			else if (nCount == TYPE_WEAPON_BUTTON)		// 武器ボタン
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (SCREEN_WIDTH / 28), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 5), (SCREEN_WIDTH / 26) - (SCREEN_WIDTH / 120), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_WEAPON));
			}
			else if (nCount == TYPE_PLAYER_ICON)		// プレイヤーアイコン
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 26), SCREEN_HEIGHT - (SCREEN_WIDTH / 25), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 30), (SCREEN_WIDTH / 30), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 0.5f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CHAR));
			}
			else if (nCount == TYPE_PLAYER_NAME)		// プレイヤー名
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7) + (SCREEN_WIDTH / 28), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38 * 4) - (SCREEN_HEIGHT / 70), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 10), (SCREEN_WIDTH / 70), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_PLAYER_NAME));
			}
			else if (nCount == TYPE_PLAYER_HP)			// プレイヤーHP
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 11), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38 * 2) - (SCREEN_HEIGHT / 80), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 80), (SCREEN_WIDTH / 100), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_HPSP));
			}
			else if (nCount == TYPE_PLAYER_MP)			// プレイヤーMP
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 11), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 80), (SCREEN_WIDTH / 100), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_HPSP));
			}
			else if (nCount == TYPE_PLAYER_RANK)		// プレイヤーランク
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7) + (SCREEN_WIDTH / 10) + (SCREEN_WIDTH / 50), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38 * 4) - (SCREEN_HEIGHT / 70), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 50), (SCREEN_WIDTH / 70), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.2f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_RANK));
			}
			else if (nCount == TYPE_ENEMY_ICON)			// 敵アイコン
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 26), (SCREEN_WIDTH / 24), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 30), (SCREEN_WIDTH / 30), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 0.5f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CHAR));
			}
			else if (nCount == TYPE_ENEMY_NAME)			// 敵名
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7) + (SCREEN_WIDTH / 28), (SCREEN_HEIGHT / 38) + (SCREEN_HEIGHT / 50), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 10), (SCREEN_WIDTH / 50), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.333f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_ENEMY_NAME));
			}
			else if (nCount == TYPE_ENEMY_HP)			// 敵HP
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 11), (SCREEN_HEIGHT / 9), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 70), (SCREEN_WIDTH / 90), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_HPSP));
			}

			// 初期化処理
			m_pUi[nCount]->Init();

			if (nCount == TYPE_PLAYER_ICON)				// プレイヤーアイコン
			{
				m_pUi[nCount]->SetTex(2, 1, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_ENEMY_ICON)			// プレイヤーアイコン
			{
				m_pUi[nCount]->SetTex(2, 0, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_PLAYER_HP)			// プレイヤーMP
			{
				m_pUi[nCount]->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_PLAYER_MP)			// プレイヤーMP
			{
				m_pUi[nCount]->SetTex(1, 1, D3DXVECTOR2(1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_PLAYER_RANK)		// プレイヤーランク
			{
				m_pUi[nCount]->SetTex(5, 0, D3DXVECTOR2(0.2f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_ENEMY_NAME)			// 敵名
			{
				m_pUi[nCount]->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.333f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_ENEMY_HP)			// 敵MP
			{
				m_pUi[nCount]->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
			}

			// 描画編集
			if (nCount == TYPE_MAP)
				m_pUi[nCount]->DrawEdit(false, false, true, 50);
		}
	}

	for (int nCount = 0; nCount < MESSAGE_MAX; nCount++)
	{// メッセージカウント
		if (m_Message[nCount] == NULL)
		{// NULLの場合
			m_Message[nCount] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);		// メモリ確保

			// 初期値設定
			m_Message[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 40) - ((SCREEN_WIDTH / 40.0f * 2.0f) * nCount), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 8), (SCREEN_WIDTH / 40), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			// 初期化処理
			m_Message[nCount]->Init();
			// テクスチャ反映
			m_Message[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_EFFECT));
		}
	}

	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_GAME)
	{// モードがゲームだったら
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// NULL以外の場合
			if (pGame->GetGame() == CGame::GAME_TUTORIAL)
			{// チュートリアルのみ
				m_Message[MESSAGE_00]->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 10), 0.0f));
				m_Message[MESSAGE_00]->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 8), (SCREEN_WIDTH / 10), 0.0f));
				m_Message[MESSAGE_00]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_Message[MESSAGE_00]->SetTexUV(D3DXVECTOR2(1.0f, 0.125f));
				m_Message[MESSAGE_00]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_TUTORIAL));

				m_Message[MESSAGE_01]->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 4), 0.0f));
				m_Message[MESSAGE_01]->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 10), (SCREEN_WIDTH / 18), 0.0f));
				m_Message[MESSAGE_01]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_Message[MESSAGE_01]->SetTexUV(D3DXVECTOR2(1.0f, 1.0f));
				m_Message[MESSAGE_01]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CHECK));

				m_Message[MESSAGE_02]->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 2), SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.9f), 0.0f));
				m_Message[MESSAGE_02]->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_HEIGHT / 14), 0.0f));
				m_Message[MESSAGE_02]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_Message[MESSAGE_02]->SetTexUV(D3DXVECTOR2(1.0f, 1.0f));
				m_Message[MESSAGE_02]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_TUTORIAL_LOGO));

				m_Message[MESSAGE_03]->Setpos(D3DXVECTOR3((SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 24 * 2) - (SCREEN_HEIGHT / 10), 0.0f));
				m_Message[MESSAGE_03]->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_HEIGHT / 10), 0.0f));
				m_Message[MESSAGE_03]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_Message[MESSAGE_03]->SetTexUV(D3DXVECTOR2(1.0f, 1.0f));
				m_Message[MESSAGE_03]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_SKIP));
			}
		}
	}

	for (int nCount = 0; nCount < GAUGE_MAX; nCount++)
	{// ゲージカウント
		if (m_pGauge[nCount] == NULL)
		{// NULLの場合
			if (nCount == GAUGE_PLAYER_HP)		// プレイヤーHP
				m_pGauge[nCount] = CGauge::Create(CGauge::TYPE_2D, D3DXVECTOR3((SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 35), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38 * 2) - (SCREEN_HEIGHT / 80), 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 12, SCREEN_HEIGHT / 60, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			if (nCount == GAUGE_PLAYER_MP)		// プレイヤーMP
				m_pGauge[nCount] = CGauge::Create(CGauge::TYPE_2D, D3DXVECTOR3((SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 35), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38), 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 12, SCREEN_HEIGHT / 60, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			if (nCount == GAUGE_ENEMY_HP)		// 敵HP
				m_pGauge[nCount] = CGauge::Create(CGauge::TYPE_2D, D3DXVECTOR3((SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 35), (SCREEN_HEIGHT / 9), 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 12, SCREEN_HEIGHT / 50, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CUi_Game ***
//=============================================================================
void CUi_Game::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// 種類カウント
		if (m_pUi[nCount] != NULL)
		{// NULL以外の場合
			m_pUi[nCount]->Uninit();	// 終了処理
			m_pUi[nCount] = NULL;		// NULLへ
		}
	}

	for (int nCount = 0; nCount < MESSAGE_MAX; nCount++)
	{// メッセージカウント
		if (m_Message[nCount] != NULL)
		{// NULLの場合
			m_Message[nCount]->Uninit();// 終了処理
			m_Message[nCount] = NULL;	// NULLへ
		}
	}

	for (int nCount = 0; nCount < GAUGE_MAX; nCount++)
	{// ゲージカウント
		if (m_pGauge[nCount] != NULL)
		{// NULLの場合
			m_pGauge[nCount]->Uninit();	// 終了処理
			m_pGauge[nCount] = NULL;	// NULLへ
		}
	}

	CUi::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CUi_Game ***
//=============================================================================
void CUi_Game::Update(void)
{
	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_GAME)
	{// モードがゲームだったら
		if (m_pUi[TYPE_PLAYER_RANK] != NULL)		// プレイヤーランク
		{// NULL以外の場合
			int nNum = 0;
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{// NULL以外の場合
				int nScore = pGame->GetScore();
				if (55000 < nScore) nNum = 4;
				else if (45000 < nScore) nNum = 3;
				else if (35000 < nScore) nNum = 2;
				else if (25000 < nScore) nNum = 1;
				else					nNum = 0;
			}

			m_pUi[TYPE_PLAYER_RANK]->SetTex(5, nNum, D3DXVECTOR2(0.2f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		}
		if (m_pUi[TYPE_ENEMY_ICON] != NULL)
		{// NULL以外の場合
			int nNum = 0;
			if (m_nEnemyName == 0)
				nNum = m_nEnemyName;
			else
				nNum = m_nEnemyName + 1;

			m_pUi[TYPE_ENEMY_ICON]->SetTex(2, nNum, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));

		}
		if (m_pUi[TYPE_ENEMY_NAME] != NULL)
		{// NULL以外の場合
			m_pUi[TYPE_ENEMY_NAME]->SetTex(1, m_nEnemyName, D3DXVECTOR2(1.0f, 0.333f), D3DXVECTOR2(0.0f, 0.0f));
		}

		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// NULL以外の場合
			if (pGame->GetGame() == CGame::GAME_TUTORIAL)
			{// チュートリアルのみ
				int nTutorialNum = pGame->GetTutprialNum();
				if (m_TutorialUV.y != (0.125f * (float)(nTutorialNum)))
				{
					m_TutorialUV.y += 0.02f;
					if ((0.125f * (float)(nTutorialNum)) < m_TutorialUV.y) m_TutorialUV.y = (0.125f * (float)(nTutorialNum));
				}
				m_Message[MESSAGE_00]->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.125f), m_TutorialUV);

				if (nTutorialNum != CGame::TUTORIAL_ACTION)
				{// アクション以外
					D3DXVECTOR3 pos = m_Message[MESSAGE_01]->Getpos();
					if (m_nCntTutorial < 90)
					{// 一定フレームまで
						pos.y -= 0.3f;
					}
					else
					{// その他
						pos.y += 0.3f;
					}
					m_Message[MESSAGE_01]->Setpos(pos);

					D3DXCOLOR col = m_Message[MESSAGE_02]->Getcol();
					if (m_nCntTutorial < 90)
					{// 一定フレームまで
						col.a -= (1.0f / 90.0f);
						if (col.a < 0.0f) col.a = 0.0f;
					}
					else
					{// その他
						col.a += (1.0f / 90.0f);
						if (1.0f < col.a) col.a = 1.0f;
					}
					m_Message[MESSAGE_02]->Setcol(col);

					m_nCntTutorial = (m_nCntTutorial + 1) % 180;
				}
				else
				{// その他
					m_Message[MESSAGE_01]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_Message[MESSAGE_02]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
			}
		}
	}
}

//=============================================================================
// 描画処理											(public)	*** CUi_Game ***
//=============================================================================
void CUi_Game::Draw(void)
{

}

//=============================================================================
// ゲージ設定処理									(public)	*** CUi_Game ***
//=============================================================================
void CUi_Game::SetGauge(GAUGE gauge, int nNow, int nMax)
{
	if (m_pGauge[gauge] != NULL)
	{// NULL以外の場合
		m_pGauge[gauge]->SetBar(D3DXVECTOR3(0.0f, 0.0f, 0.0f), nNow, nMax);
	}
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CUi_Result::m_pTexture[SS_MAX] = { NULL };		// テクスチャ

//=============================================================================
// コンストラクタ										(public)	*** CUi_Result ***
//=============================================================================
CUi_Result::CUi_Result() : CUi()
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
		m_pUi[nCount] = NULL;		// UI

	m_nCntTimer = 0;				// 時間カウント
	m_nCntLogo = 0;					// ロゴカウント

	for (int nCount = 0; nCount < 6; nCount++)
		m_pNumber[nCount];	// 番号
}

//=============================================================================
// デストラクタ										(public)	*** CUi_Result ***
//=============================================================================
CUi_Result::~CUi_Result()
{

}

//=============================================================================
// ロード処理										(public)	*** CUi_Result ***
//=============================================================================
HRESULT CUi_Result::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	char cName[64] = {};

	/*strcpy(cName, "data\\TEXTURE\\ScreenShot\\result00.bmp");
	D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[0]);	// テクスチャ読み込み
	strcpy(cName, "data\\TEXTURE\\ScreenShot\\result01.bmp");
	D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[1]);	// テクスチャ読み込み
	strcpy(cName, "data\\TEXTURE\\ScreenShot\\result02.bmp");
	D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[2]);	// テクスチャ読み込み*/

	strcpy(cName, "data\\TEXTURE\\ScreenShot\\result01.bmp");
	D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[0]);	// テクスチャ読み込み
	strcpy(cName, "data\\TEXTURE\\ScreenShot\\result02.bmp");
	D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[1]);	// テクスチャ読み込み

	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CUi_Result ***
//=============================================================================
void CUi_Result::Unload(void)
{
	for (int nCount = 0; nCount < SS_MAX; nCount++)
	{// SSカウント
		if (m_pTexture[nCount] != NULL)
		{// NULL以外の場合
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理										(public)	*** CUi_Result ***
//=============================================================================
HRESULT CUi_Result::Init(void)
{
	CGame *pGame = CManager::GetGame();

	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// 種類カウント
		if (m_pUi[nCount] == NULL)
		{// NULLの場合
			m_pUi[nCount] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);		// メモリ確保

			// 初期値設定
			// テクスチャ反映
			if (nCount == TYPE_LOGO_BG)			// ロゴbg
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 4) - (SCREEN_WIDTH / 4), (SCREEN_HEIGHT / 10), 0.0f), D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 10), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

				if (pGame != NULL)
				{
					if (pGame->GetGame() == CGame::GAME_CLEAR)
					{// クリア
						m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_RESULT_CLEAR_BG));
					}
					else if (pGame->GetGame() == CGame::GAME_OVER)
					{// オーバー
						m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_RESULT_OVER_BG));
					}
				}				
			}
			else if (nCount == TYPE_LOGO)		// ロゴ
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 4), (SCREEN_HEIGHT / 10), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 5), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				
				if (pGame != NULL)
				{// NULL以外の場合
					if (pGame->GetGame() == CGame::GAME_CLEAR)
					{// クリア
						m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_RESULT_CLEAR));
					}
					else if (pGame->GetGame() == CGame::GAME_OVER)
					{// オーバー
						m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_RESULT_OVER));
					}
				}
			}
			else if (nCount == TYPE_PLAYER_NAME)		// ロゴ
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH) - (SCREEN_WIDTH / 4), (SCREEN_HEIGHT / 6), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 5), (SCREEN_HEIGHT / 15), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_PLAYER_NAME));
			}
			else if (nCount == TYPE_PLAYER_RANK)		// ロゴ
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH) - (SCREEN_WIDTH / 12), (SCREEN_HEIGHT / 6), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 30), (SCREEN_WIDTH / 30), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.2f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_RANK));
			}
			else
			{
				//m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 6) + ((SCREEN_WIDTH / 9) * (float)(nCount - TYPE_PLAY_SS00)), (SCREEN_HEIGHT / 3) + ((SCREEN_HEIGHT / 4 + (SCREEN_HEIGHT / 80)) * (float)(nCount - TYPE_PLAY_SS00)), 0.0f),
				//	D3DXVECTOR3((SCREEN_WIDTH / 8), (SCREEN_HEIGHT / 8), 0.0f),	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				//m_pUi[nCount]->BindTexture(m_pTexture[(nCount - TYPE_PLAY_SS00)]);

				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 5) + ((SCREEN_WIDTH / 8 * 2 - 80.0f) * (float)(nCount - TYPE_PLAY_SS00)), (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 10) + ((SCREEN_HEIGHT / 8 * 2 - 20.0f) * (float)(nCount - TYPE_PLAY_SS00)), 0.0f),
					D3DXVECTOR3((SCREEN_WIDTH / 6), (SCREEN_HEIGHT / 6), 0.0f),	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(m_pTexture[(nCount - TYPE_PLAY_SS00)]);
			}

			// 初期化処理
			m_pUi[nCount]->Init();
			// 描画編集

			if (nCount == TYPE_PLAYER_RANK)		// ロゴ
			{
				int nNum = 0;
				if (pGame != NULL)
				{// NULL以外の場合
					int nScore = pGame->GetScore();

					// クリア
					nScore += 15000;

					// ボス撃破
					nScore += 1000;

					if (55000 < nScore) nNum = 4;
					else if (45000 < nScore) nNum = 3;
					else if (35000 < nScore) nNum = 2;
					else if (25000 < nScore) nNum = 1;
					else					nNum = 0;
				}

				m_pUi[nCount]->SetTex(5, nNum, D3DXVECTOR2(0.2f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			}
		}
	}

	float fScoreData = 0.0f;
	if (pGame != NULL)
	{// NULL以外の場合
		fScoreData = (float)pGame->GetScore();

		// クリア
		fScoreData += 15000.0f;

		// ボス撃破
		fScoreData += 1000.0f;
	}

	for (int nCount = 0; nCount < 6; nCount++)
	{// カウント
		if (m_pNumber[nCount] == NULL)
		{// NULLの場合
			m_pNumber[nCount] = new CNumber2D;
			m_pNumber[nCount]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 9, 0.0f) + 
				D3DXVECTOR3((-((40.0f * 2.0f) + 10.0f) * nCount) - 40.0f, 0.0f, 0.0f), D3DXVECTOR3(40.0f, 60.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			m_pNumber[nCount]->SetTex(10, (int)fScoreData % 10, D3DXVECTOR2(0.1f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));					// テクスチャ設定
		}

		fScoreData /= 10;		// 桁を進める
	}

	CRanking::Create(CRanking::TYPE_2D, D3DXVECTOR3(SCREEN_WIDTH - 80.0f, 180.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(30.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CUi_Result ***
//=============================================================================
void CUi_Result::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// 種類カウント
		if (m_pUi[nCount] != NULL)
		{// NULL以外の場合
			m_pUi[nCount]->Uninit();	// 終了処理
			m_pUi[nCount] = NULL;		// NULLへ
		}
	}

	for (int nCount = 0; nCount < 6; nCount++)
	{// カウント
		if (m_pNumber[nCount] != NULL)
		{// NULL以外の場合
			m_pNumber[nCount]->Uninit();// 終了処理
			m_pNumber[nCount] = NULL;	// NULLへ
		}
	}

	CUi::Uninit();	// 終了処理
}

//=============================================================================
// 更新処理											(public)	*** CUi_Result ***
//=============================================================================
void CUi_Result::Update(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// 種類カウント
		if (m_pUi[nCount] != NULL)
		{// NULL以外の場合
			if (nCount == TYPE_LOGO_BG)			// ロゴ
			{
				if (8 < m_nCntTimer)
				{
					if (m_nCntLogo < 60)
					{// 一定フレームまで
						D3DXVECTOR3 pos = m_pUi[nCount]->Getpos();
						pos.x += (SCREEN_WIDTH / 4) / 60.0f;
						m_pUi[nCount]->Setpos(pos);

						D3DXVECTOR3 size = m_pUi[nCount]->Getsize();
						size.x += (SCREEN_WIDTH / 4) / 60.0f;
						m_pUi[nCount]->Setsize(size);

						D3DXVECTOR2 TexUV = m_pUi[nCount]->GetTexUV();
						TexUV.x += 1.0f / 60.0f;
						m_pUi[nCount]->SetTex(0, 0, TexUV, D3DXVECTOR2(0.0f, 0.0f));
					}
					else
					{// その他
						D3DXVECTOR3 pos = m_pUi[nCount]->Getpos();
						pos.x += (SCREEN_WIDTH / 4) / 60.0f;
						m_pUi[nCount]->Setpos(pos);

						D3DXVECTOR3 size = m_pUi[nCount]->Getsize();
						size.x -= (SCREEN_WIDTH / 4) / 60.0f;
						m_pUi[nCount]->Setsize(size);

						D3DXVECTOR2 TexUV = m_pUi[nCount]->GetTexUV();
						TexUV.x -= 1.0f / 60.0f;
						m_pUi[nCount]->SetTex(0, 0, TexUV, D3DXVECTOR2(1.0f - TexUV.x, 0.0f));
					}

					m_nCntLogo++;

					if (m_nCntLogo == 240)
					{// 一定フレームだったら
						m_pUi[nCount]->Setpos(D3DXVECTOR3((SCREEN_WIDTH / 4) - (SCREEN_WIDTH / 4), (SCREEN_HEIGHT / 10), 0.0f));
						m_pUi[nCount]->Setsize(D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 10), 0.0f));
						m_pUi[nCount]->SetTex(0, 0, D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
						m_nCntLogo = 0;
					}
				}
			}
			else if (nCount == TYPE_LOGO)		// ロゴ
			{
				if (m_nCntTimer < 8)
				{
					D3DXVECTOR3 size = m_pUi[nCount]->Getsize();
					size.x += ((SCREEN_WIDTH / 4) - (SCREEN_WIDTH / 2)) / 8;
					size.y += ((SCREEN_HEIGHT / 10) - (SCREEN_HEIGHT / 5)) / 8;
					m_pUi[nCount]->Setsize(size);
				}				
			}
		}
	}

	m_nCntTimer++;
}

//=============================================================================
// 描画処理											(public)	*** CUi_Result ***
//=============================================================================
void CUi_Result::Draw(void)
{

}