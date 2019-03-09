//=========================================================================================================================
//
// オブジェクト処理 [xobject.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanfエラー解除
#include <stdio.h>					// stdio
#include "xobject.h"
#include "shadow.h"
#include "meshfield.h"
#include "fade.h"
#include "camera.h"
#include "input.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_XOBJECT				(20)						// オブジェクトの最大数
#define MAX_XOBJECT_NUM			(64)						// 各オブジェクトの最大数
#define XOBJECT_UPSPPED			(0.7f)						// オブジェクト加速速度
#define XOBJECT_DOWNSPPED		(0.2f)						// オブジェクト減速速度

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
XOBJECTINI						g_XObjectIni[MAX_XOBJECT];					// 読み込みデータ
XOBJECT							g_XObject[MAX_XOBJECT][MAX_XOBJECT_NUM];	// オブジェクトデータ
int								g_MaxXObject;								// オブジェクトの最大数

//=========================================================================================================================
// オブジェクトの初期化処理
//=========================================================================================================================
void InitXObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	int nCntXObject, nCntXObjectModel;

	// 初期設定
	g_MaxXObject = 0;

	// ファイル設定
	pFile = fopen(XOBJECT_TEXT_INFO, "r");

	if (pFile != NULL)
	{// ファイルが開けた場合
		fscanf(pFile, "%d", &g_MaxXObject);	// 読み込み

		for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
		{
			fscanf(pFile, "%s", &g_XObjectIni[nCntXObject].FileName[0]);	// 読み込み
		}

		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "設定ファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}

	// オブジェクトテキストデータ読み込み
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		// ファイル設定
		pFile = fopen(&g_XObjectIni[nCntXObject].FileName[0], "r");

		if (pFile != NULL)
		{// ファイルが開けた場合
			fscanf(pFile, "%d", &g_XObject[nCntXObject][0].nMaxModel);	// 読み込み

			for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
			{
				fscanf(pFile, "%s", &g_XObject[nCntXObject][nCntXObjectModel].FileName[0]);	// 読み込み
			}

			for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
			{
				fscanf(pFile, "%d", &g_XObject[nCntXObject][nCntXObjectModel].nOya);		// 読み込み
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initpos.x);	// 読み込み
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initpos.y);	// 読み込み
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initpos.z);	// 読み込み
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initrot.x);	// 読み込み
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initrot.y);	// 読み込み
				fscanf(pFile, "%f", &g_XObject[nCntXObject][nCntXObjectModel].Initrot.z);	// 読み込み
			}

			fclose(pFile);	// ファイルを閉じる
		}
		else
		{// ファイルが開けない場合
			MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
		}
	}

	// Xファイルの読み込み
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		D3DXLoadMeshFromX
		(
			&g_XObject[nCntXObject][0].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_XObject[nCntXObject][0].pBuffMat,
			NULL,
			&g_XObject[nCntXObject][0].nNumMat,
			&g_XObject[nCntXObject][0].pMesh
		);

		D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
		D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
		pmat = (D3DXMATERIAL*)g_XObject[nCntXObject][0].pBuffMat->GetBufferPointer();
		matDef = new D3DMATERIAL9[g_XObject[nCntXObject][0].nNumMat];
		g_XObject[nCntXObject][0].pTexture = new LPDIRECT3DTEXTURE9[g_XObject[nCntXObject][0].nNumMat];

		for (DWORD tex = 0; tex < g_XObject[nCntXObject][0].nNumMat; tex++)
		{
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			g_XObject[nCntXObject][0].pTexture[tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&g_XObject[nCntXObject][0].pTexture[tex])))
				{
					MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
				}
			}
		}

		delete matDef;
	}

	// 位置・向きの初期設定
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{
			g_XObject[nCntXObject][nCntXObjectModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
			g_XObject[nCntXObject][nCntXObjectModel].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
			g_XObject[nCntXObject][nCntXObjectModel].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow = 0;
			g_XObject[nCntXObject][nCntXObjectModel].bUse = false;
		}
	}

	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファのポインタ

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		// 頂点数の取得
		nNumVtx = g_XObject[nCntXObject][0].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_XObject[nCntXObject][0].pMesh->GetFVF());

		// 頂点バッファをロック
		g_XObject[nCntXObject][0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

			if (vtx.x > g_XObject[nCntXObject][0].vtxMax.x)
			{
				g_XObject[nCntXObject][0].vtxMax.x = vtx.x;
			}
			if (vtx.y > g_XObject[nCntXObject][0].vtxMax.y)
			{
				g_XObject[nCntXObject][0].vtxMax.y = vtx.y;
			}
			if (vtx.z > g_XObject[nCntXObject][0].vtxMax.z)
			{
				g_XObject[nCntXObject][0].vtxMax.z = vtx.z;
			}

			if (vtx.x < g_XObject[nCntXObject][0].vtxMin.x)
			{
				g_XObject[nCntXObject][0].vtxMin.x = vtx.x;
			}
			if (vtx.y < g_XObject[nCntXObject][0].vtxMin.y)
			{
				g_XObject[nCntXObject][0].vtxMin.y = vtx.y;
			}
			if (vtx.z < g_XObject[nCntXObject][0].vtxMin.z)
			{
				g_XObject[nCntXObject][0].vtxMin.z = vtx.z;
			}

			pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
		}

		// 頂点バッファをアンロック
		g_XObject[nCntXObject][0].pMesh->UnlockVertexBuffer();
	}

	// オブジェクト複製
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		for (nCntXObjectModel = 1; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{
			g_XObject[nCntXObject][nCntXObjectModel].pMesh = g_XObject[nCntXObject][0].pMesh;
			g_XObject[nCntXObject][nCntXObjectModel].pBuffMat = g_XObject[nCntXObject][0].pBuffMat;
			g_XObject[nCntXObject][nCntXObjectModel].nNumMat = g_XObject[nCntXObject][0].nNumMat;
			g_XObject[nCntXObject][nCntXObjectModel].pTexture = g_XObject[nCntXObject][0].pTexture;
			g_XObject[nCntXObject][nCntXObjectModel].vtxMin = g_XObject[nCntXObject][0].vtxMin;
			g_XObject[nCntXObject][nCntXObjectModel].vtxMax = g_XObject[nCntXObject][0].vtxMax;
		}
	}
}

