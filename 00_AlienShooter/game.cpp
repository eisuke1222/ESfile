//=========================================================================================================================
//
// ゲーム処理 [game.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include <time.h>		// time
#include "game.h"		// ゲーム
#include "player.h"		// プレイヤー
#include "enemy.h"		// 敵
#include "bullet.h"		// 弾
#include "explosion.h"	// 爆発
#include "effect.h"		// エフェクト
#include "item.h"		// アイテム
#include "bg.h"			// 背景
#include "pause.h"		// ポーズ
#include "fade.h"		// フェード
#include "sound.h"		// サウンド
#include "input.h"		// 入力処理

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void SetStage(void);	// ステージ設定

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
GAMESTATE		g_GameState;							// ゲーム画面
int				g_nCounterGameState;					// ゲーム状態カウンタ
STAGENUM		g_StageState;							// ステージ状態
bool			g_bPause;								// ポーズ
int				g_nCounterGame;							// ゲームカウンタ
int				g_nCntSetEnemy;							// 敵セットカウンタ
int				g_nCntSetStage;							// ステージセットカウンタ

//=========================================================================================================================
// ゲーム初期化処理
//=========================================================================================================================
void InitGame(void)
{
	// 状態設定
	g_GameState = GAMESTATE_NORMAL;		// ゲーム状態
	g_nCounterGameState = 0;			// カウンタ初期化
	g_StageState = STAGENUM_1;			// ステージ状態
	g_bPause = false;					// ポーズ初期設定
	g_nCounterGame = 0;					// カウンタ初期化
	g_nCntSetEnemy = 0;					// カウンタ初期化
	g_nCntSetStage = 0;					// カウンタ初期化

	// 背景初期化処理
	InitBG();
	SetNumStage(g_StageState);

	// 敵の初期化処理
	InitEnemy();
	//SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 0, 20, 4, ENEMYTYPE_NORMALENEMY,
	//	EnemyBullet{ ENEMYBULLET_NO_6, 3.0f, 30 },
	//	EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

	// プレイヤー初期化処理
	InitPlayer();

	// アイテム初期化処理
	InitItem();

	// 爆発の初期化処理
	InitExplosion();

	// エフェクトの初期化処理
	InitEffect();

	// 弾の初期化処理
	InitBullet();

	// ポーズの初期化処理
	InitPause();
}

//=========================================================================================================================
// ゲーム終了処理
//=========================================================================================================================
void UninitGame(void)
{
	// 背景の終了処理
	UninitBG();

	// 敵の終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	// アイテム終了処理
	UninitItem();

	// 爆発の終了処理
	UninitExplosion();

	// エフェクトの終了処理
	UninitEffect();

	// 弾の終了処理
	UninitBullet();

	// ポーズの終了処理
	UninitPause();
}

//=========================================================================================================================
// ゲーム更新処理
//=========================================================================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true && g_GameState == GAMESTATE_NORMAL)
	{// ポーズを切替
		g_bPause = g_bPause ? false : true;
		PlaySound(SOUND_LABEL_SE_PAUSE);
	}

	if (g_bPause == false)
	{// ポーズを使用していない場合
		// 背景の更新処理
		UpdateBG();

		// 敵の更新処理
		UpdateEnemy();

		// プレイヤーの更新処理
		UpdatePlayer();

		// アイテム更新処理
		UpdateItem();

		// 爆発の更新処理
		UpdateExplosion();

		// エフェクトの更新処理
		UpdateEffect();

		// 弾の更新処理
		UpdateBullet();

		// ステージ設定
		SetStage();

		//ポーズ初期状態へ
		SetPause();
	}

	if (g_bPause == true)
	{// ポーズが使用されている場合
		// ポーズの更新処理
		UpdatePause();
	}

	switch (g_GameState)
	{
	case GAMESTATE_NORMAL:		// ノーマル
		break;
	case GAMESTATE_GAMECLEAR:	// ゲームクリア
	case GAMESTATE_GAMEOVER:	// ゲームオーバー
		g_nCounterGameState++;	// カウンターを加算

		if (g_nCounterGameState >= 60)
		{// 一定カウント後
			// g_GameState = GAMESTATE_NONE;
			SetFade(MODE_RESULT);		// リザルト
		}
		break;
	}
}

