//=========================================================================================================================
//
// 円柱処理 [meshcylinder.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanfエラー解除
#include <stdio.h>					// stdio
#include "meshcylinder.h"
#include "input.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MESHCYLINDER_TEXT_INFO			"data\\TEXT\\meshcylinderdata.txt"		// 読み込むデータファイル
#define MAX_MESHCYLINDER				(32)									// 円柱の最大数
#define MAX_TEXTURE_MESHCYLINDER		(2)										// テクスチャの最大数
#define MESHCYLINDER_TEXTURE_NAME1		"data\\TEXTURE\\sky001.jpg"				// 読み込むテクスチャファイル1
#define MESHCYLINDER_TEXTURE_NAME2		"data\\TEXTURE\\mountain000.png"		// 読み込むテクスチャファイル2
#define MESHCYLINDER_TEXTURE_UV_U		(1.0f)									// テクスチャアニメーションU範囲
#define MESHCYLINDER_TEXTURE_UV_V		(1.0f)									// テクスチャアニメーションV範囲
#define MESHCYLINDER_MOVE_ROT			(0.0002f)								// 回転量

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice);							// 頂点情報の設定

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureMeshCylinder[MAX_TEXTURE_MESHCYLINDER] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffMeshCylinder = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9			g_pIdxBuffMeshCylinder = NULL;					// インデックスバッファへのポインタ
MeshCylinder					g_MeshCylinder[MAX_MESHCYLINDER];				// 円柱の情報
int								g_MaxMeshCylinder;								// 読み込むモデルの最大数
int								g_MaxVtxMeshCylinder;							// 頂点の最大数
int								g_MaxIdxMeshCylinder;							// インデックスの最大数
int								g_MaxPolygonMeshCylinder;						// ポリゴンの最大数

//=========================================================================================================================
// 円柱の初期化処理
//=========================================================================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntMeshCylinder;
	FILE *pFile;

	g_MaxMeshCylinder = 0;
	g_MaxPolygonMeshCylinder = 0;
	g_MaxVtxMeshCylinder = 0;

	// ファイル設定
	pFile = fopen(MESHCYLINDER_TEXT_INFO, "r");

	// 円柱テキストデータ読み込み
	if (pFile != NULL)
	{// ファイルが開けた場合
		fscanf(pFile, "%d", &g_MaxMeshCylinder);	// 読み込み

		for (nCntMeshCylinder = 0; nCntMeshCylinder < g_MaxMeshCylinder; nCntMeshCylinder++)
		{
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].pos.x);		// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].pos.y);		// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].pos.z);		// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].rot.x);		// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].rot.y);		// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].rot.z);		// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].num.x);		// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].num.y);		// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].num.z);		// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].fRadius.x);	// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].fRadius.y);	// 読み込み
			fscanf(pFile, "%f", &g_MeshCylinder[nCntMeshCylinder].fRadius.z);	// 読み込み
		}

		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}

	for (nCntMeshCylinder = 0; nCntMeshCylinder < g_MaxMeshCylinder; nCntMeshCylinder++)
	{
		// 頂点、インデックス、ポリゴン数を計算
		g_MeshCylinder[nCntMeshCylinder].MaxVtx = ((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1) * ((int)g_MeshCylinder[nCntMeshCylinder].num.y + 1);
		g_MeshCylinder[nCntMeshCylinder].MaxIdx = (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1) * 2) * (int)g_MeshCylinder[nCntMeshCylinder].num.y + (((int)g_MeshCylinder[nCntMeshCylinder].num.y - 1) * 2);
		g_MeshCylinder[nCntMeshCylinder].MaxPolygon = ((int)g_MeshCylinder[nCntMeshCylinder].num.x * (int)g_MeshCylinder[nCntMeshCylinder].num.y * 2) + (((int)g_MeshCylinder[nCntMeshCylinder].num.y - 1) * 4);
		g_MeshCylinder[nCntMeshCylinder].bUse = false;

		// 頂点、インデックス、ポリゴン数を加算
		g_MaxVtxMeshCylinder += g_MeshCylinder[nCntMeshCylinder].MaxVtx;
		g_MaxIdxMeshCylinder += g_MeshCylinder[nCntMeshCylinder].MaxIdx;
		g_MaxPolygonMeshCylinder += g_MeshCylinder[nCntMeshCylinder].MaxPolygon;
	}

	// 頂点情報の作成
	MakeVertexMeshCylinder(pDevice);
}

