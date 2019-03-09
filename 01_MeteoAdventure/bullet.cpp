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
#include "score.h"		// �X�R�A
#include "player.h"		// �v���C���[
#include "enemy.h"		// �G
#include "item.h"		// �A�C�e��
#include "sound.h"		// �T�E���h

//**********************************************************************************************************************
// �}�N����`
//**********************************************************************************************************************
#define BULLET_TEXTURE_UV_U		(1.0f)									// �e�A�j���[�V����U�͈�
#define BULLET_TEXTURE_UV_V		(1.0f)									// �e�A�j���[�V����V�͈�

//**********************************************************************************************************************
// �O���[�o���ϐ��錾
//**********************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureBullet[BULLETTEXTURETYPE_PLAYER_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffBullet = NULL;								// ���_�o�b�t�@�ւ̃|�C���^
Bullet							g_aBullet[MAX_BULLET];									// �e�̏��

//=========================================================================================================================
// �e����������
//=========================================================================================================================
void InitBullet(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntBullet;		// �e�J�E���g

	// �e�̏��̏�����
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// �e���J�E���g
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �e���S�l
		g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �e���S�l
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �e�ړ��l
		g_aBullet[nCntBullet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �e�F
		g_aBullet[nCntBullet].Type = BULLETTYPE_PLAYER;					// �e���
		g_aBullet[nCntBullet].fWidth = 0;								// ����
		g_aBullet[nCntBullet].fHeight = 0;								// ����
		g_aBullet[nCntBullet].texture = BULLETTEXTURETYPE_PLAYER_NORMAL;// �e�N�X�`��
		g_aBullet[nCntBullet].bUse = false;								// �e�g�p���Ă��Ȃ����
		g_aBullet[nCntBullet].pBlock = NULL;							// �u���b�N�|�C���^
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURE_NAME1, &g_pTextureBullet[0]);		// �v���C���[

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
	int nCntTex;	// �e�N�X�`���J�E���^

	// �e�N�X�`���[�̔j��
	for (nCntTex = 0; nCntTex < BULLETTEXTURETYPE_PLAYER_MAX; nCntTex++)
	{// �e�N�X�`�����J�E���g
		if (g_pTextureBullet[nCntTex] != NULL)
		{
			g_pTextureBullet[nCntTex]->Release();
			g_pTextureBullet[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet!= NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet= NULL;
	}
}

//=========================================================================================================================
// �e�X�V����
//=========================================================================================================================
void UpdateBullet(void)
{
	// �ϐ��錾
	int nCntBullet;					// �e�J�E���g

	PLAYER *pPlayer = GetPlayer();	// �v���C���[���

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
		{// �g�p����Ă���ꍇ

			// �ʒu���L��
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;
			// ��ʃX�N���[��
			g_aBullet[nCntBullet].pos.x -= pPlayer->move.x / 2;
			// �d�͉��Z
			g_aBullet[nCntBullet].move.y += pPlayer->fGravity;
			// �ړ��l���
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			if (g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT)
			{
				DeleteBullet(nCntBullet);
			}

			if (CollisionBlock(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight, &g_aBullet[nCntBullet].pBlock, COLLISIONBLOCKRETURN_BULLET) == true)
			{// �u���b�N�Ƃ̏Փˏ���
				DeleteBullet(nCntBullet);
			}

			if (g_aBullet[nCntBullet].pBlock != NULL)
			{
				if (g_aBullet[nCntBullet].pBlock->type== BLOCKTYPE_NORMAL_BREAK || g_aBullet[nCntBullet].pBlock->type == BLOCKTYPE_NORMAL_DAMAGE)
				{
					g_aBullet[nCntBullet].pBlock->nLife--;

					if (g_aBullet[nCntBullet].pBlock->nLife <= 0)
					{
						g_aBullet[nCntBullet].pBlock->bUse = false;
					}
				}
			}

			// �A�C�e���̏Փˏ���
			CollisionItem(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeight);

			// ���_�ݒ�
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y, 0.0f);
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
	// �ϐ��錾
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
			pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].texture]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//=========================================================================================================================
// �e�̐ݒ菈��
//=========================================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, BULLETTYPE Type, float fWidth, float fHeight, BULLETTEXTURETYPE texture)
{
	// �ϐ��錾
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
		{// �g�p����Ă��Ȃ��ꍇ
			g_aBullet[nCntBullet].pos = pos;				// �ʒu�ݒ�
			g_aBullet[nCntBullet].move = move;				// �ړ��l�ݒ�
			g_aBullet[nCntBullet].col = col;				// �F�ݒ�
			g_aBullet[nCntBullet].Type = Type;				// ��ސݒ�
			g_aBullet[nCntBullet].fWidth = fWidth;			// ����
			g_aBullet[nCntBullet].fHeight = fHeight;		// ����
			g_aBullet[nCntBullet].texture = texture;		// �e�N�X�`��
			g_aBullet[nCntBullet].bUse = true;				// �g�p���Ă�����
			g_aBullet[nCntBullet].pBlock = NULL;			// �u���b�N�|�C���^

			// ���_�ݒ�
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight, 0.0f);
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y, 0.0f);
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].pos.y, 0.0f);
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

