//=============================================================================
//
// ���b�V���E�H�[������ [meshwall.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "mesh3d.h"		// ���b�V��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MESHWALL_SIZE_X		(30)		// �傫���iX)
#define MESHWALL_SIZE_Y		(30)		// �傫���iY)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CMeshWall : public CMesh3D
{// ���b�V���E�H�[���i�e�FCMesh3D�j
public:
	CMeshWall();								// �R���X�g���N�^
	~CMeshWall();								// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CMeshWall *Create(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex);	// ����

	HRESULT Init(MESHTYPE meshType, D3DXVECTOR3 num, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, MESHTEX meshTex);				// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// �e�N�X�`�����ւ̃|�C���^

protected:

};

#endif