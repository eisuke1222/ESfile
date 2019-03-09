//=============================================================================
//
// アイテムカウンター処理 [itemCnt.cpp]
// Author : 佐藤安純　Sato_Aaumi
//
//=============================================================================
#include "itemCnt.h"
#include "questfade.h"
#include "Bottun.h"
#include "item.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_SCORE		"data/TEXTURE/number003.png"	// 読み込むテクスチャファイル名
#define MAX_NUM				(2)								// 必用な列数
#define MOVE_POS			(31.0f)							// 移動量
#define NUM_SIZE			(30.0f)							// 数字の大きさ
#define POS_X				(200.0f)						// X座標
#define POS2_X				(285.0f)						// X座標
#define POS2_Y				(20.0f)							// Y座標

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexItemCnt(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureItemCnt = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItemCnt = NULL;		// 頂点バッファへのポインタ
int						g_nItemCnt;						// トータルスコア
int						g_nMaxItem;						// アイテムの総数
ItemCnt					g_ItemCnt[MAX_NUM];				// 情報の設定

//=============================================================================
// 初期化処理
//=============================================================================
void InitItemCnt(void)
{
	//変数宣言
	int nCntNum;
	int nCntMaxNum;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// スコアの初期化
	g_nItemCnt = 0;
	g_nMaxItem = 0;

	for (nCntMaxNum = 0; nCntMaxNum < MAX_NUM; nCntMaxNum++)
	{
		for (nCntNum = 0; nCntNum < NUM_SCORE; nCntNum++)
		{
			g_ItemCnt[nCntMaxNum].aNum[nCntNum] = 0;
		}
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
							  TEXTURE_SCORE,		// ファイルの名前
							  &g_pTextureItemCnt);	// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexItemCnt(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitItemCnt(void)
{
	// テクスチャの開放
	if (g_pTextureItemCnt != NULL)
	{
		g_pTextureItemCnt->Release();
		g_pTextureItemCnt = NULL;
	}

	// 頂点バッファの開放
	if (g_pTextureItemCnt != NULL)
	{
		g_pTextureItemCnt->Release();
		g_pTextureItemCnt = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateItemCnt(void)
{
	//変数宣言
	int nCntItemCnt;
	int nCntMaxNum;
	int nCntNum;			//スコアの桁数のカウンター
	int nNum = 1;
	int nNum2 = 1;

	// 頂点情報を設定
	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItemCnt->Lock(0, 0, (void**)&pVtx, 0);

	//アイテムの総数情報を取得
	g_nMaxItem = GetItem();

	//桁数の計算
	for (nCntNum = 0; nCntNum < NUM_SCORE; nCntNum++)
	{
		nNum *= 10;
	}
	for (nCntNum = 0; nCntNum < NUM_SCORE - 1; nCntNum++)
	{
		nNum2 *= 10;
	}

	//アイテムの最大数を設定する
	for (nCntNum = 0; nCntNum < NUM_SCORE; nCntNum++)
	{
		g_ItemCnt[1].aNum[nCntNum] = (g_nMaxItem % nNum) / nNum2;		//格納する数字を計算

		//桁数を１桁減らす
		nNum = nNum / 10;
		nNum2 = nNum2 / 10;
	}

	for (nCntMaxNum = 0; nCntMaxNum < MAX_NUM; nCntMaxNum++)
	{
		for (nCntItemCnt = 0; nCntItemCnt < NUM_SCORE; nCntItemCnt++)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_ItemCnt[nCntMaxNum].aNum[nCntItemCnt]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_ItemCnt[nCntMaxNum].aNum[nCntItemCnt]), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_ItemCnt[nCntMaxNum].aNum[nCntItemCnt]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_ItemCnt[nCntMaxNum].aNum[nCntItemCnt]), 1.0f);

			//該当の位置まで進める
			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffItemCnt->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawItemCnt(void)
{
	//変数宣言
	int nCntItemCnt;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffItemCnt, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureItemCnt);

	// ポリゴンの描画
	for (nCntItemCnt = 0; nCntItemCnt < NUM_SCORE * MAX_NUM; nCntItemCnt++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntItemCnt * 4,
			2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexItemCnt(LPDIRECT3DDEVICE9 pDevice)
{
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_SCORE * MAX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItemCnt,
		NULL);

	// 頂点情報を設定
	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffItemCnt->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(POS_X + 0.0f, 10.0f , 0.0f);
	pVtx[1].pos = D3DXVECTOR3(POS_X + NUM_SIZE, 10.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(POS_X + 0.0f, 60.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(POS_X + NUM_SIZE, 60.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点座標の設定
	pVtx[4].pos = D3DXVECTOR3(POS_X + 0.0f + MOVE_POS, 10.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(POS_X + NUM_SIZE + MOVE_POS, 10.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(POS_X + 0.0f + MOVE_POS, 60.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(POS_X + NUM_SIZE + 30.0f, 60.0f, 0.0f);

	//rhwの設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[4].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点座標の設定
	pVtx[8].pos = D3DXVECTOR3(POS2_X + 0.0f, POS2_Y + 10.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(POS2_X + NUM_SIZE, POS2_Y + 10.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(POS2_X + 0.0f, POS2_Y + 60.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(POS2_X + NUM_SIZE, POS2_Y + 60.0f, 0.0f);

	//rhwの設定
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[8].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点座標の設定
	pVtx[12].pos = D3DXVECTOR3(POS2_X + MOVE_POS, POS2_Y + 10.0f, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(POS2_X + NUM_SIZE + MOVE_POS, POS2_Y + 10.0f, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(POS2_X + 0.0f + MOVE_POS, POS2_Y + 60.0f, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(POS2_X + NUM_SIZE + MOVE_POS, POS2_Y + 60.0f, 0.0f);

	//rhwの設定
	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[12].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[14].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	pVtx[15].col = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(0.1f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffItemCnt->Unlock();
}

//=============================================================================
// スコアの加算
//=============================================================================
void AddItemCnt(int nValue)
{
	//変数宣言
	int nCntNum;			//スコアの桁数のカウンター
	int nNum = 1;
	int nNum2 = 1;

	//桁数の計算
	for (nCntNum = 0; nCntNum < NUM_SCORE; nCntNum++)
	{
		nNum *= 10;
	}
	for (nCntNum = 0; nCntNum < NUM_SCORE - 1; nCntNum++)
	{
		nNum2 *= 10;
	}

	//スコア加算
	g_nItemCnt += nValue;

	for (nCntNum = 0; nCntNum < NUM_SCORE; nCntNum++)
	{
		g_ItemCnt[0].aNum[nCntNum] = (g_nItemCnt % nNum) / nNum2;	//格納する数字を計算

		//桁数を１桁減らす
		nNum = nNum / 10;
		nNum2 = nNum2 / 10;
	}

	if (g_nItemCnt >= g_nMaxItem)
	{
		SetBottunType(BOTTUNTYPE_SELECT);
	}
}

//========================================================================================
//アイテムの情報
//========================================================================================
int GetItemCnt(void)
{
	return g_nItemCnt;
}

//========================================================================================
//カウンターをリセットする
//========================================================================================
void ResetItemCnt(void)
{
	g_nItemCnt = 0;
}

//========================================================================================
// アイテムを破棄
//========================================================================================
void DeleteItemCnt(void)
{
	//変数宣言
	int nCntNum;
	int nCntMaxNum;

	// スコアの初期化
	g_nItemCnt = 0;
	g_nMaxItem = 0;

	for (nCntMaxNum = 0; nCntMaxNum < MAX_NUM; nCntMaxNum++)
	{
		for (nCntNum = 0; nCntNum < NUM_SCORE; nCntNum++)
		{
			g_ItemCnt[nCntMaxNum].aNum[nCntNum] = 0;
		}
	}
}