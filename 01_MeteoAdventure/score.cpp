//=========================================================================================================================
// 
// スコア処理 [score.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanfエラー解除
#include <stdio.h>					// stdio
#include "score.h"					// スコア
#include "input.h"					// キーボード
#include "item.h"					// アイテム
#include "player.h"					// プレイヤー
#include "map.h"					// マップ
#include "fade.h"					// フェード
#include "bgwindow.h"				// 背景

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_SCORE_SET		(10)							// スコアの最大数
#define MAX_SCORE_NUMBER	(6)								// スコアの最大桁数
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define	SCORE_FILE			"data/SAVE/scorernking.txt"		// 読み込むスコアテキストファイル名
#define SCORE_SPEED			(15.0f)							// スコアの移動速度

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureScore = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffScore = NULL;			// 頂点バッファへのポインタ
Score						g_Score[MAX_SCORE_SET];			// スコア情報
int							g_HiScore[GAMESTAGE_MAX];		// ハイスコア
int							g_nCntScoreTime;				// 時間カウンタ

//=========================================================================================================================
// スコア初期化処理
//=========================================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntSet;				// セットカウンタ
	float nCntScore;			// スコアカウンタ
	float nScorePos;			// スコア位置

	// スコアの初期化
	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// セットをカウント
		g_Score[nCntSet].nScore = 0;								// スコアを初期化 
		g_Score[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// スコア中心値
		g_Score[nCntSet].posmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// スコア間隔値
		g_Score[nCntSet].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// スコア間隔値
		g_Score[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// スコア色
		g_Score[nCntSet].nSetNum = 0;								// 桁数
		g_Score[nCntSet].type = SCORETYPE_SCORE;					// 種類
		g_Score[nCntSet].bUse = false;								// 使用しているかどうか
	}
	g_nCntScoreTime = 0;											// 初期化

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SCORE, &g_pTextureScore);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * (MAX_SCORE_NUMBER * MAX_SCORE_SET),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffScore->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// セットをカウント
		for (nCntScore = 0, nScorePos = 1200; nCntScore < MAX_SCORE_NUMBER; nCntScore++, nScorePos -= 30)
		{// 桁数とカウント
			// 頂点設定
			pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;	// 頂点データのポインタを４つ分進める
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

//=========================================================================================================================
// スコア終了処理
//=========================================================================================================================
void UninitScore(void)
{
	// テクスチャーの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=========================================================================================================================
// スコア更新処理
//=========================================================================================================================
void UpdateScore(void)
{
	// 変数宣言
	int nCntSet;		// セットカウンタ
	int nCntScore;		// スコアカウンタ
	int nScoreData;		// スコア格納
	int nSelectMap = GetSelectMap();	// ゲームステージ情報
	PLAYER *pPlayer = GetPlayer();
	bool bGoalHit = GetGoalHit();

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffScore->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// セットをカウント
		if (g_Score[nCntSet].bUse == true)
		{// スコアが使用されている場合
			if (g_Score[nCntSet].type == SCORETYPE_HISCORE)
			{
				if (g_Score[nCntSet].nScore != g_HiScore[nSelectMap])
				{
					g_Score[nCntSet].nScore = g_HiScore[nSelectMap];
				}
			}
			else if (g_Score[nCntSet].type == SCORETYPE_TIME)
			{// 時間の場合
				g_nCntScoreTime++;					// カウンタを進める
				if (g_nCntScoreTime % 60 == 0 && g_Score[nCntSet].nScore > 0)
				{// 毎秒、コインが1枚以上ある場合
					if (g_Score[nCntSet].nScore > 0 && bGoalHit == false)
					{// スコア（時間）1以上
						g_Score[nCntSet].nScore -= 1;		// スコア（時間）を減算
					
						if (g_Score[nCntSet].nScore == 30)
						{
							g_Score[nCntSet].col = D3DXCOLOR(1.0f, 0.5f, 0.3f, 1.0f);
						}
					}
					
					if (g_Score[nCntSet].nScore <= 0)
					{// スコア（時間）が0以下
						pPlayer->nNumPlayer--;

						if (pPlayer->nNumPlayer <= 0)
						{
							SetGameState(GAMESTATE_GAMEOVER);	// ゲームオーバー
						}
						else
						{
							SetFadeGame(GAMESTATE_MAP);			// ゲームマップ
						}
					}
				}
			}

			nScoreData = g_Score[nCntSet].nScore;	// スコアを格納

			for (nCntScore = 0; nCntScore < MAX_SCORE_NUMBER; nCntScore++)
			{// スコアカウント
				// テクスチャ設定
				pVtx[nCntScore * 4].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f, 0.0f);
				pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f + 0.1f, 0.0f);
				pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f, 1.0f);
				pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f + 0.1f, 1.0f);
				// 頂点カラー設定
				pVtx[nCntScore * 4].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 1].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 2].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 3].col = g_Score[nCntSet].col;

				nScoreData /= 10;		// 桁を進める
			}
		}
		pVtx += (MAX_SCORE_NUMBER * 4);		// 頂点データのポインタ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

