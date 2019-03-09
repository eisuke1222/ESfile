//=========================================================================================================================
//
// オブジェクト処理 [xobjectr.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _XOBJECT_H_
#define _XOBJECT_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define XOBJECT_TEXT_NAME			(255)				
#define XOBJECT_TEXT_INFO			"data\\TEXT\\objectdata.ini"

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// オブジェクト番号
	TEXTTYPE_TREE_0 = 0,
	TEXTTYPE_TREE_1,
	TEXTTYPE_TREE_2,
	TEXTTYPE_TREE_3,
	TEXTTYPE_TREE_4,
	TEXTTYPE_TREE_5,
	TEXTTYPE_TREE_6,
	TEXTTYPE_TREE_7,
	TEXTTYPE_TREE_8,
	TEXTTYPE_TREE_9,
	TEXTTYPE_TREE_10,
	TEXTTYPE_TREE_11,
	TEXTTYPE_TREE_12,
	TEXTTYPE_TREE_13,
	TEXTTYPE_TREE_14,
	TEXTTYPE_MARUTA_1,
	TEXTTYPE_MARUTA_2,
	TEXTTYPE_MARUTA_3,
	TEXTTYPE_MAX

}TEXTTYPE;

typedef struct
{
	char						FileName[XOBJECT_TEXT_NAME];	// Xファイル名
	LPDIRECT3DTEXTURE9			*pTexture = NULL;				// テクスチャ情報へのポインタ
	LPD3DXMESH					pMesh = NULL;					// メッシュ情報へのポインタ
	LPD3DXBUFFER				pBuffMat = NULL;				// マテリアル情報へのポインタ
	DWORD						nNumMat = NULL;					// マテリアル情報の数
	int							nMaxModel;						// モデルの最大数
	int							nOya;							// 親子関係
	D3DXVECTOR3					Initpos;						// 初期位置
	D3DXVECTOR3					pos;							// 現在位置
	D3DXVECTOR3					move;							// 移動
	D3DXVECTOR3					Initrot;						// 初期向き
	D3DXVECTOR3					rotDest;						// 目標向き
	D3DXVECTOR3					rot;							// 現在向き
	D3DXVECTOR3					vtxMin;							// 頂点の最小値
	D3DXVECTOR3					vtxMax;							// 頂点の最大値
	D3DXCOLOR					col;							// 色
	D3DXMATRIX					mtxWorld;						// ワールドマトリックス
	int							nIdxShadow;						// 影番号
	bool						bUse;							// 使用しているか
}XOBJECT;

typedef struct
{
	char						FileName[XOBJECT_TEXT_NAME];	// Xファイル名
}XOBJECTINI;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitXObject(void);				// オブジェクト初期化処理
void UninitXObject(void);			// オブジェクト終了処理
void UpdateXObject(void);			// オブジェクト更新処理
void DrawXObject(void);				// オブジェクト描画処理

void SetXObject(TEXTTYPE type);		// オブジェクト設定処理
bool CollisionXObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
void DeleteXObject(void);

#endif
