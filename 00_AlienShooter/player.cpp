//=========================================================================================================================
// 
// プレイヤー処理 [player.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include <stdlib.h>		// lib
#include <time.h>		// time
#include "player.h"		// プレイヤー
#include "bullet.h"		// 弾
#include "explosion.h"	// 爆発
#include "effect.h"		// エフェクト
#include "sound.h"		// サウンド
#include "input.h"		// 入力処理

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_PLAYER_POLGON		(11)									// ポリゴンの最大数
#define MAX_PLAYER_TEXTURE		(2)										// テクスチャの最大数
#define PLAYER_TEXTURE_NAME1	"data\\TEXTURE\\life000.png"			// 読み込むテクスチャファイル1
#define PLAYER_TEXTURE_NAME2	"data\\TEXTURE\\player001.png"			// 読み込むテクスチャファイル2
#define PLAYER_POS_X			(SCREEN_WIDTH / 2 - 25)					// プレイヤーの左上X座標
#define PLAYER_POS_Y			(SCREEN_HEIGHT - 100)					// プレイヤーの左上Y座標
#define PLAYER_WIDTH			(SCREEN_WIDTH / 2 + 25)					// プレイヤーの幅
#define PLAYER_HEIGHT			(SCREEN_HEIGHT - 50)					// プレイヤーの高さ
#define PLAYER_G_POS_X			(25/*23*/)									// g_posからのX距離
#define PLAYER_G_POS_Y			(25/*35*/)									// g_posからのY距離
#define PLAYER_TEXTURE_SPEED	(10)									// テクスチャアニメーションスピード（弾速度）
#define PLAYER_TEXTURE_UV_U		(1.0f)									// テクスチャアニメーションU範囲
#define PLAYER_TEXTURE_UV_V		(1.0f)									// テクスチャアニメーションV範囲
#define PLAYER_UPSPEED			(5.0f)									// プレイヤー加速スピード
#define PLAYER_DOWNSPEED		(0.8f)									// プレイヤー減速スピード

