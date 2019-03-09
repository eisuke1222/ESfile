//=============================================================================
//
// ポーズ画面 [pause.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "pause.h"
#include "main.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "fade.h"
#include "gamefade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_POLYGON		(5)		//ポリゴンの最大数
#define MAX_TEX			(4)		//テクスチャの最大数

//台紙
#define STAND_WHIDTH	(450)										//背景の幅
#define STAND_HEIGHT	(500)										//背景の高さ
#define STAND_POS_X		((SCREEN_WIDTH / 2) - (STAND_WHIDTH / 2))	//背景の左上X座標
#define STAND_POS_Y		((SCREEN_HEIGHT / 2) - (STAND_HEIGHT / 2))	//背景の左上Y座標

//コンテニューボタン
#define CONTINUE_WHIDTH		(350)											//背景の幅
#define CONTINUE_HEIGHT		(70)											//背景の高さ
#define CONTINUE_POS_X		((SCREEN_WIDTH / 2) - (CONTINUE_WHIDTH / 2))	//背景の左上X座標
#define CONTINUE_POS_Y		(230)											//背景の左上Y座標

//リトライボタン
#define RETRY_WHIDTH		(250)											//背景の幅
#define RETRY_HEIGHT		(80)											//背景の高さ
#define RETRY_POS_X			((SCREEN_WIDTH / 2) - (RETRY_WHIDTH / 2))		//背景の左上X座標
#define RETRY_POS_Y			(345)											//背景の左上Y座標

