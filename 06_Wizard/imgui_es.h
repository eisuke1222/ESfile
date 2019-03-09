//=============================================================================
//
// IMGUI処理 [imgui_es.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _IMGUI_ES_H_
#define _IMGUI_ES_H_

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
class CImgui_ES
{// Imgui_ES
public:
	struct AppLog
	{
		ImGuiTextBuffer     Buf;
		ImGuiTextFilter     Filter;
		ImVector<int>       LineOffsets;        // Index to lines offset
		bool                ScrollToBottom;

		void Clear() { Buf.clear(); LineOffsets.clear(); }
		void AddLog(const char* fmt, ...) IM_FMTARGS(2);
		void Draw(const char* title, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
	};

	CImgui_ES();									// コンストラクタ
	~CImgui_ES();									// デストラクタ

													// ログウィンドウ
	static void ShowAppLog(bool* p_open = NULL, ImGuiWindowFlags flags = 0);
													// デバッグウィンドウ
	static void ShowAppDebug(bool* p_open = NULL, ImGuiWindowFlags flags = 0);

													// スタイル変更
	static IMGUI_API void ShowStyleEditor(ImGuiStyle* ref = NULL);
	static void ShowHelpMarker(const char* desc);	// ヘルプ表示

	static void SetMessage(char *aMessage, ...);	// 文字設定

private:
	static AppLog m_AppLog;

protected:

};

#endif