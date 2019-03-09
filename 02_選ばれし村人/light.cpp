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

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_LIGHT		(4)						// ライトの最大数

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
D3DLIGHT9 g_Light[MAX_LIGHT];

D3DXCOLOR g_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0);
//=========================================================================================================================
// ライトの初期化処理
//=========================================================================================================================
void InitLight(void)
{
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
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(0.55f, -0.8f, 0.55f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 1)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(0.55f, -0.8f, -0.55f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 2)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.55f, -0.8f, 0.55f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// 正規化する
			g_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 3)
		{
			// ライトの種類を設定
			g_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ライトの拡散光を設定
			g_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向の設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.55f, -0.8f, -0.55f);
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
	
}

