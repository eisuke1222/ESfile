//=========================================================================================================================
//
// ���폈�� [xplayer.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanf�G���[����
#include <stdio.h>					// stdio
#include "xweapon.h"
#include "xplayer.h"
#include "xenemy.h"
#include "xobject.h"
#include "shadow.h"
#include "billboardeffect.h"
#include "camera.h"
#include "fade.h"
#include "input.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
XWEAPONMODEL					g_XWeapon[MAX_XWEAPON];		// ���핐����
int								g_MaxXWeapon;				// ����p�[�c��
int								g_nCntTimeXWeapon;			// ����J�E���^�[

//=========================================================================================================================
// ����̏���������
//=========================================================================================================================
void InitXWeapon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	XPLAYER *pXPlayer = GetXPlayer();
	FILE *pFile;
	int nCntXWeapon;

	g_MaxXWeapon = 0;		// ����p�[�c��
	g_nCntTimeXWeapon = 0;	// ����J�E���^�[

	// �t�@�C���ݒ�
	pFile = fopen(XWEAPON_TEXT_INFO, "r");

	// ����e�L�X�g�f�[�^�ǂݍ���
	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fscanf(pFile, "%d", &g_MaxXWeapon);	// �ǂݍ���

		for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
		{
			fscanf(pFile, "%s", &g_XWeapon[nCntXWeapon].FileName[0]);	// �ǂݍ���
		}
		
		for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
		{
			fscanf(pFile, "%d", &g_XWeapon[nCntXWeapon].nIndexParent);	// �ǂݍ���
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initpos.x);	// �ǂݍ���
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initpos.y);	// �ǂݍ���
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initpos.z);	// �ǂݍ���
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initrot.x);	// �ǂݍ���
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initrot.y);	// �ǂݍ���
			fscanf(pFile, "%f", &g_XWeapon[nCntXWeapon].Initrot.z);	// �ǂݍ���
		}

		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}

	// X�t�@�C���̓ǂݍ���
	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		D3DXLoadMeshFromX
		(
			&g_XWeapon[nCntXWeapon].FileName[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_XWeapon[nCntXWeapon].pBuffMat,
			NULL,
			&g_XWeapon[nCntXWeapon].nNumMat,
			&g_XWeapon[nCntXWeapon].pMesh
		);
	}

	// �ʒu�E�����̏����ݒ�
	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		g_XWeapon[nCntXWeapon].pos = g_XWeapon[nCntXWeapon].Initpos;
		g_XWeapon[nCntXWeapon].rot = g_XWeapon[nCntXWeapon].Initrot;
		g_XWeapon[nCntXWeapon].vtxMin = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		g_XWeapon[nCntXWeapon].vtxMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
		g_XWeapon[nCntXWeapon].nHitNum = 0;
		g_XWeapon[nCntXWeapon].nType = WEAPON_NONE;
		g_XWeapon[nCntXWeapon].bUse = false;
	}

	int nNumVtx;		// ���_��
	int nIndexParent;	// �e�ԍ�
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		// ���_�o�b�t�@�̃|�C���^

	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		// ���_���̎擾
		nNumVtx = g_XWeapon[nCntXWeapon].pMesh->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_XWeapon[nCntXWeapon].pMesh->GetFVF());

		// ���_�o�b�t�@�����b�N
		g_XWeapon[nCntXWeapon].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// ���_���W�̑��
			{
				if (vtx.x > g_XWeapon[nCntXWeapon].vtxMax.x)
				{
					g_XWeapon[nCntXWeapon].vtxMax.x = vtx.x;
				}
				if (vtx.y > g_XWeapon[nCntXWeapon].vtxMax.y)
				{
					g_XWeapon[nCntXWeapon].vtxMax.y = vtx.y;
				}
				if (vtx.z > g_XWeapon[nCntXWeapon].vtxMax.z)
				{
					g_XWeapon[nCntXWeapon].vtxMax.z = vtx.z;
				}

				if (vtx.x < g_XWeapon[nCntXWeapon].vtxMin.x)
				{
					g_XWeapon[nCntXWeapon].vtxMin.x = vtx.x;
				}
				if (vtx.y < g_XWeapon[nCntXWeapon].vtxMin.y)
				{
					g_XWeapon[nCntXWeapon].vtxMin.y = vtx.y;
				}
				if (vtx.z < g_XWeapon[nCntXWeapon].vtxMin.z)
				{
					g_XWeapon[nCntXWeapon].vtxMin.z = vtx.z;
				}

				if (g_XWeapon[nCntXWeapon].nIndexParent < 0)
				{
					nIndexParent = nCntXWeapon;
				}
			}

			pVtxBuff += sizeFVF;	// �T�C�Y���|�C���^��i�߂�
		}

		// ���_�o�b�t�@���A�����b�N
		g_XWeapon[nCntXWeapon].pMesh->UnlockVertexBuffer();
	}

	for (nCntXWeapon = g_MaxXWeapon; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		g_XWeapon[nCntXWeapon] = g_XWeapon[3];
	}
}

