//=========================================================================================================================
//
// フィールド処理 [meshfield.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MESHFIELD_TEXTURE_NAME1		"data\\TEXTURE\\field100.jpg"			// 読み込むテクスチャファイル1
#define MESHFIELD_TEXTURE_NAME2		"data\\TEXTURE\\field104.jpg"			// 読み込むテクスチャファイル2
#define MESHFIELD_TEXTURE_NAME3		"data\\TEXTURE\\field102.jpg"			// 読み込むテクスチャファイル3
#define MESHFIELD_TEXTURE_NAME4		"data\\TEXTURE\\field103.jpg"			// 読み込むテクスチャファイル4
#define MESHFIELD_TEXTURE_NAME5		"data\\TEXTURE\\field106.jpg"			// 読み込むテクスチャファイル5
#define MESHFIELD_TEXTURE_NAME6		"data\\TEXTURE\\hunsui.jpg"				// 読み込むテクスチャファイル6

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{
	FIELDTEXTURE_FIELD = 0,
	FIELDTEXTURE_PARK,
	FIELDTEXTURE_WALLTOP,
	FIELDTEXTURE_WALLTOP2,
	FIELDTEXTURE_PAZZLE,
	FIELDTEXTURE_HUNSUI,
	FIELDTEXTURE_MAX,
}FIELDTEXTURE;

typedef enum
{
	FIELDTEXT_FIELD_1 = 0,
	FIELDTEXT_FIELD_2,
	FIELDTEXT_FIELD_3,
	FIELDTEXT_FIELD_4,
	FIELDTEXT_FIELD_5,
	FIELDTEXT_FIELD_6,
	FIELDTEXT_FIELD_7,
	FIELDTEXT_FIELD_8,
	FIELDTEXT_FIELD_9,
	FIELDTEXT_FIELD_OTHER_1,
	FIELDTEXT_FIELD_OTHER_2,
	FIELDTEXT_FIELD_OTHER_3,
	FIELDTEXT_FIELD_OTHER_4,
	FIELDTEXT_FIELD_WALL_1,
	FIELDTEXT_FIELD_WALL_2,
	FIELDTEXT_FIELD_WALL_3,
	FIELDTEXT_FIELD_WALL_4,
	FIELDTEXT_FIELD_PAZZLE,
	FIELDTEXT_FIELD_HUNSUI,
	FIELDTEXT_MAX,
}FIELDTEXT;

typedef struct
{
	D3DXVECTOR3	pos;			// フィールドの位置
	D3DXVECTOR3	rot;			// フィールドの向き
	D3DXVECTOR3	num;			// フィールドの数
	D3DXVECTOR3	fRadius;		// フィールドの半径
	D3DXVECTOR3	tex;			// フィールドのtex
	int			MaxVtx;			// 頂点の最大数
	int			MaxIdx;			// インデックスの最大数
	int			MaxPolygon;		// ポリゴンの最大数
	int			nTexNum;		// テクスチャ番号
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	bool		bUse;			// 使用しているか
}MeshField;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitMeshField(void);		// フィールドの初期化処理
void UninitMeshField(void);		// フィールドの終了処理
void UpdateMeshField(void);		// フィールドの更新処理
void DrawMeshField(void);		// フィールドの描画処理

void SetMeshField(int SetNum, int nTexNum);		// フィールドの設定処理
MeshField *GetMeshField(void);	// フィールドの取得処理
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);		// フィールドの衝突処理
void DeleteMeshField(void);

#endif
