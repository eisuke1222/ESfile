//=========================================================================================================================
// 
// サウンド処理 [sound.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"

//****************************************************
// サウンドファイル
//****************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,	// BGMタイトル
	SOUND_LABEL_BGM_GAME_MAP,	// BGMゲームマップ
	SOUND_LABEL_BGM_GAME_STAGE1,// BGMステージ1
	SOUND_LABEL_BGM_GAME_STAGE2,// BGMステージ2
	SOUND_LABEL_BGM_GAME_STAGE3,// BGMステージ3
	SOUND_LABEL_BGM_GAME_CLEAR,	// BGMゲームクリア
	SOUND_LABEL_BGM_GAME_OVER,	// BGMゲームオーバー
	SOUND_LABEL_SE_GAME_STAGE_CLEAR,// ステージクリア
	SOUND_LABEL_SE_JUMP000,		// ジャンプ
	SOUND_LABEL_SE_SHOT000,		// 弾発射音
	SOUND_LABEL_SE_HIT000,		// ヒット音0
	SOUND_LABEL_SE_HIT001,		// ヒット音1
	SOUND_LABEL_SE_ITEMHIT000,	// アイテムヒット音
	SOUND_LABEL_SE_BULLETUP000,	// 弾変更音
	SOUND_LABEL_SE_DECIDE,		// 決定音
	SOUND_LABEL_SE_SELECT001,	// 選択音1
	SOUND_LABEL_SE_SELECT002,	// 選択音2
	SOUND_LABEL_SE_PAUSEIN,		// ポーズ入る
	SOUND_LABEL_SE_PAUSEOUT,	// ポーズ出る
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
HRESULT InitSound(HWND hWnd);			// サウンド初期化処理
void UninitSound(void);					// サウンド終了処理
HRESULT PlaySound(SOUND_LABEL label);	// サウンド再生処理
void StopSound(SOUND_LABEL label);		// サウンド停止処理
void StopSound(void);					// サウンド全停止処理

#endif