//=========================================================================================================================
// 円柱の終了処理
//=========================================================================================================================
void UninitMeshCylinder(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE_MESHCYLINDER; nCntTex++)
	{
		if (g_pTextureMeshCylinder[nCntTex] != NULL)
		{
			g_pTextureMeshCylinder[nCntTex]->Release();
			g_pTextureMeshCylinder[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	// インデックスバッファの破棄
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//=========================================================================================================================
// 円柱の更新処理
//=========================================================================================================================
void UpdateMeshCylinder(void)
{
	int nCntMeshCylinder;

	// 自動回転
	for (nCntMeshCylinder = 0; nCntMeshCylinder < g_MaxMeshCylinder; nCntMeshCylinder++)
	{
		if (g_MeshCylinder[nCntMeshCylinder].bUse == true)
		{
			if (nCntMeshCylinder == 0)
			{
				g_MeshCylinder[nCntMeshCylinder].rot.y += MESHCYLINDER_MOVE_ROT;
			}
		}
	}
}

//=========================================================================================================================
// 円柱の描画処理
//=========================================================================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	int nCntMeshCylinder, nNumVtx = 0, nNumIdx = 0;

	for (nCntMeshCylinder = 0; nCntMeshCylinder < g_MaxMeshCylinder; nCntMeshCylinder++)
	{
		if (g_MeshCylinder[nCntMeshCylinder].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_MeshCylinder[nCntMeshCylinder].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_MeshCylinder[nCntMeshCylinder].rot.y,
				g_MeshCylinder[nCntMeshCylinder].rot.x,
				g_MeshCylinder[nCntMeshCylinder].rot.z
			);

			D3DXMatrixMultiply
			(
				&g_MeshCylinder[nCntMeshCylinder].mtxWorld,
				&g_MeshCylinder[nCntMeshCylinder].mtxWorld,
				&mtxRot
			);

			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_MeshCylinder[nCntMeshCylinder].pos.x,
				g_MeshCylinder[nCntMeshCylinder].pos.y,
				g_MeshCylinder[nCntMeshCylinder].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_MeshCylinder[nCntMeshCylinder].mtxWorld,
				&g_MeshCylinder[nCntMeshCylinder].mtxWorld,
				&mtxTrans
			);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshCylinder[nCntMeshCylinder].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource
			(
				0,
				g_pVtxBuffMeshCylinder,
				0,
				sizeof(VERTEX_3D)
			);

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshCylinder);

			// 頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャ設定
			pDevice->SetTexture(0, g_pTextureMeshCylinder[g_MeshCylinder[nCntMeshCylinder].nTexNum]);
			//pDevice->SetTexture(0, NULL);

			// 円柱の描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, nNumVtx, nNumIdx, g_MeshCylinder[nCntMeshCylinder].MaxVtx, 0, g_MeshCylinder[nCntMeshCylinder].MaxPolygon);
		}

		nNumVtx += g_MeshCylinder[nCntMeshCylinder].MaxVtx;
		nNumIdx += g_MeshCylinder[nCntMeshCylinder].MaxIdx;
	}
}

