//=========================================================================================================================
//
// プレイヤー処理 [xplayer.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanfエラー解除
#include <stdio.h>					// stdio
#include "input.h"
#include "xplayer.h"
#include "xenemy.h"
#include "xobject.h"
#include "meshfield.h"
#include "shadow.h"
#include "camera.h"
#include "game.h"
#include "fade.h"
#include "item.h"
#include "itemfont.h"
#include "sound.h"
#include "flontpolygon.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define XPLAYER_UPSPPED_RUN		(1.2f)						// プレイヤー加速速度（歩く）
#define XPLAYER_UPSPPED_WALK	(0.5f)						// プレイヤー加速速度（走る）
#define XPLAYER_DOWNSPPED		(0.15f)						// プレイヤー減速速度

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
XPLAYER							g_XPlayer;					// プレイヤー情報
float							g_fDiffXPlayer;				// 差分
int								g_nCntTimeXPlayer;			// プレイヤーカウンター

//=========================================================================================================================
// プレイヤーの初期化処理
//=========================================================================================================================
void InitXPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	FILE *pFile;
	int nCntXPlayer;

	g_XPlayer.nMaxPartsNum = 0;		// プレイヤーパーツ数
	g_fDiffXPlayer = 0.0f;			// 差分計算
	g_nCntTimeXPlayer = 0;	// プレイヤーカウンター

	// ファイル設定
	pFile = fopen(XPLAYER_TEXT_INFO, "r");

	// プレイヤーテキストデータ読み込み
	if (pFile != NULL)
	{// ファイルが開けた場合
		fscanf(pFile, "%d", &g_XPlayer.nMaxPartsNum);	// 読み込み

		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			fscanf(pFile, "%s", &g_XPlayer.aModel[nCntXPlayer].FileName[0]);	// 読み込み
		}
		
		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			fscanf(pFile, "%d", &g_XPlayer.aModel[nCntXPlayer].nIndexParent);		// 読み込み
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initpos.x);			// 読み込み
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initpos.y);			// 読み込み
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initpos.z);			// 読み込み
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initrot.x);			// 読み込み
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initrot.y);			// 読み込み
			fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].Initrot.z);			// 読み込み
		}

		fscanf(pFile, "%d", &g_XPlayer.nMaxAnimNum);	// 読み込み

		for (int nCntXPlayerAnim = 0; nCntXPlayerAnim < g_XPlayer.nMaxAnimNum; nCntXPlayerAnim++)
		{
			fscanf(pFile, "%d", &g_XPlayer.nAnimloop[nCntXPlayerAnim]);				// 読み込み
			fscanf(pFile, "%d", &g_XPlayer.nAnimkey[nCntXPlayerAnim]);				// 読み込み
			fscanf(pFile, "%d", &g_XPlayer.nAnimBlend[nCntXPlayerAnim]);			// 読み込み

			for (int nCntXPlayerAnimKey = 0; nCntXPlayerAnimKey < g_XPlayer.nAnimkey[nCntXPlayerAnim]; nCntXPlayerAnimKey++)
			{
				fscanf(pFile, "%d", &g_XPlayer.nAnimfram[nCntXPlayerAnim][nCntXPlayerAnimKey]);			// 読み込み

				for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
				{
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].posAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].x);	// 読み込み
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].posAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].y);	// 読み込み
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].posAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].z);	// 読み込み
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].rotAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].x);	// 読み込み
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].rotAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].y);	// 読み込み
					fscanf(pFile, "%f", &g_XPlayer.aModel[nCntXPlayer].rotAnim[nCntXPlayerAnim][nCntXPlayerAnimKey].z);	// 読み込み
				}
			}
		}

		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}

	// Xファイルの読み込み
	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		D3DXLoadMeshFromX
		(
			&g_XPlayer.aModel[nCntXPlayer].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_XPlayer.aModel[nCntXPlayer].pBuffMat,
			NULL,
			&g_XPlayer.aModel[nCntXPlayer].nNumMat,
			&g_XPlayer.aModel[nCntXPlayer].pMesh
		);
	}

	// 位置・向きの初期設定
	g_XPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_XPlayer.vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
	g_XPlayer.nAnimChange = 0;
	g_XPlayer.nAnimkeynow = 0;
	g_XPlayer.nIdxShadow = 0;
	g_XPlayer.bAttack = false;

	g_XPlayer.bUse = false;

	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		g_XPlayer.aModel[nCntXPlayer].pos = g_XPlayer.aModel[nCntXPlayer].Initpos;
		g_XPlayer.aModel[nCntXPlayer].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XPlayer.aModel[nCntXPlayer].rot = g_XPlayer.aModel[nCntXPlayer].rotAnim[0][0];
		g_XPlayer.aModel[nCntXPlayer].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XPlayer.aModel[nCntXPlayer].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_XPlayer.aModel[nCntXPlayer].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	}

	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファのポインタ

	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		// 頂点数の取得
		nNumVtx = g_XPlayer.aModel[nCntXPlayer].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_XPlayer.aModel[nCntXPlayer].pMesh->GetFVF());

		// 頂点バッファをロック
		g_XPlayer.aModel[nCntXPlayer].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入
			{
				if (vtx.x > g_XPlayer.aModel[nCntXPlayer].vtxMax.x)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMax.x = vtx.x;
				}
				if (vtx.y > g_XPlayer.aModel[nCntXPlayer].vtxMax.y)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMax.y = vtx.y;
				}
				if (vtx.z > g_XPlayer.aModel[nCntXPlayer].vtxMax.z)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMax.z = vtx.z;
				}

				if (vtx.x < g_XPlayer.aModel[nCntXPlayer].vtxMin.x)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMin.x = vtx.x;
				}
				if (vtx.y < g_XPlayer.aModel[nCntXPlayer].vtxMin.y)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMin.y = vtx.y;
				}
				if (vtx.z < g_XPlayer.aModel[nCntXPlayer].vtxMin.z)
				{
					g_XPlayer.aModel[nCntXPlayer].vtxMin.z = vtx.z;
				}
			}

			pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
		}

		// 頂点バッファをアンロック
		g_XPlayer.aModel[nCntXPlayer].pMesh->UnlockVertexBuffer();
	}

	g_XPlayer.vtxMin.x = -20.0f;
	g_XPlayer.vtxMin.y = 0.0f;
	g_XPlayer.vtxMin.z = -20.0f;
	g_XPlayer.vtxMax.x = 20.0f;
	g_XPlayer.vtxMax.y = g_XPlayer.aModel[0].pos.y + g_XPlayer.aModel[0].vtxMax.y + g_XPlayer.aModel[1].pos.y + g_XPlayer.aModel[1].vtxMax.y + g_XPlayer.aModel[2].pos.y + g_XPlayer.aModel[2].vtxMax.y;
	g_XPlayer.vtxMax.z = 20.0f;
}

