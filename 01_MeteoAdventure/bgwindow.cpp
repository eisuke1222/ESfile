//=========================================================================================================================
// 
// 背景処理 [bgwindow.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "bgwindow.h"	// 背景
#include "input.h"		// 入力処理
#include "map.h"		// マップ
#include "player.h"		// プレイヤー
#include "block.h"		// ブロック
#include "item.h"		// アイテム
#include "score.h"		// スコア
#include "fade.h"		// フェード
#include "sound.h"		// サウンド

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_BGWINDOW_POLYGON			(64)							// ポリゴンの最大数
#define BGWINDOW_TEXTURE_SPEED			(5)								// テクスチャアニメーションスピード
#define BGWINDOW_TEXTURE_PATTERN		(5)								// テクスチャアニメーションパターン数
#define BGWINDOW_TEXTURE_UV_U			(1.0f)							// テクスチャアニメーションU範囲
#define BGWINDOW_TEXTURE_UV_V			(1.0f)							// テクスチャアニメーションV範囲
#define BGWINDOW_TEXTURE_ANIM_UV_U		(0.2f)							// テクスチャアニメーションU範囲
#define BGWINDOW_TEXTURE_ANIM_UV_V		(0.5f)							// テクスチャアニメーションV範囲
#define BGWINDOW_TEXTURE_STAGE_UV_U		(1.0f)							// テクスチャアニメーションU範囲
#define BGWINDOW_TEXTURE_STAGE_UV_V		(0.2f)							// テクスチャアニメーションV範囲
#define BGWINDOW_SPEED					(0.002f)						// 背景スピード

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureBGWindow[BGWINDOWTEXTURE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffBGWindow = NULL;						// 頂点バッファへのポインタ
BGWINDOW						g_BGWindow[MAX_BGWINDOW_POLYGON];				// 背景の情報
SELECTSTATE						g_SelectState;									// 選択の状態
float							g_fAngleUp;										// 角度加算
float							g_fAngleDown;									// 角度減算
int								g_nCntBGTimer;									// 時間カウンタ
bool							g_bGoal;										// ゴール判定

//=========================================================================================================================
// 背景初期化処理
//=========================================================================================================================
void InitBGWindow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスを取得
	int nCntBGWindow;							// 背景カウンタ

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// 背景をカウント
		g_BGWindow[nCntBGWindow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 背景の中心値
		g_BGWindow[nCntBGWindow].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 背景の中心値
		g_BGWindow[nCntBGWindow].texUV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// UV
		g_BGWindow[nCntBGWindow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 背景の移動値
		g_BGWindow[nCntBGWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 背景色
		g_BGWindow[nCntBGWindow].fWidth = 0;								// 横幅
		g_BGWindow[nCntBGWindow].fHeight = 0;								// 高さ
		g_BGWindow[nCntBGWindow].fAngle = 0;								// 角度
		g_BGWindow[nCntBGWindow].fLenght = 0;								// 長さ
		g_BGWindow[nCntBGWindow].nCounterAnim = 0;							// アニメーションカウンタ
		g_BGWindow[nCntBGWindow].nNumAnim = 0;								// アニメーション番号
		g_BGWindow[nCntBGWindow].nSpeedAnim = 0;							// アニメーション速度
		g_BGWindow[nCntBGWindow].nPatternAnim = 0;							// アニメーションパターン数
		g_BGWindow[nCntBGWindow].bUse = false;								// 使用しているか
	}
	g_SelectState = SELECTSTATE_NORMAL;										// 選択状態	
	g_fAngleUp = 0.0f;														// 初期化
	g_fAngleDown = 0.0f;													// 初期化
	g_nCntBGTimer = 0;														// 初期化
	g_bGoal = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_1, &g_pTextureBGWindow[0]);		// タイトル（背景）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_2, &g_pTextureBGWindow[1]);		// タイトル（ロゴ）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_3, &g_pTextureBGWindow[2]);		// タイトル（地面）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_4, &g_pTextureBGWindow[3]);		// ゲーム（マップ）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_5, &g_pTextureBGWindow[4]);		// ゲーム（P）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_6, &g_pTextureBGWindow[5]);		// ゲーム（ステージ番号）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_7, &g_pTextureBGWindow[6]);		// ゲーム（ハイスコアテキスト）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_8, &g_pTextureBGWindow[7]);		// ゲーム（残機）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_9, &g_pTextureBGWindow[8]);		// ゲーム（プレイヤー）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_10, &g_pTextureBGWindow[9]);	// ゲーム（体力）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_11, &g_pTextureBGWindow[10]);	// ゲーム（コイン）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_12, &g_pTextureBGWindow[11]);	// ゲーム（コインテキスト）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_13, &g_pTextureBGWindow[12]);	// ゲーム（ヘルプ）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_14, &g_pTextureBGWindow[13]);	// ゲーム（ヘルプ（歩き））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_15, &g_pTextureBGWindow[14]);	// ゲーム（ヘルプ（走る））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_16, &g_pTextureBGWindow[15]);	// ゲーム（ヘルプ（ジャンプ））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_17, &g_pTextureBGWindow[16]);	// ゲーム（ヘルプ（攻撃））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_18, &g_pTextureBGWindow[17]);	// ゲーム（背景（空））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_19, &g_pTextureBGWindow[18]);	// ゲーム（背景（岩））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_20, &g_pTextureBGWindow[19]);	// ゲーム（木（下））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_21, &g_pTextureBGWindow[20]);	// ゲーム（木（並木））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_22, &g_pTextureBGWindow[21]);	// ゲーム（木（小））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_23, &g_pTextureBGWindow[22]);	// ゲーム（砂漠（小））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_24, &g_pTextureBGWindow[23]);	// ゲーム（ゴール制限）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_25, &g_pTextureBGWindow[24]);	// ゲーム（ゴール）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_26, &g_pTextureBGWindow[25]);	// ゲーム（ゴール（小））
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_27, &g_pTextureBGWindow[26]);	// リザルト（クリア）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_28, &g_pTextureBGWindow[27]);	// リザルト（オーバー）
	D3DXCreateTextureFromFile(pDevice, BGWINDOWTEXTURE_NAME_29, &g_pTextureBGWindow[28]);	// 選択

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BGWINDOW_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBGWindow,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBGWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// 背景をカウント
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
		pVtx[1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_UV_U, BGWINDOW_TEXTURE_UV_V);
		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}
	// 頂点バッファをアンロック
	g_pVtxBuffBGWindow->Unlock();
}

