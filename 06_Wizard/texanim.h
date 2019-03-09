//=============================================================================
//
// �e�N�X�`���A�j���[�V�������� [texanim.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _TEXANIM_H_
#define _TEXANIM_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene2d.h"	// �V�[��2D
#include "scene3d.h"	// �V�[��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTexAnim : public CScene
{// �e�N�X�`���A�j���[�V�����i�e�FCScene�j
public:
	typedef enum
	{// ���
		TYPE_2D = 0,	// 2D
		TYPE_3D,		// 3D
		TYPE_3D_BILL,	// 3D_Bill
		TYPE_MAX		// �ő吔
	}TYPE;

	CTexAnim();												// �R���X�g���N�^
	virtual ~CTexAnim();									// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

															// �N���G�C�g
	static CTexAnim *Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXVECTOR2 texUV, D3DXCOLOR col, LPDIRECT3DTEXTURE9 pTexture);

	virtual HRESULT Init(void) = 0;							// ����������
	virtual void Uninit(void);								// �I������
	virtual void Update(void) = 0;							// �X�V����
	virtual void Draw(void) = 0;							// �`�揈��

	// �A�j���[�V�����ݒ�
	void SetAnim(int nMaxAnim, int nMaxLine, int nSpped = 0, int nPattern = 0, bool bLoop = false, bool bAutoAnim = false)
	{
		m_bAutoAnim = bAutoAnim;	// �����A�j���[�V����
		m_bLoop		= bLoop;		// ���[�v
		m_nMaxAnim	= nMaxAnim;		// �A�j���[�V������
		m_nMaxLine	= nMaxLine;		// ���C����
		m_nCntAnim	= 0;			// �A�j���[�V�����J�E���g	
		m_nSpeed	= nSpped;		// �A�j���[�V�������x
		m_nPattern	= nPattern;		// �p�^�[����
	}

private:

protected:
	int					m_nMax;								// �ő吔
	D3DXVECTOR3			m_pos;								// �ʒu
	D3DXVECTOR3			m_move;								// �ړ���
	D3DXVECTOR3			m_size;								// �傫��
	D3DXVECTOR2			m_texUV;							// UV			
	D3DXCOLOR			m_col;								// �F

	LPDIRECT3DTEXTURE9	m_pTexture;					
	bool				m_bAutoAnim;						// �����A�j���[�V����
	bool				m_bLoop;							// ���[�v
	int					m_nMaxAnim;							// �A�j���[�V������
	int					m_nMaxLine;							// ���C����
	int					m_nCntAnim;							// �A�j���[�V�����J�E���g	
	int					m_nSpeed;							// �A�j���[�V�������x
	int					m_nPattern;							// �p�^�[����

	bool				m_bBillBoard;						// �r���{�[�h��
};

class CAnim2D : public CTexAnim
{// �A�j���[�V����2D�i�e�FCTexAnim�j
public:
	CAnim2D();												// �R���X�g���N�^
	~CAnim2D();												// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

private:
	CScene2D			*m_pAnim2D;							// �A�j���[�V����2D

protected:

};

class CAnim3D : public CTexAnim
{// �A�j���[�V����3D�i�e�FCTexAnim�j
public:
	CAnim3D();												// �R���X�g���N�^
	~CAnim3D();												// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

private:
	CScene3D			*m_pAnim3D;							// �A�j���[�V����3D

protected:

};

#endif