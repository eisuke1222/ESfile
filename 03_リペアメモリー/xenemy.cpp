//=========================================================================================================================
//
// 敵処理 [xenemy.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanfエラー解除
#include <stdio.h>					// stdio
#include "xenemy.h"
#include "xobject.h"
#include "xplayer.h"
#include "meshfield.h"
#include "shadow.h"
#include "camera.h"
#include "mark.h"
#include "fade.h"
#include "input.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_XENEMY				(10)						// 敵の最大数
#define MAX_XENEMY_NUM			(16)						// 各敵の最大数
#define XENEMY_UPSPPED			(0.3f)						// 敵加速速度
#define XENEMY_DOWNSPPED		(0.15f)						// 敵減速速度

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
XENEMYINI						g_XEnemyIni[MAX_XENEMY];
XENEMY							g_XEnemy[MAX_XENEMY][MAX_XENEMY_NUM];		// 敵情報
int								g_XEnemyNum;

//=========================================================================================================================
// 敵の初期化処理
//=========================================================================================================================
void InitXEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	int nCntXEnemy, nCntXEnemyParts;

	// ファイル設定
	pFile = fopen(XENEMY_TEXT_INFO, "r");

	// 敵テキストデータ読み込み
	if (pFile != NULL)
	{// ファイルが開けた場合
		fscanf(pFile, "%d", &g_XEnemyNum);	// 読み込み

		for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
		{
			fscanf(pFile, "%d", &g_XEnemyIni[nCntXEnemy].nSetNum);		// 読み込み
			fscanf(pFile, "%s", &g_XEnemyIni[nCntXEnemy].FileName[0]);	// 読み込み
		}

		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "設定ファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		// ファイル設定
		pFile = fopen(&g_XEnemyIni[nCntXEnemy].FileName[0], "r");

		// 敵テキストデータ読み込み
		if (pFile != NULL)
		{// ファイルが開けた場合
			fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nMaxPartsNum);	// 読み込み

			for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
			{
				fscanf(pFile, "%s", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].FileName[0]);	// 読み込み
			}

			for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
			{
				fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].nIndexParent);	// 読み込み
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initpos.x);	// 読み込み
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initpos.y);	// 読み込み
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initpos.z);	// 読み込み
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initrot.x);	// 読み込み
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initrot.y);	// 読み込み
				fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initrot.z);	// 読み込み
			}

			fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nMaxAnimNum);	// 読み込み

			for (int nCntXEnemyPartsAnim = 0; nCntXEnemyPartsAnim < g_XEnemy[nCntXEnemy][0].nMaxAnimNum; nCntXEnemyPartsAnim++)
			{
				fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nAnimloop[nCntXEnemyPartsAnim]);				// 読み込み
				fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nAnimkey[nCntXEnemyPartsAnim]);				// 読み込み

				for (int nCntXEnemyPartsAnimKey = 0; nCntXEnemyPartsAnimKey < g_XEnemy[nCntXEnemy][0].nAnimkey[nCntXEnemyPartsAnim]; nCntXEnemyPartsAnimKey++)
				{
					fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nAnimfram[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey]);			// 読み込み

					for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
					{
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].posAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].x);	// 読み込み
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].posAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].y);	// 読み込み
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].posAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].z);	// 読み込み
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].rotAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].x);	// 読み込み
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].rotAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].y);	// 読み込み
						fscanf(pFile, "%f", &g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].rotAnim[nCntXEnemyPartsAnim][nCntXEnemyPartsAnimKey].z);	// 読み込み
					}
				}
			}

			fclose(pFile);	// ファイルを閉じる
		}
		else
		{// ファイルが開けない場合
			MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
		}
	}

	// Xファイルの読み込み
	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
		{
			D3DXLoadMeshFromX
			(
				&g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].FileName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pBuffMat,
				NULL,
				&g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].nNumMat,
				&g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh
			);
		}
	}

	// 位置・向きの初期設定
	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		g_XEnemy[nCntXEnemy][0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XEnemy[nCntXEnemy][0].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XEnemy[nCntXEnemy][0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XEnemy[nCntXEnemy][0].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XEnemy[nCntXEnemy][0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XEnemy[nCntXEnemy][0].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_XEnemy[nCntXEnemy][0].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		g_XEnemy[nCntXEnemy][0].nCntTimerand = 0;
		g_XEnemy[nCntXEnemy][0].nCntTime = 0;
		g_XEnemy[nCntXEnemy][0].nAnimnow = ENEMYANIM_NEUTRAL;
		g_XEnemy[nCntXEnemy][0].nAnimChange = 0;
		g_XEnemy[nCntXEnemy][0].nAnimkeynow = 0;
		g_XEnemy[nCntXEnemy][0].nIdxShadow = 0;
		g_XEnemy[nCntXEnemy][0].nIdxMark = 0;
		g_XEnemy[nCntXEnemy][0].bIdxMarkUse = false;
		g_XEnemy[nCntXEnemy][0].bAttack = false;
		g_XEnemy[nCntXEnemy][0].bUse = false;

		for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
		{
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pos = g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].Initpos;
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].rot = g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].rotAnim[0][0];
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		}
	}

	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファのポインタ

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
		{
			// 頂点数の取得
			nNumVtx = g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh->GetFVF());

			// 頂点バッファをロック
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入
				{
					if (vtx.x > g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.x)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.x = vtx.x;
					}
					if (vtx.y > g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.y)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.y = vtx.y;
					}
					if (vtx.z > g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.z)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMax.z = vtx.z;
					}

					if (vtx.x < g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.x)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.x = vtx.x;
					}
					if (vtx.y < g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.y)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.y = vtx.y;
					}
					if (vtx.z < g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.z)
					{
						g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].vtxMin.z = vtx.z;
					}
				}

				pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
			}

			// 頂点バッファをアンロック
			g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh->UnlockVertexBuffer();
		}
	}

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		g_XEnemy[nCntXEnemy][0].vtxMin.x = -20.0f;
		g_XEnemy[nCntXEnemy][0].vtxMin.y = 0.0f;
		g_XEnemy[nCntXEnemy][0].vtxMin.z = -20.0f;
		g_XEnemy[nCntXEnemy][0].vtxMax.x = 20.0f;

		if (g_XEnemy[nCntXEnemy][0].nMaxPartsNum == 10)
		{
			g_XEnemy[nCntXEnemy][0].vtxMax.y = g_XEnemy[nCntXEnemy][0].aModel[0].pos.y + g_XEnemy[nCntXEnemy][0].aModel[1].pos.y + g_XEnemy[nCntXEnemy][0].aModel[1].vtxMax.y;
		}
		else if (g_XEnemy[nCntXEnemy][0].nMaxPartsNum == 15)
		{
			g_XEnemy[nCntXEnemy][0].vtxMax.y = g_XEnemy[nCntXEnemy][0].aModel[0].pos.y + g_XEnemy[nCntXEnemy][0].aModel[1].pos.y + g_XEnemy[nCntXEnemy][0].aModel[2].pos.y + g_XEnemy[nCntXEnemy][0].aModel[2].vtxMax.y;
		}
		g_XEnemy[nCntXEnemy][0].vtxMax.z = 20.0f;

		for (int nCntXEnemyNum = 1; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			g_XEnemy[nCntXEnemy][nCntXEnemyNum] = g_XEnemy[nCntXEnemy][0];
		}
	}
}

