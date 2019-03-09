//=============================================================================
//
// タイトル処理 [title.h]
// Author : Shun Yokomichi
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

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
class CWindow;			// ウィンドウ
class CPlayer;			// プレイヤー
class CScene3D;
class CScene2D;
class CSceneMesh;
class CUi;
class CRanking;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTitle
{// タイトル
public:
	typedef enum
	{// 選択
		SELECT_GAME = 0,	// ゲーム
		SELECT_TUTORIAL,	// チュートリアル
		SELECT_EDIT,		// 編集
		SELECT_TITLE,		// タイトル
		SELECT_MAX			// 最大数
	}SELECT;

	const static int m_MAX_WALL = 4;

	CTitle();								// コンストラクタ
	~CTitle();								// デストラクタ

	static HRESULT Load(void);				// ロード
	static void Unload(void);				// アンロード

	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 終了処理
	void Update(void);						// 更新処理
	void Draw(void);						// 描画処理

	CPlayer *GetPlayer(void)				// プレイヤー取得
		{ return m_pPlayer; }

	bool GetTitle(void)						// タイトルか
		{ return m_bTitle; }

	int GetSelectNum(void)					// 選択番号取得
		{ return m_nSelectNum; }

	bool GetTutorial(void)					// チュートリアルか
		{ return m_bTutorial; }
	bool GetSelectSet(void)					// 選択設定しているか
		{ return m_bSelectSet; }

	CUi *GetUi(void)				// Ui取得
		{ return m_pUi_Game; }

private:
	CPlayer			*m_pPlayer;				// プレイヤー

	bool			m_bTitle;				// タイトルか

	int				m_nSelectNum;			// 選択番号
	int				m_nCntTimer;			// 時間カウンタ

	bool			m_bTutorial;			// チュートリアルか
	bool			m_bSelectSet;			// 選択設定しているか

	CWindow			*m_pWindow;				// ウィンドウ
	CScene3D		*m_pIcon;				// アイコン
	CSceneMesh		*m_pReturnCircle;		// 戻るサークル
	CSceneMesh		*m_pWall[m_MAX_WALL];	// 壁

	CUi				*m_pUi_Game;			// UI

	bool			m_bRanking;				// 
	CRanking		*m_pRanking;
	CScene2D		*m_pRankingBG;

protected:

};

#endif