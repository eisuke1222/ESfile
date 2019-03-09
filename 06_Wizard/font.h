//=============================================================================
//
// フォント処理 [font.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _FONT_H_
#define _FONT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "manager.h"	// マネージャー
#include "scene.h"		// シーン

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFont
{// フォント
public:
	typedef enum
	{// フォント番号
		FONTNUM_COMMAND = 0,		// コマンド
		FONTNUM_LOG,				// ログ
		FONTNUM_DATA,				// データ
		FONTNUM_DEBUG,				// デバッグ
		FONTNUM_TEST,				// テスト
		FONTNUM_MAX					// 最大数
	}FONTNUM;

	typedef enum
	{// セットタイプ
		SETTYPE_NEW = 0,			// 上書き
		SETTYPE_ADD,				// 追加（下）
		SETTYPE_LOG,				// 追加（上）
		SETTYPE_MAX					// 最大数
	}SETTYPE;

	static const int m_MAX_MESSAGE = 1024;		// メッセージ最大数

	CFont();									// コンストラクタ
	~CFont();									// デストラクタ

												// クリエイト
	static CFont *Create(int nHeight, int nWidth, int nWeight, RECT rect, UINT uFormat, D3DXCOLOR col);	

	static void ReleaseAll(void);				// 全破棄
	static void DrawAll(void);					// 全描画

	static bool GetDispAll(void)				// フォント描画
		{ return m_bDispAll; }
												// 初期化処理
	HRESULT Init(int nHeight, int nWidth, int nWeight, RECT rect, UINT uFormat, D3DXCOLOR col);				
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

												// 文字設定
	static void SetMessageData(CScene::OBJTYPE objType, SETTYPE nSetType, char *aMessage, ...);				
												// 文字取得
	static void *GetMessageData(CScene::OBJTYPE objType)		
		{ return m_MessageData[objType]; }

												// 文字設定
	void SetMessage(SETTYPE nSetType, char *aMessage, ...);		
	char *GetMessage(void)						// 文字取得
		{ return m_Message; }	

	void SetDisp(bool bDisp)					// 表示設定 
		{ m_bDisp = bDisp; }
	bool GetDisp(void)							// 表示取得
		{ return m_bDisp; }

private:
	static CFont	*m_apFont[FONTNUM_MAX];									// シーン
	static bool		m_bDispAll;												// 表示
	static char		m_MessageData[CScene::OBJTYPE_MAX][m_MAX_MESSAGE];		// メッセージ

	int				m_nID;													// ID番号
	LPD3DXFONT		m_pFont = NULL;											// フォントへのポインタ

	bool			m_bDisp;												// 表示
	char			m_Message[m_MAX_MESSAGE];								// メッセージ
	RECT			m_rect;													// 範囲
	UINT			m_uFormat;												// フォーマット
	D3DXCOLOR		m_col;													// 色

protected:
	void Release(void);							// 開放処理

};

#endif