//=========================================================================================================================
// 敵の終了処理
//=========================================================================================================================
void UninitXEnemy(void)
{
	// 変数宣言
	int nCntXEnemy;
	int nCntXEnemyParts;

	// メッシュの開放
	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
		{
			if (g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh != NULL)
			{
				g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh->Release();
				g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pMesh = NULL;
			}
		}
	}

	// マテリアルの開放
	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][0].nMaxPartsNum; nCntXEnemyParts++)
		{
			if (g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pBuffMat != NULL)
			{
				g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pBuffMat->Release();
				g_XEnemy[nCntXEnemy][0].aModel[nCntXEnemyParts].pBuffMat = NULL;
			}
		}
	}
}

//=========================================================================================================================
// 敵の更新処理
//=========================================================================================================================
void UpdateXEnemy(void)
{
	int nCntXEnemy;
	Camera *pCamera = GetCamera();
	XPLAYER *pXPlayer = GetXPlayer();
	float fDiff = 0.0f;
	float frac = 0.0f;
	int nCntXEnemyParts;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
			{
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime++;

				g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos;

				/*if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand == 0)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;
					}

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y += D3DX_PI / 200.0f;

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x -= sinf(D3DX_PI * 0.0f + g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * XENEMY_UPSPPED;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z -= cosf(D3DX_PI * 0.0f + g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * XENEMY_UPSPPED;
				}*/

				// 移動値代入
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos += g_XEnemy[nCntXEnemy][nCntXEnemyNum].move;

				// 慣性処理
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x += (0.0f - g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x) * XENEMY_DOWNSPPED;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z += (0.0f - g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z) * XENEMY_DOWNSPPED;

				{
					// 目標向き調整
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y > D3DX_PI)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y -= D3DX_PI * 2.0f;
					}
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y < -D3DX_PI)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y += D3DX_PI * 2.0f;
					}
					// 差分設定
					fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y - g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y);

					// 差分向き調整
					if (fDiff > D3DX_PI)
					{
						fDiff -= D3DX_PI * 2.0f;
					}
					if (fDiff < -D3DX_PI)
					{
						fDiff += D3DX_PI * 2.0f;
					}
					// 現在向き設定
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y += fDiff * 0.1f;

					// 現在向き調整
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y > D3DX_PI)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y -= D3DX_PI * 2.0f;
					}
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y < -D3DX_PI)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y += D3DX_PI * 2.0f;
					}
				}

				if (pXPlayer->pos.x + 100 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 100 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
					pXPlayer->pos.z + 100 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 100 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
				{
					/*if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bIdxMarkUse == false)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nIdxMark = SetMark(D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, 80.0f, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z));	//マークを出す
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].bIdxMarkUse = true;
					}*/

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;
				}
				else
				{
					/*if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bIdxMarkUse == true)
					{
						DeleteMark(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nIdxMark);		//マークを消す
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].bIdxMarkUse = false;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initrot.y;
					}*/

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initrot.y;
				}

				// 移動値代入
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos += g_XEnemy[nCntXEnemy][nCntXEnemyNum].move;

				// 慣性処理
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x += (0.0f - g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x) * XENEMY_DOWNSPPED;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z += (0.0f - g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z) * XENEMY_DOWNSPPED;

				// オブジェクト当たり判定
				CollisionXObject(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax);

				CollisionMeshField(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax);

				/*if (CollisionXPlayer(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax) == true)
				{
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 1;

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;

					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_NEUTRAL)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;
					}
				}
				else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand != 0)
				{
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand++;

					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand == 60)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;
						}
					}
				}*/

				SetPostionShadow(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nIdxShadow, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos);	// 影位置更新

				if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange == 1)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime % 10 == 0)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 0;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow + 1) % g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow];
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 0;
					}
				}
				else
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime % g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow] == 0)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 0;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow + 1) % g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow];

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimloop[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow] != 1 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow] - 1)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK)
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = false;
							}

							//g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;
						}
					}
				}

				if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange == 1)
				{
					frac = (10 - g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime) * 1.0f;
				}
				else
				{
					frac = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow)] - g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime) * 1.0f;
				}

				g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].Initpos + g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posAnim[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow + 1) % (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow])];

				// 差分設定
				fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.x);
				// 現在向き設定
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.x += fDiff / frac;

				// 差分設定
				fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest.y - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.y);
				// 現在向き設定
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.y += fDiff / frac;

				// 差分設定
				fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.z);
				// 現在向き設定
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].pos.z += fDiff / frac;

				for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][nCntXEnemyNum].nMaxPartsNum; nCntXEnemyParts++)
				{
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotAnim[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow + 1) % (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow])];

					{
						// 目標向き調整
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.x > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.x -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.x < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.x += D3DX_PI * 2.0f;
						}
						// 差分設定
						fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x);

						// 差分向き調整
						if (fDiff > D3DX_PI)
						{
							fDiff -= D3DX_PI * 2.0f;
						}
						if (fDiff < -D3DX_PI)
						{
							fDiff += D3DX_PI * 2.0f;
						}
						// 現在向き設定
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x += fDiff / frac;

						// 現在向き調整
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x += D3DX_PI * 2.0f;
						}

						// 目標向き調整
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.y > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.y -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.y < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.y += D3DX_PI * 2.0f;
						}
						// 差分設定
						fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.y - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y);

						// 差分向き調整
						if (fDiff > D3DX_PI)
						{
							fDiff -= D3DX_PI * 2.0f;
						}
						if (fDiff < -D3DX_PI)
						{
							fDiff += D3DX_PI * 2.0f;
						}
						// 現在向き設定
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y += fDiff / frac;

						// 現在向き調整
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y += D3DX_PI * 2.0f;
						}

						// 目標向き調整
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.z > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.z -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.z < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.z += D3DX_PI * 2.0f;
						}
						// 差分設定
						fDiff = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rotDest.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z);

						// 差分向き調整
						if (fDiff > D3DX_PI)
						{
							fDiff -= D3DX_PI * 2.0f;
						}
						if (fDiff < -D3DX_PI)
						{
							fDiff += D3DX_PI * 2.0f;
						}
						// 現在向き設定
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z += fDiff / frac;

						// 現在向き調整
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z > D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z -= D3DX_PI * 2.0f;
						}
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z < -D3DX_PI)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z += D3DX_PI * 2.0f;
						}
					}
				}
			}
		}
	}
}

