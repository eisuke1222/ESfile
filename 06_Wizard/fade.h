//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene2d.h"	// �V�[��2D
#include "game.h"		// �Q�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFade : public CScene2D
{// �t�F�[�h�i�e�FCScene2D�j
public:
	typedef enum
	{// �t�F�[�h���
		FADE_NONE = 0,		// �������Ă��Ȃ����
		FADE_IN,			// �t�F�[�h�C�����
		FADE_OUT,			// �t�F�[�h�A�E�g���
		FADE_MAX			// �ő吔
	} FADE;

	CFade();												// �R���X�g���N�^
	~CFade();												// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

															// �N���G�C�g
	static CFade *Create(CManager::MODE mode, FADE fade = FADE_OUT);
	static CFade *Create(CGame::GAME game, FADE fade = FADE_OUT);

	HRESULT Init(CManager::MODE mode, FADE fade);			// ����������
	HRESULT Init(CGame::GAME game, FADE fade);				// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static FADE GetFade(void)								// �t�F�[�h�擾
		{ return m_fade; }			

private:
	static FADE		m_fade;									// �t�F�[�h���

	int				m_fadeNum;								// �t�F�[�h�ԍ�
	CManager::MODE	m_modeNext;								// ���̃��[�h
	CGame::GAME		m_gameNext;								// ���̃Q�[��
	int				m_nCntTimer;							// ���ԃJ�E���g

protected:

};

#endif