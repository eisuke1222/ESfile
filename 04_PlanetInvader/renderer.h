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

//*****************************************************************************
// �}�N����`
//*****************************************************************************

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

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// �f�o�C�X�擾

private:
#ifdef _DEBUG
	void DrawFPS(void);							// FPS�`�揈��
#endif

	LPDIRECT3D9				m_pD3D;				// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;		// Device�I�u�W�F�N�g(�`��ɕK�v)

#ifdef _DEBUG
	LPD3DXFONT				m_pFont = NULL;		// �t�H���g�ւ̃|�C���^
#endif

protected:

};

#endif