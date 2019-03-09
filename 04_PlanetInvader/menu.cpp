//=============================================================================
//
// メニュー処理 [menu.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "menu.h"			// メニュー
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "sound.h"			// サウンド
#include "renderer.h"		// レンダラー
#include "scene3d.h"		// シーン3D
#include "camera.h"			// カメラ
#include "title.h"			// タイトル
#include "fade.h"			// フェード

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\menu000.png"		// テクスチャ1

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CMenu::m_apTexture = {};		// テクスチャ情報へのポインタ

//=============================================================================
// コンストラクタ									(public)	*** CMenu ***
//=============================================================================
CMenu::CMenu() : CScene(CScene::PRIORITY_4)
{
	// 初期値設定
	m_nSelectNum = 0;	// セレクト番号
	m_nCntTimer = 0;	// カウント初期化

	for (int nCntMenu = 0; nCntMenu < MENU_MAX_TEXTURE; nCntMenu++)
	{// カウント
		m_apScene3D[nCntMenu] = NULL;
	}
}

//=============================================================================
// デストラクタ										(public)	*** CMenu ***
//=============================================================================
CMenu::~CMenu()
{

}

//=============================================================================
// ロード処理							(public)	*** CMenu ***
//=============================================================================
HRESULT CMenu::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_apTexture == NULL)
	{// NULLの場合
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_apTexture);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CMenu ***
//=============================================================================
void CMenu::Unload(void)
{
	// テクスチャの破棄
	if (m_apTexture != NULL)
	{// NULL以外の場合
		m_apTexture->Release();		// 解放
		m_apTexture = NULL;			// NULLへ
	}
}

