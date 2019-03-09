//=========================================================================================================================
// 
// エフェクト処理 [effect.cpp]
// Author : Yamashita
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "effect.h"		// エフェクト
#include "input.h"		// エフェクト

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	MAX_EFFECT					(10000)									// エフェクトの最大数
#define	MAX_EFFECTSMOKE				(50)									// エフェクトの最大数(煙)
#define	MAX_EFFECTRAIN				(500)									// エフェクトの最大数（雨）
#define	MAX_EFFECTMAGIC				(500)									// エフェクトの最大数（雨）
#define	MAX_EFFECTEMOTION			(50)									// エフェクトの最大数（雨）

#define	TEXTURE_EFFECT_NAME		"data/TEXTURE/effect000.jpg"				// 読み込むテクスチャファイル名 /噴水
#define	TEXTURE_EFFECT_NAME1	"data/TEXTURE/flower.png"		// 読み込むテクスチャファイル名 /花
#define	TEXTURE_EFFECT_NAME2	"data/TEXTURE/Angry.png"					// 読み込むテクスチャファイル名 /怒り

#define MAX_TEX						(3)

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureEffect[MAX_TEX] = {};				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffEffect = NULL;					// 頂点バッファへのポインタ
EFFECT							g_aEffect[MAX_EFFECT];						// エフェクトの情報
int								g_nCntEffectState;							// 状態カウンタ

//=========================================================================================================================
// エフェクト初期化処理
//=========================================================================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスを取得
	int nCntEffect;		// エフェクトカウンタ
