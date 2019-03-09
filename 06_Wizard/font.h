//=============================================================================
//
// �t�H���g���� [font.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _FONT_H_
#define _FONT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFont
{// �t�H���g
public:
	typedef enum
	{// �t�H���g�ԍ�
		FONTNUM_COMMAND = 0,		// �R�}���h
		FONTNUM_LOG,				// ���O
		FONTNUM_DATA,				// �f�[�^
		FONTNUM_DEBUG,				// �f�o�b�O
		FONTNUM_TEST,				// �e�X�g
		FONTNUM_MAX					// �ő吔
	}FONTNUM;

	typedef enum
	{// �Z�b�g�^�C�v
		SETTYPE_NEW = 0,			// �㏑��
		SETTYPE_ADD,				// �ǉ��i���j
		SETTYPE_LOG,				// �ǉ��i��j
		SETTYPE_MAX					// �ő吔
	}SETTYPE;

	static const int m_MAX_MESSAGE = 1024;		// ���b�Z�[�W�ő吔

	CFont();									// �R���X�g���N�^
	~CFont();									// �f�X�g���N�^

												// �N���G�C�g
	static CFont *Create(int nHeight, int nWidth, int nWeight, RECT rect, UINT uFormat, D3DXCOLOR col);	

	static void ReleaseAll(void);				// �S�j��
	static void DrawAll(void);					// �S�`��

	static bool GetDispAll(void)				// �t�H���g�`��
		{ return m_bDispAll; }
												// ����������
	HRESULT Init(int nHeight, int nWidth, int nWeight, RECT rect, UINT uFormat, D3DXCOLOR col);				
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

												// �����ݒ�
	static void SetMessageData(CScene::OBJTYPE objType, SETTYPE nSetType, char *aMessage, ...);				
												// �����擾
	static void *GetMessageData(CScene::OBJTYPE objType)		
		{ return m_MessageData[objType]; }

												// �����ݒ�
	void SetMessage(SETTYPE nSetType, char *aMessage, ...);		
	char *GetMessage(void)						// �����擾
		{ return m_Message; }	

	void SetDisp(bool bDisp)					// �\���ݒ� 
		{ m_bDisp = bDisp; }
	bool GetDisp(void)							// �\���擾
		{ return m_bDisp; }

private:
	static CFont	*m_apFont[FONTNUM_MAX];									// �V�[��
	static bool		m_bDispAll;												// �\��
	static char		m_MessageData[CScene::OBJTYPE_MAX][m_MAX_MESSAGE];		// ���b�Z�[�W

	int				m_nID;													// ID�ԍ�
	LPD3DXFONT		m_pFont = NULL;											// �t�H���g�ւ̃|�C���^

	bool			m_bDisp;												// �\��
	char			m_Message[m_MAX_MESSAGE];								// ���b�Z�[�W
	RECT			m_rect;													// �͈�
	UINT			m_uFormat;												// �t�H�[�}�b�g
	D3DXCOLOR		m_col;													// �F

protected:
	void Release(void);							// �J������

};

#endif