//=============================================================================
//
// 背景処理 [time.h]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"

//=============================================================================
// 時間の状態
//=============================================================================
typedef enum
{
	TIMESTATE_NONE = 0,
	TIMESTATE_STOP,
	TIMESTATE_END,
	TIMESTATE_NORMAL,
	TIMESTATE_START,
	TIMESTATE_ADD,
	TIMESTATE_RESET,
	TIMESTATE_MAX
}TIME;

//=============================================================================
// 時間の構造体の定義
//=============================================================================
typedef struct
{
	int		nTime;		// 制限時間
	TIME	TimeState;	// 状態設定
}Time;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);

void CntDownTime(void);
Time *GetTime(void);
void SetTimeState(TIME state);
#endif