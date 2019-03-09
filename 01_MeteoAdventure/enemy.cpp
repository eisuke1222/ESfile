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
#include "score.h"			// �X�R�A
#include "player.h"			// �v���C���[
#include "block.h"			// �u���b�N
#include "bullet.h"			// �e
#include "sound.h"			// �T�E���h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define ENEMY_TEXTURE_SPEED				(9)									// �e�N�X�`���A�j���[�V�����X�s�[�h
#define ENEMY_TEXTURE_PATTERN			(2)									// �e�N�X�`���A�j���[�V�����p�^�[����
#define ENEMY_TEXTURE_UV_U				(0.2f)								// �e�N�X�`���A�j���[�V����U�͈�
#define ENEMY_TEXTURE_UV_V				(0.33f)								// �e�N�X�`���A�j���[�V����V�͈�
#define ENEMY_UPSPEED					(1.2f)								// �v���C���[�������x
#define ENEMY_DOWNSPEED					(0.2f)								// �v���C���[�������x
#define ENEMY_JUMPSPEED					(1.2f)								// �v���C���[�㏸���x
#define ENEMY_GRAVITY					(0.5f)								// �v���C���[�d��

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void EnemyCollisionRange(int nCntEnemy, D3DXVECTOR3 *pRange);					// �Փ˔͈͐ݒ�

//*************************************************************************************************************************
// �O���[�o���ϐ��錾
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_apTextureEnemy[ENEMYTEXTURE_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffEnemy = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
Enemy						g_aEnemy[MAX_ENEMY];							// �G�̏��
int							g_nNumEnemy;									// �G�̑���

//=========================================================================================================================
// �G����������
//=========================================================================================================================
void InitEnemy(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�̎擾
	int nCntEnemy;		// �G�J�E���^

	// �G�̏��̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G���J�E���g
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���S�l
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���S�l
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �ړ��l
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
		g_aEnemy[nCntEnemy].fWidth = 0.0f;									// ����
		g_aEnemy[nCntEnemy].fHeight = 0.0f;									// ����
		g_aEnemy[nCntEnemy].nLife = 0;										// �̗�
		g_aEnemy[nCntEnemy].Type = ENEMYTYPE_NORMALENEMY;					// ���
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_APPEAR;						// ���
		g_aEnemy[nCntEnemy].nCounterState = 0;								// ��ԃJ�E���^
		g_aEnemy[nCntEnemy].AnimType = ENEMYANIMTYPE_NORMAL;				// �A�j���[�V�������
		g_aEnemy[nCntEnemy].nCounterAnim = 0;								// �A�j���[�V�����J�E���^
		g_aEnemy[nCntEnemy].nNumAnim = 0;									// �A�j���[�V�����ԍ�
		g_aEnemy[nCntEnemy].nSpeedAnim = 0;									// �A�j���[�V�������x
		g_aEnemy[nCntEnemy].nPatternAnim = 0;								// �A�j���[�V�����p�^�[��
		g_aEnemy[nCntEnemy].nDinectionMove = 0;								// ����
		g_aEnemy[nCntEnemy].nCntTimer = 0;									// ���ԃJ�E���^
		g_aEnemy[nCntEnemy].texture = ENEMYTEXTURE_SLIME;					// �e�N�X�`��
		g_aEnemy[nCntEnemy].bUse = false;									// �g�p���Ă��邩
	}

	g_nNumEnemy = 0;					// �G����

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_NAME_1, &g_apTextureEnemy[0]);		// �X���C��
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_NAME_2, &g_apTextureEnemy[1]);		// �Ⴞ���
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_NAME_3, &g_apTextureEnemy[2]);		// �̂낢
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_NAME_4, &g_apTextureEnemy[3]);		// ��
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_NAME_5, &g_apTextureEnemy[4]);		// �S�[�X�g

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
	// �ϐ��錾
	int nCntTex;		// �e�N�X�`���J�E���^

	// �e�N�X�`���̔j��
	for(nCntTex = 0; nCntTex < ENEMYTEXTURE_MAX; nCntTex++)
	{// �e�N�X�`�����J�E���g
		if (g_apTextureEnemy[nCntTex] != NULL)
		{
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=========================================================================================================================
// �G�X�V����
//=========================================================================================================================
void UpdateEnemy(void)
{
	// �ϐ��錾
	int nCntEnemy;						// �G�J�E���g
	PLAYER *pPlayer = GetPlayer();		// �v���C���[���
	D3DXVECTOR3 Range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����蔻��
	MODE mode = GetMode();

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
		{// �g�p����Ă���ꍇ
			switch (g_aEnemy[nCntEnemy].State)
			{
			case ENEMYSTATE_NORMAL:
				break;
			case ENEMYSTATE_DAMAGE:

				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;
					g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				break;
			case ENEMYSTATE_DEATH:
				break;
			}

			g_aEnemy[nCntEnemy].nCounterAnim++;		// �J�E���^��i�߂�

			if (g_aEnemy[nCntEnemy].pos.x >= pPlayer->pos.x)
			{// ������
				g_aEnemy[nCntEnemy].nDinectionMove = 1;
			}
			else
			{// �E����
				g_aEnemy[nCntEnemy].nDinectionMove = 0;
			}

			// �ʒu���L��
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;
			// ��ʃX�N���[��
			g_aEnemy[nCntEnemy].pos.x -= pPlayer->move.x / 2;
			
			if (g_aEnemy[nCntEnemy].texture == ENEMYTEXTURE_BIRD)
			{
				// �d�͉��Z
				g_aEnemy[nCntEnemy].move.y += pPlayer->fGravity / 2;
			}
			else if (g_aEnemy[nCntEnemy].texture != ENEMYTEXTURE_GHOST)
			{
				// �d�͉��Z
				g_aEnemy[nCntEnemy].move.y += pPlayer->fGravity;
			}
		
			// �ړ��l���
			g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
			g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;
			// ��������
			g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * ENEMY_DOWNSPEED;

			if (g_aEnemy[nCntEnemy].texture != ENEMYTEXTURE_GHOST)
			{
				// �u���b�N�Ƃ̏Փˏ���
				CollisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].fHeight, NULL, COLLISIONBLOCKRETURN_ENEMY);
			}

			if (mode == MODE_GAME)
			{// �Փ˔��蒲��
				EnemyCollisionRange(nCntEnemy, &Range);

				if (CollisiomBullet(&g_aEnemy[nCntEnemy].pos, Range.x, Range.y, COLLISIONBULLETRETURN_OTHER) == true && g_aEnemy[nCntEnemy].AnimType != ENEMYANIMTYPE_KNOCKBACK)
				{// �e�ɓ��������ꍇ
					g_aEnemy[nCntEnemy].nLife--;		// �̗͂����炷

					g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 0.5f, 0.3f, 1.0f);
					g_aEnemy[nCntEnemy].nCounterState = 15;
					g_aEnemy[nCntEnemy].State = ENEMYSTATE_DAMAGE;

					PlaySound(SOUND_LABEL_SE_HIT000);
					if (g_aEnemy[nCntEnemy].nLife <= 0)
					{// �̗͂�0�ȉ��̏ꍇ
						// �m�b�N�o�b�N
						g_aEnemy[nCntEnemy].nNumAnim = 0;
						g_aEnemy[nCntEnemy].AnimType = ENEMYANIMTYPE_KNOCKBACK;
						AddScore(SCORETYPE_SCORE, (g_aEnemy[nCntEnemy].texture + 1) * 300);

						// �ړ��l���
						if (g_aEnemy[nCntEnemy].nDinectionMove == 1) { g_aEnemy[nCntEnemy].move.x += 20; }
						else { g_aEnemy[nCntEnemy].move.x -= 20; }
						g_aEnemy[nCntEnemy].move.y -= 5;
					}
				}
				else if (g_aEnemy[nCntEnemy].AnimType == ENEMYANIMTYPE_KNOCKBACK)
				{// �m�b�N�o�b�N�̏ꍇ
					g_aEnemy[nCntEnemy].nCntTimer++;	// �J�E���^��i�߂�

					if (g_aEnemy[nCntEnemy].nCntTimer > 30)
					{// 0.5�b��
						DeleteEnemy(nCntEnemy);		//�G�j������
					}
				}

				if (g_aEnemy[nCntEnemy].AnimType != ENEMYANIMTYPE_KNOCKBACK)
				{
					if (g_aEnemy[nCntEnemy].texture == ENEMYTEXTURE_SLIME)
					{
						if (pPlayer->pos.x + pPlayer->fWidth > g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth - 500 &&
							pPlayer->pos.x - pPlayer->fWidth < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth + 500)
						{
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y)) * 0.5f;
						}
					}
					else if (g_aEnemy[nCntEnemy].texture == ENEMYTEXTURE_BIRD)
					{
						if (g_aEnemy[nCntEnemy].nCounterAnim % 240 == 0)
						{
							g_aEnemy[nCntEnemy].move.y -= 8.0f;
						}
					}
					else if (g_aEnemy[nCntEnemy].texture == ENEMYTEXTURE_GHOST)
					{
						if (pPlayer->pos.x + pPlayer->fWidth > g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth - 500 &&
							pPlayer->pos.x - pPlayer->fWidth < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth + 500)
						{
							g_aEnemy[nCntEnemy].move.x = sinf(atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y)) * 0.8f;
							g_aEnemy[nCntEnemy].move.y = cosf(atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y)) * 0.8f;
						}
					}
				}
			}

			if (g_aEnemy[nCntEnemy].nCounterAnim % 9 == 0 && g_aEnemy[nCntEnemy].AnimType != ENEMYANIMTYPE_KNOCKBACK)
			{// �A�j���[�V����
				g_aEnemy[nCntEnemy].nNumAnim = (g_aEnemy[nCntEnemy].nNumAnim + 1) % 5;
			}

			// ���_�ݒ�
			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight, 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight, 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntEnemy * 4].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 1].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 2].col = g_aEnemy[nCntEnemy].col;
			pVtx[nCntEnemy * 4 + 3].col = g_aEnemy[nCntEnemy].col;

			// �e�N�X�`���ݒ�
			if (g_aEnemy[nCntEnemy].nDinectionMove == 0)
			{// ��
				pVtx[nCntEnemy * 4].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);
			}
			else
			{// �E
				pVtx[nCntEnemy * 4].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);
				pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nNumAnim * ENEMY_TEXTURE_UV_U, g_aEnemy[nCntEnemy].AnimType * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();
}