#define PLAYERBULLET_ANGLE		(0.05f)									// プレイヤー弾角度
#define PLAYERBULLET_SPEED		(5.0f)									// プレイヤー弾速度

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void PlayerBulletType(void);			// 弾発射種類

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTexturePlayer[MAX_PLAYER_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPlayer = NULL;					// 頂点バッファへのポインタ
Player						g_Player;									// プレイヤーの情報
PlayerBullet				g_PlayerBullet;								// プレイヤーの弾情報

//=========================================================================================================================
// プレイヤー初期化処理
//=========================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntPlayer;
	float fPosLife = 10;

	// プレイヤーの初期化
	g_Player.pos = D3DXVECTOR3((PLAYER_POS_X + PLAYER_WIDTH) / 2, (PLAYER_POS_Y + PLAYER_HEIGHT) / 2, 0.0f);	// プレイヤー中心値
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// プレイヤー移動値
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// プレイヤー色
	g_Player.nLife = PLAYER_LIFE;								// プレイヤー体力
	g_Player.State = PLAYERSTATE_NOMAL;							// プレイヤー状態
	g_Player.nCounterState = 0;									// プレイヤー状態カウント
	g_Player.Type = BULLETTYPE_PLAYER1;							// プレイヤー弾種類
	g_Player.bDisp = true;										// プレイヤー表示

	// プレイヤーの弾初期化
	g_PlayerBullet.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤー弾中心値
	g_PlayerBullet.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// プレイヤー弾移動値
	g_PlayerBullet.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);		// プレイヤー弾色
	g_PlayerBullet.nCounterInput = 0;							// プレイヤー弾入力カウンタ
	g_PlayerBullet.nAngle = 0.0f;								// プレイヤー弾角度

	for (nCntPlayer = 0; nCntPlayer < PLAYER_LIFE; nCntPlayer++)
	{
		g_Player.bLifeTex[nCntPlayer] = true;		// ライフ
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(// ライフ
		pDevice,
		PLAYER_TEXTURE_NAME1,
		&g_pTexturePlayer[0]
	);
	D3DXCreateTextureFromFile
	(// 自機
		pDevice,
		PLAYER_TEXTURE_NAME2,
		&g_pTexturePlayer[1]
	);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_PLAYER_POLGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);
	
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffPlayer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntPlayer = 0; nCntPlayer < MAX_PLAYER_POLGON; nCntPlayer++)
	{
		if (nCntPlayer == MAX_PLAYER_POLGON - 1)
		{
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
		}
		else
		{
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3(fPosLife, 10, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fPosLife + 50, 10, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(fPosLife, 60, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fPosLife + 50, 60, 0.0f);

			fPosLife += 50;
		}
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
		pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, PLAYER_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U, PLAYER_TEXTURE_UV_V);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=========================================================================================================================
// プレイヤー終了処理
//=========================================================================================================================
void UninitPlayer(void)
{
	// 変数宣言
	int nCntTex;

	// テクスチャーの破棄
	for (nCntTex = 0; nCntTex < MAX_PLAYER_TEXTURE; nCntTex++)
	{// テクスチャをカウント
		if (g_pTexturePlayer[nCntTex] != NULL)
		{// NULL以外の場合
			g_pTexturePlayer[nCntTex]->Release();	// 開放
			g_pTexturePlayer[nCntTex] = NULL;		// NULLへ
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{// NULL以外の場合
		g_pVtxBuffPlayer->Release();	// 開放
		g_pVtxBuffPlayer = NULL;		// NULLへ
	}
}

//=========================================================================================================================
// プレイヤー更新処理
//=========================================================================================================================
void UpdatePlayer(void)
{
	// 変数宣言
	float nPlayerSpeed = PLAYER_UPSPEED;			// プレイヤー速度

	g_PlayerBullet.pos = D3DXVECTOR3(g_Player.pos.x/* - 3.5f*/, g_Player.pos.y - 30.0f, g_Player.pos.z);	// 弾位置
	g_PlayerBullet.nAngle += PLAYERBULLET_ANGLE;	// 弾角度

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffPlayer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// プレイヤーがいる場合
	switch (g_Player.State)
	{// 敵状態
	case PLAYERSTATE_NOMAL:			// 通常状態
		break;
	case PLAYERSTATE_DAMAGE:		// ダメージ状態
		g_Player.nCounterState--;	// 状態をカウント

		if (g_Player.nCounterState <= 0)
		{// 状態カウント終了
			// 色変更
			g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 状態変化
			g_Player.State = PLAYERSTATE_NOMAL;
		}
		break;
	case PLAYERSTATE_INVINCIBLE:	// 無敵状態
		g_Player.nCounterState--;

		if (g_Player.nCounterState <= 0)
		{// 状態カウント終了
			 // 色変更
			g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 状態変化
			g_Player.State = PLAYERSTATE_NOMAL;
		}
		break;
	case PLAYERSTATE_DEATH:			// 死亡状態
		break;
	}

	if (g_Player.bDisp == true)
	{// プレイヤーがいる場合
		// 移動速度変更
		if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
		{// 任意のキー（SHIFTキー）を押した場合
			nPlayerSpeed = PLAYER_UPSPEED / 2;
			nPlayerSpeed = PLAYER_UPSPEED / 2;
		}
		else if (GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
		{// 任意のキー（SHIFTキー）を離した場合
			nPlayerSpeed = PLAYER_UPSPEED;
			nPlayerSpeed = PLAYER_UPSPEED;
		}

		// プレイヤーがエリア外の場合
		if (g_Player.pos.x - PLAYER_G_POS_X < 0)
		{// 画面反対側に移動（左）
			nPlayerSpeed = 0;
			g_Player.pos.x += 1;
		}
		else if (g_Player.pos.x + PLAYER_G_POS_X > SCREEN_WIDTH)
		{// 画面反対側に移動（右）
			nPlayerSpeed = 0;
			g_Player.pos.x -= 1;
		}
		else if (g_Player.pos.y - PLAYER_G_POS_Y < 0)
		{// 画面反対側に移動（上）
			nPlayerSpeed = 0;
			g_Player.pos.y += 1;
		}
		else if (g_Player.pos.y + PLAYER_G_POS_Y > SCREEN_HEIGHT)
		{// 画面反対側に移動（下）
			nPlayerSpeed = 0;
			g_Player.pos.y -= 1;
		}

		// 移動方向処理
		if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
		{// 任意のキー（Aキー、左キー）を押した場合
			if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
			{// 上同時入力
				g_Player.move.x += sinf(-D3DX_PI * SINCOS_ANGLE_135) * nPlayerSpeed;
				g_Player.move.y += cosf(-D3DX_PI * SINCOS_ANGLE_135) * nPlayerSpeed;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
			{// 下同時入力
				g_Player.move.x += sinf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else
			{// 左のみ
				g_Player.move.x += sinf(-D3DX_PI * SINCOS_ANGLE_90) * nPlayerSpeed;
				g_Player.move.y += cosf(-D3DX_PI * SINCOS_ANGLE_90) * nPlayerSpeed;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
		{// 任意のキー（Dキー、右キー）を押した場合
			if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
			{// 上同時入力
				g_Player.move.x += sinf(D3DX_PI * SINCOS_ANGLE_135) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI * SINCOS_ANGLE_135) * nPlayerSpeed;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
			{// 下同時入力
				g_Player.move.x += sinf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else
			{// 右のみ
				g_Player.move.x += sinf(D3DX_PI * SINCOS_ANGLE_90) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI * SINCOS_ANGLE_90) * nPlayerSpeed;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
		{// 任意のキー（Wキー、上キー）を押した場合
			if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
			{// 左同時入力
				g_Player.move.x += sinf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
			{// 右同時入力
				g_Player.move.x += sinf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else
			{// 上のみ

				g_Player.move.x += sinf(D3DX_PI) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI) * nPlayerSpeed;
			}
		}
		else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
		{// 任意のキー（Dキー、下キー）を押した場合
			if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
			{// 左同時入力
				g_Player.move.x += sinf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
			{// 右同時入力
				g_Player.move.x += sinf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else
			{// 下のみ
				g_Player.move.x += sinf(SINCOS_ANGLE_0) * nPlayerSpeed;
				g_Player.move.y += cosf(SINCOS_ANGLE_0) * nPlayerSpeed;
			}
		}

		// 移動処理
		g_Player.pos.x += g_Player.move.x;
		g_Player.pos.y += g_Player.move.y;

		// 減速処理
		g_Player.move.x += (0.0f - g_Player.move.x) * PLAYER_DOWNSPEED;
		g_Player.move.y += (0.0f - g_Player.move.y) * PLAYER_DOWNSPEED;

		// 弾発射処理
		if (GetKeyboardPress(DIK_SPACE) == true)
		{// 任意のキー（SPACEキー）を押した場合
			if ((g_PlayerBullet.nCounterInput % (PLAYER_TEXTURE_SPEED - 5)) == 0)
			{// 値（１）
				PlaySound(SOUND_LABEL_SE_SHOT000);		// 発射
				
				PlayerBulletType();		// 弾発射種類
			}
			g_PlayerBullet.nCounterInput++;					// カウンター加算
		}
		else
		{
			g_PlayerBullet.nCounterInput = 0;
		}
	}

	// 頂点設定
	pVtx[(MAX_PLAYER_POLGON - 1) * 4].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_G_POS_X, g_Player.pos.y - PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 1].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_G_POS_X, g_Player.pos.y - PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 2].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_G_POS_X, g_Player.pos.y + PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_G_POS_X, g_Player.pos.y + PLAYER_G_POS_Y, 0.0f);
	// 頂点カラー設定
	pVtx[(MAX_PLAYER_POLGON - 1) * 4].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 1].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 2].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 3].col = g_Player.col;

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=========================================================================================================================
// プレイヤー描画処理
//=========================================================================================================================
void DrawPlayer(void)
{
	// 変数宣言
	int nCntPlayer;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffPlayer,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPlayer = 0; nCntPlayer < MAX_PLAYER_POLGON; nCntPlayer++)
	{
		if (g_Player.bDisp == true && nCntPlayer == MAX_PLAYER_POLGON - 1)
		{// プレイヤーがいる場合
			// テクスチャ設定
			pDevice->SetTexture(0, g_pTexturePlayer[1]);

			// プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlayer * 4, 2);
		}
		else if (g_Player.bLifeTex[nCntPlayer] == true)
		{
			// テクスチャ設定
			pDevice->SetTexture(0, g_pTexturePlayer[0]);

			// プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlayer * 4, 2);
		}
	}
}

//=========================================================================================================================
// プレイヤー設定処理
//=========================================================================================================================
void SetPlayer(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffPlayer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);
	
	// プレイヤーの初期化
	g_Player.pos = D3DXVECTOR3((PLAYER_POS_X + PLAYER_WIDTH) / 2, (PLAYER_POS_Y + PLAYER_HEIGHT) / 2, 0.0f);	// プレイヤー中心値
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// プレイヤー移動値
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// プレイヤー色
	g_Player.nLife = 20;										// プレイヤー体力
	g_Player.State = PLAYERSTATE_NOMAL;							// プレイヤー状態
	g_Player.nCounterState = 0;									// プレイヤー状態カウント
	g_Player.Type = BULLETTYPE_PLAYER1;							// プレイヤー弾種類
	g_Player.bDisp = true;										// プレイヤー表示

	// プレイヤーの弾初期化
	g_PlayerBullet.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// プレイヤー弾中心値
	g_PlayerBullet.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// プレイヤー弾移動値
	g_PlayerBullet.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);		// プレイヤー弾色
	g_PlayerBullet.nCounterInput = 0;							// プレイヤー弾入力カウンタ
	g_PlayerBullet.nAngle = 0.0f;								// プレイヤー弾角度

	for (int nCntPlayer = 0; nCntPlayer < PLAYER_LIFE; nCntPlayer++)
	{
		g_Player.bLifeTex[nCntPlayer] = true;
	}

	// 頂点設定
	pVtx[(MAX_PLAYER_POLGON - 1) * 4].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_G_POS_X, g_Player.pos.y - PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 1].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_G_POS_X, g_Player.pos.y - PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 2].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_G_POS_X, g_Player.pos.y + PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_G_POS_X, g_Player.pos.y + PLAYER_G_POS_Y, 0.0f);
	// 頂点カラー設定
	pVtx[(MAX_PLAYER_POLGON - 1) * 4].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 1].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 2].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 3].col = g_Player.col;

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();

	SetParticleEffect(g_Player.pos, EFFECTTYPE_RESURRECTION, 80, 50, 50);			// エフェクトをセット
}

