//=========================================================================================================================
//
// 武器処理 [xplayer.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanfエラー解除
#include <stdio.h>					// stdio
#include "xweapon.h"
#include "xplayer.h"
#include "xenemy.h"
#include "xobject.h"
#include "shadow.h"
#include "billboardeffect.h"
#include "camera.h"
#include "fade.h"
#include "input.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
XWEAPONMODEL					g_XWeapon[MAX_XWEAPON];		// 武器武器情報
int								g_MaxXWeapon;				// 武器パーツ数
int								g_nCntTimeXWeapon;			// 武器カウンター

//=========================================================================================================================
// 武器の初期化処理
//=========================================================================================================================
void InitXWeapon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	XPLAYER *pXPlayer = GetXPlayer();
	FILE *pFile;
	int nCntXWeapon;

	g_MaxXWeapon = 0;		// 武器パーツ数
	g_nCntTimeXWeapon = 0;	// 武器カウンター

	// ファイル設定
	pFile = fopen(XWEAPON_TEXT_INFO, "r");

	// 武器テキストデータ読み込み
	if (pFile != NULL)
	{// ファイルが開けた場合
		fscanf(pFile, "%d", &g_MaxXWeapon);	// 読み込み

		for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
		{
			fscanf(pFile, "%s", &g_XWeapon[nCntXWeapon].FileName[0]);	// 読み込み
		}
		
		for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
		{
			fscanf(pFile, "%d", &g_XWeapon[nCntXWeapon].nIndexParent);	// 読み込み
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initpos.x);	// 読み込み
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initpos.y);	// 読み込み
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initpos.z);	// 読み込み
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initrot.x);	// 読み込み
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initrot.y);	// 読み込み
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initrot.z);	// 読み込み
		}

		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}

	// Xファイルの読み込み
	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		D3DXLoadMeshFromX
		(
			&g_XWeapon[nCntXWeapon].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_XWeapon[nCntXWeapon].pBuffMat,
			NULL,
			&g_XWeapon[nCntXWeapon].nNumMat,
			&g_XWeapon[nCntXWeapon].pMesh
		);
	}

	// 位置・向きの初期設定
	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		g_XWeapon[nCntXWeapon].pos = g_XWeapon[nCntXWeapon].Initpos;
		g_XWeapon[nCntXWeapon].rot = g_XWeapon[nCntXWeapon].Initrot;
		g_XWeapon[nCntXWeapon].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_XWeapon[nCntXWeapon].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		g_XWeapon[nCntXWeapon].nHitNum = 0;
		g_XWeapon[nCntXWeapon].nType = WEAPON_NONE;
		g_XWeapon[nCntXWeapon].bUse = false;
	}

	int nNumVtx;		// 頂点数
	int nIndexParent;	// 親番号
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファのポインタ

	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		// 頂点数の取得
		nNumVtx = g_XWeapon[nCntXWeapon].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_XWeapon[nCntXWeapon].pMesh->GetFVF());

		// 頂点バッファをロック
		g_XWeapon[nCntXWeapon].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入
			{
				if (vtx.x > g_XWeapon[nCntXWeapon].vtxMax.x)
				{
					g_XWeapon[nCntXWeapon].vtxMax.x = vtx.x;
				}
				if (vtx.y > g_XWeapon[nCntXWeapon].vtxMax.y)
				{
					g_XWeapon[nCntXWeapon].vtxMax.y = vtx.y;
				}
				if (vtx.z > g_XWeapon[nCntXWeapon].vtxMax.z)
				{
					g_XWeapon[nCntXWeapon].vtxMax.z = vtx.z;
				}

				if (vtx.x < g_XWeapon[nCntXWeapon].vtxMin.x)
				{
					g_XWeapon[nCntXWeapon].vtxMin.x = vtx.x;
				}
				if (vtx.y < g_XWeapon[nCntXWeapon].vtxMin.y)
				{
					g_XWeapon[nCntXWeapon].vtxMin.y = vtx.y;
				}
				if (vtx.z < g_XWeapon[nCntXWeapon].vtxMin.z)
				{
					g_XWeapon[nCntXWeapon].vtxMin.z = vtx.z;
				}

				if (g_XWeapon[nCntXWeapon].nIndexParent < 0)
				{
					nIndexParent = nCntXWeapon;
				}
			}

			pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
		}

		// 頂点バッファをアンロック
		g_XWeapon[nCntXWeapon].pMesh->UnlockVertexBuffer();
	}

	for (nCntXWeapon = g_MaxXWeapon; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		g_XWeapon[nCntXWeapon] = g_XWeapon[3];
	}
}

