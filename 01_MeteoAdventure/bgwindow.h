//=========================================================================================================================
// 
// �w�i���� [bgwindow.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _BGWINDOW_H_
#define _BGWINDOW_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"
#include "game.h"		// �Q�[��

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define BGWINDOWTEXTURE_NAME_1	"data\\TEXTURE\\sky001.png"				// �ǂݍ��ރe�N�X�`���t�@�C��1	�^�C�g���i�w�i�j
#define BGWINDOWTEXTURE_NAME_2	"data\\TEXTURE\\titlelogo000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��2	�^�C�g���i���S�j
#define BGWINDOWTEXTURE_NAME_3	"data\\TEXTURE\\titleground000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��3	�^�C�g���i�n�ʁj
#define BGWINDOWTEXTURE_NAME_4	"data\\TEXTURE\\gamemap000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��4	�Q�[���i�}�b�v�j
#define BGWINDOWTEXTURE_NAME_5	"data\\TEXTURE\\pausekey000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��5	�Q�[���iP�L�[�j
#define BGWINDOWTEXTURE_NAME_6	"data\\TEXTURE\\stage000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��6	�Q�[���i�X�e�[�W�ԍ��j
#define BGWINDOWTEXTURE_NAME_7	"data\\TEXTURE\\hiscoretxt000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��7	�Q�[���i�n�C�X�R�A�e�L�X�g�j
#define BGWINDOWTEXTURE_NAME_8	"data\\TEXTURE\\all004.png"				// �ǂݍ��ރe�N�X�`���t�@�C��8	�Q�[���i�c�@�j
#define BGWINDOWTEXTURE_NAME_9	"data\\TEXTURE\\all003.png"				// �ǂݍ��ރe�N�X�`���t�@�C��9	�Q�[���i�v���C���[�j
#define BGWINDOWTEXTURE_NAME_10	"data\\TEXTURE\\life000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��10	�Q�[���i�̗́j
#define BGWINDOWTEXTURE_NAME_11	"data\\TEXTURE\\coin001.png"			// �ǂݍ��ރe�N�X�`���t�@�C��11	�Q�[���i�R�C���j
#define BGWINDOWTEXTURE_NAME_12	"data\\TEXTURE\\cointxt000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��12	�Q�[���i�R�C���e�L�X�g�j
#define BGWINDOWTEXTURE_NAME_13	"data\\TEXTURE\\help000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��13 �Q�[���i�w���v�j
#define BGWINDOWTEXTURE_NAME_14	"data\\TEXTURE\\help001.png"			// �ǂݍ��ރe�N�X�`���t�@�C��14 �Q�[���i�w���v�i�ړ��j�j
#define BGWINDOWTEXTURE_NAME_15	"data\\TEXTURE\\help002.png"			// �ǂݍ��ރe�N�X�`���t�@�C��15	�Q�[���i�w���v�i����j�j
#define BGWINDOWTEXTURE_NAME_16 "data\\TEXTURE\\help003.png"			// �ǂݍ��ރe�N�X�`���t�@�C��16 �Q�[���i�w���v�i�W�����v�j�j
#define BGWINDOWTEXTURE_NAME_17	"data\\TEXTURE\\help004.png"			// �ǂݍ��ރe�N�X�`���t�@�C��17	�Q�[���i�w���v�i�U���j�j
#define BGWINDOWTEXTURE_NAME_18	"data\\TEXTURE\\sky000.png"				// �ǂݍ��ރe�N�X�`���t�@�C��18	�Q�[���i�w�i�i��j�j
#define BGWINDOWTEXTURE_NAME_19	"data\\TEXTURE\\iwahaikei000.png" 		// �ǂݍ��ރe�N�X�`���t�@�C��19	�Q�[���i�w�i�i��j�j
#define BGWINDOWTEXTURE_NAME_20	"data\\TEXTURE\\Grass.png"				// �ǂݍ��ރe�N�X�`���t�@�C��20	�Q�[���i�؁i���j
#define BGWINDOWTEXTURE_NAME_21	"data\\TEXTURE\\Grove.png"				// �ǂݍ��ރe�N�X�`���t�@�C��21	�Q�[���i�؁i���؁j�j
#define BGWINDOWTEXTURE_NAME_22	"data\\TEXTURE\\Tree.png"				// �ǂݍ��ރe�N�X�`���t�@�C��22	�Q�[���i�؁i���j�j
#define BGWINDOWTEXTURE_NAME_23	"data\\TEXTURE\\yama.png"				// �ǂݍ��ރe�N�X�`���t�@�C��23	�Q�[���i�����i�R�j�j
#define BGWINDOWTEXTURE_NAME_24	"data\\TEXTURE\\coinwall000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��24	�Q�[���i�S�[�������j
#define BGWINDOWTEXTURE_NAME_25 "data\\TEXTURE\\flag000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��25 �Q�[���i�S�[���j
#define BGWINDOWTEXTURE_NAME_26 "data\\TEXTURE\\flag001.png"			// �ǂݍ��ރe�N�X�`���t�@�C��26 �Q�[���i�S�[���i���j�j
#define BGWINDOWTEXTURE_NAME_27	"data\\TEXTURE\\gameclear000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��27	���U���g�i�N���A�j
#define BGWINDOWTEXTURE_NAME_28 "data\\TEXTURE\\gameover000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��28	���U���g�i�I�[�o�[�j
#define BGWINDOWTEXTURE_NAME_29 "data\\TEXTURE\\press_enter.png"		// �ǂݍ��ރe�N�X�`���t�@�C��29	�I��

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �e�N�X�`�����
	BGWINDOWTEXTURE_TITLE_BG = 0,		// �^�C�g��BG
	BGWINDOWTEXTURE_TITLE_LOGO,			// �^�C�g�����S
	BGWINDOWTEXTURE_TITLE_GOUND,		// �^�C�g���n��
	BGWINDOWTEXTURE_GAME_MAP,			// �Q�[���}�b�v
	BGWINDOWTEXTURE_GAME_PKEY,			// �Q�[��P
	BGWINDOWTEXTURE_GAME_NUMSTAGE,		// �Q�[���X�e�[�W�ԍ�
	BGWINDOWTEXTURE_GAME_HISCORETXT,	// �Q�[���n�C�X�R�A�e�L�X�g
	BGWINDOWTEXTURE_GAME_NUMPLAYER,		// �Q�[���c�@
	BGWINDOWTEXTURE_GAME_PLAYER,		// �Q�[���v���C���[
	BGWINDOWTEXTURE_GAME_LIFE,			// �Q�[�����C�t
	BGWINDOWTEXTURE_GAME_COIN,			// �Q�[���R�C��
	BGWINDOWTEXTURE_GAME_COINTXT,		// �Q�[���R�C���e�L�X�g
	BGWINDOWTEXTURE_GAME_HELP,			// �Q�[���w���v
	BGWINDOWTEXTURE_GAME_HELPWALK,		// �Q�[���w���v����
	BGWINDOWTEXTURE_GAME_HELPRUN,		// �Q�[���w���v����
	BGWINDOWTEXTURE_GAME_HELPJUMP,		// �Q�[���w���v�W�����v
	BGWINDOWTEXTURE_GAME_HELPATTACK,	// �Q�[���w���v�U��
	BGWINDOWTEXTURE_GAME_STAGE,			// �Q�[���X�e�[�W
	BGWINDOWTEXTURE_GAME_STAGE_IWA,		// �Q�[���X�e�[�W��w�i
	BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER,	// �Q�[���X�e�[�W�؁i���j
	BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL,	// �Q�[���X�e�[�W�؁i���؁j
	BGWINDOWTEXTURE_GAME_STAGE_TREE_SMALL,	// �Q�[���X�e�[�W�؁i���j
	BGWINDOWTEXTURE_GAME_STAGE_SABAKU_YAMA,	// �Q�[���X�e�[�W�����i�R�j
	BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT,// �Q�[���X�e�[�W�S�[������
	BGWINDOWTEXTURE_GAME_STAGEGOAL,		// �Q�[���X�e�[�W�S�[��
	BGWINDOWTEXTURE_GAME_STAGEGOAL_SMALL,	// �Q�[���X�e�[�W�S�[���i���j
	BGWINDOWTEXTURE_RESULT_CLEAR,		// ���U���g�N���A
	BGWINDOWTEXTURE_RESULT_OVER,		// ���U���g�I�[�o�[
	BGWINDOWTEXTURE_SELECT,				// �I��
	BGWINDOWTEXTURE_MAX,
}BGWINDOWTEXTURE;

typedef enum
{// �I�����
	SELECTSTATE_NORMAL = 0,				// �m�[�}��
	SELECTSTATE_ENTER,					// �G���^�[
	SELECTSTATE_MAX,
}SELECTSTATE;

typedef struct
{// �w�i���
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 tex;					// �ʒu
	D3DXVECTOR3 texUV;					// UV
	D3DXVECTOR3 move;					// �ړ��l
	D3DXCOLOR	col;					// �F
	float		fWidth;					// ����
	float		fHeight;				// ����
	float		fAngle;					// �p�x
	float		fLenght;				// ����
	int			nCounterAnim;			// �A�j���[�V�����J�E���^
	int			nNumAnim;				// �A�j���[�V�����ԍ�
	int			nSpeedAnim;				// �A�j���[�V�������x
	int			nPatternAnim;			// �A�j���[�V�����p�^�[��
	BGWINDOWTEXTURE texture;			// �e�N�X�`��
	bool bUse;
}BGWINDOW;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitBGWindow(void);				// �w�i����������
void UninitBGWindow(void);				// �w�i�I������
void UpdateBGWindow(void);				// �w�i�X�V����
void DrawBGWindow(void);				// �w�i�`�揈��

void SetBGWindow(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, BGWINDOWTEXTURE texture);				// �w�i�ݒ菈��
void SetSelectState(SELECTSTATE state);	// �w�i�ݒ菈��
void HitPlayerLife(void);				// �v���C���[�̗͌���
bool CollisionBGWindow(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight);	// �w�i�Փˏ���
bool GetGoalHit(void);
void DeleteBGWindow(int nCntBGWindow);	// �w�i�j�������i�P�j
void DeleteBGWindow(void);				// �w�i�j�������i�S���j

#endif