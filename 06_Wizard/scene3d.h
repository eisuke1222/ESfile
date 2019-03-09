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
	typedef enum
	{// �V�[��3D�^�C�v
		SCENE3DTYPE_NORMAL = 0,		// �ʏ�
		SCENE3DTYPE_BILLBOARD,		// �r���{�[�h
		SCENE3DTYPE_MAX				// �ő吔
	}SCENE3DTYPE;

	CScene3D(int nPriority, CScene::OBJTYPE objType);	// �R���X�g���N�^
	~CScene3D();										// �f�X�g���N�^

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)		// �e�N�X�`�����f
		{ m_pTexture = pTexture; }		

	HRESULT Init(void);									// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	// �`��ҏW
	void DrawEdit(bool bAdd, bool bSubtract, bool bATest, bool bZWrite, bool bLight, int nATest = 0, int nCullMode = 3, DWORD dCmpValue = D3DCMP_LESSEQUAL)
	{
		m_bDrawAdd		= bAdd;					// ���Z����
		m_bDrawSubtract	= bSubtract;			// ���Y����
		m_bDrawATest	= bATest;				// ���e�X�g
		m_bDrawZWrite	= bZWrite;				// Z���C�g
		m_bDrawLight	= bLight;				// ���C�g�e��
		m_nATest		= nATest;				// ���e�X�g���l
		m_nCullMode		= nCullMode;			// �J�����O
		m_dCmpValue		= dCmpValue;			// Z���C�g�ݒ�
	}

	// �����l�ݒ�
	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType)
	{
		m_pos			= pos;					// �ʒu
		m_rot			= rot;					// ����
		m_size			= size;					// �傫��
		m_col			= col;					// �F
		m_TexUV			= TexUV;				// �e�N�X�`��UV
		m_scene3dType	= scene3dType;			// �V�[��3D�^�C�v
	}

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// �ʒu�擾
	void Setrot(D3DXVECTOR3 rot)	{ m_rot = rot; }	// �����ݒ�
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// �����擾
	void Setsize(D3DXVECTOR3 size)	{ m_size = size; }	// �傫���ݒ�
	D3DXVECTOR3 Getsize(void)		{ return m_size; }	// �傫���擾
	void Setcol(D3DXCOLOR col)		{ m_col = col; }	// �F�ݒ�
	D3DXCOLOR Getcol(void)			{ return m_col; }	// �F�擾

	void SetVtxSize(D3DXVECTOR3 *VtxSize);				// ���_�ʒu�A�傫������
	void GetVtxSize(D3DXVECTOR3 *VtxSize);				// ���_�ʒu�A�傫���擾

	void SetBillRot(float rot);							// �r���{�[�h��]

														// �A�j���[�V�������ݒ�
	void SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV);		
	D3DXVECTOR2 GetTex(void)		{ return m_TexUV; }	// UV�擾

	D3DXVECTOR3 GetVtx(int nNum);						// ���_
	D3DXVECTOR3 GetNor(int nNum);						// �@��

	float GetHeight(D3DXVECTOR3 pos);					// �����擾

	void SetDraw(bool bDraw)	{ m_bDraw = bDraw; }	// �`��ݒ�
	bool GetDraw(void)			{return m_bDraw; }		// �`��擾

private:
	LPDIRECT3DTEXTURE9		m_pTexture;					// �e�N�X�`�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// ���_�o�b�t�@���ւ̃|�C���^

	D3DXMATRIX				m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3				m_pos;						// �ʒu
	D3DXVECTOR3 			m_rot;						// ����
	D3DXVECTOR3 			m_size;						// �傫��
	D3DXCOLOR				m_col;						// �F
	D3DXVECTOR2 			m_TexUV;					// UV
							
	bool					m_bVtxSize;					// ���_�ʒu�A�傫����������
							
	bool					m_bDraw;					// �`��

	bool					m_bDrawAdd;					// ���Z����
	bool					m_bDrawSubtract;			// ���Y����
	bool					m_bDrawATest;				// ���e�X�g
	bool					m_bDrawZWrite;				// Z���C�g
	bool					m_bDrawLight;				// ���C�g�e��
	int						m_nATest;					// ���e�X�g���l
	int						m_nCullMode;				// �J�����O
	DWORD					m_dCmpValue;				// Z���C�g�ݒ�
							
	SCENE3DTYPE 			m_scene3dType;				// �V�[��3D�^�C�v

protected:

};

#endif