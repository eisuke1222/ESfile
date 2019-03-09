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
#include "xweapon.h"
#include "meshfield.h"
#include "billboardobject.h"
#include "billboardeffect.h"
#include "score.h"
#include "shadow.h"
#include "camera.h"
#include "fade.h"
#include "sound.h"
#include "input.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_XENEMY				(10)						// 敵の最大数
#define MAX_XENEMY_NUM			(16)						// 各敵の最大数
#define XENEMY_UPSPPED			(1.2f)						// 敵加速速度
#define XENEMY_DOWNSPPED		(0.15f)						// 敵減速速度

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
XENEMYINI						g_XEnemyIni[MAX_XENEMY];					// 読み込みデータ
XENEMY							g_XEnemy[MAX_XENEMY][MAX_XENEMY_NUM];		// 敵情報
int								g_XEnemyNum;								// 敵最大数
int								g_nSetingNum;								// 設定番号
int								g_RockOn, g_RockOnNum;						// ロックオン番号

//=========================================================================================================================
// 敵の初期化処理
//=========================================================================================================================
void InitXEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	int nCntXEnemy, nCntXEnemyParts;

	// 初期設定
	g_nSetingNum = 0;
	g_RockOn = 99;
	g_RockOnNum = 99;

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
				fscanf(pFile, "%d", &g_XEnemy[nCntXEnemy][0].nAnimBlend[nCntXEnemyPartsAnim]);				// 読み込み

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
		g_XEnemy[nCntXEnemy][0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_XEnemy[nCntXEnemy][0].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_XEnemy[nCntXEnemy][0].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		g_XEnemy[nCntXEnemy][0].nCntTime = 0;
		g_XEnemy[nCntXEnemy][0].nAnimChange= 0;
		g_XEnemy[nCntXEnemy][0].nAnimnow = ENEMYANIM_NEUTRAL;
		g_XEnemy[nCntXEnemy][0].nAnimkeynow = 0;
		g_XEnemy[nCntXEnemy][0].nIdxShadow = 0;
		g_XEnemy[nCntXEnemy][0].state = ENEMYSTATE_NONE;
		g_XEnemy[nCntXEnemy][0].nCntstate = 0;
		g_XEnemy[nCntXEnemy][0].nLife = 0;
		g_XEnemy[nCntXEnemy][0].nInitLife = 0;
		g_XEnemy[nCntXEnemy][0].nBillBoardHP = 0;
		g_XEnemy[nCntXEnemy][0].bAttack = false;
		g_XEnemy[nCntXEnemy][0].nSetAttack = 1;
		g_XEnemy[nCntXEnemy][0].nCntTimerand = 0;
		g_XEnemy[nCntXEnemy][0].Pattern = ENEMYPATTERN_NONE;
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
		g_XEnemy[nCntXEnemy][0].vtxMax.y = g_XEnemy[nCntXEnemy][0].aModel[0].pos.y + g_XEnemy[nCntXEnemy][0].aModel[1].pos.y + g_XEnemy[nCntXEnemy][0].aModel[2].pos.y + g_XEnemy[nCntXEnemy][0].aModel[2].vtxMax.y;
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
	GAMESTATE game = GetGameState();
	XPLAYER *pXPlayer = GetXPlayer();
	float fDiff = 0.0f;
	float frac = 0.0f;
	int nCntXEnemyParts;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
			{// 使っている
				if (game != GAMESTATE_TUTORIAL)
				{// チュートリアル以外
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack == false)
					{// 攻撃していない
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand++;

						/*switch (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern)
						{
						case ENEMYPATTERN_NONE:		// 何もしていない

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_NEUTRAL)
							{// 初期化
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;	// ニュートラル
								
							if (nCntXEnemy != ENEMYTYPE_BOSS)
							{// ボス以外
								if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > (rand() % 120 + 60) || g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > 240)
								{
									int nRand = rand() % 3;

									// 行動変更
									if (nRand == 0)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKINIT;
									}
									else if (nRand == 1)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKRANDOM;
									}
									else
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = (rand() % 628) * 0.01f;
									}

									g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
								}
							}
							else
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;
							}

							break;
						case ENEMYPATTERN_WALKINIT:		// 歩く準備

							if ((g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.x - 5.0f && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.x + 5.0f &&
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.z - 5.0f && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.z + 5.0f) ||
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > 240)
							{// 変更
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED);

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
							{// 初期化
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;		// 歩く

							break;
						case ENEMYPATTERN_WALKRANDOM:	// 移動ランダム

							if (rand() % 120 == 0)
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = (rand() % 628) * 0.01f;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED);

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > 120)
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
							}

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > (rand() % 60 + 60))
							{
								int nRand = rand() % 3;

								// 行動変更
								if (nRand == 0)
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKINIT;
								}
								else if (nRand == 1)
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKRANDOM;
								}
								else
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;

									g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = (rand() % 628) * 0.01f;
								}

								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
							}

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
							{// 初期化
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;			// 歩く

							break;
						case ENEMYPATTERN_ATTACKSET:	// 攻撃準備

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED * 2.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED * 2.0f);

							if (nCntXEnemy == ENEMYTYPE_BOSS)
							{// ボス
								if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack == 1)
								{
									if (pXPlayer->pos.x + 500 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 500 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 500 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 500 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACK;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
								else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack >= 2)
								{
									if (pXPlayer->pos.x + 800 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 800 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 800 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 800 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACK;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
							}
							else
							{// その他
								if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand > 300)
								{
									// 行動変更
									if (rand() % 2 == 0)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKINIT;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
									else
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKRANDOM;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}

								if (nCntXEnemy == 0)
								{
									if (pXPlayer->pos.x + 100 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 100 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 100 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 100 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACK;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
								else
								{
									if (pXPlayer->pos.x + 300 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 300 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 300 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 300 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACK;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
							}

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
							{// 初期化
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;		// 歩く

							break;
						case ENEMYPATTERN_ATTACK:	// 攻撃

							if (nCntXEnemy == ENEMYTYPE_BOSS)
							{// ボス
								if (pXPlayer->pos.x + 800 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 800 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
									pXPlayer->pos.z + 800 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 800 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
								}

								if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack == 1)
								{
									// 行動変更
									if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand < 30)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

										if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_NEUTRAL)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
										}

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;	// ニュートラル
									}
									else
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

										if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand == 180)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = 2;
										}

 										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED * 4.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED * 4.0f);

										if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
										}

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;	// 歩く


										if (pXPlayer->pos.x + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
											pXPlayer->pos.z + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

											if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_ATTACK)
											{
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
											}

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_ATTACK;	// 攻撃

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = true;

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
										}
									}
								}
								else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack >= 2)
								{
									// 行動変更
									if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand < 30)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

										if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_NEUTRAL)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
										}

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;	// ニュートラル
									}
									else
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

										if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_ATTACK2)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
										}

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_ATTACK2;	// 攻撃２

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = true;

										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
							}
							else
							{// その他
								if (nCntXEnemy == 0)
								{
									if (pXPlayer->pos.x + 200 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 200 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 200 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 200 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}
								else
								{
									if (pXPlayer->pos.x + 300 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 300 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
										pXPlayer->pos.z + 300 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 300 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
									{
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
									}
								}

								g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

								if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand < 38)
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

									if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_NEUTRAL)
									{// 初期化
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
									}

									g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;	// ニュートラル
								}
								else
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * XENEMY_UPSPPED);

									if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_WALK)
									{// 初期化
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
										g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
									}

									g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_WALK;	// 歩く

									if (nCntXEnemy == 0)
									{
										if (pXPlayer->pos.x + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
											pXPlayer->pos.z + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

											if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_ATTACK)
											{// 初期化
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
											}

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_ATTACK;	// 攻撃

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = true;

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
										}
									}
									else
									{
										if (pXPlayer->pos.x + 250 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 250 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
											pXPlayer->pos.z + 250 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 250 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
										{
											g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

											if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow != ENEMYANIM_ATTACK)
											{// 初期化
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 1;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
												g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 1;
											}

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_ATTACK;	// 攻撃

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = true;

											g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
										}
									}
								}
							}

							break;
						}*/
					}
				}

				switch (g_XEnemy[nCntXEnemy][nCntXEnemyNum].state)
				{
				case ENEMYSTATE_NORMAL:		// ノーマル
					break;
				case ENEMYSTATE_DAMAGE:		// ダメージ

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate++;

					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate % 5 % 2 == 0)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].col.a = 0.3f;
					}
					else
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].col.a = 1.0f;
					}

					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate > 13)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate = 0;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].state = ENEMYSTATE_NORMAL;		// ノーマルに戻す
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}

					break;
				case ENEMYSTATE_DIE:	
					break;
				}

				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime++;

				g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos;	// 位置保管

				if (nCntXEnemy == ENEMYTYPE_BOSS)
				{
					// 行動変更
					/*if (pXPlayer->Initpos.x + 1000 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.x && pXPlayer->pos.x - 1000 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.x &&
						pXPlayer->Initpos.z + 1000 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.z && pXPlayer->pos.z - 1000 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].Initpos.z)
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack == false)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern == ENEMYPATTERN_ATTACKSET ||
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern == ENEMYPATTERN_ATTACK)
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKINIT;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
							}

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = 0;
						}
					}
					else */if (pXPlayer->pos.x + 1000 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 1000 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
						pXPlayer->pos.z + 1000 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 1000 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern != ENEMYPATTERN_ATTACKSET &&
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern != ENEMYPATTERN_ATTACK)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;

							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = rand() % 3 + 1;
						}
					}
				}
				else
				{
					// 行動変更
					if (pXPlayer->pos.x + 500 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 500 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
						pXPlayer->pos.z + 500 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 500 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
					{
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest.y = atan2f(pXPlayer->pos.x - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, pXPlayer->pos.z - g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z) + D3DX_PI;

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern != ENEMYPATTERN_ATTACKSET &&
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern != ENEMYPATTERN_ATTACK)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
						}
					}
					else
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern == ENEMYPATTERN_ATTACKSET)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKRANDOM;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
						}
						else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern == ENEMYPATTERN_ATTACK)
						{
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_WALKINIT;
							g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
						}
					}
				}

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

				if (nCntXEnemy == 1)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack == true)
					{
						// 弓矢
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 1)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 1 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == 1)
							{
								SeBulletWeaponEnemy(
								D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41, g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._42 + 40, g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43),
								D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot),
								D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 15.0f));
							}
						}
					}
				}
				else if (nCntXEnemy == 2)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack == true)
					{
						// エフェクト
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow])
							{
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f),
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot,
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
							}
						}
					}
				}
				else if (nCntXEnemy == ENEMYTYPE_BOSS)
				{// ボス
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack == true)
					{
						// エフェクト
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack == 1)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow] ||
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 6 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow] ||
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 12 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow])
							{
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41 + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43 + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
							}
						}
						else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack == 2)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime == 1)
							{
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 2) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 2) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 2), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 2) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 2) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 2) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 2) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 2), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 2) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 2) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);

								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 5.0f, 0.0f, cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 4) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 5.0f, 0.0f, cosf(-g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 4) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
							}
						}
						else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack == 3)
						{
							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2 && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime % 2 == 0)
							{
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 8) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 8) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 8), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 8) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y - (D3DX_PI / 8) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
								SetBillboardBulletEffect(
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 8) + D3DX_PI) * 50.0f, 50, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 8) + D3DX_PI) * 50.0f),
									D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 8), g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.z),
									D3DXVECTOR3(sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 8) + D3DX_PI) * 5.0f, 0.0f, cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y + (D3DX_PI / 8) + D3DX_PI) * 5.0f),
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, 1, EFFECTBULLET_ENEMY);
							}
						}
					}
				}

				// オブジェクト当たり判定
				if (CollisionXObject(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax) == true)
				{
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
				}

				if (CollisionXWeapon(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax) == true ||
					CollisionXWeaponBullet(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax, WEAPON_PLAYER) == true)
				{// 武器当たり判定
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].state == ENEMYSTATE_NORMAL)
					{
						PlaySound(SOUND_LABEL_SE_DAMAGE);

						g_XEnemy[nCntXEnemy][nCntXEnemyNum].state = ENEMYSTATE_DAMAGE;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate = 0;

						if (game == GAMESTATE_PLAY)
						{
							if (pXPlayer->nAnimnow == PLAYERANIM_SWORD_1 ||
								pXPlayer->nAnimnow == PLAYERANIM_SWORD_2)
							{
								HitXEnemy(nCntXEnemy, nCntXEnemyNum, pXPlayer->nWeapon_1Lv / 3 + 1);
							}
							else if (pXPlayer->nAnimnow == PLAYERANIM_SWORD_3)
							{
								HitXEnemy(nCntXEnemy, nCntXEnemyNum, pXPlayer->nWeapon_1Lv / 3 + 2);
							}
							else
							{
								HitXEnemy(nCntXEnemy, nCntXEnemyNum, pXPlayer->nWeapon_2Lv / 3 + 1);
							}
						}

						g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
						
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife <= 0)
						{
							DeleteXEnemy(nCntXEnemy, nCntXEnemyNum);
						}
					}
				}

				// メッシュフィールド当たり判定
				CollisionMeshField(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax);
				
				// 敵同士
				Collision(nCntXEnemy, nCntXEnemyNum);

				SetXEnemyCollNum(nCntXEnemy, nCntXEnemyNum);

				// 攻撃当たり判定
				if (CollisionSword(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos) == true)
				{// 剣
					PlaySound(SOUND_LABEL_SE_DAMAGE);

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].state = ENEMYSTATE_DAMAGE;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate = 0;

					if (game == GAMESTATE_PLAY)
					{
						HitXEnemy(nCntXEnemy, nCntXEnemyNum, pXPlayer->nWeapon_1Lv / 3);
					}

					g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;

					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife <= 0)
					{
						DeleteXEnemy(nCntXEnemy, nCntXEnemyNum);
					}
				}

				if (CollisionEffect(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, EFFECTBULLET_PLAYER) == true)
				{// エフェクト
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].state == ENEMYSTATE_NORMAL)
					{
						PlaySound(SOUND_LABEL_SE_DAMAGE);

						g_XEnemy[nCntXEnemy][nCntXEnemyNum].state = ENEMYSTATE_DAMAGE;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate = 0;					

						g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_ATTACKSET;
						g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife <= 0)
						{
							DeleteXEnemy(nCntXEnemy, nCntXEnemyNum);
						}
					}
				}

				// プレイヤー当たり判定
				if (CollisionXPlayer(&g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].move, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin, &g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax) == true)
				{
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x -= sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -3.0f;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z -= cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -3.0f;
				}

				// 移動値代入
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos += g_XEnemy[nCntXEnemy][nCntXEnemyNum].move;

				// 慣性処理
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x += (0.0f - g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x) * XENEMY_DOWNSPPED;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z += (0.0f - g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z) * XENEMY_DOWNSPPED;

				SetPosionHP(D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + 80, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z),
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].nBillBoardHP);
				
				// 影
				D3DXVECTOR3 Shadowpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				Shadowpos.x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].mtxWorld._41;
				Shadowpos.y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].mtxWorld._42;
				Shadowpos.z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].mtxWorld._43;

				SetPositionShadow(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nIdxShadow, Shadowpos, g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot);	// 影位置更新

				// アニメーション
				if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange == 1)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime % g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimBlend[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow] == 0)
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

							if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK ||
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK2)
							{
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = false;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
								g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = 1;

								if (nCntXEnemy == ENEMYTYPE_BOSS)
								{
									g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = rand() % 3 + 1;
								}
							}

							//g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;
						}
					}
				}

				// ブレンド
				if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange == 1)
				{
					frac = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimBlend[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow] - g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime) * 1.0f;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].Initpos + g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posAnim[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow];
				}
				else
				{
					frac = (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimfram[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow)] - g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime) * 1.0f;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posDest = g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].Initpos + g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[0].posAnim[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow][(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow + 1) % (g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkey[g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow])];
				}

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
						pmat[nCntMat].MatD3D.Diffuse.a = g_XEnemy[nCntXEnemy][nCntXEnemyNum].col.a;
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
void SetXEnemy(int type, D3DXVECTOR3 pos)
{
	GAMESTATE game = GetGameState();
	int nNowMap = GetNowMap();

	for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[type].nSetNum; nCntXEnemyNum++)
	{
		if (g_XEnemy[type][nCntXEnemyNum].bUse == false)
		{
			if (g_RockOn == 99 && g_RockOnNum == 99)
			{
				g_RockOn = type;
				g_RockOnNum = nCntXEnemyNum;
			}

			// 位置
			g_XEnemy[type][nCntXEnemyNum].pos = pos;
			g_XEnemy[type][nCntXEnemyNum].Initpos = pos;
			g_XEnemy[type][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 向き
			if (game == GAMESTATE_TUTORIAL)
			{
				g_XEnemy[type][nCntXEnemyNum].rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
				g_XEnemy[type][nCntXEnemyNum].rotDest = g_XEnemy[type][nCntXEnemyNum].rot;
			}
			else
			{
				g_XEnemy[type][nCntXEnemyNum].rot = g_XEnemy[type][nCntXEnemyNum].Initrot;
				g_XEnemy[type][nCntXEnemyNum].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			
			// 初期設定
			g_XEnemy[type][nCntXEnemyNum].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_XEnemy[type][nCntXEnemyNum].nIdxShadow = SetShadow(g_XEnemy[type][nCntXEnemyNum].pos, g_XEnemy[type][nCntXEnemyNum].rot,
				D3DXVECTOR3(30.0f, 0.0f, 30.0f));
			g_XEnemy[type][nCntXEnemyNum].nCntTimerand = 0;
			g_XEnemy[type][nCntXEnemyNum].nCntTime = 0;
			g_XEnemy[type][nCntXEnemyNum].state = ENEMYSTATE_NORMAL;
			g_XEnemy[type][nCntXEnemyNum].nCntstate = 0;

			// ライフ
			if (type == ENEMYTYPE_BOSS)
			{
				g_XEnemy[type][nCntXEnemyNum].nLife = 40;
				g_XEnemy[type][nCntXEnemyNum].nInitLife = 40;
			}
			else
			{
				if (nNowMap <= 3)
				{
					g_XEnemy[type][nCntXEnemyNum].nLife = 10;
					g_XEnemy[type][nCntXEnemyNum].nInitLife = 10;
				}
				else
				{
					g_XEnemy[type][nCntXEnemyNum].nLife = 20;
					g_XEnemy[type][nCntXEnemyNum].nInitLife = 20;
				}
			}

			g_XEnemy[type][nCntXEnemyNum].nBillBoardHP = SetHPNum(D3DXVECTOR3(g_XEnemy[type][nCntXEnemyNum].pos.x, g_XEnemy[type][nCntXEnemyNum].pos.y + 80, g_XEnemy[type][nCntXEnemyNum].pos.z), 
				D3DXVECTOR3(40, 10, 0.0f));
			g_XEnemy[type][nCntXEnemyNum].nAnimChange = 0;
			g_XEnemy[type][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;
			g_XEnemy[type][nCntXEnemyNum].nAnimkeynow = 0;
			g_XEnemy[type][nCntXEnemyNum].bAttack = false;
			g_XEnemy[type][nCntXEnemyNum].nSetAttack = 1;
			g_XEnemy[type][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;
			g_XEnemy[type][nCntXEnemyNum].bUse = true;

			g_nSetingNum++;

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
// 敵の総数取得処理
//=========================================================================================================================
int GetXEnemyNum(void)
{
	return g_nSetingNum;
}

//=========================================================================================================================
// 敵のロックオン番号取得処理
//=========================================================================================================================
D3DXVECTOR3 GetRockOnXEnemy(int nNum)
{
	XPLAYER *pXPlayer = GetXPlayer();
	D3DXVECTOR3 pos1, pos2;

	int nFor = 0;
	int nCntXEnemy;

	// ロックオン番号調べ
	if (nNum == 0)
	{
		for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
		{
			if (nFor == 0)
			{
				for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
				{
					if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
					{// 使用されている
						pos1.x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x - pXPlayer->pos.x;
						pos1.z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z - pXPlayer->pos.z;

						if (pos1.x < 0)
						{
							pos1.x *= -1;
						}
						if (pos1.z < 0)
						{
							pos1.z *= -1;
						}
						g_RockOn = nCntXEnemy;
						g_RockOnNum = nCntXEnemyNum;

						nFor = 1;

						break;
					}
				}
			}
		}

		for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
		{
			for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
			{
				if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
				{// 使用されている
					pos2.x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x - pXPlayer->pos.x;
					pos2.z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z - pXPlayer->pos.z;

					if (pos2.x < 0)
					{
						pos2.x *= -1;
					}

					if (pos2.z < 0)
					{
						pos2.z *= -1;
					}

					if (pos1.x >= pos2.x &&
						pos1.z >= pos2.z)
					{
						g_RockOn = nCntXEnemy;
						g_RockOnNum = nCntXEnemyNum;
					}
				}
			}
		}
	}

	if (g_XEnemy[g_RockOn][g_RockOnNum].bUse == false)
	{// 使用されている
		SetCancelRockOn();
	}

	return g_XEnemy[g_RockOn][g_RockOnNum].pos;
}

//=========================================================================================================================
// 敵のロックオン変更取得処理
//=========================================================================================================================
void ChangeRockOnXEnemy(int nNum)
{
	int nCntXEnemy;
	bool bChange = false;

	if (nNum == 0)
	{
		for (nCntXEnemy = g_RockOn; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
		{
			if (bChange == false)
			{
				if (nCntXEnemy == g_RockOn)
				{
					for (int nCntXEnemyNum = g_RockOnNum + 1; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// 使用されている
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
				else
				{
					for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// 使用されている
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
			}
		}

		if (bChange == false)
		{
			for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
			{
				if (bChange == false)
				{
					for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
					{
						if (nCntXEnemy == g_RockOn && nCntXEnemyNum == g_RockOnNum)
						{
							break;
						}

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// 使用されている
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
			}
		}
	}
	if (nNum == 1)
	{
		for (nCntXEnemy = g_RockOn; nCntXEnemy >= 0; nCntXEnemy--)
		{
			if (bChange == false)
			{
				if (nCntXEnemy == g_RockOn)
				{
					for (int nCntXEnemyNum = g_RockOnNum - 1; nCntXEnemyNum >= 0; nCntXEnemyNum--)
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// 使用されている
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
				else
				{
					for (int nCntXEnemyNum = g_XEnemyIni[nCntXEnemy].nSetNum - 1; nCntXEnemyNum >= 0; nCntXEnemyNum--)
					{
						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// 使用されている
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
			}
		}

		if (bChange == false)
		{
			for (nCntXEnemy = g_XEnemyNum - 1; nCntXEnemy >= 0; nCntXEnemy--)
			{
				if (bChange == false)
				{
					for (int nCntXEnemyNum = g_XEnemyIni[nCntXEnemy].nSetNum - 1; nCntXEnemyNum >= 0; nCntXEnemyNum--)
					{
						if (nCntXEnemy == g_RockOn && nCntXEnemyNum == g_RockOnNum)
						{
							break;
						}

						if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
						{// 使用されている
							g_RockOn = nCntXEnemy;
							g_RockOnNum = nCntXEnemyNum;

							bChange = true;

							break;
						}
					}
				}
			}
		}
	}
}

//=========================================================================================================================
// 敵の衝突処理
//=========================================================================================================================
bool Collision(int nCntXEnemy1, int nCntXEnemyNum1)
{
	int nCntXEnemy;
	int nCntHit = 0;
	bool bLand = false;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true && (nCntXEnemy != nCntXEnemy1 || nCntXEnemyNum != nCntXEnemyNum1))
			{// 使用されている
			 //if (pPos->y + vtxMax->y > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y && pPos->y + vtxMin->y < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y)
				{// X範囲確認
					if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x)
					{// X範囲確認
						if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].posOld.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.z >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z >= g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.z)
						{// Z上底判定
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z - g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.z;
							//pMove->x = 0.0f;
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].move.z *= -1;

							nCntHit++;

							bLand = true;
						}
						else if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].posOld.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.z <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z <= g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.z)
						{// Z下底判定
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z - g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.z;
							//pMove->x = 0.0f;
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].move.z *= -1;

							nCntHit++;

							bLand = true;
						}
					}

					if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.z + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z)
					{// Z範囲確認
						if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].posOld.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.x >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x >= g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.x)
						{// X右辺判定
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x - g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMin.x;
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].move.x *= -1;
							//pMove->z = 0.0f;

							nCntHit++;

							bLand = true;
						}
						else if (g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].posOld.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.x <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x <= g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x + g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.x)
						{// X左辺判定
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].pos.x = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x - g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].vtxMax.x;
							g_XEnemy[nCntXEnemy1][nCntXEnemyNum1].move.x *= -1;
							//pMove->z = 0.0f;

							nCntHit++;

							bLand = true;
						}
					}
				}

				/*if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x)
				{// X範囲確認
				if (pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z)
				{// Z範囲確認
				if (pPosOld->y + vtxMin->y >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y > pPos->y + vtxMin->y)
				{// X右辺判定
				pPos->y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y - vtxMin->y;
				//pMove->x = 0.0f;
				//pMove->z = 0.0f;

				bLand = true;
				}
				else if (pPosOld->y + vtxMax->y <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y < pPos->x + vtxMax->y)
				{// X左辺判定
				pPos->y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y - vtxMax->y;
				//pMove->x = 0.0f;
				//pMove->z = 0.0f;

				bLand = true;
				}
				}
				}*/


				if (bLand == true && nCntHit % 2 == 0)
				{
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.x -= sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -1.2f;
					g_XEnemy[nCntXEnemy][nCntXEnemyNum].move.z -= cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -1.2f;
				}
			}
		}
	}

	return bLand;
}
bool CollisionXEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	int nCntXEnemy;
	bool bLand = false;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
			{// 使用されている
				//if (pPos->y + vtxMax->y > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y && pPos->y + vtxMin->y < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y)
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

				/*if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.x && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.x)
				{// X範囲確認
					if (pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.z && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.z)
					{// Z範囲確認
						if (pPosOld->y + vtxMin->y >= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y > pPos->y + vtxMin->y)
						{// X右辺判定
							pPos->y = g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMax.y - vtxMin->y;
							//pMove->x = 0.0f;
							//pMove->z = 0.0f;

							bLand = true;
						}
						else if (pPosOld->y + vtxMax->y <= g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y && g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.y + g_XEnemy[nCntXEnemy][nCntXEnemyNum].vtxMin.y < pPos->x + vtxMax->y)
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
// 武器の衝突判定処理
//=========================================================================================================================
bool CollisionXEnemyWeapon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	XPLAYER *pXPlayer = GetXPlayer();
	int nCntXEnemy;
	bool bLand = false;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true && nCntXEnemy == 0 &&
				((g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2)))
			{// 使用されている
			 //if (pPos->y + vtxMax->y > pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMin.y && pPos->y + vtxMin->y < pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMax.y)
				{// X範囲確認
					//if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41 - 10.0f && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41 + 10.0f &&
					//	pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43 - 10.0f && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43 + 10.0f)
					if (pXPlayer->pos.x + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
						pXPlayer->pos.z + 80 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 80 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)					
					{// X範囲確認

						pMove->x = sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -5.0f;
						pMove->z = cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -5.0f;

						bLand = true;
					}
				}
			}
			else if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true && nCntXEnemy == ENEMYTYPE_BOSS &&
				((g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 2) ||
				(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 6) ||
					(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow == ENEMYANIM_ATTACK && g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow == 12)))
			{// 使用されている
			 //if (pPos->y + vtxMax->y > pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMin.y && pPos->y + vtxMin->y < pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMax.y)
				{// X範囲確認
				 //if (pPos->x + vtxMax->x > g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41 - 10.0f && pPos->x + vtxMin->x < g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._41 + 10.0f &&
				 //	pPos->z + vtxMax->z > g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43 - 10.0f && pPos->z + vtxMin->z < g_XEnemy[nCntXEnemy][nCntXEnemyNum].aModel[14].mtxWorld._43 + 10.0f)
					if (pXPlayer->pos.x + 150 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x && pXPlayer->pos.x - 150 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x &&
						pXPlayer->pos.z + 150 > g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z && pXPlayer->pos.z - 150 < g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z)
					{// X範囲確認

						pMove->x = sinf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -3.0f;
						pMove->z = cosf(g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot.y) * -3.0f;

						bLand = true;
					}
				}
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// 敵の当たり処理
//=========================================================================================================================
void HitXEnemy(int nCntXEnemy, int nCntXEnemyNum, int nDamage)
{
	if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
	{
		g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife -= nDamage;

		SetUpdateHP(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nBillBoardHP, g_XEnemy[nCntXEnemy][nCntXEnemyNum].nInitLife, g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife);
	}
}

//=========================================================================================================================
// 敵の破棄処理
//=========================================================================================================================
void DeleteXEnemy(int nCntXEnemy, int nCntXEnemyNum)
{
	int nNowMap = GetNowMap();

	g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse = false;
	DeleteShadow(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nIdxShadow);
	DeleteBillboardObject(g_XEnemy[nCntXEnemy][nCntXEnemyNum].nBillBoardHP);
	g_nSetingNum--;

	if (nCntXEnemy == ENEMYTYPE_BOSS)
	{
		AddScore(SCORETYPE_SCORE, 5000);
	}
	else if (nNowMap >= 4)
	{
		AddScore(SCORETYPE_SCORE, 3000);
	}
	else
	{
		AddScore(SCORETYPE_SCORE, 1500);
	}

	if (g_RockOn == nCntXEnemy && g_RockOnNum == nCntXEnemyNum)
	{// 使用されている
		SetCancelRockOn();
	}

	if (rand() % 2 == 0)
	{
		if (nCntXEnemy == 0)
		{
			SetBillboardObject(D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, 40, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_WEAPON_1);
		}
		else if (nCntXEnemy == 1)
		{
			SetBillboardObject(D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, 40, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_WEAPON_2);
		}
		else if (nCntXEnemy == 2)
		{
			SetBillboardObject(D3DXVECTOR3(g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.x, 40, g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos.z),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_WEAPON_3);
		}
	}

	if (g_nSetingNum <= 0 && nNowMap >= 5)
	{
		SetGameState(GAMESTATE_GAMECLEAR);
	}
}
void DeleteXEnemy(void)
{
	int nCntXEnemy;

	g_nSetingNum = 0;
	g_RockOn = 99;
	g_RockOnNum = 99;

	for (nCntXEnemy = 0; nCntXEnemy < g_XEnemyNum; nCntXEnemy++)
	{
		for (int nCntXEnemyNum = 0; nCntXEnemyNum < g_XEnemyIni[nCntXEnemy].nSetNum; nCntXEnemyNum++)
		{
			if (g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse == true)
			{// 使用されている
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTimerand = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntTime = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimChange = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimnow = ENEMYANIM_NEUTRAL;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nAnimkeynow = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nIdxShadow = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].state = ENEMYSTATE_NONE;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nCntstate = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nLife = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nInitLife = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nBillBoardHP = 0;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].bAttack = false;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].nSetAttack = 1;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].Pattern = ENEMYPATTERN_NONE;
				g_XEnemy[nCntXEnemy][nCntXEnemyNum].bUse = false;
			}
		}
	}
}