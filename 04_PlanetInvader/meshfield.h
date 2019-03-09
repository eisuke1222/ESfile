//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "mesh3d.h"		// ���b�V��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_MESHFIELD_TEXTURE	(2)			// �e�N�X�`���ő吔
#define MESHFIELD_SIZE_X		(30)		// �傫���iX)
#define MESHFIELD_SIZE_Y		(30)		// �傫���iY)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMeshField : public CMesh3D
{// ���b�V���t�B�[���h�i�e�FCMesh3D�j
public:
	typedef enum
	{// �t�B�[���h�^�C�v
		MESHFIELDTYPE_O = 0,		// �C
		MESHFIELDTYPE_G,			// ��
		MESHFIELDTYPE_MAX
	}MESHFIELDTYPE;

	CMeshField();									// �R���X�g���N�^
	~CMeshField();									// �f�X�g���N�^

	static HRESULT Load(void);						// ���[�h
	static void Unload(void);						// �A�����[�h
	static CMeshField *Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHFIELDTYPE meshfieldType, MESHTEX meshTex);		// ����

	HRESULT Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHFIELDTYPE meshfieldType, MESHTEX meshTex);					// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_MESHFIELD_TEXTURE];		// �e�N�X�`�����ւ̃|�C���^

	static D3DXVECTOR2 m_posUV;						// UV�ʒu
	MESHFIELDTYPE m_meshfieldType;					// �t�B�[���h�^�C�v

protected:

};

#endif