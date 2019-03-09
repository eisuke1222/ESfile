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
#include "effect.h"		// エフェクト
#include "score.h"		// スコア
#include "player.h"		// プレイヤー
#include "enemy.h"		// 敵

//**********************************************************************************************************************
// マクロ定義
//**********************************************************************************************************************
#define	MAX_BULLET				(1080)									// 弾の最大数
#define	MAX_BULLET_TEXTURE		(2)										// テクスチャの最大数
#define BULLET_TEXTURE_NAME1	"data\\TEXTURE\\bullet003.png"			// 読み込むテクスチャファイル1
#define BULLET_TEXTURE_NAME2	"data\\TEXTURE\\bullet002.png"			// 読み込むテクスチャファイル2
#define BULLET_TEXTURE_UV_U		(1.0f)									// 弾アニメーションU範囲
#define BULLET_TEXTURE_UV_V		(1.0f)									// 弾アニメーションV範囲

//**********************************************************************************************************************
// グローバル変数宣言
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureBullet[MAX_BULLET_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffBullet = NULL;					// 頂点バッファへのポインタ
Bullet							g_aBullet[MAX_BULLET];						// 弾の情報

//=========================================================================================================================
// 弾初期化処理
//=========================================================================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntBullet;		// 弾カウント

	// 弾の情報の初期化
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// 弾をカウント
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 弾中心値
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 弾移動値
		g_aBullet[nCntBullet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 弾色
		g_aBullet[nCntBullet].Type = BULLETTYPE_PLAYER1;				// 弾種類
		g_aBullet[nCntBullet].moveType = BULLETMOVE_NORMAL;				// 弾移動種類
		g_aBullet[nCntBullet].nCntChange = 0;							// 弾移動変更
		g_aBullet[nCntBullet].nCntPoint= 0;								// 弾位置回数
		g_aBullet[nCntBullet].bUse = false;								// 弾使用していない状態
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(// プレイヤー
		pDevice,
		BULLET_TEXTURE_NAME1,
		&g_pTextureBullet[0]
	);
	D3DXCreateTextureFromFile
	(// 敵
		pDevice,
		BULLET_TEXTURE_NAME2,
		&g_pTextureBullet[1]
	);

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
	int nCntTex;

	// テクスチャーの破棄
	for (nCntTex = 0; nCntTex < MAX_BULLET_TEXTURE; nCntTex++)
	{// テクスチャをカウント
		if (g_pTextureBullet[nCntTex] != NULL)
		{// NULL以外の場合
			g_pTextureBullet[nCntTex]->Release();	// 開放
			g_pTextureBullet[nCntTex] = NULL;		// NULLへ
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBullet!= NULL)
	{// NULL以外の場合
		g_pVtxBuffBullet->Release();	// 開放
		g_pVtxBuffBullet = NULL;		// NULLへ
	}
}

//=========================================================================================================================
// 弾更新処理
//=========================================================================================================================
void UpdateBullet(void)
{
	int nCntBullet;					// 弾カウント
	int nExplosionSize = 30;		// 爆発サイズ

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
		{// 弾が使用されている場合
			// 移動値代入
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
		//	SetBulletEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].col, EFFECTTYPE_BULLET, 10.0f, 10);		// エフェクトをセット

			if (g_aBullet[nCntBullet].Type == BULLETTYPE_PLAYER1 || g_aBullet[nCntBullet].Type == BULLETTYPE_PLAYER2 || g_aBullet[nCntBullet].Type == BULLETTYPE_PLAYER3)
			{// プレイヤーの弾
				int nCntEnemy;					// 敵カウント
				Enemy *pEnemy = GetEnemy();		// 敵の取得

				if (g_aBullet[nCntBullet].moveType == BULLETMOVE_MOVECHANGE_STRAIGHT)
				{// まっすぐ
					g_aBullet[nCntBullet].nCntChange++;		// カウント

					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// 指定した値と同じだったら
						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI) * g_aBullet[nCntBullet].fSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI) * g_aBullet[nCntBullet].fSpeed;
					}
				}

				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{// 敵をカウント
					if (pEnemy->bUse == true)
					{// 敵がいる場合
						if (pEnemy->Type == ENEMYTYPE_TUTORIALENEMY || pEnemy->Type == ENEMYTYPE_NORMALENEMY)
						{// ENEMYTYPE_TUTORIALENEMY, ENEMYTYPE_NORMALENEMYの場合
							if (pEnemy->pos.x - pEnemy->nSize < g_aBullet[nCntBullet].pos.x && pEnemy->pos.x + pEnemy->nSize > g_aBullet[nCntBullet].pos.x && pEnemy->pos.y - pEnemy->nSize < g_aBullet[nCntBullet].pos.y && pEnemy->pos.y + pEnemy->nSize > g_aBullet[nCntBullet].pos.y)
							{// 当たり判定設定
								SetParticleEffect(g_aBullet[nCntBullet].pos, EFFECTTYPE_EXPLOSION, 50, 8, 20);		// エフェクトをセット

								// 敵体力減少
								g_aBullet[nCntBullet].bUse = false;		// 使用していない状態
								HitEnemy(nCntEnemy, 1);					// ヒット処理
								AddScore(SCORETYPE_RNKING_NO1, 1);		// スコア処理
							}
						}
						else if ((pEnemy->Type == ENEMYTYPE_TUTORIALBOSSENEMY || pEnemy->Type == ENEMYTYPE_SBOSSENEMY || pEnemy->Type == ENEMYTYPE_MBOSSENEMY || pEnemy->Type == ENEMYTYPE_LBOSSENEMY) && pEnemy->move.y == 0)
						{// ENEMYTYPE_BOSSENEMYの場合
							if (pEnemy->pos.x - pEnemy->nSize < g_aBullet[nCntBullet].pos.x && pEnemy->pos.x + pEnemy->nSize > g_aBullet[nCntBullet].pos.x && pEnemy->pos.y - pEnemy->nSize < g_aBullet[nCntBullet].pos.y && pEnemy->pos.y + pEnemy->nSize > g_aBullet[nCntBullet].pos.y)
							{// 当たり判定設定
								SetParticleEffect(g_aBullet[nCntBullet].pos, EFFECTTYPE_EXPLOSION, 50, 12, 30);	// エフェクトをセット

								// 敵体力減少
								g_aBullet[nCntBullet].bUse = false;		// 使用していない状態
								HitEnemy(nCntEnemy, 1);					// ヒット処理
								AddScore(SCORETYPE_RNKING_NO1, 1);		// スコア処理
							}
						}
					}
				}
			}
			else if (g_aBullet[nCntBullet].Type == BULLETTYPE_ENEMY || g_aBullet[nCntBullet].Type == BULLETTYPE_BOSSENEMY)
			{// 敵の弾
				Player *pPlayer = GetPlayer();	// プレイヤー取得

				if (g_aBullet[nCntBullet].moveType == BULLETMOVE_MOVECHANGE_HOMING)
				{// ホーミング
					g_aBullet[nCntBullet].nCntChange++;		// カウント

					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint - 30)
					{// 指定した値と同じだったら
						g_aBullet[nCntBullet].move = D3DXVECTOR3(0, 0, 0);
					}
					else if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// 指定した値と同じだったら
						g_aBullet[nCntBullet].move.x = sinf(atan2f(-g_aBullet[nCntBullet].pos.x - -pPlayer->pos.x, -g_aBullet[nCntBullet].pos.y - -pPlayer->pos.y)) * g_aBullet[nCntBullet].fSpeed;
						g_aBullet[nCntBullet].move.y = cosf(atan2f(-g_aBullet[nCntBullet].pos.x - -pPlayer->pos.x, -g_aBullet[nCntBullet].pos.y - -pPlayer->pos.y)) * g_aBullet[nCntBullet].fSpeed;
					}
				}
				else if (g_aBullet[nCntBullet].moveType == BULLETMOVE_MOVECHANGE_STRAIGHT)
				{// まっすぐ
					g_aBullet[nCntBullet].nCntChange++;		// カウント

					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// 指定した値と同じだったら
						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI * 0) * g_aBullet[nCntBullet].fSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI * 0) * g_aBullet[nCntBullet].fSpeed;
					}
				}
				else if (g_aBullet[nCntBullet].moveType == BULLETMOVE_MOVECHANGE_START_1 || g_aBullet[nCntBullet].moveType == BULLETMOVE_MOVECHANGE_START_2)
				{// スタート
					g_aBullet[nCntBullet].nCntChange++;		// カウント

 					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// 指定した値と同じだったら
						//g_aBullet[nCntBullet].move = D3DXVECTOR3(0, 0, 0);
					}
					else if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint + 30)
					{// 指定した値と同じだったら
						g_aBullet[nCntBullet].move.x *= -1;
						g_aBullet[nCntBullet].move.y *= -1;
					}
				}
				else if (g_aBullet[nCntBullet].moveType == BULLETMOVE_SETBULLET)
				{// 弾追加
					g_aBullet[nCntBullet].nCntChange++;		// カウント

					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// 指定した値と同じだったら
						EnemyBullet *pEnemyBullet = GetEnemyBullet();

						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI * 0.25f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI * 0.25f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.8f, 1.0f, 0.f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI * 0.75f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI * 0.75f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.8f, 1.0f, 0.1f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(-D3DX_PI * 0.25f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(-D3DX_PI * 0.25f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.8f, 1.0f, 0.1f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(-D3DX_PI * 0.75f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(-D3DX_PI * 0.75f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.8f, 1.0f, 0.1f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
					}
				}
				else if (g_aBullet[nCntBullet].moveType == BULLETMOVE_SETBULLET_1)
				{// 弾追加1
					g_aBullet[nCntBullet].nCntChange++;		// カウント

					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// 指定した値と同じだったら
						EnemyBullet *pEnemyBullet = GetEnemyBullet();

						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI * 0.25f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI * 0.25f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.1f, 1.0f, 0.8f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI * 0.75f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI * 0.75f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.1f, 1.0f, 0.8f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(-D3DX_PI * 0.25f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(-D3DX_PI * 0.25f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.1f, 1.0f, 0.8f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(-D3DX_PI * 0.75f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(-D3DX_PI * 0.75f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.1f, 1.0f, 0.8f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
					}
				}

				if (pPlayer->State != PLAYERSTATE_INVINCIBLE && pPlayer->bDisp == true)
				{// プレイヤーがいる場合
					if (pPlayer->pos.x - 8 < g_aBullet[nCntBullet].pos.x && pPlayer->pos.x + 8 > g_aBullet[nCntBullet].pos.x && pPlayer->pos.y - 8 < g_aBullet[nCntBullet].pos.y && pPlayer->pos.y + 8 > g_aBullet[nCntBullet].pos.y)
					{// 当たり判定設定
						HitPlayer(1);	// ヒット処理
						g_aBullet[nCntBullet].bUse = false;		// 使用していない状態
					}
				}
			}

			if (g_aBullet[nCntBullet].pos.y < -200 || g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT + 200 || g_aBullet[nCntBullet].pos.x < -200 || g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH + 200)
			{// 画面外の場合
				g_aBullet[nCntBullet].bUse = false;		// 使用していない状態
			}

			// 頂点設定
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 7, g_aBullet[nCntBullet].pos.y - 7, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 7, g_aBullet[nCntBullet].pos.y - 7, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 7, g_aBullet[nCntBullet].pos.y + 7, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 7, g_aBullet[nCntBullet].pos.y + 7, 0.0f);
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
			pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].nTexNum]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//=========================================================================================================================
// 弾の設定処理
//=========================================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE Type, BULLETMOVE moveType, float fSpeed, int nCntPoint, int nTexNum)
{
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
		{// 弾が使用されていない場合
			g_aBullet[nCntBullet].pos			= pos;			// 位置設定
			g_aBullet[nCntBullet].move			= move;			// 移動値設定
			g_aBullet[nCntBullet].col			= col;			// 色設定
			g_aBullet[nCntBullet].Type			= Type;			// 種類設定
			g_aBullet[nCntBullet].moveType		= moveType;		// 移動種類設定
			g_aBullet[nCntBullet].fSpeed		= fSpeed;		// 移動速度設定
			g_aBullet[nCntBullet].nCntChange	= 0;			// 移動変更設定
			g_aBullet[nCntBullet].nCntPoint		= nCntPoint;	// 位置回数設定
			g_aBullet[nCntBullet].nTexNum		= nTexNum;		// テクスチャ番号設定
			g_aBullet[nCntBullet].bUse			= true;			// 使用している状態

			// 頂点設定
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 7, g_aBullet[nCntBullet].pos.y - 7, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 7, g_aBullet[nCntBullet].pos.y - 7, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 7, g_aBullet[nCntBullet].pos.y + 7, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 7, g_aBullet[nCntBullet].pos.y + 7, 0.0f);
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