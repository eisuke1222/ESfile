//=========================================================================================================================
// 
// タイトル処理 [title.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "title.h"		// タイトル
#include "select.h"		// 選択処理
#include "sound.h"		// サウンド
#include "fade.h"		// フェード
#include "input.h"		// 入力

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_TITLE_TEXTURE		(2)								// テクスチャの最大数
#define TITLE_TEXTURE_NAME_1	"data\\TEXTURE\\title000.png"	// 読み込むテクスチャファイル1
#define TITLE_TEXTURE_NAME_2	"data\\TEXTURE\\Alien-Shooter.png"		// 読み込むテクスチャファイル2
#define TITLE_POS_X				(0)								// タイトルの左上X座標
#define TITLE_POS_Y				(0)								// タイトルの左上Y座標
#define TITLE_WIDTH				(SCREEN_WIDTH)					// タイトルの幅
#define TITLE_HEIGHT			(SCREEN_HEIGHT)					// タイトルの高
#define TITLE_TEXTURE_UV_U		(1.0f)							// テクスチャアニメーションU範囲
#define TITLE_TEXTURE_UV_V		(1.0f)							// テクスチャアニメーションV範囲
#define TITLE_TEXTURE_SPEED		(2)								// テクスチャスピード

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureTitle[MAX_TITLE_TEXTURE] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTitle = NULL;							// 頂点バッファへのポインタ
D3DXVECTOR3					g_posTitle;										// タイトルの位置
D3DXVECTOR3					g_moveTitle;									// タイトルの移動値
int							g_nCntMode;										// モード切替カウンタ

//=========================================================================================================================
// タイトル初期化処理
//=========================================================================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntTitle;			// タイトルカウンタ

	g_posTitle = D3DXVECTOR3((TITLE_POS_X + TITLE_WIDTH) / 2, (TITLE_HEIGHT / 8) * -1, 0.0f);		// ポリゴンの中心値
	g_moveTitle = D3DXVECTOR3(0.0f, TITLE_TEXTURE_SPEED, 0.0f);				// ポリゴンの移動値
	g_nCntMode = 0;				// カウンタ初期化

	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(// タイトルBG
		pDevice,
		TITLE_TEXTURE_NAME_1,
		&g_pTextureTitle[0]
	);
	D3DXCreateTextureFromFile
	(// タイトルロゴ
		pDevice,
		TITLE_TEXTURE_NAME_2,
		&g_pTextureTitle[1]

	);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_TITLE_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffTitle->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntTitle = 0; nCntTitle < MAX_TITLE_TEXTURE; nCntTitle++)
	{// タイトルをカウント
		// 頂点設定
		if (nCntTitle == 0)
		{// タイトルBG
			pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(TITLE_WIDTH, TITLE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(TITLE_WIDTH, TITLE_HEIGHT, 0.0f);
		}
		else if (nCntTitle == 1)
		{// タイトルロゴ
			pVtx[0].pos = D3DXVECTOR3(TITLE_WIDTH / 4, 0, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(TITLE_WIDTH / 4 * 3, 0, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(TITLE_WIDTH / 4, 0, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(TITLE_WIDTH / 4 * 3, 0, 0.0f);
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
		pVtx[1].tex = D3DXVECTOR2(TITLE_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, TITLE_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(TITLE_TEXTURE_UV_U, TITLE_TEXTURE_UV_V);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
}

//=========================================================================================================================
// タイトル終了処理
//=========================================================================================================================
void UninitTitle(void)
{
	int nCntTex;		// テクスチャカウンタ

	// テクスチャの破棄
	for (nCntTex = 0; nCntTex < MAX_TITLE_TEXTURE; nCntTex++)
	{// テクスチャカウント
		if (g_pTextureTitle[nCntTex] != NULL)
		{// NULL以外の場合
			g_pTextureTitle[nCntTex]->Release();	// 開放
			g_pTextureTitle[nCntTex] = NULL;		// NULLへ
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{// NULL以外の場合
		g_pVtxBuffTitle->Release();		// 開放
		g_pVtxBuffTitle = NULL;			// NULLへ
	}
}

//=========================================================================================================================
// タイトル更新処理
//=========================================================================================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffTitle->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// 任意のキー（ENTERキー）を押した場合
	if (GetKeyboardPress(DIK_RETURN) == true)
	{// スピード変更
		g_posTitle.y += g_moveTitle.y * 40;
	}
	else
	{// 通常移動
		g_posTitle.y +=	g_moveTitle.y;
	}

	if (g_posTitle.y >= SCREEN_HEIGHT / 3)
	{// 一定距離で止まる
		g_moveTitle.y = 0;
		SetEnterSelect(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXVECTOR3(150, 50, 0.0f));
	}

	// 頂点設定
	pVtx[4].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, g_posTitle.y - (100.0f), 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, g_posTitle.y - (100.0f), 0.0f);
	pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, g_posTitle.y + (100.0f), 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, g_posTitle.y + (100.0f), 0.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();
	
	// 任意のキー（ENTERキー）を押した場合
	FADE Fade = GetFade();		// フェードを取得

	if (Fade == FADE_NONE)
	{// フェードを使用していない場合
		if (g_posTitle.y >= SCREEN_HEIGHT / 3 && GetKeyboardRelease(DIK_RETURN) == true)
		{// 画面切り替え
			ChangeSelect(SELECTSTATE_ENTER);
			PlaySound(SOUND_LABEL_SE_DECIDE);	// 決定音
			SetFade(MODE_TUTORIAL);				// チュートリアル
		}
	}

	g_nCntMode++;

	if (g_nCntMode > 900)
	{// 画面切り替え
		SetFade(MODE_RANKING);				// ランキング
	}
}

//=========================================================================================================================
// タイトル描画処理
//=========================================================================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntTitle;			// タイトルカウンタ

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffTitle,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTitle = 0; nCntTitle < MAX_TITLE_TEXTURE; nCntTitle++)
	{// タイトルをカウント
		// テクスチャ設定
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

		// タイトルの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}
}