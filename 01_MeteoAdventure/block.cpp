//=========================================================================================================================
//
// ブロック処理 [block.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "block.h"		// ブロック
#include "input.h"		// 入力処理
#include "player.h"		// プレイヤー
#include "bullet.h"		// 弾

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define BLOCK_TEXTURE_SPEED		(5)								// テクスチャアニメーションスピード
#define BLOCK_TEXTURE_PATTERN	(10)							// テクスチャアニメーションパターン数
#define BLOCK_TEXTURE_UV_U		(1.0f)							// テクスチャアニメーションU範囲
#define BLOCK_TEXTURE_UV_V		(1.0f)							// テクスチャアニメーションV範囲
#define BLOCK_BACKTEXTURE_UV_U	(1.0f)							// 前テクスチャアニメーションU範囲
#define BLOCK_BACKTEXTURE_UV_V	(1.0f)							// 前テクスチャアニメーションV範囲
#define BLOCK_SPEED				(0.002f)						// ブロックスピード
#define BLOCK_LIFE				(3)								// ブロック耐久値

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureBlock[BLOCKTEXTURE_MAX] = {};			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffBlock = NULL;							// 頂点バッファへのポインタ
BLOCK							g_Block[MAX_BLOCK];								// ブロックの情報
bool							g_bScroll;										// スクロール判定

//=========================================================================================================================
// ブロック初期化処理
//=========================================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスを取得
	int nCntBlock;		// ブロックカウンタ

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// ブロックをカウント
		g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_Block[nCntBlock].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_Block[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動値
		g_Block[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		g_Block[nCntBlock].fWidth = 0.0f;							// 横幅
		g_Block[nCntBlock].fHeight = 0.0f;							// 高さ
		g_Block[nCntBlock].type = BLOCKTYPE_NONE;					// ブロック種類
		g_Block[nCntBlock].nLife = 0;								// 耐久値
		g_Block[nCntBlock].nCntMove = 0;							// 移動カウンタ
		g_Block[nCntBlock].nCntMoveLimit = 0;						// 移動制限
		g_Block[nCntBlock].bBlockScroll = false;					// ブロックに当たっていない
		g_Block[nCntBlock].bUse = false;							// 使用していない
		g_Block[nCntBlock].texture = BLOCKTEXTURE_GROUND;			// テクスチャ
	}
	g_bScroll = false;			// スクロール判定

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_1, &g_pTextureBlock[0]);		// 地面
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_2, &g_pTextureBlock[1]);		// 壁
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_3, &g_pTextureBlock[2]);		// ノーマル
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_4, &g_pTextureBlock[3]);		// 土管
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_5, &g_pTextureBlock[4]);		// 砂漠
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_6, &g_pTextureBlock[5]);		// とげ
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_7, &g_pTextureBlock[6]);		// 砂

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBlock->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// ブロックをカウント
		// 頂点設定
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
		// 頂点カラー設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(BLOCK_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, BLOCK_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(BLOCK_TEXTURE_UV_U, BLOCK_TEXTURE_UV_V);
		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();
}

