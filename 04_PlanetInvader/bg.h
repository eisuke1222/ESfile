//=============================================================================
//
// �w�i���� [bg.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_SIZE_X		(10)		// �傫���iX)
#define BG_SIZE_Y		(10)		// �傫���iY)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBg : public CScene
{// �w�i�i�e�FCScene�j
public:
	CBg();										// �R���X�g���N�^
	~CBg();										// �f�X�g���N�^

	typedef enum
	{// �e�N�X�`��
		TEXTURE_TUTORIAL = 0,
		TEXTURE_GAME,
		TEXTURE_MAX
	}TEXTURE;

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CBg *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nSetNum, int nTexNum);		// ����

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[TEXTURE_MAX];		// �e�N�X�`�����ւ̃|�C���^
	CScene2D *m_apScene2D[TEXTURE_MAX];			// �V�[��2D

	D3DXVECTOR2 m_posUV[TEXTURE_MAX];			// �ʒu
	static int m_nSetNum;						// �Z�b�g��


protected:

};

#endif