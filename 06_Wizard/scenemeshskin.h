//=============================================================================
//
// �V�[�����b�V���X�L������ [scenemeshskin.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENEMESHSKIN_H_
#define _SCENEMESHSKIN_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define		MAX_VTX	(6)
#define		MAX_OFFSET	(2)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneMeshSkin : public CScene
{// �V�[�����b�V���X�L���i�e�FCScene�j
public:
	CSceneMeshSkin(int nPriority, CScene::OBJTYPE objType);		// �R���X�g���N�^
	~CSceneMeshSkin();											// �f�X�g���N�^

	static HRESULT Load(void);									// ���[�h
	static void Unload(void);									// �A�����[�h

																// �N���G�C�g
	static CSceneMeshSkin *Create(D3DXVECTOR3 OffSet_0, D3DXVECTOR3 OffSet_1);

	HRESULT Init(void);											// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��

	void SetMtxParent(D3DXMATRIX *pMtx) { m_pMtxParent = pMtx; }// �}�g���b�N�X�ݒ�

private:
	LPDIRECT3DTEXTURE9				m_pTexture;					// �e�N�X�`�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;					// ���_�o�b�t�@���ւ̃|�C���^
	D3DXMATRIX						*m_pMtxParent;				// �e�̃}�g���b�N�X�ւ̃|�C���^

	D3DXMATRIX						m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3						m_aPosVtx[MAX_VTX];			// ���_�ʒu
	D3DXVECTOR3						m_aOffSet[MAX_OFFSET];		// �I�t�Z�b�g�ʒu

protected:

};

#endif