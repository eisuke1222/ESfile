//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "game.h"			// ゲーム
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "fade.h"			// フェード
#include "text.h"			// テキスト
#include "texture.h"		// テクスチャ
#include "effect.h"			// エフェクト
#include "ranking.h"		// ランキング

#include "pause.h"			// ポーズ
#include "ui.h"				// UI
#include "map.h"			// マップ
#include "player.h"			// プレイヤー
#include "enemy.h"			// 敵

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
// コンストラクタ										(public)	*** CGame ***
//=============================================================================
CGame::CGame()
{
	// 初期値設定
	m_game					= GAME_TUTORIAL;					// ゲーム
	m_pPause				= NULL;								// ポーズ
	m_bPause				= false;							// ポーズしてるか

	m_pMap					= NULL;								// マップ
	m_pUi					= NULL;								// UI
	m_pPlayer				= NULL;								// プレイヤー

	m_nMapNum				= 0;								// マップ番号
	posOld					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回位置

	m_nSetEnemyNum			= 0;								// 敵総数

	m_nCntTimer				= 0;								// 時間カウント

	m_bSetSS				= false;							// スクリーンショットを撮る
	m_bSetSS_Death			= false;							// スクリーンショットを撮る 
	m_bPlay_SS00			= false;							// スクリーンショット00
	m_bPlay_SS01			= false;							// スクリーンショット01
	m_bPlay_SS02			= false;							// スクリーンショット02

	m_nTutorialNum			= 0;								// チュートリアル番号
	m_nCntTutorial			= 0;								// チュートリアルカウント

	for (int nCount = 0; nCount < TUTORIAL_MAX; nCount++)
		m_bTutorial[nCount] = false;							// チュートリアル

	m_pClear				= NULL;								// クリア

	m_nCntCheck				= 0;								// チェックカウント
	m_pCheck				= NULL;								// チェック

	m_nCntSkip				= 0;								// スキップカウント
	m_pSkip					= NULL;								// スキップ

	m_nPlayerHP				= CPlayer::m_MAX_HP;				// プレイヤーHP
	m_nPlayerMP				= CPlayer::m_MAX_MP;				// プレイヤーMP

	m_nTimer				= 0;								// タイマー
	m_nDeathEnemy			= 0;								// 倒した敵総数
	m_bNoDamage				= true;								// ノーダメージ
	m_nJastGuard			= 0;								// ジャストガード成功
	m_bAllDeathEnemy		= true;								// 全ての敵を倒した

	m_nCntTutorialPlus		= 0;								// チュートリアルプラスカウント
	m_pTutorialPlus			= NULL;								// チュートリアルプラス
	m_pTutorialPlusButton	= NULL;								// チュートリアルプラスボタン
}

