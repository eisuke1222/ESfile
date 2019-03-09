//=========================================================================================================================
// 
// �`���[�g���A������ [tutorial.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "tutorial.h"		// �`���[�g���A��
#include "sound.h"			// �T�E���h
#include "fade.h"			// �t�F�[�h
#include "select.h"			// �I������
#include "input.h"			// ���͏���

#include "player.h"			// �v���C���[
#include "enemy.h"			// �G
#include "bullet.h"			// �e
#include "explosion.h"		// ����
#include "effect.h"			// �G�t�F�N�g
#include "item.h"			// �A�C�e��
#include "pause.h"			// �|�[�Y

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define TUTORIAL_TEXTURE_NAME_1		"data\\TEXTURE\\tutorial004.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define TUTORIAL_POS_X				(0)									// �`���[�g���A���̍���X���W
#define TUTORIAL_POS_Y				(0)									// �`���[�g���A���̍���Y���W
#define TUTORIAL_WIDTH				(SCREEN_WIDTH)						// �`���[�g���A���̕�
#define TUTORIAL_HEIGHT				(SCREEN_HEIGHT)						// �`���[�g���A���̍�
#define TUTORIAL_TEXTURE_UV_U		(1.0f)								// �e�N�X�`���A�j���[�V����U�͈�
#define TUTORIAL_TEXTURE_UV_V		(1.0f)								// �e�N�X�`���A�j���[�V����V�͈�

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureTutorial = NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTutorial = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3					g_posTutorial;								// �`���[�g���A���̈ʒu
bool						g_bTutorialPause;							// �|�[�Y

//=========================================================================================================================
// �`���[�g���A������������
//=========================================================================================================================
void InitTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	g_posTutorial = D3DXVECTOR3((TUTORIAL_POS_X + TUTORIAL_WIDTH) / 2, (TUTORIAL_POS_Y + TUTORIAL_HEIGHT) / 2, 0.0f);		// �|���S���̒��S�l
	g_bTutorialPause = false;					// �|�[�Y�����ݒ�

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(// �`���[�g���A��
		pDevice,
		TUTORIAL_TEXTURE_NAME_1,
		&g_pTextureTutorial
	);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffTutorial->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// ���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIAL_WIDTH, TUTORIAL_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIAL_POS_X, TUTORIAL_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIAL_WIDTH, TUTORIAL_HEIGHT, 0.0f);
	// 1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// ���_�J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(TUTORIAL_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, TUTORIAL_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(TUTORIAL_TEXTURE_UV_U, TUTORIAL_TEXTURE_UV_V);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTutorial->Unlock();

	// �G�̏���������
	InitEnemy();
	
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

	SetEnterSelect(D3DXVECTOR3(SCREEN_WIDTH - 105, SCREEN_HEIGHT - 40, 0.0f), D3DXVECTOR3(100, 40, 0.0f));
}

//=========================================================================================================================
// �`���[�g���A���I������
//=========================================================================================================================
void UninitTutorial(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureTutorial != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pTextureTutorial->Release();		// �J��
		g_pTextureTutorial = NULL;			// NULL��
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorial != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffTutorial->Release();		// �J��
		g_pVtxBuffTutorial = NULL;			// NULL��
	}

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
// �`���[�g���A���X�V����
//=========================================================================================================================
void UpdateTutorial(void)
{
	// �C�ӂ̃L�[�iENTER�L�[�j���������ꍇ
	FADE Fade = GetFade();		// �t�F�[�h���擾

	if (GetKeyboardTrigger(DIK_P) == true && Fade == FADE_NONE)
	{// �|�[�Y��ؑ�
		g_bTutorialPause = g_bTutorialPause ? false : true;
		PlaySound(SOUND_LABEL_SE_PAUSE);
	}

	if (g_bTutorialPause == false)
	{// �|�[�Y���g�p���Ă��Ȃ��ꍇ
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

		//�|�[�Y������Ԃ�
		SetPause();

		Enemy *pEnemy = GetEnemy();
		int nNumEnemy = GetNumEnemy();	// �G�̑������擾

		if (nNumEnemy == 0 && pEnemy->bUse == false)
		{
			SetEnemy(D3DXVECTOR3(-50.0f, 100.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50) * 1.0f, 100.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 50, ENEMYTYPE_TUTORIALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-250.0f, 200.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50) * 1.0f, 200.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 50, ENEMYTYPE_TUTORIALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
			SetEnemy(D3DXVECTOR3(-450.0f, 300.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH + 50) * 1.0f, 300.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 3.0f, ENEMYMOVE_NORMAL, 0, 0, 0, 20, 50, ENEMYTYPE_TUTORIALENEMY,
				EnemyBullet{ ENEMYBULLET_NO_4, 3.0f, 30 },
				EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE }, EnemyBullet{ ENEMYBULLET_NONE });
		}

		Player *pPlayer = GetPlayer();
		if (pPlayer->bDisp == false)
		{
			static int nCount = 0;
			nCount++;
			if (nCount > 120)
			{
				SetPlayer();		// ������_���[�W�󂯂�Ƃ���ŃG���[
				nCount = 0;
			}
		}

		if (Fade == FADE_NONE)
		{// �t�F�[�h���g�p���Ă��Ȃ��ꍇ
			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{// ��ʐ؂�ւ�
				ChangeSelect(SELECTSTATE_ENTER);
				PlaySound(SOUND_LABEL_SE_DECIDE);		// ���艹
				SetFade(MODE_GAME);						// �Q�[��
			}
		}
	}

	if (g_bTutorialPause == true)
	{// �|�[�Y���g�p����Ă���ꍇ
		// �|�[�Y�̍X�V����
		UpdatePause();
	}
}

//=========================================================================================================================
// �`���[�g���A���`�揈��
//=========================================================================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffTutorial,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureTutorial);

	// �`���[�g���A���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

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

	FADE Fade = GetFade();
	if (g_bTutorialPause == true && Fade == FADE_NONE)
	{// �|�[�Y���g�p����Ă���ꍇ
	 // �|�[�Y�̕`�揈��
		DrawPause();
	}
}

//=========================================================================================================================
// �|�[�Y�ݒ菈��
//=========================================================================================================================
void SetTutorialPauseState(bool bTutorialPause)
{
	g_bTutorialPause = bTutorialPause;				// ��Ԃ��L��
}