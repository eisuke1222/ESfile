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
#include "bullet.h"			// 弾
#include "explosion.h"		// 爆発
#include "effect.h"			// エフェクト
#include "score.h"			// スコア
#include "player.h"			// プレイヤー
#include "item.h"			// アイテム
#include "sound.h"			// サウンド

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	MAX_ENEMY_TEXTURE				(5)									// 敵の種類
#define ENEMY_TEXTURE_NAME_1			"data\\TEXTURE\\enemy000.png"		// 読み込むテクスチャファイル1
#define ENEMY_TEXTURE_NAME_2			"data\\TEXTURE\\enemy001.png"		// 読み込むテクスチャファイル2
#define ENEMY_TEXTURE_NAME_3			"data\\TEXTURE\\enemy002.png"		// 読み込むテクスチャファイル3
#define ENEMY_TEXTURE_NAME_4			"data\\TEXTURE\\enemy003.png"		// 読み込むテクスチャファイル4
#define ENEMY_TEXTURE_NAME_5			"data\\TEXTURE\\enemy004.png"		// 読み込むテクスチャファイル5
#define ENEMY_TEXTURE_SPEED				(15)								// テクスチャアニメーションスピード
#define ENEMY_TEXTURE_PATTERN			(2)									// テクスチャアニメーションパターン数
#define ENEMY_TEXTURE_UV_U				(0.5f)								// テクスチャアニメーションU範囲
#define ENEMY_TEXTURE_UV_V				(1.0f)								// テクスチャアニメーションV範囲
#define ENEMY_SPEED						(10)								// 敵移動速度

#define MAX_ENEMYBULLET_TYPE			(5)									// 敵弾の発射パターン
#define ENEMYBULLET_ANGLE				(0.1f)								// 敵弾角度

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void EnwmyBulletType(int nCntEnemy);			// 弾発射種類
void EnwmyItemSet(int nCntEnemy);				// アイテムセット

