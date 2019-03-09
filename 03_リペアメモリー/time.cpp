//=============================================================================
//
// 背景処理 [time.cpp]
// Author : 佐藤安純
//
//=============================================================================
#include "time.h"
#include "game.h"
#include "score.h"
#include "flontpolygon.h"
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TIME		"data/TEXTURE/number003.png"						// 読み込むテクスチャファイル名
#define TIME_NUM			(3)													//表示する桁数
#define TIME_RIMIT			(60 * 300)											//制限時間
#define TIME_SIZE			(50.0f)												//ポリゴンの大きさ
#define TIME_RED			(60 * 30)											//タイムを赤にする秒数
#define TIME_POS			((SCREEN_WIDTH / 2) - ((TIME_SIZE * TIME_NUM) / 2))	//制限時間のポジション

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTime = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;			// 頂点バッファへのポインタ
int		g_aTime[TIME_NUM];								// 数字を格納する変数
Time	g_Time;

//=============================================================================
// 初期化処理
//=============================================================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数宣言
	int nCntTime;
	
	g_Time.nTime = TIME_RIMIT;
	g_Time.TimeState = TIMESTATE_STOP;

	for (nCntTime = 0; nCntTime < TIME_NUM; nCntTime++)
	{
		g_aTime[nCntTime] = 0;
	}
	
	//カウントダウンの計算
	CntDownTime();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
							  TEXTURE_TIME,			// ファイルの名前
							  &g_pTextureTime);		// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexTime(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTime(void)
{
	// テクスチャの開放
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	// 頂点バッファの開放
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTime(void)
{
	// 頂点情報を設定
	VERTEX_2D*pVtx;			//頂点情報へのポインタ

	//変数宣言
	int nCntTime;

	bool bDeletePartsTuxture = DeletePartsTexture();

	switch (g_Time.TimeState)
	{//タイム状態の設定
	case TIMESTATE_NORMAL:
		break;

	case TIMESTATE_STOP:
		g_Time.nTime = g_Time.nTime;
		break;

	case TIMESTATE_END:
		g_Time.TimeState = TIMESTATE_NONE;
		SetFade(MODE_GAMEOVER);
		break;

	case TIMESTATE_START:
		g_Time.nTime = TIME_RIMIT;		//時間を初期化する
		SetTimeState(TIMESTATE_NORMAL);
		break;
	}

	if (g_Time.TimeState == TIMESTATE_NORMAL && bDeletePartsTuxture == false)
	{
		g_Time.nTime--;		//時間をカウントダウン

		if ((g_Time.nTime % 60) == 0)	//時間表示計算
		{
			//カウントダウンの計算
			CntDownTime();

			if (g_Time.nTime == 0)
			{
				g_Time.TimeState = TIMESTATE_END;	//タイム終了状態にする
			}
		}	

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntTime = 0; nCntTime < TIME_NUM; nCntTime++)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aTime[nCntTime]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aTime[nCntTime]), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aTime[nCntTime]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aTime[nCntTime]), 1.0f);

			if (g_Time.nTime <= 300)		//残り５秒で赤くする
			{//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}

			//該当の位置まで進める
			pVtx += 4;
		}

		//頂点バッファをアンロックする
		g_pVtxBuffTime->Unlock();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTime(void)
{
	//変数宣言
	int nCntTime;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	// ポリゴンの描画
	for (nCntTime = 0; nCntTime < TIME_NUM; nCntTime++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntTime * 4,
			2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	//変数宣言
	int nCntTime;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TIME_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	// 頂点情報を設定
	VERTEX_2D*pVtx;			//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < TIME_NUM; nCntTime++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(TIME_POS + 0.0f + (TIME_SIZE * nCntTime), 15.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(TIME_POS + TIME_SIZE + (TIME_SIZE * nCntTime), 15.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(TIME_POS + 0.0f + (TIME_SIZE * nCntTime), TIME_SIZE + 15.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(TIME_POS + TIME_SIZE + (TIME_SIZE * nCntTime), TIME_SIZE + 15.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.4f, 0.5f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.4f, 0.5f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.4f, 0.5f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.4f, 0.5f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//該当の位置まで進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//=============================================================================
// 時間の処理
//=============================================================================
void CntDownTime(void)
{
	//変数宣言
	int nNum = 1;
	int nNum2 = 1;
	int nCntNum;
	int nTime;

	//桁数の計算
	for (nCntNum = 0; nCntNum < TIME_NUM; nCntNum++)
	{
		nNum *= 10;
	}
	for (nCntNum = 0; nCntNum < TIME_NUM - 1; nCntNum++)
	{
		nNum2 *= 10;
	}

	nTime = g_Time.nTime / 60;	//秒数の計算

	for (nCntNum = 0; nCntNum < TIME_NUM; nCntNum++)
	{
		g_aTime[nCntNum] = (nTime % nNum) / nNum2;	//格納する数字を計算

													//桁数を１桁減らす
		nNum = nNum / 10;
		nNum2 = nNum2 / 10;

		if (nCntNum == TIME_NUM - 1)
		{//初期化する
			nNum = 100;
			nNum2 = 10;
		}
	}
}

//=============================================================================
// 時間の情報
//=============================================================================
Time *GetTime(void)
{
	return &g_Time;
}

//=============================================================================
// 時間の情報
//=============================================================================
void SetTimeState(TIME state)
{
	int nCntTime;

	if (state == TIMESTATE_ADD)
	{
		AddScore(g_Time.nTime / 6);
	}
	else if (state == TIMESTATE_RESET)
	{
		g_Time.nTime = TIME_RIMIT;
		
		CntDownTime();

		// 頂点情報を設定
		VERTEX_2D*pVtx;			//頂点情報へのポインタ

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

		for (nCntTime = 0; nCntTime < TIME_NUM; nCntTime++)
		{
			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aTime[nCntTime]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * g_aTime[nCntTime]), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (0.1f * g_aTime[nCntTime]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aTime[nCntTime]), 1.0f);

			//該当の位置まで進める
			pVtx += 4;
		}

		//頂点バッファをアンロックする
		g_pVtxBuffTime->Unlock();
	}
	else
	{
		g_Time.TimeState = state;
	}
}