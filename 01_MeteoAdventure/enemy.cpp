//=========================================================================================================================
// 
// 敵処理 [enemy.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "enemy.h"			// 敵
#include "score.h"			// スコア
#include "player.h"			// プレイヤー
#include "block.h"			// ブロック
#include "bullet.h"			// 弾
#include "sound.h"			// サウンド

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define ENEMY_TEXTURE_SPEED				(9)									// テクスチャアニメーションスピード
#define ENEMY_TEXTURE_PATTERN			(2)									// テクスチャアニメーションパターン数
#define ENEMY_TEXTURE_UV_U				(0.2f)								// テクスチャアニメーションU範囲
#define ENEMY_TEXTURE_UV_V				(0.33f)								// テクスチャアニメーションV範囲
#define ENEMY_UPSPEED					(1.2f)								// プレイヤー加速速度
#define ENEMY_DOWNSPEED					(0.2f)								// プレイヤー減速速度
#define ENEMY_JUMPSPEED					(1.2f)								// プレイヤー上昇速度
#define ENEMY_GRAVITY					(0.5f)								// プレイヤー重力

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void EnemyCollisionRange(int nCntEnemy, D3DXVECTOR3 *pRange);					// 衝突範囲設定

//*************************************************************************************************************************
// グローバル変数宣言
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_apTextureEnemy[ENEMYTEXTURE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffEnemy = NULL;							// 頂点バッファへのポインタ
Enemy						g_aEnemy[MAX_ENEMY];							// 敵の情報
int							g_nNumEnemy;									// 敵の総数

//=========================================================================================================================
// 敵初期化処理
//=========================================================================================================================
void InitEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	int nCntEnemy;		// 敵カウンタ

	// 敵の情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵をカウント
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 中心値
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 中心値
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 移動値
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
		g_aEnemy[nCntEnemy].fWidth = 0.0f;									// 横幅
		g_aEnemy[nCntEnemy].fHeight = 0.0f;									// 高さ
		g_aEnemy[nCntEnemy].nLife = 0;										// 体力
		g_aEnemy[nCntEnemy].Type = ENEMYTYPE_NORMALENEMY;					// 種類
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_APPEAR;						// 状態
		g_aEnemy[nCntEnemy].nCounterState = 0;								// 状態カウンタ
		g_aEnemy[nCntEnemy].AnimType = ENEMYANIMTYPE_NORMAL;				// アニメーション種類
		g_aEnemy[nCntEnemy].nCounterAnim = 0;								// アニメーションカウンタ
		g_aEnemy[nCntEnemy].nNumAnim = 0;									// アニメーション番号
		g_aEnemy[nCntEnemy].nSpeedAnim = 0;									// アニメーション速度
		g_aEnemy[nCntEnemy].nPatternAnim = 0;								// アニメーションパターン
		g_aEnemy[nCntEnemy].nDinectionMove = 0;								// 方向
		g_aEnemy[nCntEnemy].nCntTimer = 0;									// 時間カウンタ
		g_aEnemy[nCntEnemy].texture = ENEMYTEXTURE_SLIME;					// テクスチャ
		g_aEnemy[nCntEnemy].bUse = false;									// 使用しているか
	}

	g_nNumEnemy = 0;					// 敵総数

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_NAME_1, &g_apTextureEnemy[0]);		// スライム
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_NAME_2, &g_apTextureEnemy[1]);		// 雪だるま
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_NAME_3, &g_apTextureEnemy[2]);		// のろい
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_NAME_4, &g_apTextureEnemy[3]);		// 鳥
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_NAME_5, &g_apTextureEnemy[4]);		// ゴースト

	// 頂点バッファを生成
    pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock
	(
		0, 
		0, 
		(void**)&pVtx, 
		0
	);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵をカウント
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
		pVtx[1].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, ENEMY_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U, ENEMY_TEXTURE_UV_V);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=========================================================================================================================
