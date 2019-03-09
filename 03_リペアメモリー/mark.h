//=============================================================================
//
// マークの処理 [mark.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _MARK_H_
#define _MARK_H_

#include "main.h"

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3		pos;			//中心座標
	D3DXVECTOR3		move;			//移動量
	D3DXVECTOR3		Oldpos;			//前回の座標
	D3DXMATRIX		mtxWorld;		//ワールドマトリックス
	float			ChangeMove;		//変化量
	int				nCntMove;		//変化量のカウンター
	bool			bUse;			//使用しているかどうか		
}Mark;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitMark(void);
void UninitMark(void);
void UpdateMark(void);
void DrawMark(void);

int SetMark(D3DXVECTOR3 pos);
void DeleteMark(void);
void DeleteMark(int nIdxMark);

#endif
