//=========================================================================================================================
// 
// ゲーム処理 [game.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"		// メイン
#include "bg.h"			// 背景

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// ゲーム状態
	GAMESTATE_NONE = 0,		// 何もしていない状態
	GAMESTATE_NORMAL,		// ノーマル
	GAMESTATE_GAMECLEAR,	// ゲームクリア
	GAMESTATE_GAMEOVER,		// ゲームオーバー
	GAMESTATE_MAX			// 最大数
}GAMESTATE;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitGame(void);							// ゲーム初期化処理
void UninitGame(void);							// ゲーム終了処理
void UpdateGame(void);							// ゲーム更新処理
void DrawGame(void);							// ゲーム描画処理

void SetGameState(GAMESTATE state);				// ゲーム設定処理
GAMESTATE GetGameState(void);					// ゲーム取得処理
void SetPauseState(bool bPause);				// ポーズ設定処理

void SetStageState(STAGENUM stage);				// ステージ設定処理

#endif