//=============================================================================
//
// 影の処理 [shadow.h]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3				pos;			//中心座標
	D3DXVECTOR3				rot;			//向き
	D3DXVECTOR3				Otherpos;		//中心座標
	D3DXMATRIX				mtxWorld;		//ワールドマトリックス
	D3DXCOLOR				color;			//色
	bool					bUse;			//使用しているかどうか
	float					fHight;			//高さ
	float					fWhidth;		//幅
	float					fLength;		//対角線の長さ
}Shadow;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);

int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHight, float fWhidth);
void SetPostionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 OldPos);
void DeleteShadow(void);
void DeleteShadow(int nIdxShadow);
#endif