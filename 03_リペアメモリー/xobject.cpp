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
#include "camera.h"
#include "questfade.h"
#include "time.h"
#include "input.h"
#include "flontpolygon.h"
#include "sound.h"
#include "game.h"
#include "effect.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_XOBJECT				(32)						// オブジェクトの最大数
#define MAX_XOBJECT_NUM			(64)						// 各オブジェクトの最大数
#define XOBJECT_UPSPPED			(0.7f)						// オブジェクト加速速度
#define XOBJECT_DOWNSPPED		(0.2f)						// オブジェクト減速速度

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef struct
{
	char cLoadName[XOBJECT_TEXT_NAME];
	int nCntXObject;
	int nCntXObjectModel;
	bool bLoad;
}LOADMODEL;

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
XOBJECTINI						g_XObjectIni[MAX_XOBJECT];
XOBJECT							g_XObject[MAX_XOBJECT][MAX_XOBJECT_NUM];
LOADMODEL						g_LoadModel[MAX_XOBJECT_NUM];
int								g_nLoadNum;
PAZZLENUM						g_PazzleNum;
int								g_MaxXObject;
int								g_PuzzleSetingNum;
int								g_PuzzleSetingTimer;
bool							g_bPuzzleSeting;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void CountPazzlemodel(int nType, int nCntXObject, int nCntXObjectModel);

