//=========================================================================================================================
// 
// ポーズ処理 [pause.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "pause.h"		// ポーズ
#include "fade.h"		// フェード
#include "sound.h"
#include "score.h"
#include "window.h"
#include "input.h"		// 入力処理

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_PAUSE_TEXTURE		(6)								// ポーズテクスチャ最大数
#define PAUSE_TEXTURE_NAME_1	"data\\TEXTURE\\brack000.png"	// 読み込むテクスチャファイル1
#define PAUSE_TEXTURE_NAME_2	"data\\TEXTURE\\pause000.png"	// 読み込むテクスチャファイル2
#define PAUSE_TEXTURE_NAME_3	"data\\TEXTURE\\pause101.png"	// 読み込むテクスチャファイル3
#define PAUSE_TEXTURE_NAME_4	"data\\TEXTURE\\pause001.png"	// 読み込むテクスチャファイル4
#define PAUSE_TEXTURE_NAME_5	"data\\TEXTURE\\pause002.png"	// 読み込むテクスチャファイル5
#define PAUSE_TEXTURE_NAME_6	"data\\TEXTURE\\pause003.png"	// 読み込むテクスチャファイル6
#define PAUSE_POS_X				(0)								// ポーズの左上X座標
#define PAUSE_POS_Y				(0)								// ポーズの左上Y座標
#define PAUSE_WIDTH				(SCREEN_WIDTH)					// ポーズの幅
#define PAUSE_HEIGHT			(SCREEN_HEIGHT)					// ポーズの高さ
#define PAUSE_POS_X1			(0)								// ポーズの左上X座標
#define PAUSE_POS_Y1			(0)								// ポーズの左上Y座標
#define PAUSE_WIDTH1			(100)							// ポーズの幅
#define PAUSE_HEIGHT1			(100)							// ポーズの高さ
#define PAUSE_TEXTURE_SPEED		(5)								// テクスチャアニメーションスピード
#define PAUSE_TEXTURE_PATTERN	(10)							// テクスチャアニメーションパターン数
#define PAUSE_TEXTURE_UV_U		(1.0f)							// テクスチャアニメーションU範囲
#define PAUSE_TEXTURE_UV_V		(1.0f)							// テクスチャアニメーションV範囲
#define PAUSE_BACKTEXTURE_UV_U	(1.0f)							// 前テクスチャアニメーションU範囲
#define PAUSE_BACKTEXTURE_UV_V	(1.0f)							// 前テクスチャアニメーションV範囲
#define PAUSE_SPEED				(0.002f)						// ポーズスピード

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTexturePause[MAX_PAUSE_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffPause = NULL;						// 頂点バッファへのポインタ
D3DXVECTOR3						g_posPause;									// ポーズの位置
int								g_SelectNum;								// 選択番号
float							g_SelectColor;								// 選択色
float							g_SelectColorChange;						// 選択色変化
D3DXVECTOR3						g_SelectSize;								// 選択サイズ
D3DXVECTOR3						g_SelectSizeChange;							// 選択サイズ変化
float							g_Selectpos;								// 選択位置
float							g_SelectLenght;								// 選択
float							g_SelectAngleUp;							// 選択
float							g_SelectAngleDown;							// 選択
bool							g_SelectEnter;								// 選択判定
bool							g_bHelp;									// ヘルプ表示状況
bool							g_bTriggerStick;

//=========================================================================================================================
// ポーズ初期化処理
//=========================================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスを取得

	g_posPause = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ポーズの中心値
	g_SelectNum = 0;							// 選択番号
	g_SelectColor = 1.0f;						// 選択カラー
	g_SelectColorChange = -0.008f;				// 選択カラー変化
	g_SelectSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 選択サイズ
	g_SelectSizeChange = D3DXVECTOR3(SCREEN_WIDTH / 800.0f, SCREEN_HEIGHT / 800.0f, 0.0f);	// 選択サイズ
	g_Selectpos = 0.0f;							// 選択位置
	g_SelectLenght = 0.0f;						// 選択
	g_SelectAngleUp = 0.0f;						// 選択
	g_SelectAngleDown = 0.0f;					// 選択
	g_SelectEnter = false;						// 選択判定
	int nCntPause;								// ポーズカウンタ
	float fPosPause = -130;						// ポーズ位置
	g_bHelp = false;
	g_bTriggerStick = false;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_1, &g_pTexturePause[0]);		// BG
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_2, &g_pTexturePause[1]);		// ポーズ
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_3, &g_pTexturePause[2]);		// コンテニュー
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_4, &g_pTexturePause[3]);		// ステージリトライ
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_5, &g_pTexturePause[4]);		// クウィットゲーム
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE_NAME_6, &g_pTexturePause[5]);		// 選択

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_PAUSE_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffPause->Lock
	(
		0,
		0, 
		(void**)&pVtx,
		0
	);
	
	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEXTURE; nCntPause++)
	{// ポーズをカウント
		// 頂点設定
		if (nCntPause == 2)
		{
			pVtx[0].pos = D3DXVECTOR3(450 + PAUSE_POS_X1, 330 + PAUSE_POS_Y1, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(450 + PAUSE_WIDTH1, 330 + PAUSE_POS_Y1, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(450 + PAUSE_POS_X1, 330 + PAUSE_HEIGHT1, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(450 + PAUSE_WIDTH1, 330 + PAUSE_HEIGHT1, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
		}

		if (nCntPause == 0 || nCntPause == 1)
		{// BG
			if (nCntPause == 0)
			{
				// 頂点カラー設定
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.3f);
			}
			else
			{
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		else
		{// ポーズメニュー		
			if (nCntPause == 2 || nCntPause == 3)
			{
				// 頂点カラー設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				// 頂点カラー設定
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}
		}
		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(PAUSE_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, PAUSE_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(PAUSE_TEXTURE_UV_U, PAUSE_TEXTURE_UV_V);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();

	g_SelectAngleUp = atan2f(
		450 - (450 + 50),
		330 - (330 + 50));
	g_SelectLenght = sqrtf(
		(50 * 50) +
		(50 * 50));
	g_SelectAngleDown = g_SelectAngleUp;
}

//=========================================================================================================================
// ポーズ終了処理
//=========================================================================================================================
void UninitPause(void)
{
	// 変数宣言
	int nCntTex;		// テクスチャカウンタ

	// テクスチャの破棄
	for (nCntTex = 0; nCntTex < MAX_PAUSE_TEXTURE; nCntTex++)
	{// テクスチャをカウント
		if (g_pTexturePause[nCntTex] != NULL)
		{
			g_pTexturePause[nCntTex]->Release();
			g_pTexturePause[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=========================================================================================================================
// ポーズ更新処理
//=========================================================================================================================
void UpdatePause(void)
{
	DIJOYSTATE pStick = GetStick();

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffPause->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if ((GetKeyboardTrigger(DIK_H) == true || GetButtonTrigger(11) == true))
	{
		if (g_bHelp == true)
		{
			DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_P);
			DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_OUT);
			SetWindow(D3DXVECTOR3(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_GAME_TUTORIAL_IN, SETTEXTURE_TOPFRONT);
		}
		else if (g_bHelp == false)
		{
			DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_IN);
			SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_GAME_TUTORIAL_P, SETTEXTURE_TOPFRONT);
			SetWindow(D3DXVECTOR3(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_GAME_TUTORIAL_OUT, SETTEXTURE_TOPFRONT);
		}

		g_bHelp = g_bHelp ? false : true;
	}

	if (g_SelectEnter == false && g_bHelp == false)
	{
		if ((GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || ((pStick.rgdwPOV[0] == 0 || pStick.lY < -100) && g_bTriggerStick == false)))
		{
			// 頂点設定
			pVtx[g_SelectNum * 4 + 12].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[g_SelectNum * 4 + 1 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[g_SelectNum * 4 + 2 + 12].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[g_SelectNum * 4 + 3 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
			// 頂点カラー設定
			pVtx[g_SelectNum * 4 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 1 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 2 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 3 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			PlaySound(SOUND_LABEL_SE_PAUSE_1);

			g_SelectNum = (g_SelectNum + 2) % 3;	// 選択を進める
			g_SelectColor = 1.0f;					// カラー変更
			g_SelectSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_bTriggerStick = true;
		}
		if ((GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || ((pStick.rgdwPOV[0] == 18000 || pStick.lY > 100) && g_bTriggerStick == false)))
		{
			// 頂点設定
			pVtx[g_SelectNum * 4 + 12].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[g_SelectNum * 4 + 1 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[g_SelectNum * 4 + 2 + 12].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[g_SelectNum * 4 + 3 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
			// 頂点カラー設定
			pVtx[g_SelectNum * 4 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 1 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 2 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[g_SelectNum * 4 + 3 + 12].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

			PlaySound(SOUND_LABEL_SE_PAUSE_1);

			g_SelectNum = (g_SelectNum + 1) % 3;	// 選択を進める
			g_SelectColor = 1.0f;					// カラー変更
			g_SelectSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			g_bTriggerStick = true;
		}

		g_SelectSize -= g_SelectSizeChange;

		g_SelectColor += g_SelectColorChange;		// 色変化

		if (g_SelectColor > 1.0f || g_SelectColor < 0.5f)
		{// 色変化繰り返し
			g_SelectColorChange *= -1;
			g_SelectSizeChange.x *= -1;
			g_SelectSizeChange.y *= -1;
		}

		// 頂点設定
		pVtx[g_SelectNum * 4 + 12].pos = D3DXVECTOR3(PAUSE_POS_X - 30 - g_SelectSize.x, PAUSE_POS_Y - 30 - g_SelectSize.y, 0.0f);
		pVtx[g_SelectNum * 4 + 1 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH + 30 + g_SelectSize.x, PAUSE_POS_Y - 30 - g_SelectSize.y, 0.0f);
		pVtx[g_SelectNum * 4 + 2 + 12].pos = D3DXVECTOR3(PAUSE_POS_X - 30 - g_SelectSize.x, PAUSE_HEIGHT + 30 + g_SelectSize.y, 0.0f);
		pVtx[g_SelectNum * 4 + 3 + 12].pos = D3DXVECTOR3(PAUSE_WIDTH + 30 + g_SelectSize.x, PAUSE_HEIGHT + 30 + g_SelectSize.y, 0.0f);
		// 頂点カラー設定
		pVtx[g_SelectNum * 4 + 12].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
		pVtx[g_SelectNum * 4 + 1 + 12].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
		pVtx[g_SelectNum * 4 + 2 + 12].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
		pVtx[g_SelectNum * 4 + 3 + 12].col = D3DXCOLOR(g_SelectColor, g_SelectColor, g_SelectColor, 1.0f);
	}

	g_Selectpos = (g_SelectNum) * 120.0f;

	g_SelectAngleUp += 0.01f;
	g_SelectAngleDown -= 0.01f;

	// 頂点設定
	/*pVtx[(2) * 4].pos = D3DXVECTOR3(400 + PAUSE_POS_X1, 280 + PAUSE_POS_Y1 + g_Selectpos, 0.0f);
	pVtx[(2) * 4 + 1].pos = D3DXVECTOR3(400 + PAUSE_WIDTH1, 280 + PAUSE_POS_Y1 + g_Selectpos, 0.0f);
	pVtx[(2) * 4 + 2].pos = D3DXVECTOR3(400 + PAUSE_POS_X1, 280 + PAUSE_HEIGHT1 + g_Selectpos, 0.0f);
	pVtx[(2) * 4 + 3].pos = D3DXVECTOR3(400 + PAUSE_WIDTH1, 280 + PAUSE_HEIGHT1 + g_Selectpos, 0.0f);*/

	pVtx[2 * 4].pos.x = 450 - sinf(g_SelectAngleUp) * g_SelectLenght;
	pVtx[2 * 4].pos.y = 330 + g_Selectpos - cosf(g_SelectAngleUp) * g_SelectLenght;
	pVtx[2 * 4 + 1].pos.x = 450 + sinf(g_SelectAngleDown) * g_SelectLenght;
	pVtx[2 * 4 + 1].pos.y = 330 + g_Selectpos - cosf(g_SelectAngleDown) * g_SelectLenght;
	pVtx[2 * 4 + 2].pos.x = 450 - sinf(g_SelectAngleDown) * g_SelectLenght;
	pVtx[2 * 4 + 2].pos.y = 330 + g_Selectpos + cosf(g_SelectAngleDown) * g_SelectLenght;
	pVtx[2 * 4 + 3].pos.x = 450 + sinf(g_SelectAngleUp) * g_SelectLenght;
	pVtx[2 * 4 + 3].pos.y = 330 + g_Selectpos + cosf(g_SelectAngleUp) * g_SelectLenght;

	// 頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
		GetButtonTrigger(2) == true) && g_bHelp == false)
	{// 任意のキー（ENTER）を押した場合
		FADE fade = GetFade();
		GAMESTATE game = GetGameState();

		if (fade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_ENTER);

			if (g_SelectNum == 0)
			{
				SetPauseState(false);				// 使用していない状態へ
				DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_IN);
			}
			else if (g_SelectNum == 1)
			{
				g_SelectEnter = true;				// 選択判定
				
				if (game == GAMESTATE_TUTORIAL)
				{
					SetFadeGame(GAMESTATE_TUTORIAL);// ゲームへ（チュートリアル）
				}
				else if (game == GAMESTATE_PLAY)
				{
					SetFadeGame(GAMESTATE_PLAY);	// ゲームへ（ゲームメイン）		
					DeleteScore();
					SetMapReset(0);
				}
			}
			else if (g_SelectNum == 2)
			{
				g_SelectEnter = true;				// 選択判定
				SetFadeMode(MODE_TITLE);			// タイトルへ				
			}
		}
	}
	if ((GetKeyboardTrigger(DIK_X) == true || GetButtonTrigger(1) == true))
	{
		PlaySound(SOUND_LABEL_SE_PAUSE_0);

		DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_P);
		DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_IN);
		DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_OUT);
		SetPauseState(false);						// 使用していない状態へ
	}

	if ((pStick.rgdwPOV[0] != 0 && pStick.rgdwPOV[0] != 18000) && (pStick.lY > -100 && pStick.lY < 100))
	{
		g_bTriggerStick = false;
	}
}

//=========================================================================================================================
// ポーズ描画処理
//=========================================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスポインタ
	int nCntPause;					// ポーズカウンタ
	int nCntTex = 0;				// テクスチャカウンタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffPause,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEXTURE; nCntPause++)
	{// ポーズをカウント
		// テクスチャ設定
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		// ポーズの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//=========================================================================================================================
// ポーズ設定処理
//=========================================================================================================================
void SetPauseState(void)
{
	g_SelectNum = 0;							// 選択番号
	g_SelectColor = 1.0f;						// 選択カラー
	g_SelectColorChange = -0.008f;				// 選択カラー変化
	g_SelectSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SelectEnter = false;						// 選択状態
	g_bHelp = false;
	g_bTriggerStick = false;

	int nCntPause;

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffPause->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	pVtx += 12;

	for (nCntPause = 0; nCntPause < MAX_PAUSE_TEXTURE - 3; nCntPause++)
	{// ポーズをカウント
		if (nCntPause == 0)
		{
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
			// 頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);
			// 頂点カラー設定
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();
}

//=========================================================================================================================
// ヘルプ情報取得処理
//=========================================================================================================================
bool GetHelpState(void)
{
	return g_bHelp;
}

//=========================================================================================================================
// スティック状態設定処理
//=========================================================================================================================
void SetStickState(bool bTriggerStick)
{
	g_bTriggerStick = bTriggerStick;
}