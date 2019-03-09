//=========================================================================================================================
//
// フィールド処理 [meshfield.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanfエラー解除
#include <stdio.h>					// stdio
#include "meshfield.h"
#include "input.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MESHFIELD_TEXT_INFO			"data\\TEXT\\meshfielddata.txt"			// 読み込むデータファイル
#define MAX_MESHFIELD				(32)									// フィールドの最大数
#define MESHFIELD_TEXTURE_UV_U		(1.0f)									// テクスチャアニメーションU範囲
#define MESHFIELD_TEXTURE_UV_V		(1.0f)									// テクスチャアニメーションV範囲

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);						// 頂点情報の設定

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureMeshField[FIELDTEXTURE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffMeshField = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9			g_pIdxBuffMeshField = NULL;					// インデックスバッファへのポインタ
MeshField						g_MeshField[MAX_MESHFIELD];					// フィールドの情報
int								g_MaxMeshField;								// 読み込むモデルの最大数
int								g_MaxVtxMeshField;							// 頂点の最大数
int								g_MaxIdxMeshField;							// インデックスの最大数
int								g_MaxPolygonMeshField;						// ポリゴンの最大数

//=========================================================================================================================
// フィールドの初期化処理
//=========================================================================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntMeshField;
	FILE *pFile;

	g_MaxMeshField = 0;
	g_MaxPolygonMeshField = 0;
	g_MaxVtxMeshField = 0;

	// ファイル設定
	pFile = fopen(MESHFIELD_TEXT_INFO, "r");

	// フィールドテキストデータ読み込み
	if (pFile != NULL)
	{// ファイルが開けた場合
		fscanf(pFile, "%d", &g_MaxMeshField);	// 読み込み

		for (nCntMeshField = 0; nCntMeshField < g_MaxMeshField; nCntMeshField++)
		{
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].pos.x);		// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].pos.y);		// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].pos.z);		// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].rot.x);		// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].rot.y);		// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].rot.z);		// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].num.x);		// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].num.y);		// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].num.z);		// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].fRadius.x);	// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].fRadius.y);	// 読み込み
			fscanf(pFile, "%f", &g_MeshField[nCntMeshField].fRadius.z);	// 読み込み
		}

		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}

	for (nCntMeshField = 0; nCntMeshField < g_MaxMeshField; nCntMeshField++)
	{
		// 頂点、インデックス、ポリゴン数を計算
		g_MeshField[nCntMeshField].MaxVtx = ((int)g_MeshField[nCntMeshField].num.x + 1) * ((int)g_MeshField[nCntMeshField].num.z + 1);
		g_MeshField[nCntMeshField].MaxIdx = (((int)g_MeshField[nCntMeshField].num.x + 1) * 2) * (int)g_MeshField[nCntMeshField].num.z + (((int)g_MeshField[nCntMeshField].num.z - 1) * 2);
		g_MeshField[nCntMeshField].MaxPolygon = ((int)g_MeshField[nCntMeshField].num.x * (int)g_MeshField[nCntMeshField].num.z * 2) + (((int)g_MeshField[nCntMeshField].num.z - 1) * 4);
		g_MeshField[nCntMeshField].nTexNum = 0;
		g_MeshField[nCntMeshField].bUse = false;

		// 頂点、インデックス、ポリゴン数を加算
		g_MaxVtxMeshField += g_MeshField[nCntMeshField].MaxVtx;
		g_MaxIdxMeshField += g_MeshField[nCntMeshField].MaxIdx;
		g_MaxPolygonMeshField += g_MeshField[nCntMeshField].MaxPolygon;
	}

	// 頂点情報の作成
	MakeVertexMeshField(pDevice);
}

