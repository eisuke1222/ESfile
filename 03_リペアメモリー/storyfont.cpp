//=============================================================================
//
// ストーリーフォント画面 [storyfont.cpp]
// Author : 佐藤安純　Sato_Asumi
//
//=============================================================================
#include "storyfont.h"
#include "input.h"
#include "flontpolygon.h"
#include "questfade.h"
#include "game.h"
#include "effect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_LINE	(15)

//=============================================================================
// グローバル変数
//=============================================================================
LPD3DXFONT			g_pStoryFont[MAX_LINE] = {};	// フォントへのポインタ
StoryFont			g_StoryFont;					//フォントの情報
StoryLine			g_StoryLine[MAX_LINE];			//会話文
int					g_CntStoryLine;					//会話文をカウントする
bool				g_bStory;						//ストーリー状態になっているかどうか
int					g_nGetIndx;						//情報を保存するko

//=============================================================================
// 初期化処理
//=============================================================================
void InitStoryFont(void)
{
	//変数宣言
	int nCntLine;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//変数の初期化
	g_StoryFont.type = TYPESTORYFONT_MAX;
	g_StoryFont.bUse = false;
	g_nGetIndx = 0;

	for (nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{
		g_StoryLine[nCntLine].Line[0] = {};
		g_StoryLine[nCntLine].nTex = -1;
		g_StoryLine[nCntLine].nIdx = 0;
		g_StoryLine[nCntLine].bUse = false;

		//フォントの設定
		D3DXCreateFont(pDevice, 50, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pStoryFont[nCntLine]);
	}
	g_CntStoryLine = 0;
	g_bStory = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitStoryFont(void)
{
	//変数宣言
	int nCntLine;

	// フォントの開放
	for (nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{
		if (g_pStoryFont[nCntLine] != NULL)
		{
			g_pStoryFont[nCntLine]->Release();
			g_pStoryFont[nCntLine] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateStoryFont(void)
{
	//変数宣言
	int nCntLine;

	//フェードの情報を取得する
	QUESTFADE Fade;
	Fade = GetQuestFade();

	QUESTSTATE pQuestState = GetQuestState();
	QUESTTYPE pQuestType = GetQuestType();

	if (g_StoryFont.bUse == false)
	{
		if (Fade != QUESTFADE_OUT)
		{//フェードアウトの状態ではなかったら
			if (pQuestState == QUESTSTATE_MISSTION)
			{
				SetQuestState(QUESTSTATE_SEARCH);
			}
			else if (pQuestState == QUESTSTATE_REPORT)
			{
				SetQuestState(QUESTSTATE_CLEAR);
			}
		}
	}

	//ストーリーの進行処理
	if (g_bStory == true)
	{//ストーリー状態だったら
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetButtonTrigger(2) == true)
		{//エンターキーを押したら
			DeleteFlontPolygon(g_nGetIndx);	//ポリゴンを消す
			g_CntStoryLine++;				//会話文のカウンターを進める

			for (nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
			{
				if (g_StoryLine[g_CntStoryLine].Line[0] != NULL)
				{//文字列がNULLじゃなかったら
					if (g_CntStoryLine == nCntLine)
					{
						g_StoryLine[nCntLine].bUse = true;		//文章を使用している状態にする

						//アイコン画像の設定
						g_StoryLine[nCntLine].nIdx = SetFlontPolygon(g_StoryLine[nCntLine].nTex, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 130.0f);
						g_nGetIndx = g_StoryLine[nCntLine].nIdx;

						if (g_StoryLine[nCntLine].nTex == FLONTPOLYGONTEXTURE_MESSAGE_NPC_0 || g_StoryLine[nCntLine].nTex == FLONTPOLYGONTEXTURE_MESSAGE_NPC_1 || g_StoryLine[nCntLine].nTex == FLONTPOLYGONTEXTURE_MESSAGE_NPC_2)
						{
							if (g_StoryFont.type == TYPESTORYFONT_02 || g_StoryFont.type == TYPESTORYFONT_12 || g_StoryFont.type == TYPESTORYFONT_22)
							{
								if (pQuestType == QUESTTYPE_FIRST)
								{
									SetEmotionEffect(D3DXVECTOR3(-4000.0f, 60.0f, 1900.0f), EFFECTTYPE_RESURRECTIONHAPPY, 10, 30, 10, 1);	//位置： エフェクトタイプ ： 個数 : 半径 ： HP ： テクスチャ番号	/喜び
								}
								else if (pQuestType == QUESTTYPE_SECOND)
								{
									SetEmotionEffect(D3DXVECTOR3(2545.0f, 60.0f, 1950.0f), EFFECTTYPE_RESURRECTIONHAPPY, 10, 30, 10, 1);	//位置： エフェクトタイプ ： 個数 : 半径 ： HP ： テクスチャ番号	/喜び
								}
								else if (pQuestType == QUESTTYPE_THIRD)
								{
									SetEmotionEffect(D3DXVECTOR3(145.0f, 60.0f, -410.0f), EFFECTTYPE_RESURRECTIONHAPPY, 10, 30, 10, 1);	//位置： エフェクトタイプ ： 個数 : 半径 ： HP ： テクスチャ番号	/喜び
								}
							}
							else if (g_StoryFont.type == TYPESTORYFONT_04 || g_StoryFont.type == TYPESTORYFONT_14 || g_StoryFont.type == TYPESTORYFONT_24)
							{
								if (pQuestType == QUESTTYPE_FIRST)
								{
									SetEmotionEffect(D3DXVECTOR3(-4000.0f, 60.0f, 1900.0f), EFFECTTYPE_RESURRECTIONANGRY, 3, 30, 10, 2);	//位置： エフェクトタイプ ： 個数 : 半径 ： HP ： テクスチャ番号　　/怒り
								}
								else if (pQuestType == QUESTTYPE_SECOND)
								{
									SetEmotionEffect(D3DXVECTOR3(2545.0f, 60.0f, 1950.0f), EFFECTTYPE_RESURRECTIONANGRY, 3, 30, 10, 2);	//位置： エフェクトタイプ ： 個数 : 半径 ： HP ： テクスチャ番号　　/怒り
								}
								else if (pQuestType == QUESTTYPE_THIRD)
								{
									SetEmotionEffect(D3DXVECTOR3(145.0f, 60.0f, -410.0f), EFFECTTYPE_RESURRECTIONANGRY, 3, 30, 10, 2);	//位置： エフェクトタイプ ： 個数 : 半径 ： HP ： テクスチャ番号　　/怒り
								}
							}
						}
					}
					else
					{
						g_StoryLine[nCntLine].bUse = false;		//文章を使用していない状態にする
					}
				}
				else
				{
					g_StoryFont.bUse = false;
					g_StoryLine[nCntLine].bUse = false;		//文章を使用していない状態にする
					DeleteAllFlontPolygon();				//ポリゴンをすべて消す
					g_bStory = false;						//ストーリー状態ではなくする
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawStoryFont(void)
{
	//変数宣言
	int nCntLine;
	RECT rect = { 400, 590, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };		//X座標, Y座標, Ｘ軸の描画範囲, Y軸の描画範囲

	// テキスト描画
	if (g_StoryFont.bUse == true)
	{
		for (nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
		{
			if (g_StoryLine[nCntLine].bUse == true)
			{
				g_pStoryFont[nCntLine]->DrawText(NULL, &g_StoryLine[nCntLine].Line[0], -1, &rect, DT_LEFT, D3DXCOLOR(0.2f, 0.1f, 0.015f, 1.0f));
			}
		}
	}
}

//=============================================================================
// 情報設定
//=============================================================================
void SetStoryFont(TYPESTORYFONT type)
{
	//変数宣言
	int nCntLine;

	//変数の初期化
	g_CntStoryLine = 0;

	for (nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{
		g_StoryLine[nCntLine].Line[0] = {};
		g_StoryLine[nCntLine].nIdx = 0;
		g_StoryLine[nCntLine].nTex = -1;
		g_StoryLine[nCntLine].bUse = false;
	}

	if (g_StoryFont.bUse == false)
	{
		g_StoryFont.type = type;
		g_StoryFont.bUse = true;
		g_bStory = true;							//ストーリー状態に設定する

		switch (g_StoryFont.type)
		{
			//==============================================================================
			//クエスト１
			//==============================================================================

		case TYPESTORYFONT_01:	//クエスト１ -依頼シーン-
			wsprintf(&g_StoryLine[0].Line[0], "「すまないがこれを急ぎで\n 直してくれないだろうか？」");
			wsprintf(&g_StoryLine[1].Line[0], "「はい！お任せください！\n　これは、オルゴールですね。」");
			wsprintf(&g_StoryLine[2].Line[0], "「そうなんじゃ、\n 今日久々に聞こうと思ったら」");
			wsprintf(&g_StoryLine[3].Line[0], "「音がならなくなってしまってたんじゃ。」");
			wsprintf(&g_StoryLine[4].Line[0], "「とても、大切な物だから\n　困ってしまってな…」");
			wsprintf(&g_StoryLine[5].Line[0], "「それでここに来たんじゃ。」");
			wsprintf(&g_StoryLine[6].Line[0], "「それは、大変ですね。\n　責任をもって直させて頂きます！」");
			wsprintf(&g_StoryLine[7].Line[0], "「完成次第お届けいたしますね。」");
			wsprintf(&g_StoryLine[8].Line[0], "「頼んだよ。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[6].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[7].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[8].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			break;

		case TYPESTORYFONT_02:	//クエスト１ -報告シーン Aランク-
			wsprintf(&g_StoryLine[0].Line[0], "「おじいさん、お待たせしました！」");
			wsprintf(&g_StoryLine[1].Line[0], "「おぉ、無事に治ったようだね。」");
			wsprintf(&g_StoryLine[2].Line[0], "「はい！\n しっかりと直させていただきました！」");
			wsprintf(&g_StoryLine[3].Line[0], "「音色もばっちりだと思います！」");

			wsprintf(&g_StoryLine[4].Line[0], "「そうじゃ、そうじゃこの音じゃよ！\n　実に懐かしいの～」");
			wsprintf(&g_StoryLine[5].Line[0], "「このオルゴールはな、わしが若いころに\n　ばあさんからもらったものなんじゃ」");
			wsprintf(&g_StoryLine[6].Line[0], "「音楽好きのわしのために\n　このオルゴールを買ってくれてな～」");
			wsprintf(&g_StoryLine[7].Line[0], "「それはもううれしかったの～」");
			wsprintf(&g_StoryLine[8].Line[0], "「そんな思い出が\n　このオルゴールにあったんですね。」");
			wsprintf(&g_StoryLine[9].Line[0], "「ありがとう、修理人さん。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;

			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[6].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[7].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[8].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[9].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			break;

		case TYPESTORYFONT_03:	//クエスト１ -報告シーン B&Cランク-
			wsprintf(&g_StoryLine[0].Line[0], "「完璧には直せませんでした…。」");

			wsprintf(&g_StoryLine[1].Line[0], "「この音色は懐かしいな～」");
			wsprintf(&g_StoryLine[2].Line[0], "「少々音が違う気もするが\n まぁ、仕方ないかの～」");
			wsprintf(&g_StoryLine[3].Line[0], "「実はこのオルゴールはな、」");
			wsprintf(&g_StoryLine[4].Line[0], "「わしが若い頃に\n おばあさんからもらったものなんじゃ。」");
			wsprintf(&g_StoryLine[5].Line[0], "「音楽好きのわしのために\n このオルゴールを買ってくれてな～」");
			wsprintf(&g_StoryLine[6].Line[0], "「それはもう嬉しかったんじゃ。」");
			wsprintf(&g_StoryLine[7].Line[0], "「じゃから昔どおりの音が\n 聴けなくなってしまったのが悲しくてな。」");
			wsprintf(&g_StoryLine[8].Line[0], "「ここなら直してくれると思ったんじゃが、\n 残念だのう…。」");
			wsprintf(&g_StoryLine[9].Line[0], "「すいません…。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;

			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[6].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[7].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[8].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[9].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_04:	//クエスト１ -報告シーン Dランク-
			wsprintf(&g_StoryLine[0].Line[0], "「すごく言いにくいんですが…。」");

			wsprintf(&g_StoryLine[1].Line[0], "「形はそっくりだが、\n 音がまったくならないじゃないか！」");
			wsprintf(&g_StoryLine[2].Line[0], "「どうなっとるんじゃ！！」");
			wsprintf(&g_StoryLine[3].Line[0], "「これは、 わしとおばあさんの\n大切な思い出だったんじゃ！」");
			wsprintf(&g_StoryLine[4].Line[0], "「ここだったら直ると思ったのに残念じゃ！」");
			wsprintf(&g_StoryLine[5].Line[0], "「本当に申し訳ございません…！」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;

			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_05:	//クエスト１ -Aランク-
			wsprintf(&g_StoryLine[0].Line[0], "「そうじゃ、そうじゃこの音じゃよ！\n　実に懐かしいの～」");
			wsprintf(&g_StoryLine[1].Line[0], "「このオルゴールはな、わしが若いころに\n　ばあさんからもらったものなんじゃ」");
			wsprintf(&g_StoryLine[2].Line[0], "「音楽好きのわしのために\n　このオルゴールを買ってくれてな～」");
			wsprintf(&g_StoryLine[3].Line[0], "「それはもううれしかったの～」");
			wsprintf(&g_StoryLine[4].Line[0], "「そんな思い出が\n　このオルゴールにあったんですね。」");
			wsprintf(&g_StoryLine[5].Line[0], "「ありがとう、修理人さん。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			break;

		case TYPESTORYFONT_06:	//クエスト１ -B&Cランク-\n
			wsprintf(&g_StoryLine[0].Line[0], "「この音色は懐かしいな～」");
			wsprintf(&g_StoryLine[1].Line[0], "「少々音が違う気もするが\n まぁ、仕方ないかの～」");
			wsprintf(&g_StoryLine[2].Line[0], "「実はこのオルゴールはな、」");
			wsprintf(&g_StoryLine[3].Line[0], "「わしが若い頃に\n おばあさんからもらったものなんじゃ。」");
			wsprintf(&g_StoryLine[4].Line[0], "「音楽好きのわしのために\n このオルゴールを買ってくれてな～」");
			wsprintf(&g_StoryLine[5].Line[0], "「それはもう嬉しかったんじゃ。」");
			wsprintf(&g_StoryLine[6].Line[0], "「じゃから昔どおりの音が\n 聴けなくなってしまったのが悲しくてな。」");
			wsprintf(&g_StoryLine[7].Line[0], "「ここなら直してくれると思ったんじゃが、\n 残念だのう…。」");
			wsprintf(&g_StoryLine[8].Line[0], "「すいません…。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[6].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[7].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[8].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_07:	//クエスト１ -Dランク-
			wsprintf(&g_StoryLine[0].Line[0], "「形はそっくりだが、\n 音がまったくならないじゃないか！」");
			wsprintf(&g_StoryLine[1].Line[0], "「どうなっとるんじゃ！！」");
			wsprintf(&g_StoryLine[2].Line[0], "「これは、 わしとおばあさんの\n大切な思い出だったんじゃ！」");
			wsprintf(&g_StoryLine[3].Line[0], "「ここだったら直ると思ったのに残念じゃ！」");
			wsprintf(&g_StoryLine[4].Line[0], "「本当に申し訳ございません…！」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_0;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

			//==============================================================================
			//クエスト２
			//==============================================================================

		case TYPESTORYFONT_11:	//クエスト2 -依頼シーン-
			wsprintf(&g_StoryLine[0].Line[0], "「これを直してくれませんか？」");
			wsprintf(&g_StoryLine[1].Line[0], "「はい！、お任せください！\n これは、時計ですね。」");
			wsprintf(&g_StoryLine[2].Line[0], "「そうなんです\n 動かなくなってしまったんです。」");
			wsprintf(&g_StoryLine[3].Line[0], "「亡くなったおじいちゃんからもらった\n 大切な時計なんです。」");
			wsprintf(&g_StoryLine[4].Line[0], "「それは、大変ですね。\n 責任をもって直させて頂きます！」");
			wsprintf(&g_StoryLine[5].Line[0], "「完成次第お届けいたしますね。」");
			wsprintf(&g_StoryLine[6].Line[0], "「お願いします。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[6].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			break;

		case TYPESTORYFONT_12:	//クエスト2 -報告シーン Aランク-
			wsprintf(&g_StoryLine[0].Line[0], "「お待たせしました！」");
			wsprintf(&g_StoryLine[1].Line[0], "「無事に直りましたか？」");
			wsprintf(&g_StoryLine[2].Line[0], "「はい！\n しっかりと直させていただきました！」");

			wsprintf(&g_StoryLine[3].Line[0], "「！！、動く！直ってる！」");
			wsprintf(&g_StoryLine[4].Line[0], "「これは、おじいちゃんから貰った\n 大切な時計なんです。」");
			wsprintf(&g_StoryLine[5].Line[0], "「本当にありがとう、修理人さん！」");
			wsprintf(&g_StoryLine[6].Line[0], "「大切な思い出を直せてよかったです！」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;

			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[6].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_13:	//クエスト2 -報告シーン B&Cランク-
			wsprintf(&g_StoryLine[0].Line[0], "「お待たせしました！」");
			wsprintf(&g_StoryLine[1].Line[0], "「無事に直りましたか？」");
			wsprintf(&g_StoryLine[2].Line[0], "「一応直せたのですが…。」");

			wsprintf(&g_StoryLine[3].Line[0], "「変なところがあるけど、動いてるね…。」");
			wsprintf(&g_StoryLine[4].Line[0], "「ありがとうございます…。」");
			wsprintf(&g_StoryLine[5].Line[0], "「完璧に直せなくすみませんでした…。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;

			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_14:	//クエスト2 -報告シーン Dランク-
			wsprintf(&g_StoryLine[0].Line[0], "「お待たせしました！」");
			wsprintf(&g_StoryLine[1].Line[0], "「無事に直りましたか？」");
			wsprintf(&g_StoryLine[2].Line[0], "「すごく言いにくいんですが…。」");

			wsprintf(&g_StoryLine[3].Line[0], "「まったく動かないじゃん！\n どうしてくれるんだよ！！」");
			wsprintf(&g_StoryLine[4].Line[0], "「おじいちゃんから貰った\n 大切な時計なのに…。」");
			wsprintf(&g_StoryLine[5].Line[0], "「本当に申し訳ございません…。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;

			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_15:	//クエスト2 -Aランク-
			wsprintf(&g_StoryLine[0].Line[0], "「！！,動く！直ってる！」");
			wsprintf(&g_StoryLine[1].Line[0], "「これは、おじいちゃんから貰った\n 大切な時計なんです。」");
			wsprintf(&g_StoryLine[2].Line[0], "「本当にありがとう、修理人さん！」");
			wsprintf(&g_StoryLine[3].Line[0], "「大切な思い出を直せてよかったです！」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_16:	//クエスト2 -B&Cランク-
			wsprintf(&g_StoryLine[0].Line[0], "「変なところがあるけど、動いてるね…。」");
			wsprintf(&g_StoryLine[1].Line[0], "「ありがとうございます…。」");
			wsprintf(&g_StoryLine[2].Line[0], "「完璧に直せなくすみませんでした…。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_17:	//クエスト2 -Dランク-
			wsprintf(&g_StoryLine[0].Line[0], "「まったく動かないじゃん！\n どうしてくれるんだよ！！」");
			wsprintf(&g_StoryLine[1].Line[0], "「おじいちゃんから貰った\n 大切な時計なのに…。」");
			wsprintf(&g_StoryLine[2].Line[0], "「本当に申し訳ございません…。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_1;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;


			//======================================================================================
			//クエスト３
			//================＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

		case TYPESTORYFONT_21:	//クエスト3 -依頼シーン-
			wsprintf(&g_StoryLine[0].Line[0], "「これを直してくれませんか？」");
			wsprintf(&g_StoryLine[1].Line[0], "「はい！、お任せください！\n　これは、ゼンマイ人形ですね。」");
			wsprintf(&g_StoryLine[2].Line[0], "「そうなんです、\n 動かなくなってしまったんです。」");
			wsprintf(&g_StoryLine[3].Line[0], "「引っ越してしまった親友からもらった\n 大切なゼンマイ人形なんです。」");
			wsprintf(&g_StoryLine[4].Line[0], "「それは、大変ですね。\n 責任をもって直させて頂きます！」");
			wsprintf(&g_StoryLine[5].Line[0], "「完成次第お届けいたしますね。」");
			wsprintf(&g_StoryLine[6].Line[0], "「お願いします。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[6].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			break;

		case TYPESTORYFONT_22:	//クエスト１-報告シーン Aランク-
			wsprintf(&g_StoryLine[0].Line[0], "「お待たせしました！」");
			wsprintf(&g_StoryLine[1].Line[0], "「無事に直りましたか？」");
			wsprintf(&g_StoryLine[2].Line[0], "「はい！\n しっかりと直させていただきました！」");

			wsprintf(&g_StoryLine[3].Line[0], "「うわー元通りだー！凄い！」");
			wsprintf(&g_StoryLine[4].Line[0], "「ありがとうございました！」");
			wsprintf(&g_StoryLine[5].Line[0], "「喜んでいただけて良かったです！」");
			wsprintf(&g_StoryLine[6].Line[0], "「直したいものがあればまた、\n 当店へお越しください！」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;

			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[6].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_23:	//クエスト１-報告シーン B&Cランク-
			wsprintf(&g_StoryLine[0].Line[0], "「お待たせしました！」");
			wsprintf(&g_StoryLine[1].Line[0], "「無事に直りましたか？」");
			wsprintf(&g_StoryLine[2].Line[0], "「一応直せたのですが…。」");

			wsprintf(&g_StoryLine[3].Line[0], "「動くけど、なんか変…。」");
			wsprintf(&g_StoryLine[4].Line[0], "「ありがとうございました…。」");
			wsprintf(&g_StoryLine[5].Line[0], "「完璧に直せなくすみません…。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;

			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_24:	//クエスト１-報告シーン Dランク-
			wsprintf(&g_StoryLine[0].Line[0], "「お待たせしました！」");
			wsprintf(&g_StoryLine[1].Line[0], "「無事に直りましたか?」");
			wsprintf(&g_StoryLine[2].Line[0], "「すいません…最善は尽くしたのですが…」");

			wsprintf(&g_StoryLine[3].Line[0], "「直ってないじゃん！！」");
			wsprintf(&g_StoryLine[4].Line[0], "「親友との大切な思い出だったのに！！」");
			wsprintf(&g_StoryLine[5].Line[0], "「本当に申し訳ございません…。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;

			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[4].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[5].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_25:	//クエスト3 -Aランク-
			wsprintf(&g_StoryLine[0].Line[0], "「うわー元通りだー！凄い！」");
			wsprintf(&g_StoryLine[1].Line[0], "「ありがとうございました！」");
			wsprintf(&g_StoryLine[2].Line[0], "「喜んでいただけて良かったです！」");
			wsprintf(&g_StoryLine[3].Line[0], "「直したいものがあればまた、\n 当店へお越しください！」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			g_StoryLine[3].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_26:	//クエスト3 -B&Cランク-
			wsprintf(&g_StoryLine[0].Line[0], "「動くけど、なんか変…。」");
			wsprintf(&g_StoryLine[1].Line[0], "「ありがとうございました…。」");
			wsprintf(&g_StoryLine[2].Line[0], "「完璧に直せなくすみません…。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;

		case TYPESTORYFONT_27:	//クエスト3 -Dランク-
			wsprintf(&g_StoryLine[0].Line[0], "「直ってないじゃん！！」");
			wsprintf(&g_StoryLine[1].Line[0], "「親友との大切な思い出だったのに！！」");
			wsprintf(&g_StoryLine[2].Line[0], "「本当に申し訳ございません…。」");

			//テクスチャの設定
			g_StoryLine[0].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[1].nTex = FLONTPOLYGONTEXTURE_MESSAGE_NPC_2;
			g_StoryLine[2].nTex = FLONTPOLYGONTEXTURE_MESSAGE_PLAYER;
			break;
		}

		//UIの設定
		SetFlontPolygon(FLONTPOLYGONTEXTURE_FONT_BG, D3DXVECTOR3(640.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH, 180.0f);

		//表示状態の設定
		for (nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
		{
			if (nCntLine == 0)
			{
				g_StoryLine[nCntLine].bUse = true;

				//画像の設定
				g_StoryLine[nCntLine].nIdx = SetFlontPolygon(g_StoryLine[nCntLine].nTex, D3DXVECTOR3(250.0f, 630.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100.0f, 130.0f);
				g_nGetIndx = g_StoryLine[nCntLine].nIdx;
			}
			else
			{
				g_StoryLine[nCntLine].bUse = false;
			}
		}
	}
}

//=============================================================================
// ストリー状態の取得
//=============================================================================
bool GetStoryFont(void)
{
	return g_bStory;
}

//=============================================================================
// ストリー破棄
//=============================================================================
void DeleteStoryFont(void)
{
	//変数宣言
	int nCntLine;

	//変数の初期化
	g_StoryFont.type = TYPESTORYFONT_MAX;
	g_StoryFont.bUse = false;
	g_nGetIndx = 0;

	for (nCntLine = 0; nCntLine < MAX_LINE; nCntLine++)
	{
		if (g_StoryLine[nCntLine].bUse == true)
		{
			g_StoryLine[nCntLine].Line[0] = {};
			g_StoryLine[nCntLine].nTex = -1;
			g_StoryLine[nCntLine].nIdx = 0;
			g_StoryLine[nCntLine].bUse = false;
		}
	}
	g_CntStoryLine = 0;
	g_bStory = false;
}