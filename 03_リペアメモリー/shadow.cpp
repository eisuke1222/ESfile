//=============================================================================
//
// 影処理 [shadow.cpp]
// Author : 佐藤安純 Sato_Asumi
//
//=============================================================================
#include "shadow.h"
#include "game.h"
#include "xobject.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTURE_NAME	"data\\TEXTURE\\shadow000.jpg"	//読みこむテクスチャファイル名
#define MAX_SHADOW		(100)							//影の最大数
#define COLOR_A			(0.6f)							//カラーの a値
#define CHANGE_COL		(0.0015f)						//色の変化量

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	// テクスチャへのポインタ
D3DXVECTOR2				g_texShadow;				//テクスチャ
Shadow					g_shadow[MAX_SHADOW];		//影の情報

													//=============================================================================
													// 初期化処理
													//=============================================================================
void InitShadow(void)
{
	//変数宣言
	int nCntShadow;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

													// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME,
		&g_pTextureShadow);

	//情報の初期化
	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[nCntShadow].Otherpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[nCntShadow].color.a = COLOR_A;
		g_shadow[nCntShadow].bUse = false;
		g_shadow[nCntShadow].fHight = 0.0f;
		g_shadow[nCntShadow].fWhidth = 0.0f;
		g_shadow[nCntShadow].fLength = 0.0f;
	}

	// 頂点情報の作成
	MakeVertexShadow(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{
	//テクスチャの開放
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	//変数宣言
	int nCntShadow;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX		  mtxRot, mtxTrans;			// 計算用マトリックス

												// αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_shadow[nCntShadow].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_shadow[nCntShadow].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_shadow[nCntShadow].rot.y, g_shadow[nCntShadow].rot.x, g_shadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_shadow[nCntShadow].mtxWorld, &g_shadow[nCntShadow].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_shadow[nCntShadow].pos.x, g_shadow[nCntShadow].pos.y, g_shadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_shadow[nCntShadow].mtxWorld, &g_shadow[nCntShadow].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_shadow[nCntShadow].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntShadow * 4,
				2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	//変数宣言
	int nCntShadow;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, COLOR_A);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, COLOR_A);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, COLOR_A);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, COLOR_A);

		//テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//該当の位置まで進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// 影の設定
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHight, float fWhidth)
{
	//変数宣言
	int		nCntShadow;
	int		nNumShadow = 0;	//影の番号保存
	float	fAngle;			//角度の設定
	VERTEX_3D * pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_shadow[nCntShadow].bUse == false)
		{
			g_shadow[nCntShadow].pos = pos;			//座標を設定
			g_shadow[nCntShadow].fHight = fHight;	//高さを設定
			g_shadow[nCntShadow].fWhidth = fWhidth;	//幅を設定

													//対角線の角度
			fAngle = (float)atan2(g_shadow[nCntShadow].fWhidth, g_shadow[nCntShadow].fHight);

			//辺の長さ
			g_shadow[nCntShadow].fLength = sqrtf((g_shadow[nCntShadow].fWhidth * g_shadow[nCntShadow].fWhidth) + (g_shadow[nCntShadow].fHight * g_shadow[nCntShadow].fHight));

			//頂点を求める
			g_shadow[nCntShadow].Otherpos.x = sinf(fAngle) * g_shadow[nCntShadow].fLength;
			g_shadow[nCntShadow].Otherpos.z = cosf(fAngle) * g_shadow[nCntShadow].fLength;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_shadow[nCntShadow].Otherpos.x, 0.0f, g_shadow[nCntShadow].Otherpos.z);
			pVtx[1].pos = D3DXVECTOR3(g_shadow[nCntShadow].Otherpos.x, 0.0f, g_shadow[nCntShadow].Otherpos.z);
			pVtx[2].pos = D3DXVECTOR3(-g_shadow[nCntShadow].Otherpos.x, 0.0f, -g_shadow[nCntShadow].Otherpos.z);
			pVtx[3].pos = D3DXVECTOR3(g_shadow[nCntShadow].Otherpos.x, 0.0f, -g_shadow[nCntShadow].Otherpos.z);

			g_shadow[nCntShadow].bUse = true;		//使用している状態にする
			nNumShadow = nCntShadow;				//影の番号保存
			break;
		}

		//該当の位置まで進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	return nNumShadow;
}