// 敵終了処理
//=========================================================================================================================
void UninitEnemy(void)
{
	// 変数宣言
	int nCntTex;		// テクスチャカウンタ

	// テクスチャの破棄
	for(nCntTex = 0; nCntTex < ENEMYTEXTURE_MAX; nCntTex++)
	{// テクスチャをカウント
		if (g_apTextureEnemy[nCntTex] != NULL)
		{
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=========================================================================================================================
// 敵更新処理
//=========================================================================================================================
void UpdateEnemy(void)
{
	// 変数宣言
	int nCntEnemy;						// 敵カウント
	PLAYER *pPlayer = GetPlayer();		// プレイヤー情報
	D3DXVECTOR3 Range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 当たり判定
	MODE mode = GetMode();

	VERTEX_2D *pVtx;					// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffEnemy->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵をカウント
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// 使用されている場合
			switch (g_aEnemy[nCntEnemy].State)
			{
			case ENEMYSTATE_NORMAL:
				break;
			case ENEMYSTATE_DAMAGE:

				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;
					g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				break;
			case ENEMYSTATE_DEATH:
				break;
			}

			g_aEnemy[nCntEnemy].nCounterAnim++;		// カウンタを進める

			if (g_aEnemy[nCntEnemy].pos.x >= pPlayer->pos.x)
			{// 左向き
				g_aEnemy[nCntEnemy].nDinectionMove = 1;
			}
			else
			{// 右向き
				g_aEnemy[nCntEnemy].nDinectionMove = 0;
			}

			// 位置を記憶
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;
			// 画面スクロール
			g_aEnemy[nCntEnemy].pos.x -= pPlayer->move.x / 2;
			
			if (g_aEnemy[nCntEnemy].texture == ENEMYTEXTURE_BIRD)
			{
				// 重力加算
				g_aEnemy[nCntEnemy].move.y += pPlayer->fGravity / 2;
			}
			else if (g_aEnemy[nCntEnemy].texture != ENEMYTEXTURE_GHOST)
			{
				// 重力加算
				g_aEnemy[nCntEnemy].move.y += pPlayer->fGravity;
			}
		
			// 移動値代入
			g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
			g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;
			// 減速処理
			g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * ENEMY_DOWNSPEED;

			if (g_aEnemy[nCntEnemy].texture != ENEMYTEXTURE_GHOST)
			{
				// ブロックとの衝突処理
				CollisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].fHeight, NULL, COLLISIONBLOCKRETURN_ENEMY);
			}

			if (mode == MODE_GAME)
			{// 衝突判定調整
				EnemyCollisionRange(nCntEnemy, &Range);

				if (CollisiomBullet(&g_aEnemy[nCntEnemy].pos, Range.x, Range.y, COLLISIONBULLETRETURN_OTHER) == true && g_aEnemy[nCntEnemy].AnimType != ENEMYANIMTYPE_KNOCKBACK)
				{// 弾に当たった場合
					g_aEnemy[nCntEnemy].nLife--;		// 体力を減らす

					g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 0.5f, 0.3f, 1.0f);
					g_aEnemy[nCntEnemy].nCounterState = 15;
					g_aEnemy[nCntEnemy].State = ENEMYSTATE_DAMAGE;

					PlaySound(SOUND_LABEL_SE_HIT000);
					if (g_aEnemy[nCntEnemy].nLife <= 0)
					{// 体力が0以下の場合
						// ノックバック
						g_aEnemy[nCntEnemy].nNumAnim = 0;
						g_aEnemy[nCntEnemy].AnimType = ENEMYANIMTYPE_KNOCKBACK;
						AddScore(SCORETYPE_SCORE, (g_aEnemy[nCntEnemy].texture + 1) * 300);

						// 移動値代入
						if (g_aEnemy[nCntEnemy].nDinectionMove == 1) { g_aEnemy[nCntEnemy].move.x += 20; }
						else { g_aEnemy[nCntEnemy].move.x -= 20; }
						g_aEnemy[nCntEnemy].move.y -= 5;
					}
				}
				else if (g_aEnemy[nCntEnemy].AnimType == ENEMYANIMTYPE_KNOCKBACK)
				{// ノックバックの場合
					g_aEnemy[nCntEnemy].nCntTimer++;	// カウンタを進める

					if (g_aEnemy[nCntEnemy].nCntTimer > 30)
					{// 0.5秒後
						DeleteEnemy(nCntEnemy);		//敵破棄処理
					}
				}

				if (g_aEnemy[nCntEnemy].AnimType != ENEMYANIMTYPE_KNOCKBACK)
				{
					if (g_aEnemy[nCntEnemy].texture == ENEMYTEXTURE_SLIME)
					{
						if (pPlayer->pos.x + pPlayer->fWidth > g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth - 500 &&
							pPlayer->pos.x - pPlayer->fWidth < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth + 500)
						{
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y)) * 0.5f;
						}
					}
					else if (g_aEnemy[nCntEnemy].texture == ENEMYTEXTURE_BIRD)
					{
						if (g_aEnemy[nCntEnemy].nCounterAnim % 240 == 0)
						{
							g_aEnemy[nCntEnemy].move.y -= 8.0f;
						}
					}
					else if (g_aEnemy[nCntEnemy].texture == ENEMYTEXTURE_GHOST)
					{
						if (pPlayer->pos.x + pPlayer->fWidth > g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth - 500 &&
							pPlayer->pos.x - pPlayer->fWidth < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth + 500)
						{
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y)) * 0.8f;
							g_aEnemy[nCntEnemy].move.y = cosf(atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y)) * 0.8f;
						}
					}
				}
			}

			if (g_aEnemy[nCntEnemy].nCounterAnim % 9 == 0 && g_aEnemy[nCntEnemy].AnimType != ENEMYANIMTYPE_KNOCKBACK)
			{// アニメーション
				g_aEnemy[nCntEnemy].nNumAnim = (g_aEnemy[nCntEnemy].nNumAnim + 1) % 5;
			}

			// 頂点設定
			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight, 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight, 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			// 頂点カラー設定
			pVtx[nCntEnemy * 4].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 1].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 2].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 3].col = g_aEnemy[nCntEnemy].col;

			// テクスチャ設定
			if (g_aEnemy[nCntEnemy].nDinectionMove == 0)
			{// 左
				pVtx[nCntEnemy * 4].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);
			}
			else
			{// 右
				pVtx[nCntEnemy * 4].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);
			}
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=========================================================================================================================
// 敵描画処理
//=========================================================================================================================
void DrawEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntEnemy;		// 敵カウント

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffEnemy,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵をカウント
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// 使用している場合
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].texture]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=========================================================================================================================
// 敵の設定処理
//=========================================================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nLife, ENEMYTYPE Type, ENEMYTEXTURE texture)
{
	// 変数宣言
	int nCntEnemy;				// 敵カウント

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffEnemy->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵をカウント
		if (g_aEnemy[nCntEnemy].bUse == false)
		{// 使用していない場合
			g_aEnemy[nCntEnemy].pos = pos;									// 位置設定
			g_aEnemy[nCntEnemy].move = move;								// 移動値設定
			g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
			g_aEnemy[nCntEnemy].fWidth = fWidth;							// 横幅
			g_aEnemy[nCntEnemy].fHeight = fHeight;							// 高さ
			g_aEnemy[nCntEnemy].nLife = nLife;								// 体力設定
			g_aEnemy[nCntEnemy].Type = Type;								// 敵種類を設定
			g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;					// 状態を設定
			g_aEnemy[nCntEnemy].nCounterState = 0;							// 状態カウンタ初期化
			g_aEnemy[nCntEnemy].AnimType = ENEMYANIMTYPE_NORMAL;			// アニメーション種類
			g_aEnemy[nCntEnemy].nCounterAnim = 0;							// カウンター初期化
			g_aEnemy[nCntEnemy].nNumAnim = 0;								// アニメーション番号初期化
			g_aEnemy[nCntEnemy].nSpeedAnim = 0;								// アニメーション速度初期化
			g_aEnemy[nCntEnemy].nPatternAnim = 0;							// アニメーションパターン初期化
			g_aEnemy[nCntEnemy].nCntTimer = 0;								// 時間カウンタ
			g_aEnemy[nCntEnemy].texture = texture;							// テクスチャ
			g_aEnemy[nCntEnemy].bUse = true;								// 使用している状態にする

			g_nNumEnemy++;													// 敵総数をカウント

			// 頂点設定
			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight, 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight, 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			// テクスチャ設定
			pVtx[nCntEnemy * 4].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U, ENEMYANIMTYPE_NORMAL * ENEMY_TEXTURE_UV_V);
			pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, ENEMYANIMTYPE_NORMAL * ENEMY_TEXTURE_UV_V);
			pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U, ENEMYANIMTYPE_NORMAL * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);
			pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, ENEMYANIMTYPE_NORMAL * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);

			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=========================================================================================================================
