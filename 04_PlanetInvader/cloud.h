//=============================================================================
//
// �_���� [cloud.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _CLOUD_H_
#define _CLOUD_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene3d.h"	// �V�[��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLOUD_SIZE_X	(30)		// �傫���iX)
#define CLOUD_SIZE_Y	(30)		// �傫���iY)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCloud : public CScene3D
{// �_�i�e�FCScene3D�j
public:
	CCloud();										// �R���X�g���N�^
	~CCloud();										// �f�X�g���N�^

	static HRESULT Load(void);						// ���[�h
	static void Unload(void);						// �A�����[�h
	static CCloud *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType);	// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType);			// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;		// �e�N�X�`�����ւ̃|�C���^

	D3DXVECTOR3 m_pos;								// �ʒu

protected:

};

#endif