//=========================================================================================================================
// 
// プレイヤー処理 [player.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "player.h"		// プレイヤー
#include "enemy.h"		// 敵
#include "block.h"		// ブロック
#include "bullet.h"		// 弾
#include "item.h"		// アイテム
#include "score.h"		// スコア
#include "bgwindow.h"	// 背景
#include "fade.h"		// フェード
#include "input.h"		// キーボード
#include "sound.h"		// サウンド

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define PLAYER_TEXTURE_NAME1	"data\\TEXTURE\\all001.png"				// 読み込むテクスチャファイル1
#define PLAYER_WIDTH			(40)									// プレイヤー横幅
#define PLAYER_HEIGHT			(80)									// プレイヤー高さ
#define PLAYER_NUMPLAYER		(3)										// プレイヤー残機
#define PLAYER_LIFE				(3)										// プレイヤー体力
#define PLAYER_TEXTURE_SPEED	(6)										// テクスチャアニメーションスピード（弾速度）
#define PLAYER_TEXTURE_PATTERN	(5)										// テクスチャアニメーションパターン数
#define PLAYER_TEXTURE_UV_U		(0.2f)									// テクスチャアニメーションU範囲
#define PLAYER_TEXTURE_UV_V		(0.1f)									// テクスチャアニメーションV範囲
#define PLAYER_UPSPEED			(1.2f)									// プレイヤー加速スピード
#define PLAYER_DOWNSPEED		(0.2f)									// プレイヤー減速スピード
#define PLAYER_JUMPSPEED		(12.0f)									// プレイヤー上昇スピード
#define PLAYER_GRAVITY			(0.5f)									// プレイヤー重力

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTexturePlayer = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffPlayer = NULL;				// 頂点バッファへのポインタ
PLAYER							g_Player;								// プレイヤー情報
int								g_nCntColor;							// 色調整