//=========================================================================================================================
// 武器の終了処理
//=========================================================================================================================
void UninitXWeapon(void)
{
	// 変数宣言
	int nCntXWeapon;

	// メッシュの開放
	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].pMesh != NULL)
		{
			g_XWeapon[nCntXWeapon].pMesh->Release();
			g_XWeapon[nCntXWeapon].pMesh = NULL;
		}
	}

	// マテリアルの開放
	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].pBuffMat != NULL)
		{
			g_XWeapon[nCntXWeapon].pBuffMat->Release();
			g_XWeapon[nCntXWeapon].pBuffMat = NULL;
		}
	}
}

//=========================================================================================================================
// 武器の更新処理
//=========================================================================================================================
void UpdateXWeapon(void)
{
	XPLAYER *pXPlayer = GetXPlayer();
	float fDiff = 0.0f;
	float frac = 0.0f;
	int nCntXWeapon;

	g_nCntTimeXWeapon++;

	for (nCntXWeapon = g_MaxXWeapon; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == true)
		{
			g_XWeapon[nCntXWeapon].pos += g_XWeapon[nCntXWeapon].move;	// 移動量代入

			SetBillboardNormalEffect(g_XWeapon[nCntXWeapon].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.5f, 1.5f, 0.0f), 0.08f);

			if (g_XWeapon[nCntXWeapon].pos.x > 1750.0f || g_XWeapon[nCntXWeapon].pos.x < -1750.0f ||
				g_XWeapon[nCntXWeapon].pos.z > 1750.0f || g_XWeapon[nCntXWeapon].pos.z < -1750.0f ||
				g_XWeapon[nCntXWeapon].pos.y < 0.0f)
			{// 当たり判定
				DeleteXWeapon(nCntXWeapon);
			}
		}
	}
}

//=========================================================================================================================
// 武器の描画処理
//=========================================================================================================================
void DrawXWeapon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	XPLAYER *pXPlayer = GetXPlayer();			// 
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;						// 
	int nCntXWeapon;

	for (nCntXWeapon = 0; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == true)
		{
			if (nCntXWeapon < g_MaxXWeapon)
			{
				mtxParent = pXPlayer->aModel[g_XWeapon[nCntXWeapon].nIndexParent].mtxWorld;
			}

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_XWeapon[nCntXWeapon].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_XWeapon[nCntXWeapon].rot.y,
				g_XWeapon[nCntXWeapon].rot.x,
				g_XWeapon[nCntXWeapon].rot.z
			);

			D3DXMatrixMultiply
			(
				&g_XWeapon[nCntXWeapon].mtxWorld,
				&g_XWeapon[nCntXWeapon].mtxWorld,
				&mtxRot
			);

			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_XWeapon[nCntXWeapon].pos.x,
				g_XWeapon[nCntXWeapon].pos.y,
				g_XWeapon[nCntXWeapon].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_XWeapon[nCntXWeapon].mtxWorld,
				&g_XWeapon[nCntXWeapon].mtxWorld,
				&mtxTrans
			);

			if (nCntXWeapon < g_MaxXWeapon)
			{
				D3DXMatrixMultiply
				(
					&g_XWeapon[nCntXWeapon].mtxWorld,
					&g_XWeapon[nCntXWeapon].mtxWorld,
					&mtxParent
				);
			}

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_XWeapon[nCntXWeapon].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pmat = (D3DXMATERIAL*)g_XWeapon[nCntXWeapon].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_XWeapon[nCntXWeapon].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

				// テクスチャ設定
				pDevice->SetTexture(0, NULL);

				// 武器(パーツ)の描画
				g_XWeapon[nCntXWeapon].pMesh->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=========================================================================================================================
