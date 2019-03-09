//=========================================================================================================================
//
// �I�u�W�F�N�g�̏��� [billboardobject.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "billboardobject.h"	// �I�u�W�F�N�g
#include "shadow.h"				// �e
#include "xplayer.h"			// �v���C���[
#include "score.h"				// �X�R�A
#include "sound.h"				// �T�E���h
#include "input.h"				// ����

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_BILLBOARDOBJECT					(256)									// �I�u�W�F�N�g�̍ő吔
#define BILLBOARDOBJECT_TEXTURE_UV_U		(1.0f)									// �e�N�X�`���A�j���[�V����U�͈�
#define BILLBOARDOBJECT_TEXTURE_UV_V		(1.0f)									// �e�N�X�`���A�j���[�V����V�͈�

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void MakeVertexBillboardObject(LPDIRECT3DDEVICE9 pDevice);							// ���_���̐ݒ�

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9							g_pTextureBillboardObject[BILLBOARDTEXTURE_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9						g_pVtxBuffBillboardObject = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
BillboardObject								g_BillboardObject[MAX_BILLBOARDOBJECT];	// �I�u�W�F�N�g�̏��

//=========================================================================================================================
// �I�u�W�F�N�g�̏���������
//=========================================================================================================================
void InitBillboardObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntBillboardObject;

	// ���_���̍쐬
	MakeVertexBillboardObject(pDevice);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// �J�E���g
		g_BillboardObject[nCntBillboardObject].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].fTex = 0.0f;
		g_BillboardObject[nCntBillboardObject].fInitRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BillboardObject[nCntBillboardObject].nTex = 0;
		g_BillboardObject[nCntBillboardObject].nIdxShadow = 0;
		g_BillboardObject[nCntBillboardObject].bUse = false;
	}
}

