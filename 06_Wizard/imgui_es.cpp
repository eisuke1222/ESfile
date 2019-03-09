//=============================================================================
//
// IMGUI処理 [imgui_es.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "imgui.h"			// ImGui
#include "imgui_es.h"		// ImGui_ES
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "fade.h"			// フェード
#include "camera.h"			// カメラ
#include "scene.h"			// シーン
#include "title.h"			// タイトル
#include "game.h"			// ゲーム
#include "player.h"			// プレイヤー

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CImgui_ES::AppLog	CImgui_ES::m_AppLog	= {};		// ログ

//=============================================================================
// メインウィンドウ
//=============================================================================
void ImGui::ShowESWindow()
{
	static bool bCollapsed = false;		// チェック

	if (bCollapsed == false)
	{
#ifdef _DEBUG
		ImGui::SetNextWindowCollapsed(false);	// 閉じる
#else
		ImGui::SetNextWindowCollapsed(true);	// 開く
#endif // _DEBUG
		bCollapsed = true;
	}

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	char CMode[32];									// モード文字変換
	CManager::MODE mode	= CManager::GetMode();		// モード取得
	CManager::ConMode(mode, CMode);					// モード文字取得

	int nNumAll			= CScene::GetNumAll();		// オブジェクト数取得

	CFade::FADE fade	= CFade::GetFade();			// フェード取得

	// ウィンドウオプション種類
	static bool bNoMove			= true;				// 移動
	static bool bNoResize		= true;				// サイズ
	static bool bNoBackground	= false;			// バックグラウンド

	// ウィンドウフラッグ設定
	ImGuiWindowFlags window_flags = 0;
	if (bNoMove)       window_flags |= ImGuiWindowFlags_NoMove;
	if (bNoResize)     window_flags |= ImGuiWindowFlags_NoResize;
	if (bNoBackground) window_flags |= ImGuiWindowFlags_NoBackground;

	// ウィンドウ種類
	static bool bLog			= false;			// ログ
	static bool bDebug			= false;			// デバッグ
	static bool bDemo			= false;			// デモ

	// ウィンドウ表示
	if (bLog)	CImgui_ES::ShowAppLog(&bLog, window_flags);
	if (bDebug) CImgui_ES::ShowAppDebug(&bDebug, window_flags);
	if (bDemo)	ImGui::ShowDemoWindow(&bDemo, window_flags);

	// ウィンドウ開始
	ImGui::Begin("ImGui ES Window", NULL, window_flags);
	{
		// 各詳細
		ImGui::Text("*** Main Menu ***");							// タイトル
		ImGui::Text("FPS	   : %.1f", ImGui::GetIO().Framerate);	// FPS
		ImGui::Text("Mode      : %s", CMode);						// モード
		ImGui::Text("ObjectNum : %d", nNumAll);						// オブジェクト数
		ImGui::NewLine();											// 改行
		ImGui::Separator();											// 線

		// ウィンドウ表示チェック
		ImGui::Text("*** Ather Window ***");						// タイトル
		ImGui::Checkbox("Log Window", &bLog);						// ログウィンドウ
		ImGui::Checkbox("Debug Window", &bDebug);					// デバッグウィンドウ
		ImGui::Checkbox("Demo Window", &bDemo);						// デモウィンドウ
		ImGui::NewLine();											// 改行

		// 各ヘッダー
		if (ImGui::CollapsingHeader("Window Option"))
		{// オプション
			ImGui::Text("*** Seting ***");							// タイトル
			ImGui::Checkbox("No Move", &bNoMove);					// 移動
			ImGui::Checkbox("No Resize", &bNoResize);				// サイズ
			ImGui::Checkbox("No Background", &bNoBackground);		// バックグラウンド
			ImGui::Separator();										// 線

			ImGui::ShowStyleEditor();								// スタイル変更
			ImGui::NewLine();										// 改行
		}
		if (ImGui::CollapsingHeader("Mode Change"))
		{// モード
			ImGui::Text("*** Next Mode Select ***");				// タイトル
			int nMode = mode;
			if (nMode != mode) { nMode = mode; }	// モードを反映
			int nNext = nMode;						// モードを記憶

			ImGui::RadioButton("[Mode] Title",		&nMode, CManager::MODE_TITLE);		// タイトルへ
			ImGui::RadioButton("[Mode] Game",		&nMode, CManager::MODE_GAME);		// ゲームへ
			ImGui::RadioButton("[Mode] Result",		&nMode, CManager::MODE_RESULT);		// リザルトへ
			ImGui::NewLine();

			if (nNext != nMode)
			{// 変更がされた
				CManager::SetMode((CManager::MODE)nMode);	// モード変更
			}
		}
		if (ImGui::CollapsingHeader("Draw Select"))
		{// 描画
			ImGui::Text("*** Object Chack ***");// タイトル
			static int nDSelect = 0;			// 描画選択

			// 値を記憶
			int nNextDSelect = nDSelect;		// 描画選択

			ImGui::BulletText("[All]:All");
			ImGui::BulletText("[ExP]:Except Player");
			ImGui::BulletText("[OnP]:Only Player");
			ImGui::Separator();

			ImGui::RadioButton("[All] None", &nDSelect, 0);

			ImGui::RadioButton("[All] WireFrame", &nDSelect, 1); ImGui::SameLine();
			ImGui::RadioButton("[All] Hidden", &nDSelect, 2);

			ImGui::RadioButton("[ExP] WireFrame", &nDSelect, 3); ImGui::SameLine();
			ImGui::RadioButton("[ExP] Hidden", &nDSelect, 4);

			ImGui::RadioButton("[OnP] WireFrame", &nDSelect, 5); ImGui::SameLine();
			ImGui::RadioButton("[OnP] Hidden", &nDSelect, 6);

			if (nNextDSelect != nDSelect)
			{// 選択が変わった
				if (nDSelect == 0) { CScene::SetDrawMode(0); CManager::GetRenderer()->SetDrawAll(true); }

				if (nDSelect == 1) { CScene::SetDrawMode(1); CManager::GetRenderer()->SetDrawAll(true); }
				if (nDSelect == 2) { CManager::GetRenderer()->SetDrawAll(false); }

				if (nDSelect == 3) { CScene::SetDrawMode(2); CManager::GetRenderer()->SetDrawAll(true); }
				if (nDSelect == 4) { CScene::SetDrawMode(4); CManager::GetRenderer()->SetDrawAll(true); }

				if (nDSelect == 5) { CScene::SetDrawMode(3); CManager::GetRenderer()->SetDrawAll(true); }
				if (nDSelect == 6) { CScene::SetDrawMode(5); CManager::GetRenderer()->SetDrawAll(true); }
			}

			ImGui::NewLine();	// 改行
		}
	}

	// ウィンドウ終了
	ImGui::End();
}

