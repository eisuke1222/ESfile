//=========================================================================================================================
//
// タイトル処理 [title.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include <time.h>
#include "title.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "xobject.h"
#include "shadow.h"
#include "font.h"
#include "meshwall.h"
#include "billboardobject.h"
#include "xenemy.h"
#include "xplayer.h"
#include "sound.h"
#include "window.h"
#include "fade.h"
#include "input.h"
#include "game.h"

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
int					g_nCntTitleTimer;		// タイトルタイマー
bool				g_bModeChange;			// モードチェンジ
bool				g_bSet;					// 設定有無
bool				g_bTitleSound = false;	//
int					g_nCntView;

//=========================================================================================================================
// タイトルの初期化処理
//=========================================================================================================================
void InitTitle(void)
{
	// 初期化
	g_nCntTitleTimer = 0;
	g_bModeChange = false;
	g_bSet = false;
	g_nCntView = 0;

	// カメラ初期化処理
	InitCamera();

	// ライト初期化処理
	InitLight();

	// フィールド初期化処理
	InitMeshField();

	// 壁初期化処理
	InitMeshWall();

	// シリンダー初期化処理
	InitMeshCylinder();

	// 影初期化処理
	InitShadow();

	// オブジェクト初期化処理
	InitXObject();

	// 敵初期化処理
	InitXEnemy();

	// プレイヤー初期化処理
	InitXPlayer();

	// ビルボードオブジェクト初期化処理
	InitBillboardObject();

	SetGameState(GAMESTATE_NONE);

	// ウィンドウ設定
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_TITLE_ANIM, SETTEXTURE_TOPFRONT);

	if (g_bTitleSound == false)
	{
		StopSound();
	}
}

//=========================================================================================================================
// タイトルの終了処理
//=========================================================================================================================
void UninitTitle(void)
{
	// カメラ終了処理
	UninitCamera();

	// ライト終了処理
	UninitLight();

	// フィールド終了処理
	UninitMeshField();

	// 壁終了処理
	UninitMeshWall();

	// シリンダー終了処理
	UninitMeshCylinder();

	// 影終了処理
	UninitShadow();

	// オブジェクト終了処理
	UninitXObject();

	// 敵終了処理
	UninitXEnemy();

	// プレイヤー終了処理
	UninitXPlayer();

	// ビルボードオブジェクト終了処理
	UninitBillboardObject();

	// ウィンドウ破棄
	DeleteWindow();

	// フォント破棄
	DeleteFont();
}

