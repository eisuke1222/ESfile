//=========================================================================================================================
// 
// �E�B���h�E���� [window.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _WINDOW_H_
#define _WINDOW_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"
#include "game.h"		// �Q�[��

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define WINDOWTEXTURE_NAME_1	"data\\TEXTURE\\title000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��1	�^�C�g���i�^�C�g���j
#define WINDOWTEXTURE_NAME_2	"data\\TEXTURE\\kouhakuhikari.png"		// �ǂݍ��ރe�N�X�`���t�@�C��2	�^�C�g���i�A�j���[�V�����j
#define WINDOWTEXTURE_NAME_3	"data\\TEXTURE\\tutorial001.png"		// �ǂݍ��ރe�N�X�`���t�@�C��3	�Q�[���i�`���[�g���A���j
#define WINDOWTEXTURE_NAME_4	"data\\TEXTURE\\tutorial003.png"		// �ǂݍ��ރe�N�X�`���t�@�C��4	�Q�[���i�`���[�g���A���j
#define WINDOWTEXTURE_NAME_5	"data\\TEXTURE\\tutorial100.png"		// �ǂݍ��ރe�N�X�`���t�@�C��5	�Q�[���i�Q�[���`���[�g���A���j
#define WINDOWTEXTURE_NAME_6	"data\\TEXTURE\\press_help000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��6	�Q�[���i�`���[�g���A���R�����g�J���j
#define WINDOWTEXTURE_NAME_7	"data\\TEXTURE\\press_help001.png"		// �ǂݍ��ރe�N�X�`���t�@�C��7	�Q�[���i�`���[�g���A���R�����g����j
#define WINDOWTEXTURE_NAME_8	"data\\TEXTURE\\tutorial_message.png"	// �ǂݍ��ރe�N�X�`���t�@�C��8	�Q�[���i�`���[�g���A�����b�Z�[�W�j
#define WINDOWTEXTURE_NAME_9	"data\\TEXTURE\\map000.png"				// �ǂݍ��ރe�N�X�`���t�@�C��9	�Q�[���i�}�b�v�j
#define WINDOWTEXTURE_NAME_10	"data\\TEXTURE\\map001.png"				// �ǂݍ��ރe�N�X�`���t�@�C��10	�Q�[���i�}�b�v�ʒu�j
#define WINDOWTEXTURE_NAME_11	"data\\TEXTURE\\hp001.png"				// �ǂݍ��ރe�N�X�`���t�@�C��11	�Q�[���iHPBG�j
#define WINDOWTEXTURE_NAME_12	"data\\TEXTURE\\hp001.png"				// �ǂݍ��ރe�N�X�`���t�@�C��12	�Q�[���iHP�j
#define WINDOWTEXTURE_NAME_13	"data\\TEXTURE\\hp002.png"				// �ǂݍ��ރe�N�X�`���t�@�C��13	�Q�[���iHP�}�[�N�j
#define WINDOWTEXTURE_NAME_14	"data\\TEXTURE\\weapon000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��14	�Q�[���i����1�j
#define WINDOWTEXTURE_NAME_15	"data\\TEXTURE\\weapon001.png"			// �ǂݍ��ރe�N�X�`���t�@�C��15	�Q�[���i����2�j
#define WINDOWTEXTURE_NAME_16	"data\\TEXTURE\\weapon002.png"			// �ǂݍ��ރe�N�X�`���t�@�C��16	�Q�[���i����3�j
#define WINDOWTEXTURE_NAME_17	"data\\TEXTURE\\weapon100.png"			// �ǂݍ��ރe�N�X�`���t�@�C��17	�Q�[���i����g�j
#define WINDOWTEXTURE_NAME_18	"data\\TEXTURE\\brack000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��18	���U���g�i�w�i�j
#define WINDOWTEXTURE_NAME_19	"data\\TEXTURE\\gameclear001.png"		// �ǂݍ��ރe�N�X�`���t�@�C��19	���U���g�i�N���A�j
#define WINDOWTEXTURE_NAME_20	"data\\TEXTURE\\gameover001.png"		// �ǂݍ��ރe�N�X�`���t�@�C��20	���U���g�i�I�[�o�[�j
#define WINDOWTEXTURE_NAME_21	"data\\TEXTURE\\ToBeContinued.png"		// �ǂݍ��ރe�N�X�`���t�@�C��21	���U���g�i���܂��j
#define WINDOWTEXTURE_NAME_22	"data\\TEXTURE\\ranking000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��22	�����L���O�i�����L���O�j
#define WINDOWTEXTURE_NAME_23	"data\\TEXTURE\\ranking001.png"			// �ǂݍ��ރe�N�X�`���t�@�C��23	�����L���O�i1�j
#define WINDOWTEXTURE_NAME_24	"data\\TEXTURE\\ranking002.png"			// �ǂݍ��ރe�N�X�`���t�@�C��24	�����L���O�i2�j
#define WINDOWTEXTURE_NAME_25	"data\\TEXTURE\\ranking003.png"			// �ǂݍ��ރe�N�X�`���t�@�C��25	�����L���O�i3�j
#define WINDOWTEXTURE_NAME_26	"data\\TEXTURE\\ranking004.png"			// �ǂݍ��ރe�N�X�`���t�@�C��26 �����L���O�i4�j
#define WINDOWTEXTURE_NAME_27	"data\\TEXTURE\\ranking005.png"			// �ǂݍ��ރe�N�X�`���t�@�C��27	�����L���O�i5�j
#define WINDOWTEXTURE_NAME_28	"data\\TEXTURE\\rankingmessage000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��27	�����L���O�i���b�Z�[�WA�j
#define WINDOWTEXTURE_NAME_29	"data\\TEXTURE\\rankingmessage001.png"	// �ǂݍ��ރe�N�X�`���t�@�C��28	�����L���O�i���b�Z�[�WB�j
#define WINDOWTEXTURE_NAME_30	"data\\TEXTURE\\rankingmessage002.png"	// �ǂݍ��ރe�N�X�`���t�@�C��29	�����L���O�i���b�Z�[�WC�j
#define WINDOWTEXTURE_NAME_31	"data\\TEXTURE\\rankingmessage003.png"	// �ǂݍ��ރe�N�X�`���t�@�C��30	�����L���O�i���b�Z�[�WD�j
#define WINDOWTEXTURE_NAME_32	"data\\TEXTURE\\message000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��31 ���b�Z�[�W
#define WINDOWTEXTURE_NAME_33	"data\\TEXTURE\\press_button000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��32 �I��
#define WINDOWTEXTURE_NAME_34	"data\\TEXTURE\\press_button001.png"	// �ǂݍ��ރe�N�X�`���t�@�C��33 �I��

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �e�N�X�`�����
	WINDOWTEXTURE_TITLE = 0,			// �^�C�g��
	WINDOWTEXTURE_TITLE_ANIM,			// �^�C�g���A�j���[�V����
	WINDOWTEXTURE_GAME_TUTORIAL,		// �Q�[���i�`���[�g���A���j
	WINDOWTEXTURE_GAME_TUTORIAL_P,		// �Q�[���i�`���[�g���A���j
	WINDOWTEXTURE_GAME_TUTORIAL_GAME,	// �Q�[���i�Q�[���`���[�g���A���j
	WINDOWTEXTURE_GAME_TUTORIAL_IN,		// �Q�[���i�`���[�g���A���J���j
	WINDOWTEXTURE_GAME_TUTORIAL_OUT,	// �Q�[���i�`���[�g���A���R�����g����j
	WINDOWTEXTURE_GAME_TUTORIAL_MESSAGE,// �Q�[���i�`���[�g���A�����b�Z�[�W�j
	WINDOWTEXTURE_GAME_MAP,				// �Q�[���i�}�b�v�j
	WINDOWTEXTURE_GAME_MAP_POS,			// �Q�[���i�}�b�v�ʒu�j
	WINDOWTEXTURE_GAME_HPBG,			// �Q�[���iHPBG�j
	WINDOWTEXTURE_GAME_HP,				// �Q�[���iHP�j
	WINDOWTEXTURE_GAME_HP_M,			// �Q�[���iHP�}�[�N�j
	WINDOWTEXTURE_GAME_WEAPON_1,		// �Q�[���i����1�j
	WINDOWTEXTURE_GAME_WEAPON_2,		// �Q�[���i����2�j
	WINDOWTEXTURE_GAME_WEAPON_3,		// �Q�[���i����3�j
	WINDOWTEXTURE_GAME_WEAPON_99,		// �Q�[���i����g�j
	WINDOWTEXTURE_RESULT_BG,			// ���U���g�N���A
	WINDOWTEXTURE_RESULT_CLEAR,			// ���U���g�N���A
	WINDOWTEXTURE_RESULT_OVER,			// ���U���g�I�[�o�[
	WINDOWTEXTURE_RESULT_OTHER,			// ���U���g
	WINDOWTEXTURE_RANKING_BG,			// �����L���O
	WINDOWTEXTURE_RANKING_1,			// �����L���O1
	WINDOWTEXTURE_RANKING_2,			// �����L���O2
	WINDOWTEXTURE_RANKING_3,			// �����L���O3
	WINDOWTEXTURE_RANKING_4,			// �����L���O4
	WINDOWTEXTURE_RANKING_5,			// �����L���O5
	WINDOWTEXTURE_RANKING_A,			// �����L���O���b�Z�[�WA
	WINDOWTEXTURE_RANKING_B,			// �����L���O���b�Z�[�WB
	WINDOWTEXTURE_RANKING_C,			// �����L���O���b�Z�[�WC
	WINDOWTEXTURE_RANKING_D,			// �����L���O���b�Z�[�WD
	WINDOWTEXTURE_MESSAGE,				// ���b�Z�[�W
	WINDOWTEXTURE_SELECT_1,				// �I��1
	WINDOWTEXTURE_SELECT_2,				// �I��2
	WINDOWTEXTURE_MAX,
	WINDOW_TUTORIAL,
	WINDOW_FADE,
}WINDOWTEXTURE;