// 敵の取得処理
//=========================================================================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];		// 値を返す
}

//=========================================================================================================================
// 敵との衝突処理
//=========================================================================================================================
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	// 変数宣言
	int nCntEnemy;		// 敵カウンタ
	bool bHit = false;	// 敵に当たったか
	D3DXVECTOR3 Range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 当たり判定

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵をカウント
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// 使用している場合
			// 衝突判定調整
			EnemyCollisionRange(nCntEnemy, &Range);

			if (pPos->x + (fWidth / 2 * 0.8f) >= g_aEnemy[nCntEnemy].pos.x - Range.x && pPos->x - (fWidth / 2 * 0.8f) <= g_aEnemy[nCntEnemy].pos.x + Range.x &&
				pPos->y - (fHeight * 0.8f)<= g_aEnemy[nCntEnemy].pos.y && pPos->y >= g_aEnemy[nCntEnemy].pos.y - Range.y)
			{
				// ノックバック設定
				pMove->x = 0.0f;

				if (pMove->y <= 0.0f)
				{
					pMove->y *= -1;
				}
				if (pPos->x < g_aEnemy[nCntEnemy].pos.x)
				{
					pMove->x -= 50;
				}
				else
				{
					pMove->x += 50;
				}

				bHit = true;	// 当たり判定
			}
		}
	}

	return bHit;		// 値を返す
}

