//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "camera.h"
#include "input.h"
#include "game.h"
#include "xplayer.h"
#include "xenemy.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define CAMERA_POS_MOVE		(2.0f)								// カメラ移動量
#define CAMERA_ROT_MOVE		(0.02f)								// カメラ回転量
#define CAMERA_ROT_AUTOMOVE	(0.0015f)							// カメラ自動回転量
#define CAMERA_RESETSPEED	(0.08f)								// カメラリセット速度

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
Camera						g_Camera;			// カメラの情報
float						g_fCameraDiff;
int							g_nCntCameraTimer;
float						g_fPlayerrotold;

//=========================================================================================================================
// カメラの初期化処理
//=========================================================================================================================
void InitCamera(void)
{
	// 初期値設定
	g_Camera.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 視点
	g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;				// 注視点
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ベクトル
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	g_Camera.fDistance = g_Camera.posV.z - g_Camera.posR.z;		// 距離
	g_Camera.fResetRot = 0.0f;									// リセット向き
	g_Camera.fResetRotDest = 0.0f;								// 目標リセット向き
	g_Camera.bReset = false;									// リセット
	g_Camera.type = CAMERATYPE_NONE;							// 種類

	g_fCameraDiff = 0.0f;
	g_nCntCameraTimer = 0;
	g_fPlayerrotold = 0.0f;
}

//=========================================================================================================================
// カメラの終了処理
//=========================================================================================================================
void UninitCamera(void)
{

}