//=========================================================================================================================
// フィールドの終了処理
//=========================================================================================================================
void UninitMeshField(void)
{
	// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < FIELDTEXTURE_MAX; nCntTex++)
	{
		if (g_pTextureMeshField[nCntTex] != NULL)
		{
			g_pTextureMeshField[nCntTex]->Release();
			g_pTextureMeshField[nCntTex] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	// インデックスバッファの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=========================================================================================================================
// フィールドの更新処理
//=========================================================================================================================
void UpdateMeshField(void)
{

}

//=========================================================================================================================
// フィールドの描画処理
//=========================================================================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	int nCntMeshField, nNumVtx = 0, nNumIdx = 0;

	for (nCntMeshField = 0; nCntMeshField < g_MaxMeshField; nCntMeshField++)
	{
		if (g_MeshField[nCntMeshField].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_MeshField[nCntMeshField].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_MeshField[nCntMeshField].rot.y,
				g_MeshField[nCntMeshField].rot.x,
				g_MeshField[nCntMeshField].rot.z
			);

			D3DXMatrixMultiply
			(
				&g_MeshField[nCntMeshField].mtxWorld,
				&g_MeshField[nCntMeshField].mtxWorld,
				&mtxRot
			);

			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_MeshField[nCntMeshField].pos.x,
				g_MeshField[nCntMeshField].pos.y,
				g_MeshField[nCntMeshField].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_MeshField[nCntMeshField].mtxWorld,
				&g_MeshField[nCntMeshField].mtxWorld,
				&mtxTrans
			);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshField[nCntMeshField].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource
			(
				0,
				g_pVtxBuffMeshField,
				0,
				sizeof(VERTEX_3D)
			);

			// インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshField);

			// 頂点フォーマット
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャ設定
			if (g_MeshField[nCntMeshField].nTexNum >= FIELDTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureMeshField[g_MeshField[nCntMeshField].nTexNum]);
			}

			// フィールドの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, nNumIdx, g_MeshField[nCntMeshField].MaxIdx + nNumIdx, nNumIdx, g_MeshField[nCntMeshField].MaxPolygon);
		}

		nNumVtx += g_MeshField[nCntMeshField].MaxVtx;
		nNumIdx += g_MeshField[nCntMeshField].MaxIdx;
	}
}

