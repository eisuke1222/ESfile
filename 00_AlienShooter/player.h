//=========================================================================================================================
// 
// �v���C���[���� [player.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"				// ���C��
#include "game.h"				// �Q�[��
#include "bullet.h"				// �e

#define PLAYER_LIFE				(10)									// �v���C���[�̗̑�

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �v���C���[���
	PLAYERSTATE_APPEAR = 0,		// �o��
	PLAYERSTATE_NOMAL,			// �m�[�}��
	PLAYERSTATE_DAMAGE,			// �_���[�W
	PLAYERSTATE_INVINCIBLE,		// ���G
	PLAYERSTATE_DEATH,			// �f�X
	PLAYERSTATE_MAX				// �ő吔
}PLAYERSTATE;

typedef struct
{// �v���C���[
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXCOLOR col;				// �F
	int nLife;					// �̗�
	bool bLifeTex[PLAYER_LIFE];	// �̗̓e�N�X�`��
	PLAYERSTATE State;			// �v���C���[�̏��
	int nCounterState;			// ��ԊǗ��J�E���^�[
	BULLETTYPE Type;			// ���
	bool bDisp;					// �\�����Ă��邩�ǂ���
}Player;

typedef struct
{// �v���C���[�e
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXCOLOR col;				// �F
	int nCounterInput;			// ���̓J�E���^
	float nAngle;				// �e�p�x
}PlayerBullet;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitPlayer(void);			// �v���C���[����������
void UninitPlayer(void);		// �v���C���[�I������
void UpdatePlayer(void);		// �v���C���[�X�V����
void DrawPlayer(void);			// �v���C���[�`�揈��

void SetPlayer(void);			// �v���C���[�ݒ菈��
Player *GetPlayer(void);		// �v���C���[�̎擾����

void HitPlayer(int nDamage);	// �v���C���[�e�����菈��

#endif