//=========================================================================================================================
// 
// �G���� [enemy.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "enemy.h"			// �G
#include "bullet.h"			// �e
#include "explosion.h"		// ����
#include "effect.h"			// �G�t�F�N�g
#include "score.h"			// �X�R�A
#include "player.h"			// �v���C���[
#include "item.h"			// �A�C�e��
#include "sound.h"			// �T�E���h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define	MAX_ENEMY_TEXTURE				(5)									// �G�̎��
#define ENEMY_TEXTURE_NAME_1			"data\\TEXTURE\\enemy000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��1
#define ENEMY_TEXTURE_NAME_2			"data\\TEXTURE\\enemy001.png"		// �ǂݍ��ރe�N�X�`���t�@�C��2
#define ENEMY_TEXTURE_NAME_3			"data\\TEXTURE\\enemy002.png"		// �ǂݍ��ރe�N�X�`���t�@�C��3
#define ENEMY_TEXTURE_NAME_4			"data\\TEXTURE\\enemy003.png"		// �ǂݍ��ރe�N�X�`���t�@�C��4
#define ENEMY_TEXTURE_NAME_5			"data\\TEXTURE\\enemy004.png"		// �ǂݍ��ރe�N�X�`���t�@�C��5
#define ENEMY_TEXTURE_SPEED				(15)								// �e�N�X�`���A�j���[�V�����X�s�[�h
#define ENEMY_TEXTURE_PATTERN			(2)									// �e�N�X�`���A�j���[�V�����p�^�[����
#define ENEMY_TEXTURE_UV_U				(0.5f)								// �e�N�X�`���A�j���[�V����U�͈�
#define ENEMY_TEXTURE_UV_V				(1.0f)								// �e�N�X�`���A�j���[�V����V�͈�
#define ENEMY_SPEED						(10)								// �G�ړ����x

#define MAX_ENEMYBULLET_TYPE			(5)									// �G�e�̔��˃p�^�[��
#define ENEMYBULLET_ANGLE				(0.1f)								// �G�e�p�x

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void EnwmyBulletType(int nCntEnemy);			// �e���ˎ��
void EnwmyItemSet(int nCntEnemy);				// �A�C�e���Z�b�g

