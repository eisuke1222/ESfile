//=========================================================================================================================
//
// オブジェクトの処理 [billboardobject.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _BILLBOARDOBJECT_H_
#define _BILLBOARDOBJECT_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define BILLBOARDOBJECT_TEXTURE_NAME1		"data\\TEXTURE\\rockon.png"			// 読み込むテクスチャファイル1
#define BILLBOARDOBJECT_TEXTURE_NAME2		"data\\TEXTURE\\hp000.png"			// 読み込むテクスチャファイル2
#define BILLBOARDOBJECT_TEXTURE_NAME3		"data\\TEXTURE\\wand000.png"		// 読み込むテクスチャファイル3
#define BILLBOARDOBJECT_TEXTURE_NAME4		"data\\TEXTURE\\wand001.png"		// 読み込むテクスチャファイル4
#define BILLBOARDOBJECT_TEXTURE_NAME5		"data\\TEXTURE\\wand002.png"		// 読み込むテクスチャファイル5
#define BILLBOARDOBJECT_TEXTURE_NAME6		"data\\TEXTURE\\map_wall.png"		// 読み込むテクスチャファイル6
#define BILLBOARDOBJECT_TEXTURE_NAME7		"data\\TEXTURE\\item000.png"		// 読み込むテクスチャファイル7
#define BILLBOARDOBJECT_TEXTURE_NAME8		"data\\TEXTURE\\item001.png"		// 読み込むテクスチャファイル8
#define BILLBOARDOBJECT_TEXTURE_NAME9		"data\\TEXTURE\\item002.png"		// 読み込むテクスチャファイル9
#define BILLBOARDOBJECT_TEXTURE_NAME10		"data\\TEXTURE\\item003.png"		// 読み込むテクスチャファイル10
#define BILLBOARDOBJECT_TEXTURE_NAME11		"data\\TEXTURE\\sword001.png"		// 読み込むテクスチャファイル11

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// テクスチャ
	BILLBOARDTEXTURE_ROCKON = 0,
	BILLBOARDTEXTURE_HP,
	BILLBOARDTEXTURE_WAND_1,
	BILLBOARDTEXTURE_WAND_2,
	BILLBOARDTEXTURE_WAND_3,
	BILLBOARDTEXTURE_MAP_WALL,
	BILLBOARDTEXTURE_ITEM_WEAPON_1,
	BILLBOARDTEXTURE_ITEM_WEAPON_2,
	BILLBOARDTEXTURE_ITEM_WEAPON_3,
	BILLBOARDTEXTURE_ITEM_LIFE,
	BILLBOARDTEXTURE_SWORD,
	BILLBOARDTEXTURE_MAX,
}BILLBOARDTEXTURE;

typedef struct
{
	D3DXVECTOR3	pos;			// オブジェクトの位置
	D3DXVECTOR3	posOld;			// オブジェクトの位置
	D3DXVECTOR3	move;			// オブジェクトの位置
	D3DXVECTOR3	rot;			// オブジェクトの位置
	D3DXCOLOR	col;			// オブジェクトの色
	D3DXVECTOR3	fRadius;		// オブジェクトの向き
	D3DXVECTOR3	fInitRadius;	// オブジェクトの向き
	float		fTex;			// テクスチャ
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	int			nTex;			// テクスチャ番号
	int			nIdxShadow;		// 影番号
	bool		bUse;			// 使用しているか
}BillboardObject;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitBillboardObject(void);			// オブジェクトの初期化処理
void UninitBillboardObject(void);		// オブジェクトの終了処理
void UpdateBillboardObject(void);		// オブジェクトの更新処理
void DrawBillboardObject(void);			// オブジェクトの描画処理

void SetBillboardObject(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, D3DXVECTOR3 fRadius, int nTex);		// オブジェクトの設定処理
void SetPosionRockOn(D3DXVECTOR3 pos);
void SetPosionHP(D3DXVECTOR3 pos, int nBillBoardHP);
void SetUpdateHP(int nBillBoardHP, int nInitHP, int nNowHP);
int SetHPNum(D3DXVECTOR3 pos, D3DXVECTOR3 fRadius);
bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMax, D3DXVECTOR3 *vtxMin);
bool CollisionSword(D3DXVECTOR3 *pPos);
void DeleteBillboardObject(int nCntBillboardObject);		// オブジェクトの破棄処理
void DeleteBillboardObject(BILLBOARDTEXTURE nTex);		// オブジェクトの破棄処理
void DeleteBillboardObject(void);		// オブジェクトの破棄処理

#endif
