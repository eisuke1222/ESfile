//=============================================================================
//
// Game処理 [game.h]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// 状態設定
//=============================================================================
typedef enum
{//ゲーム
	GAMESTATE_NONE = 0,
	GAMESTATE_QUESTSELECT,	//選択画面
	GAMESTATE_QUESTPLAY,	//ゲーム画面
	GAMESTATE_END_CLIA,		//ゲームクリア画面
	GAMESTATE_END_OVER,		//ゲームオーバー画面
	GAMESTATE_MAX
}GAMESTATE;

typedef enum
{// クエスト選択画面
	QUESTTYPE_TUTORIAL = 0,	//チュートリアル
	QUESTTYPE_FIRST,		//クエスト１
	QUESTTYPE_SECOND,		//クエスト２
	QUESTTYPE_THIRD,		//クエスト３
	QUESTTYPE_TITLE,		//タイトル
	QUESTTYPE_MAX,
}QUESTTYPE;

typedef enum
{// クエスト中
	QUESTSTATE_NONE = 0,
	QUESTSTATE_MISSTION,	//依頼をうけるシーン
	QUESTSTATE_SEARCH,		//探索シーン
	QUESTSTATE_PUZZLE,		//パズルシーン
	QUESTSTATE_REPORT,		//報告シーン
	QUESTSTATE_CLEAR,		//クエストクリアシーン
	QUESTSTATE_OVER,		//クエスト失敗シーン
	QUESTSTATE_MAX,
}QUESTSTATE;

typedef struct
{
	int nScore;
	bool bClear;
}QUESTCLEAR;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);
void SetQuestType(QUESTTYPE type);
void SetQuestState(QUESTSTATE state);
GAMESTATE GetGameState(void);
QUESTSTATE GetQuestState(void);
QUESTCLEAR *GetbClearQuest(void);

QUESTTYPE GetQuestType(void);

void SetPause(bool bPause);
bool GetGAMEPause(void);

void SetQuestClearScore(int nScore);

void SetShowTutorial(bool bTutorial);
#endif