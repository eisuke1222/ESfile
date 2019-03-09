//=========================================================================================================================
//
// オブジェクトの処理 [billboardobject.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "billboardobject.h"	// オブジェクト
#include "shadow.h"				// 影
#include "xplayer.h"			// プレイヤー
#include "score.h"				// スコア
#include "sound.h"				// サウンド
#include "input.h"				// 入力

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_BILLBOARDOBJECT					(256)									// オブジェクトの最大数
#define BILLBOARDOBJECT_TEXTURE_UV_U		(1.0f)									// テクスチャアニメーションU範囲
#define BILLBOARDOBJECT_TEXTURE_UV_V		(1.0f)									// テクスチャアニメーションV範囲

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void MakeVertexBillboardObject(LPDIRECT3DDEVICE9 pDevice);							// 頂点情報の設定

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9							g_pTextureBillboardObject[BILLBOARDTEXTURE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9						g_pVtxBuffBillboardObject = NULL;		// 頂点バッファへのポインタ
BillboardObject								g_BillboardObject[MAX_BILLBOARDOBJECT];	// オブジェクトの情報

//=========================================================================================================================
// オブジェクトの初期化処理
//=========================================================================================================================
void InitBillboardObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntBillboardObject;

	// 頂点情報の作成
	MakeVertexBillboardObject(pDevice);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// カウント
		g_BillboardObject[nCntBillboardObject].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].fTex = 0.0f;
		g_BillboardObject[nCntBillboardObject].fInitRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].nTex = 0;
		g_BillboardObject[nCntBillboardObject].nIdxShadow = 0;
		g_BillboardObject[nCntBillboardObject].bUse = false;
	}
}

