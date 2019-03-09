//=========================================================================================================================
//
// ゲーム処理 [game.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "game.h"		// ゲーム
#include "input.h"		// 入力処理
#include "map.h"		// マップ
#include "player.h"		// プレイヤー
#include "enemy.h"		// 敵
#include "bullet.h"		// 弾
#include "explosion.h"	// 爆発
#include "block.h"		// ブロック
#include "item.h"		// アイテム
#include "score.h"		// スコア
#include "pause.h"		// ポーズ
#include "fade.h"		// フェード
#include "bgwindow.h"	// 背景
#include "sound.h"		// サウンド

//*************************************************************************************************************************
//	構造体定義
//*************************************************************************************************************************
#define MAX_STAGE	(3)

//*************************************************************************************************************************
//	構造体定義
//*************************************************************************************************************************
void SetStage(void);	// ステージ設定

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
GAMESTATE		g_GameState;							// ゲーム画面
GAMESTAGE		g_GameStageOpen;						// 開放しているゲームステージ番号
int				g_nCounterGameState;					// ゲーム状態カウンタ
bool			g_bPause;								// ポーズ
int				g_nCounterGame;							// ゲームカウンタ
int				g_nCntSetStage;							// ステージセットカウンタ
bool			g_aStageClear[MAX_STAGE];				// ステージクリア情報

//=========================================================================================================================
// ゲーム初期化処理
//=========================================================================================================================
void InitGame(void)
{
	// 状態設定
	g_GameState = GAMESTATE_MAP;		// ゲーム状態
	g_GameStageOpen = GAMESTAGE_NO1;	// ゲームステージ
	g_nCounterGameState = 0;			// カウンタ初期化
	g_bPause = false;					// ポーズ初期設定
	g_nCounterGame = 0;					// カウンタ初期化
	g_nCntSetStage = 0;					// カウンタ初期化

	// ステージクリア状態
	for (int nCount = 0; nCount < MAX_STAGE; nCount++)
	{
		g_aStageClear[nCount] = false;
	}

	// マップ初期化処理
	InitMap();

	// スコア初期化処理
	InitScore();

	// ブロック初期化処理
	InitBlock();

	// 敵初期化処理
	InitEnemy();

	// プレイヤー初期化処理
	InitPlayer();

	// 弾初期化処理
	InitBullet();

	// 爆発初期化処理
	InitExplosion();

	// アイテム初期化処理
	InitItem();

	// ポーズ初期化処理
	InitPause();
}

//=========================================================================================================================
// ゲーム終了処理
//=========================================================================================================================
void UninitGame(void)
{
	// マップ終了処理
	UninitMap();

	// スコア終了処理
	UninitScore();

	// ブロック終了処理
	UninitBlock();

	// 敵終了処理
	UninitEnemy();

	// プレイヤー終了処理
	UninitPlayer();

	// 弾終了処理
	UninitBullet();

	// 爆発終了処理
	UninitExplosion();

	// アイテム終了処理
	UninitItem();

	// ポーズ終了処理
	UninitPause();
}

//=========================================================================================================================
// ゲーム更新処理
//=========================================================================================================================
void UpdateGame(void)
{
	bool bHelp = GetHelpState();

	if (GetKeyboardTrigger(DIK_P) == true && (g_GameState == GAMESTATE_MAP || g_GameState == GAMESTATE_GAMEPLAY) && bHelp == false)
	{// ポーズを切替
		g_bPause = g_bPause ? false : true;

		if (g_bPause == true)
		{
			PlaySound(SOUND_LABEL_SE_PAUSEIN);
		}
		else
		{
			PlaySound(SOUND_LABEL_SE_PAUSEOUT);
		}
	}

	// ステージ初期設定
	SetStage();

	switch (g_GameState)
	{
	case GAMESTATE_MAP:			// マップ表示

		if (g_bPause == false)
		{// ポーズが使用されていない
			// マップ更新処理
			UpdateMap();

			// スコア更新処理
			UpdateScore();

			// プレイヤー更新処理
			UpdatePlayer();

			// ポーズ状態設定
			SetPauseState();
		}

		break;
	case GAMESTATE_GAMEPLAY:	// ゲーム表示
	case GAMESTATE_GAMECLEAR:	// ゲームクリア
	case GAMESTATE_GAMEOVER:	// ゲームオーバー

		if (g_bPause == false)
		{// ポーズが使用されていない
			// スコア更新処理
			UpdateScore();

			// ブロック更新処理
			UpdateBlock();

			// 敵更新処理
			UpdateEnemy();

			// プレイヤー更新処理
			UpdatePlayer();

			// 弾更新処理
			UpdateBullet();

			// 爆発更新処理
			UpdateExplosion();

			// アイテム更新処理
			UpdateItem();

			// ポーズ状態設定
			SetPauseState();
		}

		if (g_GameState == GAMESTATE_GAMECLEAR || g_GameState == GAMESTATE_GAMEOVER)
		{// ゲームクリア、オーバーの場合
			g_nCounterGameState++;	// カウンターを加算

			if (g_nCounterGameState >= 60)
			{// 一定カウント後
				SetFadeMode(MODE_RESULT);		// リザルトへ
			}
		}

		break;
	}

	if (g_bPause == true)
	{// ポーズが使用されている場合
		// ポーズの更新処理
		UpdatePause();
	}
}

