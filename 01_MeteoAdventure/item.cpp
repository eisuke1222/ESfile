//=========================================================================================================================
// 
// �A�C�e������ [item.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "item.h"			// �A�C�e��
#include "input.h"			// ���͏���
#include "player.h"			// �v���C���[
#include "block.h"			// �u���b�N
#include "score.h"			// �X�R�A
#include "fade.h"			// �t�F�[�h			
#include "sound.h"			// �T�E���h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_ITEM				(128)								// �A�C�e���̍ő吔
#define ITEM_TEXTURE_NAME		"data\\TEXTURE\\coin001.png"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define ITEM_POS_X				(20)								// �A�C�e���̍���X���W
#define ITEM_POS_Y				(40)								// �A�C�e���̍���Y���W
#define ITEM_WIDTH				(SCREEN_WIDTH)						// �A�C�e���̕�
#define ITEM_HEIGHT				(SCREEN_HEIGHT)						// �A�C�e���̍�
#define ITEM_TEXTURE_UV_U		(0.2f)								// �e�N�X�`���A�j���[�V����U�͈�
#define ITEM_TEXTURE_UV_V		(0.5f)								// �e�N�X�`���A�j���[�V����V�͈�
#define ITEM_TEXTURE_SPEED		(5)									// �e�N�X�`���A�j���[�V�����X�s�[�h
#define ITEM_TEXTURE_PATTERN	(10)								// �e�N�X�`���A�j���[�V�����p�^�[����

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureItem = NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffItem = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
ItemState					g_Item[MAX_ITEM];						// �A�C�e�����
int							g_nCntCoin;								// �R�C�������J�E���^

//=========================================================================================================================
// �A�C�e������������
//=========================================================================================================================
void InitItem(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntItem;		// �A�C�e���J�E���^

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// �A�C�e�����J�E���g
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_Item[nCntItem].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��ʒu
		g_Item[nCntItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
		g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ��l
		g_Item[nCntItem].fWidth = ITEM_POS_X;						// ����
		g_Item[nCntItem].fHeight = ITEM_POS_Y;						// ����
		g_Item[nCntItem].nCounterAnim = 0;							// �A�j���[�V�����J�E���^
		g_Item[nCntItem].nPatternAnim = 0;							// �A�j���[�V�����p�^�[��
		g_Item[nCntItem].Type = ITEMTYPE_COIN_NORMAL;				// ���
		g_Item[nCntItem].state = ITEMSTATE_APPEAR;					// ���
		g_Item[nCntItem].nCounterState = 0;							// ��ԃJ�E���^
		g_Item[nCntItem].bUse = false;								// �g�p���Ă��邩
	}
	g_nCntCoin = 0;													// ���������l

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, ITEM_TEXTURE_NAME, &g_pTextureItem);		// �R�C��

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffItem->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// �A�C�e�����J�E���g
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
		pVtx[1].tex = D3DXVECTOR2(ITEM_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, ITEM_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(ITEM_TEXTURE_UV_U, ITEM_TEXTURE_UV_V);
	
		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffItem->Unlock();
}

