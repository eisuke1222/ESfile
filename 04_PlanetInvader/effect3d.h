//=============================================================================
//
// �G�t�F�N�g3D���� [effect3d.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _EFFECT3D_H_
#define _EFFECT3D_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "model3d.h"	// ���f��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffect3D : public CModel3D
{// �I�u�W�F�N�g�i�e�FCModel3D�j
public:
	CEffect3D();									// �R���X�g���N�^
	~CEffect3D();									// �f�X�g���N�^

	static HRESULT Load(void);						// ���[�h
	static void Unload(void);						// �A�����[�h
	static CEffect3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 rotAdd, D3DXVECTOR3 move);		// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 rotAdd, D3DXVECTOR3 move);					// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

private:
	static LPDIRECT3DTEXTURE9	*m_pTexture;		// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			m_pMesh;			// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			m_pBuffMat;			// �}�e���A�����ւ̃|�C���^
	static DWORD				m_nNumMat;			// �}�e���A�����̐�

	int			m_nID;								// �eID
	int			m_nCntTimer;						// ���ԃJ�E���^
	D3DXVECTOR3 m_rotAdd;							// ����
	D3DXVECTOR3 m_move;								// �ړ���
	bool		m_bStop;							// �~�܂�
protected:

};

#endif