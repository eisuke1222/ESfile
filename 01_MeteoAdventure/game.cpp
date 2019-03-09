//=========================================================================================================================
//
// �Q�[������ [game.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "game.h"		// �Q�[��
#include "input.h"		// ���͏���
#include "map.h"		// �}�b�v
#include "player.h"		// �v���C���[
#include "enemy.h"		// �G
#include "bullet.h"		// �e
#include "explosion.h"	// ����
#include "block.h"		// �u���b�N
#include "item.h"		// �A�C�e��
#include "score.h"		// �X�R�A
#include "pause.h"		// �|�[�Y
#include "fade.h"		// �t�F�[�h
#include "bgwindow.h"	// �w�i
#include "sound.h"		// �T�E���h

//*************************************************************************************************************************
//	�\���̒�`
//*************************************************************************************************************************
#define MAX_STAGE	(3)

//*************************************************************************************************************************
//	�\���̒�`
//*************************************************************************************************************************
void SetStage(void);	// �X�e�[�W�ݒ�

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
GAMESTATE		g_GameState;							// �Q�[�����
GAMESTAGE		g_GameStageOpen;						// �J�����Ă���Q�[���X�e�[�W�ԍ�
int				g_nCounterGameState;					// �Q�[����ԃJ�E���^
bool			g_bPause;								// �|�[�Y
int				g_nCounterGame;							// �Q�[���J�E���^
int				g_nCntSetStage;							// �X�e�[�W�Z�b�g�J�E���^
bool			g_aStageClear[MAX_STAGE];				// �X�e�[�W�N���A���

//=========================================================================================================================
// �Q�[������������
//=========================================================================================================================
void InitGame(void)
{
	// ��Ԑݒ�
	g_GameState = GAMESTATE_MAP;		// �Q�[�����
	g_GameStageOpen = GAMESTAGE_NO1;	// �Q�[���X�e�[�W
	g_nCounterGameState = 0;			// �J�E���^������
	g_bPause = false;					// �|�[�Y�����ݒ�
	g_nCounterGame = 0;					// �J�E���^������
	g_nCntSetStage = 0;					// �J�E���^������

	// �X�e�[�W�N���A���
	for (int nCount = 0; nCount < MAX_STAGE; nCount++)
	{
		g_aStageClear[nCount] = false;
	}

	// �}�b�v����������
	InitMap();

	// �X�R�A����������
	InitScore();

	// �u���b�N����������
	InitBlock();

	// �G����������
	InitEnemy();

	// �v���C���[����������
	InitPlayer();

	// �e����������
	InitBullet();

	// ��������������
	InitExplosion();

	// �A�C�e������������
	InitItem();

	// �|�[�Y����������
	InitPause();
}

//=========================================================================================================================
// �Q�[���I������
//=========================================================================================================================
void UninitGame(void)
{
	// �}�b�v�I������
	UninitMap();

	// �X�R�A�I������
	UninitScore();

	// �u���b�N�I������
	UninitBlock();

	// �G�I������
	UninitEnemy();

	// �v���C���[�I������
	UninitPlayer();

	// �e�I������
	UninitBullet();

	// �����I������
	UninitExplosion();

	// �A�C�e���I������
	UninitItem();

	// �|�[�Y�I������
	UninitPause();
}

