//=========================================================================================================================
//
// �^�C�g������ [title.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include <time.h>
#include "title.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "xobject.h"
#include "shadow.h"
#include "font.h"
#include "meshwall.h"
#include "billboardobject.h"
#include "xenemy.h"
#include "xplayer.h"
#include "sound.h"
#include "window.h"
#include "fade.h"
#include "input.h"
#include "game.h"

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
int					g_nCntTitleTimer;		// �^�C�g���^�C�}�[
bool				g_bModeChange;			// ���[�h�`�F���W
bool				g_bSet;					// �ݒ�L��
bool				g_bTitleSound = false;	//
int					g_nCntView;

//=========================================================================================================================
// �^�C�g���̏���������
//=========================================================================================================================
void InitTitle(void)
{
	// ������
	g_nCntTitleTimer = 0;
	g_bModeChange = false;
	g_bSet = false;
	g_nCntView = 0;

	// �J��������������
	InitCamera();

	// ���C�g����������
	InitLight();

	// �t�B�[���h����������
	InitMeshField();

	// �Ǐ���������
	InitMeshWall();

	// �V�����_�[����������
	InitMeshCylinder();

	// �e����������
	InitShadow();

	// �I�u�W�F�N�g����������
	InitXObject();

	// �G����������
	InitXEnemy();

	// �v���C���[����������
	InitXPlayer();

	// �r���{�[�h�I�u�W�F�N�g����������
	InitBillboardObject();

	SetGameState(GAMESTATE_NONE);

	// �E�B���h�E�ݒ�
	SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_TITLE_ANIM, SETTEXTURE_TOPFRONT);

	if (g_bTitleSound == false)
	{
		StopSound();
	}
}

//=========================================================================================================================
// �^�C�g���̏I������
//=========================================================================================================================
void UninitTitle(void)
{
	// �J�����I������
	UninitCamera();

	// ���C�g�I������
	UninitLight();

	// �t�B�[���h�I������
	UninitMeshField();

	// �ǏI������
	UninitMeshWall();

	// �V�����_�[�I������
	UninitMeshCylinder();

	// �e�I������
	UninitShadow();

	// �I�u�W�F�N�g�I������
	UninitXObject();

	// �G�I������
	UninitXEnemy();

	// �v���C���[�I������
	UninitXPlayer();

	// �r���{�[�h�I�u�W�F�N�g�I������
	UninitBillboardObject();

	// �E�B���h�E�j��
	DeleteWindow();

	// �t�H���g�j��
	DeleteFont();
}

