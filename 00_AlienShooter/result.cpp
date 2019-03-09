//=========================================================================================================================
// 
// リザルト処理 [resule.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "result.h"		// リザルト
#include "game.h"		// ゲーム		
#include "sound.h"		// サウンド
#include "fade.h"		// フェード
#include "input.h"		// 入力処理

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define RESULT_TEXTURE_NAME_1	"data\\TEXTURE\\gameclear000.png"	// 読み込むテクスチャファイル
#define RESULT_TEXTURE_NAME_2	"data\\TEXTURE\\gameover000.png"	// 読み込むテクスチャファイル
#define RESULT_POS_X			(0)									// リザルトの左上X座標
#define RESULT_POS_Y			(0)									// リザルトの左上Y座標
#define RESULT_WIDTH			(SCREEN_WIDTH)						// リザルトの幅
#define RESULT_HEIGHT			(SCREEN_HEIGHT)						// リザルトの高さ
#define RESULT_TEXTURE_UV_U		(1.0f)								// テクスチャアニメーションU範囲
#define RESULT_TEXTURE_UV_V		(1.0f)								// テクスチャアニメーションV範囲

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureResult = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffResult = NULL;				// 頂点バッファへのポインタ
D3DXVECTOR3					g_posResult;							// リザルトの位置
int							g_nCntResultState;						// 状態カウンタ

//=========================================================================================================================
// リザルト初期化処理
//=========================================================================================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_posResult = D3DXVECTOR3((RESULT_POS_X + RESULT_WIDTH) / 2, (RESULT_POS_Y + RESULT_HEIGHT) / 2, 0.0f);		// ポリゴンの中心値
	g_nCntResultState = 0;						//状態カウンタを初期化

	GAMESTATE GameStateNow = GetGameState();	// ゲームの状態を取得

	// テクスチャの読み込み
	if (GameStateNow == GAMESTATE_GAMECLEAR)
	{// ゲームクリアの場合
		D3DXCreateTextureFromFile
		(// リザルト1
			pDevice,
			RESULT_TEXTURE_NAME_1,
			&g_pTextureResult
		);

		PlaySound(SOUND_LABEL_SE_CLEAR);	// ゲームクリア
	}
	else
	{// ゲームオーバー
		D3DXCreateTextureFromFile
		(// リザルト2
			pDevice,
			RESULT_TEXTURE_NAME_2,
			&g_pTextureResult
		);

		PlaySound(SOUND_LABEL_SE_OVER);		// ゲームオーバー
	}

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffResult->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RESULT_WIDTH, RESULT_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RESULT_WIDTH, RESULT_HEIGHT, 0.0f);
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
	pVtx[1].tex = D3DXVECTOR2(RESULT_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, RESULT_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(RESULT_TEXTURE_UV_U, RESULT_TEXTURE_UV_V);
	
	// 頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();
}

//=========================================================================================================================
// リザルト終了処理
//=========================================================================================================================
void UninitResult(void)
{
	// テクスチャーの破棄
	if (g_pTextureResult != NULL)
	{// NULL以外の場合
		g_pTextureResult->Release();	// 開放
		g_pTextureResult = NULL;		// NULLへ
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{// NULL以外の場合
		g_pVtxBuffResult->Release();	// 開放
		g_pVtxBuffResult = NULL;		// NULLへ
	}
}

//=========================================================================================================================
// リザルト更新処理
//=========================================================================================================================
void UpdateResult(void)
{
	g_nCntResultState++;			// 状態をカウント
	
	FADE Fade = GetFade();			// フェードを取得

	if (Fade == FADE_NONE)
	{// フェードを使用していない場合
		if (g_nCntResultState > 180 || GetKeyboardTrigger(DIK_RETURN) == true)
		{// 画面切り替え
			SetFade(MODE_RANKING);		// ランキング
		}
	}
}

//=========================================================================================================================
// リザルト描画処理
//=========================================================================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// デバイスポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffResult,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, g_pTextureResult);

	// リザルトの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}