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
#include "main.h"
#include "game.h"	// �Q�[��

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define	MAX_ENEMY						(64)								// �G�̍ő吔
#define ENEMY_TEXTURE_NAME_1			"data\\TEXTURE\\enemy000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��1
#define ENEMY_TEXTURE_NAME_2			"data\\TEXTURE\\enemy001.png"		// �ǂݍ��ރe�N�X�`���t�@�C��2
#define ENEMY_TEXTURE_NAME_3			"data\\TEXTURE\\enemy002.png"		// �ǂݍ��ރe�N�X�`���t�@�C��3
#define ENEMY_TEXTURE_NAME_4			"data\\TEXTURE\\enemy003.png"		// �ǂݍ��ރe�N�X�`���t�@�C��4
#define ENEMY_TEXTURE_NAME_5			"data\\TEXTURE\\enemy004.png"		// �ǂݍ��ރe�N�X�`���t�@�C��5

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �G���
	ENEMYTYPE_NORMALENEMY = 0,	// �m�[�}��
	ENEMYTYPE_BOSSENEMY,		// �{�X
	ENEMYTYPE_MAX
}ENEMYTYPE;

typedef enum
{// �G���
	ENEMYSTATE_APPEAR = 0,		// �o��
	ENEMYSTATE_NORMAL,			// �m�[�}��
	ENEMYSTATE_DAMAGE,			// �_���[�W
	ENEMYSTATE_DEATH,			// �f�X
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef enum
{
	ENEMYANIMTYPE_KNOCKBACK = 0,// �m�b�N�o�b�N
	ENEMYANIMTYPE_NORMAL,		// �m�[�}��
	ENEMYANIMTYPE_WALK,			// ����
	ENEMYANIMTYPE_MAX,
}ENEMYANIMTYPE;

typedef enum
{
	ENEMYTEXTURE_SLIME = 0,		// �X���C��
	ENEMYTEXTURE_SNOWMAN,		// �Ⴞ���
	ENEMYTEXTURE_CURSE,			// �̂낢
	ENEMYTEXTURE_BIRD,			// ��
	ENEMYTEXTURE_GHOST,			// �S�[�X�g
	ENEMYTEXTURE_MAX,
}ENEMYTEXTURE;

typedef struct
{// �G
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 posOld;			// �O��ʒu
	D3DXVECTOR3 move;			// �ړ��l
	D3DXCOLOR	col;			// �F
	float		fWidth;			// ����
	float		fHeight;		// ����
	int			nLife;			// �̗�
	ENEMYTYPE	Type;			// ���
	ENEMYSTATE	State;			// ���
	int			nCounterState;	// ��ԊǗ��J�E���^�[
	ENEMYANIMTYPE AnimType;		// �A�j���[�V�������
	int			nCounterAnim;	// �A�j���[�V�����J�E���^
	int			nNumAnim;		// �A�j���[�V�����ԍ�
	int			nSpeedAnim;		// �A�j���[�V�������x
	int			nPatternAnim;	// �A�j���[�V�����p�^�[��
	int			nDinectionMove;	// ����
	int			nCntTimer;		// ���ԃJ�E���^
	ENEMYTEXTURE texture;		// �e�N�X�`��
	bool bUse;					// �g�p���Ă��邩�ǂ���
}Enemy;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitEnemy(void);			// �G����������
void UninitEnemy(void);			// �G�I������
void UpdateEnemy(void);			// �G�X�V����
void DrawEnemy(void);			// �G�`�揈��

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nLife, ENEMYTYPE Type, ENEMYTEXTURE texture);	// �G�̐ݒ菈��
Enemy *GetEnemy(void);			// �G�擾����
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
void DeleteEnemy(int nCntEnemy);// �G�j�������i�P�j
void DeleteEnemy(void);			// �G�j�������i�S���j

#endif