//=========================================================================================================================
// ����̏I������
//=========================================================================================================================
void UninitXWeapon(void)
{
	// �ϐ��錾
	int nCntXWeapon;

	// ���b�V���̊J��
	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].pMesh != NULL)
		{
			g_XWeapon[nCntXWeapon].pMesh->Release();
			g_XWeapon[nCntXWeapon].pMesh = NULL;
		}
	}

	// �}�e���A���̊J��
	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].pBuffMat != NULL)
		{
			g_XWeapon[nCntXWeapon].pBuffMat->Release();
			g_XWeapon[nCntXWeapon].pBuffMat = NULL;
		}
	}
}

//=========================================================================================================================
// ����̍X�V����
//=========================================================================================================================
void UpdateXWeapon(void)
{
	XPLAYER *pXPlayer = GetXPlayer();
	float fDiff = 0.0f;
	float frac = 0.0f;
	int nCntXWeapon;

	g_nCntTimeXWeapon++;

	for (nCntXWeapon = g_MaxXWeapon; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == true)
		{
			g_XWeapon[nCntXWeapon].pos += g_XWeapon[nCntXWeapon].move;	// �ړ��ʑ��

			SetBillboardNormalEffect(g_XWeapon[nCntXWeapon].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.5f, 1.5f, 0.0f), 0.08f);

			if (g_XWeapon[nCntXWeapon].pos.x > 1750.0f || g_XWeapon[nCntXWeapon].pos.x < -1750.0f ||
				g_XWeapon[nCntXWeapon].pos.z > 1750.0f || g_XWeapon[nCntXWeapon].pos.z < -1750.0f ||
				g_XWeapon[nCntXWeapon].pos.y < 0.0f)
			{// �����蔻��
				DeleteXWeapon(nCntXWeapon);
			}
		}
	}
}

