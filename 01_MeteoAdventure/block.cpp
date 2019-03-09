//=========================================================================================================================
//
// �u���b�N���� [block.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "block.h"		// �u���b�N
#include "input.h"		// ���͏���
#include "player.h"		// �v���C���[
#include "bullet.h"		// �e

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define BLOCK_TEXTURE_SPEED		(5)								// �e�N�X�`���A�j���[�V�����X�s�[�h
#define BLOCK_TEXTURE_PATTERN	(10)							// �e�N�X�`���A�j���[�V�����p�^�[����
#define BLOCK_TEXTURE_UV_U		(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define BLOCK_TEXTURE_UV_V		(1.0f)							// �e�N�X�`���A�j���[�V����V�͈�
#define BLOCK_BACKTEXTURE_UV_U	(1.0f)							// �O�e�N�X�`���A�j���[�V����U�͈�
#define BLOCK_BACKTEXTURE_UV_V	(1.0f)							// �O�e�N�X�`���A�j���[�V����V�͈�
#define BLOCK_SPEED				(0.002f)						// �u���b�N�X�s�[�h
#define BLOCK_LIFE				(3)								// �u���b�N�ϋv�l

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureBlock[BLOCKTEXTURE_MAX] = {};			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffBlock = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
BLOCK							g_Block[MAX_BLOCK];								// �u���b�N�̏��
bool							g_bScroll;										// �X�N���[������

//=========================================================================================================================
// �u���b�N����������
//=========================================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X���擾
	int nCntBlock;		// �u���b�N�J�E���^

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// �u���b�N���J�E���g
		g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_Block[nCntBlock].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_Block[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ��l
		g_Block[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		g_Block[nCntBlock].fWidth = 0.0f;							// ����
		g_Block[nCntBlock].fHeight = 0.0f;							// ����
		g_Block[nCntBlock].type = BLOCKTYPE_NONE;					// �u���b�N���
		g_Block[nCntBlock].nLife = 0;								// �ϋv�l
		g_Block[nCntBlock].nCntMove = 0;							// �ړ��J�E���^
		g_Block[nCntBlock].nCntMoveLimit = 0;						// �ړ�����
		g_Block[nCntBlock].bBlockScroll = false;					// �u���b�N�ɓ������Ă��Ȃ�
		g_Block[nCntBlock].bUse = false;							// �g�p���Ă��Ȃ�
		g_Block[nCntBlock].texture = BLOCKTEXTURE_GROUND;			// �e�N�X�`��
	}
	g_bScroll = false;			// �X�N���[������

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_1, &g_pTextureBlock[0]);		// �n��
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_2, &g_pTextureBlock[1]);		// ��
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_3, &g_pTextureBlock[2]);		// �m�[�}��
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_4, &g_pTextureBlock[3]);		// �y��
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_5, &g_pTextureBlock[4]);		// ����
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_6, &g_pTextureBlock[5]);		// �Ƃ�
	D3DXCreateTextureFromFile(pDevice, BLOCK_TEXTURE_NAME_7, &g_pTextureBlock[6]);		// ��

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBlock->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// �u���b�N���J�E���g
		// ���_�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
		// ���_�J���[�ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(BLOCK_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, BLOCK_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(BLOCK_TEXTURE_UV_U, BLOCK_TEXTURE_UV_V);
		// 1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBlock->Unlock();
}