//=========================================================================================================================
// オブジェクトの初期化処理
//=========================================================================================================================
void InitXObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	int nCntXObject, nCntXObjectModel;

	g_MaxXObject = 0;
	g_nLoadNum = 0;
	g_PuzzleSetingNum = 0;
	g_PuzzleSetingTimer = 0;
	g_bPuzzleSeting = false;

	for (int nCnt = 0; nCnt < MAX_XOBJECT_NUM; nCnt++)
	{
		g_LoadModel[nCnt].cLoadName[0] = NULL;
		g_LoadModel[nCnt].nCntXObject = 0;
		g_LoadModel[nCnt].nCntXObjectModel = 0;
		g_LoadModel[nCnt].bLoad = false;
	}

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

	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファのポインタ

						// Xファイルの読み込み
	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{
			g_XObject[nCntXObject][nCntXObjectModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[nCntXObject][nCntXObjectModel].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
			g_XObject[nCntXObject][nCntXObjectModel].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
			g_XObject[nCntXObject][nCntXObjectModel].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow = 0;
			g_XObject[nCntXObject][nCntXObjectModel].bUse = false;

			for (int nCnt = 0; nCnt < MAX_XOBJECT_NUM; nCnt++)
			{
				if (g_LoadModel[nCnt].bLoad == true && strcmp(g_LoadModel[nCnt].cLoadName, g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
				{
					strcpy(g_XObject[nCntXObject][nCntXObjectModel].FileName, g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].FileName);
					g_XObject[nCntXObject][nCntXObjectModel].pMesh = g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pMesh;
					g_XObject[nCntXObject][nCntXObjectModel].pBuffMat = g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pBuffMat;
					g_XObject[nCntXObject][nCntXObjectModel].nNumMat = g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].nNumMat;
					g_XObject[nCntXObject][nCntXObjectModel].pTexture = g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pTexture;
					g_XObject[nCntXObject][nCntXObjectModel].vtxMax = g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].vtxMax;
					g_XObject[nCntXObject][nCntXObjectModel].vtxMin = g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].vtxMin;
					break;
				}
				
				if (nCnt == MAX_XOBJECT_NUM - 1)
				{
					//if (g_XObject[nCntXObject][nCntXObjectModel].Initrot.y == 0.0f)
					{
						g_LoadModel[g_nLoadNum].bLoad = true;
						strcpy(g_LoadModel[g_nLoadNum].cLoadName, &g_XObject[nCntXObject][nCntXObjectModel].FileName[0]);
						g_LoadModel[g_nLoadNum].nCntXObject = nCntXObject;
						g_LoadModel[g_nLoadNum].nCntXObjectModel = nCntXObjectModel;
						g_nLoadNum++;
					}

					D3DXLoadMeshFromX
					(
						&g_XObject[nCntXObject][nCntXObjectModel].FileName[0],
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_XObject[nCntXObject][nCntXObjectModel].pBuffMat,
						NULL,
						&g_XObject[nCntXObject][nCntXObjectModel].nNumMat,
						&g_XObject[nCntXObject][nCntXObjectModel].pMesh
					);

					D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
					D3DMATERIAL9 *matDef;						// 現在のマテリアル保存用
					pmat = (D3DXMATERIAL*)g_XObject[nCntXObject][nCntXObjectModel].pBuffMat->GetBufferPointer();
					matDef = new D3DMATERIAL9[g_XObject[nCntXObject][nCntXObjectModel].nNumMat];
					g_XObject[nCntXObject][nCntXObjectModel].pTexture = new LPDIRECT3DTEXTURE9[g_XObject[nCntXObject][nCntXObjectModel].nNumMat];

					for (DWORD tex = 0; tex < g_XObject[nCntXObject][nCntXObjectModel].nNumMat; tex++)
					{
						matDef[tex] = pmat[tex].MatD3D;
						matDef[tex].Ambient = matDef[tex].Diffuse;
						g_XObject[nCntXObject][nCntXObjectModel].pTexture[tex] = NULL;
						if (pmat[tex].pTextureFilename != NULL &&
							lstrlen(pmat[tex].pTextureFilename) > 0)
						{
							if (FAILED(D3DXCreateTextureFromFile(pDevice,
								pmat[tex].pTextureFilename,
								&g_XObject[nCntXObject][nCntXObjectModel].pTexture[tex])))
							{
								MessageBox(NULL, "テクスチャの読み込みに失敗しました", NULL, MB_OK);
							}
						}
					}

					delete matDef;

					// 頂点数の取得
					nNumVtx = g_XObject[nCntXObject][nCntXObjectModel].pMesh->GetNumVertices();

					// 頂点フォーマットのサイズを取得
					sizeFVF = D3DXGetFVFVertexSize(g_XObject[nCntXObject][nCntXObjectModel].pMesh->GetFVF());

					// 頂点バッファをロック
					g_XObject[nCntXObject][nCntXObjectModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

					for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
					{
						D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

						if (vtx.x > g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x)
						{
							g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x = vtx.x;
						}
						if (vtx.y > g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y)
						{
							g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y = vtx.y;
						}
						if (vtx.z > g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z)
						{
							g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z = vtx.z;
						}

						if (vtx.x < g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x)
						{
							g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x = vtx.x;
						}
						if (vtx.y < g_XObject[nCntXObject][nCntXObjectModel].vtxMin.y)
						{
							g_XObject[nCntXObject][nCntXObjectModel].vtxMin.y = vtx.y;
						}
						if (vtx.z < g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z)
						{
							g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z = vtx.z;
						}

						pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
					}

					// 頂点バッファをアンロック
					g_XObject[nCntXObject][nCntXObjectModel].pMesh->UnlockVertexBuffer();

					break;
				}
			}
		}
	}

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{
			if (strcmp("data\\MODEL\\Map\\ki.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
			{
				g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x = 15;
				g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x = -15;

				g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z = 15;
				g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z = -15;
			}
			else
			{
				if (g_XObject[nCntXObject][nCntXObjectModel].Initrot.y > 1.56f && g_XObject[nCntXObject][nCntXObjectModel].Initrot.y < 1.58f)
				{
					float frot1, frot2;

					frot1 = g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x;
					frot2 = g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x;

					g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x = g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z;
					g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x = g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z;

					g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z = -frot2;
					g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z = -frot1;
				}
				else if (g_XObject[nCntXObject][nCntXObjectModel].Initrot.y < -1.56f && g_XObject[nCntXObject][nCntXObjectModel].Initrot.y > -1.58f)
				{
					float frot1, frot2;

					frot1 = g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x;
					frot2 = g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x;

					g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x = -g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z;
					g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x = -g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z;

					g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z = frot1;
					g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z = frot2;
				}
			}
		}
	}

	for (nCntXObject = 0; nCntXObject < 8; nCntXObject++)
	{
		g_PazzleNum.modelNum[nCntXObject] = 0;
	}
}

