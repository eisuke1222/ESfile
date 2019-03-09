//=============================================================================
//
// ポーズ処理 [pouse.h]
// Author : 佐藤安純　Sato_Aumi
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

//=============================================================================
// ポーズの状態の種類
//=============================================================================
typedef enum
{//ポーズの状態
	PAUSESTATE_CONTINUE = 0,
	PAUSESTATE_RETRY,
	PAUSESTATE_QUIT,
	PAUSESTATE_MAX,
}PAUSESTATE;

typedef enum
{//ボタンの状態
	BOTTUNSTATE_NOMAL = 0,
	BOTTUNSTATE_SELECT,
	BOTTUNSTATE_NONE,
	BOTTUNSTATE_MAX,
}BOTTUNSTATE;

//=============================================================================
// ポーズの構造体
//=============================================================================
typedef struct
{
	int				nTex;		//テクスチャの番号
	BOTTUNSTATE		state;		//状態設定
}Pause;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

void SetPauseState(PAUSESTATE state);
Pause * GetPause(void);
#endif