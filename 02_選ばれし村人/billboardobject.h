//=========================================================================================================================
//
// �I�u�W�F�N�g�̏��� [billboardobject.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _BILLBOARDOBJECT_H_
#define _BILLBOARDOBJECT_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define BILLBOARDOBJECT_TEXTURE_NAME1		"data\\TEXTURE\\rockon.png"			// �ǂݍ��ރe�N�X�`���t�@�C��1
#define BILLBOARDOBJECT_TEXTURE_NAME2		"data\\TEXTURE\\hp000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��2
#define BILLBOARDOBJECT_TEXTURE_NAME3		"data\\TEXTURE\\wand000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��3
#define BILLBOARDOBJECT_TEXTURE_NAME4		"data\\TEXTURE\\wand001.png"		// �ǂݍ��ރe�N�X�`���t�@�C��4
#define BILLBOARDOBJECT_TEXTURE_NAME5		"data\\TEXTURE\\wand002.png"		// �ǂݍ��ރe�N�X�`���t�@�C��5
#define BILLBOARDOBJECT_TEXTURE_NAME6		"data\\TEXTURE\\map_wall.png"		// �ǂݍ��ރe�N�X�`���t�@�C��6
#define BILLBOARDOBJECT_TEXTURE_NAME7		"data\\TEXTURE\\item000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��7
#define BILLBOARDOBJECT_TEXTURE_NAME8		"data\\TEXTURE\\item001.png"		// �ǂݍ��ރe�N�X�`���t�@�C��8
#define BILLBOARDOBJECT_TEXTURE_NAME9		"data\\TEXTURE\\item002.png"		// �ǂݍ��ރe�N�X�`���t�@�C��9
#define BILLBOARDOBJECT_TEXTURE_NAME10		"data\\TEXTURE\\item003.png"		// �ǂݍ��ރe�N�X�`���t�@�C��10
#define BILLBOARDOBJECT_TEXTURE_NAME11		"data\\TEXTURE\\sword001.png"		// �ǂݍ��ރe�N�X�`���t�@�C��11

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �e�N�X�`��
	BILLBOARDTEXTURE_ROCKON = 0,
	BILLBOARDTEXTURE_HP,
	BILLBOARDTEXTURE_WAND_1,
	BILLBOARDTEXTURE_WAND_2,
	BILLBOARDTEXTURE_WAND_3,
	BILLBOARDTEXTURE_MAP_WALL,
	BILLBOARDTEXTURE_ITEM_WEAPON_1,
	BILLBOARDTEXTURE_ITEM_WEAPON_2,
	BILLBOARDTEXTURE_ITEM_WEAPON_3,
	BILLBOARDTEXTURE_ITEM_LIFE,
	BILLBOARDTEXTURE_SWORD,
	BILLBOARDTEXTURE_MAX,
}BILLBOARDTEXTURE;

typedef struct
{
	D3DXVECTOR3	pos;			// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3	posOld;			// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3	move;			// �I�u�W�F�N�g�̈ʒu
	D3DXVECTOR3	rot;			// �I�u�W�F�N�g�̈ʒu
	D3DXCOLOR	col;			// �I�u�W�F�N�g�̐F
	D3DXVECTOR3	fRadius;		// �I�u�W�F�N�g�̌���
	D3DXVECTOR3	fInitRadius;	// �I�u�W�F�N�g�̌���
	float		fTex;			// �e�N�X�`��
	D3DXMATRIX	mtxWorld;		// ���[���h�}�g���b�N�X
	int			nTex;			// �e�N�X�`���ԍ�
	int			nIdxShadow;		// �e�ԍ�
	bool		bUse;			// �g�p���Ă��邩
}BillboardObject;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitBillboardObject(void);			// �I�u�W�F�N�g�̏���������
void UninitBillboardObject(void);		// �I�u�W�F�N�g�̏I������
void UpdateBillboardObject(void);		// �I�u�W�F�N�g�̍X�V����
void DrawBillboardObject(void);			// �I�u�W�F�N�g�̕`�揈��

void SetBillboardObject(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, D3DXVECTOR3 fRadius, int nTex);		// �I�u�W�F�N�g�̐ݒ菈��
void SetPosionRockOn(D3DXVECTOR3 pos);
void SetPosionHP(D3DXVECTOR3 pos, int nBillBoardHP);
void SetUpdateHP(int nBillBoardHP, int nInitHP, int nNowHP);
int SetHPNum(D3DXVECTOR3 pos, D3DXVECTOR3 fRadius);
bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMax, D3DXVECTOR3 *vtxMin);
bool CollisionSword(D3DXVECTOR3 *pPos);
void DeleteBillboardObject(int nCntBillboardObject);		// �I�u�W�F�N�g�̔j������
void DeleteBillboardObject(BILLBOARDTEXTURE nTex);		// �I�u�W�F�N�g�̔j������
void DeleteBillboardObject(void);		// �I�u�W�F�N�g�̔j������

#endif
