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
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "scene2d.h"		// シーン2D
#include "game.h"			// ゲーム
#include "tutorial.h"		// チュートリアル
#include "fade.h"			// フェード

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\menu002.png"		// テクスチャ1
#define TEXTURE_NAME_2	"data\\TEXTURE\\menu001.png"		// テクスチャ2
#define TEXTURE_NAME_3	"data\\TEXTURE\\titlelogo.png"		// テクスチャ3

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		CPause::m_apTexture[CPause::TEXTURE_MAX] = {};		// テクスチャ情報へのポインタ

//=============================================================================
// コンストラクタ									(public)	*** CPause ***
//=============================================================================
CPause::CPause() : CScene(CScene::PRIORITY_PAUSE)
{
	// 初期値設定
	m_nSelectNum = 0;		// 選択番号
	m_nCntTimer = 0;		// 時間カウンタ

	for (int nCntPause = 0; nCntPause < PAUSE_MAX_BG; nCntPause++)
	{// カウント
		m_apScene2D[nCntPause] = NULL;
	}
	for (int nCntPause = 0; nCntPause < PAUSE_MAX_SELECT; nCntPause++)
	{// カウント
		m_apScene2DSelect[nCntPause] = NULL;
	}
}

//=============================================================================
// デストラクタ										(public)	*** CPause ***
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ロード処理							(public)	*** CPause ***
//=============================================================================
HRESULT CPause::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_apTexture[0] == NULL)
	{// NULLの場合
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_apTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_apTexture[1]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_3, &m_apTexture[2]);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CPause ***
//=============================================================================
void CPause::Unload(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// カウント
		if (m_apTexture[nCntTex] != NULL)
		{// NULL以外の場合
			m_apTexture[nCntTex]->Release();		// 解放
			m_apTexture[nCntTex] = NULL;			// NULLへ
		}
	}
}

//=============================================================================
// 確保処理											(public)	*** CPause ***
//=============================================================================
CPause *CPause::Create(void)
{
	CPause *pPause = NULL;				// ポインタ

	if (pPause == NULL)
	{// NULLの場合
		pPause = new CPause;			// メモリ確保
	}

	if (pPause != NULL)
	{// NULL以外の場合
		for (int nCntPause = 0; nCntPause < PAUSE_MAX_BG; nCntPause++)
		{// カウント
			if (pPause->m_apScene2D[nCntPause] == NULL)
			{// NULL以外の場合
				pPause->m_apScene2D[nCntPause] = new CScene2D(CScene::PRIORITY_PAUSE);															// メモリ確保
				switch (nCntPause)
				{// カウント番号
				case 0:
					pPause->m_apScene2D[nCntPause]->SetInitAll(
						D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 
						D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f), D3DXVECTOR2(1.0f, 1.0f));	// 初期値設定

					break;
				case 1:
					pPause->m_apScene2D[nCntPause]->SetInitAll(
						D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 
						D3DXVECTOR3(200.0f, 260.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));							// 初期値設定

					break;
				case 2:
					pPause->m_apScene2D[nCntPause]->SetInitAll(
						D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 170.0f, 0.0f),
						D3DXVECTOR3(PAUSE_SIZE_X, PAUSE_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.25f));			// 初期値設定

					break;
				case 3:
					pPause->m_apScene2D[nCntPause]->SetInitAll(
						D3DXVECTOR3(220.0f, 60.0f, 0.0f),
						D3DXVECTOR3(200.0f, 40.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));							// 初期値設定

					break;
				}
				pPause->m_apScene2D[nCntPause]->Init();																							// 初期化処理
				pPause->m_apScene2D[nCntPause]->SetObjType(CScene::OBJTYPE_PAUSE);																// オブジェクトタイプ設定

				switch (nCntPause)
				{// カウント番号
				case 0:

					break;
				case 1:
					pPause->m_apScene2D[nCntPause]->BindTexture(m_apTexture[TEXTURE_PAUSE_BG]);													// テクスチャ設定

					break;
				case 2:
					pPause->m_apScene2D[nCntPause]->BindTexture(m_apTexture[TEXTURE_PAUSE]);													// テクスチャ設定
					pPause->m_apScene2D[nCntPause]->SetTex(1, 0, D3DXVECTOR2(0.0f, 0.0f));

					break;
				case 3:
					pPause->m_apScene2D[nCntPause]->BindTexture(m_apTexture[TEXTURE_TITLE]);													// テクスチャ設定

					break;
				}
			}
		}
		for (int nCntPause = 0; nCntPause < PAUSE_MAX_SELECT; nCntPause++)
		{// カウント
			if (pPause->m_apScene2DSelect[nCntPause] == NULL)
			{// NULL以外の場合
				pPause->m_apScene2DSelect[nCntPause] = new CScene2D(CScene::PRIORITY_PAUSE);													// メモリ確保
				pPause->m_apScene2DSelect[nCntPause]->SetInitAll(
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 60.0f - 110.0f + ((nCntPause) * 110.0f), 0.0f),
					D3DXVECTOR3(PAUSE_SIZE_X, PAUSE_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.25f));				// 初期値設定

				pPause->m_apScene2DSelect[nCntPause]->Init();																					// 初期化処理
				pPause->m_apScene2DSelect[nCntPause]->SetObjType(CScene::OBJTYPE_PAUSE);														// オブジェクトタイプ設定
				pPause->m_apScene2DSelect[nCntPause]->BindTexture(m_apTexture[TEXTURE_PAUSE]);																// テクスチャ設定
				pPause->m_apScene2DSelect[nCntPause]->SetTex(1, nCntPause + 1, D3DXVECTOR2(0.0f, 0.0f));
			}
		}
		pPause->Init();					// 初期化処理
	}

	return pPause;			// 値を返す
}

