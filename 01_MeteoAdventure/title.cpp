//=========================================================================================================================
// 
// �^�C�g������ [title.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "title.h"		// �^�C�g��
#include "input.h"		// ���͏���
#include "player.h"		// �v���C���[
#include "enemy.h"		// �G
#include "block.h"		// �u���b�N
#include "bgwindow.h"	// �w�i
#include "fade.h"		// �t�F�[�h
#include "sound.h"		// �T�E���h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************

//=========================================================================================================================
// �^�C�g������������
//=========================================================================================================================
void InitTitle(void)
{
	// �w�i�ݒ�
	SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, BGWINDOWTEXTURE_TITLE_BG);
	SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 350, 250, BGWINDOWTEXTURE_TITLE_LOGO);
	SetBGWindow(D3DXVECTOR3(100, SCREEN_HEIGHT, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 200, 200, BGWINDOWTEXTURE_TITLE_GOUND);
	SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH - 45, SCREEN_HEIGHT - 295, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 80, 150, BGWINDOWTEXTURE_GAME_STAGEGOAL);
	SetBGWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 250, 0.0f), D3DXCOLOR(0.1f, 0.8f, 1.0f, 1.0f), 200, 50, BGWINDOWTEXTURE_SELECT);

	// �u���b�N����������
	InitBlock();
	for (int nCount = 0; nCount < 6; nCount++)
	{// 1�i��
		SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 240 + (nCount * 40.0f), SCREEN_HEIGHT - 40, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40, 40, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
	}
	for (int nCount = 0; nCount < 5; nCount++)
	{// 2�i��
		SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 200 + (nCount * 40.0f), SCREEN_HEIGHT - 80, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40, 40, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
	}
	for (int nCount = 0; nCount < 4; nCount++)
	{// 3�i��
		SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 160 + (nCount * 40.0f), SCREEN_HEIGHT - 120, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40, 40, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
	}
	for (int nCount = 0; nCount < 3; nCount++)
	{// 4�i��
		SetBlock(D3DXVECTOR3(SCREEN_WIDTH - 120 + (nCount * 40.0f), SCREEN_HEIGHT - 160, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 40, 40, BLOCKTYPE_NORMAL, 0, BLOCKTEXTURE_NORMAL);
	}

	// �G����������
	InitEnemy();
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 220, SCREEN_HEIGHT - 45, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 10, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SLIME);
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 85, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 10, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_SNOWMAN);
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 140, SCREEN_HEIGHT - 125, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 10, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_CURSE);
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 100, SCREEN_HEIGHT - 165, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 100, 10, ENEMYTYPE_NORMALENEMY, ENEMYTEXTURE_BIRD);

	// �v���C���[����������
	InitPlayer();
	SetPlayer(D3DXVECTOR3(100, SCREEN_HEIGHT - 180, 0.0f));
}

//=========================================================================================================================
// �^�C�g���I������
//=========================================================================================================================
void UninitTitle(void)
{
	// �u���b�N�I������
	UninitBlock();

	// �G�I������
	UninitEnemy();

	// �v���C���[�I������
	UninitPlayer();
}

//=========================================================================================================================
// �^�C�g���X�V����
//=========================================================================================================================
void UpdateTitle(void)
{
	// �u���b�N�X�V����
	UpdateBlock();

	// �G�X�V����
	UpdateEnemy();

	// �v���C���[�X�V����
	UpdatePlayer();

	FADE fade = GetFade();	// �t�F�[�h���
	if (fade == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true)
		{// �C�ӂ̃L�[�iENTER�j���������ꍇ
			PlaySound(SOUND_LABEL_SE_DECIDE);
			SetSelectState(SELECTSTATE_ENTER);			// �I����ԕύX
			SetFadeMode(MODE_GAME);		// �Q�[����
		}
	}
}

//=========================================================================================================================
// �^�C�g���`�揈��
//=========================================================================================================================
void DrawTitle(void)
{
	// �u���b�N�`�揈��
	DrawBlock();

	// �G�`�揈��
	DrawEnemy();

	// �v���C���[�`�揈��
	DrawPlayer();
}