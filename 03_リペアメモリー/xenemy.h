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

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define XENEMY_TEXT_NAME			(255)						
#define XENEMY_TEXT_INFO			"data\\TEXT\\enemydata.ini"
//#define XENEMY_TEXT_INFO			"data\\TEXT\\enemymodeldata.txt"
#define MAX_XENEMYPARTS				(20)					
#define MAX_XENEMYANIM				(12)					
#define MAX_XENEMYANIMKEY			(10)					

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{
	ENEMYTYPE_1 = 0,
	ENEMYTYPE_2,
	ENEMYTYPE_3,
	ENEMYTYPE_4,
	ENEMYTYPE_MAX
}ENEMYTYPE;

typedef enum
{
	ENEMYANIM_NEUTRAL = 0,
	ENEMYANIM_WALK,
	ENEMYANIM_ATTACK,
	ENEMYANIM_MAX
}ENEMYANIM;

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
	D3DXVECTOR3					vtxMin;							// 頂点の最小値
	D3DXVECTOR3					vtxMax;							// 頂点の最大値
	int							nCntTimerand;					// カウンター
	int							nCntTime;						// カウンター
	int							nMaxPartsNum;					// パーツ数
	XENEMYMODEL					aModel[MAX_XENEMYPARTS];		// パーツ情報
	int							nMaxAnimNum;					// アニメーション数
	int							nAnimChange;					// アニメーションチェンジ
	int							nAnimloop[MAX_XENEMYANIM];		// ループ
	int							nAnimkey[MAX_XENEMYANIM];		// キー数
	int							nAnimfram[MAX_XENEMYANIM][MAX_XENEMYANIMKEY];		// フレーム数
	ENEMYANIM					nAnimnow;						// 現在アニメーション
	int							nAnimkeynow;					// 現在キー
	int							nIdxMark;						// 使用マーク番号
	bool						bIdxMarkUse;					// 使用マーク
	bool						bAttack;						// 使用しているか
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

void SetXEnemy(ENEMYTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 Initrot, D3DXVECTOR3 rot);				// 敵設定処理
XENEMY *GetXEnemy(void);			// 敵取得処理
bool CollisionXEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
void DeleteXEnemy(void);
#endif