//=========================================================================================================================
// �Q�[���X�V����
//=========================================================================================================================
void UpdateGame(void)
{
	bool bHelp = GetHelpState();

	if (GetKeyboardTrigger(DIK_P) == true && (g_GameState == GAMESTATE_MAP || g_GameState == GAMESTATE_GAMEPLAY) && bHelp == false)
	{// �|�[�Y��ؑ�
		g_bPause = g_bPause ? false : true;

		if (g_bPause == true)
		{
			PlaySound(SOUND_LABEL_SE_PAUSEIN);
		}
		else
		{
			PlaySound(SOUND_LABEL_SE_PAUSEOUT);
		}
	}

	// �X�e�[�W�����ݒ�
	SetStage();

	switch (g_GameState)
	{
	case GAMESTATE_MAP:			// �}�b�v�\��

		if (g_bPause == false)
		{// �|�[�Y���g�p����Ă��Ȃ�
			// �}�b�v�X�V����
			UpdateMap();

			// �X�R�A�X�V����
			UpdateScore();

			// �v���C���[�X�V����
			UpdatePlayer();

			// �|�[�Y��Ԑݒ�
			SetPauseState();
		}

		break;
	case GAMESTATE_GAMEPLAY:	// �Q�[���\��
	case GAMESTATE_GAMECLEAR:	// �Q�[���N���A
	case GAMESTATE_GAMEOVER:	// �Q�[���I�[�o�[

		if (g_bPause == false)
		{// �|�[�Y���g�p����Ă��Ȃ�
			// �X�R�A�X�V����
			UpdateScore();

			// �u���b�N�X�V����
			UpdateBlock();

			// �G�X�V����
			UpdateEnemy();

			// �v���C���[�X�V����
			UpdatePlayer();

			// �e�X�V����
			UpdateBullet();

			// �����X�V����
			UpdateExplosion();

			// �A�C�e���X�V����
			UpdateItem();

			// �|�[�Y��Ԑݒ�
			SetPauseState();
		}

		if (g_GameState == GAMESTATE_GAMECLEAR || g_GameState == GAMESTATE_GAMEOVER)
		{// �Q�[���N���A�A�I�[�o�[�̏ꍇ
			g_nCounterGameState++;	// �J�E���^�[�����Z

			if (g_nCounterGameState >= 60)
			{// ���J�E���g��
				SetFadeMode(MODE_RESULT);		// ���U���g��
			}
		}

		break;
	}

	if (g_bPause == true)
	{// �|�[�Y���g�p����Ă���ꍇ
		// �|�[�Y�̍X�V����
		UpdatePause();
	}
}

//=========================================================================================================================
// �Q�[���`�揈��
//=========================================================================================================================
void DrawGame(void)
{
	switch (g_GameState)
	{
	case GAMESTATE_MAP:			// �}�b�v�\��

		// �}�b�v�`�揈��
		DrawMap();

		// �X�R�A�`�揈��
		DrawScore();

		// �v���C���[�`�揈��
		DrawPlayer();

		break;
	case GAMESTATE_GAMEPLAY:	// �Q�[���\��
	case GAMESTATE_GAMECLEAR:	// �Q�[���N���A
	case GAMESTATE_GAMEOVER:	// �Q�[���I�[�o�[

		// �X�R�A�`�揈��
		DrawScore();

		// �u���b�N�`�揈��
		DrawBlock();

		// �G�`�揈��
		DrawEnemy();

		// �v���C���[�`�揈��
		DrawPlayer();

		// �e�`�揈��
		DrawBullet();

		// �����`�揈��
		DrawExplosion();

		// �A�C�e���`�揈��
		DrawItem();

		break;
	}

	if (g_bPause == true && (g_GameState == GAMESTATE_MAP || g_GameState == GAMESTATE_GAMEPLAY))
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
	if (state != GAMESTATE_GAMECLEAR && state != GAMESTATE_GAMEOVER)
	{// �j������
		DeleteBGWindow();			// �w�i
		DeleteMap();				// �}�b�v
		DeleteScore();				// �X�R�A
		DeleteBlock();				// �u���b�N
		DeleteEnemy();				// �G
		DeletePlayer();				// �v���C���[
		DeleteBullet();				// �e
		DeleteItem();				// �A�C�e��
	}

	GameSoundStop();

	g_bPause = false;				// �|�[�Y�����
	g_GameState = state;			// ��Ԃ��L��
	g_nCntSetStage = 0;
}

//=========================================================================================================================
// �Q�[���擾����
//=========================================================================================================================
GAMESTATE GetGameState(void)
{
	return g_GameState;				// �l��Ԃ�
}

//=========================================================================================================================
// �Q�[���X�e�[�W�ݒ�
//=========================================================================================================================
void SetNumStage(int stage)
{
	if (GAMESTAGE_NO1 == stage)
	{
		g_GameStageOpen = GAMESTAGE_NO1;
	}
	else if (GAMESTAGE_NO2 == stage)
	{
		g_GameStageOpen = GAMESTAGE_NO2;
	}
	else if (GAMESTAGE_NO3 == stage)
	{
		g_GameStageOpen = GAMESTAGE_NO3;
	}
	else if (GAMESTAGE_NO4 == stage)
	{
		g_GameStageOpen = GAMESTAGE_NO4;
	}
	else if (GAMESTAGE_NO5 == stage)
	{
		g_GameStageOpen = GAMESTAGE_NO5;
	}
}

//=========================================================================================================================
// �Q�[���X�e�[�W�擾
//=========================================================================================================================
GAMESTAGE GetNumStage(void)
{
	return g_GameStageOpen;
}

