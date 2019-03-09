//=============================================================================
//
// アイテムカウンター処理 [itemCnt.h]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#ifndef _ITEMCNT_H_
#define _ITEMCNT_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_SCORE			(2)		//スコアの桁数

//=============================================================================
// 状態の設定
//=============================================================================
typedef enum
{
	SCORETYPE_WOOD = 0,
	SCORETYPE_GLASS,
	SCORETYPE_CHAIN,
	SCORETYPE_IRON,
	SCORETYPE_POT,
	SCORETYPE_RUBBER,
	SCORETYPE_CLOSS,
	SCORETYPE_MAX,
}SCORETYPE;

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	int	aNum[NUM_SCORE];		// １桁に数字を格納する変数
}ItemCnt;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitItemCnt(void);
void UninitItemCnt(void);
void UpdateItemCnt(void);
void DrawItemCnt(void);

void AddItemCnt(int nValue);
int GetItemCnt(void);
void ResetItemCnt(void);

void DeleteItemCnt(void);
#endif
