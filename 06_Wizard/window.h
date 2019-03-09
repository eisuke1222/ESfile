//=============================================================================
//
// ウィンドウ処理 [window.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _window_H_
#define _window_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene2d.h"	// シーン2D

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CWindow : public CScene
{// ウィンドウ（親：CScene）
public:
	typedef enum
	{// 種類
		WINDOW_YESNO = 0,	// YESNO
		WINDOW_MAX			// 最大数
	}WINDOW;

	CWindow();												// コンストラクタ
	virtual ~CWindow();										// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

															// クリエイト
	static CWindow *Create(WINDOW window);

	virtual HRESULT Init(void) = 0;							// 初期化処理
	virtual void Uninit(void);								// 終了処理
	virtual void Update(void) = 0;							// 更新処理
	virtual void Draw(void) = 0;							// 描画処理

private:

protected:

};

class CWindow_YESNO : public CWindow
{// ウィンドウ（YESNO）（親：CEffect）
public:
	typedef enum
	{
		TYPE_BG = 0,
		TYPE_MESSAGE_BG,
		TYPE_MESSAGE,
		TYPE_SELECT_BG,
		TYPE_YES,
		TYPE_NO,
		TYPE_MAX
	}TYPE;

	CWindow_YESNO();										// コンストラクタ
	~CWindow_YESNO();										// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

	HRESULT Init(void);										// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

	int GetSelectNum(void) { return m_nSelectNum; }			// 選択番号取得

private:
	CScene2D	*m_pScene2D[TYPE_MAX];						// シーン2D

	int			m_nSelectNum;								// 選択番号

protected:

};

#endif