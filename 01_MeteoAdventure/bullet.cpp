//=========================================================================================================================
// 
// 弾処理 [bullet.cpp]
// Author : Einsuke Sakagawa
// 
//=========================================================================================================================

//**********************************************************************************************************************
// インクルードファイル
//**********************************************************************************************************************
#include "bullet.h"		// 弾
#include "explosion.h"	// 爆発
#include "score.h"		// スコア
#include "player.h"		// プレイヤー
#include "enemy.h"		// 敵
#include "item.h"		// アイテム
#include "sound.h"		// サウンド

//**********************************************************************************************************************
// マクロ定義
//**********************************************************************************************************************
#define BULLET_TEXTURE_UV_U		(1.0f)									// 弾アニメーションU範囲
#define BULLET_TEXTURE_UV_V		(1.0f)									// 弾アニメーションV範囲

//**********************************************************************************************************************
// グローバル変数宣言
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureBullet[BULLETTEXTURETYPE_PLAYER_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffBullet = NULL;								// 頂点バッファへのポインタ
Bullet							g_aBullet[MAX_BULLET];									// 弾の情報

//=========================================================================================================================
// 弾初期化処理
//=========================================================================================================================
void InitBullet(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntBullet;		// 弾カウント

	// 弾の情報の初期化
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// 弾をカウント
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 弾中心値
		g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 弾中心値
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 弾移動値
		g_aBullet[nCntBullet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 弾色
		g_aBullet[nCntBullet].Type = BULLETTYPE_PLAYER;					// 弾種類
		g_aBullet[nCntBullet].fWidth = 0;								// 横幅
		g_aBullet[nCntBullet].fHeight = 0;								// 高さ
		g_aBullet[nCntBullet].texture = BULLETTEXTURETYPE_PLAYER_NORMAL;// テクスチャ
		g_aBullet[nCntBullet].bUse = false;								// 弾使用していない状態
		g_aBullet[nCntBullet].pBlock = NULL;							// ブロックポインタ
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURE_NAME1, &g_pTextureBullet[0]);		// プレイヤー

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBullet->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// 弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// 弾をカウント
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
		pVtx[1].tex = D3DXVECTOR2(BULLET_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, BULLET_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(BULLET_TEXTURE_UV_U, BULLET_TEXTURE_UV_V);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=========================================================================================================================
// 弾終了処理
//=========================================================================================================================
void UninitBullet(void)
{
	// 変数宣言
	int nCntTex;	// テクスチャカウンタ

	// テクスチャーの破棄
	for (nCntTex = 0; nCntTex < BULLETTEXTURETYPE_PLAYER_MAX; nCntTex++)
	{// テクスチャをカウント
		if (g_pTextureBullet[nCntTex] != NULL)
		{
			g_pTextureBullet[nCntTex]->Release();
			g_pTextureBullet[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBullet!= NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet= NULL;
	}
}

//=========================================================================================================================
// 弾更新処理
//=========================================================================================================================
void UpdateBullet(void)
{
	// 変数宣言
	int nCntBullet;					// 弾カウント

	PLAYER *pPlayer = GetPlayer();	// プレイヤー情報

	VERTEX_2D *pVtx;				// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBullet->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// 弾をカウント
		if (g_aBullet[nCntBullet].bUse == true)
		{// 使用されている場合

			// 位置を記憶
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;
			// 画面スクロール
			g_aBullet[nCntBullet].pos.x -= pPlayer->move.x / 2;
			// 重力加算
			g_aBullet[nCntBullet].move.y += pPlayer->fGravity;
			// 移動値代入
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			if (g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT)
			{
				DeleteBullet(nCntBullet);
			}

			if (CollisionBlock(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight, &g_aBullet[nCntBullet].pBlock, COLLISIONBLOCKRETURN_BULLET) == true)
			{// ブロックとの衝突処理
				DeleteBullet(nCntBullet);
			}

			if (g_aBullet[nCntBullet].pBlock != NULL)
			{
				if (g_aBullet[nCntBullet].pBlock->type== BLOCKTYPE_NORMAL_BREAK || g_aBullet[nCntBullet].pBlock->type == BLOCKTYPE_NORMAL_DAMAGE)
				{
					g_aBullet[nCntBullet].pBlock->nLife--;

					if (g_aBullet[nCntBullet].pBlock->nLife <= 0)
					{
						g_aBullet[nCntBullet].pBlock->bUse = false;
					}
				}
			}

			// アイテムの衝突処理
			CollisionItem(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight);

			// 頂点設定
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y, 0.0f);
			// 頂点カラー設定
			pVtx[nCntBullet * 4].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 1].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 2].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 3].col = g_aBullet[nCntBullet].col;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=========================================================================================================================
// 弾描画処理
//=========================================================================================================================
void DrawBullet(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntBullet;		// 弾カウント

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBullet, 
		0, 
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// 弾をカウント
		if (g_aBullet[nCntBullet].bUse == true)
		{// 弾がある場合
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].texture]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//=========================================================================================================================
// 弾の設定処理
//=========================================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE Type, float fWidth, float fHeight, BULLETTEXTURETYPE texture)
{
	// 変数宣言
	int nCntBullet;			// 弾カウント

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBullet->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// 弾をカウント
		if (g_aBullet[nCntBullet].bUse == false)
		{// 使用されていない場合
			g_aBullet[nCntBullet].pos = pos;				// 位置設定
			g_aBullet[nCntBullet].move = move;				// 移動値設定
			g_aBullet[nCntBullet].col = col;				// 色設定
			g_aBullet[nCntBullet].Type = Type;				// 種類設定
			g_aBullet[nCntBullet].fWidth = fWidth;			// 横幅
			g_aBullet[nCntBullet].fHeight = fHeight;		// 高さ
			g_aBullet[nCntBullet].texture = texture;		// テクスチャ
			g_aBullet[nCntBullet].bUse = true;				// 使用している状態
			g_aBullet[nCntBullet].pBlock = NULL;			// ブロックポインタ

			// 頂点設定
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y, 0.0f);
			// 頂点カラー設定
			pVtx[nCntBullet * 4].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 1].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 2].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 3].col = g_aBullet[nCntBullet].col;

			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBullet->Unlock();
}