//=========================================================================================================================
// �I�u�W�F�N�g�̏I������
//=========================================================================================================================
void UninitBillboardObject(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < BILLBOARDTEXTURE_MAX; nCntTex++)
	{// �e�N�X�`���J�E���g
		if (g_pTextureBillboardObject[nCntTex] != NULL)
		{
			g_pTextureBillboardObject[nCntTex]->Release();
			g_pTextureBillboardObject[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboardObject != NULL)
	{
		g_pVtxBuffBillboardObject->Release();
		g_pVtxBuffBillboardObject = NULL;
	}
}

//=========================================================================================================================
// �I�u�W�F�N�g�̍X�V����
//=========================================================================================================================
void UpdateBillboardObject(void)
{
	int nCntBillboardObject;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// �J�E���g
		if (g_BillboardObject[nCntBillboardObject].bUse == true)
		{// �g�p���Ă�����
			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_3)
			{// ���@�w
				if (g_BillboardObject[nCntBillboardObject].fRadius.x != 150 &&
					g_BillboardObject[nCntBillboardObject].fRadius.z != 150)
				{
					g_BillboardObject[nCntBillboardObject].fRadius.x += 15;
					g_BillboardObject[nCntBillboardObject].fRadius.z += 15;
				}
			}
			else if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_MAP_WALL)
			{// ��
				g_BillboardObject[nCntBillboardObject].fTex -= 0.002f;
			}
			else if (
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE)
			{// �A�C�e��
				g_BillboardObject[nCntBillboardObject].rot.y += 0.03f;
			}

			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD)
			{// ��
				g_BillboardObject[nCntBillboardObject].posOld = g_BillboardObject[nCntBillboardObject].pos;

				g_BillboardObject[nCntBillboardObject].pos += g_BillboardObject[nCntBillboardObject].move;
			}

			if (g_BillboardObject[nCntBillboardObject].pos.x > 1750.0f || g_BillboardObject[nCntBillboardObject].pos.x < -1750.0f ||
				g_BillboardObject[nCntBillboardObject].pos.z > 1750.0f || g_BillboardObject[nCntBillboardObject].pos.z < -1750.0f)
			{// �͈̓`�F�b�N
				DeleteBillboardObject(nCntBillboardObject);
			}

			// ���_���̐ݒ�
			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_3)
			{// ���@�w
				pVtx[nCntBillboardObject * 4].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 1].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, -g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 3].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, -g_BillboardObject[nCntBillboardObject].fRadius.z);
			}
			else if (g_BillboardObject[nCntBillboardObject].nTex != BILLBOARDTEXTURE_HP)
			{// �̗�
				pVtx[nCntBillboardObject * 4].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
				pVtx[nCntBillboardObject * 4 + 1].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
				pVtx[nCntBillboardObject * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
				pVtx[nCntBillboardObject * 4 + 3].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
			}
			// �e�N�X�`���̐ݒ�
			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_MAP_WALL)
			{// ��
				pVtx[nCntBillboardObject * 4].tex = D3DXVECTOR2(g_BillboardObject[nCntBillboardObject].fTex, 0.0f);
				pVtx[nCntBillboardObject * 4 + 1].tex = D3DXVECTOR2(g_BillboardObject[nCntBillboardObject].fTex + 1.0f, 0.0f);
				pVtx[nCntBillboardObject * 4 + 2].tex = D3DXVECTOR2(g_BillboardObject[nCntBillboardObject].fTex, 1.0f);
				pVtx[nCntBillboardObject * 4 + 3].tex = D3DXVECTOR2(g_BillboardObject[nCntBillboardObject].fTex + 1.0f, 1.0f);
			}
			else
			{// ���̑�
				pVtx[nCntBillboardObject * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCntBillboardObject * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[nCntBillboardObject * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[nCntBillboardObject * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboardObject->Unlock();
}

//=========================================================================================================================
// �I�u�W�F�N�g�̕`�揈��
//=========================================================================================================================
void DrawBillboardObject(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// �v�Z�p�}�g���b�N�X
	int nCntBillboardObject;

	// �A���t�@�e�X�g�̐ݒ�
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Z���C�g�̐ݒ�
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// �J�E���g
		if (g_BillboardObject[nCntBillboardObject].bUse == true)
		{// �g�p���Ă�����
			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ROCKON ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD)
			{// ���b�N�I���A�a��
				// �A���t�@�e�X�g�̐ݒ�
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 100);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			}
			else if (
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE)
			{// �A�C�e��
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// ����
			}

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_BillboardObject[nCntBillboardObject].mtxWorld);

			// �r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD)
			{
				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll
				(
					&mtxRot,
					g_BillboardObject[nCntBillboardObject].rot.y,
					g_BillboardObject[nCntBillboardObject].rot.x,
					g_BillboardObject[nCntBillboardObject].rot.z
				);

				D3DXMatrixMultiply
				(
					&g_BillboardObject[nCntBillboardObject].mtxWorld,
					&g_BillboardObject[nCntBillboardObject].mtxWorld,
					&mtxRot
				);
			}

			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ROCKON ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_HP ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD)
			{
				// �t�s��
				g_BillboardObject[nCntBillboardObject].mtxWorld._11 = mtxView._11;
				g_BillboardObject[nCntBillboardObject].mtxWorld._12 = mtxView._21;
				g_BillboardObject[nCntBillboardObject].mtxWorld._13 = mtxView._31;
				g_BillboardObject[nCntBillboardObject].mtxWorld._21 = mtxView._12;
				g_BillboardObject[nCntBillboardObject].mtxWorld._22 = mtxView._22;
				g_BillboardObject[nCntBillboardObject].mtxWorld._23 = mtxView._32;
				g_BillboardObject[nCntBillboardObject].mtxWorld._31 = mtxView._13;
				g_BillboardObject[nCntBillboardObject].mtxWorld._32 = mtxView._23;
				g_BillboardObject[nCntBillboardObject].mtxWorld._33 = mtxView._33;
			}

			// �ʒu�𔽉f
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_BillboardObject[nCntBillboardObject].pos.x,
				g_BillboardObject[nCntBillboardObject].pos.y,
				g_BillboardObject[nCntBillboardObject].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_BillboardObject[nCntBillboardObject].mtxWorld,
				&g_BillboardObject[nCntBillboardObject].mtxWorld,
				&mtxTrans
			);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_BillboardObject[nCntBillboardObject].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource
			(
				0,
				g_pVtxBuffBillboardObject,
				0,
				sizeof(VERTEX_3D)
			);

			// ���_�t�H�[�}�b�g
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���ݒ�
			if (g_BillboardObject[nCntBillboardObject].nTex >= BILLBOARDTEXTURE_MAX)
			{
				pDevice->SetTexture(0, NULL);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureBillboardObject[g_BillboardObject[nCntBillboardObject].nTex]);
			}

			// �v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboardObject * 4, 2);

			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ROCKON ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD)
			{
				// �A���t�@�e�X�g�𖳌�
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
			else if (
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE)
			{
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O
			}
		}
	}

	// Z���C�g�̖���
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	// �A���t�@�e�X�g�𖳌�
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=========================================================================================================================
// �I�u�W�F�N�g�̐ݒ菈��
//=========================================================================================================================
void SetBillboardObject(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, D3DXVECTOR3 fRadius, int nTex)
{
	int nCntBillboardObject;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// �J�E���g
		if (g_BillboardObject[nCntBillboardObject].bUse == false)
		{// �g�p���Ă��Ȃ�������
			g_BillboardObject[nCntBillboardObject].pos = pos;
			g_BillboardObject[nCntBillboardObject].move = move;
			g_BillboardObject[nCntBillboardObject].rot = rot;
			g_BillboardObject[nCntBillboardObject].col = col;
			g_BillboardObject[nCntBillboardObject].fRadius = fRadius;
			g_BillboardObject[nCntBillboardObject].fTex = 0.0f;
			g_BillboardObject[nCntBillboardObject].nTex = nTex;
			g_BillboardObject[nCntBillboardObject].bUse = true;

			if (
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE)
			{// �A�C�e��
				g_BillboardObject[nCntBillboardObject].nIdxShadow = SetShadow(g_BillboardObject[nCntBillboardObject].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(30.0f, 0.0f, 30.0f));
			}

			// ���_���̐ݒ�
			if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_1 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_2 ||
				g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_WAND_3)
			{// ���@�w
				pVtx[nCntBillboardObject * 4].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 1].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, -g_BillboardObject[nCntBillboardObject].fRadius.z);
				pVtx[nCntBillboardObject * 4 + 3].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, -g_BillboardObject[nCntBillboardObject].fRadius.z);
			}
			else
			{
				pVtx[nCntBillboardObject * 4].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
				pVtx[nCntBillboardObject * 4 + 1].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
				pVtx[nCntBillboardObject * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
				pVtx[nCntBillboardObject * 4 + 3].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
			}
			// ���_�J���[�̐ݒ�
			pVtx[nCntBillboardObject * 4].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 1].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 2].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 3].col = g_BillboardObject[nCntBillboardObject].col;
			// �e�N�X�`���̐ݒ�
			pVtx[nCntBillboardObject * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCntBillboardObject * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboardObject->Unlock();
}
int SetHPNum(D3DXVECTOR3 pos, D3DXVECTOR3 fRadius)
{// HP
	int nCntBillboardObject;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// �J�E���g
		if (g_BillboardObject[nCntBillboardObject].bUse == false)
		{// �g�p���Ă��Ȃ�������
			g_BillboardObject[nCntBillboardObject].pos = pos;
			g_BillboardObject[nCntBillboardObject].fRadius = fRadius;
			g_BillboardObject[nCntBillboardObject].fInitRadius = fRadius * 2;

			g_BillboardObject[nCntBillboardObject].nTex = BILLBOARDTEXTURE_HP;
			g_BillboardObject[nCntBillboardObject].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_BillboardObject[nCntBillboardObject].bUse = true;

			// ���_���̐ݒ�
			pVtx[nCntBillboardObject * 4].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
			pVtx[nCntBillboardObject * 4 + 1].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, g_BillboardObject[nCntBillboardObject].fRadius.y, 0.0f);
			pVtx[nCntBillboardObject * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 3].pos = D3DXVECTOR3(g_BillboardObject[nCntBillboardObject].fRadius.x, 0.0f, 0.0f);
			// ���_�J���[�̐ݒ�
			pVtx[nCntBillboardObject * 4].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 1].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 2].col = g_BillboardObject[nCntBillboardObject].col;
			pVtx[nCntBillboardObject * 4 + 3].col = g_BillboardObject[nCntBillboardObject].col;
			// �e�N�X�`���̐ݒ�
			pVtx[nCntBillboardObject * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCntBillboardObject * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//g_BillboardObject[nCntBillboardObject].nIdxShadow = SetShadow(g_BillboardObject[nCntBillboardObject].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 50.0f));

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboardObject->Unlock();

	return nCntBillboardObject;
}