//=========================================================================================================================
// �Q�[���X�e�[�W�ݒ�
//=========================================================================================================================
void SetStageClearState(void)
{
	g_aStageClear[g_GameStageOpen] = true;

	int nCntClearStage = 0;

	for (int nCount = 0; nCount < MAX_STAGE; nCount++)
	{
		if (g_aStageClear[nCount] == true)
		{
			nCntClearStage++;
		}
	}

	if (nCntClearStage == MAX_STAGE)
	{
		SetGameState(GAMESTATE_GAMECLEAR);
	}
	else
	{
		SetFadeGame(GAMESTATE_MAP);
	}
}

//=========================================================================================================================
// �Q�[���X�e�[�W�N���A�擾
//=========================================================================================================================
bool *GetStageClearState(void)
{
	return &g_aStageClear[0];
}

//=========================================================================================================================
// �|�[�Y�ݒ菈��
//=========================================================================================================================
void SetPauseState(bool bPause)
{
	g_bPause = bPause;				// ��Ԃ��L��
}

//=========================================================================================================================
// �|�[�Y�擾����
//=========================================================================================================================
bool GetPauseState(void)
{
	return g_bPause;				// �l��Ԃ�
}

//=========================================================================================================================
// �Q�[���X�e�[�W�ݒ菈��
//=========================================================================================================================
void GameSoundStop(void)
{
	// �T�E���h�I��
	if (g_GameState == GAMESTATE_MAP)
	{// �Q�[���}�b�v
		StopSound(SOUND_LABEL_BGM_GAME_MAP);
	}
	else if (g_GameState == GAMESTATE_GAMEPLAY)
	{// �Q�[���v���C
		if (g_GameStageOpen == GAMESTAGE_NO1)
		{// �X�e�[�W�P
			StopSound(SOUND_LABEL_BGM_GAME_STAGE1);
		}
		else if (g_GameStageOpen == GAMESTAGE_NO2)
		{// �X�e�[�W�Q
			StopSound(SOUND_LABEL_BGM_GAME_STAGE2);
		}
		else if (g_GameStageOpen == GAMESTAGE_NO3)
		{// �X�e�[�W�R
			StopSound(SOUND_LABEL_BGM_GAME_STAGE3);
		}
	}

}