//=============================================================================
// 影の位置を設定
//=============================================================================
void SetPostionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 OldPos)
{
	QUESTSTATE pQuestState = GetQuestState();
	QUESTTYPE pQuestType = GetQuestType();

	VERTEX_3D * pVtx;		//頂点情報へのポインタ

	if (g_shadow[nIdxShadow].bUse == true)
	{
		//影の位置更新
		g_shadow[nIdxShadow].pos = D3DXVECTOR3(pos.x, 0.0f, pos.z);

		if (pQuestState == QUESTSTATE_PUZZLE)
		{
			if (pQuestType == QUESTTYPE_FIRST)
			{
				if (-452.8f - 150.0f < g_shadow[nIdxShadow].pos.x && 430.3f - 150.0f  > g_shadow[nIdxShadow].pos.x &&
					-531.6f < g_shadow[nIdxShadow].pos.z && 531.4f > g_shadow[nIdxShadow].pos.z)
				{
					g_shadow[nIdxShadow].pos.y = 70.0f;
				}
			}
			else if (pQuestType == QUESTTYPE_SECOND)
			{
				if (-452.8f - 150.0f < g_shadow[nIdxShadow].pos.x && 430.3f - 150.0f  > g_shadow[nIdxShadow].pos.x &&
					-531.6f < g_shadow[nIdxShadow].pos.z && 531.4f > g_shadow[nIdxShadow].pos.z)
				{
					g_shadow[nIdxShadow].pos.y = 70.0f;
				}
			}
			else if (pQuestType == QUESTTYPE_THIRD)
			{
				if (-452.8f - 150.0f < g_shadow[nIdxShadow].pos.x && 430.3f - 150.0f  > g_shadow[nIdxShadow].pos.x &&
					-531.6f < g_shadow[nIdxShadow].pos.z && 531.4f > g_shadow[nIdxShadow].pos.z)
				{
					g_shadow[nIdxShadow].pos.y = 70.0f;
				}
			}
		}

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		//現在の位置が前回の位置よりも高かったとき
		if (pos.y > OldPos.y)
		{
			if (g_shadow[nIdxShadow].color.a > 0.0f)
			{
				g_shadow[nIdxShadow].color.a -= CHANGE_COL;		//透明度を下げる
			}
		}

		//現在の位置が前回の位置よりも低かったとき
		if (pos.y < OldPos.y)
		{
			if (g_shadow[nIdxShadow].color.a < COLOR_A)
			{
				g_shadow[nIdxShadow].color.a += CHANGE_COL;		//透明度を上げる
			}
		}

		//該当の位置まで進める
		pVtx += 4 * nIdxShadow;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_shadow[nIdxShadow].color.a);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_shadow[nIdxShadow].color.a);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_shadow[nIdxShadow].color.a);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_shadow[nIdxShadow].color.a);

		//頂点バッファをアンロックする
		g_pVtxBuffShadow->Unlock();
	}
}

//=============================================================================
// 影を消す処理
//=============================================================================
void DeleteShadow(void)
{
	//変数宣言
	int		nCntShadow;

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_shadow[nCntShadow].bUse == true)
		{
			g_shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_shadow[nCntShadow].Otherpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_shadow[nCntShadow].color.a = COLOR_A;
			g_shadow[nCntShadow].bUse = false;
			g_shadow[nCntShadow].fHight = 0.0f;
			g_shadow[nCntShadow].fWhidth = 0.0f;
			g_shadow[nCntShadow].fLength = 0.0f;
		}
	}
}
void DeleteShadow(int nIdxShadow)
{
	g_shadow[nIdxShadow].bUse = false;
}