//=========================================================================================================================
// プレイヤー初期化処理
//=========================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// プレイヤー情報初期化
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回位置
	g_Player.move = D3DXVECTOR3(0, 0, 0.0f);			// 移動値
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	g_Player.fWidth = PLAYER_WIDTH;						// 横幅
	g_Player.fHeight = PLAYER_HEIGHT;					// 高さ
	g_Player.nNumPlayer = PLAYER_NUMPLAYER;				// 残機
	g_Player.nLife = PLAYER_LIFE;						// 体力
	g_Player.fGravity = PLAYER_GRAVITY;					// 重力
	g_Player.State = PLAYERSTATE_APPEAR;				// 状態
	g_Player.nCounterState = 0;							// 状態カウンタ
	g_Player.AnimType = PLAYERANIMTYPE_NORMAL;			// アニメーション種類
	g_Player.nCounterAnim = 0;							// アニメーションカウンタ
	g_Player.nNumAnim = 0;								// アニメーション番号
	g_Player.nSpeedAnim = 1;							// アニメーション速度
	g_Player.nPatternAnim = 1;							// アニメーションパターン
	g_Player.nDinectionMove = 0;						// 方向
	g_Player.bAttack = false;							// 攻撃しているか
	g_Player.bJump = false;								// ジャンプしているか
	g_Player.bDisp = false;								// 表示しているか
	g_Player.pBlock = NULL;								// ブロックポインタ

	g_nCntColor = 0;									// 色調整

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEXTURE_NAME1, &g_pTexturePlayer);	// プレイヤー

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
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

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=========================================================================================================================
// プレイヤー終了処理
//=========================================================================================================================
void UninitPlayer(void)
{
	// テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=========================================================================================================================
// プレイヤー更新処理
//=========================================================================================================================
void UpdatePlayer(void)
{
	//変数宣言
	float fSpeed = PLAYER_UPSPEED;		// 移動加速度

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffPlayer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);
	
	MODE mode = GetMode();				// 画面情報
	GAMESTATE game = GetGameState();	// ゲーム情報
	FADE fade = GetFade();				// フェード情報

	if (g_Player.bDisp == true)
	{// 表示している場合
		switch (g_Player.State)
		{// 状態
		case PLAYERSTATE_APPEAR:		// 出現
			break;
		case PLAYERSTATE_NORMAL:		// ノーマル
			break;
		case PLAYERSTATE_DAMAGE:		// ダメージ
			
			g_Player.nCounterState++;	// カウンタを進める
			
			if (g_Player.nCounterState % 5 == 0)
			{// 色切替
				g_nCntColor = (g_nCntColor + 1) % 2;	// 色変更
			}

			if (g_nCntColor == 0) { g_Player.col.a = 0.0f; }// 透明
			else { g_Player.col.a = 1.0f; }					// 不透明

			if (g_Player.nCounterState > 90)
			{// 1.5秒後
				g_Player.State = PLAYERSTATE_NORMAL;	// ノーマルへ
				g_Player.nCounterState = 0;				// 状態カウンタを初期化
				g_Player.col.a = 1.0f;					// 不透明
			}

			break;
		case PLAYERSTATE_DEATH:			// デス
			break;
		}

		g_Player.nCounterAnim++;	// カウンタを進める

		if (mode == MODE_TITLE)
		{// タイトルの場合
			// 歩く
			g_Player.AnimType = PLAYERANIMTYPE_WALK;
			g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_WALK;
			g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;

			if (g_Player.nCounterAnim % g_Player.nSpeedAnim == 0)
			{// アニメーション
				g_Player.nNumAnim = (g_Player.nNumAnim + 1) % g_Player.nPatternAnim;
			}

			// テクスチャ設定
			pVtx[0].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
			pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
			pVtx[2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
			pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
		}
		else if (mode == MODE_GAME)
		{// ゲームの場合
			if (game == GAMESTATE_MAP)
			{
				// 移動方向処理
				if (GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_A) == true || GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_LEFT) == true)
				{// 任意のキー（Aキー、左キー）を押した場合
					if (fade != FADE_OUT)
					{
						if (g_Player.bJump == false)
						{// 表示の制限
							g_Player.nNumAnim = 0;					// アニメーションを最初から

							// 走る
							g_Player.AnimType = PLAYERANIMTYPE_RUN;
							g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_RUN;
							g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
						}
					}
				}
				else if (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_D) == true || GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_RIGHT) == true)
				{// 任意のキー（Dキー、右キー）を押した場合
					if (fade != FADE_OUT)
					{
						if (g_Player.bJump == false && g_Player.bAttack == false)
						{// 表示の制限
							g_Player.nNumAnim = 0;					// アニメーションを最初から

							// 走る
							g_Player.AnimType = PLAYERANIMTYPE_RUN;
							g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_RUN;
							g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
						}
					}
				}
				else if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
				{
					if (g_Player.move.x == 0.0f && g_Player.move.y == 0.0f && fade == FADE_NONE)
					{
						g_Player.nNumAnim = 0;					// アニメーションを最初から

						// ジャンプ
						g_Player.AnimType = PLAYERANIMTYPE_JUMP;
						g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_JUMP;
						g_Player.nSpeedAnim = 20;

						g_Player.bJump = true;					// ジャンプを使用している
					}
				}
				else
				{// キーを押していないとき
					if (g_Player.bJump == false)
					{
						if (g_Player.move.x == 0.0f &&
							g_Player.move.y == 0.0f)
						{
							if (g_Player.AnimType != PLAYERANIMTYPE_NORMAL)
							{// アニメーションが変わった場合
								g_Player.nNumAnim = 0;		// アニメーションを最初から
							}

							// 立ち
							g_Player.AnimType = PLAYERANIMTYPE_NORMAL;
							g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_NORMAL;
							g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
						}
					}
				}

				// 移動処理
				g_Player.pos += g_Player.move;

				if (g_Player.nCounterAnim % g_Player.nSpeedAnim == 0 && g_Player.bJump == false)
				{// アニメーション
					g_Player.nNumAnim = (g_Player.nNumAnim + 1) % g_Player.nPatternAnim;
				}

				// 頂点設定
				pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y, 0.0f);

				// テクスチャ設定
				if (g_Player.nDinectionMove == 0)
				{// 右
					pVtx[0].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
					pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
				}
				else if (g_Player.nDinectionMove == 1)
				{// 左
					pVtx[0].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
					pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
				}
			}
			else
			{
				if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
				{// 任意のキー（SHIFT)を押した場合
					fSpeed *= 2;	// 加速度を２倍
					if (g_Player.bJump == false && g_Player.bAttack == false && g_Player.AnimType != PLAYERANIMTYPE_NORMAL)
					{// 表示の制限
						if (g_Player.AnimType != PLAYERANIMTYPE_RUN)
						{// アニメーションが変わった場合
							g_Player.nNumAnim = 0;		// アニメーションを最初から
						}

						// 走る
						g_Player.AnimType = PLAYERANIMTYPE_RUN;
						g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_RUN;
						g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
					}
				}
				else if (GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardRelease(DIK_RSHIFT) == true)
				{// 任意のキー（SHIFT)を離した場合
					if (g_Player.bJump == false && g_Player.bAttack == false && g_Player.AnimType != PLAYERANIMTYPE_NORMAL)
					{// 表示の制限
						if (g_Player.AnimType != PLAYERANIMTYPE_WALK)
						{//　アニメーションが変わった場合
							g_Player.nNumAnim = 0;		// アニメーションを最初から
						}

						// 歩く
						g_Player.AnimType = PLAYERANIMTYPE_WALK;
						g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_WALK;
						g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
					}
				}

				// 移動方向処理
				if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
				{// 任意のキー（Aキー、左キー）を押した場合
					g_Player.move.x += sinf(-D3DX_PI * 0.5f) * fSpeed;		// 移動値を代入
					if (g_Player.bJump == false && g_Player.bAttack == false)
					{// 表示の制限
						if (g_Player.AnimType != PLAYERANIMTYPE_RUN)
						{// 切替の調整
							if (g_Player.AnimType != PLAYERANIMTYPE_WALK)
							{// アニメーションが変わった場合
								g_Player.nNumAnim = 0;		// アニメーションを最初から
							}

							// 歩く
							g_Player.AnimType = PLAYERANIMTYPE_WALK;
							g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_WALK;
							g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
						}
						g_Player.nDinectionMove = 1;	// 向きを調整
					}
				}
				else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
				{// 任意のキー（Dキー、右キー）を押した場合
					g_Player.move.x += sinf(D3DX_PI * 0.5f) * fSpeed;		// 移動値を代入
					if (g_Player.bJump == false && g_Player.bAttack == false)
					{// 表示の制限
						if (g_Player.AnimType != PLAYERANIMTYPE_RUN)
						{// 切替の調整
							if (g_Player.AnimType != PLAYERANIMTYPE_WALK)
							{// アニメーションが変わった場合
								g_Player.nNumAnim = 0;		// アニメーションを最初から
							}

							// 歩く
							g_Player.AnimType = PLAYERANIMTYPE_WALK;
							g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_WALK;
							g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
						}
						g_Player.nDinectionMove = 0;	// 向きを調整
					}
				}
				else
				{// キーを押していないとき
					if (g_Player.bJump == false && g_Player.bAttack == false)
					{// 表示の制限
						if (g_Player.AnimType != PLAYERANIMTYPE_NORMAL)
						{// アニメーションが変わった場合
							g_Player.nNumAnim = 0;		// アニメーションを最初から
						}

						// 立ち
						g_Player.AnimType = PLAYERANIMTYPE_NORMAL;
						g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_NORMAL;
						g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;
					}
				}

				// ジャンプ処理
				if ((GetKeyboardTrigger(DIK_SPACE) == true || GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true) && g_Player.bJump == false && g_Player.bAttack == false)
				{// 任意のキー（スペースキー）を押した場合
					g_Player.move.y -= PLAYER_JUMPSPEED;	// 移動値を代入
					g_Player.nNumAnim = 0;					// アニメーションを最初から

					// ジャンプ
					g_Player.AnimType = PLAYERANIMTYPE_JUMP;
					g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_JUMP;
					g_Player.nSpeedAnim = 20;
					PlaySound(SOUND_LABEL_SE_JUMP000);

					g_Player.bJump = true;					// ジャンプを使用している
				}
				else if (g_Player.bJump == true && g_Player.bAttack == false)
				{// ジャンプを使用している場合
					if (g_Player.move.y == 0.0f)
					{// アニメーション
						g_Player.nNumAnim = (g_Player.nNumAnim + 1) % g_Player.nPatternAnim;
					}
				}

				if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true && g_Player.bAttack == false)
				{// 任意のキー（Z、ENTER）を押した場合
					if (g_Player.bAttack == false)
					{//攻撃していない場合
						if (g_Player.AnimType != PLAYERANIMTYPE_PATTERN_THROW)
						{// アニメーションが変わった場合
							g_Player.nNumAnim = 0;		// アニメーションを最初から
						}

						// 投げる
						g_Player.AnimType = PLAYERANIMTYPE_THROW;
						g_Player.nPatternAnim = PLAYERANIMTYPE_PATTERN_THROW;
						g_Player.nSpeedAnim = PLAYER_TEXTURE_SPEED;

						g_Player.bAttack = true;		// 攻撃を使用している
					}
				}
				else if (g_Player.bAttack == true)
				{// 攻撃を使用している場合
					if (g_Player.nNumAnim == g_Player.nPatternAnim - 1)
					{// アニメーションが最後まで進んだ場合
						g_Player.bAttack = false;		// 攻撃を使用していない状態ヘ
					}
					else if (g_Player.nNumAnim == g_Player.nPatternAnim - 4 && g_Player.nCounterAnim % g_Player.nSpeedAnim == 0)
					{// 特定のアニメーションに変わった場合
						// 弾発射処理
						if (g_Player.nDinectionMove == 0)
						{// 右
							SetBullet(D3DXVECTOR3(g_Player.pos.x + (g_Player.fWidth / 2), g_Player.pos.y - (g_Player.fHeight / 2) - 20, 0.0f),
								D3DXVECTOR3(20.0f, -0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BULLETTYPE_PLAYER, 8.0f, 16.0f, BULLETTEXTURETYPE_PLAYER_NORMAL);
						}
						else
						{// 左
							SetBullet(D3DXVECTOR3(g_Player.pos.x + (g_Player.fWidth / 2), g_Player.pos.y - (g_Player.fHeight / 2) - 20, 0.0f),
								D3DXVECTOR3(-20.0f, -0.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BULLETTYPE_PLAYER, 8.0f, 16.0f, BULLETTEXTURETYPE_PLAYER_NORMAL);
						}
						PlaySound(SOUND_LABEL_SE_SHOT000);
					}
				}

				if (g_Player.nCounterAnim % g_Player.nSpeedAnim == 0 && (g_Player.bJump == false || g_Player.bAttack == true))
				{// アニメーション
					g_Player.nNumAnim = (g_Player.nNumAnim + 1) % g_Player.nPatternAnim;
				}

				// 位置を記憶
				g_Player.posOld = g_Player.pos;
				// 重力加算
				g_Player.move.y += g_Player.fGravity;
				// 移動処理
				g_Player.pos.y += g_Player.move.y;
				// 減速処理
				g_Player.move.x += (0.0f - g_Player.move.x) * PLAYER_DOWNSPEED;

				// ブロックとの衝突処理
				if ((CollisionBGWindow(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.fWidth, g_Player.fHeight) == true) ||
					(CollisionBlock(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.fWidth, g_Player.fHeight - 30.0f, &g_Player.pBlock, COLLISIONBLOCKRETURN_PLAYER) == true))
				{// 衝突している場合
					g_Player.bJump = false;		// ジャンプを使用していない
				}
				else
				{// 衝突していない場合
					g_Player.bJump = true;		// ジャンプを使用している
				}

				if (g_Player.pBlock != NULL)
				{// pBlockがNULLでない場合
					g_Player.pos += g_Player.pBlock->move;

					if (g_Player.pBlock->type == BLOCKTYPE_NORMAL_DAMAGE && g_Player.State == PLAYERSTATE_NORMAL)
					{
						// ノックバック設定
						g_Player.move.x = 0.0f;
					
						if (g_Player.move.y <= 0.0f)
						{
							g_Player.move.y *= -1;
						}
						if (g_Player.pos.x < g_Player.pBlock->pos.x + (g_Player.pBlock->fWidth / 2))
						{
							g_Player.move.x -= 50;
						}
						else
						{
							g_Player.move.x += 50;
						}
						
						g_Player.nLife--;	// 体力減少
						g_Player.State = PLAYERSTATE_DAMAGE;	// ダメージ状態へ
						PlaySound(SOUND_LABEL_SE_HIT000);
						HitPlayerLife();	// テクスチャ設定

						if (g_Player.nLife <= 0)
						{// 体力が0以下の場合
							g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							g_Player.nNumPlayer--;				// 残機を減らす
							if (g_Player.nNumPlayer < 0)
							{// 残機が0以下の場合
								SetGameState(GAMESTATE_GAMEOVER);			// ゲームオーバーへ
							}
							else
							{// それ以外
								DeletePlayer();
								SetFadeGame(GAMESTATE_MAP);		// ゲームマップへ
							}
						}
					}
				}

				// アイテムとの衝突処理
				CollisionItem(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.fWidth, g_Player.fHeight);

				// 敵との衝突処理
				if (CollisionEnemy(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.fWidth, g_Player.fHeight) == true && g_Player.State == PLAYERSTATE_NORMAL)
				{// 衝突している場合
					g_Player.nLife--;	// 体力減少
					g_Player.State = PLAYERSTATE_DAMAGE;	// ダメージ状態へ
					PlaySound(SOUND_LABEL_SE_HIT000);
					HitPlayerLife();	// テクスチャ設定

					if (g_Player.nLife <= 0)
					{// 体力が0以下の場合
						g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						g_Player.nNumPlayer--;				// 残機を減らす
						if (g_Player.nNumPlayer < 0)
						{// 残機が0以下の場合
							SetGameState(GAMESTATE_GAMEOVER);			// ゲームオーバーへ
						}
						else
						{// それ以外
							DeletePlayer();
							SetFadeGame(GAMESTATE_MAP);		// ゲームマップへ
						}
					}
				}
				
				if (g_Player.pos.y - g_Player.fHeight > SCREEN_HEIGHT)
				{// 画面外処理
					g_Player.move.x = 0.0f;				// 横移動値を０
					g_Player.bDisp = false;				// プレイヤーを表示しない
					
					g_Player.nNumPlayer--;				// 残機を減らす
					if (g_Player.nNumPlayer < 0)
					{// 残機が0以下の場合
						SetGameState(GAMESTATE_GAMEOVER);			// ゲームオーバーへ
					}
					else
					{// それ以外
						DeletePlayer();
						SetFadeGame(GAMESTATE_MAP);		// ゲームマップへ
					}
				}

				// 頂点設定
				pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y, 0.0f);
				// 頂点カラー設定
				pVtx[0].col = g_Player.col;
				pVtx[1].col = g_Player.col;
				pVtx[2].col = g_Player.col;
				pVtx[3].col = g_Player.col;

				// テクスチャ設定
				if (g_Player.nDinectionMove == 0)
				{// 右
					pVtx[0].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
					pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
				}
				else if (g_Player.nDinectionMove == 1)
				{// 左
					pVtx[0].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V);
					pVtx[2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim + PLAYER_TEXTURE_UV_U, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
					pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * g_Player.nNumAnim, g_Player.AnimType * PLAYER_TEXTURE_UV_V + PLAYER_TEXTURE_UV_V);
				}
			}
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=========================================================================================================================
// プレイヤー描画処理
//=========================================================================================================================
void DrawPlayer(void)
{
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

	// テクスチャ設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	// プレイヤーの描画
	if (g_Player.bDisp == true)
	{// trueの場合
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=========================================================================================================================
// プレイヤー設定処理
//=========================================================================================================================
void SetPlayer(D3DXVECTOR3 pos)
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

	g_Player.pos = pos;								// 位置
	g_Player.move = D3DXVECTOR3(0, 0, 0.0f);		// 移動値
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// 色
	g_Player.fWidth = PLAYER_WIDTH;					// 横幅
	g_Player.fHeight = PLAYER_HEIGHT;				// 高さ
	g_Player.nLife = PLAYER_LIFE;					// 体力
	g_Player.fGravity = PLAYER_GRAVITY;				// 重力
	g_Player.State = PLAYERSTATE_NORMAL;			// 状態
	g_Player.nCounterState = 0;						// 状態カウンタ
	g_Player.AnimType = PLAYERANIMTYPE_NORMAL;		// アニメーション種類
	g_Player.nCounterAnim = 0;						// アニメーションカウンタ
	g_Player.nNumAnim = 0;							// アニメーション番号
	g_Player.nSpeedAnim = 1;						// アニメーション速度
	g_Player.nPatternAnim = 1;						// アニメーションパターン
	g_Player.nDinectionMove = 0;					// 方向
	g_Player.bAttack = false;						// 攻撃を使用していない
	g_Player.bJump = false;							// ジャンプを使用していない
	g_Player.bDisp = true;							// 表示する
	g_Player.pBlock = NULL;							// ブロックポインタをNULL

	AddScore(SCORETYPE_PLAYER, g_Player.nNumPlayer);// 残機設定

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y - g_Player.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - g_Player.fWidth, g_Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + g_Player.fWidth, g_Player.pos.y, 0.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//=========================================================================================================================
// プレイヤー取得処理
//=========================================================================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;		// 値を返す
}	

//=========================================================================================================================
// プレイヤー破棄処理
//=========================================================================================================================
void DeletePlayer(void)
{
	if (g_Player.bDisp == true)
	{
		// プレイヤー情報初期化
		g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
		g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 前回位置
		g_Player.move = D3DXVECTOR3(0, 0, 0.0f);			// 移動値
		g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
		g_Player.fWidth = PLAYER_WIDTH;						// 横幅
		g_Player.fHeight = PLAYER_HEIGHT;					// 高さ
		g_Player.fGravity = PLAYER_GRAVITY;					// 重力
		g_Player.State = PLAYERSTATE_APPEAR;				// 状態
		g_Player.nCounterState = 0;							// 状態カウンタ
		g_Player.AnimType = PLAYERANIMTYPE_NORMAL;			// アニメーション種類
		g_Player.nCounterAnim = 0;							// アニメーションカウンタ
		g_Player.nNumAnim = 0;								// アニメーション番号
		g_Player.nSpeedAnim = 1;							// アニメーション速度
		g_Player.nPatternAnim = 1;							// アニメーションパターン
		g_Player.nDinectionMove = 0;						// 方向
		g_Player.bAttack = false;							// 攻撃しているか
		g_Player.bJump = false;								// ジャンプしているか
		g_Player.bDisp = false;								// 表示しているか
		g_Player.pBlock = NULL;								// ブロックポインタ
	}
}