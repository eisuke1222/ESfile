//=========================================================================================================================
//
// エフェクトの処理 [billboardeffect.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include <time.h>				// time
#include "billboardeffect.h"	// エフェクト
#include "xplayer.h"			// プレイヤー
#include "xenemy.h"				// 敵
#include "shadow.h"				// 影
#include "input.h"				// 入力

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_BILLBOARDEFFECT					(1024)									// エフェクトの最大数
#define BILLBOARDEFFECT_TEXTURE_NAME1		"data\\TEXTURE\\shadow000.jpg"			// 読み込むテクスチャファイル1
#define BILLBOARDEFFECT_TEXTURE_UV_U		(1.0f)									// テクスチャアニメーションU範囲
#define BILLBOARDEFFECT_TEXTURE_UV_V		(1.0f)									// テクスチャアニメーションV範囲
#define BILLBOARDEFFECT_ALPHASUBTRACT		(0.02f)									// αの減算値

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void MakeVertexBillboardEffect(LPDIRECT3DDEVICE9 pDevice);							// 頂点情報の設定

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9						g_pTextureBillboardEffect = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9					g_pVtxBuffBillboardEffect = NULL;			// 頂点バッファへのポインタ
BillboardEffect							g_BillboardEffect[MAX_BILLBOARDEFFECT];		// エフェクトの情報
int										g_nXEnemy, g_nXEnemyNum;					// 敵種類、番号

