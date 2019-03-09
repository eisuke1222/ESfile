//=============================================================================
//
// ゲームフェード処理 [gamefade.cpp]
// Author : 佐藤 安純	Sato_Asumi
//
//=============================================================================
#include "gamefade.h"
#include "BG.h"
#include "start.h"
#include "game.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_POLYGON			(2)					//描画するプリミティブの数
#define GAMEFADE_POS_X		(0)					//背景の左上X座標
#define GAMEFADE_POS_Y		(0)					//背景の左上Y座標
#define GAMEFADE_WHIDTH		(SCREEN_WIDTH)		//背景の幅
#define GAMEFADE_HEIGHT		(SCREEN_HEIGHT)		//背景の高さ
#define GAMEFADE_SPEED		(0.03f)				//フェードのスピード

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexGameFade(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureGameFade = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameFade = NULL;	// 頂点バッファへのポインタ
GAMEFADE				g_fade;						// フェード状態
GAMESTATE				g_modeNext;					// 次のモード
D3DXCOLOR				g_colorGameFade;			// フェード色

//=============================================================================
// 初期化処理
//=============================================================================
void InitGameFade(GAMESTATE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 値の初期化
	g_fade = GAMEFADE_IN;									//フェードイン状態にする
	g_modeNext = modeNext;								
	g_colorGameFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//不透明状態にする

	// 頂点情報の設定
	MakeVertexGameFade(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGameFade(void)
{
	//テクスチャの破棄
	if (g_pTextureGameFade != NULL)
	{
		g_pTextureGameFade->Release();
		g_pTextureGameFade = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGameFade != NULL)
	{
		g_pVtxBuffGameFade->Release();
		g_pVtxBuffGameFade = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameFade(void)
{
	bool pPause = GetGAMEPause();
	QUESTSTATE pQuestState = GetQuestState();

	if (g_fade != GAMEFADE_NONE)
	{
		if (g_fade == GAMEFADE_IN)				//フェードインの状態処理
		{
			g_colorGameFade.a -= GAMEFADE_SPEED;	//画面を透明にしていく

			if (g_colorGameFade <= 0.0f)
			{
				g_colorGameFade.a = 0.0f;		//0.0fに再設定する
				g_fade = GAMEFADE_NONE;			//何もしていない状態にする
			}
		}
		else if (g_fade == GAMEFADE_OUT)		//フェードアウトの状態処理
		{
			g_colorGameFade.a += GAMEFADE_SPEED;	//画面を不透明にしていく
			
			if (g_colorGameFade.a >= 1.0f)	
			{
				DeleteBG();						//背景を消す
				DeleteStart();					//スタートボタンを消す
				g_colorGameFade.a = 1.0f;		//1.0fに設定する
				g_fade = GAMEFADE_IN;			//フェードイン状態にする

				//モード設定
				if (pPause == true || pQuestState == QUESTSTATE_CLEAR)
				{
					SetQuestState(QUESTSTATE_NONE);
				}
				else
				{
					SetQuestState(QUESTSTATE_MISSTION);
				}
				SetGameState(g_modeNext);
			}
		}

		// 頂点バッファの情報を設定
		VERTEX_2D*pVtx;		//頂点情報へのポインタ

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffGameFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorGameFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorGameFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorGameFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorGameFade.a);

		//頂点バッファをアンロックする
		g_pVtxBuffGameFade->Unlock();
	}
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawGameFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffGameFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGameFade);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexGameFade(LPDIRECT3DDEVICE9 pDevice)
{
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameFade,
		NULL);

	// 頂点バッファの情報を設定
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGameFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(GAMEFADE_POS_X, GAMEFADE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GAMEFADE_POS_X + GAMEFADE_WHIDTH, GAMEFADE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(GAMEFADE_POS_X, GAMEFADE_POS_Y + GAMEFADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GAMEFADE_POS_X + GAMEFADE_WHIDTH, GAMEFADE_POS_Y + GAMEFADE_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffGameFade->Unlock();
}

//=============================================================================
// フェードの設定
//=============================================================================
void SetGameFade(GAMESTATE modeNext)
{
	if (g_fade == GAMEFADE_NONE)
	{
		g_fade = GAMEFADE_OUT;									//フェードアウト状態にする
		g_modeNext = modeNext;
		g_colorGameFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//透明状態にする
	}
}

//=============================================================================
// フェードの取得
//=============================================================================
GAMEFADE GetGameFade(void)
{
	return g_fade;
}