//=========================================================================================================================
// ����̕`�揈��
//=========================================================================================================================
void DrawXWeapon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	XPLAYER *pXPlayer = GetXPlayer();			// 
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxParent;						// 
	int nCntXWeapon;

	for (nCntXWeapon = 0; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == true)
		{
			if (nCntXWeapon < g_MaxXWeapon)
			{
				mtxParent = pXPlayer->aModel[g_XWeapon[nCntXWeapon].nIndexParent].mtxWorld;
			}

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_XWeapon[nCntXWeapon].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_XWeapon[nCntXWeapon].rot.y,
				g_XWeapon[nCntXWeapon].rot.x,
				g_XWeapon[nCntXWeapon].rot.z
			);

			D3DXMatrixMultiply
			(
				&g_XWeapon[nCntXWeapon].mtxWorld,
				&g_XWeapon[nCntXWeapon].mtxWorld,
				&mtxRot
			);

			// �ʒu�𔽉f
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_XWeapon[nCntXWeapon].pos.x,
				g_XWeapon[nCntXWeapon].pos.y,
				g_XWeapon[nCntXWeapon].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_XWeapon[nCntXWeapon].mtxWorld,
				&g_XWeapon[nCntXWeapon].mtxWorld,
				&mtxTrans
			);

			if (nCntXWeapon < g_MaxXWeapon)
			{
				D3DXMatrixMultiply
				(
					&g_XWeapon[nCntXWeapon].mtxWorld,
					&g_XWeapon[nCntXWeapon].mtxWorld,
					&mtxParent
				);
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_XWeapon[nCntXWeapon].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			// �}�e���A���f�[�^�ւ̃|�C���^���擾
			pmat = (D3DXMATERIAL*)g_XWeapon[nCntXWeapon].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_XWeapon[nCntXWeapon].nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

				// �e�N�X�`���ݒ�
				pDevice->SetTexture(0, NULL);

				// ����(�p�[�c)�̕`��
				g_XWeapon[nCntXWeapon].pMesh->DrawSubset(nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=========================================================================================================================
// ����̐ݒ菈��
//=========================================================================================================================
void SetXWeapon(WEAPONTYPE type)
{
	XPLAYER *pXPalyer = GetXPlayer();
	int nCntXWeapon;

	// ����g�p
	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		if (type == nCntXWeapon && type != WEAPONTYPE_NONE)
		{
			g_XWeapon[nCntXWeapon].bUse = true;
		}
		else
		{
			g_XWeapon[nCntXWeapon].bUse = false;
		}
	}

	// �|��
	if (type == WEAPONTYPE_BOW)
	{
		g_XWeapon[WEAPONTYPE_SAGITTAL].bUse = true;
	}

	pXPalyer->nWeapon = type;
}

//=========================================================================================================================
// ����̎擾����
//=========================================================================================================================
WEAPONTYPE GetXWeapon(void)
{
	XPLAYER *pXPalyer = GetXPlayer();

	return pXPalyer->nWeapon;
}

//=========================================================================================================================
// ����̔��ˏ���
//=========================================================================================================================
void SeBulletWeapon(D3DXVECTOR3 move)
{// �v���C���[
	XPLAYER *pXPalyer = GetXPlayer();

	int nCntXWeapon;

	for (nCntXWeapon = g_MaxXWeapon; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == false)
		{// �g���Ă���
			// ���ːݒ�
			g_XWeapon[nCntXWeapon].pos = D3DXVECTOR3(g_XWeapon[3].mtxWorld._41, g_XWeapon[3].mtxWorld._42, g_XWeapon[3].mtxWorld._43);
			//g_XWeapon[nCntXWeapon].rot = D3DXVECTOR3(g_XWeapon[3].mtxWorld._32, g_XWeapon[3].mtxWorld._33, g_XWeapon[3].mtxWorld._34);
			g_XWeapon[nCntXWeapon].rot.y = pXPalyer->rot.y - (D3DX_PI / 2);
			g_XWeapon[nCntXWeapon].move = move;
			g_XWeapon[nCntXWeapon].nType = WEAPON_PLAYER;

			g_XWeapon[nCntXWeapon].bUse = true;

			break;
		}
	}
}
void SeBulletWeaponEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move)
{// �G
	XPLAYER *pXPalyer = GetXPlayer();

	int nCntXWeapon;

	for (nCntXWeapon = g_MaxXWeapon; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == false)
		{// �g���Ă���
			// ���ːݒ�
			g_XWeapon[nCntXWeapon].pos = pos;
			//g_XWeapon[nCntXWeapon].rot = D3DXVECTOR3(g_XWeapon[3].mtxWorld._32, g_XWeapon[3].mtxWorld._33, g_XWeapon[3].mtxWorld._34);
			g_XWeapon[nCntXWeapon].rot.y = rot.y - (D3DX_PI / 2);
			g_XWeapon[nCntXWeapon].move = move;
			g_XWeapon[nCntXWeapon].nType = WEAPON_ENEMY;

			g_XWeapon[nCntXWeapon].bUse = true;

			break;
		}
	}
}