//=========================================================================================================================
// エフェクト初期化処理
//=========================================================================================================================
void InitBillboardEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntBillboardEffect;

	g_nXEnemy = 0;
	g_nXEnemyNum = 0;

	// 頂点情報の作成
	MakeVertexBillboardEffect(pDevice);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// 初期化
		g_BillboardEffect[nCntBillboardEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_BillboardEffect[nCntBillboardEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
		g_BillboardEffect[nCntBillboardEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量
		g_BillboardEffect[nCntBillboardEffect].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 大きさ
		g_BillboardEffect[nCntBillboardEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // 色
		g_BillboardEffect[nCntBillboardEffect].coladown = 0.0f;							// 色減産量
		g_BillboardEffect[nCntBillboardEffect].nLife = 0;								// 体力
		g_BillboardEffect[nCntBillboardEffect].type = EFFECTTYPE_NONE;					// 種類
		g_BillboardEffect[nCntBillboardEffect].nIdxShadow = 0;							// 影番号
		g_BillboardEffect[nCntBillboardEffect].nDamage = 0;								// ダメージ
		g_BillboardEffect[nCntBillboardEffect].nType = 0;								// 弾種類
		g_BillboardEffect[nCntBillboardEffect].bUse = false;							// 使っているか
	}
}

//=========================================================================================================================
// エフェクト終了処理
//=========================================================================================================================
void UninitBillboardEffect(void)
{
	// テクスチャの破棄
	if (g_pTextureBillboardEffect != NULL)
	{
		g_pTextureBillboardEffect->Release();
		g_pTextureBillboardEffect = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBillboardEffect != NULL)
	{
		g_pVtxBuffBillboardEffect->Release();
		g_pVtxBuffBillboardEffect = NULL;
	}
}

//=========================================================================================================================
// エフェクト更新処理
//=========================================================================================================================
void UpdateBillboardEffect(void)
{
	// 変数宣言
	int nCntBillboardEffect;

	XPLAYER *pXPlayer = GetXPlayer();	// プレイヤー取得
	int nEnemyNum = GetXEnemyNum();		// 敵番号

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBillboardEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// エフェクトをカウント
		if (g_BillboardEffect[nCntBillboardEffect].bUse == true)
		{// 使用している場合
			if (g_BillboardEffect[nCntBillboardEffect].type == EFFECTTYPE_BULLET)
			{// 弾のエフェクト
				if (g_BillboardEffect[nCntBillboardEffect].nType == EFFECTBULLET_PLAYER2)
				{// プレイヤータイプ２
					g_BillboardEffect[nCntBillboardEffect].col.a -= g_BillboardEffect[nCntBillboardEffect].coladown;	// 透明度を下げる

					if (g_BillboardEffect[nCntBillboardEffect].col.a <= 0.0f)
					{// 透明になったら
						g_BillboardEffect[nCntBillboardEffect].col.a = 0.0f;
						DeleteBillboardEffect(nCntBillboardEffect);				// 消す
					}
				}
				else
				{// その他
					if (g_BillboardEffect[nCntBillboardEffect].nLife > 0)
					{// ライフがあったら
						g_BillboardEffect[nCntBillboardEffect].nLife--;			// ライフを減らす
					}
					else
					{// 透明にする
						g_BillboardEffect[nCntBillboardEffect].col.a -= g_BillboardEffect[nCntBillboardEffect].coladown;// 透明度を下げる

						if (g_BillboardEffect[nCntBillboardEffect].col.a <= 0.0f)
						{// 透明になったら
							g_BillboardEffect[nCntBillboardEffect].col.a = 0.0f;
							DeleteBillboardEffect(nCntBillboardEffect);			// 消す
						}
					}
				}

				if (g_BillboardEffect[nCntBillboardEffect].nType == EFFECTBULLET_PLAYER && pXPlayer->nWeapon_3Lv >= 6 && nEnemyNum > 0 && g_BillboardEffect[nCntBillboardEffect].nLife < 100)
				{// ホーミング
					D3DXVECTOR3 enemypos = GetRockOnXEnemy(1);
					// 追尾
					g_BillboardEffect[nCntBillboardEffect].move.x = sinf(atan2f(enemypos.x - g_BillboardEffect[nCntBillboardEffect].pos.x, enemypos.z - g_BillboardEffect[nCntBillboardEffect].pos.z)) * 5.0f;
					g_BillboardEffect[nCntBillboardEffect].move.z = cosf(atan2f(enemypos.x - g_BillboardEffect[nCntBillboardEffect].pos.x, enemypos.z - g_BillboardEffect[nCntBillboardEffect].pos.z)) * 5.0f;
				}

				g_BillboardEffect[nCntBillboardEffect].pos += g_BillboardEffect[nCntBillboardEffect].move;	// 移動処理

				if (g_BillboardEffect[nCntBillboardEffect].pos.x > 1750.0f || g_BillboardEffect[nCntBillboardEffect].pos.x < -1750.0f ||
					g_BillboardEffect[nCntBillboardEffect].pos.z > 1750.0f || g_BillboardEffect[nCntBillboardEffect].pos.z < -1750.0f)
				{// 範囲チェック
					DeleteBillboardEffect(nCntBillboardEffect);					// 消す
				}
			}
			else
			{// 弾以外
				g_BillboardEffect[nCntBillboardEffect].col.a -= g_BillboardEffect[nCntBillboardEffect].coladown;		// 透明度を下げる

				if (g_BillboardEffect[nCntBillboardEffect].col.a <= 0.0f)
				{// 透明になったら
					g_BillboardEffect[nCntBillboardEffect].col.a = 0.0f;
					DeleteBillboardEffect(nCntBillboardEffect);					// 消す
				}
				else if (g_BillboardEffect[nCntBillboardEffect].type == EFFECTTYPE_NORMAL)
				{// 通常
					//SetPositionShadow(g_BillboardEffect[nCntBillboardEffect].nIdxShadow, g_BillboardEffect[nCntBillboardEffect].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}

				if (g_BillboardEffect[nCntBillboardEffect].type == EFFECTTYPE_PARTICLE)
				{// パーティクル
					g_BillboardEffect[nCntBillboardEffect].pos += g_BillboardEffect[nCntBillboardEffect].move;
				}
			}

			// 頂点カラーの設定
			pVtx[nCntBillboardEffect * 4].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 1].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 2].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 3].col = g_BillboardEffect[nCntBillboardEffect].col;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboardEffect->Unlock();
}

//=========================================================================================================================
// エフェクト描画処理
//=========================================================================================================================
void DrawBillboardEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 計算用マトリックス
	int nCntBillboardEffect;

	// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zライトの設定
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{
		if (g_BillboardEffect[nCntBillboardEffect].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_BillboardEffect[nCntBillboardEffect].mtxWorld);

			// ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// 逆行列
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._11 = mtxView._11;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._12 = mtxView._21;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._13 = mtxView._31;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._21 = mtxView._12;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._22 = mtxView._22;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._23 = mtxView._32;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._31 = mtxView._13;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._32 = mtxView._23;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._33 = mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_BillboardEffect[nCntBillboardEffect].pos.x,
				g_BillboardEffect[nCntBillboardEffect].pos.y,
				g_BillboardEffect[nCntBillboardEffect].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_BillboardEffect[nCntBillboardEffect].mtxWorld,
				&g_BillboardEffect[nCntBillboardEffect].mtxWorld,
				&mtxTrans
			);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_BillboardEffect[nCntBillboardEffect].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource
			(
				0,
				g_pVtxBuffBillboardEffect,
				0,
				sizeof(VERTEX_3D)
			);

			// 頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureBillboardEffect);

			// プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboardEffect * 4, 2);
		}
	}

	// Zライト
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=========================================================================================================================
// ノーマルエフェクトの設定処理
//=========================================================================================================================
void SetBillboardNormalEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown)
{
	int nCntBillboardEffect;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBillboardEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// カウント
		if (g_BillboardEffect[nCntBillboardEffect].bUse == false)
		{// 使用していなかったら
			g_BillboardEffect[nCntBillboardEffect].pos = pos;
			g_BillboardEffect[nCntBillboardEffect].fRadius = fRadius;
			g_BillboardEffect[nCntBillboardEffect].coladown = coladown;
			g_BillboardEffect[nCntBillboardEffect].col = col;
			g_BillboardEffect[nCntBillboardEffect].type = EFFECTTYPE_NORMAL;
			g_BillboardEffect[nCntBillboardEffect].bUse = true;

			// 頂点情報の設定
			pVtx[nCntBillboardEffect * 4].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 1].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 2].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 3].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			// 頂点カラーの設定
			pVtx[nCntBillboardEffect * 4].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 1].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 2].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 3].col = g_BillboardEffect[nCntBillboardEffect].col;

			//g_BillboardEffect[nCntBillboardEffect].nIdxShadow = SetShadow(g_BillboardEffect[nCntBillboardEffect].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 4.0f));

			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboardEffect->Unlock();
}

