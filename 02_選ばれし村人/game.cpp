//=========================================================================================================================
//
// ゲーム処理 [game.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include <time.h>				// time
#include "game.h"				// ゲーム
#include "camera.h"				// カメラ
#include "light.h"				// ライト
#include "meshfield.h"			// メッシュフィールド
#include "meshcylinder.h"		// メッシュシリンダー
#include "meshwall.h"			// メッシュウォール
#include "billboardobject.h"	// ビルボードオブジェクト
#include "billboardeffect.h"	// ビルボードエフェクト
#include "shadow.h"				// 影
#include "xobject.h"			// オブジェクト
#include "xenemy.h"				// 敵
#include "xplayer.h"			// プレイヤー
#include "xweapon.h"			// 武器
#include "score.h"				// スコア
#include "pause.h"				// ポーズ
#include "sound.h"				// サウンド
#include "fade.h"				// フェード
#include "input.h"				// 入力
#include "title.h"				// タイトル
#include "font.h"				// フォント
#include "window.h"				// ウィンドウ

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
bool			g_bPause;			// ポーズ
int				g_nCntGameTimer;	// ゲームタイマー
GAMESTATE		g_GameState;		// ゲームステート
bool			g_bTutorialStep;	// チュートリアルスキップ
int				g_PlayerRespawnNum;	// リスポーン地点
int				g_nCntMap;			// マップ
int				g_nCntTutorialSkip;	// チュートリアルスキップ
bool			g_GameTutorial;		// チュートリアル

WEAPONTYPE		g_XPlayerUseWeapon;	// プレイヤー武器
int				g_XPlayerHP;		// プレイヤー体力
int				g_Weapon_1Lv, g_Weapon_2Lv, g_Weapon_3Lv;	// 武器
int				g_LeftItem, g_CenterItem, g_RightItem;		// アイテム

//=========================================================================================================================
// プロトタイプ宣言
//=========================================================================================================================
void SetStage(void);

//=========================================================================================================================
// ゲームの初期化処理
//=========================================================================================================================
void InitGame(void)
{
	// 初期化
	g_bPause = false;
	g_nCntGameTimer = 0;
	g_GameState = GAMESTATE_NONE;
	g_bTutorialStep = false;
	g_PlayerRespawnNum = 0;
	g_nCntMap = 0;
	g_nCntTutorialSkip = 0;
	g_GameTutorial = false;

	// 初期設定
	g_XPlayerUseWeapon = WEAPONTYPE_NONE;
	g_XPlayerHP = 0;
	g_Weapon_1Lv = 0;
	g_Weapon_2Lv = 0;
	g_Weapon_3Lv = 0;
	g_LeftItem = 0;
	g_CenterItem = 0;
	g_RightItem = 0;

	StopSound(SOUND_LABEL_BGM_TITLE);
	SetTitleSound(false);

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// フィールドの初期化処理
	InitMeshField();

	// 壁の初期化処理
	InitMeshWall();

	// シリンダーの初期化処理
	InitMeshCylinder();

	// ビルボードオブジェクトの初期化処理
	InitBillboardObject();
	
	// ビルボードエフェクトの初期化処理
	InitBillboardEffect();

	// 影の初期化処理
	InitShadow();

	// オブジェクトの初期化処理
	InitXObject();

	// 敵の初期化処理
	InitXEnemy();

	// プレイヤーの初期化処理
	InitXPlayer();

	// 武器の初期化処理
	InitXWeapon();

	// スコアの初期化処理
	InitScore();

	// ポーズ初期化処理
	InitPause();

	// ゲーム状態変更
	SetGameState(GAMESTATE_TUTORIAL);
	//SetGameState(GAMESTATE_PLAY);
}