//=============================================================================
// AppLog : ログ追加							(public)	*** CImgui_ES ***
//=============================================================================
void CImgui_ES::AppLog::AddLog(const char* fmt, ...) IM_FMTARGS(2)
{// Imgui_ES::AppLog ログ追加
	int old_size = Buf.size();
	va_list args;
	va_start(args, fmt);
	Buf.appendfv(fmt, args);
	va_end(args);
	for (int new_size = Buf.size(); old_size < new_size; old_size++)
		if (Buf[old_size] == '\n')
			LineOffsets.push_back(old_size);
	ScrollToBottom = true;
}

//=============================================================================
// AppLog : 描画								(public)	*** CImgui_ES ***
//=============================================================================
void CImgui_ES::AppLog::Draw(const char* title, bool* p_open, ImGuiWindowFlags flags)
{// Imgui_ES::AppLog 描画
	if (!ImGui::Begin(title, p_open, flags))
	{
		ImGui::End();
		return;
	}
	if (ImGui::Button("Clear")) Clear();
	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (copy) ImGui::LogToClipboard();

	if (Filter.IsActive())
	{
		const char* buf_begin = Buf.begin();
		const char* line = buf_begin;
		for (int line_no = 0; line != NULL; line_no++)
		{
			const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
			if (Filter.PassFilter(line, line_end))
				ImGui::TextUnformatted(line, line_end);
			line = line_end && line_end[1] ? line_end + 1 : NULL;
		}
	}
	else
	{
		ImGui::TextUnformatted(Buf.begin());
	}

	if (ScrollToBottom)
		ImGui::SetScrollHereY(1.0f);
	ScrollToBottom = false;
	ImGui::EndChild();
	ImGui::End();
}

