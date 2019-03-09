//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "title.h"			// タイトル
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "sound.h"			// サウンド
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "fade.h"			// フェード
#include "window.h"			// ウィンドウ
#include "text.h"			// テキスト
#include "texture.h"		// テクスチャ
#include "effect.h"			// エフェクト
#include "ui.h"				// UI
#include "scene2d.h"		// シーン２D
#include "map.h"			// マップ
#include "ranking.h"		// ランキング
#include "player.h"			// プレイヤー

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
// コンストラクタ										(public)	*** CTitle ***
//=============================================================================
CTitle::CTitle()
{
	// 初期値設定
	m_pPlayer		= NULL;			// プレイヤー

	m_bTitle		= false;		// タイトル

	m_nSelectNum	= 0;			// 選択番号
	m_nCntTimer		= 0;			// 時間カウンタ

	m_bTutorial		= false;		// チュートリアルか
	m_bSelectSet	= false;		// 選択設定しているか

	m_pWindow		= NULL;			// ウィンドウ
	m_pIcon			= NULL;			// アイコン
	m_pReturnCircle = NULL;			// 戻るサークル

	for (int nCount = 0; nCount < m_MAX_WALL; nCount++)
		m_pWall[nCount] = NULL;		// 壁

	m_pUi_Game		= NULL;			// UI

	m_bRanking		= false;		// ランキング
	m_pRanking		= NULL;			// ランキング
	m_pRankingBG	= NULL;			// ランキングBG
}

//=============================================================================
// デストラクタ										(public)	*** CTitle ***
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ロード処理										(public)	*** CTitle ***
//=============================================================================
HRESULT CTitle::Load(void)
{
	// ロード
	CUi_Title::Load();			// UIタイトル
	CMap_01::Load();			// マップ
	CEffect::Load();			// ロード
	CPlayer::Load();			// プレイヤー
	CRanking::LoadRanking();	// ランキングロード

	return S_OK;	// 値を返す
}

//=============================================================================
// アンロード処理										(public)	*** CTitle ***
//=============================================================================
void CTitle::Unload(void)
{
	// アンロード
	CUi_Title::Unload();		// Uiタイトル
	CMap_01::Unload();			// マップ
	CEffect::Unload();			// エフェクト
	CPlayer::Unload();			// プレイヤー

	CTexture::ReleaseAll();		// テクスチャ破棄
	CText::ReleaseXModelData();	// モデルデータ破棄
}

//=============================================================================
// 初期化処理										(public)	*** CTitle ***
//=============================================================================
HRESULT CTitle::Init(void)
{
	CSound *pSound = CManager::GetSound();
	if (pSound != NULL)
	{// NULL以外の場合
		pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME000);
		pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME001);
		pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME002);
		pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE000);
	}

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// デバイス取得
	CCamera *pCamera = CManager::GetCamera(CManager::CAMERA_MAIN);				// カメラ取得
	pCamera->SetPos(D3DXVECTOR3(120.0f, 80.0f, -150.0f), D3DXVECTOR3(80.0f, 30.0f, 0.0f), D3DXVECTOR3(-0.1f, D3DX_PI, 0.0f));		// カメラ位置設定
	pCamera->SetInitPos(D3DXVECTOR3(120.0f, 80.0f, -150.0f), D3DXVECTOR3(80.0f, 30.0f, 0.0f));	// カメラ初期位置設定
	CCameraMain *pCameraMain = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	pCameraMain->SetMove(false);

	// マップカメラ
	pCamera = CManager::GetCamera(CManager::CAMERA_GAME_MAP);	// カメラ取得
	pCamera->SetPos(D3DXVECTOR3(400.0f, 1500.0f, -1.0f), D3DXVECTOR3(400.0f, 0.0f, 0.0f));		// カメラ位置設定
	pCamera->SetInitPos(D3DXVECTOR3(400.0f, 1500.0f, -1.0f), D3DXVECTOR3(400.0f, 0.0f, 0.0f));	// カメラ初期位置設定

	// クリエイト
	// UI生成
	CUi::Create(CUi::UI_TITLE);

	// マップ
	CMap::Create(CMap::TYPE_NO_01);

	// プレイヤー
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	m_bTitle		= true;		// タイトル
	m_bSelectSet	= false;	// 選択設定

	//フォグの設定
	/*FLOAT StartPos	= 100;		// 開始位置
	FLOAT EndPos	= 1000;		// 終了位置
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);								// フォグ：ON
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); // 白色で不透明
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);					// 頂点モード
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);					// テーブルモード
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos));				// 開始位置
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));					// 終了位置*/
	
	if (m_pUi_Game == NULL)
	{// NULLの場合
		m_pUi_Game = CUi::Create(CUi::UI_GAME);
		m_pUi_Game->Uninit();
		m_pUi_Game = NULL;
	}

	CManager::GetRenderer()->SetDrawMap(false);

	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CTitle ***