//=========================================================================================================================
// 弾の当たり判定処理
//=========================================================================================================================
bool CollisiomBullet(D3DXVECTOR3 *pPos, float fWidth, float fHeight, COLLISIONBULLETRETURN collision)
{
	// 変数宣言
	int nCntBullet;			// 弾カウンタ
	bool bHit = false;		// 当たり判定

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// 弾をカウント
		if (g_aBullet[nCntBullet].bUse == true)
		{// 使用されている場合
			if (collision == COLLISIONBULLETRETURN_BLOCK)
			{
				if (g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth <= pPos->x + fWidth &&
					g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth >= pPos->x &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight <= pPos->y + fHeight &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fHeight >= pPos->y)
				{
					DeleteBullet(nCntBullet);		// 弾破棄処理
					bHit = true;					// 当たり判定
				}
			}
			else
			{
				if (g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth <= pPos->x + fWidth &&
					g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth >= pPos->x - fWidth &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight <= pPos->y &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fHeight >= pPos->y - fHeight)
				{
					DeleteBullet(nCntBullet);		// 弾破棄処理
					bHit = true;					// 当たり判定
				}
			}
		}
	}

	return bHit;		// 値を返す
}

//=========================================================================================================================
// 弾の破棄処理
//=========================================================================================================================
void DeleteBullet(int nCntBullet)
{//１つ
	SetExplosion(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - (g_aBullet[nCntBullet].fHeight / 2), 0.0f));
	g_aBullet[nCntBullet].bUse = false;
}
void DeleteBullet(void)
{// 全部
	// 変数宣言
	int nCntBullet;

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// 弾をカウント
		if (g_aBullet[nCntBullet].bUse == true)
		{// 使用されている場合
			g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 弾中心値
			g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 弾中心値
			g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 弾移動値
			g_aBullet[nCntBullet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 弾色
			g_aBullet[nCntBullet].Type = BULLETTYPE_PLAYER;					// 弾種類
			g_aBullet[nCntBullet].fWidth = 0;								// 横幅
			g_aBullet[nCntBullet].fHeight = 0;								// 高さ
			g_aBullet[nCntBullet].texture = BULLETTEXTURETYPE_PLAYER_NORMAL;// テクスチャ
			g_aBullet[nCntBullet].bUse = false;								// 弾使用していない状態
			g_aBullet[nCntBullet].pBlock = NULL;							// ブロックポインタ
		}
	}
}