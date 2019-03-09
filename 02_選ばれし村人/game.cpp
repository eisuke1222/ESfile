//=========================================================================================================================
//
// �Q�[������ [game.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include <time.h>				// time
#include "game.h"				// �Q�[��
#include "camera.h"				// �J����
#include "light.h"				// ���C�g
#include "meshfield.h"			// ���b�V���t�B�[���h
#include "meshcylinder.h"		// ���b�V���V�����_�[
#include "meshwall.h"			// ���b�V���E�H�[��
#include "billboardobject.h"	// �r���{�[�h�I�u�W�F�N�g
#include "billboardeffect.h"	// �r���{�[�h�G�t�F�N�g
#include "shadow.h"				// �e
#include "xobject.h"			// �I�u�W�F�N�g
#include "xenemy.h"				// �G
#include "xplayer.h"			// �v���C���[
#include "xweapon.h"			// ����
#include "score.h"				// �X�R�A
#include "pause.h"				// �|�[�Y
#include "sound.h"				// �T�E���h
#include "fade.h"				// �t�F�[�h
#include "input.h"				// ����
#include "title.h"				// �^�C�g��
#include "font.h"				// �t�H���g
#include "window.h"				// �E�B���h�E

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
bool			g_bPause;			// �|�[�Y
int				g_nCntGameTimer;	// �Q�[���^�C�}�[
GAMESTATE		g_GameState;		// �Q�[���X�e�[�g
bool			g_bTutorialStep;	// �`���[�g���A���X�L�b�v
int				g_PlayerRespawnNum;	// ���X�|�[���n�_
int				g_nCntMap;			// �}�b�v
int				g_nCntTutorialSkip;	// �`���[�g���A���X�L�b�v
bool			g_GameTutorial;		// �`���[�g���A��

WEAPONTYPE		g_XPlayerUseWeapon;	// �v���C���[����
int				g_XPlayerHP;		// �v���C���[�̗�
int				g_Weapon_1Lv, g_Weapon_2Lv, g_Weapon_3Lv;	// ����
int				g_LeftItem, g_CenterItem, g_RightItem;		// �A�C�e��

//=========================================================================================================================
// �v���g�^�C�v�錾
//=========================================================================================================================
void SetStage(void);

//=========================================================================================================================
// �Q�[���̏���������
//=========================================================================================================================
void InitGame(void)
{
	// ������
	g_bPause = false;
	g_nCntGameTimer = 0;
	g_GameState = GAMESTATE_NONE;
	g_bTutorialStep = false;
	g_PlayerRespawnNum = 0;
	g_nCntMap = 0;
	g_nCntTutorialSkip = 0;
	g_GameTutorial = false;

	// �����ݒ�
	g_XPlayerUseWeapon = WEAPONTYPE_NONE;
	g_XPlayerHP = 0;
	g_Weapon_1Lv = 0;
	g_Weapon_2Lv = 0;
	g_Weapon_3Lv = 0;
	g_LeftItem = 0;
	g_CenterItem = 0;
	g_RightItem = 0;

	StopSound(SOUND_LABEL_BGM_TITLE);
	SetTitleSound(false);

	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	// �t�B�[���h�̏���������
	InitMeshField();

	// �ǂ̏���������
	InitMeshWall();

	// �V�����_�[�̏���������
	InitMeshCylinder();

	// �r���{�[�h�I�u�W�F�N�g�̏���������
	InitBillboardObject();
	
	// �r���{�[�h�G�t�F�N�g�̏���������
	InitBillboardEffect();

	// �e�̏���������
	InitShadow();

	// �I�u�W�F�N�g�̏���������
	InitXObject();

	// �G�̏���������
	InitXEnemy();

	// �v���C���[�̏���������
	InitXPlayer();

	// ����̏���������
	InitXWeapon();

	// �X�R�A�̏���������
	InitScore();

	// �|�[�Y����������
	InitPause();

	// �Q�[����ԕύX
	SetGameState(GAMESTATE_TUTORIAL);
	//SetGameState(GAMESTATE_PLAY);
}

