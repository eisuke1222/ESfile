//=========================================================================================================================
// 
// 弾処理 [bullet.h]
// Author : Einsuke Sakagawa
// 
//=========================================================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"
#include "block.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	MAX_BULLET				(255)									// 弾の最大数
#define BULLET_TEXTURE_NAME1	"data\\TEXTURE\\bullet000.png"			// 読み込むテクスチャファイル1

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// 種類
	BULLETTYPE_PLAYER = 0,		// プレイヤー1
	BULLETTYPE_ENEMY,			// 敵
	BULLETTYPE_BOSSENEMY,		// ボス
	BULLETTYPE_MAX
}BULLETTYPE;

typedef enum
{
	BULLETTEXTURETYPE_PLAYER_NORMAL = 0,	// プレイヤー弾
	BULLETTEXTURETYPE_PLAYER_MAX,
}BULLETTEXTURETYPE;

typedef enum
{// 衝突種類
	COLLISIONBULLETRETURN_BLOCK = 0,	// ブロック
	COLLISIONBULLETRETURN_OTHER,
	COLLISIONBULLETRETURN_MAX,
}COLLISIONBULLETRETURN;

typedef struct
{// 弾
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posOld;			// 前回位置
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR col;				// 色
	BULLETTYPE Type;			// 弾の種類
	float fWidth;				// 横幅
	float fHeight;				// 高さ
	BULLETTEXTURETYPE texture;	// テクスチャ番号
	bool bUse;					// 使用しているかどうか
	BLOCK *pBlock;				// ブロックポインタ
}Bullet;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitBullet(void);			// 弾初期化処理
void UninitBullet(void);		// 弾終了処理
void UpdateBullet(void);		// 弾更新処理
void DrawBullet(void);			// 弾描画処理

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE Type, float fWidth, float fHeight, BULLETTEXTURETYPE texture);		// 弾の設定処理
bool CollisiomBullet(D3DXVECTOR3 *pPos, float fWidth, float fHeight, COLLISIONBULLETRETURN collision);		// 弾衝突処理
void DeleteBullet(int nCntBullet);		// 弾破棄処理（1つ）
void DeleteBullet(void);				// 弾破棄処理（全部）

#endif
