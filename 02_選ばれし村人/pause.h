//=========================================================================================================================
// 
// ポーズ処理 [pause.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitPause(void);		// ポーズ初期化処理
void UninitPause(void);		// ポーズ終了処理
void UpdatePause(void);		// ポーズ更新処理
void DrawPause(void);		// ポーズ描画処理

void SetPauseState(void);	// ポーズ状態設定処理
bool GetHelpState(void);	// ヘルプ情報取得処理
void SetStickState(bool bTriggerStick);
#endif