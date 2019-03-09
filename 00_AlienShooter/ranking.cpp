//=========================================================================================================================
// 
// ランキング処理 [ranking.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "ranking.h"		// ランキング
#include "score.h"			// スコア
#include "input.h"			// 入力処理

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_RANKING_TEXTURE			(2)								// テクスチャの最大数
#define RANKING_TEXTURE_NAME_1		"data\\TEXTURE\\title000.png"	// 読み込むテクスチャファイル
#define RANKING_TEXTURE_NAME_2		"data\\TEXTURE\\ranking006.png"	// 読み込むテクスチャファイル2
#define RANKING_POS_X				(0)								// ランキングの左上X座標
#define RANKING_POS_Y				(0)								// ランキングの左上Y座標
#define RANKING_WIDTH				(SCREEN_WIDTH)					// ランキングの幅
#define RANKING_HEIGHT				(SCREEN_HEIGHT)					// ランキングの高
#define RANKING_TEXTURE_UV_U		(1.0f)							// テクスチャアニメーションU範囲
#define RANKING_TEXTURE_UV_V		(1.0f)							// テクスチャアニメーションV範囲
#define RANKING_TEXTURE_SPEED		(15.0f)							// テクスチャスピード

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureRanking[MAX_RANKING_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffRanking = NULL;							// 頂点バッファへのポインタ
Ranking						g_Ranking;											// ランキングの情報
int							g_nCntRankingState;									// 状態カウンタ

//=========================================================================================================================
// ランキング初期化処理
//=========================================================================================================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntRnking;		// ランキングカウンタ

	g_Ranking.pos = D3DXVECTOR3(RANKING_WIDTH + (RANKING_WIDTH / 2), (RANKING_POS_Y + RANKING_HEIGHT) / 2, 0.0f);		// ポリゴンの中心値
	g_Ranking.move = D3DXVECTOR3(RANKING_TEXTURE_SPEED, 0.0f, 0.0f);		// ポリゴンの中心値
	g_nCntRankingState = 0;		// 状態カウントを初期化

	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(// BG
		pDevice,
		RANKING_TEXTURE_NAME_1,
		&g_pTextureRanking[0]
	);
	D3DXCreateTextureFromFile
	(// ランキング
		pDevice,
		RANKING_TEXTURE_NAME_2,
		&g_pTextureRanking[1]
	);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_RANKING_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffRanking->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntRnking = 0; nCntRnking < MAX_RANKING_TEXTURE; nCntRnking++)
	{//	ランキングをカウント
		if (nCntRnking == 0)
		{// BG
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3(RANKING_POS_X, RANKING_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(RANKING_WIDTH, RANKING_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(RANKING_POS_X, RANKING_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(RANKING_WIDTH, RANKING_HEIGHT, 0.0f);
		}
		else
		{// スコアランキング
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3(RANKING_WIDTH, RANKING_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(RANKING_WIDTH + RANKING_WIDTH, RANKING_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(RANKING_WIDTH, RANKING_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(RANKING_WIDTH + RANKING_WIDTH, RANKING_HEIGHT, 0.0f);
		}
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
		pVtx[1].tex = D3DXVECTOR2(RANKING_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, RANKING_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(RANKING_TEXTURE_UV_U, RANKING_TEXTURE_UV_V);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffRanking->Unlock();
}

//=========================================================================================================================
// ランキング終了処理
//=========================================================================================================================
void UninitRanking(void)
{
	int nCntTex;		// テクスチャカウンタ

	// テクスチャの破棄
	for (nCntTex = 0; nCntTex < MAX_RANKING_TEXTURE; nCntTex++)
	{//	テクスチャをカウント
		if (g_pTextureRanking[nCntTex] != NULL)
		{// NULL以外の場合
			g_pTextureRanking[nCntTex]->Release();		// 開放
			g_pTextureRanking[nCntTex] = NULL;			// NULLへ
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRanking != NULL)
	{// NULL以外の場合
		g_pVtxBuffRanking->Release();	// 開放
		g_pVtxBuffRanking = NULL;		// NULLへ
	}
}

//=========================================================================================================================
// ランキング更新処理
//=========================================================================================================================
void UpdateRanking(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffRanking->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	); 

	// 移動値代入
	g_Ranking.pos -= g_Ranking.move;

	if (g_Ranking.pos.x <= SCREEN_WIDTH / 2)
	{// 移動範囲
		if (g_nCntRankingState >= 180 || (GetKeyboardTrigger(DIK_RETURN) == true && g_nCntRankingState > 30))
		{// 移動開始
			g_Ranking.move = D3DXVECTOR3(RANKING_TEXTURE_SPEED, 0.0f, 0.0f);
		}
		else if (g_nCntRankingState == 0)
		{// 移動停止
			g_Ranking.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		g_nCntRankingState++;		// 状態をカウント
	}

	// 頂点設定
	pVtx[4].pos = D3DXVECTOR3(g_Ranking.pos.x - (RANKING_WIDTH / 2), RANKING_POS_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_Ranking.pos.x + (RANKING_WIDTH / 2), RANKING_POS_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_Ranking.pos.x - (RANKING_WIDTH / 2), RANKING_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_Ranking.pos.x + (RANKING_WIDTH / 2), RANKING_HEIGHT, 0.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffRanking->Unlock();

	if (g_Ranking.pos.x <= -SCREEN_WIDTH)
	{// 画面外になったら画面切り替え
		SetMode(MODE_TITLE);		// タイトル
	}
}

//=========================================================================================================================
// ランキング描画処理
//=========================================================================================================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntRnking;		// ランキングカウンタ

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffRanking,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntRnking = 0; nCntRnking < MAX_RANKING_TEXTURE; nCntRnking++)
	{//	ランキングをカウント
		// テクスチャ設定
		pDevice->SetTexture(0, g_pTextureRanking[nCntRnking]);

		// ランキングの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRnking * 4, 2);
	}
}

//=========================================================================================================================
// ランキング取得処理
//=========================================================================================================================
Ranking GetRanking(void)
{
	return g_Ranking;		// 値を返す
}