//=========================================================================================================================
// 背景終了処理
//=========================================================================================================================
void UninitBGWindow(void)
{
	// 変数宣言
	int nCntTex;		// テクスチャをカウント

	// テクスチャの破棄
	for (nCntTex = 0; nCntTex < BGWINDOWTEXTURE_MAX; nCntTex++)
	{// テクスチャをカウント
		if (g_pTextureBGWindow[nCntTex] != NULL)
		{
			g_pTextureBGWindow[nCntTex]->Release();
			g_pTextureBGWindow[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBGWindow != NULL)
	{
		g_pVtxBuffBGWindow->Release();
		g_pVtxBuffBGWindow = NULL;
	}
}

//=========================================================================================================================
// 背景更新処理
//=========================================================================================================================
void UpdateBGWindow(void)
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ
	int nCntBGWindow;	// 背景カウンタ

	MODE mode = GetMode();
	GAMESTATE game = GetGameState();	// ゲーム情報
	bool pPuase = GetPauseState();		// ポーズ状態
	int nSelectMap = GetSelectMap();	// 選択マップ情報
	PLAYER *pPlayer = GetPlayer();		// プレイヤ情報
	int nNumCoin = GetNumCoin();		// コイン総数
	bool bScroll = GetScrollNum();

	// 頂点バッファをロック
	g_pVtxBuffBGWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// 背景をカウント
		if (g_BGWindow[nCntBGWindow].bUse == true)
		{// 使用している場合
			switch (g_BGWindow[nCntBGWindow].texture)
			{// テクスチャ状態
			case BGWINDOWTEXTURE_TITLE_BG:		// タイトルBG

				g_BGWindow[nCntBGWindow].tex.x += 0.0008f;

				break;
			case BGWINDOWTEXTURE_TITLE_LOGO:	// タイトルロゴ
				break;
			case BGWINDOWTEXTURE_TITLE_GOUND:	// タイトル地面

				g_fAngleUp += 0.01f;
				g_fAngleDown -= 0.01f;

				break;
			case BGWINDOWTEXTURE_GAME_MAP:		// ゲームマップ
				break;
			case BGWINDOWTEXTURE_GAME_NUMPLAYER:// ゲームステージ番号
				break;
			case BGWINDOWTEXTURE_GAME_HISCORETXT:// ゲームハイスコアテキスト
				break;
			case BGWINDOWTEXTURE_GAME_NUMSTAGE: // ゲーム残機

				g_BGWindow[nCntBGWindow].nNumAnim = nSelectMap;
				
				break;
			case BGWINDOWTEXTURE_GAME_PLAYER:	// ゲームプレイヤー

				g_BGWindow[nCntBGWindow].nCounterAnim++;
				if (g_BGWindow[nCntBGWindow].nCounterAnim % (BGWINDOW_TEXTURE_SPEED + 1) == 0)
				{
					g_BGWindow[nCntBGWindow].nNumAnim = (g_BGWindow[nCntBGWindow].nNumAnim + 1) % BGWINDOW_TEXTURE_PATTERN;
				}

				break;
			case BGWINDOWTEXTURE_GAME_LIFE:		// ゲームライフ
				break;
			case BGWINDOWTEXTURE_GAME_COIN:		// ゲームコイン

				if (pPuase == false)
				{
					g_BGWindow[nCntBGWindow].nCounterAnim++;

					if (g_BGWindow[nCntBGWindow].nCounterAnim % BGWINDOW_TEXTURE_SPEED == 0)
					{
						g_BGWindow[nCntBGWindow].nNumAnim = (g_BGWindow[nCntBGWindow].nNumAnim + 1) % BGWINDOW_TEXTURE_PATTERN;
						if (g_BGWindow[nCntBGWindow].nNumAnim == 0) { g_BGWindow[nCntBGWindow].nPatternAnim = g_BGWindow[nCntBGWindow].nPatternAnim ^ 1; }
					}
				}

				break;
			case BGWINDOWTEXTURE_GAME_COINTXT:		// ゲームコイン
				break;
			case BGWINDOWTEXTURE_GAME_HELP:			// ゲームヘルプ
			case BGWINDOWTEXTURE_GAME_HELPWALK:		// ゲームヘルプ歩く
			case BGWINDOWTEXTURE_GAME_HELPRUN:		// ゲームヘルプ走る
			case BGWINDOWTEXTURE_GAME_HELPJUMP:		// ゲームヘルプジャンプ
			case BGWINDOWTEXTURE_GAME_HELPATTACK:	// ゲームヘルプ攻撃

				if (pPuase == false)
				{
					if (bScroll == false)
					{
						g_BGWindow[nCntBGWindow].pos.x -= pPlayer->move.x / 2;
					}
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGE:	// ゲームステージ

				if (pPuase == false)
				{
					if (pPlayer->move.x > 0)
					{
						g_BGWindow[nCntBGWindow].tex.x += pPlayer->move.x / 80000;
					}
					g_BGWindow[nCntBGWindow].tex.x += 0.0001f;
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGE_IWA:	// ゲームステージ背景岩

				if (pPuase == false)
				{
					g_BGWindow[nCntBGWindow].tex.x += pPlayer->move.x / 1050;
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER:	// ゲームステージ木(下）

				if (pPuase == false)
				{
					g_BGWindow[nCntBGWindow].tex.x += pPlayer->move.x / 3500;
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL:	// ゲームステージ木（並木）

				if (pPuase == false)
				{
					g_BGWindow[nCntBGWindow].tex.x += pPlayer->move.x / 700;
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGE_TREE_SMALL:	// ゲームステージ木（小）
				break;
			case BGWINDOWTEXTURE_GAME_STAGE_SABAKU_YAMA:// ゲーム砂漠（山）

				if (pPuase == false)
				{
					g_BGWindow[nCntBGWindow].tex.x += pPlayer->move.x / 10000;
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT:	// ゲームステージゴール制限

				if (pPuase == false)
				{
					if (bScroll == false)
					{
						g_BGWindow[nCntBGWindow].pos.x -= pPlayer->move.x / 2;
					}

					if (nNumCoin <= 0)
					{
						g_BGWindow[nCntBGWindow].col.a -= 0.01f;
						if (g_BGWindow[nCntBGWindow].col.a <= 0.0f)
						{
							DeleteBGWindow(nCntBGWindow);
						}
					}
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGEGOAL:// ゲームステージゴール

				if (pPuase == false || mode == MODE_TITLE)
				{
					if (bScroll == false)
					{
						g_BGWindow[nCntBGWindow].pos.x -= pPlayer->move.x / 2;
					}

					g_BGWindow[nCntBGWindow].nCounterAnim++;

					if (g_BGWindow[nCntBGWindow].nCounterAnim % 8 == 0)
					{
						g_BGWindow[nCntBGWindow].nNumAnim = (g_BGWindow[nCntBGWindow].nNumAnim + 1) % BGWINDOW_TEXTURE_PATTERN;
					}
				}

				break;
			case BGWINDOWTEXTURE_GAME_STAGEGOAL_SMALL:// ゲームステージゴール（小）

				g_BGWindow[nCntBGWindow].nCounterAnim++;

				if (g_BGWindow[nCntBGWindow].nCounterAnim % 8 == 0)
				{
					g_BGWindow[nCntBGWindow].nNumAnim = (g_BGWindow[nCntBGWindow].nNumAnim + 1) % BGWINDOW_TEXTURE_PATTERN;
				}

				break;
			case BGWINDOWTEXTURE_RESULT_CLEAR:	// ゲームクリア
			case BGWINDOWTEXTURE_RESULT_OVER:	// ゲームオーバー
				break;
			case BGWINDOWTEXTURE_SELECT:		// 選択

				g_nCntBGTimer++;

				if (g_SelectState == SELECTSTATE_NORMAL)
				{// 選択ノーマル
					if (g_nCntBGTimer == 80) { g_nCntBGTimer = 0; }

					// 点滅
					if (g_nCntBGTimer < 40) { g_BGWindow[nCntBGWindow].col.a = 1.0f; }
					else { g_BGWindow[nCntBGWindow].col.a = 0.0f; }
				}
				else if (g_SelectState == SELECTSTATE_ENTER)
				{// 選択エンター
					if (g_nCntBGTimer == 10)
					{ g_nCntBGTimer = 0; }

					// 点滅
					if (g_nCntBGTimer < 5) { g_BGWindow[nCntBGWindow].col.a = 1.0f; }
					else { g_BGWindow[nCntBGWindow].col.a = 0.0f; }
				}

				break;
			}
			
			// 頂点設定
			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_TITLE_GOUND)
			{// タイトル地面
				pVtx[nCntBGWindow * 4].pos.x = g_BGWindow[nCntBGWindow].pos.x - sinf(g_fAngleUp) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4].pos.y = g_BGWindow[nCntBGWindow].pos.y - cosf(g_fAngleUp) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 1].pos.x = g_BGWindow[nCntBGWindow].pos.x + sinf(g_fAngleDown) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 1].pos.y = g_BGWindow[nCntBGWindow].pos.y - cosf(g_fAngleDown) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 2].pos.x = g_BGWindow[nCntBGWindow].pos.x - sinf(g_fAngleDown) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 2].pos.y = g_BGWindow[nCntBGWindow].pos.y + cosf(g_fAngleDown) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 3].pos.x = g_BGWindow[nCntBGWindow].pos.x + sinf(g_fAngleUp) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 3].pos.y = g_BGWindow[nCntBGWindow].pos.y + cosf(g_fAngleUp) * g_BGWindow[nCntBGWindow].fLenght;
			}
			else
			{// それ以外
				pVtx[nCntBGWindow * 4].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y + g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 3].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y + g_BGWindow[nCntBGWindow].fHeight, 0.0f);
			}

			// 頂点カラー設定
			pVtx[nCntBGWindow * 4].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 1].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 2].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 3].col = g_BGWindow[nCntBGWindow].col;

			// テクスチャ設定
			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_PLAYER || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_COIN || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL)
			{// ゲームプレイヤー、コイン、ゴール
				
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U, g_BGWindow[nCntBGWindow].nPatternAnim * BGWINDOW_TEXTURE_ANIM_UV_V);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U + BGWINDOW_TEXTURE_ANIM_UV_U, g_BGWindow[nCntBGWindow].nPatternAnim * BGWINDOW_TEXTURE_ANIM_UV_V);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U, g_BGWindow[nCntBGWindow].nPatternAnim * BGWINDOW_TEXTURE_ANIM_UV_V + BGWINDOW_TEXTURE_ANIM_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U + BGWINDOW_TEXTURE_ANIM_UV_U, g_BGWindow[nCntBGWindow].nPatternAnim * BGWINDOW_TEXTURE_ANIM_UV_V + BGWINDOW_TEXTURE_ANIM_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_NUMSTAGE)
			{// ゲームステージ番号
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_STAGE_UV_V);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_STAGE_UV_U, g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_STAGE_UV_V);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_STAGE_UV_V + BGWINDOW_TEXTURE_STAGE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_STAGE_UV_U, g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_STAGE_UV_V + BGWINDOW_TEXTURE_STAGE_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL_SMALL)
			{
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U + BGWINDOW_TEXTURE_ANIM_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].nNumAnim * BGWINDOW_TEXTURE_ANIM_UV_U + BGWINDOW_TEXTURE_ANIM_UV_U, BGWINDOW_TEXTURE_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_IWA || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL)
			{// ゲーム背景岩、木
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + g_BGWindow[nCntBGWindow].texUV.x, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + g_BGWindow[nCntBGWindow].texUV.x + g_BGWindow[nCntBGWindow].texUV.x, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + g_BGWindow[nCntBGWindow].texUV.x, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + g_BGWindow[nCntBGWindow].texUV.x + g_BGWindow[nCntBGWindow].texUV.x, BGWINDOW_TEXTURE_UV_V);
			}
			else
			{// それ以外
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + BGWINDOW_TEXTURE_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + BGWINDOW_TEXTURE_UV_U + BGWINDOW_TEXTURE_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + BGWINDOW_TEXTURE_UV_U, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].tex.x + BGWINDOW_TEXTURE_UV_U + BGWINDOW_TEXTURE_UV_U, BGWINDOW_TEXTURE_UV_V);
			}
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffBGWindow->Unlock();

	if (g_bGoal == true)
	{
		g_nCntBGTimer++;
		if (g_nCntBGTimer == 60)
		{
			PlaySound(SOUND_LABEL_SE_GAME_STAGE_CLEAR);

			SetHiScore();
			SetStageClearState();
			//SetFadeGame(GAMESTATE_MAP);		// ゲームマップ
		}
	}
}