//クイットボタン
#define QUIT_WHIDTH			(250)										//背景の幅
#define QUIT_HEIGHT			(80)										//背景の高さ
#define QUIT_POS_X			((SCREEN_WIDTH / 2) - (QUIT_WHIDTH / 2))	//背景の左上X座標
#define QUIT_POS_Y			(460)										//背景の左上Y座標

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePause[MAX_TEX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPause = NULL;			//頂点バッファへのポインタ
int							g_SelectMode;					//選択されているモード
Pause						g_Pause[MAX_POLYGON];			//情報設定
int							g_PauseTime;					//時間カウンター
bool						g_bTriggerStickPause;

//=============================================================================
// 初期化処理
//=============================================================================
void InitPause(void)
{
	//変数宣言
	int		nCntTex;
	char	cTexName[MAX_TEX][48];		//ファイル名を格納する

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数の初期化
	g_Pause[0].nTex = 0;						//テクスチャの設定
	g_Pause[1].nTex = 0;
	g_Pause[2].nTex = 1;
	g_Pause[3].nTex = 2;
	g_Pause[4].nTex = 3;

	g_Pause[0].state = BOTTUNSTATE_NONE;		//ボタンの状態設定
	g_Pause[1].state = BOTTUNSTATE_NONE;
	g_Pause[2].state = BOTTUNSTATE_SELECT;
	g_Pause[3].state = BOTTUNSTATE_NOMAL;
	g_Pause[4].state = BOTTUNSTATE_NOMAL;

	g_SelectMode = PAUSESTATE_CONTINUE;			//洗濯状態の設定
	g_PauseTime = 0;							//時間カウンター
	g_bTriggerStickPause = false;

	//テクスチャファイル名を格納する
	wsprintf(&cTexName[0][0], "data\\TEXTURE\\pause100.png");		//台紙
	wsprintf(&cTexName[1][0], "data\\TEXTURE\\pause000.png");		//コンテニューボタン
	wsprintf(&cTexName[2][0], "data\\TEXTURE\\pause001.png");		//リトライボタン
	wsprintf(&cTexName[3][0], "data\\TEXTURE\\pause002.png");		//クイットボタン

	//テクスチャの読み込み
	for (nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
								  &cTexName[nCntTex][0],
								  &g_pTexturePause[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffPause,
								NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

//背景の黒-----------------------------------------------------------

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

//台紙-----------------------------------------------------------

	//頂点座標の設定
	pVtx[4].pos = D3DXVECTOR3(STAND_POS_X, STAND_POS_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(STAND_POS_X + STAND_WHIDTH, STAND_POS_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(STAND_POS_X, STAND_POS_Y + STAND_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(STAND_POS_X + STAND_WHIDTH, STAND_POS_Y + STAND_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

//コンテニューボタン-----------------------------------------------------------

	//頂点座標の設定
	pVtx[8].pos = D3DXVECTOR3(CONTINUE_POS_X, CONTINUE_POS_Y, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(CONTINUE_POS_X + CONTINUE_WHIDTH, CONTINUE_POS_Y, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(CONTINUE_POS_X, CONTINUE_POS_Y + CONTINUE_HEIGHT, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(CONTINUE_POS_X + CONTINUE_WHIDTH, CONTINUE_POS_Y + CONTINUE_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[9].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[10].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[11].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

//リトライボタン-----------------------------------------------------------

	//頂点座標の設定
	pVtx[12].pos = D3DXVECTOR3(RETRY_POS_X, RETRY_POS_Y, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(RETRY_POS_X + RETRY_WHIDTH, RETRY_POS_Y, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(RETRY_POS_X, RETRY_POS_Y + RETRY_HEIGHT, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(RETRY_POS_X + RETRY_WHIDTH, RETRY_POS_Y + RETRY_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);

//クイットボタン-----------------------------------------------------------

//頂点座標の設定
	pVtx[16].pos = D3DXVECTOR3(QUIT_POS_X, QUIT_POS_Y, 0.0f);
	pVtx[17].pos = D3DXVECTOR3(QUIT_POS_X + QUIT_WHIDTH, QUIT_POS_Y, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(QUIT_POS_X, QUIT_POS_Y + QUIT_HEIGHT, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(QUIT_POS_X + QUIT_WHIDTH, QUIT_POS_Y + QUIT_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[16].rhw = 1.0f;
	pVtx[17].rhw = 1.0f;
	pVtx[18].rhw = 1.0f;
	pVtx[19].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[16].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[17].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[18].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[19].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	//変数宣言
	int nCntTex;

	//テクスチャの破棄
	for (nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		if (g_pTexturePause[nCntTex] != NULL)
		{
			g_pTexturePause[nCntTex]->Release();
			g_pTexturePause[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
	//変数宣言
	int nCntPause;

	DIJOYSTATE pStick = GetStick();

	//フェード情報を取得する
	FADE fade;
	fade = GetFade();

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//カーソル処理
	if ((GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || ((pStick.rgdwPOV[0] == 0 || pStick.lY < -100) && g_bTriggerStickPause == false)))
	{
		PlaySound(SOUND_LABEL_SE_SELECT);	//サウンド再生
		g_SelectMode = (g_SelectMode + (PAUSESTATE_MAX - 1)) % PAUSESTATE_MAX;

		g_bTriggerStickPause = true;
	}
	if ((GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || ((pStick.rgdwPOV[0] == 18000 || pStick.lY > 100) && g_bTriggerStickPause == false)))
	{
		PlaySound(SOUND_LABEL_SE_SELECT);	//サウンド再生
		g_SelectMode = (g_SelectMode + 1) % PAUSESTATE_MAX;

		g_bTriggerStickPause = true;
	}

	if ((pStick.rgdwPOV[0] != 0 && pStick.rgdwPOV[0] != 18000) && (pStick.lY > -100 && pStick.lY < 100))
	{
		g_bTriggerStickPause = false;
	}

	//選択処理
	switch (g_SelectMode)
	{
	case PAUSESTATE_CONTINUE:

		//ボタンの状態設定
		g_Pause[2].state = BOTTUNSTATE_SELECT;
		g_Pause[3].state = BOTTUNSTATE_NOMAL;
		g_Pause[4].state = BOTTUNSTATE_NOMAL;

		//決定キーが押されたかどうか
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
			g_SelectMode = PAUSESTATE_MAX;
			g_Pause[2].state = BOTTUNSTATE_MAX;
			SetPause(false);
			g_Pause[2].state = BOTTUNSTATE_SELECT;
			g_SelectMode = PAUSESTATE_CONTINUE;
		}
		break;

	case PAUSESTATE_RETRY:

		//ボタンの状態設定
		g_Pause[2].state = BOTTUNSTATE_NOMAL;
		g_Pause[3].state = BOTTUNSTATE_SELECT;
		g_Pause[4].state = BOTTUNSTATE_NOMAL;

		//決定キーが押されたかどうか
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
			g_SelectMode = PAUSESTATE_MAX;
			g_Pause[3].state = BOTTUNSTATE_MAX;

			if (fade != FADE_OUT)		//フェードアウトが実行されていないなら
			{
				//フェードモード設定
				SetGameFade(GAMESTATE_QUESTSELECT);
				StopSound(SOUND_LABEL_BGM_GAME);	//サウンド停止
			}
		}
		break;

	case PAUSESTATE_QUIT:

		//ボタンの状態設定
		g_Pause[2].state = BOTTUNSTATE_NOMAL;
		g_Pause[3].state = BOTTUNSTATE_NOMAL;
		g_Pause[4].state = BOTTUNSTATE_SELECT;

		//決定キーが押されたかどうか
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
			g_SelectMode = PAUSESTATE_MAX;
			g_Pause[4].state = BOTTUNSTATE_MAX;

			if (fade != FADE_OUT)		//フェードアウトが実行されていないなら
			{
				//フェードモード設定
				SetFade(MODE_TITLE);
				StopSound(SOUND_LABEL_BGM_GAME);	//サウンド停止
			}
		}
		break;
	}

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < MAX_POLYGON; nCntPause++)
	{
		switch (g_Pause[nCntPause].state)
		{
		case BOTTUNSTATE_NOMAL:

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			break;

		case BOTTUNSTATE_SELECT:

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case BOTTUNSTATE_NONE:

			if (nCntPause == 0)
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f);
			}
			else
			{
				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			break;

		case BOTTUNSTATE_MAX:

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		}

		//該当の位置まで進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
	//変数宣言
	int nCntPause;

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < MAX_POLYGON; nCntPause++)
	{
		//テクスチャの設定
		if (nCntPause == 0)
		{
			pDevice->SetTexture(0, NULL);
		}
		else
		{
			pDevice->SetTexture(0, g_pTexturePause[g_Pause[nCntPause].nTex]);
		}

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							   nCntPause * 4,			//開始する頂点のインデックス
							   2);						//プリミティブの数
	}
}

//=============================================================================
// 描画処理
//=============================================================================
Pause * GetPause(void)
{
	return &g_Pause[0];
}

//=============================================================================
// 状態の設定
//=============================================================================
void SetPauseState(PAUSESTATE state)
{
	g_SelectMode = state;
}