//*************************************************************************************************************************
// �O���[�o���ϐ��錾
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_apTextureEnemy[MAX_ENEMY_TEXTURE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffEnemy = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
Enemy						g_aEnemy[MAX_ENEMY];							// �G�̏��
EnemyBullet					g_aEnemyBullet[MAX_ENEMY][MAX_ENEMYBULLET_TYPE];// �G�e�̏��
int							g_nNumEnemy;									// �G�̑���

//=========================================================================================================================
// �G����������
//=========================================================================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̎擾
	int nCntEnemy;		// �G�J�E���g

	// �G�̏��̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G���J�E���g
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �G���S�l
		g_aEnemy[nCntEnemy].posTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ڕW�ʒu
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �G�ړ��l
		g_aEnemy[nCntEnemy].moveSpeed = 0;									// �ړ����x
		g_aEnemy[nCntEnemy].moveType = ENEMYMOVE_NONE;						// �ړ����
		g_aEnemy[nCntEnemy].moveStart = 0;									// �ړ��J�n����
		g_aEnemy[nCntEnemy].moveLimit = 0;									// �ړ���
		g_aEnemy[nCntEnemy].nCntTarget = 0;									// �ڕW�ʒu��
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �G�F
		g_aEnemy[nCntEnemy].nType = 0;										// �G���
		g_aEnemy[nCntEnemy].nLife = 0;										// �G�̗�
		g_aEnemy[nCntEnemy].Type = ENEMYTYPE_NORMALENEMY;					// �G�N���X
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_NOMAL;						// �G���
		g_aEnemy[nCntEnemy].nCounterState = 0;								// �G��ԃJ�E���g
		g_aEnemy[nCntEnemy].nCounterAnim = 0;								// �G�J�E���^�A�j���[�V����
		g_aEnemy[nCntEnemy].nPatternAnim = 0;								// �G�p�^�[���A�j���[�V����
		g_aEnemy[nCntEnemy].bUse = false;									// �G�g�p���Ă��邩

		// �G�̒e���̏�����
		for (int nCount = 0; nCount < MAX_ENEMYBULLET_TYPE; nCount++)
		{// �G�e�̃p�^�[�����J�E���g
			g_aEnemyBullet[nCntEnemy][nCount].nPattenType = ENEMYBULLET_NONE;			// �G�e���˃p�^�[��
			g_aEnemyBullet[nCntEnemy][nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �G�e�ړ����x
			g_aEnemyBullet[nCntEnemy][nCount].moveSpeed = 0.0f;							// �G�e�ړ����x
			g_aEnemyBullet[nCntEnemy][nCount].nTiming = 0;								// �G�e���˃^�C�~���O
			g_aEnemyBullet[nCntEnemy][nCount].Type = BULLETTYPE_ENEMY;					// �G�e���
			g_aEnemyBullet[nCntEnemy][nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �G�e�ړ��l
			g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �G�e�F
			g_aEnemyBullet[nCntEnemy][nCount].nCounterInput = 0;						// �G�e���̓J�E���^
			g_aEnemyBullet[nCntEnemy][nCount].nAngle = 0.0f;							// �G�e�p�x
		}
	}

	g_nNumEnemy = 0;			// �G����

	// �e�N�X�`���̓ǂݍ���(�S)
	D3DXCreateTextureFromFile
	(// �G1
		pDevice,
		ENEMY_TEXTURE_NAME_1,
		&g_apTextureEnemy[0]
	);
	D3DXCreateTextureFromFile
	(// �G2
		pDevice,
		ENEMY_TEXTURE_NAME_2,
		&g_apTextureEnemy[1]
	);
	D3DXCreateTextureFromFile
	(// �G3
		pDevice,
		ENEMY_TEXTURE_NAME_3,
		&g_apTextureEnemy[2]
	);
	D3DXCreateTextureFromFile
	(// �G4
		pDevice,
		ENEMY_TEXTURE_NAME_4,
		&g_apTextureEnemy[3]
	);
	D3DXCreateTextureFromFile
	(// �G4
		pDevice,
		ENEMY_TEXTURE_NAME_5,
		&g_apTextureEnemy[4]
	);

	// ���_�o�b�t�@�𐶐�
    pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL
	);

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock
	(
		0, 
		0, 
		(void**)&pVtx, 
		0
	);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G���J�E���g
		// ���_�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
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
		pVtx[1].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, ENEMY_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U, ENEMY_TEXTURE_UV_V);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//=========================================================================================================================