//=========================================================================================================================
// フィールドの設定処理
//=========================================================================================================================
void SetMeshField(int SetNum, int nTexNum)
{
	int nCntMeshField, nNumVtx = 0, nNumIdx = 0;
	int nCount_x, nCount_z, nCount_mesh = 0;

	int ip = 0;
	float fx = 0, fz = 0;

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロック
	g_pVtxBuffMeshField->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	WORD *pIdx;			// インデックスデータへのポインタ

						// インデックスバッファをロック
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshField = 0; nCntMeshField < SetNum; nCntMeshField++)
	{// ポリゴンをカウント
		nCount_mesh = 0;
		nNumVtx += g_MeshField[nCntMeshField].MaxVtx;
		nNumIdx += g_MeshField[nCntMeshField].MaxIdx;
	}

	if (g_MeshField[SetNum].bUse == false)
	{// 使用していない場合
		g_MeshField[SetNum].nTexNum = nTexNum;
		g_MeshField[SetNum].bUse = true;				// 使用する

														// 頂点情報の設定
		for (nCount_z = 0; nCount_z < g_MeshField[SetNum].num.z + 1; nCount_z++)
		{// zをカウント
			for (nCount_x = 0; nCount_x < g_MeshField[SetNum].num.x + 1; nCount_x++)
			{// xをカウント

			 // pVtx[(メッシュカウンタ * 頂点数) + (zカウンタ * (xの設置数 + 1)) + (xカウンタ)]
			 // 例：	(		nCntxxx	  *	  4	  )	+ (	  0		 * (    2  	  + 1)) + (    0	)
			 // .pos.x = -((xの大きさ / 2) * xの設置数) + (xの大きさ * xカウンタ)
			 //		z =  ((zの大きさ / 2) * zの設置数) - (zの大きさ * zカウンタ)
			 // 中心に描画するために割２を行う。
			 // 最も左と奥の値を求め、加算減算により他頂点を求める

				pVtx[(nNumVtx)+(nCount_z * (((int)g_MeshField[SetNum].num.x + 1))) + (nCount_x)].pos = D3DXVECTOR3(
					-((g_MeshField[SetNum].fRadius.x / 2) * g_MeshField[SetNum].num.x) + (g_MeshField[SetNum].fRadius.x * nCount_x), 0.0f,
					((g_MeshField[SetNum].fRadius.z / 2) * g_MeshField[SetNum].num.z) - (g_MeshField[SetNum].fRadius.z * (nCount_z)));

				ip = (nNumVtx)+(nCount_z * (((int)g_MeshField[SetNum].num.x + 1))) + (nCount_x);
				fx = -((g_MeshField[SetNum].fRadius.x / 2) * g_MeshField[SetNum].num.x) + (g_MeshField[SetNum].fRadius.x * nCount_x);
				fz = ((g_MeshField[SetNum].fRadius.z / 2) * g_MeshField[SetNum].num.z) - (g_MeshField[SetNum].fRadius.z * (nCount_z));

				// pVtx[(メッシュカウンタ * 頂点数) + (zカウンタ * (xの設置数 + 1)) + (xカウンタ)]
				// 例：	(		nCntxxx	  *	  4	  )	+ (	  0		 * (    2  	  + 1)) + (    0	)
				// .pos.x = -((xの大きさ / 2) * xの設置数) + (xの大きさ * xカウンタ)
				//		z =  ((zの大きさ / 2) * zの設置数) - (zの大きさ * zカウンタ)
				// 中心に描画するために割２を行う。
				// 最も左と奥の値を求め、加算減算により他頂点を求める

				// まだなおしてないよ

				pVtx[(nNumVtx)+(nCount_z * (((int)g_MeshField[SetNum].num.x + 1))) + (nCount_x)].tex =
					//D3DXVECTOR2((MESHFIELD_TEXTURE_UV_U / (g_MeshField[SetNum].num.x)) * (nCount_x % ((int)g_MeshField[SetNum].num.x + 1)), (MESHFIELD_TEXTURE_UV_V / (g_MeshField[SetNum].num.z)) * (nCount_z));
					D3DXVECTOR2(MESHFIELD_TEXTURE_UV_U * (nCount_x % ((int)g_MeshField[SetNum].num.x + 1)), MESHFIELD_TEXTURE_UV_V * (nCount_z));
			}
		}

		// インデックスの設定
		for (nCount_z = 0; nCount_z < g_MeshField[SetNum].num.z; nCount_z++)
		{
			for (nCount_x = 0; nCount_x < g_MeshField[SetNum].num.x + 1; nCount_x++)
			{
				// pIdx[(メッシュカウンタ * 頂点数) + (zカウンタ * (xの設置数 + 1)) + (xカウンタ * 2) + フィールドに使用した頂点数]
				// 例：	(		nCntxxx	  *	  4	  )	+ (	  0		 * (    2  	  + 1)) + (    0	 * 2) + 0
				//	=	((zカウンタ + 1) * (xの設置数 + 1)) + (xカウンタ)
				// pIdxの	xカウンタ * 2	は2頂点ずつ設定するため行う（2頂点目は	+ 1	で行う)
				// zカウンタ + 1	により下を求め、次に	zカウンタ　のみで上を求める	

				pIdx[(nNumIdx)+(nCount_z * (((int)g_MeshField[SetNum].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh] =
					(WORD)(((nNumVtx)+(nCount_z + 1) * (g_MeshField[SetNum].num.x + 1)) + nCount_x);

				pIdx[(nNumIdx)+(nCount_z * (((int)g_MeshField[SetNum].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh + 1] =
					(WORD)(((nNumVtx)+(nCount_z) * (g_MeshField[SetNum].num.x + 1)) + nCount_x);

				ip = (nNumIdx)+(nCount_z * (((int)g_MeshField[SetNum].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh;
				fx = (((nNumVtx)+(nCount_z + 1) * (g_MeshField[SetNum].num.x + 1)) + nCount_x);
				fz = (((nNumVtx)+(nCount_z) * (g_MeshField[SetNum].num.x + 1)) + nCount_x);

				if (g_MeshField[SetNum].num.z > 1 && nCount_z != g_MeshField[SetNum].num.z - 1 && nCount_x == g_MeshField[SetNum].num.x)
				{
					pIdx[(nNumIdx)+(nCount_z * (((int)g_MeshField[SetNum].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh + 1 + 1] =
						(WORD)(((nNumVtx)+(nCount_z) * (g_MeshField[SetNum].num.x + 1)) + nCount_x);

					pIdx[(nNumIdx)+(nCount_z * (((int)g_MeshField[SetNum].num.x + 1) * 2)) + (nCount_x * 2) + nCount_mesh + 1 + 2] =
						(WORD)(((nNumVtx)+(nCount_z + 2) * (g_MeshField[SetNum].num.x + 1)));

					nCount_mesh += 2;		// フィールドの頂点加算
				}
			}
		}
	}

	// インデックスバッファをアンロック
	g_pIdxBuffMeshField->Unlock();

	// 頂点バッファをアンロック
	g_pVtxBuffMeshField->Unlock();
}

//=========================================================================================================================
// フィールドの取得処理
//=========================================================================================================================
MeshField *GetMeshField(void)
{
	return &g_MeshField[0];
}

//=========================================================================================================================
// フィールドの衝突処理
//=========================================================================================================================
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	int nCntMeshField;
	bool bLand = false;

	for (nCntMeshField = 0; nCntMeshField < g_MaxMeshField; nCntMeshField++)
	{
		if (g_MeshField[nCntMeshField].bUse == true)
		{// 使用されている
			if (pPos->x + vtxMax->x > g_MeshField[nCntMeshField].pos.x - ((g_MeshField[nCntMeshField].fRadius.x / 2.0f) * g_MeshField[nCntMeshField].num.x) && pPos->x + vtxMin->x < g_MeshField[nCntMeshField].pos.x + ((g_MeshField[nCntMeshField].fRadius.x / 2.0f) * g_MeshField[nCntMeshField].num.x))
			{// X範囲確認
				if (pPos->z + vtxMax->z > g_MeshField[nCntMeshField].pos.z - ((g_MeshField[nCntMeshField].fRadius.z / 2.0f) * g_MeshField[nCntMeshField].num.z) && pPos->z + vtxMin->z < g_MeshField[nCntMeshField].pos.z + ((g_MeshField[nCntMeshField].fRadius.z / 2.0f) * g_MeshField[nCntMeshField].num.z))
				{// Z範囲確認
					if (pPosOld->y >= g_MeshField[nCntMeshField].pos.y + g_MeshField[nCntMeshField].fRadius.y && g_MeshField[nCntMeshField].pos.y + g_MeshField[nCntMeshField].fRadius.y > pPos->y)
					{// Y判定
						pPos->y = g_MeshField[nCntMeshField].pos.y + g_MeshField[nCntMeshField].fRadius.y;
						pMove->y = 0.0f;
						//pMove->z = 0.0f;
					}
				}
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// フィールドの破棄処理
//=========================================================================================================================
void DeleteMeshField(void)
{
	int nCntMeshField;

	for (nCntMeshField = 0; nCntMeshField < g_MaxMeshField; nCntMeshField++)
	{
		if (g_MeshField[nCntMeshField].bUse == true)
		{// 使用されている
			g_MeshField[nCntMeshField].bUse = false;
		}
	}
}

//=========================================================================================================================
// 頂点情報の作成
//=========================================================================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntMeshField;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTURE_NAME1, &g_pTextureMeshField[0]);
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTURE_NAME2, &g_pTextureMeshField[1]);
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTURE_NAME3, &g_pTextureMeshField[2]);
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTURE_NAME4, &g_pTextureMeshField[3]);
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTURE_NAME5, &g_pTextureMeshField[4]);
	D3DXCreateTextureFromFile(pDevice, MESHFIELD_TEXTURE_NAME6, &g_pTextureMeshField[5]);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * g_MaxVtxMeshField,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL
	);

	// インデックスバッファ生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * g_MaxIdxMeshField,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL
	);

	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

						// 頂点バッファをロック
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMeshField = 0; nCntMeshField < g_MaxVtxMeshField; nCntMeshField++)
	{
		// 頂点情報の設定
		pVtx[nCntMeshField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 法線の設定
		pVtx[nCntMeshField].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// 頂点カラーの設定
		pVtx[nCntMeshField].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// テクスチャの設定
		pVtx[nCntMeshField].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMeshField->Unlock();

	WORD *pIdx;			// インデックスデータへのポインタ

						// インデックスバッファをロック
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (nCntMeshField = 0; nCntMeshField < g_MaxIdxMeshField; nCntMeshField++)
	{
		// インデックスの設定
		pIdx[nCntMeshField] = nCntMeshField;
	}

	// インデックスバッファをアンロック
	g_pIdxBuffMeshField->Unlock();
}