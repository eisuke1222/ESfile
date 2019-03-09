//=========================================================================================================================
// 
// アイテム処理 [item.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// アイテム種類
	ITEMTYPE_COIN_NORMAL = 0,		// コイン
	ITEMTYPE_COIN_GRAVITY,			// コイン（重力あり）
	ITEMTYPE_MAX
}ITEMTYPE;

typedef enum
{// アイテム状態
	ITEMSTATE_APPEAR = 0,
	ITEMSTATE_NORMAL,
	ITEMSTATE_DISAPPEAR,
	ITEMSTATE_MAX
}ITEMSTATE;

typedef struct
{// アイテム
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 posOld;				// 前回位置
	D3DXCOLOR col;					// 色
	D3DXVECTOR3 move;				// 移動値
	float fWidth;					// 横幅
	float fHeight;					// 高さ
	int nCounterAnim;				// アニメーションカウンタ
	int nPatternAnim;				// アニメーションパターン
	ITEMTYPE Type;					// 種類
	ITEMSTATE state;				// 状態
	int nCounterState;				// 状態カウンタ
	bool bUse;						// 使用しているか
}ItemState;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitItem(void);				// アイテム初期化処理
void UninitItem(void);				// アイテム終了処理
void UpdateItem(void);				// アイテム更新処理
void DrawItem(void);				// アイテム描画処理

void SetItem(D3DXVECTOR3 pos, ITEMTYPE type);		// アイテム設定処理
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float fHeight);		// アイテム衝突処理
int GetNumCoin(void);								// アイテム取得処理
void DeleteItem(int nCntItem);						// アイテム破棄処理（１つ）
void DeleteItem(void);								// アイテム破棄処理（全部）

#endif