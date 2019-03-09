//=========================================================================================================================
//
// �G�t�F�N�g�̏��� [billboardeffect.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include <time.h>				// time
#include "billboardeffect.h"	// �G�t�F�N�g
#include "xplayer.h"			// �v���C���[
#include "xenemy.h"				// �G
#include "shadow.h"				// �e
#include "input.h"				// ����

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_BILLBOARDEFFECT					(1024)									// �G�t�F�N�g�̍ő吔
#define BILLBOARDEFFECT_TEXTURE_NAME1		"data\\TEXTURE\\shadow000.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C��1
#define BILLBOARDEFFECT_TEXTURE_UV_U		(1.0f)									// �e�N�X�`���A�j���[�V����U�͈�
#define BILLBOARDEFFECT_TEXTURE_UV_V		(1.0f)									// �e�N�X�`���A�j���[�V����V�͈�
#define BILLBOARDEFFECT_ALPHASUBTRACT		(0.02f)									// ���̌��Z�l

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void MakeVertexBillboardEffect(LPDIRECT3DDEVICE9 pDevice);							// ���_���̐ݒ�

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9						g_pTextureBillboardEffect = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9					g_pVtxBuffBillboardEffect = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
BillboardEffect							g_BillboardEffect[MAX_BILLBOARDEFFECT];		// �G�t�F�N�g�̏��
int										g_nXEnemy, g_nXEnemyNum;					// �G��ށA�ԍ�

//=========================================================================================================================
// �G�t�F�N�g����������
//=========================================================================================================================
void InitBillboardEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntBillboardEffect;

	g_nXEnemy = 0;
	g_nXEnemyNum = 0;

	// ���_���̍쐬
	MakeVertexBillboardEffect(pDevice);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// ������
		g_BillboardEffect[nCntBillboardEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		g_BillboardEffect[nCntBillboardEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		g_BillboardEffect[nCntBillboardEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ���
		g_BillboardEffect[nCntBillboardEffect].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �傫��
		g_BillboardEffect[nCntBillboardEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �F
		g_BillboardEffect[nCntBillboardEffect].coladown = 0.0f;							// �F���Y��
		g_BillboardEffect[nCntBillboardEffect].nLife = 0;								// �̗�
		g_BillboardEffect[nCntBillboardEffect].type = EFFECTTYPE_NONE;					// ���
		g_BillboardEffect[nCntBillboardEffect].nIdxShadow = 0;							// �e�ԍ�
		g_BillboardEffect[nCntBillboardEffect].nDamage = 0;								// �_���[�W
		g_BillboardEffect[nCntBillboardEffect].nType = 0;								// �e���
		g_BillboardEffect[nCntBillboardEffect].bUse = false;							// �g���Ă��邩
	}
}

//=========================================================================================================================
// �G�t�F�N�g�I������
//=========================================================================================================================
void UninitBillboardEffect(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureBillboardEffect != NULL)
	{
		g_pTextureBillboardEffect->Release();
		g_pTextureBillboardEffect = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboardEffect != NULL)
	{
		g_pVtxBuffBillboardEffect->Release();
		g_pVtxBuffBillboardEffect = NULL;
	}
}

//=========================================================================================================================
// �G�t�F�N�g�X�V����
//=========================================================================================================================
void UpdateBillboardEffect(void)
{
	// �ϐ��錾
	int nCntBillboardEffect;

	XPLAYER *pXPlayer = GetXPlayer();	// �v���C���[�擾
	int nEnemyNum = GetXEnemyNum();		// �G�ԍ�

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// �G�t�F�N�g���J�E���g
		if (g_BillboardEffect[nCntBillboardEffect].bUse == true)
		{// �g�p���Ă���ꍇ
			if (g_BillboardEffect[nCntBillboardEffect].type == EFFECTTYPE_BULLET)
			{// �e�̃G�t�F�N�g
				if (g_BillboardEffect[nCntBillboardEffect].nType == EFFECTBULLET_PLAYER2)
				{// �v���C���[�^�C�v�Q
					g_BillboardEffect[nCntBillboardEffect].col.a -= g_BillboardEffect[nCntBillboardEffect].coladown;	// �����x��������

					if (g_BillboardEffect[nCntBillboardEffect].col.a <= 0.0f)
					{// �����ɂȂ�����
						g_BillboardEffect[nCntBillboardEffect].col.a = 0.0f;
						DeleteBillboardEffect(nCntBillboardEffect);				// ����
					}
				}
				else
				{// ���̑�
					if (g_BillboardEffect[nCntBillboardEffect].nLife > 0)
					{// ���C�t����������
						g_BillboardEffect[nCntBillboardEffect].nLife--;			// ���C�t�����炷
					}
					else
					{// �����ɂ���
						g_BillboardEffect[nCntBillboardEffect].col.a -= g_BillboardEffect[nCntBillboardEffect].coladown;// �����x��������

						if (g_BillboardEffect[nCntBillboardEffect].col.a <= 0.0f)
						{// �����ɂȂ�����
							g_BillboardEffect[nCntBillboardEffect].col.a = 0.0f;
							DeleteBillboardEffect(nCntBillboardEffect);			// ����
						}
					}
				}

				if (g_BillboardEffect[nCntBillboardEffect].nType == EFFECTBULLET_PLAYER && pXPlayer->nWeapon_3Lv >= 6 && nEnemyNum > 0 && g_BillboardEffect[nCntBillboardEffect].nLife < 100)
				{// �z�[�~���O
					D3DXVECTOR3 enemypos = GetRockOnXEnemy(1);
					// �ǔ�
					g_BillboardEffect[nCntBillboardEffect].move.x = sinf(atan2f(enemypos.x - g_BillboardEffect[nCntBillboardEffect].pos.x, enemypos.z - g_BillboardEffect[nCntBillboardEffect].pos.z)) * 5.0f;
					g_BillboardEffect[nCntBillboardEffect].move.z = cosf(atan2f(enemypos.x - g_BillboardEffect[nCntBillboardEffect].pos.x, enemypos.z - g_BillboardEffect[nCntBillboardEffect].pos.z)) * 5.0f;
				}

				g_BillboardEffect[nCntBillboardEffect].pos += g_BillboardEffect[nCntBillboardEffect].move;	// �ړ�����

				if (g_BillboardEffect[nCntBillboardEffect].pos.x > 1750.0f || g_BillboardEffect[nCntBillboardEffect].pos.x < -1750.0f ||
					g_BillboardEffect[nCntBillboardEffect].pos.z > 1750.0f || g_BillboardEffect[nCntBillboardEffect].pos.z < -1750.0f)
				{// �͈̓`�F�b�N
					DeleteBillboardEffect(nCntBillboardEffect);					// ����
				}
			}
			else
			{// �e�ȊO
				g_BillboardEffect[nCntBillboardEffect].col.a -= g_BillboardEffect[nCntBillboardEffect].coladown;		// �����x��������

				if (g_BillboardEffect[nCntBillboardEffect].col.a <= 0.0f)
				{// �����ɂȂ�����
					g_BillboardEffect[nCntBillboardEffect].col.a = 0.0f;
					DeleteBillboardEffect(nCntBillboardEffect);					// ����
				}
				else if (g_BillboardEffect[nCntBillboardEffect].type == EFFECTTYPE_NORMAL)
				{// �ʏ�
					//SetPositionShadow(g_BillboardEffect[nCntBillboardEffect].nIdxShadow, g_BillboardEffect[nCntBillboardEffect].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				}

				if (g_BillboardEffect[nCntBillboardEffect].type == EFFECTTYPE_PARTICLE)
				{// �p�[�e�B�N��
					g_BillboardEffect[nCntBillboardEffect].pos += g_BillboardEffect[nCntBillboardEffect].move;
				}
			}

			// ���_�J���[�̐ݒ�
			pVtx[nCntBillboardEffect * 4].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 1].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 2].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 3].col = g_BillboardEffect[nCntBillboardEffect].col;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboardEffect->Unlock();
}

