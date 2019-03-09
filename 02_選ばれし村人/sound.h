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
	SOUND_LABEL_BGM_GAME_0,		// BGMゲーム0
	SOUND_LABEL_BGM_GAME_1,		// BGMゲーム1
	SOUND_LABEL_SE_TITLE_0,		// SEタイトル0
	SOUND_LABEL_SE_TITLE_1,		// SEタイトル1
	SOUND_LABEL_SE_ATTACK_0,	// SE攻撃0
	SOUND_LABEL_SE_ATTACK_1,	// SE攻撃1
	SOUND_LABEL_SE_ATTACK_2,	// SE攻撃2
	SOUND_LABEL_SE_ATTACK_99,	// SE攻撃99
	SOUND_LABEL_SE_DAMAGE,		// SE負傷
	SOUND_LABEL_SE_ITEM,		// SEアイテム取得
	SOUND_LABEL_SE_LVUP,		// SEレベルアップ
	SOUND_LABEL_SE_WOLK,		// SE歩く
	SOUND_LABEL_SE_PAUSE_0,		// SEポーズ
	SOUND_LABEL_SE_PAUSE_1,		// SEポーズ選択
	SOUND_LABEL_SE_RESULT_0,	// SEリザルト0
	SOUND_LABEL_SE_RESULT_1,	// SEリザルト1
	SOUND_LABEL_SE_RESULT_2,	// SEリザルト2
	SOUND_LABEL_SE_RESULT_3,	// SEリザルト3
	SOUND_LABEL_SE_RANKING,		// SEランキング
	SOUND_LABEL_SE_MESSAGE,		// SEメッセージ
	SOUND_LABEL_SE_ENTER,		// SE決定
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
