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
#include "main.h"
#include "block.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define	MAX_BULLET				(255)									// �e�̍ő吔
#define BULLET_TEXTURE_NAME1	"data\\TEXTURE\\bullet000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��1

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// ���
	BULLETTYPE_PLAYER = 0,		// �v���C���[1
	BULLETTYPE_ENEMY,			// �G
	BULLETTYPE_BOSSENEMY,		// �{�X
	BULLETTYPE_MAX
}BULLETTYPE;

typedef enum
{
	BULLETTEXTURETYPE_PLAYER_NORMAL = 0,	// �v���C���[�e
	BULLETTEXTURETYPE_PLAYER_MAX,
}BULLETTEXTURETYPE;

typedef enum
{// �Փˎ��
	COLLISIONBULLETRETURN_BLOCK = 0,	// �u���b�N
	COLLISIONBULLETRETURN_OTHER,
	COLLISIONBULLETRETURN_MAX,
}COLLISIONBULLETRETURN;

typedef struct
{// �e
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 posOld;			// �O��ʒu
	D3DXVECTOR3 move;			// �ړ���
	D3DXCOLOR col;				// �F
	BULLETTYPE Type;			// �e�̎��
	float fWidth;				// ����
	float fHeight;				// ����
	BULLETTEXTURETYPE texture;	// �e�N�X�`���ԍ�
	bool bUse;					// �g�p���Ă��邩�ǂ���
	BLOCK *pBlock;				// �u���b�N�|�C���^
}Bullet;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitBullet(void);			// �e����������
void UninitBullet(void);		// �e�I������
void UpdateBullet(void);		// �e�X�V����
void DrawBullet(void);			// �e�`�揈��

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE Type, float fWidth, float fHeight, BULLETTEXTURETYPE texture);		// �e�̐ݒ菈��
bool CollisiomBullet(D3DXVECTOR3 *pPos, float fWidth, float fHeight, COLLISIONBULLETRETURN collision);		// �e�Փˏ���
void DeleteBullet(int nCntBullet);		// �e�j�������i1�j
void DeleteBullet(void);				// �e�j�������i�S���j

#endif
