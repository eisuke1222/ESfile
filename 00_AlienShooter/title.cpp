//=========================================================================================================================
// 
// �^�C�g������ [title.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "title.h"		// �^�C�g��
#include "select.h"		// �I������
#include "sound.h"		// �T�E���h
#include "fade.h"		// �t�F�[�h
#include "input.h"		// ����

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_TITLE_TEXTURE		(2)								// �e�N�X�`���̍ő吔
#define TITLE_TEXTURE_NAME_1	"data\\TEXTURE\\title000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��1
#define TITLE_TEXTURE_NAME_2	"data\\TEXTURE\\Alien-Shooter.png"		// �ǂݍ��ރe�N�X�`���t�@�C��2
#define TITLE_POS_X				(0)								// �^�C�g���̍���X���W
#define TITLE_POS_Y				(0)								// �^�C�g���̍���Y���W
#define TITLE_WIDTH				(SCREEN_WIDTH)					// �^�C�g���̕�
#define TITLE_HEIGHT			(SCREEN_HEIGHT)					// �^�C�g���̍�
#define TITLE_TEXTURE_UV_U		(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define TITLE_TEXTURE_UV_V		(1.0f)							// �e�N�X�`���A�j���[�V����V�͈�
#define TITLE_TEXTURE_SPEED		(2)								// �e�N�X�`���X�s�[�h

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureTitle[MAX_TITLE_TEXTURE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffTitle = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3					g_posTitle;										// �^�C�g���̈ʒu
D3DXVECTOR3					g_moveTitle;									// �^�C�g���̈ړ��l
int							g_nCntMode;										// ���[�h�ؑփJ�E���^

//=========================================================================================================================
// �^�C�g������������
//=========================================================================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntTitle;			// �^�C�g���J�E���^

	g_posTitle = D3DXVECTOR3((TITLE_POS_X + TITLE_WIDTH) / 2, (TITLE_HEIGHT / 8) * -1, 0.0f);		// �|���S���̒��S�l
	g_moveTitle = D3DXVECTOR3(0.0f, TITLE_TEXTURE_SPEED, 0.0f);				// �|���S���̈ړ��l
	g_nCntMode = 0;				// �J�E���^������

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(// �^�C�g��BG
		pDevice,
		TITLE_TEXTURE_NAME_1,
		&g_pTextureTitle[0]
	);
	D3DXCreateTextureFromFile
	(// �^�C�g�����S
		pDevice,
		TITLE_TEXTURE_NAME_2,
		&g_pTextureTitle[1]

	);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_TITLE_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntTitle = 0; nCntTitle < MAX_TITLE_TEXTURE; nCntTitle++)
	{// �^�C�g�����J�E���g
		// ���_�ݒ�
		if (nCntTitle == 0)
		{// �^�C�g��BG
			pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(TITLE_WIDTH, TITLE_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(TITLE_WIDTH, TITLE_HEIGHT, 0.0f);
		}
		else if (nCntTitle == 1)
		{// �^�C�g�����S
			pVtx[0].pos = D3DXVECTOR3(TITLE_WIDTH / 4, 0, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(TITLE_WIDTH / 4 * 3, 0, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(TITLE_WIDTH / 4, 0, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(TITLE_WIDTH / 4 * 3, 0, 0.0f);
		}
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
		pVtx[1].tex = D3DXVECTOR2(TITLE_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, TITLE_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(TITLE_TEXTURE_UV_U, TITLE_TEXTURE_UV_V);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}

//=========================================================================================================================
// �^�C�g���I������
//=========================================================================================================================
void UninitTitle(void)
{
	int nCntTex;		// �e�N�X�`���J�E���^

	// �e�N�X�`���̔j��
	for (nCntTex = 0; nCntTex < MAX_TITLE_TEXTURE; nCntTex++)
	{// �e�N�X�`���J�E���g
		if (g_pTextureTitle[nCntTex] != NULL)
		{// NULL�ȊO�̏ꍇ
			g_pTextureTitle[nCntTex]->Release();	// �J��
			g_pTextureTitle[nCntTex] = NULL;		// NULL��
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffTitle->Release();		// �J��
		g_pVtxBuffTitle = NULL;			// NULL��
	}
}

//=========================================================================================================================
// �^�C�g���X�V����
//=========================================================================================================================
void UpdateTitle(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// �C�ӂ̃L�[�iENTER�L�[�j���������ꍇ
	if (GetKeyboardPress(DIK_RETURN) == true)
	{// �X�s�[�h�ύX
		g_posTitle.y += g_moveTitle.y * 40;
	}
	else
	{// �ʏ�ړ�
		g_posTitle.y +=	g_moveTitle.y;
	}

	if (g_posTitle.y >= SCREEN_HEIGHT / 3)
	{// ��苗���Ŏ~�܂�
		g_moveTitle.y = 0;
		SetEnterSelect(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXVECTOR3(150, 50, 0.0f));
	}

	// ���_�ݒ�
	pVtx[4].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, g_posTitle.y - (100.0f), 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, g_posTitle.y - (100.0f), 0.0f);
	pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 500.0f, g_posTitle.y + (100.0f), 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 500.0f, g_posTitle.y + (100.0f), 0.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
	
	// �C�ӂ̃L�[�iENTER�L�[�j���������ꍇ
	FADE Fade = GetFade();		// �t�F�[�h���擾

	if (Fade == FADE_NONE)
	{// �t�F�[�h���g�p���Ă��Ȃ��ꍇ
		if (g_posTitle.y >= SCREEN_HEIGHT / 3 && GetKeyboardRelease(DIK_RETURN) == true)
		{// ��ʐ؂�ւ�
			ChangeSelect(SELECTSTATE_ENTER);
			PlaySound(SOUND_LABEL_SE_DECIDE);	// ���艹
			SetFade(MODE_TUTORIAL);				// �`���[�g���A��
		}
	}

	g_nCntMode++;

	if (g_nCntMode > 900)
	{// ��ʐ؂�ւ�
		SetFade(MODE_RANKING);				// �����L���O
	}
}

//=========================================================================================================================
// �^�C�g���`�揈��
//=========================================================================================================================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntTitle;			// �^�C�g���J�E���^

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffTitle,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntTitle = 0; nCntTitle < MAX_TITLE_TEXTURE; nCntTitle++)
	{// �^�C�g�����J�E���g
		// �e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[nCntTitle]);

		// �^�C�g���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
	}
}