//=============================================================================
//
// ストーリー画面 [story.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "story.h"
#include "main.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
#include "BG.h"
#include "start.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TEX	(3)				//読みこむテクスチャの最大数
#define MAX_POLYGON	(3)			//背景の最大数
#define FADE_SPEED	(0.015f)		//フェードのスピード

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureStory[MAX_TEX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStory = NULL;			//頂点バッファへのポインタ
Story					g_Story[MAX_POLYGON];			//情報設定
STORYSTATE				g_state;						//状態設定

//=============================================================================
// 初期化処理
//=============================================================================
void InitStory(void)
{
	//変数宣言
	int		nCntTex;
	int		nCntStory;
	char	cTexName[MAX_TEX][48];		//ファイル名を格納する


	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数の初期化
	for (nCntStory = 0; nCntStory < MAX_POLYGON; nCntStory++)
	{
		g_Story[nCntStory].bAppear = false;
		g_Story[nCntStory].fCol = 0.0f;
		g_Story[nCntStory].nTexNum = nCntStory;
	}
	g_state = STORYSTATE_FADE;

	//テクスチャファイル名を格納する
	wsprintf(&cTexName[0][0], "data\\TEXTURE\\story00.png");			//１文目
	wsprintf(&cTexName[1][0], "data\\TEXTURE\\story01.png");			//2文目
	wsprintf(&cTexName[2][0], "data\\TEXTURE\\story02.png");			//3文目

	//テクスチャの読み込み
	for (nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
								  &cTexName[nCntTex][0],
								  &g_pTextureStory[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffStory,
								NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStory->Lock(0, 0, (void**)&pVtx, 0);

//１文目------------------------------------------------------
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(100.0f, 150.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1200.0f, 150.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(100.0f, 270.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1200.0f, 270.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

//2文目-----------------------------------------------------
		//頂点座標の設定
		pVtx[4].pos = D3DXVECTOR3(100.0f, 300.0f, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(1200.0f, 300.0f, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(100.0f, 420.0f, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(1200.0f, 420.0f, 0.0f);

		//rhwの設定
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

//3文目----------------------------------------------------
		//頂点座標の設定
		pVtx[8].pos = D3DXVECTOR3(100.0f, 450.0f, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(1200.0f, 450.0f, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(100.0f, 570.0f, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(1200.0f, 570.0f, 0.0f);

		//rhwの設定
		pVtx[8].rhw = 1.0f;
		pVtx[9].rhw = 1.0f;
		pVtx[10].rhw = 1.0f;
		pVtx[11].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffStory->Unlock();

	//変数呼び出し
	InitStart();		//エンターキー

	//背景
	SetBG(0);
	
	//エンターキー
	SetEnter(D3DXVECTOR3(190.0f, 695.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStory(void)
{
	//変数宣言
	int nCntTex;

	//テクスチャの破棄
	for (nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		if (g_pTextureStory[nCntTex] != NULL)
		{
			g_pTextureStory[nCntTex]->Release();
			g_pTextureStory[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffStory != NULL)
	{
		g_pVtxBuffStory->Release();
		g_pVtxBuffStory = NULL;
	}

	UninitStart();		//エンターキー
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStory(void)
{
	//変数宣言
	int nCntStory;

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStory->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_state)
	{
	case STORYSTATE_FADE:
		if (g_Story[0].bAppear == false)
		{
			g_Story[0].fCol += FADE_SPEED;	//文字を浮き上がらせる

			if (g_Story[0].fCol >= 1.0f)
			{
				g_Story[0].fCol = 1.0f;		//値を戻す
				g_Story[0].bAppear = true;
			}

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[0].fCol);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[0].fCol);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[0].fCol);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[0].fCol);
		}

		if (g_Story[0].bAppear == true && g_Story[1].bAppear == false)
		{
			g_Story[1].fCol += FADE_SPEED;	//文字を浮き上がらせる

			if (g_Story[1].fCol >= 1.0f)
			{
				g_Story[1].fCol = 1.0f;		//値を戻す
				g_Story[1].bAppear = true;
			}

			//頂点カラーの設定
			pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[1].fCol);
			pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[1].fCol);
			pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[1].fCol);
			pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[1].fCol);
		}

		if (g_Story[1].bAppear == true && g_Story[2].bAppear == false)
		{
			g_Story[2].fCol += FADE_SPEED;	//文字を浮き上がらせる

			if (g_Story[2].fCol >= 1.0f)
			{
				g_Story[2].fCol = 1.0f;		//値を戻す
				g_Story[2].bAppear = true;
			}

			//頂点カラーの設定
			pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[2].fCol);
			pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[2].fCol);
			pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[2].fCol);
			pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_Story[2].fCol);
		}

		//全ての文が表示状態になったら
		if (g_Story[0].bAppear == true &&
			g_Story[1].bAppear == true &&
			g_Story[2].bAppear == true)
		{
			g_state = STORYSTATE_APPEAR;	//表示状態にする
		}

		//エンターキーを押したら
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);	//サウンド再生
			g_state = STORYSTATE_APPEAR;		//表示状態にする
		}
		break;

	case STORYSTATE_APPEAR:

		for (nCntStory = 0; nCntStory < MAX_POLYGON; nCntStory++)
		{
			g_Story[nCntStory].bAppear = true;	//表示状態にする
			
			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//該当の位置まで進める
			pVtx += 4;
		}

		//フェード情報を取得する
		FADE fade;
		fade = GetFade();

		//決定キーが押されたかどうか
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
		{
			if (fade != FADE_OUT)		//フェードアウトが実行されていないなら
			{
				PlaySound(SOUND_LABEL_SE_DECIDE);	//サウンド再生
				SetPush(STARTSTRACT_ENTER);			//スタートボタンの設定

				//フェードモード設定
				SetFade(MODE_GAME);
			}
		}

		break;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffStory->Unlock();

	UpdateStart();		//エンターキー
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStory(void)
{
	//変数宣言
	int nCntStory;

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStory, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntStory = 0; nCntStory < MAX_POLYGON; nCntStory++)
	{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureStory[g_Story[nCntStory].nTexNum]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								   nCntStory * 4,			//開始する頂点のインデックス
								   2);						//プリミティブの数
	}

	DrawStart();		//エンターキー
}