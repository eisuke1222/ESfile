//=============================================================================
//
// ポリゴン処理 [flontpolygon.h]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#ifndef _FLONTPOLYGON_H_
#define _FLONTPOLYGON_H_

#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_FLONTPOLYGON	(64)		//背景の最大数

//=============================================================================
// 構造体の定義
//=============================================================================
typedef enum
{
	FLONTPOLYGONTEXTURE_GAME_OVER = 0,
	FLONTPOLYGONTEXTURE_FONT_BG,
	FLONTPOLYGONTEXTURE_MESSAGE_PLAYER,
	FLONTPOLYGONTEXTURE_MESSAGE_NPC_0,
	FLONTPOLYGONTEXTURE_MESSAGE_NPC_1,
	FLONTPOLYGONTEXTURE_MESSAGE_NPC_2,
	FLONTPOLYGONTEXTURE_GEAR_0,
	FLONTPOLYGONTEXTURE_GEAR_1,
	FLONTPOLYGONTEXTURE_GEAR_2,
	FLONTPOLYGONTEXTURE_GEAR_3,
	FLONTPOLYGONTEXTURE_CLOCK_0,
	FLONTPOLYGONTEXTURE_CLOCK_1,
	FLONTPOLYGONTEXTURE_CLOCK_2,
	FLONTPOLYGONTEXTURE_MUSICBOX_0,
	FLONTPOLYGONTEXTURE_MUSICBOX_1,
	FLONTPOLYGONTEXTURE_MUSICBOX_2,
	FLONTPOLYGONTEXTURE_NUMBER_SLASH,
	FLONTPOLYGONTEXTURE_MENU_BG,
	FLONTPOLYGONTEXTURE_MENU_1,
	FLONTPOLYGONTEXTURE_MENU_2,
	FLONTPOLYGONTEXTURE_MENU_3,
	FLONTPOLYGONTEXTURE_MENU_4,
	FLONTPOLYGONTEXTURE_MENU_5,
	FLONTPOLYGONTEXTURE_MENU_6,
	FLONTPOLYGONTEXTURE_MENU_7,
	FLONTPOLYGONTEXTURE_MENU_8,
	FLONTPOLYGONTEXTURE_MENU_ZENMAI,
	FLONTPOLYGONTEXTURE_MENU_MB1,
	FLONTPOLYGONTEXTURE_MENU_MB2,
	FLONTPOLYGONTEXTURE_MENU_S0,
	FLONTPOLYGONTEXTURE_MENU_S1,
	FLONTPOLYGONTEXTURE_PAZZLE_IN,
	FLONTPOLYGONTEXTURE_PAZZLE_START,
	FLONTPOLYGONTEXTURE_PAZZLE_END,
	FLONTPOLYGONTEXTURE_QUESTSTAMP_A,
	FLONTPOLYGONTEXTURE_QUESTSTAMP_B,
	FLONTPOLYGONTEXTURE_QUESTSTAMP_C,
	FLONTPOLYGONTEXTURE_QUESTSTAMP_D,
	FLONTPOLYGONTEXTURE_PARTS_WIN_CLOCK,
	FLONTPOLYGONTEXTURE_PARTS_WIN_MUSICBOX,
	FLONTPOLYGONTEXTURE_PARTS_WIN_DOLL,
	FLONTPOLYGONTEXTURE_POINTER,
	FLONTPOLYGONTEXTURE_CLEAR_RANK_A,
	FLONTPOLYGONTEXTURE_CLEAR_RANK_B,
	FLONTPOLYGONTEXTURE_CLEAR_RANK_C,
	FLONTPOLYGONTEXTURE_CLEAR_RANK_D,
	FLONTPOLYGONTEXTURE_CLEARRESULT,
	FLONTPOLYGONTEXTURE_TUTORIAL00,
	FLONTPOLYGONTEXTURE_TUTORIAL01,
	FLONTPOLYGONTEXTURE_TUTORIAL02,
	FLONTPOLYGONTEXTURE_MAX,
	FLONTPOLYGONTEXTURE_QUESTCLEAR,
}FLONTPOLYGONTEXTURE;

typedef struct 
{
	D3DXVECTOR3		pos;			//中心座標
	D3DXCOLOR		col;			//色
	float			fWidth;			//幅
	float			fHeight;		//高さ
	int				nCntAnim;		// アニメーションカウント
	int				nCntPattrn;		// アニメーションパターン
	int				nTexNum;		//テクスチャの番号
	int				nCntPolygon;	//ポリゴンの番号
	bool			bUse;			//使用しているかどうか
}FlontPolygon;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitFlontPolygon(void);
void UninitFlontPolygon(void);
void UpdateFlontPolygon(void);
void DrawFlontPolygon(void);

int SetFlontPolygon(int nTexNum, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHight);
void DeleteAllFlontPolygon(void);
void DeleteFlontPolygon(int nIndx);
void DeleteFlontPolygonA(int nTexNum);
FlontPolygon * GetFlontPolygon(void);

bool DeletePartsTexture(void);
#endif