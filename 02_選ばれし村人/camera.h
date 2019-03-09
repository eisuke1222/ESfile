//=========================================================================================================================
//
// カメラ処理 [camera.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// カメラの構造体
//*************************************************************************************************************************
typedef enum
{
	CAMERATYPE_NONE = 0,				
	CAMERATYPE_TITLE,				// タイトル
	CAMERATYPE_TITLE_GAME,			// タイトル
	CAMERATYPE_GAME,				// ゲーム
	CAMERATYPE_RESULT,				// リザルト
	CAMERATYPE_RANKING,				// ランキング
	CAMERATYPE_MAX,
}CAMERATYPE;

typedef struct
{
	D3DXVECTOR3 posV;				// 視点
	D3DXVECTOR3 posVDest;			// 目標視点
	D3DXVECTOR3 posR;				// 注視点
	D3DXVECTOR3 posRDest;			// 目標注視点
	D3DXVECTOR3 vecU;				// 上方向ベクトル
	D3DXVECTOR3	rot;				// 角度
	float		fDistance;			// 距離
	float		fResetRot;			// リセット向き
	float		fResetRotDest;		// 目標リセット向き
	bool		bReset;				// リセット
	int			nCntRockOn;			// ロックオンカウント
	float		fRockOnRot;			// ロックオン向き
	float		fRockOnRotDest;		// 目標ロックオン向き
	bool		bRockOn;			// ロックオンしてるか
	bool		bRockOnMove;		// ロックオンしてるか
	D3DXMATRIX	mtxProjection;		// プロジェクションマトリックス
	D3DXMATRIX	mtxView;			// ビューマトリックス
	CAMERATYPE type;				// 種類
}Camera;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitCamera(void);				// カメラ初期化処理
void UninitCamera(void);			// カメラ終了処理
void UpdateCamera(void);			// カメラ更新処理
void SetUpCamera(void);				// カメラ設定処理

void SetCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR, CAMERATYPE type);	// カメラ設定処理
Camera *GetCamera(void);			// カメラ取得処理
void SetCancelRockOn(void);

#endif