//*************************************************************************************************************************
// グローバル変数宣言
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_apTextureEnemy[MAX_ENEMY_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffEnemy = NULL;							// 頂点バッファへのポインタ
Enemy						g_aEnemy[MAX_ENEMY];							// 敵の情報
EnemyBullet					g_aEnemyBullet[MAX_ENEMY][MAX_ENEMYBULLET_TYPE];// 敵弾の情報
int							g_nNumEnemy;									// 敵の総数

//=========================================================================================================================
// 敵初期化処理
//=========================================================================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得
	int nCntEnemy;		// 敵カウント

	// 敵の情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// 敵をカウント
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 敵中心値
		g_aEnemy[nCntEnemy].posTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目標位置
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// 敵移動値
		g_aEnemy[nCntEnemy].moveSpeed = 0;									// 移動速度
		g_aEnemy[nCntEnemy].moveType = ENEMYMOVE_NONE;						// 移動種類
		g_aEnemy[nCntEnemy].moveStart = 0;									// 移動開始時間
		g_aEnemy[nCntEnemy].moveLimit = 0;									// 移動回数
		g_aEnemy[nCntEnemy].nCntTarget = 0;									// 目標位置回数
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 敵色
		g_aEnemy[nCntEnemy].nType = 0;										// 敵種類
		g_aEnemy[nCntEnemy].nLife = 0;										// 敵体力
		g_aEnemy[nCntEnemy].Type = ENEMYTYPE_NORMALENEMY;					// 敵クラス
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_NOMAL;						// 敵状態
		g_aEnemy[nCntEnemy].nCounterState = 0;								// 敵状態カウント
		g_aEnemy[nCntEnemy].nCounterAnim = 0;								// 敵カウンタアニメーション
		g_aEnemy[nCntEnemy].nPatternAnim = 0;								// 敵パターンアニメーション
		g_aEnemy[nCntEnemy].bUse = false;									// 敵使用しているか

		// 敵の弾情報の初期化
		for (int nCount = 0; nCount < MAX_ENEMYBULLET_TYPE; nCount++)
		{// 敵弾のパターン分カウント
			g_aEnemyBullet[nCntEnemy][nCount].nPattenType = ENEMYBULLET_NONE;			// 敵弾発射パターン
			g_aEnemyBullet[nCntEnemy][nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 敵弾移動速度
			g_aEnemyBullet[nCntEnemy][nCount].moveSpeed = 0.0f;							// 敵弾移動速度
			g_aEnemyBullet[nCntEnemy][nCount].nTiming = 0;								// 敵弾発射タイミング
			g_aEnemyBullet[nCntEnemy][nCount].Type = BULLETTYPE_ENEMY;					// 敵弾種類
			g_aEnemyBullet[nCntEnemy][nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 敵弾移動値
			g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 敵弾色
			g_aEnemyBullet[nCntEnemy][nCount].nCounterInput = 0;						// 敵弾入力カウンタ
			g_aEnemyBullet[nCntEnemy][nCount].nAngle = 0.0f;							// 敵弾角度
		}
	}

	g_nNumEnemy = 0;			// 敵総数

	// テクスチャの読み込み(４)
	D3DXCreateTextureFromFile
	(// 敵1
		pDevice,
		ENEMY_TEXTURE_NAME_1,
		&g_apTextureEnemy[0]
	);
	D3DXCreateTextureFromFile
	(// 敵2
		pDevice,
		ENEMY_TEXTURE_NAME_2,
		&g_apTextureEnemy[1]
	);
	D3DXCreateTextureFromFile
	(// 敵3
		pDevice,
		ENEMY_TEXTURE_NAME_3,
		&g_apTextureEnemy[2]
	);
	D3DXCreateTextureFromFile
	(// 敵4
		pDevice,
		ENEMY_TEXTURE_NAME_4,
		&g_apTextureEnemy[3]
	);
	D3DXCreateTextureFromFile
	(// 敵4
		pDevice,
		ENEMY_TEXTURE_NAME_5,
		&g_apTextureEnemy[4]
	);

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
	int nCntTex;		// テクスチャカウンタ

	// テクスチャの破棄
	for(nCntTex = 0; nCntTex < MAX_ENEMY_TEXTURE; nCntTex++)
	{// テクスチャカウンタ
		if (g_apTextureEnemy[nCntTex] != NULL)
		{// NULL以外の場合
			g_apTextureEnemy[nCntTex]->Release();	// 開放
			g_apTextureEnemy[nCntTex] = NULL;		// NULLへ
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEnemy != NULL)
	{// NULL以外の場合
		g_pVtxBuffEnemy->Release();		// 開放
		g_pVtxBuffEnemy = NULL;			// NULLへ
	}
}

//=========================================================================================================================
// 敵更新処理
//=========================================================================================================================
void UpdateEnemy(void)
{
	int nCntEnemy;						// 敵カウント
	int nExplosionSize = 40;			// 爆発サイズ

	Player *pPlayer = GetPlayer();		// プレイヤー取得

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
		{// 敵が使用されている場合
			switch (g_aEnemy[nCntEnemy].State)
			{// 敵状態
			case ENEMYSTATE_NOMAL:	// 通常状態
				break;
			case ENEMYSTATE_DAMAGE:	// ダメージ状態
				g_aEnemy[nCntEnemy].nCounterState--;	// 状態をカウント

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{// 状態カウント終了
					// 色変更
					g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// 状態変化
					g_aEnemy[nCntEnemy].State = ENEMYSTATE_NOMAL;
				}
				break;
			// case ENEMYSTATE_DEATH:
			// 	g_aEnemy[nCntEnemy].nCounterState--;	// 状態をカウント

			// 	// if (g_aEnemy[nCntEnemy].nCounterState <= 0)
			// 	// {// 状態カウント終了
			// 		if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
			// 		{
			// 			SetMode(MODE_RESULT);
			// 		}
			// 	}
			// 	return;
			// 	break;
			}

			if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_TUTORIALENEMY)
			{// ENEMYTYPE_TUTORIALENEMYの場合		
				g_aEnemy[nCntEnemy].moveStart--;	// 減らす

				if (g_aEnemy[nCntEnemy].moveStart <= 0)
				{// ０以下になったら
					if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_NORMAL)
					{// 通常
						if (g_aEnemy[nCntEnemy].pos.x >= g_aEnemy[nCntEnemy].posTarget.x - 5 && g_aEnemy[nCntEnemy].pos.x <= g_aEnemy[nCntEnemy].posTarget.x + 5 && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].posTarget.y - 5 && g_aEnemy[nCntEnemy].pos.y <= g_aEnemy[nCntEnemy].posTarget.y + 5)
						{// 範囲チェック
							g_aEnemy[nCntEnemy].bUse = false;
							g_nNumEnemy--;
						}
					}
				}
			
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;	// 移動量代入

				EnwmyBulletType(nCntEnemy);			// 弾発射種類
			}
			else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_TUTORIALBOSSENEMY)
			{// ENEMYTYPE_TUTORIALBOSSENEMYの場合
			 // 移動値代入
			/*	g_aEnemy[nCntEnemy].moveStart--;

				if (g_aEnemy[nCntEnemy].moveStart <= 0)
				{
					if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_NORMAL)
					{

					}
				}

				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

				EnwmyBulletType(nCntEnemy);			// 弾発射種類*/
			}
			else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_NORMALENEMY)
			{// ENEMYTYPE_NORMALENEMYの場合
				// 移動値代入
				g_aEnemy[nCntEnemy].moveStart--;	// 減らす

				if (g_aEnemy[nCntEnemy].moveStart <= 0)
				{// ０以下だったら
					if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_NORMAL)
					{// 通常
						g_aEnemy[nCntEnemy].moveChange = 1;
					}
					else if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_HOMING)
					{// ホーミング
						if (g_aEnemy[nCntEnemy].pos.x >= g_aEnemy[nCntEnemy].posTarget.x - 5 && g_aEnemy[nCntEnemy].pos.x <= g_aEnemy[nCntEnemy].posTarget.x + 5 && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].posTarget.y - 5 && g_aEnemy[nCntEnemy].pos.y <= g_aEnemy[nCntEnemy].posTarget.y + 5)
						{// 範囲チェック
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0, 0, 0);
						}
						else
						{// 移動
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							g_aEnemy[nCntEnemy].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							if (g_aEnemy[nCntEnemy].moveLimit <= 100) { g_aEnemy[nCntEnemy].moveLimit++; }
						}
					}
					else if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_HOMING_KEEP)
					{// ホーミング維持
						if (g_aEnemy[nCntEnemy].pos.x >= g_aEnemy[nCntEnemy].posTarget.x - 5 && g_aEnemy[nCntEnemy].pos.x <= g_aEnemy[nCntEnemy].posTarget.x + 5 && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].posTarget.y - 5 && g_aEnemy[nCntEnemy].pos.y <= g_aEnemy[nCntEnemy].posTarget.y + 5)
						{// 範囲チェック
							g_aEnemy[nCntEnemy].moveChange = 1;
						}
						else if (g_aEnemy[nCntEnemy].moveChange == 0)
						{// 移動
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							g_aEnemy[nCntEnemy].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							if (g_aEnemy[nCntEnemy].moveLimit <= 100) { g_aEnemy[nCntEnemy].moveLimit++; }
						}
					}
					else if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_HOMING_OUT)
					{// ホーミングアウト
						if (g_aEnemy[nCntEnemy].pos.x >= g_aEnemy[nCntEnemy].posTarget.x - 5 && g_aEnemy[nCntEnemy].pos.x <= g_aEnemy[nCntEnemy].posTarget.x + 5 && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].posTarget.y - 5 && g_aEnemy[nCntEnemy].pos.y <= g_aEnemy[nCntEnemy].posTarget.y + 5)
						{// 範囲チェック
							g_aEnemy[nCntEnemy].moveChange++;
							
							if (g_aEnemy[nCntEnemy].moveChange > g_aEnemy[nCntEnemy].nCntTarget)
							{// 一定以上だったら
								if (g_aEnemy[nCntEnemy].pos.x < SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4)
								{
									g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-1.5f, -1.0f, 0);
								}
								else if (g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4)
								{
									g_aEnemy[nCntEnemy].move = D3DXVECTOR3(1.5f, -1.0f, 0);
								}
								else if (g_aEnemy[nCntEnemy].pos.y < SCREEN_HEIGHT / 2)
								{
									g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0, -1.0f, 0);
								}
								else
								{
									g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0, 1.0f, 0);
								}
							}
							else
							{
								g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0, 0, 0);
							}
						}
						else if (g_aEnemy[nCntEnemy].moveChange == 0)
						{
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							g_aEnemy[nCntEnemy].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							if (g_aEnemy[nCntEnemy].moveLimit <= 100) { g_aEnemy[nCntEnemy].moveLimit++; }
						}
					}
				}
				
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;		// 移動量代入

				EnwmyBulletType(nCntEnemy);			// 弾発射種類
			}
			else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_SBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_MBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
			{// ENEMYTYPE_LBOSSENEMYの場合
					 // 移動値代入
				g_aEnemy[nCntEnemy].moveStart--;	// 減らす

				if (g_aEnemy[nCntEnemy].moveStart <= 0)
				{// ０以下だったら
					if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_HOMING)
					{// ホーミング
						if (g_aEnemy[nCntEnemy].pos.x >= g_aEnemy[nCntEnemy].posTarget.x - 5 && g_aEnemy[nCntEnemy].pos.x <= g_aEnemy[nCntEnemy].posTarget.x + 5 && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].posTarget.y - 5 && g_aEnemy[nCntEnemy].pos.y <= g_aEnemy[nCntEnemy].posTarget.y + 5)
						{// 範囲チェック
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0, 0, 0);
						}
						else
						{// 移動
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							g_aEnemy[nCntEnemy].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							if (g_aEnemy[nCntEnemy].moveLimit <= 100) { g_aEnemy[nCntEnemy].moveLimit++; }
						}
					}
				}

				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;	// 移動量代入

				if (g_aEnemy[nCntEnemy].move == D3DXVECTOR3(0, 0, 0))
				{// 移動範囲
					EnwmyBulletType(nCntEnemy);				// 弾発射種類
				}
			}

			if (pPlayer->State != PLAYERSTATE_INVINCIBLE && pPlayer->bDisp == true)
			{// プレイヤーがいる場合
				if (pPlayer->pos.x - 11 < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nSize && pPlayer->pos.x + 11 > g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nSize && pPlayer->pos.y - 17 < g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nSize && pPlayer->pos.y + 17 > g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nSize)
				{// 当たり判定設定
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), (nExplosionSize - 20));	// 爆発
					HitPlayer(1);	// ヒット処理
				}
			}
			
			if ((-300 > g_aEnemy[nCntEnemy].pos.x || SCREEN_WIDTH + 300 < g_aEnemy[nCntEnemy].pos.x || -300 > g_aEnemy[nCntEnemy].pos.y || SCREEN_HEIGHT + 300 < g_aEnemy[nCntEnemy].pos.y) && g_aEnemy[nCntEnemy].moveChange != 0)
			{// 画面外の場合
				g_aEnemy[nCntEnemy].bUse = false;
				g_nNumEnemy--;
			}

			g_aEnemy[nCntEnemy].nCounterAnim++;		// カウンター加算

			if ((g_aEnemy[nCntEnemy].nCounterAnim % ENEMY_TEXTURE_SPEED) == 0)
			{// アニメーション速度
				g_aEnemy[nCntEnemy].nPatternAnim = (g_aEnemy[nCntEnemy].nPatternAnim + 1) % ENEMY_TEXTURE_PATTERN;		// パターンNo.更新

				// テクスチャ設定
				pVtx[nCntEnemy * 4].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
				pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U * g_aEnemy[nCntEnemy].nPatternAnim + ENEMY_TEXTURE_UV_U, 0.0f);
				pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U * g_aEnemy[nCntEnemy].nPatternAnim, ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U * g_aEnemy[nCntEnemy].nPatternAnim + ENEMY_TEXTURE_UV_U, ENEMY_TEXTURE_UV_V);
			}

			// 頂点設定
			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nSize, 0.0f);
			// 頂点カラー設定
			pVtx[nCntEnemy * 4].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 1].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 2].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 3].col = g_aEnemy[nCntEnemy].col;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=========================================================================================================================
