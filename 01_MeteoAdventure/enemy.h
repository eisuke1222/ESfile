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
#include "main.h"
#include "game.h"	// ゲーム

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	MAX_ENEMY						(64)								// 敵の最大数
#define ENEMY_TEXTURE_NAME_1			"data\\TEXTURE\\enemy000.png"		// 読み込むテクスチャファイル1
#define ENEMY_TEXTURE_NAME_2			"data\\TEXTURE\\enemy001.png"		// 読み込むテクスチャファイル2
#define ENEMY_TEXTURE_NAME_3			"data\\TEXTURE\\enemy002.png"		// 読み込むテクスチャファイル3
#define ENEMY_TEXTURE_NAME_4			"data\\TEXTURE\\enemy003.png"		// 読み込むテクスチャファイル4
#define ENEMY_TEXTURE_NAME_5			"data\\TEXTURE\\enemy004.png"		// 読み込むテクスチャファイル5

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// 敵種類
	ENEMYTYPE_NORMALENEMY = 0,	// ノーマル
	ENEMYTYPE_BOSSENEMY,		// ボス
	ENEMYTYPE_MAX
}ENEMYTYPE;

typedef enum
{// 敵状態
	ENEMYSTATE_APPEAR = 0,		// 出現
	ENEMYSTATE_NORMAL,			// ノーマル
	ENEMYSTATE_DAMAGE,			// ダメージ
	ENEMYSTATE_DEATH,			// デス
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef enum
{
	ENEMYANIMTYPE_KNOCKBACK = 0,// ノックバック
	ENEMYANIMTYPE_NORMAL,		// ノーマル
	ENEMYANIMTYPE_WALK,			// 歩く
	ENEMYANIMTYPE_MAX,
}ENEMYANIMTYPE;

typedef enum
{
	ENEMYTEXTURE_SLIME = 0,		// スライム
	ENEMYTEXTURE_SNOWMAN,		// 雪だるま
	ENEMYTEXTURE_CURSE,			// のろい
	ENEMYTEXTURE_BIRD,			// 鳥
	ENEMYTEXTURE_GHOST,			// ゴースト
	ENEMYTEXTURE_MAX,
}ENEMYTEXTURE;

typedef struct
{// 敵
	D3DXVECTOR3 pos;			// 位置
	D3DXVECTOR3 posOld;			// 前回位置
	D3DXVECTOR3 move;			// 移動値
	D3DXCOLOR	col;			// 色
	float		fWidth;			// 横幅
	float		fHeight;		// 高さ
	int			nLife;			// 体力
	ENEMYTYPE	Type;			// 種類
	ENEMYSTATE	State;			// 情報
	int			nCounterState;	// 状態管理カウンター
	ENEMYANIMTYPE AnimType;		// アニメーション種類
	int			nCounterAnim;	// アニメーションカウンタ
	int			nNumAnim;		// アニメーション番号
	int			nSpeedAnim;		// アニメーション速度
	int			nPatternAnim;	// アニメーションパターン
	int			nDinectionMove;	// 方向
	int			nCntTimer;		// 時間カウンタ
	ENEMYTEXTURE texture;		// テクスチャ
	bool bUse;					// 使用しているかどうか
}Enemy;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitEnemy(void);			// 敵初期化処理
void UninitEnemy(void);			// 敵終了処理
void UpdateEnemy(void);			// 敵更新処理
void DrawEnemy(void);			// 敵描画処理

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nLife, ENEMYTYPE Type, ENEMYTEXTURE texture);	// 敵の設定処理
Enemy *GetEnemy(void);			// 敵取得処理
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
void DeleteEnemy(int nCntEnemy);// 敵破棄処理（１つ）
void DeleteEnemy(void);			// 敵破棄処理（全部）

#endif