//=========================================================================================================================
// プレイヤーの終了処理
//=========================================================================================================================
void UninitXPlayer(void)
{
	// 変数宣言
	int nCntXPlayer;

	// メッシュの開放
	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		if (g_XPlayer.aModel[nCntXPlayer].pMesh != NULL)
		{
			g_XPlayer.aModel[nCntXPlayer].pMesh->Release();
			g_XPlayer.aModel[nCntXPlayer].pMesh = NULL;
		}
	}

	// マテリアルの開放
	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		if (g_XPlayer.aModel[nCntXPlayer].pBuffMat != NULL)
		{
			g_XPlayer.aModel[nCntXPlayer].pBuffMat->Release();
			g_XPlayer.aModel[nCntXPlayer].pBuffMat = NULL;
		}
	}
}

//=========================================================================================================================
// プレイヤーの更新処理
//=========================================================================================================================
void UpdateXPlayer(void)
{
	//変数宣言
	Camera *pCamera = GetCamera();
	float fDiff = 0.0f;
	float frac = 0.0f;
	float fSpeed = 0.0f;
	int nCntXPlayer;
	bool bUseItemFont = false;

	QUESTSTATE pQuestState = GetQuestState();
	DIJOYSTATE pStick = GetStick();

	bool bDeletePartsTexture = DeletePartsTexture();

	//アイテムフォントの情報を取得する
	bUseItemFont = GetUseItemFont();

	
	if (g_XPlayer.bUse == true)
	{
		g_XPlayer.posOld = g_XPlayer.pos;
		g_nCntTimeXPlayer++;

		if (g_XPlayer.bAttack == false && pQuestState == QUESTSTATE_SEARCH && bDeletePartsTexture == false)
		{
			if (bUseItemFont == false)
			{
				if ((GetKeyboardPress(DIK_UP) == true || (pStick.rgdwPOV[0] == 0 || pStick.rgdwPOV[0] == 31500 || pStick.rgdwPOV[0] == 4500)) || pStick.lY < -100)
				{// 任意のキー（上）を押した場合
					if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true || GetButtonPress(7) == true)
					{
						if (g_XPlayer.nAnimnow != PLAYERANIM_RUN)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						fSpeed = XPLAYER_UPSPPED_RUN;
						g_XPlayer.nAnimnow = PLAYERANIM_RUN;
					}
					else
					{
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						fSpeed = XPLAYER_UPSPPED_WALK;
						g_XPlayer.nAnimnow = PLAYERANIM_WALK;
					}

					if ((GetKeyboardPress(DIK_LEFT) == true || pStick.rgdwPOV[0] == 31500) || pStick.lX < -100)
					{// 任意のキー（左）を押した場合
						g_XPlayer.move.x -= sinf(D3DX_PI * 0.75f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(D3DX_PI * 0.75f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (D3DX_PI * 0.75f + pCamera->rot.y);
					}
					else if ((GetKeyboardPress(DIK_RIGHT) == true || pStick.rgdwPOV[0] == 4500) || pStick.lX > 100)
					{// 任意のキー（右）を押した場合
						g_XPlayer.move.x -= sinf(-D3DX_PI * 0.75f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (-D3DX_PI * 0.75f + pCamera->rot.y);
					}
					else
					{// 上のみの場合
						g_XPlayer.move.x -= sinf(D3DX_PI * 1.0f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(D3DX_PI * 1.0f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (D3DX_PI * 1.0f + pCamera->rot.y);
					}
				}
				else if ((GetKeyboardPress(DIK_DOWN) == true || (pStick.rgdwPOV[0] == 18000 || pStick.rgdwPOV[0] == 13500 || pStick.rgdwPOV[0] == 22500)) || pStick.lY > 100)
				{// 任意のキー（下）を押した場合
					if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true || GetButtonPress(7) == true)
					{
						if (g_XPlayer.nAnimnow != PLAYERANIM_RUN)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						fSpeed = XPLAYER_UPSPPED_RUN;
						g_XPlayer.nAnimnow = PLAYERANIM_RUN;
					}
					else
					{
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						fSpeed = XPLAYER_UPSPPED_WALK;
						g_XPlayer.nAnimnow = PLAYERANIM_WALK;
					}

					if ((GetKeyboardPress(DIK_LEFT) == true || pStick.rgdwPOV[0] == 22500) || pStick.lX < -100)
					{// 任意のキー（左）を押した場合
						g_XPlayer.move.x -= sinf(D3DX_PI * 0.25f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(D3DX_PI * 0.25f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (D3DX_PI * 0.25f + pCamera->rot.y);
					}
					else if ((GetKeyboardPress(DIK_RIGHT) == true || pStick.rgdwPOV[0] == 13500) || pStick.lX > 100)
					{// 任意のキー（右）を押した場合
						g_XPlayer.move.x -= sinf(-D3DX_PI * 0.25f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (-D3DX_PI * 0.25f + pCamera->rot.y);
					}
					else
					{// 下のみの場合
						g_XPlayer.move.x -= sinf(D3DX_PI * 0.0f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(D3DX_PI * 0.0f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (D3DX_PI * 0.0f + pCamera->rot.y);
					}
				}
				else if ((GetKeyboardPress(DIK_LEFT) == true || (pStick.rgdwPOV[0] == 27000 || pStick.rgdwPOV[0] == 31500 || pStick.rgdwPOV[0] == 22500)) || pStick.lX < -100)
				{// 任意のキー（左）を押した場合
					if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true || GetButtonPress(7) == true)
					{
						if (g_XPlayer.nAnimnow != PLAYERANIM_RUN)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						fSpeed = XPLAYER_UPSPPED_RUN;
						g_XPlayer.nAnimnow = PLAYERANIM_RUN;
					}
					else
					{
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						fSpeed = XPLAYER_UPSPPED_WALK;
						g_XPlayer.nAnimnow = PLAYERANIM_WALK;
					}

					if ((GetKeyboardPress(DIK_UP) == true || pStick.rgdwPOV[0] == 31500 || pStick.lY < -100))
					{// 任意のキー（上）を押した場合
						g_XPlayer.move.x -= sinf(D3DX_PI * 0.75f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(D3DX_PI * 0.75f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (D3DX_PI * 0.75f + pCamera->rot.y);
					}
					else if ((GetKeyboardPress(DIK_DOWN) == true || pStick.rgdwPOV[0] == 22500 || pStick.lY > 100))
					{// 任意のキー（下）を押した場合
						g_XPlayer.move.x -= sinf(D3DX_PI * 0.25f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(D3DX_PI * 0.25f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (D3DX_PI * 0.25f + pCamera->rot.y);
					}
					else
					{// 左のみの場合
						g_XPlayer.move.x -= sinf(D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (D3DX_PI * 0.5f + pCamera->rot.y);
					}
				}
				else if ((GetKeyboardPress(DIK_RIGHT) == true || (pStick.rgdwPOV[0] == 9000 || pStick.rgdwPOV[0] == 4500 || pStick.rgdwPOV[0] == 22500)) || pStick.lX > 100)
				{// 任意のキー（右）を押した場合
					if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true || GetButtonPress(7) == true)
					{
						if (g_XPlayer.nAnimnow != PLAYERANIM_RUN)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						fSpeed = XPLAYER_UPSPPED_RUN;
						g_XPlayer.nAnimnow = PLAYERANIM_RUN;
					}
					else
					{
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						fSpeed = XPLAYER_UPSPPED_WALK;
						g_XPlayer.nAnimnow = PLAYERANIM_WALK;
					}

					if ((GetKeyboardPress(DIK_UP) == true || pStick.rgdwPOV[0] == 4500 || pStick.lY < -100))
					{// 任意のキー（上）を押した場合
						g_XPlayer.move.x -= sinf(-D3DX_PI * 0.75f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (-D3DX_PI * 0.75f + pCamera->rot.y);
					}
					else if ((GetKeyboardPress(DIK_DOWN) == true || pStick.rgdwPOV[0] == 22500 || pStick.lY > 100))
					{// 任意のキー（下）を押した場合
						g_XPlayer.move.x -= sinf(-D3DX_PI * 0.25f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (-D3DX_PI * 0.25f + pCamera->rot.y);
					}
					else
					{// 右のみの場合
						g_XPlayer.move.x -= sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;
						g_XPlayer.move.z -= cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * fSpeed;

						g_XPlayer.rotDest.y = (-D3DX_PI * 0.5f + pCamera->rot.y);
					}
				}
				else
				{
					if (g_XPlayer.bAttack == false)
					{
						if (g_XPlayer.nAnimnow != PLAYERANIM_NEUTRAL)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
					}
				}
			}
		}

		if (g_XPlayer.nAnimnow == PLAYERANIM_WALK)
		{
			if ((g_XPlayer.nAnimkeynow == 0 || g_XPlayer.nAnimkeynow == 3 || g_XPlayer.nAnimkeynow == 5) && g_nCntTimeXPlayer == 1)
			{
				PlaySound(SOUND_LABEL_SE_WALK);
			}
		}
		if (g_XPlayer.nAnimnow == PLAYERANIM_RUN)
		{
			if ((g_XPlayer.nAnimkeynow == 3 || g_XPlayer.nAnimkeynow == 9) && g_nCntTimeXPlayer == 1)
			{
				PlaySound(SOUND_LABEL_SE_WALK);
			}
		}

		if (g_XPlayer.pos.x < -4810.0f)
		{
			g_XPlayer.pos.x = -4810.0f;
		}
		if (g_XPlayer.pos.x > 4810.0f)
		{
			g_XPlayer.pos.x = 4810.0f;
		}
		if (g_XPlayer.pos.z < -4810.0f)
		{
			g_XPlayer.pos.z = -4810.0f;
		}
		if (g_XPlayer.pos.z > 4810.0f)
		{
			g_XPlayer.pos.z = 4810.0f;
		}

		{
			// 目標向き調整
			if (g_XPlayer.rotDest.y > D3DX_PI)
			{
				g_XPlayer.rotDest.y -= D3DX_PI * 2.0f;
			}
			if (g_XPlayer.rotDest.y < -D3DX_PI)
			{
				g_XPlayer.rotDest.y += D3DX_PI * 2.0f;
			}
			// 差分設定
			fDiff = (g_XPlayer.rotDest.y - g_XPlayer.rot.y);

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
			g_XPlayer.rot.y += fDiff * 0.1f;

			// 現在向き調整
			if (g_XPlayer.rot.y > D3DX_PI)
			{
				g_XPlayer.rot.y -= D3DX_PI * 2.0f;
			}
			if (g_XPlayer.rot.y < -D3DX_PI)
			{
				g_XPlayer.rot.y += D3DX_PI * 2.0f;
			}
		}

		// 移動値代入
		g_XPlayer.pos += g_XPlayer.move;

		// 慣性処理
		g_XPlayer.move.x += (0.0f - g_XPlayer.move.x) * XPLAYER_DOWNSPPED;
		g_XPlayer.move.z += (0.0f - g_XPlayer.move.z) * XPLAYER_DOWNSPPED;

		// オブジェクト当たり判定
		CollisionXObject(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax);
		CollisionMeshField(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax);
		CollisionXEnemy(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax);
		
		if (CollsionItem(g_XPlayer.vtxMin, g_XPlayer.vtxMax, &g_XPlayer.pos, &g_XPlayer.posOld) == true)
		{
			if (g_XPlayer.nAnimnow != PLAYERANIM_ATTACK)
			{
				g_XPlayer.bAttack = true;

				g_nCntTimeXPlayer = 1;
				g_XPlayer.nAnimkeynow = 0;
				g_XPlayer.nAnimChange = 1;
			}

			g_XPlayer.nAnimnow = PLAYERANIM_ATTACK;
		}

		D3DXVECTOR3 nShadow;

		nShadow = D3DXVECTOR3(g_XPlayer.aModel[0].mtxWorld._41, g_XPlayer.aModel[0].mtxWorld._42, g_XPlayer.aModel[0].mtxWorld._43);

		SetPostionShadow(g_XPlayer.nIdxShadow, nShadow, nShadow);	// 影位置更新

		if (g_XPlayer.nAnimChange == 1)
		{
			if (g_nCntTimeXPlayer % g_XPlayer.nAnimBlend[g_XPlayer.nAnimnow] == 0)
			{
				g_nCntTimeXPlayer = 0;
				g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
				g_XPlayer.nAnimChange = 0;
			}
		}
		else
		{
			if (g_nCntTimeXPlayer % g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][g_XPlayer.nAnimkeynow] == 0)
			{
				g_nCntTimeXPlayer = 0;
				g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];

				if (g_XPlayer.nAnimloop[g_XPlayer.nAnimnow] != 1 && g_XPlayer.nAnimkeynow == g_XPlayer.nAnimkey[g_XPlayer.nAnimnow] - 1)
				{
					g_nCntTimeXPlayer = 1;
					g_XPlayer.nAnimkeynow = 0;
					g_XPlayer.nAnimChange = 1;

					if (g_XPlayer.nAnimnow == PLAYERANIM_ATTACK)
					{
						g_XPlayer.bAttack = false;
					}

					g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
				}
			}
		}

		if (g_XPlayer.nAnimChange == 1)
		{
			frac = (g_XPlayer.nAnimBlend[g_XPlayer.nAnimnow] - g_nCntTimeXPlayer) * 1.0f;;
		}
		else
		{
			frac = (g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] - g_nCntTimeXPlayer) * 1.0f;
		}

		g_XPlayer.aModel[0].posDest = g_XPlayer.aModel[0].Initpos + g_XPlayer.aModel[0].posAnim[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow + 1) % (g_XPlayer.nAnimkey[g_XPlayer.nAnimnow])];

		// 差分設定
		fDiff = (g_XPlayer.aModel[0].posDest.x - g_XPlayer.aModel[0].pos.x);
		// 現在向き設定
		g_XPlayer.aModel[0].pos.x += fDiff / frac;

		// 差分設定
		fDiff = (g_XPlayer.aModel[0].posDest.y - g_XPlayer.aModel[0].pos.y);
		// 現在向き設定
		g_XPlayer.aModel[0].pos.y += fDiff / frac;

		// 差分設定
		fDiff = (g_XPlayer.aModel[0].posDest.z - g_XPlayer.aModel[0].pos.z);
		// 現在向き設定
		g_XPlayer.aModel[0].pos.z += fDiff / frac;

		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			g_XPlayer.aModel[nCntXPlayer].rotDest = g_XPlayer.aModel[nCntXPlayer].rotAnim[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow + 1) % (g_XPlayer.nAnimkey[g_XPlayer.nAnimnow])];

			{
				// 目標向き調整
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.x > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.x -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.x < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.x += D3DX_PI * 2.0f;
				}
				// 差分設定
				fDiff = (g_XPlayer.aModel[nCntXPlayer].rotDest.x - g_XPlayer.aModel[nCntXPlayer].rot.x);

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
				g_XPlayer.aModel[nCntXPlayer].rot.x += fDiff / frac;

				// 現在向き調整
				if (g_XPlayer.aModel[nCntXPlayer].rot.x > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.x -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rot.x < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.x += D3DX_PI * 2.0f;
				}

				// 目標向き調整
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.y > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.y -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.y < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.y += D3DX_PI * 2.0f;
				}
				// 差分設定
				fDiff = (g_XPlayer.aModel[nCntXPlayer].rotDest.y - g_XPlayer.aModel[nCntXPlayer].rot.y);

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
				g_XPlayer.aModel[nCntXPlayer].rot.y += fDiff / frac;

				// 現在向き調整
				if (g_XPlayer.aModel[nCntXPlayer].rot.y > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.y -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rot.y < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.y += D3DX_PI * 2.0f;
				}

				// 目標向き調整
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.z > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.z -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rotDest.z < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rotDest.z += D3DX_PI * 2.0f;
				}
				// 差分設定
				fDiff = (g_XPlayer.aModel[nCntXPlayer].rotDest.z - g_XPlayer.aModel[nCntXPlayer].rot.z);

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
				g_XPlayer.aModel[nCntXPlayer].rot.z += fDiff / frac;

				// 現在向き調整
				if (g_XPlayer.aModel[nCntXPlayer].rot.z > D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.z -= D3DX_PI * 2.0f;
				}
				if (g_XPlayer.aModel[nCntXPlayer].rot.z < -D3DX_PI)
				{
					g_XPlayer.aModel[nCntXPlayer].rot.z += D3DX_PI * 2.0f;
				}
			}
		}
	}
}

//=========================================================================================================================
// プレイヤーの描画処理
//=========================================================================================================================
void DrawXPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pmat;							// マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;						// 
	int nCntXPlayer;

	if (g_XPlayer.bUse == true)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_XPlayer.mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll
		(
			&mtxRot,
			g_XPlayer.rot.y,
			g_XPlayer.rot.x,
			g_XPlayer.rot.z
		);

		D3DXMatrixMultiply
		(
			&g_XPlayer.mtxWorld,
			&g_XPlayer.mtxWorld,
			&mtxRot
		);

		// 位置を反映
		D3DXMatrixTranslation
		(
			&mtxTrans,
			g_XPlayer.pos.x,
			g_XPlayer.pos.y,
			g_XPlayer.pos.z
		);

		D3DXMatrixMultiply
		(
			&g_XPlayer.mtxWorld,
			&g_XPlayer.mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_XPlayer.mtxWorld);

		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			if (g_XPlayer.aModel[nCntXPlayer].nIndexParent < 0)
			{
				mtxParent = g_XPlayer.mtxWorld;
			}
			else
			{
				mtxParent = g_XPlayer.aModel[g_XPlayer.aModel[nCntXPlayer].nIndexParent].mtxWorld;
			}

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_XPlayer.aModel[nCntXPlayer].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_XPlayer.aModel[nCntXPlayer].rot.y,
				g_XPlayer.aModel[nCntXPlayer].rot.x,
				g_XPlayer.aModel[nCntXPlayer].rot.z
			);

			D3DXMatrixMultiply
			(
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&mtxRot
			);

			// 位置を反映
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_XPlayer.aModel[nCntXPlayer].pos.x,
				g_XPlayer.aModel[nCntXPlayer].pos.y,
				g_XPlayer.aModel[nCntXPlayer].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&mtxTrans
			);

			D3DXMatrixMultiply
			(
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&g_XPlayer.aModel[nCntXPlayer].mtxWorld,
				&mtxParent
			);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_XPlayer.aModel[nCntXPlayer].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pmat = (D3DXMATERIAL*)g_XPlayer.aModel[nCntXPlayer].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_XPlayer.aModel[nCntXPlayer].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

				// テクスチャ設定
				pDevice->SetTexture(0, NULL);

				// プレイヤー(パーツ)の描画
				g_XPlayer.aModel[nCntXPlayer].pMesh->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=========================================================================================================================
// プレイヤーの設定処理
//=========================================================================================================================
void SetXPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_XPlayer.bUse == false)
	{
		g_XPlayer.pos = pos;
		g_XPlayer.Initpos = pos;
		g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XPlayer.rot = rot;
		g_XPlayer.rotDest = rot;
		g_XPlayer.Initrot = rot;
		g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
		g_XPlayer.nAnimChange = 0;
		g_XPlayer.nAnimkeynow = 0;
		g_XPlayer.nIdxShadow = SetShadow(g_XPlayer.pos, g_XPlayer.rot, 20.0f, 20.0f);
		g_XPlayer.bUse = true;

		for (int nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			g_XPlayer.aModel[nCntXPlayer].pos = g_XPlayer.aModel[nCntXPlayer].Initpos;
			g_XPlayer.aModel[nCntXPlayer].rot = g_XPlayer.aModel[nCntXPlayer].rotAnim[0][0];
		}
	}
}

