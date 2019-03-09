//=========================================================================================================================
//
// �I�u�W�F�N�g���� [xobjectr.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _XOBJECT_H_
#define _XOBJECT_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define XOBJECT_TEXT_NAME			(255)				
#define XOBJECT_TEXT_INFO			"data\\TEXT\\objectdata.ini"

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �I�u�W�F�N�g�ԍ�
	TEXTTYPE_TREE_0 = 0,
	TEXTTYPE_TREE_1,
	TEXTTYPE_TREE_2,
	TEXTTYPE_TREE_3,
	TEXTTYPE_TREE_4,
	TEXTTYPE_TREE_5,
	TEXTTYPE_TREE_6,
	TEXTTYPE_TREE_7,
	TEXTTYPE_TREE_8,
	TEXTTYPE_TREE_9,
	TEXTTYPE_TREE_10,
	TEXTTYPE_TREE_11,
	TEXTTYPE_TREE_12,
	TEXTTYPE_TREE_13,
	TEXTTYPE_TREE_14,
	TEXTTYPE_MARUTA_1,
	TEXTTYPE_MARUTA_2,
	TEXTTYPE_MARUTA_3,
	TEXTTYPE_MAX

}TEXTTYPE;

typedef struct
{
	char						FileName[XOBJECT_TEXT_NAME];	// X�t�@�C����
	LPDIRECT3DTEXTURE9			*pTexture = NULL;				// �e�N�X�`�����ւ̃|�C���^
	LPD3DXMESH					pMesh = NULL;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER				pBuffMat = NULL;				// �}�e���A�����ւ̃|�C���^
	DWORD						nNumMat = NULL;					// �}�e���A�����̐�
	int							nMaxModel;						// ���f���̍ő吔
	int							nOya;							// �e�q�֌W
	D3DXVECTOR3					Initpos;						// �����ʒu
	D3DXVECTOR3					pos;							// ���݈ʒu
	D3DXVECTOR3					move;							// �ړ�
	D3DXVECTOR3					Initrot;						// ��������
	D3DXVECTOR3					rotDest;						// �ڕW����
	D3DXVECTOR3					rot;							// ���݌���
	D3DXVECTOR3					vtxMin;							// ���_�̍ŏ��l
	D3DXVECTOR3					vtxMax;							// ���_�̍ő�l
	D3DXCOLOR					col;							// �F
	D3DXMATRIX					mtxWorld;						// ���[���h�}�g���b�N�X
	int							nIdxShadow;						// �e�ԍ�
	bool						bUse;							// �g�p���Ă��邩
}XOBJECT;

typedef struct
{
	char						FileName[XOBJECT_TEXT_NAME];	// X�t�@�C����
}XOBJECTINI;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitXObject(void);				// �I�u�W�F�N�g����������
void UninitXObject(void);			// �I�u�W�F�N�g�I������
void UpdateXObject(void);			// �I�u�W�F�N�g�X�V����
void DrawXObject(void);				// �I�u�W�F�N�g�`�揈��

void SetXObject(TEXTTYPE type);		// �I�u�W�F�N�g�ݒ菈��
bool CollisionXObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
void DeleteXObject(void);

#endif
