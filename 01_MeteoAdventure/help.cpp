//=========================================================================================================================
// 
// ヘルプ処理 [help.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "help.h"		// ヘルプ
#include "input.h"		// 入力処理
#include "sound.h"		// サウンド

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_HELP_TEXTURE		(1)								// ヘルプテクスチャ最大数
#define HELP_TEXTURE_NAME_1		"data\\TEXTURE\\help102.png"	// 読み込むテクスチャファイル1
#define HELP_POS_X				(0)								// ヘルプの左上X座標
#define HELP_POS_Y				(0)								// ヘルプの左上Y座標
#define HELP_WIDTH				(SCREEN_WIDTH)					// ヘルプの幅
#define HELP_HEIGHT				(SCREEN_HEIGHT)					// ヘルプの高さ
#define HELP_TEXTURE_SPEED		(5)								// テクスチャアニメーションスピード
#define HELP_TEXTURE_PATTERN	(10)							// テクスチャアニメーションパターン数
#define HELP_TEXTURE_UV_U		(1.0f)							// テクスチャアニメーションU範囲
#define HELP_TEXTURE_UV_V		(1.0f)							// テクスチャアニメーションV範囲
#define HELP_BACKTEXTURE_UV_U	(1.0f)							// 前テクスチャアニメーションU範囲
#define HELP_BACKTEXTURE_UV_V	(1.0f)							// 前テクスチャアニメーションV範囲
#define HELP_SPEED				(0.002f)						// ヘルプスピード

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureHelp = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffHelp = NULL;						// 頂点バッファへのポインタ

//=========================================================================================================================
// ヘルプ初期化処理
//=========================================================================================================================
void InitHelp(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスを取得

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, HELP_TEXTURE_NAME_1, &g_pTextureHelp);		// BG

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHelp,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffHelp->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(HELP_POS_X, HELP_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(HELP_WIDTH, HELP_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(HELP_POS_X, HELP_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(HELP_WIDTH, HELP_HEIGHT, 0.0f);
	// 頂点カラー設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(HELP_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, HELP_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(HELP_TEXTURE_UV_U, HELP_TEXTURE_UV_V);

	// 頂点バッファをアンロック
	g_pVtxBuffHelp->Unlock();
}

//=========================================================================================================================
// ヘルプ終了処理
//=========================================================================================================================
void UninitHelp(void)
{
	// テクスチャの破棄
	if (g_pTextureHelp != NULL)
	{
		g_pTextureHelp->Release();
		g_pTextureHelp = NULL;

	}
	// 頂点バッファの破棄
	if (g_pVtxBuffHelp != NULL)
	{
		g_pVtxBuffHelp->Release();
		g_pVtxBuffHelp = NULL;
	}
}

//=========================================================================================================================
// ヘルプ更新処理
//=========================================================================================================================
void UpdateHelp(void)
{	

}

//=========================================================================================================================
// ヘルプ描画処理
//=========================================================================================================================
void DrawHelp(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffHelp,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, g_pTextureHelp);

	// ヘルプの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}