//=============================================================================
// デストラクタ										(public)	*** CGame ***
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ロード処理										(public)	*** CGame ***
//=============================================================================
HRESULT CGame::Load(void)
{
	// ロード
	CUi_Game::Load();			// Uiゲーム
	CMap_02::Load();			// マップ
	CMap_03::Load();			// マップ
	CEffect::Load();			// エフェクト
	CPlayer::Load();			// プレイヤー
	CEnemy::Load();				// 敵
	CRanking::LoadRanking();	// ランキングロード

	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CGame ***
//=============================================================================
void CGame::Unload(void)
{
	// アンロード
	CUi_Game::Unload();			// Uiゲーム
	CUi_Result::Unload();		// Uiリザルト
	CMap_02::Unload();			// マップ
	CMap_03::Unload();			// マップ
	CEffect::Unload();			// エフェクト
	CPlayer::Unload();			// プレイヤー
	CEnemy::Unload();			// 敵

	CTexture::ReleaseAll();		// テクスチャ破棄
	CText::ReleaseXModelData();	// モデルデータ破棄
}

//=============================================================================
// 初期化処理										(public)	*** CGame ***
//=============================================================================
HRESULT CGame::Init(void)
{
	CSound *pSound = CManager::GetSound();		// サウンド取得
	if (pSound != NULL)
	{// NULL以外の場合
		if (m_game == GAME_TUTORIAL)
		{// チュートリアルだったら
			pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME000);
		}
	}

	CCamera *pCamera = NULL;	// カメラ

	D3DXVECTOR3 posPlayer;		// プレイヤー位置
	
	//m_game = GAME_PLAY_1;
	//m_game = GAME_PLAY_B;
	if (m_game == GAME_TUTORIAL || m_game == GAME_PLAY_B)
	{// チュートリアル、ボスだったら
		posPlayer = D3DXVECTOR3(0.0f, 0.0f, -500.0f);
	}
	else if (m_game == GAME_PLAY_1)
	{// プレイ１だったら
		posPlayer.x = (float)((rand() % ((1470 - 250) * 2)) - (1470 - 250));
		posPlayer.y = 0.0f;
		posPlayer.z = (float)((rand() % ((1470 - 250) * 2)) - (1470 - 250));
	}
	else
	{// その他
		posPlayer = posOld;
	}

	// メインカメラ
	pCamera = CManager::GetCamera(CManager::CAMERA_MAIN);	// カメラ取得
	pCamera->SetPos(D3DXVECTOR3(posPlayer.x, 120.0f, posPlayer.z - 230.0f), D3DXVECTOR3(posPlayer.x, 50.0f, posPlayer.z), D3DXVECTOR3(-0.3f, D3DX_PI, 0.0f));		// カメラ位置設定
	pCamera->SetInitPos(D3DXVECTOR3(posPlayer.x, 120.0f, posPlayer.z - 230.0f), D3DXVECTOR3(posPlayer.x, 50.0f, posPlayer.z));	// カメラ初期位置設定
	CCameraMain *pCameraMain = (CCameraMain*)pCamera;
	pCameraMain->SetMove(true);
	// マップカメラ
	pCamera = CManager::GetCamera(CManager::CAMERA_GAME_MAP);	// カメラ取得
	pCamera->SetPos(D3DXVECTOR3(0.0f, 4000.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// カメラ位置設定
	pCamera->SetInitPos(D3DXVECTOR3(0.0f, 4000.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// カメラ初期位置設定
//	pCamera->SetPos(D3DXVECTOR3(0.0f, 500.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// カメラ位置設定
//	pCamera->SetInitPos(D3DXVECTOR3(0.0f, 500.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// カメラ初期位置設定

	// クリエイト
	// UI
	m_pUi = CUi::Create(CUi::UI_GAME);
	
	// 敵総数初期化
	m_nSetEnemyNum = 0;

	// マップ
	if (m_game == GAME_TUTORIAL || m_game == GAME_PLAY_B)
	{// チュートリアル、ボスだったら
		m_pMap = CMap::Create(CMap::TYPE_NO_04);
		CMap_04 *pMap_04 = (CMap_04*)m_pMap;
		pMap_04->SetField(posPlayer);
		pMap_04->SetObject();

		m_nMapNum = 4;
	}
	else
	{// その他
		int nNum = 1;
		
		/*if (-1200.0f < posOld.x && posOld.x < 1200.0f &&
			-1200.0f < posOld.z && posOld.z < 1200.0f)
		{// 範囲外だったら
			nNum = 1;
		}
		else
		{
			nNum = rand() % 10;
		}*/

		if (nNum == 0)
		{
			m_pMap = CMap::Create(CMap::TYPE_NO_03);
			CMap_03 *pMap_03 = (CMap_03*)m_pMap;
			pMap_03->SetField(posPlayer);
			pMap_03->SetObject();

			m_nMapNum = 3;
		}
		else
		{
			m_pMap = CMap::Create(CMap::TYPE_NO_02);
			CMap_02 *pMap_02 = (CMap_02*)m_pMap;
			pMap_02->SetField(posPlayer);
			pMap_02->SetObject();

			m_nMapNum = 2;
		}
	}

	// プレイヤー
	m_pPlayer = CPlayer::Create(posPlayer, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pPlayer->SetMove(true);

	if (m_game != CGame::GAME_TUTORIAL && m_game != CGame::GAME_PLAY_1)
	{// チュートリアル + プレイ１以外
		m_pPlayer->SetHP(m_nPlayerHP);
		m_pPlayer->SetMP(m_nPlayerMP);
	}

	if (m_game == GAME_TUTORIAL)
	{// チュートリアル
		if (m_pPlayer != NULL)
			m_pPlayer->SetAction(true, true);	// 制限

		if (m_pClear == NULL)
		{// NULLの場合
			m_pClear = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			m_pClear->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 10), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 10), (SCREEN_HEIGHT / 12), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pClear->Init();
			m_pClear->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CLEAR));
		}

		if (m_pCheck == NULL)
		{// NULLの場合
			m_pCheck = new CScene3D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			m_pCheck->SetInitAll(D3DXVECTOR3(0.0f, 120.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 20.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
			m_pCheck->Init();
			m_pCheck->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CHECK));
			m_pCheck->DrawEdit(false, false, false, true, true, 0, D3DCULL_NONE, D3DCMP_ALWAYS);
			m_pCheck->SetTex(8, 3, D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		}

		if (m_pSkip == NULL)
		{// NULLの場合
			m_pSkip = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			m_pSkip->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7) - (SCREEN_WIDTH / 8), SCREEN_HEIGHT - (SCREEN_WIDTH / 24 * 2) - (SCREEN_HEIGHT / 10 * 2) - (SCREEN_HEIGHT / 100), 0.0f), D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 100), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pSkip->Init();
		}

		if (m_pTutorialPlus == NULL)
		{// NULLの場合
			m_pTutorialPlus = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			m_pTutorialPlus->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pTutorialPlus->Init();
			m_pTutorialPlus->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_TUTORIAL_PLUS));
		}

		if (m_pTutorialPlusButton == NULL)
		{// NULLの場合
			m_pTutorialPlusButton = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			m_pTutorialPlusButton->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 35), SCREEN_HEIGHT - (SCREEN_WIDTH / 35), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 40), (SCREEN_WIDTH / 40), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.125f, 1.0f));
			m_pTutorialPlusButton->Init();
			m_pTutorialPlusButton->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
			m_pTutorialPlusButton->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		}

		if (m_pPlayer != NULL)
			m_pPlayer->SetMove(false);		// 制限
		if (pCameraMain != NULL)
			pCameraMain->SetMove(false);	// 制限
	}

	CManager::GetRenderer()->SetDrawMap(true);		// マップ描画

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CGame ***
//=============================================================================
void CGame::Uninit(void)
{
	// ポーズ
	if (m_pPause != NULL)
	{// NULL以外の場合
		m_pPause->Uninit();		// 終了処理
		m_pPause = NULL;		// NULLへ
	}

	// マップ
	if (m_pMap != NULL)
	{// NULL以外の場合
		m_pMap->Uninit();		// 終了処理
		m_pMap = NULL;			// NULLへ
	}

	// UI
	if (m_pUi != NULL)
	{// NULL以外の場合
		m_pUi->Uninit();		// 終了処理
		m_pUi = NULL;			// NULLへ
	}

	// プレイヤー
	if (m_pPlayer != NULL)
	{// NULL以外の場合
		m_pPlayer->Uninit();	// 終了処理
		m_pPlayer = NULL;		// NULLへ
	}

	// クリア
	if (m_pClear != NULL)
	{// NULL以外の場合
		m_pClear->Uninit();		// 終了処理
		m_pClear = NULL;		// NULLへ
	}

	// チェック
	if (m_pCheck != NULL)
	{// NULL以外の場合
		m_pCheck->Uninit();		// 終了処理
		m_pCheck = NULL;		// NULLへ
	}

	// スキップ
	if (m_pSkip != NULL)
	{// NULL以外の場合
		m_pSkip->Uninit();		// 終了処理
		m_pSkip = NULL;			// NULLへ
	}

	// チュートリアルプラス
	if (m_pTutorialPlus != NULL)
	{// NULL以外の場合
		m_pTutorialPlus->Uninit();		// 終了処理
		m_pTutorialPlus = NULL;			// NULLへ
	}

	// チュートリアルプラスボタン
	if (m_pTutorialPlusButton != NULL)
	{// NULL以外の場合
		m_pTutorialPlusButton->Uninit();		// 終了処理
		m_pTutorialPlusButton = NULL;			// NULLへ
	}

	CScene::ReleaseMode();		// モード破棄
}

