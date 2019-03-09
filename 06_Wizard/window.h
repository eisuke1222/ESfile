//=============================================================================
//
// �E�B���h�E���� [window.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _window_H_
#define _window_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene2d.h"	// �V�[��2D

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CWindow : public CScene
{// �E�B���h�E�i�e�FCScene�j
public:
	typedef enum
	{// ���
		WINDOW_YESNO = 0,	// YESNO
		WINDOW_MAX			// �ő吔
	}WINDOW;

	CWindow();												// �R���X�g���N�^
	virtual ~CWindow();										// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

															// �N���G�C�g
	static CWindow *Create(WINDOW window);

	virtual HRESULT Init(void) = 0;							// ����������
	virtual void Uninit(void);								// �I������
	virtual void Update(void) = 0;							// �X�V����
	virtual void Draw(void) = 0;							// �`�揈��

private:

protected:

};

class CWindow_YESNO : public CWindow
{// �E�B���h�E�iYESNO�j�i�e�FCEffect�j
public:
	typedef enum
	{
		TYPE_BG = 0,
		TYPE_MESSAGE_BG,
		TYPE_MESSAGE,
		TYPE_SELECT_BG,
		TYPE_YES,
		TYPE_NO,
		TYPE_MAX
	}TYPE;

	CWindow_YESNO();										// �R���X�g���N�^
	~CWindow_YESNO();										// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	int GetSelectNum(void) { return m_nSelectNum; }			// �I��ԍ��擾

private:
	CScene2D	*m_pScene2D[TYPE_MAX];						// �V�[��2D

	int			m_nSelectNum;								// �I��ԍ�

protected:

};

#endif