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
#include "input.h"			// 入力処理
#include "player.h"			// プレイヤー
#include "block.h"			// ブロック
#include "score.h"			// スコア
#include "fade.h"			// フェード			
#include "sound.h"			// サウンド

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_ITEM				(128)								// アイテムの最大数
#define ITEM_TEXTURE_NAME		"data\\TEXTURE\\coin001.png"		// 読み込むテクスチャファイル
#define ITEM_POS_X				(20)								// アイテムの左上X座標
#define ITEM_POS_Y				(40)								// アイテムの左上Y座標
#define ITEM_WIDTH				(SCREEN_WIDTH)						// アイテムの幅
#define ITEM_HEIGHT				(SCREEN_HEIGHT)						// アイテムの高
#define ITEM_TEXTURE_UV_U		(0.2f)								// テクスチャアニメーションU範囲
#define ITEM_TEXTURE_UV_V		(0.5f)								// テクスチャアニメーションV範囲
#define ITEM_TEXTURE_SPEED		(5)									// テクスチャアニメーションスピード
#define ITEM_TEXTURE_PATTERN	(10)								// テクスチャアニメーションパターン数

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureItem = NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffItem = NULL;					// 頂点バッファへのポインタ
ItemState					g_Item[MAX_ITEM];						// アイテム情報
int							g_nCntCoin;								// コイン総数カウンタ

//=========================================================================================================================
// アイテム初期化処理
//=========================================================================================================================
void InitItem(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntItem;		// アイテムカウンタ

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// アイテムをカウント
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_Item[nCntItem].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回位置
		g_Item[nCntItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動値
		g_Item[nCntItem].fWidth = ITEM_POS_X;						// 横幅
		g_Item[nCntItem].fHeight = ITEM_POS_Y;						// 高さ
		g_Item[nCntItem].nCounterAnim = 0;							// アニメーションカウンタ
		g_Item[nCntItem].nPatternAnim = 0;							// アニメーションパターン
		g_Item[nCntItem].Type = ITEMTYPE_COIN_NORMAL;				// 種類
		g_Item[nCntItem].state = ITEMSTATE_APPEAR;					// 状態
		g_Item[nCntItem].nCounterState = 0;							// 状態カウンタ
		g_Item[nCntItem].bUse = false;								// 使用しているか
	}
	g_nCntCoin = 0;													// 総数初期値

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURE_NAME, &g_pTextureItem);		// コイン

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
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//=========================================================================================================================
// アイテム更新処理
//=========================================================================================================================
void UpdateItem(void)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ
	int nCntItem;		// アイテムカウンタ
	PLAYER *pPlayer = GetPlayer();		// プレイヤー情報
	bool bScroll = GetScrollNum();

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
		if (g_Item[nCntItem].bUse == true)
		{// 使用している場合
			switch (g_Item[nCntItem].state)
			{
			case ITEMSTATE_APPEAR:
				break;
			case ITEMSTATE_NORMAL:

				g_Item[nCntItem].nCounterAnim++;	// カウンタを進める

				if (g_Item[nCntItem].nCounterAnim % ITEM_TEXTURE_SPEED == 0)
				{// パターンを進める
					g_Item[nCntItem].nPatternAnim = (g_Item[nCntItem].nPatternAnim + 1) % ITEM_TEXTURE_PATTERN;
				}

				if (g_Item[nCntItem].Type != ITEMTYPE_COIN_NORMAL)
				{
					g_Item[nCntItem].posOld = g_Item[nCntItem].pos;
					g_Item[nCntItem].move.y += pPlayer->fGravity;
					g_Item[nCntItem].pos.y += g_Item[nCntItem].move.y;

					CollisionBlock(&g_Item[nCntItem].pos, &g_Item[nCntItem].posOld, &g_Item[nCntItem].move, g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, NULL, COLLISIONBLOCKRETURN_OTHER);
				}

				break;
			case ITEMSTATE_DISAPPEAR:

				g_Item[nCntItem].nCounterState++;
				g_Item[nCntItem].nPatternAnim = 0;

				if (g_Item[nCntItem].nCounterState < 10)
				{
					g_Item[nCntItem].pos.y += -5;
				}
				else
				{
					g_Item[nCntItem].col.a -= 0.06f;
				
					if (g_Item[nCntItem].col.a <= 0.0f)
					{
						DeleteItem(nCntItem);
					}
				}

				break;
			}

			if (bScroll == false)
			{
				// 画面スクロール
				g_Item[nCntItem].pos.x -= pPlayer->move.x / 2;
			}

			// 頂点設定
			pVtx[nCntItem * 4].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y - g_Item[nCntItem].fHeight, 0.0f);
			pVtx[nCntItem * 4 + 1].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y - g_Item[nCntItem].fHeight, 0.0f);
			pVtx[nCntItem * 4 + 2].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y, 0.0f);
			pVtx[nCntItem * 4 + 3].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y, 0.0f);
			// 頂点カラー設定
			pVtx[nCntItem * 4].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 1].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 2].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 3].col = g_Item[nCntItem].col;
			// テクスチャ設定
			pVtx[nCntItem * 4].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim % 5 * ITEM_TEXTURE_UV_U, g_Item[nCntItem].nPatternAnim / 5 * ITEM_TEXTURE_UV_V);
			pVtx[nCntItem * 4 + 1].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim % 5 * ITEM_TEXTURE_UV_U + ITEM_TEXTURE_UV_U, g_Item[nCntItem].nPatternAnim / 5 * ITEM_TEXTURE_UV_V);
			pVtx[nCntItem * 4 + 2].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim % 5 * ITEM_TEXTURE_UV_U, g_Item[nCntItem].nPatternAnim / 5 * ITEM_TEXTURE_UV_V + ITEM_TEXTURE_UV_V);
			pVtx[nCntItem * 4 + 3].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim % 5 * ITEM_TEXTURE_UV_U + ITEM_TEXTURE_UV_U, g_Item[nCntItem].nPatternAnim / 5 * ITEM_TEXTURE_UV_V + ITEM_TEXTURE_UV_V);
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
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntItem;		// アイテムカウンタ

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
		if (g_Item[nCntItem].bUse == true)
		{// 使用している場合
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
	// 変数宣言
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
		if (g_Item[nCntItem].bUse == false)
		{// 使用していない場合
			g_Item[nCntItem].pos = pos;				// 位置
			g_Item[nCntItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
			g_Item[nCntItem].nCounterAnim = 0;		// アニメーションカウンタ初期化
			g_Item[nCntItem].nPatternAnim = 0;		// アニメーションパターン初期化
			g_Item[nCntItem].Type = type;			// 種類
			g_Item[nCntItem].state = ITEMSTATE_NORMAL;	// 状態
			g_Item[nCntItem].nCounterState = 0;		// 状態カウンタ
			g_Item[nCntItem].bUse = true;			// 使用している

			g_nCntCoin++;							// 総数を加算

			// 頂点設定
			pVtx[nCntItem * 4].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y - g_Item[nCntItem].fHeight, 0.0f);
			pVtx[nCntItem * 4 + 1].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y - g_Item[nCntItem].fHeight, 0.0f);
			pVtx[nCntItem * 4 + 2].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y, 0.0f);
			pVtx[nCntItem * 4 + 3].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y, 0.0f);
			// 頂点カラー設定
			pVtx[nCntItem * 4].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 1].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 2].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 3].col = g_Item[nCntItem].col;
			// テクスチャ設定
			pVtx[nCntItem * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCntItem * 4 + 1].tex = D3DXVECTOR2(ITEM_TEXTURE_UV_U, 0.0f);
			pVtx[nCntItem * 4 + 2].tex = D3DXVECTOR2(0.0f, ITEM_TEXTURE_UV_V);
			pVtx[nCntItem * 4 + 3].tex = D3DXVECTOR2(ITEM_TEXTURE_UV_U, ITEM_TEXTURE_UV_V);
			
			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffItem->Unlock();
}