//=========================================================================================================================
// 敵の破棄処理
//=========================================================================================================================
void DeleteEnemy(int nCntEnemy)
{// １つ
	g_aEnemy[nCntEnemy].bUse = false;		// 敵を破棄
}
void DeleteEnemy(void)
{// 全部
	// 変数宣言
	int nCntEnemy;		// 敵カウンタ

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵をカウント
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// 使用されている場合
			g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 中心値
			g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 中心値
			g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 移動値
			g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
			g_aEnemy[nCntEnemy].fWidth = 0.0f;									// 横幅
			g_aEnemy[nCntEnemy].fHeight = 0.0f;									// 高さ
			g_aEnemy[nCntEnemy].nLife = 0;										// 体力
			g_aEnemy[nCntEnemy].Type = ENEMYTYPE_NORMALENEMY;					// 種類
			g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;						// 状態
			g_aEnemy[nCntEnemy].nCounterState = 0;								// 状態カウンタ
			g_aEnemy[nCntEnemy].AnimType = ENEMYANIMTYPE_NORMAL;				// アニメーション種類
			g_aEnemy[nCntEnemy].nCounterAnim = 0;								// アニメーションカウンタ
			g_aEnemy[nCntEnemy].nNumAnim = 0;									// アニメーション番号
			g_aEnemy[nCntEnemy].nSpeedAnim = 0;									// アニメーション速度
			g_aEnemy[nCntEnemy].nPatternAnim = 0;								// アニメーションパターン
			g_aEnemy[nCntEnemy].nDinectionMove = 0;								// 方向
			g_aEnemy[nCntEnemy].nCntTimer = 0;									// 時間カウンタ
			g_aEnemy[nCntEnemy].texture = ENEMYTEXTURE_SLIME;					// テクスチャ
			g_aEnemy[nCntEnemy].bUse = false;									// 使用しているか

		}
	}
	g_nNumEnemy = 0;	// 敵総数
}

//==============================================================
// 衝突範囲調整
//==============================================================
void EnemyCollisionRange(int nCntEnemy, D3DXVECTOR3 *pRange)
{
	switch (g_aEnemy[nCntEnemy].texture)
	{// テクスチャ種類
	case ENEMYTEXTURE_SLIME:		// スライム
		
		pRange->x = 30;
		pRange->y = 30;
		
		break;
	case ENEMYTEXTURE_SNOWMAN:		// 雪だるま
		
		pRange->x = 40;
		pRange->y = 60;

		break;
	case ENEMYTEXTURE_CURSE:		// のろい
		
		pRange->x = 30;
		pRange->y = 35;

		break;
	case ENEMYTEXTURE_BIRD:			// 鳥

		pRange->x = 45;
		pRange->y = 60;

		break;
	case ENEMYTEXTURE_GHOST:		// ゴースト

		pRange->x = 45;
		pRange->y = 60;

		break;
	}
}