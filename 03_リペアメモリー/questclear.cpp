//=============================================================================
//
// クエストクリア画面 [questclear.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "questclear.h"
#include "main.h"
#include "gamefade.h"
#include "sound.h"
#include "input.h"
#include "start.h"
#include "BG.h"
#include "flontpolygon.h"
#include "game.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================
int nCntQuestClearTimer;

//=============================================================================
// 初期化処理
//=============================================================================
void InitQuestClear(void)
{
	nCntQuestClearTimer = 0;

	//変数呼び出し
	InitStart();		//エンターキー
	
	//背景
	//SetBG(2);

	//エンターキー
	SetEnter(D3DXVECTOR3(190.0f, 695.0f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitQuestClear(void)
{
	DeleteStart();		//エンターキー
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateQuestClear(void)
{
	//フェード情報を取得する
	GAMEFADE fade;
	fade = GetGameFade();

	QUESTTYPE pQuestType= GetQuestType();
	QUESTCLEAR *pClearQuest = GetbClearQuest();

	nCntQuestClearTimer++;

	if (nCntQuestClearTimer == 75)
	{
		if (pQuestType == QUESTTYPE_FIRST)
		{
			if (pClearQuest[0].nScore > 45000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_A, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
			else if (pClearQuest[0].nScore > 30000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_B, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
			else if (pClearQuest[0].nScore > 10000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_C, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
			else
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_D, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
		}
		else if (pQuestType == QUESTTYPE_SECOND)
		{
			if (pClearQuest[1].nScore > 45000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_A, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
			else if (pClearQuest[1].nScore > 30000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_B, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
			else if (pClearQuest[1].nScore > 10000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_C, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
			else
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_D, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
		}
		else if (pQuestType == QUESTTYPE_THIRD)
		{
			if (pClearQuest[2].nScore > 45000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_A, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
			else if (pClearQuest[2].nScore > 30000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_B, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
			else if (pClearQuest[2].nScore > 10000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_C, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
			else
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_D, D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 500 * 10, 500 * 10);

				PlaySound(SOUND_LABEL_SE_RESULT00);
			}
		}
	}

	//決定キーが押されたかどうか
	if (nCntQuestClearTimer > 90 && (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true))
	{
		if (fade != GAMEFADE_OUT)		//フェードアウトが実行されていないなら
		{
			//スタートボタンの設定
			SetPush(STARTSTRACT_ENTER);

			//フェードモード設定
			SetGameFade(GAMESTATE_QUESTSELECT);		//選択画面に戻る
		}
	}

	UpdateStart();		//エンターキー
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawQuestClear(void)
{
	DrawStart();		//エンターキー
}