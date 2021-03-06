//=========================================================================================================================
// 
// フェード処理 [fade.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "fade.h"		// フェード
#include "input.h"		// 入力処理

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_FADE_TEXTURE			(2)								// フェードテクスチャ最大数
#define FADE_TEXTURE_NAME_1			"data\\TEXTURE\\utyuu002.png"	// 読み込むテクスチャファイル
#define FADE_POS_X					(0)								// フェードの左上X座標
#define FADE_POS_Y					(0)								// フェードの左上Y座標
#define FADE_WIDTH					(SCREEN_WIDTH)					// フェードの幅
#define FADE_HEIGHT					(SCREEN_HEIGHT)					// フェードの高さ
#define FADE_TEXTURE_SPEED			(5)								// テクスチャアニメーションスピード
#define FADE_TEXTURE_PATTERN		(10)							// テクスチャアニメーションパターン数
#define FADE_TEXTURE_UV_U			(1.0f)							// テクスチャアニメーションU範囲
#define FADE_TEXTURE_UV_V			(1.0f)							// テクスチャアニメーションV範囲
#define FADE_BACKTEXTURE_UV_U		(1.0f)							// 前テクスチャアニメーションU範囲
#define FADE_BACKTEXTURE_UV_V		(1.0f)							// 前テクスチャアニメーションV範囲
#define FADE_SPEED					(0.002f)						// フェードスピード

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9					g_pTextureFade = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9				g_pVtxBuffFade = NULL;			// 頂点バッファへのポインタ
FADE								g_fade;							// フェード状態
MODE								g_modeNext;						// 次のモード
D3DXCOLOR							g_colorFade;					// フェード色

//=========================================================================================================================
// フェード初期化処理
//=========================================================================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_fade = FADE_IN;			// フェード状態
	g_modeNext = modeNext;		// 画面状態
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// フェード色

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffFade->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_WIDTH, FADE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(FADE_POS_X, FADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_WIDTH, FADE_HEIGHT, 0.0f);
	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラー設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;
	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(FADE_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, FADE_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(FADE_TEXTURE_UV_U, FADE_TEXTURE_UV_V);

	// 頂点バッファをアンロック
	g_pVtxBuffFade->Unlock();
}

//=========================================================================================================================
// フェード終了処理
//=========================================================================================================================
void UninitFade(void)
{
	// テクスチャの破棄
	if (g_pTextureFade != NULL)
	{// NULL以外の場合
		g_pTextureFade->Release();		// 開放
		g_pTextureFade = NULL;			// NULLへ
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{// NULL以外の場合
		g_pVtxBuffFade->Release();		// 開放
		g_pVtxBuffFade = NULL;			// NULLへ
	}
}

//=========================================================================================================================
// フェード更新処理
//=========================================================================================================================
void UpdateFade(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffFade->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0

	);
	
	if (g_fade != FADE_NONE)
	{// フェードが開始
		if (g_fade == FADE_IN)
		{// フェードイン
			g_colorFade.a -= 0.03f;

			if (g_colorFade.a <= 0.0f)
			{// 透明
				g_colorFade.a = 0.0f;	// 透明度を調整
				g_fade = FADE_NONE;		// 何もしていない状態
			}
		}
		else if (g_fade == FADE_OUT)
		{// フェードアウト
			g_colorFade.a += 0.03f;

			if (g_colorFade.a >= 1.0f)
			{// 不透明
				g_colorFade.a = 1.0f;	// 透明度を調整
				g_fade = FADE_IN;		// フェードイン

				// モードの設定
				SetMode(g_modeNext);
			}
		}
	}

	// 頂点カラー設定
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;

	// 頂点バッファをアンロック
	g_pVtxBuffFade->Unlock();
}

//=========================================================================================================================
// フェード描画処理
//=========================================================================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffFade,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, g_pTextureFade);

	// フェードの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=========================================================================================================================
// フェードの設定
//=========================================================================================================================
void SetFade(MODE modeNext)
{
	g_fade		= FADE_OUT;		// フェードアウト
	g_modeNext	= modeNext;		// 次の画面
}

//=========================================================================================================================
// フェードの取得
//=========================================================================================================================
FADE GetFade(void)
{
	return g_fade;		// 値を返す
}