//=============================================================================
void CTitle::Uninit(void)
{
	CSound *pSound = CManager::GetSound();
	if (pSound != NULL)
	{// NULL以外の場合
		pSound->StopSound(CSound::SOUND_LABEL_BGM_TITLE000);
	}

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// デバイス取得

	// プレイヤー
	if (m_pPlayer != NULL)
	{
		m_pPlayer->Uninit();	// 終了処理
		m_pPlayer = NULL;		// NULLへ
	}

	// ウィンドウ
	if (m_pWindow != NULL)
	{
		m_pWindow->Uninit();	// 終了処理
		m_pWindow = NULL;		// NULLへ
	}

	// アイコン
	if (m_pIcon != NULL)
	{// NULL以外の場合
		m_pIcon->Uninit();		// 終了処理
		m_pIcon = NULL;			// NULLへ
	}
	
	// 戻るサークル
	if (m_pReturnCircle != NULL)
	{// NULL以外の場合
		m_pReturnCircle->Uninit();		// 終了処理
		m_pReturnCircle = NULL;			// NULLへ
	}

	// 壁
	for (int nCount = 0; nCount < m_MAX_WALL; nCount++)
	{// 壁カウント
		if (m_pWall[nCount] != NULL)
		{// NULL以外の場合
			m_pWall[nCount]->Uninit();		// 終了処理
			m_pWall[nCount] = NULL;			// NULLへ
		}
	}

	if (m_pUi_Game != NULL)
	{
		m_pUi_Game->Uninit();	// 終了処理
		m_pUi_Game = NULL;		// NULLへ
	}

	if (m_pRanking != NULL)
	{
		m_pRanking->Uninit();	// 終了処理
		m_pRanking = NULL;	 	// NULLへ
	}

	if (m_pRankingBG != NULL)
	{
		m_pRankingBG->Uninit();	// 終了処理
		m_pRankingBG = NULL;   	// NULLへ
	}

	CScene::ReleaseMode();		// モード破棄

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);							// フォグ：OFF
}