// �G�I������
//=========================================================================================================================
void UninitEnemy(void)
{
	int nCntTex;		// �e�N�X�`���J�E���^

	// �e�N�X�`���̔j��
	for(nCntTex = 0; nCntTex < MAX_ENEMY_TEXTURE; nCntTex++)
	{// �e�N�X�`���J�E���^
		if (g_apTextureEnemy[nCntTex] != NULL)
		{// NULL�ȊO�̏ꍇ
			g_apTextureEnemy[nCntTex]->Release();	// �J��
			g_apTextureEnemy[nCntTex] = NULL;		// NULL��
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffEnemy->Release();		// �J��
		g_pVtxBuffEnemy = NULL;			// NULL��
	}
}

//=========================================================================================================================
// �G�X�V����
//=========================================================================================================================
void UpdateEnemy(void)
{
	int nCntEnemy;						// �G�J�E���g
	int nExplosionSize = 40;			// �����T�C�Y

	Player *pPlayer = GetPlayer();		// �v���C���[�擾

	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffEnemy->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G���J�E���g
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// �G���g�p����Ă���ꍇ
			switch (g_aEnemy[nCntEnemy].State)
			{// �G���
			case ENEMYSTATE_NOMAL:	// �ʏ���
				break;
			case ENEMYSTATE_DAMAGE:	// �_���[�W���
				g_aEnemy[nCntEnemy].nCounterState--;	// ��Ԃ��J�E���g

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{// ��ԃJ�E���g�I��
					// �F�ύX
					g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// ��ԕω�
					g_aEnemy[nCntEnemy].State = ENEMYSTATE_NOMAL;
				}
				break;
			// case ENEMYSTATE_DEATH:
			// 	g_aEnemy[nCntEnemy].nCounterState--;	// ��Ԃ��J�E���g

			// 	// if (g_aEnemy[nCntEnemy].nCounterState <= 0)
			// 	// {// ��ԃJ�E���g�I��
			// 		if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
			// 		{
			// 			SetMode(MODE_RESULT);
			// 		}
			// 	}
			// 	return;
			// 	break;
			}

			if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_TUTORIALENEMY)
			{// ENEMYTYPE_TUTORIALENEMY�̏ꍇ		
				g_aEnemy[nCntEnemy].moveStart--;	// ���炷

				if (g_aEnemy[nCntEnemy].moveStart <= 0)
				{// �O�ȉ��ɂȂ�����
					if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_NORMAL)
					{// �ʏ�
						if (g_aEnemy[nCntEnemy].pos.x >= g_aEnemy[nCntEnemy].posTarget.x - 5 && g_aEnemy[nCntEnemy].pos.x <= g_aEnemy[nCntEnemy].posTarget.x + 5 && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].posTarget.y - 5 && g_aEnemy[nCntEnemy].pos.y <= g_aEnemy[nCntEnemy].posTarget.y + 5)
						{// �͈̓`�F�b�N
							g_aEnemy[nCntEnemy].bUse = false;
							g_nNumEnemy--;
						}
					}
				}
			
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;	// �ړ��ʑ��

				EnwmyBulletType(nCntEnemy);			// �e���ˎ��
			}
			else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_TUTORIALBOSSENEMY)
			{// ENEMYTYPE_TUTORIALBOSSENEMY�̏ꍇ
			 // �ړ��l���
			/*	g_aEnemy[nCntEnemy].moveStart--;

				if (g_aEnemy[nCntEnemy].moveStart <= 0)
				{
					if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_NORMAL)
					{

					}
				}

				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

				EnwmyBulletType(nCntEnemy);			// �e���ˎ��*/
			}
			else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_NORMALENEMY)
			{// ENEMYTYPE_NORMALENEMY�̏ꍇ
				// �ړ��l���
				g_aEnemy[nCntEnemy].moveStart--;	// ���炷

				if (g_aEnemy[nCntEnemy].moveStart <= 0)
				{// �O�ȉ���������
					if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_NORMAL)
					{// �ʏ�
						g_aEnemy[nCntEnemy].moveChange = 1;
					}
					else if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_HOMING)
					{// �z�[�~���O
						if (g_aEnemy[nCntEnemy].pos.x >= g_aEnemy[nCntEnemy].posTarget.x - 5 && g_aEnemy[nCntEnemy].pos.x <= g_aEnemy[nCntEnemy].posTarget.x + 5 && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].posTarget.y - 5 && g_aEnemy[nCntEnemy].pos.y <= g_aEnemy[nCntEnemy].posTarget.y + 5)
						{// �͈̓`�F�b�N
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0, 0, 0);
						}
						else
						{// �ړ�
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							g_aEnemy[nCntEnemy].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							if (g_aEnemy[nCntEnemy].moveLimit <= 100) { g_aEnemy[nCntEnemy].moveLimit++; }
						}
					}
					else if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_HOMING_KEEP)
					{// �z�[�~���O�ێ�
						if (g_aEnemy[nCntEnemy].pos.x >= g_aEnemy[nCntEnemy].posTarget.x - 5 && g_aEnemy[nCntEnemy].pos.x <= g_aEnemy[nCntEnemy].posTarget.x + 5 && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].posTarget.y - 5 && g_aEnemy[nCntEnemy].pos.y <= g_aEnemy[nCntEnemy].posTarget.y + 5)
						{// �͈̓`�F�b�N
							g_aEnemy[nCntEnemy].moveChange = 1;
						}
						else if (g_aEnemy[nCntEnemy].moveChange == 0)
						{// �ړ�
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							g_aEnemy[nCntEnemy].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							if (g_aEnemy[nCntEnemy].moveLimit <= 100) { g_aEnemy[nCntEnemy].moveLimit++; }
						}
					}
					else if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_HOMING_OUT)
					{// �z�[�~���O�A�E�g
						if (g_aEnemy[nCntEnemy].pos.x >= g_aEnemy[nCntEnemy].posTarget.x - 5 && g_aEnemy[nCntEnemy].pos.x <= g_aEnemy[nCntEnemy].posTarget.x + 5 && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].posTarget.y - 5 && g_aEnemy[nCntEnemy].pos.y <= g_aEnemy[nCntEnemy].posTarget.y + 5)
						{// �͈̓`�F�b�N
							g_aEnemy[nCntEnemy].moveChange++;
							
							if (g_aEnemy[nCntEnemy].moveChange > g_aEnemy[nCntEnemy].nCntTarget)
							{// ���ȏゾ������
								if (g_aEnemy[nCntEnemy].pos.x < SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4)
								{
									g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-1.5f, -1.0f, 0);
								}
								else if (g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4)
								{
									g_aEnemy[nCntEnemy].move = D3DXVECTOR3(1.5f, -1.0f, 0);
								}
								else if (g_aEnemy[nCntEnemy].pos.y < SCREEN_HEIGHT / 2)
								{
									g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0, -1.0f, 0);
								}
								else
								{
									g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0, 1.0f, 0);
								}
							}
							else
							{
								g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0, 0, 0);
							}
						}
						else if (g_aEnemy[nCntEnemy].moveChange == 0)
						{
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							g_aEnemy[nCntEnemy].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							if (g_aEnemy[nCntEnemy].moveLimit <= 100) { g_aEnemy[nCntEnemy].moveLimit++; }
						}
					}
				}
				
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;		// �ړ��ʑ��

				EnwmyBulletType(nCntEnemy);			// �e���ˎ��
			}
			else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_SBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_MBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
			{// ENEMYTYPE_LBOSSENEMY�̏ꍇ
					 // �ړ��l���
				g_aEnemy[nCntEnemy].moveStart--;	// ���炷

				if (g_aEnemy[nCntEnemy].moveStart <= 0)
				{// �O�ȉ���������
					if (g_aEnemy[nCntEnemy].moveType == ENEMYMOVE_HOMING)
					{// �z�[�~���O
						if (g_aEnemy[nCntEnemy].pos.x >= g_aEnemy[nCntEnemy].posTarget.x - 5 && g_aEnemy[nCntEnemy].pos.x <= g_aEnemy[nCntEnemy].posTarget.x + 5 && g_aEnemy[nCntEnemy].pos.y >= g_aEnemy[nCntEnemy].posTarget.y - 5 && g_aEnemy[nCntEnemy].pos.y <= g_aEnemy[nCntEnemy].posTarget.y + 5)
						{// �͈̓`�F�b�N
							g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0, 0, 0);
						}
						else
						{// �ړ�
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							g_aEnemy[nCntEnemy].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -g_aEnemy[nCntEnemy].posTarget.x, -g_aEnemy[nCntEnemy].pos.y - -g_aEnemy[nCntEnemy].posTarget.y) * (g_aEnemy[nCntEnemy].moveLimit * 0.01f)) * g_aEnemy[nCntEnemy].moveSpeed;
							if (g_aEnemy[nCntEnemy].moveLimit <= 100) { g_aEnemy[nCntEnemy].moveLimit++; }
						}
					}
				}

				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;	// �ړ��ʑ��

				if (g_aEnemy[nCntEnemy].move == D3DXVECTOR3(0, 0, 0))
				{// �ړ��͈�
					EnwmyBulletType(nCntEnemy);				// �e���ˎ��
				}
			}

			if (pPlayer->State != PLAYERSTATE_INVINCIBLE && pPlayer->bDisp == true)
			{// �v���C���[������ꍇ
				if (pPlayer->pos.x - 11 < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nSize && pPlayer->pos.x + 11 > g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nSize && pPlayer->pos.y - 17 < g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nSize && pPlayer->pos.y + 17 > g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nSize)
				{// �����蔻��ݒ�
					SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), (nExplosionSize - 20));	// ����
					HitPlayer(1);	// �q�b�g����
				}
			}
			
			if ((-300 > g_aEnemy[nCntEnemy].pos.x || SCREEN_WIDTH + 300 < g_aEnemy[nCntEnemy].pos.x || -300 > g_aEnemy[nCntEnemy].pos.y || SCREEN_HEIGHT + 300 < g_aEnemy[nCntEnemy].pos.y) && g_aEnemy[nCntEnemy].moveChange != 0)
			{// ��ʊO�̏ꍇ
				g_aEnemy[nCntEnemy].bUse = false;
				g_nNumEnemy--;
			}

			g_aEnemy[nCntEnemy].nCounterAnim++;		// �J�E���^�[���Z

			if ((g_aEnemy[nCntEnemy].nCounterAnim % ENEMY_TEXTURE_SPEED) == 0)
			{// �A�j���[�V�������x
				g_aEnemy[nCntEnemy].nPatternAnim = (g_aEnemy[nCntEnemy].nPatternAnim + 1) % ENEMY_TEXTURE_PATTERN;		// �p�^�[��No.�X�V

				// �e�N�X�`���ݒ�
				pVtx[nCntEnemy * 4].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
				pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U * g_aEnemy[nCntEnemy].nPatternAnim + ENEMY_TEXTURE_UV_U, 0.0f);
				pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U * g_aEnemy[nCntEnemy].nPatternAnim, ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U * g_aEnemy[nCntEnemy].nPatternAnim + ENEMY_TEXTURE_UV_U, ENEMY_TEXTURE_UV_V);
			}

			// ���_�ݒ�
			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nSize, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntEnemy * 4].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 1].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 2].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 3].col = g_aEnemy[nCntEnemy].col;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();
}