// 武器の設定処理
//=========================================================================================================================
void SetXWeapon(WEAPONTYPE type)
{
	XPLAYER *pXPalyer = GetXPlayer();
	int nCntXWeapon;

	// 武器使用
	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		if (type == nCntXWeapon && type != WEAPONTYPE_NONE)
		{
			g_XWeapon[nCntXWeapon].bUse = true;
		}
		else
		{
			g_XWeapon[nCntXWeapon].bUse = false;
		}
	}

	// 弓矢
	if (type == WEAPONTYPE_BOW)
	{
		g_XWeapon[WEAPONTYPE_SAGITTAL].bUse = true;
	}

	pXPalyer->nWeapon = type;
}

//=========================================================================================================================
// 武器の取得処理
//=========================================================================================================================
WEAPONTYPE GetXWeapon(void)
{
	XPLAYER *pXPalyer = GetXPlayer();

	return pXPalyer->nWeapon;
}

//=========================================================================================================================
// 武器の発射処理
//=========================================================================================================================
void SeBulletWeapon(D3DXVECTOR3 move)
{// プレイヤー
	XPLAYER *pXPalyer = GetXPlayer();

	int nCntXWeapon;

	for (nCntXWeapon = g_MaxXWeapon; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == false)
		{// 使っている
			// 発射設定
			g_XWeapon[nCntXWeapon].pos = D3DXVECTOR3(g_XWeapon[3].mtxWorld._41, g_XWeapon[3].mtxWorld._42, g_XWeapon[3].mtxWorld._43);
			//g_XWeapon[nCntXWeapon].rot = D3DXVECTOR3(g_XWeapon[3].mtxWorld._32, g_XWeapon[3].mtxWorld._33, g_XWeapon[3].mtxWorld._34);
			g_XWeapon[nCntXWeapon].rot.y = pXPalyer->rot.y - (D3DX_PI / 2);
			g_XWeapon[nCntXWeapon].move = move;
			g_XWeapon[nCntXWeapon].nType = WEAPON_PLAYER;

			g_XWeapon[nCntXWeapon].bUse = true;

			break;
		}
	}
}
void SeBulletWeaponEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move)
{// 敵
	XPLAYER *pXPalyer = GetXPlayer();

	int nCntXWeapon;

	for (nCntXWeapon = g_MaxXWeapon; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == false)
		{// 使っている
			// 発射設定
			g_XWeapon[nCntXWeapon].pos = pos;
			//g_XWeapon[nCntXWeapon].rot = D3DXVECTOR3(g_XWeapon[3].mtxWorld._32, g_XWeapon[3].mtxWorld._33, g_XWeapon[3].mtxWorld._34);
			g_XWeapon[nCntXWeapon].rot.y = rot.y - (D3DX_PI / 2);
			g_XWeapon[nCntXWeapon].move = move;
			g_XWeapon[nCntXWeapon].nType = WEAPON_ENEMY;

			g_XWeapon[nCntXWeapon].bUse = true;

			break;
		}
	}
}