//=============================================================================
// 更新処理											(public)	*** CTitle ***
//=============================================================================
void CTitle::Update(void)
{
	CSound *pSound = CManager::GetSound();	

	CInputKeyboard	*pKey		= CManager::GetInputKeyboard();		// キーボード取得
	CInputXPad		*pXPad		= CManager::GetInputXPad();			// Xパッド取得
	CCameraMain		*pCamera	= (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);		// カメラ取得（メイン）
	CFade::FADE		fade		= CFade::GetFade();					// フェード取得
	bool			bUiEdit		= CUi_Title::GetUiEdit();			// Ui編集中か
	
	if (m_bTitle == true)
	{// タイトル表示
		if (bUiEdit == false)
		{// Ui編集中でない
			if (fade == CFade::FADE_NONE)
			{// フェードしていない
				if (pKey->GetALL(1) == true || pXPad->GetALL(1, 0) == true)
				{// なにか押したら
					m_bTitle = false;
					m_bSelectSet = true;

					if (pSound != NULL)
					{// NULL以外の場合
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
					}
				}
			}
			m_nSelectNum = 0;
		}
	}
	else
	{// メニュー表示
		if (m_bSelectSet == true)
		{// 選択設定
			if (fade == CFade::FADE_NONE)
			{// フェードしていない
				if (pKey->GetTrigger(DIK_W) == true || pKey->GetTrigger(DIK_UP) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, 0) == true)
				{// キーボード（W）が押された、Xパッド（Lスティック上）が押された
					m_nSelectNum = (m_nSelectNum + SELECT_MAX - 1) % SELECT_MAX;

					if (pSound != NULL)
					{// NULL以外の場合
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SERECT000);
					}
				}
				else if (pKey->GetTrigger(DIK_S) == true || pKey->GetTrigger(DIK_DOWN) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, 0) == true)
				{// キーボード（S）が押された、Xパッド（Lスティック下）が押された
					m_nSelectNum = (m_nSelectNum + 1) % SELECT_MAX;

					if (pSound != NULL)
					{// NULL以外の場合
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SERECT000);
					}
				}
			}
		}

		D3DXVECTOR3 posPlayer;
		D3DXVECTOR3 posCenter;
		D3DXVECTOR3 sizeCenter;
		float fRange;

		if (m_pPlayer != NULL && m_pReturnCircle != NULL)
		{
			posPlayer = m_pPlayer->Getpos();
			posCenter = m_pReturnCircle->Getpos();
			sizeCenter = m_pReturnCircle->Getsize();
			fRange = sqrtf(powf(posPlayer.x - posCenter.x, 2.0f) + powf(posPlayer.z - posCenter.z, 2.0f));

			// アイコン
			if (m_pIcon != NULL)
			{// NULL以外の場合
				D3DXCOLOR col = m_pIcon->Getcol();
				if (fRange < sizeCenter.x * 0.8f)
				{// 範囲内
					if (col.a != 1.0f)
					{// 不透明以外以外
						col.a += 0.05f;
						if (1.0f < col.a) col.a = 1.0f;
					}
				}
				else
				{// 範囲外
					if (col.a != 0.0f)
					{// 透明以外
						col.a -= 0.05f;
						if (col.a < 0.0f) col.a = 0.0f;
					}
				}
				m_pIcon->Setcol(col);
			}

			// 戻るサークル
			if (m_pReturnCircle != NULL)
			{// NULL以外の場合
				D3DXVECTOR3 size = m_pReturnCircle->Getsize();
				if (size.y != 100.0f)
				{
					size.y += 8.0f;
					if (100.0f < size.y) size.y = 100.0f;
					m_pReturnCircle->SetStart(m_pReturnCircle->Getpos(), m_pReturnCircle->Getrot(), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
				}
			}
		}

		// 壁
		for (int nCount = 0; nCount < m_MAX_WALL; nCount++)
		{// 壁カウント
			if (m_pWall[nCount] != NULL)
			{// NULL以外の場合
				D3DXVECTOR3 size = m_pWall[nCount]->Getsize();
				if (size.y != 100.0f)
				{
					size.y += 8.0f;
					if (100.0f < size.y) size.y = 100.0f;
					m_pWall[nCount]->SetStart(m_pWall[nCount]->Getpos(), m_pWall[nCount]->Getrot(), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
				}
			}
		}

		if (fade == CFade::FADE_NONE)
		{// フェードしていない
			if (bUiEdit == false)
			{// Ui編集中でない
				if (pKey->GetTrigger(DIK_RETURN) == true || pKey->GetTrigger(DIK_E) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
				{// キーボード（ENTER）が押された、Xパッド（B、4）が押された
					if (pSound != NULL)
					{// MILLI
						if (m_nSelectNum != SELECT_TUTORIAL)
						{// チュートリアル以外
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
						}
					}

					switch (m_nSelectNum)
					{// 選択番号
					case SELECT_GAME:		// ゲーム
						CFade::Create(CManager::MODE_GAME);		// フェード生成（ゲームモードへ）

						break;
					case SELECT_TUTORIAL:	// チュートリアル
					{
						bool bTutorial = m_bTutorial ? false : true;

						if (bTutorial == true)
						{// チュートリアル中
							if (pSound != NULL)
							{// NULL以外の場合
								pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
							}

							m_bTutorial = m_bTutorial ? false : true;
							m_bSelectSet = m_bSelectSet ? false : true;

							m_pPlayer->SetMove(true);
							pCamera->SetMove(true);
							pCamera->Setrot(D3DXVECTOR3(-0.3f, -D3DX_PI * 0.5f, 0.0f));
							pCamera->SetDistance(0, 230.0f);
							pCamera->SetDistance(1, 230.0f);

							if (m_pIcon == NULL)
							{// NULLの場合
								m_pIcon = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_MAPICON);
								m_pIcon->SetInitAll(D3DXVECTOR3(180.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(0.125f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
								m_pIcon->Init();
								m_pIcon->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
								m_pIcon->DrawEdit(false, false, false, true, true, 0, D3DCULL_NONE);
								m_pIcon->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
							}
							if (m_pReturnCircle == NULL)
							{// NULLの場合
								m_pReturnCircle = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_WARP);
								m_pReturnCircle->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), D3DXVECTOR3(180.0f, 0.0f, 0.0f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
								m_pReturnCircle->Init();
								m_pReturnCircle->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
								m_pReturnCircle->DrawEdit(true, false, false, true, true, 0, D3DCULL_NONE);
							}
							/*for (int nCount = 0; nCount < m_MAX_WALL; nCount++)
							{// 壁カウント
								if (m_pWall[nCount] == NULL)
								{// NULLの場合
									D3DXVECTOR3 pos, rot, size;
									if (nCount == 0)
									{// ０だったら
										pos = D3DXVECTOR3(160.0f - 10.0f, 0.0f, 0.0f);
										rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
										size = D3DXVECTOR3(180.0f, 0.0f, 0.0f);
									}
									else if (nCount == 1)
									{// １だったら
										pos = D3DXVECTOR3((690.0f - 160.0f) / 2.0f + 160.0f + 5.0f, 0.0f, -160.0f - 20.0f);
										rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										size = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
									}
									else if (nCount == 2)
									{// ２だったら
										pos = D3DXVECTOR3(690.0f + 20.0f, 0.0f, 0.0f);
										rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
										size = D3DXVECTOR3(180.0f, 0.0f, 0.0f);
									}
									else if (nCount == 3)
									{// ３だったら
										pos = D3DXVECTOR3((690.0f - 160.0f) / 2.0f + 160.0f + 5.0f, 0.0f, 160.0f + 20.0f);
										rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										size = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
									}

									m_pWall[nCount] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_WARP);
									m_pWall[nCount]->SetInitAll(CSceneMesh::MESHTYPE_WALL, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(1.0f, 1.0f, 0.0f), pos,
										rot, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
									m_pWall[nCount]->Init();
									m_pWall[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
									m_pWall[nCount]->DrawEdit(true, false, false, true, true, 0, D3DCULL_NONE);
								}
							}*/

							if (m_pUi_Game == NULL)
							{// NULLの場合
								m_pUi_Game = CUi::Create(CUi::UI_GAME);
							}

							CManager::GetRenderer()->SetDrawMap(true);
						}
						else
						{// メニュー表示
							if (m_pPlayer != NULL && m_pReturnCircle != NULL)
							{// NULL以外の場合
								if (fRange < sizeCenter.x * 0.8f)
								{// 範囲内
									if (pSound != NULL)
									{// NULL以外の場合
										pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
									}

									if (m_pWindow == NULL)
									{// NULLの場合
										m_pWindow = CWindow::Create(CWindow::WINDOW_YESNO);

										m_pPlayer->SetMove(false);
										pCamera->SetMove(false);
									}
									else
									{// NULL以外の場合
										CWindow_YESNO *pWindow_YESNO = (CWindow_YESNO*)m_pWindow;
										if (pWindow_YESNO->GetSelectNum() == 0)
										{// 敵いなくなったら
											m_bTutorial = m_bTutorial ? false : true;
											m_bSelectSet = m_bSelectSet ? false : true;

											m_pPlayer->SetMove(false);
											pCamera->SetMove(false);
											pCamera->SetPosDest(pCamera->GetInitPos(0), pCamera->GetInitPos(1));
											pCamera->Setrot(D3DXVECTOR3(-0.1f, D3DX_PI, 0.0f));
											pCamera->SetDistance(0, 150.0f);
											pCamera->SetDistance(1, 150.0f);

											// アイコン
											if (m_pIcon != NULL)
											{// NULL以外の場合
												m_pIcon->Uninit();		// 終了処理
												m_pIcon = NULL;			// NULLへ
											}

											// 戻るサークル
											if (m_pReturnCircle != NULL)
											{// NULL以外の場合
												m_pReturnCircle->Uninit();	// 終了処理
												m_pReturnCircle = NULL;	  	// NULLへ
											}

											// 壁
											for (int nCount = 0; nCount < m_MAX_WALL; nCount++)
											{// 壁カウント
												if (m_pWall[nCount] != NULL)
												{// NULL以外の場合
													m_pWall[nCount]->Uninit();	// 終了処理
													m_pWall[nCount] = NULL;	  	// NULLへ
												}
											}

											if (m_pUi_Game != NULL)
											{// NULL以外の場合
												m_pUi_Game->Uninit();	// 終了処理
												m_pUi_Game = NULL;	 	// NULLへ
											}

											CManager::GetRenderer()->SetDrawMap(false);
										}
										else
										{// 敵がいる
											m_pPlayer->SetMove(true);	// 移動できる
											pCamera->SetMove(true);		// カメラ動かせる
										}
										m_pWindow->Uninit();	// 終了処理
										m_pWindow = NULL;		// NULLへ
									}
								}
							}
						}						
					}
						break;
					case SELECT_EDIT:		// エディット
						m_bRanking = m_bRanking ? false : true;
						m_bSelectSet = m_bSelectSet ? false : true;

						if (m_bRanking == true)
						{// ランキング
							if (m_pRankingBG == NULL)
							{// NULLの場合
								m_pRankingBG = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
								m_pRankingBG->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.4f, 0.0f), 
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f), D3DXVECTOR2(1.0f, 1.0f));
								m_pRankingBG->Init();
							}
							if (m_pRanking == NULL)
							{// NULLの場合
								m_pRanking = CRanking::Create(CRanking::TYPE_2D, D3DXVECTOR3(SCREEN_WIDTH - 80.0f, 180.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
									D3DXVECTOR3(30.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f));
							}
						}
						else
						{
							if (m_pRankingBG != NULL)
							{// NULL以外の場合
								m_pRankingBG->Uninit();		// 終了処理
								m_pRankingBG = NULL;		// NULLへ
							}
							if (m_pRanking != NULL)
							{// NULL以外の場合
								m_pRanking->Uninit();		// 終了処理
								m_pRanking = NULL;			// NULLへ
							}
						}

						break;
					case SELECT_TITLE:		// タイトル
						m_bTitle = true;		// タイトル
						m_bSelectSet = false;	// 選択設定

						break;
					}
				}
			}
		}
	}
}

//=============================================================================
// 描画処理											(public)	*** CTitle ***
//=============================================================================
void CTitle::Draw(void)
{
	
}