//=========================================================================================================================
// �`�揈��
//=========================================================================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntEnemy;		// �G�J�E���g

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffEnemy,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G���J�E���g
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// �G������ꍇ
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=========================================================================================================================
// �G�̐ݒ菈��
//=========================================================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, D3DXVECTOR3 move, float moveSpeed, ENEMYMOVE moveType, int moveStart, int nCntTarget, int nType, int nSize, int nLife, ENEMYTYPE Type,
	EnemyBullet bullet1, EnemyBullet bullet2, EnemyBullet bullet3, EnemyBullet bullet4, EnemyBullet bullet5)
{
	int nCntEnemy;				// �G�J�E���g

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffEnemy->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G���J�E���g
		if (g_aEnemy[nCntEnemy].bUse == false)
		{// �G���g�p����Ă��Ȃ�
			g_aEnemy[nCntEnemy].pos = pos;									// �ʒu�ݒ�
			g_aEnemy[nCntEnemy].posTarget = posTarget;						// �ڕW�ʒu�ݒ�
			g_aEnemy[nCntEnemy].move = move;								// �ړ��l�ݒ�
			g_aEnemy[nCntEnemy].moveSpeed = moveSpeed;						// �ړ����x�ݒ�
			g_aEnemy[nCntEnemy].moveType = moveType;						// �ړ���ސݒ�
			g_aEnemy[nCntEnemy].moveStart = moveStart;						// �ړ��J�n���Ԑݒ�
			g_aEnemy[nCntEnemy].moveLimit = 0;								// �ړ��񐔐ݒ�
			g_aEnemy[nCntEnemy].moveChange = 0;								// �ړ��ؑ֐ݒ�
			g_aEnemy[nCntEnemy].nCntTarget = nCntTarget;					// �ڕW�ʒu�񐔐ݒ�
			g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
			g_aEnemy[nCntEnemy].nType = nType;								// �e�N�X�`����ސݒ�
			g_aEnemy[nCntEnemy].nSize = nSize;								// �傫���ݒ�
			g_aEnemy[nCntEnemy].nLife = nLife;								// �̗͐ݒ�
			g_aEnemy[nCntEnemy].Type = Type;								// �G��ނ�ݒ�

			g_aEnemyBullet[nCntEnemy][0].nPattenType = bullet1.nPattenType;	// �e��ނ�ݒ�
			g_aEnemyBullet[nCntEnemy][0].moveSpeed = bullet1.moveSpeed;		// �e�ړ����x�ݒ�
			g_aEnemyBullet[nCntEnemy][0].nTiming = bullet1.nTiming;			// ���˃^�C�~���O��ݒ�

			g_aEnemyBullet[nCntEnemy][1].nPattenType = bullet2.nPattenType;	// �e��ނ�ݒ�
			g_aEnemyBullet[nCntEnemy][1].moveSpeed = bullet2.moveSpeed;		// �e�ړ����x�ݒ�
			g_aEnemyBullet[nCntEnemy][1].nTiming = bullet2.nTiming;			// ���˃^�C�~���O��ݒ�

			g_aEnemyBullet[nCntEnemy][2].nPattenType = bullet3.nPattenType;	// �e��ނ�ݒ�
			g_aEnemyBullet[nCntEnemy][2].moveSpeed = bullet3.moveSpeed;		// �e�ړ����x�ݒ�
			g_aEnemyBullet[nCntEnemy][2].nTiming = bullet3.nTiming;			// ���˃^�C�~���O��ݒ�

			g_aEnemyBullet[nCntEnemy][3].nPattenType = bullet4.nPattenType;	// �e��ނ�ݒ�
			g_aEnemyBullet[nCntEnemy][3].moveSpeed = bullet4.moveSpeed;		// �e�ړ����x�ݒ�
			g_aEnemyBullet[nCntEnemy][3].nTiming = bullet4.nTiming;			// ���˃^�C�~���O��ݒ�

			g_aEnemyBullet[nCntEnemy][4].nPattenType = bullet5.nPattenType;	// �e��ނ�ݒ�
			g_aEnemyBullet[nCntEnemy][4].moveSpeed = bullet5.moveSpeed;		// �e�ړ����x�ݒ�
			g_aEnemyBullet[nCntEnemy][4].nTiming = bullet5.nTiming;			// ���˃^�C�~���O��ݒ�

			g_aEnemy[nCntEnemy].State = ENEMYSTATE_NOMAL;					// ��Ԃ�ݒ�
			g_aEnemy[nCntEnemy].nCounterState = 0;							// ��ԃJ�E���^�����l�ݒ�
			g_aEnemy[nCntEnemy].nCounterAnim = 0;							// �J�E���^�[�����l�ݒ�
			g_aEnemy[nCntEnemy].nPatternAnim = 0;							// �p�^�[��No.�����l�ݒ�
			g_aEnemy[nCntEnemy].bUse = true;								// �g�p���Ă����Ԃɂ���

			for (int nCount = 0; nCount < MAX_ENEMYBULLET_TYPE; nCount++)
			{
				g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f);
			}

			// ���_�ݒ�
			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nSize, 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].nSize, g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].nSize, 0.0f);

			g_nNumEnemy++;	// �G�������J�E���g
			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();
}