//	float fData;
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// エフェクトをカウント
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// エフェクト中心値
	
		
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// エフェクト中心値

		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// エフェクト移動値
		//g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// エフェクト色
		g_aEffect[nCntEffect].Type = EFFECTTYPE_NONE;						// エフェクト種類
		g_aEffect[nCntEffect].nTypeTx;
		

		g_aEffect[nCntEffect].fRadius = 0.0f;								// エフェクト半径
		g_aEffect[nCntEffect].nLife = 0;									// エフェクト寿命
		g_aEffect[nCntEffect].bUse = false;									// エフェクト使用していない状態

		g_nCntEffectState = 0;			// 状態を初期化

	}
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,TEXTURE_EFFECT_NAME,&g_pTextureEffect[0]);

	// テクスチャの読み込み(煙)
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT_NAME1, &g_pTextureEffect[1]);

	// テクスチャの読み込み(煙)
	D3DXCreateTextureFromFile(pDevice, TEXTURE_EFFECT_NAME2, &g_pTextureEffect[2]);

	
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer
	(sizeof(VERTEX_3D) * 4 * MAX_EFFECT, D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBuffEffect , NULL);
	// 頂点情報の作成
	VERTEX_3D *pVtx;
	
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

		//法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラー設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);

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
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		// テクスチャの破棄
		if (g_pTextureEffect[nCnt] != NULL)
		{
			g_pTextureEffect[nCnt]->Release();
			g_pTextureEffect[nCnt] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=========================================================================================================================
// エフェクト更新処理
//=========================================================================================================================
void UpdateEffect(void)
{
	int nCntEffect;			// エフェクトカウンタ

	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

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
				g_aEffect[nCntEffect].fRadius -= 0.2f;

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
				{
					g_aEffect[nCntEffect].fRadius += 0.005f;
				}
				else
				{
					g_aEffect[nCntEffect].fRadius -= 0.5f;	
				}

				if (g_aEffect[nCntEffect].fRadius <= 0)
				{// ライフが0以下の場合
					g_aEffect[nCntEffect].bUse = false;
				}
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONRAIN)
			{// 爆発エフェクトの場合
				g_aEffect[nCntEffect].pos -= g_aEffect[nCntEffect].move;	// 移動値代入

				g_aEffect[nCntEffect].nLife++;				// 寿命減少

				// 半径減少
				if (g_aEffect[nCntEffect].nLife > 0)
				{
					g_aEffect[nCntEffect].fRadius -= 0.005f;
				}
				else
				{
					g_aEffect[nCntEffect].fRadius -= 0.5f;
				}

				if (g_aEffect[nCntEffect].nLife > 100)
				{// ライフが0以下の場合
					g_aEffect[nCntEffect].move.x = false;
					g_aEffect[nCntEffect].move.y = false;
					g_aEffect[nCntEffect].move.z = false;
				}
				if (g_aEffect[nCntEffect].nLife > 150)
				{
					/*g_aEffect[nCntEffect].rot.x += 1;
					g_aEffect[nCntEffect].rot.y += 1;
					g_aEffect[nCntEffect].rot.z += 1;*/
					//重力
					/*g_aEffect[nCntEffect].move.y += 1;
					g_aEffect[nCntEffect].move.y -= (0.0f - g_aEffect[nCntEffect].move.y) * 0.5f;
					g_aEffect[nCntEffect].pos.y -= g_aEffect[nCntEffect].move.y; */

					//中心に向かってくる
					g_aEffect[nCntEffect].move.x -= sinf(atan2f(g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.x)) * 1.0f;
					g_aEffect[nCntEffect].move.y -= cosf(atan2f(g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.x)) * 1.0f;

				}
				if (g_aEffect[nCntEffect].nLife == 300)
				{
					g_aEffect[nCntEffect].bUse = false;
				}
				
			}	
			
			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONHAPPY || 
				g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONUSUALLY || 
				g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONANGRY)
			{// 爆発エフェクトの場合
				g_aEffect[nCntEffect].pos -= g_aEffect[nCntEffect].move;	// 移動値代入

				g_aEffect[nCntEffect].nLife--;				// 寿命減少
				//g_aEffect[nCntEffect].rot.z += 0.02f;
				
				// 半径減少
				if (g_aEffect[nCntEffect].nLife > 0)
				{
					g_aEffect[nCntEffect].fRadius += 0.005f;
				}
				else
				{
					g_aEffect[nCntEffect].fRadius -= 0.2f;
				}
				

				if (g_aEffect[nCntEffect].fRadius <= 0)
				{// ライフが0以下の場合
					g_aEffect[nCntEffect].bUse = false;
				}
			}

			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONRIPPLE)
			{// 爆発エフェクトの場合
				g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;	// 移動値代入

				g_aEffect[nCntEffect].nLife--;				// 寿命減少
				//g_aEffect[nCntEffect].nor = g_aEffect[nCntEffect].move;
				//g_aEffect[nCntEffect].rot.z += 0.02f;

				// 半径減少
				if (g_aEffect[nCntEffect].nLife > 0)
				{
					g_aEffect[nCntEffect].fRadius += 0.005f;
				}
				else
				{
					g_aEffect[nCntEffect].fRadius += 0.5f;
				}


				if (g_aEffect[nCntEffect].fRadius <= 40)
				{// ライフが0以下の場合
					g_aEffect[nCntEffect].bUse = false;
				}
			}

			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONSAD)
			{// 悲しいエフェクトの場合
				//g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;	// 移動値代入

				//g_aEffect[nCntEffect].nLife++;				// 寿命減少
															//g_aEffect[nCntEffect].nor = g_aEffect[nCntEffect].move;
				g_aEffect[nCntEffect].rot.z += 0.1f;

															// 半径減少
				/*if (g_aEffect[nCntEffect].nLife > 0)
				{
					g_aEffect[nCntEffect].fRadius ;
				}
				else
				{
					g_aEffect[nCntEffect].fRadius;
				}*/

				//if (g_aEffect[nCntEffect].nLife > 60)
				{// ライフが0以下の場合
					/*g_aEffect[nCntEffect].move.x = false;
					g_aEffect[nCntEffect].move.y = false;
					g_aEffect[nCntEffect].move.z = false;*/
				}

				//if (g_aEffect[nCntEffect].nLife == 75)
				{// ライフが0以下の場合
					//g_aEffect[nCntEffect].bUse = false;
				}
			}
			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONSTAMP)
			{// スタンプ
			// g_aEffect[nCntEffect].pos.z += g_aEffect[nCntEffect].move.z;	// 移動値代入
			// g_aEffect[nCntEffect].move.y -= 0.05;

			 g_aEffect[nCntEffect].nLife++;				// 寿命減少
			 g_aEffect[nCntEffect].col.a += 0.02f;		//透明度	

				// 半径減少
				
				
			 if (g_aEffect[nCntEffect].nLife < 2)
			 {
				// g_aEffect[nCntEffect].move.z += 10.0f;
				 g_aEffect[nCntEffect].fRadius -= 1.0f;

			 }
				if (g_aEffect[nCntEffect].nLife > 60)
				{// ライフが0以下の場合
				 g_aEffect[nCntEffect].move.z = false;
				}

				if (g_aEffect[nCntEffect].nLife == 75)
				{// ライフが0以下の場合
				 g_aEffect[nCntEffect].bUse = false;
				}
			}
			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONWATER)
			{// 爆発エフェクトの場合
				g_aEffect[nCntEffect].move.y -= 0.05f;

				g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;	// 移動値代入

				g_aEffect[nCntEffect].nLife++;				// 寿命減少

															// 半径減少
				if (g_aEffect[nCntEffect].nLife > 0)
				{
					g_aEffect[nCntEffect].fRadius += 0.005f;
				}
				else
				{
					g_aEffect[nCntEffect].fRadius -= 0.5f;
				}
				if (g_aEffect[nCntEffect].nLife > 30)
				{// ライフが0以下の場合
					//SetRainEffect(D3DXVECTOR3(0, 0, 100), EFFECTTYPE_RESURRECTIONWATER, 1, 10, 0, 2);	//個数:半径：HP：テクスチャ番号
				}
				if (g_aEffect[nCntEffect].nLife > 100)
				{// ライフが0以下の場合
					
					//g_aEffect[nCntEffect].rot += g_aEffect[nCntEffect].pos;	// 移動値代入

					g_aEffect[nCntEffect].bUse = false;

					//SetRainEffect(D3DXVECTOR3(0, 0, 100), EFFECTTYPE_RESURRECTIONWATER, 5, 10, 20, 2);	//個数:半径：HP：テクスチャ番号
				}
			}
			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONWATER_1)
			{// 爆発エフェクトの場合
				g_aEffect[nCntEffect].move.y += 0.002f;	// 移動値代入
				g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;	// 移動値代入

				g_aEffect[nCntEffect].nLife++;				// 寿命減少

				// 半径減少
				if (g_aEffect[nCntEffect].nLife > 20)
				{
					g_aEffect[nCntEffect].fRadius -= 0.4f;
				}

				if (g_aEffect[nCntEffect].fRadius < 0.0f)
				{// 
					g_aEffect[nCntEffect].bUse = false;
				}
			}
		}
		// 頂点設定
		pVtx[nCntEffect * 4].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius,g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius,  -g_aEffect[nCntEffect].fRadius, 0.0f);

		// 頂点設定
		pVtx[nCntEffect * 4].col = g_aEffect[nCntEffect].col;
		pVtx[nCntEffect * 4 + 1].col = g_aEffect[nCntEffect].col;
		pVtx[nCntEffect * 4 + 2].col = g_aEffect[nCntEffect].col;
		pVtx[nCntEffect * 4 + 3].col = g_aEffect[nCntEffect].col;
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

	D3DXMATRIX mtxRot, mtxTrans, mtxView;		//計算用マトリックス
	//LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCntEffect].mtxWorld);

			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			g_aEffect[nCntEffect].mtxWorld._11 = mtxView._11;
			g_aEffect[nCntEffect].mtxWorld._12 = mtxView._21;
			g_aEffect[nCntEffect].mtxWorld._13 = mtxView._31;
			g_aEffect[nCntEffect].mtxWorld._21 = mtxView._12;
			g_aEffect[nCntEffect].mtxWorld._22 = mtxView._22;
			g_aEffect[nCntEffect].mtxWorld._23 = mtxView._32;
			g_aEffect[nCntEffect].mtxWorld._31 = mtxView._13;
			g_aEffect[nCntEffect].mtxWorld._32 = mtxView._23;
			g_aEffect[nCntEffect].mtxWorld._33 = mtxView._33;

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEffect[nCntEffect].rot.y, g_aEffect[nCntEffect].rot.x, g_aEffect[nCntEffect].rot.z);

			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxRot);						//保存

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCntEffect].pos.x, g_aEffect[nCntEffect].pos.y, g_aEffect[nCntEffect].pos.z);

			D3DXMatrixMultiply(&g_aEffect[nCntEffect].mtxWorld, &g_aEffect[nCntEffect].mtxWorld, &mtxTrans);						//保存

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCntEffect].mtxWorld);

			//アルファテスト
			/*pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);						//基準値
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);		//比較方法（より大きい））
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DCMP_ALWAYS);		//比較方法（より大きい））*/
			//pDevice->SetRenderState(D3DRS_ZENABLE, true);

			//デフォルトの場合FALSEになっているからTRUEにする
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//比較方法（より大きい））
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTION || g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONWATER || g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONWATER_1)
			{
				// αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);


			// ポリゴンの描画

			// エフェクトが使用されている場合
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect[g_aEffect[nCntEffect].nTypeTx]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);


			//アルファ値を戻す
			//->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

			//デフォルトの場合FALSEになっているからTRUEにするc





			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//比較方法（より大きい））
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTION || g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONWATER || g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONWATER_1)
			{
				// αブレンディングを元に戻す
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
		}
	}
}

