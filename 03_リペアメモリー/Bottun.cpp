//=============================================================================
//
// ボタン処理 [Bottun.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "Bottun.h"
#include "main.h"
#include "sound.h"
#include "input.h"
#include "questfade.h"
#include "flontpolygon.h"
#include  "time.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_TEX			(1)		//テクスチャの最大数
#define	MAX_POLYGON		(1)		//ポリゴンの最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureBottun[MAX_TEX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBottun = NULL;		//頂点バッファへのポインタ
int						g_nBottunType;					//選択されているタイプ
Bottun					g_Bottun[MAX_POLYGON];			//情報の最大数
int						g_nCntBottun;

//=============================================================================
// 初期化処理
//=============================================================================
void InitBottun(void)
{
	//変数宣言
	int		nCntTex;
	int		nCntBottun;
	char	cTexName[MAX_TEX][48];		//ファイル名を格納する

	g_nCntBottun = 0;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数の初期化
	for (nCntBottun = 0; nCntBottun < MAX_POLYGON; nCntBottun++)
	{
		g_Bottun[nCntBottun].nTex = nCntBottun;
		g_Bottun[nCntBottun].bUse = false;
		g_Bottun[nCntBottun].type = BOTTUNTYPE_NOMAL;
	}
	g_nBottunType = QUESTTYPE_TUTORIAL;		//チュートリアルに設定する

	//テクスチャファイル名を格納する
	wsprintf(&cTexName[0][0], "data\\TEXTURE\\bottun_atelier.png");		//[工房へ]のボタン

	//テクスチャの読み込み
	for (nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
								  &cTexName[nCntTex][0],
								  &g_pTextureBottun[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBottun,
		NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBottun->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBottun = 0; nCntBottun < MAX_POLYGON; nCntBottun++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(970.0f, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1270.0f, 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(970.0f, 80.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1270.0f, 80.0f, 0.0f);

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
	g_pVtxBuffBottun->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBottun(void)
{
	//変数宣言
	int nCntTex;

	//テクスチャの破棄
	for (nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		if (g_pTextureBottun[nCntTex] != NULL)
		{
			g_pTextureBottun[nCntTex]->Release();
			g_pTextureBottun[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBottun != NULL)
	{
		g_pVtxBuffBottun->Release();
		g_pVtxBuffBottun = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBottun(void)
{
	//変数宣言
	int nCntBottun;

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBottun->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBottun = 0; nCntBottun < MAX_POLYGON; nCntBottun++)
	{
		if (g_Bottun[nCntBottun].bUse == true)
		{
			switch (g_Bottun[nCntBottun].type)
			{
			case BOTTUNTYPE_NOMAL:

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
				break;

			case BOTTUNTYPE_SELECT:

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				if (g_nCntBottun == 0)
				{
					SetFlontPolygon(FLONTPOLYGONTEXTURE_PAZZLE_IN, D3DXVECTOR3(SCREEN_WIDTH + 400, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 700, 300);
					SetTimeState(TIMESTATE_STOP);
				}
				else if (g_nCntBottun == 120/*GetKeyboardTrigger(DIK_O) == true*/)
				{
					SetBottunType(BOTTUNTYPE_ENTER);
				}
				
				g_nCntBottun++;
				
				break;

			case BOTTUNTYPE_ENTER:

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				//クエストフェードの情報を取得する
				QUESTFADE  pQuestFade;
				pQuestFade = GetQuestFade();

				if (pQuestFade != QUESTFADE_OUT)
				{//パズルモードへ移行する
					SetQuestFade(QUESTSTATE_PUZZLE);
				}
				break;
			}
		}
		//該当の位置まで進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBottun->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBottun(void)
{
	//変数宣言
	int nCntBottun;

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBottun, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBottun = 0; nCntBottun < MAX_POLYGON; nCntBottun++)
	{
		if (g_Bottun[nCntBottun].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBottun[g_Bottun[nCntBottun].nTex]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCntBottun * 4,			//開始する頂点のインデックス
				2);						//プリミティブの数
		}
	}
}

//=============================================================================
// ボタンの状態設定
//=============================================================================
void SetBottunType(BOTTUNTYPE type)
{
	//変数宣言
	int nCntBottun;

	for (nCntBottun = 0; nCntBottun < MAX_POLYGON; nCntBottun++)
	{
		if (g_Bottun[nCntBottun].bUse == true)
		{
			g_Bottun[nCntBottun].type = type;
		}
	}
}

//=============================================================================
// ボタンの設定
//=============================================================================
void SetBottun(void)
{
	//変数宣言
	int nCntBottun;

	for (nCntBottun = 0; nCntBottun < MAX_POLYGON; nCntBottun++)
	{
		if (g_Bottun[nCntBottun].bUse == false)
		{
			g_Bottun[nCntBottun].bUse = true;
			g_Bottun[nCntBottun].type = BOTTUNTYPE_NOMAL;
			break;
		}
	}
}

//=============================================================================
// ボタンを消す
//=============================================================================
void DeleteBottun(void)
{
	//変数宣言
	int nCntBottun;

	for (nCntBottun = 0; nCntBottun < MAX_POLYGON; nCntBottun++)
	{
		g_Bottun[nCntBottun].bUse = false;
	}

	g_nCntBottun = 0;
}