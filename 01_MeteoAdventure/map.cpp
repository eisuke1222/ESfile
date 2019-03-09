//=========================================================================================================================
// 
// マップ処理 [map.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "map.h"		// マップ
#include "player.h"		// プレイヤー
#include "input.h"		// 入力
#include "bgwindow.h"	// 背景
#include "fade.h"		// フェード
#include "sound.h"		// サウンド

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_MAP_POLYGON			(10)									// ポリゴンの最大数
#define MAP_TEXTURE_NAME1		"data\\TEXTURE\\gamemappoint000.png"	// 読み込むテクスチャファイル1
#define MAP_WIDTH				(40)									// プレイヤー横幅
#define MAP_HEIGHT				(40)									// プレイヤー高さ
#define MAP_TEXTURE_SPEED		(10)									// テクスチャアニメーションスピード（弾速度）
#define MAP_TEXTURE_PATTERN		(5)										// テクスチャアニメーションパターン数
#define MAP_TEXTURE_UV_U		(0.2f)									// テクスチャアニメーションU範囲
#define MAP_TEXTURE_UV_V		(0.5f)									// テクスチャアニメーションV範囲

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureMap = NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffMap = NULL;					// 頂点バッファへのポインタ
MAP								g_Map[MAX_MAP_POLYGON];					// マップ情報
int								g_nCntSetMap;							// マップ総数
int								g_nSelectMap;							// マップ選択

