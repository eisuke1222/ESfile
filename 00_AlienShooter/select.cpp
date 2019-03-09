//=========================================================================================================================
// 
// 選択処理 [bg.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "select.h"		// 選択
#include "input.h"		// 入力処理

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_SELECT_TEXTURE			(2)								// 選択テクスチャ最大数
#define SELECT_TEXTURE_NAME_1		"data\\TEXTURE\\press_enter.png"// 読み込むテクスチャファイル1
#define SELECT_POS_X				(SCREEN_WIDTH - 210)			// 選択の左上X座標
#define SELECT_POS_Y				(SCREEN_HEIGHT - 80)			// 選択の左上Y座標
#define SELECT_WIDTH				(SCREEN_WIDTH - 10)				// 選択の幅
#define SELECT_HEIGHT				(SCREEN_HEIGHT)					// 選択の高さ
#define SELECT_TEXTURE_SPEED		()								// テクスチャアニメーションスピード
#define SELECT_TEXTURE_PATTERN		(10)							// テクスチャアニメーションパターン数
#define SELECT_TEXTURE_UV_U			(1.0f)							// テクスチャアニメーションU範囲
#define SELECT_TEXTURE_UV_V			(1.0f)							// テクスチャアニメーションV範囲
#define SELECT_BACKTEXTURE_UV_U		(1.0f)							// 前テクスチャアニメーションU範囲
#define SELECT_BACKTEXTURE_UV_V		(1.0f)							// 前テクスチャアニメーションV範囲

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9					g_pTextureSelect = {};			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9				g_pVtxBuffSelect = NULL;		// 頂点バッファへのポインタ
SELECTSTATE							g_SelectState;					// 選択の状態
D3DXVECTOR3							g_posSelect;					// 選択の位置
D3DXVECTOR3							g_RadiusSelect;					// 選択の大きさ（半径）
int									g_nCntSelectState;				// 状態カウンタ

//=========================================================================================================================
// 選択初期化処理
//=========================================================================================================================
void InitSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_SelectState = SELECTSTATE_NONE;			// 状態初期化
	g_posSelect = D3DXVECTOR3((SELECT_POS_X + SELECT_WIDTH) / 2, (SELECT_POS_Y + SELECT_HEIGHT) / 2, 0.0f);		// 選択の中心値
	g_RadiusSelect = D3DXVECTOR3(0, 0, 0.0f);	// 選択の大きさ
	g_nCntSelectState = 0;						// 状態カウンタ

	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(// 選択
		pDevice,
		SELECT_TEXTURE_NAME_1,
		&g_pTextureSelect
	);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelect,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffSelect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

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
	pVtx[1].tex = D3DXVECTOR2(SELECT_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, SELECT_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(SELECT_TEXTURE_UV_U, SELECT_TEXTURE_UV_V);

	// 頂点バッファをアンロック
	g_pVtxBuffSelect->Unlock();
}

//=========================================================================================================================
// 選択終了処理
//=========================================================================================================================
void UninitSelect(void)
{
	// テクスチャの破棄
	if (g_pTextureSelect != NULL)
	{// NULL以外の場合
		g_pTextureSelect->Release();	// 開放
		g_pTextureSelect = NULL;		// NULLへ
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffSelect != NULL)
	{// NULL以外の場合
		g_pVtxBuffSelect->Release();	// 開放
		g_pVtxBuffSelect = NULL;		// NULLへ
	}
}

//=========================================================================================================================
// 選択更新処理
//=========================================================================================================================
void UpdateSelect(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ
	D3DXCOLOR nColor;

	// 頂点バッファをロック
	g_pVtxBuffSelect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	switch (g_SelectState)
	{// 選択番号
	case SELECTSTATE_NONE:		// 何もしていない
		g_nCntSelectState++;	// カウント

		if (g_nCntSelectState % 80 == 0)
		{// 一定フレームになったら
			g_nCntSelectState = 0;
		}

		if (g_nCntSelectState < 40)
		{// 指定した値より低い
			nColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{// 指定した値以上
			nColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		break;
	case SELECTSTATE_ENTER:		// 決定した
		g_nCntSelectState++;	// カウント

		if (g_nCntSelectState % 10 == 0)
		{// 一定フレームになったら
			g_nCntSelectState = 0;
		}

		if (g_nCntSelectState < 5)
		{// 指定した値より低い
			nColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{// 指定した値以上
			nColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		break;
	}

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);
	// 頂点カラー設定
	pVtx[0].col = nColor;
	pVtx[1].col = nColor;
	pVtx[2].col = nColor;
	pVtx[3].col = nColor;

	// 頂点バッファをアンロック
	g_pVtxBuffSelect->Unlock();
}

//=========================================================================================================================
// 選択描画処理
//=========================================================================================================================
void DrawSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffSelect,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, g_pTextureSelect);

	// 選択の描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=========================================================================================================================
// 選択設定処理
//=========================================================================================================================
void SetEnterSelect(D3DXVECTOR3 pos, D3DXVECTOR3 Radius)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffSelect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	g_posSelect = pos;
	g_RadiusSelect = Radius;

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffSelect->Unlock();
}

//=========================================================================================================================
// 選択設定処理
//=========================================================================================================================
void SetMenuSelect(D3DXVECTOR3 pos, D3DXVECTOR3 Radius)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffSelect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	g_posSelect = pos;
	g_RadiusSelect = Radius;

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffSelect->Unlock();
}

//=========================================================================================================================
// 選択変化処理
//=========================================================================================================================
void ChangeSelect(SELECTSTATE state)
{
	g_SelectState = state;
}