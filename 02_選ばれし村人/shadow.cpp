//=========================================================================================================================
//
// 影処理 [shadow.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "shadow.h"
#include "input.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_SHADOW				(512)									// 影の最大数
#define MAX_MODEL_SHADOW		(1)										// 影の1モデル最大数
#define SHADOW_TEXTURE_NAME1	"data\\TEXTURE\\shadow000.jpg"			// 読み込むテクスチャファイル1
#define SHADOW_TEXTURE_UV_U		(1.0f)									// テクスチャアニメーションU範囲
#define SHADOW_TEXTURE_UV_V		(1.0f)									// テクスチャアニメーションV範囲

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);						// 頂点情報の設定

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureShadow = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffShadow = NULL;				// 頂点バッファへのポインタ
SHADOW							g_Shadow[MAX_SHADOW];					// 影の情報

//=========================================================================================================================
// 影の初期化処理
//=========================================================================================================================
void InitShadow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntShadow;

	// 頂点情報の作成
	MakeVertexShadow(pDevice);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].bUse = false;
	}
}

//=========================================================================================================================
// 影の終了処理
//=========================================================================================================================
void UninitShadow(void)
{
	// テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=========================================================================================================================
// 影の更新処理
//=========================================================================================================================
void UpdateShadow(void)
{

}

//=========================================================================================================================
// 影の描画処理
//=========================================================================================================================
void DrawShadow(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	int nCntShadow;

	// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Shadow[nCntShadow].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_Shadow[nCntShadow].rot.y,
				g_Shadow[nCntShadow].rot.x,
				g_Shadow[nCntShadow].rot.z
			);

			D3DXMatrixMultiply
			(
				&g_Shadow[nCntShadow].mtxWorld,
				&g_Shadow[nCntShadow].mtxWorld,
				&mtxRot
			);

			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_Shadow[nCntShadow].pos.x,
				g_Shadow[nCntShadow].pos.y,
				g_Shadow[nCntShadow].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_Shadow[nCntShadow].mtxWorld,
				&g_Shadow[nCntShadow].mtxWorld,
				&mtxTrans
			);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Shadow[nCntShadow].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource
			(
				0,
				g_pVtxBuffShadow,
				0,
				sizeof(VERTEX_3D)
			);

			// 頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureShadow);

			// プレイヤーの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=========================================================================================================================
// 影の設定処理
//=========================================================================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 fRadius)
{
	// 変数宣言
	int nCntShadow;
	int nCntSet = 0;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffShadow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == false)
		{
			g_Shadow[nCntShadow].pos = pos;
			g_Shadow[nCntShadow].pos.y = 0.1f;
			g_Shadow[nCntShadow].rot = rot;
			g_Shadow[nCntShadow].bUse = true;

			if (nCntSet == 0)
			{
				g_Shadow[nCntShadow].fRadius = fRadius;
			}
			else
			{
				g_Shadow[nCntShadow].fRadius = g_Shadow[nCntShadow - 1].fRadius - (fRadius / MAX_MODEL_SHADOW);
			}

			// 頂点情報の設定
			pVtx[nCntShadow * 4].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fRadius.x, 0.0f, g_Shadow[nCntShadow].fRadius.z);
			pVtx[nCntShadow * 4 + 1].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fRadius.x, 0.0f, g_Shadow[nCntShadow].fRadius.z);
			pVtx[nCntShadow * 4 + 2].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fRadius.x, 0.0f, -g_Shadow[nCntShadow].fRadius.z);
			pVtx[nCntShadow * 4 + 3].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fRadius.x, 0.0f, -g_Shadow[nCntShadow].fRadius.z);

			nCntSet++;
			if (nCntSet == MAX_MODEL_SHADOW)
			{
				break;
			}
		}
	}
	
	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//=========================================================================================================================
// 影の位置設定処理
//=========================================================================================================================
void SetPositionShadow(int IdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// 変数宣言
	int nCntShadow;

	for (nCntShadow = IdxShadow - MAX_MODEL_SHADOW + 1; nCntShadow <= IdxShadow; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			g_Shadow[nCntShadow].pos = pos;
			g_Shadow[nCntShadow].pos.y = 0.1f;
			g_Shadow[nCntShadow].rot = rot;
		}
	}
}

//=========================================================================================================================
// 影の破棄処理
//=========================================================================================================================
void DeleteShadow(int IdxShadow)
{
	// 変数宣言
	int nCntShadow;

	for (nCntShadow = IdxShadow - MAX_MODEL_SHADOW + 1; nCntShadow <= IdxShadow; nCntShadow++) 
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].bUse = false;
		}
	}
}
void DeleteShadow(void)
{
	// 変数宣言
	int nCntShadow;

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].bUse = false;
		}
	}
}

//=========================================================================================================================
// 頂点情報の作成
//=========================================================================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	int nCntShadow;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTURE_NAME1, &g_pTextureShadow);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL
	);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffShadow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// 頂点情報の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(SHADOW_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, SHADOW_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(SHADOW_TEXTURE_UV_U, SHADOW_TEXTURE_UV_V);
	
		pVtx += 4;		// 頂点バッファのポインタを4つ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

