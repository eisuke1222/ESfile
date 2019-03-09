//=========================================================================================================================
// 
// 選択処理 [select.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _SELECT_H_
#define _SELECT_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"		// メイン

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// 状態
	SELECTSTATE_NONE = 0,			// 何もしていない状態
	SELECTSTATE_ENTER,				// ENTER入力状態
	SELECTSTATE_MAX,				// 最大数
}SELECTSTATE;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitSelect(void);				// 選択初期化処理
void UninitSelect(void);			// 選択終了処理
void UpdateSelect(void);			// 選択更新処理
void DrawSelect(void);				// 選択描画処理

void SetEnterSelect(D3DXVECTOR3 pos, D3DXVECTOR3 nRadius);		// 選択設定処理
void ChangeSelect(SELECTSTATE state);	// 選択変化処理

#endif