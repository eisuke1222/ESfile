//=============================================================================
//
// �e���� [bullet.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene3d.h"	// �V�[��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_MAX_TEXTURE	(2)			// �e�N�X�`���̍ő吔
#define BULLET_SIZE_X		(6)			// �傫���iX)
#define BULLET_SIZE_Y		(6)			// �傫���iY)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CBullet : public CScene3D
{// �e�i�e�FCScene3D�j
public:
	typedef enum
	{// �e�̎��
		BULLETTYPE_PLAYER_1 = 0,		// �v���C���[1
		BULLETTYPE_PLAYER_2,			// �v���C���[2
		BULLETTYPE_ENEMY,				// �G
		BULLETTYPE_MAX
	}BULLETTYPE;

	typedef enum
	{// �e�̎��
		BULLETSTATE_NORMAL = 0,			// �ʏ�
		BULLETSTATE_BOMB,				// �{��
		BULLETSTATE_MAX
	}BULLETSTATE;

	CBullet();									// �R���X�g���N�^
	~CBullet();									// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, BULLETTYPE bulletType, BULLETSTATE bulletState);		// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 size, D3DXCOLOR col, BULLETTYPE bulletType, BULLETSTATE bulletState);				// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[BULLET_MAX_TEXTURE];	// �e�N�X�`�����ւ̃|�C���^

	BULLETTYPE m_bulletType;					// ���
	BULLETSTATE m_bulletState;					// ���
	D3DXVECTOR3 m_pos;							// �ړ���
	D3DXVECTOR3 m_move;							// �ړ���
	int m_Life;									// �̗�

protected:

};

#endif