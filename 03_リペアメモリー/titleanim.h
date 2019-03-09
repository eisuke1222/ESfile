//=============================================================================
//
// タイトル処理 [titleanim.h]
// Author : 佐藤安純　Sato_Aumi
//
//=============================================================================
#ifndef _TITLEANIM_H_
#define _TITLEANIM_H_

#include "main.h"

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{// タイトルアニメーション
	int nCntAnim;
	int nCntAnimNum;
	bool bUse;
}TitleAnim;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitTitleAnim(void);
void UninitTitleAnim(void);
void UpdateTitleAnim(void);
void DrawTitleAnim(void);

void SetTitleAnim(void);
TitleAnim GetTitleAnim(void);
void SetTitleAnimNum(void);

void DeleteTitleAnim(void);
#endif