// 描画処理
//=========================================================================================================================
void DrawEnemy(void)
{
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
		{// 敵がいる場合
			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=========================================================================================================================
// 敵の設定処理
//=========================================================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, D3DXVECTOR3 move, float moveSpeed, ENEMYMOVE moveType, int moveStart, int nCntTarget, int nType, int nSize, int nLife, ENEMYTYPE Type,
	EnemyBullet bullet1, EnemyBullet bullet2, EnemyBullet bullet3, EnemyBullet bullet4, EnemyBullet bullet5)
{
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
		{// 敵が使用されていない
			g_aEnemy[nCntEnemy].pos = pos;									// 位置設定
			g_aEnemy[nCntEnemy].posTarget = posTarget;						// 目標位置設定
			g_aEnemy[nCntEnemy].move = move;								// 移動値設定
			g_aEnemy[nCntEnemy].moveSpeed = moveSpeed;						// 移動速度設定
			g_aEnemy[nCntEnemy].moveType = moveType;						// 移動種類設定
			g_aEnemy[nCntEnemy].moveStart = moveStart;						// 移動開始時間設定
			g_aEnemy[nCntEnemy].moveLimit = 0;								// 移動回数設定
			g_aEnemy[nCntEnemy].moveChange = 0;								// 移動切替設定
			g_aEnemy[nCntEnemy].nCntTarget = nCntTarget;					// 目標位置回数設定
			g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
			g_aEnemy[nCntEnemy].nType = nType;								// テクスチャ種類設定
			g_aEnemy[nCntEnemy].nSize = nSize;								// 大きさ設定
			g_aEnemy[nCntEnemy].nLife = nLife;								// 体力設定
			g_aEnemy[nCntEnemy].Type = Type;								// 敵種類を設定

			g_aEnemyBullet[nCntEnemy][0].nPattenType = bullet1.nPattenType;	// 弾種類を設定
			g_aEnemyBullet[nCntEnemy][0].moveSpeed = bullet1.moveSpeed;		// 弾移動速度設定
			g_aEnemyBullet[nCntEnemy][0].nTiming = bullet1.nTiming;			// 発射タイミングを設定

			g_aEnemyBullet[nCntEnemy][1].nPattenType = bullet2.nPattenType;	// 弾種類を設定
			g_aEnemyBullet[nCntEnemy][1].moveSpeed = bullet2.moveSpeed;		// 弾移動速度設定
			g_aEnemyBullet[nCntEnemy][1].nTiming = bullet2.nTiming;			// 発射タイミングを設定

			g_aEnemyBullet[nCntEnemy][2].nPattenType = bullet3.nPattenType;	// 弾種類を設定
			g_aEnemyBullet[nCntEnemy][2].moveSpeed = bullet3.moveSpeed;		// 弾移動速度設定
			g_aEnemyBullet[nCntEnemy][2].nTiming = bullet3.nTiming;			// 発射タイミングを設定

			g_aEnemyBullet[nCntEnemy][3].nPattenType = bullet4.nPattenType;	// 弾種類を設定
			g_aEnemyBullet[nCntEnemy][3].moveSpeed = bullet4.moveSpeed;		// 弾移動速度設定
			g_aEnemyBullet[nCntEnemy][3].nTiming = bullet4.nTiming;			// 発射タイミングを設定

			g_aEnemyBullet[nCntEnemy][4].nPattenType = bullet5.nPattenType;	// 弾種類を設定
			g_aEnemyBullet[nCntEnemy][4].moveSpeed = bullet5.moveSpeed;		// 弾移動速度設定
			g_aEnemyBullet[nCntEnemy][4].nTiming = bullet5.nTiming;			// 発射タイミングを設定

			g_aEnemy[nCntEnemy].State = ENEMYSTATE_NOMAL;					// 状態を設定
			g_aEnemy[nCntEnemy].nCounterState = 0;							// 状態カウンタ初期値設定
			g_aEnemy[nCntEnemy].nCounterAnim = 0;							// カウンター初期値設定
			g_aEnemy[nCntEnemy].nPatternAnim = 0;							// パターンNo.初期値設定
			g_aEnemy[nCntEnemy].bUse = true;								// 使用している状態にする

			for (int nCount = 0; nCount < MAX_ENEMYBULLET_TYPE; nCount++)
			{
				g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f);
			}

			// 頂点設定
			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nSize, 0.0f);

			g_nNumEnemy++;	// 敵総数をカウント
			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEnemy->Unlock();
}