//=========================================================================================================================
// �u���b�N�I������
//=========================================================================================================================
void UninitBlock(void)
{
	// �ϐ��錾
	int nCntTex;

	// �e�N�X�`���̔j��
	for (nCntTex = 0; nCntTex < BLOCKTEXTURE_MAX; nCntTex++)
	{// �e�N�X�`�����J�E���g
		if (g_pTextureBlock[nCntTex] != NULL)
		{
			g_pTextureBlock[nCntTex]->Release();
			g_pTextureBlock[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//=========================================================================================================================
// �u���b�N�X�V����
//=========================================================================================================================
void UpdateBlock(void)
{
	// �ϐ��錾
	int nCntBlock;		// �u���b�N�J�E���g
	g_bScroll = false;	// �X�N���[������
	PLAYER *pPlayer = GetPlayer();		// �v���C���[���
	GAMESTAGE stage = GetNumStage();

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBlock->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// �u���b�N�ɓ������Ă��邩�m�F
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// �u���b�N���J�E���g
		if (g_Block[nCntBlock].bUse == true)
		{// �u���b�N���g�p���Ă���
			if (g_Block[nCntBlock].bBlockScroll == true)
			{
				g_bScroll = true;	// ���l��ύX
			}
		}
	}

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// �u���b�N���J�E���g
		if (g_Block[nCntBlock].bUse == true)
		{// �u���b�N���g�p���Ă���
			switch (g_Block[nCntBlock].type)
			{// �u���b�N���
			case BLOCKTYPE_NONE:		// �g�p���Ă��Ȃ�
				break;
			case BLOCKTYPE_NORMAL:
				break;
			case BLOCKTYPE_NORMAL_BREAK:
				break;
			case BLOCKTYPE_NORMAL_WARP_X:
				break;
			case BLOCKTYPE_NORMAL_WARP_Y:
				break;
			case BLOCKTYPE_NORMAL_FALL:

				if (g_Block[nCntBlock].move.y != 0.0f)
				{
					g_Block[nCntBlock].nCntMove++;

					if (g_Block[nCntBlock].nCntMove > g_Block[nCntBlock].nCntMoveLimit)
					{
						g_Block[nCntBlock].col.a -= 0.06f;

						if (g_Block[nCntBlock].col.a <= 0.0f)
						{
							DeleteBlock(nCntBlock);
						}
					}
				}

				break;
			case BLOCKTYPE_NORMAL_APPEAR:

				if (g_Block[nCntBlock].pos.x - 200 < pPlayer->pos.x + pPlayer->fWidth && g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + 200 > pPlayer->pos.x - pPlayer->fWidth)
				{
					if (g_Block[nCntBlock].col.a <= 1.0f)
					{
						g_Block[nCntBlock].col.a += 0.03f;
					}
				}
				else
				{
					if (g_Block[nCntBlock].col.a >= 0.0f)
					{
						g_Block[nCntBlock].col.a -= 0.03f;
					}
				}

				break;
			case BLOCKTYPE_NORMAL_DAMAGE:
				break;
			case BLOCKTYPE_NORMAL_SUNA:

				if (g_Block[nCntBlock].move.y != 0.0f)
				{
					g_Block[nCntBlock].nCntMove++;

					if (g_Block[nCntBlock].nCntMove > g_Block[nCntBlock].nCntMoveLimit)
					{
						g_Block[nCntBlock].col.a -= 0.06f;

						if (g_Block[nCntBlock].col.a <= 0.0f)
						{
							DeleteBlock(nCntBlock);
						}
					}
				}

				break;
			case BLOCKTYPE_NORMAL_SUNA_RE:

				if (g_Block[nCntBlock].move.y > 0.0f)
				{
					g_Block[nCntBlock].nCntMove++;

					if (g_Block[nCntBlock].nCntMove > g_Block[nCntBlock].nCntMoveLimit)
					{
						if (g_Block[nCntBlock].col.a >= 0.0f)
						{
							g_Block[nCntBlock].col.a -= 0.01f;
						}

						if (g_Block[nCntBlock].nCntMove == g_Block[nCntBlock].nCntMoveLimit * 2)
						{
							g_Block[nCntBlock].move.y *= -1;
							g_Block[nCntBlock].nCntMove = 0;
						}
					}
				}

				if (g_Block[nCntBlock].move.y < 0.0f)
				{
					g_Block[nCntBlock].nCntMove++;
					if (g_Block[nCntBlock].nCntMove == g_Block[nCntBlock].nCntMoveLimit * 2)
					{
						g_Block[nCntBlock].move.y = 0.0f;
						g_Block[nCntBlock].nCntMove = 0;
					}
				}
				else
				{
					if (g_Block[nCntBlock].move.y == 0.0f && g_Block[nCntBlock].col.a <= 1.0f)
					{
						g_Block[nCntBlock].col.a += 0.08f;
					}
				}

				break;
			case BLOCKTYPE_MOVE_TRIP:	// ����

				g_Block[nCntBlock].nCntMove++;

				if (g_Block[nCntBlock].nCntMove == g_Block[nCntBlock].nCntMoveLimit)
				{
					g_Block[nCntBlock].move *= -1;
					g_Block[nCntBlock].nCntMove = 0;
				}

				break;
			case BLOCKTYPE_MOVE_LOOP:	// �J��Ԃ�

				if (g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight < 0)
				{
					g_Block[nCntBlock].pos.y = SCREEN_HEIGHT + g_Block[nCntBlock].fHeight;
				}
				else if (g_Block[nCntBlock].pos.y > SCREEN_HEIGHT + g_Block[nCntBlock].fHeight)
				{
					g_Block[nCntBlock].pos.y = -g_Block[nCntBlock].fHeight;
				}

				if (g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth < 0)
				{
					g_Block[nCntBlock].pos.x = SCREEN_WIDTH + g_Block[nCntBlock].fWidth;
				}
				else if (g_Block[nCntBlock].pos.x > SCREEN_WIDTH + g_Block[nCntBlock].fWidth)
				{
					g_Block[nCntBlock].pos.x = -g_Block[nCntBlock].fWidth;
				}

				break;
			}

			if (g_bScroll == false)
			{// �X�N���[��
				g_Block[nCntBlock].pos.x -= pPlayer->move.x / 2;
			}

			g_Block[nCntBlock].pos += g_Block[nCntBlock].move;

			// ���_�ݒ�
			pVtx[nCntBlock * 4].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, 0.0f);
			pVtx[nCntBlock * 4 + 1].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y, 0.0f);
			pVtx[nCntBlock * 4 + 2].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, 0.0f);
			pVtx[nCntBlock * 4 + 3].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntBlock * 4].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 1].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 2].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 3].col = g_Block[nCntBlock].col;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBlock->Unlock();
}

