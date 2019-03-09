//=========================================================================================================================
// 
// �T�E���h���� [sound.h]
// Author : AKIRA TANAKA
// 
//=========================================================================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"	// ���C��

//****************************************************
// �T�E���h�t�@�C��
//****************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// BGM�^�C�g��
	SOUND_LABEL_BGM001,			// BGM�o�g���v�����[�O
	SOUND_LABEL_BGM002,			// BGM�o�g�����[�v
	SOUND_LABEL_BGM003,			// BGM�o�g���{�X
	SOUND_LABEL_BGM004,			// BGM�����L���O
	SOUND_LABEL_SE_SHOT000,		// �e���ˉ�0
	SOUND_LABEL_SE_SHOT001,		// �e���ˉ�1
	SOUND_LABEL_SE_HIT000,		// �q�b�g��0
	SOUND_LABEL_SE_HIT001,		// �q�b�g��1
	SOUND_LABEL_SE_ITEMHIT000,	// �A�C�e���q�b�g��
	SOUND_LABEL_SE_BULLETUP000,	// �e�ύX��
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_DECIDE,		// ���艹
	SOUND_LABEL_SE_SELECT,		// �I����
	SOUND_LABEL_SE_PAUSE,		// �|�[�Y��
	SOUND_LABEL_SE_CLEAR,		// �Q�[���N���A��
	SOUND_LABEL_SE_OVER,		// �Q�[���I�[�o�[��
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
