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
#include "sound.h"			// �T�E���h
#include "input.h"			// ���͏���

#include "player.h"			// �v���C���[

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_ITEM				(128)								// �A�C�e���̍ő吔
#define ITEM_TEXTURE_NAME		"data\\TEXTURE\\item_all000.png"		// �ǂݍ��ރe�N�X�`���t�@�C��
#define ITEM_POS_X				(0)									// �A�C�e���̍���X���W
#define ITEM_POS_Y				(0)									// �A�C�e���̍���Y���W
#define ITEM_WIDTH				(SCREEN_WIDTH)						// �A�C�e���̕�
#define ITEM_HEIGHT				(SCREEN_HEIGHT)						// �A�C�e���̍�
#define ITEM_TEXTURE_UV_U		(0.25f)								// �e�N�X�`���A�j���[�V����U�͈�
#define ITEM_TEXTURE_UV_V		(1.0f)								// �e�N�X�`���A�j���[�V����V�͈�

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureItem = NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffItem = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
ItemState					g_ItemState[MAX_ITEM];					// �A�C�e�����
int							g_nCntPowerHit;

//=========================================================================================================================
// �A�C�e������������
//=========================================================================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{// �A�C�e�����J�E���g
		g_ItemState[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_ItemState[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
		g_ItemState[nCntItem].bDisp = false;							// �\��
	}
	g_nCntPowerHit = 0;		// �q�b�g�J�E���g

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(// �A�C�e��
		pDevice,
		ITEM_TEXTURE_NAME,
		&g_pTextureItem
	);

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
	{// NULL�ȊO�̏ꍇ
		g_pTextureItem->Release();		// �J��
		g_pTextureItem = NULL;			// NULL��
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffItem->Release();		// �J��
		g_pVtxBuffItem = NULL;			// NULL��
	}
}

//=========================================================================================================================
// �A�C�e���X�V����
//=========================================================================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^
	int nCntItem;

	Player *pPlayer = GetPlayer();

	if (pPlayer->bDisp == false)
	{// ��\��
		g_nCntPowerHit = 0;		// ������
	}

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
		if (g_ItemState[nCntItem].bDisp == true)
		{// �\������Ȃ�
			g_ItemState[nCntItem].nCntPos--;	// ���炷

			if (g_ItemState[nCntItem].nCntPos <= 0)
			{// �O�ȉ���������
				g_ItemState[nCntItem].move = D3DXVECTOR3(0.0f, 3.0f, 0.0f);
			}

			if (pPlayer->bDisp == true)
			{// �v���C���[������ꍇ
				if (pPlayer->pos.x - 50 < g_ItemState[nCntItem].pos.x + 12 && pPlayer->pos.x + 50 > g_ItemState[nCntItem].pos.x - 12 && pPlayer->pos.y - 50 < g_ItemState[nCntItem].pos.y + 12 && pPlayer->pos.y + 50 > g_ItemState[nCntItem].pos.y - 12)
				{// �͈̓`�F�b�N
					g_ItemState[nCntItem].move.x = sinf(atan2f(-g_ItemState[nCntItem].pos.x - -pPlayer->pos.x, -g_ItemState[nCntItem].pos.y - -pPlayer->pos.y)) * 5.0f;
					g_ItemState[nCntItem].move.y = cosf(atan2f(-g_ItemState[nCntItem].pos.x - -pPlayer->pos.x, -g_ItemState[nCntItem].pos.y - -pPlayer->pos.y)) * 5.0f;
				}

				if (pPlayer->pos.x < g_ItemState[nCntItem].pos.x + 12 && pPlayer->pos.x > g_ItemState[nCntItem].pos.x - 12 && pPlayer->pos.y < g_ItemState[nCntItem].pos.y + 12 && pPlayer->pos.y > g_ItemState[nCntItem].pos.y - 12)
				{// �����蔻��ݒ�
					PlaySound(SOUND_LABEL_SE_ITEMHIT000);		// �A�C�e���q�b�g��

					HitItem(nCntItem);		// �q�b�g
				}
			}

			g_ItemState[nCntItem].pos += g_ItemState[nCntItem].move;	// �ړ��ʑ��

			if (g_ItemState[nCntItem].pos.y > SCREEN_HEIGHT)
			{// �͈͊O��������
				g_ItemState[nCntItem].bDisp = false;	// ��\��
			}

			// ���_�ݒ�
			pVtx[nCntItem * 4].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x - 12, g_ItemState[nCntItem].pos.y - 12, 0.0f);
			pVtx[nCntItem * 4 + 1].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x + 12, g_ItemState[nCntItem].pos.y - 12, 0.0f);
			pVtx[nCntItem * 4 + 2].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x - 12, g_ItemState[nCntItem].pos.y + 12, 0.0f);
			pVtx[nCntItem * 4 + 3].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x + 12, g_ItemState[nCntItem].pos.y + 12, 0.0f);
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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntItem;

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
		if (g_ItemState[nCntItem].bDisp == true)
		{// �\������Ȃ�
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
		if (g_ItemState[nCntItem].bDisp == false)
		{// ��\���Ȃ�
			g_ItemState[nCntItem].pos		= pos;		// �ʒu
			g_ItemState[nCntItem].nCntPos	= 50;		// �J�E���g
			g_ItemState[nCntItem].Type		= type;		// ���
			g_ItemState[nCntItem].bDisp		= true;		// �\������

			// �ړ���
			g_ItemState[nCntItem].move.x = sinf((rand() % 628) / 100.0f) * 2.0f;
			g_ItemState[nCntItem].move.y = cosf((rand() % 628) / 100.0f) * 2.0f;

			// ���_�ݒ�
			pVtx[nCntItem * 4].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x - 12, g_ItemState[nCntItem].pos.y - 12, 0.0f);
			pVtx[nCntItem * 4 + 1].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x + 12, g_ItemState[nCntItem].pos.y - 12, 0.0f);
			pVtx[nCntItem * 4 + 2].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x - 12, g_ItemState[nCntItem].pos.y + 12, 0.0f);
			pVtx[nCntItem * 4 + 3].pos = D3DXVECTOR3(g_ItemState[nCntItem].pos.x + 12, g_ItemState[nCntItem].pos.y + 12, 0.0f);
			// �e�N�X�`���ݒ�
			pVtx[nCntItem * 4].tex = D3DXVECTOR2(type * ITEM_TEXTURE_UV_U, 0.0f);
			pVtx[nCntItem * 4 + 1].tex = D3DXVECTOR2(type * ITEM_TEXTURE_UV_U + ITEM_TEXTURE_UV_U, 0.0f);
			pVtx[nCntItem * 4 + 2].tex = D3DXVECTOR2(type * ITEM_TEXTURE_UV_U, ITEM_TEXTURE_UV_V);
			pVtx[nCntItem * 4 + 3].tex = D3DXVECTOR2(type * ITEM_TEXTURE_UV_U + ITEM_TEXTURE_UV_U, ITEM_TEXTURE_UV_V);
			
			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffItem->Unlock();
}

