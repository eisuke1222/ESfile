//=========================================================================================================================
//
// �e���� [shadow.cpp]
// Author : Eisuke Sakagawa
//
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "shadow.h"
#include "input.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_SHADOW				(512)									// �e�̍ő吔
#define MAX_MODEL_SHADOW		(1)										// �e��1���f���ő吔
#define SHADOW_TEXTURE_NAME1	"data\\TEXTURE\\shadow000.jpg"			// �ǂݍ��ރe�N�X�`���t�@�C��1
#define SHADOW_TEXTURE_UV_U		(1.0f)									// �e�N�X�`���A�j���[�V����U�͈�
#define SHADOW_TEXTURE_UV_V		(1.0f)									// �e�N�X�`���A�j���[�V����V�͈�

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);						// ���_���̐ݒ�

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureShadow = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffShadow = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
SHADOW							g_Shadow[MAX_SHADOW];					// �e�̏��

//=========================================================================================================================
// �e�̏���������
//=========================================================================================================================
void InitShadow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntShadow;

	// ���_���̍쐬
	MakeVertexShadow(pDevice);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Shadow[nCntShadow].bUse = false;
	}
}

//=========================================================================================================================
// �e�̏I������
//=========================================================================================================================
void UninitShadow(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=========================================================================================================================
// �e�̍X�V����
//=========================================================================================================================
void UpdateShadow(void)
{

}

//=========================================================================================================================
// �e�̕`�揈��
//=========================================================================================================================
void DrawShadow(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	int nCntShadow;

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Shadow[nCntShadow].mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll
			(
				&mtxRot,
				g_Shadow[nCntShadow].rot.y,
				g_Shadow[nCntShadow].rot.x,
				g_Shadow[nCntShadow].rot.z
			);

			D3DXMatrixMultiply
			(
				&g_Shadow[nCntShadow].mtxWorld,
				&g_Shadow[nCntShadow].mtxWorld,
				&mtxRot
			);

			// �ʒu�𔽉f
			D3DXMatrixTranslation
			(
				&mtxTrans,
				g_Shadow[nCntShadow].pos.x,
				g_Shadow[nCntShadow].pos.y,
				g_Shadow[nCntShadow].pos.z
			);

			D3DXMatrixMultiply
			(
				&g_Shadow[nCntShadow].mtxWorld,
				&g_Shadow[nCntShadow].mtxWorld,
				&mtxTrans
			);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Shadow[nCntShadow].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource
			(
				0,
				g_pVtxBuffShadow,
				0,
				sizeof(VERTEX_3D)
			);

			// ���_�t�H�[�}�b�g
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			// �v���C���[�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, 2);
		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=========================================================================================================================
// �e�̐ݒ菈��
//=========================================================================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 fRadius)
{
	// �ϐ��錾
	int nCntShadow;
	int nCntSet = 0;

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffShadow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == false)
		{
			g_Shadow[nCntShadow].pos = pos;
			g_Shadow[nCntShadow].pos.y = 0.1f;
			g_Shadow[nCntShadow].rot = rot;
			g_Shadow[nCntShadow].bUse = true;

			if (nCntSet == 0)
			{
				g_Shadow[nCntShadow].fRadius = fRadius;
			}
			else
			{
				g_Shadow[nCntShadow].fRadius = g_Shadow[nCntShadow - 1].fRadius - (fRadius / MAX_MODEL_SHADOW);
			}

			// ���_���̐ݒ�
			pVtx[nCntShadow * 4].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fRadius.x, 0.0f, g_Shadow[nCntShadow].fRadius.z);
			pVtx[nCntShadow * 4 + 1].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fRadius.x, 0.0f, g_Shadow[nCntShadow].fRadius.z);
			pVtx[nCntShadow * 4 + 2].pos = D3DXVECTOR3(-g_Shadow[nCntShadow].fRadius.x, 0.0f, -g_Shadow[nCntShadow].fRadius.z);
			pVtx[nCntShadow * 4 + 3].pos = D3DXVECTOR3(g_Shadow[nCntShadow].fRadius.x, 0.0f, -g_Shadow[nCntShadow].fRadius.z);

			nCntSet++;
			if (nCntSet == MAX_MODEL_SHADOW)
			{
				break;
			}
		}
	}
	
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//=========================================================================================================================
// �e�̈ʒu�ݒ菈��
//=========================================================================================================================
void SetPositionShadow(int IdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �ϐ��錾
	int nCntShadow;

	for (nCntShadow = IdxShadow - MAX_MODEL_SHADOW + 1; nCntShadow <= IdxShadow; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			g_Shadow[nCntShadow].pos = pos;
			g_Shadow[nCntShadow].pos.y = 0.1f;
			g_Shadow[nCntShadow].rot = rot;
		}
	}
}

//=========================================================================================================================
// �e�̔j������
//=========================================================================================================================
void DeleteShadow(int IdxShadow)
{
	// �ϐ��錾
	int nCntShadow;

	for (nCntShadow = IdxShadow - MAX_MODEL_SHADOW + 1; nCntShadow <= IdxShadow; nCntShadow++) 
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].bUse = false;
		}
	}
}
void DeleteShadow(void)
{
	// �ϐ��錾
	int nCntShadow;

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_Shadow[nCntShadow].bUse == true)
		{
			g_Shadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Shadow[nCntShadow].bUse = false;
		}
	}
}

//=========================================================================================================================
// ���_���̍쐬
//=========================================================================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// �ϐ��錾
	int nCntShadow;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTURE_NAME1, &g_pTextureShadow);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL
	);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffShadow->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		// ���_���̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// �@���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(SHADOW_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, SHADOW_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(SHADOW_TEXTURE_UV_U, SHADOW_TEXTURE_UV_V);
	
		pVtx += 4;		// ���_�o�b�t�@�̃|�C���^��4�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();
}

