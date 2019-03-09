//=============================================================================
//
// タイトルアニメーション画面 [titleanim.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "titleanim.h"
#include "main.h"
#include "input.h"
//#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TITLEANIM_TEXTURENAME   "data\\TEXTURE\\titleanim.png"		//読み込むテクスチャファイル
#define TITLEANIM_POS_X			(75)								//背景の左上X座標
#define TITLEANIM_POS_Y			(0)									//背景の左上Y座標
#define TITLEANIM_WHIDTH		(SCREEN_WIDTH - 125)				//背景の幅
#define TITLEANIM_HEIGHT		(SCREEN_HEIGHT - 100)				//背景の高さ
#define TITLEANIM_ANIM_U		(0.1f)								//U
#define TITLEANIM_ANIM_V		(0.08333f)							//V

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureTitleAnim = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleAnim = NULL;		//頂点バッファへのポインタ
TitleAnim				g_TitleAnim;

//=============================================================================
// タイトルアニメーション初期化処理
//=============================================================================
void InitTitleAnim(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得
	
	g_TitleAnim.nCntAnim = 0;
	g_TitleAnim.nCntAnimNum = 0;
	g_TitleAnim.bUse = false;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TITLEANIM_TEXTURENAME,
		&g_pTextureTitleAnim);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleAnim,
		NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTitleAnim->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(TITLEANIM_POS_X, TITLEANIM_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLEANIM_POS_X + TITLEANIM_WHIDTH, TITLEANIM_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLEANIM_POS_X, TITLEANIM_POS_Y + TITLEANIM_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLEANIM_POS_X + TITLEANIM_WHIDTH, TITLEANIM_POS_Y + TITLEANIM_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(TITLEANIM_ANIM_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, TITLEANIM_ANIM_V);
	pVtx[3].tex = D3DXVECTOR2(TITLEANIM_ANIM_U, TITLEANIM_ANIM_V);

	//頂点バッファをアンロックする
	g_pVtxBuffTitleAnim->Unlock();
}

//=============================================================================
// タイトルアニメーション終了処理
//=============================================================================
void UninitTitleAnim(void)
{
	//テクスチャの破棄
	if (g_pTextureTitleAnim != NULL)
	{
		g_pTextureTitleAnim->Release();
		g_pTextureTitleAnim = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitleAnim != NULL)
	{
		g_pVtxBuffTitleAnim->Release();
		g_pVtxBuffTitleAnim = NULL;
	}
}

//=============================================================================
// タイトルアニメーション更新処理
//=============================================================================
void UpdateTitleAnim(void)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTitleAnim->Lock(0, 0, (void**)&pVtx, 0);

	if (g_TitleAnim.bUse == true)
	{
		g_TitleAnim.nCntAnim++;

		if (g_TitleAnim.nCntAnim % 1 == 0)
		{
			if (g_TitleAnim.nCntAnimNum < 118)
			{
				g_TitleAnim.nCntAnimNum++;
			}
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((g_TitleAnim.nCntAnimNum % 10) * TITLEANIM_ANIM_U, (g_TitleAnim.nCntAnimNum / 10) * TITLEANIM_ANIM_V);
		pVtx[1].tex = D3DXVECTOR2((g_TitleAnim.nCntAnimNum % 10) * TITLEANIM_ANIM_U + TITLEANIM_ANIM_U, (g_TitleAnim.nCntAnimNum / 10) * TITLEANIM_ANIM_V);
		pVtx[2].tex = D3DXVECTOR2((g_TitleAnim.nCntAnimNum % 10) * TITLEANIM_ANIM_U, (g_TitleAnim.nCntAnimNum / 10) * TITLEANIM_ANIM_V + TITLEANIM_ANIM_V);
		pVtx[3].tex = D3DXVECTOR2((g_TitleAnim.nCntAnimNum % 10) * TITLEANIM_ANIM_U + TITLEANIM_ANIM_U, (g_TitleAnim.nCntAnimNum / 10) * TITLEANIM_ANIM_V + TITLEANIM_ANIM_V);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitleAnim->Unlock();
}

//=============================================================================
// タイトルアニメーション描画処理
//=============================================================================
void DrawTitleAnim(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTitleAnim, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_TitleAnim.bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitleAnim);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,						//開始する頂点のインデックス
			2);						//プリミティブの数
	}
}

//=============================================================================
// タイトルアニメーション設定処理
//=============================================================================
void SetTitleAnim(void)
{
	g_TitleAnim.nCntAnim = 0;
	g_TitleAnim.nCntAnimNum = 0;
	g_TitleAnim.bUse = true;
}

//=============================================================================
// タイトルアニメーション取得処理
//=============================================================================
TitleAnim GetTitleAnim(void)
{
	return g_TitleAnim;
}

//=============================================================================
// タイトルアニメーション番号処理
//=============================================================================
void SetTitleAnimNum(void)
{
	g_TitleAnim.nCntAnimNum = 118;
}

//=============================================================================
// タイトルアニメーション破棄処理
//=============================================================================
void DeleteTitleAnim(void)
{
	g_TitleAnim.nCntAnim = 0;
	g_TitleAnim.nCntAnimNum = 0;
	g_TitleAnim.bUse = false;
}