//=========================================================================================================================
// プレイヤー取得処理
//=========================================================================================================================
Player *GetPlayer(void)
{
	return &g_Player;		// 値を返す
}

//=========================================================================================================================
// プレイヤー弾当たり処理
//=========================================================================================================================
void HitPlayer(int nDamage)
{
	// 変数宣言
	int ExplosionSize = 100;	// 爆発サイズ

	// ライフが減少
	g_Player.nLife -= nDamage;
	g_Player.bLifeTex[g_Player.nLife] = false;

	if (g_Player.nLife <= 0)
	{// ライフが0以下になった場合
		SetExplosion(g_Player.pos, D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), ExplosionSize);	// 爆発
		SetParticleEffect(g_Player.pos, EFFECTTYPE_EXPLOSION, 100, 50, 100);			// エフェクトをセット
		g_Player.bDisp = false;					// 使用していない状態
		g_Player.State = PLAYERSTATE_DEATH;		// デス状態
		// g_Player.nCounterState = 60;

		PlaySound(SOUND_LABEL_SE_EXPLOSION);	// 爆発

		MODE mode = GetMode();					// モード取得

		if (mode != MODE_TUTORIAL)
		{// チュートリアル以外の場合
			SetGameState(GAMESTATE_GAMEOVER);	// ゲームオーバー
		}
	}
	else
	{// ライフが1以上の場合
		g_Player.State = PLAYERSTATE_DAMAGE;	// ダメージ状態
		g_Player.nCounterState = 10;			// カウンタを設定

		PlaySound(SOUND_LABEL_SE_HIT001);		// ヒット音

		// 色変更
		g_Player.col = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
	}
}

