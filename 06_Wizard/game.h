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

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CPause;			// ポーズ
class CMap;				// マップ
class CUi;				// UI
class CPlayer;			// プレイヤー
class CScene2D_Preset;	// シーン2Dプリセット
class CScene3D;			// シーン3D

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame
{// ゲーム
public:
	typedef enum
	{	// ゲーム
		GAME_NONE = 0,		// 何もしていない
		GAME_TUTORIAL,		// チュートリアル
		GAME_PLAY_1,		// ゲームプレイ1
		GAME_PLAY_2,		// ゲームプレイ2
		//GAME_PLAY_3,		// ゲームプレイ3
		GAME_PLAY_B,		// ゲームプレイBoss
		GAME_CLEAR,			// ゲームクリア
		GAME_OVER,			// ゲームオーバー
		GAME_MAX			// 最大数
	}GAME;

	typedef enum
	{
		TUTORIAL_MOVE = 0,
		TUTORIAL_CAMERA,
		TUTORIAL_JUMP,
		TUTORIAL_LOCKON,
		TUTORIAL_ATTACK,
		TUTORIAL_GUARD,
		TUTORIAL_SKILL,
		TUTORIAL_ACTION,
		TUTORIAL_MAX
	}TUTORIAL;

	CGame();						// コンストラクタ
	~CGame();						// デストラクタ

	static HRESULT Load(void);		// ロード
	static void Unload(void);		// アンロード

	HRESULT Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	void SetGame(GAME game, bool bReset = true);	// ゲーム設定
	GAME GetGame(void)				// ゲーム取得
		{ return m_game; };

	CMap *GetMap(void)				// マップ取得
		{ return m_pMap; }
	CUi *GetUi(void)				// Ui取得
		{ return m_pUi; }
	CPlayer *GetPlayer(void)		// プレイヤー取得
		{ return m_pPlayer; }

	void PauseSeting(void);			// ポーズ設定

	int GetMapNum(void)				// マップ番号取得
		{ return m_nMapNum; }

	void SetEnemyNum(int nNum)		// 敵総数設定
		{ m_nSetEnemyNum = nNum; }
	int GetEnemyNum(void)			// 敵総数取得
		{ return m_nSetEnemyNum; }

	void SetSS(bool bSetSS)			// スクリーンショットを撮る
		{ m_bSetSS = bSetSS; }
	void SetSS_Death(bool bSetSS)	// スクリーンショットを撮る
		{ m_bSetSS_Death = bSetSS; }

	int GetTutprialNum(void)		// チュートリアル番号取得
		{ return m_nTutorialNum; }

	void AddDeathEnemy(void)		// 倒した敵加算
		{ m_nDeathEnemy++; }
	void SetNoDamage(void)			// ノーダメージ
		{ m_bNoDamage = false; }
	void AddJustGuard(void)			// ジャストガード成功
		{ m_nJastGuard++; }

	int GetScore(void);				// スコア取得

private:
	GAME		m_game;				// ゲーム
	CPause		*m_pPause;			// ポーズ
	bool		m_bPause;			// ポーズしてるか

	CMap		*m_pMap;			// マップ
	CUi			*m_pUi;				// UI
	CPlayer		*m_pPlayer;			// プレイヤー

	int			m_nMapNum;			// マップ番号
	D3DXVECTOR3 posOld;				// 前回位置

	int			m_nSetEnemyNum;		// 敵総数

	int			m_nCntTimer;		// 時間カウント

	bool		m_bSetSS;			// スクリーンショットを撮る
	bool		m_bSetSS_Death;		// スクリーンショットを撮る
	bool		m_bPlay_SS00;		// スクリーンショット00
	bool		m_bPlay_SS01;		// スクリーンショット01
	bool		m_bPlay_SS02;		// スクリーンショット02

	int			m_nTutorialNum;				// チュートリアル番号
	int			m_nCntTutorial;				// チュートリアルカウント
	bool		m_bTutorial[TUTORIAL_MAX];  // チュートリアル
	CScene2D_Preset *m_pClear;				// クリア
	int			m_nCntCheck;				// チェックカウント
	CScene3D	*m_pCheck;					// チェック

	int			m_nCntSkip;					// スキップカウント
	CScene2D_Preset *m_pSkip;				// スキップ

	int			m_nPlayerHP;				// プレイヤーHP
	int			m_nPlayerMP;				// プレイヤーMP

	int			m_nTimer;					// タイマー
	int			m_nDeathEnemy;				// 倒した敵総数
	bool		m_bNoDamage;				// ノーダメージ
	int			m_nJastGuard;				// ジャストガード成功
	bool		m_bAllDeathEnemy;			// 全ての敵を倒した

	int				m_nCntTutorialPlus;		// チュートリアルプラスカウント
	CScene2D_Preset *m_pTutorialPlus;		// チュートリアルプラス
	CScene2D_Preset *m_pTutorialPlusButton;	// チュートリアルプラスボタン

protected:

};

#endif