//=========================================================================================================================
// オブジェクトの終了処理
//=========================================================================================================================
void UninitBillboardObject(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < BILLBOARDTEXTURE_MAX; nCntTex++)
	{// テクスチャカウント
		if (g_pTextureBillboardObject[nCntTex] != NULL)
		{
			g_pTextureBillboardObject[nCntTex]->Release();
			g_pTextureBillboardObject[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBillboardObject != NULL)
	{
		g_pVtxBuffBillboardObject->Release();
		g_pVtxBuffBillboardObject = NULL;
	}
}

//=========================================================================================================================
// オブジェクトの更新処理
//=========================================================================================================================
void UpdateBillboardObject(void)
{
	int nCntBillboardObject;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// カウント
		if (g_BillboardObject[nCntBillboardObject].bUse == true)
		{// 使用していたら
			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_3)
			{// 魔法陣
				if (g_BillboardObject[nCntBillboardObject].fRadius.x != 150 &&
					g_BillboardObject[nCntBillboardObject].fRadius.z != 150)
				{
					g_BillboardObject[nCntBillboardObject].fRadius.x += 15;
					g_BillboardObject[nCntBillboardObject].fRadius.z += 15;
				}
			}
			else if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_MAP_WALL)
			{// 壁
				g_BillboardObject[nCntBillboardObject].fTex -= 0.002f;
			}
			else if (
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE)
			{// アイテム
				g_BillboardObject[nCntBillboardObject].rot.y += 0.03f;
			}

			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD)
			{// 剣
				g_BillboardObject[nCntBillboardObject].posOld = g_BillboardObject[nCntBillboardObject].pos;

				g_BillboardObject[nCntBillboardObject].pos += g_BillboardObject[nCntBillboardObject].move;
			}

			if (g_BillboardObject[nCntBillboardObject].pos.x > 1750.0f || g_BillboardObject[nCntBillboardObject].pos.x < -1750.0f ||
				g_BillboardObject[nCntBillboardObject].pos.z > 1750.0f || g_BillboardObject[nCntBillboardObject].pos.z < -1750.0f)
			{// 範囲チェック
				DeleteBillboardObject(nCntBillboardObject);
			}

			// 頂点情報の設定
			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_3)
			{// 魔法陣
				pVtx[nCntBillboardObject * 4].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 1].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, -g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 3].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, -g_BillboardObject[nCntBillboardObject].fRadius.z);
			}
			else if (g_BillboardObject[nCntBillboardObject].nTex != BILLBOARDTEXTURE_HP)
			{// 体力
				pVtx[nCntBillboardObject * 4].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
				pVtx[nCntBillboardObject * 4 + 1].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
				pVtx[nCntBillboardObject * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
				pVtx[nCntBillboardObject * 4 + 3].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
			}
			// テクスチャの設定
			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_MAP_WALL)
			{// 壁
				pVtx[nCntBillboardObject * 4].tex = D3DXVECTOR2(g_BillboardObject[nCntBillboardObject].fTex, 0.0f);
				pVtx[nCntBillboardObject * 4 + 1].tex = D3DXVECTOR2(g_BillboardObject[nCntBillboardObject].fTex + 1.0f, 0.0f);
				pVtx[nCntBillboardObject * 4 + 2].tex = D3DXVECTOR2(g_BillboardObject[nCntBillboardObject].fTex, 1.0f);
				pVtx[nCntBillboardObject * 4 + 3].tex = D3DXVECTOR2(g_BillboardObject[nCntBillboardObject].fTex + 1.0f, 1.0f);
			}
			else
			{// その他
				pVtx[nCntBillboardObject * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBillboardObject * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[nCntBillboardObject * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[nCntBillboardObject * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboardObject->Unlock();
}

//=========================================================================================================================
// オブジェクトの描画処理
//=========================================================================================================================
void DrawBillboardObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	int nCntBillboardObject;

	// アルファテストの設定
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Zライトの設定
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// カウント
		if (g_BillboardObject[nCntBillboardObject].bUse == true)
		{// 使用していたら
			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ROCKON ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD)
			{// ロックオン、斬撃
				// アルファテストの設定
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			}
			else if (
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE)
			{// アイテム
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// 両面
			}

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_BillboardObject[nCntBillboardObject].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD)
			{
				// 回転を反映
				D3DXMatrixRotationYawPitchRoll
				(
					&mtxRot,
					g_BillboardObject[nCntBillboardObject].rot.y,
					g_BillboardObject[nCntBillboardObject].rot.x,
					g_BillboardObject[nCntBillboardObject].rot.z
				);

				D3DXMatrixMultiply
				(
					&g_BillboardObject[nCntBillboardObject].mtxWorld,
					&g_BillboardObject[nCntBillboardObject].mtxWorld,
					&mtxRot
				);
			}

			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ROCKON ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_HP ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD)
			{
				// 逆行列
				g_BillboardObject[nCntBillboardObject].mtxWorld._11 = mtxView._11;
				g_BillboardObject[nCntBillboardObject].mtxWorld._12 = mtxView._21;
				g_BillboardObject[nCntBillboardObject].mtxWorld._13 = mtxView._31;
				g_BillboardObject[nCntBillboardObject].mtxWorld._21 = mtxView._12;
				g_BillboardObject[nCntBillboardObject].mtxWorld._22 = mtxView._22;
				g_BillboardObject[nCntBillboardObject].mtxWorld._23 = mtxView._32;
				g_BillboardObject[nCntBillboardObject].mtxWorld._31 = mtxView._13;
				g_BillboardObject[nCntBillboardObject].mtxWorld._32 = mtxView._23;
				g_BillboardObject[nCntBillboardObject].mtxWorld._33 = mtxView._33;
			}

			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_BillboardObject[nCntBillboardObject].pos.x,
				g_BillboardObject[nCntBillboardObject].pos.y,
				g_BillboardObject[nCntBillboardObject].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_BillboardObject[nCntBillboardObject].mtxWorld,
				&g_BillboardObject[nCntBillboardObject].mtxWorld,
				&mtxTrans
			);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_BillboardObject[nCntBillboardObject].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource
			(
				0,
				g_pVtxBuffBillboardObject,
				0,
				sizeof(VERTEX_3D)
			);

			// 頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャ設定
			if (g_BillboardObject[nCntBillboardObject].nTex >= BILLBOARDTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureBillboardObject[g_BillboardObject[nCntBillboardObject].nTex]);
			}

			// プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboardObject * 4, 2);

			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ROCKON ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD)
			{
				// アルファテストを無効
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
			else if (
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE)
			{
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
			}
		}
	}

	// Zライトの無効
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	// アルファテストを無効
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=========================================================================================================================
// オブジェクトの設定処理
//=========================================================================================================================
void SetBillboardObject(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, D3DXVECTOR3 fRadius, int nTex)
{
	int nCntBillboardObject;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// カウント
		if (g_BillboardObject[nCntBillboardObject].bUse == false)
		{// 使用していなかったら
			g_BillboardObject[nCntBillboardObject].pos = pos;
			g_BillboardObject[nCntBillboardObject].move = move;
			g_BillboardObject[nCntBillboardObject].rot = rot;
			g_BillboardObject[nCntBillboardObject].col = col;
			g_BillboardObject[nCntBillboardObject].fRadius = fRadius;
			g_BillboardObject[nCntBillboardObject].fTex = 0.0f;
			g_BillboardObject[nCntBillboardObject].nTex = nTex;
			g_BillboardObject[nCntBillboardObject].bUse = true;

			if (
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE)
			{// アイテム
				g_BillboardObject[nCntBillboardObject].nIdxShadow = SetShadow(g_BillboardObject[nCntBillboardObject].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 30.0f));
			}

			// 頂点情報の設定
			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_3)
			{// 魔法陣
				pVtx[nCntBillboardObject * 4].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 1].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, -g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 3].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, -g_BillboardObject[nCntBillboardObject].fRadius.z);
			}
			else
			{
				pVtx[nCntBillboardObject * 4].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
				pVtx[nCntBillboardObject * 4 + 1].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
				pVtx[nCntBillboardObject * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
				pVtx[nCntBillboardObject * 4 + 3].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
			}
			// 頂点カラーの設定
			pVtx[nCntBillboardObject * 4].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 1].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 2].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 3].col = g_BillboardObject[nCntBillboardObject].col;
			// テクスチャの設定
			pVtx[nCntBillboardObject * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCntBillboardObject * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboardObject->Unlock();
}
int SetHPNum(D3DXVECTOR3 pos, D3DXVECTOR3 fRadius)
{// HP
	int nCntBillboardObject;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// カウント
		if (g_BillboardObject[nCntBillboardObject].bUse == false)
		{// 使用していなかったら
			g_BillboardObject[nCntBillboardObject].pos = pos;
			g_BillboardObject[nCntBillboardObject].fRadius = fRadius;
			g_BillboardObject[nCntBillboardObject].fInitRadius = fRadius * 2;

			g_BillboardObject[nCntBillboardObject].nTex = BILLBOARDTEXTURE_HP;
			g_BillboardObject[nCntBillboardObject].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_BillboardObject[nCntBillboardObject].bUse = true;

			// 頂点情報の設定
			pVtx[nCntBillboardObject * 4].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
			pVtx[nCntBillboardObject * 4 + 1].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
			pVtx[nCntBillboardObject * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 3].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
			// 頂点カラーの設定
			pVtx[nCntBillboardObject * 4].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 1].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 2].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 3].col = g_BillboardObject[nCntBillboardObject].col;
			// テクスチャの設定
			pVtx[nCntBillboardObject * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCntBillboardObject * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//g_BillboardObject[nCntBillboardObject].nIdxShadow = SetShadow(g_BillboardObject[nCntBillboardObject].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 50.0f));

			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboardObject->Unlock();

	return nCntBillboardObject;
}