//=============================================================================
// コンストラクタ								(public)	*** CImgui_ES ***
//=============================================================================
CImgui_ES::CImgui_ES()
{

}

//=============================================================================
// デストラクタ								(public)	*** CImgui_ES ***
//=============================================================================
CImgui_ES::~CImgui_ES()
{

}

//=============================================================================
// ログウィンドウ								(public)	*** CImgui_ES ***
//=============================================================================
void CImgui_ES::ShowAppLog(bool* p_open, ImGuiWindowFlags flags)
{
	// ログ描画
	m_AppLog.Draw("Log Window", p_open, flags);
}

//=============================================================================
// デバッグウィンドウ							(public)	*** CImgui_ES ***
//=============================================================================
void CImgui_ES::ShowAppDebug(bool* p_open, ImGuiWindowFlags flags)
{
	CManager::MODE mode = CManager::GetMode();		// モード取得

	// ウィンドウ開始
	ImGui::Begin("Debug Window", p_open, flags);
	{
		ImGui::Text("*** Data Edit ***");		// タイトル

		ImGui::BulletText("*** All ***");		// 全部

		if (ImGui::CollapsingHeader("Input"))
		{// 入力
			CInputMouse *pMouse = CManager::GetInputMouse();

			POINT pt;
			GetCursorPos(&pt);

			D3DXVECTOR2 posPointer = pMouse->GetPointer();
			int nWheel = pMouse->GetWheel();

			ImGui::Text("*** Seting ***");
			ImGui::BulletText("Mouse");
			ImGui::InputFloat2("[Input] Pointer Pos", posPointer);
			ImGui::InputFloat2("[Input] Pointer Disp Pos", D3DXVECTOR2((float)pt.x, (float)pt.y));
			ImGui::InputInt("[Input] Wheel Num", &nWheel);
		}

		if (ImGui::CollapsingHeader("Camera"))
		{// カメラ
			CCamera *pCamera = CManager::GetCamera(CManager::CAMERA_MAIN);

			bool bEdit = false;
			D3DXVECTOR3 posV = pCamera->Getpos(0);
			D3DXVECTOR3 posR = pCamera->Getpos(1);
			D3DXVECTOR3 rot = pCamera->Getrot();

			ImGui::Text("*** Seting ***");
			if (ImGui::Button("Click")) { bEdit = true; posV = pCamera->GetInitPos(0); posR = pCamera->GetInitPos(1); }
			ImGui::SameLine();	ImGui::Text("[Camera] Reset position");

			if (ImGui::DragFloat3("[Camera] posV", posV, 0.5f, 0.0f, 0.0f, "%.3f", 1.0f))bEdit = true;
			if (ImGui::DragFloat3("[Camera] posR", posR, 0.5f, 0.0f, 0.0f, "%.3f", 1.0f))bEdit = true;
			ImGui::InputFloat3("[Camera] rot", rot);

			if (bEdit) pCamera->SetPos(posV, posR);
		}

		ImGui::Separator();

		ImGui::BulletText("*** Mode ***");		// モード

		if (ImGui::CollapsingHeader("Player"))
		{// プレイヤー
			CPlayer *pPlayer = NULL;

			if (mode == CManager::MODE_TITLE)
				pPlayer = CManager::GetTitle()->GetPlayer();
			else if (mode == CManager::MODE_GAME)
				pPlayer = CManager::GetGame()->GetPlayer();

			D3DXVECTOR3 pos;
			D3DXVECTOR3 rot;
			D3DXVECTOR3 move;
			float fJump = 0.0f;

			if (pPlayer != NULL)
			{
				pos = pPlayer->Getpos();
				rot = pPlayer->Getrot();
				move = pPlayer->Getmove();
			}

			ImGui::Text("*** Seting ***");

			if (pPlayer != NULL)
			{
				if (ImGui::Button("Click"))pPlayer->SetUpdate(pPlayer->GetUpdate() ? false : true);
				ImGui::SameLine();	ImGui::Text("[Player] Update Stop[%d]", pPlayer->GetUpdate());
			}

			ImGui::DragFloat3("[Player] Initpos", pos, 0.1f, 0.0f, 0.0f, "%.3f", 1.0f);
			ImGui::DragFloat3("[Player] Initrot", rot, 0.1f, 0.0f, 0.0f, "%.3f", 1.0f);
			ImGui::DragFloat3("[Player] pos", pos, 0.1f, 0.0f, 0.0f, "%.3f", 1.0f);
			ImGui::DragFloat3("[Player] rot", rot, 0.002f, 0.0f, 0.0f, "%.3f", 1.0f);
			ImGui::DragFloat3("[Player] move", move, 0.01f, 0.0f, 0.0f, "%.3f", 1.0f);
			ImGui::DragFloat("[Player] jump", &fJump, 0.01f, 0.0f, 0.0f, "%.3f", 1.0f);
			ImGui::DragFloat("[Player] Gravity", &fJump, 0.01f, 0.0f, 0.0f, "%.3f", 1.0f);

			if (pPlayer != NULL)
			{
				pPlayer->Setpos(pos);
			}
		}

		if (ImGui::CollapsingHeader("Enemy"))
		{// 
			ImGui::Text("*** Seting ***");
		}
	}

	// ウィンドウ終了
	ImGui::End();
}