//=========================================================================================================================
// ブロック終了処理
//=========================================================================================================================
void UninitBlock(void)
{
	// 変数宣言
	int nCntTex;

	// テクスチャの破棄
	for (nCntTex = 0; nCntTex < BLOCKTEXTURE_MAX; nCntTex++)
	{// テクスチャをカウント
		if (g_pTextureBlock[nCntTex] != NULL)
		{
			g_pTextureBlock[nCntTex]->Release();
			g_pTextureBlock[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//=========================================================================================================================
// ブロック更新処理
//=========================================================================================================================
void UpdateBlock(void)
{
	// 変数宣言
	int nCntBlock;		// ブロックカウント
	g_bScroll = false;	// スクロール判定
	PLAYER *pPlayer = GetPlayer();		// プレイヤー情報
	GAMESTAGE stage = GetNumStage();

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBlock->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// ブロックに当たっているか確認
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// ブロックをカウント
		if (g_Block[nCntBlock].bUse == true)
		{// ブロックを使用している
			if (g_Block[nCntBlock].bBlockScroll == true)
			{
				g_bScroll = true;	// 数値を変更
			}
		}
	}

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// ブロックをカウント
		if (g_Block[nCntBlock].bUse == true)
		{// ブロックを使用している
			switch (g_Block[nCntBlock].type)
			{// ブロック種類
			case BLOCKTYPE_NONE:		// 使用していない
				break;
			case BLOCKTYPE_NORMAL:
				break;
			case BLOCKTYPE_NORMAL_BREAK:
				break;
			case BLOCKTYPE_NORMAL_WARP_X:
				break;
			case BLOCKTYPE_NORMAL_WARP_Y:
				break;
			case BLOCKTYPE_NORMAL_FALL:

				if (g_Block[nCntBlock].move.y != 0.0f)
				{
					g_Block[nCntBlock].nCntMove++;

					if (g_Block[nCntBlock].nCntMove > g_Block[nCntBlock].nCntMoveLimit)
					{
						g_Block[nCntBlock].col.a -= 0.06f;

						if (g_Block[nCntBlock].col.a <= 0.0f)
						{
							DeleteBlock(nCntBlock);
						}
					}
				}

				break;
			case BLOCKTYPE_NORMAL_APPEAR:

				if (g_Block[nCntBlock].pos.x - 200 < pPlayer->pos.x + pPlayer->fWidth && g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + 200 > pPlayer->pos.x - pPlayer->fWidth)
				{
					if (g_Block[nCntBlock].col.a <= 1.0f)
					{
						g_Block[nCntBlock].col.a += 0.03f;
					}
				}
				else
				{
					if (g_Block[nCntBlock].col.a >= 0.0f)
					{
						g_Block[nCntBlock].col.a -= 0.03f;
					}
				}

				break;
			case BLOCKTYPE_NORMAL_DAMAGE:
				break;
			case BLOCKTYPE_NORMAL_SUNA:

				if (g_Block[nCntBlock].move.y != 0.0f)
				{
					g_Block[nCntBlock].nCntMove++;

					if (g_Block[nCntBlock].nCntMove > g_Block[nCntBlock].nCntMoveLimit)
					{
						g_Block[nCntBlock].col.a -= 0.06f;

						if (g_Block[nCntBlock].col.a <= 0.0f)
						{
							DeleteBlock(nCntBlock);
						}
					}
				}

				break;
			case BLOCKTYPE_NORMAL_SUNA_RE:

				if (g_Block[nCntBlock].move.y > 0.0f)
				{
					g_Block[nCntBlock].nCntMove++;

					if (g_Block[nCntBlock].nCntMove > g_Block[nCntBlock].nCntMoveLimit)
					{
						if (g_Block[nCntBlock].col.a >= 0.0f)
						{
							g_Block[nCntBlock].col.a -= 0.01f;
						}

						if (g_Block[nCntBlock].nCntMove == g_Block[nCntBlock].nCntMoveLimit * 2)
						{
							g_Block[nCntBlock].move.y *= -1;
							g_Block[nCntBlock].nCntMove = 0;
						}
					}
				}

				if (g_Block[nCntBlock].move.y < 0.0f)
				{
					g_Block[nCntBlock].nCntMove++;
					if (g_Block[nCntBlock].nCntMove == g_Block[nCntBlock].nCntMoveLimit * 2)
					{
						g_Block[nCntBlock].move.y = 0.0f;
						g_Block[nCntBlock].nCntMove = 0;
					}
				}
				else
				{
					if (g_Block[nCntBlock].move.y == 0.0f && g_Block[nCntBlock].col.a <= 1.0f)
					{
						g_Block[nCntBlock].col.a += 0.08f;
					}
				}

				break;
			case BLOCKTYPE_MOVE_TRIP:	// 往復

				g_Block[nCntBlock].nCntMove++;

				if (g_Block[nCntBlock].nCntMove == g_Block[nCntBlock].nCntMoveLimit)
				{
					g_Block[nCntBlock].move *= -1;
					g_Block[nCntBlock].nCntMove = 0;
				}

				break;
			case BLOCKTYPE_MOVE_LOOP:	// 繰り返し

				if (g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight < 0)
				{
					g_Block[nCntBlock].pos.y = SCREEN_HEIGHT + g_Block[nCntBlock].fHeight;
				}
				else if (g_Block[nCntBlock].pos.y > SCREEN_HEIGHT + g_Block[nCntBlock].fHeight)
				{
					g_Block[nCntBlock].pos.y = -g_Block[nCntBlock].fHeight;
				}

				if (g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth < 0)
				{
					g_Block[nCntBlock].pos.x = SCREEN_WIDTH + g_Block[nCntBlock].fWidth;
				}
				else if (g_Block[nCntBlock].pos.x > SCREEN_WIDTH + g_Block[nCntBlock].fWidth)
				{
					g_Block[nCntBlock].pos.x = -g_Block[nCntBlock].fWidth;
				}

				break;
			}

			if (g_bScroll == false)
			{// スクロール
				g_Block[nCntBlock].pos.x -= pPlayer->move.x / 2;
			}

			g_Block[nCntBlock].pos += g_Block[nCntBlock].move;

			// 頂点設定
			pVtx[nCntBlock * 4].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, 0.0f);
			pVtx[nCntBlock * 4 + 1].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y, 0.0f);
			pVtx[nCntBlock * 4 + 2].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, 0.0f);
			pVtx[nCntBlock * 4 + 3].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, 0.0f);
			// 頂点カラー設定
			pVtx[nCntBlock * 4].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 1].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 2].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 3].col = g_Block[nCntBlock].col;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();
}

