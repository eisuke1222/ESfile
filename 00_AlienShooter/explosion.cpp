//=========================================================================================================================
// 
// 爆発処理 [explosion.cpp]
// Author : Einsuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "explosion.h"	// 爆発

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	MAX_PLAYER					(256)								// 爆発の最大数
#define PLAYER_TEXTURE_NAME			"data\\TEXTURE\\explosion002.png"	// 読み込むテクスチャファイル
#define PLAYER_G_POS_X				(30)								// g_posからのX距離
#define PLAYER_G_POS_Y				(30)								// g_posからのY距離
#define PLAYER_TEXTURE_SPEED		(3)									// テクスチャアニメーションスピード
#define PLAYER_TEXTURE_PATTERN		(16)								// テクスチャアニメーションパターン数
#define PLAYER_TEXTURE_UV_U			(0.125f)							// テクスチャアニメーションU範囲
#define PLAYER_TEXTURE_UV_V			(0.5f)								// テクスチャアニメーションV範囲

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef struct
{// 爆発
	D3DXVECTOR3 pos;		// 位置
	D3DXCOLOR col;			// 色
	int nRadius;			// g_posからの距離
	int nCounterAnim;		// アニメーションカウンター
	int nPatternAnim;		// アニメーションパターンNo.
	bool bDisp;				// 使用しているかどうか
}Explosion;

//*************************************************************************************************************************
// グローバル変数宣言
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureExplosion = NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffExplosion = NULL;					// 頂点バッファへのポインタ
Explosion					g_aExplosion[MAX_PLAYER];					// 爆発の情報

//=========================================================================================================================
// 爆発初期化処理
//=========================================================================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntExplosion;		// 爆発カウント

	// 爆発の情報の初期化
	for(nCntExplosion = 0; nCntExplosion < MAX_PLAYER; nCntExplosion++)
	{// 爆発をカウント
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 爆発位置
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 爆発色
		g_aExplosion[nCntExplosion].nRadius = 0;								// 爆発半径
		g_aExplosion[nCntExplosion].nCounterAnim = 0;							// カウンターアニメーション
		g_aExplosion[nCntExplosion].nPatternAnim = 0;							// パターンNo.アニメーション
		g_aExplosion[nCntExplosion].bDisp = false;								// 表示設定
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		PLAYER_TEXTURE_NAME,
		&g_pTextureExplosion
	);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffExplosion->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// 爆発の情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_PLAYER; nCntExplosion++)
	{// 爆発をカウント
		// 頂点設定
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
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
		pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, PLAYER_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U, PLAYER_TEXTURE_UV_V);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=========================================================================================================================
// 爆発終了処理
//=========================================================================================================================
void UninitExplosion(void)
{
	// テクスチャーの破棄
	if (g_pTextureExplosion != NULL)
	{// NULL以外の場合
		g_pTextureExplosion->Release();		// 開放
		g_pTextureExplosion = NULL;			// NULLへ
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffExplosion != NULL)
	{// NULL以外の場合
		g_pVtxBuffExplosion->Release();		// 開放
		g_pVtxBuffExplosion = NULL;			// NULLへ
	}
}

