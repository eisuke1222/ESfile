//=========================================================================================================================
// 
// エフェクト処理 [effect.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{
	EFFECTTYPE_NONE = 0,
	EFFECTTYPE_BULLET,
	EFFECTTYPE_EXPLOSION,
	EFFECTTYPE_RESURRECTION,
	EFFECTTYPE_RESURRECTIONRAIN,
	EFFECTTYPE_RESURRECTIONHAPPY,		//嬉しい
	EFFECTTYPE_RESURRECTIONUSUALLY,		//普通
	EFFECTTYPE_RESURRECTIONSAD,			//悲しい
	EFFECTTYPE_RESURRECTIONRIPPLE,		//失敗作
	EFFECTTYPE_RESURRECTIONANGRY,		//怒り
	EFFECTTYPE_RESURRECTIONSTAMP,
	EFFECTTYPE_RESURRECTIONWATER,
	EFFECTTYPE_RESURRECTIONWATER_1,


	EFFECTTYPE_MAX
}EFFECTTYPE;

typedef struct
{
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posR;			// 位置
	D3DXVECTOR3 nor;
	D3DXVECTOR3 move;			// 移動値
	D3DXCOLOR col;				// 色
	EFFECTTYPE Type;			// 種類
	D3DXVECTOR3 rot;			//法線ベクトル
	D3DXMATRIX mtxWorld;		// 頂点カラー
	float fRadius;				// 半径(大きさ)
	float fLength;				//距離

	int nTypeTx;
	

	int nLife;					// 表示時間(寿命)
	bool bUse;					// 使用しているかどうか
} EFFECT;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitEffect(void);			// エフェクト初期化処理
void UninitEffect(void);		// エフェクト終了処理
void UpdateEffect(void);		// エフェクト更新処理
void DrawEffect(void);			// エフェクト描画処理

void SetBulletEffect(D3DXVECTOR3 pos, D3DXCOLOR col, EFFECTTYPE Type, float fRadius, int nLife, int nTypeTx);	// 弾エフェクト設定処理
void SetParticleEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx);		// 爆発エフェクト設定処理
void SetSmokeEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife,int nTypeTx);			// 煙エフェクト設定処理
void SetRainEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx);			// 煙エフェクト設定処理
void SetMagicEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx);			// 煙エフェクト設定処理

void SetEmotionEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx);			// 感情エフェクト設定処理
void SetRippleEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx);			// 煙エフェクト設定処理
//void SetAngryEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx);			// 煙エフェクト設定処理

void DeleteEffect(void);
#endif
