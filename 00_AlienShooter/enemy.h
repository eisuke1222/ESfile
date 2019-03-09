//=========================================================================================================================
// 
// 敵処理 [enemy.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"	// メイン
#include "game.h"	// ゲーム
#include "bullet.h"	// 弾

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	MAX_ENEMY		(512)	// 敵の最大数

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// 敵種類
	ENEMYTYPE_TUTORIALENEMY = 0,// チュートリアル
	ENEMYTYPE_TUTORIALBOSSENEMY,// チュートリアルボス
	ENEMYTYPE_NORMALENEMY,		// ノーマル
	ENEMYTYPE_SBOSSENEMY,		// 小ボス
	ENEMYTYPE_MBOSSENEMY,		// 中ボス
	ENEMYTYPE_LBOSSENEMY,		// 大ボス
	ENEMYTYPE_MAX				// 最大数
}ENEMYTYPE;

typedef enum
{// 敵状態
	ENEMYSTATE_APPEAR = 0,		// 出現
	ENEMYSTATE_NOMAL,			// ノーマル
	ENEMYSTATE_DAMAGE,			// ダメージ
	ENEMYSTATE_DEATH,			// デス
	ENEMYSTATE_MAX				// 最大数
}ENEMYSTATE;

typedef enum
{// 敵移動種類
	ENEMYMOVE_NONE = 0,			// 何もない状態
	ENEMYMOVE_NORMAL,			// 通常
	ENEMYMOVE_HOMING,			// 追尾停止
	ENEMYMOVE_HOMING_KEEP,		// 追尾続行
	ENEMYMOVE_HOMING_OUT,		// 追尾画面外へ
	ENEMYMOVE_MAX,				// 最大数
}ENEMYMOVE;

typedef enum
{// 弾種類
	ENEMYBULLET_NONE = 0,		// 何もない状態
	ENEMYBULLET_NO_1,			// 弾１
	ENEMYBULLET_NO_2,			// 弾２
	ENEMYBULLET_NO_3,			// 弾３
	ENEMYBULLET_NO_4,			// 弾４
	ENEMYBULLET_NO_5,			// 弾５
	ENEMYBULLET_NO_6,			// 弾６
	ENEMYBULLET_NO_7,			// 弾７
	ENEMYBULLET_NO_8,			// 弾８
	ENEMYBULLET_NO_9,			// 弾９
	ENEMYBULLET_NO_10,			// 弾１０
	ENEMYBULLET_NO_11,			// 弾１１
	ENEMYBULLET_NO_12,			// 弾１２
	ENEMYBULLET_NO_13,			// 弾１３
	ENEMYBULLET_NO_14,			// 弾１４
	ENEMYBULLET_NO_15,			// 弾１５
	ENEMYBULLET_MAX,			// 最大数
}ENEMYBULLETTYPE;

typedef struct
{// 敵
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posTarget;		// 目標位置
	D3DXVECTOR3 move;			// 移動値
	float moveSpeed;			// 移動速度
	ENEMYMOVE moveType;			// 移動種類
	int moveStart;				// 移動開始
	int moveLimit;				// 移動回数
	int moveChange;				// 移動変更
	int nCntTarget;				// 目標位置回数
	D3DXCOLOR col;				// 色
	int nType;					// 種類
	int nSize;					// 大きさ
	int nLife;					// 寿命
	ENEMYTYPE Type;				// 敵の種類
	ENEMYSTATE State;			// 敵の情報
	int nCounterState;			// 状態管理カウンター
	int nCounterAnim;			// アニメーションカウンター
	int nPatternAnim;			// アニメーションパターンNo.
	bool bUse;					// 使用しているかどうか
}Enemy;

typedef struct
{// 敵弾
	ENEMYBULLETTYPE nPattenType;// 弾種類
	float moveSpeed;			// 移動速度
	int nTiming;				// 発射タイミング
	BULLETTYPE Type;			// 種類
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR col;				// 色
	int nCounterInput;			// 入力カウンタ
	float nAngle;				// 弾角度
}EnemyBullet;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitEnemy(void);			// 敵初期化処理
void UninitEnemy(void);			// 敵終了処理
void UpdateEnemy(void);			// 敵更新処理
void DrawEnemy(void);			// 敵描画処理

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, D3DXVECTOR3 move, float moveSpeed, ENEMYMOVE moveType, int moveStart, int nCntTarget, int nType, int nSize, int nLife, ENEMYTYPE Type,
	EnemyBullet bullet1, EnemyBullet bullet2, EnemyBullet bullet3, EnemyBullet bullet4, EnemyBullet bullet5);	// 敵の設定処理
Enemy *GetEnemy(void);									// 敵の取得
EnemyBullet *GetEnemyBullet(void);						// 敵弾の取得
void HitEnemy(int nCntEnemy, int nDamage);				// 敵の弾当たり処理
int GetNumEnemy(void);									// 敵の総数取得

#endif
