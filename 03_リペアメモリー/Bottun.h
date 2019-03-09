//=============================================================================
//
// ボタン処理 [Bottun.h]
// Author : 佐藤安純　Sato_Aumi
//
//=============================================================================
#ifndef _BOTTUN_H_
#define _BOTTUN_H_

#include "main.h"

//=============================================================================
// 状態の設定
//=============================================================================
typedef enum
{
	BOTTUNTYPE_NOMAL = 0,
	BOTTUNTYPE_SELECT,
	BOTTUNTYPE_ENTER,
	BOTTUNTYPE_MAX,
}BOTTUNTYPE;

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	int				nTex;		//テクスチャの名前
	BOTTUNTYPE		type;		//ボタンの状態
	bool			bUse;		//使用しているかどうか
}Bottun;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitBottun(void);
void UninitBottun(void);
void UpdateBottun(void);
void DrawBottun(void);

void SetBottunType(BOTTUNTYPE type);
void SetBottun(void);
void DeleteBottun(void);
#endif

