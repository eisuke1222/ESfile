//=========================================================================================================================
//
// ライト処理 [light.cpp]
// Author : 
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "light.h"
#include "input.h"
#include "game.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_LIGHT		(4)						// ライトの最大数

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
D3DLIGHT9 g_Light[MAX_LIGHT];

int		  g_nCntColTimer;
D3DXCOLOR g_color;

//=========================================================================================================================
// ライトの初期化処理
//=========================================================================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir[MAX_LIGHT];				// 設定用方向ベクトル
	int nCntLight;

	g_nCntColTimer = 0;
	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

	for (nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&g_Light[nCntLight], sizeof(D3DLIGHT9));

		if (nCntLight == 0)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(0.5f, -0.8f, 0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 1)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(0.5f, -0.8f, -0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 2)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.5f, -0.8f, 0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 3)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.5f, -0.8f, -0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}

		// ライトを設定する
		pDevice->SetLight(nCntLight, &g_Light[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=========================================================================================================================
// ライトの終了処理
//=========================================================================================================================
void UninitLight(void)
{

}

//=========================================================================================================================
// ライトの更新処理
//=========================================================================================================================
void UpdateLight(void)
{
	QUESTSTATE pQuestState = GetQuestState();

	if (pQuestState == QUESTSTATE_SEARCH)
	{
		g_nCntColTimer++;

		if (g_nCntColTimer > 10000)
		{
			if (g_color.r > 0.98f)
			{
				g_color.r -= (1.0f - 0.98f) / 5000.0f;
			}
			if (g_color.g > 0.68f)
			{
				g_color.g -= (1.0f - 0.58f) / 5000.0f;
			}
			if (g_color.b > 0.53f)
			{
				g_color.b -= (1.0f - 0.43f) / 5000.0f;
			}
		}
	}

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir[MAX_LIGHT];				// 設定用方向ベクトル
	int nCntLight;

	for (nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&g_Light[nCntLight], sizeof(D3DLIGHT9));

		if (nCntLight == 0)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = g_color;

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(0.5f, -0.8f, 0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 1)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = g_color;

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(0.5f, -0.8f, -0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 2)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = g_color;

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.5f, -0.8f, 0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 3)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = g_color;

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.5f, -0.8f, -0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}

		// ライトを設定する
		pDevice->SetLight(nCntLight, &g_Light[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

void SetLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir[MAX_LIGHT];				// 設定用方向ベクトル
	int nCntLight;

	g_nCntColTimer = 0;
	g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

	for (nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&g_Light[nCntLight], sizeof(D3DLIGHT9));

		if (nCntLight == 0)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(0.5f, -0.8f, 0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 1)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(0.5f, -0.8f, -0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 2)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.5f, -0.8f, 0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 3)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.5f, -0.8f, -0.5f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}

		// ライトを設定する
		pDevice->SetLight(nCntLight, &g_Light[nCntLight]);

		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}