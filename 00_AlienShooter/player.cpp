//=========================================================================================================================
// 
// �v���C���[���� [player.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include <stdlib.h>		// lib
#include <time.h>		// time
#include "player.h"		// �v���C���[
#include "bullet.h"		// �e
#include "explosion.h"	// ����
#include "effect.h"		// �G�t�F�N�g
#include "sound.h"		// �T�E���h
#include "input.h"		// ���͏���

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_PLAYER_POLGON		(11)									// �|���S���̍ő吔
#define MAX_PLAYER_TEXTURE		(2)										// �e�N�X�`���̍ő吔
#define PLAYER_TEXTURE_NAME1	"data\\TEXTURE\\life000.png"			// �ǂݍ��ރe�N�X�`���t�@�C��1
#define PLAYER_TEXTURE_NAME2	"data\\TEXTURE\\player001.png"			// �ǂݍ��ރe�N�X�`���t�@�C��2
#define PLAYER_POS_X			(SCREEN_WIDTH / 2 - 25)					// �v���C���[�̍���X���W
#define PLAYER_POS_Y			(SCREEN_HEIGHT - 100)					// �v���C���[�̍���Y���W
#define PLAYER_WIDTH			(SCREEN_WIDTH / 2 + 25)					// �v���C���[�̕�
#define PLAYER_HEIGHT			(SCREEN_HEIGHT - 50)					// �v���C���[�̍���
#define PLAYER_G_POS_X			(25/*23*/)									// g_pos�����X����
#define PLAYER_G_POS_Y			(25/*35*/)									// g_pos�����Y����
#define PLAYER_TEXTURE_SPEED	(10)									// �e�N�X�`���A�j���[�V�����X�s�[�h�i�e���x�j
#define PLAYER_TEXTURE_UV_U		(1.0f)									// �e�N�X�`���A�j���[�V����U�͈�
#define PLAYER_TEXTURE_UV_V		(1.0f)									// �e�N�X�`���A�j���[�V����V�͈�
#define PLAYER_UPSPEED			(5.0f)									// �v���C���[�����X�s�[�h
#define PLAYER_DOWNSPEED		(0.8f)									// �v���C���[�����X�s�[�h