//=============================================================================
// スタイル設定									(public)	*** CImgui_ES ***
//=============================================================================
void CImgui_ES::ShowStyleEditor(ImGuiStyle* ref)
{
	// You can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it compares to an internally stored reference)
	ImGuiStyle& style = ImGui::GetStyle();
	static ImGuiStyle ref_saved_style;

	// Default to using internal storage as reference
	static bool init = true;
	if (init && ref == NULL)
		ref_saved_style = style;
	init = false;
	if (ref == NULL)
		ref = &ref_saved_style;

	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

	if (ImGui::ShowStyleSelector("Colors##Selector"))
		ref_saved_style = style;
	ImGui::ShowFontSelector("Fonts##Selector");

	// Simplified Settings
	if (ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f"))
		style.GrabRounding = style.FrameRounding; // Make GrabRounding always the same value as FrameRounding
	{ bool window_border = (style.WindowBorderSize > 0.0f); if (ImGui::Checkbox("WindowBorder", &window_border)) style.WindowBorderSize = window_border ? 1.0f : 0.0f; }
	ImGui::SameLine();
	{ bool frame_border = (style.FrameBorderSize > 0.0f); if (ImGui::Checkbox("FrameBorder", &frame_border)) style.FrameBorderSize = frame_border ? 1.0f : 0.0f; }
	ImGui::SameLine();
	{ bool popup_border = (style.PopupBorderSize > 0.0f); if (ImGui::Checkbox("PopupBorder", &popup_border)) style.PopupBorderSize = popup_border ? 1.0f : 0.0f; }

	// Save/Revert button
	if (ImGui::Button("Save Ref"))
		*ref = ref_saved_style = style;
	ImGui::SameLine();
	if (ImGui::Button("Revert Ref"))
		style = *ref;
	ImGui::SameLine();
	ShowHelpMarker("Save/Revert in local non-persistent storage. Default Colors definition are not affected. Use \"Export Colors\" below to save them somewhere.");

	if (ImGui::TreeNode("Rendering"))
	{
		ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines); ImGui::SameLine(); ShowHelpMarker("When disabling anti-aliasing lines, you'll probably want to disable borders in your style as well.");
		ImGui::Checkbox("Anti-aliased fill", &style.AntiAliasedFill);
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, FLT_MAX, "%.2f", 2.0f);
		if (style.CurveTessellationTol < 0.10f) style.CurveTessellationTol = 0.10f;
		ImGui::DragFloat("Global Alpha", &style.Alpha, 0.005f, 0.20f, 1.0f, "%.2f"); // Not exposing zero here so user doesn't "lose" the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
		ImGui::PopItemWidth();
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Settings"))
	{
		ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat("PopupRounding", &style.PopupRounding, 0.0f, 16.0f, "%.0f");
		ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
		ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
		ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
		ImGui::SliderFloat("ScrollbarSize", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
		ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
		ImGui::Text("BorderSize");
		ImGui::SliderFloat("WindowBorderSize", &style.WindowBorderSize, 0.0f, 1.0f, "%.0f");
		ImGui::SliderFloat("ChildBorderSize", &style.ChildBorderSize, 0.0f, 1.0f, "%.0f");
		ImGui::SliderFloat("PopupBorderSize", &style.PopupBorderSize, 0.0f, 1.0f, "%.0f");
		ImGui::SliderFloat("FrameBorderSize", &style.FrameBorderSize, 0.0f, 1.0f, "%.0f");
		ImGui::Text("Rounding");
		ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 14.0f, "%.0f");
		ImGui::SliderFloat("ChildRounding", &style.ChildRounding, 0.0f, 16.0f, "%.0f");
		ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 12.0f, "%.0f");
		ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 12.0f, "%.0f");
		ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 12.0f, "%.0f");
		ImGui::Text("Alignment");
		ImGui::SliderFloat2("WindowTitleAlign", (float*)&style.WindowTitleAlign, 0.0f, 1.0f, "%.2f");
		ImGui::SliderFloat2("ButtonTextAlign", (float*)&style.ButtonTextAlign, 0.0f, 1.0f, "%.2f"); ImGui::SameLine(); ShowHelpMarker("Alignment applies when a button is larger than its text content.");
		ImGui::Text("Safe Area Padding"); ImGui::SameLine(); ShowHelpMarker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
		ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&style.DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Colors"))
	{
		static int output_dest = 0;
		static bool output_only_modified = true;
		if (ImGui::Button("Export Unsaved"))
		{
			if (output_dest == 0)
				ImGui::LogToClipboard();
			else
				ImGui::LogToTTY();
			ImGui::LogText("ImVec4* colors = ImGui::GetStyle().Colors;" IM_NEWLINE);
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const ImVec4& col = style.Colors[i];
				const char* name = ImGui::GetStyleColorName(i);
				if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
					ImGui::LogText("colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" IM_NEWLINE, name, 23 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
			}
			ImGui::LogFinish();
		}
		ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY\0"); ImGui::PopItemWidth();
		ImGui::SameLine(); ImGui::Checkbox("Only Modified Colors", &output_only_modified);

		ImGui::Text("Tip: Left-click on colored square to open color picker,\nRight-click to open edit options menu.");

		static ImGuiTextFilter filter;
		filter.Draw("Filter colors", 200);

		static ImGuiColorEditFlags alpha_flags = 0;
		ImGui::RadioButton("Opaque", &alpha_flags, 0); ImGui::SameLine();
		ImGui::RadioButton("Alpha", &alpha_flags, ImGuiColorEditFlags_AlphaPreview); ImGui::SameLine();
		ImGui::RadioButton("Both", &alpha_flags, ImGuiColorEditFlags_AlphaPreviewHalf);

		ImGui::BeginChild("#colors", ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar | ImGuiWindowFlags_NavFlattened);
		ImGui::PushItemWidth(-160);
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName(i);
			if (!filter.PassFilter(name))
				continue;
			ImGui::PushID(i);
			ImGui::ColorEdit4("##color", (float*)&style.Colors[i], ImGuiColorEditFlags_AlphaBar | alpha_flags);
			if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
			{
				// Tips: in a real user application, you may want to merge and use an icon font into the main font, so instead of "Save"/"Revert" you'd use icons.
				// Read the FAQ and misc/fonts/README.txt about using icon fonts. It's really easy and super convenient!
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Save")) ref->Colors[i] = style.Colors[i];
				ImGui::SameLine(0.0f, style.ItemInnerSpacing.x); if (ImGui::Button("Revert")) style.Colors[i] = ref->Colors[i];
			}
			ImGui::SameLine(0.0f, style.ItemInnerSpacing.x);
			ImGui::TextUnformatted(name);
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
		ImGui::EndChild();

		ImGui::TreePop();
	}

	bool fonts_opened = ImGui::TreeNode("Fonts", "Fonts (%d)", ImGui::GetIO().Fonts->Fonts.Size);
	if (fonts_opened)
	{
		ImFontAtlas* atlas = ImGui::GetIO().Fonts;
		if (ImGui::TreeNode("Atlas texture", "Atlas texture (%dx%d pixels)", atlas->TexWidth, atlas->TexHeight))
		{
			ImGui::Image(atlas->TexID, ImVec2((float)atlas->TexWidth, (float)atlas->TexHeight), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
			ImGui::TreePop();
		}
		ImGui::PushItemWidth(100);
		for (int i = 0; i < atlas->Fonts.Size; i++)
		{
			ImFont* font = atlas->Fonts[i];
			ImGui::PushID(font);
			bool font_details_opened = ImGui::TreeNode(font, "Font %d: \'%s\', %.2f px, %d glyphs", i, font->ConfigData ? font->ConfigData[0].Name : "", font->FontSize, font->Glyphs.Size);
			ImGui::SameLine(); if (ImGui::SmallButton("Set as default")) ImGui::GetIO().FontDefault = font;
			if (font_details_opened)
			{
				ImGui::PushFont(font);
				ImGui::Text("The quick brown fox jumps over the lazy dog");
				ImGui::PopFont();
				ImGui::DragFloat("Font scale", &font->Scale, 0.005f, 0.3f, 2.0f, "%.1f");   // Scale only this font
				ImGui::SameLine(); ShowHelpMarker("Note than the default embedded font is NOT meant to be scaled.\n\nFont are currently rendered into bitmaps at a given size at the time of building the atlas. You may oversample them to get some flexibility with scaling. You can also render at multiple sizes and select which one to use at runtime.\n\n(Glimmer of hope: the atlas system should hopefully be rewritten in the future to make scaling more natural and automatic.)");
				ImGui::InputFloat("Font offset", &font->DisplayOffset.y, 1, 1, "%.0f");
				ImGui::Text("Ascent: %f, Descent: %f, Height: %f", font->Ascent, font->Descent, font->Ascent - font->Descent);
				ImGui::Text("Fallback character: '%c' (%d)", font->FallbackChar, font->FallbackChar);
				ImGui::Text("Texture surface: %d pixels (approx) ~ %dx%d", font->MetricsTotalSurface, (int)sqrtf((float)font->MetricsTotalSurface), (int)sqrtf((float)font->MetricsTotalSurface));
				for (int config_i = 0; config_i < font->ConfigDataCount; config_i++)
					if (ImFontConfig* cfg = &font->ConfigData[config_i])
						ImGui::BulletText("Input %d: \'%s\', Oversample: (%d,%d), PixelSnapH: %d", config_i, cfg->Name, cfg->OversampleH, cfg->OversampleV, cfg->PixelSnapH);
				if (ImGui::TreeNode("Glyphs", "Glyphs (%d)", font->Glyphs.Size))
				{
					// Display all glyphs of the fonts in separate pages of 256 characters
					for (int base = 0; base < 0x10000; base += 256)
					{
						int count = 0;
						for (int n = 0; n < 256; n++)
							count += font->FindGlyphNoFallback((ImWchar)(base + n)) ? 1 : 0;
						if (count > 0 && ImGui::TreeNode((void*)(intptr_t)base, "U+%04X..U+%04X (%d %s)", base, base + 255, count, count > 1 ? "glyphs" : "glyph"))
						{
							float cell_size = font->FontSize * 1;
							float cell_spacing = style.ItemSpacing.y;
							ImVec2 base_pos = ImGui::GetCursorScreenPos();
							ImDrawList* draw_list = ImGui::GetWindowDrawList();
							for (int n = 0; n < 256; n++)
							{
								ImVec2 cell_p1(base_pos.x + (n % 16) * (cell_size + cell_spacing), base_pos.y + (n / 16) * (cell_size + cell_spacing));
								ImVec2 cell_p2(cell_p1.x + cell_size, cell_p1.y + cell_size);
								const ImFontGlyph* glyph = font->FindGlyphNoFallback((ImWchar)(base + n));
								draw_list->AddRect(cell_p1, cell_p2, glyph ? IM_COL32(255, 255, 255, 100) : IM_COL32(255, 255, 255, 50));
								if (glyph)
									font->RenderChar(draw_list, cell_size, cell_p1, ImGui::GetColorU32(ImGuiCol_Text), (ImWchar)(base + n)); // We use ImFont::RenderChar as a shortcut because we don't have UTF-8 conversion functions available to generate a string.
								if (glyph && ImGui::IsMouseHoveringRect(cell_p1, cell_p2))
								{
									ImGui::BeginTooltip();
									ImGui::Text("Codepoint: U+%04X", base + n);
									ImGui::Separator();
									ImGui::Text("AdvanceX: %.1f", glyph->AdvanceX);
									ImGui::Text("Pos: (%.2f,%.2f)->(%.2f,%.2f)", glyph->X0, glyph->Y0, glyph->X1, glyph->Y1);
									ImGui::Text("UV: (%.3f,%.3f)->(%.3f,%.3f)", glyph->U0, glyph->V0, glyph->U1, glyph->V1);
									ImGui::EndTooltip();
								}
							}
							ImGui::Dummy(ImVec2((cell_size + cell_spacing) * 16, (cell_size + cell_spacing) * 16));
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
			ImGui::PopID();
		}
		static float window_scale = 1.0f;
		ImGui::DragFloat("this window scale", &window_scale, 0.005f, 0.3f, 2.0f, "%.1f");              // scale only this window
		ImGui::DragFloat("global scale", &ImGui::GetIO().FontGlobalScale, 0.005f, 0.3f, 2.0f, "%.1f"); // scale everything
		ImGui::PopItemWidth();
		ImGui::SetWindowFontScale(window_scale);
		ImGui::TreePop();
	}

	ImGui::PopItemWidth();
}

//=============================================================================
// ヘルプ設定									(public)	*** CImgui_ES ***
//=============================================================================
void CImgui_ES::ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

//=============================================================================
// 文字設定									(public)	*** CImgui_ES ***
//=============================================================================
void CImgui_ES::SetMessage(char *aMessage, ...)
{
	char cMessage[128];		// 文字受け取り

	// 可変
	va_list arg;
	va_start(arg, aMessage);
	vsprintf(cMessage, aMessage, arg);
	va_end(arg);

	// ログ追加
	m_AppLog.AddLog("%s", cMessage);
}