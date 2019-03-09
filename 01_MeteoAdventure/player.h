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
#include "main.h"
#include "game.h"		// ゲーム
#include "block.h"		// ブロック

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// プレイヤー状態
	PLAYERSTATE_APPEAR = 0,		// 出現
	PLAYERSTATE_NORMAL,			// ノーマル
	PLAYERSTATE_DAMAGE,			// ダメージ
	PLAYERSTATE_DEATH,			// デス
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{// アニメーションパターン数
	PLAYERANIMTYPE_PATTERN_JUMP = 2,		// ジャンプ
	PLAYERANIMTYPE_PATTERN_ATTACK = 5,		// 攻撃
	PLAYERANIMTYPE_PATTERN_SETSWIM = 3,		// 泳ぐ準備
	PLAYERANIMTYPE_PATTERN_RUN = 5,			// 走る
	PLAYERANIMTYPE_PATTERN_RUNSTOP = 2,		// 止まる
	PLAYERANIMTYPE_PATTERN_SWIM = 5,		// 泳ぐ
	PLAYERANIMTYPE_PATTERN_NORMAL = 5,		// 立ち
	PLAYERANIMTYPE_PATTERN_THROW = 5,		// 投げる
	PLAYERANIMTYPE_PATTERN_WALK = 5,		// 歩く
	PLAYERANIMTYPE_PATTERN_DOORIN = 3,		// ドアに入る+その他
	PLAYERANIMTYPE_PATTERN_MAX = 10,
}PLAYERANIMTYPE_PATTERN;

typedef enum
{// アニメーション種類
	PLAYERANIMTYPE_JUMP = 0,		// ジャンプ
	PLAYERANIMTYPE_ATTACK,			// 攻撃
	PLAYERANIMTYPE_SETSWIM,			// 泳ぐ準備
	PLAYERANIMTYPE_RUN,				// 走る
	PLAYERANIMTYPE_RUNSTOP,			// 止まる
	PLAYERANIMTYPE_SWIM,			// 泳ぐ
	PLAYERANIMTYPE_NORMAL,			// 立ち
	PLAYERANIMTYPE_THROW,			// 投げる
	PLAYERANIMTYPE_WALK,			// 歩く
	PLAYERANIMTYPE_DOORIN,			// ドアに入る+その他
	PLAYERANIMTYPE_MAX,
}PLAYERANIMTYPE;

typedef struct
{// プレイヤー
	D3DXVECTOR3 pos;				// 位置
	D3DXVECTOR3 posOld;				// 前回位置
	D3DXVECTOR3 move;				// 移動値
	D3DXCOLOR   col;				// 色
	float		fWidth;				// 横幅
	float		fHeight;			// 高さ
	int			nNumPlayer;			// 残機
	int			nLife;				// 体力
	float		fGravity;			// 重力
	PLAYERSTATE	State;				// 情報
	int			nCounterState;		// 状態管理カウンター
	PLAYERANIMTYPE AnimType;		// アニメーション種類
	int			nCounterAnim;		// アニメーションカウンタ
	int			nNumAnim;			// アニメーション番号
	int			nSpeedAnim;			// アニメーション速度
	int			nPatternAnim;		// アニメーションパターン
	int			nDinectionMove;		// 方向
	bool		bAttack;			// 攻撃しているか
	bool		bJump;				// ジャンプしているか
	bool		bDisp;				// 表示しているか
	BLOCK		*pBlock;			// ブロックポインタ
}PLAYER;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitPlayer(void);				// プレイヤー初期化処理
void UninitPlayer(void);			// プレイヤー終了処理
void UpdatePlayer(void);			// プレイヤー更新処理
void DrawPlayer(void);				// プレイヤー描画処理

void SetPlayer(D3DXVECTOR3 pos);	// プレイヤー設定処理
PLAYER *GetPlayer(void);			// プレイヤー取得処理
void DeletePlayer(void);			// プレイヤー破棄処理

#endif