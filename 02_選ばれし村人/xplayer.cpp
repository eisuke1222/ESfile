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
#include "xplayer.h"
#include "xweapon.h"
#include "xenemy.h"
#include "xobject.h"
#include "billboardeffect.h"
#include "billboardObject.h"
#include "meshWall.h"
#include "meshfield.h"
#include "shadow.h"
#include "camera.h"
#include "fade.h"
#include "sound.h"
#include "input.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define XPLAYER_UPSPPED_RUN		(1.2f)						// プレイヤー加速速度（走る）
#define XPLAYER_DOWNSPPED		(0.15f)						// プレイヤー減速速度

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
XPLAYER							g_XPlayer;					// プレイヤー情報
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
	g_XPlayer.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_XPlayer.vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	g_XPlayer.vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	g_XPlayer.nAnimChange = 0;
	g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
	g_XPlayer.nAnimkeynow = 0;
	g_XPlayer.nIdxShadow = 0;
	g_XPlayer.nWeapon = WEAPONTYPE_NONE;
	g_XPlayer.nChargeNum = 0;
	g_XPlayer.nCntstate = 0;
	g_XPlayer.state = PLAYERSTATE_NONE;
	g_XPlayer.nLife = 0;
	g_XPlayer.nInitLife = 0;
	g_XPlayer.nWeapon_1Lv = 0;
	g_XPlayer.nWeapon_2Lv = 0;
	g_XPlayer.nWeapon_3Lv = 0;
	g_XPlayer.bAttack = false;
	g_XPlayer.bNextAttack = false;
	g_XPlayer.nCntChargeAttack = 0;
	g_XPlayer.bChargeAttack = false;
	g_XPlayer.bChargeOn = false;
	g_XPlayer.bMove = false;
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

	// 判定
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
	MODE mode = GetMode();
	GAMESTATE game = GetGameState();
	FADE fade = GetFade();
	Camera *pCamera = GetCamera();
	float fDiff = 0.0f;
	float frac = 0.0f;
	int nCntXPlayer;

	DIJOYSTATE pStick = GetStick();

	if (g_XPlayer.bChargeAttack == false)
	{// 攻撃チェンジ
		g_nCntTimeXPlayer++;
		g_XPlayer.nCntChargeAttack = 0;
	}

	if (g_XPlayer.bUse == true)
	{// 使っている
		switch (mode)
		{
		case MODE_GAME:
			// ゲームモード
			if (g_XPlayer.bMove == false)
			{// 動けない場合　ニュートラルモーション
				g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
			}
			else if (g_XPlayer.bMove == true)
			{
				switch (game)
				{
				case GAMESTATE_TUTORIAL:
					// ゲームチュートリアル
					if (g_XPlayer.pos.x + g_XPlayer.vtxMax.x > 500.0f)
					{
						g_XPlayer.pos.x = 500.0f - g_XPlayer.vtxMax.x;
						g_XPlayer.move.x = 0.0f;
					}
					if (g_XPlayer.pos.x + g_XPlayer.vtxMin.x < -500.0f)
					{
						g_XPlayer.pos.x = -500.0f - g_XPlayer.vtxMin.x;
						g_XPlayer.move.x = 0.0f;
					}
					if (g_XPlayer.pos.z + g_XPlayer.vtxMax.z > 500.0f)
					{
						g_XPlayer.pos.z = 500.0f - g_XPlayer.vtxMax.z;
						g_XPlayer.move.z = 0.0f;
					}
					if (g_XPlayer.pos.z + g_XPlayer.vtxMin.z < -500.0f)
					{
						g_XPlayer.pos.z = -500.0f - g_XPlayer.vtxMin.z;
						g_XPlayer.move.z = 0.0f;
					}

					break;
				case GAMESTATE_PLAY:
					// ゲームプレイ
					switch (g_XPlayer.state)
					{// 状態
					case PLAYERSTATE_NORMAL:
						break;
					case PLAYERSTATE_DAMAGE:	// ダメージ

						g_XPlayer.nCntstate++;

						// 点滅
						if (g_XPlayer.nCntstate % 5 % 2 == 0)
						{
							g_XPlayer.col.a = 0.3f;
						}
						else
						{
							g_XPlayer.col.a = 1.0f;
						}

						if (g_XPlayer.nCntstate > 20)
						{// 状態を戻す
							g_XPlayer.nCntstate = 0;
							g_XPlayer.state = PLAYERSTATE_NORMAL;
							g_XPlayer.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						}

						break;
					case PLAYERSTATE_DIE:
						break;
					}

					if (g_XPlayer.pos.z + g_XPlayer.vtxMin.z > 1650.0f)
					{// マップ移動
						if (g_XPlayer.pos.x < -500.0f)
						{
							SetPlayerRespawn(1);
						}
						else if (g_XPlayer.pos.x > 500.0f)
						{
							SetPlayerRespawn(2);
						}
						else
						{
							SetPlayerRespawn(0);
						}

						SetFadeGame(GAMESTATE_PLAY);

						g_nCntTimeXPlayer = 1;
						g_XPlayer.nAnimkeynow = 0;
						g_XPlayer.nAnimChange = 1;

						g_XPlayer.bMove = false;
					}

					// 範囲チェック
					if (g_XPlayer.pos.x + g_XPlayer.vtxMax.x > 1750.0f)
					{
						g_XPlayer.pos.x = 1750.0f - g_XPlayer.vtxMax.x;
						g_XPlayer.move.x = 0.0f;
					}
					if (g_XPlayer.pos.x + g_XPlayer.vtxMin.x < -1750.0f)
					{
						g_XPlayer.pos.x = -1750.0f - g_XPlayer.vtxMin.x;
						g_XPlayer.move.x = 0.0f;
					}
					if (g_XPlayer.pos.z + g_XPlayer.vtxMax.z > 1750.0f)
					{
						g_XPlayer.pos.z = 1750.0f - g_XPlayer.vtxMax.z;
						g_XPlayer.move.z = 0.0f;
					}
					if (g_XPlayer.pos.z + g_XPlayer.vtxMin.z < -1750.0f)
					{
						g_XPlayer.pos.z = -1750.0f - g_XPlayer.vtxMin.z;
						g_XPlayer.move.z = 0.0f;
					}

					break;
				}

				g_XPlayer.posOld = g_XPlayer.pos;	// 位置保管

				if (g_XPlayer.bAttack == false)
				{// 攻撃していない
					if ((GetKeyboardPress(DIK_UP) == true || pStick.rgdwPOV[0] == 0) || pStick.lY < -100)
					{// 任意のキー（上）を押した場合
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						g_XPlayer.nAnimnow = PLAYERANIM_WALK;	// 歩く

						if ((GetKeyboardPress(DIK_LEFT) == true || pStick.rgdwPOV[0] == 27000) || pStick.lX < -100)
						{// 任意のキー（左）を押した場合
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.75f + pCamera->rot.y);
						}
						else if ((GetKeyboardPress(DIK_RIGHT) == true || pStick.rgdwPOV[0] == 9000) || pStick.lX > 100)
						{// 任意のキー（右）を押した場合
							g_XPlayer.move.x -= sinf(-D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (-D3DX_PI * 0.75f + pCamera->rot.y);
						}
						else
						{// 上のみの場合
							g_XPlayer.move.x -= sinf(D3DX_PI * 1.0f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 1.0f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 1.0f + pCamera->rot.y);
						}
					}
					else if ((GetKeyboardPress(DIK_DOWN) == true || pStick.rgdwPOV[0] == 18000) || pStick.lY > 100)
					{// 任意のキー（下）を押した場合
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}
						
						g_XPlayer.nAnimnow = PLAYERANIM_WALK;	// 歩く

						if ((GetKeyboardPress(DIK_LEFT) == true || pStick.rgdwPOV[0] == 27000) || pStick.lX < -100)
						{// 任意のキー（左）を押した場合
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.25f + pCamera->rot.y);
						}
						else if ((GetKeyboardPress(DIK_RIGHT) == true || pStick.rgdwPOV[0] == 9000) || pStick.lX > 100)
						{// 任意のキー（右）を押した場合
							g_XPlayer.move.x -= sinf(-D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (-D3DX_PI * 0.25f + pCamera->rot.y);
						}
						else
						{// 下のみの場合
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.0f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.0f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.0f + pCamera->rot.y);
						}
					}
					else if ((GetKeyboardPress(DIK_LEFT) == true || pStick.rgdwPOV[0] == 27000) || pStick.lX < -100)
					{// 任意のキー（左）を押した場合
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						g_XPlayer.nAnimnow = PLAYERANIM_WALK;	// 歩く

						if ((GetKeyboardPress(DIK_UP) == true || pStick.rgdwPOV[0] == 0 || pStick.lY < -100))
						{// 任意のキー（上）を押した場合
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.75f + pCamera->rot.y);
						}
						else if ((GetKeyboardPress(DIK_DOWN) == true || pStick.rgdwPOV[0] == 18000 || pStick.lY > 100))
						{// 任意のキー（下）を押した場合
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.25f + pCamera->rot.y);
						}
						else
						{// 左のみの場合
							g_XPlayer.move.x -= sinf(D3DX_PI * 0.5f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(D3DX_PI * 0.5f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (D3DX_PI * 0.5f + pCamera->rot.y);
						}
					}
					else if ((GetKeyboardPress(DIK_RIGHT) == true || pStick.rgdwPOV[0] == 9000) || pStick.lX > 100)
					{// 任意のキー（右）を押した場合
						if (g_XPlayer.nAnimnow != PLAYERANIM_WALK)
						{
							g_nCntTimeXPlayer = 1;
							g_XPlayer.nAnimkeynow = 0;
							g_XPlayer.nAnimChange = 1;
						}

						g_XPlayer.nAnimnow = PLAYERANIM_WALK;	// 歩く

						if ((GetKeyboardPress(DIK_UP) == true || pStick.rgdwPOV[0] == 0 || pStick.lY < -100))
						{// 任意のキー（上）を押した場合
							g_XPlayer.move.x -= sinf(-D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(-D3DX_PI * 0.75f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (-D3DX_PI * 0.75f + pCamera->rot.y);
						}
						else if ((GetKeyboardPress(DIK_DOWN) == true || pStick.rgdwPOV[0] == 18000 || pStick.lY > 100))
						{// 任意のキー（下）を押した場合
							g_XPlayer.move.x -= sinf(-D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(-D3DX_PI * 0.25f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (-D3DX_PI * 0.25f + pCamera->rot.y);
						}
						else
						{// 右のみの場合
							g_XPlayer.move.x -= sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;
							g_XPlayer.move.z -= cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * XPLAYER_UPSPPED_RUN;

							g_XPlayer.rotDest.y = (-D3DX_PI * 0.5f + pCamera->rot.y);
						}
					}
					else
					{// 操作キーを押していない
						if (g_XPlayer.bAttack == false && g_XPlayer.bNextAttack == false)
						{// ニュートラルモーションへ
							if (g_XPlayer.nAnimnow != PLAYERANIM_NEUTRAL)
							{
								g_nCntTimeXPlayer = 1;
								g_XPlayer.nAnimkeynow = 0;
								g_XPlayer.nAnimChange = 1;
							}

							g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;	// ニュートラル
						}
					}
				}

				if (g_XPlayer.nAnimnow == PLAYERANIM_WALK)
				{// 足音
					if (g_XPlayer.nAnimkeynow % 2 == 0)
					{
						PlaySound(SOUND_LABEL_SE_WOLK);
					}
				}

				if (g_XPlayer.bAttack == false)
				{// 攻撃していない
					// 武器切り替え
					if (GetButtonTrigger(3) == true)
					{
						if (((g_XPlayer.nWeapon + 1) % 3) == WEAPONTYPE_SWORD)
						{
							SetXWeapon(WEAPONTYPE_SWORD);
						}
						else if (((g_XPlayer.nWeapon + 1) % 3) == WEAPONTYPE_BOW)
						{
							SetXWeapon(WEAPONTYPE_BOW);
						}
						else if (((g_XPlayer.nWeapon + 1) % 3) == WEAPONTYPE_WAND)
						{
							SetXWeapon(WEAPONTYPE_WAND);
						}
					}

					if (GetKeyboardTrigger(DIK_1) == true)
					{
						SetXWeapon(WEAPONTYPE_SWORD);
					}
					else if (GetKeyboardTrigger(DIK_2) == true)
					{
						SetXWeapon(WEAPONTYPE_BOW);
					}
					else if (GetKeyboardTrigger(DIK_3) == true)
					{
						SetXWeapon(WEAPONTYPE_WAND);
					}
				}

				if (fade == FADETYPE_NONE)
				{
					if ((GetKeyboardTrigger(DIK_SPACE) == true || GetButtonTrigger(0) == true))
					{// 攻撃
						if (g_XPlayer.nWeapon == WEAPONTYPE_SWORD)
						{// 剣
							if (g_XPlayer.bAttack == false)
							{// 初撃
								if (g_XPlayer.nAnimnow != PLAYERANIM_SWORD_1 ||
									g_XPlayer.nAnimnow != PLAYERANIM_SWORD_2 ||
									g_XPlayer.nAnimnow != PLAYERANIM_SWORD_3)
								{
									g_XPlayer.bAttack = true;

									g_nCntTimeXPlayer = 1;
									g_XPlayer.nAnimChange = 1;
								}

								g_XPlayer.nAnimkeynow = 0;

								g_XPlayer.nAnimnow = PLAYERANIM_SWORD_1;
							}
							else
							{// 連撃
								if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_1 ||
									g_XPlayer.nAnimnow == PLAYERANIM_SWORD_2)
								{
									g_XPlayer.bNextAttack = true;
								}
							}
						}
						else if (g_XPlayer.nWeapon == WEAPONTYPE_BOW)
						{// 弓
							if (g_XPlayer.bAttack == false)
							{// 撃つ
								if (g_XPlayer.nAnimnow != PLAYERANIM_BOW)
								{
									g_XPlayer.bAttack = true;

									g_nCntTimeXPlayer = 1;
									g_XPlayer.nAnimChange = 1;
								}

								g_XPlayer.nAnimkeynow = 0;

								g_XPlayer.nAnimnow = PLAYERANIM_BOW;
							}
						}
						else if (g_XPlayer.nWeapon == WEAPONTYPE_WAND)
						{// 杖
							if (g_XPlayer.bAttack == false)
							{// 撃つ
								if (g_XPlayer.nAnimnow != PLAYERANIM_WAND)
								{
									g_XPlayer.bAttack = true;

									g_nCntTimeXPlayer = 1;
									g_XPlayer.nAnimChange = 1;
								}

								g_XPlayer.nAnimkeynow = 0;

								g_XPlayer.nAnimnow = PLAYERANIM_WAND;
							}
						}
					}
				}

				if (g_XPlayer.bAttack == true)
				{// 攻撃していたら
					if (g_XPlayer.nWeapon == WEAPONTYPE_SWORD)
					{// 剣
						if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_1)
						{// 1撃
							if (g_XPlayer.nAnimkeynow == 1)
							{
								PlaySound(SOUND_LABEL_SE_ATTACK_0);

								if (g_nCntTimeXPlayer == 2 && g_XPlayer.nWeapon_1Lv >= 12)
								{// レベルで斬撃
									SetBillboardObject(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 100.0f, 0, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 100.0f),
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 25.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 25.0f), g_XPlayer.rot,
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(80.0f, 100.0f, 0.0f), BILLBOARDTEXTURE_SWORD);
								}
							}
						}
						else if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_2)
						{// 2撃
							if (g_XPlayer.nAnimkeynow == 0)
							{
								PlaySound(SOUND_LABEL_SE_ATTACK_0);

								if (g_nCntTimeXPlayer == 2 && g_XPlayer.nWeapon_1Lv >= 12)
								{// レベルで斬撃
									SetBillboardObject(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 100.0f, 0, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 100.0f),
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 25.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 25.0f), g_XPlayer.rot,
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(80.0f, 100.0f, 0.0f), BILLBOARDTEXTURE_SWORD);
								}
							}
						}
						else if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_3)
						{// 3撃
							if (g_XPlayer.nAnimkeynow == 2)
							{
								PlaySound(SOUND_LABEL_SE_ATTACK_0);

								if (g_nCntTimeXPlayer == 2 && g_XPlayer.nWeapon_1Lv >= 6)
								{// レベルで斬撃
									SetBillboardObject(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 100.0f, 0, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 100.0f),
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 25.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 25.0f), g_XPlayer.rot,
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(80.0f, 100.0f, 0.0f), BILLBOARDTEXTURE_SWORD);
								}
							}
						}
					}
					else if (g_XPlayer.nWeapon == WEAPONTYPE_BOW)
					{// 杖
 						if (g_XPlayer.nAnimkeynow == 1)
						{
							if ((GetKeyboardPress(DIK_SPACE) == true || GetButtonPress(0) == true))
							{// チャージ
								g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

								if (g_XPlayer.bChargeOn == false)
								{
									g_XPlayer.bChargeAttack = true;
									g_XPlayer.bChargeOn = true;
								}
							}
							else
							{// 撃つ
								if (g_XPlayer.bChargeAttack == true && g_XPlayer.nChargeNum != 0)
								{
									g_nCntTimeXPlayer = 0;
									g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
								}

								g_XPlayer.bChargeAttack = false;
							}

							if (g_XPlayer.bChargeAttack == true)
							{// チャージ
								if (g_XPlayer.nWeapon_2Lv < 6)
								{// レベルでチャージ間隔変更
									if (g_XPlayer.nCntChargeAttack == 0)
									{
										g_XPlayer.nChargeNum = 0;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);
									}
									else if (g_XPlayer.nCntChargeAttack == 40)
									{
										g_XPlayer.nChargeNum = 1;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);
									}
									else if (g_XPlayer.nCntChargeAttack == 80)
									{
										g_XPlayer.nChargeNum = 2;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);

										g_nCntTimeXPlayer = 0;
										g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
										g_XPlayer.bChargeAttack = false;
									}
								}
								else
								{// レベルでチャージ間隔変更
									if (g_XPlayer.nCntChargeAttack == 0)
									{
										g_XPlayer.nChargeNum = 0;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);
									}
									else if (g_XPlayer.nCntChargeAttack == 20)
									{
										g_XPlayer.nChargeNum = 1;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);
									}
									else if (g_XPlayer.nCntChargeAttack == 40)
									{
										g_XPlayer.nChargeNum = 2;
										SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);

										PlaySound(SOUND_LABEL_SE_ATTACK_99);

										g_nCntTimeXPlayer = 0;
										g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
										g_XPlayer.bChargeAttack = false;
									}
								}

								g_XPlayer.nCntChargeAttack++;
							}
						}

						// レベルで打ち方を変える
						if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == 1)
						{
							if (g_XPlayer.bChargeAttack == false)
							{
								if (g_XPlayer.nChargeNum == 0)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
								}
								else if (g_XPlayer.nChargeNum == 1)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f));
								}
								else if (g_XPlayer.nChargeNum == 2)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
								}

								PlaySound(SOUND_LABEL_SE_ATTACK_1);
							}
						}
						else if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 2 && g_XPlayer.nChargeNum == 2)
						{
							SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));

							PlaySound(SOUND_LABEL_SE_ATTACK_1);
						}
						else if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] && g_XPlayer.nChargeNum == 2)
						{
							SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));

							PlaySound(SOUND_LABEL_SE_ATTACK_1);
						}

						// レベルで打ち方を変える
						if (g_XPlayer.nWeapon_2Lv >= 12)
						{
							if (g_XPlayer.nChargeNum == 0)
							{
								if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 2)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
								}
								else if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)])
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
								}
							}
							else if (g_XPlayer.nChargeNum == 1)
							{
								if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 2)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f));
								}
								else if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)])
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f));
								}
							}
							else if (g_XPlayer.nChargeNum == 2)
							{
								if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 2 - g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 4)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f));
								}
								else if (g_XPlayer.nAnimkeynow == 1 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] - g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] / 2)
								{
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 16)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 8)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI + (D3DX_PI / 8)) * 15.0f));
									SeBulletWeapon(D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 8)) * 15.0f, -0.5f, cosf(g_XPlayer.rot.y + D3DX_PI - (D3DX_PI / 8)) * 15.0f));
								}
							}
						}
					}
					else if (g_XPlayer.nWeapon == WEAPONTYPE_WAND)
					{// 杖
						if (g_XPlayer.nAnimkeynow == 0)
						{
							if ((GetKeyboardPress(DIK_SPACE) == true || GetButtonPress(0) == true))
							{// チャージ
								g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

								if (g_XPlayer.bChargeOn == false)
								{
									g_XPlayer.bChargeAttack = true;
									g_XPlayer.bChargeOn = true;
								}
							}
							else
							{// 撃つ
								if (g_XPlayer.bChargeAttack == true)
								{
									g_nCntTimeXPlayer = 1;
									g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
								}

								g_XPlayer.bChargeAttack = false;
							}

							if (g_XPlayer.bChargeAttack == true)
							{// レベルとチャージで変更
								if (g_XPlayer.nCntChargeAttack == 0)
								{
									g_XPlayer.nChargeNum = 0;
									SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);
									SetBillboardObject(g_XPlayer.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_XPlayer.rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BILLBOARDTEXTURE_WAND_1);

									PlaySound(SOUND_LABEL_SE_ATTACK_99);

									if (g_XPlayer.nWeapon_3Lv >= 12)
									{
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);

										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
									}
								}
								else if (g_XPlayer.nCntChargeAttack == 60)
								{
									g_XPlayer.nChargeNum = 1;
									SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);
									SetBillboardObject(g_XPlayer.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_XPlayer.rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BILLBOARDTEXTURE_WAND_2);

									PlaySound(SOUND_LABEL_SE_ATTACK_99);

									if (g_XPlayer.nWeapon_3Lv >= 12)
									{
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);

										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
									}
								}
								else if (g_XPlayer.nCntChargeAttack == 120)
								{
									g_XPlayer.nChargeNum = 2;
									SetBillboardParticleEffect(g_XPlayer.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(5.0f, 5.0f, 0.0f), 0.02f, 30, EFFECTSTATE_NORMAL);
									SetBillboardObject(g_XPlayer.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_XPlayer.rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), BILLBOARDTEXTURE_WAND_3);

									PlaySound(SOUND_LABEL_SE_ATTACK_99);

									if (g_XPlayer.nWeapon_3Lv >= 12)
									{
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 2) + D3DX_PI) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);

										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y - (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
										SetBillboardBulletEffect(
											D3DXVECTOR3(g_XPlayer.pos.x, 50, g_XPlayer.pos.z),
											g_XPlayer.rot,
											D3DXVECTOR3(sinf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f, 0.0f, cosf(g_XPlayer.rot.y + (D3DX_PI / 4)) * 3.0f),
											D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.03f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER2);
									}

									g_nCntTimeXPlayer = 1;
									g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
									g_XPlayer.bChargeAttack = false;
								}

								g_XPlayer.nCntChargeAttack++;
							}
						}
						else if (g_XPlayer.nAnimkeynow == 2 && g_nCntTimeXPlayer == g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][g_XPlayer.nAnimkeynow])
						{
							if (g_XPlayer.bChargeAttack == false)
							{
								if (g_XPlayer.nChargeNum == 0)
								{
									SetBillboardBulletEffect(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 50.0f, 50, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 50.0f),
										g_XPlayer.rot,
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 5.0f),
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), 0.1f, 150, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER);
								}
								else if (g_XPlayer.nChargeNum == 1)
								{
									SetBillboardBulletEffect(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 50.0f, 50, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 50.0f),
										g_XPlayer.rot,
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 5.0f),
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), 0.1f, 120, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER);
								}
								else if (g_XPlayer.nChargeNum == 2)
								{
									SetBillboardBulletEffect(
										D3DXVECTOR3(g_XPlayer.pos.x + sinf(g_XPlayer.rot.y + D3DX_PI) * 50.0f, 50, g_XPlayer.pos.z + cosf(g_XPlayer.rot.y + D3DX_PI) * 50.0f),
										g_XPlayer.rot,
										D3DXVECTOR3(sinf(g_XPlayer.rot.y + D3DX_PI) * 5.0f, 0.0f, cosf(g_XPlayer.rot.y + D3DX_PI) * 5.0f),
										D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), 0.1f, 100, g_XPlayer.nWeapon_3Lv / 3 + 1, EFFECTBULLET_PLAYER);
								}

								PlaySound(SOUND_LABEL_SE_ATTACK_2);
							}
						}
					}
				}
			}

			break;
		case MODE_RESULT:
			break;
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

		if (g_XPlayer.bMove == true)
		{
			// オブジェクト当たり判定
			CollisionXObject(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax);
			CollisionItem(&g_XPlayer.pos, &g_XPlayer.move, &g_XPlayer.vtxMax, &g_XPlayer.vtxMin);
			CollisionMeshField(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax);
			CollisionXEnemy(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax);

			if (CollisionXEnemyWeapon(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax) == true ||
				CollisionXWeaponBullet(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, &g_XPlayer.vtxMin, &g_XPlayer.vtxMax, WEAPON_ENEMY) == true)
			{
				if (g_XPlayer.state == ENEMYSTATE_NORMAL)
				{
					PlaySound(SOUND_LABEL_SE_DAMAGE);

					g_XPlayer.state = PLAYERSTATE_DAMAGE;
					g_XPlayer.nCntstate = 0;

					g_XPlayer.nLife -= 1;

					if (g_XPlayer.nLife <= 0)
					{
						SetGameState(GAMESTATE_GAMEOVER);
						DeleteXPlayer();
					}
				}
			}

			// 敵当たり判定
			if (CollisionEffect(&g_XPlayer.pos, &g_XPlayer.move, &g_XPlayer.vtxMax, &g_XPlayer.vtxMin, EFFECTBULLET_ENEMY) == true)
			{
				if (g_XPlayer.state == ENEMYSTATE_NORMAL)
				{
					PlaySound(SOUND_LABEL_SE_DAMAGE);

					g_XPlayer.state = PLAYERSTATE_DAMAGE;
					g_XPlayer.nCntstate = 0;

					if (g_XPlayer.nLife <= 0)
					{
						SetGameState(GAMESTATE_GAMEOVER);
						DeleteXPlayer();
					}
				}
			}
		}

		/*if (CollisionWall(&g_XPlayer.pos, &g_XPlayer.posOld, &g_XPlayer.move, NULL, D3DXVECTOR3(0.0f, 0.0f, 0.0f)) == true)
		{
			g_XPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}*/

		// 影
		D3DXVECTOR3 Shadowpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		Shadowpos.x = g_XPlayer.aModel[0].mtxWorld._41;
		Shadowpos.y = g_XPlayer.aModel[0].mtxWorld._42;
		Shadowpos.z = g_XPlayer.aModel[0].mtxWorld._43;

		SetPositionShadow(g_XPlayer.nIdxShadow, Shadowpos, g_XPlayer.rot);	// 影位置更新

		// アニメーション
		if (g_XPlayer.nAnimChange == 1)
		{
			if (g_nCntTimeXPlayer % g_XPlayer.nAnimBlend[g_XPlayer.nAnimnow] == 0)
			{
				g_nCntTimeXPlayer = 0;
				//g_XPlayer.nAnimkeynow = (g_XPlayer.nAnimkeynow + 1) % g_XPlayer.nAnimkey[g_XPlayer.nAnimnow];
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

					// アニメーションによって変更
					if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_1 ||
						g_XPlayer.nAnimnow == PLAYERANIM_SWORD_2 ||
						g_XPlayer.nAnimnow == PLAYERANIM_SWORD_3 ||
						g_XPlayer.nAnimnow == PLAYERANIM_BOW ||
						g_XPlayer.nAnimnow == PLAYERANIM_WAND)
					{
						g_XPlayer.nChargeNum = 0;
						g_XPlayer.bChargeOn = false;
						g_XPlayer.bChargeAttack = false;
						g_XPlayer.bAttack = false;
						DeleteBillboardObject(BILLBOARDTEXTURE_WAND_1);
						DeleteBillboardObject(BILLBOARDTEXTURE_WAND_2);
						DeleteBillboardObject(BILLBOARDTEXTURE_WAND_3);
					}

					if (g_XPlayer.bNextAttack == true)
					{
						if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_1)
						{
							g_XPlayer.nAnimnow = PLAYERANIM_SWORD_2;
							g_XPlayer.bNextAttack = false;
							g_XPlayer.bAttack = true;
							g_XPlayer.nAnimChange = 0;
						}
						else if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_2)
						{
							g_XPlayer.nAnimnow = PLAYERANIM_SWORD_3;
							g_XPlayer.bNextAttack = false;
							g_XPlayer.bAttack = true;
							g_XPlayer.nAnimChange = 0;
						}
					}
					else
					{
						if (g_XPlayer.nAnimnow == PLAYERANIM_SWORD_3)
						{
							g_XPlayer.move.x -= sinf(g_XPlayer.rot.y) * 7.0f;
							g_XPlayer.move.z -= cosf(g_XPlayer.rot.y) * 7.0f;
						}

						g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
					}
				}
			}
		}

		// ブレンド
		if (g_XPlayer.nAnimChange == 1)
		{
			frac = (g_XPlayer.nAnimBlend[g_XPlayer.nAnimnow] - g_nCntTimeXPlayer) * 1.0f;
			g_XPlayer.aModel[0].posDest = g_XPlayer.aModel[0].Initpos + g_XPlayer.aModel[0].posAnim[g_XPlayer.nAnimnow][g_XPlayer.nAnimkeynow];
		}
		else
		{
			frac = (g_XPlayer.nAnimfram[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow)] - g_nCntTimeXPlayer) * 1.0f;
			g_XPlayer.aModel[0].posDest = g_XPlayer.aModel[0].Initpos + g_XPlayer.aModel[0].posAnim[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow + 1) % (g_XPlayer.nAnimkey[g_XPlayer.nAnimnow])];
		}

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

		// 向き変更
		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			if (g_XPlayer.nAnimChange == 1)
			{
				g_XPlayer.aModel[nCntXPlayer].rotDest = g_XPlayer.aModel[nCntXPlayer].rotAnim[g_XPlayer.nAnimnow][g_XPlayer.nAnimkeynow];
			}
			else
			{
				g_XPlayer.aModel[nCntXPlayer].rotDest = g_XPlayer.aModel[nCntXPlayer].rotAnim[g_XPlayer.nAnimnow][(g_XPlayer.nAnimkeynow + 1) % (g_XPlayer.nAnimkey[g_XPlayer.nAnimnow])];
			}

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
				pmat[nCntMat].MatD3D.Diffuse.a = g_XPlayer.col.a;
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
	Camera *pCamera = GetCamera();
	int nCntXPlayer;

	if (g_XPlayer.bUse == false)
	{
		g_nCntTimeXPlayer = 0;

		// 設定
		g_XPlayer.pos = pos;
		g_XPlayer.Initpos = g_XPlayer.pos;
		g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XPlayer.rot = rot;
		g_XPlayer.rotDest = g_XPlayer.rot;
		g_XPlayer.Initrot = g_XPlayer.rot;

		g_XPlayer.nAnimChange = 0;
		g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
		g_XPlayer.nAnimkeynow = 0;
		g_XPlayer.nWeapon = WEAPONTYPE_NONE;
		g_XPlayer.state = PLAYERSTATE_NORMAL;

		g_XPlayer.nLife = 30;
		g_XPlayer.nInitLife = 30;

		g_XPlayer.bAttack = false;

		g_XPlayer.nIdxShadow = SetShadow(g_XPlayer.pos, g_XPlayer.rot,
			D3DXVECTOR3(20.0f, 0.0f, 20.0f));
		g_XPlayer.bUse = true;

		for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
		{
			g_XPlayer.aModel[nCntXPlayer].pos = g_XPlayer.aModel[nCntXPlayer].Initpos;
			g_XPlayer.aModel[nCntXPlayer].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_XPlayer.aModel[nCntXPlayer].rot = g_XPlayer.aModel[nCntXPlayer].rotAnim[0][0];
			g_XPlayer.aModel[nCntXPlayer].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		// カメラ調整
		if (pCamera->type == CAMERATYPE_GAME)
		{
			pCamera->posV += g_XPlayer.pos;
			pCamera->posR += g_XPlayer.pos;
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
// プレイヤーの体力設定処理
//=========================================================================================================================
void SetXPlayerHP(int nLife)
{
	g_XPlayer.nLife = nLife;
}

//=========================================================================================================================
// プレイヤーの体力取得処理
//=========================================================================================================================
int GetXPlayerHP(void)
{
	return g_XPlayer.nLife;
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
// プレイヤーのアイテム判定処理
//=========================================================================================================================
void SetHitItem(int nItem)
{
	// 武器レベル
	if (nItem == 0)
	{
		if (g_XPlayer.nWeapon_1Lv < 15)
		{
			g_XPlayer.nWeapon_1Lv += 1;
		}

		if (g_XPlayer.nWeapon_1Lv % 3 == 0)
		{
			PlaySound(SOUND_LABEL_SE_LVUP);
		}
	}
	else if (nItem == 1)
	{
		if (g_XPlayer.nWeapon_2Lv < 15)
		{
			g_XPlayer.nWeapon_2Lv += 1;
		}

		if (g_XPlayer.nWeapon_2Lv % 3 == 0)
		{
			PlaySound(SOUND_LABEL_SE_LVUP);
		}
	}
	else if (nItem == 2)
	{
		if (g_XPlayer.nWeapon_3Lv < 15)
		{
			g_XPlayer.nWeapon_3Lv += 1;
		}

		if (g_XPlayer.nWeapon_3Lv % 3 == 0)
		{
			PlaySound(SOUND_LABEL_SE_LVUP);
		}
	}
	else if (nItem == 3)
	{
		if (g_XPlayer.nLife < g_XPlayer.nInitLife)
		{
			g_XPlayer.nLife += 3;

			if (g_XPlayer.nLife > g_XPlayer.nInitLife)
			{
				g_XPlayer.nLife = g_XPlayer.nInitLife;
			}
		}
	}
}

void SetWeaponLv(int nWeapon_1, int nWeapon_2, int nWeapon_3)
{
	// 武器レベル設定
	g_XPlayer.nWeapon_1Lv = nWeapon_1;
	g_XPlayer.nWeapon_2Lv = nWeapon_2;
	g_XPlayer.nWeapon_3Lv = nWeapon_3;
}

//=========================================================================================================================
// プレイヤーの移動処理
//=========================================================================================================================
void SetMove(bool bMove)
{
	g_XPlayer.bMove = bMove;
}

//=========================================================================================================================
// プレイヤーの破棄処理
//=========================================================================================================================
void DeleteXPlayer(void)
{
	GAMESTATE game = GetGameState();

	int nCntXPlayer;

	// 初期設定
	SetXWeapon(WEAPONTYPE_NONE);
	DeleteShadow(g_XPlayer.nIdxShadow);

	g_nCntTimeXPlayer = 0;

	g_XPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_XPlayer.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_XPlayer.nAnimChange = 0;
	g_XPlayer.nAnimnow = PLAYERANIM_NEUTRAL;
	g_XPlayer.nAnimkeynow = 0;
	g_XPlayer.nIdxShadow = 0;
	g_XPlayer.nWeapon = WEAPONTYPE_NONE;
	g_XPlayer.nChargeNum = 0;
	g_XPlayer.nCntstate = 0;
	g_XPlayer.state = PLAYERSTATE_NONE;
	g_XPlayer.nLife = 0;
	g_XPlayer.nInitLife = 0;

	if (game == GAMESTATE_PLAY)
	{
		g_XPlayer.nWeapon_1Lv = 0;
		g_XPlayer.nWeapon_2Lv = 0;
		g_XPlayer.nWeapon_3Lv = 0;
	}

	g_XPlayer.bAttack = false;
	g_XPlayer.bNextAttack = false;
	g_XPlayer.nCntChargeAttack = 0;
	g_XPlayer.bChargeAttack = false;
	g_XPlayer.bChargeOn = false;
	g_XPlayer.bMove = false;
	g_XPlayer.bUse = false;

	for (nCntXPlayer = 0; nCntXPlayer < g_XPlayer.nMaxPartsNum; nCntXPlayer++)
	{
		g_XPlayer.aModel[nCntXPlayer].pos = g_XPlayer.aModel[nCntXPlayer].Initpos;
		g_XPlayer.aModel[nCntXPlayer].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_XPlayer.aModel[nCntXPlayer].rot = g_XPlayer.aModel[nCntXPlayer].rotAnim[0][0];
		g_XPlayer.aModel[nCntXPlayer].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}