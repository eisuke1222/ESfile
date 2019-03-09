//=============================================================================
//
// 選択画面 [select.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "select.h"
#include "main.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
#include "gamefade.h"
#include "BG.h"
#include "flontpolygon.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_TEX			(5)		//テクスチャの最大数
#define	MAX_POLYGON		(5)		//ポリゴンの最大数
#define MAX_QUEST		(3)		//クエストの最大数

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureSelect[MAX_TEX] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;		//頂点バッファへのポインタ
QUESTTYPE				g_QuestType = QUESTTYPE_MAX;	//クエストの状態
int						g_nSelectType;					//選択されているタイプ
Select					g_Select[MAX_POLYGON];			//情報の最大数
bool					g_bTriggerStickSelect;

//=============================================================================
// 初期化処理
//=============================================================================
void InitSelect(void)
{
	//変数宣言
	int		nCntTex;
	int		nCntSelect;
	char	cTexName[MAX_TEX][48];		//ファイル名を格納する

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数の初期化
	for (nCntSelect = 0; nCntSelect < MAX_POLYGON; nCntSelect++)
	{
		g_Select[nCntSelect].nTex = nCntSelect;

		if (nCntSelect == 0)
		{
			g_Select[nCntSelect].mode = BOTTUNMODE_SELECT;
		}
		else
		{
			g_Select[nCntSelect].mode = BOTTUNMODE_NOMAL;
		}
	}
	g_nSelectType = QUESTTYPE_TUTORIAL;		//チュートリアルに設定する
	g_bTriggerStickSelect = false;

	//テクスチャファイル名を格納する
	wsprintf(&cTexName[0][0], "data\\TEXTURE\\botunn_tutorial.png");		//チュートリアルボタン
	wsprintf(&cTexName[1][0], "data\\TEXTURE\\misstion00.png");				//クエスト１ボタン
	wsprintf(&cTexName[2][0], "data\\TEXTURE\\misstion01.png");				//クエスト2ボタン
	wsprintf(&cTexName[3][0], "data\\TEXTURE\\misstion02.png");				//クエスト3ボタン
	wsprintf(&cTexName[4][0], "data\\TEXTURE\\bottun_title.png");			//「タイトルへ」ボタン

	//テクスチャの読み込み
	for (nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
								  &cTexName[nCntTex][0],
								  &g_pTextureSelect[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelect,
		NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSelect = 0; nCntSelect < MAX_POLYGON - 1; nCntSelect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(500.0f, 150.0f + (nCntSelect * 130.0f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(800.0f, 150.0f + (nCntSelect * 130.0f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(500.0f, 220.0f + (nCntSelect * 130.0f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(800.0f, 220.0f + (nCntSelect * 130.0f), 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//該当の位置まで進める
		pVtx += 4;
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(1000.0f, 650.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1250.0f, 650.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(1000.0f, 700.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1250.0f, 700.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();

	//背景
	SetBG(1);

	QUESTCLEAR *pClearQuest = GetbClearQuest();

	for (int nCntQuest = 0; nCntQuest < MAX_QUEST; nCntQuest++)
	{
		if (pClearQuest[nCntQuest].bClear == true)
		{
			if (pClearQuest[nCntQuest].nScore > 45000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_A, D3DXVECTOR3(855.0f, 315.0f + (nCntQuest * 130.0f), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 100.0f);
			}
			else if (pClearQuest[nCntQuest].nScore > 30000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_B, D3DXVECTOR3(855.0f, 315.0f + (nCntQuest * 130.0f), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 100.0f);
			}
			else if (pClearQuest[nCntQuest].nScore > 10000)
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_C, D3DXVECTOR3(855.0f, 315.0f + (nCntQuest * 130.0f), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 100.0f);
			}
			else
			{
				SetFlontPolygon(FLONTPOLYGONTEXTURE_QUESTSTAMP_D, D3DXVECTOR3(855.0f, 315.0f + (nCntQuest * 130.0f), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 100.0f);
			}
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSelect(void)
{
	//変数宣言
	int nCntTex;

	//テクスチャの破棄
	for (nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		if (g_pTextureSelect[nCntTex] != NULL)
		{
			g_pTextureSelect[nCntTex]->Release();
			g_pTextureSelect[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSelect(void)
{
	//変数宣言
	int nCntSelect;

	DIJOYSTATE pStick = GetStick();

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//ゲームフェード情報を取得する
	GAMEFADE GameFade;
	GameFade = GetGameFade();

	//フェード情報を取得する
	FADE fade;
	fade = GetFade();

	//カーソル処理
	if (fade == FADE_NONE && GameFade == GAMEFADE_NONE)
	{
		if ((GetKeyboardTrigger(DIK_UP) == true || GetKeyboardTrigger(DIK_W) == true || ((pStick.rgdwPOV[0] == 0 || pStick.lY < -100) && g_bTriggerStickSelect == false)))
		{
			PlaySound(SOUND_LABEL_SE_SELECT);	//サウンド再生
			g_nSelectType = (g_nSelectType + (QUESTTYPE_MAX - 1)) % QUESTTYPE_MAX;

			g_bTriggerStickSelect = true;
		}
		if ((GetKeyboardTrigger(DIK_DOWN) == true || GetKeyboardTrigger(DIK_S) == true || ((pStick.rgdwPOV[0] == 18000 || pStick.lY > 100) && g_bTriggerStickSelect == false)))
		{
			PlaySound(SOUND_LABEL_SE_SELECT);	//サウンド再生
			g_nSelectType = (g_nSelectType + 1) % QUESTTYPE_MAX;

			g_bTriggerStickSelect = true;
		}
	}

	if ((pStick.rgdwPOV[0] != 0 && pStick.rgdwPOV[0] != 18000) && (pStick.lY > -100 && pStick.lY < 100))
	{
		g_bTriggerStickSelect = false;
	}

	for (nCntSelect = 0; nCntSelect < MAX_POLYGON; nCntSelect++)
	{
		if (g_Select[nCntSelect].nTex == g_nSelectType)
		{
			g_Select[nCntSelect].mode = BOTTUNMODE_SELECT;
		}
		else
		{
			g_Select[nCntSelect].mode = BOTTUNMODE_NOMAL;
		}
	}

	//選択状態処理
	switch (g_nSelectType)
	{
	case QUESTTYPE_TUTORIAL:	//チュートリアル

		if (GameFade != GAMEFADE_OUT)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
			{//エンターキーを押す
				PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
				//SetQuestType(QUESTTYPE_TUTORIAL);
				SetShowTutorial(true);
			}
		}
		break;

	case QUESTTYPE_FIRST:		//クエスト１
		if (GameFade != GAMEFADE_OUT)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
			{//エンターキーを押す
				PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
				SetQuestType(QUESTTYPE_FIRST);
			}
		}
		break;

	case QUESTTYPE_SECOND:		//クエスト2
		if (GameFade != GAMEFADE_OUT)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
			{//エンターキーを押す
				PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
				SetQuestType(QUESTTYPE_SECOND);
			}
		}
		break;

	case QUESTTYPE_THIRD:		//クエスト3
		if (GameFade != GAMEFADE_OUT)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
			{//エンターキーを押す
				PlaySound(SOUND_LABEL_SE_DECIDE);	//決定音
				SetQuestType(QUESTTYPE_THIRD);
			}
		}
		break;

	case QUESTTYPE_TITLE:		//タイトル
								//フェードモード設定
		if (fade != FADE_OUT)
		{
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
			{//エンターキーを押す
				PlaySound(SOUND_LABEL_SE_DECIDE);		//決定音
				SetQuestType(QUESTTYPE_TITLE);
				StopSound(SOUND_LABEL_BGM_TUTORIAL);	//サウンド停止
			}
		}
		break;
	}

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSelect = 0; nCntSelect < MAX_POLYGON; nCntSelect++)
	{
		switch (g_Select[nCntSelect].mode)
		{
		case BOTTUNMODE_NOMAL:

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			break;

		case BOTTUNMODE_SELECT:

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}

		//該当の位置まで進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSelect->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSelect(void)
{
	//変数宣言
	int nCntSelect;

	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntSelect = 0; nCntSelect < MAX_POLYGON; nCntSelect++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSelect[g_Select[nCntSelect].nTex]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
							   nCntSelect * 4,			//開始する頂点のインデックス
							   2);						//プリミティブの数
	}
}