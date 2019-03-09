//=========================================================================================================================
//
// 壁処理 [meshwall.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanfエラー解除
#include <stdio.h>					// stdio
#include "meshwall.h"
#include "input.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MESHWALL_TEXT_INFO			"data\\TEXT\\meshwalldata.txt"			// 読み込むデータファイル
#define MAX_MESHWALL				(32)									// 壁の最大数
#define MESHWALL_TEXTURE_NAME1		"data\\TEXTURE\\field001.jpg"			// 読み込むテクスチャファイル1
#define MESHWALL_TEXTURE_UV_U		(1.0f)									// テクスチャアニメーションU範囲
#define MESHWALL_TEXTURE_UV_V		(1.0f)									// テクスチャアニメーションV範囲

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice);						// 頂点情報の設定

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureMeshWall = NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffMeshWall = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9			g_pIdxBuffMeshWall = NULL;					// インデックスバッファへのポインタ
MeshWall						g_MeshWall[MAX_MESHWALL];					// 壁の情報
int								g_MaxMeshWall;								// 読み込むモデルの最大数
int								g_MaxVtxMeshWall;							// 頂点の最大数
int								g_MaxIdxMeshWall;							// インデックスの最大数
int								g_MaxPolygonMeshWall;						// ポリゴンの最大数

//=========================================================================================================================
// 壁の初期化処理
//=========================================================================================================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntMeshWall;
	FILE *pFile;

	g_MaxMeshWall = 0;
	g_MaxPolygonMeshWall = 0;
	g_MaxVtxMeshWall = 0;

	// ファイル設定
	pFile = fopen(MESHWALL_TEXT_INFO, "r");

	// 壁テキストデータ読み込み
	if (pFile != NULL)
	{// ファイルが開けた場合
		fscanf(pFile, "%d", &g_MaxMeshWall);	// 読み込み

		for (nCntMeshWall = 0; nCntMeshWall < g_MaxMeshWall; nCntMeshWall++)
		{
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].pos.x);		// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].pos.y);		// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].pos.z);		// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].rot.x);		// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].rot.y);		// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].rot.z);		// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].num.x);		// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].num.y);		// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].num.z);		// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].fRadius.x);	// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].fRadius.y);	// 読み込み
			fscanf(pFile, "%f", &g_MeshWall[nCntMeshWall].fRadius.z);	// 読み込み
		}

		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}

	for (nCntMeshWall = 0; nCntMeshWall < g_MaxMeshWall; nCntMeshWall++)
	{
		// 頂点、インデックス、ポリゴン数を計算
		g_MeshWall[nCntMeshWall].MaxVtx = ((int)g_MeshWall[nCntMeshWall].num.x + 1) * ((int)g_MeshWall[nCntMeshWall].num.y + 1);
		g_MeshWall[nCntMeshWall].MaxIdx = (((int)g_MeshWall[nCntMeshWall].num.x + 1) * 2) * (int)g_MeshWall[nCntMeshWall].num.y + (((int)g_MeshWall[nCntMeshWall].num.y - 1) * 2);
		g_MeshWall[nCntMeshWall].MaxPolygon = ((int)g_MeshWall[nCntMeshWall].num.x * (int)g_MeshWall[nCntMeshWall].num.y * 2) + (((int)g_MeshWall[nCntMeshWall].num.y - 1) * 4);
		g_MeshWall[nCntMeshWall].nTexNum = 0;
		g_MeshWall[nCntMeshWall].bUse = false;

		// 頂点、インデックス、ポリゴン数を加算
		g_MaxVtxMeshWall += g_MeshWall[nCntMeshWall].MaxVtx;
		g_MaxIdxMeshWall += g_MeshWall[nCntMeshWall].MaxIdx;
		g_MaxPolygonMeshWall += g_MeshWall[nCntMeshWall].MaxPolygon;
	}

	// 頂点情報の作成
	MakeVertexMeshWall(pDevice);
}

