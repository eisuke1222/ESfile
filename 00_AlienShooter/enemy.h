//=========================================================================================================================
// 
// �G���� [enemy.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"	// ���C��
#include "game.h"	// �Q�[��
#include "bullet.h"	// �e

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define	MAX_ENEMY		(512)	// �G�̍ő吔

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �G���
	ENEMYTYPE_TUTORIALENEMY = 0,// �`���[�g���A��
	ENEMYTYPE_TUTORIALBOSSENEMY,// �`���[�g���A���{�X
	ENEMYTYPE_NORMALENEMY,		// �m�[�}��
	ENEMYTYPE_SBOSSENEMY,		// ���{�X
	ENEMYTYPE_MBOSSENEMY,		// ���{�X
	ENEMYTYPE_LBOSSENEMY,		// ��{�X
	ENEMYTYPE_MAX				// �ő吔
}ENEMYTYPE;

typedef enum
{// �G���
	ENEMYSTATE_APPEAR = 0,		// �o��
	ENEMYSTATE_NOMAL,			// �m�[�}��
	ENEMYSTATE_DAMAGE,			// �_���[�W
	ENEMYSTATE_DEATH,			// �f�X
	ENEMYSTATE_MAX				// �ő吔
}ENEMYSTATE;

typedef enum
{// �G�ړ����
	ENEMYMOVE_NONE = 0,			// �����Ȃ����
	ENEMYMOVE_NORMAL,			// �ʏ�
	ENEMYMOVE_HOMING,			// �ǔ���~
	ENEMYMOVE_HOMING_KEEP,		// �ǔ����s
	ENEMYMOVE_HOMING_OUT,		// �ǔ���ʊO��
	ENEMYMOVE_MAX,				// �ő吔
}ENEMYMOVE;

typedef enum
{// �e���
	ENEMYBULLET_NONE = 0,		// �����Ȃ����
	ENEMYBULLET_NO_1,			// �e�P
	ENEMYBULLET_NO_2,			// �e�Q
	ENEMYBULLET_NO_3,			// �e�R
	ENEMYBULLET_NO_4,			// �e�S
	ENEMYBULLET_NO_5,			// �e�T
	ENEMYBULLET_NO_6,			// �e�U
	ENEMYBULLET_NO_7,			// �e�V
	ENEMYBULLET_NO_8,			// �e�W
	ENEMYBULLET_NO_9,			// �e�X
	ENEMYBULLET_NO_10,			// �e�P�O
	ENEMYBULLET_NO_11,			// �e�P�P
	ENEMYBULLET_NO_12,			// �e�P�Q
	ENEMYBULLET_NO_13,			// �e�P�R
	ENEMYBULLET_NO_14,			// �e�P�S
	ENEMYBULLET_NO_15,			// �e�P�T
	ENEMYBULLET_MAX,			// �ő吔
}ENEMYBULLETTYPE;

typedef struct
{// �G
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 posTarget;		// �ڕW�ʒu
	D3DXVECTOR3 move;			// �ړ��l
	float moveSpeed;			// �ړ����x
	ENEMYMOVE moveType;			// �ړ����
	int moveStart;				// �ړ��J�n
	int moveLimit;				// �ړ���
	int moveChange;				// �ړ��ύX
	int nCntTarget;				// �ڕW�ʒu��
	D3DXCOLOR col;				// �F
	int nType;					// ���
	int nSize;					// �傫��
	int nLife;					// ����
	ENEMYTYPE Type;				// �G�̎��
	ENEMYSTATE State;			// �G�̏��
	int nCounterState;			// ��ԊǗ��J�E���^�[
	int nCounterAnim;			// �A�j���[�V�����J�E���^�[
	int nPatternAnim;			// �A�j���[�V�����p�^�[��No.
	bool bUse;					// �g�p���Ă��邩�ǂ���
}Enemy;

typedef struct
{// �G�e
	ENEMYBULLETTYPE nPattenType;// �e���
	float moveSpeed;			// �ړ����x
	int nTiming;				// ���˃^�C�~���O
	BULLETTYPE Type;			// ���
	D3DXVECTOR3 move;			// �ړ���
	D3DXCOLOR col;				// �F
	int nCounterInput;			// ���̓J�E���^
	float nAngle;				// �e�p�x
}EnemyBullet;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitEnemy(void);			// �G����������
void UninitEnemy(void);			// �G�I������
void UpdateEnemy(void);			// �G�X�V����
void DrawEnemy(void);			// �G�`�揈��

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, D3DXVECTOR3 move, float moveSpeed, ENEMYMOVE moveType, int moveStart, int nCntTarget, int nType, int nSize, int nLife, ENEMYTYPE Type,
	EnemyBullet bullet1, EnemyBullet bullet2, EnemyBullet bullet3, EnemyBullet bullet4, EnemyBullet bullet5);	// �G�̐ݒ菈��
Enemy *GetEnemy(void);									// �G�̎擾
EnemyBullet *GetEnemyBullet(void);						// �G�e�̎擾
void HitEnemy(int nCntEnemy, int nDamage);				// �G�̒e�����菈��
int GetNumEnemy(void);									// �G�̑����擾

#endif
