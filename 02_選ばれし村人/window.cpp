//=========================================================================================================================
// 
// ウィンドウ処理 [frontwindow.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "window.h"		// ウィンドウ
#include "xweapon.h"
#include "xplayer.h"	
#include "input.h"		// 入力処理
#include "fade.h"		// フェード
#include "sound.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_WINDOW_POLYGON				(64)							// ポリゴンの最大数
#define WINDOW_TEXTURE_SPEED			(5)								// テクスチャアニメーションスピード
#define WINDOW_TEXTURE_PATTERN			(5)								// テクスチャアニメーションパターン数
#define WINDOW_TEXTURE_UV_U				(1.0f)							// テクスチャアニメーションU範囲
#define WINDOW_TEXTURE_UV_V				(1.0f)							// テクスチャアニメーションV範囲
#define WINDOW_TEXTURE_SPEED_TEX2		(3)								// テクスチャアニメーション
#define WINDOW_TEXTURE_PATTERN_TEX2		(20)							// テクスチャアニメーション
#define WINDOW_TEXTURE_PATTERN_U_TEX2	(2)								// テクスチャアニメーションU
#define WINDOW_TEXTURE_UV_U_TEX2		(0.5f)							// テクスチャアニメーションU範囲
#define WINDOW_TEXTURE_UV_V_TEX2		(0.1f)							// テクスチャアニメーションV範囲
#define WINDOW_TEXTURE_PATTERN_1		(4)								// テクスチャアニメーション
#define WINDOW_TEXTURE_UV_U_1			(0.25f)							// テクスチャアニメーションU範囲
#define WINDOW_TEXTURE_UV_V_1			(0.2f)							// テクスチャアニメーションV範囲
#define WINDOW_TEXTURE_UV_U_3			(0.2f)							// テクスチャアニメーションU範囲
#define WINDOW_TEXTURE_UV_V_3			(1.0f)							// テクスチャアニメーションV範囲
#define WINDOW_SPEED					(0.002f)						// ウィンドウスピード

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureWindow[WINDOWTEXTURE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffWindow = NULL;					// 頂点バッファへのポインタ
WINDOW							g_Window[MAX_WINDOW_POLYGON];				// ウィンドウの情報
SELECTSTATE						g_SelectState;								// 選択の状態
float							g_fAngleUp;									// 角度加算
float							g_fAngleDown;								// 角度減算

//=========================================================================================================================
// ウィンドウ初期化処理
//=========================================================================================================================
void InitWindow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスを取得
	int nCntWindow;							// ウィンドウカウンタ

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
		g_Window[nCntWindow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ウィンドウの中心値
		g_Window[nCntWindow].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ウィンドウの中心値
		g_Window[nCntWindow].texUV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// UV
		g_Window[nCntWindow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ウィンドウの移動値
		g_Window[nCntWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ウィンドウ色
		g_Window[nCntWindow].fWidth = 0;								// 横幅
		g_Window[nCntWindow].fHeight = 0;								// 高さ
		g_Window[nCntWindow].fAngle = 0;								// 角度
		g_Window[nCntWindow].fLenght = 0;								// 長さ
		g_Window[nCntWindow].nCntTimer = 0;								// 時間カウンタ
		g_Window[nCntWindow].nCounterAnim = 0;							// アニメーションカウンタ
		g_Window[nCntWindow].nNumAnim = 0;								// アニメーション番号
		g_Window[nCntWindow].nSpeedAnim = 0;							// アニメーション速度
		g_Window[nCntWindow].nPatternAnim = 0;							// アニメーションパターン数
		g_Window[nCntWindow].bUse = false;								// 使用しているか
	}
	g_SelectState = SELECTSTATE_NORMAL;									// 選択状態	
	g_fAngleUp = 0.0f;													// 初期化
	g_fAngleDown = 0.0f;												// 初期化

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_1, &g_pTextureWindow[0]);		// タイトル（タイトル）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_2, &g_pTextureWindow[1]);		// タイトル（アニメーション）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_3, &g_pTextureWindow[2]);		// ゲーム（チュートリアル）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_4, &g_pTextureWindow[3]);		// ゲーム（チュートリアル）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_5, &g_pTextureWindow[4]);		// ゲーム（ゲームチュートリアル）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_6, &g_pTextureWindow[5]);		// ゲーム（チュートリアルコメント開く）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_7, &g_pTextureWindow[6]);		// ゲーム（チュートリアルコメント閉じる）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_8, &g_pTextureWindow[7]);		// ゲーム（チュートリアルメッセージ）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_9, &g_pTextureWindow[8]);		// ゲーム（マップ）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_10, &g_pTextureWindow[9]);	// ゲーム（マップ位置）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_11, &g_pTextureWindow[10]);	// ゲーム（HPBG）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_12, &g_pTextureWindow[11]);	// ゲーム（HP）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_13, &g_pTextureWindow[12]);	// ゲーム（HPマーク）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_14, &g_pTextureWindow[13]);	// ゲーム（武器1）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_15, &g_pTextureWindow[14]);	// ゲーム（武器2）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_16, &g_pTextureWindow[15]);	// ゲーム（武器3）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_17, &g_pTextureWindow[16]);	// ゲーム（武器枠）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_18, &g_pTextureWindow[17]);	// リザルト（背景）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_19, &g_pTextureWindow[18]);	// リザルト（クリア）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_20, &g_pTextureWindow[19]);	// リザルト（オーバー）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_21, &g_pTextureWindow[20]);	// リザルト（おまけ）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_22, &g_pTextureWindow[21]);	// ランキング（文字）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_23, &g_pTextureWindow[22]);	// ランキング（1）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_24, &g_pTextureWindow[23]);	// ランキング（2）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_25, &g_pTextureWindow[24]);	// ランキング（3）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_26, &g_pTextureWindow[25]);	// ランキング（4）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_27, &g_pTextureWindow[26]);	// ランキング（5）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_28, &g_pTextureWindow[27]);	// ランキング（メッセージA）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_29, &g_pTextureWindow[28]);	// ランキング（メッセージB）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_30, &g_pTextureWindow[29]);	// ランキング（メッセージC）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_31, &g_pTextureWindow[30]);	// ランキング（メッセージD）
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_32, &g_pTextureWindow[31]);	// メッセージ
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_33, &g_pTextureWindow[32]);	// 選択
	D3DXCreateTextureFromFile(pDevice, WINDOWTEXTURE_NAME_34, &g_pTextureWindow[33]);	// 選択

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_WINDOW_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWindow,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
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
		pVtx[1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, WINDOW_TEXTURE_UV_V);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffWindow->Unlock();
}

