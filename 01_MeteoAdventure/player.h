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
#include "main.h"
#include "game.h"		// �Q�[��
#include "block.h"		// �u���b�N

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �v���C���[���
	PLAYERSTATE_APPEAR = 0,		// �o��
	PLAYERSTATE_NORMAL,			// �m�[�}��
	PLAYERSTATE_DAMAGE,			// �_���[�W
	PLAYERSTATE_DEATH,			// �f�X
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{// �A�j���[�V�����p�^�[����
	PLAYERANIMTYPE_PATTERN_JUMP = 2,		// �W�����v
	PLAYERANIMTYPE_PATTERN_ATTACK = 5,		// �U��
	PLAYERANIMTYPE_PATTERN_SETSWIM = 3,		// �j������
	PLAYERANIMTYPE_PATTERN_RUN = 5,			// ����
	PLAYERANIMTYPE_PATTERN_RUNSTOP = 2,		// �~�܂�
	PLAYERANIMTYPE_PATTERN_SWIM = 5,		// �j��
	PLAYERANIMTYPE_PATTERN_NORMAL = 5,		// ����
	PLAYERANIMTYPE_PATTERN_THROW = 5,		// ������
	PLAYERANIMTYPE_PATTERN_WALK = 5,		// ����
	PLAYERANIMTYPE_PATTERN_DOORIN = 3,		// �h�A�ɓ���+���̑�
	PLAYERANIMTYPE_PATTERN_MAX = 10,
}PLAYERANIMTYPE_PATTERN;

typedef enum
{// �A�j���[�V�������
	PLAYERANIMTYPE_JUMP = 0,		// �W�����v
	PLAYERANIMTYPE_ATTACK,			// �U��
	PLAYERANIMTYPE_SETSWIM,			// �j������
	PLAYERANIMTYPE_RUN,				// ����
	PLAYERANIMTYPE_RUNSTOP,			// �~�܂�
	PLAYERANIMTYPE_SWIM,			// �j��
	PLAYERANIMTYPE_NORMAL,			// ����
	PLAYERANIMTYPE_THROW,			// ������
	PLAYERANIMTYPE_WALK,			// ����
	PLAYERANIMTYPE_DOORIN,			// �h�A�ɓ���+���̑�
	PLAYERANIMTYPE_MAX,
}PLAYERANIMTYPE;

typedef struct
{// �v���C���[
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 posOld;				// �O��ʒu
	D3DXVECTOR3 move;				// �ړ��l
	D3DXCOLOR   col;				// �F
	float		fWidth;				// ����
	float		fHeight;			// ����
	int			nNumPlayer;			// �c�@
	int			nLife;				// �̗�
	float		fGravity;			// �d��
	PLAYERSTATE	State;				// ���
	int			nCounterState;		// ��ԊǗ��J�E���^�[
	PLAYERANIMTYPE AnimType;		// �A�j���[�V�������
	int			nCounterAnim;		// �A�j���[�V�����J�E���^
	int			nNumAnim;			// �A�j���[�V�����ԍ�
	int			nSpeedAnim;			// �A�j���[�V�������x
	int			nPatternAnim;		// �A�j���[�V�����p�^�[��
	int			nDinectionMove;		// ����
	bool		bAttack;			// �U�����Ă��邩
	bool		bJump;				// �W�����v���Ă��邩
	bool		bDisp;				// �\�����Ă��邩
	BLOCK		*pBlock;			// �u���b�N�|�C���^
}PLAYER;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitPlayer(void);				// �v���C���[����������
void UninitPlayer(void);			// �v���C���[�I������
void UpdatePlayer(void);			// �v���C���[�X�V����
void DrawPlayer(void);				// �v���C���[�`�揈��

void SetPlayer(D3DXVECTOR3 pos);	// �v���C���[�ݒ菈��
PLAYER *GetPlayer(void);			// �v���C���[�擾����
void DeletePlayer(void);			// �v���C���[�j������

#endif