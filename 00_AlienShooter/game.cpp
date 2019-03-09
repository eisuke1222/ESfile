//=========================================================================================================================
//
// �Q�[������ [game.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include <time.h>		// time
#include "game.h"		// �Q�[��
#include "player.h"		// �v���C���[
#include "enemy.h"		// �G
#include "bullet.h"		// �e
#include "explosion.h"	// ����
#include "effect.h"		// �G�t�F�N�g
#include "item.h"		// �A�C�e��
#include "bg.h"			// �w�i
#include "pause.h"		// �|�[�Y
#include "fade.h"		// �t�F�[�h
#include "sound.h"		// �T�E���h
#include "input.h"		// ���͏���

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void SetStage(void);	// �X�e�[�W�ݒ�

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
GAMESTATE		g_GameState;							// �Q�[�����
int				g_nCounterGameState;					// �Q�[����ԃJ�E���^
STAGENUM		g_StageState;							// �X�e�[�W���
bool			g_bPause;								// �|�[�Y
int				g_nCounterGame;							// �Q�[���J�E���^
int				g_nCntSetEnemy;							// �G�Z�b�g�J�E���^
int				g_nCntSetStage;							// �X�e�[�W�Z�b�g�J�E���^

//=========================================================================================================================
// �Q�[������������
//=========================================================================================================================
void InitGame(void)
{
	// ��Ԑݒ�
	g_GameState = GAMESTATE_NORMAL;		// �Q�[�����
	g_nCounterGameState = 0;			// �J�E���^������
	g_StageState = STAGENUM_1;			// �X�e�[�W���
	g_bPause = false;					// �|�[�Y�����ݒ�
	g_nCounterGame = 0;					// �J�E���^������
	g_nCntSetEnemy = 0;					// �J�E���^������
	g_nCntSetStage = 0;					// �J�E���^������

	// �w�i����������
	InitBG();
	SetNumStage(g_StageState);

	// �G�̏���������
	InitEnemy();
	//SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 0, 20, 4, ENEMYTYPE_NORMALENEMY,
	//	EnemyBullet{ ENEMYBULLET_NO_6, 3.0f, 30 },
	//	EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

	// �v���C���[����������
	InitPlayer();

	// �A�C�e������������
	InitItem();

	// �����̏���������
	InitExplosion();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �e�̏���������
	InitBullet();

	// �|�[�Y�̏���������
	InitPause();
}

//=========================================================================================================================
// �Q�[���I������
//=========================================================================================================================
void UninitGame(void)
{
	// �w�i�̏I������
	UninitBG();

	// �G�̏I������
	UninitEnemy();

	// �v���C���[�̏I������
	UninitPlayer();

	// �A�C�e���I������
	UninitItem();

	// �����̏I������
	UninitExplosion();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �e�̏I������
	UninitBullet();

	// �|�[�Y�̏I������
	UninitPause();
}

//=========================================================================================================================
// �Q�[���X�V����
//=========================================================================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true && g_GameState == GAMESTATE_NORMAL)
	{// �|�[�Y��ؑ�
		g_bPause = g_bPause ? false : true;
		PlaySound(SOUND_LABEL_SE_PAUSE);
	}

	if (g_bPause == false)
	{// �|�[�Y���g�p���Ă��Ȃ��ꍇ
		// �w�i�̍X�V����
		UpdateBG();

		// �G�̍X�V����
		UpdateEnemy();

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// �A�C�e���X�V����
		UpdateItem();

		// �����̍X�V����
		UpdateExplosion();

		// �G�t�F�N�g�̍X�V����
		UpdateEffect();

		// �e�̍X�V����
		UpdateBullet();

		// �X�e�[�W�ݒ�
		SetStage();

		//�|�[�Y������Ԃ�
		SetPause();
	}

	if (g_bPause == true)
	{// �|�[�Y���g�p����Ă���ꍇ
		// �|�[�Y�̍X�V����
		UpdatePause();
	}

	switch (g_GameState)
	{
	case GAMESTATE_NORMAL:		// �m�[�}��
		break;
	case GAMESTATE_GAMECLEAR:	// �Q�[���N���A
	case GAMESTATE_GAMEOVER:	// �Q�[���I�[�o�[
		g_nCounterGameState++;	// �J�E���^�[�����Z

		if (g_nCounterGameState >= 60)
		{// ���J�E���g��
			// g_GameState = GAMESTATE_NONE;
			SetFade(MODE_RESULT);		// ���U���g
		}
		break;
	}
}