//=========================================================================================================================
// ロックオンの位置更新処理
//=========================================================================================================================
void SetPosionRockOn(D3DXVECTOR3 pos)
{
	int nCntBillboardObject;

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// カウント
		if (g_BillboardObject[nCntBillboardObject].bUse == true && g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ROCKON)
		{// ロックオンが使用されていたら
			g_BillboardObject[nCntBillboardObject].pos = pos;
		}
	}
}

//=========================================================================================================================
// HPの位置更新処理
//=========================================================================================================================
void SetPosionHP(D3DXVECTOR3 pos, int nBillBoardHP)
{
	if (g_BillboardObject[nBillBoardHP].bUse == true && g_BillboardObject[nBillBoardHP].nTex == BILLBOARDTEXTURE_HP)
	{
		g_BillboardObject[nBillBoardHP].pos = pos;
	}
}

//=========================================================================================================================
// HPの更新処理
//=========================================================================================================================
void SetUpdateHP(int nBillBoardHP, int nInitHP, int nNowHP)
{
	float fHP;
	float fHPRadius;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if (g_BillboardObject[nBillBoardHP].bUse == true)
	{
		// HP割合
		fHP = ((float)nNowHP / (float)nInitHP);

		// HPサイズ
		fHPRadius = g_BillboardObject[nBillBoardHP].fInitRadius.x * fHP;

		// 頂点情報の設定
		pVtx[nBillBoardHP * 4].pos = D3DXVECTOR3(-g_BillboardObject[nBillBoardHP].fRadius.x, g_BillboardObject[nBillBoardHP].fRadius.y, 0.0f);
		pVtx[nBillBoardHP * 4 + 1].pos = D3DXVECTOR3(fHPRadius - g_BillboardObject[nBillBoardHP].fRadius.x, g_BillboardObject[nBillBoardHP].fRadius.y, 0.0f);
		pVtx[nBillBoardHP * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nBillBoardHP].fRadius.x, 0.0f, 0.0f);
		pVtx[nBillBoardHP * 4 + 3].pos = D3DXVECTOR3(fHPRadius - g_BillboardObject[nBillBoardHP].fRadius.x, 0.0f, 0.0f);
		// テクスチャの設定
		pVtx[nBillBoardHP * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nBillBoardHP * 4 + 1].tex = D3DXVECTOR2(fHP, 0.0f);
		pVtx[nBillBoardHP * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nBillBoardHP * 4 + 3].tex = D3DXVECTOR2(fHP, 1.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboardObject->Unlock();
}

//=========================================================================================================================
// アイテムの取得判定処理
//=========================================================================================================================
bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMax, D3DXVECTOR3 *vtxMin)
{
	int nCntBillboardObject;
	bool bLand = false;

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// カウント
		if (g_BillboardObject[nCntBillboardObject].bUse == true && (
			g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
			g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
			g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
			g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE))
		{// アイテムのみ
			if (g_BillboardObject[nCntBillboardObject].pos.x < pPos->x + vtxMax->x && g_BillboardObject[nCntBillboardObject].pos.x > pPos->x + vtxMin->x &&
				g_BillboardObject[nCntBillboardObject].pos.z < pPos->z + vtxMax->z && g_BillboardObject[nCntBillboardObject].pos.z > pPos->z + vtxMin->z)
			{// 当たった
				bLand = true;

				AddScore(SCORETYPE_SCORE, 300);

				PlaySound(SOUND_LABEL_SE_ITEM);

				if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1)
				{
					SetHitItem(0);
				}
				else if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2)
				{
					SetHitItem(1);
				}
				else if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3)
				{
					SetHitItem(2);
				}
				else if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE)
				{
					SetHitItem(3);
				}

				DeleteBillboardObject(nCntBillboardObject);
				DeleteShadow(g_BillboardObject[nCntBillboardObject].nIdxShadow);

				break;
			}
		}
	}

	return bLand;
}
bool CollisionSword(D3DXVECTOR3 *pPos)
{
	int nCntBillboardObject;
	bool bLand = false;

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// カウント
		if (g_BillboardObject[nCntBillboardObject].bUse == true && (
			g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD))
		{// 斬撃が使用されていたら
			if (g_BillboardObject[nCntBillboardObject].pos.x - g_BillboardObject[nCntBillboardObject].fRadius.x < pPos->x && g_BillboardObject[nCntBillboardObject].pos.x + g_BillboardObject[nCntBillboardObject].fRadius.x > pPos->x)
			{
				if (g_BillboardObject[nCntBillboardObject].posOld.z > pPos->z && pPos->z > g_BillboardObject[nCntBillboardObject].pos.z)
				{
					bLand = true;

					break;
				}
				else if (g_BillboardObject[nCntBillboardObject].posOld.z < pPos->z && pPos->z < g_BillboardObject[nCntBillboardObject].pos.z)
				{
					bLand = true;

					break;
				}
			}

			if (g_BillboardObject[nCntBillboardObject].pos.z - g_BillboardObject[nCntBillboardObject].fRadius.x < pPos->z && g_BillboardObject[nCntBillboardObject].pos.z + g_BillboardObject[nCntBillboardObject].fRadius.x > pPos->z)
			{
				if (g_BillboardObject[nCntBillboardObject].posOld.x > pPos->x && pPos->x > g_BillboardObject[nCntBillboardObject].pos.x)
				{
					bLand = true;

					break;
				}
				else if (g_BillboardObject[nCntBillboardObject].posOld.x < pPos->x && pPos->x < g_BillboardObject[nCntBillboardObject].pos.x)
				{
					bLand = true;

					break;
				}
			}
		}
	}

	return bLand;
}


