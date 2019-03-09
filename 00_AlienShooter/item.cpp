//=========================================================================================================================
// 
// アイテム処理 [item.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "item.h"			// アイテム
#include "sound.h"			// サウンド
#include "input.h"			// 入力処理

#include "player.h"			// プレイヤー

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_ITEM				(128)								// アイテムの最大数
#define ITEM_TEXTURE_NAME		"data\\TEXTURE\\item_all000.png"		// 読み込むテクスチャファイル
#define ITEM_POS_X				(0)									// アイテムの左上X座標
#define ITEM_POS_Y				(0)									// アイテムの左上Y座標
#define ITEM_WIDTH				(SCREEN_WIDTH)						// アイテムの幅
#define ITEM_HEIGHT				(SCREEN_HEIGHT)						// アイテムの高
#define ITEM_TEXTURE_UV_U		(0.25f)								// テクスチャアニメーションU範囲
#define ITEM_TEXTURE_UV_V		(1.0f)								// テクスチャアニメーションV範囲

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureItem = NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffItem = NULL;					// 頂点バッファへのポインタ
ItemState					g_ItemState[MAX_ITEM];					// アイテム情報
int							g_nCntPowerHit;

//=========================================================================================================================
// アイテム初期化処理
//=========================================================================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// アイテムをカウント
		g_ItemState[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_ItemState[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		g_ItemState[nCntItem].bDisp = false;							// 表示
	}
	g_nCntPowerHit = 0;		// ヒットカウント

	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(// アイテム
		pDevice,
		ITEM_TEXTURE_NAME,
		&g_pTextureItem
	);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffItem->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// アイテムをカウント
		// 頂点設定
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		// 頂点カラー設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(ITEM_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, ITEM_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(ITEM_TEXTURE_UV_U, ITEM_TEXTURE_UV_V);
	
		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();
}

//=========================================================================================================================
// アイテム終了処理
//=========================================================================================================================
void UninitItem(void)
{
	// テクスチャの破棄
	if (g_pTextureItem != NULL)
	{// NULL以外の場合
		g_pTextureItem->Release();		// 開放
		g_pTextureItem = NULL;			// NULLへ
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffItem != NULL)
	{// NULL以外の場合
		g_pVtxBuffItem->Release();		// 開放
		g_pVtxBuffItem = NULL;			// NULLへ
	}
}

//=========================================================================================================================
// アイテム更新処理
//=========================================================================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ
	int nCntItem;

	Player *pPlayer = GetPlayer();

	if (pPlayer->bDisp == false)
	{// 非表示
		g_nCntPowerHit = 0;		// 初期化
	}

	// 頂点バッファをロック
	g_pVtxBuffItem->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// アイテムをカウント
		if (g_ItemState[nCntItem].bDisp == true)
		{// 表示するなら
			g_ItemState[nCntItem].nCntPos--;	// 減らす

			if (g_ItemState[nCntItem].nCntPos <= 0)
			{// ０以下だったら
				g_ItemState[nCntItem].move = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
			}

			if (pPlayer->bDisp == true)
			{// プレイヤーがいる場合
				if (pPlayer->pos.x - 50 < g_ItemState[nCntItem].pos.x + 12 && pPlayer->pos.x + 50 > g_ItemState[nCntItem].pos.x - 12 && pPlayer->pos.y - 50 < g_ItemState[nCntItem].pos.y + 12 && pPlayer->pos.y + 50 > g_ItemState[nCntItem].pos.y - 12)
				{// 範囲チェック
					g_ItemState[nCntItem].move.x = sinf(atan2f(-g_ItemState[nCntItem].pos.x - -pPlayer->pos.x, -g_ItemState[nCntItem].pos.y - -pPlayer->pos.y)) * 5.0f;
					g_ItemState[nCntItem].move.y = cosf(atan2f(-g_ItemState[nCntItem].pos.x - -pPlayer->pos.x, -g_ItemState[nCntItem].pos.y - -pPlayer->pos.y)) * 5.0f;
				}

				if (pPlayer->pos.x < g_ItemState[nCntItem].pos.x + 12 && pPlayer->pos.x > g_ItemState[nCntItem].pos.x - 12 && pPlayer->pos.y < g_ItemState[nCntItem].pos.y + 12 && pPlayer->pos.y > g_ItemState[nCntItem].pos.y - 12)
				{// 当たり判定設定
					PlaySound(SOUND_LABEL_SE_ITEMHIT000);		// アイテムヒット音

					HitItem(nCntItem);		// ヒット
				}
			}

			g_ItemState[nCntItem].pos += g_ItemState[nCntItem].move;	// 移動量代入

			if (g_ItemState[nCntItem].pos.y > SCREEN_HEIGHT)
			{// 範囲外だったら
				g_ItemState[nCntItem].bDisp = false;	// 非表示
			}

			// 頂点設定
			pVtx[nCntItem * 4].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x - 12, g_ItemState[nCntItem].pos.y - 12, 0.0f);
			pVtx[nCntItem * 4 + 1].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x + 12, g_ItemState[nCntItem].pos.y - 12, 0.0f);
			pVtx[nCntItem * 4 + 2].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x - 12, g_ItemState[nCntItem].pos.y + 12, 0.0f);
			pVtx[nCntItem * 4 + 3].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x + 12, g_ItemState[nCntItem].pos.y + 12, 0.0f);
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();

}

