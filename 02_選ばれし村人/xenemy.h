//=========================================================================================================================
//
// �G���� [xenemy.h]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================
#ifndef _XENEMY_H_
#define _XENEMY_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"
#include "game.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define XENEMY_TEXT_NAME			(255)						
#define XENEMY_TEXT_INFO			"data\\TEXT\\enemydata.ini"
//#define XENEMY_TEXT_INFO			"data\\TEXT\\enemymodeldata.txt"
#define MAX_XENEMYPARTS				(20)					
#define MAX_XENEMYANIM				(12)					
#define MAX_XENEMYANIMKEY			(15)					

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �G���
	ENEMYTYPE_1 = 0,
	ENEMYTYPE_2,
	ENEMYTYPE_3,
	ENEMYTYPE_BOSS,
	ENEMYTYPE_MAX
}ENEMYTYPE;

typedef enum
{// �G���
	ENEMYSTATE_NONE = 0,
	ENEMYSTATE_NORMAL,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_DIE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef enum
{// �G�A�j���[�V����
	ENEMYANIM_NEUTRAL = 0,
	ENEMYANIM_WALK,
	ENEMYANIM_ATTACK,
	ENEMYANIM_ATTACK2,
	ENEMYANIM_MAX
}ENEMYANIM;

typedef enum
{// �G�p�^�[��
	ENEMYPATTERN_NONE = 0,
	ENEMYPATTERN_WALKINIT,
	ENEMYPATTERN_WALKRANDOM,
	ENEMYPATTERN_ATTACKSET,
	ENEMYPATTERN_ATTACK,
	ENEMYPATTERN_MAX,
}ENEMYPATTERN;

typedef struct
{
	char						FileName[XENEMY_TEXT_NAME];		// X�t�@�C����
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
	D3DXVECTOR3					posAnim[MAX_XENEMYANIM][MAX_XENEMYANIMKEY];		// �A�j���[�V����pos
	D3DXVECTOR3					rotAnim[MAX_XENEMYANIM][MAX_XENEMYANIMKEY];		// �A�j���[�V����rot
	D3DXVECTOR3					vtxMin;							// ���_�̍ŏ��l
	D3DXVECTOR3					vtxMax;							// ���_�̍ő�l
}XENEMYMODEL;

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
	int							nCntTime;						// �J�E���^�[
	int							nMaxPartsNum;					// �p�[�c��
	XENEMYMODEL					aModel[MAX_XENEMYPARTS];		// �p�[�c���
	int							nMaxAnimNum;					// �A�j���[�V������
	int							nAnimloop[MAX_XENEMYANIM];		// ���[�v
	int							nAnimkey[MAX_XENEMYANIM];		// �L�[��
	int							nAnimBlend[MAX_XENEMYANIM];		// �u�����h��
	int							nAnimfram[MAX_XENEMYANIM][MAX_XENEMYANIMKEY];		// �t���[����
	int							nAnimChange;					// �A�j���[�V�����`�F���W
	ENEMYANIM					nAnimnow;						// ���݃A�j���[�V����
	int							nAnimkeynow;					// ���݃L�[
	ENEMYSTATE					state;							// ���
	int							nCntstate;						// ��ԃJ�E���^
	int							nLife;							// �̗�
	int							nInitLife;						// �����ݒ�̗�
	int							nBillBoardHP;					// �̗̓e�N�X�`��
	bool						bAttack;						// �g�p���Ă��邩
	int							nSetAttack;						// �U���p�^�[��
	int							nCntTimerand;					// �J�E���^�[
	ENEMYPATTERN				Pattern;						// �s���p�^�[��
	bool						bUse;							// �g�p���Ă��邩
}XENEMY;

typedef struct
{
	int							nSetNum;						// �\����
	char						FileName[XENEMY_TEXT_NAME];		// X�t�@�C����
}XENEMYINI;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitXEnemy(void);				// �G����������
void UninitXEnemy(void);			// �G�I������
void UpdateXEnemy(void);			// �G�X�V����
void DrawXEnemy(void);				// �G�`�揈��

void SetXEnemy(int type, D3DXVECTOR3 pos);				// �G�ݒ菈��
XENEMY *GetXEnemy(void);			// �G�擾����
int GetXEnemyNum(void);				// �G�����擾
D3DXVECTOR3 GetRockOnXEnemy(int nNum);	// �G���b�N�I���ԍ�
void ChangeRockOnXEnemy(int nNum);	// �G���b�N�I���ύX
bool Collision(int nCntXEnemy1, int nCntXEnemyNum1);
bool CollisionXEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
bool CollisionXEnemyWeapon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax);
void HitXEnemy(int nCntXEnemy, int nCntXEnemyNum, int nDamage);
void DeleteXEnemy(int nCntXEnemy, int nCntXEnemyNum);
void DeleteXEnemy(void);

#endif