//=========================================================================================================================
// 武器の衝突判定処理
//=========================================================================================================================
bool CollisionXWeapon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	GAMESTATE game = GetGameState();
	XPLAYER *pXPlayer = GetXPlayer();
	int nCntXWeapon;
	bool bLand = false;

	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == true && 
			(
				(pXPlayer->nAnimnow == PLAYERANIM_SWORD_1 && pXPlayer->nAnimkeynow == 2) ||
				(pXPlayer->nAnimnow == PLAYERANIM_SWORD_2 && pXPlayer->nAnimkeynow == 1) ||
				(pXPlayer->nAnimnow == PLAYERANIM_SWORD_3 && pXPlayer->nAnimkeynow == 3)
			))
		{// 使用されている
			//if (pPos->y + vtxMax->y > pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMin.y && pPos->y + vtxMin->y < pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMax.y)
			{// X範囲確認
				if (pPos->x + vtxMax->x > g_XWeapon[nCntXWeapon].mtxWorld._41 - 5.0f && pPos->x + vtxMin->x < g_XWeapon[nCntXWeapon].mtxWorld._41 + 5.0f &&
					pPos->z + vtxMax->z > g_XWeapon[nCntXWeapon].mtxWorld._43 - 5.0f && pPos->z + vtxMin->z < g_XWeapon[nCntXWeapon].mtxWorld._43 + 5.0f)
				{// X範囲確認

					if (game == GAMESTATE_PLAY)
					{
						if (pXPlayer->nAnimnow == PLAYERANIM_SWORD_1 ||
							pXPlayer->nAnimnow == PLAYERANIM_SWORD_2)
						{
							pMove->x = sinf(pXPlayer->rot.y) * -5.0f;
							pMove->z = cosf(pXPlayer->rot.y) * -5.0f;
						}
						else if (pXPlayer->nAnimnow == PLAYERANIM_SWORD_3)
						{
							pMove->x = sinf(pXPlayer->rot.y) * -8.0f;
							pMove->z = cosf(pXPlayer->rot.y) * -8.0f;
						}
					}

					bLand = true;
				}
			}
		}
	}

	return bLand;
}
bool CollisionXWeaponBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax, int nType)
{
	GAMESTATE game = GetGameState();
	XPLAYER *pXPlayer = GetXPlayer();
	int nCntXWeapon;
	bool bLand = false;

	for (nCntXWeapon = g_MaxXWeapon; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == true && g_XWeapon[nCntXWeapon].nType == nType)
		{// 使用されている
		 //if (pPos->y + vtxMax->y > pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMin.y && pPos->y + vtxMin->y < pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMax.y)
			{// X範囲確認
				if (pPos->x + vtxMax->x > g_XWeapon[nCntXWeapon].pos.x && pPos->x + vtxMin->x < g_XWeapon[nCntXWeapon].pos.x &&
					pPos->z + vtxMax->z > g_XWeapon[nCntXWeapon].pos.z && pPos->z + vtxMin->z < g_XWeapon[nCntXWeapon].pos.z)
				{// X範囲確認

					g_XWeapon[nCntXWeapon].nHitNum++;

					if (game == GAMESTATE_PLAY)
					{
						pMove->x = sinf(g_XWeapon[nCntXWeapon].rot.y + (D3DX_PI / 2)) * -5.0f;
						pMove->z = cosf(g_XWeapon[nCntXWeapon].rot.y + (D3DX_PI / 2)) * -5.0f;
					}

					if (g_XWeapon[nCntXWeapon].nHitNum > 10)
					{
						DeleteXWeapon(nCntXWeapon);
					}

					bLand = true;
				}
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// 武器の破棄処理
//=========================================================================================================================
void DeleteXWeapon(int nCntXWeapon)
{
	if (g_XWeapon[nCntXWeapon].bUse == true)
	{
		g_XWeapon[nCntXWeapon].nHitNum = 0;
		g_XWeapon[nCntXWeapon].bUse = false;
	}
}
void DeleteXWeapon(void)
{
	int nCntXWeapon;

	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == true)
		{
			g_XWeapon[nCntXWeapon].nHitNum = 0;
			g_XWeapon[nCntXWeapon].nType = WEAPON_NONE;
			g_XWeapon[nCntXWeapon].bUse = false;
		}
	}
}