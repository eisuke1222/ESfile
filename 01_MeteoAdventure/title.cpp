//=========================================================================================================================
// 
// タイトル処理 [title.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "title.h"		// タイトル
#include "input.h"		// 入力処理
#include "player.h"		// プレイヤー
#include "enemy.h"		// 敵
#include "block.h"		// ブロック
#include "bgwindow.h"	// 背景
#include "fade.h"		// フェード
#include "sound.h"		// サウンド

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************

//=========================================================================================================================
// タイトル初期化処理
//=========================================================================================================================
void InitTitle(void)
{
	// 背景設定
	SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_TITLE_BG);
	SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 350, 250, BGWINDOWTEXTURE_TITLE_LOGO);
	SetBGWindow(D3DXVECTOR3(100, SCREEN_HEIGHT, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 200, 200, BGWINDOWTEXTURE_TITLE_GOUND);
	SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH - 45, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);
	SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 250, 0.0f), D3DXCOLOR(0.1f, 0.8f, 1.0f, 1.0f), 200, 50, BGWINDOWTEXTURE_SELECT);

	// ブロック初期化処理
	InitBlock();
	for (int nCount = 0; nCount < 6; nCount++)
	{// 1段目
		SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 240 + (nCount * 40.0f), SCREEN_HEIGHT - 40, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40, 40, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
	}
	for (int nCount = 0; nCount < 5; nCount++)
	{// 2段目
		SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 200 + (nCount * 40.0f), SCREEN_HEIGHT - 80, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40, 40, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
	}
	for (int nCount = 0; nCount < 4; nCount++)
	{// 3段目
		SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 160 + (nCount * 40.0f), SCREEN_HEIGHT - 120, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40, 40, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
	}
	for (int nCount = 0; nCount < 3; nCount++)
	{// 4段目
		SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 120 + (nCount * 40.0f), SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40, 40, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
	}

	// 敵初期化処理
	InitEnemy();
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 220, SCREEN_HEIGHT - 45, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 10, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 85, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 10, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SNOWMAN);
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 140, SCREEN_HEIGHT - 125, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 10, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_CURSE);
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 165, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 10, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_BIRD);

	// プレイヤー初期化処理
	InitPlayer();
	SetPlayer(D3DXVECTOR3(100, SCREEN_HEIGHT - 180, 0.0f));
}

//=========================================================================================================================
// タイトル終了処理
//=========================================================================================================================
void UninitTitle(void)
{
	// ブロック終了処理
	UninitBlock();

	// 敵終了処理
	UninitEnemy();

	// プレイヤー終了処理
	UninitPlayer();
}

//=========================================================================================================================
// タイトル更新処理
//=========================================================================================================================
void UpdateTitle(void)
{
	// ブロック更新処理
	UpdateBlock();

	// 敵更新処理
	UpdateEnemy();

	// プレイヤー更新処理
	UpdatePlayer();

	FADE fade = GetFade();	// フェード情報
	if (fade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{// 任意のキー（ENTER）を押した場合
			PlaySound(SOUND_LABEL_SE_DECIDE);
			SetSelectState(SELECTSTATE_ENTER);			// 選択状態変更
			SetFadeMode(MODE_GAME);		// ゲームへ
		}
	}
}

//=========================================================================================================================
// タイトル描画処理
//=========================================================================================================================
void DrawTitle(void)
{
	// ブロック描画処理
	DrawBlock();

	// 敵描画処理
	DrawEnemy();

	// プレイヤー描画処理
	DrawPlayer();
}