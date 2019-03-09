//=========================================================================================================================
// 
// �������� [explosion.cpp]
// Author : Einsuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "explosion.h"	// ����

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define	MAX_PLAYER					(256)								// �����̍ő吔
#define PLAYER_TEXTURE_NAME			"data\\TEXTURE\\explosion002.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define PLAYER_G_POS_X				(30)								// g_pos�����X����
#define PLAYER_G_POS_Y				(30)								// g_pos�����Y����
#define PLAYER_TEXTURE_SPEED		(3)									// �e�N�X�`���A�j���[�V�����X�s�[�h
#define PLAYER_TEXTURE_PATTERN		(16)								// �e�N�X�`���A�j���[�V�����p�^�[����
#define PLAYER_TEXTURE_UV_U			(0.125f)							// �e�N�X�`���A�j���[�V����U�͈�
#define PLAYER_TEXTURE_UV_V			(0.5f)								// �e�N�X�`���A�j���[�V����V�͈�

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef struct
{// ����
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	int nRadius;			// g_pos����̋���
	int nCounterAnim;		// �A�j���[�V�����J�E���^�[
	int nPatternAnim;		// �A�j���[�V�����p�^�[��No.
	bool bDisp;				// �g�p���Ă��邩�ǂ���
}Explosion;

//*************************************************************************************************************************
// �O���[�o���ϐ��錾
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureExplosion = NULL;					// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffExplosion = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
Explosion					g_aExplosion[MAX_PLAYER];					// �����̏��

