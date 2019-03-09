//=========================================================================================================================
//
// プレイヤー処理 [xplayer.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _XPLAYER_H_
#define _XPLAYER_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define XPLAYER_TEXT_NAME			(255)						
#define XPLAYER_TEXT_INFO			"data\\TEXT\\playermodeldata.txt"
#define MAX_XPLAYER					(20)					
#define MAX_XPLAYERANIM				(12)					
#define MAX_XPLAYERANIMKEY			(10)					

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// アニメーション
	PLAYERANIM_NEUTRAL = 0,
	PLAYERANIM_WALK,
	PLAYERANIM_SWORD_1,
	PLAYERANIM_SWORD_2,
	PLAYERANIM_SWORD_3,
	PLAYERANIM_BOW,
	PLAYERANIM_WAND,
	PLAYERANIM_MAX
}PLAYERANIM;

typedef enum
{// プレイヤー状態
	PLAYERSTATE_NONE = 0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_DIE,
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{// 武器種類
	WEAPONTYPE_SWORD = 0,
	WEAPONTYPE_BOW,
	WEAPONTYPE_WAND,
	WEAPONTYPE_SAGITTAL,
	WEAPONTYPE_NONE,
	WEAPONTYPE_MAX
}WEAPONTYPE;

typedef struct
{
	char						FileName[XPLAYER_TEXT_NAME];	// Xファイル名
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
	D3DXVECTOR3					posAnim[MAX_XPLAYERANIM][MAX_XPLAYERANIMKEY];		// アニメーションpos
	D3DXVECTOR3					rotAnim[MAX_XPLAYERANIM][MAX_XPLAYERANIMKEY];		// アニメーションrot
	D3DXVECTOR3					vtxMin;							// 頂点の最小値
	D3DXVECTOR3					vtxMax;							// 頂点の最大値
}XPLAYERMODEL;

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
	int							nMaxPartsNum;					// パーツ数
	XPLAYERMODEL				aModel[MAX_XPLAYER];			// パーツ情報
	int							nMaxAnimNum;					// アニメーション数
	int							nAnimloop[MAX_XPLAYERANIM];		// ループ
	int							nAnimkey[MAX_XPLAYERANIM];		// キー数
	int							nAnimBlend[MAX_XPLAYERANIM];	// ブレンド数
	int							nAnimfram[MAX_XPLAYERANIM][MAX_XPLAYERANIMKEY];		// フレーム数
	int							nAnimChange;					// アニメーションチェンジ
	PLAYERANIM					nAnimnow;						// 現在アニメーション
	int							nAnimkeynow;					// 現在キー
	int							nCntstate;						// 状態カウンタ
	PLAYERSTATE					state;							// 状態
	WEAPONTYPE					nWeapon;						// 現在武器
	int							nChargeNum;						// 溜め
	int							nLife;							// 体力
	int							nInitLife;						// 初期体力
	int							nWeapon_1Lv;
	int							nWeapon_2Lv;
	int							nWeapon_3Lv;
	bool						bAttack;						// 攻撃しているか
	bool						bNextAttack;					// 攻撃を続けるか
	int							nCntChargeAttack;				// 攻撃を続けるか
	bool						bChargeAttack;					// 攻撃を続けるか
	bool						bChargeOn;						// 攻撃を続けるか
	bool						bMove;							// 使用しているか
	bool						bUse;							// 使用しているか
}XPLAYER;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitXPlayer(void);				// プレイヤー初期化処理
void UninitXPlayer(void);			// プレイヤー終了処理
void UpdateXPlayer(void);			// プレイヤー更新処理
void DrawXPlayer(void);				// プレイヤー描画処理

void SetXPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);				// プレイヤー設定処理
XPLAYER *GetXPlayer(void);			// プレイヤー取得処理
void SetXPlayerHP(int nLife);
int GetXPlayerHP(void);
bool CollisionXPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
void SetHitItem(int nItem);
void SetWeaponLv(int nWeapon_1, int nWeapon_2, int nWeapon_3);
void SetMove(bool bMove);
void DeleteXPlayer(void);

#endif
