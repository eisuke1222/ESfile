//=========================================================================================================================
//
// フォント処理 [font.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanfエラー解除
#include "stdio.h"					// io
#include "font.h"					// フォント
#include "title.h"					// タイトル
#include "game.h"					// ゲーム
#include "xplayer.h"				// プレイヤー
#include "sound.h"					// サウンド
#include "window.h"					// ウィンドウ
#include "input.h"					// 入力

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define MAX_FONT		(8)								// フォントの最大数
#define MAX_MASSAGE		(64)							// 文字の最大数
#define FONTFAILE_NAME "data\\TEXT\\messagedata.txt"	// フォントファイル名

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
FONTSTATE g_aFont[MAX_FONT];							// フォント
char g_aMassage[MAX_MASSAGE][MAX_FONTNUM];				// メッセージ
int	g_nCntFontTimer;									// カウント
bool g_bMassageSet;										// setチェック
int	g_MassageFontNum;
int	g_MassageNum;
int g_MaxMassageNum;

//=========================================================================================================================
// フォントの初期化処理
//=========================================================================================================================
void InitFont(void)
{
	int nCntFont;
	FILE *pFile;

	g_bMassageSet = false;
	g_nCntFontTimer = 0;
	g_MassageFontNum = 0;
	g_MassageNum = 0;

	// ファイル設定
	pFile = fopen(FONTFAILE_NAME, "r");

	if (pFile != NULL)
	{// ファイルが開けた場合
		fscanf(pFile, "%d", &g_MaxMassageNum);	// 読み込み

		for (nCntFont = 0; nCntFont < g_MaxMassageNum; nCntFont++)
		{
			fscanf(pFile, "%s", &g_aMassage[nCntFont][0]);	// 読み込み
		}

		fclose(pFile);	// ファイルを閉じる
	}
	else
	{// ファイルが開けない場合
		MessageBox(0, "設定ファイルのチェックに失敗！", "警告！", MB_ICONWARNING);
	}

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{// カウント
		g_aFont[nCntFont].pFont = NULL;
		g_aFont[nCntFont].nWidth = 0;
		g_aFont[nCntFont].nHeight = 0;
		g_aFont[nCntFont].nFont = SHIFTJIS_CHARSET;
		g_aFont[nCntFont].rect = { 0, 0, 0, 0 };
		g_aFont[nCntFont].nSet = DT_LEFT;
		g_aFont[nCntFont].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aFont[nCntFont].bUse = false;
	}
}

//=========================================================================================================================
// フォントの終了処理
//=========================================================================================================================
void UninitFont(void)
{
	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (g_aFont[nCntFont].pFont != NULL)
		{// デバッグ表示用フォントの開放
			g_aFont[nCntFont].pFont->Release();
			g_aFont[nCntFont].pFont = NULL;
		}
	}
}

//=========================================================================================================================
// フォントの更新処理
//=========================================================================================================================
void UpdateFont(void)
{
	MODE mode = GetMode();
	GAMESTATE game = GetGameState();
	int nTitleView = GetViewNum();
	char cData[128];
	bool bUpMessage = false;

	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{// カウント
		if (g_aFont[nCntFont].bUse == true)
		{//
			bUpMessage = true;

			break;
		}
	}
	if (bUpMessage == true)
	{
		if ((mode == MODE_TITLE && nTitleView == 1) || game == GAMESTATE_TUTORIAL)
		{// チュートリアル画面
			g_nCntFontTimer++;

			if (g_nCntFontTimer >= 30)
			{// 表示開始
				if (g_bMassageSet == true)
				{// 使用するなら
					if (g_nCntFontTimer % 3 == 0)
					{// 表示間隔
						wsprintf(&cData[0], &g_aMassage[g_MassageNum][g_MassageFontNum]);
						cData[2] = '\0';
						SetwsFont((g_MassageNum + 1) % 2 == 0, 1, &cData[0]);

						PlaySound(SOUND_LABEL_SE_MESSAGE);

						g_MassageFontNum += 2;

						if (g_aMassage[g_MassageNum][g_MassageFontNum] == NULL)
						{// NULLになったら
							g_MassageNum++;
							g_MassageFontNum = 0;
							g_nCntFontTimer = 0;

							if (g_MassageNum % 2 == 0)
							{
								g_bMassageSet = false;
							}
						}
					}
				}
				else
				{// 表示終了
					if (g_MassageNum < g_MaxMassageNum)
					{
						if (g_MassageNum == 6 && g_nCntFontTimer == 30)
						{// フェード
							SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOW_TUTORIAL, SETTEXTURE_FRONT);
						}

						if (g_nCntFontTimer >= 60)
						{// 初期化して次へ
							g_bMassageSet = true;
							g_nCntFontTimer = 0;

							SetwsFont(0, 0, "");
							SetwsFont(1, 0, "");
						}
					}
					else
					{// 全文終了
						if (g_nCntFontTimer == 60)
						{// メッセージ破棄
							DeleteWindow(WINDOWTEXTURE_MESSAGE);
							SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0, WINDOWTEXTURE_GAME_TUTORIAL, SETTEXTURE_FRONT);

							SetwsFont(0, 0, "");
							SetwsFont(1, 0, "");

							SetMove(true);	// プレイヤー動けるように
						}
						else if (g_nCntFontTimer == 420)
						{// 操作メニュー破棄
							//if (GetKeyboardTrigger(DIK_RETURN) == true)
							{
								DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL);
								DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_MESSAGE);

								SetTutorialStep(true);

								SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 430, SCREEN_HEIGHT - 80, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_SELECT_2, SETTEXTURE_FRONT);
							}
						}
					}
				}
			}
		}
	}
}