//=========================================================================================================================
// 背景描画処理
//=========================================================================================================================
void DrawBGWindow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntBGWindow;		// 背景カウンタ

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBGWindow,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// 背景をカウント
		if (g_BGWindow[nCntBGWindow].bUse == true)
		{// 使用している場合
			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureBGWindow[g_BGWindow[nCntBGWindow].texture]);

			// 背景の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBGWindow * 4, 2);
		}
	}
}

//=========================================================================================================================
// 背景設定処理
//=========================================================================================================================
void SetBGWindow(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, BGWINDOWTEXTURE texture)
{
	// 変数宣言
	int nCntBGWindow;	// 背景カウンタ

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBGWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// 背景をカウント
		if (g_BGWindow[nCntBGWindow].bUse == false)
		{// 使用していない場合
			g_BGWindow[nCntBGWindow].pos = pos;				// 中心値
			g_BGWindow[nCntBGWindow].tex = pos;				// 中心値
			g_BGWindow[nCntBGWindow].col = col;				// 色
			g_BGWindow[nCntBGWindow].fWidth = fWidth;		// 横幅
			g_BGWindow[nCntBGWindow].fHeight = fHeight;		// 高さ
			g_BGWindow[nCntBGWindow].texture = texture;		// テクスチャ
			g_BGWindow[nCntBGWindow].bUse = true;			// 使用している状態へ

			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL)
			{
				g_bGoal = false;
			}

			// 頂点設定
			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_TITLE_GOUND)
			{// 回転
				g_BGWindow[nCntBGWindow].fAngle = atan2f(
					g_BGWindow[nCntBGWindow].pos.x - (g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth),
					g_BGWindow[nCntBGWindow].pos.y - (g_BGWindow[nCntBGWindow].pos.y + g_BGWindow[nCntBGWindow].fHeight));
				g_BGWindow[nCntBGWindow].fLenght = sqrtf(
					(g_BGWindow[nCntBGWindow].fWidth * 	g_BGWindow[nCntBGWindow].fWidth) + 
					(g_BGWindow[nCntBGWindow].fHeight * g_BGWindow[nCntBGWindow].fHeight));
				g_fAngleUp = g_BGWindow[nCntBGWindow].fAngle;
				g_fAngleDown = g_BGWindow[nCntBGWindow].fAngle;
				
				pVtx[nCntBGWindow * 4].pos.x = g_BGWindow[nCntBGWindow].pos.x - sinf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4].pos.y = g_BGWindow[nCntBGWindow].pos.y - cosf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 1].pos.x = g_BGWindow[nCntBGWindow].pos.x + sinf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 1].pos.y = g_BGWindow[nCntBGWindow].pos.y - cosf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 2].pos.x = g_BGWindow[nCntBGWindow].pos.x - sinf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 2].pos.y = g_BGWindow[nCntBGWindow].pos.y + cosf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 3].pos.x = g_BGWindow[nCntBGWindow].pos.x + sinf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
				pVtx[nCntBGWindow * 4 + 3].pos.y = g_BGWindow[nCntBGWindow].pos.y + cosf(g_BGWindow[nCntBGWindow].fAngle) * g_BGWindow[nCntBGWindow].fLenght;
			}
			else
			{// それ以外
				pVtx[nCntBGWindow * 4].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y + g_BGWindow[nCntBGWindow].fHeight, 0.0f);
				pVtx[nCntBGWindow * 4 + 3].pos = D3DXVECTOR3(g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth, g_BGWindow[nCntBGWindow].pos.y + g_BGWindow[nCntBGWindow].fHeight, 0.0f);
			}

			// 頂点カラー設定
			pVtx[nCntBGWindow * 4].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 1].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 2].col = g_BGWindow[nCntBGWindow].col;
			pVtx[nCntBGWindow * 4 + 3].col = g_BGWindow[nCntBGWindow].col;
			
			// テクスチャ設定
			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_PLAYER || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_COIN || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL)
			{// アニメーション
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_ANIM_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_ANIM_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_ANIM_UV_U, BGWINDOW_TEXTURE_ANIM_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_NUMSTAGE)
			{
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_STAGE_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_STAGE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_STAGE_UV_U, BGWINDOW_TEXTURE_STAGE_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL_SMALL)
			{
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_ANIM_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_ANIM_UV_U, BGWINDOW_TEXTURE_UV_V);
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_IWA || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER || g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL)
			{
				if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_IWA)
				{
					g_BGWindow[nCntBGWindow].texUV.x = BGWINDOW_TEXTURE_UV_U / (256 / g_BGWindow[nCntBGWindow].fWidth);
				}
				else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER)
				{
					g_BGWindow[nCntBGWindow].texUV.x = BGWINDOW_TEXTURE_UV_U / (400 / g_BGWindow[nCntBGWindow].fWidth);
				}
				else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL)
				{
					g_BGWindow[nCntBGWindow].texUV.x = BGWINDOW_TEXTURE_UV_U / (150 / g_BGWindow[nCntBGWindow].fWidth);
				}

				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].texUV.x, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(g_BGWindow[nCntBGWindow].texUV.x, BGWINDOW_TEXTURE_UV_V);
			}
			else
			{// それ以外
				pVtx[nCntBGWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBGWindow * 4 + 1].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_UV_U, 0.0f);
				pVtx[nCntBGWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, BGWINDOW_TEXTURE_UV_V);
				pVtx[nCntBGWindow * 4 + 3].tex = D3DXVECTOR2(BGWINDOW_TEXTURE_UV_U, BGWINDOW_TEXTURE_UV_V);
			}

			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBGWindow->Unlock();
}