//=========================================================================================================================
// ウィンドウ終了処理
//=========================================================================================================================
void UninitWindow(void)
{
	// 変数宣言
	int nCntTex;		// テクスチャをカウント

	// テクスチャの破棄
	for (nCntTex = 0; nCntTex < WINDOWTEXTURE_MAX; nCntTex++)
	{// テクスチャをカウント
		if (g_pTextureWindow[nCntTex] != NULL)
		{
			g_pTextureWindow[nCntTex]->Release();
			g_pTextureWindow[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffWindow != NULL)
	{
		g_pVtxBuffWindow->Release();
		g_pVtxBuffWindow = NULL;
	}
}

//=========================================================================================================================
// ウィンドウ更新処理
//=========================================================================================================================
void UpdateWindow(void)
{
	int nCntWindow;
	MODE mode = GetMode();
	XPLAYER *pXPlayer = GetXPlayer();
	float fHP;
	float fHPRadius;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
		if (g_Window[nCntWindow].bUse == true)
		{// 使用している場合
			switch (g_Window[nCntWindow].texture)
			{
			case WINDOWTEXTURE_TITLE:	// タイトル

				// 拡大
				if (g_Window[nCntWindow].fWidth != SCREEN_WIDTH / 2 - 100 && g_Window[nCntWindow].fHeight != SCREEN_HEIGHT / 2 - 100)
				{
					if (g_Window[nCntWindow].fWidth >= SCREEN_WIDTH / 2 - 100 && g_Window[nCntWindow].fHeight >= SCREEN_HEIGHT / 2 - 100)
					{
						g_Window[nCntWindow].fWidth = SCREEN_WIDTH / 2 - 100;
						g_Window[nCntWindow].fHeight = SCREEN_HEIGHT / 2 - 100;
					}
					else
					{
						g_Window[nCntWindow].fWidth += (SCREEN_WIDTH / 2 - 100) * 0.1f;
						g_Window[nCntWindow].fHeight += (SCREEN_HEIGHT / 2 - 100) * 0.1f;
					}
				}

				break;
			case WINDOWTEXTURE_TITLE_ANIM:	// タイトルアニメーション

				// アニメーション
				g_Window[nCntWindow].nCounterAnim++;

				if (g_Window[nCntWindow].nCounterAnim % WINDOW_TEXTURE_SPEED_TEX2 == 0)
				{
					if (g_Window[nCntWindow].nNumAnim < WINDOW_TEXTURE_PATTERN_TEX2)
					{
						g_Window[nCntWindow].nNumAnim += 1;
					}
					else
					{
						DeleteWindow(nCntWindow);
					}

					if (g_Window[nCntWindow].nNumAnim == 15)
					{
						SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0, WINDOWTEXTURE_TITLE, SETTEXTURE_TOPFRONT);
						SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 120, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_SELECT_1, SETTEXTURE_TOPFRONT);
					}
				}

				break;
			case WINDOWTEXTURE_GAME_TUTORIAL:	// チュートリアル

				// チュートリアル
				if (g_Window[nCntWindow].fWidth != SCREEN_WIDTH / 2 - 100 && g_Window[nCntWindow].fHeight != SCREEN_HEIGHT / 2 - 100)
				{
					if (g_Window[nCntWindow].fWidth >= SCREEN_WIDTH / 2 - 100 && g_Window[nCntWindow].fHeight >= SCREEN_HEIGHT / 2 - 100)
					{
						g_Window[nCntWindow].fWidth = SCREEN_WIDTH / 2 - 100;
						g_Window[nCntWindow].fHeight = SCREEN_HEIGHT / 2 - 100;
					}
					else
					{
						g_Window[nCntWindow].fWidth += (SCREEN_WIDTH / 2 - 100) * 0.1f;
						g_Window[nCntWindow].fHeight += (SCREEN_HEIGHT / 2 - 100) * 0.1f;

						if (g_Window[nCntWindow].fWidth + (SCREEN_WIDTH / 2 - 100) * 0.1f == SCREEN_WIDTH / 2 - 100 &&
							g_Window[nCntWindow].fHeight + (SCREEN_HEIGHT / 2 - 100) * 0.1f == SCREEN_HEIGHT / 2 - 100)
						{
							SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 200, 30, WINDOWTEXTURE_GAME_TUTORIAL_MESSAGE, SETTEXTURE_FRONT);
						}
					}
				}

				break;
			case WINDOWTEXTURE_GAME_TUTORIAL_P:
				break;
			case WINDOWTEXTURE_GAME_HP:		// HP

				fHP = (float)pXPlayer->nLife / (float)pXPlayer->nInitLife;

				fHPRadius = g_Window[nCntWindow].fHPWidth * fHP;

				if (fHP < 0.3f)
				{
					g_Window[nCntWindow].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else if (fHP < 0.5f)
				{
					g_Window[nCntWindow].col = D3DXCOLOR(1.0f, 0.8f, 0.1f, 1.0f);
				}
				else if (fHP < 1.0f)
				{
					g_Window[nCntWindow].col = D3DXCOLOR(0.3f, 1.0f, 0.1f, 1.0f);
				}

				break;
			case WINDOWTEXTURE_GAME_WEAPON_1:	// 武器

				if (pXPlayer->nWeapon_1Lv < 3)
				{
					g_Window[nCntWindow].nNumAnim = 0;
				}
				else if (pXPlayer->nWeapon_1Lv < 6)
				{
					g_Window[nCntWindow].nNumAnim = 1;
				}
				else if (pXPlayer->nWeapon_1Lv < 9)
				{
					g_Window[nCntWindow].nNumAnim = 2;
				}
				else if (pXPlayer->nWeapon_1Lv < 12)
				{
					g_Window[nCntWindow].nNumAnim = 3;
				}
				else
				{
					g_Window[nCntWindow].nNumAnim = 4;
				}

				break;
			case WINDOWTEXTURE_GAME_WEAPON_2:	// 武器２

				if (pXPlayer->nWeapon_2Lv < 3)
				{
					g_Window[nCntWindow].nNumAnim = 0;
				}
				else if (pXPlayer->nWeapon_2Lv < 6)
				{
					g_Window[nCntWindow].nNumAnim = 1;
				}
				else if (pXPlayer->nWeapon_2Lv < 9)
				{
					g_Window[nCntWindow].nNumAnim = 2;
				}
				else if (pXPlayer->nWeapon_2Lv < 12)
				{
					g_Window[nCntWindow].nNumAnim = 3;
				}
				else
				{
					g_Window[nCntWindow].nNumAnim = 4;
				}

				break;
			case WINDOWTEXTURE_GAME_WEAPON_3:	// 武器３

				if (pXPlayer->nWeapon_3Lv < 3)
				{
					g_Window[nCntWindow].nNumAnim = 0;
				}
				else if (pXPlayer->nWeapon_3Lv < 6)
				{
					g_Window[nCntWindow].nNumAnim = 1;
				}
				else if (pXPlayer->nWeapon_3Lv < 9)
				{
					g_Window[nCntWindow].nNumAnim = 2;
				}
				else if (pXPlayer->nWeapon_3Lv < 12)
				{
					g_Window[nCntWindow].nNumAnim = 3;
				}
				else
				{
					g_Window[nCntWindow].nNumAnim = 4;
				}

				break;
			case WINDOWTEXTURE_GAME_WEAPON_99:

				if (pXPlayer->nWeapon == 0)
				{
					for (int nCnt = 0; nCnt < MAX_WINDOW_POLYGON; nCnt++)
					{// ウィンドウをカウント
						if (g_Window[nCnt].bUse == true && g_Window[nCnt].texture == WINDOWTEXTURE_GAME_WEAPON_1)
						{// 使用している場合
							g_Window[nCntWindow].pos = g_Window[nCnt].pos;

							break;
						}
					}
				}
				else if (pXPlayer->nWeapon == 1)
				{
					for (int nCnt = 0; nCnt < MAX_WINDOW_POLYGON; nCnt++)
					{// ウィンドウをカウント
						if (g_Window[nCnt].bUse == true && g_Window[nCnt].texture == WINDOWTEXTURE_GAME_WEAPON_2)
						{// 使用している場合
							g_Window[nCntWindow].pos = g_Window[nCnt].pos;

							break;
						}
					}
				}
				else if (pXPlayer->nWeapon == 2)
				{
					for (int nCnt = 0; nCnt < MAX_WINDOW_POLYGON; nCnt++)
					{// ウィンドウをカウント
						if (g_Window[nCnt].bUse == true && g_Window[nCnt].texture == WINDOWTEXTURE_GAME_WEAPON_3)
						{// 使用している場合
							g_Window[nCntWindow].pos = g_Window[nCnt].pos;

							break;
						}
					}
				}

				break;
			case WINDOWTEXTURE_RESULT_BG:	// リザルト

				g_Window[nCntWindow].nCntTimer++;

				if (g_Window[nCntWindow].nCntTimer > 60)
				{
					if (g_Window[nCntWindow].pos.x <= SCREEN_WIDTH / 2)
					{
						g_Window[nCntWindow].pos.x = SCREEN_WIDTH / 2;
					}
					else
					{
						g_Window[nCntWindow].pos.x -= 20.0f;
					}
				}

				break;
			case WINDOWTEXTURE_RESULT_CLEAR:	// クリア
			case WINDOWTEXTURE_RESULT_OVER:		// オーバー

				if (g_Window[nCntWindow].fWidth != SCREEN_WIDTH / 2 && g_Window[nCntWindow].fHeight != SCREEN_HEIGHT / 2)
				{
					g_Window[nCntWindow].fWidth -= (SCREEN_WIDTH / 4);
					g_Window[nCntWindow].fHeight -= (SCREEN_HEIGHT / 4);

					if (g_Window[nCntWindow].fWidth == SCREEN_WIDTH / 2 && g_Window[nCntWindow].fHeight == SCREEN_HEIGHT / 2)
					{
						PlaySound(SOUND_LABEL_SE_RESULT_3);
					}
				}

				if (mode == MODE_RESULT)
				{
					if (g_Window[nCntWindow].pos.y > 180.0f)
					{
						g_Window[nCntWindow].pos.y -= 10.0f;
					}
				}

				break;
			case WINDOWTEXTURE_RESULT_OTHER:

				g_Window[nCntWindow].nCounterAnim++;

				if (g_Window[nCntWindow].nCounterAnim % 3 == 0)
				{
					if (g_Window[nCntWindow].nNumAnim < 19)
					{
						g_Window[nCntWindow].nNumAnim += 1;
					}
					//g_Window[nCntWindow].nNumAnim = (g_Window[nCntWindow].nNumAnim + 1) % 49;
				}

				//if (GetKeyboardTrigger(DIK_1) == true)
				{
					//g_Window[nCntWindow].nNumAnim = 0;
				}

				break;
			case WINDOWTEXTURE_SELECT_1:		// 選択
			case WINDOWTEXTURE_SELECT_2:		// 選択

				g_Window[nCntWindow].nCntTimer++;

				if (g_SelectState == SELECTSTATE_NORMAL)
				{// 選択ノーマル
					if (g_Window[nCntWindow].nCntTimer == 80) { g_Window[nCntWindow].nCntTimer = 0; }

					// 点滅
					if (g_Window[nCntWindow].nCntTimer < 40) { g_Window[nCntWindow].col.a = 1.0f; }
					else { g_Window[nCntWindow].col.a = 0.0f; }
				}
				else if (g_SelectState == SELECTSTATE_ENTER)
				{// 選択エンター
					if (g_Window[nCntWindow].nCntTimer == 10)
					{
						g_Window[nCntWindow].nCntTimer = 0;
					}

					// 点滅
					if (g_Window[nCntWindow].nCntTimer < 5) { g_Window[nCntWindow].col.a = 1.0f; }
					else { g_Window[nCntWindow].col.a = 0.0f; }
				}

				break;
			case WINDOW_TUTORIAL:	// チュートリアル

				g_Window[nCntWindow].nCntTimer++;

				if (g_Window[nCntWindow].nCntTimer < 30)
				{
					g_Window[nCntWindow].col.a += (1.0f / 30.0f);
				}
				if (g_Window[nCntWindow].nCntTimer == 35)
				{
					SetXWeapon(WEAPONTYPE_SWORD);
				}
				else if (g_Window[nCntWindow].nCntTimer >= 40)
				{
					g_Window[nCntWindow].col.a -= (1.0f / 30.0f);
				}

				if (g_Window[nCntWindow].nCntTimer == 70)
				{
					DeleteWindow(WINDOW_TUTORIAL);
				}

				break;
			case WINDOW_FADE:	// フェード

				g_Window[nCntWindow].nCntTimer++;

				if (g_Window[nCntWindow].nCntTimer < 30)
				{
					g_Window[nCntWindow].col.a += (1.0f / 30.0f);
				}
				else if (g_Window[nCntWindow].nCntTimer >= 40)
				{
					g_Window[nCntWindow].col.a -= (1.0f / 30.0f);
				}

				if (g_Window[nCntWindow].nCntTimer == 70)
				{
					DeleteWindow(WINDOW_FADE);
				}

				break;
			}

			// 頂点設定
			if (g_Window[nCntWindow].texture == WINDOWTEXTURE_GAME_HP)
			{// HP
				pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + (fHPRadius - g_Window[nCntWindow].fWidth), g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + (fHPRadius - g_Window[nCntWindow].fWidth), g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);
			}
			else
			{// その他
				pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);
				pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);
			}
			
			// 頂点カラー設定
			pVtx[nCntWindow * 4].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 1].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_Window[nCntWindow].col;

			// テクスチャ設定
			if (WINDOWTEXTURE_TITLE_ANIM == g_Window[nCntWindow].texture)
			{// タイトルアニメーション
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_U_TEX2, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_V_TEX2);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_U_TEX2 + WINDOW_TEXTURE_UV_U_TEX2, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_V_TEX2);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_U_TEX2, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_V_TEX2 + WINDOW_TEXTURE_UV_V_TEX2);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_U_TEX2 + WINDOW_TEXTURE_UV_U_TEX2, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_U_TEX2) * WINDOW_TEXTURE_UV_V_TEX2 + WINDOW_TEXTURE_UV_V_TEX2);
			}
			else if (WINDOWTEXTURE_RESULT_OTHER == g_Window[nCntWindow].texture)
			{// リザルト
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_U_1, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_V_1);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_U_1 + WINDOW_TEXTURE_UV_U_1, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_V_1);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_U_1, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_V_1 + WINDOW_TEXTURE_UV_V_1);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2((g_Window[nCntWindow].nNumAnim % WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_U_1 + WINDOW_TEXTURE_UV_U_1, (g_Window[nCntWindow].nNumAnim / WINDOW_TEXTURE_PATTERN_1) * WINDOW_TEXTURE_UV_V_1 + WINDOW_TEXTURE_UV_V_1);
			}
			else if (
				WINDOWTEXTURE_GAME_WEAPON_1 == g_Window[nCntWindow].texture ||
				WINDOWTEXTURE_GAME_WEAPON_2 == g_Window[nCntWindow].texture ||
				WINDOWTEXTURE_GAME_WEAPON_3 == g_Window[nCntWindow].texture)
			{// 武器
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(g_Window[nCntWindow].nNumAnim * WINDOW_TEXTURE_UV_U_3, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(g_Window[nCntWindow].nNumAnim * WINDOW_TEXTURE_UV_U_3 + WINDOW_TEXTURE_UV_U_3, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(g_Window[nCntWindow].nNumAnim * WINDOW_TEXTURE_UV_U_3, WINDOW_TEXTURE_UV_V_3);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(g_Window[nCntWindow].nNumAnim * WINDOW_TEXTURE_UV_U_3 + WINDOW_TEXTURE_UV_U_3, WINDOW_TEXTURE_UV_V_3);
			}
			else
			{// その他
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, WINDOW_TEXTURE_UV_V);
			}
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffWindow->Unlock();	
}