//=========================================================================================================================
// 壁の終了処理
//=========================================================================================================================
void UninitMeshWall(void)
{
	// テクスチャの破棄
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	// インデックスバッファの破棄
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}
}

//=========================================================================================================================
// 壁の更新処理
//=========================================================================================================================
void UpdateMeshWall(void)
{

}

//=========================================================================================================================
// 壁の描画処理
//=========================================================================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	int nCntMeshWall, nNumVtx = 0, nNumIdx = 0;

	for (nCntMeshWall = 0; nCntMeshWall < g_MaxMeshWall; nCntMeshWall++)
	{
		if (g_MeshWall[nCntMeshWall].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_MeshWall[nCntMeshWall].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_MeshWall[nCntMeshWall].rot.y,
				g_MeshWall[nCntMeshWall].rot.x,
				g_MeshWall[nCntMeshWall].rot.z
			);

			D3DXMatrixMultiply
			(
				&g_MeshWall[nCntMeshWall].mtxWorld,
				&g_MeshWall[nCntMeshWall].mtxWorld,
				&mtxRot
			);

			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_MeshWall[nCntMeshWall].pos.x,
				g_MeshWall[nCntMeshWall].pos.y,
				g_MeshWall[nCntMeshWall].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_MeshWall[nCntMeshWall].mtxWorld,
				&g_MeshWall[nCntMeshWall].mtxWorld,
				&mtxTrans
			);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshWall[nCntMeshWall].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource
			(
				0,
				g_pVtxBuffMeshWall,
				0,
				sizeof(VERTEX_3D)
			);

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshWall);

			// 頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャ設定
			if (g_MeshWall[nCntMeshWall].nTexNum == 1)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureMeshWall);
			}

			// 壁の描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, nNumIdx, nNumVtx, 0, g_MeshWall[nCntMeshWall].MaxPolygon);
		}

		nNumVtx += g_MeshWall[nCntMeshWall].MaxVtx;		// 頂点バッファを調整
		nNumIdx += g_MeshWall[nCntMeshWall].MaxIdx;		// 頂点バッファを調整
	}
}