//=========================================================================================================================
// �G�t�F�N�g�`�揈��
//=========================================================================================================================
void DrawBillboardEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	int nCntBillboardEffect;

	// �A���t�@�e�X�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Z���C�g�̐ݒ�
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{
		if (g_BillboardEffect[nCntBillboardEffect].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_BillboardEffect[nCntBillboardEffect].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// �t�s��
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._11 = mtxView._11;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._12 = mtxView._21;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._13 = mtxView._31;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._21 = mtxView._12;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._22 = mtxView._22;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._23 = mtxView._32;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._31 = mtxView._13;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._32 = mtxView._23;
			g_BillboardEffect[nCntBillboardEffect].mtxWorld._33 = mtxView._33;

			// �ʒu�𔽉f
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_BillboardEffect[nCntBillboardEffect].pos.x,
				g_BillboardEffect[nCntBillboardEffect].pos.y,
				g_BillboardEffect[nCntBillboardEffect].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_BillboardEffect[nCntBillboardEffect].mtxWorld,
				&g_BillboardEffect[nCntBillboardEffect].mtxWorld,
				&mtxTrans
			);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_BillboardEffect[nCntBillboardEffect].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource
			(
				0,
				g_pVtxBuffBillboardEffect,
				0,
				sizeof(VERTEX_3D)
			);

			// ���_�t�H�[�}�b�g
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureBillboardEffect);

			// �v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboardEffect * 4, 2);
		}
	}

	// Z���C�g
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �A���t�@�e�X�g�𖳌�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=========================================================================================================================
// �m�[�}���G�t�F�N�g�̐ݒ菈��
//=========================================================================================================================
void SetBillboardNormalEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown)
{
	int nCntBillboardEffect;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// �J�E���g
		if (g_BillboardEffect[nCntBillboardEffect].bUse == false)
		{// �g�p���Ă��Ȃ�������
			g_BillboardEffect[nCntBillboardEffect].pos = pos;
			g_BillboardEffect[nCntBillboardEffect].fRadius = fRadius;
			g_BillboardEffect[nCntBillboardEffect].coladown = coladown;
			g_BillboardEffect[nCntBillboardEffect].col = col;
			g_BillboardEffect[nCntBillboardEffect].type = EFFECTTYPE_NORMAL;
			g_BillboardEffect[nCntBillboardEffect].bUse = true;

			// ���_���̐ݒ�
			pVtx[nCntBillboardEffect * 4].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 1].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 2].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 3].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			// ���_�J���[�̐ݒ�
			pVtx[nCntBillboardEffect * 4].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 1].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 2].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 3].col = g_BillboardEffect[nCntBillboardEffect].col;

			//g_BillboardEffect[nCntBillboardEffect].nIdxShadow = SetShadow(g_BillboardEffect[nCntBillboardEffect].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 4.0f));

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboardEffect->Unlock();
}

//=========================================================================================================================
// �e�G�t�F�N�g�̐ݒ菈��
//=========================================================================================================================
void SetBillboardBulletEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown, int nLife, int nDamage, int nType)
{
	int nCntBillboardEffect;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

						// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// �J�E���g
		if (g_BillboardEffect[nCntBillboardEffect].bUse == false)
		{// �g�p���Ă��Ȃ�������
			g_BillboardEffect[nCntBillboardEffect].pos = pos;					// �ʒu
			g_BillboardEffect[nCntBillboardEffect].rot = rot;					// ����
			g_BillboardEffect[nCntBillboardEffect].move = move;					// �ړ���
			g_BillboardEffect[nCntBillboardEffect].fRadius = fRadius;			// �傫��
			g_BillboardEffect[nCntBillboardEffect].coladown = coladown;			// �F���Y��
			g_BillboardEffect[nCntBillboardEffect].col = col;					// �F
			g_BillboardEffect[nCntBillboardEffect].nLife = nLife;				// �̗�
			g_BillboardEffect[nCntBillboardEffect].type = EFFECTTYPE_BULLET;	// ���
			g_BillboardEffect[nCntBillboardEffect].nDamage = nDamage;			// �_���[�W
			g_BillboardEffect[nCntBillboardEffect].nType = nType;				// �e���
			g_BillboardEffect[nCntBillboardEffect].bUse = true;					// �g�p���Ă���

			// ���_���̐ݒ�
			pVtx[nCntBillboardEffect * 4].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 1].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 2].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 3].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			// ���_�J���[�̐ݒ�
			pVtx[nCntBillboardEffect * 4].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 1].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 2].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 3].col = g_BillboardEffect[nCntBillboardEffect].col;

			//g_BillboardEffect[nCntBillboardEffect].nIdxShadow = SetShadow(g_BillboardEffect[nCntBillboardEffect].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 4.0f));

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboardEffect->Unlock();
}

