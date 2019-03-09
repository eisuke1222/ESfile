//=========================================================================================================================
// 
// �Q�[������ [game.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// �Q�[�����
	GAMESTATE_NONE = 0,		// �������Ă��Ȃ����
	GAMESTATE_MAP,			// �}�b�v�\��
	GAMESTATE_GAMEPLAY,		// �Q�[���\��
	GAMESTATE_GAMECLEAR,	// �Q�[���N���A
	GAMESTATE_GAMEOVER,		// �Q�[���I�[�o�[
	GAMESTATE_MAX
}GAMESTATE;

typedef enum
{// �X�e�[�W
	GAMESTAGE_NO1 = 0,		// �X�e�[�W�P
	GAMESTAGE_NO2,			// �X�e�[�W�Q
	GAMESTAGE_NO3,			// �X�e�[�W�R
	GAMESTAGE_NO4,			// �X�e�[�W�S
	GAMESTAGE_NO5,			// �X�e�[�W�T
	GAMESTAGE_MAX
}GAMESTAGE;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitGame(void);							// �Q�[������������
void UninitGame(void);							// �Q�[���I������
void UpdateGame(void);							// �Q�[���X�V����
void DrawGame(void);							// �Q�[���`�揈��

void SetGameState(GAMESTATE state);				// �Q�[���ݒ菈��
GAMESTATE GetGameState(void);					// �Q�[���擾����
void SetNumStage(int stage);					// �Q�[���X�e�[�W�ݒ�
GAMESTAGE GetNumStage(void);					// �Q�[���X�e�[�W�擾
void SetStageClearState(void);					// �Q�[���X�e�[�W�N���A�ݒ�
bool *GetStageClearState(void);					// �Q�[���X�e�[�W�N���A�擾
void SetPauseState(bool bPause);				// �|�[�Y��Ԑݒ�
bool GetPauseState(void);						// �|�[�Y��Ԏ擾
void GameSoundStop(void);						// �Q�[��BGM��~

#endif