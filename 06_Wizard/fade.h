//=============================================================================
//
// フェード処理 [fade.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene2d.h"	// シーン2D
#include "game.h"		// ゲーム

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFade : public CScene2D
{// フェード（親：CScene2D）
public:
	typedef enum
	{// フェード状態
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX			// 最大数
	} FADE;

	CFade();												// コンストラクタ
	~CFade();												// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード

															// クリエイト
	static CFade *Create(CManager::MODE mode, FADE fade = FADE_OUT);
	static CFade *Create(CGame::GAME game, FADE fade = FADE_OUT);

	HRESULT Init(CManager::MODE mode, FADE fade);			// 初期化処理
	HRESULT Init(CGame::GAME game, FADE fade);				// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

	static FADE GetFade(void)								// フェード取得
		{ return m_fade; }			

private:
	static FADE		m_fade;									// フェード状態

	int				m_fadeNum;								// フェード番号
	CManager::MODE	m_modeNext;								// 次のモード
	CGame::GAME		m_gameNext;								// 次のゲーム
	int				m_nCntTimer;							// 時間カウント

protected:

};

#endif