//=========================================================================================================================
// 
// ウィンドウ処理 [window.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _WINDOW_H_
#define _WINDOW_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"
#include "game.h"		// ゲーム

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define WINDOWTEXTURE_NAME_1	"data\\TEXTURE\\title000.png"			// 読み込むテクスチャファイル1	タイトル（タイトル）
#define WINDOWTEXTURE_NAME_2	"data\\TEXTURE\\kouhakuhikari.png"		// 読み込むテクスチャファイル2	タイトル（アニメーション）
#define WINDOWTEXTURE_NAME_3	"data\\TEXTURE\\tutorial001.png"		// 読み込むテクスチャファイル3	ゲーム（チュートリアル）
#define WINDOWTEXTURE_NAME_4	"data\\TEXTURE\\tutorial003.png"		// 読み込むテクスチャファイル4	ゲーム（チュートリアル）
#define WINDOWTEXTURE_NAME_5	"data\\TEXTURE\\tutorial100.png"		// 読み込むテクスチャファイル5	ゲーム（ゲームチュートリアル）
#define WINDOWTEXTURE_NAME_6	"data\\TEXTURE\\press_help000.png"		// 読み込むテクスチャファイル6	ゲーム（チュートリアルコメント開く）
#define WINDOWTEXTURE_NAME_7	"data\\TEXTURE\\press_help001.png"		// 読み込むテクスチャファイル7	ゲーム（チュートリアルコメント閉じる）
#define WINDOWTEXTURE_NAME_8	"data\\TEXTURE\\tutorial_message.png"	// 読み込むテクスチャファイル8	ゲーム（チュートリアルメッセージ）
#define WINDOWTEXTURE_NAME_9	"data\\TEXTURE\\map000.png"				// 読み込むテクスチャファイル9	ゲーム（マップ）
#define WINDOWTEXTURE_NAME_10	"data\\TEXTURE\\map001.png"				// 読み込むテクスチャファイル10	ゲーム（マップ位置）
#define WINDOWTEXTURE_NAME_11	"data\\TEXTURE\\hp001.png"				// 読み込むテクスチャファイル11	ゲーム（HPBG）
#define WINDOWTEXTURE_NAME_12	"data\\TEXTURE\\hp001.png"				// 読み込むテクスチャファイル12	ゲーム（HP）
#define WINDOWTEXTURE_NAME_13	"data\\TEXTURE\\hp002.png"				// 読み込むテクスチャファイル13	ゲーム（HPマーク）
#define WINDOWTEXTURE_NAME_14	"data\\TEXTURE\\weapon000.png"			// 読み込むテクスチャファイル14	ゲーム（武器1）
#define WINDOWTEXTURE_NAME_15	"data\\TEXTURE\\weapon001.png"			// 読み込むテクスチャファイル15	ゲーム（武器2）
#define WINDOWTEXTURE_NAME_16	"data\\TEXTURE\\weapon002.png"			// 読み込むテクスチャファイル16	ゲーム（武器3）
#define WINDOWTEXTURE_NAME_17	"data\\TEXTURE\\weapon100.png"			// 読み込むテクスチャファイル17	ゲーム（武器枠）
#define WINDOWTEXTURE_NAME_18	"data\\TEXTURE\\brack000.png"			// 読み込むテクスチャファイル18	リザルト（背景）
#define WINDOWTEXTURE_NAME_19	"data\\TEXTURE\\gameclear001.png"		// 読み込むテクスチャファイル19	リザルト（クリア）
#define WINDOWTEXTURE_NAME_20	"data\\TEXTURE\\gameover001.png"		// 読み込むテクスチャファイル20	リザルト（オーバー）
#define WINDOWTEXTURE_NAME_21	"data\\TEXTURE\\ToBeContinued.png"		// 読み込むテクスチャファイル21	リザルト（おまけ）
#define WINDOWTEXTURE_NAME_22	"data\\TEXTURE\\ranking000.png"			// 読み込むテクスチャファイル22	ランキング（ランキング）
#define WINDOWTEXTURE_NAME_23	"data\\TEXTURE\\ranking001.png"			// 読み込むテクスチャファイル23	ランキング（1）
#define WINDOWTEXTURE_NAME_24	"data\\TEXTURE\\ranking002.png"			// 読み込むテクスチャファイル24	ランキング（2）
#define WINDOWTEXTURE_NAME_25	"data\\TEXTURE\\ranking003.png"			// 読み込むテクスチャファイル25	ランキング（3）
#define WINDOWTEXTURE_NAME_26	"data\\TEXTURE\\ranking004.png"			// 読み込むテクスチャファイル26 ランキング（4）
#define WINDOWTEXTURE_NAME_27	"data\\TEXTURE\\ranking005.png"			// 読み込むテクスチャファイル27	ランキング（5）
#define WINDOWTEXTURE_NAME_28	"data\\TEXTURE\\rankingmessage000.png"	// 読み込むテクスチャファイル27	ランキング（メッセージA）
#define WINDOWTEXTURE_NAME_29	"data\\TEXTURE\\rankingmessage001.png"	// 読み込むテクスチャファイル28	ランキング（メッセージB）
#define WINDOWTEXTURE_NAME_30	"data\\TEXTURE\\rankingmessage002.png"	// 読み込むテクスチャファイル29	ランキング（メッセージC）
#define WINDOWTEXTURE_NAME_31	"data\\TEXTURE\\rankingmessage003.png"	// 読み込むテクスチャファイル30	ランキング（メッセージD）
#define WINDOWTEXTURE_NAME_32	"data\\TEXTURE\\message000.png"			// 読み込むテクスチャファイル31 メッセージ
#define WINDOWTEXTURE_NAME_33	"data\\TEXTURE\\press_button000.png"	// 読み込むテクスチャファイル32 選択
#define WINDOWTEXTURE_NAME_34	"data\\TEXTURE\\press_button001.png"	// 読み込むテクスチャファイル33 選択

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// テクスチャ種類
	WINDOWTEXTURE_TITLE = 0,			// タイトル
	WINDOWTEXTURE_TITLE_ANIM,			// タイトルアニメーション
	WINDOWTEXTURE_GAME_TUTORIAL,		// ゲーム（チュートリアル）
	WINDOWTEXTURE_GAME_TUTORIAL_P,		// ゲーム（チュートリアル）
	WINDOWTEXTURE_GAME_TUTORIAL_GAME,	// ゲーム（ゲームチュートリアル）
	WINDOWTEXTURE_GAME_TUTORIAL_IN,		// ゲーム（チュートリアル開く）
	WINDOWTEXTURE_GAME_TUTORIAL_OUT,	// ゲーム（チュートリアルコメント閉じる）
	WINDOWTEXTURE_GAME_TUTORIAL_MESSAGE,// ゲーム（チュートリアルメッセージ）
	WINDOWTEXTURE_GAME_MAP,				// ゲーム（マップ）
	WINDOWTEXTURE_GAME_MAP_POS,			// ゲーム（マップ位置）
	WINDOWTEXTURE_GAME_HPBG,			// ゲーム（HPBG）
	WINDOWTEXTURE_GAME_HP,				// ゲーム（HP）
	WINDOWTEXTURE_GAME_HP_M,			// ゲーム（HPマーク）
	WINDOWTEXTURE_GAME_WEAPON_1,		// ゲーム（武器1）
	WINDOWTEXTURE_GAME_WEAPON_2,		// ゲーム（武器2）
	WINDOWTEXTURE_GAME_WEAPON_3,		// ゲーム（武器3）
	WINDOWTEXTURE_GAME_WEAPON_99,		// ゲーム（武器枠）
	WINDOWTEXTURE_RESULT_BG,			// リザルトクリア
	WINDOWTEXTURE_RESULT_CLEAR,			// リザルトクリア
	WINDOWTEXTURE_RESULT_OVER,			// リザルトオーバー
	WINDOWTEXTURE_RESULT_OTHER,			// リザルト
	WINDOWTEXTURE_RANKING_BG,			// ランキング
	WINDOWTEXTURE_RANKING_1,			// ランキング1
	WINDOWTEXTURE_RANKING_2,			// ランキング2
	WINDOWTEXTURE_RANKING_3,			// ランキング3
	WINDOWTEXTURE_RANKING_4,			// ランキング4
	WINDOWTEXTURE_RANKING_5,			// ランキング5
	WINDOWTEXTURE_RANKING_A,			// ランキングメッセージA
	WINDOWTEXTURE_RANKING_B,			// ランキングメッセージB
	WINDOWTEXTURE_RANKING_C,			// ランキングメッセージC
	WINDOWTEXTURE_RANKING_D,			// ランキングメッセージD
	WINDOWTEXTURE_MESSAGE,				// メッセージ
	WINDOWTEXTURE_SELECT_1,				// 選択1
	WINDOWTEXTURE_SELECT_2,				// 選択2
	WINDOWTEXTURE_MAX,
	WINDOW_TUTORIAL,
	WINDOW_FADE,
}WINDOWTEXTURE;