//=========================================================================================================================
// ウィンドウ描画処理
//=========================================================================================================================
void DrawTopFrontWindow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntWindow;		// ウィンドウカウンタ

						// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffWindow,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].type == SETTEXTURE_TOPFRONT)
		{// 使用している場合
			// テクスチャ設定
			if (g_Window[nCntWindow].texture >= WINDOWTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureWindow[g_Window[nCntWindow].texture]);
			}

			// ウィンドウの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, 2);
		}
	}
}
void DrawFrontWindow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntWindow;		// ウィンドウカウンタ

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffWindow,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].type == SETTEXTURE_FRONT)
		{// 使用している場合
			// テクスチャ設定
			if (g_Window[nCntWindow].texture >= WINDOWTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureWindow[g_Window[nCntWindow].texture]);
			}

			// ウィンドウの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, 2);
		}
	}
}
void DrawTopBGWindow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntWindow;		// ウィンドウカウンタ

						// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffWindow,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].type == SETTEXTURE_TOPBG)
		{// 使用している場合

			if (g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_CLEAR || g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_OVER)
			{
				// アルファテストの設定
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			}

			// テクスチャ設定
			if (g_Window[nCntWindow].texture >= WINDOWTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureWindow[g_Window[nCntWindow].texture]);
			}

			// ウィンドウの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, 2);

			if (g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_CLEAR || g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_OVER)
			{
				// アルファテストを無効
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
		}
	}
}
void DrawBGWindow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntWindow;		// ウィンドウカウンタ

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffWindow,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].type == SETTEXTURE_BG)
		{// 使用している場合

			if (g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_CLEAR || g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_OVER)
			{
				// アルファテストの設定
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			}

			// テクスチャ設定
			if (g_Window[nCntWindow].texture >= WINDOWTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureWindow[g_Window[nCntWindow].texture]);
			}

			// ウィンドウの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, 2);

			if (g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_CLEAR || g_Window[nCntWindow].texture == WINDOWTEXTURE_RESULT_OVER)
			{
				// アルファテストを無効
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
		}
	}
}

