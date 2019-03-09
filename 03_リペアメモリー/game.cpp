//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "game.h"
#include "main.h"
#include "input.h"
#include "camera.h"
#include "sound.h"
#include "fade.h"
#include "gamefade.h"
#include "questfade.h"
#include "meshcylinder.h"
#include "meshwall.h"
#include "select.h"
#include "meshfield.h"
#include "shadow.h"
#include "xplayer.h"
#include "xenemy.h"
#include "xobject.h"
#include "effect.h"
#include "item.h"
#include "pause.h"
#include "start.h"
#include "itemCnt.h"
#include "mark.h"
#include "flontpolygon.h"
#include "itemfont.h"
#include "storyfont.h"
#include "Bottun.h"
#include "time.h"
#include "score.h"
#include "questclear.h"
#include "BG.h"
#include "light.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_QUEST	(3)		//クエストの最大数

//=============================================================================
// グローバル変数
//=============================================================================
GAMESTATE	g_gameState = GAMESTATE_NONE;		//ゲームの状態
QUESTTYPE	g_questType = QUESTTYPE_MAX;		//クエストのタイプ
QUESTSTATE	g_QuesutState = QUESTSTATE_NONE;	//クエストの状態
int			g_nCounterGameState;				//状態管理カウンター
int			g_nCounterQuest;					//クエストの状態管理カウンター
int			g_nCounterGame;						//エネミー出現のカウンター
bool		g_bPause;							//ポーズを使用しているかどうか
bool		g_bWireframe;						//ワイヤーフレームをしているかどうか
QUESTCLEAR	g_bClearQuest[MAX_QUEST];			//クエストをクリアしたかどうか
bool		g_bAllClear;
bool		g_Tutorial;
int			g_nCntTutorial;

