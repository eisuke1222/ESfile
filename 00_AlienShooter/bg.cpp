//=========================================================================================================================
// 
// �w�i���� [bg.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "bg.h"			// �w�i
#include "input.h"		// ���͏���

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_BG_TEXTURE			(2)									// �w�i�e�N�X�`���ő吔
#define BG_TEXTURE_NAME_1		"data\\TEXTURE\\utyuu002.png"		// �ǂݍ��ރe�N�X�`���t�@�C��1
#define BG_TEXTURE_NAME_2		"data\\TEXTURE\\stage.png"			// �ǂݍ��ރe�N�X�`���t�@�C��2
#define BG_POS_X				(0)									// �w�i�̍���X���W
#define BG_POS_Y				(0)									// �w�i�̍���Y���W
#define BG_WIDTH				(SCREEN_WIDTH)						// �w�i�̕�
#define BG_HEIGHT				(SCREEN_HEIGHT)						// �w�i�̍���
#define BG_TEXTURE_SPEED		(5)									// �e�N�X�`���A�j���[�V�����X�s�[�h
#define BG_TEXTURE_PATTERN		(10)								// �e�N�X�`���A�j���[�V�����p�^�[����
#define BG_TEXTURE_UV_U			(1.0f)								// �e�N�X�`���A�j���[�V����U�͈�
#define BG_TEXTURE_UV_V			(1.0f)								// �e�N�X�`���A�j���[�V����V�͈�
#define BG_BACKTEXTURE_UV_U		(1.0f)								// �O�e�N�X�`���A�j���[�V����U�͈�
#define BG_BACKTEXTURE_UV_V		(1.0f)								// �O�e�N�X�`���A�j���[�V����V�͈�
#define BG_SPEED				(0.002f)							// �w�i�X�s�[�h

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef struct
{// �X�e�[�W�ԍ����
	STAGENUM	stage;				// �X�e�[�W�ԍ�			
	float		Color;				// �F
	int			nCounerStageNum;	// �J�E���g
	bool		bDisp;				// �\��
}STAGENUMSTATE;

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9				g_pTextureBG[MAX_BG_TEXTURE] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9			g_pVtxBuffBG = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3						g_posBG;							// �w�i�̈ʒu
STAGENUMSTATE					g_StageNum;							// �X�e�[�W�ԍ����

