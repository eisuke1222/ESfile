//=============================================================================
//
// クエストフェード処理 [questfade.h]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#ifndef _QUESTFADE_H_
#define _QUESTFADE_H_

#include "main.h"
#include "game.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define QUESTINTERVAL_TIME		(60 * 10)						//画面切り替えの時間

//=============================================================================
// フェードの状態
//=============================================================================
typedef enum
{
	QUESTFADE_NONE = 0,		// 何もしていない状態
	QUESTFADE_IN,			// フェードイン状態
	QUESTFADE_OUT,			// フェードアウト状態
	QUESTFADE_MAX
} QUESTFADE;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitQuestFade(QUESTSTATE modeNext);
void UninitQuestFade(void);
void UpdateQuestFade(void);
void DrawQuestFade(void);

void SetQuestFade(QUESTSTATE modeNext);
QUESTFADE GetQuestFade(void);

#endif
