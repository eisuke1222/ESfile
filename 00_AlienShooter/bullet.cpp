//=========================================================================================================================
// 
// �e���� [bullet.cpp]
// Author : Einsuke Sakagawa
// 
//=========================================================================================================================

//**********************************************************************************************************************
// �C���N���[�h�t�@�C��
//**********************************************************************************************************************
#include "bullet.h"		// �e
#include "explosion.h"	// ����
#include "effect.h"		// �G�t�F�N�g
#include "score.h"		// �X�R�A
#include "player.h"		// �v���C���[
#include "enemy.h"		// �G

//**********************************************************************************************************************
// �}�N����`
//**********************************************************************************************************************
#define	MAX_BULLET				(1080)									// �e�̍ő吔
#define	MAX_BULLET_TEXTURE		(2)										// �e�N�X�`���̍ő吔
#define BULLET_TEXTURE_NAME1	"data\\TEXTURE\\bullet003.png"			// �ǂݍ��ރe�N�X�`���t�@�C��1
#define BULLET_TEXTURE_NAME2	"data\\TEXTURE\\bullet002.png"			// �ǂݍ��ރe�N�X�`���t�@�C��2
#define BULLET_TEXTURE_UV_U		(1.0f)									// �e�A�j���[�V����U�͈�
#define BULLET_TEXTURE_UV_V		(1.0f)									// �e�A�j���[�V����V�͈�

//**********************************************************************************************************************
// �O���[�o���ϐ��錾
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureBullet[MAX_BULLET_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffBullet = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
Bullet							g_aBullet[MAX_BULLET];						// �e�̏��

//=========================================================================================================================
// �e����������
//=========================================================================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntBullet;		// �e�J�E���g

	// �e�̏��̏�����
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// �e���J�E���g
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �e���S�l
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �e�ړ��l
		g_aBullet[nCntBullet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �e�F
		g_aBullet[nCntBullet].Type = BULLETTYPE_PLAYER1;				// �e���
		g_aBullet[nCntBullet].moveType = BULLETMOVE_NORMAL;				// �e�ړ����
		g_aBullet[nCntBullet].nCntChange = 0;							// �e�ړ��ύX
		g_aBullet[nCntBullet].nCntPoint= 0;								// �e�ʒu��
		g_aBullet[nCntBullet].bUse = false;								// �e�g�p���Ă��Ȃ����
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(// �v���C���[
		pDevice,
		BULLET_TEXTURE_NAME1,
		&g_pTextureBullet[0]
	);
	D3DXCreateTextureFromFile
	(// �G
		pDevice,
		BULLET_TEXTURE_NAME2,
		&g_pTextureBullet[1]
	);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// �e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// �e���J�E���g
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
		pVtx[1].tex = D3DXVECTOR2(BULLET_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, BULLET_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(BULLET_TEXTURE_UV_U, BULLET_TEXTURE_UV_V);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//=========================================================================================================================
// �e�I������
//=========================================================================================================================
void UninitBullet(void)
{
	// �ϐ��錾
	int nCntTex;

	// �e�N�X�`���[�̔j��
	for (nCntTex = 0; nCntTex < MAX_BULLET_TEXTURE; nCntTex++)
	{// �e�N�X�`�����J�E���g
		if (g_pTextureBullet[nCntTex] != NULL)
		{// NULL�ȊO�̏ꍇ
			g_pTextureBullet[nCntTex]->Release();	// �J��
			g_pTextureBullet[nCntTex] = NULL;		// NULL��
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet!= NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffBullet->Release();	// �J��
		g_pVtxBuffBullet = NULL;		// NULL��
	}
}

//=========================================================================================================================
// �e�X�V����
//=========================================================================================================================
void UpdateBullet(void)
{
	int nCntBullet;					// �e�J�E���g
	int nExplosionSize = 30;		// �����T�C�Y

	VERTEX_2D *pVtx;				// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// �e���J�E���g
		if (g_aBullet[nCntBullet].bUse == true)
		{// �e���g�p����Ă���ꍇ
			// �ړ��l���
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;
		//	SetBulletEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].col, EFFECTTYPE_BULLET, 10.0f, 10);		// �G�t�F�N�g���Z�b�g

			if (g_aBullet[nCntBullet].Type == BULLETTYPE_PLAYER1 || g_aBullet[nCntBullet].Type == BULLETTYPE_PLAYER2 || g_aBullet[nCntBullet].Type == BULLETTYPE_PLAYER3)
			{// �v���C���[�̒e
				int nCntEnemy;					// �G�J�E���g
				Enemy *pEnemy = GetEnemy();		// �G�̎擾

				if (g_aBullet[nCntBullet].moveType == BULLETMOVE_MOVECHANGE_STRAIGHT)
				{// �܂�����
					g_aBullet[nCntBullet].nCntChange++;		// �J�E���g

					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// �w�肵���l�Ɠ�����������
						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI) * g_aBullet[nCntBullet].fSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI) * g_aBullet[nCntBullet].fSpeed;
					}
				}

				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{// �G���J�E���g
					if (pEnemy->bUse == true)
					{// �G������ꍇ
						if (pEnemy->Type == ENEMYTYPE_TUTORIALENEMY || pEnemy->Type == ENEMYTYPE_NORMALENEMY)
						{// ENEMYTYPE_TUTORIALENEMY, ENEMYTYPE_NORMALENEMY�̏ꍇ
							if (pEnemy->pos.x - pEnemy->nSize < g_aBullet[nCntBullet].pos.x && pEnemy->pos.x + pEnemy->nSize > g_aBullet[nCntBullet].pos.x && pEnemy->pos.y - pEnemy->nSize < g_aBullet[nCntBullet].pos.y && pEnemy->pos.y + pEnemy->nSize > g_aBullet[nCntBullet].pos.y)
							{// �����蔻��ݒ�
								SetParticleEffect(g_aBullet[nCntBullet].pos, EFFECTTYPE_EXPLOSION, 50, 8, 20);		// �G�t�F�N�g���Z�b�g

								// �G�̗͌���
								g_aBullet[nCntBullet].bUse = false;		// �g�p���Ă��Ȃ����
								HitEnemy(nCntEnemy, 1);					// �q�b�g����
								AddScore(SCORETYPE_RNKING_NO1, 1);		// �X�R�A����
							}
						}
						else if ((pEnemy->Type == ENEMYTYPE_TUTORIALBOSSENEMY || pEnemy->Type == ENEMYTYPE_SBOSSENEMY || pEnemy->Type == ENEMYTYPE_MBOSSENEMY || pEnemy->Type == ENEMYTYPE_LBOSSENEMY) && pEnemy->move.y == 0)
						{// ENEMYTYPE_BOSSENEMY�̏ꍇ
							if (pEnemy->pos.x - pEnemy->nSize < g_aBullet[nCntBullet].pos.x && pEnemy->pos.x + pEnemy->nSize > g_aBullet[nCntBullet].pos.x && pEnemy->pos.y - pEnemy->nSize < g_aBullet[nCntBullet].pos.y && pEnemy->pos.y + pEnemy->nSize > g_aBullet[nCntBullet].pos.y)
							{// �����蔻��ݒ�
								SetParticleEffect(g_aBullet[nCntBullet].pos, EFFECTTYPE_EXPLOSION, 50, 12, 30);	// �G�t�F�N�g���Z�b�g

								// �G�̗͌���
								g_aBullet[nCntBullet].bUse = false;		// �g�p���Ă��Ȃ����
								HitEnemy(nCntEnemy, 1);					// �q�b�g����
								AddScore(SCORETYPE_RNKING_NO1, 1);		// �X�R�A����
							}
						}
					}
				}
			}
			else if (g_aBullet[nCntBullet].Type == BULLETTYPE_ENEMY || g_aBullet[nCntBullet].Type == BULLETTYPE_BOSSENEMY)
			{// �G�̒e
				Player *pPlayer = GetPlayer();	// �v���C���[�擾

				if (g_aBullet[nCntBullet].moveType == BULLETMOVE_MOVECHANGE_HOMING)
				{// �z�[�~���O
					g_aBullet[nCntBullet].nCntChange++;		// �J�E���g

					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint - 30)
					{// �w�肵���l�Ɠ�����������
						g_aBullet[nCntBullet].move = D3DXVECTOR3(0, 0, 0);
					}
					else if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// �w�肵���l�Ɠ�����������
						g_aBullet[nCntBullet].move.x = sinf(atan2f(-g_aBullet[nCntBullet].pos.x - -pPlayer->pos.x, -g_aBullet[nCntBullet].pos.y - -pPlayer->pos.y)) * g_aBullet[nCntBullet].fSpeed;
						g_aBullet[nCntBullet].move.y = cosf(atan2f(-g_aBullet[nCntBullet].pos.x - -pPlayer->pos.x, -g_aBullet[nCntBullet].pos.y - -pPlayer->pos.y)) * g_aBullet[nCntBullet].fSpeed;
					}
				}
				else if (g_aBullet[nCntBullet].moveType == BULLETMOVE_MOVECHANGE_STRAIGHT)
				{// �܂�����
					g_aBullet[nCntBullet].nCntChange++;		// �J�E���g

					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// �w�肵���l�Ɠ�����������
						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI * 0) * g_aBullet[nCntBullet].fSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI * 0) * g_aBullet[nCntBullet].fSpeed;
					}
				}
				else if (g_aBullet[nCntBullet].moveType == BULLETMOVE_MOVECHANGE_START_1 || g_aBullet[nCntBullet].moveType == BULLETMOVE_MOVECHANGE_START_2)
				{// �X�^�[�g
					g_aBullet[nCntBullet].nCntChange++;		// �J�E���g

 					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// �w�肵���l�Ɠ�����������
						//g_aBullet[nCntBullet].move = D3DXVECTOR3(0, 0, 0);
					}
					else if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint + 30)
					{// �w�肵���l�Ɠ�����������
						g_aBullet[nCntBullet].move.x *= -1;
						g_aBullet[nCntBullet].move.y *= -1;
					}
				}
				else if (g_aBullet[nCntBullet].moveType == BULLETMOVE_SETBULLET)
				{// �e�ǉ�
					g_aBullet[nCntBullet].nCntChange++;		// �J�E���g

					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// �w�肵���l�Ɠ�����������
						EnemyBullet *pEnemyBullet = GetEnemyBullet();

						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI * 0.25f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI * 0.25f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.8f, 1.0f, 0.f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI * 0.75f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI * 0.75f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.8f, 1.0f, 0.1f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(-D3DX_PI * 0.25f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(-D3DX_PI * 0.25f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.8f, 1.0f, 0.1f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(-D3DX_PI * 0.75f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(-D3DX_PI * 0.75f + pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.8f, 1.0f, 0.1f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
					}
				}
				else if (g_aBullet[nCntBullet].moveType == BULLETMOVE_SETBULLET_1)
				{// �e�ǉ�1
					g_aBullet[nCntBullet].nCntChange++;		// �J�E���g

					if (g_aBullet[nCntBullet].nCntChange == g_aBullet[nCntBullet].nCntPoint)
					{// �w�肵���l�Ɠ�����������
						EnemyBullet *pEnemyBullet = GetEnemyBullet();

						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI * 0.25f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI * 0.25f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.1f, 1.0f, 0.8f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(D3DX_PI * 0.75f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(D3DX_PI * 0.75f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.1f, 1.0f, 0.8f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(-D3DX_PI * 0.25f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(-D3DX_PI * 0.25f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.1f, 1.0f, 0.8f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
						g_aBullet[nCntBullet].move.x = sinf(-D3DX_PI * 0.75f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						g_aBullet[nCntBullet].move.y = cosf(-D3DX_PI * 0.75f - pEnemyBullet->nAngle) * pEnemyBullet->moveSpeed;
						SetBullet(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].move, D3DXCOLOR(0.1f, 1.0f, 0.8f, 1.0f), BULLETTYPE_BOSSENEMY, BULLETMOVE_NORMAL, 0, 0, 1);
					}
				}

				if (pPlayer->State != PLAYERSTATE_INVINCIBLE && pPlayer->bDisp == true)
				{// �v���C���[������ꍇ
					if (pPlayer->pos.x - 8 < g_aBullet[nCntBullet].pos.x && pPlayer->pos.x + 8 > g_aBullet[nCntBullet].pos.x && pPlayer->pos.y - 8 < g_aBullet[nCntBullet].pos.y && pPlayer->pos.y + 8 > g_aBullet[nCntBullet].pos.y)
					{// �����蔻��ݒ�
						HitPlayer(1);	// �q�b�g����
						g_aBullet[nCntBullet].bUse = false;		// �g�p���Ă��Ȃ����
					}
				}
			}

			if (g_aBullet[nCntBullet].pos.y < -200 || g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT + 200 || g_aBullet[nCntBullet].pos.x < -200 || g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH + 200)
			{// ��ʊO�̏ꍇ
				g_aBullet[nCntBullet].bUse = false;		// �g�p���Ă��Ȃ����
			}

			// ���_�ݒ�
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 7, g_aBullet[nCntBullet].pos.y - 7, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 7, g_aBullet[nCntBullet].pos.y - 7, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 7, g_aBullet[nCntBullet].pos.y + 7, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 7, g_aBullet[nCntBullet].pos.y + 7, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntBullet * 4].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 1].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 2].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 3].col = g_aBullet[nCntBullet].col;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//=========================================================================================================================
// �e�`�揈��
//=========================================================================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntBullet;		// �e�J�E���g

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBullet, 
		0, 
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// �e���J�E���g
		if (g_aBullet[nCntBullet].bUse == true)
		{// �e������ꍇ
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].nTexNum]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//=========================================================================================================================
// �e�̐ݒ菈��
//=========================================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE Type, BULLETMOVE moveType, float fSpeed, int nCntPoint, int nTexNum)
{
	int nCntBullet;			// �e�J�E���g

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// �e���J�E���g
		if (g_aBullet[nCntBullet].bUse == false)
		{// �e���g�p����Ă��Ȃ��ꍇ
			g_aBullet[nCntBullet].pos			= pos;			// �ʒu�ݒ�
			g_aBullet[nCntBullet].move			= move;			// �ړ��l�ݒ�
			g_aBullet[nCntBullet].col			= col;			// �F�ݒ�
			g_aBullet[nCntBullet].Type			= Type;			// ��ސݒ�
			g_aBullet[nCntBullet].moveType		= moveType;		// �ړ���ސݒ�
			g_aBullet[nCntBullet].fSpeed		= fSpeed;		// �ړ����x�ݒ�
			g_aBullet[nCntBullet].nCntChange	= 0;			// �ړ��ύX�ݒ�
			g_aBullet[nCntBullet].nCntPoint		= nCntPoint;	// �ʒu�񐔐ݒ�
			g_aBullet[nCntBullet].nTexNum		= nTexNum;		// �e�N�X�`���ԍ��ݒ�
			g_aBullet[nCntBullet].bUse			= true;			// �g�p���Ă�����

			// ���_�ݒ�
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 7, g_aBullet[nCntBullet].pos.y - 7, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 7, g_aBullet[nCntBullet].pos.y - 7, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - 7, g_aBullet[nCntBullet].pos.y + 7, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + 7, g_aBullet[nCntBullet].pos.y + 7, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntBullet * 4].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 1].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 2].col = g_aBullet[nCntBullet].col;
			pVtx[nCntBullet * 4 + 3].col = g_aBullet[nCntBullet].col;

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}