//=========================================================================================================================
// アイテム衝突処理
//=========================================================================================================================
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	// 変数宣言
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// ブロックをカウント
		if (g_Item[nCntItem].bUse == true && g_Item[nCntItem].state == ITEMSTATE_NORMAL)
		{// ブロックを使用している場合
			if (pPos->x + fWidth / 2 > g_Item[nCntItem].pos.x - g_Item[nCntItem].fWidth && pPos->x - fWidth / 2 < g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth &&
				pPos->y - fHeight < g_Item[nCntItem].pos.y && pPos->y > g_Item[nCntItem].pos.y - g_Item[nCntItem].fHeight)
			{// 衝突している
				g_nCntCoin--;						// 総数を減算
				PlaySound(SOUND_LABEL_SE_ITEMHIT000);
				AddScore(SCORETYPE_COIN, -1);		// 残りコイン枚数を表示
				AddScore(SCORETYPE_SCORE, 100);		// 取得スコアを加算
				g_Item[nCntItem].state = ITEMSTATE_DISAPPEAR;
			}
		}
	}
}

//=========================================================================================================================
// アイテム破棄処理
//=========================================================================================================================
void DeleteItem(int nCntItem)
{
	g_Item[nCntItem].bUse = false;		// 衝突したアイテムを破棄

	if (g_nCntCoin <= 0)
	{// 総数が以下の場合
	//	SetFadeGame(GAMESTATE_MAP, FADETYPE_GAME);		// ゲームマップ
	}
}
void DeleteItem(void)
{
	// 変数宣言
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// アイテムをカウント
		if (g_Item[nCntItem].bUse == true)
		{// 使用している場合
			g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
			g_Item[nCntItem].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回位置
			g_Item[nCntItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
			g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動値
			g_Item[nCntItem].fWidth = ITEM_POS_X;						// 横幅
			g_Item[nCntItem].fHeight = ITEM_POS_Y;						// 高さ
			g_Item[nCntItem].nCounterAnim = 0;							// アニメーションカウンタ
			g_Item[nCntItem].nPatternAnim = 0;							// アニメーションパターン
			g_Item[nCntItem].Type = ITEMTYPE_COIN_NORMAL;				// 種類
			g_Item[nCntItem].state = ITEMSTATE_APPEAR;					// 状態
			g_Item[nCntItem].nCounterState = 0;							// 状態カウンタ
			g_Item[nCntItem].bUse = false;								// 使用しているか
		}
	}
	
	g_nCntCoin = 0;			// 総数初期値
}

//=========================================================================================================================
// アイテム(コイン）総数取得処理
//=========================================================================================================================
int GetNumCoin(void)
{
	return g_nCntCoin;		// 値を返す
}