typedef enum
{// 選択状態
	SELECTSTATE_NORMAL = 0,				// ノーマル
	SELECTSTATE_ENTER,					// エンター
	SELECTSTATE_MAX,
}SELECTSTATE;

typedef enum
{
	SETTEXTURE_FRONT = 0,
	SETTEXTURE_TOPFRONT,
	SETTEXTURE_BG,
	SETTEXTURE_TOPBG,
	SETTEXTURE_MAX,
}SETTEXTURE;

typedef struct
{// ウィンドウ状態
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 tex;					// 位置
	D3DXVECTOR3 texUV;					// UV
	D3DXVECTOR3 move;					// 移動値
	D3DXCOLOR	col;					// 色
	float		fWidth;					// 横幅
	float		fHPWidth;				// 横幅
	float		fHeight;				// 高さ
	float		fAngle;					// 角度
	float		fLenght;				// 長さ
	int			nCntTimer;				// 時間カウンタ
	int			nCounterAnim;			// アニメーションカウンタ
	int			nNumAnim;				// アニメーション番号
	int			nSpeedAnim;				// アニメーション速度
	int			nPatternAnim;			// アニメーションパターン
	WINDOWTEXTURE texture;				// テクスチャ種類
	SETTEXTURE type;					// 表示タイプ
	bool bUse;
}WINDOW;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitWindow(void);				// ウィンドウ初期化処理
void UninitWindow(void);			// ウィンドウ終了処理
void UpdateWindow(void);			// ウィンドウ更新処理
void DrawTopFrontWindow(void);		// ウィンドウ一番前景描画処理
void DrawFrontWindow(void);			// ウィンドウ前景描画処理
void DrawTopBGWindow(void);			// ウィンドウ背景描画処理
void DrawBGWindow(void);			// ウィンドウ背景描画処理

void SetWindow(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, WINDOWTEXTURE texture, SETTEXTURE type);				// ウィンドウ設定処理
void SetSelectState(SELECTSTATE state);			// ウィンドウ設定処理
void DeleteWindow(int nCntWindow);				// ウィンドウ破棄処理（１つ）
void DeleteWindow(WINDOWTEXTURE texture);		// ウィンドウ破棄処理（テクスチャ）
void DeleteWindow(void);						// ウィンドウ破棄処理（全部）

#endif