//=========================================================================================================================
// ゲームの終了処理
//=========================================================================================================================
void UninitGame(void)
{
	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// フィールドの終了処理
	UninitMeshField();

	// 壁の終了処理
	UninitMeshWall();

	// シリンダーの終了処理
	UninitMeshCylinder();

	// ビルボードオブジェクトの終了処理
	UninitBillboardObject();

	// ビルボードエフェクトの終了処理
	UninitBillboardEffect();

	// 影の終了処理
	UninitShadow();

	// オブジェクトの終了処理
	UninitXObject();

	// 敵の終了処理
	UninitXEnemy();

	// プレイヤーの終了処理
	UninitXPlayer();

	// 武器の終了処理
	UninitXWeapon();

	// ポーズ終了処理
	UninitPause();

	// ウィンドウ破棄処理
	DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_GAME);
	DeleteWindow(WINDOWTEXTURE_SELECT_2);
	DeleteWindow(WINDOWTEXTURE_MESSAGE);
	DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_IN);
	DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_OUT);
	DeleteWindow(WINDOWTEXTURE_GAME_MAP);
	DeleteWindow(WINDOWTEXTURE_GAME_MAP_POS);
	DeleteWindow(WINDOWTEXTURE_GAME_HPBG);
	DeleteWindow(WINDOWTEXTURE_GAME_HP_M);
	DeleteWindow(WINDOWTEXTURE_GAME_HP);
	DeleteWindow(WINDOWTEXTURE_GAME_WEAPON_1);
	DeleteWindow(WINDOWTEXTURE_GAME_WEAPON_2);
	DeleteWindow(WINDOWTEXTURE_GAME_WEAPON_3);
	DeleteWindow(WINDOWTEXTURE_GAME_WEAPON_99);

	// フォント破棄
	DeleteFont();

	StopSound();
}

//=========================================================================================================================
// ゲームの更新処理
//=========================================================================================================================
void UpdateGame(void)
{
	FADE fade = GetFade();

	if (fade == FADE_NONE)
	{
		if ((GetKeyboardTrigger(DIK_P) == true || GetButtonTrigger(9) == true) && (g_GameState == GAMESTATE_TUTORIAL || g_GameState == GAMESTATE_PLAY))
		{// ポーズ
			PlaySound(SOUND_LABEL_SE_PAUSE_0);

			if (g_bPause == false)
			{
				SetStickState(true);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_GAME_TUTORIAL_IN, SETTEXTURE_TOPFRONT);
			}
			else
			{
				DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_P);
				DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_IN);
				DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_OUT);
			}

			g_bPause = g_bPause ? false : true;
		}
	}

	if (g_bPause == false)
	{// ポーズをしていない
		switch (g_GameState)
		{// ゲーム状態
		case GAMESTATE_TUTORIAL:

			if ((GetKeyboardPress(DIK_RETURN) == true || GetKeyboardPress(DIK_Z) == true ||
				GetButtonPress(2) == true))
			{// チュートリアルスキップ
				g_nCntTutorialSkip++;

				if (g_nCntTutorialSkip > 150)
				{// ゲーム本編へ
					SetFadeGame(GAMESTATE_PLAY);
				}
			}
			else
			{
				g_nCntTutorialSkip = 0;
			}

			if ((GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
				GetButtonTrigger(2) == true) && g_bTutorialStep == true)
			{// ゲーム本編へ
				SetFadeGame(GAMESTATE_PLAY);
			}

		case GAMESTATE_PLAY:

			if (g_nCntMap == 1 && (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
				GetButtonTrigger(2) == true) && g_GameTutorial == false)
			{// ゲーム目標表示
				DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_GAME);
				DeleteWindow(WINDOWTEXTURE_SELECT_2);
				SetMove(true);

				g_GameTutorial = true;
			}

			// カメラの更新処理
			UpdateCamera();

			// ライトの更新処理
			UpdateLight();

			// フィールドの更新処理
			UpdateMeshField();

			// 壁の更新処理
			UpdateMeshWall();

			// シリンダーの更新処理
			UpdateMeshCylinder();

			// ビルボードオブジェクトの更新処理
			UpdateBillboardObject();

			// ビルボードエフェクトの更新処理
			UpdateBillboardEffect();

			// 影の更新処理
			UpdateShadow();

			// オブジェクトの更新処理
			UpdateXObject();

			// 敵の更新処理
			UpdateXEnemy();

			// プレイヤーの更新処理
			UpdateXPlayer();

			// 武器の更新処理
			UpdateXWeapon();

			// スコアの更新処理
			UpdateScore();

			// ポーズ状態設定
			SetPauseState();

			break;
		case GAMESTATE_GAMECLEAR:
		case GAMESTATE_GAMEOVER:

			if (g_nCntGameTimer == 60)
			{// １秒後終了
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH + SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_RESULT_BG, SETTEXTURE_BG);

				if (g_GameState == GAMESTATE_GAMECLEAR)
				{
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2 * 10, SCREEN_HEIGHT / 2 * 10, WINDOWTEXTURE_RESULT_CLEAR, SETTEXTURE_BG);
				}
				else
				{
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2 * 10, SCREEN_HEIGHT / 2 * 10, WINDOWTEXTURE_RESULT_OVER, SETTEXTURE_BG);
				}
			}

			g_nCntGameTimer++;

			if (g_nCntGameTimer >= 240)
			{// リザルトへ
				SetMode(MODE_RESULT);
			}

			break;
		}
	}
	else if (g_bPause == true)
	{// ポーズが使用されている場合
		// ポーズの更新処理
		UpdatePause();
	}
}