//=========================================================================================================================
// �A�C�e���I������
//=========================================================================================================================
void UninitItem(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureItem != NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//=========================================================================================================================
// �A�C�e���X�V����
//=========================================================================================================================
void UpdateItem(void)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^
	int nCntItem;		// �A�C�e���J�E���^
	PLAYER *pPlayer = GetPlayer();		// �v���C���[���
	bool bScroll = GetScrollNum();

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffItem->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// �A�C�e�����J�E���g
		if (g_Item[nCntItem].bUse == true)
		{// �g�p���Ă���ꍇ
			switch (g_Item[nCntItem].state)
			{
			case ITEMSTATE_APPEAR:
				break;
			case ITEMSTATE_NORMAL:

				g_Item[nCntItem].nCounterAnim++;	// �J�E���^��i�߂�

				if (g_Item[nCntItem].nCounterAnim % ITEM_TEXTURE_SPEED == 0)
				{// �p�^�[����i�߂�
					g_Item[nCntItem].nPatternAnim = (g_Item[nCntItem].nPatternAnim + 1) % ITEM_TEXTURE_PATTERN;
				}

				if (g_Item[nCntItem].Type != ITEMTYPE_COIN_NORMAL)
				{
					g_Item[nCntItem].posOld = g_Item[nCntItem].pos;
					g_Item[nCntItem].move.y += pPlayer->fGravity;
					g_Item[nCntItem].pos.y += g_Item[nCntItem].move.y;

					CollisionBlock(&g_Item[nCntItem].pos, &g_Item[nCntItem].posOld, &g_Item[nCntItem].move, g_Item[nCntItem].fWidth, g_Item[nCntItem].fHeight, NULL, COLLISIONBLOCKRETURN_OTHER);
				}

				break;
			case ITEMSTATE_DISAPPEAR:

				g_Item[nCntItem].nCounterState++;
				g_Item[nCntItem].nPatternAnim = 0;

				if (g_Item[nCntItem].nCounterState < 10)
				{
					g_Item[nCntItem].pos.y += -5;
				}
				else
				{
					g_Item[nCntItem].col.a -= 0.06f;
				
					if (g_Item[nCntItem].col.a <= 0.0f)
					{
						DeleteItem(nCntItem);
					}
				}

				break;
			}

			if (bScroll == false)
			{
				// ��ʃX�N���[��
				g_Item[nCntItem].pos.x -= pPlayer->move.x / 2;
			}

			// ���_�ݒ�
			pVtx[nCntItem * 4].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y - g_Item[nCntItem].fHeight, 0.0f);
			pVtx[nCntItem * 4 + 1].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y - g_Item[nCntItem].fHeight, 0.0f);
			pVtx[nCntItem * 4 + 2].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y, 0.0f);
			pVtx[nCntItem * 4 + 3].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntItem * 4].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 1].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 2].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 3].col = g_Item[nCntItem].col;
			// �e�N�X�`���ݒ�
			pVtx[nCntItem * 4].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim % 5 * ITEM_TEXTURE_UV_U, g_Item[nCntItem].nPatternAnim / 5 * ITEM_TEXTURE_UV_V);
			pVtx[nCntItem * 4 + 1].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim % 5 * ITEM_TEXTURE_UV_U + ITEM_TEXTURE_UV_U, g_Item[nCntItem].nPatternAnim / 5 * ITEM_TEXTURE_UV_V);
			pVtx[nCntItem * 4 + 2].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim % 5 * ITEM_TEXTURE_UV_U, g_Item[nCntItem].nPatternAnim / 5 * ITEM_TEXTURE_UV_V + ITEM_TEXTURE_UV_V);
			pVtx[nCntItem * 4 + 3].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim % 5 * ITEM_TEXTURE_UV_U + ITEM_TEXTURE_UV_U, g_Item[nCntItem].nPatternAnim / 5 * ITEM_TEXTURE_UV_V + ITEM_TEXTURE_UV_V);
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffItem->Unlock();
}

//=========================================================================================================================
// �A�C�e���`�揈��
//=========================================================================================================================
void DrawItem(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntItem;		// �A�C�e���J�E���^

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffItem,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// �A�C�e�����J�E���g
		if (g_Item[nCntItem].bUse == true)
		{// �g�p���Ă���ꍇ
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureItem);

			// �A�C�e���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntItem * 4, 2);
		}
	}
}