//=========================================================================================================================
// 弾エフェクトの設定処理
//=========================================================================================================================
void SetBillboardBulletEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown, int nLife, int nDamage, int nType)
{
	int nCntBillboardEffect;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロック
	g_pVtxBuffBillboardEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// カウント
		if (g_BillboardEffect[nCntBillboardEffect].bUse == false)
		{// 使用していなかったら
			g_BillboardEffect[nCntBillboardEffect].pos = pos;					// 位置
			g_BillboardEffect[nCntBillboardEffect].rot = rot;					// 向き
			g_BillboardEffect[nCntBillboardEffect].move = move;					// 移動量
			g_BillboardEffect[nCntBillboardEffect].fRadius = fRadius;			// 大きさ
			g_BillboardEffect[nCntBillboardEffect].coladown = coladown;			// 色減産量
			g_BillboardEffect[nCntBillboardEffect].col = col;					// 色
			g_BillboardEffect[nCntBillboardEffect].nLife = nLife;				// 体力
			g_BillboardEffect[nCntBillboardEffect].type = EFFECTTYPE_BULLET;	// 種類
			g_BillboardEffect[nCntBillboardEffect].nDamage = nDamage;			// ダメージ
			g_BillboardEffect[nCntBillboardEffect].nType = nType;				// 弾種類
			g_BillboardEffect[nCntBillboardEffect].bUse = true;					// 使用している

			// 頂点情報の設定
			pVtx[nCntBillboardEffect * 4].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 1].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 2].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 3].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			// 頂点カラーの設定
			pVtx[nCntBillboardEffect * 4].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 1].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 2].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 3].col = g_BillboardEffect[nCntBillboardEffect].col;

			//g_BillboardEffect[nCntBillboardEffect].nIdxShadow = SetShadow(g_BillboardEffect[nCntBillboardEffect].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 4.0f));

			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboardEffect->Unlock();
}