//=========================================================================================================================
// ��������������
//=========================================================================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntExplosion;		// �����J�E���g

	// �����̏��̏�����
	for(nCntExplosion = 0; nCntExplosion < MAX_PLAYER; nCntExplosion++)
	{// �������J�E���g
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �����ʒu
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �����F
		g_aExplosion[nCntExplosion].nRadius = 0;								// �������a
		g_aExplosion[nCntExplosion].nCounterAnim = 0;							// �J�E���^�[�A�j���[�V����
		g_aExplosion[nCntExplosion].nPatternAnim = 0;							// �p�^�[��No.�A�j���[�V����
		g_aExplosion[nCntExplosion].bDisp = false;								// �\���ݒ�
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		PLAYER_TEXTURE_NAME,
		&g_pTextureExplosion
	);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	// �����̏��̏�����
	for (nCntExplosion = 0; nCntExplosion < MAX_PLAYER; nCntExplosion++)
	{// �������J�E���g
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
		pVtx[1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, PLAYER_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U, PLAYER_TEXTURE_UV_V);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//=========================================================================================================================
// �����I������
//=========================================================================================================================
void UninitExplosion(void)
{
	// �e�N�X�`���[�̔j��
	if (g_pTextureExplosion != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pTextureExplosion->Release();		// �J��
		g_pTextureExplosion = NULL;			// NULL��
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffExplosion != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffExplosion->Release();		// �J��
		g_pVtxBuffExplosion = NULL;			// NULL��
	}
}

//=========================================================================================================================
// �����X�V����
//=========================================================================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;				// �����J�E���g

	VERTEX_2D *pVtx;				// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for(nCntExplosion = 0; nCntExplosion < MAX_PLAYER; nCntExplosion++)
	{// �������J�E���g
		if (g_aExplosion[nCntExplosion].bDisp == true)
		{// �������g�p����Ă���ꍇ
			g_aExplosion[nCntExplosion].nCounterAnim++;		// �J�E���^�[���Z

			if ((g_aExplosion[nCntExplosion].nCounterAnim % PLAYER_TEXTURE_SPEED) == 0)
			{// �A�j���[�V�������x
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % PLAYER_TEXTURE_PATTERN;		// �p�^�[��No.�X�V

				// ���_�ݒ�
				pVtx[nCntExplosion * 4].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].nRadius, 0.0f);
				pVtx[nCntExplosion * 4 + 1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].nRadius, 0.0f);
				pVtx[nCntExplosion * 4 + 2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].nRadius, 0.0f);
				pVtx[nCntExplosion * 4 + 3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].nRadius, 0.0f);
				// ���_�J���[�ݒ�
				pVtx[nCntExplosion * 4].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
				pVtx[nCntExplosion * 4 + 1].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
				pVtx[nCntExplosion * 4 + 2].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
				pVtx[nCntExplosion * 4 + 3].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
				// �e�N�X�`���ݒ�
				pVtx[nCntExplosion * 4].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * (g_aExplosion[nCntExplosion].nPatternAnim % 8), PLAYER_TEXTURE_UV_V * (g_aExplosion[nCntExplosion].nPatternAnim / 8));
				pVtx[nCntExplosion * 4 + 1].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * (g_aExplosion[nCntExplosion].nPatternAnim % 8) + PLAYER_TEXTURE_UV_U, PLAYER_TEXTURE_UV_V * (g_aExplosion[nCntExplosion].nPatternAnim / 8));
				pVtx[nCntExplosion * 4 + 2].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * (g_aExplosion[nCntExplosion].nPatternAnim % 8), PLAYER_TEXTURE_UV_V * (g_aExplosion[nCntExplosion].nPatternAnim / 8) + PLAYER_TEXTURE_UV_V);
				pVtx[nCntExplosion * 4 + 3].tex = D3DXVECTOR2(PLAYER_TEXTURE_UV_U * (g_aExplosion[nCntExplosion].nPatternAnim % 8) + PLAYER_TEXTURE_UV_U, PLAYER_TEXTURE_UV_V * (g_aExplosion[nCntExplosion].nPatternAnim / 8) + PLAYER_TEXTURE_UV_V);

				if (g_aExplosion[nCntExplosion].nPatternAnim == PLAYER_TEXTURE_PATTERN - 1)
				{// �A�j���[�V�������Ō�܂ł������ꍇ
					g_aExplosion[nCntExplosion].bDisp = false;		// �������g�p���Ă��Ȃ����
				}
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//=========================================================================================================================
// �����`�揈��
//=========================================================================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntExplosion;		// �e�J�E���g

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffExplosion,
		0, 
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	// �|���S���̕`��
	for(nCntExplosion = 0; nCntExplosion < MAX_PLAYER; nCntExplosion++)
	{// �������J�E���g
		if (g_aExplosion[nCntExplosion].bDisp == true)
		{// ����������ꍇ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//=========================================================================================================================
// �����̐ݒ菈��
//=========================================================================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int nRadius)
{
	int nCntExplosion;			// �����J�E���g

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for(nCntExplosion = 0; nCntExplosion< MAX_PLAYER; nCntExplosion++)
	{// �������J�E���g
		if (g_aExplosion[nCntExplosion].bDisp == false)
		{// �������g�p����Ă��Ȃ�
			g_aExplosion[nCntExplosion].pos = pos;			// pos�l���
			g_aExplosion[nCntExplosion].col = col;			// move�l���
			g_aExplosion[nCntExplosion].nRadius = nRadius;	// nRadius����̋������
			g_aExplosion[nCntExplosion].nCounterAnim = 0;	// �J�E���^�[�����l�ݒ�
			g_aExplosion[nCntExplosion].nPatternAnim = 0;	// �p�^�[��No.�����l�ݒ�
			g_aExplosion[nCntExplosion].bDisp = true;		// �g�p���Ă����Ԃɂ���

			// ���_�ݒ�
			pVtx[nCntExplosion * 4].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].nRadius, 0.0f);
			pVtx[nCntExplosion * 4 + 1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y - g_aExplosion[nCntExplosion].nRadius, 0.0f);
			pVtx[nCntExplosion * 4 + 2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].nRadius, 0.0f);
			pVtx[nCntExplosion * 4 + 3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + g_aExplosion[nCntExplosion].nRadius, g_aExplosion[nCntExplosion].pos.y + g_aExplosion[nCntExplosion].nRadius, 0.0f);
			// ���_�J���[�ݒ�
			pVtx[nCntExplosion * 4].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
			pVtx[nCntExplosion * 4 + 1].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
			pVtx[nCntExplosion * 4 + 2].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);
			pVtx[nCntExplosion * 4 + 3].col = D3DXCOLOR(g_aExplosion[nCntExplosion].col.r, g_aExplosion[nCntExplosion].col.g, g_aExplosion[nCntExplosion].col.b, g_aExplosion[nCntExplosion].col.a);

			break;
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}