//=========================================================================================================================
// 弾エフェクト設定処理
//=========================================================================================================================
void SetBulletEffect(D3DXVECTOR3 pos, D3DXCOLOR col, EFFECTTYPE Type, float fRadius, int nLife, int nTypeTx)
{
	int nCntEffect;			// エフェクトカウンタ

	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

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
			g_aEffect[nCntEffect].col = col;			// col値代入
			g_aEffect[nCntEffect].Type = Type;			// 種類設定
			g_aEffect[nCntEffect].nTypeTx = nTypeTx;			// 種類設定

			g_aEffect[nCntEffect].fRadius = fRadius;	// 半径設定
			g_aEffect[nCntEffect].nLife = nLife;		// 寿命設定
			g_aEffect[nCntEffect].bUse = true;			// 使用している状態

			// 頂点設定
			/*pVtx[nCntEffect * 4].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);*/
			// 頂点カラー設定
			pVtx[nCntEffect * 4].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 1].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 2].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 3].col = g_aEffect[nCntEffect].col;

			////法線ベクトル
			//pVtx[nCntEffect * 4].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			//pVtx[nCntEffect * 4 + 1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			//pVtx[nCntEffect * 4 + 2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			//pVtx[nCntEffect * 4 + 3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			break;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// パーティクルエフェクト設定処理