//=========================================================================================================================
// 円柱の設定処理
//=========================================================================================================================
void SetMeshCylinder(int nTexNum)
{
	int nCntMeshCylinder, nNumVtx = 0, nNumIdx = 0;
	int nCount_x, nCount_y, nCount_mesh = 0;

	D3DXVECTOR3 vecNor = D3DXVECTOR3(0.0f, 0.0f, 0.0);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	WORD *pIdx;			// インデックスデータへのポインタ

	// インデックスバッファをロック
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshCylinder = 0; nCntMeshCylinder < g_MaxMeshCylinder; nCntMeshCylinder++)
	{// ポリゴンをカウント
		if (g_MeshCylinder[nCntMeshCylinder].bUse == false)
		{// 使用していない場合
			g_MeshCylinder[nCntMeshCylinder].nTexNum = nTexNum;
			g_MeshCylinder[nCntMeshCylinder].bUse = true;				// 使用する

			// 頂点情報の設定
			for (nCount_y = 0; nCount_y < g_MeshCylinder[nCntMeshCylinder].num.y + 1; nCount_y++)
			{// yをカウント
				for (nCount_x = 0; nCount_x < g_MeshCylinder[nCntMeshCylinder].num.x + 1; nCount_x++)
				{// xをカウント
					pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].pos.x = sinf(D3DX_PI + (D3DX_PI * (1.0f / (g_MeshCylinder[nCntMeshCylinder].num.x / 2))) * nCount_x) * g_MeshCylinder[nCntMeshCylinder].fRadius.x;
					pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].pos.y = ((g_MeshCylinder[nCntMeshCylinder].fRadius.y) * g_MeshCylinder[nCntMeshCylinder].num.y) - (g_MeshCylinder[nCntMeshCylinder].fRadius.y * (nCount_y));
					pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].pos.z = cosf(D3DX_PI + (D3DX_PI * (1.0f / (g_MeshCylinder[nCntMeshCylinder].num.x / 2))) * nCount_x) * g_MeshCylinder[nCntMeshCylinder].fRadius.x;

					vecNor.x = g_MeshCylinder[nCntMeshCylinder].pos.x - (pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].pos.x / pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].pos.x);
					//vecNor.y = g_MeshCylinder[nCntMeshCylinder].pos.y - (pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].pos.y / pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].pos.y);
					vecNor.z = g_MeshCylinder[nCntMeshCylinder].pos.z - (pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].pos.z / pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].pos.z);
					D3DXVec3Normalize(&vecNor, &vecNor);		// 正規化する
					pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].nor = vecNor;

					pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1))) + (nCount_x)].tex =
						D3DXVECTOR2((MESHCYLINDER_TEXTURE_UV_U / (g_MeshCylinder[nCntMeshCylinder].num.x)) * (nCount_x % ((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1)), (MESHCYLINDER_TEXTURE_UV_V / (g_MeshCylinder[nCntMeshCylinder].num.y)) * (nCount_y));
						//D3DXVECTOR2(MESHCYLINDER_TEXTURE_UV_U * (nCount_x % ((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1)), MESHCYLINDER_TEXTURE_UV_V * (nCount_y));
				}
			}

			// インデックスの設定
			for (nCount_y = 0; nCount_y < g_MeshCylinder[nCntMeshCylinder].num.y; nCount_y++)
			{
				for (nCount_x = 0; nCount_x < g_MeshCylinder[nCntMeshCylinder].num.x + 1; nCount_x++)
				{
					// pIdx[(メッシュカウンタ * 頂点数) + (yカウンタ * (xの設置数 + 1)) + (xカウンタ * 2) + 円柱に使用した頂点数]

					// 例：	(		nCntxxx	  *	  4	  )	+ (	  0		 * (    2  	  + 1)) + (    0	 * 2) + 0
					//	=	((yカウンタ + 1) * (xの設置数 + 1)) + (xカウンタ)
					// pIdxの	xカウンタ * 2	は2頂点ずつ設定するため行う（2頂点目は	+ 1	で行う)
					// yカウンタ + 1	により下を求め、次に	yカウンタ　のみで上を求める

					pIdx[(nNumIdx) + (nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh] =
						(WORD)(((nCount_y + 1) * (g_MeshCylinder[nCntMeshCylinder].num.x + 1)) + nCount_x);

					pIdx[(nNumIdx) + (nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh + 1] =
						(WORD)(((nCount_y) * (g_MeshCylinder[nCntMeshCylinder].num.x + 1)) + nCount_x);

					if (g_MeshCylinder[nCntMeshCylinder].num.y > 1 && nCount_y != g_MeshCylinder[nCntMeshCylinder].num.y - 1 && nCount_x == g_MeshCylinder[nCntMeshCylinder].num.x)
					{
						pIdx[(nNumIdx) + (nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh + 1 + 1] =
							(WORD)(((nCount_y) * (g_MeshCylinder[nCntMeshCylinder].num.x + 1)) + nCount_x);

						pIdx[(nNumIdx) + (nCount_y * (((int)g_MeshCylinder[nCntMeshCylinder].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh + 1 + 2] =
							(WORD)(((nCount_y + 2) * (g_MeshCylinder[nCntMeshCylinder].num.x + 1)));

						nCount_mesh += 2;		// 円柱の頂点加算
					}
				}
			}

			break;
		}

		nCount_mesh = 0;
		nNumVtx += g_MeshCylinder[nCntMeshCylinder].MaxVtx;		// 頂点バッファを調整
		nNumIdx += g_MeshCylinder[nCntMeshCylinder].MaxIdx;		// インデックスを調整
	}

	// インデックスバッファをアンロック
	g_pIdxBuffMeshCylinder->Unlock();

	// 頂点バッファをアンロック
	g_pVtxBuffMeshCylinder->Unlock();
}

//=========================================================================================================================
// シリンダーの破棄処理
//=========================================================================================================================
void DeleteMeshCylinder(void)
{
	int nCntMeshCylinder;

	for (nCntMeshCylinder = 0; nCntMeshCylinder < g_MaxMeshCylinder; nCntMeshCylinder++)
	{
		if (g_MeshCylinder[nCntMeshCylinder].bUse == true)
		{
			g_MeshCylinder[nCntMeshCylinder].bUse = false;
		}
	}
}

//=========================================================================================================================
// 頂点情報の作成
//=========================================================================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMeshCylinder;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MESHCYLINDER_TEXTURE_NAME1, &g_pTextureMeshCylinder[0]);
	D3DXCreateTextureFromFile(pDevice, MESHCYLINDER_TEXTURE_NAME2, &g_pTextureMeshCylinder[1]);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * g_MaxVtxMeshCylinder,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL
	);

	// インデックスバッファ生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * g_MaxIdxMeshCylinder,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL
	);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshCylinder = 0; nCntMeshCylinder < g_MaxVtxMeshCylinder; nCntMeshCylinder++)
	{
		// 頂点情報の設定
		pVtx[nCntMeshCylinder].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 法線の設定
		pVtx[nCntMeshCylinder].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		// 頂点カラーの設定
		pVtx[nCntMeshCylinder].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// テクスチャの設定
		pVtx[nCntMeshCylinder].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMeshCylinder->Unlock();

	WORD *pIdx;			// インデックスデータへのポインタ

	// インデックスバッファをロック
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshCylinder = 0; nCntMeshCylinder < g_MaxIdxMeshCylinder; nCntMeshCylinder++)
	{
		// インデックスの設定
		pIdx[nCntMeshCylinder] = nCntMeshCylinder;
	}

	// インデックスバッファをアンロック
	g_pIdxBuffMeshCylinder->Unlock();
}

