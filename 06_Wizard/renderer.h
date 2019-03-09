//=============================================================================
//
// �����_���[���� [renderer.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "font.h"		// �t�H���g

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CFont;	// �t�H���g

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRenderer
{// �����_���[
public:
	CRenderer();								// �R���X�g���N�^
	~CRenderer();								// �f�X�g���N�^

	HRESULT Init(HWND hWnd, bool bWindow);		// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	LPDIRECT3DDEVICE9 GetDevice(void)			// �f�o�C�X�擾 
		{ return m_pD3DDevice; }	

	static CFont *GetFont(int nFontNum)			// �t�H���g�擾
		{ return m_pFont[nFontNum]; }	

	static void SetImGuiWindow(bool bImGuiWindow)	// ImGui�`��ݒ�
		{ m_bImGuiWindow = bImGuiWindow; }
	static bool GetImGuiWindow(void)				// ImGui�`��擾
		{ return m_bImGuiWindow; }


	static void SetDrawAll(bool bDrawAll)		// �S�`��ݒ�
		{ m_bDrawAll = bDrawAll; }

	static void SetDrawMap(bool bDrawMap)		// �}�b�v�`��ݒ�
		{ m_bDrawMap = bDrawMap; }

private:
	LPDIRECT3D9				m_pD3D;				// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;		// Device�I�u�W�F�N�g(�`��ɕK�v)

	static CFont			*m_pFont[CFont::FONTNUM_MAX];	// �t�H���g�ւ̃|�C���^
	static bool				m_bImGuiWindow;		// ImGui�\��
	static bool				m_bDrawAll;			// �S�`��
	static bool				m_bDrawMap;			// �}�b�v�`��

protected:

};

#endif