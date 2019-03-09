//=========================================================================================================================
// 
// エフェクト処理 [effect.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "effect.h"		// エフェクト

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	MAX_EFFECT				(14096)									// エフェクトの最大数
#define	TEXTURE_EFFECT_NAME		"data/TEXTURE/effect000.jpg"			// 読み込むテクスチャファイル名

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureEffect = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffEffect = NULL;				// 頂点バッファへのポインタ
EFFECT							g_aEffect[MAX_EFFECT];					// エフェクトの情報
int								g_nCntEffectState;						// 状態カウンタ

//=========================================================================================================================
// エフェクト初期化処理
//=========================================================================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスを取得
	int nCntEffect;		// エフェクトカウンタ

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// エフェクトをカウント
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// エフェクト中心値
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// エフェクト移動値
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// エフェクト色
		g_aEffect[nCntEffect].Type = EFFECTTYPE_NONE;						// エフェクト種類
		g_aEffect[nCntEffect].fRadius = 0.0f;								// エフェクト半径
		g_aEffect[nCntEffect].nLife = 0;									// エフェクト寿命
		g_aEffect[nCntEffect].bUse = false;									// エフェクト使用していない状態
	}
	g_nCntEffectState = 0;			// 状態を初期化

	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_NAME,
		&g_pTextureEffect
	);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// エフェクトをカウント
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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// エフェクト終了処理
//=========================================================================================================================
void UninitEffect(void)
{
	// テクスチャの破棄
	if (g_pTextureEffect != NULL)
	{// NULL以外の場合
		g_pTextureEffect->Release();	// 開放
		g_pTextureEffect = NULL;		// NULLへ
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{// NULL以外の場合
		g_pVtxBuffEffect->Release();	// 開放
		g_pVtxBuffEffect = NULL;		// NULLへ
	}
}

//=========================================================================================================================
// エフェクト更新処理
//=========================================================================================================================
void UpdateEffect(void)
{
	int nCntEffect;			// エフェクトカウンタ

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// エフェクトをカウント
		if (g_aEffect[nCntEffect].bUse == true)
		{// エフェクトが使用されている場合
			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_BULLET)
			{// 弾エフェクトの場合
				// g_aEffect[nCntEffect].nLife--;
				g_aEffect[nCntEffect].fRadius -= 0.8f;

				if (/*g_aEffect[nCntEffect].nLife <= 0 || */g_aEffect[nCntEffect].fRadius <= 0)
				{// ライフが0以下の場合
					g_aEffect[nCntEffect].bUse = false;
				}
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_EXPLOSION || g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTION)
			{// 爆発エフェクトの場合
				g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;	// 移動値代入

				g_aEffect[nCntEffect].nLife--;				// 寿命減少
			
				// 半径減少
				if (g_aEffect[nCntEffect].nLife > 0) 
				{// ライフがある
					g_aEffect[nCntEffect].fRadius -= 0.05f;	// 小さくする
				}
				else
				{// ライフがなくなった
					g_aEffect[nCntEffect].fRadius -= 0.5f;	// 小さくする
				}

				if (g_aEffect[nCntEffect].fRadius <= 0)
				{// ライフが0以下の場合
					g_aEffect[nCntEffect].bUse = false;
				}
			}
		}

		// 頂点設定
		pVtx[nCntEffect * 4].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// エフェクト描画処理
//=========================================================================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスを取得
	int nCntEffect;		// エフェクトカウンタ

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureEffect);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ポリゴンの描画
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// エフェクトをカウント
		if (g_aEffect[nCntEffect].bUse == true)
		{// エフェクトが使用されている場合
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=========================================================================================================================
// 弾エフェクト設定処理
//=========================================================================================================================
void SetBulletEffect(D3DXVECTOR3 pos, D3DXCOLOR col, EFFECTTYPE Type, float fRadius, int nLife)
{
	int nCntEffect;			// エフェクトカウンタ

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// エフェクトをカウント
		if (g_aEffect[nCntEffect].bUse == false)
		{// エフェクトが使用されていない
			g_aEffect[nCntEffect].pos		= pos;			// pos値代入
			g_aEffect[nCntEffect].col		= col;			// col値代入
			g_aEffect[nCntEffect].Type		= Type;			// 種類設定
			g_aEffect[nCntEffect].fRadius	= fRadius;		// 半径設定
			g_aEffect[nCntEffect].nLife		= nLife;		// 寿命設定
			g_aEffect[nCntEffect].bUse		= true;			// 使用している状態

			// 頂点設定
			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			// 頂点カラー設定
			pVtx[nCntEffect * 4].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 1].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 2].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 3].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);

			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// パーティクルエフェクト設定処理
//=========================================================================================================================
void SetParticleEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife)
{
	int nCntEffect;			// エフェクトカウンタ
	int nCount = 0;			// エフェクト個数カウンタ

	VERTEX_2D *pVtx;		// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// エフェクトをカウント
		if (g_aEffect[nCntEffect].bUse == false)
		{// エフェクトが使用されていない
			g_aEffect[nCntEffect].pos = pos;			// pos値代入
			g_aEffect[nCntEffect].Type = Type;			// 種類設定
			g_aEffect[nCntEffect].nLife = nLife;		// 寿命設定
			g_aEffect[nCntEffect].bUse = true;			// 使用している状態

			// 移動ランダム化
			g_aEffect[nCntEffect].move.x = sinf((rand() % 628) / 100.0f) * ((rand() % 3) + 1);
			g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 100.0f) * ((rand() % 3) + 1);

			int nNumber = rand() % 3 + 1;

			// col値代入
			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_EXPLOSION)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 0.5f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f); }
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTION)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.8f, 0.8f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 0.5f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f); }
			}

			g_aEffect[nCntEffect].fRadius = (rand() % nRadius + 1) * 1.0f;	// 半径設定

			// 頂点設定
			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			// 頂点カラー設定
			pVtx[nCntEffect * 4].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 1].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 2].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 3].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);

			nCount++;		// 個数をカウント
			if (nCount == nSetMax)
			{// 個数が最大数になった場合
				break;
			}
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}