//=============================================================================
//
// 弾処理 [start.h]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#ifndef _START_H_
#define _START_H_

#include "main.h"

//=============================================================================
// 状態の種類	
//=============================================================================
typedef enum
{
	STARTSTRACT_NORMAL = 0,
	STARTSTRACT_ENTER,
	STARTSTRACT_MAX
}STARTSTRACT;

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct 
{
	D3DXVECTOR3		pos;					//中心点
	D3DXCOLOR		col;					//色
	float			ChangeCol;				//透明度の変化量
	STARTSTRACT		Struct;					//状態の設定
	bool			bUse;					//使用しているかどうか
}ENTER;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitStart(void);
void UninitStart(void);
void UpdateStart(void);
void DrawStart(void);

void SetEnter(D3DXVECTOR3 pos, D3DXCOLOR col);
void SetPush(STARTSTRACT stract);
void DeleteStart(void);

#endif