//=========================================================================================================================
//
// 壁処理 [meshwall.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef struct
{
	D3DXVECTOR3	pos;			// 壁の位置
	D3DXVECTOR3	rot;			// 壁の向き
	D3DXVECTOR3	num;			// 壁の数
	D3DXVECTOR3	fRadius;		// 壁の半径
	D3DXVECTOR3	tex;			// 壁のtex
	int			MaxVtx;			// 頂点の最大数
	int			MaxIdx;			// インデックスの最大数
	int			MaxPolygon;		// ポリゴンの最大数
	D3DXMATRIX	mtxWorld;		// ワールドマトリックス
	int			nTexNum;
	bool		bUse;			// 使用しているか
}MeshWall;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitMeshWall(void);		// 壁の初期化処理
void UninitMeshWall(void);		// 壁の終了処理
void UpdateMeshWall(void);		// 壁の更新処理
void DrawMeshWall(void);		// 壁の描画処理

void SetMeshWall(int nTexNum, D3DXCOLOR col);	// 壁の設定処理
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, D3DXVECTOR3 fRadius);		// 壁の衝突処理
void DeleteMeshWall(void);

#endif