//=========================================================================================================================
// �A�C�e�������菈��
//=========================================================================================================================
void HitItem(int nCntItem)
{
	Player *pPlayer = GetPlayer();

	if (g_ItemState[nCntItem].Type == ITEMTYPE_POWER)
	{// �p���[
		g_nCntPowerHit++;

		if (g_nCntPowerHit == 10)
		{
			PlaySound(SOUND_LABEL_SE_BULLETUP000);		// �e�ω���

			pPlayer->Type = BULLETTYPE_PLAYER2;
		}
		else if (g_nCntPowerHit == 20)
		{
			PlaySound(SOUND_LABEL_SE_BULLETUP000);		// �e�ω���

			pPlayer->Type = BULLETTYPE_PLAYER3;
		}
	/*	else if (g_nCntPowerHit == 30)
		{
			PlaySound(SOUND_LABEL_SE_BULLETUP000);		// �e�ω���

			pPlayer->Type = BULLETTYPE_PLAYER3;
		}
		else if (g_nCntPowerHit == 40)
		{
			PlaySound(SOUND_LABEL_SE_BULLETUP000);		// �e�ω���

			pPlayer->Type = BULLETTYPE_PLAYER3;
		}*/
	}
	else if (g_ItemState[nCntItem].Type == ITEMTYPE_LIFE)
	{// ���C�t
		if (pPlayer->nLife < PLAYER_LIFE)
		{// ���C�t���ő���Ⴂ
			pPlayer->bLifeTex[pPlayer->nLife] = true;
			pPlayer->nLife += 1;
		}
	}
	else if (g_ItemState[nCntItem].Type == ITEMTYPE_STAR)
	{// �X�^�[
		pPlayer->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pPlayer->State = PLAYERSTATE_INVINCIBLE;
		pPlayer->nCounterState = 180;
	}

	g_ItemState[nCntItem].bDisp = false;
}