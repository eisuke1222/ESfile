//=============================================================================
//
// �V�[��2D���� [scene2d.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

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
class CScene2D : public CScene
{// �V�[��2D�i�e�FCScene�j
public:
	CScene2D(int nPriority, CScene::OBJTYPE objType);			// �R���X�g���N�^
	~CScene2D();												// �f�X�g���N�^

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)				// �e�N�X�`�����f
		{ m_pTexture = pTexture; }
	
	HRESULT Init(void);											// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��

																// �`��ҏW
	void DrawEdit(bool bAdd, bool bSubtract, bool bATest, int nATest = 0)
	{
		m_bDrawAdd		= bAdd;									// ���Z����
		m_bDrawSubtract	= bSubtract;							// ���Y����
		m_bDrawATest	= bATest;								// ���e�X�g
		m_nATest		= nATest;								// ���e�X�g���l
	}

																// �����l�ݒ�
	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV)
	{
		m_pos			= pos;									// �ʒu							
		m_size			= size;									// �傫��
		m_col			= col;									// �F
		m_TexUV			= TexUV;								// UV
	}

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }			// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }			// �ʒu�擾
	void Setrot(float rot)			{ m_rot = rot; }			// �����ݒ�
	float Getrot(void)				{ return m_rot; }			// �����擾
	void Setsize(D3DXVECTOR3 size)	{ m_size = size; }			// �傫���ݒ�
	D3DXVECTOR3 Getsize(void)		{ return m_size; }			// �傫���擾
	void Setcol(D3DXCOLOR col)		{ m_col = col; }			// �F�ݒ�
	D3DXCOLOR Getcol(void)			{ return m_col; }			// �F�擾
	void SetTexUV(D3DXVECTOR2 TexUV){ m_TexUV = TexUV; }		// �F�ݒ�
	D3DXVECTOR2 GetTexUV(void)		{ return m_TexUV; }			// �F�擾

																// �A�j���[�V�������ݒ�
	void SetTex(int nAnimLineNum, int nAnimPattern, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV);

private:
	LPDIRECT3DTEXTURE9		m_pTexture;							// �e�N�X�`�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// ���_�o�b�t�@���ւ̃|�C���^

	D3DXVECTOR3				m_pos;								// �ʒu
	float					m_rot;								// ����
	D3DXVECTOR3				m_size;								// �傫��
	D3DXCOLOR				m_col;								// �F
	D3DXVECTOR2				m_TexUV;							// UV

	bool					m_bDrawAdd;							// ���Z����
	bool					m_bDrawSubtract;					// ���Y����
	bool					m_bDrawATest;						// ���e�X�g
	int						m_nATest;							// ���e�X�g���l

protected:

};

class CScene2D_Preset : public CScene2D
{// �V�[��2D�v���Z�b�g�i�e�FCScene2D�j
public:
	typedef enum
	{// ���
		TYPE_NONE = 0,	// �����Ȃ�
		TYPE_COL,		// �F
		TYPE_SIZE,		// �傫��
		TYPE_FLASING,	// �_��
		TYPE_MAX		// �ő吔
	}TYPE;

	CScene2D_Preset(int nPriority, CScene::OBJTYPE objType);	// �R���X�g���N�^
	~CScene2D_Preset();											// �f�X�g���N�^

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)				// �e�N�X�`�����f
		{ CScene2D::BindTexture(pTexture); }

	HRESULT Init(void);											// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��

	void SetcolType(int nMaxFrame, D3DXCOLOR colDest)			// �F�ݒ�
	{ for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++) 
		{ if (m_nType[nCntType] == TYPE_NONE) 
			{ m_nType[nCntType] = TYPE_COL; m_nCntFrame[nCntType] = 0; m_nMaxFrame[nCntType] = nMaxFrame; m_colDest_1[nCntType] = colDest;  break; }
		}
	}
	void SetSizeType(int nMaxFrame, D3DXVECTOR3 sizeDest)		// �傫���ݒ�
	{ for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++) 
		{ if (m_nType[nCntType] == TYPE_NONE) 
			{ m_nType[nCntType] = TYPE_SIZE; m_nCntFrame[nCntType] = 0; m_nMaxFrame[nCntType] = nMaxFrame; m_sizeDest[nCntType] = sizeDest;  break; }
		}
	}

private:
	void Preset(void);											// �v���Z�b�g

	int						m_nType[TYPE_MAX];					// ���				
	int						m_nCntFrame[TYPE_MAX];				// �t���[���J�E���g
	int						m_nMaxFrame[TYPE_MAX];				// �t���[���̍ő�l
	D3DXCOLOR				m_colDest_1[TYPE_MAX];				// �ڕW�̐F1
	D3DXCOLOR				m_colDest_2[TYPE_MAX];				// �ڕW�̐F2
	D3DXVECTOR3				m_sizeDest[TYPE_MAX];				// �ڕW�̑傫��

protected:

};

#endif