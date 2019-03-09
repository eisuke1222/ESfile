//=============================================================================
//
// クエストフェード処理 [questfade.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "questfade.h"
#include "BG.h"
#include "xenemy.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_POLYGON			(2)					//描画するプリミティブの数
#define QUESTFADE_POS_X		(0)					//背景の左上X座標
#define QUESTFADE_POS_Y		(0)					//背景の左上Y座標
#define QUESTFADE_WHIDTH	(SCREEN_WIDTH)		//背景の幅
#define QUESTFADE_HEIGHT	(SCREEN_HEIGHT)		//背景の高さ
#define QUESTFADE_SPEED		(0.03f)				//フェードのスピード

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexQuestFade(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureQuestFade = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffQuestFade = NULL;	// 頂点バッファへのポインタ
QUESTFADE				g_fade;						// フェード状態
QUESTSTATE				g_QuestModeNext;			// 次のモード
D3DXCOLOR				g_colorQuestFade;			// フェード色

//=============================================================================
// 初期化処理
//=============================================================================
void InitQuestFade(QUESTSTATE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 値の初期化
	g_fade = QUESTFADE_IN;									//フェードイン状態にする
	g_QuestModeNext = modeNext;								
	g_colorQuestFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//不透明状態にする

	// 頂点情報の設定
	MakeVertexQuestFade(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitQuestFade(void)
{
	//テクスチャの破棄
	if (g_pTextureQuestFade != NULL)
	{
		g_pTextureQuestFade->Release();
		g_pTextureQuestFade = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffQuestFade != NULL)
	{
		g_pVtxBuffQuestFade->Release();
		g_pVtxBuffQuestFade = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateQuestFade(void)
{
	if (g_fade != QUESTFADE_NONE)
	{
		if (g_fade == QUESTFADE_IN)				//フェードインの状態処理
		{
			g_colorQuestFade.a -= QUESTFADE_SPEED;	//画面を透明にしていく

			if (g_colorQuestFade <= 0.0f)
			{
				g_colorQuestFade.a = 0.0f;		//0.0fに再設定する
				g_fade = QUESTFADE_NONE;			//何もしていない状態にする
			}
		}
		else if (g_fade == QUESTFADE_OUT)		//フェードアウトの状態処理
		{
			g_colorQuestFade.a += QUESTFADE_SPEED;	//画面を不透明にしていく
			
			if (g_colorQuestFade.a >= 1.0f)	
			{
				DeleteXEnemy();					//住人を消す
				DeleteBG();						//背景を消す
				g_colorQuestFade.a = 1.0f;		//1.0fに設定する
				g_fade = QUESTFADE_IN;			//フェードイン状態にする

				//モード設定
				SetQuestState(g_QuestModeNext);
			}
		}

		// 頂点バッファの情報を設定
		VERTEX_2D*pVtx;		//頂点情報へのポインタ

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffQuestFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorQuestFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorQuestFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorQuestFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorQuestFade.a);

		//頂点バッファをアンロックする
		g_pVtxBuffQuestFade->Unlock();
	}
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawQuestFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffQuestFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureQuestFade);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexQuestFade(LPDIRECT3DDEVICE9 pDevice)
{
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffQuestFade,
		NULL);

	// 頂点バッファの情報を設定
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffQuestFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(QUESTFADE_POS_X, QUESTFADE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(QUESTFADE_POS_X + QUESTFADE_WHIDTH, QUESTFADE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(QUESTFADE_POS_X, QUESTFADE_POS_Y + QUESTFADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(QUESTFADE_POS_X + QUESTFADE_WHIDTH, QUESTFADE_POS_Y + QUESTFADE_HEIGHT, 0.0f);

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
	g_pVtxBuffQuestFade->Unlock();
}

//=============================================================================
// フェードの設定
//=============================================================================
void SetQuestFade(QUESTSTATE modeNext)
{
	if (g_fade == QUESTFADE_NONE)
	{
		g_fade = QUESTFADE_OUT;									//フェードアウト状態にする
		g_QuestModeNext = modeNext;
		g_colorQuestFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//透明状態にする
	}
}

//=============================================================================
// フェードの取得
//=============================================================================
QUESTFADE GetQuestFade(void)
{
	return g_fade;
}