//=========================================================================================================================
// �Q�[���̏I������
//=========================================================================================================================
void UninitGame(void)
{
	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	// �t�B�[���h�̏I������
	UninitMeshField();

	// �ǂ̏I������
	UninitMeshWall();

	// �V�����_�[�̏I������
	UninitMeshCylinder();

	// �r���{�[�h�I�u�W�F�N�g�̏I������
	UninitBillboardObject();

	// �r���{�[�h�G�t�F�N�g�̏I������
	UninitBillboardEffect();

	// �e�̏I������
	UninitShadow();

	// �I�u�W�F�N�g�̏I������
	UninitXObject();

	// �G�̏I������
	UninitXEnemy();

	// �v���C���[�̏I������
	UninitXPlayer();

	// ����̏I������
	UninitXWeapon();

	// �|�[�Y�I������
	UninitPause();

	// �E�B���h�E�j������
	DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_GAME);
	DeleteWindow(WINDOWTEXTURE_SELECT_2);
	DeleteWindow(WINDOWTEXTURE_MESSAGE);
	DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_IN);
	DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_OUT);
	DeleteWindow(WINDOWTEXTURE_GAME_MAP);
	DeleteWindow(WINDOWTEXTURE_GAME_MAP_POS);
	DeleteWindow(WINDOWTEXTURE_GAME_HPBG);
	DeleteWindow(WINDOWTEXTURE_GAME_HP_M);
	DeleteWindow(WINDOWTEXTURE_GAME_HP);
	DeleteWindow(WINDOWTEXTURE_GAME_WEAPON_1);
	DeleteWindow(WINDOWTEXTURE_GAME_WEAPON_2);
	DeleteWindow(WINDOWTEXTURE_GAME_WEAPON_3);
	DeleteWindow(WINDOWTEXTURE_GAME_WEAPON_99);

	// �t�H���g�j��
	DeleteFont();

	StopSound();
}

//=========================================================================================================================
// �Q�[���̍X�V����
//=========================================================================================================================
void UpdateGame(void)
{
	FADE fade = GetFade();

	if (fade == FADE_NONE)
	{
		if ((GetKeyboardTrigger(DIK_P) == true || GetButtonTrigger(9) == true) && (g_GameState == GAMESTATE_TUTORIAL || g_GameState == GAMESTATE_PLAY))
		{// �|�[�Y
			PlaySound(SOUND_LABEL_SE_PAUSE_0);

			if (g_bPause == false)
			{
				SetStickState(true);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 50, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_GAME_TUTORIAL_IN, SETTEXTURE_TOPFRONT);
			}
			else
			{
				DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_P);
				DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_IN);
				DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_OUT);
			}

			g_bPause = g_bPause ? false : true;
		}
	}

	if (g_bPause == false)
	{// �|�[�Y�����Ă��Ȃ�
		switch (g_GameState)
		{// �Q�[�����
		case GAMESTATE_TUTORIAL:

			if ((GetKeyboardPress(DIK_RETURN) == true || GetKeyboardPress(DIK_Z) == true ||
				GetButtonPress(2) == true))
			{// �`���[�g���A���X�L�b�v
				g_nCntTutorialSkip++;

				if (g_nCntTutorialSkip > 150)
				{// �Q�[���{�҂�
					SetFadeGame(GAMESTATE_PLAY);
				}
			}
			else
			{
				g_nCntTutorialSkip = 0;
			}

			if ((GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
				GetButtonTrigger(2) == true) && g_bTutorialStep == true)
			{// �Q�[���{�҂�
				SetFadeGame(GAMESTATE_PLAY);
			}

		case GAMESTATE_PLAY:

			if (g_nCntMap == 1 && (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
				GetButtonTrigger(2) == true) && g_GameTutorial == false)
			{// �Q�[���ڕW�\��
				DeleteWindow(WINDOWTEXTURE_GAME_TUTORIAL_GAME);
				DeleteWindow(WINDOWTEXTURE_SELECT_2);
				SetMove(true);

				g_GameTutorial = true;
			}

			// �J�����̍X�V����
			UpdateCamera();

			// ���C�g�̍X�V����
			UpdateLight();

			// �t�B�[���h�̍X�V����
			UpdateMeshField();

			// �ǂ̍X�V����
			UpdateMeshWall();

			// �V�����_�[�̍X�V����
			UpdateMeshCylinder();

			// �r���{�[�h�I�u�W�F�N�g�̍X�V����
			UpdateBillboardObject();

			// �r���{�[�h�G�t�F�N�g�̍X�V����
			UpdateBillboardEffect();

			// �e�̍X�V����
			UpdateShadow();

			// �I�u�W�F�N�g�̍X�V����
			UpdateXObject();

			// �G�̍X�V����
			UpdateXEnemy();

			// �v���C���[�̍X�V����
			UpdateXPlayer();

			// ����̍X�V����
			UpdateXWeapon();

			// �X�R�A�̍X�V����
			UpdateScore();

			// �|�[�Y��Ԑݒ�
			SetPauseState();

			break;
		case GAMESTATE_GAMECLEAR:
		case GAMESTATE_GAMEOVER:

			if (g_nCntGameTimer == 60)
			{// �P�b��I��
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH + SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_RESULT_BG, SETTEXTURE_BG);

				if (g_GameState == GAMESTATE_GAMECLEAR)
				{
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2 * 10, SCREEN_HEIGHT / 2 * 10, WINDOWTEXTURE_RESULT_CLEAR, SETTEXTURE_BG);
				}
				else
				{
					SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2 * 10, SCREEN_HEIGHT / 2 * 10, WINDOWTEXTURE_RESULT_OVER, SETTEXTURE_BG);
				}
			}

			g_nCntGameTimer++;

			if (g_nCntGameTimer >= 240)
			{// ���U���g��
				SetMode(MODE_RESULT);
			}

			break;
		}
	}
	else if (g_bPause == true)
	{// �|�[�Y���g�p����Ă���ꍇ
		// �|�[�Y�̍X�V����
		UpdatePause();
	}
}

