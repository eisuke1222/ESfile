//=============================================================================
//
// �������� [explosion.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene3d.h"	// �V�[��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EXPLOSION_SIZE_X	(30)		// �傫���iX)
#define EXPLOSION_SIZE_Y	(30)		// �傫���iY)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CExplosion : public CScene3D
{// �����i�e�FCScene3D�j
public:
	typedef enum
	{// �����^�C�v
		EXPLOSIONTYPE_NORMAL = 0,
		EXPLOSIONTYPE_DAMAGE,
		EXPLOSIONTYPE_MAX
	}EXPLOSIONTYPE;

	CExplosion();								// �R���X�g���N�^
	~CExplosion();								// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CExplosion *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, EXPLOSIONTYPE explosionType, int nShotIdx);		// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, EXPLOSIONTYPE explosionType, int nShotIdx);					// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// �e�N�X�`�����ւ̃|�C���^

	int m_nAnimCounter;							// �A�j���[�V�����J�E���^
	int m_AnimPattern;							// �A�j���[�V�����p�^�[��

	EXPLOSIONTYPE m_explosionType;				// �����^�C�v
	int m_nShotIdx;								// �������lID

protected:

};

#endif