//=============================================================================
// 確保処理											(public)	*** CMenu ***
//=============================================================================
CMenu *CMenu::Create(void)
{
	CMenu *pMenu = NULL;				// ポインタ

	if (pMenu == NULL)
	{// NULLの場合
		pMenu = new CMenu;				// メモリ確保
	}

	if (pMenu != NULL)
	{// NULL以外の場合
		for (int nCntMenu = 0; nCntMenu < MENU_MAX_TEXTURE; nCntMenu++)
		{// カウント
			if (pMenu->m_apScene3D[nCntMenu] == NULL)
			{// NULL以外の場合
				pMenu->m_apScene3D[nCntMenu] = new CScene3D(CScene::PRIORITY_4);																				// メモリ確保
				
			//	pMenu->m_apScene3D[nCntMenu]->SetInitAll(
			//		D3DXVECTOR3(180.0f, (MENU_SIZE_Y * (2.5f * 1.5f)) - ((nCntMenu) * (MENU_SIZE_Y * 2.5f)), 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			//		D3DXVECTOR3(MENU_SIZE_X, MENU_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.25f), CScene3D::SCENE3DTYPE_NORMAL);		// 初期値設定

				pMenu->m_apScene3D[nCntMenu]->SetInitAll(
					D3DXVECTOR3(180.0f, (MENU_SIZE_Y * (2.5f)) - ((nCntMenu - (nCntMenu / 2)) * (MENU_SIZE_Y * 2.5f)), 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(MENU_SIZE_X, MENU_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.25f), CScene3D::SCENE3DTYPE_NORMAL);		// 初期値設定

				if (nCntMenu == 1)
					pMenu->m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

				pMenu->m_apScene3D[nCntMenu]->Init();																											// 初期化処理
				pMenu->m_apScene3D[nCntMenu]->SetObjType(CScene::OBJTYPE_MENU);																					// オブジェクトタイプ設定
				pMenu->m_apScene3D[nCntMenu]->BindTexture(m_apTexture);																							// テクスチャ設定
				pMenu->m_apScene3D[nCntMenu]->SetTex(1, nCntMenu, D3DXVECTOR2(D3DXVECTOR2(1.0f, 0.25f)), D3DXVECTOR2(0.0f, 0.0f));
			}
		}
		pMenu->Init();								// 初期化処理
	}

	return pMenu;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CMenu ***
//=============================================================================
HRESULT CMenu::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CMenu ***
//=============================================================================
void CMenu::Uninit(void)
{
	for (int nCntMenu = 0; nCntMenu < MENU_MAX_TEXTURE; nCntMenu++)
	{// カウント
		if (m_apScene3D[nCntMenu] != NULL)
		{// NULL以外の場合
			m_apScene3D[nCntMenu]->Uninit();		// 終了処理
			m_apScene3D[nCntMenu] = NULL;			// NULLへ
		}
	}

	CScene::Release();			// リリース
}

//=============================================================================
// 更新処理											(public)	*** CMenu ***
//=============================================================================
void CMenu::Update(void)
{
	// カメラ
	CCamera *pCamera = CManager::GetCamera();							// カメラ取得
	//CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// 入力デバイス
	CInputXPad *pInputXPad = CManager::GetInputXPad();					// 入力デバイス
	CSound *pSound = CManager::GetSound();								// サウンド
	CFade::FADE fade = CFade::GetFade();								// フェード取得
	bool bPlayGame = CTitle::GetPlayGame();								// プレイゲーム
	int nPlayerIn = 0;													// プレイヤー入力

	if (CManager::GetPlayNum() == 0)
	{
		nPlayerIn = 0;
	}
	else
	{
		nPlayerIn = 1;
	}

	if (fade == CFade::FADE_NONE && bPlayGame == false)
	{// フェードをしていない状態
		if (pCamera->GetDest() == false)
		{// カメラが動いていない
			for (int nCntCont = 0; nCntCont < nPlayerIn; nCntCont++)
			{// カウント
				//if (pInputKeyboard->GetTrigger(DIK_S) == true || pInputKeyboard->GetTrigger(DIK_DOWN) == true)
				if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN, nCntCont) == true || pInputXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, nCntCont) == true)
				{// 下キーを押したら
					m_nSelectNum = (m_nSelectNum + 1) % MENU_MAX_TEXTURE;						// 選択を下へ
					m_nCntTimer = 0;			// カウント初期化

					if (m_nSelectNum == 1)
					{
						m_nSelectNum++;
					}

					pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
				}
				//else if (pInputKeyboard->GetTrigger(DIK_W) == true || pInputKeyboard->GetTrigger(DIK_UP) == true)
				else if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, nCntCont) == true || pInputXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, nCntCont) == true)
				{// 上キーを押したら
					m_nSelectNum = (m_nSelectNum + MENU_MAX_TEXTURE - 1) % MENU_MAX_TEXTURE;	// 選択を上へ
					m_nCntTimer = 0;			// カウント初期化

					if (m_nSelectNum == 1)
					{
						m_nSelectNum--;
					}

					pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
				}
			}
		}
	}

	m_nCntTimer++;		// カウント

	for (int nCntMenu = 0; nCntMenu < MENU_MAX_TEXTURE; nCntMenu++)
	{// カウント
		if (m_apScene3D[nCntMenu] != NULL)
		{// NULL以外の場合
			if (nCntMenu == 1)
			{
				//m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));		// 色変更
				m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.0f));		// 色変更
			}
			else if (nCntMenu == m_nSelectNum)
			{// 選択番号と一致したら
				m_apScene3D[nCntMenu]->Setsize(D3DXVECTOR3(MENU_SIZE_X * 1.3f, MENU_SIZE_Y * 1.3f, 0.0f));		// 大きさ変更

				if (m_nCntTimer % 90 < 45)
				{// 一定フレームで
					m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_apScene3D[nCntMenu]->Getcol().a - (0.3f / 45.0f)));		// 色変更
				}
				else
				{// その他
					m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_apScene3D[nCntMenu]->Getcol().a + (0.3f / 45.0f)));		// 色変更
				}
			}
			else
			{// その他
				m_apScene3D[nCntMenu]->Setsize(D3DXVECTOR3(MENU_SIZE_X, MENU_SIZE_Y, 0.0f));					// デフォルトサイズに
				m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));								// デフォルトカラーに
			}

			if (bPlayGame == true)
			{// ゲームに進む
				if (m_apScene3D[nCntMenu]->Getpos().x < 500.0f)
				{// 一定位置まで
					m_apScene3D[nCntMenu]->Setpos(D3DXVECTOR3(m_apScene3D[nCntMenu]->Getpos().x + 5.0f, m_apScene3D[nCntMenu]->Getpos().y, m_apScene3D[nCntMenu]->Getpos().z));		// 移動
				}
			}

			m_apScene3D[nCntMenu]->Update();		// 更新処理
		}
	}

	if (fade == CFade::FADE_NONE && bPlayGame == false)
	{
		if (pCamera->GetDest() == false)
		{// カメラが動いていない
			for (int nCntCont = 0; nCntCont < nPlayerIn; nCntCont++)
			{// カウント
				//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true && bPlayGame == false)
				if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_A, nCntCont) == true || pInputXPad->GetTrigger(XINPUT_GAMEPAD_B, nCntCont) == true && bPlayGame == false)
				{// ENTERを押したら
					switch (m_nSelectNum)
					{// 選択番号
					case 0:		// ゲーム
						CTitle::SetPlayGame();
						break;
					case 1:		// カスタム
						pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1200.0f, 0.0f, 0.0f), 1200, 90, CCamera::CPOSDEST_MENU_C);
						break;
					case 2:		// チュートリアル
						CFade::Create(CManager::MODE_TUTORIAL, CFade::FADE_OUT);
						break;
					case 3:		// タイトル
						pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 0.0f, 0.0f), 1200, 90, CCamera::CPOSDEST_MENU_T);
						break;
					}

					pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
				}
			}
		}
}

	if (pCamera->GetPosDest(CCamera::CPOSDEST_MENU_C) == true)
	{// カスタムカメラセット完了
		pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1200.0f), 1200, 90, CCamera::CPOSDEST_MENU_C);
		CTitle::SetTitle(CTitle::TITLE_CUSTOM);		// カスタムへ
	}
	else if (pCamera->GetPosDest(CCamera::CPOSDEST_MENU_T) == true)
	{// タイトルカメラセット完了
		pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1200.0f), 1200, 90, CCamera::CPOSDEST_MENU_T);
		CTitle::SetTitle(CTitle::TITLE_LOGO);		// タイトルへ
	}
}

//=============================================================================
// 描画処理											(public)	*** CMenu ***
//=============================================================================
void CMenu::Draw(void)
{
	
}