//=========================================================================================================================
// �p�[�e�B�N���G�t�F�N�g�̐ݒ菈��
//=========================================================================================================================
void SetBillboardParticleEffect(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 fRadius, float coladown, int nMaxSet, EFFECTSTATE state)
{
	int nCntBillboardEffect, nCntSet = 0;

	//srand((unsigned int)time(0));

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// �J�E���g
		if (g_BillboardEffect[nCntBillboardEffect].bUse == false)
		{// �g�p���Ă��Ȃ�������
			g_BillboardEffect[nCntBillboardEffect].pos = pos;
			g_BillboardEffect[nCntBillboardEffect].fRadius = fRadius;
			g_BillboardEffect[nCntBillboardEffect].col = col;
			g_BillboardEffect[nCntBillboardEffect].coladown = coladown;
			g_BillboardEffect[nCntBillboardEffect].type = EFFECTTYPE_PARTICLE;
			g_BillboardEffect[nCntBillboardEffect].bUse = true;

			// �ړ������_����
			g_BillboardEffect[nCntBillboardEffect].move.x = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			g_BillboardEffect[nCntBillboardEffect].move.y = cosf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);
			g_BillboardEffect[nCntBillboardEffect].move.z = sinf((rand() % 628) / 100.0f) * ((rand() % 1) + 1);

			g_BillboardEffect[nCntBillboardEffect].move.y += 1;

			int nNumber = rand() % 3 + 1;

			// col�l���
			if (nNumber == 1) { g_BillboardEffect[nCntBillboardEffect].col = D3DXCOLOR(0.8f, 0.3f, 0.0f, 0.8f); }
			if (nNumber == 2) { g_BillboardEffect[nCntBillboardEffect].col = D3DXCOLOR(0.2f, 0.8f, 0.0f, 0.8f); }
			if (nNumber == 3) { g_BillboardEffect[nCntBillboardEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f); }

			int nAngle = (rand() % (int)fRadius.x + 1);
			g_BillboardEffect[nCntBillboardEffect].fRadius.x = (nAngle) * 1.0f;	// ���a�ݒ�
			g_BillboardEffect[nCntBillboardEffect].fRadius.y = (nAngle) * 1.0f;	// ���a�ݒ�

			// ���_���̐ݒ�
			pVtx[nCntBillboardEffect * 4].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 1].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 2].pos = D3DXVECTOR3(-g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			pVtx[nCntBillboardEffect * 4 + 3].pos = D3DXVECTOR3(g_BillboardEffect[nCntBillboardEffect].fRadius.x, -g_BillboardEffect[nCntBillboardEffect].fRadius.y, 0.0f);
			// ���_�J���[�̐ݒ�
			pVtx[nCntBillboardEffect * 4].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 1].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 2].col = g_BillboardEffect[nCntBillboardEffect].col;
			pVtx[nCntBillboardEffect * 4 + 3].col = g_BillboardEffect[nCntBillboardEffect].col;

			// �e���Z�b�g
			//g_BillboardEffect[nCntBillboardEffect].nIdxShadow = SetShadow(g_BillboardEffect[nCntBillboardEffect].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 0.0f, 4.0f));

			if (state != EFFECTSTATE_LOOP)
			{// ���[�v�ȊO
				nCntSet++;		// �����J�E���g
				if (nCntSet == nMaxSet)
				{// �����ő吔�ɂȂ����ꍇ
					break;
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboardEffect->Unlock();
}

//=========================================================================================================================
// �G�t�F�N�g�̓G����ԍ��擾����
//=========================================================================================================================
void SetXEnemyCollNum(int nXEnemy, int nXEnemyNum)
{
	g_nXEnemy = nXEnemy;
	g_nXEnemyNum = nXEnemyNum;
}

//=========================================================================================================================
// �G�t�F�N�g�̏Փ˔��菈��
//=========================================================================================================================
bool CollisionEffect(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMax, D3DXVECTOR3 *vtxMin, int nType)
{
	GAMESTATE game = GetGameState();
	XPLAYER *pXPlayer = GetXPlayer();

	int nCntBillboardEffect;
	bool bLand = false;

	int nType2 = nType;
	if (nType == EFFECTBULLET_PLAYER)
	{
		nType2 = EFFECTBULLET_PLAYER2;
	}

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// �J�E���g
		if (g_BillboardEffect[nCntBillboardEffect].bUse == true && g_BillboardEffect[nCntBillboardEffect].type == EFFECTTYPE_BULLET && (g_BillboardEffect[nCntBillboardEffect].nType == nType || g_BillboardEffect[nCntBillboardEffect].nType == nType2))
		{// �g�p���Ă���
 			if (g_BillboardEffect[nCntBillboardEffect].pos.x < pPos->x + vtxMax->x && g_BillboardEffect[nCntBillboardEffect].pos.x > pPos->x + vtxMin->x &&
				g_BillboardEffect[nCntBillboardEffect].pos.z < pPos->z + vtxMax->z && g_BillboardEffect[nCntBillboardEffect].pos.z > pPos->z + vtxMin->z)
			{// ����������

				if (game == GAMESTATE_PLAY)
				{// �Q�[���v���C���Ȃ�
					if (g_BillboardEffect[nCntBillboardEffect].nType == EFFECTBULLET_PLAYER ||
						g_BillboardEffect[nCntBillboardEffect].nType == EFFECTBULLET_PLAYER2)
					{// �v���C���[
						HitXEnemy(g_nXEnemy, g_nXEnemyNum, g_BillboardEffect[nCntBillboardEffect].nDamage);
					}
					else if (g_BillboardEffect[nCntBillboardEffect].nType == EFFECTBULLET_ENEMY)
					{// �G
						pXPlayer->nLife -= g_BillboardEffect[nCntBillboardEffect].nDamage;
					}

					// �m�b�N�o�b�N
					pMove->x = sinf(g_BillboardEffect[nCntBillboardEffect].rot.y) * -5.0f;
					pMove->z = cosf(g_BillboardEffect[nCntBillboardEffect].rot.y) * -5.0f;
				}

				DeleteBillboardEffect(nCntBillboardEffect);
				bLand = true;
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// �G�t�F�N�g�̔j������
//=========================================================================================================================
void DeleteBillboardEffect(int nCntBillboardEffect)
{// 1��
	if (g_BillboardEffect[nCntBillboardEffect].bUse == true)
	{// �g�p���Ă�����
		g_BillboardEffect[nCntBillboardEffect].bUse = false;

		DeleteShadow(g_BillboardEffect[nCntBillboardEffect].nIdxShadow);
	}
}
void DeleteBillboardEffect(void)
{// �S��
	int nCntBillboardEffect;

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{// �J�E���g
		if (g_BillboardEffect[nCntBillboardEffect].bUse == true)
		{// �g�p���Ă�����
			g_BillboardEffect[nCntBillboardEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardEffect[nCntBillboardEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardEffect[nCntBillboardEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardEffect[nCntBillboardEffect].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardEffect[nCntBillboardEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			g_BillboardEffect[nCntBillboardEffect].coladown = 0.0f;
			g_BillboardEffect[nCntBillboardEffect].nLife = 0;
			g_BillboardEffect[nCntBillboardEffect].type = EFFECTTYPE_NONE;
			g_BillboardEffect[nCntBillboardEffect].nIdxShadow = 0;
			g_BillboardEffect[nCntBillboardEffect].nDamage = 0;
			g_BillboardEffect[nCntBillboardEffect].nType = 0;
			g_BillboardEffect[nCntBillboardEffect].bUse = false;
		}
	}
}

//=========================================================================================================================
// ���_���̍쐬
//=========================================================================================================================
void MakeVertexBillboardEffect(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntBillboardEffect;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BILLBOARDEFFECT_TEXTURE_NAME1, &g_pTextureBillboardEffect);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARDEFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboardEffect,
		NULL
	);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardEffect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardEffect = 0; nCntBillboardEffect < MAX_BILLBOARDEFFECT; nCntBillboardEffect++)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboardEffect->Unlock();
}