//=========================================================================================================================
// �^�C�g���̍X�V����
//=========================================================================================================================
void UpdateTitle(void)
{
	FADE fade = GetFade();		// �t�F�[�h��Ԏ擾

	// �J�����X�V����
	UpdateCamera();

	// ���C�g�X�V����
	UpdateLight();

	// �t�B�[���h�X�V����
	UpdateMeshField();

	// �ǍX�V����
	UpdateMeshWall();

	// �V�����_�[�X�V����
	UpdateMeshCylinder();

	// �e�X�V����
	UpdateShadow();

	// �I�u�W�F�N�g�X�V����
	UpdateXObject();

	// �G�X�V����
	UpdateXEnemy();

	// �v���C���[�X�V����
	UpdateXPlayer();

	// �r���{�[�h�I�u�W�F�g�X�V����
	UpdateBillboardObject();

	// ���ԃJ�E���^
	g_nCntTitleTimer++;

	if (g_nCntTitleTimer == 600)
	{// �����Ń����L���O�Ɉڍs
		g_nCntView++;

		if (g_nCntView == 3)
		{
			SetFadeMode(MODE_RANKING);
		}
		else
		{
			SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOW_FADE, SETTEXTURE_TOPBG);
		}
	}
	else if (g_nCntTitleTimer == 635)
	{// �؂�ւ�
		g_nCntTitleTimer = 0;

		if (g_nCntView == 1)
		{
			srand((unsigned int)time(0));

			// �j��
			DeleteMeshField();
			DeleteMeshCylinder();
			DeleteShadow();
			DeleteXObject();

			// �X�e�[�Wor�`���[�g���A��
			if ((rand() % 2) == 0)
			{
				// �J����
				SetCamera(D3DXVECTOR3(0.0f, 180.0f, -350.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f), CAMERATYPE_GAME);

				// ���b�V���t�B�[���h
				SetMeshField(9, 4);

				// �v���C���[
				SetXPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				// ���b�Z�[�W
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, WINDOWTEXTURE_MESSAGE, SETTEXTURE_FRONT);

				// �t�H���g
				SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 600, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 640, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				SetFont(35, 0, SHIFTJIS_CHARSET, RECT{ 300, 675, SCREEN_WIDTH, SCREEN_HEIGHT }, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				SetMessage(true);
			}
			else
			{
				// �J����
				SetCamera(D3DXVECTOR3(0.0f, 180.0f, -2050.0f), D3DXVECTOR3(0.0f, 50.0f, -1650.0f), CAMERATYPE_TITLE_GAME);

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

				// �I�u�W�F�N�g
				SetXObject(TEXTTYPE_MARUTA_2);
				SetXObject(TEXTTYPE_MARUTA_3);
				SetXPlayer(D3DXVECTOR3(0.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 3.14f, 0.0f));

				// ���b�V���E�H�[��
				SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
				SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
				SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));
				SetMeshWall(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f));

				// ���b�V���V�����_�[
				SetMeshCylinder(0);
				SetMeshCylinder(1);

				// �G
				SetXEnemy(rand() % 3, D3DXVECTOR3(-1000.0f, 0.0f, -500.0f));
				SetXEnemy(rand() % 3, D3DXVECTOR3(0.0f, 0.0f, -500.0f));
				SetXEnemy(rand() % 3, D3DXVECTOR3(1000.0f, 0.0f, -500.0f));

				SetXEnemy(rand() % 3, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				SetXEnemy(ENEMYTYPE_1, D3DXVECTOR3(0.0f, 0.0f, 5.0f));
				SetXEnemy(ENEMYTYPE_2, D3DXVECTOR3(150.0f, 0.0f, 600.0f));
				SetXEnemy(ENEMYTYPE_3, D3DXVECTOR3(-150.0f, 0.0f, 600.0f));

				// �A�C�e��
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

				SetBillboardObject(D3DXVECTOR3(0 - 160, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(0 - 80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(0, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(0 + 80, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);
				SetBillboardObject(D3DXVECTOR3(0 + 160, 40, -1200),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(20.0f, 40.0f, 0.0f), rand() % 4 + 6);

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
		}
		else if (g_nCntView == 2)
		{
			// �j��
			DeleteMeshField();
			DeleteMeshWall();
			DeleteMeshCylinder();
			DeleteShadow();
			DeleteXObject();
			DeleteXEnemy();
			DeleteXPlayer();
			DeleteBillboardObject();
			DeleteWindow(WINDOWTEXTURE_MESSAGE);
			DeleteFont();

			// �J����
			SetCamera(D3DXVECTOR3(0.0f, 650.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_TITLE);

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

			// ���b�V���V�����_�[
			SetMeshCylinder(0);
			SetMeshCylinder(1);

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
		}
	}

	if ((GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
		GetButtonTrigger(2) == true) && fade == FADETYPE_NONE && g_bModeChange == true)
	{// ���[�h�`�F���W
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFadeMode(MODE_GAME);
		SetSelectState(SELECTSTATE_ENTER);
	}

	if (g_bTitleSound == false && g_nCntTitleTimer == 1 && g_nCntView == 0)
	{
		StopSound(SOUND_LABEL_SE_TITLE_1);

		PlaySound(SOUND_LABEL_SE_TITLE_0);
	}

	if (g_bModeChange == false)
	{
		if ((GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_Z) == true ||
			GetButtonTrigger(2) == true) && g_nCntTitleTimer < 45)
		{// �^�C�g���X�L�b�v
			if (g_bTitleSound == false)
			{
				PlaySound(SOUND_LABEL_SE_TITLE_1);

				PlaySound(SOUND_LABEL_BGM_TITLE);
				g_bTitleSound = true;
			}

			// �}�b�v����
			if (g_bSet == false && g_nCntView == 0)
			{// �}�b�v����������Ă��Ȃ��ꍇ
				// �J����
				SetCamera(D3DXVECTOR3(0.0f, 650.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_TITLE);

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

				// ���b�V���V�����_�[
				SetMeshCylinder(0);
				SetMeshCylinder(1);

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
			}

			g_bModeChange = true;	// ���[�h�ύX
			g_bSet = true;			// �Z�b�g

			// �^�C�g���\��
			if (g_nCntView == 0)
			{
				DeleteWindow(0);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 150, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, WINDOWTEXTURE_TITLE, SETTEXTURE_TOPFRONT);
				SetWindow(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 120, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 180, 50, WINDOWTEXTURE_SELECT_1, SETTEXTURE_TOPFRONT);
			}
		}
		else if (g_nCntTitleTimer == 45)
		{// ���[�h�ؑւ��\��
			g_bModeChange = true;

			if (g_bTitleSound == false)
			{
				PlaySound(SOUND_LABEL_SE_TITLE_1);

				PlaySound(SOUND_LABEL_BGM_TITLE);
				g_bTitleSound = true;
			}

			// �}�b�v����
			if (g_bSet == false && g_nCntView == 0)
			{// �}�b�v����������Ă��Ȃ��ꍇ
				// �J����
				SetCamera(D3DXVECTOR3(0.0f, 650.0f, -1800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CAMERATYPE_TITLE);

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

				// ���b�V���V�����_�[
				SetMeshCylinder(0);
				SetMeshCylinder(1);

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
			}
		}
	}
}

//=========================================================================================================================
// �^�C�g���̕`�揈��
//=========================================================================================================================
void DrawTitle(void)
{
	// �J�����ݒ�
	SetUpCamera();

	// �t�B�[���h�`�揈��
	DrawMeshField();

	// �V�����_�[�`�揈��
	DrawMeshCylinder();

	// �I�u�W�F�N�g�`�揈��
	DrawXObject();

	// �Ǖ`�揈��
	DrawMeshWall();

	// �e�`�揈��
	DrawShadow();

	// �G�`�揈��
	DrawXEnemy();

	// �v���C���[�`�揈��
	DrawXPlayer();

	// �r���{�[�h�I�u�W�F�N�g�`�揈��
	DrawBillboardObject();
}

//=========================================================================================================================
// �^�C�g���̃T�E���h����
//=========================================================================================================================
void SetTitleSound(bool bSound)
{
	g_bTitleSound = bSound;
}

//=========================================================================================================================
// �^�C�g���̃T�E���h����
//=========================================================================================================================
int GetViewNum(void)
{
	return g_nCntView;
}