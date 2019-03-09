//=============================================================================
//
// �V�[�����b�V���I�[�r�b�g���� [scenemeshorbit.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _SCENEMESHOBRBIT_H_
#define _SCENEMESHOBRBIT_H_

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
class CSceneMeshOrbit : public CScene
{// �V�[�����b�V���I�[�r�b�g�i�e�FCScene�j
public:
	const static int m_MAXOFFSET = 2;						// �I�t�Z�b�g��

	CSceneMeshOrbit(int nPriority, CScene::OBJTYPE objType);// �R���X�g���N�^
	~CSceneMeshOrbit();										// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h

															// �N���G�C�g
	static CSceneMeshOrbit *Create(D3DXVECTOR3 OffSet_0, D3DXVECTOR3 OffSet_1, D3DXVECTOR3 col_rgb, float fMaxcol_a, float fMincol_a, int nMaxVtx, LPDIRECT3DTEXTURE9 pTexture = NULL);

	HRESULT Init(void);										// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	void SetMtxParent(D3DXMATRIX *pMtx)						// �}�g���b�N�X�ݒ�
		{ m_pMtxParent = pMtx; }

	void Setcol(D3DXVECTOR3 col_rgb)						// �F�ݒ�
		{ m_col_rgb = col_rgb; }

private:
	LPDIRECT3DTEXTURE9			m_pTexture;					// �e�N�X�`�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;					// ���_�o�b�t�@���ւ̃|�C���^
	D3DXMATRIX					*m_pMtxParent;				// �e�̃}�g���b�N�X�ւ̃|�C���^

	int							m_nMaxVtx;					// �ő咸�_��

	D3DXVECTOR3					*m_aPosVtx;					// ���_�ʒu
	D3DXVECTOR3					m_aOffSet[m_MAXOFFSET];		// �I�t�Z�b�g�ʒu
	D3DXVECTOR3					m_col_rgb;					// �F
	float						m_fMaxcol_a;				// �ő哧���x
	float						m_fMincol_a;				// �ŏ������x

protected:

};

#endif