//=========================================================================================================================
// �G�`�揈��
//=========================================================================================================================
void DrawEnemy(void)
{
	// �ϐ��錾
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
		{// �g�p���Ă���ꍇ
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].texture]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=========================================================================================================================
// �G�̐ݒ菈��
//=========================================================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nLife, ENEMYTYPE Type, ENEMYTEXTURE texture)
{
	// �ϐ��錾
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
		{// �g�p���Ă��Ȃ��ꍇ
			g_aEnemy[nCntEnemy].pos = pos;									// �ʒu�ݒ�
			g_aEnemy[nCntEnemy].move = move;								// �ړ��l�ݒ�
			g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
			g_aEnemy[nCntEnemy].fWidth = fWidth;							// ����
			g_aEnemy[nCntEnemy].fHeight = fHeight;							// ����
			g_aEnemy[nCntEnemy].nLife = nLife;								// �̗͐ݒ�
			g_aEnemy[nCntEnemy].Type = Type;								// �G��ނ�ݒ�
			g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;					// ��Ԃ�ݒ�
			g_aEnemy[nCntEnemy].nCounterState = 0;							// ��ԃJ�E���^������
			g_aEnemy[nCntEnemy].AnimType = ENEMYANIMTYPE_NORMAL;			// �A�j���[�V�������
			g_aEnemy[nCntEnemy].nCounterAnim = 0;							// �J�E���^�[������
			g_aEnemy[nCntEnemy].nNumAnim = 0;								// �A�j���[�V�����ԍ�������
			g_aEnemy[nCntEnemy].nSpeedAnim = 0;								// �A�j���[�V�������x������
			g_aEnemy[nCntEnemy].nPatternAnim = 0;							// �A�j���[�V�����p�^�[��������
			g_aEnemy[nCntEnemy].nCntTimer = 0;								// ���ԃJ�E���^
			g_aEnemy[nCntEnemy].texture = texture;							// �e�N�X�`��
			g_aEnemy[nCntEnemy].bUse = true;								// �g�p���Ă����Ԃɂ���

			g_nNumEnemy++;													// �G�������J�E���g

			// ���_�ݒ�
			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight, 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fHeight, 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			// �e�N�X�`���ݒ�
			pVtx[nCntEnemy * 4].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U, ENEMYANIMTYPE_NORMAL * ENEMY_TEXTURE_UV_V);
			pVtx[nCntEnemy * 4 + 1].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, ENEMYANIMTYPE_NORMAL * ENEMY_TEXTURE_UV_V);
			pVtx[nCntEnemy * 4 + 2].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U, ENEMYANIMTYPE_NORMAL * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);
			pVtx[nCntEnemy * 4 + 3].tex = D3DXVECTOR2(ENEMY_TEXTURE_UV_U + ENEMY_TEXTURE_UV_U, ENEMYANIMTYPE_NORMAL * ENEMY_TEXTURE_UV_V + ENEMY_TEXTURE_UV_V);

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEnemy->Unlock();
}

