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
	SOUND_LABEL_BGM_GAME_MAP,	// BGM�Q�[���}�b�v
	SOUND_LABEL_BGM_GAME_STAGE1,// BGM�X�e�[�W1
	SOUND_LABEL_BGM_GAME_STAGE2,// BGM�X�e�[�W2
	SOUND_LABEL_BGM_GAME_STAGE3,// BGM�X�e�[�W3
	SOUND_LABEL_BGM_GAME_CLEAR,	// BGM�Q�[���N���A
	SOUND_LABEL_BGM_GAME_OVER,	// BGM�Q�[���I�[�o�[
	SOUND_LABEL_SE_GAME_STAGE_CLEAR,// �X�e�[�W�N���A
	SOUND_LABEL_SE_JUMP000,		// �W�����v
	SOUND_LABEL_SE_SHOT000,		// �e���ˉ�
	SOUND_LABEL_SE_HIT000,		// �q�b�g��0
	SOUND_LABEL_SE_HIT001,		// �q�b�g��1
	SOUND_LABEL_SE_ITEMHIT000,	// �A�C�e���q�b�g��
	SOUND_LABEL_SE_BULLETUP000,	// �e�ύX��
	SOUND_LABEL_SE_DECIDE,		// ���艹
	SOUND_LABEL_SE_SELECT001,	// �I����1
	SOUND_LABEL_SE_SELECT002,	// �I����2
	SOUND_LABEL_SE_PAUSEIN,		// �|�[�Y����
	SOUND_LABEL_SE_PAUSEOUT,	// �|�[�Y�o��
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