//=========================================================================================================================
// パーティクルエフェクトの設定処理
//=========================================================================================================================
void SetBillboardParticleEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown, int nMaxSet, EFFECTSTATE state)
{
	int nCntBillboardEffect, nCntSet = 0;

	//srand((unsigned int)time(0));

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBillboardEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// カウント
		if (g_BillboardEffect[nCntBillboardEffect].bUse == false)
		{// 使用していなかったら
			g_BillboardEffect[nCntBillboardEffect].pos = pos;
			g_BillboardEffect[nCntBillboardEffect].fRadius = fRadius;
			g_BillboardEffect[nCntBillboardEffect].col = col;
			g_BillboardEffect[nCntBillboardEffect].coladown = coladown;
			g_BillboardEffect[nCntBillboardEffect].type = EFFECTTYPE_PARTICLE;
			g_BillboardEffect[nCntBillboardEffect].bUse = true;

			// 移動ランダム化
			g_BillboardEffect[nCntBillboardEffect].move.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			g_BillboardEffect[nCntBillboardEffect].move.y = cosf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			g_BillboardEffect[nCntBillboardEffect].move.z = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);

			g_BillboardEffect[nCntBillboardEffect].move.y += 1;

			int nNumber = rand() % 3 + 1;

			// col値代入
			if (nNumber == 1) { g_BillboardEffect[nCntBillboardEffect].col = D3DXCOLOR(0.8f, 0.3f, 0.0f, 0.8f); }
			if (nNumber == 2) { g_BillboardEffect[nCntBillboardEffect].col = D3DXCOLOR(0.2f, 0.8f, 0.0f, 0.8f); }
			if (nNumber == 3) { g_BillboardEffect[nCntBillboardEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f); }

			int nAngle = (rand() % (int)fRadius.x + 1);
			g_BillboardEffect[nCntBillboardEffect].fRadius.x = (nAngle) * 1.0f;	// 半径設定
			g_BillboardEffect[nCntBillboardEffect].fRadius.y = (nAngle) * 1.0f;	// 半径設定

			// 頂点情報の設定
			pVtx[nCntBillboardEffect * 4].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 1].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 2].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 3].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			// 頂点カラーの設定
			pVtx[nCntBillboardEffect * 4].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 1].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 2].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 3].col = g_BillboardEffect[nCntBillboardEffect].col;

			// 影をセット
			//g_BillboardEffect[nCntBillboardEffect].nIdxShadow = SetShadow(g_BillboardEffect[nCntBillboardEffect].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 4.0f));

			if (state != EFFECTSTATE_LOOP)
			{// ループ以外
				nCntSet++;		// 個数をカウント
				if (nCntSet == nMaxSet)
				{// 個数が最大数になった場合
					break;
				}
			}
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBillboardEffect->Unlock();
}

//=========================================================================================================================
// エフェクトの敵判定番号取得処理
//=========================================================================================================================
void SetXEnemyCollNum(int nXEnemy, int nXEnemyNum)
{
	g_nXEnemy = nXEnemy;
	g_nXEnemyNum = nXEnemyNum;
}

