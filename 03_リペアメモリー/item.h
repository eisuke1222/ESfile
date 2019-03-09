//=============================================================================
//
// アイテム処理 [item.h]
// Author : 佐藤安純	Sato_Asumi
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//=============================================================================
// 状態設定
//=============================================================================
typedef enum
{
	ITEMTYPE_GIAR_00 = 0,
	ITEMTYPE_GIAR_01,
	ITEMTYPE_GIAR_02,
	ITEMTYPE_GIAR_03,
	ITEMTYPE_WATCH_00,
	ITEMTYPE_WATCH_01,
	ITEMTYPE_WATCH_02,
	ITEMTYPE_ORUGORU_00,
	ITEMTYPE_ORUGORU_01,
	ITEMTYPE_ORUGORU_02,
	ITEMTYPE_MAX,
}ITEMTYPE;

//=============================================================================
// 構造体の定義
//=============================================================================
typedef struct
{
	LPD3DXMESH			pMeshItem = NULL;		//メッシュ情報へのポインタ
	LPD3DXBUFFER		pBuffMatItem = NULL;	//マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9	*pTextureItem = NULL;	// テクスチャへのポインタ
	DWORD				nNumMatItem = NULL;		//マテリアル情報の数
	D3DXVECTOR3			vtxMinItem;				//モデルの最小値
	D3DXVECTOR3			vtxMaxItem;				//モデルの最大値
	D3DXVECTOR3			pos;					//中心座標
	D3DXVECTOR3			OldPos;					//前回の座標
	D3DXVECTOR3			rot;					//向き			
	D3DXMATRIX			mtxWorld;				//ワールドマトリックス
	D3DXVECTOR3			Rot;					//回転量
	D3DXVECTOR3			CircleMax;				//アイテムの最大値の範囲設定
	D3DXVECTOR3			CircleMin;				//アイテムの最小値の範囲設定
	ITEMTYPE			type;					//アイテムの種類
	int					nIdxMark;				//影の使用番号
	int					nIdxFlontPolygon[2];	//フロントポリゴンの使用番号
	int					nCntTimer;
	bool				bInPlayer;				//範囲内に入っているかどうか
	bool				bUse;					//使用しているかどうか
}Item;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ITEMTYPE type);
bool CollsionItem(D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, D3DXVECTOR3 * pPos, D3DXVECTOR3 * pOldPos);
void DeleteItem(int nCntItem, ITEMTYPE type);
void AllDeleteItem(void);
int GetItem(void);
#endif
