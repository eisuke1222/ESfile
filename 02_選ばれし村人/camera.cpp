//=========================================================================================================================

//
// カメラ処理 [camera.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "camera.h"				// カメラ
#include "input.h"				// 入力
#include "billboardobject.h"	// オブジェクト
#include "xplayer.h"			// プレイヤー
#include "xenemy.h"				// 敵

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define CAMERA_POS_MOVE		(2.0f)								// カメラ移動量
#define CAMERA_ROT_MOVE		(0.03f)								// カメラ回転量
#define CAMERA_ROT_AUTOMOVE	(0.0015f)							// カメラ自動回転量
#define CAMERA_RESETSPEED	(0.08f)								// カメラリセット速度

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
Camera						g_Camera;							// カメラの情報
int							g_nCntCameraTimer;					// カメラタイマー
D3DXVECTOR3					g_RockOnpos;

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
	g_Camera.bReset= false;										// リセット
	g_Camera.nCntRockOn = 0;									// ロックオンカウント
	g_Camera.fRockOnRot = 0.0f;									// ロックオン向き
	g_Camera.fRockOnRotDest = 0.0f;								// 目標ロックオン向き
	g_Camera.bRockOn = false;									// ロックオンしてるか
	g_Camera.bRockOnMove = false;								// ロックオンしてるか
	g_Camera.type = CAMERATYPE_NONE;							// 種類

	g_nCntCameraTimer = 0;										// カメラタイマー
	g_RockOnpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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
	XPLAYER *pXPlayer = GetXPlayer();
	XENEMY *pXEnemy = GetXEnemy();
	int nXEnemyNum = GetXEnemyNum();

	float fDiff = 0.0f, fcof = 0.0f;

	//DIMOUSESTATE2 pMouse = GetMousePointer();
	DIJOYSTATE pStick = GetStick();

	switch (g_Camera.type)
	{
	case CAMERATYPE_NONE:
		break;
	case CAMERATYPE_TITLE:			// タイトル

		// 自動回転
		g_Camera.rot.y -= CAMERA_ROT_AUTOMOVE;
		g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
		g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

		break;
	case CAMERATYPE_TITLE_GAME:			// タイトルゲーム

		g_Camera.posR.z += 5.0f;
		g_Camera.posV.z += 5.0f;

		break;
	case CAMERATYPE_GAME:			// ゲーム
	{
		if (pXPlayer->bMove == true)
		{// プレイヤー動けるようになったら
			if ((GetKeyboardTrigger(DIK_R) == true || GetButtonTrigger(11) == true))
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
					g_Camera.bRockOn = false;
					g_Camera.bRockOnMove = false;
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
					g_Camera.bRockOnMove = false;
				}
			}

			// 視点回転
			if ((GetKeyboardPress(DIK_Q) == true || GetButtonPress(4) == true || pStick.lZ < -300))
			{
				g_Camera.rot.y -= CAMERA_ROT_MOVE;
				g_Camera.posVDest.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
				g_Camera.posVDest.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

				g_Camera.bRockOn = false;
				g_Camera.bRockOnMove = false;
			}
			if ((GetKeyboardPress(DIK_E) == true || GetButtonPress(5) == true || pStick.lZ > 300))
			{
				g_Camera.rot.y += CAMERA_ROT_MOVE;
				g_Camera.posVDest.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
				g_Camera.posVDest.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

				g_Camera.bRockOn = false;
				g_Camera.bRockOnMove = false;
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

			if (((GetKeyboardPress(DIK_C) == true || GetKeyboardPress(DIK_V) == true) ||
				(GetButtonPress(6) == true || GetButtonPress(7) == true || GetButtonPress(10) == true)) && nXEnemyNum > 0)
			{// ロックオン
				g_Camera.nCntRockOn++;

				if (g_Camera.nCntRockOn == 20)
				{// 長押し確認
					if (g_Camera.bRockOn == false)
					{
						g_RockOnpos = GetRockOnXEnemy(0);

						g_Camera.bRockOnMove = true;

						SetBillboardObject(D3DXVECTOR3(g_RockOnpos.x, g_RockOnpos.y + 100, g_RockOnpos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), BILLBOARDTEXTURE_ROCKON);
					}

					g_Camera.bRockOn = g_Camera.bRockOn ? false : true;
				}
			}
			else
			{// 押していない
				g_Camera.nCntRockOn = 0;
			}

			if (g_Camera.bRockOn == true)
			{
				if ((GetKeyboardTrigger(DIK_C) == true || GetButtonTrigger(6) == true))
				{
					ChangeRockOnXEnemy(1);
				}
				if ((GetKeyboardTrigger(DIK_V) == true || GetButtonTrigger(7) == true))
				{
					ChangeRockOnXEnemy(0);
				}

				g_RockOnpos = GetRockOnXEnemy(1);

				if (g_Camera.bRockOnMove == true)
				{// ロックオン使用している
					pXPlayer->rotDest.y = atan2f(g_RockOnpos.x - pXPlayer->pos.x, g_RockOnpos.z - pXPlayer->pos.z) + D3DX_PI;

					fDiff = (pXPlayer->rotDest.y - g_Camera.rot.y + D3DX_PI);
					g_Camera.fRockOnRot = pXPlayer->rotDest.y;
					g_Camera.fRockOnRotDest = fDiff + g_Camera.rot.y;
					// 値調整
					if (g_Camera.fRockOnRotDest > D3DX_PI)
					{
						g_Camera.fRockOnRotDest -= D3DX_PI * 2.0f;
					}
					if (g_Camera.fRockOnRotDest < -D3DX_PI)
					{
						g_Camera.fRockOnRotDest += D3DX_PI * 2.0f;
					}

					fDiff = (g_Camera.fRockOnRot - g_Camera.rot.y + D3DX_PI);
					if (fDiff > D3DX_PI)
					{
						fDiff -= D3DX_PI * 2.0f;
					}
					if (fDiff < -D3DX_PI)
					{
						fDiff += D3DX_PI * 2.0f;
					}

					g_Camera.rot.y += fDiff * 0.2f;
					if (g_Camera.rot.y > D3DX_PI)
					{
						g_Camera.rot.y -= D3DX_PI * 2.0f;
					}
					if (g_Camera.rot.y < -D3DX_PI)
					{
						g_Camera.rot.y += D3DX_PI * 2.0f;
					}

					// 視点調整

					{// 停止時
						g_Camera.posVDest.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
						g_Camera.posVDest.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;
					}

					if (g_Camera.rot.y > g_Camera.fRockOnRotDest - 0.03f && g_Camera.rot.y < g_Camera.fRockOnRotDest + 0.03f)
					{// 値を代入
						g_Camera.rot.y = g_Camera.fRockOnRotDest;

						pXPlayer->rotDest.y = atan2f(g_RockOnpos.x - pXPlayer->pos.x, g_RockOnpos.z - pXPlayer->pos.z) + D3DX_PI;
						g_Camera.rot.y = atan2f(g_RockOnpos.x - pXPlayer->pos.x, g_RockOnpos.z - pXPlayer->pos.z);

						g_Camera.bRockOnMove = false;
					}

					fcof = 1.0f;
				}
				else
				{
					pXPlayer->rotDest.y = atan2f(g_RockOnpos.x - pXPlayer->pos.x, g_RockOnpos.z - pXPlayer->pos.z) + D3DX_PI;
					g_Camera.rot.y = atan2f(g_RockOnpos.x - pXPlayer->pos.x, g_RockOnpos.z - pXPlayer->pos.z);

					// 視点調整
					g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
					g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;
				}

				SetPosionRockOn(D3DXVECTOR3(g_RockOnpos.x, g_RockOnpos.y + 100, g_RockOnpos.z));
			}
			else
			{
				DeleteBillboardObject(BILLBOARDTEXTURE_ROCKON);
			}

			// 値代入
			g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * fcof;
			g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * fcof;
			g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * fcof;
			g_Camera.posV.z += (g_Camera.posVDest.z - g_Camera.posV.z) * fcof;
		}
	}

	break;
	case CAMERATYPE_RESULT:			// リザルト
		break;
	case CAMERATYPE_RANKING:		// ランキング

		// 自動回転
		g_Camera.rot.y += CAMERA_ROT_AUTOMOVE;
		g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fDistance;
		g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fDistance;

		break;
	}

	// カメラ向き調整
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
		7500.0f
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
void SetCamera(D3DXVECTOR3 posV, D3DXVECTOR3 posR, CAMERATYPE type)
{
	g_Camera.posV = posV;										// 視点
	g_Camera.posR = posR;										// 注視点
	g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);				// ベクトル
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
	g_Camera.fDistance = g_Camera.posV.z - g_Camera.posR.z;		// 距離
	g_Camera.type = type;										// 種類

	g_nCntCameraTimer = 0;										// カメラタイマー
}

//=========================================================================================================================
// ロックオン解除処理
//=========================================================================================================================
void SetCancelRockOn(void)
{
	g_Camera.bRockOn = false;
	g_Camera.bRockOnMove = false;
}