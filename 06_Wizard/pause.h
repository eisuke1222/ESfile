//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScene2D;			// シーン２D

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CPause : public CScene
{// ポーズ（親：CScene）
public:
	CPause();									// コンストラクタ
	~CPause();									// デストラクタ

	typedef enum
	{// 種類
		TYPE_BG = 0,			// 背景
		TYPE_FRAME,				// 枠
		TYPE_LINE,				// 線
		TYPE_MENU,				// メニュー
		TYPE_MAX				// 最大数
	}TYPE;

	typedef enum
	{// 選択
		SELECT_CONTINUE = 0,	// コンテニュー
		SELECT_RETRY,			// リトライ
		SELECT_QUIT,			// クイット
		SELECT_MAX				// 最大数
	}SELECT;

	typedef enum
	{// マーク
		MARK_LEFT = 0,
		MARK_RIGHT,
		MARK_MAX
	}MARK;

	static HRESULT Load(void);					// ロード
	static void Unload(void);					// アンロード
	static CPause *Create(void);				// 生成

	HRESULT Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

private:
	CScene2D *m_apPauseBg[TYPE_MAX];			// ポーズ背景
	CScene2D *m_apSelect[SELECT_MAX];			// 選択
	
	int m_nSelectNum;							// 選択番号
	int m_nCntTimer;							// 時間カウンタ

	CScene2D *m_pSelectChange;					// 選択変化
	CScene2D *m_apMark[MARK_MAX];				// 選択マーク

protected:

};

#endif