//=========================================================================================================================
// スコア描画処理
//=========================================================================================================================
void DrawScore(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntSet;		// セットカウンタ
	int nCntScore;		// スコアカウンタ

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffScore,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, g_pTextureScore);

	// スコアの描画
	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// セットをカウント
		if (g_Score[nCntSet].bUse == true)
		{// 使用している場合
			for (nCntScore = 0; nCntScore < g_Score[nCntSet].nSetNum; nCntScore++)
			{// スコアカウント
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4 + nCntSet * (MAX_SCORE_NUMBER * 4), 2);
			}
		}
	}
}

//=========================================================================================================================
// スコア設定処理
//=========================================================================================================================
void SetScore(D3DXVECTOR3 pos, D3DXVECTOR3 fRadius, D3DXCOLOR col, SCORETYPE type)
{
	// 変数宣言
	int nCntSet;		// セットカウンタ
	int nCntScore;		// スコアカウンタ
	D3DXVECTOR3 nPos;	// 位置を格納

	PLAYER *pPlayer = GetPlayer();		// プレイヤー情報
	int nNumCoin = GetNumCoin();		// コイン総数

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffScore->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// セットをカウント
		if (g_Score[nCntSet].bUse == false)
		{// 使用されている場合
			g_Score[nCntSet].pos = pos;							// 位置
			g_Score[nCntSet].posmove = g_Score[nCntSet].pos;	// 移動値
			nPos = g_Score[nCntSet].pos;						// 位置を格納
			g_Score[nCntSet].fRadius = fRadius;					// 大きさ
			g_Score[nCntSet].col = col;							// 色
			g_Score[nCntSet].type = type;						// スコアタイプ
			g_Score[nCntSet].bUse = true;						// 使用している状態

			switch (g_Score[nCntSet].type)
			{// スコア種類
			case SCORETYPE_SCORE:		// スコア
				g_Score[nCntSet].nScore = 0;					// スコア初期値
				g_Score[nCntSet].nSetNum = MAX_SCORE_NUMBER;	// 桁数
				break;
			case SCORETYPE_HISCORE:		// ハイスコア
				g_Score[nCntSet].nScore = 0;					// スコア初期値
				g_Score[nCntSet].nSetNum = MAX_SCORE_NUMBER;	// 桁数
				break;
			case SCORETYPE_COIN:		// コイン
				g_Score[nCntSet].nScore = nNumCoin;				// スコア初期値
				g_Score[nCntSet].nSetNum = 2;					// 桁数
				break;
			case SCORETYPE_TIME:		// 時間
				g_Score[nCntSet].nScore = 99;					// スコア初期値
				g_Score[nCntSet].nSetNum = 2;					// 桁数
				break;
			case SCORETYPE_PLAYER:		// プレイヤー
				g_Score[nCntSet].nScore = pPlayer->nNumPlayer;	// スコア初期値
				g_Score[nCntSet].nSetNum = 2;					// 桁数
				break;
			}
			
			for (nCntScore = 0; nCntScore < g_Score[nCntSet].nSetNum; nCntScore++)
			{// スコアをカウント
				// 頂点設定
				pVtx[nCntScore * 4].pos = D3DXVECTOR3(nPos.x, nPos.y - g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 1].pos = D3DXVECTOR3(nPos.x + g_Score[nCntSet].fRadius.x, nPos.y - g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 2].pos = D3DXVECTOR3(nPos.x, nPos.y + g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 3].pos = D3DXVECTOR3(nPos.x + g_Score[nCntSet].fRadius.x, nPos.y + g_Score[nCntSet].fRadius.y, 0.0f);
				// 頂点カラー設定
				pVtx[nCntScore * 4].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 1].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 2].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 3].col = g_Score[nCntSet].col;

				nPos.x -= g_Score[nCntSet].fRadius.x;	//位置をずらす
			}

			break;
		}
		pVtx += (MAX_SCORE_NUMBER * 4); 	// 頂点データのポインタ進める
	}

	// 頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

