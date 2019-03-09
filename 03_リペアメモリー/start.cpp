//=============================================================================
//
// スタートボタン [start.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "start.h"
#include "main.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define START_TEXTURENAME	"data\\TEXTURE\\press_enter.png"	//読み込むテクスチャファイル
#define WIDTH				(350)								//横幅
#define HIGHT				(50)								//縦幅
#define HERF_W				(WIDTH / 2)							//横幅 / ２
#define HERF_H				(HIGHT / 2)							//縦幅 / ２

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureStart = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStart = NULL;		//頂点バッファへのポインタ
ENTER					g_Enter;					//情報設定

//=============================================================================
// スタートボタン初期化処理
//=============================================================================
void InitStart(void)
{
	//変数の初期化
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	g_Enter.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置情報
	g_Enter.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色情報
	g_Enter.ChangeCol = 0.01f;							//変化量の情報
	g_Enter.Struct = STARTSTRACT_NORMAL;				//状態の情報
	g_Enter.bUse = false;								//使用していない状態にする

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		START_TEXTURENAME,
		&g_pTextureStart);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStart,
		NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffStart->Unlock();
}

//=============================================================================
// タイトル終了処理
//=============================================================================
void UninitStart(void)
{
	//テクスチャの破棄
	if (g_pTextureStart != NULL)
	{
		g_pTextureStart->Release();
		g_pTextureStart = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffStart != NULL)
	{
		g_pVtxBuffStart->Release();
		g_pVtxBuffStart = NULL;
	}
}

//=============================================================================
// タイトル更新処理
//=============================================================================
void UpdateStart(void)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Enter.bUse == true)
	{
		switch (g_Enter.Struct)
		{
		case STARTSTRACT_NORMAL:

			//変化量の符号変換
			if (g_Enter.col.a <= 0.0f || g_Enter.col.a >= 1.0f)
			{
				g_Enter.ChangeCol *= -1;
			}

			//カラー更新
			g_Enter.col.a += g_Enter.ChangeCol;

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(g_Enter.col.r, g_Enter.col.g, g_Enter.col.b, g_Enter.col.a);
			pVtx[1].col = D3DXCOLOR(g_Enter.col.r, g_Enter.col.g, g_Enter.col.b, g_Enter.col.a);
			pVtx[2].col = D3DXCOLOR(g_Enter.col.r, g_Enter.col.g, g_Enter.col.b, g_Enter.col.a);
			pVtx[3].col = D3DXCOLOR(g_Enter.col.r, g_Enter.col.g, g_Enter.col.b, g_Enter.col.a);
			break;

		case STARTSTRACT_ENTER:
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffStart->Unlock();
}

//=============================================================================
// タイトル描画処理
//=============================================================================
void DrawStart(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStart, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureStart);

	if (g_Enter.bUse == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
			0,											//開始する頂点のインデックス
			2);											//プリミティブの数
	}
}

//=============================================================================
// 情報設定
//=============================================================================
void SetEnter(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffStart->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Enter.bUse == false)
	{
		g_Enter.pos = pos;						//位置情報

												//位置情報の設定
		pVtx[0].pos = D3DXVECTOR3(g_Enter.pos.x - HERF_W, g_Enter.pos.y - HERF_H, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Enter.pos.x + HERF_W, g_Enter.pos.y - HERF_H, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Enter.pos.x - HERF_W, g_Enter.pos.y + HERF_H, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Enter.pos.x + HERF_W, g_Enter.pos.y + HERF_H, 0.0f);

		g_Enter.col = col;						//色

												//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(g_Enter.col.r, g_Enter.col.g, g_Enter.col.b, g_Enter.col.a);
		pVtx[1].col = D3DXCOLOR(g_Enter.col.r, g_Enter.col.g, g_Enter.col.b, g_Enter.col.a);
		pVtx[2].col = D3DXCOLOR(g_Enter.col.r, g_Enter.col.g, g_Enter.col.b, g_Enter.col.a);
		pVtx[3].col = D3DXCOLOR(g_Enter.col.r, g_Enter.col.g, g_Enter.col.b, g_Enter.col.a);

		g_Enter.ChangeCol = 0.01f;				//変化量
		g_Enter.Struct = STARTSTRACT_NORMAL;	//情報設定
		g_Enter.bUse = true;					//使用している状態にする
	}
	//頂点バッファをアンロックする
	g_pVtxBuffStart->Unlock();
}

//=============================================================================
// スタートボタンの状態設定
//=============================================================================
void SetPush(STARTSTRACT stract)
{//状態の設定
	g_Enter.Struct = stract;
}

//=============================================================================
// スタートボタンの状態設定
//=============================================================================
void DeleteStart(void)
{//状態の設定
	g_Enter.bUse = false;
}
