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
#include "main.h"	// メイン

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define SINCOS_ANGLE_135		(0.75f)		// sin,cos角度（0.75）				
#define SINCOS_ANGLE_90			(0.5f)		// sin,cos角度（0.5）
#define SINCOS_ANGLE_45			(0.25f)		// sin,cos角度（0.25）
#define SINCOS_ANGLE_0			(0.0f)		// sin,cos角度（0.0）

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// 種類
	BULLETTYPE_PLAYER1 = 0,		// プレイヤー1
	BULLETTYPE_PLAYER2,			// プレイヤー2
	BULLETTYPE_PLAYER3,			// プレイヤー3
	BULLETTYPE_PLAYER4,			// プレイヤー4
	BULLETTYPE_PLAYER5,			// プレイヤー5
	BULLETTYPE_ENEMY,			// 敵
	BULLETTYPE_BOSSENEMY,		// ボス
	BULLETTYPE_MAX				// 最大数
}BULLETTYPE;

typedef enum
{// 弾移動
	BULLETMOVE_NORMAL = 0,				// 通常
	BULLETMOVE_MOVECHANGE_HOMING,		// ホーミング
	BULLETMOVE_MOVECHANGE_STRAIGHT,		// まっすぐ
	BULLETMOVE_MOVECHANGE_START_1,		// スタート1
	BULLETMOVE_MOVECHANGE_START_2,		// スタート2
	BULLETMOVE_SETBULLET,				// 弾追加
	BULLETMOVE_SETBULLET_1,				// 弾追加1
	BULLETMOVE_MAX
}BULLETMOVE;

typedef struct
{// 弾
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 move;			// 移動量
	D3DXCOLOR col;				// 色
	BULLETTYPE Type;			// 弾の種類
	BULLETMOVE moveType;		// 移動の種類
	float fSpeed;				// 移動速度
	int nCntChange;				// 移動変更
	int nCntPoint;				// 位置カウンタ
	int nTexNum;				// テクスチャ番号
	bool bUse;					// 使用しているかどうか
}Bullet;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitBullet(void);			// 弾初期化処理
void UninitBullet(void);		// 弾終了処理
void UpdateBullet(void);		// 弾更新処理
void DrawBullet(void);			// 弾描画処理

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE Type, BULLETMOVE moveType, float fSpeed, int nCntPoint, int nTexNum);		// 弾の設定処理

#endif
