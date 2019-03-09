//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// サウンドファイル
//*************************************
typedef enum
{
	SOUND_LABEL_BGM_GAME = 0,	// ゲーム中のBGM
	SOUND_LABEL_BGM_TITLE,		// タイトルBGM
	SOUND_LABEL_BGM_GAMEOVER,	// ゲームオーバーBGM
	SOUND_LABEL_BGM_GAMECLEAR,	// ゲームクリアBGM
	SOUND_LABEL_BGM_TUTORIAL,	// チュートリアルBGM
	SOUND_LABEL_SE_GEAR,		// 歯車をはめる音
	SOUND_LABEL_SE_MISS,		// 失敗音
	SOUND_LABEL_SE_RUN,			// 走る音
	SOUND_LABEL_SE_WALK,		// 歩く音
	SOUND_LABEL_SE_PICKUP,		// 拾う音
	SOUND_LABEL_SE_DECIDE,		// 決定音
	SOUND_LABEL_SE_SELECT,		// 選択音
	SOUND_LABEL_SE_PAUSE,		// 
	SOUND_LABEL_SE_PAZZLEIN,	// 
	SOUND_LABEL_SE_PAZZLEEND,	// 
	SOUND_LABEL_SE_RESULT00,	// 
	SOUND_LABEL_SE_RESULT01,	// 
	SOUND_LABEL_SE_RESULT02,	// 
	SOUND_LABEL_SE_RESULT03,	// 
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
