//=============================================================================
//
// リザルト[ ゲームオーバー ]画面 [result.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "resultover.h"
#include "main.h"
//#include "sound.h"
#include "input.h"
#include "fade.h"
#include "start.h"
#include "flontpolygon.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================
int g_nCntTimeResultOver;	//ゲーム画面切り替えカウンター

//=============================================================================
// リザルト初期化処理
//=============================================================================
void InitResultOver(void)
{
	g_nCntTimeResultOver = 0;	//カウンターの初期化

	InitFlontPolygon();		//フロントポリゴン
	InitStart();			//エンターキー

	//ゲームオーバーロゴ
	SetFlontPolygon(FLONTPOLYGONTEXTURE_GAME_OVER, D3DXVECTOR3(650.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1000.0f, 300.0f);

	//エンターキー
	SetEnter(D3DXVECTOR3(1090.0f, 685.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//=============================================================================
// リザルト終了処理
//=============================================================================
void UninitResultOver(void)
{
	UninitFlontPolygon();	//フロントポリゴン
	UninitStart();			//エンターキー
}

//=============================================================================
// リザルト更新処理
//=============================================================================
void UpdateResultOver(void)
{
	g_nCntTimeResultOver++;

	//フェード情報を取得する
	FADE g_fade;
	g_fade = GetFade();

	//決定キーが押されたかどうか
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
	{
		if (g_fade != FADE_OUT)		//フェードアウトが実行されていないなら
		{
			//サウンド再生
			//PlaySound(SOUND_LABEL_SE_ENTER);

			//エンターキーの状態変更
			SetPush(STARTSTRACT_ENTER);

			//フェードモード設定
			SetFade(MODE_TITLE);
		}
	}

	if (g_nCntTimeResultOver == INTERVAL_TIME)
	{
		//フェードモード設定
		SetFade(MODE_TITLE);
	}

	if (GetKeyboardTrigger(DIK_B) == true)
	{
		if (g_fade != FADE_OUT)		//フェードアウトが実行されていないなら
		{
			//StopSound(SOUND_LABEL_BGM003);

			//フェードモード設定
			SetFade(MODE_GAME);
		}
	}

	UpdateFlontPolygon();	//フロントポリゴン
	UpdateStart();			//エンターキー
}

//=============================================================================
// リザルト描画処理
//=============================================================================
void DrawResultOver(void)
{
	DrawFlontPolygon();		//フロントポリゴン
	DrawStart();			//エンターキー
}