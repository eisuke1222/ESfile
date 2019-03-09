//=========================================================================================================================
// 
// �G�t�F�N�g���� [effect.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "effect.h"		// �G�t�F�N�g

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define	MAX_EFFECT				(14096)									// �G�t�F�N�g�̍ő吔
#define	TEXTURE_EFFECT_NAME		"data/TEXTURE/effect000.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C����

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureEffect = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffEffect = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
EFFECT							g_aEffect[MAX_EFFECT];					// �G�t�F�N�g�̏��
int								g_nCntEffectState;						// ��ԃJ�E���^

//=========================================================================================================================
// �G�t�F�N�g����������
//=========================================================================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X���擾
	int nCntEffect;		// �G�t�F�N�g�J�E���^

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// �G�t�F�N�g���J�E���g
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �G�t�F�N�g���S�l
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �G�t�F�N�g�ړ��l
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �G�t�F�N�g�F
		g_aEffect[nCntEffect].Type = EFFECTTYPE_NONE;						// �G�t�F�N�g���
		g_aEffect[nCntEffect].fRadius = 0.0f;								// �G�t�F�N�g���a
		g_aEffect[nCntEffect].nLife = 0;									// �G�t�F�N�g����
		g_aEffect[nCntEffect].bUse = false;									// �G�t�F�N�g�g�p���Ă��Ȃ����
	}
	g_nCntEffectState = 0;			// ��Ԃ�������

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_EFFECT_NAME,
		&g_pTextureEffect
	);

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// �G�t�F�N�g���J�E���g
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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// �G�t�F�N�g�I������
//=========================================================================================================================
void UninitEffect(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pTextureEffect->Release();	// �J��
		g_pTextureEffect = NULL;		// NULL��
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffEffect->Release();	// �J��
		g_pVtxBuffEffect = NULL;		// NULL��
	}
}

//=========================================================================================================================
// �G�t�F�N�g�X�V����
//=========================================================================================================================
void UpdateEffect(void)
{
	int nCntEffect;			// �G�t�F�N�g�J�E���^

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// �G�t�F�N�g���J�E���g
		if (g_aEffect[nCntEffect].bUse == true)
		{// �G�t�F�N�g���g�p����Ă���ꍇ
			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_BULLET)
			{// �e�G�t�F�N�g�̏ꍇ
				// g_aEffect[nCntEffect].nLife--;
				g_aEffect[nCntEffect].fRadius -= 0.8f;

				if (/*g_aEffect[nCntEffect].nLife <= 0 || */g_aEffect[nCntEffect].fRadius <= 0)
				{// ���C�t��0�ȉ��̏ꍇ
					g_aEffect[nCntEffect].bUse = false;
				}
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_EXPLOSION || g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTION)
			{// �����G�t�F�N�g�̏ꍇ
				g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;	// �ړ��l���

				g_aEffect[nCntEffect].nLife--;				// ��������
			
				// ���a����
				if (g_aEffect[nCntEffect].nLife > 0) 
				{// ���C�t������
					g_aEffect[nCntEffect].fRadius -= 0.05f;	// ����������
				}
				else
				{// ���C�t���Ȃ��Ȃ���
					g_aEffect[nCntEffect].fRadius -= 0.5f;	// ����������
				}

				if (g_aEffect[nCntEffect].fRadius <= 0)
				{// ���C�t��0�ȉ��̏ꍇ
					g_aEffect[nCntEffect].bUse = false;
				}
			}
		}

		// ���_�ݒ�
		pVtx[nCntEffect * 4].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// �G�t�F�N�g�`�揈��
//=========================================================================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X���擾
	int nCntEffect;		// �G�t�F�N�g�J�E���^

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEffect);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �|���S���̕`��
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// �G�t�F�N�g���J�E���g
		if (g_aEffect[nCntEffect].bUse == true)
		{// �G�t�F�N�g���g�p����Ă���ꍇ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=========================================================================================================================
// �e�G�t�F�N�g�ݒ菈��
//=========================================================================================================================
void SetBulletEffect(D3DXVECTOR3 pos, D3DXCOLOR col, EFFECTTYPE Type, float fRadius, int nLife)
{
	int nCntEffect;			// �G�t�F�N�g�J�E���^

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// �G�t�F�N�g���J�E���g
		if (g_aEffect[nCntEffect].bUse == false)
		{// �G�t�F�N�g���g�p����Ă��Ȃ�
			g_aEffect[nCntEffect].pos		= pos;			// pos�l���
			g_aEffect[nCntEffect].col		= col;			// col�l���
			g_aEffect[nCntEffect].Type		= Type;			// ��ސݒ�
			g_aEffect[nCntEffect].fRadius	= fRadius;		// ���a�ݒ�
			g_aEffect[nCntEffect].nLife		= nLife;		// �����ݒ�
			g_aEffect[nCntEffect].bUse		= true;			// �g�p���Ă�����

			// ���_�ݒ�
			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntEffect * 4].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 1].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 2].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 3].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
}

//=========================================================================================================================
// �p�[�e�B�N���G�t�F�N�g�ݒ菈��
//=========================================================================================================================
void SetParticleEffect(D3DXVECTOR3 pos, EFFECTTYPE Type, int nSetMax, int nRadius, int nLife)
{
	int nCntEffect;			// �G�t�F�N�g�J�E���^
	int nCount = 0;			// �G�t�F�N�g���J�E���^

	VERTEX_2D *pVtx;		// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{// �G�t�F�N�g���J�E���g
		if (g_aEffect[nCntEffect].bUse == false)
		{// �G�t�F�N�g���g�p����Ă��Ȃ�
			g_aEffect[nCntEffect].pos = pos;			// pos�l���
			g_aEffect[nCntEffect].Type = Type;			// ��ސݒ�
			g_aEffect[nCntEffect].nLife = nLife;		// �����ݒ�
			g_aEffect[nCntEffect].bUse = true;			// �g�p���Ă�����

			// �ړ������_����
			g_aEffect[nCntEffect].move.x = sinf((rand() % 628) / 100.0f) * ((rand() % 3) + 1);
			g_aEffect[nCntEffect].move.y = cosf((rand() % 628) / 100.0f) * ((rand() % 3) + 1);

			int nNumber = rand() % 3 + 1;

			// col�l���
			if (g_aEffect[nCntEffect].Type == EFFECTTYPE_EXPLOSION)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.8f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 0.5f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f); }
			}
			else if (g_aEffect[nCntEffect].Type == EFFECTTYPE_RESURRECTION)
			{
				if (nNumber == 1) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.8f, 0.8f); }
				if (nNumber == 2) { g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 0.5f); }
				if (nNumber == 3) { g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f); }
			}

			g_aEffect[nCntEffect].fRadius = (rand() % nRadius + 1) * 1.0f;	// ���a�ݒ�

			// ���_�ݒ�
			pVtx[nCntEffect * 4].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[nCntEffect * 4 + 3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntEffect * 4].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 1].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 2].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);
			pVtx[nCntEffect * 4 + 3].col = D3DXCOLOR(g_aEffect[nCntEffect].col.r, g_aEffect[nCntEffect].col.g, g_aEffect[nCntEffect].col.b, g_aEffect[nCntEffect].col.a);

			nCount++;		// �����J�E���g
			if (nCount == nSetMax)
			{// �����ő吔�ɂȂ����ꍇ
				break;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffEffect->Unlock();
}