#define PLAYERBULLET_ANGLE		(0.05f)									// �v���C���[�e�p�x
#define PLAYERBULLET_SPEED		(5.0f)									// �v���C���[�e���x

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void PlayerBulletType(void);			// �e���ˎ��

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTexturePlayer[MAX_PLAYER_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPlayer = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
Player						g_Player;									// �v���C���[�̏��
PlayerBullet				g_PlayerBullet;								// �v���C���[�̒e���

//=========================================================================================================================
// �v���C���[����������
//=========================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntPlayer;
	float fPosLife = 10;

	// �v���C���[�̏�����
	g_Player.pos = D3DXVECTOR3((PLAYER_POS_X + PLAYER_WIDTH) / 2, (PLAYER_POS_Y + PLAYER_HEIGHT) / 2, 0.0f);	// �v���C���[���S�l
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �v���C���[�ړ��l
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �v���C���[�F
	g_Player.nLife = PLAYER_LIFE;								// �v���C���[�̗�
	g_Player.State = PLAYERSTATE_NOMAL;							// �v���C���[���
	g_Player.nCounterState = 0;									// �v���C���[��ԃJ�E���g
	g_Player.Type = BULLETTYPE_PLAYER1;							// �v���C���[�e���
	g_Player.bDisp = true;										// �v���C���[�\��

	// �v���C���[�̒e������
	g_PlayerBullet.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�e���S�l
	g_PlayerBullet.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�e�ړ��l
	g_PlayerBullet.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);		// �v���C���[�e�F
	g_PlayerBullet.nCounterInput = 0;							// �v���C���[�e���̓J�E���^
	g_PlayerBullet.nAngle = 0.0f;								// �v���C���[�e�p�x

	for (nCntPlayer = 0; nCntPlayer < PLAYER_LIFE; nCntPlayer++)
	{
		g_Player.bLifeTex[nCntPlayer] = true;		// ���C�t
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(// ���C�t
		pDevice,
		PLAYER_TEXTURE_NAME1,
		&g_pTexturePlayer[0]
	);
	D3DXCreateTextureFromFile
	(// ���@
		pDevice,
		PLAYER_TEXTURE_NAME2,
		&g_pTexturePlayer[1]
	);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_PLAYER_POLGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL
	);
	
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffPlayer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntPlayer = 0; nCntPlayer < MAX_PLAYER_POLGON; nCntPlayer++)
	{
		if (nCntPlayer == MAX_PLAYER_POLGON - 1)
		{
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
		}
		else
		{
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(fPosLife, 10, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fPosLife + 50, 10, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(fPosLife, 60, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fPosLife + 50, 60, 0.0f);

			fPosLife += 50;
		}
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
		pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, PLAYER_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U, PLAYER_TEXTURE_UV_V);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//=========================================================================================================================
// �v���C���[�I������
//=========================================================================================================================
void UninitPlayer(void)
{
	// �ϐ��錾
	int nCntTex;

	// �e�N�X�`���[�̔j��
	for (nCntTex = 0; nCntTex < MAX_PLAYER_TEXTURE; nCntTex++)
	{// �e�N�X�`�����J�E���g
		if (g_pTexturePlayer[nCntTex] != NULL)
		{// NULL�ȊO�̏ꍇ
			g_pTexturePlayer[nCntTex]->Release();	// �J��
			g_pTexturePlayer[nCntTex] = NULL;		// NULL��
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffPlayer->Release();	// �J��
		g_pVtxBuffPlayer = NULL;		// NULL��
	}
}

//=========================================================================================================================
// �v���C���[�X�V����
//=========================================================================================================================
void UpdatePlayer(void)
{
	// �ϐ��錾
	float nPlayerSpeed = PLAYER_UPSPEED;			// �v���C���[���x

	g_PlayerBullet.pos = D3DXVECTOR3(g_Player.pos.x/* - 3.5f*/, g_Player.pos.y - 30.0f, g_Player.pos.z);	// �e�ʒu
	g_PlayerBullet.nAngle += PLAYERBULLET_ANGLE;	// �e�p�x

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffPlayer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// �v���C���[������ꍇ
	switch (g_Player.State)
	{// �G���
	case PLAYERSTATE_NOMAL:			// �ʏ���
		break;
	case PLAYERSTATE_DAMAGE:		// �_���[�W���
		g_Player.nCounterState--;	// ��Ԃ��J�E���g

		if (g_Player.nCounterState <= 0)
		{// ��ԃJ�E���g�I��
			// �F�ύX
			g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ��ԕω�
			g_Player.State = PLAYERSTATE_NOMAL;
		}
		break;
	case PLAYERSTATE_INVINCIBLE:	// ���G���
		g_Player.nCounterState--;

		if (g_Player.nCounterState <= 0)
		{// ��ԃJ�E���g�I��
			 // �F�ύX
			g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ��ԕω�
			g_Player.State = PLAYERSTATE_NOMAL;
		}
		break;
	case PLAYERSTATE_DEATH:			// ���S���
		break;
	}

	if (g_Player.bDisp == true)
	{// �v���C���[������ꍇ
		// �ړ����x�ύX
		if (GetKeyboardPress(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
		{// �C�ӂ̃L�[�iSHIFT�L�[�j���������ꍇ
			nPlayerSpeed = PLAYER_UPSPEED / 2;
			nPlayerSpeed = PLAYER_UPSPEED / 2;
		}
		else if (GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardPress(DIK_RSHIFT) == true)
		{// �C�ӂ̃L�[�iSHIFT�L�[�j�𗣂����ꍇ
			nPlayerSpeed = PLAYER_UPSPEED;
			nPlayerSpeed = PLAYER_UPSPEED;
		}

		// �v���C���[���G���A�O�̏ꍇ
		if (g_Player.pos.x - PLAYER_G_POS_X < 0)
		{// ��ʔ��Α��Ɉړ��i���j
			nPlayerSpeed = 0;
			g_Player.pos.x += 1;
		}
		else if (g_Player.pos.x + PLAYER_G_POS_X > SCREEN_WIDTH)
		{// ��ʔ��Α��Ɉړ��i�E�j
			nPlayerSpeed = 0;
			g_Player.pos.x -= 1;
		}
		else if (g_Player.pos.y - PLAYER_G_POS_Y < 0)
		{// ��ʔ��Α��Ɉړ��i��j
			nPlayerSpeed = 0;
			g_Player.pos.y += 1;
		}
		else if (g_Player.pos.y + PLAYER_G_POS_Y > SCREEN_HEIGHT)
		{// ��ʔ��Α��Ɉړ��i���j
			nPlayerSpeed = 0;
			g_Player.pos.y -= 1;
		}

		// �ړ���������
		if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
		{// �C�ӂ̃L�[�iA�L�[�A���L�[�j���������ꍇ
			if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
			{// �㓯������
				g_Player.move.x += sinf(-D3DX_PI * SINCOS_ANGLE_135) * nPlayerSpeed;
				g_Player.move.y += cosf(-D3DX_PI * SINCOS_ANGLE_135) * nPlayerSpeed;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
			{// ����������
				g_Player.move.x += sinf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else
			{// ���̂�
				g_Player.move.x += sinf(-D3DX_PI * SINCOS_ANGLE_90) * nPlayerSpeed;
				g_Player.move.y += cosf(-D3DX_PI * SINCOS_ANGLE_90) * nPlayerSpeed;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
		{// �C�ӂ̃L�[�iD�L�[�A�E�L�[�j���������ꍇ
			if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
			{// �㓯������
				g_Player.move.x += sinf(D3DX_PI * SINCOS_ANGLE_135) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI * SINCOS_ANGLE_135) * nPlayerSpeed;
			}
			else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
			{// ����������
				g_Player.move.x += sinf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else
			{// �E�̂�
				g_Player.move.x += sinf(D3DX_PI * SINCOS_ANGLE_90) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI * SINCOS_ANGLE_90) * nPlayerSpeed;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true)
		{// �C�ӂ̃L�[�iW�L�[�A��L�[�j���������ꍇ
			if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
			{// ����������
				g_Player.move.x += sinf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
			{// �E��������
				g_Player.move.x += sinf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else
			{// ��̂�

				g_Player.move.x += sinf(D3DX_PI) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI) * nPlayerSpeed;
			}
		}
		else if (GetKeyboardPress(DIK_S) == true || GetKeyboardPress(DIK_DOWN) == true)
		{// �C�ӂ̃L�[�iD�L�[�A���L�[�j���������ꍇ
			if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
			{// ����������
				g_Player.move.x += sinf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(-D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
			{// �E��������
				g_Player.move.x += sinf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
				g_Player.move.y += cosf(D3DX_PI * SINCOS_ANGLE_45) * nPlayerSpeed;
			}
			else
			{// ���̂�
				g_Player.move.x += sinf(SINCOS_ANGLE_0) * nPlayerSpeed;
				g_Player.move.y += cosf(SINCOS_ANGLE_0) * nPlayerSpeed;
			}
		}

		// �ړ�����
		g_Player.pos.x += g_Player.move.x;
		g_Player.pos.y += g_Player.move.y;

		// ��������
		g_Player.move.x += (0.0f - g_Player.move.x) * PLAYER_DOWNSPEED;
		g_Player.move.y += (0.0f - g_Player.move.y) * PLAYER_DOWNSPEED;

		// �e���ˏ���
		if (GetKeyboardPress(DIK_SPACE) == true)
		{// �C�ӂ̃L�[�iSPACE�L�[�j���������ꍇ
			if ((g_PlayerBullet.nCounterInput % (PLAYER_TEXTURE_SPEED - 5)) == 0)
			{// �l�i�P�j
				PlaySound(SOUND_LABEL_SE_SHOT000);		// ����
				
				PlayerBulletType();		// �e���ˎ��
			}
			g_PlayerBullet.nCounterInput++;					// �J�E���^�[���Z
		}
		else
		{
			g_PlayerBullet.nCounterInput = 0;
		}
	}

	// ���_�ݒ�
	pVtx[(MAX_PLAYER_POLGON - 1) * 4].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_G_POS_X, g_Player.pos.y - PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 1].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_G_POS_X, g_Player.pos.y - PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 2].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_G_POS_X, g_Player.pos.y + PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_G_POS_X, g_Player.pos.y + PLAYER_G_POS_Y, 0.0f);
	// ���_�J���[�ݒ�
	pVtx[(MAX_PLAYER_POLGON - 1) * 4].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 1].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 2].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 3].col = g_Player.col;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//=========================================================================================================================
// �v���C���[�`�揈��
//=========================================================================================================================
void DrawPlayer(void)
{
	// �ϐ��錾
	int nCntPlayer;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffPlayer,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPlayer = 0; nCntPlayer < MAX_PLAYER_POLGON; nCntPlayer++)
	{
		if (g_Player.bDisp == true && nCntPlayer == MAX_PLAYER_POLGON - 1)
		{// �v���C���[������ꍇ
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTexturePlayer[1]);

			// �v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlayer * 4, 2);
		}
		else if (g_Player.bLifeTex[nCntPlayer] == true)
		{
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTexturePlayer[0]);

			// �v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPlayer * 4, 2);
		}
	}
}

//=========================================================================================================================
// �v���C���[�ݒ菈��
//=========================================================================================================================
void SetPlayer(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffPlayer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);
	
	// �v���C���[�̏�����
	g_Player.pos = D3DXVECTOR3((PLAYER_POS_X + PLAYER_WIDTH) / 2, (PLAYER_POS_Y + PLAYER_HEIGHT) / 2, 0.0f);	// �v���C���[���S�l
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �v���C���[�ړ��l
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �v���C���[�F
	g_Player.nLife = 20;										// �v���C���[�̗�
	g_Player.State = PLAYERSTATE_NOMAL;							// �v���C���[���
	g_Player.nCounterState = 0;									// �v���C���[��ԃJ�E���g
	g_Player.Type = BULLETTYPE_PLAYER1;							// �v���C���[�e���
	g_Player.bDisp = true;										// �v���C���[�\��

	// �v���C���[�̒e������
	g_PlayerBullet.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �v���C���[�e���S�l
	g_PlayerBullet.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �v���C���[�e�ړ��l
	g_PlayerBullet.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);		// �v���C���[�e�F
	g_PlayerBullet.nCounterInput = 0;							// �v���C���[�e���̓J�E���^
	g_PlayerBullet.nAngle = 0.0f;								// �v���C���[�e�p�x

	for (int nCntPlayer = 0; nCntPlayer < PLAYER_LIFE; nCntPlayer++)
	{
		g_Player.bLifeTex[nCntPlayer] = true;
	}

	// ���_�ݒ�
	pVtx[(MAX_PLAYER_POLGON - 1) * 4].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_G_POS_X, g_Player.pos.y - PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 1].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_G_POS_X, g_Player.pos.y - PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 2].pos = D3DXVECTOR3(g_Player.pos.x - PLAYER_G_POS_X, g_Player.pos.y + PLAYER_G_POS_Y, 0.0f);
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 3].pos = D3DXVECTOR3(g_Player.pos.x + PLAYER_G_POS_X, g_Player.pos.y + PLAYER_G_POS_Y, 0.0f);
	// ���_�J���[�ݒ�
	pVtx[(MAX_PLAYER_POLGON - 1) * 4].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 1].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 2].col = g_Player.col;
	pVtx[(MAX_PLAYER_POLGON - 1) * 4 + 3].col = g_Player.col;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();

	SetParticleEffect(g_Player.pos, EFFECTTYPE_RESURRECTION, 80, 50, 50);			// �G�t�F�N�g���Z�b�g
}

//=========================================================================================================================
// �v���C���[�擾����
//=========================================================================================================================
Player *GetPlayer(void)
{
	return &g_Player;		// �l��Ԃ�
}

//=========================================================================================================================
// �v���C���[�e�����菈��
//=========================================================================================================================
void HitPlayer(int nDamage)
{
	// �ϐ��錾
	int ExplosionSize = 100;	// �����T�C�Y

	// ���C�t������
	g_Player.nLife -= nDamage;
	g_Player.bLifeTex[g_Player.nLife] = false;

	if (g_Player.nLife <= 0)
	{// ���C�t��0�ȉ��ɂȂ����ꍇ
		SetExplosion(g_Player.pos, D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), ExplosionSize);	// ����
		SetParticleEffect(g_Player.pos, EFFECTTYPE_EXPLOSION, 100, 50, 100);			// �G�t�F�N�g���Z�b�g
		g_Player.bDisp = false;					// �g�p���Ă��Ȃ����
		g_Player.State = PLAYERSTATE_DEATH;		// �f�X���
		// g_Player.nCounterState = 60;

		PlaySound(SOUND_LABEL_SE_EXPLOSION);	// ����

		MODE mode = GetMode();					// ���[�h�擾

		if (mode != MODE_TUTORIAL)
		{// �`���[�g���A���ȊO�̏ꍇ
			SetGameState(GAMESTATE_GAMEOVER);	// �Q�[���I�[�o�[
		}
	}
	else
	{// ���C�t��1�ȏ�̏ꍇ
		g_Player.State = PLAYERSTATE_DAMAGE;	// �_���[�W���
		g_Player.nCounterState = 10;			// �J�E���^��ݒ�

		PlaySound(SOUND_LABEL_SE_HIT001);		// �q�b�g��

		// �F�ύX
		g_Player.col = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
	}
}

