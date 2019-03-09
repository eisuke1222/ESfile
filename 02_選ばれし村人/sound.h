//=========================================================================================================================
// 
// �T�E���h���� [sound.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"

//****************************************************
// �T�E���h�t�@�C��
//****************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,	// BGM�^�C�g��
	SOUND_LABEL_BGM_GAME_0,		// BGM�Q�[��0
	SOUND_LABEL_BGM_GAME_1,		// BGM�Q�[��1
	SOUND_LABEL_SE_TITLE_0,		// SE�^�C�g��0
	SOUND_LABEL_SE_TITLE_1,		// SE�^�C�g��1
	SOUND_LABEL_SE_ATTACK_0,	// SE�U��0
	SOUND_LABEL_SE_ATTACK_1,	// SE�U��1
	SOUND_LABEL_SE_ATTACK_2,	// SE�U��2
	SOUND_LABEL_SE_ATTACK_99,	// SE�U��99
	SOUND_LABEL_SE_DAMAGE,		// SE����
	SOUND_LABEL_SE_ITEM,		// SE�A�C�e���擾
	SOUND_LABEL_SE_LVUP,		// SE���x���A�b�v
	SOUND_LABEL_SE_WOLK,		// SE����
	SOUND_LABEL_SE_PAUSE_0,		// SE�|�[�Y
	SOUND_LABEL_SE_PAUSE_1,		// SE�|�[�Y�I��
	SOUND_LABEL_SE_RESULT_0,	// SE���U���g0
	SOUND_LABEL_SE_RESULT_1,	// SE���U���g1
	SOUND_LABEL_SE_RESULT_2,	// SE���U���g2
	SOUND_LABEL_SE_RESULT_3,	// SE���U���g3
	SOUND_LABEL_SE_RANKING,		// SE�����L���O
	SOUND_LABEL_SE_MESSAGE,		// SE���b�Z�[�W
	SOUND_LABEL_SE_ENTER,		// SE����
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
HRESULT InitSound(HWND hWnd);			// �T�E���h����������
void UninitSound(void);					// �T�E���h�I������
HRESULT PlaySound(SOUND_LABEL label);	// �T�E���h�Đ�����
void StopSound(SOUND_LABEL label);		// �T�E���h��~����
void StopSound(void);					// �T�E���h�S��~����

#endif
