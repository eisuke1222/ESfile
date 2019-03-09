//=============================================================================
//
// リザルト[ クリア ]画面 [result.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "result.h"
#include "main.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
//#include "start.h"
#include "flontpolygon.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define RESULT_POS_X		(0)								//背景の左上X座標
#define RESULT_POS_Y		(0)								//背景の左上Y座標
#define RESULT_WHIDTH		(SCREEN_WIDTH)					//背景の幅
#define RESULT_HEIGHT		(SCREEN_HEIGHT)					//背景の高さ

//=============================================================================
// グローバル変数
//=============================================================================
int g_nCntTimeResult;								//ゲーム画面切り替えカウンター
int g_nCntRanckNum;

//=============================================================================
// リザルト初期化処理
//=============================================================================
void InitResult(void)
{
	g_nCntTimeResult = 0;	//カウンターの初期化
	g_nCntRanckNum = 0;

	//変数の呼び出し
	//InitStart();		//エンターキー

	InitFlontPolygon();

	SetFlontPolygon(FLONTPOLYGONTEXTURE_CLEARRESULT, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTCLEAR, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f), SCREEN_WIDTH, SCREEN_HEIGHT);

	//エンターキー
	//SetEnter(D3DXVECTOR3(1090.0f, 685.0f, 0.0f), D3DXCOLOR(0.05f, 0.5f, 0.05f, 1.0f));	

	PlaySound(SOUND_LABEL_SE_RESULT01);
}

//=============================================================================
// リザルト終了処理
//=============================================================================
void UninitResult(void)
{
	UninitFlontPolygon();

	//UninitStart();		//エンターキー
}

//=============================================================================
// リザルト更新処理
//=============================================================================
void UpdateResult(void)
{
	QUESTCLEAR *pClearQuest = GetbClearQuest();

	g_nCntTimeResult++;

	if (g_nCntTimeResult == 60)
	{
		StopSound(SOUND_LABEL_SE_RESULT01);

		if (pClearQuest[0].nScore > 45000)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_A, D3DXVECTOR3(SCREEN_WIDTH / 2 - 350.0f, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);
			g_nCntRanckNum += 3;

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
		else if (pClearQuest[0].nScore > 30000)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_B, D3DXVECTOR3(SCREEN_WIDTH / 2 - 350.0f, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);
			g_nCntRanckNum += 2;

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
		else if (pClearQuest[0].nScore > 10000)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_C, D3DXVECTOR3(SCREEN_WIDTH / 2 - 350.0f, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);
			g_nCntRanckNum += 1;

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
		else 
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_D, D3DXVECTOR3(SCREEN_WIDTH / 2 - 350.0f, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
	}
	else if (g_nCntTimeResult == 90)
	{
		if (pClearQuest[1].nScore > 45000)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_A, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);
			g_nCntRanckNum += 3;

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
		else if (pClearQuest[1].nScore > 30000)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_B, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);
			g_nCntRanckNum += 2;

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
		else if (pClearQuest[1].nScore > 10000)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_C, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);
			g_nCntRanckNum += 1;

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
		else
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_D, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
	}
	else if (g_nCntTimeResult == 120)
	{
		if (pClearQuest[2].nScore > 45000)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_A, D3DXVECTOR3(SCREEN_WIDTH / 2 + 350.0f, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);
			g_nCntRanckNum += 3;

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
		else if (pClearQuest[2].nScore > 30000)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_B, D3DXVECTOR3(SCREEN_WIDTH / 2 + 350.0f, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);
			g_nCntRanckNum += 2;

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
		else if (pClearQuest[2].nScore > 10000)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_C, D3DXVECTOR3(SCREEN_WIDTH / 2 + 350.0f, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);
			g_nCntRanckNum += 1;

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
		else
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_D, D3DXVECTOR3(SCREEN_WIDTH / 2 + 350.0f, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300 * 10, 300 * 10);

			PlaySound(SOUND_LABEL_SE_RESULT02);
		}
	}
	else if (g_nCntTimeResult == 180)
	{
		if (g_nCntRanckNum > 7)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_CLEAR_RANK_A, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1000 * 10, 400 * 10);

			PlaySound(SOUND_LABEL_SE_RESULT03);
		}
		else if (g_nCntRanckNum > 4)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_CLEAR_RANK_B, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1000 * 10, 400 * 10);

			PlaySound(SOUND_LABEL_SE_RESULT03);
		}
		else if (g_nCntRanckNum > 1)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_CLEAR_RANK_C, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1000 * 10, 400 * 10);

			PlaySound(SOUND_LABEL_SE_RESULT03);
		}
		else
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_CLEAR_RANK_D, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1000 * 10, 400 * 10);

			PlaySound(SOUND_LABEL_SE_RESULT03);
		}
	}

	//フェード情報を取得する
	FADE g_fade;
	g_fade = GetFade();
	
	//決定キーが押されたかどうか
	if (g_nCntTimeResult > 200 && GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
	{
		if (g_fade != FADE_OUT)		//フェードアウトが実行されていないなら
		{
			//サウンド再生
			//PlaySound(SOUND_LABEL_SE_ENTER);

			//エンターキーの状態変更
			//SetPush(STARTSTRACT_ENTER);

			//フェードモード設定
			SetFade(MODE_TITLE);
		}		
	}

	//UpdateStart();		//エンターキー

	UpdateFlontPolygon();
}

//=============================================================================
// リザルト描画処理
//=============================================================================
void DrawResult(void)
{
	//DrawStart();		//エンターキー

	DrawFlontPolygon();
}