//=========================================================================================================================
// �w�i����������
//=========================================================================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X���擾
	int nCntBG;		// �J�E���g

	g_posBG = D3DXVECTOR3((BG_POS_X + BG_WIDTH) / 2, (BG_POS_Y + BG_HEIGHT) / 2, 0.0f);		// �w�i�̒��S�l

	g_StageNum.stage = STAGENUM_1;		// �X�e�[�W�ԍ�
	g_StageNum.Color = 0.0f;			// �F
	g_StageNum.nCounerStageNum = 0;		// �J�E���g
	g_StageNum.bDisp = false;			// �\��

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(// �w�i�P
		pDevice,
		BG_TEXTURE_NAME_1,
		&g_pTextureBG[0]
	);
	D3DXCreateTextureFromFile
	(// �X�e�[�W�ԍ�
		pDevice,
		BG_TEXTURE_NAME_2,
		&g_pTextureBG[1]
	);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_BG_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBG->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntBG = 0; nCntBG < MAX_BG_TEXTURE; nCntBG++)
	{// �J�E���g
		if (nCntBG == 0)
		{// �w�i
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, BG_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// �e�N�X�`���ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, BG_TEXTURE_UV_V);
			pVtx[3].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, BG_TEXTURE_UV_V);
		}
		else
		{// �X�e�[�W�ԍ�
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(BG_WIDTH / 2 - 150, BG_HEIGHT / 4 - 50, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(BG_WIDTH / 2 + 150, BG_HEIGHT / 4 - 50, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(BG_WIDTH / 2 - 150, BG_HEIGHT / 4 + 50, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(BG_WIDTH / 2 + 150, BG_HEIGHT / 4 + 50, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			// �e�N�X�`���ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f);
			pVtx[3].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, 0.2f);
		}
		// 1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	
		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBG->Unlock();
}

//=========================================================================================================================
// �w�i�I������
//=========================================================================================================================
void UninitBG(void)
{
	// �ϐ��錾
	int nCntTex;

	// �e�N�X�`���̔j��
	for (nCntTex = 0; nCntTex < MAX_BG_TEXTURE; nCntTex++)
	{// �e�N�X�`�����J�E���g
		if (g_pTextureBG[nCntTex] != NULL)
		{// NULL�ȊO�̏ꍇ
			g_pTextureBG[nCntTex]->Release();	// �J��
			g_pTextureBG[nCntTex] = NULL;		// NULL��
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBG != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffBG->Release();	// �J��
		g_pVtxBuffBG = NULL;		// NULL��
	}
}

//=========================================================================================================================
// �w�i�X�V����
//=========================================================================================================================
void UpdateBG(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBG->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// BG
	g_posBG.y -= 0.001f;	// �ړ��l���

	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, g_posBG.y - (BG_TEXTURE_UV_V / 2));
	pVtx[1].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, g_posBG.y - (BG_TEXTURE_UV_V / 2));
	pVtx[2].tex = D3DXVECTOR2(0.0f, g_posBG.y + (BG_TEXTURE_UV_V / 2));
	pVtx[3].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, g_posBG.y + (BG_TEXTURE_UV_V / 2));

	// �X�e�[�W
	if (g_StageNum.bDisp == true)
	{// �\�����邾������
		if (g_StageNum.Color < 1.0f && g_StageNum.nCounerStageNum == 0)
		{// �F���w�肵���l�ȉ��A�J�E���g���O
			g_StageNum.Color += 0.008f;		// ���₷
		}
		else
		{// ���̑�
			g_StageNum.Color -= 0.008f;		// ���炷
			g_StageNum.nCounerStageNum = 1;	// �J�E���g
		}

		if (g_StageNum.Color < 0.0f)
		{// �w�肵���l�ȉ���������
			g_StageNum.bDisp = false;		// ��\��
		}

		// ���_�J���[�ݒ�
		pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StageNum.Color);
		pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StageNum.Color);
		pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StageNum.Color);
		pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_StageNum.Color);
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBG->Unlock();
}

//=========================================================================================================================
// �w�i�`�揈��
//=========================================================================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntTex;

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffBG,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTex = 0; nCntTex < MAX_BG_TEXTURE; nCntTex++)
	{// �e�N�X�`�����J�E���g
		if (nCntTex == 0)
		{// �w�i
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureBG[nCntTex]);

			// �w�i�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
		}
		else if (nCntTex == 1 && g_StageNum.bDisp == true)
		{// �X�e�[�W�ԍ�
			// �e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureBG[nCntTex]);

			// �w�i�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
		}
	}
}

//=========================================================================================================================
// �X�e�[�W�ԍ��ݒ菈��
//=========================================================================================================================
void SetNumStage(STAGENUM stage)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffBG->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// �ݒ�
	g_StageNum.stage = stage;			// �X�e�[�W�ԍ�
	g_StageNum.Color = 0.0f;			// �F
	g_StageNum.nCounerStageNum = 0;		// �J�E���g
	g_StageNum.bDisp = true;			// �\������

	// ���_�J���[�ݒ�
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	// �e�N�X�`���ݒ�
	pVtx[4].tex = D3DXVECTOR2(0.0f, (g_StageNum.stage * 0.2f));
	pVtx[5].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, (g_StageNum.stage * 0.2f));
	pVtx[6].tex = D3DXVECTOR2(0.0f, (g_StageNum.stage * 0.2f) + 0.2f);
	pVtx[7].tex = D3DXVECTOR2(BG_TEXTURE_UV_U, (g_StageNum.stage * 0.2f) + 0.2f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffBG->Unlock();
}