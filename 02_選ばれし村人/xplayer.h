//=========================================================================================================================
//
// �v���C���[���� [xplayer.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _XPLAYER_H_
#define _XPLAYER_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define XPLAYER_TEXT_NAME			(255)						
#define XPLAYER_TEXT_INFO			"data\\TEXT\\playermodeldata.txt"
#define MAX_XPLAYER					(20)					
#define MAX_XPLAYERANIM				(12)					
#define MAX_XPLAYERANIMKEY			(10)					

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �A�j���[�V����
	PLAYERANIM_NEUTRAL = 0,
	PLAYERANIM_WALK,
	PLAYERANIM_SWORD_1,
	PLAYERANIM_SWORD_2,
	PLAYERANIM_SWORD_3,
	PLAYERANIM_BOW,
	PLAYERANIM_WAND,
	PLAYERANIM_MAX
}PLAYERANIM;

typedef enum
{// �v���C���[���
	PLAYERSTATE_NONE = 0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_DIE,
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{// ������
	WEAPONTYPE_SWORD = 0,
	WEAPONTYPE_BOW,
	WEAPONTYPE_WAND,
	WEAPONTYPE_SAGITTAL,
	WEAPONTYPE_NONE,
	WEAPONTYPE_MAX
}WEAPONTYPE;

typedef struct
{
	char						FileName[XPLAYER_TEXT_NAME];	// X�t�@�C����
	LPD3DXMESH					pMesh = NULL;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER				pBuffMat = NULL;				// �}�e���A�����ւ̃|�C���^
	DWORD						nNumMat = NULL;					// �}�e���A�����̐�
	D3DXMATRIX					mtxWorld;						// ���[���h�}�g���b�N�X
	int							nIndexParent;					// �e�q�֌W
	D3DXVECTOR3					Initpos;						// �����ʒu
	D3DXVECTOR3					posDest;						// �ڕW�ʒu
	D3DXVECTOR3					pos;							// ���݈ʒu
	D3DXVECTOR3					move;							// �ړ�
	D3DXVECTOR3					Initrot;						// ��������
	D3DXVECTOR3					rotDest;						// �ڕW����
	D3DXVECTOR3					rot;							// ���݌���
	D3DXVECTOR3					posAnim[MAX_XPLAYERANIM][MAX_XPLAYERANIMKEY];		// �A�j���[�V����pos
	D3DXVECTOR3					rotAnim[MAX_XPLAYERANIM][MAX_XPLAYERANIMKEY];		// �A�j���[�V����rot
	D3DXVECTOR3					vtxMin;							// ���_�̍ŏ��l
	D3DXVECTOR3					vtxMax;							// ���_�̍ő�l
}XPLAYERMODEL;

typedef struct
{
	D3DXMATRIX					mtxWorld;						// ���[���h�}�g���b�N�X
	int							nIdxShadow;						// �e�ԍ�
	D3DXVECTOR3					Initpos;						// �����ʒu
	D3DXVECTOR3					pos;							// ���݈ʒu
	D3DXVECTOR3					posOld;							// �O��ʒu
	D3DXVECTOR3					move;							// �ړ�
	D3DXVECTOR3					Initrot;						// ��������
	D3DXVECTOR3					rotDest;						// �ڕW����
	D3DXVECTOR3					rot;							// ���݌���
	D3DXCOLOR					col;							// �F
	D3DXVECTOR3					vtxMin;							// ���_�̍ŏ��l
	D3DXVECTOR3					vtxMax;							// ���_�̍ő�l
	int							nMaxPartsNum;					// �p�[�c��
	XPLAYERMODEL				aModel[MAX_XPLAYER];			// �p�[�c���
	int							nMaxAnimNum;					// �A�j���[�V������
	int							nAnimloop[MAX_XPLAYERANIM];		// ���[�v
	int							nAnimkey[MAX_XPLAYERANIM];		// �L�[��
	int							nAnimBlend[MAX_XPLAYERANIM];	// �u�����h��
	int							nAnimfram[MAX_XPLAYERANIM][MAX_XPLAYERANIMKEY];		// �t���[����
	int							nAnimChange;					// �A�j���[�V�����`�F���W
	PLAYERANIM					nAnimnow;						// ���݃A�j���[�V����
	int							nAnimkeynow;					// ���݃L�[
	int							nCntstate;						// ��ԃJ�E���^
	PLAYERSTATE					state;							// ���
	WEAPONTYPE					nWeapon;						// ���ݕ���
	int							nChargeNum;						// ����
	int							nLife;							// �̗�
	int							nInitLife;						// �����̗�
	int							nWeapon_1Lv;
	int							nWeapon_2Lv;
	int							nWeapon_3Lv;
	bool						bAttack;						// �U�����Ă��邩
	bool						bNextAttack;					// �U���𑱂��邩
	int							nCntChargeAttack;				// �U���𑱂��邩
	bool						bChargeAttack;					// �U���𑱂��邩
	bool						bChargeOn;						// �U���𑱂��邩
	bool						bMove;							// �g�p���Ă��邩
	bool						bUse;							// �g�p���Ă��邩
}XPLAYER;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitXPlayer(void);				// �v���C���[����������
void UninitXPlayer(void);			// �v���C���[�I������
void UpdateXPlayer(void);			// �v���C���[�X�V����
void DrawXPlayer(void);				// �v���C���[�`�揈��

void SetXPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);				// �v���C���[�ݒ菈��
XPLAYER *GetXPlayer(void);			// �v���C���[�擾����
void SetXPlayerHP(int nLife);
int GetXPlayerHP(void);
bool CollisionXPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
void SetHitItem(int nItem);
void SetWeaponLv(int nWeapon_1, int nWeapon_2, int nWeapon_3);
void SetMove(bool bMove);
void DeleteXPlayer(void);

#endif