//=========================================================================================================================
// �G�̎擾
//=========================================================================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];		// �l��Ԃ�
}

//=========================================================================================================================
// �G�̎擾
//=========================================================================================================================
EnemyBullet *GetEnemyBullet(void)
{
	return &g_aEnemyBullet[0][0];	// �l��Ԃ�
}

//=========================================================================================================================
// �G�̒e�����菈��
//=========================================================================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	// �ϐ��錾
	int ExplosionSize1 = 0;		// nLife��0�ȉ��̔����T�C�Y
	int ExplosionSize2 = 0;		// nLife��1�ȏ�̔����T�C�Y

	if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_NORMALENEMY)
	{// ENEMYTYPE_NORMALENEMY�̔���
		ExplosionSize1 = 30;
		ExplosionSize2 = 20;
	}
	else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
	{// ENEMYTYPE_LBOSSENEMY�̔����T�C�Y
		ExplosionSize1 = 100;
		ExplosionSize2 = 0;
	}

	// ���C�t������
	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{// ���C�t��0�ȉ��ɂȂ����ꍇ
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f), ExplosionSize1);	// ����

		EnwmyItemSet(nCntEnemy);						// �A�C�e���ݒ�

		g_aEnemy[nCntEnemy].bUse = false;				// �g�p���ĂȂ����
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_DEATH;	// ��ԕω�
		g_aEnemy[nCntEnemy].nCounterState = 60;			// ��ԃJ�E���^
		g_nNumEnemy--;									// �G����������

		PlaySound(SOUND_LABEL_SE_EXPLOSION);			// ����

		// �X�R�A�ݒ�
		if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_NORMALENEMY)
		{// ENEMYTYPE_NORMALENEMY�̏ꍇ
			AddScore(SCORETYPE_RNKING_NO1, (g_aEnemy[nCntEnemy].nType + 1) * 1000);
			SetParticleEffect(g_aEnemy[nCntEnemy].pos, EFFECTTYPE_EXPLOSION, 50, 30, 30);			// �G�t�F�N�g���Z�b�g
		}
		else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_SBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_MBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
		{// ENEMYTYPE_LBOSSENEMY�̏ꍇ
			if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_SBOSSENEMY) { AddScore(SCORETYPE_RNKING_NO1, (g_aEnemy[nCntEnemy].nType + 1) * 10000); }
			if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_MBOSSENEMY) { AddScore(SCORETYPE_RNKING_NO1, (g_aEnemy[nCntEnemy].nType + 1) * 15000); }
			if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY) { AddScore(SCORETYPE_RNKING_NO1, (g_aEnemy[nCntEnemy].nType + 1) * 20000); }

			SetParticleEffect(g_aEnemy[nCntEnemy].pos, EFFECTTYPE_EXPLOSION, 100, 50, 100);			// �G�t�F�N�g���Z�b�g
		}
	}
	else
	{// ���C�t��1�ȏ�̏ꍇ
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_DAMAGE;		// ��ԕω�
		g_aEnemy[nCntEnemy].nCounterState = 10;				// ��ԃJ�E���^

		PlaySound(SOUND_LABEL_SE_HIT000);					// �q�b�g��

		// �F�ύX
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 0.1f, 0.0f, 1.0f);
	}
}

