//=============================================================================
//
// ゲーム処理 [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MAP			(128)		// マップの最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;			// シーン2D
class CScene3D;			// シーン2D
class CPause;			// ポーズ
class CRanking;			// ランキング

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame
{// ゲーム
public:
	typedef enum
	{// ゲーム
		GAME_NONE = 0,		// 何もしていない
		GAME_PLAY,			// プレイ
		GAME_CLEAR,			// クリア
		GAME_OVER,			// オーバー
		GAME_MAX
	}GAME;

	CGame();						// コンストラクタ
	~CGame();						// デストラクタ

	static HRESULT Load(void);		// ロード
	static void Unload(void);		// アンロード

	HRESULT Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	static void SetPause(bool bPause);					// ポーズ設定
	static bool GetPause(void) { return m_bPause; }		// ポーズ取得

	static void SetGame(GAME game) { m_game = game; m_nCntGame = 0; }	// ゲーム設定
	static GAME GetGame(void) { return m_game; }						// ゲーム取得

private:
	typedef struct
	{// マップ
		int nNumModel;		// モデル番号
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
	}MAP;

	typedef struct
	{// マップ
		int nNumEnemy;		// モデル番号
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 move;	// 向き
		int nShotTiming;	// 発射タイミング
	}ENEMY;

	static LPDIRECT3DTEXTURE9		m_pTexture;						// テクスチャ情報へのポインタ
	static CScene2D					*m_pScene2D[MAX_PLAYER];		// シーン2D

	static CRanking					*m_pRanking;					// ランキング

	static CPause *m_pPause;		// ポーズ
	static bool m_bPause;			// ポーズしているか

	static GAME m_game;				// ゲーム
	static int m_nCntGame;			// ゲームカウンタ

	static LPDIRECT3DTEXTURE9		m_pENDTexture[2];					// テクスチャ情報へのポインタ
	static CScene3D *m_pScene3D;

	void SetMap(void);				// マップ設定

	static MAP m_mapState[MAX_MAP];	// マップ情報
	static int m_mapMax;			// マップ最大数
	static ENEMY m_enemyState[MAX_MAP];	// 敵情報
	static int m_enemyMax;			// 敵最大数

	int m_mapNum;					// マップ番号
	int m_enemyNum;					// マップ番号
	int m_mapfield;					// フィールド番号


protected:

};

#endif