//=========================================================================================================================
// ゲーム描画処理
//=========================================================================================================================
void DrawGame(void)
{
	// 背景の描画処理
	DrawBG();

	// 敵の描画処理
	DrawEnemy();

	// プレイヤーの描画
	DrawPlayer();

	// アイテム描画処理
	DrawItem();

	// 爆発の描画処理
	DrawExplosion();

	// エフェクトの描画処理
	DrawEffect();

	// 弾の描画処理
	DrawBullet();

	if (g_bPause == true && g_GameState == GAMESTATE_NORMAL)
	{// ポーズが使用されている場合
		// ポーズの描画処理
		DrawPause();
	}
}

//=========================================================================================================================
// ゲーム設定処理
//=========================================================================================================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;			// 状態を記憶
	g_nCounterGameState = 0;		// カウントを0
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
// ゲーム設定処理
//=========================================================================================================================
void SetStageState(STAGENUM stage)
{
	g_StageState = stage;			// 状態を記憶
}

//==================================================================================================
// ステージ設定
//==================================================================================================
void SetStage(void)
{
	Enemy *pEnemy = GetEnemy();		// 敵を取得
	int nNumEnemy = GetNumEnemy();	// 敵の総数を取得

	g_nCounterGame++;				// ゲームをカウント

	// SetEnemy : 初期位置、目標位置、移動速度、atan速度、移動種類、移動開始、目標位置カウント、テクスチャタイプ、半径、体力、敵種類
	//			  弾１、弾２、弾３、弾４、弾５ (弾種類、弾速度、発射タイミング）

	if (g_StageState == STAGENUM_1)
	{// ステージ１
		if (g_nCounterGame == 240)
		{// 4秒
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 300.0f) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 1, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 300.0f) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 30, 0, 1, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 200.0f) * 1.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 40, 0, 0, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -200.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 200.0f) * 1.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 50, 0, 0, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 660)
		{// 13秒
			SetEnemy(D3DXVECTOR3(0, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 400.0f) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 3, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(0, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 200.0f) * 1.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 1, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_3, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 0, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 200.0f) * 1.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 1, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_3, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 400.0f) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 3, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 1260)
		{// 21秒
			SetEnemy(D3DXVECTOR3(0, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 10) * 1.0f, (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 2, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 1.0f, -50.0f, 0.0f), D3DXVECTOR3(10.0f, (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 2, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame >= 1560 && g_nCounterGame <= 2160)
		{// 31秒
			if (g_nCounterGame % 60 == 0)
			{
				if (g_nCntSetEnemy < 10)
				{
					SetEnemy(D3DXVECTOR3(116.0f * (g_nCntSetEnemy + 1), -50.0f, 0.0f), D3DXVECTOR3(116.0f * (g_nCntSetEnemy + 1), (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 20, 300, 2, 20, 3, ENEMYTYPE_NORMALENEMY,
						EnemyBullet{ ENEMYBULLET_NO_3, 3.0f, 15 },
						EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
					g_nCntSetEnemy++;
				}
			}
		}
		else if (g_nCounterGame > 2160 && nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_NORMALENEMY)
		{// ENEMYTYPE_NORMALENEMYがいなくなった場合
			StopSound();							// サウンドを止める
			PlaySound(SOUND_LABEL_BGM003);			// ボスサウンド

			// ボスをセット
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING, 0, 0, 0, 80, 100, ENEMYTYPE_LBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame > 2460 && pEnemy->Type == ENEMYTYPE_LBOSSENEMY && pEnemy->bUse == false)
		{// ボスがいなくなった場合
			g_nCntSetStage++;

			if (g_nCntSetStage > 180)
			{
				g_StageState = STAGENUM_2;		// ステージ2に移行
				SetNumStage(g_StageState);		// ステージ番号表示

				g_nCntSetStage = 0;
				g_nCntSetEnemy = 0;
				g_nCounterGame = 0;

				StopSound();							// サウンドを止める
				PlaySound(SOUND_LABEL_BGM002);			// サウンド
			}
		}
	}
	else if (g_StageState == STAGENUM_2)
	{// ステージ２
		if (g_nCounterGame == 240)
		{// 4秒
			SetEnemy(D3DXVECTOR3(-30, 150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 30) * 1.0f, (SCREEN_HEIGHT * 0.6) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 20, 0, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, 50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 30) * 1.0f, (SCREEN_HEIGHT * 0.6 - 100) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 30, 0, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 30) * 1.0f, (SCREEN_HEIGHT * 0.6 - 200) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 40, 0, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 30) * 1.0f, (SCREEN_HEIGHT * 0.6 - 300) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 50, 0, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, 150.0f, 0.0f), D3DXVECTOR3(-30, (SCREEN_HEIGHT * 0.6) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 20, 1, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, 50.0f, 0.0f), D3DXVECTOR3(-30, (SCREEN_HEIGHT * 0.6 - 100) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 30, 1, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, -50.0f, 0.0f), D3DXVECTOR3(-30, (SCREEN_HEIGHT * 0.6 - 200) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 40, 0, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, -150.0f, 0.0f), D3DXVECTOR3(-30, (SCREEN_HEIGHT * 0.6 - 300) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 50, 0, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 600)
		{// 10秒
			SetEnemy(D3DXVECTOR3(-30, 150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 30) * 1.0f, (SCREEN_HEIGHT * 0.6) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, 50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 50) * 1.0f, (SCREEN_HEIGHT * 0.6 - 100) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 30, 600, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 130) * 1.0f, (SCREEN_HEIGHT * 0.6 - 200) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 40, 600, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 210) * 1.0f, (SCREEN_HEIGHT * 0.6 - 300) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 50, 600, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, 150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 30), (SCREEN_HEIGHT * 0.6) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, 50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 50), (SCREEN_HEIGHT * 0.6 - 100) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 30, 600, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 130), (SCREEN_HEIGHT * 0.6 - 200) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 40, 600, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 210), (SCREEN_HEIGHT * 0.6 - 300) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 50, 600, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 1260)
		{// 21秒
			SetEnemy(D3DXVECTOR3(0, -50.0f, 0.0f), D3DXVECTOR3(75, 100, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, -50.0f, 0.0f), D3DXVECTOR3(25, 100, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 40 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(25, 150, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 75) * 1.0f, 100, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 25) * 1.0f, 100, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 40 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 0.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 25) * 1.0f, 150, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame >= 1860 && g_nCounterGame <= 2160)
		{// 31秒
			if (g_nCounterGame % 60 == 0)
			{
				if (g_nCntSetEnemy < 5)
				{
					SetEnemy(D3DXVECTOR3(-50.0f, -30.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 70.0f * (g_nCntSetEnemy + 1), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 4.0f, ENEMYMOVE_HOMING_KEEP, 0, 0, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
						EnemyBullet{ ENEMYBULLET_NO_4,3.0f, 20 },
						EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
					SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, -30.0f, 0.0f), D3DXVECTOR3(-50.0f, 70.0f * (g_nCntSetEnemy + 1), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 4.0f, ENEMYMOVE_HOMING_KEEP, 0, 0, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
						EnemyBullet{ ENEMYBULLET_NO_4,3.0f, 20 },
						EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
					g_nCntSetEnemy++;
				}
			}
		}
		else if (nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_NORMALENEMY && g_nCounterGame > 2160)
		{// ENEMYTYPE_NORMALENEMYがいなくなった場合
			StopSound();							// サウンドを止める
			PlaySound(SOUND_LABEL_BGM003);			// ボスサウンド

			// ボスをセット
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 0, 50, 100, ENEMYTYPE_MBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 45 },
				EnemyBullet{ ENEMYBULLET_NO_4, 4.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_MBOSSENEMY && g_nCounterGame > 2460)
		{// ENEMYTYPE_NORMALENEMYがいなくなった場合
			// ボスをセット
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 1, 80, 150, ENEMYTYPE_LBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_5, 3.0f, 5 },
				EnemyBullet{ ENEMYBULLET_NO_3, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame > 2460 && pEnemy->Type == ENEMYTYPE_LBOSSENEMY && pEnemy->bUse == false)
		{// ボスがいなくなった場合
			g_nCntSetStage++;


			if (g_nCntSetStage > 180)
			{
				g_StageState = STAGENUM_3;		// ステージ2に移行
				SetNumStage(g_StageState);		// ステージ番号表示

				g_nCntSetStage = 0;
				g_nCntSetEnemy = 0;
				g_nCounterGame = 0;

				StopSound();							// サウンドを止める
				PlaySound(SOUND_LABEL_BGM002);			// サウンド
			}
		}
	}
	else if (g_StageState == STAGENUM_3)
	{// ステージ３
		if (g_nCounterGame == 240)
		{// 4秒
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 4 - 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 0, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 4 - 100.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 100.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 35 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 4 - 150.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 150.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 40 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 4 - 200.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 200.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 3, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 + 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 0, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 + 100.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 100.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 35 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 + 150.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 150.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 45 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 + 200.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 200.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 3, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 300)
		{// 5秒
			SetEnemy(D3DXVECTOR3(-50.0f, 0, 0.0f), D3DXVECTOR3(20.0f, 200, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, 50.0f, 0.0f), D3DXVECTOR3(20.0f, 250, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 35 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, 100.0f, 0.0f), D3DXVECTOR3(20.0f, 300, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 45 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, 150.0f, 0.0f), D3DXVECTOR3(20.0f, 350, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 0, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 20.0f) * 1.0f, 200, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 20.0f) * 1.0f, 250, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 35 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 20.0f) * 1.0f, 300, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 45 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 20.0f) * 1.0f, 350, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 600)
		{// 10秒
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, (SCREEN_HEIGHT / 2 - 100.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_14, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, (SCREEN_HEIGHT / 2 - 50.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_9, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_13, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, (SCREEN_HEIGHT / 2 - 100.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_10, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, (SCREEN_HEIGHT / 2 - 50.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 4, 20, 15, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, (SCREEN_HEIGHT / 2 - 100.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_12, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, (SCREEN_HEIGHT / 2 - 50.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_8, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_11, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 1380)
		{// 23秒
			SetEnemy(D3DXVECTOR3(-50.0f, -50.0f, 0.0f), D3DXVECTOR3(50.0f, 75.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 0, 600, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 50.0f) * 1.0f, 75.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 0, 600, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, (SCREEN_HEIGHT + 50.0f) * 1.0f, 0.0f), D3DXVECTOR3(50.0f, (SCREEN_HEIGHT - 50.f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 0, 600, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, (SCREEN_HEIGHT + 50.0f) * 1.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 50.0f) * 1.0f, (SCREEN_HEIGHT - 50.f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 0, 600, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 2100)
		{// 35秒
			SetEnemy(D3DXVECTOR3((426.0f) * 1.0f, (SCREEN_HEIGHT + 50.0f) * 1.0f, 0.0f), D3DXVECTOR3((426.0f) * 1.0f, (SCREEN_HEIGHT - 50.f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 300, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_10, 5.0f, 5 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((426.0f * 2) * 1.0f, (SCREEN_HEIGHT + 50.0f) * 1.0f, 0.0f), D3DXVECTOR3((426.0f * 2) * 1.0f, (SCREEN_HEIGHT - 50.f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 300, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_10, 5.0f, 5 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 2340)
		{// 39秒
			srand((unsigned int)time(0));

			int nSet = rand() % 3;
			float pos;

			if (nSet == 0) { pos = -25.0; }
			else if (nSet == 1) { pos = 410.0f; }
			else if (nSet == 2) { pos = 815.0f; }

			SetEnemy(D3DXVECTOR3(pos + 80.0f, -50.0f, 0.0f), D3DXVECTOR3(pos + 80.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 160.0f, -50.0f, 0.0f), D3DXVECTOR3(pos + 160.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 240.0f, -50.0f, 0.0f), D3DXVECTOR3(pos + 240.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 320.0f, -50.0f, 0.0f), D3DXVECTOR3(pos + 320.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 400.0f, -50.0f, 0.0f), D3DXVECTOR3(pos + 400.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3(pos + 80.0f, -100.0f, 0.0f), D3DXVECTOR3(pos + 80.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 160.0f, -100.0f, 0.0f), D3DXVECTOR3(pos + 160.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 240.0f, -100.0f, 0.0f), D3DXVECTOR3(pos + 240.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 320.0f, -100.0f, 0.0f), D3DXVECTOR3(pos + 320.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 400.0f, -100.0f, 0.0f), D3DXVECTOR3(pos + 400.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3(pos + 80.0f, -150.0f, 0.0f), D3DXVECTOR3(pos + 80.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 160.0f, -150.0f, 0.0f), D3DXVECTOR3(pos + 160.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 240.0f, -150.0f, 0.0f), D3DXVECTOR3(pos + 240.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 320.0f, -150.0f, 0.0f), D3DXVECTOR3(pos + 320.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 400.0f, -150.0f, 0.0f), D3DXVECTOR3(pos + 400.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });


			SetEnemy(D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-100.0f, 80.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-150.0f, 80.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-200.0f, 80.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-250.0f, 80.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 100.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 150.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 200.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 250.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_NORMALENEMY && g_nCounterGame > 2340)
		{// ENEMYTYPE_NORMALENEMYがいなくなった場合
			StopSound();							// サウンドを止める
			PlaySound(SOUND_LABEL_BGM003);			// ボスサウンド

			// ボスをセット
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 0, 30, 100, ENEMYTYPE_SBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_6, 3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NO_7, 3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NO_4, 6.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_SBOSSENEMY && g_nCounterGame > 2520)
		{// ENEMYTYPE_SBOSSENEMYがいなくなった場合
		 // ボスをセット
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 - SCREEN_WIDTH / 8) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - SCREEN_WIDTH / 8) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 0, 50, 120, ENEMYTYPE_MBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 18 },
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 8) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 1, 50, 120, ENEMYTYPE_MBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_5, 3.0f, 23 },
				EnemyBullet{ ENEMYBULLET_NO_3, 3.0f, 23 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_MBOSSENEMY && g_nCounterGame > 2700)
		{// ENEMYTYPE_NORMALENEMYがいなくなった場合
		 // ボスをセット
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 4, 100, 250, ENEMYTYPE_LBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_8, 3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NO_9, 3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame > 2460 && pEnemy->Type == ENEMYTYPE_LBOSSENEMY && pEnemy->bUse == false)
		{// ボスがいなくなった場合
			g_nCntSetStage++;

			if (g_nCntSetStage > 120)
			{
				g_GameState = GAMESTATE_GAMECLEAR;
			}
		}
	}

	// アイテム
	if (pEnemy->Type == ENEMYTYPE_SBOSSENEMY && pEnemy->bUse == true)
	{
		if (rand() % 3 == 0)
		{
			if (rand() % 1000 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_POWER); }
			if (rand() % 1000 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_LIFE); }
			if (rand() % 1000 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_STAR); }
		}
	}
	else if (pEnemy->Type == ENEMYTYPE_MBOSSENEMY && pEnemy->bUse == true)
	{
		if (rand() % 1200 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_POWER); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_POWER); } }
		if (rand() % 1200 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_LIFE); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_LIFE); } }
		if (rand() % 1200 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_STAR); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_STAR); } }
	}
	else if (pEnemy->Type == ENEMYTYPE_LBOSSENEMY && pEnemy->bUse == true)
	{
		if (rand() % 1500 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_POWER); SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_LIFE); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_LIFE); } }
		if (rand() % 1500 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_LIFE); SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_LIFE); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_LIFE); } }
		if (rand() % 1500 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_STAR); SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_STAR); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_STAR); } }
	}
}