//=============================================================================
// 更新処理											(public)	*** CGame ***
//=============================================================================
void CGame::Update(void)
{
	CSound *pSound = CManager::GetSound();						// サウンド取得

	CInputKeyboard	*pKey = CManager::GetInputKeyboard();		// キーボード取得
	CInputMouse		*pMouse = CManager::GetInputMouse();		// マウス取得
	CInputXPad		*pXPad = CManager::GetInputXPad();			// Xパッド取得
	CFade::FADE		fade = CFade::GetFade();					// フェード取得
	CCameraMain		*pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);	// カメラ取得

	if (m_game != GAME_CLEAR && m_game != GAME_OVER)
	{// クリア + オーバー以外
		if (pKey != NULL)
		{// NULL以外の場合
			if (fade == CFade::FADE_NONE)
			{// フェードしていない
				if (pKey->GetTrigger(DIK_P) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_START, 0) == true)
				{// キーボード（P）が押された、Xパッド（START、12）が押された
					PauseSeting();		// ポーズ設定

					if (pSound != NULL)
					{// NULL以外の場合
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_PAUSE000);
					}
				}
			}
		}
	}

	if (m_game == GAME_TUTORIAL)
	{// チュートリアル面
		if (m_pTutorialPlus != NULL || m_pTutorialPlusButton != NULL)
		{// NULL以外の場合
			if (fade == CFade::FADE_NONE)
			{// フェードしていない
				if (pKey->GetPress(DIK_Z) == true || pKey->GetPress(DIK_E) == true || pKey->GetPress(DIK_RETURN) == true || pXPad->GetPress(XINPUT_GAMEPAD_B, 0) == true)
				{// アクションを押した
					if (m_pPlayer != NULL)
						m_pPlayer->SetMove(true);		// 動ける
					if (pCamera != NULL)
						pCamera->SetMove(true);			// 動ける

					if (m_nCntTutorialPlus < 150)
					{// 一定フレームより低かったら
						m_nCntTutorialPlus = 150;

						if (pSound != NULL)
						{// NULL以外の場合
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
						}
					}
				}

				if (m_nCntTutorialPlus == 150)
				{// 一定フレームだったら
					if (m_pTutorialPlus != NULL)
					{// NULL以外の場合
						D3DXCOLOR col = m_pTutorialPlus->Getcol();
						col.a -= 0.05f;
						m_pTutorialPlus->Setcol(col);

						if (col.a <= 0.0f)
						{// 透明になったら
							m_pTutorialPlus->Uninit();			// 終了処理
							m_pTutorialPlus = NULL;				// NULLへ
						}
					}

					if (m_pTutorialPlusButton != NULL)
					{// NULL以外の場合
						D3DXCOLOR col = m_pTutorialPlusButton->Getcol();
						col.a -= 0.05f;
						m_pTutorialPlusButton->Setcol(col);

						if (col.a <= 0.0f)
						{// 透明になったら
							m_pTutorialPlusButton->Uninit();	// 終了処理
							m_pTutorialPlusButton = NULL;		// NULLへ
						}
					}
				}
				else
				{// その他
					if (m_pTutorialPlus != NULL)
					{// NULL以外の場合
						if (m_nCntTutorialPlus == 0)
						{// 一定フレームだったら
							if (m_pTutorialPlusButton != NULL)
								m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
						}
						else if (m_nCntTutorialPlus == 60)
						{// 一定フレームだったら
							if (m_pTutorialPlusButton != NULL)
								m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
						m_nCntTutorialPlus = (m_nCntTutorialPlus + 1) % 120;	// カウント
					}
				}
			}
		}
		else
		{// NULLの場合
			if (m_nCntTutorial == 0)
			{// ０だったら
				if (m_bTutorial[m_nTutorialNum] == false)
				{// クリアしていない
					if (fade == CFade::FADE_NONE)
					{// フェードしていない
						if (m_nTutorialNum == TUTORIAL_MOVE)
						{// 移動
							if (pKey->GetPress(DIK_W) == true || pKey->GetPress(DIK_S) == true || pKey->GetPress(DIK_A) == true || pKey->GetPress(DIK_D) == true ||
								pKey->GetPress(DIK_UP) == true || pKey->GetPress(DIK_DOWN) == true || pKey->GetPress(DIK_LEFT) == true || pKey->GetPress(DIK_RIGHT) == true ||
								pXPad->GetStick(0, 0) == true)
							{// 移動キーを押した
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								m_nCntTutorial = 40;
							}
						}
						else if (m_nTutorialNum == TUTORIAL_CAMERA)
						{// カメラ
							if (pMouse->GetPointer().x != 0.0f || pMouse->GetPointer().y != 0.0f ||
								pXPad->GetStick(1, 0) == true)
							{// カメラを動かした
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								m_nCntTutorial = 40;
							}
						}
						else if (m_nTutorialNum == TUTORIAL_JUMP)
						{// ジャンプ
							if (pKey->GetPress(DIK_SPACE) == true ||
								pXPad->GetPress(XINPUT_GAMEPAD_A, 0) == true)
							{// ジャンプキーを押した
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								m_nCntTutorial = 40;
								CEnemy::Create(CEnemy::TYPE_N_SLIME, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
								CEffect::Create(CEffect::TYPE_CREATE, CEffect::SHOT_NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}
						}
						else if (m_nTutorialNum == TUTORIAL_LOCKON)
						{// ロックオン
							if (pCamera->GetLockOn() == true)
							{// ロックオンした
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								if (m_pPlayer != NULL)
									m_pPlayer->SetAction(false, true);

								m_nCntTutorial = 40;
							}
						}
						else if (m_nTutorialNum == TUTORIAL_ATTACK)
						{// 攻撃
							if (m_nSetEnemyNum == 0)
							{// 倒した
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								m_nCntTutorial = 40;
							}
						}
						else if (m_nTutorialNum == TUTORIAL_GUARD)
						{// ガード
							if (pKey->GetPress(DIK_V) == true ||
								pMouse->GetPress(2) == true ||
								pXPad->GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER, 0) == true)
							{// ガードキーを押した
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								if (m_pPlayer != NULL)
									m_pPlayer->SetAction(false, false);

								m_nCntTutorial = 40;
								CEnemy::Create(CEnemy::TYPE_N_SLIME, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
								CEffect::Create(CEffect::TYPE_CREATE, CEffect::SHOT_NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}
						}
						else if (m_nTutorialNum == TUTORIAL_SKILL)
						{// スキル
							if (m_nSetEnemyNum == 0)
							{// 倒した
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								CMap_04 *pMap_04 = (CMap_04*)m_pMap;
								if (pMap_04->GetGoal() == false)
									pMap_04->SetGoal(true);

								m_nCntTutorial = 40;
							}
						}
						else if (m_nTutorialNum == TUTORIAL_ACTION)
						{// アクション
							if (fade != CFade::FADE_NONE)
							{// フェードした
								m_bTutorial[m_nTutorialNum] = true;
								//m_nTutorialNum++;

								m_nCntTutorial = 40;
							}

							if (m_pCheck != NULL)
							{// NULL以外の場合
								m_pCheck->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

								D3DXVECTOR3 pos = m_pCheck->Getpos();		// 位置取得
								
								// 上下に移動
								if (m_nCntCheck < 60)
									pos.y -= 0.1f;
								else
									pos.y += 0.1f;

								m_pCheck->Setpos(pos);						// 移動設定

								m_nCntCheck = (m_nCntCheck + 1) % 120;		// カウント
							}
						}
					}
				}
			}
			else
			{// 0以外だったら
				if (m_nCntTutorial == 40)
				{// 一定フレームだったら
					if (m_bTutorial[m_nTutorialNum] == true)
					{// クリアだったら
						if (m_pClear != NULL)
						{// NULL以外の場合
							m_pClear->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 10), 0.0f));
							m_pClear->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 10) * 2.0f, (SCREEN_HEIGHT / 12) * 2.0, 0.0f));
							m_pClear->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							m_pClear->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CLEAR));
							m_pClear->SetSizeType(10, D3DXVECTOR3((SCREEN_WIDTH / 10), (SCREEN_HEIGHT / 12), 0.0f));
						}

						if (pSound != NULL)
						{// NULL以外の場合
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_TUTORIAL_CLEAR);
						}
					}
				}
				else if (m_nCntTutorial == 20)
				{// 一定フレームだったら
					if (m_bTutorial[m_nTutorialNum] == true)
					{// クリアだったら
						if (m_pClear != NULL)
						{// NULL以外の場合
							m_pClear->SetcolType(10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
						}

						if (m_nTutorialNum < TUTORIAL_ACTION)	// アクションより前のみ
							m_nTutorialNum++;
					}
				}
				else if (m_nCntTutorial == 1)
				{// 一定フレームだったら
					if (m_nTutorialNum == TUTORIAL_ATTACK || m_nTutorialNum == TUTORIAL_SKILL)
					{// 攻撃、スキルだったら
						if (m_pClear != NULL)
						{// NULL以外の場合
							if (m_pClear->Getcol().a != 1.0f)
							{// 不透明以外
								//m_pClear->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 6), 0.0f));
								m_pClear->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 2), SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.6f), 0.0f));
								m_pClear->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 10) * 2.0f, (SCREEN_HEIGHT / 12) * 2.0, 0.0f));
								m_pClear->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
								m_pClear->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_MISION));
								m_pClear->SetSizeType(10, D3DXVECTOR3((SCREEN_WIDTH / 8), (SCREEN_HEIGHT / 14), 0.0f));

								m_nCntTutorial = 39;
							}
						}
					}
				}

				m_nCntTutorial--;
				if (m_nCntTutorial < 0) m_nCntTutorial = 0;
			}

			if (fade == CFade::FADE_NONE)
			{// フェードしていない
				if (pKey->GetPress(DIK_Z) == true || pKey->GetPress(DIK_E) == true || pKey->GetPress(DIK_RETURN) == true || pXPad->GetPress(XINPUT_GAMEPAD_B, 0) == true)
				{// アクションを押した
					if (m_nCntSkip < 60)
					{// 一定フレームより低かったら
						m_nCntSkip++;		// カウント
					}
					else if (m_nCntSkip == 60)
					{// 一定フレームになったら
						if (m_game < GAME_PLAY_B)
						{// ボスより前だったら
							m_game = (CGame::GAME)(m_game + 1);
							CFade::Create(m_game);
							m_pPlayer->SetMove(false);

							if (pSound != NULL)
							{// NULL以外の場合
								pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
							}
						}
					}
				}
				else
				{// 押していない
					m_nCntSkip = 0;
				}

				if (m_pSkip != NULL)
				{// NULL以外の場合
					// 位置と大きさ
					m_pSkip->Setpos(D3DXVECTOR3(((SCREEN_WIDTH / 7) - (SCREEN_WIDTH / 8)) + ((SCREEN_WIDTH / 8) * ((float)m_nCntSkip / 60.0f)), SCREEN_HEIGHT - (SCREEN_WIDTH / 24 * 2) - (SCREEN_HEIGHT / 10 * 2) - (SCREEN_HEIGHT / 100), 0.0f));
					m_pSkip->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 8) * ((float)m_nCntSkip / 60.0f), (SCREEN_HEIGHT / 100), 0.0f));
				}
			}
		}
	}
	else if (m_game == GAME_PLAY_B)
	{// ボス面
		if (m_nSetEnemyNum == 0)
		{// 敵が０
			m_game = GAME_CLEAR;
			m_nCntTimer = 0;

			if (m_bPlay_SS02 == false)
			{
				if (fade == CFade::FADE_NONE)
				{// フェードしていない
					if (ScreenShot("result02", true))
						m_bPlay_SS02 = true;
				}
			}
		}
		else
		{// 敵がいる
			if (m_nCntTimer == 0)
			{// ０だったら
				m_pPlayer->SetMove(false);		// 移動できない
				pCamera->SetPickUp(true);		// ピックアップ
				m_nCntTimer = 1;

				CScene::SetDrawUi(false);		// UI非表示

				if (pSound != NULL)
				{// NULL以外の場合
					pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME000);
				}
			}

			if (m_nCntTimer == 1 && pCamera->GetPickUp() == false)
			{// ピックアップが終了した
				m_pPlayer->SetMove(true);		// 移動できる
				m_nCntTimer = 2;

				CScene::SetDrawUi(true);		// UI表示

				if (pSound != NULL)
				{// NULL以外の場合
					pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME001);
				}
			}
		}
	}
	else if (m_game == GAME_CLEAR)
	{// クリア
		if (m_nCntTimer == 0)
		{// 一定フレームだったら
			CScene::SetStartUp(CScene::PRIORITY_UI);
			pCamera->SetMove(false);
			m_pPlayer->SetMove(false);

			if (pSound != NULL)
			{// NULL以外の場合
				pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME001);
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_GAMEEND);
			}
		}
		else if (m_nCntTimer == 45)
		{// 一定フレームだったら
			CScene::SetUpdateTime(4);					// スロー
			CScene::SetStartUp(CScene::PRIORITY_BG);	// 背景から更新
		}
		else if (m_nCntTimer == 165)
		{// 一定フレームだったら
			CScene::SetStartUp(CScene::PRIORITY_UI);
			CScene::SetUpdateTime(1);

			CScene2D_Preset *pScene2D_Preset = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			pScene2D_Preset->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			pScene2D_Preset->Init();
			pScene2D_Preset->SetcolType(30, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.75f));
		}
		else if (m_nCntTimer == 210)
		{// 一定フレームだったら
			int nScore = GetScore();		// スコア取得

			// クリア
			nScore += 15000;

			// ボス撃破
			nScore += 1000;

			CRanking::SetScore(nScore);		// スコア設定
			CRanking::SaveRanking();		// ランキングセーブ
			CUi_Result::Load();				// Uiリザルト
			CUi::Create(CUi::UI_RESULT);	// Uiリザルト生成

			if (m_pTutorialPlusButton == NULL)
			{// NULLの場合
				m_pTutorialPlusButton = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
				m_pTutorialPlusButton->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 35), SCREEN_HEIGHT - (SCREEN_WIDTH / 35), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 40), (SCREEN_WIDTH / 40), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.125f, 1.0f));
				m_pTutorialPlusButton->Init();
				m_pTutorialPlusButton->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
				m_pTutorialPlusButton->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			}

			if (pSound != NULL)
			{// NULL以外の場合
				pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME002);
			}
		}

		if (210 < m_nCntTimer)
		{// 一定フレームより大きかったら
			if (pKey != NULL)
			{// NULL以外の場合
				if (fade == CFade::FADE_NONE)
				{// フェードしていない
					if (pKey->GetTrigger(DIK_RETURN) == true || pKey->GetTrigger(DIK_E) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
					{// キーボード（P）が押された、Xパッド（START、12）が押された
						CFade::Create(CManager::MODE_TITLE);

						if (pSound != NULL)
						{// NULL以外の場合
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
						}
					}
				}
			}
		}

		m_nCntTimer++;		// カウント

		if (m_pTutorialPlusButton != NULL)
		{// NULL以外の場合
			if (m_nCntTutorialPlus == 0)
			{// 一定フレームだったら
				m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
			else if (m_nCntTutorialPlus == 60)
			{// 一定フレームだったら
				m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			m_nCntTutorialPlus = (m_nCntTutorialPlus + 1) % 120;		// カウント
		}
	}
	else if (m_game == GAME_OVER)
	{// オーバー
		if (m_nCntTimer == 0)
		{// 一定フレームだったら
			CScene::SetUpdateTime(4);						// スロー
			CManager::GetRenderer()->SetDrawMap(false);		// マップ非表示

			if (m_pUi != NULL)
			{// NULL以外の場合
				m_pUi->Uninit();
				m_pUi = NULL;
			}

			if (pSound != NULL)
			{// NULL以外の場合
				pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME001);
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_GAMEEND);
			}
		}
		else if (m_nCntTimer == 165)
		{// 一定フレームだったら
			//CScene::SetStartUp(CScene::PRIORITY_UI);
			CScene::SetUpdateTime(1);

			CScene2D_Preset *pScene2D_Preset = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			pScene2D_Preset->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			pScene2D_Preset->Init();
			pScene2D_Preset->SetcolType(30, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.75f));
		}
		else if (m_nCntTimer == 210)
		{// 一定フレームだったら
			CUi_Result::Load();				// Uiリザルト
			CUi::Create(CUi::UI_RESULT);	// Uiリザルト生成

			if (m_pTutorialPlusButton == NULL)
			{// NULLの場合
				m_pTutorialPlusButton = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
				m_pTutorialPlusButton->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 35), SCREEN_HEIGHT - (SCREEN_WIDTH / 35), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 40), (SCREEN_WIDTH / 40), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.125f, 1.0f));
				m_pTutorialPlusButton->Init();
				m_pTutorialPlusButton->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
				m_pTutorialPlusButton->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			}

			if (pSound != NULL)
			{// NULL以外の場合
				pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME002);
			}
		}

		if (210 < m_nCntTimer)
		{// 一定フレームより大きかったら
			if (pKey != NULL)
			{// NULL以外の場合
				if (fade == CFade::FADE_NONE)
				{// フェードしていない
					if (pKey->GetTrigger(DIK_RETURN) == true || pKey->GetTrigger(DIK_E) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
					{// キーボード（P）が押された、Xパッド（START、12）が押された
						CFade::Create(CManager::MODE_TITLE);

						if (pSound != NULL)
						{// NULL以外の場合
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
						}
					}
				}
			}
		}

		m_nCntTimer++;		// カウント

		if (m_pTutorialPlusButton != NULL)
		{// NULL以外の場合
			if (m_nCntTutorialPlus == 0)
			{// 一定フレームだったら
				m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
			else if (m_nCntTutorialPlus == 60)
			{// 一定フレームだったら
				m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			m_nCntTutorialPlus = (m_nCntTutorialPlus + 1) % 120;		// カウント
		}
	}

	fade = CFade::GetFade();					// フェード取得
	if (m_bPlay_SS00 == false)
	{// まだ撮っていない
		if (m_game == GAME_PLAY_1)
		{// プレイ１だったら
			if (fade == CFade::FADE_NONE)
			{// フェードしていない
				if (ScreenShot("result00", true))
					m_bPlay_SS00 = true;
			}
		}
	}

	if (m_bSetSS == true)
	{// スクショを撮る
		if (m_bPlay_SS01 == false)
		{// まだ撮っていない
			if (GAME_PLAY_1 <= m_game)
			{// プレイ１以上だったら
				if (fade == CFade::FADE_NONE)
				{// フェードしていない
					if (ScreenShot("result01", true))
						m_bPlay_SS01 = true;
				}
			}
		}
	}
	
	if (m_bSetSS_Death == true)
	{// スクショを撮る
		if (m_bPlay_SS02 == false)
		{// まだ撮っていない
			if (GAME_PLAY_1 <= m_game)
			{// プレイ１以上だったら
				if (fade == CFade::FADE_NONE)
				{// フェードしていない
					if (ScreenShot("result02", true))
						m_bPlay_SS02 = true;
				}
			}
		}
	}

	m_bSetSS = false;
	m_bSetSS_Death = false;

	if (fade == CFade::FADE_NONE)
	{// フェードしていない
		if (m_bPause == false)
		{// ポーズしていない
			if (m_game != GAME_TUTORIAL && m_game != GAME_CLEAR && m_game != GAME_OVER)
			{// チュートリアル + クリア + オーバー以外
				m_nTimer++;
			}
		}
	}
}

//=============================================================================
// 描画処理											(public)	*** CGame ***
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// ゲーム設定処理											(public)	*** CGame ***
//=============================================================================
void CGame::SetGame(CGame::GAME game, bool bReset)
{
	if (m_pPlayer != NULL)
	{// NULL以外の場合
		if (m_game != CGame::GAME_TUTORIAL)
		{// チュートリアル以外
			m_nPlayerHP = m_pPlayer->GetHP();
			m_nPlayerMP = m_pPlayer->GetMP();
		}
	}

	if (bReset == true)
	{// リセットする
		if (m_game != GAME_TUTORIAL && m_game != GAME_CLEAR && m_game != GAME_OVER)
		{// チュートリアル + クリア + オーバー以外
			if (m_nSetEnemyNum != 0)
			{// 敵がいる
				m_bAllDeathEnemy = false;
			}
		}

		posOld = m_pPlayer->Getpos();		// 位置記憶

		// 終了処理
		Uninit();

		// カメラリセット
		{
			CCameraMain *pCameraMein = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
			CCameraMap *pCameraMap = (CCameraMap*)CManager::GetCamera(CManager::CAMERA_GAME_MAP);

			if (pCameraMein != NULL)
				pCameraMein->Uninit();		// 終了処理
			if (pCameraMap != NULL)
				pCameraMap->Uninit();		// 終了処理
		}
	}

	// ゲームを更新
	m_game = game;
	m_nCntTimer = 0;

	if (bReset == true)
	{// リセットする
		// 初期化処理
		Init();
	}
}

//=============================================================================
// ポーズ設定処理										(public)	*** CGame ***
//=============================================================================
void CGame::PauseSeting(void)
{
	CCameraMain		*pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	m_bPause = m_bPause ? false : true;		// ポーズ切り替え

	if (m_bPause == true)
	{// ポーズしている
		if (m_pPause == NULL)
		{// NULLの場合
			m_pPause = CPause::Create();	// ポーズ
		}
		CScene::SetStartUp(CScene::PRIORITY_PAUSE);		// 更新範囲（ポーズから）
		pCamera->SetMove(false);

		if (pCamera->GetPickUp() == true)
			CScene::SetDrawUi(true);
	}
	else
	{// ポーズしていない
		if (m_pPause != NULL)
		{// NULL以外の場合
			m_pPause->Uninit();	// 終了処理
			m_pPause = NULL;	// NULLへ
		}
		CScene::SetStartUp(0);							// 更新範囲（０から）
		pCamera->SetMove(true);

		if (pCamera->GetPickUp() == true)
			CScene::SetDrawUi(false);
	}
}

//=============================================================================
// スコア取得処理										(public)	*** CGame ***
//=============================================================================
int CGame::GetScore(void)
{
	int nScore = 0;

	// タイム
	if (m_nTimer < 4000) nScore += 15000;
	else if (m_nTimer < 4500) nScore += 12000;
	else if (m_nTimer < 5000) nScore += 10000;
	else if (m_nTimer < 5500) nScore += 8000;
	else if (m_nTimer < 6000) nScore += 5000;
	else nScore += 3000;

	// 撃破数
	nScore += (500 * m_nDeathEnemy);

	// ノーダメージ
	if (m_bNoDamage == true) nScore += 10000;
	else if (m_pPlayer->GetHP() == CPlayer::m_MAX_HP) nScore += 5000;

	// ジャストガード成功
	nScore += (800 * m_nJastGuard);

	// 全て撃破
	if (m_bAllDeathEnemy == true) nScore += 10000;

	// ボス直行
	if (m_nTimer < 3000) nScore += 5000;

	return nScore;
}