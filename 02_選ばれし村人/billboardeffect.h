//=========================================================================================================================
//
// エフェクトの処理 [billboardeffect.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _BILLBOARDEFFECT_H_
#define _BILLBOARDEFFECT_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// エフェクトタイプ
	EFFECTTYPE_NONE = 0,		// 何もしていない状態
	EFFECTTYPE_NORMAL,			// ノーマル
	EFFECTTYPE_BULLET,			// 弾
	EFFECTTYPE_PARTICLE,		// パーティクル
	EFFECTTYPE_MAX
}EFFECTTYPE;

typedef enum
{// エフェクト状態
	EFFECTSTATE_NORMAL,			// ノーマル
	EFFECTSTATE_LOOP,			// ループ
	EFFECTSTATE_MAX
}EFFECTSTATE;

typedef enum
{// エフェクト弾
	EFFECTBULLET_NONE = 0,
	EFFECTBULLET_PLAYER,
	EFFECTBULLET_PLAYER2,
	EFFECTBULLET_ENEMY,
	EFFECTBULLET_MAX,
}EFFECTBULLET;

typedef struct
{// 情報
	D3DXVECTOR3	pos;			// 位置
	D3DXVECTOR3	rot;			// 向き
	D3DXVECTOR3	move;			// 移動量
	D3DXVECTOR3	fRadius;		// 大きさ
	D3DXCOLOR	col;			// 色
	float		coladown;
	int			nLife;
	EFFECTTYPE	type;			// 種類
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	int			nIdxShadow;		// 影番号
	int			nDamage;
	int			nType;
	bool		bUse;			// 使用しているか
}BillboardEffect;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitBillboardEffect(void);			// エフェクトの初期化処理
void UninitBillboardEffect(void);		// エフェクトの終了処理
void UpdateBillboardEffect(void);		// エフェクトの更新処理
void DrawBillboardEffect(void);			// エフェクトの描画処理

void SetBillboardNormalEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown);		// ノーマルエフェクトの設定処理
void SetBillboardBulletEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown, int nLife, int nDamage, int nType);		// 弾エフェクトの設定処理
void SetBillboardParticleEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown, int nMaxSet, EFFECTSTATE state);	// パーティクルエフェクトの設定処理
void SetXEnemyCollNum(int nXEnemy, int nXEnemyNum);
bool CollisionEffect(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMax, D3DXVECTOR3 *vtxMin, int nType);
void DeleteBillboardEffect(int nCntBillboardEffect);		// エフェクトの破棄処理
void DeleteBillboardEffect(void);		// エフェクトの破棄処理

#endif
