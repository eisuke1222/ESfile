//=========================================================================================================================
//
// �G�t�F�N�g�̏��� [billboardeffect.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _BILLBOARDEFFECT_H_
#define _BILLBOARDEFFECT_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �G�t�F�N�g�^�C�v
	EFFECTTYPE_NONE = 0,		// �������Ă��Ȃ����
	EFFECTTYPE_NORMAL,			// �m�[�}��
	EFFECTTYPE_BULLET,			// �e
	EFFECTTYPE_PARTICLE,		// �p�[�e�B�N��
	EFFECTTYPE_MAX
}EFFECTTYPE;

typedef enum
{// �G�t�F�N�g���
	EFFECTSTATE_NORMAL,			// �m�[�}��
	EFFECTSTATE_LOOP,			// ���[�v
	EFFECTSTATE_MAX
}EFFECTSTATE;

typedef enum
{// �G�t�F�N�g�e
	EFFECTBULLET_NONE = 0,
	EFFECTBULLET_PLAYER,
	EFFECTBULLET_PLAYER2,
	EFFECTBULLET_ENEMY,
	EFFECTBULLET_MAX,
}EFFECTBULLET;

typedef struct
{// ���
	D3DXVECTOR3	pos;			// �ʒu
	D3DXVECTOR3	rot;			// ����
	D3DXVECTOR3	move;			// �ړ���
	D3DXVECTOR3	fRadius;		// �傫��
	D3DXCOLOR	col;			// �F
	float		coladown;
	int			nLife;
	EFFECTTYPE	type;			// ���
	D3DXMATRIX	mtxWorld;		// ���[���h�}�g���b�N�X
	int			nIdxShadow;		// �e�ԍ�
	int			nDamage;
	int			nType;
	bool		bUse;			// �g�p���Ă��邩
}BillboardEffect;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitBillboardEffect(void);			// �G�t�F�N�g�̏���������
void UninitBillboardEffect(void);		// �G�t�F�N�g�̏I������
void UpdateBillboardEffect(void);		// �G�t�F�N�g�̍X�V����
void DrawBillboardEffect(void);			// �G�t�F�N�g�̕`�揈��

void SetBillboardNormalEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown);		// �m�[�}���G�t�F�N�g�̐ݒ菈��
void SetBillboardBulletEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown, int nLife, int nDamage, int nType);		// �e�G�t�F�N�g�̐ݒ菈��
void SetBillboardParticleEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown, int nMaxSet, EFFECTSTATE state);	// �p�[�e�B�N���G�t�F�N�g�̐ݒ菈��
void SetXEnemyCollNum(int nXEnemy, int nXEnemyNum);
bool CollisionEffect(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMax, D3DXVECTOR3 *vtxMin, int nType);
void DeleteBillboardEffect(int nCntBillboardEffect);		// �G�t�F�N�g�̔j������
void DeleteBillboardEffect(void);		// �G�t�F�N�g�̔j������

#endif
