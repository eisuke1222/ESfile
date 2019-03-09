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
#define MAX_MESSAGE		(1024)		// ���b�Z�[�W�ő吔

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
		FONTNUM_MAX
	}FONTNUM;

	typedef enum
	{// �Z�b�g�^�C�v
		SETTYPE_NEW = 0,			// �㏑��
		SETTYPE_ADD,				// �ǉ��i���j
		SETTYPE_LOG,				// �ǉ��i��j
		SETTYPE_MAX
	}SETTYPE;

	CFont();						// �R���X�g���N�^
	~CFont();						// �f�X�g���N�^

	static CFont *Create(int nHeight, int nWidth, int nWeight, RECT rect, UINT uFormat, D3DXCOLOR col);		// �N���G�C�g

	static void ReleaseAll(void);				// �S�j��
	static void DrawAll(void);					// �S�`��

	static bool GetDispAll(void) { return m_bDispAll; }			// �t�H���g�`��

	HRESULT Init(int nHeight, int nWidth, int nWeight, RECT rect, UINT uFormat, D3DXCOLOR col);				// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

												// �����i�ݒ�A�擾�j
	static void SetMessageData(CScene::OBJTYPE objType, SETTYPE nSetType, char *aMessage, ...);
	static void *GetMessageData(CScene::OBJTYPE objType) { return m_MessageData[objType]; }

	// �����i�ݒ�A�擾�j
	void SetMessage(SETTYPE nSetType, char *aMessage, ...);
	char *GetMessage(void) { return m_Message; }

	// �\���i�ݒ�A�擾�j
	void SetDisp(bool bDisp) { m_bDisp = bDisp; }
	bool GetDisp(void) { return m_bDisp; }

private:
	static CFont *m_apFont[FONTNUM_MAX];		// �V�[��
	static bool m_bDispAll;						// �\��
	static char m_MessageData[CScene::OBJTYPE_MAX][MAX_MESSAGE];

	int m_nID;									// ID�ԍ�
	LPD3DXFONT	m_pFont = NULL;					// �t�H���g�ւ̃|�C���^

	bool m_bDisp;								// �\��
	char m_Message[MAX_MESSAGE];				// ���b�Z�[�W
	RECT m_rect;								// �͈�
	UINT m_uFormat;								// �t�H�[�}�b�g
	D3DXCOLOR m_col;							// �F

protected:
	void Release(void);							// �J������

};

#endif