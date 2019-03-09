//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

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
class CSound;			// サウンド
class CInputKeyboard;	// キーボード
class CInputMouse;		// マウス
class CInputDIPad;		// DIパッド
class CInputXPad;		// Xパッド
class CRenderer;		// レンダラー
class CCamera;			// カメラ
class CLight;			// ライト

class CTitle;			// タイトル
class CGame;			// ゲーム
class CResult;			// リザルト
class CRanking;			// ランキング

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CManager
{// マネージャー
public:
	const static int m_PLAYERMAX = 4;					// プレイヤー数

	typedef enum
	{// カメラ
		CAMERA_MAIN = 0,	// メイン
		CAMERA_GAME_MAP,	// ゲームマップ
		CAMERA_MAX			// 最大数
	}CAMERA;

	typedef enum
	{// モード
		MODE_NONE = 0,		// 何もしていない
		MODE_TITLE,			// タイトル-
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
		MODE_MAX			// 最大数
	}MODE;

	CManager();											// コンストラクタ
	virtual ~CManager();								// デストラクタ

														// 初期化処理
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		
	virtual void Uninit(void);							// 終了処理
	virtual void Update(void);							// 更新処理
	virtual void Draw(void);							// 描画処理

	static CSound *GetSound(void)						// サウンド取得
		{ return m_pSound; }
	static CInputKeyboard *GetInputKeyboard(void)		// キーボード取得
		{ return m_pInputKeyboard; }
	static CInputMouse *GetInputMouse(void)				// マウス取得
		{ return m_pInputMouse; }
	static CInputDIPad *GetInputDIPad(void)				// DIパッド取得
		{ return m_pInputDIPad; }
	static CInputXPad *GetInputXPad(void)				// Xパッド取得
		{ return m_pInputXPad; }
	static CRenderer *GetRenderer(void)					// レンダラー取得
		{ return m_pRenderer; }
	static CCamera *GetCamera(int nNum)					// カメラ取得
		{ return m_pCamara[nNum]; }
	static CLight *GetLight(void)						// ライト取得
		{ return m_pLight; }

	static CTitle *GetTitle(void)						// タイトル取得
		{ return m_pTitle; }
	static CGame *GetGame(void)							// ゲーム取得
		{ return m_pGame; }
	static CResult *GetResult(void)						// リザルト取得
		{ return m_pResult; }

	static void SetMode(MODE mode);						// モード設定
	static MODE GetMode(void)							// モード取得 
		{ return m_mode; }

	static void SetPlayNum(int nPlayerNum)				// プレイ人数設定
		{ if (nPlayerNum <= m_PLAYERMAX)m_nPlayNum = nPlayerNum; else m_nPlayNum = m_PLAYERMAX; }
	static int GetPlayNum(void)							// プレイ人数取得
		{ return m_nPlayNum; }

	static void ConMode(MODE mode, char *cMode);		// モード文字変換

private:
	void DebugFont(void);								// デバッグフォント

	static MODE				m_mode;						// モード

	static CSound			*m_pSound;					// サウンド
	static CInputKeyboard	*m_pInputKeyboard;			// キーボード
	static CInputMouse		*m_pInputMouse;				// マウス
	static CInputDIPad		*m_pInputDIPad;				// DIパッド
	static CInputXPad		*m_pInputXPad;				// Xパッド
	static CRenderer		*m_pRenderer;				// レンダラー
	static CCamera			*m_pCamara[CAMERA_MAX];		// カメラ
	static CLight			*m_pLight;					// ライト

	static CTitle			*m_pTitle;					// タイトル
	static CGame			*m_pGame;					// ゲーム
	static CResult			*m_pResult;					// リザルト

	static int				m_nPlayNum;					// プレイ人数

protected:

};

#endif