//=========================================================================================================================
// ゲーム描画処理
//=========================================================================================================================
void DrawGame(void)
{
	switch (g_GameState)
	{
	case GAMESTATE_MAP:			// マップ表示

		// マップ描画処理
		DrawMap();

		// スコア描画処理
		DrawScore();

		// プレイヤー描画処理
		DrawPlayer();

		break;
	case GAMESTATE_GAMEPLAY:	// ゲーム表示
	case GAMESTATE_GAMECLEAR:	// ゲームクリア
	case GAMESTATE_GAMEOVER:	// ゲームオーバー

		// スコア描画処理
		DrawScore();

		// ブロック描画処理
		DrawBlock();

		// 敵描画処理
		DrawEnemy();

		// プレイヤー描画処理
		DrawPlayer();

		// 弾描画処理
		DrawBullet();

		// 爆発描画処理
		DrawExplosion();

		// アイテム描画処理
		DrawItem();

		break;
	}

	if (g_bPause == true && (g_GameState == GAMESTATE_MAP || g_GameState == GAMESTATE_GAMEPLAY))
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
	if (state != GAMESTATE_GAMECLEAR && state != GAMESTATE_GAMEOVER)
	{// 破棄処理
		DeleteBGWindow();			// 背景
		DeleteMap();				// マップ
		DeleteScore();				// スコア
		DeleteBlock();				// ブロック
		DeleteEnemy();				// 敵
		DeletePlayer();				// プレイヤー
		DeleteBullet();				// 弾
		DeleteItem();				// アイテム
	}

	GameSoundStop();

	g_bPause = false;				// ポーズを閉じる
	g_GameState = state;			// 状態を記憶
	g_nCntSetStage = 0;
}

//=========================================================================================================================
// ゲーム取得処理
//=========================================================================================================================
GAMESTATE GetGameState(void)
{
	return g_GameState;				// 値を返す
}

//=========================================================================================================================
// ゲームステージ設定
//=========================================================================================================================
void SetNumStage(int stage)
{
	if (GAMESTAGE_NO1 == stage)
	{
		g_GameStageOpen = GAMESTAGE_NO1;
	}
	else if (GAMESTAGE_NO2 == stage)
	{
		g_GameStageOpen = GAMESTAGE_NO2;
	}
	else if (GAMESTAGE_NO3 == stage)
	{
		g_GameStageOpen = GAMESTAGE_NO3;
	}
	else if (GAMESTAGE_NO4 == stage)
	{
		g_GameStageOpen = GAMESTAGE_NO4;
	}
	else if (GAMESTAGE_NO5 == stage)
	{
		g_GameStageOpen = GAMESTAGE_NO5;
	}
}

//=========================================================================================================================
// ゲームステージ取得
//=========================================================================================================================
GAMESTAGE GetNumStage(void)
{
	return g_GameStageOpen;
}

//=========================================================================================================================
// ゲームステージ設定
//=========================================================================================================================
void SetStageClearState(void)
{
	g_aStageClear[g_GameStageOpen] = true;

	int nCntClearStage = 0;

	for (int nCount = 0; nCount < MAX_STAGE; nCount++)
	{
		if (g_aStageClear[nCount] == true)
		{
			nCntClearStage++;
		}
	}

	if (nCntClearStage == MAX_STAGE)
	{
		SetGameState(GAMESTATE_GAMECLEAR);
	}
	else
	{
		SetFadeGame(GAMESTATE_MAP);
	}
}

