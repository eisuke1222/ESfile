//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene3d.h"	// �V�[��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EFFECT_SIZE_X	(10)			// �傫���iX)
#define EFFECT_SIZE_Y	(10)			// �傫���iY)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffect : public CScene3D
{// �G�t�F�N�g�i�e�FCScene3D�j
public:
	typedef enum
	{// �G�t�F�N�g�^�C�v
		EFFECTTYPE_NONE = 0,		// �������Ă��Ȃ�
		EFFECTTYPE_PLAYER_T,		// �v���C���[�^�C�g��
		EFFECTTYPE_PLAYER,			// �v���C���[
		EFFECTTYPE_ENEMY,			// �G
		EFFECTTYPE_BULLET,			// �e
		EFFECTTYPE_EXPLOSION,		// ����
		EFFECTTYPE_SMOKE,			// ��
		EFFECTTYPE_MAX
	}EFFECTTYPE;

	typedef enum
	{// �e�N�X�`��
		TEXTURE_EFFECT = 0,
		TEXTURE_WATER,
		TEXTURE_SMOKE,
		TEXTURE_TANK,
		TEXTURE_MAX
	}TEXTURE;

	CEffect();									// �R���X�g���N�^
	CEffect(int);								// �R���X�g���N�^
	~CEffect();									// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXVECTOR3 sizeDown, D3DXCOLOR col, float faDown, EFFECTTYPE effectType, TEXTURE texture);	// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXVECTOR3 sizeDown, D3DXCOLOR col, float faDown, EFFECTTYPE effectType, TEXTURE texture);				// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[TEXTURE_MAX];	// �e�N�X�`�����ւ̃|�C���^

	TEXTURE		m_TexNum;						// �e�N�X�`��
	EFFECTTYPE	m_effectType;					// �G�t�F�N�g�^�C�v
	int			m_nCntTimer;					// ���ԃJ�E���^
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_moveInit;						// �ړ��ʏ���
	D3DXVECTOR3 m_move;							// �ړ���
	D3DXVECTOR3 m_sizeDown;						// �傫������
	float		m_faDown;						// �A���t�@����

protected:

};

#endif