//=========================================================================================================================
// ウィンドウ設定処理
//=========================================================================================================================
void SetWindow(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, WINDOWTEXTURE texture, SETTEXTURE type)
{
	// 変数宣言
	int nCntWindow;	// ウィンドウカウンタ

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffWindow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
		if (g_Window[nCntWindow].bUse == false)
		{// 使用していない場合
			g_Window[nCntWindow].pos = pos;				// 中心値
			g_Window[nCntWindow].tex = pos;				// 中心値
			g_Window[nCntWindow].col = col;				// 色
			g_Window[nCntWindow].fWidth = fWidth;		// 横幅
			if (texture == WINDOWTEXTURE_GAME_HP)
			{
				g_Window[nCntWindow].fHPWidth = fWidth * 2;	// 横幅
			}
			g_Window[nCntWindow].fHeight = fHeight;		// 高さ
			g_Window[nCntWindow].texture = texture;		// テクスチャ種類
			g_Window[nCntWindow].type = type;			// 表示タイプ
			g_Window[nCntWindow].nCntTimer = 0;			// 時間カウンタ
			g_Window[nCntWindow].nCounterAnim = 0;		// アニメーションカウンタ
			g_Window[nCntWindow].nNumAnim = 0;			// アニメーション番号
			g_Window[nCntWindow].nSpeedAnim = 0;		// アニメーション速度
			g_Window[nCntWindow].nPatternAnim = 0;		// アニメーションパターン数
			g_Window[nCntWindow].bUse = true;			// 使用している状態へ

			// 頂点設定
			pVtx[nCntWindow * 4].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
			pVtx[nCntWindow * 4 + 1].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y - g_Window[nCntWindow].fHeight, 0.0f);
			pVtx[nCntWindow * 4 + 2].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x - g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);
			pVtx[nCntWindow * 4 + 3].pos = D3DXVECTOR3(g_Window[nCntWindow].pos.x + g_Window[nCntWindow].fWidth, g_Window[nCntWindow].pos.y + g_Window[nCntWindow].fHeight, 0.0f);

			// 頂点カラー設定
			pVtx[nCntWindow * 4].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 1].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 2].col = g_Window[nCntWindow].col;
			pVtx[nCntWindow * 4 + 3].col = g_Window[nCntWindow].col;

			// テクスチャ設定
			if (WINDOWTEXTURE_TITLE_ANIM == g_Window[nCntWindow].texture)
			{// タイトルアニメーション
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_TEX2, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V_TEX2);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_TEX2, WINDOW_TEXTURE_UV_V_TEX2);
			}
			else if (WINDOWTEXTURE_RESULT_OTHER == g_Window[nCntWindow].texture)
			{// リザルト
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_1, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V_1);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_1, WINDOW_TEXTURE_UV_V_1);
			}
			else if (WINDOWTEXTURE_GAME_WEAPON_1 == g_Window[nCntWindow].texture ||
				WINDOWTEXTURE_GAME_WEAPON_2 == g_Window[nCntWindow].texture ||
				WINDOWTEXTURE_GAME_WEAPON_3 == g_Window[nCntWindow].texture)
			{// 武器
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_3, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V_3);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U_3, WINDOW_TEXTURE_UV_V_3);
			}
			else
			{// その他
				pVtx[nCntWindow * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntWindow * 4 + 1].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, 0.0f);
				pVtx[nCntWindow * 4 + 2].tex = D3DXVECTOR2(0.0f, WINDOW_TEXTURE_UV_V);
				pVtx[nCntWindow * 4 + 3].tex = D3DXVECTOR2(WINDOW_TEXTURE_UV_U, WINDOW_TEXTURE_UV_V);
			}
			
			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffWindow->Unlock();
}