//=========================================================================================================================
// ゲームステージクリア取得
//=========================================================================================================================
bool *GetStageClearState(void)
{
	return &g_aStageClear[0];
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
// ゲームステージ設定処理
//=========================================================================================================================
void GameSoundStop(void)
{
	// サウンド終了
	if (g_GameState == GAMESTATE_MAP)
	{// ゲームマップ
		StopSound(SOUND_LABEL_BGM_GAME_MAP);
	}
	else if (g_GameState == GAMESTATE_GAMEPLAY)
	{// ゲームプレイ
		if (g_GameStageOpen == GAMESTAGE_NO1)
		{// ステージ１
			StopSound(SOUND_LABEL_BGM_GAME_STAGE1);
		}
		else if (g_GameStageOpen == GAMESTAGE_NO2)
		{// ステージ２
			StopSound(SOUND_LABEL_BGM_GAME_STAGE2);
		}
		else if (g_GameStageOpen == GAMESTAGE_NO3)
		{// ステージ３
			StopSound(SOUND_LABEL_BGM_GAME_STAGE3);
		}
	}

}

//===========================================================================================
// ゲームステージ設定処理
//===========================================================================================
void SetStage(void)
{
	if (g_nCntSetStage == 0)
	{// 一回のみ
		switch (g_GameState)
		{// ゲーム状態
		case GAMESTATE_MAP:		// ゲームマップ

			// 背景設定
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_MAP);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 - 250, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 30, BGWINDOWTEXTURE_GAME_NUMPLAYER);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 - 100, 33, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 38, BGWINDOWTEXTURE_GAME_NUMSTAGE);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 120, 30, BGWINDOWTEXTURE_GAME_HISCORETXT);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400, SCREEN_HEIGHT - 80, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300, 100, BGWINDOWTEXTURE_GAME_PKEY);

			// マップ設定
			SetMap(D3DXVECTOR3(SCREEN_WIDTH / 2 - 230, SCREEN_HEIGHT - 140, 0.0f));
			SetMap(D3DXVECTOR3(SCREEN_WIDTH / 2 - 270, SCREEN_HEIGHT / 2 - 120, 0.0f));
			SetMap(D3DXVECTOR3(SCREEN_WIDTH / 2 + 330, SCREEN_HEIGHT / 2 + 110, 0.0f));

			// スコア設定
			SetScore(D3DXVECTOR3(SCREEN_WIDTH / 4 - 180, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_PLAYER);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH - 30, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_HISCORE);

			// サウンド設定
			PlaySound(SOUND_LABEL_BGM_GAME_MAP);

			break;

		case GAMESTATE_GAMEPLAY:// ゲームプレイ

			switch (g_GameStageOpen)
			{// ゲームステージ
			case GAMESTAGE_NO1:	// ステージ１
			{
				//背景設定
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_STAGE);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80, BGWINDOWTEXTURE_GAME_HELP);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 240, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 35, BGWINDOWTEXTURE_GAME_HELPWALK);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80, BGWINDOWTEXTURE_GAME_HELP);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 240, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 35, BGWINDOWTEXTURE_GAME_HELPRUN);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80, BGWINDOWTEXTURE_GAME_HELP);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 240, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 35, BGWINDOWTEXTURE_GAME_HELPJUMP);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 2 + 800, SCREEN_HEIGHT - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80, BGWINDOWTEXTURE_GAME_HELP);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 2 + 800, SCREEN_HEIGHT - 240, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 35, BGWINDOWTEXTURE_GAME_HELPATTACK);
				//SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 -100, SCREEN_HEIGHT - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80, BGWINDOWTEXTURE_GAME_HELP);
				//SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 -100, SCREEN_HEIGHT - 240, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 35, BGWINDOWTEXTURE_GAME_HELPATTACK);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 380, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 75, 250, BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);

				// ブロック設定
				{
					// 階段
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2, SCREEN_HEIGHT - 280, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 50, SCREEN_HEIGHT - 330, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 100, SCREEN_HEIGHT - 380, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 150, SCREEN_HEIGHT - 430, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 200, SCREEN_HEIGHT - 480, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 530, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					// 左壁
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 480, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 430, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 380, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 330, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 280, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					// 右壁
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 630, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 580, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 530, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 480, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 430, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 380, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 330, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 280, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					// 囲い
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 50, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 50, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 100, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 50, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 100, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 100, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 150, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);

					// 地面 + 左壁
					SetBlock(D3DXVECTOR3(-SCREEN_WIDTH + 100, 0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_WALL);
					SetBlock(D3DXVECTOR3(-400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
				}

				// アイテム設定
				{
					// 歩き
					SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2 + 290, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2 + 330, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2 + 370, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2 + 410, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					// 走り
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 250, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 290, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 330, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 370, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 410, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					// ジャンプ
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 250 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 290 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 230, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 330 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 280, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 370 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 280, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 410 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 230, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 450 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					// 階段
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 25, SCREEN_HEIGHT - 280, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 125, SCREEN_HEIGHT - 380, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 480, 0.0f), ITEMTYPE_COIN_NORMAL);
					// 落下
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 325, SCREEN_HEIGHT - 580, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 325, SCREEN_HEIGHT - 530, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 325, SCREEN_HEIGHT - 480, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 325, SCREEN_HEIGHT - 430, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 325, SCREEN_HEIGHT - 380, 0.0f), ITEMTYPE_COIN_NORMAL);
					// 囲い
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 + 75, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
				}

				// 敵設定
				SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 3 - 80, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);

				// サウンド設定
				PlaySound(SOUND_LABEL_BGM_GAME_STAGE1);
			}
			break;
			case GAMESTAGE_NO2:	// ステージ２
			{
				//背景設定
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),				SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_STAGE);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 130, 0.0f), D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f),		SCREEN_WIDTH / 2, 40, BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 150, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f),	SCREEN_WIDTH / 2 + 50, 40, BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 170, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f),	SCREEN_WIDTH / 2 + 100, 40, BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 30, 0.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f),			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),	SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 4 - 30, BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + 900, SCREEN_HEIGHT - 380, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),		75, 250, BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 4 - 180, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),		80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 75, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),				SCREEN_WIDTH / 2, 75, BGWINDOWTEXTURE_GAME_STAGE_IWA);

				// ブロック設定
				{

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 100, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 500, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 550, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 600, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 750, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 750, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 750, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 750, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 750, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 900, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 950, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 1000, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 100, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 150, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 200, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 300, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 100, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_BREAK, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 100, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_BREAK, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 300, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_BREAK, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 300, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_BREAK, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 500, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 300, BLOCKTYPE_NORMAL_WARP_Y, 0, BLOCKTEXTURE_DOKAN);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 440, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 560, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 610, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 660, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 710, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 760, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 810, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 860, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 910, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 960, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);

				//	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 1010, SCREEN_HEIGHT - 600, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1000, 50, BLOCKTYPE_NORMAL_WARP_X, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 80, SCREEN_HEIGHT - 150, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, 50, BLOCKTYPE_NORMAL_APPEAR, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 120, SCREEN_HEIGHT - 150, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, 50, BLOCKTYPE_NORMAL_APPEAR, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 320, SCREEN_HEIGHT - 150, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, 50, BLOCKTYPE_NORMAL_APPEAR, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 520, SCREEN_HEIGHT - 150, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, 50, BLOCKTYPE_NORMAL_APPEAR, 0, BLOCKTEXTURE_NORMAL);

					// 地面 + 左壁
					SetBlock(D3DXVECTOR3(-SCREEN_WIDTH + 100, 0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_WALL);
					SetBlock(D3DXVECTOR3(-400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH - 430, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 - 180, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 720, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH + SCREEN_WIDTH / 2, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
				}

				// アイテム設定
				{
					SetItem(D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 50, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 280, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 50, SCREEN_HEIGHT - 280, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 575, SCREEN_HEIGHT - 400, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 575, SCREEN_HEIGHT - 450, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 575, SCREEN_HEIGHT - 500, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 775, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 775, SCREEN_HEIGHT - 350, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 775, SCREEN_HEIGHT - 450, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 975, SCREEN_HEIGHT - 400, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 975, SCREEN_HEIGHT - 450, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 975, SCREEN_HEIGHT - 500, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 175, SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 175, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 275, SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 275, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 175, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 350, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 550, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 275, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 785, SCREEN_HEIGHT - 600, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 985, SCREEN_HEIGHT - 600, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 - 30, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 + 170, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 + 370, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 + 570, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
				}

				// 敵設定
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 300, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 4, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_BIRD);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 575, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_CURSE);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 975, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_CURSE);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 2 + 200, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);
				}

				// サウンド設定
				PlaySound(SOUND_LABEL_BGM_GAME_STAGE2);
			}
			break;
			case GAMESTAGE_NO3:	// ステージ３
			{
				//背景設定
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_STAGE);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 30, 0.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, BGWINDOWTEXTURE_GAME_STAGE_SABAKU_YAMA);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT - 380, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 75, 250, BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);

				// ブロック設定
				{
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 50, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 100, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 150, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 200, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 250, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 310, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 250, BLOCKTYPE_NORMAL_WARP_Y, 180, BLOCKTEXTURE_DOKAN);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 370, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 420, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 470, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 520, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 570, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 620, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 300, BLOCKTYPE_NORMAL_WARP_Y, 0, BLOCKTEXTURE_DOKAN);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 120, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 300, BLOCKTYPE_NORMAL_WARP_Y, 0, BLOCKTEXTURE_DOKAN);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 290, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 300, BLOCKTYPE_NORMAL_WARP_Y, 0, BLOCKTEXTURE_DOKAN);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 170, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 300, BLOCKTYPE_NORMAL_WARP_Y, 0, BLOCKTEXTURE_DOKAN);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 240, SCREEN_HEIGHT - 155, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300, 150, BLOCKTYPE_NORMAL_SUNA_RE, 180, BLOCKTEXTURE_SUNA);


					// 地面 + 左壁
					SetBlock(D3DXVECTOR3(-SCREEN_WIDTH + 100, 0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_WALL);
					SetBlock(D3DXVECTOR3(-400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_SABAKU);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_SABAKU);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_SABAKU);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH / 2, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_SABAKU);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 530, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH * 2, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_SABAKU);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 800, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 800, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 800, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);
				}

				// アイテム設定
				{
					SetItem(D3DXVECTOR3(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH - 110, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 75, SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 125, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 175, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 225, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 445, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 495, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 545, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 595, SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 275, SCREEN_HEIGHT - 500, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 335, SCREEN_HEIGHT - 550, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 395, SCREEN_HEIGHT - 500, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 780, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 870, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);

					for (int nCount = 0; nCount < 8; nCount++)
					{
						SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 235 + (nCount * 100.0f), SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);
					}
					for (int nCount = 0; nCount < 7; nCount++)
					{
						SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 285 + (nCount * 100.0f), SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					}

					for (int nCount = 0; nCount < 5; nCount++)
					{
						SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 235, SCREEN_HEIGHT - 300 - (nCount * 50.0f), 0.0f), ITEMTYPE_COIN_NORMAL);
						SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 935, SCREEN_HEIGHT - 300 - (nCount * 50.0f), 0.0f), ITEMTYPE_COIN_NORMAL);
					}

					for (int nCount = 0; nCount < 5; nCount++)
					{
						SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 + 290 + (nCount * 50.0f), SCREEN_HEIGHT - 100, 0.0f), ITEMTYPE_COIN_NORMAL);
					}
				}

				// 敵設定
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 500, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 155, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_CURSE);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 250, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 5, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_GHOST);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 430, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 5, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_GHOST);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 830, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_CURSE);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 2 + 500, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 5, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_GHOST);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 2 + 580, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 5, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_GHOST);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 2 + 660, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 5, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_GHOST);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 3, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 4, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_BIRD);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 3 + 200, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 4, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_BIRD);
				}

				// サウンド設定
				PlaySound(SOUND_LABEL_BGM_GAME_STAGE3);
			}
			break;
			case GAMESTAGE_NO4: // ステージ４
			{
				//背景設定
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_STAGE);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 380, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 75, 250, BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);

				// ブロック設定
				{
					// 地面 + 左壁
					SetBlock(D3DXVECTOR3(-SCREEN_WIDTH + 100, 0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_WALL);
					SetBlock(D3DXVECTOR3(-400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
				}
			}
			break;
			case GAMESTAGE_NO5:	// ステージ５
			{
				//背景設定
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_STAGE);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 380, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 75, 250, BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);

				// ブロック設定
				{
					// 地面 + 左壁
					SetBlock(D3DXVECTOR3(-SCREEN_WIDTH + 100, 0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_WALL);
					SetBlock(D3DXVECTOR3(-400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
				}
			}
			break;
			}

			// プレイヤー設定
			SetPlayer(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT - 160, 0.0f));

			// ステータス
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 - 250, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 30, BGWINDOWTEXTURE_GAME_NUMPLAYER);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 + 180, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 20, BGWINDOWTEXTURE_GAME_LIFE);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 + 130, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 20, BGWINDOWTEXTURE_GAME_LIFE);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 + 80, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 20, BGWINDOWTEXTURE_GAME_LIFE);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 * 3 - 170, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 25, 25, BGWINDOWTEXTURE_GAME_COIN);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 * 3 - 65, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 90, 40, BGWINDOWTEXTURE_GAME_COINTXT);

			// スコア設定
			SetScore(D3DXVECTOR3(SCREEN_WIDTH / 4 - 180, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_PLAYER);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_TIME);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH / 4 * 3 - 50, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_COIN);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH / 4 * 3 - 50, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_COIN);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH - 30, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_SCORE);

			break;
		}
	}
	g_nCntSetStage = 1;
}