//=========================================================================================================================
// アイテム描画処理
//=========================================================================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntItem;

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffItem,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// アイテムをカウント
		if (g_ItemState[nCntItem].bDisp == true)
		{// 表示するなら
			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureItem);

			// アイテムの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
		}
	}
}

//=========================================================================================================================
// アイテム設定処理
//=========================================================================================================================
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ
	int nCntItem;

	// 頂点バッファをロック
	g_pVtxBuffItem->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// アイテムをカウント
		if (g_ItemState[nCntItem].bDisp == false)
		{// 非表示なら
			g_ItemState[nCntItem].pos		= pos;		// 位置
			g_ItemState[nCntItem].nCntPos	= 50;		// カウント
			g_ItemState[nCntItem].Type		= type;		// 種類
			g_ItemState[nCntItem].bDisp		= true;		// 表示する

			// 移動量
			g_ItemState[nCntItem].move.x = sinf((rand() % 628) / 100.0f) * 2.0f;
			g_ItemState[nCntItem].move.y = cosf((rand() % 628) / 100.0f) * 2.0f;

			// 頂点設定
			pVtx[nCntItem * 4].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x - 12, g_ItemState[nCntItem].pos.y - 12, 0.0f);
			pVtx[nCntItem * 4 + 1].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x + 12, g_ItemState[nCntItem].pos.y - 12, 0.0f);
			pVtx[nCntItem * 4 + 2].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x - 12, g_ItemState[nCntItem].pos.y + 12, 0.0f);
			pVtx[nCntItem * 4 + 3].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x + 12, g_ItemState[nCntItem].pos.y + 12, 0.0f);
			// テクスチャ設定
			pVtx[nCntItem * 4].tex = D3DXVECTOR2(type * ITEM_TEXTURE_UV_U, 0.0f);
			pVtx[nCntItem * 4 + 1].tex = D3DXVECTOR2(type * ITEM_TEXTURE_UV_U + ITEM_TEXTURE_UV_U, 0.0f);
			pVtx[nCntItem * 4 + 2].tex = D3DXVECTOR2(type * ITEM_TEXTURE_UV_U, ITEM_TEXTURE_UV_V);
			pVtx[nCntItem * 4 + 3].tex = D3DXVECTOR2(type * ITEM_TEXTURE_UV_U + ITEM_TEXTURE_UV_U, ITEM_TEXTURE_UV_V);
			
			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();
}

//=========================================================================================================================
// アイテム当たり処理
//=========================================================================================================================
void HitItem(int nCntItem)
{
	Player *pPlayer = GetPlayer();

	if (g_ItemState[nCntItem].Type == ITEMTYPE_POWER)
	{// パワー
		g_nCntPowerHit++;

		if (g_nCntPowerHit == 10)
		{
			PlaySound(SOUND_LABEL_SE_BULLETUP000);		// 弾変化音

			pPlayer->Type = BULLETTYPE_PLAYER2;
		}
		else if (g_nCntPowerHit == 20)
		{
			PlaySound(SOUND_LABEL_SE_BULLETUP000);		// 弾変化音

			pPlayer->Type = BULLETTYPE_PLAYER3;
		}
	/*	else if (g_nCntPowerHit == 30)
		{
			PlaySound(SOUND_LABEL_SE_BULLETUP000);		// 弾変化音

			pPlayer->Type = BULLETTYPE_PLAYER3;
		}
		else if (g_nCntPowerHit == 40)
		{
			PlaySound(SOUND_LABEL_SE_BULLETUP000);		// 弾変化音

			pPlayer->Type = BULLETTYPE_PLAYER3;
		}*/
	}
	else if (g_ItemState[nCntItem].Type == ITEMTYPE_LIFE)
	{// ライフ
		if (pPlayer->nLife < PLAYER_LIFE)
		{// ライフが最大より低い
			pPlayer->bLifeTex[pPlayer->nLife] = true;
			pPlayer->nLife += 1;
		}
	}
	else if (g_ItemState[nCntItem].Type == ITEMTYPE_STAR)
	{// スター
		pPlayer->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pPlayer->State = PLAYERSTATE_INVINCIBLE;
		pPlayer->nCounterState = 180;
	}

	g_ItemState[nCntItem].bDisp = false;
}