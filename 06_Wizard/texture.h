//=============================================================================
//
// テクスチャ処理 [texture.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTexture
{// テクスチャ
public:
	typedef enum
	{// 種類
		TYPE_MODE_PRESS = 0,				// プレス
		TYPE_MODE_NUMBER,					// 番号
		TYPE_MODE_END,						// 終了確認
		TYPE_MODE_YESNO,					// YESNO
		TYPE_MODE_BUTTON,					// ボタン
		TYPE_TITLE_LOGO,					// タイトル
		TYPE_TITLE_LOGO_BG,					// タイトルエフェクト用
		TYPE_TITLE_MENU,					// タイトルメニュー
		TYPE_TITLE_MENU_BG,					// タイトルメニューBG
		TYPE_GAME_FIELD,					// フィールド
		TYPE_GAME_EFFECT,					// エフェクト
		TYPE_GAME_MAP,						// マップ
		TYPE_GAME_MAPICON,					// マップアイコン
		TYPE_GAME_SKY_1,					// 空１
		TYPE_GAME_SKY_2,					// 空２
		TYPE_GAME_GRASS,					// 草
		TYPE_GAME_WARP,						// ワープ
		TYPE_GAME_GOAL,						// ゴール
		TYPE_GAME_PLAYER_STEP,				// ステップ
		TYPE_GAME_PLAYER_WALK,				// 歩く
		TYPE_GAME_PLAYER_SKILL,				// スキル
		TYPE_GAME_CAMERA_ROCKON_FRAME,		// ロックオンフレーム
		TYPE_GAME_CAMERA_ROCKON_PLAY,		// ロックオンプレイ
		TYPE_GAME_UI_FRAME,					// UIフレーム
		TYPE_GAME_UI_CHAR,					// UIキャラ
		TYPE_GAME_UI_HPSP,					// UIHPSP
		TYPE_GAME_UI_WEAPON,				// UI武器
		TYPE_GAME_UI_PLAYER_NAME,			// UIプレイヤー名
		TYPE_GAME_UI_ENEMY_NAME,			// UI敵名
		TYPE_GAME_UI_RANK,					// UIランク
		TYPE_GAME_UI_CHECK,					// チェック
		TYPE_GAME_UI_TUTORIAL_PLUS,			// チュートリアルプラス
		TYPE_GAME_UI_TUTORIAL_LOGO,			// チュートリアルロゴ
		TYPE_GAME_UI_TUTORIAL,				// チュートリアル
		TYPE_GAME_UI_CLEAR,					// クリア
		TYPE_GAME_UI_MISION,				// ミッション
		TYPE_GAME_UI_SKIP,					// スキップ
		TYPE_GAME_PAUSE_LOGO,				// ポーズロゴ
		TYPE_GAME_PAUSE_MENU,				// ポーズメニュー
		TYPE_GAME_PAUSE_BG,					// ポーズ背景
		TYPE_RESULT_CLEAR,					// クリア
		TYPE_RESULT_CLEAR_BG,				// クリアエフェクト用
		TYPE_RESULT_OVER,					// オーバー
		TYPE_RESULT_OVER_BG,				// オーバーエフェクト用
		TYPE_RESULT_RANK,					// ランク
		TYPE_MAX							// 最大数
	}TYPE;

	CTexture();											// コンストラクタ
	~CTexture();										// デストラクタ

	static void ReleaseAll(void);						// アンロード

	static LPDIRECT3DTEXTURE9 GetTexture(int nNum);		// モデル取得

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[TYPE_MAX];	// テクスチャ情報へのポインタ
	static int					m_pTexType[TYPE_MAX];	// テクスチャ種類
	static int					m_nMaxTexture;			// テクスチャ数

	const static char			*m_TexName[TYPE_MAX];	// テクスチャ名


protected:

};

#endif