//=========================================================================================================================
// 敵の描画処理
//=========================================================================================================================
void DrawXEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;						//
	int nCntXEnemy;
	int nCntXEnemyParts;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll
				(
					&mtxRot,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z
				);

				D3DXMatrixMultiply
				(
					&g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld,
					&g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld,
					&mtxRot
				);

				// 位置を反映
				D3DXMatrixTranslation
				(
					&mtxTrans,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y,
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z
				);

				D3DXMatrixMultiply
				(
					&g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld,
					&g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld,
					&mtxTrans
				);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld);

				for (nCntXEnemyParts = 0; nCntXEnemyParts < g_XEnemy[nCntXEnemy][nCntXEnemyNum].nMaxPartsNum; nCntXEnemyParts++)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].nIndexParent < 0)
					{
						mtxParent = g_XEnemy[nCntXEnemy][nCntXEnemyNum].mtxWorld;
					}
					else
					{
						mtxParent = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].nIndexParent].mtxWorld;
					}

					// ワールドマトリックスの初期化
					D3DXMatrixIdentity(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld);

					// 回転を反映
					D3DXMatrixRotationYawPitchRoll
					(
						&mtxRot,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.y,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.x,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].rot.z
					);

					D3DXMatrixMultiply
					(
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&mtxRot
					);

					// 位置を反映
					D3DXMatrixTranslation
					(
						&mtxTrans,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].pos.x,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].pos.y,
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].pos.z
					);

					D3DXMatrixMultiply
					(
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&mtxTrans
					);

					D3DXMatrixMultiply
					(
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld,
						&mtxParent
					);

					// ワールドマトリックスの設定
					pDevice->SetTransform(D3DTS_WORLD, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].mtxWorld);

					// 現在のマテリアルを取得
					pDevice->GetMaterial(&matDef);

					// マテリアルデータへのポインタを取得
					pmat = (D3DXMATERIAL*)g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].pBuffMat->GetBufferPointer();

					for (int nCntMat = 0; nCntMat < (int)g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].nNumMat; nCntMat++)
					{
						// マテリアルの設定
						pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

						// テクスチャ設定
						pDevice->SetTexture(0, NULL);

						// 敵(パーツ)の描画
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[nCntXEnemyParts].pMesh->DrawSubset(nCntMat);
					}

					// マテリアルをデフォルトに戻す
					pDevice->SetMaterial(&matDef);
				}
			}
		}
	}
}

