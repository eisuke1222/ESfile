//=========================================================================================================================
// 
// �e���� [bullet.h]
// Author : Einsuke Sakagawa
// 
//=========================================================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"	// ���C��

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define SINCOS_ANGLE_135		(0.75f)		// sin,cos�p�x�i0.75�j				
#define SINCOS_ANGLE_90			(0.5f)		// sin,cos�p�x�i0.5�j
#define SINCOS_ANGLE_45			(0.25f)		// sin,cos�p�x�i0.25�j
#define SINCOS_ANGLE_0			(0.0f)		// sin,cos�p�x�i0.0�j

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// ���
	BULLETTYPE_PLAYER1 = 0,		// �v���C���[1
	BULLETTYPE_PLAYER2,			// �v���C���[2
	BULLETTYPE_PLAYER3,			// �v���C���[3
	BULLETTYPE_PLAYER4,			// �v���C���[4
	BULLETTYPE_PLAYER5,			// �v���C���[5
	BULLETTYPE_ENEMY,			// �G
	BULLETTYPE_BOSSENEMY,		// �{�X
	BULLETTYPE_MAX				// �ő吔
}BULLETTYPE;

typedef enum
{// �e�ړ�
	BULLETMOVE_NORMAL = 0,				// �ʏ�
	BULLETMOVE_MOVECHANGE_HOMING,		// �z�[�~���O
	BULLETMOVE_MOVECHANGE_STRAIGHT,		// �܂�����
	BULLETMOVE_MOVECHANGE_START_1,		// �X�^�[�g1
	BULLETMOVE_MOVECHANGE_START_2,		// �X�^�[�g2
	BULLETMOVE_SETBULLET,				// �e�ǉ�
	BULLETMOVE_SETBULLET_1,				// �e�ǉ�1
	BULLETMOVE_MAX
}BULLETMOVE;

typedef struct
{// �e
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXCOLOR col;				// �F
	BULLETTYPE Type;			// �e�̎��
	BULLETMOVE moveType;		// �ړ��̎��
	float fSpeed;				// �ړ����x
	int nCntChange;				// �ړ��ύX
	int nCntPoint;				// �ʒu�J�E���^
	int nTexNum;				// �e�N�X�`���ԍ�
	bool bUse;					// �g�p���Ă��邩�ǂ���
}Bullet;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitBullet(void);			// �e����������
void UninitBullet(void);		// �e�I������
void UpdateBullet(void);		// �e�X�V����
void DrawBullet(void);			// �e�`�揈��

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE Type, BULLETMOVE moveType, float fSpeed, int nCntPoint, int nTexNum);		// �e�̐ݒ菈��

#endif
