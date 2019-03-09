//=============================================================================
//
// �|�[�Y���� [pause.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;			// �V�[���QD

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPause : public CScene
{// �|�[�Y�i�e�FCScene�j
public:
	CPause();									// �R���X�g���N�^
	~CPause();									// �f�X�g���N�^

	typedef enum
	{// ���
		TYPE_BG = 0,			// �w�i
		TYPE_FRAME,				// �g
		TYPE_LINE,				// ��
		TYPE_MENU,				// ���j���[
		TYPE_MAX				// �ő吔
	}TYPE;

	typedef enum
	{// �I��
		SELECT_CONTINUE = 0,	// �R���e�j���[
		SELECT_RETRY,			// ���g���C
		SELECT_QUIT,			// �N�C�b�g
		SELECT_MAX				// �ő吔
	}SELECT;

	typedef enum
	{// �}�[�N
		MARK_LEFT = 0,
		MARK_RIGHT,
		MARK_MAX
	}MARK;

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CPause *Create(void);				// ����

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	CScene2D *m_apPauseBg[TYPE_MAX];			// �|�[�Y�w�i
	CScene2D *m_apSelect[SELECT_MAX];			// �I��
	
	int m_nSelectNum;							// �I��ԍ�
	int m_nCntTimer;							// ���ԃJ�E���^

	CScene2D *m_pSelectChange;					// �I��ω�
	CScene2D *m_apMark[MARK_MAX];				// �I���}�[�N

protected:

};

#endif