//=========================================================================================================================
// スコアの取得処理
//=========================================================================================================================
Score GetScore(void)
{
	return g_Score[0];		// 値を返す
}

//=========================================================================================================================
// スコアの加算処理
//=========================================================================================================================
void AddScore(SCORETYPE Type, int nValue)
{
	// 変数宣言
	int nCntSet;

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// セットをカウント
		if (g_Score[nCntSet].bUse == true && g_Score[nCntSet].type == Type)
		{// 使用している場合
			if (g_Score[nCntSet].type == SCORETYPE_HISCORE)
			{
				g_Score[nCntSet].nScore = nValue;	// スコアを代入
			}
			else
			{
				g_Score[nCntSet].nScore += nValue;	// スコアを代入
			}
		}
	}
}

//=========================================================================================================================
// スコア格納処理
//=========================================================================================================================
void SetHiScore(void)
{
	// 変数宣言
	int nCntSet;

	GAMESTATE game = GetGameState();	// ゲーム情報
	GAMESTAGE stage = GetNumStage();	// ゲームステージ情報

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// セットをカウント
		if (g_Score[nCntSet].bUse == true)
		{// 使用されている場合
			if (game == GAMESTATE_GAMEPLAY && g_Score[nCntSet].type == SCORETYPE_SCORE)
			{
				if (g_HiScore[stage] < g_Score[nCntSet].nScore)
				{
					g_HiScore[stage] += g_Score[nCntSet].nScore;
				}
			}
			else if (game == GAMESTATE_GAMEPLAY && g_Score[nCntSet].type == SCORETYPE_TIME)
			{
				if (g_Score[nCntSet].nScore > 0)
				{
					g_HiScore[stage] += g_Score[nCntSet].nScore * 10;
				}
			}
		}
	}
}

//=========================================================================================================================
// スコアの破棄処理
//=========================================================================================================================
void DeleteScore(void)
{
	// 変数宣言
	int nCntSet;

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// セットをカウント
		if (g_Score[nCntSet].bUse == true)
		{// 使用されている場合
			g_Score[nCntSet].nScore = 0;								// スコアを初期化 
			g_Score[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// スコア中心値
			g_Score[nCntSet].posmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// スコア間隔値
			g_Score[nCntSet].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// スコア間隔値
			g_Score[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// スコア色
			g_Score[nCntSet].nSetNum = 0;								// 桁数
			g_Score[nCntSet].type = SCORETYPE_SCORE;					// 種類
			g_Score[nCntSet].bUse = false;								// 使用しているかどうか
		}
	}
	g_nCntScoreTime = 0;												// 初期化
}

//=========================================================================================================================
// スコアのセーブ処理
//=========================================================================================================================
void SaveScore(SCORETYPE Type)
{/*
	// 変数宣言
	FILE *pFile;		// ファイルポインタ
	int nCntSet;		// セットカウンタ
	int nCntSetSort;	// 整列
	int nScoreData;		// 値を格納

	// ファイル設定
	pFile = fopen(SCORE_FILE, "w");

	if (pFile != NULL)
	{// ファイルが開けた場合
	
		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}*/
}

//=========================================================================================================================
// スコアのロード処理
//=========================================================================================================================
void LoadScore(void)
{/*
	// 変数宣言
	FILE *pFile;	// ファイルポインタ
	int nCntSet;	// セットカウンタ

	// ファイル設定
	pFile = fopen(SCORE_FILE, "r");

	if (pFile != NULL)
	{// ファイルが開けた場合
	
		fclose(pFile);				//ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "ロードファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}*/
}