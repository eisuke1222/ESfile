//=========================================================================================================================
//
// ランキング処理 [ranking.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "ranking.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "xobject.h"
#include "shadow.h"
#include "score.h"
#include "fade.h"
#include "input.h"
#include "window.h"

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
int				g_nCntRankingTimer;

//=========================================================================================================================
// ランキングの初期化処理
//=========================================================================================================================
void InitRanking(void)
{
	g_nCntRankingTimer = 0;

	// カメラ
	InitCamera();
	SetCamera(D3DXVECTOR3(100.0f, 650.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_RANKING);

	// ライト
	InitLight();

	// フィールド
	InitMeshField();

	// シリンダー
	InitMeshCylinder();

	// 影
	InitShadow();

	// Xオブジェクト
	InitXObject();

	// スコア
	InitScore();

	// 設定
	// フィールド
	SetMeshField(0, 2);
	SetMeshField(1, 0);
	SetMeshField(2, 3);
	SetMeshField(3, 0);
	SetMeshField(4, 0);
	SetMeshField(5, 0);
	SetMeshField(6, 0);
	SetMeshField(7, 0);
	SetMeshField(8, 0);

	// シリンダー
	SetMeshCylinder(0);
	SetMeshCylinder(1);

	// オブジェクト
	SetXObject(TEXTTYPE_TREE_9);
	SetXObject(TEXTTYPE_TREE_10);
	SetXObject(TEXTTYPE_TREE_11);
	SetXObject(TEXTTYPE_TREE_12);
	SetXObject(TEXTTYPE_TREE_13);
	SetXObject(TEXTTYPE_TREE_14);

	//
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, 95, 0.0f), D3DXVECTOR3(35, 45, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_RANKING);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, 235, 0.0f), D3DXVECTOR3(35, 45, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_RANKING);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, 375, 0.0f), D3DXVECTOR3(35, 45, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_RANKING);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, 515, 0.0f), D3DXVECTOR3(35, 45, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_RANKING);
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, 655, 0.0f), D3DXVECTOR3(35, 45, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_RANKING);
	AddScore(SCORETYPE_RANKING, 0);

	SetWindow(D3DXVECTOR3(275.0f, 130.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 400, 200, WINDOWTEXTURE_RANKING_BG, SETTEXTURE_FRONT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, 95.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 180, WINDOWTEXTURE_RANKING_1, SETTEXTURE_FRONT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 220, 230.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130, 130, WINDOWTEXTURE_RANKING_2, SETTEXTURE_FRONT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 220, 370.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130, 130, WINDOWTEXTURE_RANKING_3, SETTEXTURE_FRONT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 220, 510.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130, 130, WINDOWTEXTURE_RANKING_4, SETTEXTURE_FRONT);
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 220, 650.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 130, 130, WINDOWTEXTURE_RANKING_5, SETTEXTURE_FRONT);
}

//=========================================================================================================================
// ランキングの終了処理
//=========================================================================================================================
void UninitRanking(void)
{
	// カメラ
	UninitCamera();

	// ライト
	UninitLight();

	// フィールド
	UninitMeshField();

	// シリンダー
	UninitMeshCylinder();

	// 影
	UninitShadow();

	// Xオブジェクト
	UninitXObject();

	// スコア
	UninitScore();

	// ウィンドウ
	DeleteWindow();
}

//=========================================================================================================================
// ランキングの更新処理
//=========================================================================================================================
void UpdateRanking(void)
{
	// カメラ
	UpdateCamera();

	// ライト
	UpdateLight();

	// フィールド
	UpdateMeshField();

	// シリンダー
	UpdateMeshCylinder();

	// 影
	UpdateShadow();

	// Xオブジェクト
	UpdateXObject();

	// スコア
	UpdateScore();

	g_nCntRankingTimer++;

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
		GetButtonTrigger(2) == true) || g_nCntRankingTimer > 600)
	{// タイトルへ移行
		SetFadeMode(MODE_TITLE);
	}
}

//=========================================================================================================================
// ランキングの描画処理
//=========================================================================================================================
void DrawRanking(void)
{
	// カメラ
	SetUpCamera();

	// フィールド
	DrawMeshField();

	// シリンダー
	DrawMeshCylinder();

	// 影
	DrawShadow();

	// Xオブジェクト
	DrawXObject();

	// スコア
	DrawScore();
}