//=========================================================================================================================
// �G�̎擾����
//=========================================================================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];		// �l��Ԃ�
}

//=========================================================================================================================
// �G�Ƃ̏Փˏ���
//=========================================================================================================================
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	// �ϐ��錾
	int nCntEnemy;		// �G�J�E���^
	bool bHit = false;	// �G�ɓ���������
	D3DXVECTOR3 Range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �����蔻��

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G���J�E���g
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// �g�p���Ă���ꍇ
			// �Փ˔��蒲��
			EnemyCollisionRange(nCntEnemy, &Range);

			if (pPos->x + (fWidth / 2 * 0.8f) >= g_aEnemy[nCntEnemy].pos.x - Range.x && pPos->x - (fWidth / 2 * 0.8f) <= g_aEnemy[nCntEnemy].pos.x + Range.x &&
				pPos->y - (fHeight * 0.8f)<= g_aEnemy[nCntEnemy].pos.y && pPos->y >= g_aEnemy[nCntEnemy].pos.y - Range.y)
			{
				// �m�b�N�o�b�N�ݒ�
				pMove->x = 0.0f;

				if (pMove->y <= 0.0f)
				{
					pMove->y *= -1;
				}
				if (pPos->x < g_aEnemy[nCntEnemy].pos.x)
				{
					pMove->x -= 50;
				}
				else
				{
					pMove->x += 50;
				}

				bHit = true;	// �����蔻��
			}
		}
	}

	return bHit;		// �l��Ԃ�
}

//=========================================================================================================================
// �G�̔j������
//=========================================================================================================================
void DeleteEnemy(int nCntEnemy)
{// �P��
	g_aEnemy[nCntEnemy].bUse = false;		// �G��j��
}
void DeleteEnemy(void)
{// �S��
	// �ϐ��錾
	int nCntEnemy;		// �G�J�E���^

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{// �G���J�E���g
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// �g�p����Ă���ꍇ
			g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���S�l
			g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���S�l
			g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �ړ��l
			g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
			g_aEnemy[nCntEnemy].fWidth = 0.0f;									// ����
			g_aEnemy[nCntEnemy].fHeight = 0.0f;									// ����
			g_aEnemy[nCntEnemy].nLife = 0;										// �̗�
			g_aEnemy[nCntEnemy].Type = ENEMYTYPE_NORMALENEMY;					// ���
			g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;						// ���
			g_aEnemy[nCntEnemy].nCounterState = 0;								// ��ԃJ�E���^
			g_aEnemy[nCntEnemy].AnimType = ENEMYANIMTYPE_NORMAL;				// �A�j���[�V�������
			g_aEnemy[nCntEnemy].nCounterAnim = 0;								// �A�j���[�V�����J�E���^
			g_aEnemy[nCntEnemy].nNumAnim = 0;									// �A�j���[�V�����ԍ�
			g_aEnemy[nCntEnemy].nSpeedAnim = 0;									// �A�j���[�V�������x
			g_aEnemy[nCntEnemy].nPatternAnim = 0;								// �A�j���[�V�����p�^�[��
			g_aEnemy[nCntEnemy].nDinectionMove = 0;								// ����
			g_aEnemy[nCntEnemy].nCntTimer = 0;									// ���ԃJ�E���^
			g_aEnemy[nCntEnemy].texture = ENEMYTEXTURE_SLIME;					// �e�N�X�`��
			g_aEnemy[nCntEnemy].bUse = false;									// �g�p���Ă��邩

		}
	}
	g_nNumEnemy = 0;	// �G����
}

//==============================================================
// �Փ˔͈͒���
//==============================================================
void EnemyCollisionRange(int nCntEnemy, D3DXVECTOR3 *pRange)
{
	switch (g_aEnemy[nCntEnemy].texture)
	{// �e�N�X�`�����
	case ENEMYTEXTURE_SLIME:		// �X���C��
		
		pRange->x = 30;
		pRange->y = 30;
		
		break;
	case ENEMYTEXTURE_SNOWMAN:		// �Ⴞ���
		
		pRange->x = 40;
		pRange->y = 60;

		break;
	case ENEMYTEXTURE_CURSE:		// �̂낢
		
		pRange->x = 30;
		pRange->y = 35;

		break;
	case ENEMYTEXTURE_BIRD:			// ��

		pRange->x = 45;
		pRange->y = 60;

		break;
	case ENEMYTEXTURE_GHOST:		// �S�[�X�g

		pRange->x = 45;
		pRange->y = 60;

		break;
	}
}