//=========================================================================================================================
// カメラの更新処理
//=========================================================================================================================
void UpdateCamera(void)
{
	QUESTSTATE pQuestState = GetQuestState();
	XPLAYER *pXPlayer = GetXPlayer();
	float fDiff = 0.0f, fcof = 0.0f;
/*
	XPLAYER *pXPlayer = GetXPlayer();
	float fDiff = 0.0f, fcof = 0.0f;

	DIMOUSESTATE2 pMouse = GetMousePointer();

	//g_Camera.posR.x = g_Camera.posR.x + pMouse.lX;
	//g_Camera.posR.y = g_Camera.posR.y - pMouse.lY;
	
	// 
	if (GetKeyboardPress(DIK_R) == true)
	{
		if (g_Camera.rot.y > D3DX_PI)
		{
			g_Camera.rot.y -= D3DX_PI * 2.0f;
		}
		if (g_Camera.rot.y < -D3DX_PI)
		{
			g_Camera.rot.y += D3DX_PI * 2.0f;
		}

		fDiff = (pXPlayer->rot.y - g_Camera.rot.y + D3DX_PI);

		if (fDiff > D3DX_PI)
		{
			fDiff -= D3DX_PI * 2.0f;
		}
		if (fDiff < -D3DX_PI)
		{
			fDiff += D3DX_PI * 2.0f;
		}

		g_Camera.rot.y += fDiff * 0.1f;

		if (g_Camera.rot.y > D3DX_PI)
		{
			g_Camera.rot.y -= D3DX_PI * 2.0f;
		}
		if (g_Camera.rot.y < -D3DX_PI)
		{
			g_Camera.rot.y += D3DX_PI * 2.0f;
		}
	}
	
	// カメラ追従
	if (pXPlayer->move.x > 0.7f || pXPlayer->move.x < -0.7f || pXPlayer->move.z > 0.7f || pXPlayer->move.z < -0.7f)
	{// 移動時
		g_Camera.posRDest.x = pXPlayer->pos.x - sinf(pXPlayer->rot.y) * 40.0f;
		g_Camera.posRDest.y = pXPlayer->pos.y;
		g_Camera.posRDest.z = pXPlayer->pos.z - cosf(pXPlayer->rot.y) * 40.0f;
		g_Camera.posVDest.x = pXPlayer->pos.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
		g_Camera.posVDest.y = pXPlayer->pos.y;
		g_Camera.posVDest.z = pXPlayer->pos.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

		if (g_fPlayerrotold != pXPlayer->rot.y)
		{
			g_nCntCameraTimer = 0;
		}

		if (g_nCntCameraTimer < 30)
		{
			g_nCntCameraTimer++;
		}
		
		fcof = 0.1f + (float)(g_nCntCameraTimer * 0.001f);
	}
	else
	{// 停止時
		g_Camera.posRDest.x = pXPlayer->pos.x - sinf(pXPlayer->rot.y);
		g_Camera.posRDest.y = pXPlayer->pos.y;
		g_Camera.posRDest.z = pXPlayer->pos.z - cosf(pXPlayer->rot.y);
		g_Camera.posVDest.x = pXPlayer->pos.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
		g_Camera.posVDest.y = pXPlayer->pos.y;
		g_Camera.posVDest.z = pXPlayer->pos.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

		g_nCntCameraTimer = 0;

		fcof = 0.03f;
	}
	
	g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * fcof;
	g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * fcof;
	g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * fcof;
	g_Camera.posV.z += (g_Camera.posVDest.z - g_Camera.posV.z) * fcof;

	//g_fPlayerrotold = pXPlayer->rot.y;

	//SetBillboardNormalEffect(g_Camera.posR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(2.0f, 2.0f, 0.0f));
*/

	DIMOUSESTATE2 pMouse = GetMousePointer();
	DIJOYSTATE pStick = GetStick();

	switch (g_Camera.type)
	{
	case CAMERATYPE_NONE:
			break;
	case CAMERATYPE_TITLE:

		g_Camera.rot.y -= CAMERA_ROT_AUTOMOVE;
		g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
		g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

		break;
	case CAMERATYPE_GAME_SEARCH:
	{
		if (pQuestState != QUESTSTATE_MISSTION && pQuestState != QUESTSTATE_REPORT && pQuestState != QUESTSTATE_CLEAR)
		{
			if ((GetKeyboardTrigger(DIK_R) == true || GetButtonTrigger(6) == true))
			{// カメラリセット
				if (g_Camera.bReset == false)
				{// 使用していない場合
					fDiff = (pXPlayer->rot.y - g_Camera.rot.y + D3DX_PI);
					g_Camera.fResetRot = pXPlayer->rot.y;
					g_Camera.fResetRotDest = fDiff + g_Camera.rot.y;
					// 値調整
					if (g_Camera.fResetRotDest > D3DX_PI)
					{
						g_Camera.fResetRotDest -= D3DX_PI * 2.0f;
					}
					if (g_Camera.fResetRotDest < -D3DX_PI)
					{
						g_Camera.fResetRotDest += D3DX_PI * 2.0f;
					}

					g_Camera.bReset = true;
				}
			}

			if (g_Camera.bReset == true)
			{// 使用している場合
				fDiff = (g_Camera.fResetRot - g_Camera.rot.y + D3DX_PI);
				if (fDiff > D3DX_PI)
				{
					fDiff -= D3DX_PI * 2.0f;
				}
				if (fDiff < -D3DX_PI)
				{
					fDiff += D3DX_PI * 2.0f;
				}

				g_Camera.rot.y += fDiff * CAMERA_RESETSPEED;
				if (g_Camera.rot.y > D3DX_PI)
				{
					g_Camera.rot.y -= D3DX_PI * 2.0f;
				}
				if (g_Camera.rot.y < -D3DX_PI)
				{
					g_Camera.rot.y += D3DX_PI * 2.0f;
				}

				// 視点調整
				if (pXPlayer->move.x > 0.7f || pXPlayer->move.x < -0.7f || pXPlayer->move.z > 0.7f || pXPlayer->move.z < -0.7f)
				{// 移動時
					g_Camera.posVDest.x = pXPlayer->pos.x + g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
					g_Camera.posVDest.z = pXPlayer->pos.z + g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;
				}
				else
				{// 停止時
					g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
					g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;
				}

				if (g_Camera.rot.y > g_Camera.fResetRotDest - 0.05f && g_Camera.rot.y < g_Camera.fResetRotDest + 0.05f)
				{// 値を代入
					g_Camera.rot.y = g_Camera.fResetRotDest;
					g_Camera.bReset = false;
				}
			}

			// 視点回転
			if ((GetKeyboardPress(DIK_Q) == true || GetButtonPress(4) == true || pStick.lZ < -100))
			{
				g_Camera.rot.y -= CAMERA_ROT_MOVE;
				g_Camera.posVDest.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
				g_Camera.posVDest.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;
			}
			if ((GetKeyboardPress(DIK_E) == true || GetButtonPress(5) == true || pStick.lZ > 100))
			{
				g_Camera.rot.y += CAMERA_ROT_MOVE;
				g_Camera.posVDest.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
				g_Camera.posVDest.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;
			}

			// カメラ追従
			if (pXPlayer->move.x > 0.7f || pXPlayer->move.x < -0.7f || pXPlayer->move.z > 0.7f || pXPlayer->move.z < -0.7f)
			{// 移動時
				g_Camera.posRDest.x = pXPlayer->pos.x - sinf(pXPlayer->rot.y) * 40.0f;
				g_Camera.posRDest.y = pXPlayer->pos.y;
				g_Camera.posRDest.z = pXPlayer->pos.z - cosf(pXPlayer->rot.y) * 40.0f;
				g_Camera.posVDest.x = pXPlayer->pos.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
				g_Camera.posVDest.y = pXPlayer->pos.y;
				g_Camera.posVDest.z = pXPlayer->pos.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

				if (g_nCntCameraTimer < 30)
				{
					g_nCntCameraTimer++;
				}

				fcof = 0.1f + (float)(g_nCntCameraTimer * 0.001f);
			}
			else
			{// 停止時
				g_Camera.posRDest.x = pXPlayer->pos.x - sinf(pXPlayer->rot.y);
				g_Camera.posRDest.y = pXPlayer->pos.y;
				g_Camera.posRDest.z = pXPlayer->pos.z - cosf(pXPlayer->rot.y);
				g_Camera.posVDest.x = pXPlayer->pos.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
				g_Camera.posVDest.y = pXPlayer->pos.y;
				g_Camera.posVDest.z = pXPlayer->pos.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

				g_nCntCameraTimer = 0;

				fcof = 0.03f;
			}

			// 値代入
			g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * fcof;
			g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * fcof;
			g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * fcof;
			g_Camera.posV.z += (g_Camera.posVDest.z - g_Camera.posV.z) * fcof;
		}

#if 0
		// カメラ移動
		if (GetKeyboardPress(DIK_W) == true)
		{// 前a
			g_Camera.posV.x += sinf(D3DX_PI * 0.0f + g_Camera.rot.y) * CAMERA_POS_MOVE * 10;
			g_Camera.posV.z += cosf(D3DX_PI * 0.0f + g_Camera.rot.y) * CAMERA_POS_MOVE * 10;
			g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fDistance;
			g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fDistance;
		}
		if (GetKeyboardPress(DIK_S) == true)
		{// 後ろ
			g_Camera.posV.x -= sinf(D3DX_PI * 0.0f + g_Camera.rot.y) * CAMERA_POS_MOVE;
			g_Camera.posV.z -= cosf(D3DX_PI * 0.0f + g_Camera.rot.y) * CAMERA_POS_MOVE;
			g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fDistance;
			g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fDistance;
		}
		if (GetKeyboardPress(DIK_A) == true)
		{// 左
			g_Camera.posV.x -= sinf(D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_POS_MOVE;
			g_Camera.posV.z -= cosf(D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_POS_MOVE;
			g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fDistance;
			g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fDistance;
		}
		if (GetKeyboardPress(DIK_D) == true)
		{// 右
			g_Camera.posV.x += sinf(D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_POS_MOVE;
			g_Camera.posV.z += cosf(D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_POS_MOVE;
			g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fDistance;
			g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fDistance;
		}
#endif

/*		// 注視点位置変更
		if (pMouse.lY < 0)
		{// Y座標
			g_Camera.posR.y -= cosf(D3DX_PI * 0.0f) * (pMouse.lY / 1.0f);
		}
		if (pMouse.lY > 0)
		{// Y座標
			g_Camera.posR.y -= cosf(D3DX_PI * 0.0f) * (pMouse.lY / 1.0f);
		}
		if (pMouse.lX < 0)
		{// 注視点
			g_Camera.rot.y += (pMouse.lX / 200.0f);
			g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fDistance;
			g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fDistance;
		}
		if (pMouse.lX > 0)
		{// 注視点
			g_Camera.rot.y += (pMouse.lX / 200.0f);
			g_Camera.posR.x = g_Camera.posV.x - sinf(g_Camera.rot.y) * g_Camera.fDistance;
			g_Camera.posR.z = g_Camera.posV.z - cosf(g_Camera.rot.y) * g_Camera.fDistance;
		}

		if (pMouse.lZ > 0)
		{// Y座標:;:
			g_Camera.posV.y += (pMouse.lZ / 1.0f);
		}
		if (pMouse.lZ < 0)
		{// Y座標
			g_Camera.posV.y += (pMouse.lZ / 1.0f);
		}*/
	}
		break;
	case CAMERATYPE_RESULT:
		break;
	case CAMERATYPE_RANKING:
		
		g_Camera.rot.y += CAMERA_ROT_AUTOMOVE;
		g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
		g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

		break;
	}

	if (g_Camera.rot.y > D3DX_PI)
	{
		g_Camera.rot.y -= D3DX_PI * 2.0f;
	}
	if (g_Camera.rot.y < -D3DX_PI)
	{
		g_Camera.rot.y += D3DX_PI * 2.0f;
	}
}

//=========================================================================================================================
// カメラの設定処理
//=========================================================================================================================
void SetUpCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH
	(
		&g_Camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		15000.0f
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH
	(
		&g_Camera.mtxView,
		&g_Camera.posV,
		&g_Camera.posR,
		&g_Camera.vecU
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

//=========================================================================================================================
// カメラの取得処理
//=========================================================================================================================
Camera *GetCamera(void)
{
	return &g_Camera;
}

//=========================================================================================================================
// カメラの設定処理
//=========================================================================================================================
void SetCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot, float fDistance, CAMERATYPE type)
{
	g_Camera.posV = posV;										// 視点
	g_Camera.posVDest = posV;									// 視点
	g_Camera.posR = posR;										// 注視点
	g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 注視点
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ベクトル
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);											// 向き
	g_Camera.fDistance = fDistance;								// 距離
	g_Camera.type = type;										// 種類

	g_fCameraDiff = 0.0f;
	g_nCntCameraTimer = 0;
	g_fPlayerrotold = 0.0f;
}