//=========================================================================================================================
// �u���b�N�`�揈��
//=========================================================================================================================
void DrawBlock(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntBlock;		// �u���b�N�J�E���^

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBlock,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// �u���b�N���J�E���g
		if (g_Block[nCntBlock].bUse == true)
		{// �g�p���Ă���ꍇ
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureBlock[g_Block[nCntBlock].texture]);

			// �u���b�N�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}

//=========================================================================================================================
// �u���b�N�ݒ菈��
//=========================================================================================================================
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, BLOCKTYPE type, int nCntMoveLimit, BLOCKTEXTURE texture)
{
	// �ϐ��錾
	int nCntBlock;		// �u���b�N�J�E���^

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBlock->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// �u���b�N���J�E���g
		if (g_Block[nCntBlock].bUse == false)
		{// �g�p���Ă��Ȃ��ꍇ
			g_Block[nCntBlock].pos = pos;						// �ʒu
			g_Block[nCntBlock].move = move;						// �ړ��l
			g_Block[nCntBlock].fWidth = fWidth;					// ����
			g_Block[nCntBlock].fHeight = fHeight;				// ����
			g_Block[nCntBlock].type = type;						// �u���b�N���
			g_Block[nCntBlock].nCntMove = 0;					// �ړ��J�E���g
			g_Block[nCntBlock].nCntMoveLimit = nCntMoveLimit;	// �ړ�����
			g_Block[nCntBlock].bUse = true;						// �g�p���Ă���
			g_Block[nCntBlock].texture = texture;				// �e�N�X�`��

			switch (g_Block[nCntBlock].type)
			{
			case BLOCKTYPE_NORMAL_BREAK:

				g_Block[nCntBlock].nLife = 1;				// �u���b�N�ϋv�l

				break;
			case BLOCKTYPE_NORMAL_DAMAGE:

				g_Block[nCntBlock].nLife = 2;				// �u���b�N�ϋv�l

				break;
			}

			if (g_Block[nCntBlock].texture == BLOCKTEXTURE_DOKAN)
			{
				g_Block[nCntBlock].col = D3DXCOLOR(0.5f, 1.0f, 0.1f, 1.0f);
			}
			else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_BREAK)
			{
				g_Block[nCntBlock].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_FALL)
			{
				g_Block[nCntBlock].col = D3DXCOLOR(0.3f, 1.0f, 0.6f, 1.0f);
			}
			else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_APPEAR)
			{
				g_Block[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			}
			else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_DAMAGE)
			{
				g_Block[nCntBlock].col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
			}
			else
			{
				g_Block[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// ���_�ݒ�
			pVtx[nCntBlock * 4].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x,										g_Block[nCntBlock].pos.y, 0.0f);
			pVtx[nCntBlock * 4 + 1].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth,		g_Block[nCntBlock].pos.y, 0.0f);
			pVtx[nCntBlock * 4 + 2].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x,									g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, 0.0f);
			pVtx[nCntBlock * 4 + 3].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth,		g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntBlock * 4].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 1].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 2].col = g_Block[nCntBlock].col;
			pVtx[nCntBlock * 4 + 3].col = g_Block[nCntBlock].col;

			// �e�N�X�`���ݒ�
			if (g_Block[nCntBlock].texture == BLOCKTEXTURE_GROUND || g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_APPEAR || g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA)
			{
				if (g_Block[nCntBlock].texture == BLOCKTEXTURE_GROUND)
				{
					g_Block[nCntBlock].tex.x = BLOCK_TEXTURE_UV_U / (SCREEN_WIDTH / (fWidth));
				}
				else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_APPEAR || g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA)
				{
					g_Block[nCntBlock].tex.x = BLOCK_TEXTURE_UV_U / (50 / (fWidth));
				}

				pVtx[nCntBlock * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBlock * 4 + 1].tex = D3DXVECTOR2(g_Block[nCntBlock].tex.x, 0.0f);
				pVtx[nCntBlock * 4 + 2].tex = D3DXVECTOR2(0.0f, BLOCK_TEXTURE_UV_V);
				pVtx[nCntBlock * 4 + 3].tex = D3DXVECTOR2(g_Block[nCntBlock].tex.x, BLOCK_TEXTURE_UV_V);
			}
			else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA_RE)
			{
				g_Block[nCntBlock].tex.x = BLOCK_TEXTURE_UV_U / (50 / (fWidth));
				g_Block[nCntBlock].tex.y = BLOCK_TEXTURE_UV_V / (50 / (fHeight));

				pVtx[nCntBlock * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBlock * 4 + 1].tex = D3DXVECTOR2(g_Block[nCntBlock].tex.x, 0.0f);
				pVtx[nCntBlock * 4 + 2].tex = D3DXVECTOR2(0.0f, g_Block[nCntBlock].tex.y);
				pVtx[nCntBlock * 4 + 3].tex = D3DXVECTOR2(g_Block[nCntBlock].tex.x, g_Block[nCntBlock].tex.y);
			}
			else
			{
				pVtx[nCntBlock * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBlock * 4 + 1].tex = D3DXVECTOR2(BLOCK_TEXTURE_UV_U, 0.0f);
				pVtx[nCntBlock * 4 + 2].tex = D3DXVECTOR2(0.0f, BLOCK_TEXTURE_UV_V);
				pVtx[nCntBlock * 4 + 3].tex = D3DXVECTOR2(BLOCK_TEXTURE_UV_U, BLOCK_TEXTURE_UV_V);
			}

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBlock->Unlock();
}