//=========================================================================================================================
// 爆発更新処理
//=========================================================================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;				// 爆発カウント

	VERTEX_2D *pVtx;				// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffExplosion->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for(nCntExplosion = 0; nCntExplosion < MAX_PLAYER; nCntExplosion++)
	{// 爆発をカウント
		if (g_aExplosion[nCntExplosion].bDisp == true)
		{// 爆発が使用されている場合
			g_aExplosion[nCntExplosion].nCounterAnim++;		// カウンター加算

			if ((g_aExplosion[nCntExplosion].nCounterAnim % PLAYER_TEXTURE_SPEED) == 0)
			{// アニメーション速度
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % PLAYER_TEXTURE_PATTERN;		// パターンNo.更新

				// 頂点設定
				pVtx[nCntExplosion * 4].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].nRadius, 0.0f);
				pVtx[nCntExplosion * 4 + 1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].nRadius, 0.0f);
				pVtx[nCntExplosion * 4 + 2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].nRadius, 0.0f);
				pVtx[nCntExplosion * 4 + 3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].nRadius, 0.0f);
				// 頂点カラー設定
				pVtx[nCntExplosion * 4].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
				pVtx[nCntExplosion * 4 + 1].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
				pVtx[nCntExplosion * 4 + 2].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
				pVtx[nCntExplosion * 4 + 3].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
				// テクスチャ設定
				pVtx[nCntExplosion * 4].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * (g_aExplosion[nCntExplosion].nPatternAnim % 8), PLAYER_TEXTURE_UV_V * (g_aExplosion[nCntExplosion].nPatternAnim / 8));
				pVtx[nCntExplosion * 4 + 1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * (g_aExplosion[nCntExplosion].nPatternAnim % 8) + PLAYER_TEXTURE_UV_U, PLAYER_TEXTURE_UV_V * (g_aExplosion[nCntExplosion].nPatternAnim / 8));
				pVtx[nCntExplosion * 4 + 2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * (g_aExplosion[nCntExplosion].nPatternAnim % 8), PLAYER_TEXTURE_UV_V * (g_aExplosion[nCntExplosion].nPatternAnim / 8) + PLAYER_TEXTURE_UV_V);
				pVtx[nCntExplosion * 4 + 3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * (g_aExplosion[nCntExplosion].nPatternAnim % 8) + PLAYER_TEXTURE_UV_U, PLAYER_TEXTURE_UV_V * (g_aExplosion[nCntExplosion].nPatternAnim / 8) + PLAYER_TEXTURE_UV_V);

				if (g_aExplosion[nCntExplosion].nPatternAnim == PLAYER_TEXTURE_PATTERN - 1)
				{// アニメーションが最後までいった場合
					g_aExplosion[nCntExplosion].bDisp = false;		// 爆発を使用していない状態
				}
			}
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=========================================================================================================================
// 爆発描画処理
//=========================================================================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntExplosion;		// 弾カウント

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffExplosion,
		0, 
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	// ポリゴンの描画
	for(nCntExplosion = 0; nCntExplosion < MAX_PLAYER; nCntExplosion++)
	{// 爆発をカウント
		if (g_aExplosion[nCntExplosion].bDisp == true)
		{// 爆発がある場合
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//=========================================================================================================================
// 爆発の設定処理
//=========================================================================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int nRadius)
{
	int nCntExplosion;			// 爆発カウント

	VERTEX_2D *pVtx;			// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffExplosion->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for(nCntExplosion = 0; nCntExplosion< MAX_PLAYER; nCntExplosion++)
	{// 爆発をカウント
		if (g_aExplosion[nCntExplosion].bDisp == false)
		{// 爆発が使用されていない
			g_aExplosion[nCntExplosion].pos = pos;			// pos値代入
			g_aExplosion[nCntExplosion].col = col;			// move値代入
			g_aExplosion[nCntExplosion].nRadius = nRadius;	// nRadiusからの距離代入
			g_aExplosion[nCntExplosion].nCounterAnim = 0;	// カウンター初期値設定
			g_aExplosion[nCntExplosion].nPatternAnim = 0;	// パターンNo.初期値設定
			g_aExplosion[nCntExplosion].bDisp = true;		// 使用している状態にする

			// 頂点設定
			pVtx[nCntExplosion * 4].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].nRadius, 0.0f);
			pVtx[nCntExplosion * 4 + 1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].nRadius, 0.0f);
			pVtx[nCntExplosion * 4 + 2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].nRadius, 0.0f);
			pVtx[nCntExplosion * 4 + 3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].nRadius, 0.0f);
			// 頂点カラー設定
			pVtx[nCntExplosion * 4].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
			pVtx[nCntExplosion * 4 + 1].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
			pVtx[nCntExplosion * 4 + 2].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
			pVtx[nCntExplosion * 4 + 3].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);

			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}