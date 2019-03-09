//=============================================================================
//
// �V�[��3D���� [scene3d.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

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

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene3D : public CScene
{// �V�[��3D�i�e�FCScene�j
public:
	CScene3D(int nPriority);					// �R���X�g���N�^
	CScene3D(int nPriority, int nID);			// �R���X�g���N�^
	~CScene3D();								// �f�X�g���N�^
	
	typedef enum
	{// �V�[��3D�^�C�v
		SCENE3DTYPE_NORMAL = 0,		// �ʏ�
		SCENE3DTYPE_BILLBOARD,		// �r���{�[�h
		SCENE3DTYPE_MAX
	}SCENE3DTYPE;

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }		// �e�N�X�`�����f
	
	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType)
	{// �����l�ݒ�
		m_pos			= pos;					// �ʒu
		m_rot			= rot;					// ����
		m_size			= size;					// �傫��
		m_col			= col;					// �F
		m_TexUV			= TexUV;				// �e�N�X�`��UV
		m_scene3dType	= scene3dType;			// �V�[��3D�^�C�v
	}

	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }		// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void) { return m_pos; }			// �ʒu�擾
	void Setsize(D3DXVECTOR3 size) { m_size = size; }	// �傫���ݒ�
	D3DXVECTOR3 Getsize(void) { return m_size; }		// �傫���擾
	void Setcol(D3DXCOLOR col) { m_col = col; }			// �F�ݒ�
	D3DXCOLOR Getcol(void) { return m_col; }			// �F�擾

	void SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV);		// �A�j���[�V�������ݒ�
	D3DXVECTOR2 GetTex(void) { return m_TexUV; }		// UV�擾

private:
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@���ւ̃|�C���^

	D3DXMATRIX	m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_rot;							// ����
	D3DXVECTOR3 m_size;							// �傫��
	D3DXCOLOR m_col;							// �F
	D3DXVECTOR2 m_TexUV;						// UV

	SCENE3DTYPE m_scene3dType;					// �V�[��3D�^�C�v

protected:

};

#endif