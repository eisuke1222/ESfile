//=========================================================================================================================
// 
// 背景処理 [bgwindow.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _BGWINDOW_H_
#define _BGWINDOW_H_

//*************************************************************************************************************************
// インクルードファイル
//*************************************************************************************************************************
#include "main.h"
#include "game.h"		// ゲーム

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define BGWINDOWTEXTURE_NAME_1	"data\\TEXTURE\\sky001.png"				// 読み込むテクスチャファイル1	タイトル（背景）
#define BGWINDOWTEXTURE_NAME_2	"data\\TEXTURE\\titlelogo000.png"		// 読み込むテクスチャファイル2	タイトル（ロゴ）
#define BGWINDOWTEXTURE_NAME_3	"data\\TEXTURE\\titleground000.png"		// 読み込むテクスチャファイル3	タイトル（地面）
#define BGWINDOWTEXTURE_NAME_4	"data\\TEXTURE\\gamemap000.png"			// 読み込むテクスチャファイル4	ゲーム（マップ）
#define BGWINDOWTEXTURE_NAME_5	"data\\TEXTURE\\pausekey000.png"		// 読み込むテクスチャファイル5	ゲーム（Pキー）
#define BGWINDOWTEXTURE_NAME_6	"data\\TEXTURE\\stage000.png"			// 読み込むテクスチャファイル6	ゲーム（ステージ番号）
#define BGWINDOWTEXTURE_NAME_7	"data\\TEXTURE\\hiscoretxt000.png"		// 読み込むテクスチャファイル7	ゲーム（ハイスコアテキスト）
#define BGWINDOWTEXTURE_NAME_8	"data\\TEXTURE\\all004.png"				// 読み込むテクスチャファイル8	ゲーム（残機）
#define BGWINDOWTEXTURE_NAME_9	"data\\TEXTURE\\all003.png"				// 読み込むテクスチャファイル9	ゲーム（プレイヤー）
#define BGWINDOWTEXTURE_NAME_10	"data\\TEXTURE\\life000.png"			// 読み込むテクスチャファイル10	ゲーム（体力）
#define BGWINDOWTEXTURE_NAME_11	"data\\TEXTURE\\coin001.png"			// 読み込むテクスチャファイル11	ゲーム（コイン）
#define BGWINDOWTEXTURE_NAME_12	"data\\TEXTURE\\cointxt000.png"			// 読み込むテクスチャファイル12	ゲーム（コインテキスト）
#define BGWINDOWTEXTURE_NAME_13	"data\\TEXTURE\\help000.png"			// 読み込むテクスチャファイル13 ゲーム（ヘルプ）
#define BGWINDOWTEXTURE_NAME_14	"data\\TEXTURE\\help001.png"			// 読み込むテクスチャファイル14 ゲーム（ヘルプ（移動））
#define BGWINDOWTEXTURE_NAME_15	"data\\TEXTURE\\help002.png"			// 読み込むテクスチャファイル15	ゲーム（ヘルプ（走る））
#define BGWINDOWTEXTURE_NAME_16 "data\\TEXTURE\\help003.png"			// 読み込むテクスチャファイル16 ゲーム（ヘルプ（ジャンプ））
#define BGWINDOWTEXTURE_NAME_17	"data\\TEXTURE\\help004.png"			// 読み込むテクスチャファイル17	ゲーム（ヘルプ（攻撃））
#define BGWINDOWTEXTURE_NAME_18	"data\\TEXTURE\\sky000.png"				// 読み込むテクスチャファイル18	ゲーム（背景（空））
#define BGWINDOWTEXTURE_NAME_19	"data\\TEXTURE\\iwahaikei000.png" 		// 読み込むテクスチャファイル19	ゲーム（背景（岩））
#define BGWINDOWTEXTURE_NAME_20	"data\\TEXTURE\\Grass.png"				// 読み込むテクスチャファイル20	ゲーム（木（下）
#define BGWINDOWTEXTURE_NAME_21	"data\\TEXTURE\\Grove.png"				// 読み込むテクスチャファイル21	ゲーム（木（並木））
#define BGWINDOWTEXTURE_NAME_22	"data\\TEXTURE\\Tree.png"				// 読み込むテクスチャファイル22	ゲーム（木（小））
#define BGWINDOWTEXTURE_NAME_23	"data\\TEXTURE\\yama.png"				// 読み込むテクスチャファイル23	ゲーム（砂漠（山））
#define BGWINDOWTEXTURE_NAME_24	"data\\TEXTURE\\coinwall000.png"		// 読み込むテクスチャファイル24	ゲーム（ゴール制限）
#define BGWINDOWTEXTURE_NAME_25 "data\\TEXTURE\\flag000.png"			// 読み込むテクスチャファイル25 ゲーム（ゴール）
#define BGWINDOWTEXTURE_NAME_26 "data\\TEXTURE\\flag001.png"			// 読み込むテクスチャファイル26 ゲーム（ゴール（小））
#define BGWINDOWTEXTURE_NAME_27	"data\\TEXTURE\\gameclear000.png"		// 読み込むテクスチャファイル27	リザルト（クリア）
#define BGWINDOWTEXTURE_NAME_28 "data\\TEXTURE\\gameover000.png"		// 読み込むテクスチャファイル28	リザルト（オーバー）
#define BGWINDOWTEXTURE_NAME_29 "data\\TEXTURE\\press_enter.png"		// 読み込むテクスチャファイル29	選択

