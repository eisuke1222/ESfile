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
#define MAX_XPLAYERANIMKEY			(20)					

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{
	PLAYERANIM_NEUTRAL = 0,
	PLAYERANIM_WALK,
	PLAYERANIM_RUN,
	PLAYERANIM_ATTACK,
	PLAYERANIM_MAX
}PLAYERANIM;

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
	D3DXVECTOR3					vtxMin;							// 頂点の最小値
	D3DXVECTOR3					vtxMax;							// 頂点の最大値
	int							nMaxPartsNum;					// パーツ数
	XPLAYERMODEL				aModel[MAX_XPLAYER];			// パーツ情報
	int							nMaxAnimNum;					// アニメーション数
	int							nAnimChange;					// アニメーションチェンジ
	int							nAnimloop[MAX_XPLAYERANIM];		// ループ
	int							nAnimkey[MAX_XPLAYERANIM];		// キー数
	int							nAnimBlend[MAX_XPLAYERANIM];		// ブレンドフレーム数
	int							nAnimfram[MAX_XPLAYERANIM][MAX_XPLAYERANIMKEY];		// フレーム数
	PLAYERANIM					nAnimnow;						// 現在アニメーション
	int							nAnimkeynow;					// 現在キー
	bool						bAttack;						// 使用しているか
	bool						bUse;							// 使用しているか
}XPLAYER;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitXPlayer(void);				// プレイヤー初期化処理
void UninitXPlayer(void);			// プレイヤー終了処理
void UpdateXPlayer(void);			// プレイヤー更新処理
void DrawXPlayer(void);				// プレイヤー描画処理

void SetXPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// プレイヤー設定処理
XPLAYER *GetXPlayer(void);			// プレイヤー取得処理
bool CollisionXPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
void DeleteXPlayer(void);

#endif
