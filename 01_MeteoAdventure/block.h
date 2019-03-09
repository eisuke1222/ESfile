//=========================================================================================================================
// 
// �u���b�N���� [block.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_BLOCK				(128)							// �u���b�N�̍ő吔

#define BLOCK_TEXTURE_NAME_1	"data\\TEXTURE\\Ground000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��1
#define BLOCK_TEXTURE_NAME_2	"data\\TEXTURE\\Wall000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��2
#define BLOCK_TEXTURE_NAME_3	"data\\TEXTURE\\block000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��3
#define BLOCK_TEXTURE_NAME_4	"data\\TEXTURE\\dokan.png"		// �ǂݍ��ރe�N�X�`���t�@�C��4
#define BLOCK_TEXTURE_NAME_5	"data\\TEXTURE\\Ground001.png"	// �ǂݍ��ރe�N�X�`���t�@�C��5
#define BLOCK_TEXTURE_NAME_6	"data\\TEXTURE\\toge000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��6
#define BLOCK_TEXTURE_NAME_7	"data\\TEXTURE\\suna001.png"	// �ǂݍ��ރe�N�X�`���t�@�C��7

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �u���b�N���
	BLOCKTYPE_NONE = 0,				// �g�p���Ă��Ȃ�
	BLOCKTYPE_NORMAL,				// �m�[�}��
	BLOCKTYPE_NORMAL_BREAK,			// �m�[�}���j��
	BLOCKTYPE_NORMAL_WARP_X,		// �m�[�}�����[�vX
	BLOCKTYPE_NORMAL_WARP_Y,		// �m�[�}�����[�vY
	BLOCKTYPE_NORMAL_FALL,			// �m�[�}��������
	BLOCKTYPE_NORMAL_APPEAR,		// �m�[�}���o��
	BLOCKTYPE_NORMAL_DAMAGE,		// �m�[�}���_���[�W
	BLOCKTYPE_NORMAL_SUNA,			// �m�[�}����
	BLOCKTYPE_NORMAL_SUNA_RE,		// �m�[�}��������
	BLOCKTYPE_MOVE_TRIP,			// ����
	BLOCKTYPE_MOVE_LOOP,			// �J��Ԃ�
	BLOCKTYPE_MAX,
}BLOCKTYPE;

typedef enum
{// �e�N�X�`�����
	BLOCKTEXTURE_GROUND = 0,		// �n��
	BLOCKTEXTURE_WALL,				// ��
	BLOCKTEXTURE_NORMAL,			// �m�[�}��
	BLOCKTEXTURE_DOKAN,				// �y��
	BLOCKTEXTURE_SABAKU,			// ����
	BLOCKTEXTURE_TOGE,				// �Ƃ�
	BLOCKTEXTURE_SUNA,				// ��
	BLOCKTEXTURE_MAX,
}BLOCKTEXTURE;

typedef enum
{// �Փˎ��
	COLLISIONBLOCKRETURN_PLAYER = 0,// �v���C���[
	COLLISIONBLOCKRETURN_ENEMY,		// �G
	COLLISIONBLOCKRETURN_BULLET,	// �e
	COLLISIONBLOCKRETURN_OTHER,		//
	COLLISIONBLOCKRETURN_MAX,
}COLLISIONBLOCKRETURN;

typedef struct
{// �u���b�N
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 tex;				// �ʒu
	D3DXVECTOR3 move;				// �ړ��l
	D3DXCOLOR col;					// �F
	float fWidth;					// ����
	float fHeight;					// ����
	BLOCKTYPE type;					// �u���b�N���
	int nLife;						// �ϋv�l
	int nCntMove;					// �ړ��J�E���g
	int nCntMoveLimit;				// �ړ�����
	bool bBlockScroll;				// �X�N���[�����邩
	bool bUse;						// �g�p���Ă��邩
	BLOCKTEXTURE texture;			// �e�N�X�`��
}BLOCK;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitBlock(void);				// �u���b�N����������
void UninitBlock(void);				// �u���b�N�I������
void UpdateBlock(void);				// �u���b�N�X�V����
void DrawBlock(void);				// �u���b�N�`�揈��

void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, BLOCKTYPE type, int nCntMoveLimit, BLOCKTEXTURE texture);						// �u���b�N�ݒ菈��
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, BLOCK **pBlock, COLLISIONBLOCKRETURN collision);	// �u���b�N�Ƃ̏Փˏ���
bool GetScrollNum(void);			// �X�N���[������擾
void DeleteBlock(int nCntBlock);	// �u���b�N�j������
void DeleteBlock(void);				// �u���b�N�j������

#endif