//=========================================================================================================================
// ブロック描画処理
//=========================================================================================================================
void DrawBlock(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntBlock;		// ブロックカウンタ

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBlock,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// ブロックをカウント
		if (g_Block[nCntBlock].bUse == true)
		{// 使用している場合
			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureBlock[g_Block[nCntBlock].texture]);

			// ブロックの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}

//=========================================================================================================================
// ブロック設定処理
//=========================================================================================================================
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, BLOCKTYPE type, int nCntMoveLimit, BLOCKTEXTURE texture)
{
	// 変数宣言
	int nCntBlock;		// ブロックカウンタ

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBlock->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// ブロックをカウント
		if (g_Block[nCntBlock].bUse == false)
		{// 使用していない場合
			g_Block[nCntBlock].pos = pos;						// 位置
			g_Block[nCntBlock].move = move;						// 移動値
			g_Block[nCntBlock].fWidth = fWidth;					// 横幅
			g_Block[nCntBlock].fHeight = fHeight;				// 高さ
			g_Block[nCntBlock].type = type;						// ブロック種類
			g_Block[nCntBlock].nCntMove = 0;					// 移動カウント
			g_Block[nCntBlock].nCntMoveLimit = nCntMoveLimit;	// 移動制限
			g_Block[nCntBlock].bUse = true;						// 使用している
			g_Block[nCntBlock].texture = texture;				// テクスチャ

			switch (g_Block[nCntBlock].type)
			{
			case BLOCKTYPE_NORMAL_BREAK:

				g_Block[nCntBlock].nLife = 1;				// ブロック耐久値

				break;
			case BLOCKTYPE_NORMAL_DAMAGE:

				g_Block[nCntBlock].nLife = 2;				// ブロック耐久値

				break;
			}

			if (g_Block[nCntBlock].texture == BLOCKTEXTURE_DOKAN)
			{
				g_Block[nCntBlock].col = D3DXCOLOR(0.5f, 1.0f, 0.1f, 1.0f);
			}
			else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_BREAK)
			{
				g_Block[nCntBlock].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_FALL)
			{
				g_Block[nCntBlock].col = D3DXCOLOR(0.3f, 1.0f, 0.6f, 1.0f);
			}
			else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_APPEAR)
			{
				g_Block[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			}
			else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_DAMAGE)
			{
				g_Block[nCntBlock].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
			}
			else
			{
				g_Block[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// 頂点設定
			pVtx[nCntBlock * 4].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x,										g_Block[nCntBlock].pos.y, 0.0f);
			pVtx[nCntBlock * 4 + 1].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth,		g_Block[nCntBlock].pos.y, 0.0f);
			pVtx[nCntBlock * 4 + 2].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x,									g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, 0.0f);
			pVtx[nCntBlock * 4 + 3].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth,		g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, 0.0f);
			// 頂点カラー設定
			pVtx[nCntBlock * 4].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 1].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 2].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 3].col = g_Block[nCntBlock].col;

			// テクスチャ設定
			if (g_Block[nCntBlock].texture == BLOCKTEXTURE_GROUND || g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_APPEAR || g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA)
			{
				if (g_Block[nCntBlock].texture == BLOCKTEXTURE_GROUND)
				{
					g_Block[nCntBlock].tex.x = BLOCK_TEXTURE_UV_U / (SCREEN_WIDTH / (fWidth));
				}
				else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_APPEAR || g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA)
				{
					g_Block[nCntBlock].tex.x = BLOCK_TEXTURE_UV_U / (50 / (fWidth));
				}

				pVtx[nCntBlock * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBlock * 4 + 1].tex = D3DXVECTOR2(g_Block[nCntBlock].tex.x, 0.0f);
				pVtx[nCntBlock * 4 + 2].tex = D3DXVECTOR2(0.0f, BLOCK_TEXTURE_UV_V);
				pVtx[nCntBlock * 4 + 3].tex = D3DXVECTOR2(g_Block[nCntBlock].tex.x, BLOCK_TEXTURE_UV_V);
			}
			else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA_RE)
			{
				g_Block[nCntBlock].tex.x = BLOCK_TEXTURE_UV_U / (50 / (fWidth));
				g_Block[nCntBlock].tex.y = BLOCK_TEXTURE_UV_V / (50 / (fHeight));

				pVtx[nCntBlock * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBlock * 4 + 1].tex = D3DXVECTOR2(g_Block[nCntBlock].tex.x, 0.0f);
				pVtx[nCntBlock * 4 + 2].tex = D3DXVECTOR2(0.0f, g_Block[nCntBlock].tex.y);
				pVtx[nCntBlock * 4 + 3].tex = D3DXVECTOR2(g_Block[nCntBlock].tex.x, g_Block[nCntBlock].tex.y);
			}
			else
			{
				pVtx[nCntBlock * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBlock * 4 + 1].tex = D3DXVECTOR2(BLOCK_TEXTURE_UV_U, 0.0f);
				pVtx[nCntBlock * 4 + 2].tex = D3DXVECTOR2(0.0f, BLOCK_TEXTURE_UV_V);
				pVtx[nCntBlock * 4 + 3].tex = D3DXVECTOR2(BLOCK_TEXTURE_UV_U, BLOCK_TEXTURE_UV_V);
			}

			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();
}