//=========================================================================================================================
void SetParticleEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife ,int nTypeTx)
{
	int nCntEffect;			// エフェクトカウンタ
	int nCount = 0;			// エフェクト個数カウンタ

	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

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
			g_aEffect[nCntEffect].nTypeTx = nTypeTx;			// 種類設定

			g_aEffect[nCntEffect].bUse = true;			// 使用している状態

			// 移動ランダム化
			g_aEffect[nCntEffect].move.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			//g_aEffect[nCntEffect].move.z = tanf((rand() % 628) / 100.0f) * ((rand() % 3) + 1);

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
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.7f, 0.9f, 0.0f, 0.7f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.6f, 0.8f, 0.0f, 0.6f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.5f, 0.7f, 0.0f, 0.5f); }
			}

			g_aEffect[nCntEffect].fRadius = (rand() % nRadius + 1) * 1.0f;	// 半径設定

			// 頂点設定
			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius,  -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);

			// 頂点カラー設定
			pVtx[nCntEffect * 4].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 1].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 2].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 3].col = g_aEffect[nCntEffect].col;

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

//=========================================================================================================================
// パーティクルエフェクト設定処理
//=========================================================================================================================
void SetSmokeEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx)
{
	int nCntEffect;			// エフェクトカウンタ 
	int nCount = 0;			// エフェクト個数カウンタ

	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

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
			g_aEffect[nCntEffect].nTypeTx = nTypeTx;	// 種類設定

			g_aEffect[nCntEffect].bUse = true;			// 使用している状態


			// 移動ランダム化
			g_aEffect[nCntEffect].move.x = sinf((rand() % 25) / 700.0f) * ((rand() % 1) + 1);
			g_aEffect[nCntEffect].move.y = cosf((rand() % 100) / 100.0f)  * ((rand() % 1) + 1);
			g_aEffect[nCntEffect].rot.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			g_aEffect[nCntEffect].rot.y = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);

			/*g_aEffect[nCntEffect].move.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 100.0f)  * ((rand() % 1) + 1);*/
			//g_aEffect[nCntEffect].move.z = tanf((rand() % 628) / 100.0f) * ((rand() % 3) + 1);

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
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f); }
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONRAIN)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
			}

			g_aEffect[nCntEffect].fRadius = (rand() % nRadius + 1) * 1.0f;	// 半径設定

			// 頂点設定
			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);

			// 頂点カラー設定
			pVtx[nCntEffect * 4].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 1].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 2].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 3].col = g_aEffect[nCntEffect].col;

			nCount++;		// 個数をカウント
			if (nCount == nSetMax)
			{// 個数が最大数になった場合
				//g_aEffect[nCntEffect].bUse = false;
				nSetMax = 0;
				break;
			}
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// パーティクルエフェクト設定処理
//=========================================================================================================================
void SetRainEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx)
{
		int nCntEffect;			// エフェクトカウンタ
		int nCount = 0;			// エフェクト個数カウンタ

		VERTEX_3D *pVtx;		// 頂点情報へのポインタ

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
				g_aEffect[nCntEffect].nTypeTx = nTypeTx;			// 種類設定

				g_aEffect[nCntEffect].bUse = true;			// 使用している状態
				//g_aEffect[nCntEffect].move.y += 500.0f;

				if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONRAIN)
				{
					// 移動ランダム化											
					g_aEffect[nCntEffect].move.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
					g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 100.0f)  * ((rand() % 1) + 1);
					g_aEffect[nCntEffect].move.z = cosf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);

					/*g_aEffect[nCntEffect].rot.y = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
					g_aEffect[nCntEffect].rot.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
					g_aEffect[nCntEffect].rot.z = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);*/
				}
				if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONWATER || g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONWATER_1)
				{
					// 移動ランダム化											
					//g_aEffect[nCntEffect].move.x = sinf((rand() % 25) / 700.0f) * ((rand() % 1) + 1);
					g_aEffect[nCntEffect].move.x = sinf((rand() % 500) / 100.0f) * 0.8f;
					g_aEffect[nCntEffect].move.y = cosf((rand() % 100) / 100.0f)  * 3.0f;
					g_aEffect[nCntEffect].move.z = sinf((rand() % 500) / 100.0f) * 0.8f;
				}
				int nNumber = rand() % 3 + 1;

				// col値代入
				if (g_aEffect[nCntEffect].Type == EFFECTTYPE_EXPLOSION)
				{
					if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.8f); }
					if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f); }
					if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.3f); }
				}
				else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTION)
				{
					if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f); }
					if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f); }
					if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f); }
				}
				else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONRAIN)
				{
					if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f); }
					if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f); }
					if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f); }
				}
				else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONWATER)
				{
					if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.7f, 0.8f, 0.7f); }
					if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.7f, 0.8f, 0.5f); }
					if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.7f, 0.8f, 0.3f); }
				}
				else
				{
					if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
					if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
					if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
				}

				g_aEffect[nCntEffect].fRadius = (rand() % nRadius + 1) * 1.0f;	// 半径設定

				// 頂点設定
				pVtx[nCntEffect * 4].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
				pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);

				// 頂点カラー設定
				pVtx[nCntEffect * 4].col = g_aEffect[nCntEffect].col;
				pVtx[nCntEffect * 4 + 1].col = g_aEffect[nCntEffect].col;
				pVtx[nCntEffect * 4 + 2].col = g_aEffect[nCntEffect].col;
				pVtx[nCntEffect * 4 + 3].col = g_aEffect[nCntEffect].col;

				nCount++;		// 個数をカウント
				if (nCount == nSetMax)
				{// 個数が最大数になった場合
					nSetMax = 0;
					break;
				}
			}
		}
		// 頂点バッファをアンロック
		g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// パーティクルエフェクト設定処理