//=========================================================================================================================
// ゲームの描画処理
//=========================================================================================================================
void DrawGame(void)
{
	// カメラの設定
	SetUpCamera();

	// フィールドの描画処理
	DrawMeshField();

	// シリンダーの描画処理
	DrawMeshCylinder();

	// オブジェクトの描画処理
	DrawXObject();

	// 壁の描画処理
	DrawMeshWall();

	// ビルボードオブジェクトの描画処理
	DrawBillboardObject();

	// 影の描画処理
	DrawShadow();

	// 敵の描画処理
	DrawXEnemy();

	// プレイヤーの描画処理
	DrawXPlayer();

	// ビルボードエフェクトの描画処理
	DrawBillboardEffect();

	// 武器の描画処理
	DrawXWeapon();

	// スコアの描画処理
	DrawScore();
}

//=========================================================================================================================
// ゲーム設定処理
//=========================================================================================================================
void SetGameState(GAMESTATE state)
{
	if (g_GameState == GAMESTATE_TUTORIAL)
	{// ゲームメインに移行する準備
		StopSound(SOUND_LABEL_BGM_GAME_0);
	}

	if (g_GameState == GAMESTATE_PLAY && (state == GAMESTATE_GAMECLEAR || state == GAMESTATE_GAMEOVER))
	{// クリア、オーバーに移行する準備
		StopSound(SOUND_LABEL_BGM_GAME_1);
		g_nCntGameTimer = 0;
	}

	if (g_GameState != state)
	{// 状態設定
		g_GameState = state;			// 状態を記憶
	}

	if (g_GameState == GAMESTATE_TUTORIAL || g_GameState == GAMESTATE_PLAY)
	{// ステージ生成
		SetStage();
	}

	SetPauseState(false);
}

//=========================================================================================================================
// ゲーム取得処理
//=========================================================================================================================
GAMESTATE GetGameState(void)
{
	return g_GameState;				// 値を返す
}

//=========================================================================================================================
// ポーズ設定処理
//=========================================================================================================================
void SetPauseState(bool bPause)
{
	g_bPause = bPause;				// 状態を記憶
}

//=========================================================================================================================
// ポーズ取得処理
//=========================================================================================================================
bool GetPauseState(void)
{
	return g_bPause;				// 値を返す
}

//=========================================================================================================================
// チュートリアルステップ処理
//=========================================================================================================================
void SetTutorialStep(bool bTutorial)
{
	g_bTutorialStep = bTutorial;
}

//=========================================================================================================================
// プレイヤー出現位置設定処理
//=========================================================================================================================
void SetPlayerRespawn(int nRespawn)
{
	g_PlayerRespawnNum = nRespawn;	
}

//=========================================================================================================================
// プレイマップリセット処理
//=========================================================================================================================
void SetMapReset(int ResetMap)
{
	g_nCntMap = ResetMap;
}