//=========================================================================================================================
// 壁の設定処理
//=========================================================================================================================
void SetMeshWall(int nTexNum, D3DXCOLOR col)
{
	int nCntMeshWall, nNumVtx = 0, nNumIdx = 0;
	int nCount_x, nCount_y, nCount_mesh = 0;

	int ip = 0;
	float fx = 0.0f, fy = 0.0f;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffMeshWall->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	WORD *pIdx;			// インデックスデータへのポインタ

	// インデックスバッファをロック
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshWall = 0; nCntMeshWall < g_MaxMeshWall; nCntMeshWall++)
	{// ポリゴンをカウント
		if (g_MeshWall[nCntMeshWall].bUse == false)
		{// 使用していない場合
			g_MeshWall[nCntMeshWall].nTexNum = nTexNum;
			g_MeshWall[nCntMeshWall].bUse = true;				// 使用する

			// 頂点情報の設定
			for (nCount_y = 0; nCount_y < g_MeshWall[nCntMeshWall].num.y + 1; nCount_y++)
			{// yをカウント
				for (nCount_x = 0; nCount_x < g_MeshWall[nCntMeshWall].num.x + 1; nCount_x++)
				{// xをカウント

					// pVtx[(メッシュカウンタ * 頂点数) + (yカウンタ * (xの設置数 + 1)) + (xカウンタ)]
					// 例：	(		nCntxxx	  *	  4	  )	+ (	  0		 * (    2  	  + 1)) + (    0	)
					// .pos.x = -((xの大きさ / 2) * xの設置数) + (xの大きさ * xカウンタ)
					//		y =  ((yの大きさ / 2) * yの設置数) - (yの大きさ * yカウンタ)
					// 中心に描画するために割２を行う。
					// 最も左と奥の値を求め、加算減算により他頂点を求める

					pVtx[(nNumVtx) + (nCount_y * (((int)g_MeshWall[nCntMeshWall].num.x + 1))) + (nCount_x)].pos = D3DXVECTOR3(
						-((g_MeshWall[nCntMeshWall].fRadius.x / 2) * g_MeshWall[nCntMeshWall].num.x) + (g_MeshWall[nCntMeshWall].fRadius.x * nCount_x),
						((g_MeshWall[nCntMeshWall].fRadius.y) * g_MeshWall[nCntMeshWall].num.y) - (g_MeshWall[nCntMeshWall].fRadius.y * (nCount_y)), 0.0f);

					ip = (nNumVtx) + (nCount_y * (((int)g_MeshWall[nCntMeshWall].num.x + 1))) + (nCount_x);

					fx = -((g_MeshWall[nCntMeshWall].fRadius.x / 2) * g_MeshWall[nCntMeshWall].num.x) + (g_MeshWall[nCntMeshWall].fRadius.x * nCount_x);
					fy = ((g_MeshWall[nCntMeshWall].fRadius.y) * g_MeshWall[nCntMeshWall].num.y) - (g_MeshWall[nCntMeshWall].fRadius.y * (nCount_y));

					// pVtx[(メッシュカウンタ * 頂点数) + (yカウンタ * (xの設置数 + 1)) + (xカウンタ)]
					// 例：	(		nCntxxx	  *	  4	  )	+ (	  0		 * (    2  	  + 1)) + (    0	)
					// .pos.x = -((xの大きさ / 2) * xの設置数) + (xの大きさ * xカウンタ)
					//		y =  ((yの大きさ / 2) * yの設置数) - (yの大きさ * yカウンタ)
					// 中心に描画するために割２を行う。
					// 最も左と奥の値を求め、加算減算により他頂点を求める

					// まだなおしてないよ
					pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshWall[nCntMeshWall].num.x + 1))) + (nCount_x)].col = col;

					pVtx[(nNumVtx)+(nCount_y * (((int)g_MeshWall[nCntMeshWall].num.x + 1))) + (nCount_x)].tex =
						D3DXVECTOR2(MESHWALL_TEXTURE_UV_U * (nCount_x % ((int)g_MeshWall[nCntMeshWall].num.x + 1)), MESHWALL_TEXTURE_UV_V * (nCount_y));
				}
			}

			// インデックスの設定
			for (nCount_y = 0; nCount_y < g_MeshWall[nCntMeshWall].num.y; nCount_y++)
			{
				for (nCount_x = 0; nCount_x < g_MeshWall[nCntMeshWall].num.x + 1; nCount_x++)
				{
					// pIdx[(メッシュカウンタ * 頂点数) + (yカウンタ * (xの設置数 + 1)) + (xカウンタ * 2) + 壁に使用した頂点数]
					// 例：	(		nCntxxx	  *	  4	  )	+ (	  0		 * (    2  	  + 1)) + (    0	 * 2) + 0
					//	=	((yカウンタ + 1) * (xの設置数 + 1)) + (xカウンタ)
					// pIdxの	xカウンタ * 2	は2頂点ずつ設定するため行う（2頂点目は	+ 1	で行う)
					// yカウンタ + 1	により下を求め、次に	yカウンタ　のみで上を求める

					pIdx[(nNumIdx) + (nCount_y * (((int)g_MeshWall[nCntMeshWall].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh] =
						(WORD)((nNumVtx) + ((nCount_y + 1) * (g_MeshWall[nCntMeshWall].num.x + 1)) + nCount_x);

					pIdx[(nNumIdx) + (nCount_y * (((int)g_MeshWall[nCntMeshWall].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh + 1] =
						(WORD)(((nCount_y) * (g_MeshWall[nCntMeshWall].num.x + 1)) + nCount_x);

					ip = (nNumIdx) + (nCount_y * (((int)g_MeshWall[nCntMeshWall].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh;
					fx = ((nNumVtx) + ((nCount_y + 1) * (g_MeshWall[nCntMeshWall].num.x + 1)) + nCount_x);
					fy = ((nNumVtx) + ((nCount_y) * (g_MeshWall[nCntMeshWall].num.x + 1)) + nCount_x);


					if (g_MeshWall[nCntMeshWall].num.y > 1 && nCount_y != g_MeshWall[nCntMeshWall].num.y - 1 && nCount_x == g_MeshWall[nCntMeshWall].num.x)
					{
						pIdx[(nNumIdx) + (nCount_y * (((int)g_MeshWall[nCntMeshWall].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh + 1 + 1] =
							(WORD)((nNumVtx) + ((nCount_y) * (g_MeshWall[nCntMeshWall].num.x + 1)) + nCount_x);

						pIdx[(nNumIdx) + (nCount_y * (((int)g_MeshWall[nCntMeshWall].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh + 1 + 2] =
							(WORD)((nNumVtx) + ((nCount_y + 2) * (g_MeshWall[nCntMeshWall].num.x + 1)));

						ip = (nNumIdx) + (nCount_y * (((int)g_MeshWall[nCntMeshWall].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh + 1 + 1;
						fx = ((nNumVtx) + ((nCount_y) * (g_MeshWall[nCntMeshWall].num.x + 1)) + nCount_x);
						fy = ((nNumVtx) + ((nCount_y + 2) * (g_MeshWall[nCntMeshWall].num.x + 1)));

						nCount_mesh += 2;		// 壁の頂点加算
					}
				}
			}

			break;
		}
		nCount_mesh = 0;
		nNumVtx += g_MeshWall[nCntMeshWall].MaxVtx;		// 頂点バッファを調整
		nNumIdx += g_MeshWall[nCntMeshWall].MaxIdx;
	}

	// インデックスバッファをアンロック
	g_pIdxBuffMeshWall->Unlock();

	// 頂点バッファをアンロック
	g_pVtxBuffMeshWall->Unlock();
}

//=========================================================================================================================
// 壁の衝突処理
//=========================================================================================================================
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pRot, D3DXVECTOR3 fRadius)
{
	int nCntMeshWall, nCount = 0;
	bool bLand = false;
	float vecA_x, vecA_z, vecC_x, vecC_z;
	float fx1, fx2, fz1, fz2;
	D3DXVECTOR3 Radius;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshWall = 0; nCntMeshWall < g_MaxMeshWall; nCntMeshWall++)
	{
		if (g_MeshWall[nCntMeshWall].bUse == true)
		{// 使用されている
			fx1 = (g_MeshWall[nCntMeshWall].pos.x + sinf(g_MeshWall[nCntMeshWall].rot.y) + pVtx[nCount + 1].pos.x);
			fx2 = (g_MeshWall[nCntMeshWall].pos.x + sinf(g_MeshWall[nCntMeshWall].rot.y) + pVtx[nCount].pos.x);
			fz1 = (g_MeshWall[nCntMeshWall].pos.z + cosf(g_MeshWall[nCntMeshWall].rot.y) + pVtx[nCount + 1].pos.z);
			fz2 = (g_MeshWall[nCntMeshWall].pos.z + cosf(g_MeshWall[nCntMeshWall].rot.y) + pVtx[nCount].pos.z);

			Radius = g_MeshWall[nCntMeshWall].fRadius;
			Radius.x = g_MeshWall[nCntMeshWall].pos.x + sinf(g_MeshWall[nCntMeshWall].rot.y) * pVtx[nCount + 1].pos.x;
			Radius.z = g_MeshWall[nCntMeshWall].pos.x + cosf(g_MeshWall[nCntMeshWall].rot.y) * pVtx[nCount + 1].pos.z;

			vecA_x = (g_MeshWall[nCntMeshWall].pos.x + Radius.x) - (g_MeshWall[nCntMeshWall].pos.x + Radius.x);
			vecA_z = (g_MeshWall[nCntMeshWall].pos.z + Radius.z) - (g_MeshWall[nCntMeshWall].pos.z + Radius.z);
			vecC_x = (pPos->x) - (g_MeshWall[nCntMeshWall].pos.x + Radius.x);
			vecC_z = (pPos->z) - (g_MeshWall[nCntMeshWall].pos.z + Radius.z);

			if (((vecA_z * vecC_x) - (vecA_x * vecC_z)) < 0)
			{
				bLand = true;
			}

			nCount += 4;


			/*if (pPos->x + fRadius.x > g_MeshWall[nCntMeshWall].pos.x - g_MeshWall[nCntMeshWall].fRadius.x && pPos->x - fRadius.x < g_MeshWall[nCntMeshWall].pos.x + g_MeshWall[nCntMeshWall].fRadius.x)
			{// X範囲確認
				if (pPosOld->z + fRadius.z >= g_MeshWall[nCntMeshWall].pos.z && g_MeshWall[nCntMeshWall].pos.z > pPos->z + fRadius.z)
				{// Z上底判定
					pPos->z = g_MeshWall[nCntMeshWall].pos.z - fRadius.z;
					//pMove->x = 0.0f;
					//pMove->z = 0.0f;
				}
				else if (pPosOld->z + fRadius.z <= g_MeshWall[nCntMeshWall].pos.z && g_MeshWall[nCntMeshWall].pos.z < pPos->z + fRadius.z)
				{// Z下底判定
					pPos->z = g_MeshWall[nCntMeshWall].pos.z - fRadius.z;
					//pMove->x = 0.0f;
					//pMove->z = 0.0f;
				}
			}

			if (pPos->z + fRadius.z > g_MeshWall[nCntMeshWall].pos.z && pPos->z + fRadius.z < g_MeshWall[nCntMeshWall].pos.z)
			{// Z範囲確認
				if (pPosOld->x + fRadius.x >= g_MeshWall[nCntMeshWall].pos.x && g_MeshWall[nCntMeshWall].pos.x > pPos->x + fRadius.x)
				{// X右辺判定
					pPos->x = g_MeshWall[nCntMeshWall].pos.x - fRadius.x;
					//pMove->x = 0.0f;
					//pMove->z = 0.0f;
				}
				else if (pPosOld->x + fRadius.x <= g_MeshWall[nCntMeshWall].pos.x && g_MeshWall[nCntMeshWall].pos.x < pPos->x + fRadius.x)
				{// X左辺判定
					pPos->x = g_MeshWall[nCntMeshWall].pos.x - fRadius.x;
					//pMove->x = 0.0f;
					//pMove->z = 0.0f;
				}
			}*/
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMeshWall->Unlock();

	return bLand;
}

//=========================================================================================================================
// 壁の破棄処理
//=========================================================================================================================
void DeleteMeshWall(void)
{
	int nCntMeshWall;

	for (nCntMeshWall = 0; nCntMeshWall < g_MaxMeshWall; nCntMeshWall++)
	{
		if (g_MeshWall[nCntMeshWall].bUse == true)
		{// 使用されている
			g_MeshWall[nCntMeshWall].bUse = false;
		}
	}
}

//=========================================================================================================================
// 頂点情報の作成
//=========================================================================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMeshWall;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MESHWALL_TEXTURE_NAME1, &g_pTextureMeshWall);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * g_MaxVtxMeshWall,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL
	);

	// インデックスバッファ生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * g_MaxIdxMeshWall,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshWall,
		NULL
	);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshWall = 0; nCntMeshWall < g_MaxVtxMeshWall; nCntMeshWall++)
	{
		// 頂点情報の設定
		pVtx[nCntMeshWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 法線の設定
		pVtx[nCntMeshWall].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		// 頂点カラーの設定
		pVtx[nCntMeshWall].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// テクスチャの設定
		pVtx[nCntMeshWall].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMeshWall->Unlock();

	WORD *pIdx;			// インデックスデータへのポインタ

	// インデックスバッファをロック
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshWall = 0; nCntMeshWall < g_MaxIdxMeshWall; nCntMeshWall++)
	{
		// インデックスの設定
		pIdx[nCntMeshWall] = nCntMeshWall;
	}

	// インデックスバッファをアンロック
	g_pIdxBuffMeshWall->Unlock();
}

