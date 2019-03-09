//=========================================================================================================================
//
// 敵処理 [xenemy.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _XENEMY_H_
#define _XENEMY_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"
#include "game.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define XENEMY_TEXT_NAME			(255)						
#define XENEMY_TEXT_INFO			"data\\TEXT\\enemydata.ini"
//#define XENEMY_TEXT_INFO			"data\\TEXT\\enemymodeldata.txt"
#define MAX_XENEMYPARTS				(20)					
#define MAX_XENEMYANIM				(12)					
#define MAX_XENEMYANIMKEY			(15)					

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// 敵種類
	ENEMYTYPE_1 = 0,
	ENEMYTYPE_2,
	ENEMYTYPE_3,
	ENEMYTYPE_BOSS,
	ENEMYTYPE_MAX
}ENEMYTYPE;

typedef enum
{// 敵状態
	ENEMYSTATE_NONE = 0,
	ENEMYSTATE_NORMAL,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_DIE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef enum
{// 敵アニメーション
	ENEMYANIM_NEUTRAL = 0,
	ENEMYANIM_WALK,
	ENEMYANIM_ATTACK,
	ENEMYANIM_ATTACK2,
	ENEMYANIM_MAX
}ENEMYANIM;

typedef enum
{// 敵パターン
	ENEMYPATTERN_NONE = 0,
	ENEMYPATTERN_WALKINIT,
	ENEMYPATTERN_WALKRANDOM,
	ENEMYPATTERN_ATTACKSET,
	ENEMYPATTERN_ATTACK,
	ENEMYPATTERN_MAX,
}ENEMYPATTERN;

typedef struct
{
	char						FileName[XENEMY_TEXT_NAME];		// Xファイル名
	LPD3DXMESH					pMesh = NULL;					// メッシュ情報へのポインタ
	LPD3DXBUFFER				pBuffMat = NULL;				// マテリアル情報へのポインタ
	DWORD						nNumMat = NULL;					// マテリアル情報の数
	D3DXMATRIX					mtxWorld;						// ワールドマトリックス
	int							nIndexParent;					// 親子関係
	D3DXVECTOR3					Initpos;						// 初期位置
	D3DXVECTOR3					posDest;						// 目標位置
	D3DXVECTOR3					pos;							// 現在位置
	D3DXVECTOR3					move;							// 移動
	D3DXVECTOR3					Initrot;						// 初期向き
	D3DXVECTOR3					rotDest;						// 目標向き
	D3DXVECTOR3					rot;							// 現在向き
	D3DXVECTOR3					posAnim[MAX_XENEMYANIM][MAX_XENEMYANIMKEY];		// アニメーションpos
	D3DXVECTOR3					rotAnim[MAX_XENEMYANIM][MAX_XENEMYANIMKEY];		// アニメーションrot
	D3DXVECTOR3					vtxMin;							// 頂点の最小値
	D3DXVECTOR3					vtxMax;							// 頂点の最大値
}XENEMYMODEL;

typedef struct
{
	D3DXMATRIX					mtxWorld;						// ワールドマトリックス
	int							nIdxShadow;						// 影番号
	D3DXVECTOR3					Initpos;						// 初期位置
	D3DXVECTOR3					pos;							// 現在位置
	D3DXVECTOR3					posOld;							// 前回位置
	D3DXVECTOR3					move;							// 移動
	D3DXVECTOR3					Initrot;						// 初期向き
	D3DXVECTOR3					rotDest;						// 目標向き
	D3DXVECTOR3					rot;							// 現在向き
	D3DXCOLOR					col;							// 色
	D3DXVECTOR3					vtxMin;							// 頂点の最小値
	D3DXVECTOR3					vtxMax;							// 頂点の最大値
	int							nCntTime;						// カウンター
	int							nMaxPartsNum;					// パーツ数
	XENEMYMODEL					aModel[MAX_XENEMYPARTS];		// パーツ情報
	int							nMaxAnimNum;					// アニメーション数
	int							nAnimloop[MAX_XENEMYANIM];		// ループ
	int							nAnimkey[MAX_XENEMYANIM];		// キー数
	int							nAnimBlend[MAX_XENEMYANIM];		// ブレンド数
	int							nAnimfram[MAX_XENEMYANIM][MAX_XENEMYANIMKEY];		// フレーム数
	int							nAnimChange;					// アニメーションチェンジ
	ENEMYANIM					nAnimnow;						// 現在アニメーション
	int							nAnimkeynow;					// 現在キー
	ENEMYSTATE					state;							// 状態
	int							nCntstate;						// 状態カウンタ
	int							nLife;							// 体力
	int							nInitLife;						// 初期設定体力
	int							nBillBoardHP;					// 体力テクスチャ
	bool						bAttack;						// 使用しているか
	int							nSetAttack;						// 攻撃パターン
	int							nCntTimerand;					// カウンター
	ENEMYPATTERN				Pattern;						// 行動パターン
	bool						bUse;							// 使用しているか
}XENEMY;

typedef struct
{
	int							nSetNum;						// 表示数
	char						FileName[XENEMY_TEXT_NAME];		// Xファイル名
}XENEMYINI;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitXEnemy(void);				// 敵初期化処理
void UninitXEnemy(void);			// 敵終了処理
void UpdateXEnemy(void);			// 敵更新処理
void DrawXEnemy(void);				// 敵描画処理

void SetXEnemy(int type, D3DXVECTOR3 pos);				// 敵設定処理
XENEMY *GetXEnemy(void);			// 敵取得処理
int GetXEnemyNum(void);				// 敵総数取得
D3DXVECTOR3 GetRockOnXEnemy(int nNum);	// 敵ロックオン番号
void ChangeRockOnXEnemy(int nNum);	// 敵ロックオン変更
bool Collision(int nCntXEnemy1, int nCntXEnemyNum1);
bool CollisionXEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
bool CollisionXEnemyWeapon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
void HitXEnemy(int nCntXEnemy, int nCntXEnemyNum, int nDamage);
void DeleteXEnemy(int nCntXEnemy, int nCntXEnemyNum);
void DeleteXEnemy(void);

#endif
