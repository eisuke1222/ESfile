//=========================================================================================================================
// 
// �����L���O���� [ranking.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "ranking.h"		// �����L���O
#include "score.h"			// �X�R�A
#include "input.h"			// ���͏���

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_RANKING_TEXTURE			(2)								// �e�N�X�`���̍ő吔
#define RANKING_TEXTURE_NAME_1		"data\\TEXTURE\\title000.png"	// �ǂݍ��ރe�N�X�`���t�@�C��
#define RANKING_TEXTURE_NAME_2		"data\\TEXTURE\\ranking006.png"	// �ǂݍ��ރe�N�X�`���t�@�C��2
#define RANKING_POS_X				(0)								// �����L���O�̍���X���W
#define RANKING_POS_Y				(0)								// �����L���O�̍���Y���W
#define RANKING_WIDTH				(SCREEN_WIDTH)					// �����L���O�̕�
#define RANKING_HEIGHT				(SCREEN_HEIGHT)					// �����L���O�̍�
#define RANKING_TEXTURE_UV_U		(1.0f)							// �e�N�X�`���A�j���[�V����U�͈�
#define RANKING_TEXTURE_UV_V		(1.0f)							// �e�N�X�`���A�j���[�V����V�͈�
#define RANKING_TEXTURE_SPEED		(15.0f)							// �e�N�X�`���X�s�[�h

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureRanking[MAX_RANKING_TEXTURE] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffRanking = NULL;							// ���_�o�b�t�@�ւ̃|�C���^
Ranking						g_Ranking;											// �����L���O�̏��
int							g_nCntRankingState;									// ��ԃJ�E���^

//=========================================================================================================================
// �����L���O����������
//=========================================================================================================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntRnking;		// �����L���O�J�E���^

	g_Ranking.pos = D3DXVECTOR3(RANKING_WIDTH + (RANKING_WIDTH / 2), (RANKING_POS_Y + RANKING_HEIGHT) / 2, 0.0f);		// �|���S���̒��S�l
	g_Ranking.move = D3DXVECTOR3(RANKING_TEXTURE_SPEED, 0.0f, 0.0f);		// �|���S���̒��S�l
	g_nCntRankingState = 0;		// ��ԃJ�E���g��������

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(// BG
		pDevice,
		RANKING_TEXTURE_NAME_1,
		&g_pTextureRanking[0]
	);
	D3DXCreateTextureFromFile
	(// �����L���O
		pDevice,
		RANKING_TEXTURE_NAME_2,
		&g_pTextureRanking[1]
	);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_RANKING_TEXTURE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffRanking->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntRnking = 0; nCntRnking < MAX_RANKING_TEXTURE; nCntRnking++)
	{//	�����L���O���J�E���g
		if (nCntRnking == 0)
		{// BG
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(RANKING_POS_X, RANKING_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(RANKING_WIDTH, RANKING_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(RANKING_POS_X, RANKING_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(RANKING_WIDTH, RANKING_HEIGHT, 0.0f);
		}
		else
		{// �X�R�A�����L���O
			// ���_�ݒ�
			pVtx[0].pos = D3DXVECTOR3(RANKING_WIDTH, RANKING_POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(RANKING_WIDTH + RANKING_WIDTH, RANKING_POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(RANKING_WIDTH, RANKING_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(RANKING_WIDTH + RANKING_WIDTH, RANKING_HEIGHT, 0.0f);
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
		pVtx[1].tex = D3DXVECTOR2(RANKING_TEXTURE_UV_U, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, RANKING_TEXTURE_UV_V);
		pVtx[3].tex = D3DXVECTOR2(RANKING_TEXTURE_UV_U, RANKING_TEXTURE_UV_V);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffRanking->Unlock();
}

//=========================================================================================================================
// �����L���O�I������
//=========================================================================================================================
void UninitRanking(void)
{
	int nCntTex;		// �e�N�X�`���J�E���^

	// �e�N�X�`���̔j��
	for (nCntTex = 0; nCntTex < MAX_RANKING_TEXTURE; nCntTex++)
	{//	�e�N�X�`�����J�E���g
		if (g_pTextureRanking[nCntTex] != NULL)
		{// NULL�ȊO�̏ꍇ
			g_pTextureRanking[nCntTex]->Release();		// �J��
			g_pTextureRanking[nCntTex] = NULL;			// NULL��
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffRanking != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffRanking->Release();	// �J��
		g_pVtxBuffRanking = NULL;		// NULL��
	}
}

//=========================================================================================================================
// �����L���O�X�V����
//=========================================================================================================================
void UpdateRanking(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffRanking->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	); 

	// �ړ��l���
	g_Ranking.pos -= g_Ranking.move;

	if (g_Ranking.pos.x <= SCREEN_WIDTH / 2)
	{// �ړ��͈�
		if (g_nCntRankingState >= 180 || (GetKeyboardTrigger(DIK_RETURN) == true && g_nCntRankingState > 30))
		{// �ړ��J�n
			g_Ranking.move = D3DXVECTOR3(RANKING_TEXTURE_SPEED, 0.0f, 0.0f);
		}
		else if (g_nCntRankingState == 0)
		{// �ړ���~
			g_Ranking.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		g_nCntRankingState++;		// ��Ԃ��J�E���g
	}

	// ���_�ݒ�
	pVtx[4].pos = D3DXVECTOR3(g_Ranking.pos.x - (RANKING_WIDTH / 2), RANKING_POS_Y, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_Ranking.pos.x + (RANKING_WIDTH / 2), RANKING_POS_Y, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_Ranking.pos.x - (RANKING_WIDTH / 2), RANKING_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_Ranking.pos.x + (RANKING_WIDTH / 2), RANKING_HEIGHT, 0.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffRanking->Unlock();

	if (g_Ranking.pos.x <= -SCREEN_WIDTH)
	{// ��ʊO�ɂȂ������ʐ؂�ւ�
		SetMode(MODE_TITLE);		// �^�C�g��
	}
}

//=========================================================================================================================
// �����L���O�`�揈��
//=========================================================================================================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntRnking;		// �����L���O�J�E���^

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffRanking,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntRnking = 0; nCntRnking < MAX_RANKING_TEXTURE; nCntRnking++)
	{//	�����L���O���J�E���g
		// �e�N�X�`���ݒ�
		pDevice->SetTexture(0, g_pTextureRanking[nCntRnking]);

		// �����L���O�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRnking * 4, 2);
	}
}

//=========================================================================================================================
// �����L���O�擾����
//=========================================================================================================================
Ranking GetRanking(void)
{
	return g_Ranking;		// �l��Ԃ�
}