//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(void)
{
	//変数の初期化
	g_gameState = GAMESTATE_QUESTSELECT;	//ゲームの状態
	g_nCounterGameState = 0;				//状態管理カウンター
	g_nCounterGame = 0;						//カウンター
	g_bPause = false;						//ポーズ初期化

	for (int nCnt = 0; nCnt < MAX_QUEST; nCnt++)
	{
		g_bClearQuest[nCnt].nScore = 0;					//クエストをクリア時スコア
		g_bClearQuest[nCnt].bClear = false;				//クエストをクリアしているかどうか
	}
	g_bAllClear = false;
	g_Tutorial = false;
	g_nCntTutorial = 0;

	//関数呼び出し
	InitCamera();					//カメラ
	InitPause();					//ポーズ処理
	InitMeshCylinder();				//空
	InitMeshWall();					//壁
	InitMeshField();				//地面
	InitShadow();					//影
	InitXPlayer();					//プレイヤー
	InitXEnemy();					//住民
	InitXObject();					//オブジェクト
	InitItem();						//アイテム
	InitEffect();					//エフェクト
	InitStart();					//スタートボタン
	InitItemCnt();					//アイテムカウンター
	InitMark();						//マーク
	InitFlontPolygon();				//フロントポリゴン
	InitItemFont();					//ゲームフォント
	InitStoryFont();				//ストーリーフォント
	InitBottun();					//ボタン
	InitTime();						//時間
	InitScore();					//スコア
	InitGameFade(g_gameState);		//ゲームフェード
	InitQuestFade(g_QuesutState);	//クエストフェード

	//画面のモード設定
	SetGameState(g_gameState);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	//関数呼び出し
	UninitCamera();			//カメラ
	UninitPause();			//ポーズ処理
	UninitMeshCylinder();	//空
	UninitMeshWall();		//壁
	UninitMeshField();		//地面
	UninitShadow();			//影
	UninitXPlayer();		//プレイヤー
	UninitXEnemy();			//住民
	UninitXObject();		//オブジェクト
	UninitItem();			//アイテム
	UninitEffect();			//エフェクト
	UninitStart();			//スタートボタン
	UninitItemCnt();		//アイテムカウンター
	UninitMark();			//マーク
	UninitFlontPolygon();	//フロントポリゴン
	UninitItemFont();		//フォント
	UninitStoryFont();		//ストーリーフォント
	UninitBottun();			//ボタン
	UninitSelect();			//クエスト選択画面
	UninitTime();			//タイム
	UninitScore();			//スコア
	UninitGameFade();		//ゲームフェード
	UninitQuestFade();		//クエストフェード
	UninitQuestClear();
	DeleteBG();

	SetLight();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	//フェード情報を取得する
	FADE fade;
	fade = GetFade();

	//クエストフェードの情報を取得する
	QUESTFADE questfade;
	questfade = GetQuestFade();

	if (fade == FADE_NONE && GetKeyboardTrigger(DIK_P) == true || GetButtonTrigger(3) == true)
	{//ポーズON/OFF
		g_bPause = g_bPause ? false : true;

		PlaySound(SOUND_LABEL_SE_PAUSE);
	}

	if (g_bPause == false)
	{
		UpdateCamera();				//カメラ
		UpdateStart();				//スタートボタン
		switch (g_gameState)
		{
		case GAMESTATE_QUESTSELECT:
			if (g_bAllClear == false)
			{
				if (g_Tutorial == false)
				{
					UpdateSelect();
				}
				else
				{
					if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
					{
						if (g_nCntTutorial == 2)
						{
							DeleteFlontPolygonA(FLONTPOLYGONTEXTURE_TUTORIAL00);
							PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
							g_nCntTutorial--;
						}
						else if (g_nCntTutorial == 1)
						{
							DeleteFlontPolygonA(FLONTPOLYGONTEXTURE_TUTORIAL01);
							PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
							g_nCntTutorial--;
						}
						else if (g_nCntTutorial == 0)
						{
							DeleteFlontPolygonA(FLONTPOLYGONTEXTURE_TUTORIAL02);
							PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
							g_Tutorial = false;
						}
					}
				}
			}
			break;

		case GAMESTATE_QUESTPLAY:
			switch (g_QuesutState)
			{
			case QUESTSTATE_MISSTION:
				UpdateMeshCylinder();		//空
				UpdateMeshWall();			//壁
				UpdateMeshField();			//地面
				UpdateShadow();				//影
				UpdateXObject();			//オブジェクト
				UpdateXEnemy();				//住民
				UpdateXPlayer();			//プレイヤー
				SetRainEffect(D3DXVECTOR3(50, 90, 100), EFFECTTYPE_RESURRECTIONWATER, 1, 10, 0, 0);	//位置： エフェクトタイプ ： 個数 : 半径 ： HP ： テクスチャ番号	/噴水
				UpdateEffect();				//エフェクト
				UpdateFlontPolygon();		//フロントポリゴン
				UpdateStoryFont();			//ストーリーフォント
				break;

			case QUESTSTATE_SEARCH:
				UpdateMeshCylinder();		//空
				UpdateMeshWall();			//壁
				UpdateMeshField();			//地面
				UpdateShadow();				//影
				UpdateXObject();			//オブジェクト
				UpdateXEnemy();				//住民
				UpdateXPlayer();			//プレイヤー
				UpdateItem();				//アイテム

				SetRainEffect(D3DXVECTOR3(50, 90, 100), EFFECTTYPE_RESURRECTIONWATER, 1, 10, 0, 0);	//位置： エフェクトタイプ ： 個数 : 半径 ： HP ： テクスチャ番号	/噴水
				UpdateEffect();				//エフェクト
				UpdateMark();				//マーク
				UpdateBottun();				//ボタン
				UpdateFlontPolygon();		//フロントポリゴン
				UpdateItemCnt();			//アイテムカウンター
				UpdateItemFont();			//アイテムフォント
				UpdateTime();				//時間
				UpdateScore();				//スコア
				break;

			case QUESTSTATE_PUZZLE:
				UpdateMeshField();			//地面
				UpdateShadow();				//影
				UpdateXObject();			//オブジェクト
				UpdateEffect();				//エフェクト
				UpdateFlontPolygon();		//フロントポリゴン
				UpdateTime();				//時間
				break;

			case QUESTSTATE_REPORT:
			case QUESTSTATE_CLEAR:
				UpdateMeshCylinder();		//空
				UpdateMeshWall();			//壁
				UpdateMeshField();			//地面
				UpdateShadow();				//影
				UpdateXObject();			//オブジェクト
				UpdateXEnemy();				//住民
				UpdateXPlayer();			//プレイヤー
				UpdateEffect();				//エフェクト
				UpdateFlontPolygon();		//フロントポリゴン
				UpdateStoryFont();			//ストーリーフォント

				if (g_QuesutState == QUESTSTATE_CLEAR)
				{
					UpdateQuestClear();		//クエストクリア画面
					UpdateScore();			//スコア
				}
				break;

			case QUESTSTATE_OVER:
				break;
			}
			break;

	/*	case GAMESTATE_END_CLIA:
			g_nCounterGameState++;	//状態管理カウンターを加算

			if (g_nCounterGameState >= 60)
			{
				g_gameState = GAMESTATE_NONE;

				//フェードモード設定
				if (fade != FADE_OUT)
				{
					SetFade(MODE_GAMECLEAR);
				}
			}
			break;
			*/
		case GAMESTATE_END_OVER:
			//g_nCounterGameState++;	//状態管理カウンターを加算

			//if (g_nCounterGameState > 60)
			{
				g_gameState = GAMESTATE_NONE;

				//フェードモード設定
				if (fade != FADE_OUT)
				{
					SetFade(MODE_GAMEOVER);
				}
			}
			break;
		}


		//ポーズの状態をリセットする
		SetPauseState(PAUSESTATE_CONTINUE);
	}
	else if (fade == FADE_NONE)
	{
		UpdatePause();		//ポーズ処理
	}

		//クエストの状態設定
		UpdateGameFade();			//ゲームフェード
		UpdateQuestFade();			//クエストフェード

	/*//デバッグキー
	if (questfade != QUESTFADE_OUT)
	{
		if (GetKeyboardTrigger(DIK_F7) == true)
		{//クエストクリア状態にする
			SetQuestFade(QUESTSTATE_CLEAR);
		}
	}
	if (fade != FADE_OUT)
	{
		if (GetKeyboardTrigger(DIK_F8) == true)
		{//ゲームクリア状態にする
			SetGameFade(GAMESTATE_END_CLIA);
		}
		if (GetKeyboardTrigger(DIK_F9) == true)
		{//ゲームオーバー状態にする
			SetGameFade(GAMESTATE_END_OVER);
		}
	}*/
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	//if (GetKeyboardTrigger(DIK_F3) == true)
	{//ポーズON/OFF
		//g_bWireframe = g_bWireframe ? false : true;
	}

	if (g_bWireframe == true)
	{
		//ワイヤーフレームの表示
		//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}

	SetUpCamera();		//カメラ

	switch (g_gameState)
	{
	case GAMESTATE_QUESTSELECT:
		if (g_bAllClear == false)
		{
			DrawSelect();				//選択画面
			DrawFlontPolygon();			//フロントポリゴン
		}
		break;

	case GAMESTATE_QUESTPLAY:

		switch (g_QuesutState)
		{//クエストの状態設定
		case QUESTSTATE_MISSTION:
			DrawMeshCylinder();	//空
			DrawMeshWall();		//壁
			DrawMeshField();	//地面
			DrawShadow();		//影
			DrawXObject();		//オブジェクト
			DrawXEnemy();		//住民
			DrawXPlayer();		//プレイヤー
			DrawEffect();		//エフェクト
			DrawFlontPolygon();	//フロントポリゴン
			DrawStoryFont();	//ストーリーフォント
			break;

		case QUESTSTATE_SEARCH:
			DrawMeshCylinder();	//空
			DrawMeshWall();		//壁
			DrawMeshField();	//地面
			DrawShadow();		//影
			DrawXObject();		//オブジェクト
			DrawXEnemy();		//住民
			DrawXPlayer();		//プレイヤー
			DrawItem();			//アイテム
			DrawEffect();		//エフェクト
			DrawMark();			//マーク
			DrawFlontPolygon();	//フロントポリゴン
			//DrawBottun();		//ボタン
			DrawScore();		//スコア
			DrawItemCnt();		//アイテムカウンター
			DrawItemFont();		//アイテムフォント
			DrawTime();			//時間
			//DrawScore();		//スコア
			break;

		case QUESTSTATE_PUZZLE:
			DrawMeshField();	//地面
			DrawXObject();		//オブジェクト
			DrawShadow();		//影
			DrawEffect();		//エフェクト
			DrawFlontPolygon();	//フロントポリゴン
			DrawTime();			//時間
			break;

		case QUESTSTATE_REPORT:
		case QUESTSTATE_CLEAR:
			DrawMeshCylinder();	//空
			DrawMeshWall();		//壁
			DrawMeshField();	//地面
			DrawShadow();		//影
			DrawXObject();		//オブジェクト
			DrawXEnemy();		//住民
			DrawXPlayer();		//プレイヤー
			DrawEffect();		//エフェクト
			DrawFlontPolygon();	//フロントポリゴン
			DrawStoryFont();	//ストーリーフォント

			if (g_QuesutState == QUESTSTATE_CLEAR)
			{
				DrawQuestClear();		//クエストクリア画面
				DrawScore();			//スコア
			}
			break;

		case QUESTSTATE_OVER:
			break;
		}

		DrawStart();		//スタートボタン
		break;
	}


	if (g_bWireframe == false)
	{
		//ワイヤーフレームの表示をデフォルトに戻す
		//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	if (g_bPause == true)
	{
		DrawPause();	//ポーズ処理
	}
	DrawQuestFade();	//クエストフェード
	DrawGameFade();		//ゲームフェード
}

//=============================================================================
// ゲーム状態の設定
//=============================================================================
void SetGameState(GAMESTATE state)
{
	//画面を消す
	switch (g_gameState)
	{
	case GAMESTATE_QUESTSELECT:
		StopSound(SOUND_LABEL_BGM_TUTORIAL);	//サウンド停止
		g_QuesutState = QUESTSTATE_NONE;
		UninitSelect();							//クエスト選択画面
		break;

	case GAMESTATE_QUESTPLAY:
		StopSound(SOUND_LABEL_BGM_GAME);		//サウンド停止
		g_gameState = GAMESTATE_NONE;
		g_QuesutState = QUESTSTATE_NONE;
		break;
	}

	if (g_bPause == true)
	{
		DeleteMeshCylinder();				//空を消す
		DeleteMeshWall();					//壁を消す
		DeleteMeshField();					//地面を消す
		DeleteShadow();						//影を消す
		DeleteXObject();					//オブジェクトを消す
		DeleteXEnemy();						//住人を消す
		DeleteXPlayer();					//プレイヤーを消す
		AllDeleteItem();					//アイテムを消す
		DeleteEffect();						//エフェクトを消す
		DeleteMark();						//マークを消す
		DeleteBottun();						//ボタンを消す
		SetBottunType(BOTTUNTYPE_NOMAL);	//ボタンを通常状態に戻す
		DeleteAllFlontPolygon();			//フロントポリゴンをすべて消す
		DeleteItemCnt();					//アイテムカウンタを初期化
		DeleteItemFont();					//アイテムフォントを初期化
		DeleteStoryFont();					//ストーリーフォントを初期化
											//スコア
		SetTimeState(TIMESTATE_STOP);		//時間を止める

		g_Tutorial = false;
		g_nCntTutorial = 0;
		g_bPause = false;
	}

	int nCntClear = 0;

	for (int nCnt = 0; nCnt < MAX_QUEST; nCnt++)
	{
		if (g_bClearQuest[nCnt].bClear == true)
		{
			nCntClear++;
		}
	}

	if (nCntClear == 3)
	{
		g_bAllClear = true;
		SetFade(MODE_GAMECLEAR);
	}
	else
	{
		//画面を生成する
		switch (state)
		{
		case GAMESTATE_QUESTSELECT:
			PlaySound(SOUND_LABEL_BGM_TUTORIAL);	//サウンド再生
			InitSelect();							//クエスト選択画面
			break;

		case GAMESTATE_QUESTPLAY:
			PlaySound(SOUND_LABEL_BGM_GAME);		//サウンド再生
			g_QuesutState = QUESTSTATE_MISSTION;

			switch (g_questType)
			{
			case QUESTTYPE_FIRST:
				SetStoryFont(TYPESTORYFONT_01);		//ストーリーフォント
				break;

			case QUESTTYPE_SECOND:
				SetStoryFont(TYPESTORYFONT_11);		//ストーリーフォント
				break;

			case QUESTTYPE_THIRD:
				SetStoryFont(TYPESTORYFONT_21);		//ストーリーフォント
				break;
			}

			break;
		}
	}

	//代入
	g_gameState = state;
}

//=============================================================================
// クエストタイプの設定
//=============================================================================
void SetQuestType(QUESTTYPE type)
{
	//代入
	g_questType = type;

	switch (g_questType)
	{
	case QUESTTYPE_TUTORIAL:
		SetGameFade(GAMESTATE_QUESTPLAY);	//クエストプレイモードにする
		break;

	case QUESTTYPE_FIRST:
		SetGameFade(GAMESTATE_QUESTPLAY);	//クエストプレイモードにする
		break;

	case QUESTTYPE_SECOND:
		SetGameFade(GAMESTATE_QUESTPLAY);	//クエストプレイモードにする
		break;

	case QUESTTYPE_THIRD:
		SetGameFade(GAMESTATE_QUESTPLAY);	//クエストプレイモードにする
		break;

	case QUESTTYPE_TITLE:
		g_gameState = GAMESTATE_NONE;	//ゲームを使用しない状態に設定
		SetFade(MODE_TITLE);			//タイトルへ設定
		break;
	}
}

//=============================================================================
// クエストの状態設定
//=============================================================================
void SetQuestState(QUESTSTATE state)
{
	int nClearScore = 0;

	int nScore;
	nScore = 45000;
//	nScore = 30000;
//	nScore = 10000;
//	nScore = 5000;

	//画面を消す
	switch (g_QuesutState)
	{
	case QUESTSTATE_NONE:

		DeleteAllFlontPolygon();			//フロントポリゴンをすべて消す

		break;
	case QUESTSTATE_MISSTION:

		SetTimeState(TIMESTATE_START);

	case QUESTSTATE_SEARCH:
	case QUESTSTATE_CLEAR:

		DeleteMeshCylinder();				//空を消す
		DeleteMeshWall();					//壁を消す
		DeleteMeshField();					//地面を消す
		DeleteShadow();						//影を消す
		DeleteXObject();					//オブジェクトを消す
		DeleteXEnemy();						//住人を消す
		DeleteXPlayer();					//プレイヤーを消す
		AllDeleteItem();					//アイテムを消す
		DeleteEffect();						//エフェクトを消す
		DeleteMark();						//マークを消す
		DeleteBottun();						//ボタンを消す
		SetBottunType(BOTTUNTYPE_NOMAL);	//ボタンを通常状態に戻す
		DeleteAllFlontPolygon();			//フロントポリゴンをすべて消す
		DeleteItemCnt();					//アイテムカウンタを初期化
		DeleteItemFont();					//アイテムフォントを初期化
		DeleteStoryFont();					//ストーリーフォントを初期化
											//スコア
		SetTimeState(TIMESTATE_STOP);		//時間を止める

		if (g_QuesutState == QUESTSTATE_CLEAR)
		{
			UninitQuestClear();						//クエストクリア画面
													//スコア

			//クエストのリザルト処理
			SetGameFade(GAMESTATE_QUESTSELECT);		//クエスト選択画面に戻る
		}
		break;

	case QUESTSTATE_PUZZLE:
		DeleteMeshField();					//地面を消す
		DeleteShadow();						//影を消す
		DeleteXObject();					//オブジェクトを消す
		DeleteEffect();						//エフェクトを消す
		DeleteAllFlontPolygon();			//フロントポリゴンをすべて消す
		SetTimeState(TIMESTATE_STOP);		//時間を止める

		nClearScore = GetScore();
		SetQuestClearScore(nClearScore);

		break;

	case QUESTSTATE_OVER:
		SetGameFade(GAMESTATE_END_OVER);		//ゲームオーバー画面に移動する
		break;
	}

	//画面を生成する
	switch (state)
	{
	case QUESTSTATE_NONE:
		break;
	case QUESTSTATE_MISSTION:

		ReSetScore();
		SetTimeState(TIMESTATE_RESET);

	case QUESTSTATE_SEARCH:
	case QUESTSTATE_REPORT:

		SetLight();

		//空
		SetMeshCylinder();

		//壁
		SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		SetMeshWall(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		//地面
		SetMeshField(FIELDTEXT_FIELD_1, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_2, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_3, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_4, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_5, FIELDTEXTURE_PARK);
		SetMeshField(FIELDTEXT_FIELD_6, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_7, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_8, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_9, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_OTHER_1, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_OTHER_2, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_OTHER_3, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_OTHER_4, FIELDTEXTURE_FIELD);
		SetMeshField(FIELDTEXT_FIELD_WALL_1, FIELDTEXTURE_WALLTOP2);
		SetMeshField(FIELDTEXT_FIELD_WALL_2, FIELDTEXTURE_WALLTOP2);
		SetMeshField(FIELDTEXT_FIELD_WALL_3, FIELDTEXTURE_WALLTOP);
		SetMeshField(FIELDTEXT_FIELD_WALL_4, FIELDTEXTURE_WALLTOP);
		SetMeshField(FIELDTEXT_FIELD_HUNSUI, FIELDTEXTURE_HUNSUI);

		//オブジェクト
		SetXObject(TEXTTYPE_SEARCH_HOUSE_0);
		SetXObject(TEXTTYPE_SEARCH_HOUSE_1);
		SetXObject(TEXTTYPE_SEARCH_HOUSE_2);
		SetXObject(TEXTTYPE_SEARCH_HOUSE_3);
		SetXObject(TEXTTYPE_SEARCH_HOUSE_4);
		SetXObject(TEXTTYPE_SEARCH_HOUSE_5);
		SetXObject(TEXTTYPE_SEARCH_HOUSE_6);
		SetXObject(TEXTTYPE_SEARCH_HOUSE_7);
		SetXObject(TEXTTYPE_SEARCH_HOUSE_8);

		switch (g_questType)
		{
		case QUESTTYPE_TUTORIAL:
			break;

		case QUESTTYPE_FIRST:
			//カメラ
			SetCamera(D3DXVECTOR3(-3831.0f, 150.0f, 1571.0f), D3DXVECTOR3(-3943.0f, 50.0f, 1857.0f), D3DXVECTOR3(0.0f, 0.38f, 0.0f), -280.0f, CAMERATYPE_GAME_SEARCH);

			//プレイヤー
			SetXPlayer(D3DXVECTOR3(-3937.0f, 0.0f, 1869.0f), D3DXVECTOR3(0.0f, 2.02f, 0.0f));

			if (state == QUESTSTATE_MISSTION)
			{
				//NPC
				SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(-4000.0f, 0.0f, 1900.0f), D3DXVECTOR3(0.0f, -1.11f, 0.0f), D3DXVECTOR3(0.0f, -1.11f, 0.0f));
			}
			else if (state == QUESTSTATE_SEARCH)
			{
				//NPC
				SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(-4000.0f, 0.0f, 1900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -1.11f, 0.0f));

				//アイテム
				//SetItem(D3DXVECTOR3(-3800.0f, 0.0f, 1900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_00);

				//アイテム
				SetItem(D3DXVECTOR3(-3700.0f, 0.0f, 1900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_00);
				SetItem(D3DXVECTOR3(-900.0f, 0.0f, 2200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_01);
				SetItem(D3DXVECTOR3(-3800.0f, 0.0f, 1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_02);
				SetItem(D3DXVECTOR3(2100.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), ITEMTYPE_GIAR_03);
				SetItem(D3DXVECTOR3(-1980.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), ITEMTYPE_GIAR_00);
				SetItem(D3DXVECTOR3(2600.0f, 0.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_01);
				SetItem(D3DXVECTOR3(-1950.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_02);
				SetItem(D3DXVECTOR3(-2500.0f, 0.0f, -3000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_03);
				SetItem(D3DXVECTOR3(2230.0f, 0.0f, 3880.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_01);
				SetItem(D3DXVECTOR3(-700.0f, 0.0f, -2120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_02);

				SetFlontPolygon(FLONTPOLYGONTEXTURE_PARTS_WIN_CLOCK, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
			}
			else if (state == QUESTSTATE_REPORT)
			{
				//NPC
				SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(-4000.0f, 0.0f, 1900.0f), D3DXVECTOR3(0.0f, -1.11f, 0.0f), D3DXVECTOR3(0.0f, -1.11f, 0.0f));

				g_bClearQuest[0].nScore = rand() % 4500 + nScore;
				SetScore(g_bClearQuest[0].nScore);
				
				if (g_bClearQuest[0].nScore > 45000)
				{
					SetStoryFont(TYPESTORYFONT_02);		//ストーリーフォント
				}
				else if (g_bClearQuest[0].nScore > 10000)
				{
					SetStoryFont(TYPESTORYFONT_03);		//ストーリーフォント
				}
				else
				{
					SetStoryFont(TYPESTORYFONT_04);		//ストーリーフォント
				}
			}

			break;

		case QUESTTYPE_SECOND:
			//カメラ
			SetCamera(D3DXVECTOR3(2252.0f, 150.0f, 1740.0f), D3DXVECTOR3(2446.0f, 50.0f, 1951.0f), D3DXVECTOR3(0.0f, -1.57f, 0.0f), -280.0f, CAMERATYPE_GAME_SEARCH);

			//プレイヤー
			SetXPlayer(D3DXVECTOR3(2445.0f, 0.0f, 1950.0f), D3DXVECTOR3(0.0f, -1.57f, 0.0f));

			if (state == QUESTSTATE_MISSTION)
			{
				//NPC
				SetXEnemy(ENEMYTYPE_4, D3DXVECTOR3(2545.0f, 0.0f, 1950.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f));
			}
			else if (state == QUESTSTATE_SEARCH)
			{
				//NPC
				SetXEnemy(ENEMYTYPE_4, D3DXVECTOR3(2545.0f, 0.0f, 1950.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f));

				//アイテム
				//SetItem(D3DXVECTOR3(2205.0f, 0.0f, 1950.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_00);

				//アイテム
				SetItem(D3DXVECTOR3(-3700.0f, 0.0f, 1900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_00);
				SetItem(D3DXVECTOR3(-900.0f, 0.0f, 2200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_01);
				SetItem(D3DXVECTOR3(-3800.0f, 0.0f, 1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_02);
				SetItem(D3DXVECTOR3(2100.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), ITEMTYPE_GIAR_03);
				SetItem(D3DXVECTOR3(-1980.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), ITEMTYPE_GIAR_00);
				SetItem(D3DXVECTOR3(2600.0f, 0.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_01);
				SetItem(D3DXVECTOR3(-1950.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_02);
				SetItem(D3DXVECTOR3(-2500.0f, 0.0f, -3000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_03);
				SetItem(D3DXVECTOR3(2230.0f, 0.0f, 3880.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_01);
				SetItem(D3DXVECTOR3(-700.0f, 0.0f, -2120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_02);

				SetFlontPolygon(FLONTPOLYGONTEXTURE_PARTS_WIN_MUSICBOX, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
			}
			else if (state == QUESTSTATE_REPORT)
			{
				//NPC
				SetXEnemy(ENEMYTYPE_4, D3DXVECTOR3(2545.0f, 0.0f, 1950.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f), D3DXVECTOR3(0.0f, 1.57f, 0.0f));

				g_bClearQuest[1].nScore = rand() % 4500 + nScore;
				SetScore(g_bClearQuest[1].nScore);

				if (g_bClearQuest[1].nScore > 45000)
				{
					SetStoryFont(TYPESTORYFONT_12);		//ストーリーフォント
				}
				else if (g_bClearQuest[1].nScore > 10000)
				{
					SetStoryFont(TYPESTORYFONT_13);		//ストーリーフォント
				}
				else
				{
					SetStoryFont(TYPESTORYFONT_14);		//ストーリーフォント
				}
			}

			break;

		case QUESTTYPE_THIRD:
			//カメラ
			SetCamera(D3DXVECTOR3(209.7f, 150.0f, -734.4f), D3DXVECTOR3(96.1f, 50.0f, -478.0f), D3DXVECTOR3(0.0f, 0.38f, 0.0f), -280.0f, CAMERATYPE_GAME_SEARCH);

			//プレイヤー
			SetXPlayer(D3DXVECTOR3(95.5f, 0.0f, -478.7f), D3DXVECTOR3(0.0f, -2.50f, 0.0f));

			if (state == QUESTSTATE_MISSTION)
			{
				//NPC
				SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(145.0f, 0.0f, -410.0f), D3DXVECTOR3(0.0f, 0.62f, 0.0f), D3DXVECTOR3(0.0f, 0.62f, 0.0f));
			}
			else if (state == QUESTSTATE_SEARCH)
			{
				//NPC
				SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(145.0f, 0.0f, -410.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.62f, 0.0f));

				//アイテム
				//SetItem(D3DXVECTOR3(-100.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_00);

				//アイテム
				SetItem(D3DXVECTOR3(-3700.0f, 0.0f, 1900.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_00);
				SetItem(D3DXVECTOR3(-900.0f, 0.0f, 2200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_01);
				SetItem(D3DXVECTOR3(-3800.0f, 0.0f, 1480.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_02);
				SetItem(D3DXVECTOR3(2100.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), ITEMTYPE_GIAR_03);
				SetItem(D3DXVECTOR3(-1980.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), ITEMTYPE_GIAR_00);
				SetItem(D3DXVECTOR3(2600.0f, 0.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_01);
				SetItem(D3DXVECTOR3(-1950.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_02);
				SetItem(D3DXVECTOR3(-2500.0f, 0.0f, -3000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_03);
				SetItem(D3DXVECTOR3(2230.0f, 0.0f, 3880.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_01);
				SetItem(D3DXVECTOR3(-700.0f, 0.0f, -2120.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_GIAR_02);

				SetFlontPolygon(FLONTPOLYGONTEXTURE_PARTS_WIN_DOLL, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0, 0);
			}
			else if (state == QUESTSTATE_REPORT)
			{
				//NPC
				SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(145.0f, 0.0f, -410.0f), D3DXVECTOR3(0.0f, 0.62f, 0.0f), D3DXVECTOR3(0.0f, 0.62f, 0.0f));

				g_bClearQuest[2].nScore = rand() % 4500 + nScore;
				SetScore(g_bClearQuest[2].nScore);

				if (g_bClearQuest[2].nScore > 45000)
				{
					SetStoryFont(TYPESTORYFONT_22);		//ストーリーフォント
				}
				else if (g_bClearQuest[2].nScore > 10000)
				{
					SetStoryFont(TYPESTORYFONT_23);		//ストーリーフォント
				}
				else
				{
					SetStoryFont(TYPESTORYFONT_24);		//ストーリーフォント
				}
			}

			break;
		}

		if (state == QUESTSTATE_SEARCH)
		{
			SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTCLEAR, D3DXVECTOR3(SCREEN_WIDTH / 2, 40, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 80);

			SetScore(D3DXVECTOR3(950.0f, 15.0f, 0.0f), 40.0f, 60.0f);

			//ボタン
			SetBottun();

			//数字の線
			SetFlontPolygon(FLONTPOLYGONTEXTURE_NUMBER_SLASH, D3DXVECTOR3(275, 50, 0), D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f), 50.0f, 50.0f);

			SetTimeState(TIMESTATE_START);		//時間を初期化する

			PlaySound(SOUND_LABEL_SE_PAZZLEIN);
		}

		break;

	case QUESTSTATE_PUZZLE:

		SetLight();

		SetCamera(D3DXVECTOR3(0.0f, 900.0f, -350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), -350.0f, CAMERATYPE_GAME_PUZZLE);	//カメラを設定する
		SetTimeState(TIMESTATE_STOP);																			//時間を進める

		//地面
		SetMeshField(FIELDTEXT_FIELD_PAZZLE, FIELDTEXTURE_PAZZLE);

		switch (g_questType)
		{
		case QUESTTYPE_TUTORIAL:
			break;

		case QUESTTYPE_FIRST:

			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_BG, D3DXVECTOR3(SCREEN_WIDTH / 2 + 516, SCREEN_HEIGHT / 2 + 35, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 240, SCREEN_HEIGHT - 70);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 230, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_2, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 230, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_3, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 330, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_4, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 330, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_5, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 550, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_6, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 550, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_MB1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 650, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_MB2, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 650, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);

			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_S0, D3DXVECTOR3(SCREEN_WIDTH / 2 + 516, SCREEN_HEIGHT / 2 - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 240, 60);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_S1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 516, SCREEN_HEIGHT / 2 + 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 240, 60);

			SetXObject(TEXTTYPE_PUZZLE_1);
			SetXObject(TEXTTYPE_PUZZLE_1_A);
			SetXObject(TEXTTYPE_PUZZLE_1_W);
			break;

		case QUESTTYPE_SECOND:

			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_BG, D3DXVECTOR3(SCREEN_WIDTH / 2 + 516, SCREEN_HEIGHT / 2 + 35, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 240, SCREEN_HEIGHT - 70);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 230, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_2, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 230, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_3, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 330, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_4, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 330, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_5, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 550, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_6, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 550, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_7, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 650, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_8, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 650, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);

			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_S0, D3DXVECTOR3(SCREEN_WIDTH / 2 + 516, SCREEN_HEIGHT / 2 - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 240, 60);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_S1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 516, SCREEN_HEIGHT / 2 + 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 240, 60);

			SetXObject(TEXTTYPE_PUZZLE_2);
			SetXObject(TEXTTYPE_PUZZLE_2_A);
			SetXObject(TEXTTYPE_PUZZLE_2_W);
			break;

		case QUESTTYPE_THIRD:

			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_BG, D3DXVECTOR3(SCREEN_WIDTH / 2 + 516, SCREEN_HEIGHT / 2 + 35, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 240, SCREEN_HEIGHT - 70);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 230, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_2, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 230, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_3, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 330, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_4, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 330, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_5, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 550, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_6, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 550, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_7, D3DXVECTOR3(SCREEN_WIDTH / 2 + 466, 650, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_ZENMAI, D3DXVECTOR3(SCREEN_WIDTH / 2 + 566, 650, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80);

			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_S0, D3DXVECTOR3(SCREEN_WIDTH / 2 + 516, SCREEN_HEIGHT / 2 - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 240, 60);
			SetFlontPolygon(FLONTPOLYGONTEXTURE_MENU_S1, D3DXVECTOR3(SCREEN_WIDTH / 2 + 516, SCREEN_HEIGHT / 2 + 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 240, 60);

			SetXObject(TEXTTYPE_PUZZLE_3);
			SetXObject(TEXTTYPE_PUZZLE_3_A);
			SetXObject(TEXTTYPE_PUZZLE_3_W);
			break;
		}

		SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTCLEAR, D3DXVECTOR3(SCREEN_WIDTH / 2, 40, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 80);

		//SetFlontPolygon(FLONTPOLYGONTEXTURE_POINTER, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(0.2f, 0.5f, 1.0f, 1.0f), 30, 30);
		SetFlontPolygon(FLONTPOLYGONTEXTURE_POINTER, D3DXVECTOR3(SCREEN_WIDTH - 120, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(0.2f, 0.5f, 1.0f, 1.0f), 30, 30);

		SetFlontPolygon(FLONTPOLYGONTEXTURE_PAZZLE_START, D3DXVECTOR3(SCREEN_WIDTH + 400, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 700, 300);
		PlaySound(SOUND_LABEL_SE_PAZZLEIN);

		break;

	case QUESTSTATE_CLEAR:
		InitQuestClear();		//クエストクリア画面

		SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTCLEAR, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
		SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2 - 420, SCREEN_HEIGHT / 2 + 110, 0.0f), 80.0f, 120.0f);

		switch (g_questType)
		{
		case QUESTTYPE_FIRST:
			g_bClearQuest[0].bClear = true;
			break;

		case QUESTTYPE_SECOND:
			g_bClearQuest[1].bClear = true;
			break;

		case QUESTTYPE_THIRD:
			g_bClearQuest[2].bClear = true;
			break;
		}
		break;

	case QUESTSTATE_OVER:
		break;
	}

	//代入
	g_QuesutState = state;
}

//=============================================================================
// ゲーム状態の取得
//=============================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//=============================================================================
// クエスト状態の取得
//=============================================================================
QUESTSTATE GetQuestState(void)
{
	return g_QuesutState;
}

//=============================================================================
// クエスト状態の取得
//=============================================================================
QUESTTYPE GetQuestType(void)
{
	return g_questType;
}

//=============================================================================
// ポーズ状態の取得
//=============================================================================
void SetPause(bool bPause)
{
	g_bPause = bPause;
}

//=============================================================================
// ポーズ状態の取得
//=============================================================================
bool GetGAMEPause(void)
{
	return g_bPause;
}

//=============================================================================
// クエストの状態の情報の取得
//=============================================================================
QUESTCLEAR *GetbClearQuest(void)
{
	return &g_bClearQuest[0];
}

//=============================================================================
// クエストのスコア取得
//=============================================================================
void SetQuestClearScore(int nScore)
{
	if (g_questType == QUESTTYPE_FIRST)
	{
		if (g_bClearQuest[0].nScore < nScore)
		{
			g_bClearQuest[0].nScore = nScore;
		}
	}
	else if (g_questType == QUESTTYPE_SECOND)
	{
		if (g_bClearQuest[1].nScore < nScore)
		{
			g_bClearQuest[1].nScore = nScore;
		}
	}
	else if (g_questType == QUESTTYPE_THIRD)
	{
		if (g_bClearQuest[2].nScore < nScore)
		{
			g_bClearQuest[2].nScore = nScore;
		}
	}
}

void SetShowTutorial(bool bTutorial)
{
	if (g_Tutorial == false && bTutorial == true)
	{
		SetFlontPolygon(FLONTPOLYGONTEXTURE_TUTORIAL02, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
		SetFlontPolygon(FLONTPOLYGONTEXTURE_TUTORIAL01, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
		SetFlontPolygon(FLONTPOLYGONTEXTURE_TUTORIAL00, D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

		g_nCntTutorial = 2;
		g_Tutorial = true;
	}

	g_Tutorial = bTutorial;
}