//===========================================================================================
// �Q�[���X�e�[�W�ݒ菈��
//===========================================================================================
void SetStage(void)
{
	if (g_nCntSetStage == 0)
	{// ���̂�
		switch (g_GameState)
		{// �Q�[�����
		case GAMESTATE_MAP:		// �Q�[���}�b�v

			// �w�i�ݒ�
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_MAP);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 - 250, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 30, BGWINDOWTEXTURE_GAME_NUMPLAYER);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 - 100, 33, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 38, BGWINDOWTEXTURE_GAME_NUMSTAGE);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 300, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 120, 30, BGWINDOWTEXTURE_GAME_HISCORETXT);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400, SCREEN_HEIGHT - 80, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300, 100, BGWINDOWTEXTURE_GAME_PKEY);

			// �}�b�v�ݒ�
			SetMap(D3DXVECTOR3(SCREEN_WIDTH / 2 - 230, SCREEN_HEIGHT - 140, 0.0f));
			SetMap(D3DXVECTOR3(SCREEN_WIDTH / 2 - 270, SCREEN_HEIGHT / 2 - 120, 0.0f));
			SetMap(D3DXVECTOR3(SCREEN_WIDTH / 2 + 330, SCREEN_HEIGHT / 2 + 110, 0.0f));

			// �X�R�A�ݒ�
			SetScore(D3DXVECTOR3(SCREEN_WIDTH / 4 - 180, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_PLAYER);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH - 30, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_HISCORE);

			// �T�E���h�ݒ�
			PlaySound(SOUND_LABEL_BGM_GAME_MAP);

			break;

		case GAMESTATE_GAMEPLAY:// �Q�[���v���C

			switch (g_GameStageOpen)
			{// �Q�[���X�e�[�W
			case GAMESTAGE_NO1:	// �X�e�[�W�P
			{
				//�w�i�ݒ�
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_STAGE);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80, BGWINDOWTEXTURE_GAME_HELP);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 240, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 35, BGWINDOWTEXTURE_GAME_HELPWALK);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80, BGWINDOWTEXTURE_GAME_HELP);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 240, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 35, BGWINDOWTEXTURE_GAME_HELPRUN);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80, BGWINDOWTEXTURE_GAME_HELP);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 240, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 35, BGWINDOWTEXTURE_GAME_HELPJUMP);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 2 + 800, SCREEN_HEIGHT - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80, BGWINDOWTEXTURE_GAME_HELP);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 2 + 800, SCREEN_HEIGHT - 240, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 35, BGWINDOWTEXTURE_GAME_HELPATTACK);
				//SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 -100, SCREEN_HEIGHT - 220, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 80, BGWINDOWTEXTURE_GAME_HELP);
				//SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 -100, SCREEN_HEIGHT - 240, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 60, 35, BGWINDOWTEXTURE_GAME_HELPATTACK);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 380, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 75, 250, BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);

				// �u���b�N�ݒ�
				{
					// �K�i
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2, SCREEN_HEIGHT - 280, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 50, SCREEN_HEIGHT - 330, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 100, SCREEN_HEIGHT - 380, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 150, SCREEN_HEIGHT - 430, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 200, SCREEN_HEIGHT - 480, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 530, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					// ����
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 480, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 430, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 380, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 330, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 280, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					// �E��
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 630, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 580, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 530, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 480, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 430, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 380, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 330, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 350, SCREEN_HEIGHT - 280, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					// �͂�
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 50, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 50, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 100, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 50, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 100, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 100, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 150, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);

					// �n�� + ����
					SetBlock(D3DXVECTOR3(-SCREEN_WIDTH + 100, 0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_WALL);
					SetBlock(D3DXVECTOR3(-400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
				}

				// �A�C�e���ݒ�
				{
					// ����
					SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2 + 250, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2 + 290, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2 + 330, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2 + 370, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH / 2 + 410, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					// ����
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 250, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 290, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 330, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 370, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 410, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					// �W�����v
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 250 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 290 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 230, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 330 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 280, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 370 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 280, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 410 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 230, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 450 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 180, 0.0f), ITEMTYPE_COIN_NORMAL);
					// �K�i
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 25, SCREEN_HEIGHT - 280, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 125, SCREEN_HEIGHT - 380, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 480, 0.0f), ITEMTYPE_COIN_NORMAL);
					// ����
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 325, SCREEN_HEIGHT - 580, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 325, SCREEN_HEIGHT - 530, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 325, SCREEN_HEIGHT - 480, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 325, SCREEN_HEIGHT - 430, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 325, SCREEN_HEIGHT - 380, 0.0f), ITEMTYPE_COIN_NORMAL);
					// �͂�
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 + 75, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
				}

				// �G�ݒ�
				SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 3 - 80, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);

				// �T�E���h�ݒ�
				PlaySound(SOUND_LABEL_BGM_GAME_STAGE1);
			}
			break;
			case GAMESTAGE_NO2:	// �X�e�[�W�Q
			{
				//�w�i�ݒ�
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),				SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_STAGE);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 130, 0.0f), D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f),		SCREEN_WIDTH / 2, 40, BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 150, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f),	SCREEN_WIDTH / 2 + 50, 40, BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 170, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f),	SCREEN_WIDTH / 2 + 100, 40, BGWINDOWTEXTURE_GAME_STAGE_TREE_UNDER);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 30, 0.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f),			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),	SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 4 - 30, BGWINDOWTEXTURE_GAME_STAGE_TREE_ALL);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + 900, SCREEN_HEIGHT - 380, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f),		75, 250, BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 4 - 180, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),		80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 75, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),				SCREEN_WIDTH / 2, 75, BGWINDOWTEXTURE_GAME_STAGE_IWA);

				// �u���b�N�ݒ�
				{

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 100, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 500, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 550, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 600, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 750, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 750, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 750, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 750, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 750, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 900, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 950, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 1000, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), 50, 50, BLOCKTYPE_MOVE_TRIP, 300, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 100, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 150, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 200, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 300, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 100, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_BREAK, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 100, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_BREAK, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 300, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_BREAK, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 300, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_BREAK, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 500, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 300, BLOCKTYPE_NORMAL_WARP_Y, 0, BLOCKTEXTURE_DOKAN);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 440, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 560, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 610, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 660, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 710, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 760, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 810, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 860, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 910, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 960, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);

				//	SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 1010, SCREEN_HEIGHT - 600, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1000, 50, BLOCKTYPE_NORMAL_WARP_X, 0, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 80, SCREEN_HEIGHT - 150, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, 50, BLOCKTYPE_NORMAL_APPEAR, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 120, SCREEN_HEIGHT - 150, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, 50, BLOCKTYPE_NORMAL_APPEAR, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 320, SCREEN_HEIGHT - 150, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, 50, BLOCKTYPE_NORMAL_APPEAR, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 520, SCREEN_HEIGHT - 150, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100, 50, BLOCKTYPE_NORMAL_APPEAR, 0, BLOCKTEXTURE_NORMAL);

					// �n�� + ����
					SetBlock(D3DXVECTOR3(-SCREEN_WIDTH + 100, 0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_WALL);
					SetBlock(D3DXVECTOR3(-400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH - 430, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 - 180, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 720, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH + SCREEN_WIDTH / 2, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
				}

				// �A�C�e���ݒ�
				{
					SetItem(D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 50, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH - 50, SCREEN_HEIGHT - 280, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 50, SCREEN_HEIGHT - 280, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 575, SCREEN_HEIGHT - 400, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 575, SCREEN_HEIGHT - 450, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 575, SCREEN_HEIGHT - 500, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 775, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 775, SCREEN_HEIGHT - 350, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 775, SCREEN_HEIGHT - 450, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 975, SCREEN_HEIGHT - 400, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 975, SCREEN_HEIGHT - 450, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 975, SCREEN_HEIGHT - 500, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 175, SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 175, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 275, SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 275, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 175, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 350, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 225, SCREEN_HEIGHT - 550, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 275, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 785, SCREEN_HEIGHT - 600, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 985, SCREEN_HEIGHT - 600, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 - 30, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 + 170, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 + 370, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 + 570, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
				}

				// �G�ݒ�
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 300, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 4, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_BIRD);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 575, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_CURSE);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 975, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_CURSE);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 2 + 200, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 2 + 250, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);
				}

				// �T�E���h�ݒ�
				PlaySound(SOUND_LABEL_BGM_GAME_STAGE2);
			}
			break;
			case GAMESTAGE_NO3:	// �X�e�[�W�R
			{
				//�w�i�ݒ�
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_STAGE);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 30, 0.0f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, BGWINDOWTEXTURE_GAME_STAGE_SABAKU_YAMA);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT - 380, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 75, 250, BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);

				// �u���b�N�ݒ�
				{
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 50, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 100, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 150, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 200, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 250, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 310, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 250, BLOCKTYPE_NORMAL_WARP_Y, 180, BLOCKTEXTURE_DOKAN);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 370, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 420, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 470, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 520, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 570, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 620, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_FALL, 180, BLOCKTEXTURE_NORMAL);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 300, BLOCKTYPE_NORMAL_WARP_Y, 0, BLOCKTEXTURE_DOKAN);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 60, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 120, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 300, BLOCKTYPE_NORMAL_WARP_Y, 0, BLOCKTEXTURE_DOKAN);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 + 180, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 800, 50, BLOCKTYPE_NORMAL_SUNA, 180, BLOCKTEXTURE_SUNA);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 290, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 300, BLOCKTYPE_NORMAL_WARP_Y, 0, BLOCKTEXTURE_DOKAN);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 500, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 450, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 350, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 230, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 170, SCREEN_HEIGHT - 550, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 300, BLOCKTYPE_NORMAL_WARP_Y, 0, BLOCKTEXTURE_DOKAN);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 240, SCREEN_HEIGHT - 155, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300, 150, BLOCKTYPE_NORMAL_SUNA_RE, 180, BLOCKTEXTURE_SUNA);


					// �n�� + ����
					SetBlock(D3DXVECTOR3(-SCREEN_WIDTH + 100, 0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_WALL);
					SetBlock(D3DXVECTOR3(-400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_SABAKU);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_SABAKU);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_SABAKU);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH / 2, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_SABAKU);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 + 530, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH * 2, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_SABAKU);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);

					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 800, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 800, SCREEN_HEIGHT - 250, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH + 800, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, BLOCKTYPE_NORMAL_DAMAGE, 180, BLOCKTEXTURE_TOGE);
				}

				// �A�C�e���ݒ�
				{
					SetItem(D3DXVECTOR3(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH - 110, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 75, SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 125, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 175, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 225, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 445, SCREEN_HEIGHT - 300, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 495, SCREEN_HEIGHT - 250, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 545, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 595, SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 275, SCREEN_HEIGHT - 500, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 335, SCREEN_HEIGHT - 550, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 395, SCREEN_HEIGHT - 500, 0.0f), ITEMTYPE_COIN_NORMAL);

					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 780, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					SetItem(D3DXVECTOR3(SCREEN_WIDTH + 870, SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);

					for (int nCount = 0; nCount < 8; nCount++)
					{
						SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 235 + (nCount * 100.0f), SCREEN_HEIGHT - 150, 0.0f), ITEMTYPE_COIN_NORMAL);
					}
					for (int nCount = 0; nCount < 7; nCount++)
					{
						SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 285 + (nCount * 100.0f), SCREEN_HEIGHT - 200, 0.0f), ITEMTYPE_COIN_NORMAL);
					}

					for (int nCount = 0; nCount < 5; nCount++)
					{
						SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 235, SCREEN_HEIGHT - 300 - (nCount * 50.0f), 0.0f), ITEMTYPE_COIN_NORMAL);
						SetItem(D3DXVECTOR3(SCREEN_WIDTH * 2 + 935, SCREEN_HEIGHT - 300 - (nCount * 50.0f), 0.0f), ITEMTYPE_COIN_NORMAL);
					}

					for (int nCount = 0; nCount < 5; nCount++)
					{
						SetItem(D3DXVECTOR3(SCREEN_WIDTH * 3 + 290 + (nCount * 50.0f), SCREEN_HEIGHT - 100, 0.0f), ITEMTYPE_COIN_NORMAL);
					}
				}

				// �G�ݒ�
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 500, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 155, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_CURSE);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 250, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 5, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_GHOST);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 430, SCREEN_HEIGHT - 200, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 5, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_GHOST);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH + 830, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 3, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_CURSE);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 2 + 500, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 5, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_GHOST);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 2 + 580, SCREEN_HEIGHT - 400, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 5, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_GHOST);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 2 + 660, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 5, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_GHOST);

					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 3, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 4, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_BIRD);
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH * 3 + 200, SCREEN_HEIGHT - 300, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 4, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_BIRD);
				}

				// �T�E���h�ݒ�
				PlaySound(SOUND_LABEL_BGM_GAME_STAGE3);
			}
			break;
			case GAMESTAGE_NO4: // �X�e�[�W�S
			{
				//�w�i�ݒ�
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_STAGE);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 380, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 75, 250, BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);

				// �u���b�N�ݒ�
				{
					// �n�� + ����
					SetBlock(D3DXVECTOR3(-SCREEN_WIDTH + 100, 0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_WALL);
					SetBlock(D3DXVECTOR3(-400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
				}
			}
			break;
			case GAMESTAGE_NO5:	// �X�e�[�W�T
			{
				//�w�i�ݒ�
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_GAME_STAGE);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 380, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 75, 250, BGWINDOWTEXTURE_GAME_STAGEGOALLIMIT);
				SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH * 3 + SCREEN_WIDTH / 2, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);

				// �u���b�N�ݒ�
				{
					// �n�� + ����
					SetBlock(D3DXVECTOR3(-SCREEN_WIDTH + 100, 0, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_WALL);
					SetBlock(D3DXVECTOR3(-400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 2 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 3 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
					SetBlock(D3DXVECTOR3(SCREEN_WIDTH * 4 - 400, SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, 160.0f, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_GROUND);
				}
			}
			break;
			}

			// �v���C���[�ݒ�
			SetPlayer(D3DXVECTOR3(SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT - 160, 0.0f));

			// �X�e�[�^�X
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 - 250, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 30, BGWINDOWTEXTURE_GAME_NUMPLAYER);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 + 180, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 20, BGWINDOWTEXTURE_GAME_LIFE);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 + 130, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 20, BGWINDOWTEXTURE_GAME_LIFE);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 + 80, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 20, BGWINDOWTEXTURE_GAME_LIFE);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 * 3 - 170, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 25, 25, BGWINDOWTEXTURE_GAME_COIN);
			SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 4 * 3 - 65, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 90, 40, BGWINDOWTEXTURE_GAME_COINTXT);

			// �X�R�A�ݒ�
			SetScore(D3DXVECTOR3(SCREEN_WIDTH / 4 - 180, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_PLAYER);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_TIME);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH / 4 * 3 - 50, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_COIN);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH / 4 * 3 - 50, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_COIN);
			SetScore(D3DXVECTOR3(SCREEN_WIDTH - 30, 30, 0.0f), D3DXVECTOR3(30, 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_SCORE);

			break;
		}
	}
	g_nCntSetStage = 1;
}