//=========================================================================================================================
// プレイヤーの取得処理
//=========================================================================================================================
XPLAYER *GetXPlayer(void)
{
	return &g_XPlayer;
}

//=========================================================================================================================
// プレイヤーの衝突処理
//=========================================================================================================================
bool CollisionXPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	bool bLand = false;

	if (g_XPlayer.bUse == true)
	{// 使用されている
		if (pPos->y + vtxMax->y > g_XPlayer.pos.y + g_XPlayer.vtxMin.y && pPos->y + vtxMin->y < g_XPlayer.pos.y + g_XPlayer.vtxMax.y)
		{// X範囲確認
			if (pPos->x + vtxMax->x > g_XPlayer.pos.x + g_XPlayer.vtxMin.x && pPos->x + vtxMin->x < g_XPlayer.pos.x + g_XPlayer.vtxMax.x)
			{// X範囲確認
				if (pPosOld->z + vtxMin->z >= g_XPlayer.pos.z + g_XPlayer.vtxMax.z && g_XPlayer.pos.z + g_XPlayer.vtxMax.z > pPos->z + vtxMin->z)
				{// Z上底判定
					pPos->z = g_XPlayer.pos.z + g_XPlayer.vtxMax.z - vtxMin->z;
					//pMove->x = 0.0f;
					pMove->z = 0.0f;

					bLand = true;
				}
				else if (pPosOld->z + vtxMax->z <= g_XPlayer.pos.z + g_XPlayer.vtxMin.z && g_XPlayer.pos.z + g_XPlayer.vtxMin.z < pPos->z + vtxMax->z)
				{// Z下底判定
					pPos->z = g_XPlayer.pos.z + g_XPlayer.vtxMin.z - vtxMax->z;
					//pMove->x = 0.0f;
					pMove->z = 0.0f;

					bLand = true;
				}
			}

			if (pPos->z + vtxMax->z > g_XPlayer.pos.z + g_XPlayer.vtxMin.z && pPos->z + vtxMin->z < g_XPlayer.pos.z + g_XPlayer.vtxMax.z)
			{// Z範囲確認
				if (pPosOld->x + vtxMin->x >= g_XPlayer.pos.x + g_XPlayer.vtxMax.x && g_XPlayer.pos.x + g_XPlayer.vtxMax.x > pPos->x + vtxMin->x)
				{// X右辺判定
					pPos->x = g_XPlayer.pos.x + g_XPlayer.vtxMax.x - vtxMin->x;
					pMove->x = 0.0f;
					//pMove->z = 0.0f;

					bLand = true;
				}
				else if (pPosOld->x + vtxMax->x <= g_XPlayer.pos.x + g_XPlayer.vtxMin.x && g_XPlayer.pos.x + g_XPlayer.vtxMin.x < pPos->x + vtxMax->x)
				{// X左辺判定
					pPos->x = g_XPlayer.pos.x + g_XPlayer.vtxMin.x - vtxMax->x;
					pMove->x = 0.0f;
					//pMove->z = 0.0f;

					bLand = true;
				}
			}
		}

		if (pPos->x + vtxMax->x > g_XPlayer.pos.x + g_XPlayer.vtxMin.x && pPos->x + vtxMin->x < g_XPlayer.pos.x + g_XPlayer.vtxMax.x)
		{// X範囲確認
			if (pPos->z + vtxMax->z > g_XPlayer.pos.z + g_XPlayer.vtxMin.z && pPos->z + vtxMin->z < g_XPlayer.pos.z + g_XPlayer.vtxMax.z)
			{// Z範囲確認
				if (pPosOld->y + vtxMin->y >= g_XPlayer.pos.y + g_XPlayer.vtxMax.y && g_XPlayer.pos.y + g_XPlayer.vtxMax.y > pPos->y + vtxMin->y)
				{// X右辺判定
					pPos->y = g_XPlayer.pos.y + g_XPlayer.vtxMax.y - vtxMin->y;
					//pMove->x = 0.0f;
					//pMove->z = 0.0f;

					bLand = true;
				}
				else if (pPosOld->y + vtxMax->y <= g_XPlayer.pos.y + g_XPlayer.vtxMin.y && g_XPlayer.pos.y + g_XPlayer.vtxMin.y < pPos->x + vtxMax->y)
				{// X左辺判定
					pPos->y = g_XPlayer.pos.y + g_XPlayer.vtxMin.y - vtxMax->y;
					//pMove->x = 0.0f;
					//pMove->z = 0.0f;

					bLand = true;
				}
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// プレイヤーを消す
//=========================================================================================================================
void DeleteXPlayer(void)
{
	if (g_XPlayer.bUse == true)
	{
		g_XPlayer.bUse = false;					//使用していない状態に設定する
		DeleteShadow(g_XPlayer.nIdxShadow);		//影を消す
	}
}