//=========================================================================================================================
// ����̏Փ˔��菈��
//=========================================================================================================================
bool CollisionXWeapon(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax)
{
	GAMESTATE game = GetGameState();
	XPLAYER *pXPlayer = GetXPlayer();
	int nCntXWeapon;
	bool bLand = false;

	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == true && 
			(
				(pXPlayer->nAnimnow == PLAYERANIM_SWORD_1 && pXPlayer->nAnimkeynow == 2) ||
				(pXPlayer->nAnimnow == PLAYERANIM_SWORD_2 && pXPlayer->nAnimkeynow == 1) ||
				(pXPlayer->nAnimnow == PLAYERANIM_SWORD_3 && pXPlayer->nAnimkeynow == 3)
			))
		{// �g�p����Ă���
			//if (pPos->y + vtxMax->y > pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMin.y && pPos->y + vtxMin->y < pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMax.y)
			{// X�͈͊m�F
				if (pPos->x + vtxMax->x > g_XWeapon[nCntXWeapon].mtxWorld._41 - 5.0f && pPos->x + vtxMin->x < g_XWeapon[nCntXWeapon].mtxWorld._41 + 5.0f &&
					pPos->z + vtxMax->z > g_XWeapon[nCntXWeapon].mtxWorld._43 - 5.0f && pPos->z + vtxMin->z < g_XWeapon[nCntXWeapon].mtxWorld._43 + 5.0f)
				{// X�͈͊m�F

					if (game == GAMESTATE_PLAY)
					{
						if (pXPlayer->nAnimnow == PLAYERANIM_SWORD_1 ||
							pXPlayer->nAnimnow == PLAYERANIM_SWORD_2)
						{
							pMove->x = sinf(pXPlayer->rot.y) * -5.0f;
							pMove->z = cosf(pXPlayer->rot.y) * -5.0f;
						}
						else if (pXPlayer->nAnimnow == PLAYERANIM_SWORD_3)
						{
							pMove->x = sinf(pXPlayer->rot.y) * -8.0f;
							pMove->z = cosf(pXPlayer->rot.y) * -8.0f;
						}
					}

					bLand = true;
				}
			}
		}
	}

	return bLand;
}
bool CollisionXWeaponBullet(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *vtxMin, D3DXVECTOR3 *vtxMax, int nType)
{
	GAMESTATE game = GetGameState();
	XPLAYER *pXPlayer = GetXPlayer();
	int nCntXWeapon;
	bool bLand = false;

	for (nCntXWeapon = g_MaxXWeapon; nCntXWeapon < MAX_XWEAPON; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == true && g_XWeapon[nCntXWeapon].nType == nType)
		{// �g�p����Ă���
		 //if (pPos->y + vtxMax->y > pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMin.y && pPos->y + vtxMin->y < pXPlayer->pos.y + g_XWeapon[nCntXWeapon].mtxWorld._42 + g_XWeapon[nCntXWeapon].vtxMax.y)
			{// X�͈͊m�F
				if (pPos->x + vtxMax->x > g_XWeapon[nCntXWeapon].pos.x && pPos->x + vtxMin->x < g_XWeapon[nCntXWeapon].pos.x &&
					pPos->z + vtxMax->z > g_XWeapon[nCntXWeapon].pos.z && pPos->z + vtxMin->z < g_XWeapon[nCntXWeapon].pos.z)
				{// X�͈͊m�F

					g_XWeapon[nCntXWeapon].nHitNum++;

					if (game == GAMESTATE_PLAY)
					{
						pMove->x = sinf(g_XWeapon[nCntXWeapon].rot.y + (D3DX_PI / 2)) * -5.0f;
						pMove->z = cosf(g_XWeapon[nCntXWeapon].rot.y + (D3DX_PI / 2)) * -5.0f;
					}

					if (g_XWeapon[nCntXWeapon].nHitNum > 10)
					{
						DeleteXWeapon(nCntXWeapon);
					}

					bLand = true;
				}
			}
		}
	}

	return bLand;
}

//=========================================================================================================================
// ����̔j������
//=========================================================================================================================
void DeleteXWeapon(int nCntXWeapon)
{
	if (g_XWeapon[nCntXWeapon].bUse == true)
	{
		g_XWeapon[nCntXWeapon].nHitNum = 0;
		g_XWeapon[nCntXWeapon].bUse = false;
	}
}
void DeleteXWeapon(void)
{
	int nCntXWeapon;

	for (nCntXWeapon = 0; nCntXWeapon < g_MaxXWeapon; nCntXWeapon++)
	{
		if (g_XWeapon[nCntXWeapon].bUse == true)
		{
			g_XWeapon[nCntXWeapon].nHitNum = 0;
			g_XWeapon[nCntXWeapon].nType = WEAPON_NONE;
			g_XWeapon[nCntXWeapon].bUse = false;
		}
	}
}