typedef enum
{// �I�����
	SELECTSTATE_NORMAL = 0,				// �m�[�}��
	SELECTSTATE_ENTER,					// �G���^�[
	SELECTSTATE_MAX,
}SELECTSTATE;

typedef enum
{
	SETTEXTURE_FRONT = 0,
	SETTEXTURE_TOPFRONT,
	SETTEXTURE_BG,
	SETTEXTURE_TOPBG,
	SETTEXTURE_MAX,
}SETTEXTURE;

typedef struct
{// �E�B���h�E���
	D3DXVECTOR3 pos;					// �ʒu
	D3DXVECTOR3 tex;					// �ʒu
	D3DXVECTOR3 texUV;					// UV
	D3DXVECTOR3 move;					// �ړ��l
	D3DXCOLOR	col;					// �F
	float		fWidth;					// ����
	float		fHPWidth;				// ����
	float		fHeight;				// ����
	float		fAngle;					// �p�x
	float		fLenght;				// ����
	int			nCntTimer;				// ���ԃJ�E���^
	int			nCounterAnim;			// �A�j���[�V�����J�E���^
	int			nNumAnim;				// �A�j���[�V�����ԍ�
	int			nSpeedAnim;				// �A�j���[�V�������x
	int			nPatternAnim;			// �A�j���[�V�����p�^�[��
	WINDOWTEXTURE texture;				// �e�N�X�`�����
	SETTEXTURE type;					// �\���^�C�v
	bool bUse;
}WINDOW;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitWindow(void);				// �E�B���h�E����������
void UninitWindow(void);			// �E�B���h�E�I������
void UpdateWindow(void);			// �E�B���h�E�X�V����
void DrawTopFrontWindow(void);		// �E�B���h�E��ԑO�i�`�揈��
void DrawFrontWindow(void);			// �E�B���h�E�O�i�`�揈��
void DrawTopBGWindow(void);			// �E�B���h�E�w�i�`�揈��
void DrawBGWindow(void);			// �E�B���h�E�w�i�`�揈��

void SetWindow(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, WINDOWTEXTURE texture, SETTEXTURE type);				// �E�B���h�E�ݒ菈��
void SetSelectState(SELECTSTATE state);			// �E�B���h�E�ݒ菈��
void DeleteWindow(int nCntWindow);				// �E�B���h�E�j�������i�P�j
void DeleteWindow(WINDOWTEXTURE texture);		// �E�B���h�E�j�������i�e�N�X�`���j
void DeleteWindow(void);						// �E�B���h�E�j�������i�S���j

#endif