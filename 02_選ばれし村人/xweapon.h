//=========================================================================================================================
//
// 武器処理 [xplayer.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _XWEAPON_H_
#define _XWEAPON_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"
#include "xplayer.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define XWEAPON_TEXT_INFO		"data\\TEXT\\weapondata.txt"
#define MAX_XWEAPON				(32)					

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// 武器タイプ
	WEAPON_NONE = 0,
	WEAPON_PLAYER,
	WEAPON_ENEMY,
	WEAPON_MAX,
}WEAPON;

typedef struct
{
	char						FileName[XPLAYER_TEXT_NAME];	// Xファイル名
	LPD3DXMESH					pMesh = NULL;					// メッシュ情報へのポインタ
	LPD3DXBUFFER				pBuffMat = NULL;				// マテリアル情報へのポインタ
	DWORD						nNumMat = NULL;					// マテリアル情報の数
	D3DXMATRIX					mtxWorld;						// ワールドマトリックス
	int							nIndexParent;					// 親子関係
	D3DXVECTOR3					Initpos;						// 初期位置
	D3DXVECTOR3					pos;							// 現在位置
	D3DXVECTOR3					posOld;							// 前回位置
	D3DXVECTOR3					move;							// 移動
	D3DXVECTOR3					Initrot;						// 初期向き
	D3DXVECTOR3					rotDest;						// 目標向き
	D3DXVECTOR3					rot;							// 現在向き
	D3DXVECTOR3					vtxMin;							// 頂点の最小値
	D3DXVECTOR3					vtxMax;							// 頂点の最大値
	int							nHitNum;						// 当たった回数
	WEAPON						nType;
	bool						bUse;							// 使用しているか
}XWEAPONMODEL;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitXWeapon(void);			// 武器初期化処理
void UninitXWeapon(void);		// 武器終了処理
void UpdateXWeapon(void);		// 武器更新処理
void DrawXWeapon(void);			// 武器描画処理

void SetXWeapon(WEAPONTYPE type);	// 武器設定処理
WEAPONTYPE GetXWeapon(void);
void SeBulletWeapon(D3DXVECTOR3 move);
void SeBulletWeaponEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move);
bool CollisionXWeapon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
bool CollisionXWeaponBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax, int nType);
void DeleteXWeapon(int nCntXWeapon);
void DeleteXWeapon(void);

#endif