//=========================================================================================================================
// �A�C�e���ݒ菈��
//=========================================================================================================================
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type)
{
	// �ϐ��錾
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^
	int nCntItem;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffItem->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// �A�C�e�����J�E���g
		if (g_Item[nCntItem].bUse == false)
		{// �g�p���Ă��Ȃ��ꍇ
			g_Item[nCntItem].pos = pos;				// �ʒu
			g_Item[nCntItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
			g_Item[nCntItem].nCounterAnim = 0;		// �A�j���[�V�����J�E���^������
			g_Item[nCntItem].nPatternAnim = 0;		// �A�j���[�V�����p�^�[��������
			g_Item[nCntItem].Type = type;			// ���
			g_Item[nCntItem].state = ITEMSTATE_NORMAL;	// ���
			g_Item[nCntItem].nCounterState = 0;		// ��ԃJ�E���^
			g_Item[nCntItem].bUse = true;			// �g�p���Ă���

			g_nCntCoin++;							// ���������Z

			// ���_�ݒ�
			pVtx[nCntItem * 4].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y - g_Item[nCntItem].fHeight, 0.0f);
			pVtx[nCntItem * 4 + 1].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y - g_Item[nCntItem].fHeight, 0.0f);
			pVtx[nCntItem * 4 + 2].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x - g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y, 0.0f);
			pVtx[nCntItem * 4 + 3].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth, g_Item[nCntItem].pos.y, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntItem * 4].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 1].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 2].col = g_Item[nCntItem].col;
			pVtx[nCntItem * 4 + 3].col = g_Item[nCntItem].col;
			// �e�N�X�`���ݒ�
			pVtx[nCntItem * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCntItem * 4 + 1].tex = D3DXVECTOR2(ITEM_TEXTURE_UV_U, 0.0f);
			pVtx[nCntItem * 4 + 2].tex = D3DXVECTOR2(0.0f, ITEM_TEXTURE_UV_V);
			pVtx[nCntItem * 4 + 3].tex = D3DXVECTOR2(ITEM_TEXTURE_UV_U, ITEM_TEXTURE_UV_V);
			
			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffItem->Unlock();
}

//=========================================================================================================================
// �A�C�e���Փˏ���
//=========================================================================================================================
void CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	// �ϐ��錾
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// �u���b�N���J�E���g
		if (g_Item[nCntItem].bUse == true && g_Item[nCntItem].state == ITEMSTATE_NORMAL)
		{// �u���b�N���g�p���Ă���ꍇ
			if (pPos->x + fWidth / 2 > g_Item[nCntItem].pos.x - g_Item[nCntItem].fWidth && pPos->x - fWidth / 2 < g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth &&
				pPos->y - fHeight < g_Item[nCntItem].pos.y && pPos->y > g_Item[nCntItem].pos.y - g_Item[nCntItem].fHeight)
			{// �Փ˂��Ă���
				g_nCntCoin--;						// ���������Z
				PlaySound(SOUND_LABEL_SE_ITEMHIT000);
				AddScore(SCORETYPE_COIN, -1);		// �c��R�C��������\��
				AddScore(SCORETYPE_SCORE, 100);		// �擾�X�R�A�����Z
				g_Item[nCntItem].state = ITEMSTATE_DISAPPEAR;
			}
		}
	}
}

//=========================================================================================================================
// �A�C�e���j������
//=========================================================================================================================
void DeleteItem(int nCntItem)
{
	g_Item[nCntItem].bUse = false;		// �Փ˂����A�C�e����j��

	if (g_nCntCoin <= 0)
	{// �������ȉ��̏ꍇ
	//	SetFadeGame(GAMESTATE_MAP, FADETYPE_GAME);		// �Q�[���}�b�v
	}
}
void DeleteItem(void)
{
	// �ϐ��錾
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// �A�C�e�����J�E���g
		if (g_Item[nCntItem].bUse == true)
		{// �g�p���Ă���ꍇ
			g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
			g_Item[nCntItem].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��ʒu
			g_Item[nCntItem].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �F
			g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ��l
			g_Item[nCntItem].fWidth = ITEM_POS_X;						// ����
			g_Item[nCntItem].fHeight = ITEM_POS_Y;						// ����
			g_Item[nCntItem].nCounterAnim = 0;							// �A�j���[�V�����J�E���^
			g_Item[nCntItem].nPatternAnim = 0;							// �A�j���[�V�����p�^�[��
			g_Item[nCntItem].Type = ITEMTYPE_COIN_NORMAL;				// ���
			g_Item[nCntItem].state = ITEMSTATE_APPEAR;					// ���
			g_Item[nCntItem].nCounterState = 0;							// ��ԃJ�E���^
			g_Item[nCntItem].bUse = false;								// �g�p���Ă��邩
		}
	}
	
	g_nCntCoin = 0;			// ���������l
}

//=========================================================================================================================
// �A�C�e��(�R�C���j�����擾����
//=========================================================================================================================
int GetNumCoin(void)
{
	return g_nCntCoin;		// �l��Ԃ�
}