//=========================================================================================================================
// ブロックとの衝突処理
//=========================================================================================================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, BLOCK **pBlock, COLLISIONBLOCKRETURN collision)
{
	// 変数宣言
	int nCntBlock;			// ブロックカウンタ
	int nPointplus;			// 当たり判定位置調整
	bool bLand = false;		// 着地したかどうか
	int BlockCheck = NULL;	// 当たったか確認
	int nHit = 0;

	if (collision == COLLISIONBLOCKRETURN_PLAYER || collision == COLLISIONBLOCKRETURN_BULLET)
	{// プレイヤーのみ調整
		*pBlock = NULL;
		if (collision == COLLISIONBLOCKRETURN_PLAYER)
		{
			pPosOld->x -= pMove->x;
		}
	}

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// ブロックをカウント
		if (g_Block[nCntBlock].bUse == true)
		{// ブロックを使用している
			if (g_Block[nCntBlock].texture == BLOCKTEXTURE_GROUND || g_Block[nCntBlock].texture == BLOCKTEXTURE_SABAKU)
			{// 当たり位置調整
				nPointplus = 15;
			}
			else
			{
				nPointplus = 5;
			}

			if (collision == COLLISIONBLOCKRETURN_BULLET)
			{
				if ((pPosOld->x - fWidth > g_Block[nCntBlock].pos.x && pPosOld->x + fWidth < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth &&
					pPosOld->y - fHeight > g_Block[nCntBlock].pos.y && pPosOld->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight) ||
					(pPos->x - fWidth > g_Block[nCntBlock].pos.x	&& pPos->x + fWidth < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth &&
					pPos->y - fHeight > g_Block[nCntBlock].pos.y	&& pPos->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight))
				{
					bLand = true;
				}
			}

			if (pPos->x + (fWidth / 2) > g_Block[nCntBlock].pos.x && pPos->x - (fWidth / 2) < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
			{// X範囲確認
				if (pPos->y >= g_Block[nCntBlock].pos.y + nPointplus && g_Block[nCntBlock].pos.y + nPointplus >= pPosOld->y)
				{// 上からの当たり判定
					bLand = true;

					if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_WARP_Y && (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true))
					{
						if (pPos->x - (fWidth / 2) > g_Block[nCntBlock].pos.x && pPos->x + (fWidth / 2) < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
						{
							pMove->x = 0.0f;
							pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight + fHeight;
						}
						else
						{
							pPos->y = g_Block[nCntBlock].pos.y + nPointplus;
							pMove->y = 0.0f;
						}
					}
					else
					{
						if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA_RE && g_Block[nCntBlock].col.a < 1.0f)
						{

						}
						else
						{
							pPos->y = g_Block[nCntBlock].pos.y + nPointplus;
							pMove->y = 0.0f;
						}

						if (collision == COLLISIONBLOCKRETURN_PLAYER && g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_FALL)
						{
							g_Block[nCntBlock].move.y = 1.0f;
						}
						else if (collision == COLLISIONBLOCKRETURN_PLAYER && (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA || g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA_RE))
						{
							if (g_Block[nCntBlock].move.y == 0.0f)
							{
								g_Block[nCntBlock].move.y = 0.5f;
								g_Block[nCntBlock].nCntMove = 0;
							}
						}
					}

					BlockCheck = nCntBlock;
				}
				else if (pPos->y - (fHeight) <= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight && g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight <= pPosOld->y - (fHeight))
				{// 下からの当たり判定
					if (collision == COLLISIONBLOCKRETURN_OTHER || collision == COLLISIONBLOCKRETURN_BULLET)
					{
						bLand = true;
					}

					if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_WARP_Y)
					{
						if (pPos->x - (fWidth / 2) + 2 > g_Block[nCntBlock].pos.x && pPos->x + (fWidth / 2) - 2 < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
						{
							pMove->x = 0.0f;
							pPos->y = g_Block[nCntBlock].pos.y + nPointplus;
						}
						else
						{
							pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight + fHeight;
							pMove->y = 0.0f;
						}
					}
					else
					{
						pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight + fHeight;
						pMove->y = 0.0f;
					}

					BlockCheck = nCntBlock;
				}
				else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_FALL || g_Block[nCntBlock].type == BLOCKTYPE_MOVE_LOOP || g_Block[nCntBlock].type == BLOCKTYPE_MOVE_TRIP && g_Block[nCntBlock].move.y != 0.0f)
				{// X範囲確認
					if (pPos->y >= g_Block[nCntBlock].pos.y && g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight >= pPos->y)
					{// 上からの当たり判定
						bLand = true;
						pPos->y = g_Block[nCntBlock].pos.y + nPointplus;
						pMove->y = 0.0f;
						BlockCheck = nCntBlock;
					}
				}
			}

			if (pPos->y > g_Block[nCntBlock].pos.y + nPointplus && pPos->y - (fHeight) < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight)
			{// Y範囲確認
				if (pPos->x + (fWidth / 2) >= g_Block[nCntBlock].pos.x && g_Block[nCntBlock].pos.x >= pPosOld->x + (fWidth / 2))
				{// 左からの当たり判定
					if (collision == COLLISIONBLOCKRETURN_OTHER || collision == COLLISIONBLOCKRETURN_BULLET)
					{
						bLand = true;
					}

					if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_WARP_X)
					{
						//	pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + (fWidth / 2);
					}
					else
					{
						pPos->x = g_Block[nCntBlock].pos.x - fWidth / 2;
						pMove->x = 0.0f;
					}

					BlockCheck = nCntBlock;

					if (collision == COLLISIONBLOCKRETURN_PLAYER)
					{
						g_Block[nCntBlock].bBlockScroll = true;
					}
				}
				else if (pPos->x - (fWidth / 2) <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth && g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth <= pPosOld->x - (fWidth / 2))
				{// 右からの当たり判定
					if (collision == COLLISIONBLOCKRETURN_OTHER || collision == COLLISIONBLOCKRETURN_BULLET)
					{
						bLand = true;
					}

					if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_WARP_X)
					{
					//	pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + (fWidth / 2);
					}
					else
					{
						pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + (fWidth / 2);
						pMove->x = 0.0f;
					}

					BlockCheck = nCntBlock;

					if (collision == COLLISIONBLOCKRETURN_PLAYER)
					{
						g_Block[nCntBlock].bBlockScroll = true;
					}
				}
				else if (g_Block[nCntBlock].type == BLOCKTYPE_MOVE_LOOP || g_Block[nCntBlock].type == BLOCKTYPE_MOVE_TRIP && g_Block[nCntBlock].move.x != 0.0f)
				{// Y範囲確認
					if (pPos->x + (fWidth / 2) >= g_Block[nCntBlock].pos.x && g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth >= pPos->x + (fWidth / 2))
					{// 左からの当たり判定
						if (collision == COLLISIONBLOCKRETURN_OTHER || collision == COLLISIONBLOCKRETURN_BULLET)
						{
							bLand = true;
						}
						pPos->x = g_Block[nCntBlock].pos.x - (fWidth / 2);
						pMove->x = 0.0f;
						BlockCheck = nCntBlock;
					}
					else if (pPos->x - (fWidth / 2) <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth && g_Block[nCntBlock].pos.x <= pPos->x - (fWidth / 2))
					{// 右からの当たり判定
						if (collision == COLLISIONBLOCKRETURN_OTHER || collision == COLLISIONBLOCKRETURN_BULLET)
						{
							bLand = true;
						}
						pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + (fWidth / 2);
						pMove->x = 0.0f;
						BlockCheck = nCntBlock;
					}
				}
				else
				{// 当たっていない
					g_Block[nCntBlock].bBlockScroll = false;
				}
			}
			else
			{// 当たっていない
				g_Block[nCntBlock].bBlockScroll = false;
			}
		}

	}
	if (BlockCheck != NULL && (collision == COLLISIONBLOCKRETURN_PLAYER || collision == COLLISIONBLOCKRETURN_BULLET))
	{// 当たっている
		*pBlock = &g_Block[BlockCheck];
	}

	return bLand;		// 値を返す
}

