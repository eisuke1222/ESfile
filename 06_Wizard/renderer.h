//=============================================================================
//
// レンダラー処理 [renderer.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "font.h"		// フォント

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CFont;	// フォント

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRenderer
{// レンダラー
public:
	CRenderer();								// コンストラクタ
	~CRenderer();								// デストラクタ

	HRESULT Init(HWND hWnd, bool bWindow);		// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	LPDIRECT3DDEVICE9 GetDevice(void)			// デバイス取得 
		{ return m_pD3DDevice; }	

	static CFont *GetFont(int nFontNum)			// フォント取得
		{ return m_pFont[nFontNum]; }	

	static void SetImGuiWindow(bool bImGuiWindow)	// ImGui描画設定
		{ m_bImGuiWindow = bImGuiWindow; }
	static bool GetImGuiWindow(void)				// ImGui描画取得
		{ return m_bImGuiWindow; }


	static void SetDrawAll(bool bDrawAll)		// 全描画設定
		{ m_bDrawAll = bDrawAll; }

	static void SetDrawMap(bool bDrawMap)		// マップ描画設定
		{ m_bDrawMap = bDrawMap; }

private:
	LPDIRECT3D9				m_pD3D;				// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;		// Deviceオブジェクト(描画に必要)

	static CFont			*m_pFont[CFont::FONTNUM_MAX];	// フォントへのポインタ
	static bool				m_bImGuiWindow;		// ImGui表示
	static bool				m_bDrawAll;			// 全描画
	static bool				m_bDrawMap;			// マップ描画

protected:

};

#endif