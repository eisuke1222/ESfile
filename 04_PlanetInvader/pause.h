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
#define PAUSE_MAX_BG		(4)			// �|�[�Y�̍ő吔
#define PAUSE_MAX_SELECT	(3)			// �|�[�Y�̍ő吔
#define PAUSE_SIZE_X		(160)		// �傫���iX)
#define PAUSE_SIZE_Y		(50)		// �傫���iY)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPause : public CScene
{// �|�[�Y�i�e�FCScene�j
public:
	CPause();									// �R���X�g���N�^
	~CPause();									// �f�X�g���N�^

	typedef enum
	{
		TEXTURE_PAUSE_BG = 0,
		TEXTURE_PAUSE,
		TEXTURE_TITLE,
		TEXTURE_MAX
	}TEXTURE;

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CPause *Create(void);				// ����

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TEXTURE_MAX];		// �e�N�X�`�����ւ̃|�C���^
	CScene2D *m_apScene2D[PAUSE_MAX_BG];					// �V�[��2D
	CScene2D *m_apScene2DSelect[PAUSE_MAX_SELECT];			// �V�[��2D

	int m_nSelectNum;							// �I��ԍ�
	int m_nCntTimer;							// ���ԃJ�E���^

protected:

};

#endif