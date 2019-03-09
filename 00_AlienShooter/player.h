//=========================================================================================================================
// 
// プレイヤー処理 [player.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"				// メイン
#include "game.h"				// ゲーム
#include "bullet.h"				// 弾

#define PLAYER_LIFE				(10)									// プレイヤーの体力

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// プレイヤー状態
	PLAYERSTATE_APPEAR = 0,		// 出現
	PLAYERSTATE_NOMAL,			// ノーマル
	PLAYERSTATE_DAMAGE,			// ダメージ
	PLAYERSTATE_INVINCIBLE,		// 無敵
	PLAYERSTATE_DEATH,			// デス
	PLAYERSTATE_MAX				// 最大数
}PLAYERSTATE;

typedef struct
{// プレイヤー
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR col;				// 色
	int nLife;					// 体力
	bool bLifeTex[PLAYER_LIFE];	// 体力テクスチャ
	PLAYERSTATE State;			// プレイヤーの情報
	int nCounterState;			// 状態管理カウンター
	BULLETTYPE Type;			// 種類
	bool bDisp;					// 表示しているかどうか
}Player;

typedef struct
{// プレイヤー弾
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR col;				// 色
	int nCounterInput;			// 入力カウンタ
	float nAngle;				// 弾角度
}PlayerBullet;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitPlayer(void);			// プレイヤー初期化処理
void UninitPlayer(void);		// プレイヤー終了処理
void UpdatePlayer(void);		// プレイヤー更新処理
void DrawPlayer(void);			// プレイヤー描画処理

void SetPlayer(void);			// プレイヤー設定処理
Player *GetPlayer(void);		// プレイヤーの取得処理

void HitPlayer(int nDamage);	// プレイヤー弾当たり処理

#endif