//=============================================================================
//
// IMGUI���� [imgui_es.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _IMGUI_ES_H_
#define _IMGUI_ES_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
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

	CImgui_ES();									// �R���X�g���N�^
	~CImgui_ES();									// �f�X�g���N�^

													// ���O�E�B���h�E
	static void ShowAppLog(bool* p_open = NULL, ImGuiWindowFlags flags = 0);
													// �f�o�b�O�E�B���h�E
	static void ShowAppDebug(bool* p_open = NULL, ImGuiWindowFlags flags = 0);

													// �X�^�C���ύX
	static IMGUI_API void ShowStyleEditor(ImGuiStyle* ref = NULL);
	static void ShowHelpMarker(const char* desc);	// �w���v�\��

	static void SetMessage(char *aMessage, ...);	// �����ݒ�

private:
	static AppLog m_AppLog;

protected:

};

#endif