//=========================================================================================================================
// �e�̓����蔻�菈��
//=========================================================================================================================
bool CollisiomBullet(D3DXVECTOR3 *pPos, float fWidth, float fHeight, COLLISIONBULLETRETURN collision)
{
	// �ϐ��錾
	int nCntBullet;			// �e�J�E���^
	bool bHit = false;		// �����蔻��

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// �e���J�E���g
		if (g_aBullet[nCntBullet].bUse == true)
		{// �g�p����Ă���ꍇ
			if (collision == COLLISIONBULLETRETURN_BLOCK)
			{
				if (g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth <= pPos->x + fWidth &&
					g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth >= pPos->x &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight <= pPos->y + fHeight &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fHeight >= pPos->y)
				{
					DeleteBullet(nCntBullet);		// �e�j������
					bHit = true;					// �����蔻��
				}
			}
			else
			{
				if (g_aBullet[nCntBullet].pos.x - g_aBullet[nCntBullet].fWidth <= pPos->x + fWidth &&
					g_aBullet[nCntBullet].pos.x + g_aBullet[nCntBullet].fWidth >= pPos->x - fWidth &&
					g_aBullet[nCntBullet].pos.y - g_aBullet[nCntBullet].fHeight <= pPos->y &&
					g_aBullet[nCntBullet].pos.y + g_aBullet[nCntBullet].fHeight >= pPos->y - fHeight)
				{
					DeleteBullet(nCntBullet);		// �e�j������
					bHit = true;					// �����蔻��
				}
			}
		}
	}

	return bHit;		// �l��Ԃ�
}

//=========================================================================================================================
// �e�̔j������
//=========================================================================================================================
void DeleteBullet(int nCntBullet)
{//�P��
	SetExplosion(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - (g_aBullet[nCntBullet].fHeight / 2), 0.0f));
	g_aBullet[nCntBullet].bUse = false;
}
void DeleteBullet(void)
{// �S��
	// �ϐ��錾
	int nCntBullet;

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{// �e���J�E���g
		if (g_aBullet[nCntBullet].bUse == true)
		{// �g�p����Ă���ꍇ
			g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �e���S�l
			g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �e���S�l
			g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �e�ړ��l
			g_aBullet[nCntBullet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �e�F
			g_aBullet[nCntBullet].Type = BULLETTYPE_PLAYER;					// �e���
			g_aBullet[nCntBullet].fWidth = 0;								// ����
			g_aBullet[nCntBullet].fHeight = 0;								// ����
			g_aBullet[nCntBullet].texture = BULLETTEXTURETYPE_PLAYER_NORMAL;// �e�N�X�`��
			g_aBullet[nCntBullet].bUse = false;								// �e�g�p���Ă��Ȃ����
			g_aBullet[nCntBullet].pBlock = NULL;							// �u���b�N�|�C���^
		}
	}
}