//==================================================================================================
// �v���C���[�e���ˎ��
//==================================================================================================
void PlayerBulletType(void)
{
	if (g_Player.Type == BULLETTYPE_PLAYER1)
	{
		// �O
		g_PlayerBullet.move.x = sinf(D3DX_PI) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_NORMAL, PLAYERBULLET_SPEED, 0, 0);
	}
	else if (g_Player.Type == BULLETTYPE_PLAYER2)
	{
		// �O
		g_PlayerBullet.move.x = sinf(D3DX_PI) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_NORMAL, PLAYERBULLET_SPEED, 0, 0);

		// �O�g
		g_PlayerBullet.move.x = sinf(D3DX_PI + g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
	}
	else if (g_Player.Type == BULLETTYPE_PLAYER3)
	{
		// �O
		g_PlayerBullet.move.x = sinf(D3DX_PI) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_NORMAL, PLAYERBULLET_SPEED, 0, 0);

		// �O�g
		g_PlayerBullet.move.x = sinf(D3DX_PI + g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
		g_PlayerBullet.move.x = sinf(D3DX_PI - g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
	}
	else if (g_Player.Type == BULLETTYPE_PLAYER4)
	{
		// �O
		g_PlayerBullet.move.x = sinf(D3DX_PI) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_NORMAL, PLAYERBULLET_SPEED, 0, 0);

		// �O�g
		g_PlayerBullet.move.x = sinf(D3DX_PI + g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
		g_PlayerBullet.move.x = sinf(D3DX_PI - g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
	}
	else if (g_Player.Type == BULLETTYPE_PLAYER5)
	{
		// �O
		g_PlayerBullet.move.x = sinf(D3DX_PI) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_NORMAL, PLAYERBULLET_SPEED, 0, 0);

		// �O�g
		g_PlayerBullet.move.x = sinf(D3DX_PI + g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
		g_PlayerBullet.move.x = sinf(D3DX_PI - g_PlayerBullet.nAngle) * PLAYERBULLET_SPEED;
		g_PlayerBullet.move.y = cosf(D3DX_PI) * PLAYERBULLET_SPEED;
		SetBullet(g_PlayerBullet.pos, g_PlayerBullet.move, g_PlayerBullet.col, g_Player.Type, BULLETMOVE_MOVECHANGE_STRAIGHT, PLAYERBULLET_SPEED, 30, 0);
	}
}