//=========================================================================================================================
// タイトルの更新処理
//=========================================================================================================================
void UpdateTitle(void)
{
	FADE fade = GetFade();		// フェード状態取得

	// カメラ更新処理
	UpdateCamera();

	// ライト更新処理
	UpdateLight();

	// フィールド更新処理
	UpdateMeshField();

	// 壁更新処理
	UpdateMeshWall();

	// シリンダー更新処理
	UpdateMeshCylinder();

	// 影更新処理
	UpdateShadow();

	// オブジェクト更新処理
	UpdateXObject();

	// 敵更新処理
	UpdateXEnemy();

	// プレイヤー更新処理
	UpdateXPlayer();

	// ビルボードオブジェト更新処理
	UpdateBillboardObject();

	// 時間カウンタ
	g_nCntTitleTimer++;

	if (g_nCntTitleTimer == 600)
	{// 自動でランキングに移行
		g_nCntView++;

		if (g_nCntView == 3)
		{
			SetFadeMode(MODE_RANKING);
		}
		else
		{
			SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOW_FADE, SETTEXTURE_TOPBG);
		}
	}
	else if (g_nCntTitleTimer == 635)
	{// 切り替え
		g_nCntTitleTimer = 0;

		if (g_nCntView == 1)
		{
			srand((unsigned int)time(0));

			// 破棄
			DeleteMeshField();
			DeleteMeshCylinder();
			DeleteShadow();
			DeleteXObject();

			// ステージorチュートリアル
			if ((rand() % 2) == 0)
			{
				// カメラ
				SetCamera(D3DXVECTOR3(0.0f, 180.0f, -350.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), CAMERATYPE_GAME);

				// メッシュフィールド
				SetMeshField(9, 4);

				// プレイヤー
				SetXPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				// メッセージ
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_MESSAGE, SETTEXTURE_FRONT);

				// フォント
				SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 600, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 640, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 675, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				SetMessage(true);
			}
			else
			{
				// カメラ
				SetCamera(D3DXVECTOR3(0.0f, 180.0f, -2050.0f), D3DXVECTOR3(0.0f, 50.0f, -1650.0f), CAMERATYPE_TITLE_GAME);

				// メッシュフィールド
				SetMeshField(0, 1);
				SetMeshField(1, 0);
				SetMeshField(2, 1);
				SetMeshField(3, 0);
				SetMeshField(4, 0);
				SetMeshField(5, 0);
				SetMeshField(6, 0);
				SetMeshField(7, 1);
				SetMeshField(8, 0);

				// オブジェクト
				SetXObject(TEXTTYPE_TREE_0);
				SetXObject(TEXTTYPE_TREE_1);
				SetXObject(TEXTTYPE_TREE_2);
				SetXObject(TEXTTYPE_TREE_3);
				SetXObject(TEXTTYPE_TREE_4);
				SetXObject(TEXTTYPE_TREE_5);
				SetXObject(TEXTTYPE_TREE_6);
				SetXObject(TEXTTYPE_TREE_7);
				SetXObject(TEXTTYPE_TREE_8);

				// ビルボードオブジェクト
				SetBillboardObject(D3DXVECTOR3(-1000.0f, 0.0f, 1650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXVECTOR3(300.0f, 50.0f, 0.0f), BILLBOARDTEXTURE_MAP_WALL);
				SetBillboardObject(D3DXVECTOR3(0.0f, 0.0f, 1650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXVECTOR3(300.0f, 50.0f, 0.0f), BILLBOARDTEXTURE_MAP_WALL);
				SetBillboardObject(D3DXVECTOR3(1000.0f, 0.0f, 1650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXVECTOR3(300.0f, 50.0f, 0.0f), BILLBOARDTEXTURE_MAP_WALL);

				// オブジェクト
				SetXObject(TEXTTYPE_MARUTA_2);
				SetXObject(TEXTTYPE_MARUTA_3);
				SetXPlayer(D3DXVECTOR3(0.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f));

				// メッシュウォール
				SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
				SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
				SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
				SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));

				// メッシュシリンダー
				SetMeshCylinder(0);
				SetMeshCylinder(1);

				// 敵
				SetXEnemy(rand() % 3, D3DXVECTOR3(-1000.0f, 0.0f, -500.0f));
				SetXEnemy(rand() % 3, D3DXVECTOR3(0.0f, 0.0f, -500.0f));
				SetXEnemy(rand() % 3, D3DXVECTOR3(1000.0f, 0.0f, -500.0f));

				SetXEnemy(rand() % 3, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, 5.0f));
				SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(150.0f, 0.0f, 600.0f));
				SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-150.0f, 0.0f, 600.0f));

				// アイテム
				SetBillboardObject(D3DXVECTOR3(-1000 - 160, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(-1000 - 80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(-1000, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(-1000 + 80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(-1000 + 160, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);

				SetBillboardObject(D3DXVECTOR3(0 - 160, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(0 - 80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(0, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(0 + 80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(0 + 160, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);

				SetBillboardObject(D3DXVECTOR3(1000 - 160, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(1000 - 80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(1000, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(1000 + 80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(1000 + 160, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
			}
		}
		else if (g_nCntView == 2)
		{
			// 破棄
			DeleteMeshField();
			DeleteMeshWall();
			DeleteMeshCylinder();
			DeleteShadow();
			DeleteXObject();
			DeleteXEnemy();
			DeleteXPlayer();
			DeleteBillboardObject();
			DeleteWindow(WINDOWTEXTURE_MESSAGE);
			DeleteFont();

			// カメラ
			SetCamera(D3DXVECTOR3(0.0f, 650.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_TITLE);

			// メッシュフィールド
			SetMeshField(0, 1);
			SetMeshField(1, 0);
			SetMeshField(2, 1);
			SetMeshField(3, 0);
			SetMeshField(4, 0);
			SetMeshField(5, 0);
			SetMeshField(6, 0);
			SetMeshField(7, 1);
			SetMeshField(8, 0);

			// メッシュシリンダー
			SetMeshCylinder(0);
			SetMeshCylinder(1);

			// オブジェクト
			SetXObject(TEXTTYPE_TREE_0);
			SetXObject(TEXTTYPE_TREE_1);
			SetXObject(TEXTTYPE_TREE_2);
			SetXObject(TEXTTYPE_TREE_3);
			SetXObject(TEXTTYPE_TREE_4);
			SetXObject(TEXTTYPE_TREE_5);
			SetXObject(TEXTTYPE_TREE_6);
			SetXObject(TEXTTYPE_TREE_7);
			SetXObject(TEXTTYPE_TREE_8);
		}
	}

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
		GetButtonTrigger(2) == true) && fade == FADETYPE_NONE && g_bModeChange == true)
	{// モードチェンジ
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFadeMode(MODE_GAME);
		SetSelectState(SELECTSTATE_ENTER);
	}

	if (g_bTitleSound == false && g_nCntTitleTimer == 1 && g_nCntView == 0)
	{
		StopSound(SOUND_LABEL_SE_TITLE_1);

		PlaySound(SOUND_LABEL_SE_TITLE_0);
	}

	if (g_bModeChange == false)
	{
		if ((GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
			GetButtonTrigger(2) == true) && g_nCntTitleTimer < 45)
		{// タイトルスキップ
			if (g_bTitleSound == false)
			{
				PlaySound(SOUND_LABEL_SE_TITLE_1);

				PlaySound(SOUND_LABEL_BGM_TITLE);
				g_bTitleSound = true;
			}

			// マップ生成
			if (g_bSet == false && g_nCntView == 0)
			{// マップが生成されていない場合
				// カメラ
				SetCamera(D3DXVECTOR3(0.0f, 650.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_TITLE);

				// メッシュフィールド
				SetMeshField(0, 1);
				SetMeshField(1, 0);
				SetMeshField(2, 1);
				SetMeshField(3, 0);
				SetMeshField(4, 0);
				SetMeshField(5, 0);
				SetMeshField(6, 0);
				SetMeshField(7, 1);
				SetMeshField(8, 0);

				// メッシュシリンダー
				SetMeshCylinder(0);
				SetMeshCylinder(1);

				// オブジェクト
				SetXObject(TEXTTYPE_TREE_0);
				SetXObject(TEXTTYPE_TREE_1);
				SetXObject(TEXTTYPE_TREE_2);
				SetXObject(TEXTTYPE_TREE_3);
				SetXObject(TEXTTYPE_TREE_4);
				SetXObject(TEXTTYPE_TREE_5);
				SetXObject(TEXTTYPE_TREE_6);
				SetXObject(TEXTTYPE_TREE_7);
				SetXObject(TEXTTYPE_TREE_8);
			}

			g_bModeChange = true;	// モード変更
			g_bSet = true;			// セット

			// タイトル表示
			if (g_nCntView == 0)
			{
				DeleteWindow(0);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, WINDOWTEXTURE_TITLE, SETTEXTURE_TOPFRONT);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 120, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_SELECT_1, SETTEXTURE_TOPFRONT);
			}
		}
		else if (g_nCntTitleTimer == 45)
		{// モード切替を可能に
			g_bModeChange = true;

			if (g_bTitleSound == false)
			{
				PlaySound(SOUND_LABEL_SE_TITLE_1);

				PlaySound(SOUND_LABEL_BGM_TITLE);
				g_bTitleSound = true;
			}

			// マップ生成
			if (g_bSet == false && g_nCntView == 0)
			{// マップが生成されていない場合
				// カメラ
				SetCamera(D3DXVECTOR3(0.0f, 650.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_TITLE);

				// メッシュフィールド
				SetMeshField(0, 1);
				SetMeshField(1, 0);
				SetMeshField(2, 1);
				SetMeshField(3, 0);
				SetMeshField(4, 0);
				SetMeshField(5, 0);
				SetMeshField(6, 0);
				SetMeshField(7, 1);
				SetMeshField(8, 0);

				// メッシュシリンダー
				SetMeshCylinder(0);
				SetMeshCylinder(1);

				// オブジェクト
				SetXObject(TEXTTYPE_TREE_0);
				SetXObject(TEXTTYPE_TREE_1);
				SetXObject(TEXTTYPE_TREE_2);
				SetXObject(TEXTTYPE_TREE_3);
				SetXObject(TEXTTYPE_TREE_4);
				SetXObject(TEXTTYPE_TREE_5);
				SetXObject(TEXTTYPE_TREE_6);
				SetXObject(TEXTTYPE_TREE_7);
				SetXObject(TEXTTYPE_TREE_8);
			}
		}
	}
}

//=========================================================================================================================
// タイトルの描画処理
//=========================================================================================================================
void DrawTitle(void)
{
	// カメラ設定
	SetUpCamera();

	// フィールド描画処理
	DrawMeshField();

	// シリンダー描画処理
	DrawMeshCylinder();

	// オブジェクト描画処理
	DrawXObject();

	// 壁描画処理
	DrawMeshWall();

	// 影描画処理
	DrawShadow();

	// 敵描画処理
	DrawXEnemy();

	// プレイヤー描画処理
	DrawXPlayer();

	// ビルボードオブジェクト描画処理
	DrawBillboardObject();
}

//=========================================================================================================================
// タイトルのサウンド処理
//=========================================================================================================================
void SetTitleSound(bool bSound)
{
	g_bTitleSound = bSound;
}

//=========================================================================================================================
// タイトルのサウンド処理
//=========================================================================================================================
int GetViewNum(void)
{
	return g_nCntView;
}