//=========================================================================================================================
// マップ初期化処理
//=========================================================================================================================
void InitMap(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntMap;		// マップカウンタ

	// マップ情報初期化
	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{// マップをカウント
		g_Map[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Map[nCntMap].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_Map[nCntMap].nCntAnim = 0;
		g_Map[nCntMap].nNumAnim = 0;
		g_Map[nCntMap].bSelect = false;
		g_Map[nCntMap].bUse = false;
	}
	g_nCntSetMap = 0;
	g_nSelectMap = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MAP_TEXTURE_NAME1, &g_pTextureMap);	// プレイヤー

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_MAP_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffMap->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{// マップをカウント
		// 頂点設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		// 頂点カラー設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(MAP_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, MAP_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(MAP_TEXTURE_UV_U, MAP_TEXTURE_UV_V);

		pVtx += 4;	// 頂点データのポインタを４つ分進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMap->Unlock();
}

//=========================================================================================================================
// マップ終了処理
//=========================================================================================================================
void UninitMap(void)
{
	// テクスチャの破棄
	if (g_pTextureMap != NULL)
	{
		g_pTextureMap->Release();
		g_pTextureMap = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffMap != NULL)
	{
		g_pVtxBuffMap->Release();
		g_pVtxBuffMap = NULL;
	}
}

//=========================================================================================================================
// マップ更新処理
//=========================================================================================================================
void UpdateMap(void)
{
	//変数宣言
	int nCntMap;
	PLAYER *pPlayer = GetPlayer();		// プレイヤー情報
	FADE fade = GetFade();

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffMap->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if (fade != FADE_OUT)
	{
		if (GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_LEFT) == true || GetKeyboardTrigger(DIK_W) == true || GetKeyboardTrigger(DIK_A) == true)
		{// 任意のキー（上、左、W、D）を押した場合
			PlaySound(SOUND_LABEL_SE_SELECT002);

			g_nSelectMap = (g_nSelectMap + (g_nCntSetMap - 1)) % g_nCntSetMap;	// 選択を進める

			pPlayer->move.x = sinf(atan2f(-pPlayer->pos.x - -g_Map[g_nSelectMap].pos.x, -pPlayer->pos.y - -g_Map[g_nSelectMap].pos.y)) * 8.0f;
			pPlayer->move.y = cosf(atan2f(-pPlayer->pos.x - -g_Map[g_nSelectMap].pos.x, -pPlayer->pos.y - -g_Map[g_nSelectMap].pos.y)) * 8.0f;

			if (pPlayer->pos.x < g_Map[g_nSelectMap].pos.x)
			{
				pPlayer->nDinectionMove = 0;
			}
			else
			{
				pPlayer->nDinectionMove = 1;
			}
		}
		if (GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_RIGHT) == true || GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_D) == true)
		{// 任意のキー（下、右、S、A）を押した場合
			PlaySound(SOUND_LABEL_SE_SELECT002);

			g_nSelectMap = (g_nSelectMap + 1) % g_nCntSetMap;	// 選択を進める

			pPlayer->move.x = sinf(atan2f(-pPlayer->pos.x - -g_Map[g_nSelectMap].pos.x, -pPlayer->pos.y - -g_Map[g_nSelectMap].pos.y)) * 8.0f;
			pPlayer->move.y = cosf(atan2f(-pPlayer->pos.x - -g_Map[g_nSelectMap].pos.x, -pPlayer->pos.y - -g_Map[g_nSelectMap].pos.y)) * 8.0f;

			if (pPlayer->pos.x < g_Map[g_nSelectMap].pos.x)
			{
				pPlayer->nDinectionMove = 0;
			}
			else
			{
				pPlayer->nDinectionMove = 1;
			}
		}
	}

	if (pPlayer->pos.x >= g_Map[g_nSelectMap].pos.x - 5 && pPlayer->pos.x <= g_Map[g_nSelectMap].pos.x + 5 &&
		pPlayer->pos.y >= g_Map[g_nSelectMap].pos.y - 5 && pPlayer->pos.y <= g_Map[g_nSelectMap].pos.y + 5)
	{
		pPlayer->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{// マップをカウンタ
		if (g_Map[nCntMap].bUse == true)
		{// 使用している場合
			if (nCntMap == g_nSelectMap)
			{// 選択マップ
				g_Map[nCntMap].nCntAnim++;		// カウンタを進める
				if (g_Map[nCntMap].nCntAnim % MAP_TEXTURE_SPEED == 0)
				{// アニメーション
					g_Map[nCntMap].nNumAnim = (g_Map[nCntMap].nNumAnim + 1) % MAP_TEXTURE_PATTERN;
				}

				// テクスチャ設定
				pVtx[nCntMap * 4].tex = D3DXVECTOR2(((g_Map[nCntMap].nNumAnim % 5)* MAP_TEXTURE_UV_U), (MAP_TEXTURE_UV_V));
				pVtx[nCntMap * 4 + 1].tex = D3DXVECTOR2(((g_Map[nCntMap].nNumAnim % 5) * MAP_TEXTURE_UV_U) + MAP_TEXTURE_UV_U, (MAP_TEXTURE_UV_V));
				pVtx[nCntMap * 4 + 2].tex = D3DXVECTOR2(((g_Map[nCntMap].nNumAnim % 5) * MAP_TEXTURE_UV_U), (MAP_TEXTURE_UV_V + MAP_TEXTURE_UV_V));
				pVtx[nCntMap * 4 + 3].tex = D3DXVECTOR2(((g_Map[nCntMap].nNumAnim % 5) * MAP_TEXTURE_UV_U) + MAP_TEXTURE_UV_U, (MAP_TEXTURE_UV_V + MAP_TEXTURE_UV_V));
			}
			else
			{// それ以外
				g_Map[nCntMap].nNumAnim = 4;		// アニメーション設定

				// テクスチャ設定
				pVtx[nCntMap * 4].tex = D3DXVECTOR2((g_Map[nCntMap].nNumAnim* MAP_TEXTURE_UV_U), 0.0f);
				pVtx[nCntMap * 4 + 1].tex = D3DXVECTOR2((g_Map[nCntMap].nNumAnim * MAP_TEXTURE_UV_U) + MAP_TEXTURE_UV_U, 0.0f);
				pVtx[nCntMap * 4 + 2].tex = D3DXVECTOR2((g_Map[nCntMap].nNumAnim * MAP_TEXTURE_UV_U), MAP_TEXTURE_UV_V);
				pVtx[nCntMap * 4 + 3].tex = D3DXVECTOR2((g_Map[nCntMap].nNumAnim * MAP_TEXTURE_UV_U) + MAP_TEXTURE_UV_U, MAP_TEXTURE_UV_V);
			}
		}
	}
	// 頂点バッファをアンロック
	g_pVtxBuffMap->Unlock();

	if (fade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{// 任意のキー（ENTER）を押した場合
			PlaySound(SOUND_LABEL_SE_DECIDE);
			SetNumStage(g_nSelectMap);				// ゲームステージ設定
			SetFadeGame(GAMESTATE_GAMEPLAY);		// ゲームプレイへ
		}
	}
}

