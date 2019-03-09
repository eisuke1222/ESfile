//=============================================================================
//
// 背景処理 [BG.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "BG.h"
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TEX	(3)		//読みこむテクスチャの最大数
#define MAX_BG	(2)		//背景の最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureBG[MAX_TEX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;		//頂点バッファへのポインタ
BG						g_BG[MAX_BG];				//情報設定

//=============================================================================
// スタートボタン初期化処理
//=============================================================================
void InitBG(void)
{
	//変数宣言
	char	BGName[MAX_TEX][48];		//ファイル名を格納する
	int		nCntTex;
	int     nCntBG;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数の初期化
	for (nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{
		g_BG[nCntBG].nTexNum = 0;				//テクスチャの番号
		g_BG[nCntBG].bUse = false;				//使用していない状態にする
	}

	//テクスチャファイル名を格納する
	wsprintf(&BGName[0][0], "data\\TEXTURE\\bg004.jpg");	//ストーリー背景
	wsprintf(&BGName[1][0], "data\\TEXTURE\\select.jpg");	//クエスト選択画面背景
	wsprintf(&BGName[2][0], "data\\TEXTURE\\quest.jpg");	//クエストクリア画面


	//テクスチャの読み込み
	for (nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
								  &BGName[nCntTex][0],
								  &g_pTextureBG[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBG,
								NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTex = 0; nCntTex < MAX_BG; nCntTex++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

		//該当の位置まで進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitBG(void)
{
	//変数宣言
	int nCntTex;

	//テクスチャの破棄
	for (nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		if (g_pTextureBG[nCntTex] != NULL)
		{
			g_pTextureBG[nCntTex]->Release();
			g_pTextureBG[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateBG(void)
{
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawBG(void)
{
	//変数宣言
	int nCntBG;

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{
		if (g_BG[nCntBG].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBG[g_BG[nCntBG].nTexNum]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
								   nCntBG * 4,				//開始する頂点のインデックス
								   2);						//プリミティブの数
		}
	}
}

//=============================================================================
// 情報設定
//=============================================================================
void SetBG(int nTexNum)
{
	//変数宣言
	int nCntBG;

	for (nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{
		if (g_BG[nCntBG].bUse == false)
		{
			g_BG[nCntBG].nTexNum = nTexNum;		//テクスチャの番号
			g_BG[nCntBG].bUse = true;			//使用している状態にする
			break;
		}
	}
}

//=============================================================================
// 背景を消す
//=============================================================================
void DeleteBG()
{
	//変数宣言
	int nCntBG;

	for (nCntBG = 0; nCntBG < MAX_BG; nCntBG++)
	{
		if (g_BG[nCntBG].bUse == true)
		{
			g_BG[nCntBG].bUse = false;		//使用していない状態にする
		}
	}
}