//=========================================================================================================================
// フォントの描画処理
//=========================================================================================================================
void DrawFont(void)
{
	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{// カウント
		if (g_aFont[nCntFont].bUse == true)
		{// 使用しているなら
			// テキスト描画
			// hDC      : デバイスコンテキストのハンドル
			// lpString : 描画する文字列へのポインタ
			// nCount   : 文字列の文字数
			// lpRect   : 長方形領域
			// uFormat  : テキストの整形方法を指定
			g_aFont[nCntFont].pFont->DrawText(NULL, &g_aFont[nCntFont].aFont[0], -1, &g_aFont[nCntFont].rect, g_aFont[nCntFont].nSet, g_aFont[nCntFont].col);
		}
	}
}

//=========================================================================================================================
// フォントの描画処理
//=========================================================================================================================
void SetFont(int nWidth, int nHeight, int nFont, RECT rect, int nSet, D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCntFont;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{// カウント
		if (g_aFont[nCntFont].bUse == false)
		{// 使用していないなら
			g_aFont[nCntFont].nWidth = nWidth;
			g_aFont[nCntFont].nHeight = nHeight;
			g_aFont[nCntFont].nFont = nFont;
			g_aFont[nCntFont].rect = rect;
			g_aFont[nCntFont].nSet = nSet;
			g_aFont[nCntFont].col = col;
			g_aFont[nCntFont].bUse = true;

			// デバッグ表示用フォントを設定
			// pDevice         : デバイスへのポインタ
			// Height          : 文字の高さ
			// Width           : 文字の幅
			// Weight          : フォントの太さ
			// MipLevels       : ミップマップレベル数
			// Italic          : イタリックフォント
			// CharSet         : フォントの文字セット
			// OutputPrecision : 実際のフォントと目的のフォントのサイズおよび特性の一致方法を指定
			// Quality         : 実際のフォントと目的のフォントとの一致方法を指定
			// PitchAndFamily  : ピッチとファミリインデックス
			// pFacename       : フォントの名前を保持する文字列
			// ppFont          : フォントへのポインタ
			D3DXCreateFont(pDevice, nWidth, nHeight, 0, 0, FALSE, nFont,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Font", &g_aFont[nCntFont].pFont);

			break;
		}
	}
}

//=========================================================================================================================
// フォントの代入処理
//=========================================================================================================================
void SetwsFont(int nSetNum, int nSettype, char *aFont)
{
	if (g_aFont[nSetNum].bUse == true)
	{
		// 文字列を代入
		if (nSettype == 0)
		{// 全文
			wsprintf(&g_aFont[nSetNum].aFont[0], &aFont[0]);
		}
		else if (nSettype == 1)
		{// 追記
			strcat(&g_aFont[nSetNum].aFont[0], &aFont[0]);
		}
	}
}

//=========================================================================================================================
// フォントの破棄処理
//=========================================================================================================================
void DeleteFont(void)
{
	int nCntFont;

	g_bMassageSet = false;
	g_nCntFontTimer = 0;
	g_MassageFontNum = 0;
	g_MassageNum = 0;

	for (nCntFont = 0; nCntFont < MAX_FONT; nCntFont++)
	{
		if (g_aFont[nCntFont].bUse == true)
		{
			SetwsFont(nCntFont, 0, "");

			g_aFont[nCntFont].pFont = NULL;
			g_aFont[nCntFont].nWidth = 0;
			g_aFont[nCntFont].nHeight = 0;
			g_aFont[nCntFont].nFont = SHIFTJIS_CHARSET;
			g_aFont[nCntFont].rect = { 0, 0, 0, 0 };
			g_aFont[nCntFont].nSet = DT_LEFT;
			g_aFont[nCntFont].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			g_aFont[nCntFont].bUse = false;
		}
	}
}

//=========================================================================================================================
// メッセージ表示処理
//=========================================================================================================================
void SetMessage(bool bMassageSet)
{
	g_bMassageSet = bMassageSet;
}