//=========================================================================================================================
// オブジェクトの破棄処理
//=========================================================================================================================
void DeleteBillboardObject(int nCntBillboardObject)
{// １つ
	if (g_BillboardObject[nCntBillboardObject].bUse == true)
	{
		g_BillboardObject[nCntBillboardObject].fTex = 0.0f;
		g_BillboardObject[nCntBillboardObject].bUse = false;
		//DeleteShadow(g_BillboardObject[nCntBillboardObject].nIdxShadow);
	}
}
void DeleteBillboardObject(BILLBOARDTEXTURE nTex)
{// テクスチャ
	int nCntBillboardObject;

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{
		if (g_BillboardObject[nCntBillboardObject].bUse == true && g_BillboardObject[nCntBillboardObject].nTex == nTex)
		{
			g_BillboardObject[nCntBillboardObject].fTex = 0.0f;
			g_BillboardObject[nCntBillboardObject].bUse = false;
			//DeleteShadow(g_BillboardObject[nCntBillboardObject].nIdxShadow);
		}
	}
}
void DeleteBillboardObject(void)
{// 全て
	int nCntBillboardObject;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// カウント
		if (g_BillboardObject[nCntBillboardObject].bUse == true)
		{// 使用していたら
			g_BillboardObject[nCntBillboardObject].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].fInitRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].nTex = 0;
			g_BillboardObject[nCntBillboardObject].nIdxShadow = 0;
			g_BillboardObject[nCntBillboardObject].bUse = false;

			// テクスチャの設定
			pVtx[nCntBillboardObject * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCntBillboardObject * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboardObject->Unlock();
}

//=========================================================================================================================
// 頂点情報の作成
//=========================================================================================================================
void MakeVertexBillboardObject(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntBillboardObject;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME1, &g_pTextureBillboardObject[0]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME2, &g_pTextureBillboardObject[1]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME3, &g_pTextureBillboardObject[2]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME4, &g_pTextureBillboardObject[3]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME5, &g_pTextureBillboardObject[4]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME6, &g_pTextureBillboardObject[5]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME7, &g_pTextureBillboardObject[6]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME8, &g_pTextureBillboardObject[7]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME9, &g_pTextureBillboardObject[8]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME10, &g_pTextureBillboardObject[9]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME11, &g_pTextureBillboardObject[10]);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARDOBJECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboardObject,
		NULL
	);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboardObject->Unlock();
}

