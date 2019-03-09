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
#include "sound.h"
#include "input.h"					// キーボード
#include "fade.h"					// フェード
#include "game.h"
#include "window.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_SCORE_SET		(10)							// スコアの最大数
#define MAX_SCORE_NUMBER	(6)								// スコアの最大桁数
#define MAX_SCORE_RANKING	(6)								// ランキングの最大数
#define	TEXTURE_SCORE		"data/TEXTURE/number002.png"	// 読み込むテクスチャファイル名
#define	SCORE_FILE			"data/SAVE/scorernking.bin"		// 読み込むスコアテキストファイル名
#define SCORE_SPEED			(15.0f)							// スコアの移動速度

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureScore = NULL;			// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffScore = NULL;			// 頂点バッファへのポインタ
Score						g_Score[MAX_SCORE_SET];			// スコア情報
int							g_RankingScore[MAX_SCORE_RANKING];		// ハイスコア
int							g_nCntScoreTime;				// 時間カウンタ
int							g_RankinScore = 5;

//=========================================================================================================================
// スコア初期化処理
//=========================================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntSet, nCntRanking;	// セットカウンタ
	float nCntScore;			// スコアカウンタ
	float nScorePos;			// スコア位置
	MODE mode = GetMode();

	// スコアの初期化
	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// セットをカウント
		g_Score[nCntSet].nScore = 0;								// スコアを初期化 
		g_Score[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// スコア中心値
		g_Score[nCntSet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// スコア間隔値
		g_Score[nCntSet].fLimitMove = 0.0f;		//
		g_Score[nCntSet].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// スコア間隔値
		g_Score[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// スコア色
		g_Score[nCntSet].nSetNum = 0;								// 桁数
		g_Score[nCntSet].type = SCORETYPE_SCORE;					// 種類
		g_Score[nCntSet].bUse = false;								// 使用しているかどうか
	}
	for (nCntRanking = 0; nCntRanking < MAX_SCORE_RANKING; nCntRanking++)
	{
		g_RankingScore[nCntRanking] = 0;
	}

	g_nCntScoreTime = 0;											// 初期化

	if (mode != MODE_RESULT)
	{
		g_RankinScore = 5;
	}

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
		for (nCntScore = 0, nScorePos = 1200; nCntScore < MAX_SCORE_NUMBER; nCntScore++, nScorePos -= 50)
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
	D3DXVECTOR3 nPos;	// 位置を格納
	MODE mode = GetMode();
	GAMESTATE game = GetGameState();

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

			switch (g_Score[nCntSet].type)
			{// スコア種類
			case SCORETYPE_SCORE:		// スコア

				if (mode == MODE_RESULT)
				{// リザルト
					if (1000 + 10 > g_Score[nCntSet].pos.x && 1000 - 10 < g_Score[nCntSet].pos.x && 500 + 10 > g_Score[nCntSet].pos.y && 500 - 10 < g_Score[nCntSet].pos.y)
					{// 止まる
						g_Score[nCntSet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

						if (g_nCntScoreTime == 1)
						{
							StopSound(SOUND_LABEL_SE_RESULT_0);
							PlaySound(SOUND_LABEL_SE_RESULT_1);
							PlaySound(SOUND_LABEL_SE_RESULT_2);
							g_nCntScoreTime = 2;
						}
					}
					else
					{// 中央移動
						g_Score[nCntSet].move.x = sinf(atan2f((1000) - g_Score[nCntSet].pos.x, (500) - g_Score[nCntSet].pos.y)) * (g_Score[nCntSet].fLimitMove) * 20.0f;
						g_Score[nCntSet].move.y = cosf(atan2f((1000) - g_Score[nCntSet].pos.x, (500) - g_Score[nCntSet].pos.y)) * (g_Score[nCntSet].fLimitMove) * 20.0f;

						g_Score[nCntSet].fRadius.x += 0.8f;
						g_Score[nCntSet].fRadius.y += 0.8f;

						if (g_nCntScoreTime == 0)
						{
							PlaySound(SOUND_LABEL_SE_RESULT_0);
							g_nCntScoreTime = 1;
						}
					}

					if (g_Score[nCntSet].fLimitMove > 1.0f)
					{
						g_Score[nCntSet].fLimitMove = 1.0f;
					}
					else
					{
						g_Score[nCntSet].fLimitMove += 0.015f;
					}

					g_Score[nCntSet].pos += g_Score[nCntSet].move;
				}

				break;
			case SCORETYPE_RANKING:		// ランキング

				if (game == GAMESTATE_GAMECLEAR || game == GAMESTATE_GAMEOVER)
				{// クリア・オーバー
					if (g_nCntScoreTime == 0 && g_RankinScore < 5)
					{
						PlaySound(SOUND_LABEL_SE_RESULT_2);
					}
					else if (g_nCntScoreTime == 0 && g_RankinScore == 5)
					{
						PlaySound(SOUND_LABEL_SE_RANKING);
					}

					// ランク
					if (game == GAMESTATE_GAMECLEAR)
					{
						if (g_Score[g_RankinScore].nScore > 40000)
						{
							SetWindow(D3DXVECTOR3(275.0f, 530.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, WINDOWTEXTURE_RANKING_A, SETTEXTURE_FRONT);
						}
						else
						{
							SetWindow(D3DXVECTOR3(275.0f, 530.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, WINDOWTEXTURE_RANKING_B, SETTEXTURE_FRONT);
						}
					}
					else if (game == GAMESTATE_GAMEOVER)
					{
						if (g_Score[g_RankinScore].nScore > 20000)
						{
							SetWindow(D3DXVECTOR3(275.0f, 530.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, WINDOWTEXTURE_RANKING_C, SETTEXTURE_FRONT);
						}
						else
						{
							SetWindow(D3DXVECTOR3(275.0f, 530.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, WINDOWTEXTURE_RANKING_D, SETTEXTURE_FRONT);
						}
					}
				}

				g_nCntScoreTime++;

				// 自分スコア点滅
				if (g_nCntScoreTime < 30)
				{
					g_Score[g_RankinScore].col = D3DXCOLOR(0.8f, 1.0f, 0.1f, 1.0f);
				}
				else
				{
					g_Score[g_RankinScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				if (g_nCntScoreTime == 61)
				{
					g_nCntScoreTime = 1;
				}

				break;
			}

			nPos = g_Score[nCntSet].pos;			// 位置を格納
			nScoreData = g_Score[nCntSet].nScore;	// スコアを格納

			for (nCntScore = 0; nCntScore < MAX_SCORE_NUMBER; nCntScore++)
			{// スコアカウント
				// 頂点設定
				pVtx[nCntScore * 4].pos = D3DXVECTOR3(nPos.x, nPos.y - g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 1].pos = D3DXVECTOR3(nPos.x + g_Score[nCntSet].fRadius.x, nPos.y - g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 2].pos = D3DXVECTOR3(nPos.x, nPos.y + g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 3].pos = D3DXVECTOR3(nPos.x + g_Score[nCntSet].fRadius.x, nPos.y + g_Score[nCntSet].fRadius.y, 0.0f);
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

				nPos.x -= g_Score[nCntSet].fRadius.x + 3;	//位置をずらす
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
	bool SetScore = 0;
	D3DXVECTOR3 nPos;	// 位置を格納

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
		if (g_Score[nCntSet].bUse == true && g_Score[nCntSet].type == SCORETYPE_SCORE && type == SCORETYPE_SCORE)
		{// 使用されている場合
			SetScore = 1;
		}
	}

	if (SetScore == 0)
	{
		for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
		{// セットをカウント
			if (g_Score[nCntSet].bUse == false)
			{// 使用されている場合
				g_Score[nCntSet].pos = pos;							// 位置
				g_Score[nCntSet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動値
				g_Score[nCntSet].fLimitMove = 0.01f;
				nPos = g_Score[nCntSet].pos;						// 位置を格納
				g_Score[nCntSet].fRadius = fRadius;					// 大きさ
				g_Score[nCntSet].col = col;							// 色
				g_Score[nCntSet].type = type;						// スコアタイプ
				g_Score[nCntSet].bUse = true;						// 使用している状態

				switch (g_Score[nCntSet].type)
				{// スコア種類
				case SCORETYPE_SCORE:		// スコア
				case SCORETYPE_RANKING:		// ランキング
					g_Score[nCntSet].nScore = 0;					// スコア初期値
					g_Score[nCntSet].nSetNum = MAX_SCORE_NUMBER;	// 桁数
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

					nPos.x -= g_Score[nCntSet].fRadius.x + 3;	//位置をずらす
				}

				break;
			}

			pVtx += (MAX_SCORE_NUMBER * 4); 	// 頂点データのポインタ進める
		}
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
			if (g_Score[nCntSet].type == SCORETYPE_RANKING)
			{
				LoadScore();

				for (int nCntRanking = nCntSet, nCntScore = 0; nCntRanking < MAX_SCORE_RANKING - 1; nCntRanking++, nCntScore++)
				{// セットをカウント
					g_Score[nCntRanking].nScore = g_RankingScore[nCntScore];	// スコアを代入
				}
				
				break;
			}
			else
			{
				if (g_Score[nCntSet].nScore < 999999)
				{
					g_Score[nCntSet].nScore += nValue;	// スコアを代入
				}
				if (g_Score[nCntSet].nScore > 999999)
				{
					g_Score[nCntSet].nScore = 999999;
				}
			}

			break;
		}
	}
}

//=========================================================================================================================
// スコア格納処理
//=========================================================================================================================
void SetRankingScore(void)
{
	// 変数宣言
	int nCntSet;
	int nCntNum1, nCntNum2, nNumDate;

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// セットをカウント
		if (g_Score[nCntSet].bUse == true && g_Score[nCntSet].type == SCORETYPE_SCORE)
		{// 使用している場合
			LoadScore();

			g_RankingScore[MAX_SCORE_RANKING - 1] = g_Score[nCntSet].nScore;

			for (nCntNum1 = 0; nCntNum1 < MAX_SCORE_RANKING - 1; nCntNum1++)
			{
				for (nCntNum2 = nCntNum1; nCntNum2 < MAX_SCORE_RANKING; nCntNum2++)
				{
					if (g_RankingScore[nCntNum1] < g_RankingScore[nCntNum2])
					{
						nNumDate = g_RankingScore[nCntNum1];
						g_RankingScore[nCntNum1] = g_RankingScore[nCntNum2];
						g_RankingScore[nCntNum2] = nNumDate;

						if (g_RankinScore == 5)
						{
							g_RankinScore = nCntNum1;
						}
					}
				}
			}

			break;
		}
	}

	SaveScore();
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
			g_Score[nCntSet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// スコア間隔値
			g_Score[nCntSet].fLimitMove = 0.0f;
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
void SaveScore(void)
{
	// 変数宣言
	FILE *pFile;		// ファイルポインタ
	int nCntRanking;	// セットカウンタ

	// ファイル設定
	pFile = fopen(SCORE_FILE, "wb");

	if (pFile != NULL)
	{// ファイルが開けた場合
		/*for (nCntRanking = 0; nCntRanking < MAX_SCORE_RANKING; nCntRanking++)
		{
			g_RankingScore[nCntRanking] = 0;
		}*/

		fwrite(&g_RankingScore, sizeof(int), MAX_SCORE_RANKING, pFile);

		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "セーブファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}
}

//=========================================================================================================================
// スコアのロード処理
//=========================================================================================================================
void LoadScore(void)
{
	// 変数宣言
	FILE *pFile;		// ファイルポインタ
	int nCntRanking;	// セットカウンタ

	// ファイル設定
	pFile = fopen(SCORE_FILE, "rb");

	if (pFile != NULL)
	{// ファイルが開けた場合
		/*for (nCntRanking = 0; nCntRanking < MAX_SCORE_RANKING; nCntRanking++)
		{
			g_RankingScore[nCntRanking] = 0;
		}*/

		fread(&g_RankingScore, sizeof(int), MAX_SCORE_RANKING, pFile);

		fclose(pFile);				//ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "ロードファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}
}