//*************************************************************************************************************************
// 構造体定義
//*************************************************************************************************************************
typedef enum
{// テクスチャ種類
	BGWINDOWTEXTURE_TITLE_BG = 0,		// タイトルBG
	BGWINDOWTEXTURE_TITLE_LOGO,			// タイトルロゴ
	BGWINDOWTEXTURE_TITLE_GOUND,		// タイトル地面
	BGWINDOWTEXTURE_GAME_MAP,			// ゲームマップ
	BGWINDOWTEXTURE_GAME_PKEY,			// ゲームP
	BGWINDOWTEXTURE_GAME_NUMSTAGE,		// ゲームステージ番号
	BGWINDOWTEXTURE_GAME_HISCORETXT,	// ゲームハイスコアテキスト
	BGWINDOWTEXTURE_GAME_NUMPLAYER,		// ゲーム残機
	BGWINDOWTEXTURE_GAME_PLAYER,		// ゲームプレイヤー
	BGWINDOWTEXTURE_GAME_LIFE,			// ゲームライフ
	BGWINDOWTEXTURE_GAME_COIN,			// ゲームコイン
	BGWINDOWTEXTURE_GAME_COINTXT,		// ゲームコインテキスト
	BGWINDOWTEXTURE_GAME_HELP,			// ゲームヘルプ
	BGWINDOWTEXTURE_GAME_HELPWALK,		// ゲームヘルプ歩く
	BGWINDOWTEXTURE_GAME_HELPRUN,		// ゲームヘルプ走る
	BGWINDOWTEXTURE_GAME_HELPJUMP,		// ゲームヘルプジャンプ
	BGWINDOWTEXTURE_GAME_HELPATTACK,	// ゲームヘルプ攻撃
	BGWINDOWTEXTURE_GAME_STAGE,			// ゲームステージ
	BGWINDOWTEXTURE_GAME_STAGE_IWA,		// ゲームステージ岩背景
	BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER,	// ゲームステージ木（下）
	BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL,	// ゲームステージ木（並木）
	BGWINDOWTEXTURE_GAME_STAGE_TREE_SMALL,	// ゲームステージ木（小）
	BGWINDOWTEXTURE_GAME_STAGE_SABAKU_YAMA,	// ゲームステージ砂漠（山）
	BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT,// ゲームステージゴール制限
	BGWINDOWTEXTURE_GAME_STAGEGOAL,		// ゲームステージゴール
	BGWINDOWTEXTURE_GAME_STAGEGOAL_SMALL,	// ゲームステージゴール（小）
	BGWINDOWTEXTURE_RESULT_CLEAR,		// リザルトクリア
	BGWINDOWTEXTURE_RESULT_OVER,		// リザルトオーバー
	BGWINDOWTEXTURE_SELECT,				// 選択
	BGWINDOWTEXTURE_MAX,
}BGWINDOWTEXTURE;

typedef enum
{// 選択状態
	SELECTSTATE_NORMAL = 0,				// ノーマル
	SELECTSTATE_ENTER,					// エンター
	SELECTSTATE_MAX,
}SELECTSTATE;

typedef struct
{// 背景状態
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 tex;					// 位置
	D3DXVECTOR3 texUV;					// UV
	D3DXVECTOR3 move;					// 移動値
	D3DXCOLOR	col;					// 色
	float		fWidth;					// 横幅
	float		fHeight;				// 高さ
	float		fAngle;					// 角度
	float		fLenght;				// 長さ
	int			nCounterAnim;			// アニメーションカウンタ
	int			nNumAnim;				// アニメーション番号
	int			nSpeedAnim;				// アニメーション速度
	int			nPatternAnim;			// アニメーションパターン
	BGWINDOWTEXTURE texture;			// テクスチャ
	bool bUse;
}BGWINDOW;

//*************************************************************************************************************************
// プロトタイプ宣言
//*************************************************************************************************************************
void InitBGWindow(void);				// 背景初期化処理
void UninitBGWindow(void);				// 背景終了処理
void UpdateBGWindow(void);				// 背景更新処理
void DrawBGWindow(void);				// 背景描画処理

void SetBGWindow(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, BGWINDOWTEXTURE texture);				// 背景設定処理
void SetSelectState(SELECTSTATE state);	// 背景設定処理
void HitPlayerLife(void);				// プレイヤー体力減少
bool CollisionBGWindow(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);	// 背景衝突処理
bool GetGoalHit(void);
void DeleteBGWindow(int nCntBGWindow);	// 背景破棄処理（１つ）
void DeleteBGWindow(void);				// 背景破棄処理（全部）

#endif