//=========================================================================================================================
// オブジェクトの終了処理
//=========================================================================================================================
void UninitXObject(void)
{
	// 変数宣言
	//	int nCntXObject, nCntXObjectModel;

	// メッシュの開放
	for (int nCnt = 0; nCnt < g_nLoadNum; nCnt++)
	{// モデルをカウント
		if (g_LoadModel[nCnt].bLoad == true)
		{
			if (g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pMesh != NULL)
			{// NULL以外の場合
				g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pMesh->Release();
				g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pMesh = NULL;
			}
		}
	}

	// マテリアルの開放
	for (int nCnt = 0; nCnt < g_nLoadNum; nCnt++)
	{// モデルをカウント
		if (g_LoadModel[nCnt].bLoad == true)
		{
			if (g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pBuffMat != NULL)
			{// NULL以外の場合
				g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pBuffMat->Release();
				g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pBuffMat = NULL;
			}
		}
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < g_nLoadNum; nCnt++)
	{// モデルをカウント
		if (g_LoadModel[nCnt].bLoad == true)
		{
			for (DWORD tex = 0; tex < g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].nNumMat; tex++)
			{// マテリアルをカウント
				if (g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pTexture[tex] != NULL)
				{// NULL以外の場合
					g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pTexture[tex]->Release();
					g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pTexture[tex] = NULL;
				}
			}

			delete[] g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pTexture;
			g_XObject[g_LoadModel[nCnt].nCntXObject][g_LoadModel[nCnt].nCntXObjectModel].pTexture = NULL;
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

	QUESTTYPE pQuestType = GetQuestType();

	Time *pTime = GetTime();

	DIMOUSESTATE2 pMouse = GetMousePointer();
	DIJOYSTATE pStick = GetStick();

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// オブジェクトをカウント
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// モデルをカウント

			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
			{// 使用している場合

				g_XObject[nCntXObject][nCntXObjectModel].posOld = g_XObject[nCntXObject][nCntXObjectModel].pos;

				if ((nCntXObject == TEXTTYPE_PUZZLE_1 || nCntXObject == TEXTTYPE_PUZZLE_2 || nCntXObject == TEXTTYPE_PUZZLE_3) && g_bPuzzleSeting == false && pTime->TimeState == TIMESTATE_NORMAL)
				{// 操作パズルのみ
					if (g_XObject[nCntXObject][nCntXObjectModel].move.y == 0.0f)
					{
						/*if (GetKeyboardTrigger(DIK_RSHIFT) == true || pMouse.lZ != 0)
						{// 操作パズル切り替え
						g_XObject[nCntXObject][nCntXObjectModel].bUse = false;
						g_XObject[nCntXObject][(nCntXObjectModel + 1) % g_XObject[nCntXObject][0].nMaxModel].pos.x = g_XObject[nCntXObject][nCntXObjectModel].pos.x;
						g_XObject[nCntXObject][(nCntXObjectModel + 1) % g_XObject[nCntXObject][0].nMaxModel].pos.z = g_XObject[nCntXObject][nCntXObjectModel].pos.z;
						g_XObject[nCntXObject][(nCntXObjectModel + 1) % g_XObject[nCntXObject][0].nMaxModel].bUse = true;

						break;
						}*/

						// 移動処理
						/*if (GetMousePress(0) == true)
						{
						if (pMouse.lX > 0)
						{
						g_XObject[nCntXObject][nCntXObjectModel].pos.x += (pMouse.lX / 1.0f);
						}
						if (pMouse.lX < 0)
						{
						g_XObject[nCntXObject][nCntXObjectModel].pos.x += (pMouse.lX / 1.0f);
						}
						if (pMouse.lY > 0)
						{
						g_XObject[nCntXObject][nCntXObjectModel].pos.z -= (pMouse.lY / 1.0f);
						}
						if (pMouse.lY < 0)
						{
						g_XObject[nCntXObject][nCntXObjectModel].pos.z -= (pMouse.lY / 1.0f);
						}
						}*/

						// 移動処理
						if ((GetKeyboardPress(DIK_UP) == true || pStick.rgdwPOV[0] == 0) || pStick.lY < -300)
						{// 任意のキー（↑）を押した場合
							g_XObject[nCntXObject][nCntXObjectModel].pos.z += 5.0f;
						}
						if ((GetKeyboardPress(DIK_DOWN) == true || pStick.rgdwPOV[0] == 18000) || pStick.lY > 300)
						{// 任意のキー（↓）を押した場合
							g_XObject[nCntXObject][nCntXObjectModel].pos.z -= 5.0f;
						}
						if ((GetKeyboardPress(DIK_LEFT) == true || pStick.rgdwPOV[0] == 27000) || pStick.lX < -300)
						{// 任意のキー（←）を押した場合
							g_XObject[nCntXObject][nCntXObjectModel].pos.x -= 5.0f;
						}
						if ((GetKeyboardPress(DIK_RIGHT) == true || pStick.rgdwPOV[0] == 9000) || pStick.lX > 300)
						{// 任意のキー（→）を押した場合
							g_XObject[nCntXObject][nCntXObjectModel].pos.x += 5.0f;
						}

						if (pQuestType == QUESTTYPE_FIRST)
						{
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.x > 195.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.x = 195.0f;
							}
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.x < -455.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.x = -455.0f;
							}
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.z > 125.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.z = 125.0f;
							}
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.z < -315.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.z = -315.0f;
							}
						}
						else if (pQuestType == QUESTTYPE_SECOND)
						{
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.x > 295.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.x = 295.0f;
							}
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.x < -455.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.x = -455.0f;
							}
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.z > 190.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.z = 190.0f;
							}
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.z < -315.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.z = -315.0f;
							}
						}
						else if (pQuestType == QUESTTYPE_THIRD)
						{
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.x > 200.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.x = 200.0f;
							}
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.x < -455.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.x = -455.0f;
							}
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.z > 190.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.z = 190.0f;
							}
							if (g_XObject[nCntXObject][nCntXObjectModel].pos.z < -315.0f)
							{
								g_XObject[nCntXObject][nCntXObjectModel].pos.z = -315.0f;
							}
						}

						// パズルをはめる
						if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_LCONTROL) == true || GetKeyboardTrigger(DIK_RCONTROL) == true || GetButtonTrigger(2) == true/* || GetMouseTrigger(1) == true*/)
						{// 任意のキー（右CTRL）を押した場合
							g_XObject[nCntXObject][nCntXObjectModel].move.y -= 8.0f;
						}
					}

					if (g_XObject[nCntXObject][nCntXObjectModel].vtxMax.y > g_XObject[nCntXObject][nCntXObjectModel].pos.y)
					{
						g_XObject[nCntXObject][nCntXObjectModel].move.y = 0.0f;
						g_XObject[nCntXObject][nCntXObjectModel].pos.y = g_XObject[nCntXObject][0].Initpos.y;

						if (PuzzleSeting(&g_XObject[nCntXObject][nCntXObjectModel].FileName[0], &g_XObject[nCntXObject][nCntXObjectModel].pos, &g_XObject[nCntXObject][nCntXObjectModel].vtxMin, &g_XObject[nCntXObject][nCntXObjectModel].vtxMax) == true)
						{// 全てはめた場合
						 //SetFadeMode(MODE_RESULT);
							g_XObject[nCntXObject][nCntXObjectModel].move.y = 5.0f;
							g_bPuzzleSeting = true;

							SetTimeState(TIMESTATE_ADD);
						}
					}

					g_XObject[nCntXObject][nCntXObjectModel].pos += g_XObject[nCntXObject][nCntXObjectModel].move;
				}
				else if ((nCntXObject == TEXTTYPE_PUZZLE_1 || nCntXObject == TEXTTYPE_PUZZLE_2 || nCntXObject == TEXTTYPE_PUZZLE_3) && g_bPuzzleSeting == true)
				{
					if (g_XObject[nCntXObject][nCntXObjectModel].pos.y < 1000.0f)
					{
						g_XObject[nCntXObject][nCntXObjectModel].pos += g_XObject[nCntXObject][nCntXObjectModel].move;
					}
				}

				if ((nCntXObject == TEXTTYPE_PUZZLE_1_A || nCntXObject == TEXTTYPE_PUZZLE_2_A || nCntXObject == TEXTTYPE_PUZZLE_3_A) && g_bPuzzleSeting == true)
				{
					g_PuzzleSetingTimer++;

					if (g_PuzzleSetingTimer > 600)
					{
						if (strcmp("data\\MODEL\\Pazzle\\zenmai.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
						{
							g_XObject[nCntXObject][nCntXObjectModel].rot.z -= 0.01f;
						}
						else if (strcmp("data\\MODEL\\Pazzle\\MBSPIN.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
						{
							g_XObject[nCntXObject][nCntXObjectModel].rot.x += 0.05f;
						}
						else if (strcmp("data\\MODEL\\Pazzle\\MBDown.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
						{

						}
						else
						{
							if (nCntXObjectModel % 2 == 0)
							{
								if (strcmp("data\\MODEL\\Pazzle\\gear0001.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
									strcmp("data\\MODEL\\Pazzle\\gear0101.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
									strcmp("data\\MODEL\\Pazzle\\gear0201.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
									strcmp("data\\MODEL\\Pazzle\\gear0301.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
								{
									g_XObject[nCntXObject][nCntXObjectModel].rot.y -= 0.0334f;
								}
								else
								{
									g_XObject[nCntXObject][nCntXObjectModel].rot.y -= 0.05f;
								}
							}
							else
							{
								if (strcmp("data\\MODEL\\Pazzle\\gear0001.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
									strcmp("data\\MODEL\\Pazzle\\gear0101.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
									strcmp("data\\MODEL\\Pazzle\\gear0201.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
									strcmp("data\\MODEL\\Pazzle\\gear0301.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
								{
									g_XObject[nCntXObject][nCntXObjectModel].rot.y += 0.0334f;
								}
								else
								{
									g_XObject[nCntXObject][nCntXObjectModel].rot.y += 0.05f;
								}
							}
						}
					}

					if (g_PuzzleSetingTimer == 3000)
					{
						SetQuestFade(QUESTSTATE_REPORT);
					}
				}

				if (nCntXObject == TEXTTYPE_PUZZLE_1 || nCntXObject == TEXTTYPE_PUZZLE_2 || nCntXObject == TEXTTYPE_PUZZLE_3)
				{// 操作パズルのみ
					SetPostionShadow(g_XObject[nCntXObject][0].nIdxShadow, g_XObject[nCntXObject][nCntXObjectModel].pos, g_XObject[nCntXObject][nCntXObjectModel].posOld);
				}
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

	MeshField *pMeshField = GetMeshField();

	// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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

				if (nCntXObject == TEXTTYPE_SEARCH_HOUSE_0 || nCntXObject == TEXTTYPE_SEARCH_HOUSE_1 || nCntXObject == TEXTTYPE_SEARCH_HOUSE_2 ||
					nCntXObject == TEXTTYPE_SEARCH_HOUSE_3 || nCntXObject == TEXTTYPE_SEARCH_HOUSE_4 || nCntXObject == TEXTTYPE_SEARCH_HOUSE_5 ||
					nCntXObject == TEXTTYPE_SEARCH_HOUSE_6 || nCntXObject == TEXTTYPE_SEARCH_HOUSE_7 || nCntXObject == TEXTTYPE_SEARCH_HOUSE_8)
				{
					//g_XObject[nCntXObject][nCntXObjectModel].mtxWorld = pMeshField[8].mtxWorld;
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
					//if (nCntXObject == TEXTTYPE_PUZZLE_A)
					{// 解答パズルのみ
						pmat[nCntMat].MatD3D.Diffuse.a = g_XObject[nCntXObject][nCntXObjectModel].col.a;
					}
					pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

					// テクスチャ設定
					pDevice->SetTexture(0, g_XObject[nCntXObject][nCntXObjectModel].pTexture[nCntMat]);

					// オブジェクト(パーツ)の描画
					g_XObject[nCntXObject][nCntXObjectModel].pMesh->DrawSubset(nCntMat);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=========================================================================================================================
// オブジェクトの設定処理
//=========================================================================================================================
void SetXObject(TEXTTYPE type)
{
	// 変数宣言
	int nCntXObjectModel;
	MeshField *pMeshField = GetMeshField();

	for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[type][0].nMaxModel; nCntXObjectModel++)
	{// モデルをカウント
		if (g_XObject[type][nCntXObjectModel].bUse == false)
		{// 使用している場合
			g_XObject[type][nCntXObjectModel].pos = g_XObject[type][nCntXObjectModel].Initpos;
			g_XObject[type][nCntXObjectModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XObject[type][nCntXObjectModel].rot = g_XObject[type][nCntXObjectModel].Initrot;

			if (type == TEXTTYPE_SEARCH_HOUSE_0 || type == TEXTTYPE_SEARCH_HOUSE_1 || type == TEXTTYPE_SEARCH_HOUSE_2 ||
				type == TEXTTYPE_SEARCH_HOUSE_3 || type == TEXTTYPE_SEARCH_HOUSE_4 || type == TEXTTYPE_SEARCH_HOUSE_5 ||
				type == TEXTTYPE_SEARCH_HOUSE_6 || type == TEXTTYPE_SEARCH_HOUSE_7 || type == TEXTTYPE_SEARCH_HOUSE_8)
			{
				g_XObject[type][nCntXObjectModel].pos += pMeshField[type].pos;
			}

			if (type == TEXTTYPE_PUZZLE_1 || type == TEXTTYPE_PUZZLE_2 || type == TEXTTYPE_PUZZLE_3)
			{// 操作パズルのみ
				if (nCntXObjectModel == 0)
				{// 1つ目もモデルのみ
					g_XObject[type][nCntXObjectModel].nIdxShadow = SetShadow(g_XObject[type][nCntXObjectModel].pos, g_XObject[type][nCntXObjectModel].rot, 50, 50);
				}
			}
			//else if (type != TEXTTYPE_PUZZLE_1_A || type != TEXTTYPE_PUZZLE_2_A || type != TEXTTYPE_PUZZLE_3_A)
			{// 解答パズル以外
			 //g_XObject[type][nCntXObjectModel].nIdxShadow = SetShadow(g_XObject[type][nCntXObjectModel].pos, g_XObject[type][nCntXObjectModel].rot, g_XObject[type][nCntXObjectModel].vtxMax);
			}

			if (type == TEXTTYPE_PUZZLE_1_A || type == TEXTTYPE_PUZZLE_2_A || type == TEXTTYPE_PUZZLE_3_A)
			{// 解答パズルのみ
				g_XObject[type][nCntXObjectModel].col.a = 0.5f;

				CountPazzlemodel(0, type, nCntXObjectModel);
			}
			else
			{
				g_XObject[type][nCntXObjectModel].col.a = 1.0f;
			}

			if (type == TEXTTYPE_PUZZLE_1 || type == TEXTTYPE_PUZZLE_2 || type == TEXTTYPE_PUZZLE_3)
			{// 操作パズルのみ
				if (nCntXObjectModel == 0)
				{// 1つ目のモデルのみ
					g_XObject[type][nCntXObjectModel].bUse = true;
				}
				else
				{// その他
					g_XObject[type][nCntXObjectModel].bUse = false;
				}
			}
			else
			{// その他
				g_XObject[type][nCntXObjectModel].bUse = true;
			}
		}
	}
}

//=========================================================================================================================
// オブジェクトの取得処理
//=========================================================================================================================
XOBJECT *GetXObject(void)
{
	return &g_XObject[0][0];
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
							//pMove->z = 0.0f;
						}
						else if (pPosOld->z + vtxMax->z <= g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z && g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z <= pPos->z + vtxMax->z)
						{// Z下底判定
							pPos->z = g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z - vtxMax->z;
							//pMove->x = 0.0f;
							//pMove->z = 0.0f;
						}
					}

					if (pPos->z + vtxMax->z > g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z && pPos->z + vtxMin->z < g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z)
					{// Z範囲確認
						if (pPosOld->x + vtxMin->x >= g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x && g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x >= pPos->x + vtxMin->x)
						{// X右辺判定
							pPos->x = g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x - vtxMin->x;
							//pMove->x = 0.0f;
							//pMove->z = 0.0f;
						}
						else if (pPosOld->x + vtxMax->x <= g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x && g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x <= pPos->x + vtxMax->x)
						{// X左辺判定
							pPos->x = g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x - vtxMax->x;
							//pMove->x = 0.0f;
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
// パズル設定処理
//=========================================================================================================================
bool PuzzleSeting(char *pName, D3DXVECTOR3 *pPos, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	// 変数宣言
	int nCntXObject, nCntXObjectModel;
	bool bLand = false;

	int nCountPazzleNum = g_PuzzleSetingNum;

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// オブジェクトをカウント
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// モデルをカウント
			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true && (nCntXObject == TEXTTYPE_PUZZLE_1_A || nCntXObject == TEXTTYPE_PUZZLE_2_A || nCntXObject == TEXTTYPE_PUZZLE_3_A) &&
				strcmp(pName, g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 && g_XObject[nCntXObject][nCntXObjectModel].col.a != 1.0f)
			{// 使用されている、解答パズルである、データパスが同一、α値が1.0fでない場合
				if (g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.x < pPos->x + (vtxMin->x / 2.0f) &&
					g_XObject[nCntXObject][nCntXObjectModel].pos.x + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.x > pPos->x + (vtxMax->x / 2.0f) &&
					g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMin.z < pPos->z + (vtxMin->z / 2.0f) &&
					g_XObject[nCntXObject][nCntXObjectModel].pos.z + g_XObject[nCntXObject][nCntXObjectModel].vtxMax.z > pPos->z + (vtxMax->z / 2.0f))
				{
					PlaySound(SOUND_LABEL_SE_GEAR);

					g_PuzzleSetingNum++;	//
					CountPazzlemodel(1, nCntXObject, nCntXObjectModel);

					//SetParticleEffect(g_XObject[nCntXObject][nCntXObjectModel].pos, EFFECTTYPE_EXPLOSION, 30, 30, 30, 0);
					SetRainEffect(g_XObject[nCntXObject][nCntXObjectModel].pos, EFFECTTYPE_RESURRECTIONWATER_1, 30, 30, 0, 0);	//位置： エフェクトタイプ ： 個数 : 半径 ： HP ： テクスチャ番号	/噴水

					g_XObject[nCntXObject][nCntXObjectModel].col.a = 1.0f;

					if (g_PuzzleSetingNum == g_XObject[nCntXObject][0].nMaxModel)
					{
						SetFlontPolygon(FLONTPOLYGONTEXTURE_PAZZLE_END, D3DXVECTOR3(SCREEN_WIDTH + 400, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 700, 300);

						PlaySound(SOUND_LABEL_SE_PAZZLEEND);

						bLand = true;
					}
					break;
				}
			}
		}
	}

	if (nCountPazzleNum == g_PuzzleSetingNum)
	{
		PlaySound(SOUND_LABEL_SE_MISS);
	}

	return bLand;
}

//=========================================================================================================================
// 移動パズル設定処理
//=========================================================================================================================
void SetMovePazzelObj(int nNum)
{
	// 変数宣言
	int nCntXObject, nCntXObjectModel;

	for (nCntXObject = 0; nCntXObject < g_MaxXObject; nCntXObject++)
	{// オブジェクトをカウント
		for (nCntXObjectModel = 0; nCntXObjectModel < g_XObject[nCntXObject][0].nMaxModel; nCntXObjectModel++)
		{// モデルをカウント
			if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
			{// 使用している場合
				if ((nCntXObject == TEXTTYPE_PUZZLE_1 || nCntXObject == TEXTTYPE_PUZZLE_2 || nCntXObject == TEXTTYPE_PUZZLE_3) && g_bPuzzleSeting == false)
				{// 操作パズルのみ
					g_XObject[nCntXObject][nCntXObjectModel].bUse = false;
					g_XObject[nCntXObject][(nNum)].pos.x = g_XObject[nCntXObject][nCntXObjectModel].pos.x;
					g_XObject[nCntXObject][(nNum)].pos.z = g_XObject[nCntXObject][nCntXObjectModel].pos.z;
					g_XObject[nCntXObject][(nNum)].bUse = true;
				}
			}
		}
	}
}

//=========================================================================================================================
// パズル数取得処理
//=========================================================================================================================
PAZZLENUM *GetPazzleNum(void)
{
	return &g_PazzleNum;
}

//=========================================================================================================================
// パズル数確認処理
//=========================================================================================================================
void CountPazzlemodel(int nType, int nCntXObject, int nCntXObjectModel)
{
	switch (nType)
	{
	case 0:

		if (strcmp("data\\MODEL\\Pazzle\\gear0000.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[0]++;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0100.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[1]++;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0200.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[2]++;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0300.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[3]++;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0001.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[4]++;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0101.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[5]++;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0201.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
			strcmp("data\\MODEL\\Pazzle\\MBSPIN.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[6]++;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0301.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
			strcmp("data\\MODEL\\Pazzle\\MBDown.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
			strcmp("data\\MODEL\\Pazzle\\zenmai.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[7]++;
		}

		break;
	case 1:

		if (strcmp("data\\MODEL\\Pazzle\\gear0000.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[0]--;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0100.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[1]--;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0200.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[2]--;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0300.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[3]--;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0001.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[4]--;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0101.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[5]--;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0201.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
			strcmp("data\\MODEL\\Pazzle\\MBSPIN.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[6]--;
		}
		else if (strcmp("data\\MODEL\\Pazzle\\gear0301.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
			strcmp("data\\MODEL\\Pazzle\\MBDown.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0 ||
			strcmp("data\\MODEL\\Pazzle\\zenmai.x", g_XObject[nCntXObject][nCntXObjectModel].FileName) == 0)
		{
			g_PazzleNum.modelNum[7]--;
		}

		break;
	}
}

//=========================================================================================================================
// オブジェクトを消す
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
			{
				g_XObject[nCntXObject][nCntXObjectModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_XObject[nCntXObject][nCntXObjectModel].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow = 0;
				g_XObject[nCntXObject][nCntXObjectModel].bUse = false;

				DeleteShadow(g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow);
			}
		}
	}
	g_PuzzleSetingNum = 0;
	g_PuzzleSetingTimer = 0;
	g_bPuzzleSeting = false;
}
void DeleteXObject(int nCntXObject, int nCntXObjectModel)
{
	if (g_XObject[nCntXObject][nCntXObjectModel].bUse == true)
	{
		g_XObject[nCntXObject][nCntXObjectModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XObject[nCntXObject][nCntXObjectModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XObject[nCntXObject][nCntXObjectModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XObject[nCntXObject][nCntXObjectModel].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XObject[nCntXObject][nCntXObjectModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XObject[nCntXObject][nCntXObjectModel].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow = 0;
		g_XObject[nCntXObject][nCntXObjectModel].bUse = false;

		DeleteShadow(g_XObject[nCntXObject][nCntXObjectModel].nIdxShadow);
	}
}