//=========================================================================================================================
// �G�̑����擾
//=========================================================================================================================
int GetNumEnemy(void)
{
	return g_nNumEnemy;		// �l��Ԃ�
}

//==================================================================================================
// �G�̒e���ˎ��
//==================================================================================================
void EnwmyBulletType(int nCntEnemy)
{
	int nCount;

	Player *pPlayer = GetPlayer();

	for (nCount = 0; nCount < MAX_ENEMYBULLET_TYPE; nCount++)
	{
		if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType != ENEMYBULLET_NONE)
		{
			if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_TUTORIALENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_NORMALENEMY)
			{// ENEMYTYPE_NORMALENEMY�̏ꍇ
				// �G�e���x
				g_aEnemyBullet[nCntEnemy][nCount].nCounterInput++;
				if ((g_aEnemyBullet[nCntEnemy][nCount].nCounterInput % g_aEnemyBullet[nCntEnemy][nCount].nTiming) == 0)
				{
					PlaySound(SOUND_LABEL_SE_SHOT001);		// ����

					// �G�e���ˏ���
					if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_1)
					{// �^�C�v�P �O
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_2)
					{// �^�C�v�Q �v���C���[�p�x
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_3)
					{// �^�C�v�R �����v��]
						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE * 3;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_4)
					{// �^�C�v�S �����v��] * 4
						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE * 3;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_5)
					{// �^�C�v�T �O�g
						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_6)
					{// �^�C�v�U �O�R����
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * SINCOS_ANGLE_45) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * SINCOS_ANGLE_45) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * SINCOS_ANGLE_45) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * SINCOS_ANGLE_45) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_7)
					{// �^�C�v�V �O�v���C���[�p�x�R����
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y) + (D3DX_PI * SINCOS_ANGLE_45)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y) + (D3DX_PI * SINCOS_ANGLE_45)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 60, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 60, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y) - (D3DX_PI * SINCOS_ANGLE_45)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(atan2f(-g_aEnemy[nCntEnemy].pos.x - -pPlayer->pos.x, -g_aEnemy[nCntEnemy].pos.y - -pPlayer->pos.y) - (D3DX_PI * SINCOS_ANGLE_45)) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 60, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_8)
					{// �^�C�v
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_9)
					{// �^�C�v
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_10)
					{// �^�C�v
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_11)
					{// �^�C�v
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_12)
					{// �^�C�v
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_13)
					{// �^�C�v
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_14)
					{// �^�C�v
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
				}
			}
			else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_SBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_MBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
			{// ENEMYTYPE_LBOSSENEMY�̏ꍇ
				g_aEnemyBullet[nCntEnemy][nCount].nCounterInput++;	// �J�E���^�����Z

				if ((g_aEnemyBullet[nCntEnemy][nCount].nCounterInput % g_aEnemyBullet[nCntEnemy][nCount].nTiming) == 0)
				{
					PlaySound(SOUND_LABEL_SE_SHOT001);		// ����

					if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_1)
					{// �^�C�v�P ���ꂢ��
						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;	// �G�e�p�x���

						// �E
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.0f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.0f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);

						// ��
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.0f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.0f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_2)
					{// �^�C�v�Q �ǔ�
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(0.5f, 0.8f, 1.0f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_HOMING, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_3)
					{// �^�C�v�R �W����
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(0.5f, 1.0f, 0.3f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);

					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_4)
					{// �^�C�v�S �O�g���i
						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_STRAIGHT, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 50, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_STRAIGHT, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 50, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_5)
					{// �^�C�v�T 3�{��] * 2�i�t��]�j
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.33f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.333f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.33f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.33f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);

						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(0.5f, 0.8f, 1.0f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0 + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0 + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.66f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.66f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.66f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.66f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_NORMAL, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 0, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_6)
					{// �^�C�v�U �ǔ�
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.25f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.75f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_7)
					{// �^�C�v�V �ǔ�
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(0.5f, 0.8f, 1.0f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.125f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.375f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.625f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.875f) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_MOVECHANGE_START_2, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, g_aEnemy[nCntEnemy].nSize, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_8)
					{// �^�C�v�W
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(1.0f, 0.3f, 0.1f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0 + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0 + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI + g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
					}
					else if (g_aEnemyBullet[nCntEnemy][nCount].nPattenType == ENEMYBULLET_NO_9)
					{// �^�C�v�X
						g_aEnemyBullet[nCntEnemy][nCount].col = D3DXCOLOR(0.5f, 0.8f, 1.0f, 1.0f);

						g_aEnemyBullet[nCntEnemy][nCount].nAngle += ENEMYBULLET_ANGLE;

						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0 - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI * 0.5f - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
						g_aEnemyBullet[nCntEnemy][nCount].move.x = sinf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						g_aEnemyBullet[nCntEnemy][nCount].move.y = cosf(-D3DX_PI - g_aEnemyBullet[nCntEnemy][nCount].nAngle) * g_aEnemyBullet[nCntEnemy][nCount].moveSpeed;
						SetBullet(g_aEnemy[nCntEnemy].pos, g_aEnemyBullet[nCntEnemy][nCount].move, g_aEnemyBullet[nCntEnemy][nCount].col, g_aEnemyBullet[nCntEnemy][nCount].Type, BULLETMOVE_SETBULLET_1, g_aEnemyBullet[nCntEnemy][nCount].moveSpeed, 80, 1);
					}
				}
			}
		}
	}
}

//==================================================================================================
// �G�̃A�C�e���ݒ�
//==================================================================================================
void EnwmyItemSet(int nCntEnemy)
{
	if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_TUTORIALENEMY)
	{// ENEMYTYPE_NORMALENEMY
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);
	}
	else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_NORMALENEMY)
	{// ENEMYTYPE_NORMALENEMY
		if (rand() % 5 == 0) { SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER); }
		if (rand() % 5 == 0) { SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE); }
		if (rand() % 5 == 0) { SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR); }
	}
	else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_SBOSSENEMY || g_aEnemy[nCntEnemy].Type == ENEMYTYPE_MBOSSENEMY)
	{// ENEMYTYPE_SBOSSENEMY�܂���ENEMYTYPE_MBOSSENEMY

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);
	}
	else if (g_aEnemy[nCntEnemy].Type == ENEMYTYPE_LBOSSENEMY)
	{// ENEMYTYPE_LBOSSENEMY
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);

		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_POWER);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_LIFE);
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_STAR);
	}
}