//=============================================================================
// 初期化処理										(public)	*** CPause ***
//=============================================================================
HRESULT CPause::Init(void)
{
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CPause ***
//=============================================================================
void CPause::Uninit(void)
{
	for (int nCntPause = 0; nCntPause < PAUSE_MAX_BG; nCntPause++)
	{// カウント
		if (m_apScene2D[nCntPause] != NULL)
		{// NULL以外の場合
			m_apScene2D[nCntPause]->Uninit();			// 終了処理
			m_apScene2D[nCntPause] = NULL;				// NULLへ
		}
	}
	for (int nCntPause = 0; nCntPause < PAUSE_MAX_SELECT; nCntPause++)
	{// カウント
		if (m_apScene2DSelect[nCntPause] != NULL)
		{// NULL以外の場合
			m_apScene2DSelect[nCntPause]->Uninit();		// 終了処理
			m_apScene2DSelect[nCntPause] = NULL;		// NULLへ
		}
	}

	CScene::Release();
}

//=============================================================================
// 更新処理											(public)	*** CPause ***
//=============================================================================
void CPause::Update(void)
{
	CManager::MODE mode = CManager::GetMode();						// モード
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// 入力デバイス
	CInputXPad *pInputXPad = CManager::GetInputXPad();				// 入力デバイス
	CSound *pSound = CManager::GetSound();							// サウンド
	CFade::FADE fade = CFade::GetFade();							// フェード取得

	if (fade == CFade::FADE_NONE)
	{// フェードしていない状態
		for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
		{
			//if (pInputKeyboard->GetTrigger(DIK_S) == true || pInputKeyboard->GetTrigger(DIK_DOWN) == true)
			if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN, nCnt) == true || pInputXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, nCnt) == true)
			{// 下キーを押したら
				m_nSelectNum = (m_nSelectNum + 1) % 3;					// 選択を下へ
				m_nCntTimer = 0;			// カウント初期化

				pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
			}
			//else if (pInputKeyboard->GetTrigger(DIK_W) == true || pInputKeyboard->GetTrigger(DIK_UP) == true)
			else if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, nCnt) == true || pInputXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, nCnt) == true)
			{// 上キーを押したら
				m_nSelectNum = (m_nSelectNum + 2) % 3;					// 選択を上へ
				m_nCntTimer = 0;			// カウント初期化

				pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
			}
		}
	}

	m_nCntTimer++;		// カウント

	for (int nCntPause = 0; nCntPause < PAUSE_MAX_SELECT; nCntPause++)
	{// カウント
		if (m_apScene2DSelect[nCntPause] != NULL)
		{// NULL以外の場合
			if (nCntPause == m_nSelectNum)
			{// 選択番号と一致したら
				if (m_nCntTimer % 90 < 45)
				{// 一定フレームで
					m_apScene2DSelect[nCntPause]->Setcol(D3DXCOLOR(m_apScene2DSelect[nCntPause]->Getcol().r - (0.5f / 45.0f), m_apScene2DSelect[nCntPause]->Getcol().g - (0.5f / 45.0f), m_apScene2DSelect[nCntPause]->Getcol().b - (0.5f / 45.0f), 1.0f));		// 色変更
				}
				else
				{// その他
					m_apScene2DSelect[nCntPause]->Setcol(D3DXCOLOR(m_apScene2DSelect[nCntPause]->Getcol().r + (0.5f / 45.0f), m_apScene2DSelect[nCntPause]->Getcol().g + (0.5f / 45.0f), m_apScene2DSelect[nCntPause]->Getcol().b + (0.5f / 45.0f), 1.0f));		// 色変更
				}
			}
			else
			{// その他
				m_apScene2DSelect[nCntPause]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// 色変更
			}

			m_apScene2DSelect[nCntPause]->Update();			// 更新処理
		}
	}

	if (fade == CFade::FADE_NONE)
	{// フェードしていない状態
		for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
		{
			//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_A, nCnt) == true || pInputXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true)
			{// ENTERキーを押したら
				switch (m_nSelectNum)
				{// 選択番号
				case 0:			// 選択解除
					if (mode == CManager::MODE_TUTORIAL)
					{
						CTutorial::SetPause(false);
					}
					else
					{
						CGame::SetPause(false);
					}

					break;
				case 1:			// リトライ
					if (mode == CManager::MODE_TUTORIAL)
					{
						CFade::Create(CManager::MODE_TUTORIAL, CFade::FADE_OUT);
					}
					else
					{
						CFade::Create(CManager::MODE_GAME, CFade::FADE_OUT);
					}

					break;
				case 2:			// タイトル
					CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);

					break;
				}

				pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
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