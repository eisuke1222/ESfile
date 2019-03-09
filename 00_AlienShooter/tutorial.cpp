//=========================================================================================================================
// 
// チュートリアル処理 [tutorial.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "tutorial.h"		// チュートリアル
#include "sound.h"			// サウンド
#include "fade.h"			// フェード
#include "select.h"			// 選択処理
#include "input.h"			// 入力処理

#include "player.h"			// プレイヤー
#include "enemy.h"			// 敵
#include "bullet.h"			// 弾
#include "explosion.h"		// 爆発
#include "effect.h"			// エフェクト
#include "item.h"			// アイテム
#include "pause.h"			// ポーズ

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define TUTORIAL_TEXTURE_NAME_1		"data\\TEXTURE\\tutorial004.png"	// 読み込むテクスチャファイル
#define TUTORIAL_POS_X				(0)									// チュートリアルの左上X座標
#define TUTORIAL_POS_Y				(0)									// チュートリアルの左上Y座標
#define TUTORIAL_WIDTH				(SCREEN_WIDTH)						// チュートリアルの幅
#define TUTORIAL_HEIGHT				(SCREEN_HEIGHT)						// チュートリアルの高
#define TUTORIAL_TEXTURE_UV_U		(1.0f)								// テクスチャアニメーションU範囲
#define TUTORIAL_TEXTURE_UV_V		(1.0f)								// テクスチャアニメーションV範囲

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureTutorial = NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTutorial = NULL;					// 頂点バッファへのポインタ
D3DXVECTOR3					g_posTutorial;								// チュートリアルの位置
bool						g_bTutorialPause;							// ポーズ

//=========================================================================================================================
// チュートリアル初期化処理
//=========================================================================================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	g_posTutorial = D3DXVECTOR3((TUTORIAL_POS_X + TUTORIAL_WIDTH) / 2, (TUTORIAL_POS_Y + TUTORIAL_HEIGHT) / 2, 0.0f);		// ポリゴンの中心値
	g_bTutorialPause = false;					// ポーズ初期設定

	// テクスチャの読み込み
	D3DXCreateTextureFromFile
	(// チュートリアル
		pDevice,
		TUTORIAL_TEXTURE_NAME_1,
		&g_pTextureTutorial
	);

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL
	);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	g_pVtxBuffTutorial->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// 頂点設定
	pVtx[0].pos = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIAL_WIDTH, TUTORIAL_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIAL_WIDTH, TUTORIAL_HEIGHT, 0.0f);
	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラー設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(TUTORIAL_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, TUTORIAL_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(TUTORIAL_TEXTURE_UV_U, TUTORIAL_TEXTURE_UV_V);

	// 頂点バッファをアンロック
	g_pVtxBuffTutorial->Unlock();

	// 敵の初期化処理
	InitEnemy();
	
	// プレイヤー初期化処理
	InitPlayer();

	// アイテム初期化処理
	InitItem();

	// 爆発の初期化処理
	InitExplosion();

	// エフェクトの初期化処理
	InitEffect();

	// 弾の初期化処理
	InitBullet();

	// ポーズの初期化処理
	InitPause();

	SetEnterSelect(D3DXVECTOR3(SCREEN_WIDTH - 105, SCREEN_HEIGHT - 40, 0.0f), D3DXVECTOR3(100, 40, 0.0f));
}

//=========================================================================================================================
// チュートリアル終了処理
//=========================================================================================================================
void UninitTutorial(void)
{
	// テクスチャの破棄
	if (g_pTextureTutorial != NULL)
	{// NULL以外の場合
		g_pTextureTutorial->Release();		// 開放
		g_pTextureTutorial = NULL;			// NULLへ
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{// NULL以外の場合
		g_pVtxBuffTutorial->Release();		// 開放
		g_pVtxBuffTutorial = NULL;			// NULLへ
	}

	// 敵の終了処理
	UninitEnemy();

	// プレイヤーの終了処理
	UninitPlayer();

	// アイテム終了処理
	UninitItem();

	// 爆発の終了処理
	UninitExplosion();

	// エフェクトの終了処理
	UninitEffect();

	// 弾の終了処理
	UninitBullet();

	// ポーズの終了処理
	UninitPause();
}

//=========================================================================================================================
// チュートリアル更新処理
//=========================================================================================================================
void UpdateTutorial(void)
{
	// 任意のキー（ENTERキー）を押した場合
	FADE Fade = GetFade();		// フェードを取得

	if (GetKeyboardTrigger(DIK_P) == true && Fade == FADE_NONE)
	{// ポーズを切替
		g_bTutorialPause = g_bTutorialPause ? false : true;
		PlaySound(SOUND_LABEL_SE_PAUSE);
	}

	if (g_bTutorialPause == false)
	{// ポーズを使用していない場合
		// 敵の更新処理
		UpdateEnemy();

		// プレイヤーの更新処理
		UpdatePlayer();

		// アイテム更新処理
		UpdateItem();

		// 爆発の更新処理
		UpdateExplosion();

		// エフェクトの更新処理
		UpdateEffect();

		// 弾の更新処理
		UpdateBullet();

		//ポーズ初期状態へ
		SetPause();

		Enemy *pEnemy = GetEnemy();
		int nNumEnemy = GetNumEnemy();	// 敵の総数を取得

		if (nNumEnemy == 0 && pEnemy->bUse == false)
		{
			SetEnemy(D3DXVECTOR3(-50.0f, 100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 50, ENEMYTYPE_TUTORIALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-250.0f, 200.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 50, ENEMYTYPE_TUTORIALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-450.0f, 300.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50) * 1.0f, 300.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 50, ENEMYTYPE_TUTORIALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}

		Player *pPlayer = GetPlayer();
		if (pPlayer->bDisp == false)
		{
			static int nCount = 0;
			nCount++;
			if (nCount > 120)
			{
				SetPlayer();		// 復活後ダメージ受けるところでエラー
				nCount = 0;
			}
		}

		if (Fade == FADE_NONE)
		{// フェードを使用していない場合
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{// 画面切り替え
				ChangeSelect(SELECTSTATE_ENTER);
				PlaySound(SOUND_LABEL_SE_DECIDE);		// 決定音
				SetFade(MODE_GAME);						// ゲーム
			}
		}
	}

	if (g_bTutorialPause == true)
	{// ポーズが使用されている場合
		// ポーズの更新処理
		UpdatePause();
	}
}

//=========================================================================================================================
// チュートリアル描画処理
//=========================================================================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffTutorial,
		0,
		sizeof(VERTEX_2D)
	);

	// 頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャ設定
	pDevice->SetTexture(0, g_pTextureTutorial);

	// チュートリアルの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// 敵の描画処理
	DrawEnemy();

	// プレイヤーの描画
	DrawPlayer();

	// アイテム描画処理
	DrawItem();

	// 爆発の描画処理
	DrawExplosion();

	// エフェクトの描画処理
	DrawEffect();

	// 弾の描画処理
	DrawBullet();

	FADE Fade = GetFade();
	if (g_bTutorialPause == true && Fade == FADE_NONE)
	{// ポーズが使用されている場合
	 // ポーズの描画処理
		DrawPause();
	}
}

//=========================================================================================================================
// ポーズ設定処理
//=========================================================================================================================
void SetTutorialPauseState(bool bTutorialPause)
{
	g_bTutorialPause = bTutorialPause;				// 状態を記憶
}