//=========================================================================================================================
// 敵の取得
//=========================================================================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];		// 値を返す
}

//=========================================================================================================================
// 敵の取得
//=========================================================================================================================
EnemyBullet *GetEnemyBullet(void)
{
	return &g_aEnemyBullet[0][0];	// 値を返す
}

//=========================================================================================================================
// 敵の弾当たり処理
//=========================================================================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	// 変数宣言
	int ExplosionSize1 = 0;		// nLifeが0以下の爆発サイズ
	int ExplosionSize2 = 0;		// nLifeが1以上の爆発サイズ

	if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_NORMALENEMY)
	{// ENEMYTYPE_NORMALENEMYの爆発
		ExplosionSize1 = 30;
		ExplosionSize2 = 20;
	}
	else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
	{// ENEMYTYPE_LBOSSENEMYの爆発サイズ
		ExplosionSize1 = 100;
		ExplosionSize2 = 0;
	}

	// ライフが減少
	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{// ライフが0以下になった場合
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), ExplosionSize1);	// 爆発

		EnwmyItemSet(nCntEnemy);						// アイテム設定

		g_aEnemy[nCntEnemy].bUse = false;				// 使用してない状態
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_DEATH;	// 状態変化
		g_aEnemy[nCntEnemy].nCounterState = 60;			// 状態カウンタ
		g_nNumEnemy--;									// 敵総数を減少

		PlaySound(SOUND_LABEL_SE_EXPLOSION);			// 爆発

		// スコア設定
		if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_NORMALENEMY)
		{// ENEMYTYPE_NORMALENEMYの場合
			AddScore(SCORETYPE_RNKING_NO1, (g_aEnemy[nCntEnemy].nType + 1) * 1000);
			SetParticleEffect(g_aEnemy[nCntEnemy].pos, EFFECTTYPE_EXPLOSION, 50, 30, 30);			// エフェクトをセット
		}
		else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_SBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_MBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
		{// ENEMYTYPE_LBOSSENEMYの場合
			if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_SBOSSENEMY) { AddScore(SCORETYPE_RNKING_NO1, (g_aEnemy[nCntEnemy].nType + 1) * 10000); }
			if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_MBOSSENEMY) { AddScore(SCORETYPE_RNKING_NO1, (g_aEnemy[nCntEnemy].nType + 1) * 15000); }
			if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY) { AddScore(SCORETYPE_RNKING_NO1, (g_aEnemy[nCntEnemy].nType + 1) * 20000); }

			SetParticleEffect(g_aEnemy[nCntEnemy].pos, EFFECTTYPE_EXPLOSION, 100, 50, 100);			// エフェクトをセット
		}
	}
	else
	{// ライフが1以上の場合
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_DAMAGE;		// 状態変化
		g_aEnemy[nCntEnemy].nCounterState = 10;				// 状態カウンタ

		PlaySound(SOUND_LABEL_SE_HIT000);					// ヒット音

		// 色変更
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
	}
}

