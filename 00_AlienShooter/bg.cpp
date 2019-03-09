//=========================================================================================================================
// 
// 背景処理 [bg.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "bg.h"			// 背景
#include "input.h"		// 入力処理

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_BG_TEXTURE			(2)									// 背景テクスチャ最大数
#define BG_TEXTURE_NAME_1		"data\\TEXTURE\\utyuu002.png"		// 読み込むテクスチャファイル1
#define BG_TEXTURE_NAME_2		"data\\TEXTURE\\stage.png"			// 読み込むテクスチャファイル2
#define BG_POS_X				(0)									// 背景の左上X座標
#define BG_POS_Y				(0)									// 背景の左上Y座標
#define BG_WIDTH				(SCREEN_WIDTH)						// 背景の幅
#define BG_HEIGHT				(SCREEN_HEIGHT)						// 背景の高さ
#define BG_TEXTURE_SPEED		(5)									// テクスチャアニメーションスピード
#define BG_TEXTURE_PATTERN		(10)								// テクスチャアニメーションパターン数
#define BG_TEXTURE_UV_U			(1.0f)								// テクスチャアニメーションU範囲
#define BG_TEXTURE_UV_V			(1.0f)								// テクスチャアニメーションV範囲
#define BG_BACKTEXTURE_UV_U		(1.0f)								// 前テクスチャアニメーションU範囲
#define BG_BACKTEXTURE_UV_V		(1.0f)								// 前テクスチャアニメーションV範囲
#define BG_SPEED				(0.002f)							// 背景スピード

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef struct
{// ステージ番号状態
	STAGENUM	stage;				// ステージ番号			
	float		Color;				// 色
	int			nCounerStageNum;	// カウント
	bool		bDisp;				// 表示
}STAGENUMSTATE;

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureBG[MAX_BG_TEXTURE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffBG = NULL;				// 頂点バッファへのポインタ
D3DXVECTOR3						g_posBG;							// 背景の位置
STAGENUMSTATE					g_StageNum;							// ステージ番号情報

//=========================================================================================================================
// 背景初期化処理
//=========================================================================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスを取得
	int nCntBG;		// カウント

	g_posBG = D3DXVECTOR3((BG_POS_X + BG_WIDTH) / 2, (BG_POS_Y + BG_HEIGHT) / 2, 0.0f);		// 背景の中心値

	g_StageNum.stage = STAGENUM_1;		// ステージ番号
	g_StageNum.Color = 0.0f;			// 色
	g_StageNum.nCounerStageNum = 0;		// カウント
	g_StageNum.bDisp = false;			// 表示

	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(// 背景１
		pDevice,
		BG_TEXTURE_NAME_1,
		&g_pTextureBG[0]
	);
	D3DXCreateTextureFromFile
	(// ステージ番号
		pDevice,
		BG_TEXTURE_NAME_2,
		&g_pTextureBG[1]
	);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BG_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBG->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBG = 0; nCntBG < MAX_BG_TEXTURE; nCntBG++)
	{// カウント
		if (nCntBG == 0)
		{// 背景
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, BG_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);
			// 頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャ設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, BG_TEXTURE_UV_V);
			pVtx[3].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, BG_TEXTURE_UV_V);
		}
		else
		{// ステージ番号
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3(BG_WIDTH / 2 - 150, BG_HEIGHT / 4 - 50, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(BG_WIDTH / 2 + 150, BG_HEIGHT / 4 - 50, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(BG_WIDTH / 2 - 150, BG_HEIGHT / 4 + 50, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(BG_WIDTH / 2 + 150, BG_HEIGHT / 4 + 50, 0.0f);
			// 頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			// テクスチャ設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f);
			pVtx[3].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, 0.2f);
		}
		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	
		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBG->Unlock();
}

//=========================================================================================================================
// 背景終了処理
//=========================================================================================================================
void UninitBG(void)
{
	// 変数宣言
	int nCntTex;

	// テクスチャの破棄
	for (nCntTex = 0; nCntTex < MAX_BG_TEXTURE; nCntTex++)
	{// テクスチャをカウント
		if (g_pTextureBG[nCntTex] != NULL)
		{// NULL以外の場合
			g_pTextureBG[nCntTex]->Release();	// 開放
			g_pTextureBG[nCntTex] = NULL;		// NULLへ
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffBG != NULL)
	{// NULL以外の場合
		g_pVtxBuffBG->Release();	// 開放
		g_pVtxBuffBG = NULL;		// NULLへ
	}
}

//=========================================================================================================================
// 背景更新処理
//=========================================================================================================================
void UpdateBG(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBG->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// BG
	g_posBG.y -= 0.001f;	// 移動値代入

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, g_posBG.y - (BG_TEXTURE_UV_V / 2));
	pVtx[1].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, g_posBG.y - (BG_TEXTURE_UV_V / 2));
	pVtx[2].tex = D3DXVECTOR2(0.0f, g_posBG.y + (BG_TEXTURE_UV_V / 2));
	pVtx[3].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, g_posBG.y + (BG_TEXTURE_UV_V / 2));

	// ステージ
	if (g_StageNum.bDisp == true)
	{// 表示するだったら
		if (g_StageNum.Color < 1.0f && g_StageNum.nCounerStageNum == 0)
		{// 色が指定した値以下、カウントが０
			g_StageNum.Color += 0.008f;		// 増やす
		}
		else
		{// その他
			g_StageNum.Color -= 0.008f;		// 減らす
			g_StageNum.nCounerStageNum = 1;	// カウント
		}

		if (g_StageNum.Color < 0.0f)
		{// 指定した値以下だったら
			g_StageNum.bDisp = false;		// 非表示
		}

		// 頂点カラー設定
		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StageNum.Color);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StageNum.Color);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StageNum.Color);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StageNum.Color);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffBG->Unlock();
}

//=========================================================================================================================
// 背景描画処理
//=========================================================================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntTex;

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBG,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTex = 0; nCntTex < MAX_BG_TEXTURE; nCntTex++)
	{// テクスチャをカウント
		if (nCntTex == 0)
		{// 背景
			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureBG[nCntTex]);

			// 背景の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
		}
		else if (nCntTex == 1 && g_StageNum.bDisp == true)
		{// ステージ番号
			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureBG[nCntTex]);

			// 背景の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
		}
	}
}

//=========================================================================================================================
// ステージ番号設定処理
//=========================================================================================================================
void SetNumStage(STAGENUM stage)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffBG->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// 設定
	g_StageNum.stage = stage;			// ステージ番号
	g_StageNum.Color = 0.0f;			// 色
	g_StageNum.nCounerStageNum = 0;		// カウント
	g_StageNum.bDisp = true;			// 表示する

	// 頂点カラー設定
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	// テクスチャ設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, (g_StageNum.stage * 0.2f));
	pVtx[5].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, (g_StageNum.stage * 0.2f));
	pVtx[6].tex = D3DXVECTOR2(0.0f, (g_StageNum.stage * 0.2f) + 0.2f);
	pVtx[7].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, (g_StageNum.stage * 0.2f) + 0.2f);

	// 頂点バッファをアンロック
	g_pVtxBuffBG->Unlock();
}