//=========================================================================================================================
// 選択設定処理
//=========================================================================================================================
void SetSelectState(SELECTSTATE state)
{
	// 変数宣言
	int nCntWindow;		// ウィンドウカウンタ

	g_SelectState = state;		// 状態を記憶

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].texture == WINDOWTEXTURE_SELECT_1 || g_Window[nCntWindow].texture == WINDOWTEXTURE_SELECT_2)
		{// 使用していない場合
			g_Window[nCntWindow].nCntTimer = 0;			// 時間カウンタ初期化
		}
	}
}

//=========================================================================================================================
// ウィンドウ破棄処理
//=========================================================================================================================
void DeleteWindow(int nCntWindow)
{
	g_Window[nCntWindow].bUse = false;
}
void DeleteWindow(WINDOWTEXTURE texture)
{
	// 変数宣言
	int nCntWindow;		// ウィンドウカウンタ

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
		if (g_Window[nCntWindow].bUse == true && g_Window[nCntWindow].texture == texture)
		{// 使用している場合
			g_Window[nCntWindow].bUse = false;
			break;
		}
	}
}
void DeleteWindow(void)
{
	// 変数宣言
	int nCntWindow;		// ウィンドウカウンタ

	for (nCntWindow = 0; nCntWindow < MAX_WINDOW_POLYGON; nCntWindow++)
	{// ウィンドウをカウント
		if (g_Window[nCntWindow].bUse == true)
		{// 使用している場合
			g_Window[nCntWindow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ウィンドウの中心値
			g_Window[nCntWindow].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ウィンドウの中心値
			g_Window[nCntWindow].texUV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// UV
			g_Window[nCntWindow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ウィンドウの移動値
			g_Window[nCntWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ウィンドウ色
			g_Window[nCntWindow].fWidth = 0;								// 横幅
			g_Window[nCntWindow].fHeight = 0;								// 高さ
			g_Window[nCntWindow].fAngle = 0;								// 角度
			g_Window[nCntWindow].fLenght = 0;								// 長さ
			g_Window[nCntWindow].nCntTimer = 0;								// 時間カウンタ
			g_Window[nCntWindow].nCounterAnim = 0;							// アニメーションカウンタ
			g_Window[nCntWindow].nNumAnim = 0;								// アニメーション番号
			g_Window[nCntWindow].nSpeedAnim = 0;							// アニメーション速度
			g_Window[nCntWindow].nPatternAnim = 0;							// アニメーションパターン数
			g_Window[nCntWindow].bUse = false;								// 使用しているか
		}
	}

	g_SelectState = SELECTSTATE_NORMAL;										// 選択状態	
	g_fAngleUp = 0.0f;														// 初期化
	g_fAngleDown = 0.0f;													// 初期化
}