//=========================================================================================================================
// �u���b�N�Ƃ̏Փˏ���
//=========================================================================================================================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight, BLOCK **pBlock, COLLISIONBLOCKRETURN collision)
{
	// �ϐ��錾
	int nCntBlock;			// �u���b�N�J�E���^
	int nPointplus;			// �����蔻��ʒu����
	bool bLand = false;		// ���n�������ǂ���
	int BlockCheck = NULL;	// �����������m�F
	int nHit = 0;

	if (collision == COLLISIONBLOCKRETURN_PLAYER || collision == COLLISIONBLOCKRETURN_BULLET)
	{// �v���C���[�̂ݒ���
		*pBlock = NULL;
		if (collision == COLLISIONBLOCKRETURN_PLAYER)
		{
			pPosOld->x -= pMove->x;
		}
	}

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// �u���b�N���J�E���g
		if (g_Block[nCntBlock].bUse == true)
		{// �u���b�N���g�p���Ă���
			if (g_Block[nCntBlock].texture == BLOCKTEXTURE_GROUND || g_Block[nCntBlock].texture == BLOCKTEXTURE_SABAKU)
			{// ������ʒu����
				nPointplus = 15;
			}
			else
			{
				nPointplus = 5;
			}

			if (collision == COLLISIONBLOCKRETURN_BULLET)
			{
				if ((pPosOld->x - fWidth > g_Block[nCntBlock].pos.x && pPosOld->x + fWidth < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth &&
					pPosOld->y - fHeight > g_Block[nCntBlock].pos.y && pPosOld->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight) ||
					(pPos->x - fWidth > g_Block[nCntBlock].pos.x	&& pPos->x + fWidth < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth &&
					pPos->y - fHeight > g_Block[nCntBlock].pos.y	&& pPos->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight))
				{
					bLand = true;
				}
			}

			if (pPos->x + (fWidth / 2) > g_Block[nCntBlock].pos.x && pPos->x - (fWidth / 2) < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
			{// X�͈͊m�F
				if (pPos->y >= g_Block[nCntBlock].pos.y + nPointplus && g_Block[nCntBlock].pos.y + nPointplus >= pPosOld->y)
				{// �ォ��̓����蔻��
					bLand = true;

					if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_WARP_Y && (GetKeyboardTrigger(DIK_S) == true || GetKeyboardTrigger(DIK_DOWN) == true))
					{
						if (pPos->x - (fWidth / 2) > g_Block[nCntBlock].pos.x && pPos->x + (fWidth / 2) < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
						{
							pMove->x = 0.0f;
							pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight + fHeight;
						}
						else
						{
							pPos->y = g_Block[nCntBlock].pos.y + nPointplus;
							pMove->y = 0.0f;
						}
					}
					else
					{
						if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA_RE && g_Block[nCntBlock].col.a < 1.0f)
						{

						}
						else
						{
							pPos->y = g_Block[nCntBlock].pos.y + nPointplus;
							pMove->y = 0.0f;
						}

						if (collision == COLLISIONBLOCKRETURN_PLAYER && g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_FALL)
						{
							g_Block[nCntBlock].move.y = 1.0f;
						}
						else if (collision == COLLISIONBLOCKRETURN_PLAYER && (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA || g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_SUNA_RE))
						{
							if (g_Block[nCntBlock].move.y == 0.0f)
							{
								g_Block[nCntBlock].move.y = 0.5f;
								g_Block[nCntBlock].nCntMove = 0;
							}
						}
					}

					BlockCheck = nCntBlock;
				}
				else if (pPos->y - (fHeight) <= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight && g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight <= pPosOld->y - (fHeight))
				{// ������̓����蔻��
					if (collision == COLLISIONBLOCKRETURN_OTHER || collision == COLLISIONBLOCKRETURN_BULLET)
					{
						bLand = true;
					}

					if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_WARP_Y)
					{
						if (pPos->x - (fWidth / 2) + 2 > g_Block[nCntBlock].pos.x && pPos->x + (fWidth / 2) - 2 < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
						{
							pMove->x = 0.0f;
							pPos->y = g_Block[nCntBlock].pos.y + nPointplus;
						}
						else
						{
							pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight + fHeight;
							pMove->y = 0.0f;
						}
					}
					else
					{
						pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight + fHeight;
						pMove->y = 0.0f;
					}

					BlockCheck = nCntBlock;
				}
				else if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_FALL || g_Block[nCntBlock].type == BLOCKTYPE_MOVE_LOOP || g_Block[nCntBlock].type == BLOCKTYPE_MOVE_TRIP && g_Block[nCntBlock].move.y != 0.0f)
				{// X�͈͊m�F
					if (pPos->y >= g_Block[nCntBlock].pos.y && g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight >= pPos->y)
					{// �ォ��̓����蔻��
						bLand = true;
						pPos->y = g_Block[nCntBlock].pos.y + nPointplus;
						pMove->y = 0.0f;
						BlockCheck = nCntBlock;
					}
				}
			}

			if (pPos->y > g_Block[nCntBlock].pos.y + nPointplus && pPos->y - (fHeight) < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight)
			{// Y�͈͊m�F
				if (pPos->x + (fWidth / 2) >= g_Block[nCntBlock].pos.x && g_Block[nCntBlock].pos.x >= pPosOld->x + (fWidth / 2))
				{// ������̓����蔻��
					if (collision == COLLISIONBLOCKRETURN_OTHER || collision == COLLISIONBLOCKRETURN_BULLET)
					{
						bLand = true;
					}

					if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_WARP_X)
					{
						//	pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + (fWidth / 2);
					}
					else
					{
						pPos->x = g_Block[nCntBlock].pos.x - fWidth / 2;
						pMove->x = 0.0f;
					}

					BlockCheck = nCntBlock;

					if (collision == COLLISIONBLOCKRETURN_PLAYER)
					{
						g_Block[nCntBlock].bBlockScroll = true;
					}
				}
				else if (pPos->x - (fWidth / 2) <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth && g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth <= pPosOld->x - (fWidth / 2))
				{// �E����̓����蔻��
					if (collision == COLLISIONBLOCKRETURN_OTHER || collision == COLLISIONBLOCKRETURN_BULLET)
					{
						bLand = true;
					}

					if (g_Block[nCntBlock].type == BLOCKTYPE_NORMAL_WARP_X)
					{
					//	pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + (fWidth / 2);
					}
					else
					{
						pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + (fWidth / 2);
						pMove->x = 0.0f;
					}

					BlockCheck = nCntBlock;

					if (collision == COLLISIONBLOCKRETURN_PLAYER)
					{
						g_Block[nCntBlock].bBlockScroll = true;
					}
				}
				else if (g_Block[nCntBlock].type == BLOCKTYPE_MOVE_LOOP || g_Block[nCntBlock].type == BLOCKTYPE_MOVE_TRIP && g_Block[nCntBlock].move.x != 0.0f)
				{// Y�͈͊m�F
					if (pPos->x + (fWidth / 2) >= g_Block[nCntBlock].pos.x && g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth >= pPos->x + (fWidth / 2))
					{// ������̓����蔻��
						if (collision == COLLISIONBLOCKRETURN_OTHER || collision == COLLISIONBLOCKRETURN_BULLET)
						{
							bLand = true;
						}
						pPos->x = g_Block[nCntBlock].pos.x - (fWidth / 2);
						pMove->x = 0.0f;
						BlockCheck = nCntBlock;
					}
					else if (pPos->x - (fWidth / 2) <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth && g_Block[nCntBlock].pos.x <= pPos->x - (fWidth / 2))
					{// �E����̓����蔻��
						if (collision == COLLISIONBLOCKRETURN_OTHER || collision == COLLISIONBLOCKRETURN_BULLET)
						{
							bLand = true;
						}
						pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + (fWidth / 2);
						pMove->x = 0.0f;
						BlockCheck = nCntBlock;
					}
				}
				else
				{// �������Ă��Ȃ�
					g_Block[nCntBlock].bBlockScroll = false;
				}
			}
			else
			{// �������Ă��Ȃ�
				g_Block[nCntBlock].bBlockScroll = false;
			}
		}

	}
	if (BlockCheck != NULL && (collision == COLLISIONBLOCKRETURN_PLAYER || collision == COLLISIONBLOCKRETURN_BULLET))
	{// �������Ă���
		*pBlock = &g_Block[BlockCheck];
	}

	return bLand;		// �l��Ԃ�
}

