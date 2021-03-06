//=============================================================================
//
// 選択画面 [select.h]
// Author : 佐藤安純　Sato_Aumi
//
//=============================================================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "main.h"

//=============================================================================
// 状態の設定
//=============================================================================
typedef enum
{
	BOTTUNMODE_NOMAL = 0,
	BOTTUNMODE_SELECT,
	BOTTUNMODE_MAX,
}BOTTUNMODE;

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	int				nTex;		//テクスチャの名前
	BOTTUNMODE		mode;		//ボタンの状態
}Select;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitSelect(void);
void UninitSelect(void);
void UpdateSelect(void);
void DrawSelect(void);

#endif