//=========================================================================================================================
// エフェクトの衝突判定処理
//=========================================================================================================================
bool CollisionEffect(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMax, D3DXVECTOR3 *vtxMin, int nType)
{
	GAMESTATE game = GetGameState();
	XPLAYER *pXPlayer = GetXPlayer();

	int nCntBillboardEffect;
	bool bLand = false;

	int nType2 = nType;
	if (nType == EFFECTBULLET_PLAYER)
	{
		nType2 = EFFECTBULLET_PLAYER2;
	}

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// カウント
		if (g_BillboardEffect[nCntBillboardEffect].bUse == true && g_BillboardEffect[nCntBillboardEffect].type == EFFECTTYPE_BULLET && (g_BillboardEffect[nCntBillboardEffect].nType == nType || g_BillboardEffect[nCntBillboardEffect].nType == nType2))
		{// 使用している
 			if (g_BillboardEffect[nCntBillboardEffect].pos.x < pPos->x + vtxMax->x && g_BillboardEffect[nCntBillboardEffect].pos.x > pPos->x + vtxMin->x &&
				g_BillboardEffect[nCntBillboardEffect].pos.z < pPos->z + vtxMax->z && g_BillboardEffect[nCntBillboardEffect].pos.z > pPos->z + vtxMin->z)
			{// 当たったら

				if (game == GAMESTATE_PLAY)
				{// ゲームプレイ中なら
					if (g_BillboardEffect[nCntBillboardEffect].nType == EFFECTBULLET_PLAYER ||
						g_BillboardEffect[nCntBillboardEffect].nType == EFFECTBULLET_PLAYER2)
					{// プレイヤー
						HitXEnemy(g_nXEnemy, g_nXEnemyNum, g_BillboardEffect[nCntBillboardEffect].nDamage);
					}
					else if (g_BillboardEffect[nCntBillboardEffect].nType == EFFECTBULLET_ENEMY)
					{// 敵
						pXPlayer->nLife -= g_BillboardEffect[nCntBillboardEffect].nDamage;
					}

					// ノックバック
					pMove->x = sinf(g_BillboardEffect[nCntBillboardEffect].rot.y) * -5.0f;
					pMove->z = cosf(g_BillboardEffect[nCntBillboardEffect].rot.y) * -5.0f;
				}

				DeleteBillboardEffect(nCntBillboardEffect);
				bLand = true;
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// エフェクトの破棄処理
//=========================================================================================================================
void DeleteBillboardEffect(int nCntBillboardEffect)
{// 1つ
	if (g_BillboardEffect[nCntBillboardEffect].bUse == true)
	{// 使用していたら
		g_BillboardEffect[nCntBillboardEffect].bUse = false;

		DeleteShadow(g_BillboardEffect[nCntBillboardEffect].nIdxShadow);
	}
}
void DeleteBillboardEffect(void)
{// 全て
	int nCntBillboardEffect;

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// カウント
		if (g_BillboardEffect[nCntBillboardEffect].bUse == true)
		{// 使用していたら
			g_BillboardEffect[nCntBillboardEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardEffect[nCntBillboardEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardEffect[nCntBillboardEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardEffect[nCntBillboardEffect].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardEffect[nCntBillboardEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			g_BillboardEffect[nCntBillboardEffect].coladown = 0.0f;
			g_BillboardEffect[nCntBillboardEffect].nLife = 0;
			g_BillboardEffect[nCntBillboardEffect].type = EFFECTTYPE_NONE;
			g_BillboardEffect[nCntBillboardEffect].nIdxShadow = 0;
			g_BillboardEffect[nCntBillboardEffect].nDamage = 0;
			g_BillboardEffect[nCntBillboardEffect].nType = 0;
			g_BillboardEffect[nCntBillboardEffect].bUse = false;
		}
	}
}

//=========================================================================================================================
// 頂点情報の作成
//=========================================================================================================================
void MakeVertexBillboardEffect(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntBillboardEffect;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BILLBOARDEFFECT_TEXTURE_NAME1, &g_pTextureBillboardEffect);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARDEFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboardEffect,
		NULL
	);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBillboardEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
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
	g_pVtxBuffBillboardEffect->Unlock();
}

