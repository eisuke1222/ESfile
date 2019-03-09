//=========================================================================================================================
// 
// �A�C�e������ [item.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �A�C�e�����
	ITEMTYPE_COIN_NORMAL = 0,		// �R�C��
	ITEMTYPE_COIN_GRAVITY,			// �R�C���i�d�͂���j
	ITEMTYPE_MAX
}ITEMTYPE;

typedef enum
{// �A�C�e�����
	ITEMSTATE_APPEAR = 0,
	ITEMSTATE_NORMAL,
	ITEMSTATE_DISAPPEAR,
	ITEMSTATE_MAX
}ITEMSTATE;

typedef struct
{// �A�C�e��
	D3DXVECTOR3 pos;				// �ʒu
	D3DXVECTOR3 posOld;				// �O��ʒu
	D3DXCOLOR col;					// �F
	D3DXVECTOR3 move;				// �ړ��l
	float fWidth;					// ����
	float fHeight;					// ����
	int nCounterAnim;				// �A�j���[�V�����J�E���^
	int nPatternAnim;				// �A�j���[�V�����p�^�[��
	ITEMTYPE Type;					// ���
	ITEMSTATE state;				// ���
	int nCounterState;				// ��ԃJ�E���^
	bool bUse;						// �g�p���Ă��邩
}ItemState;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitItem(void);				// �A�C�e������������
void UninitItem(void);				// �A�C�e���I������
void UpdateItem(void);				// �A�C�e���X�V����
void DrawItem(void);				// �A�C�e���`�揈��

void SetItem(D3DXVECTOR3 pos, ITEMTYPE type);		// �A�C�e���ݒ菈��
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float Width, float fHeight);		// �A�C�e���Փˏ���
int GetNumCoin(void);								// �A�C�e���擾����
void DeleteItem(int nCntItem);						// �A�C�e���j�������i�P�j
void DeleteItem(void);								// �A�C�e���j�������i�S���j

#endif