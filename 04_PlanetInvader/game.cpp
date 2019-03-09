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
#include "scene2d.h"		// シーン2D
#include "scene3d.h"		// シーン2D
#include "fade.h"			// フェード
#include "camera.h"			// カメラ
#include "meshfield.h"		// メッシュフィールド
#include "object3d.h"		// オブジェクト3D
#include "effect3d.h"		// エフェクト3D
#include "bg.h"				// 背景
#include "enemy.h"			// 敵
#include "player.h"			// プレイヤー
#include "bullet.h"			// 弾
#include "effect.h"			// エフェクト
#include "explosion.h"		// 爆発
#include "cloud.h"			// 雲
#include "number.h"			// 数字
#include "bomb.h"			// 爆弾
#include "gauge.h"			// ゲージ
#include "life.h"			// 体力
#include "score.h"			// スコア
#include "ranking.h"		// ランキング
#include "pause.h"			// ポーズ

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAME_TEXT_MAP		"data\\TEXT\\map.txt"
#define GAME_TEXT_ENEMY		"data\\TEXT\\enemy.txt"
#define TEXTURE_NAME		"data\\TEXTURE\\playerNum.png"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CGame::m_pTexture				= NULL;		// テクスチャ情報へのポインタ
CScene2D			*CGame::m_pScene2D[MAX_PLAYER]	= {};		// シーン2D
CRanking			*CGame::m_pRanking			= NULL;			// ランキング
CPause				*CGame::m_pPause			= NULL;			// ポーズ
bool				CGame::m_bPause				= false;		// ポーズ使用
CGame::GAME			CGame::m_game				= GAME_NONE;	// ゲーム
int					CGame::m_nCntGame			= 0;			// ゲームカウンタ

LPDIRECT3DTEXTURE9	CGame::m_pENDTexture[2] = {};
CScene3D			*CGame::m_pScene3D = NULL;

CGame::MAP			CGame::m_mapState[MAX_MAP] = {};			// マップ情報
CGame::ENEMY		CGame::m_enemyState[MAX_MAP]= {};			// 敵情報
int					CGame::m_mapMax				= 0;			// マップ最大数
int					CGame::m_enemyMax			= 0;			// マップ最大数

//=============================================================================
// コンストラクタ									(public)	*** CGame ***
//=============================================================================
CGame::CGame()
{
	// 初期設定
	m_bPause	= false;		// ポーズ使用
	m_game		= GAME_NONE;	// ゲーム
	m_nCntGame	= 0;			// ゲームカウンタ
	m_mapNum	= 0;			// マップ番号
	m_enemyNum	= 0;			// マップ番号
	m_mapfield	= 0;			// フィールド番号
}