//=========================================================================================================================
// ���b�N�I���̈ʒu�X�V����
//=========================================================================================================================
void SetPosionRockOn(D3DXVECTOR3 pos)
{
	int nCntBillboardObject;

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// �J�E���g
		if (g_BillboardObject[nCntBillboardObject].bUse == true && g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ROCKON)
		{// ���b�N�I�����g�p����Ă�����
			g_BillboardObject[nCntBillboardObject].pos = pos;
		}
	}
}

//=========================================================================================================================
// HP�̈ʒu�X�V����
//=========================================================================================================================
void SetPosionHP(D3DXVECTOR3 pos, int nBillBoardHP)
{
	if (g_BillboardObject[nBillBoardHP].bUse == true && g_BillboardObject[nBillBoardHP].nTex == BILLBOARDTEXTURE_HP)
	{
		g_BillboardObject[nBillBoardHP].pos = pos;
	}
}

//=========================================================================================================================
// HP�̍X�V����
//=========================================================================================================================
void SetUpdateHP(int nBillBoardHP, int nInitHP, int nNowHP)
{
	float fHP;
	float fHPRadius;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if (g_BillboardObject[nBillBoardHP].bUse == true)
	{
		// HP����
		fHP = ((float)nNowHP / (float)nInitHP);

		// HP�T�C�Y
		fHPRadius = g_BillboardObject[nBillBoardHP].fInitRadius.x * fHP;

		// ���_���̐ݒ�
		pVtx[nBillBoardHP * 4].pos = D3DXVECTOR3(-g_BillboardObject[nBillBoardHP].fRadius.x, g_BillboardObject[nBillBoardHP].fRadius.y, 0.0f);
		pVtx[nBillBoardHP * 4 + 1].pos = D3DXVECTOR3(fHPRadius - g_BillboardObject[nBillBoardHP].fRadius.x, g_BillboardObject[nBillBoardHP].fRadius.y, 0.0f);
		pVtx[nBillBoardHP * 4 + 2].pos = D3DXVECTOR3(-g_BillboardObject[nBillBoardHP].fRadius.x, 0.0f, 0.0f);
		pVtx[nBillBoardHP * 4 + 3].pos = D3DXVECTOR3(fHPRadius - g_BillboardObject[nBillBoardHP].fRadius.x, 0.0f, 0.0f);
		// �e�N�X�`���̐ݒ�
		pVtx[nBillBoardHP * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nBillBoardHP * 4 + 1].tex = D3DXVECTOR2(fHP, 0.0f);
		pVtx[nBillBoardHP * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nBillBoardHP * 4 + 3].tex = D3DXVECTOR2(fHP, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboardObject->Unlock();
}

//=========================================================================================================================
// �A�C�e���̎擾���菈��
//=========================================================================================================================
bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMax, D3DXVECTOR3 *vtxMin)
{
	int nCntBillboardObject;
	bool bLand = false;

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// �J�E���g
		if (g_BillboardObject[nCntBillboardObject].bUse == true && (
			g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1 ||
			g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2 ||
			g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3 ||
			g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE))
		{// �A�C�e���̂�
			if (g_BillboardObject[nCntBillboardObject].pos.x < pPos->x + vtxMax->x && g_BillboardObject[nCntBillboardObject].pos.x > pPos->x + vtxMin->x &&
				g_BillboardObject[nCntBillboardObject].pos.z < pPos->z + vtxMax->z && g_BillboardObject[nCntBillboardObject].pos.z > pPos->z + vtxMin->z)
			{// ��������
				bLand = true;

				AddScore(SCORETYPE_SCORE, 300);

				PlaySound(SOUND_LABEL_SE_ITEM);

				if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_1)
				{
					SetHitItem(0);
				}
				else if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_2)
				{
					SetHitItem(1);
				}
				else if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_WEAPON_3)
				{
					SetHitItem(2);
				}
				else if (g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_ITEM_LIFE)
				{
					SetHitItem(3);
				}

				DeleteBillboardObject(nCntBillboardObject);
				DeleteShadow(g_BillboardObject[nCntBillboardObject].nIdxShadow);

				break;
			}
		}
	}

	return bLand;
}
bool CollisionSword(D3DXVECTOR3 *pPos)
{
	int nCntBillboardObject;
	bool bLand = false;

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// �J�E���g
		if (g_BillboardObject[nCntBillboardObject].bUse == true && (
			g_BillboardObject[nCntBillboardObject].nTex == BILLBOARDTEXTURE_SWORD))
		{// �a�����g�p����Ă�����
			if (g_BillboardObject[nCntBillboardObject].pos.x - g_BillboardObject[nCntBillboardObject].fRadius.x < pPos->x && g_BillboardObject[nCntBillboardObject].pos.x + g_BillboardObject[nCntBillboardObject].fRadius.x > pPos->x)
			{
				if (g_BillboardObject[nCntBillboardObject].posOld.z > pPos->z && pPos->z > g_BillboardObject[nCntBillboardObject].pos.z)
				{
					bLand = true;

					break;
				}
				else if (g_BillboardObject[nCntBillboardObject].posOld.z < pPos->z && pPos->z < g_BillboardObject[nCntBillboardObject].pos.z)
				{
					bLand = true;

					break;
				}
			}

			if (g_BillboardObject[nCntBillboardObject].pos.z - g_BillboardObject[nCntBillboardObject].fRadius.x < pPos->z && g_BillboardObject[nCntBillboardObject].pos.z + g_BillboardObject[nCntBillboardObject].fRadius.x > pPos->z)
			{
				if (g_BillboardObject[nCntBillboardObject].posOld.x > pPos->x && pPos->x > g_BillboardObject[nCntBillboardObject].pos.x)
				{
					bLand = true;

					break;
				}
				else if (g_BillboardObject[nCntBillboardObject].posOld.x < pPos->x && pPos->x < g_BillboardObject[nCntBillboardObject].pos.x)
				{
					bLand = true;

					break;
				}
			}
		}
	}

	return bLand;
}