//=========================================================================================================================
// マップ描画処理
//=========================================================================================================================
void DrawMap(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntMap;		// マップカウンタ

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffMap,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, g_pTextureMap);

	// マップの描画
	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{
		if (g_Map[nCntMap].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMap * 4, 2);
		}
	}
}

//=========================================================================================================================
// マップ設定処理
//=========================================================================================================================
void SetMap(D3DXVECTOR3 pos)
{
	// 変数宣言
	int nCntMap;
	PLAYER *pPlayer = GetPlayer();
	bool *aClearStage = GetStageClearState();

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffMap->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{
		if (g_Map[nCntMap].bUse == false)
		{
			g_Map[nCntMap].pos = pos;
			g_Map[nCntMap].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_Map[nCntMap].nCntAnim = 0;
			g_Map[nCntMap].nNumAnim = 0;
			g_Map[nCntMap].bSelect = false;
			g_Map[nCntMap].bUse = true;
			
			g_nCntSetMap++;

			if (aClearStage[nCntMap] == true)
			{
				SetBGWindow(D3DXVECTOR3(g_Map[nCntMap].pos.x - 30, g_Map[nCntMap].pos.y - 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50.0f, 50.0f, BGWINDOWTEXTURE_GAME_STAGEGOAL_SMALL);
			}

			// 頂点設定
			pVtx[nCntMap * 4].pos = D3DXVECTOR3(g_Map[nCntMap].pos.x - MAP_WIDTH, g_Map[nCntMap].pos.y - MAP_HEIGHT, 0.0f);
			pVtx[nCntMap * 4 + 1].pos = D3DXVECTOR3(g_Map[nCntMap].pos.x + MAP_WIDTH, g_Map[nCntMap].pos.y - MAP_HEIGHT, 0.0f);
			pVtx[nCntMap * 4 + 2].pos = D3DXVECTOR3(g_Map[nCntMap].pos.x - MAP_WIDTH, g_Map[nCntMap].pos.y + MAP_HEIGHT, 0.0f);
			pVtx[nCntMap * 4 + 3].pos = D3DXVECTOR3(g_Map[nCntMap].pos.x + MAP_WIDTH, g_Map[nCntMap].pos.y + MAP_HEIGHT, 0.0f);

			break;
		}
	}

	if (g_Map[g_nSelectMap].bUse == true && pPlayer->bDisp == false)
	{
		// プレイヤー設定
		SetPlayer(g_Map[g_nSelectMap].pos);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffMap->Unlock();
}

//=========================================================================================================================
// マップ破棄処理
//=========================================================================================================================
int  GetSelectMap(void)
{
	return g_nSelectMap;
}

//=========================================================================================================================
// マップ破棄処理
//=========================================================================================================================
void DeleteMap(void)
{
	// 変数宣言
	int nCntMap;

	for (nCntMap = 0; nCntMap < MAX_MAP_POLYGON; nCntMap++)
	{
		if (g_Map[nCntMap].bUse == true)
		{
			g_Map[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Map[nCntMap].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			g_Map[nCntMap].nCntAnim = 0;
			g_Map[nCntMap].nNumAnim = 0;
			g_Map[nCntMap].bSelect = false;
			g_Map[nCntMap].bUse = false;
		}
	}
	g_nCntSetMap = 0;
}