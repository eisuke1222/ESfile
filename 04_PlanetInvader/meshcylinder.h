//=============================================================================
//
// ���b�V���V�����_�[���� [meshcylinder.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "mesh3d.h"		// ���b�V��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MESHCYLINDER_MAX_TEXTURE	(3)			// �e�N�X�`���ő吔
#define MESHCYLINDER_SIZE_X			(30)		// �傫���iX)
#define MESHCYLINDER_SIZE_Y			(30)		// �傫���iY)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMeshCylinder : public CMesh3D
{// ���b�V���V�����_�[�i�e�FCMesh3D�j
public:
	CMeshCylinder();							// �R���X�g���N�^
	~CMeshCylinder();							// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CMeshCylinder *Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nTexNum, D3DXVECTOR2 TexMoveUV, MESHTEX meshTex);		// ����

	HRESULT Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, D3DXVECTOR2 TexMoveUV, MESHTEX meshTex);				// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MESHCYLINDER_MAX_TEXTURE];		// �e�N�X�`�����ւ̃|�C���^

	D3DXVECTOR2 m_posUV;						// �ʒu
	D3DXVECTOR2 m_moveUV;						// �ړ���

protected:

};

#endif