//==================================================================================================
// プレイヤー弾発射種類
//==================================================================================================
void PlayerBulletType(void)
{
	if (g_Player.Type == BULLETTYPE_PLAYER1)
	{
		// 前
		g_PlayerBullet.move.x = sinf(D3DX_PI) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_NORMAL, PLAYERBULLET_SPEED, 0, 0);
	}
	else if (g_Player.Type == BULLETTYPE_PLAYER2)
	{
		// 前
		g_PlayerBullet.move.x = sinf(D3DX_PI) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_NORMAL, PLAYERBULLET_SPEED, 0, 0);

		// 前波
		g_PlayerBullet.move.x = sinf(D3DX_PI + g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
	}
	else if (g_Player.Type == BULLETTYPE_PLAYER3)
	{
		// 前
		g_PlayerBullet.move.x = sinf(D3DX_PI) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_NORMAL, PLAYERBULLET_SPEED, 0, 0);

		// 前波
		g_PlayerBullet.move.x = sinf(D3DX_PI + g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
		g_PlayerBullet.move.x = sinf(D3DX_PI - g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
	}
	else if (g_Player.Type == BULLETTYPE_PLAYER4)
	{
		// 前
		g_PlayerBullet.move.x = sinf(D3DX_PI) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_NORMAL, PLAYERBULLET_SPEED, 0, 0);

		// 前波
		g_PlayerBullet.move.x = sinf(D3DX_PI + g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
		g_PlayerBullet.move.x = sinf(D3DX_PI - g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
	}
	else if (g_Player.Type == BULLETTYPE_PLAYER5)
	{
		// 前
		g_PlayerBullet.move.x = sinf(D3DX_PI) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_NORMAL, PLAYERBULLET_SPEED, 0, 0);

		// 前波
		g_PlayerBullet.move.x = sinf(D3DX_PI + g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
		g_PlayerBullet.move.x = sinf(D3DX_PI - g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
	}
}