//=========================================================================================================================
// �Q�[���`�揈��
//=========================================================================================================================
void DrawGame(void)
{
	// �w�i�̕`�揈��
	DrawBG();

	// �G�̕`�揈��
	DrawEnemy();

	// �v���C���[�̕`��
	DrawPlayer();

	// �A�C�e���`�揈��
	DrawItem();

	// �����̕`�揈��
	DrawExplosion();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �e�̕`�揈��
	DrawBullet();

	if (g_bPause == true && g_GameState == GAMESTATE_NORMAL)
	{// �|�[�Y���g�p����Ă���ꍇ
		// �|�[�Y�̕`�揈��
		DrawPause();
	}
}

//=========================================================================================================================
// �Q�[���ݒ菈��
//=========================================================================================================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;			// ��Ԃ��L��
	g_nCounterGameState = 0;		// �J�E���g��0
}

//=========================================================================================================================
// �Q�[���擾����
//=========================================================================================================================
GAMESTATE GetGameState(void)
{
	return g_GameState;				// �l��Ԃ�
}

//=========================================================================================================================
// �|�[�Y�ݒ菈��
//=========================================================================================================================
void SetPauseState(bool bPause)
{
	g_bPause = bPause;				// ��Ԃ��L��
}

//=========================================================================================================================
// �Q�[���ݒ菈��
//=========================================================================================================================
void SetStageState(STAGENUM stage)
{
	g_StageState = stage;			// ��Ԃ��L��
}

