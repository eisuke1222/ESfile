//=============================================================================
//
// �`���[�g���A������ [tutorial.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;			// �V�[��2D
class CPause;			// �|�[�Y

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTutorial
{// �`���[�g���A��
public:
	CTutorial();					// �R���X�g���N�^
	~CTutorial();					// �f�X�g���N�^

	static HRESULT Load(void);		// ���[�h
	static void Unload(void);		// �A�����[�h

	HRESULT Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static void SetPause(bool bPause);					// �|�[�Y�ݒ�
	static bool GetPause(void) { return m_bPause; }	// �|�[�Y�擾

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;						// �e�N�X�`�����ւ̃|�C���^
	static CScene2D					*m_pScene2D[MAX_PLAYER];		// �V�[��2D

	static CPause *m_pPause;		// �|�[�Y
	static bool m_bPause;			// �|�[�Y���Ă��邩

	void SetMap(void);				// �}�b�v�ݒ�

	int m_mapfield;					// �t�B�[���h�ԍ�
	int m_mapfieldOld;				// �t�B�[���h�ԍ�

protected:

};

#endif