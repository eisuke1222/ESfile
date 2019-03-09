//=============================================================================
//
// ランキング画面 [rank.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "rank.h"
#include "main.h"
//#include "sound.h"
#include "input.h"
#include "fade.h"
#include "game.h"
//#include "start.h"
//#include "ranking.h"
//#include "score.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define RANK_TEXTURENAME		"data\\TEXTURE\\score000.png"		//読み込むテクスチャファイル
#define RANKLOGO_TEXTURENAME	"data\\TEXTURE\\score003.png"		//読み込むテクスチャファイル
#define RANKTEXTURE_MAX			(2)									//テクスチャの最大数
#define RANK_POS_X				(0)									//背景の左上X座標
#define RANK_POS_Y				(0)									//背景の左上Y座標
#define RANK_WHIDTH				(SCREEN_WIDTH)						//背景の幅
#define RANK_HEIGHT				(SCREEN_HEIGHT)						//背景の高さ

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureRank[RANKTEXTURE_MAX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;				//頂点バッファへのポインタ
int g_nCntTimeRank;											//ゲーム画面切り替えカウンター

//=============================================================================
// 初期化処理
//=============================================================================
void InitRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		RANK_TEXTURENAME,
		&g_pTextureRank[0]);

	D3DXCreateTextureFromFile(pDevice,
							  RANKLOGO_TEXTURENAME,
							  &g_pTextureRank[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RANKTEXTURE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank,
		NULL);

	VERTEX_2D*pVtx;			//頂点情報へのポインタ
	g_nCntTimeRank = 0;		//カウンターの初期化

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RANK_POS_X + RANK_WHIDTH, RANK_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RANK_POS_X, RANK_POS_Y + RANK_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RANK_POS_X + RANK_WHIDTH, RANK_POS_Y + RANK_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点座標の設定
	pVtx[4].pos = D3DXVECTOR3(350, 0, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(1000, 0, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(350, 150, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(1000, 150, 0.0f);

	//rhwの設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRank->Unlock();

	//変数呼び出し
	//InitStart();		//エンターキー
	//InitHighScore();	//ランキング		

	//エンターキー
	//SetEnter(D3DXVECTOR3(190.0f, 695.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRank(void)
{
	//変数宣言
	int nCntRank;

	//テクスチャの破棄
	for (nCntRank = 0; nCntRank < RANKTEXTURE_MAX; nCntRank++)
	{
		if (g_pTextureRank[nCntRank] != NULL)
		{
			g_pTextureRank[nCntRank]->Release();
			g_pTextureRank[nCntRank] = NULL;
		}
	}
	
	//頂点バッファの破棄
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}

	//UninitStart();		//エンターキー
	//UninitHighScore();	//ランキング
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRank(void)
{
	g_nCntTimeRank++;

	//フェード情報を取得する
	FADE g_fade;
	g_fade = GetFade();

	//決定キーが押されたかどうか
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (g_fade != FADE_OUT)		//フェードアウトが実行されていないなら
		{
			//サウンド再生
			//PlaySound(SOUND_LABEL_SE_ENTER);

			//エンターキーの状態変更
			//SetPush(STARTSTRACT_ENTER);

			//フェードモード設定
			SetFade(MODE_TITLE);
		}
	}
	if (g_nCntTimeRank == INTERVAL_TIME)
	{
		//フェードモード設定
		SetFade(MODE_TITLE);
	}

	//SetHighScore(GetScore());

	//UpdateStart();		//エンターキー
	//UpdateHighScore();	//ハイスコア
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRank(void)
{
	//変数宣言
	int nCntRank;

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntRank = 0; nCntRank < RANKTEXTURE_MAX; nCntRank++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRank[nCntRank]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							  nCntRank * 4,				//開始する頂点のインデックス
							  2);						//プリミティブの数
	}

	//DrawStart();		//エンターキー
	//DrawHighScore();	//ランキング
}