//==================================================================================================
// �X�e�[�W�ݒ�
//==================================================================================================
void SetStage(void)
{
	Enemy *pEnemy = GetEnemy();		// �G���擾
	int nNumEnemy = GetNumEnemy();	// �G�̑������擾

	g_nCounterGame++;				// �Q�[�����J�E���g

	// SetEnemy : �����ʒu�A�ڕW�ʒu�A�ړ����x�Aatan���x�A�ړ���ށA�ړ��J�n�A�ڕW�ʒu�J�E���g�A�e�N�X�`���^�C�v�A���a�A�̗́A�G���
	//			  �e�P�A�e�Q�A�e�R�A�e�S�A�e�T (�e��ށA�e���x�A���˃^�C�~���O�j

	if (g_StageState == STAGENUM_1)
	{// �X�e�[�W�P
		if (g_nCounterGame == 240)
		{// 4�b
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 300.0f) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 1, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 300.0f) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 30, 0, 1, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 200.0f) * 1.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 40, 0, 0, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -200.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 200.0f) * 1.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 50, 0, 0, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 660)
		{// 13�b
			SetEnemy(D3DXVECTOR3(0, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 400.0f) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 3, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(0, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 200.0f) * 1.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 1, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_3, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 0, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 200.0f) * 1.0f, 150.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 1, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_3, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 400.0f) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 3, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 1260)
		{// 21�b
			SetEnemy(D3DXVECTOR3(0, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 10) * 1.0f, (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 2, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 1.0f, -50.0f, 0.0f), D3DXVECTOR3(10.0f, (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING, 20, 0, 2, 20, 4, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame >= 1560 && g_nCounterGame <= 2160)
		{// 31�b
			if (g_nCounterGame % 60 == 0)
			{
				if (g_nCntSetEnemy < 10)
				{
					SetEnemy(D3DXVECTOR3(116.0f * (g_nCntSetEnemy + 1), -50.0f, 0.0f), D3DXVECTOR3(116.0f * (g_nCntSetEnemy + 1), (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 20, 300, 2, 20, 3, ENEMYTYPE_NORMALENEMY,
						EnemyBullet{ ENEMYBULLET_NO_3, 3.0f, 15 },
						EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
					g_nCntSetEnemy++;
				}
			}
		}
		else if (g_nCounterGame > 2160 && nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_NORMALENEMY)
		{// ENEMYTYPE_NORMALENEMY�����Ȃ��Ȃ����ꍇ
			StopSound();							// �T�E���h���~�߂�
			PlaySound(SOUND_LABEL_BGM003);			// �{�X�T�E���h

			// �{�X���Z�b�g
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING, 0, 0, 0, 80, 100, ENEMYTYPE_LBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame > 2460 && pEnemy->Type == ENEMYTYPE_LBOSSENEMY && pEnemy->bUse == false)
		{// �{�X�����Ȃ��Ȃ����ꍇ
			g_nCntSetStage++;

			if (g_nCntSetStage > 180)
			{
				g_StageState = STAGENUM_2;		// �X�e�[�W2�Ɉڍs
				SetNumStage(g_StageState);		// �X�e�[�W�ԍ��\��

				g_nCntSetStage = 0;
				g_nCntSetEnemy = 0;
				g_nCounterGame = 0;

				StopSound();							// �T�E���h���~�߂�
				PlaySound(SOUND_LABEL_BGM002);			// �T�E���h
			}
		}
	}
	else if (g_StageState == STAGENUM_2)
	{// �X�e�[�W�Q
		if (g_nCounterGame == 240)
		{// 4�b
			SetEnemy(D3DXVECTOR3(-30, 150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 30) * 1.0f, (SCREEN_HEIGHT * 0.6) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 20, 0, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, 50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 30) * 1.0f, (SCREEN_HEIGHT * 0.6 - 100) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 30, 0, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 30) * 1.0f, (SCREEN_HEIGHT * 0.6 - 200) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 40, 0, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 30) * 1.0f, (SCREEN_HEIGHT * 0.6 - 300) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 50, 0, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, 150.0f, 0.0f), D3DXVECTOR3(-30, (SCREEN_HEIGHT * 0.6) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 20, 1, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, 50.0f, 0.0f), D3DXVECTOR3(-30, (SCREEN_HEIGHT * 0.6 - 100) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 30, 1, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, -50.0f, 0.0f), D3DXVECTOR3(-30, (SCREEN_HEIGHT * 0.6 - 200) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 40, 0, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, -150.0f, 0.0f), D3DXVECTOR3(-30, (SCREEN_HEIGHT * 0.6 - 300) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_KEEP, 50, 0, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 600)
		{// 10�b
			SetEnemy(D3DXVECTOR3(-30, 150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 30) * 1.0f, (SCREEN_HEIGHT * 0.6) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, 50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 50) * 1.0f, (SCREEN_HEIGHT * 0.6 - 100) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 30, 600, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 130) * 1.0f, (SCREEN_HEIGHT * 0.6 - 200) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 40, 600, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-30, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 210) * 1.0f, (SCREEN_HEIGHT * 0.6 - 300) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 50, 600, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, 150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 30), (SCREEN_HEIGHT * 0.6) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, 50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 50), (SCREEN_HEIGHT * 0.6 - 100) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 30, 600, 1, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 130), (SCREEN_HEIGHT * 0.6 - 200) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 40, 600, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 30, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 210), (SCREEN_HEIGHT * 0.6 - 300) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 50, 600, 0, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1,3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 1260)
		{// 21�b
			SetEnemy(D3DXVECTOR3(0, -50.0f, 0.0f), D3DXVECTOR3(75, 100, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, -50.0f, 0.0f), D3DXVECTOR3(25, 100, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 40 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(25, 150, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 75) * 1.0f, 100, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 25) * 1.0f, 100, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 40 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 0.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 25) * 1.0f, 150, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 20, 600, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2,3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame >= 1860 && g_nCounterGame <= 2160)
		{// 31�b
			if (g_nCounterGame % 60 == 0)
			{
				if (g_nCntSetEnemy < 5)
				{
					SetEnemy(D3DXVECTOR3(-50.0f, -30.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 70.0f * (g_nCntSetEnemy + 1), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 4.0f, ENEMYMOVE_HOMING_KEEP, 0, 0, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
						EnemyBullet{ ENEMYBULLET_NO_4,3.0f, 20 },
						EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
					SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, -30.0f, 0.0f), D3DXVECTOR3(-50.0f, 70.0f * (g_nCntSetEnemy + 1), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 4.0f, ENEMYMOVE_HOMING_KEEP, 0, 0, 2, 20, 5, ENEMYTYPE_NORMALENEMY,
						EnemyBullet{ ENEMYBULLET_NO_4,3.0f, 20 },
						EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
					g_nCntSetEnemy++;
				}
			}
		}
		else if (nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_NORMALENEMY && g_nCounterGame > 2160)
		{// ENEMYTYPE_NORMALENEMY�����Ȃ��Ȃ����ꍇ
			StopSound();							// �T�E���h���~�߂�
			PlaySound(SOUND_LABEL_BGM003);			// �{�X�T�E���h

			// �{�X���Z�b�g
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 0, 50, 100, ENEMYTYPE_MBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 45 },
				EnemyBullet{ ENEMYBULLET_NO_4, 4.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_MBOSSENEMY && g_nCounterGame > 2460)
		{// ENEMYTYPE_NORMALENEMY�����Ȃ��Ȃ����ꍇ
			// �{�X���Z�b�g
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 1, 80, 150, ENEMYTYPE_LBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_5, 3.0f, 5 },
				EnemyBullet{ ENEMYBULLET_NO_3, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame > 2460 && pEnemy->Type == ENEMYTYPE_LBOSSENEMY && pEnemy->bUse == false)
		{// �{�X�����Ȃ��Ȃ����ꍇ
			g_nCntSetStage++;


			if (g_nCntSetStage > 180)
			{
				g_StageState = STAGENUM_3;		// �X�e�[�W2�Ɉڍs
				SetNumStage(g_StageState);		// �X�e�[�W�ԍ��\��

				g_nCntSetStage = 0;
				g_nCntSetEnemy = 0;
				g_nCounterGame = 0;

				StopSound();							// �T�E���h���~�߂�
				PlaySound(SOUND_LABEL_BGM002);			// �T�E���h
			}
		}
	}
	else if (g_StageState == STAGENUM_3)
	{// �X�e�[�W�R
		if (g_nCounterGame == 240)
		{// 4�b
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 4 - 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 0, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 4 - 100.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 100.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 35 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 4 - 150.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 150.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 40 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 4 - 200.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 200.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 3, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 + 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 0, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 + 100.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 100.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 35 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 + 150.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 150.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 45 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 + 200.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 200.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 3, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 300)
		{// 5�b
			SetEnemy(D3DXVECTOR3(-50.0f, 0, 0.0f), D3DXVECTOR3(20.0f, 200, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, 50.0f, 0.0f), D3DXVECTOR3(20.0f, 250, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 35 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, 100.0f, 0.0f), D3DXVECTOR3(20.0f, 300, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 45 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, 150.0f, 0.0f), D3DXVECTOR3(20.0f, 350, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 0, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 20.0f) * 1.0f, 200, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 20.0f) * 1.0f, 250, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 35 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 20.0f) * 1.0f, 300, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 45 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 20.0f) * 1.0f, 350, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 2, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_2, 3.0f, 50 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 600)
		{// 10�b
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, (SCREEN_HEIGHT / 2 - 100.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_14, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, (SCREEN_HEIGHT / 2 - 50.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_9, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - 50.0f) * 1.0f, (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_13, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, (SCREEN_HEIGHT / 2 - 100.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_10, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, (SCREEN_HEIGHT / 2 - 50.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 4, 20, 15, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, -150.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, (SCREEN_HEIGHT / 2 - 100.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_12, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, (SCREEN_HEIGHT / 2 - 50.0f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_8, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + 50.0f) * 1.0f, (SCREEN_HEIGHT / 2) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 1200, 1, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_11, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 1380)
		{// 23�b
			SetEnemy(D3DXVECTOR3(-50.0f, -50.0f, 0.0f), D3DXVECTOR3(50.0f, 75.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 0, 600, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, -50.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 50.0f) * 1.0f, 75.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 0, 600, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-50.0f, (SCREEN_HEIGHT + 50.0f) * 1.0f, 0.0f), D3DXVECTOR3(50.0f, (SCREEN_HEIGHT - 50.f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 0, 600, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, (SCREEN_HEIGHT + 50.0f) * 1.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH - 50.0f) * 1.0f, (SCREEN_HEIGHT - 50.f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, ENEMYMOVE_HOMING_OUT, 0, 600, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 2100)
		{// 35�b
			SetEnemy(D3DXVECTOR3((426.0f) * 1.0f, (SCREEN_HEIGHT + 50.0f) * 1.0f, 0.0f), D3DXVECTOR3((426.0f) * 1.0f, (SCREEN_HEIGHT - 50.f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 300, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_10, 5.0f, 5 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((426.0f * 2) * 1.0f, (SCREEN_HEIGHT + 50.0f) * 1.0f, 0.0f), D3DXVECTOR3((426.0f * 2) * 1.0f, (SCREEN_HEIGHT - 50.f) * 1.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 3.0f, ENEMYMOVE_HOMING_OUT, 0, 300, 4, 20, 8, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_10, 5.0f, 5 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame == 2340)
		{// 39�b
			srand((unsigned int)time(0));

			int nSet = rand() % 3;
			float pos;

			if (nSet == 0) { pos = -25.0; }
			else if (nSet == 1) { pos = 410.0f; }
			else if (nSet == 2) { pos = 815.0f; }

			SetEnemy(D3DXVECTOR3(pos + 80.0f, -50.0f, 0.0f), D3DXVECTOR3(pos + 80.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 160.0f, -50.0f, 0.0f), D3DXVECTOR3(pos + 160.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 240.0f, -50.0f, 0.0f), D3DXVECTOR3(pos + 240.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 320.0f, -50.0f, 0.0f), D3DXVECTOR3(pos + 320.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 400.0f, -50.0f, 0.0f), D3DXVECTOR3(pos + 400.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3(pos + 80.0f, -100.0f, 0.0f), D3DXVECTOR3(pos + 80.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 160.0f, -100.0f, 0.0f), D3DXVECTOR3(pos + 160.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 240.0f, -100.0f, 0.0f), D3DXVECTOR3(pos + 240.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 320.0f, -100.0f, 0.0f), D3DXVECTOR3(pos + 320.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 400.0f, -100.0f, 0.0f), D3DXVECTOR3(pos + 400.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3(pos + 80.0f, -150.0f, 0.0f), D3DXVECTOR3(pos + 80.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 160.0f, -150.0f, 0.0f), D3DXVECTOR3(pos + 160.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 240.0f, -150.0f, 0.0f), D3DXVECTOR3(pos + 240.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 320.0f, -150.0f, 0.0f), D3DXVECTOR3(pos + 320.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(pos + 400.0f, -150.0f, 0.0f), D3DXVECTOR3(pos + 400.0f, (SCREEN_HEIGHT + 50.0f), 0.0f), D3DXVECTOR3(0.0f, 4.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });


			SetEnemy(D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-100.0f, 80.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-150.0f, 80.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-200.0f, 80.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-250.0f, 80.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 50.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 100.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 150.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 200.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH + 250.0f) * 1.0f, 80.0f, 0.0f), D3DXVECTOR3(-50.0f, 80.0f, 0.0f), D3DXVECTOR3(-4.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 10, ENEMYTYPE_NORMALENEMY,
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_NORMALENEMY && g_nCounterGame > 2340)
		{// ENEMYTYPE_NORMALENEMY�����Ȃ��Ȃ����ꍇ
			StopSound();							// �T�E���h���~�߂�
			PlaySound(SOUND_LABEL_BGM003);			// �{�X�T�E���h

			// �{�X���Z�b�g
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 0, 30, 100, ENEMYTYPE_SBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_6, 3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NO_7, 3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 15 },
				EnemyBullet{ ENEMYBULLET_NO_4, 6.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_SBOSSENEMY && g_nCounterGame > 2520)
		{// ENEMYTYPE_SBOSSENEMY�����Ȃ��Ȃ����ꍇ
		 // �{�X���Z�b�g
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 - SCREEN_WIDTH / 8) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 - SCREEN_WIDTH / 8) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 0, 50, 120, ENEMYTYPE_MBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_1, 3.0f, 18 },
				EnemyBullet{ ENEMYBULLET_NONE },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });

			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2 + SCREEN_WIDTH / 8) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 1, 50, 120, ENEMYTYPE_MBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_5, 3.0f, 23 },
				EnemyBullet{ ENEMYBULLET_NO_3, 3.0f, 23 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (nNumEnemy <= 0 && pEnemy->Type == ENEMYTYPE_MBOSSENEMY && g_nCounterGame > 2700)
		{// ENEMYTYPE_NORMALENEMY�����Ȃ��Ȃ����ꍇ
		 // �{�X���Z�b�g
			SetEnemy(D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, -100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 2.0f, ENEMYMOVE_HOMING, 20, 0, 4, 100, 250, ENEMYTYPE_LBOSSENEMY,
				EnemyBullet{ ENEMYBULLET_NO_8, 3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NO_9, 3.0f, 20 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}
		else if (g_nCounterGame > 2460 && pEnemy->Type == ENEMYTYPE_LBOSSENEMY && pEnemy->bUse == false)
		{// �{�X�����Ȃ��Ȃ����ꍇ
			g_nCntSetStage++;

			if (g_nCntSetStage > 120)
			{
				g_GameState = GAMESTATE_GAMECLEAR;
			}
		}
	}

	// �A�C�e��
	if (pEnemy->Type == ENEMYTYPE_SBOSSENEMY && pEnemy->bUse == true)
	{
		if (rand() % 3 == 0)
		{
			if (rand() % 1000 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_POWER); }
			if (rand() % 1000 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_LIFE); }
			if (rand() % 1000 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_STAR); }
		}
	}
	else if (pEnemy->Type == ENEMYTYPE_MBOSSENEMY && pEnemy->bUse == true)
	{
		if (rand() % 1200 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_POWER); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_POWER); } }
		if (rand() % 1200 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_LIFE); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_LIFE); } }
		if (rand() % 1200 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_STAR); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_STAR); } }
	}
	else if (pEnemy->Type == ENEMYTYPE_LBOSSENEMY && pEnemy->bUse == true)
	{
		if (rand() % 1500 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_POWER); SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_LIFE); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_LIFE); } }
		if (rand() % 1500 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_LIFE); SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_LIFE); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_LIFE); } }
		if (rand() % 1500 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, -10.0f, 0.0f), ITEMTYPE_STAR); SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_STAR); if (rand() % 3 == 0) { SetItem(D3DXVECTOR3((rand() % 1200 + 40) * 1.0f, 0.0f, 0.0f), ITEMTYPE_STAR); } }
	}
}