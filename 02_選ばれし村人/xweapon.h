//=========================================================================================================================
//
// ���폈�� [xplayer.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _XWEAPON_H_
#define _XWEAPON_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"
#include "xplayer.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define XWEAPON_TEXT_INFO		"data\\TEXT\\weapondata.txt"
#define MAX_XWEAPON				(32)					

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// ����^�C�v
	WEAPON_NONE = 0,
	WEAPON_PLAYER,
	WEAPON_ENEMY,
	WEAPON_MAX,
}WEAPON;

typedef struct
{
	char						FileName[XPLAYER_TEXT_NAME];	// X�t�@�C����
	LPD3DXMESH					pMesh = NULL;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER				pBuffMat = NULL;				// �}�e���A�����ւ̃|�C���^
	DWORD						nNumMat = NULL;					// �}�e���A�����̐�
	D3DXMATRIX					mtxWorld;						// ���[���h�}�g���b�N�X
	int							nIndexParent;					// �e�q�֌W
	D3DXVECTOR3					Initpos;						// �����ʒu
	D3DXVECTOR3					pos;							// ���݈ʒu
	D3DXVECTOR3					posOld;							// �O��ʒu
	D3DXVECTOR3					move;							// �ړ�
	D3DXVECTOR3					Initrot;						// ��������
	D3DXVECTOR3					rotDest;						// �ڕW����
	D3DXVECTOR3					rot;							// ���݌���
	D3DXVECTOR3					vtxMin;							// ���_�̍ŏ��l
	D3DXVECTOR3					vtxMax;							// ���_�̍ő�l
	int							nHitNum;						// ����������
	WEAPON						nType;
	bool						bUse;							// �g�p���Ă��邩
}XWEAPONMODEL;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitXWeapon(void);			// ���평��������
void UninitXWeapon(void);		// ����I������
void UpdateXWeapon(void);		// ����X�V����
void DrawXWeapon(void);			// ����`�揈��

void SetXWeapon(WEAPONTYPE type);	// ����ݒ菈��
WEAPONTYPE GetXWeapon(void);
void SeBulletWeapon(D3DXVECTOR3 move);
void SeBulletWeaponEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move);
bool CollisionXWeapon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
bool CollisionXWeaponBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax, int nType);
void DeleteXWeapon(int nCntXWeapon);
void DeleteXWeapon(void);

#endif