//=========================================================================================================================
// オブジェクトの終了処理
//=========================================================================================================================
void UninitXObject(void)
{
	// 変数宣言
	int nCntXObject;

	// メッシュの開放
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// オブジェクトをカウント
		if (g_XObject[nCntXObject][0].pMesh != NULL)
		{// NULL以外の場合
			g_XObject[nCntXObject][0].pMesh->Release();
			g_XObject[nCntXObject][0].pMesh = NULL;
		}
	}

	// マテリアルの開放
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// オブジェクトをカウント
		if (g_XObject[nCntXObject][0].pBuffMat != NULL)
		{// NULL以外の場合
			g_XObject[nCntXObject][0].pBuffMat->Release();
			g_XObject[nCntXObject][0].pBuffMat = NULL;
		}
	}

	// テクスチャの開放
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// オブジェクトをカウント
		for (DWORD tex = 0; tex < g_XObject[nCntXObject][0].nNumMat; tex++)
		{// マテリアルをカウント
			if (g_XObject[nCntXObject][0].pTexture[tex] != NULL)
			{// NULL以外の場合
				g_XObject[nCntXObject][0].pTexture[tex]->Release();
				g_XObject[nCntXObject][0].pTexture[tex] = NULL;
			}
		}
		if (g_XObject[nCntXObject][0].pTexture != NULL)
		{
			delete[] g_XObject[nCntXObject][0].pTexture;
			g_XObject[nCntXObject][0].pTexture;
		}
	}
}

//=========================================================================================================================
// オブジェクトの更新処理
//=========================================================================================================================
void UpdateXObject(void)
{
	// 変数宣言
	int nCntXObject, nCntXObjectModel;

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// オブジェクトをカウント
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// モデルをカウント
			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
			{// 使用している場合
				SetPositionShadow(g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow, g_XObject[nCntXObject][nCntXObjectModel].pos, g_XObject[nCntXObject][nCntXObjectModel].rot);
			}
		}
	}
}

//=========================================================================================================================
// オブジェクトの描画処理
//=========================================================================================================================
void DrawXObject(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
	int nCntXObject, nCntXObjectModel;

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// オブジェクトをカウント
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// モデルをカウント
			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
			{// 使用している場合
				if (g_XObject[nCntXObject][nCntXObjectModel].nOya < 0)
				{
					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&g_XObject[nCntXObject][nCntXObjectModel].mtxWorld);
				}
				else
				{
					g_XObject[nCntXObject][nCntXObjectModel].mtxWorld = g_XObject[nCntXObject][g_XObject[nCntXObject][nCntXObjectModel].nOya].mtxWorld;
				}

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll
				(
					&mtxRot,
					g_XObject[nCntXObject][nCntXObjectModel].rot.y,
					g_XObject[nCntXObject][nCntXObjectModel].rot.x,
					g_XObject[nCntXObject][nCntXObjectModel].rot.z
				);

				D3DXMatrixMultiply
				(
					&g_XObject[nCntXObject][nCntXObjectModel].mtxWorld,
					&g_XObject[nCntXObject][nCntXObjectModel].mtxWorld,
					&mtxRot
				);

				// 位置を反映
				D3DXMatrixTranslation
				(
					&mtxTrans,
					g_XObject[nCntXObject][nCntXObjectModel].pos.x,
					g_XObject[nCntXObject][nCntXObjectModel].pos.y,
					g_XObject[nCntXObject][nCntXObjectModel].pos.z
				);

				D3DXMatrixMultiply
				(
					&g_XObject[nCntXObject][nCntXObjectModel].mtxWorld,
					&g_XObject[nCntXObject][nCntXObjectModel].mtxWorld,
					&mtxTrans
				);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_XObject[nCntXObject][nCntXObjectModel].mtxWorld);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアルデータへのポインタを取得
				pmat = (D3DXMATERIAL*)g_XObject[nCntXObject][nCntXObjectModel].pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_XObject[nCntXObject][nCntXObjectModel].nNumMat; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

					// テクスチャ設定
					pDevice->SetTexture(0, g_XObject[nCntXObject][nCntXObjectModel].pTexture[nCntMat]);
					//pDevice->SetTexture(0, NULL);

					// オブジェクト(パーツ)の描画
					g_XObject[nCntXObject][nCntXObjectModel].pMesh->DrawSubset(nCntMat);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

