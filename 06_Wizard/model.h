//=============================================================================
//
// ���f������ [model.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CModel
{// ���f��
public:
	typedef struct
	{// ���f�����
		char cFileName[256];		// �t�@�C����
		int nIndex;					// �e�ԍ�
		D3DXVECTOR3 pos;			// �ʒu
		D3DXVECTOR3 rot;			// ����
	}MODEL_INFO;

	CModel();											// �R���X�g���N�^
	~CModel();											// �f�X�g���N�^

	HRESULT Init(void);									// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	void SetInitAll(LPDIRECT3DTEXTURE9 *pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	{// �����l�ݒ�
		m_pTexture = pTexture;		// �e�N�X�`��
		m_pMesh = pMesh;			// ���b�V��
		m_pBuffMat = pBuffMat;		// �}�e���A���o�b�t�@
		m_nNumMat = nNumMat;		// �}�e���A����
		m_Initpos = pos;			// �����ʒu
		m_pos = pos;				// �ʒu
		m_Initrot = rot;			// ��������
		m_rot = rot;				// ����
	}

	D3DXMATRIX &GetmtxWorld(void)						// ���[���h�}�g���b�N�X�擾
		{ return m_mtxWorld; }		
	void SetParent(CModel *pModel)						// �e���f���ݒ�
		{ m_pParent = pModel; }		
	CModel *GetParent(void) { return m_pParent; }

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// �ʒu�擾
	void Setrot(D3DXVECTOR3 rot)	{ m_rot = rot; }	// �����ݒ�
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// �����擾

	D3DXVECTOR3 GetInitpos(void) { return m_Initpos; }
	D3DXVECTOR3 GetInitrot(void) { return m_Initrot; }

	D3DXVECTOR3 Getsize(int nNum)						// �傫���擾
		{ if (nNum == 0) return m_vtxMax;	else return m_vtxMin; }		
	void Setcol_rgb(D3DXCOLOR col)						// �F�ݒ�rgb
		{ m_col = col; m_bcolChange_rgb = true; }					
	void Setcol_a(float col_a)							// �F�ݒ�a
		{ m_col.a = col_a; m_bcolChange_a = true; }	

private:
	LPDIRECT3DTEXTURE9	*m_pTexture;					// �e�N�X�`�����ւ̃|�C���^
	LPD3DXMESH			m_pMesh;						// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		m_pBuffMat;						// �}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMat;						// �}�e���A�����̐�
	D3DXMATRIX			m_mtxWorld;						// ���[���h�}�g���b�N�X

	CModel				*m_pParent;						// �e�}�g���b�N�X

	D3DXVECTOR3			m_vtxMin;						// ���_�̍ŏ��l
	D3DXVECTOR3			m_vtxMax;						// ���_�̍ő�l

	D3DXVECTOR3			m_Initpos;						// �����ʒu
	D3DXVECTOR3			m_pos;							// �ʒu
	D3DXVECTOR3			m_Initrot;						// ��������
	D3DXVECTOR3			m_rot;							// ����
	D3DXCOLOR			m_col;							// �F
	bool				m_bcolChange_rgb;				// �F�ύXrgb
	bool				m_bcolChange_a;					// �F�ύXa

protected:

};

#endif