//=========================================================================================================================
// 選択設定処理
//=========================================================================================================================
void SetSelectState(SELECTSTATE state)
{
	g_SelectState = state;		// 状態を記憶
	g_nCntBGTimer = 0;			// 時間カウンタ初期化
}

//=========================================================================================================================
// 選択設定処理
//=========================================================================================================================
void HitPlayerLife(void)
{
	// 変数宣言
	int nCntBGWindow;
	
	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// 背景をカウント
		if (g_BGWindow[nCntBGWindow].bUse == true && g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_LIFE)
		{// ライフを使用している場合
			DeleteBGWindow(nCntBGWindow);		// 背景破棄処理
			break;
		}
	}
}

//=========================================================================================================================
// 背景衝突処理
//=========================================================================================================================
bool CollisionBGWindow(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	// 変数宣言
	int nCntBGWindow;
	bool bLand = false;
	pPosOld->x -= pMove->x;

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{
		if (g_BGWindow[nCntBGWindow].bUse == true)
		{
			if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_HELP)
			{
				if (pPos->x + (fWidth / 2) > g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth && pPos->x - (fWidth / 2) < g_BGWindow[nCntBGWindow].pos.x + g_BGWindow[nCntBGWindow].fWidth)
				{
					if (pPos->y >= g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight + 23 && g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight + 23 >= pPosOld->y)
					{
						pPos->y = g_BGWindow[nCntBGWindow].pos.y - g_BGWindow[nCntBGWindow].fHeight + 23;
						pMove->y = 0.0f;
						bLand = true;
					}
				}
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT)
			{
				if (pPos->x + (fWidth / 2) >= g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth && g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth >= pPosOld->x + (fWidth / 2))
				{
					pPos->x = g_BGWindow[nCntBGWindow].pos.x - g_BGWindow[nCntBGWindow].fWidth - (fWidth / 2);
					pMove->x = 0.0f;
				}
			}
			else if (g_BGWindow[nCntBGWindow].texture == BGWINDOWTEXTURE_GAME_STAGEGOAL)
			{
				if (pPos->x >= g_BGWindow[nCntBGWindow].pos.x - 10)
				{
					g_bGoal = true;
				}
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// ゴール判定取得処理
//=========================================================================================================================
bool GetGoalHit(void)
{
	return g_bGoal;
}

//=========================================================================================================================
// 背景破棄処理
//=========================================================================================================================
void DeleteBGWindow(int nCntBGWindow)
{
	g_BGWindow[nCntBGWindow].bUse = false;
}
void DeleteBGWindow(void)
{
	// 変数宣言
	int nCntBGWindow;		// 背景カウンタ

	for (nCntBGWindow = 0; nCntBGWindow < MAX_BGWINDOW_POLYGON; nCntBGWindow++)
	{// 背景をカウント
		if (g_BGWindow[nCntBGWindow].bUse == true)
		{// 使用している場合
			g_BGWindow[nCntBGWindow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 背景の中心値
			g_BGWindow[nCntBGWindow].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 背景の中心値
			g_BGWindow[nCntBGWindow].texUV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// UV
			g_BGWindow[nCntBGWindow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 背景の移動値
			g_BGWindow[nCntBGWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 背景色
			g_BGWindow[nCntBGWindow].fWidth = 0;								// 横幅
			g_BGWindow[nCntBGWindow].fHeight = 0;								// 高さ
			g_BGWindow[nCntBGWindow].fAngle = 0;								// 角度
			g_BGWindow[nCntBGWindow].fLenght = 0;								// 長さ
			g_BGWindow[nCntBGWindow].nCounterAnim = 0;							// アニメーションカウンタ
			g_BGWindow[nCntBGWindow].nNumAnim = 0;								// アニメーション番号
			g_BGWindow[nCntBGWindow].nSpeedAnim = 0;							// アニメーション速度
			g_BGWindow[nCntBGWindow].nPatternAnim = 0;							// アニメーションパターン数
			g_BGWindow[nCntBGWindow].bUse = false;								// 使用しているか
		}
	}

	g_SelectState = SELECTSTATE_NORMAL;											// 選択状態	
	g_fAngleUp = 0.0f;															// 初期化
	g_fAngleDown = 0.0f;														// 初期化
	g_nCntBGTimer = 0;															// 初期化
	g_nCntBGTimer++;
	g_bGoal = false;
}