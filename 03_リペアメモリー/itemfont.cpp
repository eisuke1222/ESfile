//=============================================================================
//
// アイテムフォント画面 [itemfont.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "itemfont.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_LINE	(2)

//=============================================================================
// グローバル変数
//=============================================================================
LPD3DXFONT			g_pItemFont[MAX_LINE] = {};			// フォントへのポインタ
ItemFont			g_ItemFont[MAX_LINE];				//フォントの情報
bool				g_bUseItemFont;						//フォントを使用しているかどうか

//=============================================================================
// 初期化処理
//=============================================================================
void InitItemFont(void)
{
	//変数宣言
	int nCntItemFont;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数の初期化
	for (nCntItemFont = 0; nCntItemFont < MAX_LINE; nCntItemFont++)
	{
		g_ItemFont[nCntItemFont].font[0] = {};
		g_ItemFont[nCntItemFont].type = TYPEITEMFONT_MAX;
		g_ItemFont[nCntItemFont].bUse = false;

		//フォントの設定
		D3DXCreateFont(pDevice, 50, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
					   OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pItemFont[nCntItemFont]);
	}

	//グローバル変数の初期化
	g_bUseItemFont = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItemFont(void)
{
	//変数宣言
	int nCntItemFont;

	// フォントの開放
	for (nCntItemFont = 0; nCntItemFont < MAX_LINE; nCntItemFont++)
	{
		if (g_pItemFont[nCntItemFont] != NULL)
		{
			g_pItemFont[nCntItemFont] ->Release();
			g_pItemFont[nCntItemFont] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItemFont(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItemFont(void)
{
	//変数宣言
	int nCntItemFont;
	RECT rect = { 400, 610, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };		//X座標, Y座標, Ｘ軸の描画範囲, Y軸の描画範囲

	// テキスト描画
	for (nCntItemFont = 0; nCntItemFont < MAX_LINE; nCntItemFont++)
	{
		if (g_ItemFont[nCntItemFont].bUse == true)
		{
			g_pItemFont[0]->DrawText(NULL, &g_ItemFont[nCntItemFont].font[0], -1, &rect, DT_LEFT, D3DXCOLOR(0.2f, 0.1f, 0.015f, 1.0f));
		}
	}
}

//=============================================================================
// 情報設定
//=============================================================================
void SetItemFont(char font[256], TYPEITEMFONT type)
{
	//変数宣言
	int nCntItemFont;

	for (nCntItemFont = 0; nCntItemFont < MAX_LINE; nCntItemFont++)
	{
		if (g_ItemFont[nCntItemFont].bUse == false)
		{
			//文字列の代入
			wsprintf(&g_ItemFont[nCntItemFont].font[0], "%s", &font[0]);
			g_ItemFont[nCntItemFont].type = type;
			g_ItemFont[nCntItemFont].bUse = true;
			g_bUseItemFont = true;					//フォントを使用している状態にする
			break;
		}
	}
}

//=============================================================================
// フォントを消す
//=============================================================================
void DeleteItemFont(void)
{
	//変数宣言
	int nCntItemFont;

	for (nCntItemFont = 0; nCntItemFont < MAX_LINE; nCntItemFont++)
	{
		g_ItemFont[nCntItemFont].font[0] = {};
		g_ItemFont[nCntItemFont].type = TYPEITEMFONT_MAX;
		g_ItemFont[nCntItemFont].bUse = false;
	}
	
	g_bUseItemFont = false;						//フォントを使用していない状態にする
}

//=============================================================================
// 情報を取得する
//=============================================================================
bool GetUseItemFont(void)
{
	return g_bUseItemFont;
}

