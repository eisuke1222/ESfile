//=========================================================================================================================
// 
// �I������ [bg.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "select.h"		// �I��
#include "input.h"		// ���͏���

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_SELECT_TEXTURE			(2)								// �I���e�N�X�`���ő吔
#define SELECT_TEXTURE_NAME_1		"data\\TEXTURE\\press_enter.png"// �ǂݍ��ރe�N�X�`���t�@�C��1
#define SELECT_POS_X				(SCREEN_WIDTH - 210)			// �I���̍���X���W
#define SELECT_POS_Y				(SCREEN_HEIGHT - 80)			// �I���̍���Y���W
#define SELECT_WIDTH				(SCREEN_WIDTH - 10)				// �I���̕�
#define SELECT_HEIGHT				(SCREEN_HEIGHT)					// �I���̍���
#define SELECT_TEXTURE_SPEED		()								// �e�N�X�`���A�j���[�V�����X�s�[�h
#define SELECT_TEXTURE_PATTERN		(10)							// �e�N�X�`���A�j���[�V�����p�^�[����
#define SELECT_TEXTURE_UV_U			(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define SELECT_TEXTURE_UV_V			(1.0f)							// �e�N�X�`���A�j���[�V����V�͈�
#define SELECT_BACKTEXTURE_UV_U		(1.0f)							// �O�e�N�X�`���A�j���[�V����U�͈�
#define SELECT_BACKTEXTURE_UV_V		(1.0f)							// �O�e�N�X�`���A�j���[�V����V�͈�

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9					g_pTextureSelect = {};			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9				g_pVtxBuffSelect = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
SELECTSTATE							g_SelectState;					// �I���̏��
D3DXVECTOR3							g_posSelect;					// �I���̈ʒu
D3DXVECTOR3							g_RadiusSelect;					// �I���̑傫���i���a�j
int									g_nCntSelectState;				// ��ԃJ�E���^

//=========================================================================================================================
// �I������������
//=========================================================================================================================
void InitSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	g_SelectState = SELECTSTATE_NONE;			// ��ԏ�����
	g_posSelect = D3DXVECTOR3((SELECT_POS_X + SELECT_WIDTH) / 2, (SELECT_POS_Y + SELECT_HEIGHT) / 2, 0.0f);		// �I���̒��S�l
	g_RadiusSelect = D3DXVECTOR3(0, 0, 0.0f);	// �I���̑傫��
	g_nCntSelectState = 0;						// ��ԃJ�E���^

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(// �I��
		pDevice,
		SELECT_TEXTURE_NAME_1,
		&g_pTextureSelect
	);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelect,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffSelect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

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
	pVtx[1].tex = D3DXVECTOR2(SELECT_TEXTURE_UV_U, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, SELECT_TEXTURE_UV_V);
	pVtx[3].tex = D3DXVECTOR2(SELECT_TEXTURE_UV_U, SELECT_TEXTURE_UV_V);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffSelect->Unlock();
}

//=========================================================================================================================
// �I���I������
//=========================================================================================================================
void UninitSelect(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureSelect != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pTextureSelect->Release();	// �J��
		g_pTextureSelect = NULL;		// NULL��
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffSelect != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffSelect->Release();	// �J��
		g_pVtxBuffSelect = NULL;		// NULL��
	}
}

//=========================================================================================================================
// �I���X�V����
//=========================================================================================================================
void UpdateSelect(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^
	D3DXCOLOR nColor;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffSelect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	switch (g_SelectState)
	{// �I��ԍ�
	case SELECTSTATE_NONE:		// �������Ă��Ȃ�
		g_nCntSelectState++;	// �J�E���g

		if (g_nCntSelectState % 80 == 0)
		{// ���t���[���ɂȂ�����
			g_nCntSelectState = 0;
		}

		if (g_nCntSelectState < 40)
		{// �w�肵���l���Ⴂ
			nColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{// �w�肵���l�ȏ�
			nColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		break;
	case SELECTSTATE_ENTER:		// ���肵��
		g_nCntSelectState++;	// �J�E���g

		if (g_nCntSelectState % 10 == 0)
		{// ���t���[���ɂȂ�����
			g_nCntSelectState = 0;
		}

		if (g_nCntSelectState < 5)
		{// �w�肵���l���Ⴂ
			nColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{// �w�肵���l�ȏ�
			nColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		}
		break;
	}

	// ���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);
	// ���_�J���[�ݒ�
	pVtx[0].col = nColor;
	pVtx[1].col = nColor;
	pVtx[2].col = nColor;
	pVtx[3].col = nColor;

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffSelect->Unlock();
}

//=========================================================================================================================
// �I��`�揈��
//=========================================================================================================================
void DrawSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffSelect,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureSelect);

	// �I���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=========================================================================================================================
// �I��ݒ菈��
//=========================================================================================================================
void SetEnterSelect(D3DXVECTOR3 pos, D3DXVECTOR3 Radius)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffSelect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	g_posSelect = pos;
	g_RadiusSelect = Radius;

	// ���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffSelect->Unlock();
}

//=========================================================================================================================
// �I��ݒ菈��
//=========================================================================================================================
void SetMenuSelect(D3DXVECTOR3 pos, D3DXVECTOR3 Radius)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffSelect->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	g_posSelect = pos;
	g_RadiusSelect = Radius;

	// ���_�ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y - g_RadiusSelect.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posSelect.x - g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posSelect.x + g_RadiusSelect.x, g_posSelect.y + g_RadiusSelect.y, 0.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffSelect->Unlock();
}

//=========================================================================================================================
// �I��ω�����
//=========================================================================================================================
void ChangeSelect(SELECTSTATE state)
{
	g_SelectState = state;
}