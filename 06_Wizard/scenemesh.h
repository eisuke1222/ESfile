//=============================================================================
//
// �V�[�����b�V������ [sceneMesh.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENEMESH_H_
#define _SCENEMESH_H_

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
class CSceneMesh : public CScene
{// �V�[�����b�V���i�e�FCScene�j
public:
	typedef enum
	{// ���b�V���^�C�v
		MESHTYPE_FIELD = 0,			// �t�B�[���h
		MESHTYPE_WALL,				// �E�H�[��
		MESHTYPE_CYLINDER,			// �V�����_�[
		MESHTYPE_DOME,				// �h�[��
		MESHTYPE_MAX				// �ő吔
	}MESHTYPE;

	typedef enum
	{// �e�N�X�`���^�C�v
		MESHTEX_ALL = 0,			// �S��
		MESHTEX_NUMSIZE,			// ����
		MESHTEX_MAX					// �ő吔
	}MESHTEX;

	typedef enum
	{// �T�C�����
		SINTYPE_CIRCLE = 0,			// �~
		SINTYPE_RIGHT,				// �E
		SINTYPE_LEFT,				// ��
		SINTYPE_UP,					// ��
		SINTYPE_DOWN,				// ��
		SINTYPE_MAX					// �ő吔
	}SINTYPE;

	typedef enum
	{// �J�[�u���S�_
		SINSTART_POS = 0,			// �ʒu
		SINSTART_CENTER,			// �~
		SINSTART_LU,				// ����
		SINSTART_RU,				// �E��
		SINSTART_LD,				// ����
		SINSTART_RD,				// �E��
		SINSTART_MAX				// �ő吔
	}SINSTART;

	CSceneMesh(int nPriority = 3, CScene::OBJTYPE objType = (CScene::OBJTYPE)0);	// �R���X�g���N�^
	~CSceneMesh();										// �f�X�g���N�^

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)		// �e�N�X�`�����f
		{ m_pTexture = pTexture; }		

	HRESULT Init(void);									// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	// �`��ҏW
	void DrawEdit(bool bAdd, bool bSubtract, bool bATest, bool bZWrite, bool bLight, int nATest = 0, int nCullMode = 3, DWORD dCmpValue = D3DCMP_LESSEQUAL)
	{
		m_bDrawAdd		= bAdd;				// ���Z����
		m_bDrawSubtract	= bSubtract;		// ���Y����
		m_bDrawATest	= bATest;			// ���e�X�g
		m_bDrawZWrite	= bZWrite;			// Z���C�g
		m_bDrawLight	= bLight;			// ���C�g�e��
		m_nATest		= nATest;			// ���e�X�g���l
		m_nCullMode		= nCullMode;		// �J�����O
		m_dCmpValue = dCmpValue;			// Z���C�g�ݒ�
	}

														// �����l�ݒ�
	void SetInitAll(MESHTYPE meshType, MESHTEX meshTex, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV);		// �����l�ݒ�

	void Setpos(D3DXVECTOR3 pos)	{ m_pos = pos; }	// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void)		{ return m_pos; }	// �ʒu�擾
	void Setrot(D3DXVECTOR3 rot)	{ m_rot = rot; }	// ��]�ݒ�
	D3DXVECTOR3 Getrot(void)		{ return m_rot; }	// ��]�擾
	D3DXVECTOR3 Getnum(void)		{ return m_num; }	// �u���b�N���擾
	D3DXVECTOR3 Getsize(void)		{ return m_size; }	// �傫���擾

	void SetStart(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV);	// �X�e�[�^�X�ݒ�

	void SetTex(D3DXVECTOR2 TexMoveUV);					// �A�j���[�V�������ݒ�

	void SetVtx(int nNum, D3DXVECTOR3 pos);				// ���_�ݒ�
	D3DXVECTOR3 GetVtx(int nNum);						// ���_�擾
	D3DXVECTOR3 GetNor(int nNum);						// �@���擾
	void SetCol(int nNum, D3DXCOLOR col);				// �F�ݒ�

	int GetMaxVtx(void)			{ return m_nMaxVtx; }	// ���_���擾

	bool ColInMesh(D3DXVECTOR3 pos);					// ���b�V�����ɂ��邩

														// �T�C���J�[�u�X�V
	void SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine);
	void SinUpdate(SINTYPE sinType, SINSTART sinStart, D3DXVECTOR3 pos, float frot, float fUp, float fLine, float fRange);

														// �����ݒ�
	void SetHeight(D3DXVECTOR3 pos, float fValue, float fRange);
	float GetHeight(D3DXVECTOR3 pos);					// �����擾
	float GetUpDown(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// �X���擾
	float GetBlockNor(D3DXVECTOR3 pos);					// �u���b�N�@���擾

	void SetVecNor(void);								// �@���v�Z
private:

	LPDIRECT3DTEXTURE9			m_pTexture;				// �e�N�X�`�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				// ���_�o�b�t�@���ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				// �C���f�b�N�X���ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;				// ���[���h�}�g���b�N�X
	int							m_nMaxVtx;				// ���_�̍ő吔
	int							m_nMaxIdx;				// �C���f�b�N�X�̍ő吔
	int							m_nMaxPolygon;			// �|���S���̍ő吔

	MESHTYPE					m_meshType;				// ���b�V���^�C�v
	MESHTEX						m_meshTex;				// �e�N�X�`���^�C�v
	D3DXVECTOR3					m_num;					// ����
	D3DXVECTOR3					m_pos;					// �ʒu
	D3DXVECTOR3					m_rot;					// ����
	D3DXVECTOR3					m_size;					// �傫��
	D3DXVECTOR2					m_TexUV;				// UV

	bool						m_bDrawAdd;				// ���Z����
	bool						m_bDrawSubtract;		// ���Y����
	bool						m_bDrawATest;			// ���e�X�g
	bool						m_bDrawZWrite;			// Z���C�g
	bool						m_bDrawLight;			// ���C�g�e��
	int							m_nATest;				// ���e�X�g���l
	int							m_nCullMode;			// �J�����O�ԍ�
	DWORD						m_dCmpValue;			// Z���C�g�ݒ�

protected:

};

#endif