//=============================================================================
// デストラクタ										(public)	*** CGame ***
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ロード処理							(public)	*** CBullet ***
//=============================================================================
HRESULT CGame::Load(void)
{
	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{// カウント
		m_mapState[nCntMap].nNumModel = 0;							// モデル番号
		m_mapState[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		m_mapState[nCntMap].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	}

	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{// カウント
		m_enemyState[nCntMap].nNumEnemy = 0;							// モデル番号
		m_enemyState[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		m_enemyState[nCntMap].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		m_enemyState[nCntMap].nShotTiming = 0;							// 向き
	}

	m_mapMax = 0;				// マップ最大数
	m_enemyMax = 0;

	FILE *pFile;				// ファイルポインタ
	char cMessage[255] = {};	// 文字チェック

	{
		// ファイル設定
		pFile = fopen(GAME_TEXT_MAP, "r");

		// プレイヤーテキストデータ読み込み
		if (pFile != NULL)
		{// ファイルが開けた場合
			fscanf(pFile, "%s", &cMessage[0]);		// 読み込み

			if (strcmp(cMessage, "SCRIPT") == 0)
			{// 読み込み開始
				do
				{// 終了警告までループ
					fscanf(pFile, "%s", &cMessage[0]);		// 読み込み

					if (cMessage[0] == 35)
					{// #で始まっている場合
						fgets(cMessage, 255, pFile);		// コメントとして一行読み取り
					}
					else
					{// その他
						if (strcmp(cMessage, "MAP_SET") == 0)
						{// マップ設定開始
							do
							{// 終了警告までループ
								fscanf(pFile, "%s", &cMessage[0]);		// 読み込み

								if (cMessage[0] == 35)
								{// #で始まっている場合
									fgets(cMessage, 255, pFile);		// コメントとして一行読み取り
								}
								else
								{// その他
									if (strcmp(cMessage, "MODEL_SET") == 0)
									{// モデル設定開始
										do
										{// 終了警告までループ
											fscanf(pFile, "%s", &cMessage[0]);		// 読み込み

											if (cMessage[0] == 35)
											{// #で始まっている場合
												fgets(cMessage, 255, pFile);		// コメントとして一行読み取り
											}
											else
											{// その他
												if (strcmp(cMessage, "MODEL_NUM") == 0)
												{// モデル番号
													fscanf(pFile, "%s %d", &cMessage[0], &m_mapState[m_mapMax].nNumModel);
												}
												else if (strcmp(cMessage, "POS") == 0)
												{// 位置
													fscanf(pFile, "%s %f %f %f", &cMessage[0], &m_mapState[m_mapMax].pos.x, &m_mapState[m_mapMax].pos.y, &m_mapState[m_mapMax].pos.z);
												}
												else if (strcmp(cMessage, "ROT") == 0)
												{// 向き
													fscanf(pFile, "%s %f %f %f", &cMessage[0], &m_mapState[m_mapMax].rot.x, &m_mapState[m_mapMax].rot.y, &m_mapState[m_mapMax].rot.z);
												}
											}
										} while (strcmp(cMessage, "END_MODEL_SET") != 0);		// モデル設定終了
										m_mapMax++;		// モデル最大数を増やす
									}
								}
							} while (strcmp(cMessage, "END_MAP_SET") != 0);			// マップ設定終了
						}
					}
				} while (strcmp(cMessage, "END_SCRIPT") != 0);		// 読み込み終了
			}

			fclose(pFile);	// ファイルを閉じる
		}
		else
		{// ファイルが開けない場合
			MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
		}

		int nMin;	// ソート対象
		MAP map;	// マップ

		for (int nCntMap = 1; nCntMap < m_mapMax; nCntMap++)
		{// カウント
			nMin = nCntMap;		// 番号を記憶

			while (nMin > 0 && m_mapState[nMin - 1].pos.z > m_mapState[nMin].pos.z)
			{// 値が小さかったら
				 // マップをソート
				map = m_mapState[nMin - 1];
				m_mapState[nMin - 1] = m_mapState[nMin];
				m_mapState[nMin] = map;

				nMin--;			// ソート対象を移動

				// ソートをメッセージで表示
				wsprintf(cMessage, "%d と %d を入れ替えました", nMin, nCntMap);
				MessageBox(0, cMessage, "マップデータをソート", MB_OK);
			}
		}
	}

	{
		// ファイル設定
		pFile = fopen(GAME_TEXT_ENEMY, "r");

		// プレイヤーテキストデータ読み込み
		if (pFile != NULL)
		{// ファイルが開けた場合
			fscanf(pFile, "%s", &cMessage[0]);		// 読み込み

			if (strcmp(cMessage, "SCRIPT") == 0)
			{// 読み込み開始
				do
				{// 終了警告までループ
					fscanf(pFile, "%s", &cMessage[0]);		// 読み込み

					if (cMessage[0] == 35)
					{// #で始まっている場合
						fgets(cMessage, 255, pFile);		// コメントとして一行読み取り
					}
					else
					{// その他
						if (strcmp(cMessage, "ENEMY_SET") == 0)
						{// マップ設定開始
							do
							{// 終了警告までループ
								fscanf(pFile, "%s", &cMessage[0]);		// 読み込み

								if (cMessage[0] == 35)
								{// #で始まっている場合
									fgets(cMessage, 255, pFile);		// コメントとして一行読み取り
								}
								else
								{// その他
									if (strcmp(cMessage, "MODEL_SET") == 0)
									{// モデル設定開始
										do
										{// 終了警告までループ
											fscanf(pFile, "%s", &cMessage[0]);		// 読み込み

											if (cMessage[0] == 35)
											{// #で始まっている場合
												fgets(cMessage, 255, pFile);		// コメントとして一行読み取り
											}
											else
											{// その他
												if (strcmp(cMessage, "ENEMY_NUM") == 0)
												{// モデル番号
													fscanf(pFile, "%s %d", &cMessage[0], &m_enemyState[m_enemyMax].nNumEnemy);
												}
												else if (strcmp(cMessage, "POS") == 0)
												{// 位置
													fscanf(pFile, "%s %f %f %f", &cMessage[0], &m_enemyState[m_enemyMax].pos.x, &m_enemyState[m_enemyMax].pos.y, &m_enemyState[m_enemyMax].pos.z);
												}
												else if (strcmp(cMessage, "MOVE") == 0)
												{// 向き
													fscanf(pFile, "%s %f %f %f", &cMessage[0], &m_enemyState[m_enemyMax].move.x, &m_enemyState[m_enemyMax].move.y, &m_enemyState[m_enemyMax].move.z);
												}
												else if (strcmp(cMessage, "SHOT") == 0)
												{// 向き
													fscanf(pFile, "%s %d", &cMessage[0], &m_enemyState[m_enemyMax].nShotTiming);
												}
											}
										} while (strcmp(cMessage, "END_MODEL_SET") != 0);		// モデル設定終了
										m_enemyMax++;		// モデル最大数を増やす
									}
								}
							} while (strcmp(cMessage, "END_ENEMY_SET") != 0);			// マップ設定終了
						}
					}
				} while (strcmp(cMessage, "END_SCRIPT") != 0);		// 読み込み終了
			}

			fclose(pFile);	// ファイルを閉じる
		}
		else
		{// ファイルが開けない場合
			MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
		}

		int nMin;	// ソート対象
		ENEMY enemy;	// 敵

		for (int nCntMap = 1; nCntMap < m_enemyMax; nCntMap++)
		{// カウント
			nMin = nCntMap;		// 番号を記憶

			while (nMin > 0 && m_enemyState[nMin - 1].pos.z > m_enemyState[nMin].pos.z)
			{// 値が小さかったら
			 // マップをソート
				enemy = m_enemyState[nMin - 1];
				m_enemyState[nMin - 1] = m_enemyState[nMin];
				m_enemyState[nMin] = enemy;

				nMin--;			// ソート対象を移動

				// ソートをメッセージで表示
				wsprintf(cMessage, "%d と %d を入れ替えました", nMin, nCntMap);
				MessageBox(0, cMessage, "マップデータをソート", MB_OK);
			}
		}
	}

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイス取得

	if (m_pTexture == NULL)
	{// NULLの場合
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	if (m_pENDTexture[0] == NULL)
	{// NULLの場合
	 // テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\GAMECLEAR.png", &m_pENDTexture[0]);
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\GAMEOVER.png", &m_pENDTexture[1]);
	}

	return S_OK;
}

//=============================================================================
// アンロード処理								(public)	*** CBullet ***
//=============================================================================
void CGame::Unload(void)
{
	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{// カウント
		m_mapState[nCntMap].nNumModel = 0;							// モデル番号
		m_mapState[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
		m_mapState[nCntMap].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	}

	m_mapMax = 0;		// モデル最大数

	if (m_pTexture != NULL)
	{// NULL以外の場合
		m_pTexture->Release();			// 解放
		m_pTexture = NULL;				// NULLへ
	}
}

//=============================================================================
// 初期化処理										(public)	*** CGame ***
//=============================================================================
HRESULT CGame::Init(void)
{
	CCamera *pCamera = CManager::GetCamera();		// カメラ取得
	pCamera->SetPos(D3DXVECTOR3(0.0f, 750.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// カメラ初期設定

#ifdef _DEBUG
	CManager::SetPlayNum(CInputXPad::GetInputNum());
#endif // !1

	// 3Dロード
	CMeshField::Load();		// メッシュフィールド
	CObject3D::Load();		// モデル
	CEffect3D::Load();		// モデル
	CEnemy::Load();			// 敵
	CPlayer::Load();		// プレイヤー
	CBullet::Load();		// 弾
	CEffect::Load();		// エフェクト
	CExplosion::Load();		// 爆発
	CCloud::Load();			// 雲
	// 3Dクリエイト
	CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
		D3DXVECTOR3(0.0f, -100.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);		// メッシュフィールド
	CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
		D3DXVECTOR3(0.0f, -100.0f, 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);	// メッシュフィールド

	// 2Dロード
	CBg::Load();			// 背景
	CLife::Load();			// 体力
	CBomb::Load();			// 爆弾
	CGauge::Load();			// ゲージ
	CNumber::Load();		// 数字
	CRanking::Load();		// ランキング
	// 2Dクリエイト
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, CBg::TEXTURE_GAME);	// 背景

	for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
	{
		CLife::Create(D3DXVECTOR3(50.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 180, 0.0f), D3DXVECTOR3(LIFE_SIZE_X, LIFE_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);					// 体力
		CBomb::Create(3, D3DXVECTOR3(50.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 260, 0.0f), D3DXVECTOR3(BOMB_SIZE_X, BOMB_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);				// 爆弾
		CGauge::Create(D3DXVECTOR3(100.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 340, 0.0f), D3DXVECTOR3(GAUGE_SIZE_X, GAUGE_SIZE_Y, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.1f, 1.0f), nCntCont);				// ゲージ
		CScore::Create(D3DXVECTOR3(SCREEN_WIDTH_0 - 50.0f + ((SCREEN_WIDTH - 300.0f) * nCntCont), 100.0f, 0.0f), D3DXVECTOR3(12.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);			// スコア
	}

	CRanking::LoadRanking();			// ランキングロード
	//CRanking::Create(D3DXVECTOR3(0.0f, 500.0f, -300.0f), D3DXVECTOR3(0.89f, 0.0f, 0.0f), D3DXVECTOR3(18.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -20.0f), 1);

	CPause::Load();			// ポーズ

	for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
	{
		if (m_pScene2D[nCnt] == NULL)
		{
			m_pScene2D[nCnt] = new CScene2D(CScene::PRIORITY_UI);
			m_pScene2D[nCnt]->SetInitAll(D3DXVECTOR3(0.0f + 60.0f + ((SCREEN_WIDTH - 60.0f - 60.0f) * nCnt), SCREEN_HEIGHT - 40.0f, 0.0f), D3DXVECTOR3(35.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
			m_pScene2D[nCnt]->Init();
			m_pScene2D[nCnt]->SetObjType(CScene::OBJTYPE_PLAYERNUM);
			m_pScene2D[nCnt]->BindTexture(m_pTexture);

			m_pScene2D[nCnt]->SetTex(1, nCnt, D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	// 初期設定
	m_pPause	= NULL;			// ポーズ
	m_bPause	= false;		// ポーズ使用
	m_pRanking	= NULL;			// ランキング
	m_game		= GAME_PLAY;	// ゲーム
	m_nCntGame	= 0;			// ゲームカウンタ

	m_pScene3D = NULL;

	m_mapNum	= 0;			// マップ番号
	m_mapfield	= 0;			// フィールド番号
	
	return S_OK;
}

//=============================================================================
// 終了処理											(public)	*** CGame ***
//=============================================================================
void CGame::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pScene2D[nCnt] != NULL)
		{
			m_pScene2D[nCnt]->Uninit();		// 終了処理
			m_pScene2D[nCnt] = NULL;		// NULLへ
		}
	}

	if (m_pPause != NULL)
	{// NULL以外の場合
		m_pPause->Uninit();		// 終了処理
		m_pPause = NULL;		// NULL
	}

	if (m_pRanking != NULL)
	{
		m_pRanking->Uninit();	// 終了処理
		m_pRanking = NULL;		// NULL
	}

	if (m_pScene3D != NULL)
	{
		m_pScene3D->Uninit();
		m_pScene3D = NULL;
	}

	// 3Dアンロード
	CMeshField::Unload();	// メッシュフィールド
	CObject3D::Unload();	// モデル
	CEffect3D::Unload();	// モデル
	CEnemy::Unload();		// 敵
	CPlayer::Unload();		// プレイヤー
	CBullet::Unload();		// 弾
	CEffect::Unload();		// エフェクト
	CExplosion::Unload();	// 爆発

	// 2Dアンロード
	CBg::Unload();			// 背景
	CLife::Unload();		// 体力
	CBomb::Unload();		// 爆弾
	CGauge::Unload();		// ゲージ
	CNumber::Unload();		// 数字
	CRanking::Unload();		// ランキング

	CPause::Unload();		// ポーズ

	CScene *pScene;			// シーンポインタ

	for (int nCntPriority = 0; nCntPriority <= CScene::PRIORITY_PAUSE; nCntPriority++)
	{// プライオリティカウント
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// シーンカウント
			pScene = CScene::GetScene(nCntPriority, nCntScene);		// シーン読み込み

			if (pScene != NULL)
			{// NULL以外の場合
				pScene->Uninit();		// 終了処理
				pScene = NULL;			// NULLへ
			}
		}
	}
}

//=============================================================================
// 更新処理											(public)	*** CGame ***
//=============================================================================
void CGame::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// 入力デバイス
	CInputXPad *pInputXPad= CManager::GetInputXPad();				// 入力デバイス
	CSound *pSound = CManager::GetSound();							// サウンド
	CFade::FADE fade = CFade::GetFade();							// フェード状態取得
	D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();		// カメラ注視点取得

	if (m_bPause == false)
	{
		SetMap();		// マップ設定
	}

	switch (m_game)
	{// ゲーム
	case GAME_PLAY:		// プレイ
		//if (pInputKeyboard->GetTrigger(DIK_P) == true && fade == CFade::FADE_NONE)
		for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
		{
			if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_START, nCnt) == true && fade == CFade::FADE_NONE)
			{// 任意のキー（P）を押した場合
				m_bPause = m_bPause ? false : true;		// 表示or非表示

				switch (m_bPause)
				{// ポーズし使用
				case true:
					if (m_pPause == NULL)
					{// 生成と更新制御
						m_pPause = CPause::Create();					// ポーズクリエイト

						CScene::SetStartUp(CScene::PRIORITY_PAUSE);		// 更新範囲をポーズから
					}

					break;
				case false:
					if (m_pPause != NULL)
					{// NULL以外の場合
						m_pPause->Uninit();		// 終了処理
						m_pPause = NULL;		// NULL

						CScene::SetStartUp(0);	// 更新範囲を初期化
					}

					break;
				}

				pSound->PlaySoundA(CSound::SOUND_LABEL_SE000);
			}
		}

		break;

	case GAME_CLEAR:	// クリア
	case GAME_OVER:		// オーバー
		if (m_nCntGame == 0)
		{
			CRanking::SaveRanking();		// ランキングセーブ

			if (m_pScene3D == NULL)
			{
				m_pScene3D = new CScene3D(CScene::PRIORITY_UI);
				m_pScene3D->SetInitAll(D3DXVECTOR3(0.0f, 550.0f, -300.0f + CameraposR.z), D3DXVECTOR3(0.89f, 0.0f, 0.0f), D3DXVECTOR3(80.0f, 40.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
				m_pScene3D->Init();

				if (m_game == GAME_CLEAR)
				{
					m_pScene3D->BindTexture(m_pENDTexture[0]);
				}
				else
				{
					m_pScene3D->BindTexture(m_pENDTexture[1]);
				}				
			}
		}

		m_nCntGame++;						// ゲームカウンタ

		if (m_pScene3D != NULL)
		{
			m_pScene3D->Setpos(D3DXVECTOR3(m_pScene3D->Getpos().x, m_pScene3D->Getpos().y, m_pScene3D->Getpos().z + 3.0f));
		}

		if (m_nCntGame == 180)
		{
			m_pRanking = CRanking::Create(D3DXVECTOR3(0.0f, 470.0f, -300.0f + CameraposR.z), D3DXVECTOR3(0.89f, 0.0f, 0.0f), D3DXVECTOR3(18.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -20.0f), 1);
		}
		else if (m_nCntGame == 300)
		{
			CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
		}

		break;
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true && m_bPause == false)
	{
		CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
	}
}

//=============================================================================
// 描画処理											(public)	*** CGame ***
//=============================================================================
void CGame::Draw(void)
{
	
}

//=============================================================================
// ポーズ設定処理									(public)	*** CGame ***
//=============================================================================
void CGame::SetPause(bool bPause)
{
	m_bPause = bPause;		// ポーズ

	switch (CGame::m_bPause)
	{// ポーズ
	case true:
		if (CGame::m_pPause == NULL)
		{// NULLの場合
			m_pPause = CPause::Create();					// ポーズクリエイト

			CScene::SetStartUp(CScene::PRIORITY_PAUSE);		// 更新範囲をポーズから
		}

		break;
	case false:
		if (m_pPause != NULL)
		{// NULL以外の場合
			m_pPause->Uninit();		// 終了処理
			m_pPause = NULL;		// NULL

			CScene::SetStartUp(0);	// 更新範囲を初期化
		}

		break;
	}
}

//=============================================================================
// マップ設定処理									(public)	*** CGame ***
//=============================================================================
void CGame::SetMap(void)
{
	CSound *pSound = CManager::GetSound();							// サウンド
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposV();		// カメラ視点取得

	if ((int)(Camerapos.z - 1.0f) % 1200 == 0)
	{// カメラが一定間隔進んだら
		if ((7200.0f < Camerapos.z && Camerapos.z < 12000.0f)
			)
		{// 地面
			CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
				D3DXVECTOR3(0.0f, 0.0f, 2600 + (m_mapfield * 1200.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), 
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_G, CMesh3D::MESHTEX_NUMSIZE);				// メッシュフィールド

			if (rand() % 3 == 0)
			{
				CObject3D::Create(D3DXVECTOR3(-150.0f + (150.0f * (rand() % 3)), 0.0f, Camerapos.z + 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject3D::OBJECTTYPE_LIFE);		// モデル
			}
		}
		else
		{// 海
			CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
				D3DXVECTOR3(0.0f, -100.0f, 2600 + (m_mapfield * 1200.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), 
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);				// メッシュフィールド

			if (rand() % 3 == 0)
			{
				CObject3D::Create(D3DXVECTOR3(-150.0f + (150.0f * (rand() % 3)), 0.0f, Camerapos.z + 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject3D::OBJECTTYPE_LIFE);		// モデル
			}

			if ((int)(Camerapos.z - 1.0f) == 8000)
			{
				CObject3D::Create(D3DXVECTOR3(-150.0f + (150.0f * (rand() % 3)), 0.0f, Camerapos.z + 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject3D::OBJECTTYPE_LIFE);		// モデル
			}
		}
		m_mapfield++;		// フィールドカウント
	}

	if ((int)(Camerapos.z - 1.0f) % 300 == 0)
	{// カメラが一定間隔進んだら
		float size = (float)((rand() % 50) * 0.01f) + 1.0f;		// 大きさランダム

		CCloud::Create(D3DXVECTOR3(-150.0f + (float)(rand() % 300), 400.0f, Camerapos.z + 600.0f + (float)(rand() % 100)), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f), 
			D3DXVECTOR3(50.0f * size, 30.0f * size, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f - ((rand() % 2) * 2.0f), 1.0f - ((rand() % 2) * 2.0f)), CScene3D::SCENE3DTYPE_NORMAL);			// 雲
	}

	while (1)
	{// ループ
		if (m_mapNum < m_mapMax)
		{
			if (m_mapState[m_mapNum].pos.z <= Camerapos.z + 1400.0f)
			{// カメラが近くなったら
				CObject3D::Create(m_mapState[m_mapNum].pos, m_mapState[m_mapNum].rot, (CObject3D::OBJECTTYPE)m_mapState[m_mapNum].nNumModel);		// モデル
				m_mapNum++;		// マップカウント
			}
			else
			{// 該当なし
				break;			// 処理を抜ける
			}
		}
		else
		{// 該当なし
			break;			// 処理を抜ける
		}
	}

	while (1)
	{// ループ
		if (m_enemyNum < m_enemyMax)
		{
			if (m_enemyState[m_enemyNum].pos.z <= Camerapos.z + 1400.0f)
			{// カメラが近くなったら
				CEnemy::Create(m_enemyState[m_enemyNum].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_enemyState[m_enemyNum].move, (CEnemy::ENEMYTYPE)m_enemyState[m_enemyNum].nNumEnemy, m_enemyState[m_enemyNum].nShotTiming);
				m_enemyNum++;		// マップカウント

				if (m_enemyNum == m_enemyMax)
				{
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE006);
				}
			}
			else
			{// 該当なし
				break;			// 処理を抜ける
			}
		}
		else
		{// 該当なし
			break;			// 処理を抜ける
		}
	}
}