//=========================================================================================================================
// オブジェクトの設定処理
//=========================================================================================================================
void SetXObject(TEXTTYPE type)
{
	// 変数宣言
	int nCntXObjectModel;

	for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[type][0].nMaxModel; nCntXObjectModel++)
	{// モデルをカウント
		if (g_XObject[type][nCntXObjectModel].bUse == false)
		{// 使用している場合
			g_XObject[type][nCntXObjectModel].pos = g_XObject[type][nCntXObjectModel].Initpos;
			g_XObject[type][nCntXObjectModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[type][nCntXObjectModel].rot = g_XObject[type][nCntXObjectModel].Initrot;
			g_XObject[type][nCntXObjectModel].nIdxShadow = SetShadow(g_XObject[type][nCntXObjectModel].pos, g_XObject[type][nCntXObjectModel].rot, g_XObject[type][nCntXObjectModel].vtxMax);
			g_XObject[type][nCntXObjectModel].bUse = true;
		}
	}
}

//=========================================================================================================================
// オブジェクトの衝突処理
//=========================================================================================================================
bool CollisionXObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	// 変数宣言
	int nCntXObject, nCntXObjectModel;
	bool bLand = false;

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// オブジェクトをカウント
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// モデルをカウント
			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
			{// 使用されている
				//if (pPos->y + vtxMax->y > g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.y && pPos->y + vtxMin->y < g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y)
				{// X範囲確認
					if (pPos->x + vtxMax->x > g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x && pPos->x + vtxMin->x < g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x)
					{// X範囲確認
						if (pPosOld->z + vtxMin->z >= g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z && g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z >= pPos->z + vtxMin->z)
						{// Z上底判定
							pPos->z = g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z - vtxMin->z;
							//pMove->x = 0.0f;
							pMove->z = 0.0f;
						}
						else if (pPosOld->z + vtxMax->z <= g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z && g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z <= pPos->z + vtxMax->z)
						{// Z下底判定
							pPos->z = g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z - vtxMax->z;
							//pMove->x = 0.0f;
							pMove->z = 0.0f;
						}
					}

					if (pPos->z + vtxMax->z > g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z && pPos->z + vtxMin->z < g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z)
					{// Z範囲確認
						if (pPosOld->x + vtxMin->x >= g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x && g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x >= pPos->x + vtxMin->x)
						{// X右辺判定
							pPos->x = g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x - vtxMin->x;
							pMove->x = 0.0f;
							//pMove->z = 0.0f;
						}
						else if (pPosOld->x + vtxMax->x <= g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x && g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x <= pPos->x + vtxMax->x)
						{// X左辺判定
							pPos->x = g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x - vtxMax->x;
							pMove->x = 0.0f;
							//pMove->z = 0.0f;
						}
					}
				}

				/*if (pPos->x + vtxMax->x > g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x && pPos->x + vtxMin->x < g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x)
				{// X範囲確認
				if (pPos->z + vtxMax->z > g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z && pPos->z + vtxMin->z < g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z)
				{// Z範囲確認
				if (pPosOld->y + vtxMin->y >= g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y && g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y > pPos->y + vtxMin->y)
				{// X右辺判定
				pPos->y = g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y - vtxMin->y;
				//pMove->x = 0.0f;
				//pMove->z = 0.0f;
				}
				else if (pPosOld->y + vtxMax->y <= g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.y && g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.y < pPos->x + vtxMax->y)
				{// X左辺判定
				pPos->y = g_XObject[nCntXObject][nCntXObjectModel].pos.y + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.y - vtxMax->y;
				//pMove->x = 0.0f;
				//pMove->z = 0.0f;
				}
				}
				}*/
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// オブジェクトの破棄処理
//=========================================================================================================================
void DeleteXObject(void)
{
	// 変数宣言
	int nCntXObject, nCntXObjectModel;

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// オブジェクトをカウント
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// モデルをカウント
			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
			{// 使用されている
				g_XObject[nCntXObject][nCntXObjectModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow = 0;
				g_XObject[nCntXObject][nCntXObjectModel].bUse = false;
			}
		}
	}
}