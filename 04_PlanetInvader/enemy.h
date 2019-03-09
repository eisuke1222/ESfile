//=============================================================================
//
// �G���� [enemy.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "model3d.h"	// ���f��3D

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemy : public CModel3D
{// �G�i�e�FCModel3D�j
public:
	typedef enum
	{// �G�^�C�v
		ENEMYTYPE_S_1 = 0,		// ��01
		ENEMYTYPE_S_2,			// ��02
		ENEMYTYPE_G_1,			// ��01
		ENEMYTYPE_G_2,			// ��02
		ENEMYTYPE_O_1,			// �C01
		ENEMYTYPE_O_2,			// �C02
		ENEMYTYPE_B_1,			// �C02
		ENEMYTYPE_B_2,			// �C02
		ENEMYTYPE_B_3,			// �C02
		ENEMYTYPE_MAX
	}ENEMYTYPE;

	CEnemy();									// �R���X�g���N�^
	~CEnemy();									// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, ENEMYTYPE enemyType, int nShotTiming);	// ����

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, ENEMYTYPE enemyType, int nShotTiming);			// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	bool HitDamage(int nDamage) { m_nLife -= nDamage; if (m_nLife < 1)return true; return false; }
	ENEMYTYPE GetenemyType(void) { return m_enemyType; }

private:
	static LPDIRECT3DTEXTURE9			*m_pTexture[ENEMYTYPE_MAX];			// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH					m_pMesh[ENEMYTYPE_MAX];				// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER					m_pBuffMat[ENEMYTYPE_MAX];			// �}�e���A�����ւ̃|�C���^
	static DWORD						m_nNumMat[ENEMYTYPE_MAX];			// �}�e���A�����̐�

	ENEMYTYPE m_enemyType;						// ���
	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_move;							// �ړ���
	int m_nCntTimer;							// ���ԃJ�E���^
	int m_nLife;								// �̗�
	int m_nShotTiming;							// ���˃^�C�~���O

	int m_nNumPattern;							// �p�^�[���ԍ�

protected:

};

#endif