//=========================================================================================================================
// 敵の設定処理
//=========================================================================================================================
void SetXEnemy(ENEMYTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 Initrot, D3DXVECTOR3 rot)
{
	for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[type].nSetNum; nCntXEnemyNum++)
	{
		if (g_XEnemy[type][nCntXEnemyNum].bUse == false)
		{
			g_XEnemy[type][nCntXEnemyNum].pos = pos;
			g_XEnemy[type][nCntXEnemyNum].Initpos = pos;
			g_XEnemy[type][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XEnemy[type][nCntXEnemyNum].rot = rot;
			g_XEnemy[type][nCntXEnemyNum].rotDest = rot;
			g_XEnemy[type][nCntXEnemyNum].Initrot = Initrot;
			g_XEnemy[type][nCntXEnemyNum].nIdxShadow = SetShadow(g_XEnemy[type][nCntXEnemyNum].pos, g_XEnemy[type][nCntXEnemyNum].rot, 30.0f, 30.0f);
			g_XEnemy[type][nCntXEnemyNum].bUse = true;

			break;
		}
	}
}

//=========================================================================================================================
// 敵の取得処理
//=========================================================================================================================
XENEMY *GetXEnemy(void)
{
	return &g_XEnemy[0][0];
}

//=========================================================================================================================
// 敵の衝突処理
//=========================================================================================================================
bool CollisionXEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	int nCntXEnemy;
	bool bLand = false;

	//float vecA_x, vecA_z, vecC_x, vecC_z;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
			{// 使用されている

				/*if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x)
				{// X範囲確認
					vecA_x = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x) - (g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x);
					vecA_z = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z) - (g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z);
					vecC_x = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x) - (pPos->x);
					vecC_z = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z) - (pPos->z);

					if (((vecA_z * vecC_x) - (vecA_x * vecC_z)) < 0)
					{
						pPos->z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z;
						pMove->z = 0.0f;

						bLand = true;
					}
				}*/

			//	if (pPos->y + vtxMax->y > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y && pPos->y + vtxMin->y < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y)
				{// X範囲確認
					if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x)
					{// X範囲確認
						if (pPosOld->z + vtxMin->z >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z >= pPos->z + vtxMin->z)
						{// Z上底判定
							pPos->z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z - vtxMin->z;
							//pMove->x = 0.0f;
							pMove->z = 0.0f;

							bLand = true;
						}
						else if (pPosOld->z + vtxMax->z <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z <= pPos->z + vtxMax->z)
						{// Z下底判定
							pPos->z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z - vtxMax->z;
							//pMove->x = 0.0f;
							pMove->z = 0.0f;

							bLand = true;
						}
					}

					if (pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z)
					{// Z範囲確認
						if (pPosOld->x + vtxMin->x >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x >= pPos->x + vtxMin->x)
						{// X右辺判定
							pPos->x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x - vtxMin->x;
							pMove->x = 0.0f;
							//pMove->z = 0.0f;

							bLand = true;
						}
						else if (pPosOld->x + vtxMax->x <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x <= pPos->x + vtxMax->x)
						{// X左辺判定
							pPos->x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x - vtxMax->x;
							pMove->x = 0.0f;
							//pMove->z = 0.0f;

							bLand = true;
						}
					}
				}

			/*	if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x)
				{// X範囲確認
					if (pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z)
					{// Z範囲確認
						if (pPosOld->y + vtxMin->y >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y >= pPos->y + vtxMin->y)
						{// X右辺判定
							pPos->y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y - vtxMin->y;
							//pMove->x = 0.0f;
							//pMove->z = 0.0f;

							bLand = true;
						}
						else if (pPosOld->y + vtxMax->y <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y <= pPos->x + vtxMax->y)
						{// X左辺判定
							pPos->y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y - vtxMax->y;
							//pMove->x = 0.0f;
							//pMove->z = 0.0f;

							bLand = true;
						}
					}
				}*/
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// 敵を消す処理
//=========================================================================================================================
void DeleteXEnemy(void)
{
	//変数宣言
	int nCntXEnemy;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse = false;				//使用していない状態にする
			DeleteShadow(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nIdxShadow);	//影を消す
		}
	}
}