//=========================================================================================================================
// �I�u�W�F�N�g�̔j������
//=========================================================================================================================
void DeleteBillboardObject(int nCntBillboardObject)
{// �P��
	if (g_BillboardObject[nCntBillboardObject].bUse == true)
	{
		g_BillboardObject[nCntBillboardObject].fTex = 0.0f;
		g_BillboardObject[nCntBillboardObject].bUse = false;
		//DeleteShadow(g_BillboardObject[nCntBillboardObject].nIdxShadow);
	}
}
void DeleteBillboardObject(BILLBOARDTEXTURE nTex)
{// �e�N�X�`��
	int nCntBillboardObject;

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{
		if (g_BillboardObject[nCntBillboardObject].bUse == true && g_BillboardObject[nCntBillboardObject].nTex == nTex)
		{
			g_BillboardObject[nCntBillboardObject].fTex = 0.0f;
			g_BillboardObject[nCntBillboardObject].bUse = false;
			//DeleteShadow(g_BillboardObject[nCntBillboardObject].nIdxShadow);
		}
	}
}
void DeleteBillboardObject(void)
{// �S��
	int nCntBillboardObject;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
	{// �J�E���g
		if (g_BillboardObject[nCntBillboardObject].bUse == true)
		{// �g�p���Ă�����
			g_BillboardObject[nCntBillboardObject].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].fInitRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_BillboardObject[nCntBillboardObject].nTex = 0;
			g_BillboardObject[nCntBillboardObject].nIdxShadow = 0;
			g_BillboardObject[nCntBillboardObject].bUse = false;

			// �e�N�X�`���̐ݒ�
			pVtx[nCntBillboardObject * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCntBillboardObject * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCntBillboardObject * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboardObject->Unlock();
}

//=========================================================================================================================
// ���_���̍쐬
//=========================================================================================================================
void MakeVertexBillboardObject(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntBillboardObject;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME1, &g_pTextureBillboardObject[0]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME2, &g_pTextureBillboardObject[1]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME3, &g_pTextureBillboardObject[2]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME4, &g_pTextureBillboardObject[3]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME5, &g_pTextureBillboardObject[4]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME6, &g_pTextureBillboardObject[5]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME7, &g_pTextureBillboardObject[6]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME8, &g_pTextureBillboardObject[7]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME9, &g_pTextureBillboardObject[8]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME10, &g_pTextureBillboardObject[9]);
	D3DXCreateTextureFromFile(pDevice, BILLBOARDOBJECT_TEXTURE_NAME11, &g_pTextureBillboardObject[10]);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARDOBJECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboardObject,
		NULL
	);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBillboardObject->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBillboardObject = 0; nCntBillboardObject < MAX_BILLBOARDOBJECT; nCntBillboardObject++)
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
	g_pVtxBuffBillboardObject->Unlock();
}