//=========================================================================================================================
void SetMagicEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx)
{
	int nCntEffect;			// エフェクトカウンタ
	int nCount = 0;			// エフェクト個数カウンタ

	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

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
		{// エフェクトが使用されていないR

			g_aEffect[nCntEffect].pos = pos;			// pos値代入
			g_aEffect[nCntEffect].Type = Type;			// 種類設定
			g_aEffect[nCntEffect].nLife = nLife;		// 寿命設定
			g_aEffect[nCntEffect].nTypeTx = nTypeTx;			// 種類設定

			g_aEffect[nCntEffect].bUse = true;			// 使用している状態

														// 移動ランダム化											
			 g_aEffect[nCntEffect].move.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			 g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			 g_aEffect[nCntEffect].move.z = cosf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);

			 g_aEffect[nCntEffect].rot.y = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			 g_aEffect[nCntEffect].rot.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			 g_aEffect[nCntEffect].rot.z = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);

			// g_aEffect[nCntEffect].move.z = cosf((rand() % 100) / 100.0f) * ((rand() % 1) + 1);

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
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 0.7f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 0.6f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.7f, 1.0f, 0.0f, 0.5f); }
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONRAIN)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f); }
			}

			g_aEffect[nCntEffect].fRadius = (rand() % nRadius + 1) * 1.0f;	// 半径設定

																			// 頂点設定
			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);

			// 頂点カラー設定
			pVtx[nCntEffect * 4].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 1].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 2].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 3].col = g_aEffect[nCntEffect].col;

			nCount++;		// 個数をカウント
			if (nCount == nSetMax)
			{// 個数が最大数になった場合
				//g_aEffect[nCntEffect].bUse = false;
				nSetMax = 0;
				break;
			}
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// 感情パーティクルエフェクト設定処理
//=========================================================================================================================
void SetEmotionEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx)		// 感情エフェクト設定処理
{
	int nCntEffect;			// エフェクトカウンタ
	int nCount = 0;			// エフェクト個数カウンタ

	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

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
			g_aEffect[nCntEffect].nTypeTx = nTypeTx;			// 種類設定
			


			g_aEffect[nCntEffect].bUse = true;			// 使用している状態

			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONHAPPY)
			{
				// 移動ランダム化											
				g_aEffect[nCntEffect].move.x = sinf((rand() % 628) / 100.0f) * 0.5f;
				g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 100.0f)  * 0.5f;

				//g_aEffect[nCntEffect].move.z = cosf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONUSUALLY)
			{
				// 移動ランダム化											
				g_aEffect[nCntEffect].move.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
				g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 100.0f)  * ((rand() % 1) + 1);
				//g_aEffect[nCntEffect].move.z = cosf((rand() % 500) / 100.0f) * ((rand() % 1) + 1);
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONSAD)
			{
				// 移動ランダム化											
				//g_aEffect[nCntEffect].move.x = sinf((rand() % 100) / 10.0f) * ((rand() % 1) + 0.5f);
				//g_aEffect[nCntEffect].move.y = cosf((rand() % 100) / 200.0f)  * ((rand() % 1) + 0.5f);
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONANGRY)
			{
				// 移動ランダム化											
				g_aEffect[nCntEffect].move.x = sinf((rand() % 628) / 100.0f) * 0.7f;
				g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 100.0f)  * 0.7f;
				//g_aEffect[nCntEffect].rot.z = cosf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONSTAMP)
			{
				// 移動ランダム化											
				//g_aEffect[nCntEffect].move.z = 1;

				//g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 100.0f)  * 0.7f;
				//g_aEffect[nCntEffect].rot.z = cosf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			}
			//g_aEffect[nCntEffect].rot.y = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			//g_aEffect[nCntEffect].rot.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			//g_aEffect[nCntEffect].rot.z = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);

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
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.7f, 0.9f, 0.0f, 0.7f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.6f, 0.8f, 0.0f, 0.6f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.5f, 0.7f, 0.0f, 0.5f); }
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONHAPPY)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 1.0f, 0.3f, 1.0f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f); }
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONSAD)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONANGRY)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); }
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONSTAMP)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f); }
			}

			g_aEffect[nCntEffect].fRadius = (7) * 2.0f;	// 半径設定

																			// 頂点設定
			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);

			// 頂点カラー設定
			pVtx[nCntEffect * 4].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 1].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 2].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 3].col = g_aEffect[nCntEffect].col;

			nCount++;		// 個数をカウント
			if (nCount == nSetMax)
			{// 個数が最大数になった場合
				nSetMax = 0;
				break;
			}
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// 波紋パーティクルエフェクト設定処理
//=========================================================================================================================
void SetRippleEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife, int nTypeTx)
{
	int nCntEffect;			// エフェクトカウンタ
	int nCount = 0;			// エフェクト個数カウンタ

	VERTEX_3D *pVtx;		// 頂点情報へのポインタ

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
			g_aEffect[nCntEffect].nTypeTx = nTypeTx;	// 種類設定

			g_aEffect[nCntEffect].bUse = true;			// 使用している状態

			// 移動ランダム化											
			g_aEffect[nCntEffect].move.x = sinf((rand() % 628) / 1.0f) * 0.1f;
			g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 1.0f)  * 0.1f;


			//g_aEffect[nCntEffect].rot.y = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			//g_aEffect[nCntEffect].rot.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			//g_aEffect[nCntEffect].rot.z = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);

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
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.7f, 0.9f, 0.0f, 0.7f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.6f, 0.8f, 0.0f, 0.6f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.5f, 0.7f, 0.0f, 0.5f); }
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTIONRIPPLE)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f); }
			}


			g_aEffect[nCntEffect].fRadius = (rand() % nRadius + 1) * 1.0f;	// 半径設定

			// 頂点設定
			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, -g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].fRadius, 0.0f);

			// 頂点カラー設定
			pVtx[nCntEffect * 4].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 1].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 2].col = g_aEffect[nCntEffect].col;
			pVtx[nCntEffect * 4 + 3].col = g_aEffect[nCntEffect].col;

			nCount++;		// 個数をカウント
			if (nCount == nSetMax)
			{// 個数が最大数になった場合
				//nSetMax = 0;
				break;
			}
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// エフェクト破棄処理
//=========================================================================================================================
void DeleteEffect(void)
{
	int nCntEffect;			// エフェクトカウンタ

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// エフェクトをカウント
		if (g_aEffect[nCntEffect].bUse == true)
		{// エフェクトが使用されていない
			g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// エフェクト中心値
			g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// エフェクト中心値
			g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// エフェクト移動値																				
			//g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// エフェクト色
			g_aEffect[nCntEffect].Type = EFFECTTYPE_NONE;						// エフェクト種類
			g_aEffect[nCntEffect].nTypeTx;
			g_aEffect[nCntEffect].fRadius = 0.0f;								// エフェクト半径
			g_aEffect[nCntEffect].nLife = 0;									// エフェクト寿命
			g_aEffect[nCntEffect].bUse = false;									// エフェクト使用していない状態
		}
	}
	
	g_nCntEffectState = 0;			// 状態を初期化
}