//=========================================================================================================================
// 敵の総数取得
//=========================================================================================================================
int GetNumEnemy(void)
{
	return g_nNumEnemy;		// 値を返す
}

//==================================================================================================
// 敵の弾発射種類
//==================================================================================================
void EnwmyBulletType(int nCntEnemy)
{
	int nCount;

	Player *pPlayer = GetPlayer();

	for (nCount = 0; nCount < MAX_ENEMYBULLET_TYPE; nCount++)
	{
		if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType != ENEMYBULLET_NONE)
		{
			if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_TUTORIALENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_NORMALENEMY)
			{// ENEMYTYPE_NORMALENEMYの場合
				// 敵弾速度
				g_aEnemyBullet[nCntEnemy][nCount].nCounterInput++;
				if ((g_aEnemyBullet[nCntEnemy][nCount].nCounterInput % g_aEnemyBullet[nCntEnemy][nCount].nTiming) == 0)
				{
					PlaySound(SOUND_LABEL_SE_SHOT001);		// 発射

					// 敵弾発射処理
					if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_1)
					{// タイプ１ 前
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_2)
					{// タイプ２ プレイヤー角度
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_3)
					{// タイプ３ 半時計回転
						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE * 3;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_4)
					{// タイプ４ 半時計回転 * 4
						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE * 3;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_5)
					{// タイプ５ 前波
						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_6)
					{// タイプ６ 前３方向
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * SINCOS_ANGLE_45) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * SINCOS_ANGLE_45) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * SINCOS_ANGLE_45) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * SINCOS_ANGLE_45) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_7)
					{// タイプ７ 前プレイヤー角度３方向
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y) + (D3DX_PI * SINCOS_ANGLE_45)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y) + (D3DX_PI * SINCOS_ANGLE_45)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 60, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 60, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y) - (D3DX_PI * SINCOS_ANGLE_45)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y) - (D3DX_PI * SINCOS_ANGLE_45)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 60, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_8)
					{// タイプ
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_9)
					{// タイプ
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_10)
					{// タイプ
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_11)
					{// タイプ
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_12)
					{// タイプ
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_13)
					{// タイプ
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_14)
					{// タイプ
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
				}
			}
			else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_SBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_MBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
			{// ENEMYTYPE_LBOSSENEMYの場合
				g_aEnemyBullet[nCntEnemy][nCount].nCounterInput++;	// カウンタを加算

				if ((g_aEnemyBullet[nCntEnemy][nCount].nCounterInput % g_aEnemyBullet[nCntEnemy][nCount].nTiming) == 0)
				{
					PlaySound(SOUND_LABEL_SE_SHOT001);		// 発射

					if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_1)
					{// タイプ１ きれいな
						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;	// 敵弾角度代入

						// 右
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.0f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.0f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);

						// 左
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.0f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.0f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_2)
					{// タイプ２ 追尾
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(0.5f, 0.8f, 1.0f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_3)
					{// タイプ３ ８方向
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(0.5f, 1.0f, 0.3f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);

					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_4)
					{// タイプ４ 前波直進
						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_STRAIGHT, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 50, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_STRAIGHT, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 50, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_5)
					{// タイプ５ 3本回転 * 2（逆回転）
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.33f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.333f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.33f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.33f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);

						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(0.5f, 0.8f, 1.0f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0 + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0 + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.66f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.66f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.66f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.66f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_6)
					{// タイプ６ 追尾
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_7)
					{// タイプ７ 追尾
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(0.5f, 0.8f, 1.0f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_8)
					{// タイプ８
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0 + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0 + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_9)
					{// タイプ９
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(0.5f, 0.8f, 1.0f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
					}
				}
			}
		}
	}
}

//==================================================================================================
// 敵のアイテム設定
//==================================================================================================
void EnwmyItemSet(int nCntEnemy)
{
	if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_TUTORIALENEMY)
	{// ENEMYTYPE_NORMALENEMY
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);
	}
	else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_NORMALENEMY)
	{// ENEMYTYPE_NORMALENEMY
		if (rand() % 5 == 0) { SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER); }
		if (rand() % 5 == 0) { SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE); }
		if (rand() % 5 == 0) { SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR); }
	}
	else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_SBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_MBOSSENEMY)
	{// ENEMYTYPE_SBOSSENEMYまたはENEMYTYPE_MBOSSENEMY

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);
	}
	else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
	{// ENEMYTYPE_LBOSSENEMY
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);
	}
}