//=========================================================================================================================
// プレイマップ番号取得処理
//=========================================================================================================================
int GetNowMap(void)
{
	return g_nCntMap;
}

//=========================================================================================================================
// ステージ設定処理
//=========================================================================================================================
void SetStage(void)
{
	srand((unsigned int)time(0));

	XPLAYER *pXPlayer = GetXPlayer();

	// 現在の状態を読み込み
	g_XPlayerUseWeapon = GetXWeapon();
	g_XPlayerHP = GetXPlayerHP();
	g_Weapon_1Lv = pXPlayer->nWeapon_1Lv;
	g_Weapon_2Lv = pXPlayer->nWeapon_2Lv;
	g_Weapon_3Lv = pXPlayer->nWeapon_3Lv;

	/*g_Weapon_1Lv = 15;
	g_Weapon_2Lv = 15;
	g_Weapon_3Lv = 15;*/

	// 初期化
	SetCancelRockOn();			// ロックオン
	DeleteMeshField();			// メッシュフィールド
	DeleteMeshCylinder();		// メッシュシリンダー
	DeleteMeshWall();			// メッシュウォール
	DeleteBillboardObject();	// ビルボードオブジェクト
	DeleteBillboardEffect();	// ビルボードエフェクト
	DeleteShadow();				// 影
	DeleteXObject();			// オブジェクト
	DeleteXEnemy();				// 敵
	DeleteXPlayer();			// プレイヤー
	DeleteXWeapon();			// 武器
	DeleteWindow();				// 背景
	DeleteFont();				// フォント

	switch (g_GameState)
	{
	case GAMESTATE_TUTORIAL:	// チュートリアル

		g_bTutorialStep = false;

		// カメラ
		SetCamera(D3DXVECTOR3(0.0f, 180.0f, -350.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), CAMERATYPE_GAME);

		// メッシュフィールド
		SetMeshField(9, 4);
		SetMeshField(10, 4);

		// メッシュウォール
		//SetMeshWall(1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		//SetMeshWall(1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		//SetMeshWall(1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		//SetMeshWall(1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		// 敵
		SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, -1000.0f));
		SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(300.0f, 0.0f,-400.0f));
		SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-300.0f, 0.0f, -400.0f));

		// プレイヤー
		SetXPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// 背景
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_MESSAGE, SETTEXTURE_FRONT);

		// フォント
		SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 600, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 640, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 675, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		SetMessage(true);

		PlaySound(SOUND_LABEL_BGM_GAME_0);

		break;
	case GAMESTATE_PLAY:	// ゲーム

		g_nCntMap++;
		//g_nCntMap = 5;

		// カメラ
		SetCamera(D3DXVECTOR3(0.0f, 180.0f, -350.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), CAMERATYPE_GAME);

		// マップ
		if (g_nCntMap >= 5)
		{
			g_nCntMap = 5;

			// メッシュフィールド
			SetMeshField(0, 2);
			SetMeshField(1, 0);
			SetMeshField(2, 3);
			SetMeshField(3, 0);
			SetMeshField(4, 0);
			SetMeshField(5, 0);
			SetMeshField(6, 0);
			SetMeshField(7, 0);
			SetMeshField(8, 0);

			// オブジェクト
			SetXObject(TEXTTYPE_TREE_9);
			SetXObject(TEXTTYPE_TREE_10);
			SetXObject(TEXTTYPE_TREE_11);
			SetXObject(TEXTTYPE_TREE_12);
			SetXObject(TEXTTYPE_TREE_13);
			SetXObject(TEXTTYPE_TREE_14);

			// プレイヤー
			SetXPlayer(D3DXVECTOR3(0.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f));
		}
		else
		{
			// メッシュフィールド
			SetMeshField(0, 1);
			SetMeshField(1, 0);
			SetMeshField(2, 1);
			SetMeshField(3, 0);
			SetMeshField(4, 0);
			SetMeshField(5, 0);
			SetMeshField(6, 0);
			SetMeshField(7, 1);
			SetMeshField(8, 0);

			// オブジェクト
			SetXObject(TEXTTYPE_TREE_0);
			SetXObject(TEXTTYPE_TREE_1);
			SetXObject(TEXTTYPE_TREE_2);
			SetXObject(TEXTTYPE_TREE_3);
			SetXObject(TEXTTYPE_TREE_4);
			SetXObject(TEXTTYPE_TREE_5);
			SetXObject(TEXTTYPE_TREE_6);
			SetXObject(TEXTTYPE_TREE_7);
			SetXObject(TEXTTYPE_TREE_8);

			// ビルボードオブジェクト
			SetBillboardObject(D3DXVECTOR3(-1000.0f, 0.0f, 1650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXVECTOR3(300.0f, 50.0f, 0.0f), BILLBOARDTEXTURE_MAP_WALL);
			SetBillboardObject(D3DXVECTOR3(0.0f, 0.0f, 1650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXVECTOR3(300.0f, 50.0f, 0.0f), BILLBOARDTEXTURE_MAP_WALL);
			SetBillboardObject(D3DXVECTOR3(1000.0f, 0.0f, 1650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXVECTOR3(300.0f, 50.0f, 0.0f), BILLBOARDTEXTURE_MAP_WALL);

			// マップ移動地点
			if (g_PlayerRespawnNum == 0)
			{
				SetXObject(TEXTTYPE_MARUTA_2);
				SetXObject(TEXTTYPE_MARUTA_3);
				SetXPlayer(D3DXVECTOR3(0.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f));
			}
			else if (g_PlayerRespawnNum == 1)
			{
				SetXObject(TEXTTYPE_MARUTA_1);
				SetXObject(TEXTTYPE_MARUTA_3);
				SetXPlayer(D3DXVECTOR3(-1000.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f));
			}
			else if (g_PlayerRespawnNum == 2)
			{
				SetXObject(TEXTTYPE_MARUTA_1);
				SetXObject(TEXTTYPE_MARUTA_2);
				SetXPlayer(D3DXVECTOR3(1000.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f));
			}			
		}	

		// ウォール
		SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		//SetMeshWall(1, D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f));

		// メッシュシリンダー
		SetMeshCylinder(0);
		SetMeshCylinder(1);

		// 敵
		if (g_nCntMap == 1)
		{
			SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(150.0f, 0.0f, 400.0f));
			SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-150.0f, 0.0f, 400.0f));
		}
		else if (g_nCntMap == 2)
		{
			SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(150.0f, 0.0f, 200.0f));
			SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-150.0f, 0.0f, 200.0f));

			SetXEnemy(rand() % 3, D3DXVECTOR3(75.0f, 0.0f, 100.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(-75.0f, 0.0f, 100.0f));
		}
		else if (g_nCntMap == 3)
		{
			SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(150.0f, 0.0f, 400.0f));
			SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-150.0f, 0.0f, 400.0f));

			SetXEnemy(rand() % 3, D3DXVECTOR3(-500.0f, 0.0f, -500.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(0.0f, 0.0f, -500.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(500.0f, 0.0f, -500.0f));
		}
		else if (g_nCntMap == 4)
		{
			SetXEnemy(rand() % 3, D3DXVECTOR3(-1000.0f, 0.0f, -500.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(0.0f, 0.0f, -500.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(1000.0f, 0.0f, -500.0f));

			SetXEnemy(rand() % 3, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, 5.0f));
			SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(150.0f, 0.0f, 600.0f));
			SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-150.0f, 0.0f, 600.0f));
		}
		else if (g_nCntMap == 5)
		{
			SetXEnemy(rand() % 3, D3DXVECTOR3(-300.0f, 0.0f, 350.0f));
			SetXEnemy(ENEMYTYPE_BOSS, D3DXVECTOR3(0.0f, 0.0f, 300.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(300.0f, 0.0f, 350.0f));
		}

		// スコア
		SetScore(D3DXVECTOR3(SCREEN_WIDTH - 50, 50, 0.0f), D3DXVECTOR3(25, 35, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_SCORE);

		SetWindow(D3DXVECTOR3(50.0f, 120.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 40, 110, WINDOWTEXTURE_GAME_MAP, SETTEXTURE_FRONT);

		SetWindow(D3DXVECTOR3(50.0f, 250.0f - (40.0f * g_nCntMap), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 30, WINDOWTEXTURE_GAME_MAP_POS, SETTEXTURE_FRONT);

		SetWindow(D3DXVECTOR3(360, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 350, 50, WINDOWTEXTURE_GAME_HPBG, SETTEXTURE_FRONT);
		SetWindow(D3DXVECTOR3(70, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 55, 50, WINDOWTEXTURE_GAME_HP_M, SETTEXTURE_FRONT);
		SetWindow(D3DXVECTOR3(410, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(0.3f, 1.0f, 0.1f, 1.0f), 295, 45, WINDOWTEXTURE_GAME_HP, SETTEXTURE_FRONT);

		SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50, 50, WINDOWTEXTURE_GAME_WEAPON_1, SETTEXTURE_FRONT);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 350, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50, 50, WINDOWTEXTURE_GAME_WEAPON_2, SETTEXTURE_FRONT);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50, 50, WINDOWTEXTURE_GAME_WEAPON_3, SETTEXTURE_FRONT);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 58, 58, WINDOWTEXTURE_GAME_WEAPON_99, SETTEXTURE_FRONT);

		// クリア目標表示・非表示
		if (g_nCntMap == 1)
		{
			PlaySound(SOUND_LABEL_BGM_GAME_1);
			SetXWeapon(WEAPONTYPE_SWORD);

			g_LeftItem = rand() % 3;
			g_CenterItem = rand() % 3;
			g_RightItem = rand() % 3;

			if (g_GameTutorial == false)
			{
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_GAME_TUTORIAL_GAME, SETTEXTURE_FRONT);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 80, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_SELECT_2, SETTEXTURE_FRONT);
			}
			else
			{
				SetMove(true);
			}
		}
		else
		{
			SetXWeapon(g_XPlayerUseWeapon);
			SetWeaponLv(g_Weapon_1Lv, g_Weapon_2Lv, g_Weapon_3Lv);
			SetXPlayerHP(g_XPlayerHP);

			SetMove(true);

			if (g_nCntMap != 5)
			{
				// アイテム
				if (g_LeftItem == 0)
				{
					SetBillboardObject(D3DXVECTOR3(-1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				}
				else if (g_LeftItem == 1)
				{
					SetBillboardObject(D3DXVECTOR3(-1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
				}
				else if (g_LeftItem == 2)
				{
					SetBillboardObject(D3DXVECTOR3(-1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
					SetBillboardObject(D3DXVECTOR3(-1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
				}

				if (g_CenterItem == 0)
				{
					SetBillboardObject(D3DXVECTOR3(-160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(-80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(0, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				}
				else if (g_CenterItem == 1)
				{
					SetBillboardObject(D3DXVECTOR3(-160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(0, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
				}
				else if (g_CenterItem == 2)
				{
					SetBillboardObject(D3DXVECTOR3(-160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
					SetBillboardObject(D3DXVECTOR3(-80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(0, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
				}

				if (g_RightItem == 0)
				{
					SetBillboardObject(D3DXVECTOR3(1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				}
				else if (g_RightItem == 1)
				{
					SetBillboardObject(D3DXVECTOR3(1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
				}
				else if (g_RightItem == 2)
				{
					SetBillboardObject(D3DXVECTOR3(1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
					SetBillboardObject(D3DXVECTOR3(1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
				}
			}
			else
			{
				SetBillboardObject(D3DXVECTOR3(-80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(0, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);

				SetBillboardObject(D3DXVECTOR3(-800, 40, 1000),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
				SetBillboardObject(D3DXVECTOR3(-400, 40, 1000),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
				SetBillboardObject(D3DXVECTOR3(0, 40, 1000),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
				SetBillboardObject(D3DXVECTOR3(400, 40, 1000),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
				SetBillboardObject(D3DXVECTOR3(800, 40, 1000),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
			}
		}

		break;
	}	
}