//=========================================================================================================================
// �X�N���[�����菈��
//=========================================================================================================================
bool GetScrollNum(void)
{
	return g_bScroll;
}

//=========================================================================================================================
// �u���b�N�j������
//=========================================================================================================================
void DeleteBlock(int nCntBlock)
{
	g_Block[nCntBlock].bUse = false;
}
void DeleteBlock(void)
{
	// �ϐ��錾
	int nCntBlock;		// �u���b�N�J�E���^

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{// �u���b�N���J�E���g
		if (g_Block[nCntBlock].bUse == true)
		{// �g�p���Ă���ꍇ
			g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
			g_Block[nCntBlock].tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
			g_Block[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ��l
			g_Block[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
			g_Block[nCntBlock].fWidth = 0.0f;							// ����
			g_Block[nCntBlock].fHeight = 0.0f;							// ����
			g_Block[nCntBlock].type = BLOCKTYPE_NONE;					// �u���b�N���
			g_Block[nCntBlock].nLife = 0;								// �u���b�N�ϋv�l
			g_Block[nCntBlock].nCntMove = 0;							// �ړ��J�E���^
			g_Block[nCntBlock].nCntMoveLimit = 0;						// �ړ�����
			g_Block[nCntBlock].bBlockScroll = false;					// �u���b�N�ɓ������Ă��Ȃ�
			g_Block[nCntBlock].bUse = false;							// �g�p���Ă��Ȃ�
			g_Block[nCntBlock].texture = BLOCKTEXTURE_GROUND;			// �e�N�X�`��
		}
	}
	g_bScroll = false;			// �X�N���[������
}