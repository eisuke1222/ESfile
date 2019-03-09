//=============================================================================
//
// アイテムフォント処理 [itemfont.h]
// Author : 佐藤安純　Sato_Aumi
//
//=============================================================================
#ifndef _ITEMFONT_H_
#define _ITEMFONT_H_

#include "main.h"

//=============================================================================
// 状態の設定
//=============================================================================
typedef enum
{
	TYPEITEMFONT_ITEM = 0,
	TYPEITEMFONT_STORY,
	TYPEITEMFONT_MAX,
}TYPEITEMFONT;

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	char			font[256];		//格納できる文字数
	TYPEITEMFONT	type;			//フォントのタイプ
	bool			bUse;			//使用しているかどうか
}ItemFont;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitItemFont(void);
void UninitItemFont(void);
void UpdateItemFont(void);
void DrawItemFont(void);

void SetItemFont(char font[256], TYPEITEMFONT type);
void DeleteItemFont(void);
bool GetUseItemFont(void);
#endif