//=========================================================================================================================
// スクロール判定処理
//=========================================================================================================================
bool GetScrollNum(void)
{
	return g_bScroll;
}

//=========================================================================================================================
// ブロック破棄処理
//=========================================================================================================================
void DeleteBlock(int nCntBlock)
{
	g_Block[nCntBlock].bUse = false;
}
void DeleteBlock(void)
{
	// 変数宣言
	int nCntBlock;		// ブロックカウンタ

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// ブロックをカウント
		if (g_Block[nCntBlock].bUse == true)
		{// 使用している場合
			g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
			g_Block[nCntBlock].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
			g_Block[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動値
			g_Block[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
			g_Block[nCntBlock].fWidth = 0.0f;							// 横幅
			g_Block[nCntBlock].fHeight = 0.0f;							// 高さ
			g_Block[nCntBlock].type = BLOCKTYPE_NONE;					// ブロック種類
			g_Block[nCntBlock].nLife = 0;								// ブロック耐久値
			g_Block[nCntBlock].nCntMove = 0;							// 移動カウンタ
			g_Block[nCntBlock].nCntMoveLimit = 0;						// 移動制限
			g_Block[nCntBlock].bBlockScroll = false;					// ブロックに当たっていない
			g_Block[nCntBlock].bUse = false;							// 使用していない
			g_Block[nCntBlock].texture = BLOCKTEXTURE_GROUND;			// テクスチャ
		}
	}
	g_bScroll = false;			// スクロール判定
}