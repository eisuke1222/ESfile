//=========================================================================================================================
// 
// ブロック処理 [block.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_BLOCK				(128)							// ブロックの最大数

#define BLOCK_TEXTURE_NAME_1	"data\\TEXTURE\\Ground000.png"	// 読み込むテクスチャファイル1
#define BLOCK_TEXTURE_NAME_2	"data\\TEXTURE\\Wall000.png"	// 読み込むテクスチャファイル2
#define BLOCK_TEXTURE_NAME_3	"data\\TEXTURE\\block000.png"	// 読み込むテクスチャファイル3
#define BLOCK_TEXTURE_NAME_4	"data\\TEXTURE\\dokan.png"		// 読み込むテクスチャファイル4
#define BLOCK_TEXTURE_NAME_5	"data\\TEXTURE\\Ground001.png"	// 読み込むテクスチャファイル5
#define BLOCK_TEXTURE_NAME_6	"data\\TEXTURE\\toge000.png"	// 読み込むテクスチャファイル6
#define BLOCK_TEXTURE_NAME_7	"data\\TEXTURE\\suna001.png"	// 読み込むテクスチャファイル7

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// ブロック種類
	BLOCKTYPE_NONE = 0,				// 使用していない
	BLOCKTYPE_NORMAL,				// ノーマル
	BLOCKTYPE_NORMAL_BREAK,			// ノーマル破壊
	BLOCKTYPE_NORMAL_WARP_X,		// ノーマルワープX
	BLOCKTYPE_NORMAL_WARP_Y,		// ノーマルワープY
	BLOCKTYPE_NORMAL_FALL,			// ノーマル落ちる
	BLOCKTYPE_NORMAL_APPEAR,		// ノーマル出現
	BLOCKTYPE_NORMAL_DAMAGE,		// ノーマルダメージ
	BLOCKTYPE_NORMAL_SUNA,			// ノーマル砂
	BLOCKTYPE_NORMAL_SUNA_RE,		// ノーマル砂復活
	BLOCKTYPE_MOVE_TRIP,			// 往復
	BLOCKTYPE_MOVE_LOOP,			// 繰り返し
	BLOCKTYPE_MAX,
}BLOCKTYPE;

typedef enum
{// テクスチャ種類
	BLOCKTEXTURE_GROUND = 0,		// 地面
	BLOCKTEXTURE_WALL,				// 壁
	BLOCKTEXTURE_NORMAL,			// ノーマル
	BLOCKTEXTURE_DOKAN,				// 土管
	BLOCKTEXTURE_SABAKU,			// 砂漠
	BLOCKTEXTURE_TOGE,				// とげ
	BLOCKTEXTURE_SUNA,				// 砂
	BLOCKTEXTURE_MAX,
}BLOCKTEXTURE;

typedef enum
{// 衝突種類
	COLLISIONBLOCKRETURN_PLAYER = 0,// プレイヤー
	COLLISIONBLOCKRETURN_ENEMY,		// 敵
	COLLISIONBLOCKRETURN_BULLET,	// 弾
	COLLISIONBLOCKRETURN_OTHER,		//
	COLLISIONBLOCKRETURN_MAX,
}COLLISIONBLOCKRETURN;

typedef struct
{// ブロック
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 tex;				// 位置
	D3DXVECTOR3 move;				// 移動値
	D3DXCOLOR col;					// 色
	float fWidth;					// 横幅
	float fHeight;					// 高さ
	BLOCKTYPE type;					// ブロック種類
	int nLife;						// 耐久値
	int nCntMove;					// 移動カウント
	int nCntMoveLimit;				// 移動制限
	bool bBlockScroll;				// スクロールするか
	bool bUse;						// 使用しているか
	BLOCKTEXTURE texture;			// テクスチャ
}BLOCK;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitBlock(void);				// ブロック初期化処理
void UninitBlock(void);				// ブロック終了処理
void UpdateBlock(void);				// ブロック更新処理
void DrawBlock(void);				// ブロック描画処理

void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, BLOCKTYPE type, int nCntMoveLimit, BLOCKTEXTURE texture);						// ブロック設定処理
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, BLOCK **pBlock, COLLISIONBLOCKRETURN collision);	// ブロックとの衝突処理
bool GetScrollNum(void);			// スクロール判定取得
void DeleteBlock(int nCntBlock);	// ブロック破棄処理
void DeleteBlock(void);				// ブロック破棄処理

#endif