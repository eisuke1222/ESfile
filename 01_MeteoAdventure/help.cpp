//=========================================================================================================================
// 
// �w���v���� [help.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "help.h"		// �w���v
#include "input.h"		// ���͏���
#include "sound.h"		// �T�E���h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_HELP_TEXTURE		(1)								// �w���v�e�N�X�`���ő吔
#define HELP_TEXTURE_NAME_1		"data\\TEXTURE\\help102.png"	// �ǂݍ��ރe�N�X�`���t�@�C��1
#define HELP_POS_X				(0)								// �w���v�̍���X���W
#define HELP_POS_Y				(0)								// �w���v�̍���Y���W
#define HELP_WIDTH				(SCREEN_WIDTH)					// �w���v�̕�
#define HELP_HEIGHT				(SCREEN_HEIGHT)					// �w���v�̍���
#define HELP_TEXTURE_SPEED		(5)								// �e�N�X�`���A�j���[�V�����X�s�[�h
#define HELP_TEXTURE_PATTERN	(10)							// �e�N�X�`���A�j���[�V�����p�^�[����
#define HELP_TEXTURE_UV_U		(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define HELP_TEXTURE_UV_V		(1.0f)							// �e�N�X�`���A�j���[�V����V�͈�
#define HELP_BACKTEXTURE_UV_U	(1.0f)							// �O�e�N�X�`���A�j���[�V����U�͈�
#define HELP_BACKTEXTURE_UV_V	(1.0f)							// �O�e�N�X�`���A�j���[�V����V�͈�
#define HELP_SPEED				(0.002f)						// �w���v�X�s�[�h

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureHelp = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffHelp = NULL;						// ���_�o�b�t�@�ւ̃|�C���^

//=========================================================================================================================
// �w���v����������
//=========================================================================================================================
void InitHelp(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X���擾

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, HELP_TEXTURE_NAME_1, &g_pTextureHelp);		// BG

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHelp,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffHelp->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// ���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(HELP_POS_X, HELP_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(HELP_WIDTH, HELP_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(HELP_POS_X, HELP_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(HELP_WIDTH, HELP_HEIGHT, 0.0f);
	// ���_�J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	// 1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(HELP_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, HELP_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(HELP_TEXTURE_UV_U, HELP_TEXTURE_UV_V);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffHelp->Unlock();
}

//=========================================================================================================================
// �w���v�I������
//=========================================================================================================================
void UninitHelp(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureHelp != NULL)
	{
		g_pTextureHelp->Release();
		g_pTextureHelp = NULL;

	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffHelp != NULL)
	{
		g_pVtxBuffHelp->Release();
		g_pVtxBuffHelp = NULL;
	}
}

//=========================================================================================================================
// �w���v�X�V����
//=========================================================================================================================
void UpdateHelp(void)
{	

}

//=========================================================================================================================
// �w���v�`�揈��
//=========================================================================================================================
void DrawHelp(void)
{
	LPDIRECT3DDEVICE9 pDevice;		// �f�o�C�X�|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffHelp,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureHelp);

	// �w���v�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}