//=========================================================================================================================
// �Q�[���̕`�揈��
//=========================================================================================================================
void DrawGame(void)
{
	// �J�����̐ݒ�
	SetUpCamera();

	// �t�B�[���h�̕`�揈��
	DrawMeshField();

	// �V�����_�[�̕`�揈��
	DrawMeshCylinder();

	// �I�u�W�F�N�g�̕`�揈��
	DrawXObject();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// �r���{�[�h�I�u�W�F�N�g�̕`�揈��
	DrawBillboardObject();

	// �e�̕`�揈��
	DrawShadow();

	// �G�̕`�揈��
	DrawXEnemy();

	// �v���C���[�̕`�揈��
	DrawXPlayer();

	// �r���{�[�h�G�t�F�N�g�̕`�揈��
	DrawBillboardEffect();

	// ����̕`�揈��
	DrawXWeapon();

	// �X�R�A�̕`�揈��
	DrawScore();
}

//=========================================================================================================================
// �Q�[���ݒ菈��
//=========================================================================================================================
void SetGameState(GAMESTATE state)
{
	if (g_GameState == GAMESTATE_TUTORIAL)
	{// �Q�[�����C���Ɉڍs���鏀��
		StopSound(SOUND_LABEL_BGM_GAME_0);
	}

	if (g_GameState == GAMESTATE_PLAY && (state == GAMESTATE_GAMECLEAR || state == GAMESTATE_GAMEOVER))
	{// �N���A�A�I�[�o�[�Ɉڍs���鏀��
		StopSound(SOUND_LABEL_BGM_GAME_1);
		g_nCntGameTimer = 0;
	}

	if (g_GameState != state)
	{// ��Ԑݒ�
		g_GameState = state;			// ��Ԃ��L��
	}

	if (g_GameState == GAMESTATE_TUTORIAL || g_GameState == GAMESTATE_PLAY)
	{// �X�e�[�W����
		SetStage();
	}

	SetPauseState(false);
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
// �|�[�Y�擾����
//=========================================================================================================================
bool GetPauseState(void)
{
	return g_bPause;				// �l��Ԃ�
}

//=========================================================================================================================
// �`���[�g���A���X�e�b�v����
//=========================================================================================================================
void SetTutorialStep(bool bTutorial)
{
	g_bTutorialStep = bTutorial;
}

//=========================================================================================================================
// �v���C���[�o���ʒu�ݒ菈��
//=========================================================================================================================
void SetPlayerRespawn(int nRespawn)
{
	g_PlayerRespawnNum = nRespawn;	
}

//=========================================================================================================================
// �v���C�}�b�v���Z�b�g����
//=========================================================================================================================
void SetMapReset(int ResetMap)
{
	g_nCntMap = ResetMap;
}

//=========================================================================================================================
// �v���C�}�b�v�ԍ��擾����
//=========================================================================================================================
int GetNowMap(void)
{
	return g_nCntMap;
}

//=========================================================================================================================
// �X�e�[�W�ݒ菈��
//=========================================================================================================================
void SetStage(void)
{
	srand((unsigned int)time(0));

	XPLAYER *pXPlayer = GetXPlayer();

	// ���݂̏�Ԃ�ǂݍ���
	g_XPlayerUseWeapon = GetXWeapon();
	g_XPlayerHP = GetXPlayerHP();
	g_Weapon_1Lv = pXPlayer->nWeapon_1Lv;
	g_Weapon_2Lv = pXPlayer->nWeapon_2Lv;
	g_Weapon_3Lv = pXPlayer->nWeapon_3Lv;

	/*g_Weapon_1Lv = 15;
	g_Weapon_2Lv = 15;
	g_Weapon_3Lv = 15;*/

	// ������
	SetCancelRockOn();			// ���b�N�I��
	DeleteMeshField();			// ���b�V���t�B�[���h
	DeleteMeshCylinder();		// ���b�V���V�����_�[
	DeleteMeshWall();			// ���b�V���E�H�[��
	DeleteBillboardObject();	// �r���{�[�h�I�u�W�F�N�g
	DeleteBillboardEffect();	// �r���{�[�h�G�t�F�N�g
	DeleteShadow();				// �e
	DeleteXObject();			// �I�u�W�F�N�g
	DeleteXEnemy();				// �G
	DeleteXPlayer();			// �v���C���[
	DeleteXWeapon();			// ����
	DeleteWindow();				// �w�i
	DeleteFont();				// �t�H���g

	switch (g_GameState)
	{
	case GAMESTATE_TUTORIAL:	// �`���[�g���A��

		g_bTutorialStep = false;

		// �J����
		SetCamera(D3DXVECTOR3(0.0f, 180.0f, -350.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), CAMERATYPE_GAME);

		// ���b�V���t�B�[���h
		SetMeshField(9, 4);
		SetMeshField(10, 4);

		// ���b�V���E�H�[��
		//SetMeshWall(1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		//SetMeshWall(1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		//SetMeshWall(1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		//SetMeshWall(1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

		// �G
		SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, -1000.0f));
		SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(300.0f, 0.0f,-400.0f));
		SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-300.0f, 0.0f, -400.0f));

		// �v���C���[
		SetXPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// �w�i
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_MESSAGE, SETTEXTURE_FRONT);

		// �t�H���g
		SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 600, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 640, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 675, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		SetMessage(true);

		PlaySound(SOUND_LABEL_BGM_GAME_0);

		break;
	case GAMESTATE_PLAY:	// �Q�[��

		g_nCntMap++;
		//g_nCntMap = 5;

		// �J����
		SetCamera(D3DXVECTOR3(0.0f, 180.0f, -350.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), CAMERATYPE_GAME);

		// �}�b�v
		if (g_nCntMap >= 5)
		{
			g_nCntMap = 5;

			// ���b�V���t�B�[���h
			SetMeshField(0, 2);
			SetMeshField(1, 0);
			SetMeshField(2, 3);
			SetMeshField(3, 0);
			SetMeshField(4, 0);
			SetMeshField(5, 0);
			SetMeshField(6, 0);
			SetMeshField(7, 0);
			SetMeshField(8, 0);

			// �I�u�W�F�N�g
			SetXObject(TEXTTYPE_TREE_9);
			SetXObject(TEXTTYPE_TREE_10);
			SetXObject(TEXTTYPE_TREE_11);
			SetXObject(TEXTTYPE_TREE_12);
			SetXObject(TEXTTYPE_TREE_13);
			SetXObject(TEXTTYPE_TREE_14);

			// �v���C���[
			SetXPlayer(D3DXVECTOR3(0.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f));
		}
		else
		{
			// ���b�V���t�B�[���h
			SetMeshField(0, 1);
			SetMeshField(1, 0);
			SetMeshField(2, 1);
			SetMeshField(3, 0);
			SetMeshField(4, 0);
			SetMeshField(5, 0);
			SetMeshField(6, 0);
			SetMeshField(7, 1);
			SetMeshField(8, 0);

			// �I�u�W�F�N�g
			SetXObject(TEXTTYPE_TREE_0);
			SetXObject(TEXTTYPE_TREE_1);
			SetXObject(TEXTTYPE_TREE_2);
			SetXObject(TEXTTYPE_TREE_3);
			SetXObject(TEXTTYPE_TREE_4);
			SetXObject(TEXTTYPE_TREE_5);
			SetXObject(TEXTTYPE_TREE_6);
			SetXObject(TEXTTYPE_TREE_7);
			SetXObject(TEXTTYPE_TREE_8);

			// �r���{�[�h�I�u�W�F�N�g
			SetBillboardObject(D3DXVECTOR3(-1000.0f, 0.0f, 1650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXVECTOR3(300.0f, 50.0f, 0.0f), BILLBOARDTEXTURE_MAP_WALL);
			SetBillboardObject(D3DXVECTOR3(0.0f, 0.0f, 1650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXVECTOR3(300.0f, 50.0f, 0.0f), BILLBOARDTEXTURE_MAP_WALL);
			SetBillboardObject(D3DXVECTOR3(1000.0f, 0.0f, 1650.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), D3DXVECTOR3(300.0f, 50.0f, 0.0f), BILLBOARDTEXTURE_MAP_WALL);

			// �}�b�v�ړ��n�_
			if (g_PlayerRespawnNum == 0)
			{
				SetXObject(TEXTTYPE_MARUTA_2);
				SetXObject(TEXTTYPE_MARUTA_3);
				SetXPlayer(D3DXVECTOR3(0.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f));
			}
			else if (g_PlayerRespawnNum == 1)
			{
				SetXObject(TEXTTYPE_MARUTA_1);
				SetXObject(TEXTTYPE_MARUTA_3);
				SetXPlayer(D3DXVECTOR3(-1000.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f));
			}
			else if (g_PlayerRespawnNum == 2)
			{
				SetXObject(TEXTTYPE_MARUTA_1);
				SetXObject(TEXTTYPE_MARUTA_2);
				SetXPlayer(D3DXVECTOR3(1000.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f));
			}			
		}	

		// �E�H�[��
		SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
		//SetMeshWall(1, D3DXCOLOR(0.3f, 0.5f, 1.0f, 1.0f));

		// ���b�V���V�����_�[
		SetMeshCylinder(0);
		SetMeshCylinder(1);

		// �G
		if (g_nCntMap == 1)
		{
			SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(150.0f, 0.0f, 400.0f));
			SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-150.0f, 0.0f, 400.0f));
		}
		else if (g_nCntMap == 2)
		{
			SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(150.0f, 0.0f, 200.0f));
			SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-150.0f, 0.0f, 200.0f));

			SetXEnemy(rand() % 3, D3DXVECTOR3(75.0f, 0.0f, 100.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(-75.0f, 0.0f, 100.0f));
		}
		else if (g_nCntMap == 3)
		{
			SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(150.0f, 0.0f, 400.0f));
			SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-150.0f, 0.0f, 400.0f));

			SetXEnemy(rand() % 3, D3DXVECTOR3(-500.0f, 0.0f, -500.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(0.0f, 0.0f, -500.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(500.0f, 0.0f, -500.0f));
		}
		else if (g_nCntMap == 4)
		{
			SetXEnemy(rand() % 3, D3DXVECTOR3(-1000.0f, 0.0f, -500.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(0.0f, 0.0f, -500.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(1000.0f, 0.0f, -500.0f));

			SetXEnemy(rand() % 3, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, 5.0f));
			SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(150.0f, 0.0f, 600.0f));
			SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-150.0f, 0.0f, 600.0f));
		}
		else if (g_nCntMap == 5)
		{
			SetXEnemy(rand() % 3, D3DXVECTOR3(-300.0f, 0.0f, 350.0f));
			SetXEnemy(ENEMYTYPE_BOSS, D3DXVECTOR3(0.0f, 0.0f, 300.0f));
			SetXEnemy(rand() % 3, D3DXVECTOR3(300.0f, 0.0f, 350.0f));
		}

		// �X�R�A
		SetScore(D3DXVECTOR3(SCREEN_WIDTH - 50, 50, 0.0f), D3DXVECTOR3(25, 35, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCORETYPE_SCORE);

		SetWindow(D3DXVECTOR3(50.0f, 120.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 40, 110, WINDOWTEXTURE_GAME_MAP, SETTEXTURE_FRONT);

		SetWindow(D3DXVECTOR3(50.0f, 250.0f - (40.0f * g_nCntMap), 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30, 30, WINDOWTEXTURE_GAME_MAP_POS, SETTEXTURE_FRONT);

		SetWindow(D3DXVECTOR3(360, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 350, 50, WINDOWTEXTURE_GAME_HPBG, SETTEXTURE_FRONT);
		SetWindow(D3DXVECTOR3(70, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 55, 50, WINDOWTEXTURE_GAME_HP_M, SETTEXTURE_FRONT);
		SetWindow(D3DXVECTOR3(410, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(0.3f, 1.0f, 0.1f, 1.0f), 295, 45, WINDOWTEXTURE_GAME_HP, SETTEXTURE_FRONT);

		SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50, 50, WINDOWTEXTURE_GAME_WEAPON_1, SETTEXTURE_FRONT);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 350, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50, 50, WINDOWTEXTURE_GAME_WEAPON_2, SETTEXTURE_FRONT);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 500, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50, 50, WINDOWTEXTURE_GAME_WEAPON_3, SETTEXTURE_FRONT);
		SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT - 60, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 58, 58, WINDOWTEXTURE_GAME_WEAPON_99, SETTEXTURE_FRONT);

		// �N���A�ڕW�\���E��\��
		if (g_nCntMap == 1)
		{
			PlaySound(SOUND_LABEL_BGM_GAME_1);
			SetXWeapon(WEAPONTYPE_SWORD);

			g_LeftItem = rand() % 3;
			g_CenterItem = rand() % 3;
			g_RightItem = rand() % 3;

			if (g_GameTutorial == false)
			{
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_GAME_TUTORIAL_GAME, SETTEXTURE_FRONT);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 80, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_SELECT_2, SETTEXTURE_FRONT);
			}
			else
			{
				SetMove(true);
			}
		}
		else
		{
			SetXWeapon(g_XPlayerUseWeapon);
			SetWeaponLv(g_Weapon_1Lv, g_Weapon_2Lv, g_Weapon_3Lv);
			SetXPlayerHP(g_XPlayerHP);

			SetMove(true);

			if (g_nCntMap != 5)
			{
				// �A�C�e��
				if (g_LeftItem == 0)
				{
					SetBillboardObject(D3DXVECTOR3(-1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				}
				else if (g_LeftItem == 1)
				{
					SetBillboardObject(D3DXVECTOR3(-1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
				}
				else if (g_LeftItem == 2)
				{
					SetBillboardObject(D3DXVECTOR3(-1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
					SetBillboardObject(D3DXVECTOR3(-1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
				}

				if (g_CenterItem == 0)
				{
					SetBillboardObject(D3DXVECTOR3(-160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(-80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(0, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				}
				else if (g_CenterItem == 1)
				{
					SetBillboardObject(D3DXVECTOR3(-160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(-80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(0, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
				}
				else if (g_CenterItem == 2)
				{
					SetBillboardObject(D3DXVECTOR3(-160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
					SetBillboardObject(D3DXVECTOR3(-80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(0, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
				}

				if (g_RightItem == 0)
				{
					SetBillboardObject(D3DXVECTOR3(1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				}
				else if (g_RightItem == 1)
				{
					SetBillboardObject(D3DXVECTOR3(1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
				}
				else if (g_RightItem == 2)
				{
					SetBillboardObject(D3DXVECTOR3(1000 - 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
					SetBillboardObject(D3DXVECTOR3(1000 - 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 80, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
					SetBillboardObject(D3DXVECTOR3(1000 + 160, 40, -1200),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
				}
			}
			else
			{
				SetBillboardObject(D3DXVECTOR3(-80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(0, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);

				SetBillboardObject(D3DXVECTOR3(-800, 40, 1000),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
				SetBillboardObject(D3DXVECTOR3(-400, 40, 1000),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
				SetBillboardObject(D3DXVECTOR3(0, 40, 1000),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
				SetBillboardObject(D3DXVECTOR3(400, 40, 1000),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 3 + 6);
				SetBillboardObject(D3DXVECTOR3(800, 40, 1000),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), BILLBOARDTEXTURE_ITEM_LIFE);
			}
		}

		break;
	}	
}