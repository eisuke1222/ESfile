//=========================================================================================================================
// 
// �X�R�A���� [score.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS		// scanf�G���[����
#include <stdio.h>					// stdio
#include "score.h"					// �X�R�A
#include "input.h"					// �L�[�{�[�h
#include "item.h"					// �A�C�e��
#include "player.h"					// �v���C���[
#include "map.h"					// �}�b�v
#include "fade.h"					// �t�F�[�h
#include "bgwindow.h"				// �w�i

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_SCORE_SET		(10)							// �X�R�A�̍ő吔
#define MAX_SCORE_NUMBER	(6)								// �X�R�A�̍ő包��
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SCORE_FILE			"data/SAVE/scorernking.txt"		// �ǂݍ��ރX�R�A�e�L�X�g�t�@�C����
#define SCORE_SPEED			(15.0f)							// �X�R�A�̈ړ����x

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureScore = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffScore = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
Score						g_Score[MAX_SCORE_SET];			// �X�R�A���
int							g_HiScore[GAMESTAGE_MAX];		// �n�C�X�R�A
int							g_nCntScoreTime;				// ���ԃJ�E���^

//=========================================================================================================================
// �X�R�A����������
//=========================================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntSet;				// �Z�b�g�J�E���^
	float nCntScore;			// �X�R�A�J�E���^
	float nScorePos;			// �X�R�A�ʒu

	// �X�R�A�̏�����
	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		g_Score[nCntSet].nScore = 0;								// �X�R�A�������� 
		g_Score[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �X�R�A���S�l
		g_Score[nCntSet].posmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�R�A�Ԋu�l
		g_Score[nCntSet].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�R�A�Ԋu�l
		g_Score[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �X�R�A�F
		g_Score[nCntSet].nSetNum = 0;								// ����
		g_Score[nCntSet].type = SCORETYPE_SCORE;					// ���
		g_Score[nCntSet].bUse = false;								// �g�p���Ă��邩�ǂ���
	}
	g_nCntScoreTime = 0;											// ������

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_SCORE, &g_pTextureScore);

	// ���_�o�b�t�@����
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * (MAX_SCORE_NUMBER * MAX_SCORE_SET),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		for (nCntScore = 0, nScorePos = 1200; nCntScore < MAX_SCORE_NUMBER; nCntScore++, nScorePos -= 30)
		{// �����ƃJ�E���g
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
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pVtx += 4;	// ���_�f�[�^�̃|�C���^���S���i�߂�
		}
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//=========================================================================================================================
// �X�R�A�I������
//=========================================================================================================================
void UninitScore(void)
{
	// �e�N�X�`���[�̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=========================================================================================================================
// �X�R�A�X�V����
//=========================================================================================================================
void UpdateScore(void)
{
	// �ϐ��錾
	int nCntSet;		// �Z�b�g�J�E���^
	int nCntScore;		// �X�R�A�J�E���^
	int nScoreData;		// �X�R�A�i�[
	int nSelectMap = GetSelectMap();	// �Q�[���X�e�[�W���
	PLAYER *pPlayer = GetPlayer();
	bool bGoalHit = GetGoalHit();

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		if (g_Score[nCntSet].bUse == true)
		{// �X�R�A���g�p����Ă���ꍇ
			if (g_Score[nCntSet].type == SCORETYPE_HISCORE)
			{
				if (g_Score[nCntSet].nScore != g_HiScore[nSelectMap])
				{
					g_Score[nCntSet].nScore = g_HiScore[nSelectMap];
				}
			}
			else if (g_Score[nCntSet].type == SCORETYPE_TIME)
			{// ���Ԃ̏ꍇ
				g_nCntScoreTime++;					// �J�E���^��i�߂�
				if (g_nCntScoreTime % 60 == 0 && g_Score[nCntSet].nScore > 0)
				{// ���b�A�R�C����1���ȏ゠��ꍇ
					if (g_Score[nCntSet].nScore > 0 && bGoalHit == false)
					{// �X�R�A�i���ԁj1�ȏ�
						g_Score[nCntSet].nScore -= 1;		// �X�R�A�i���ԁj�����Z
					
						if (g_Score[nCntSet].nScore == 30)
						{
							g_Score[nCntSet].col = D3DXCOLOR(1.0f, 0.5f, 0.3f, 1.0f);
						}
					}
					
					if (g_Score[nCntSet].nScore <= 0)
					{// �X�R�A�i���ԁj��0�ȉ�
						pPlayer->nNumPlayer--;

						if (pPlayer->nNumPlayer <= 0)
						{
							SetGameState(GAMESTATE_GAMEOVER);	// �Q�[���I�[�o�[
						}
						else
						{
							SetFadeGame(GAMESTATE_MAP);			// �Q�[���}�b�v
						}
					}
				}
			}

			nScoreData = g_Score[nCntSet].nScore;	// �X�R�A���i�[

			for (nCntScore = 0; nCntScore < MAX_SCORE_NUMBER; nCntScore++)
			{// �X�R�A�J�E���g
				// �e�N�X�`���ݒ�
				pVtx[nCntScore * 4].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f, 0.0f);
				pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f + 0.1f, 0.0f);
				pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f, 1.0f);
				pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f + 0.1f, 1.0f);
				// ���_�J���[�ݒ�
				pVtx[nCntScore * 4].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 1].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 2].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 3].col = g_Score[nCntSet].col;

				nScoreData /= 10;		// ����i�߂�
			}
		}
		pVtx += (MAX_SCORE_NUMBER * 4);		// ���_�f�[�^�̃|�C���^�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//=========================================================================================================================
// �X�R�A�`�揈��
//=========================================================================================================================
void DrawScore(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntSet;		// �Z�b�g�J�E���^
	int nCntScore;		// �X�R�A�J�E���^

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource
	(
		0,
		g_pVtxBuffScore,
		0,
		sizeof(VERTEX_2D)
	);

	// ���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	// �X�R�A�̕`��
	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		if (g_Score[nCntSet].bUse == true)
		{// �g�p���Ă���ꍇ
			for (nCntScore = 0; nCntScore < g_Score[nCntSet].nSetNum; nCntScore++)
			{// �X�R�A�J�E���g
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4 + nCntSet * (MAX_SCORE_NUMBER * 4), 2);
			}
		}
	}
}

//=========================================================================================================================
// �X�R�A�ݒ菈��
//=========================================================================================================================
void SetScore(D3DXVECTOR3 pos, D3DXVECTOR3 fRadius, D3DXCOLOR col, SCORETYPE type)
{
	// �ϐ��錾
	int nCntSet;		// �Z�b�g�J�E���^
	int nCntScore;		// �X�R�A�J�E���^
	D3DXVECTOR3 nPos;	// �ʒu���i�[

	PLAYER *pPlayer = GetPlayer();		// �v���C���[���
	int nNumCoin = GetNumCoin();		// �R�C������

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		if (g_Score[nCntSet].bUse == false)
		{// �g�p����Ă���ꍇ
			g_Score[nCntSet].pos = pos;							// �ʒu
			g_Score[nCntSet].posmove = g_Score[nCntSet].pos;	// �ړ��l
			nPos = g_Score[nCntSet].pos;						// �ʒu���i�[
			g_Score[nCntSet].fRadius = fRadius;					// �傫��
			g_Score[nCntSet].col = col;							// �F
			g_Score[nCntSet].type = type;						// �X�R�A�^�C�v
			g_Score[nCntSet].bUse = true;						// �g�p���Ă�����

			switch (g_Score[nCntSet].type)
			{// �X�R�A���
			case SCORETYPE_SCORE:		// �X�R�A
				g_Score[nCntSet].nScore = 0;					// �X�R�A�����l
				g_Score[nCntSet].nSetNum = MAX_SCORE_NUMBER;	// ����
				break;
			case SCORETYPE_HISCORE:		// �n�C�X�R�A
				g_Score[nCntSet].nScore = 0;					// �X�R�A�����l
				g_Score[nCntSet].nSetNum = MAX_SCORE_NUMBER;	// ����
				break;
			case SCORETYPE_COIN:		// �R�C��
				g_Score[nCntSet].nScore = nNumCoin;				// �X�R�A�����l
				g_Score[nCntSet].nSetNum = 2;					// ����
				break;
			case SCORETYPE_TIME:		// ����
				g_Score[nCntSet].nScore = 99;					// �X�R�A�����l
				g_Score[nCntSet].nSetNum = 2;					// ����
				break;
			case SCORETYPE_PLAYER:		// �v���C���[
				g_Score[nCntSet].nScore = pPlayer->nNumPlayer;	// �X�R�A�����l
				g_Score[nCntSet].nSetNum = 2;					// ����
				break;
			}
			
			for (nCntScore = 0; nCntScore < g_Score[nCntSet].nSetNum; nCntScore++)
			{// �X�R�A���J�E���g
				// ���_�ݒ�
				pVtx[nCntScore * 4].pos = D3DXVECTOR3(nPos.x, nPos.y - g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 1].pos = D3DXVECTOR3(nPos.x + g_Score[nCntSet].fRadius.x, nPos.y - g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 2].pos = D3DXVECTOR3(nPos.x, nPos.y + g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 3].pos = D3DXVECTOR3(nPos.x + g_Score[nCntSet].fRadius.x, nPos.y + g_Score[nCntSet].fRadius.y, 0.0f);
				// ���_�J���[�ݒ�
				pVtx[nCntScore * 4].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 1].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 2].col = g_Score[nCntSet].col;
				pVtx[nCntScore * 4 + 3].col = g_Score[nCntSet].col;

				nPos.x -= g_Score[nCntSet].fRadius.x;	//�ʒu�����炷
			}

			break;
		}
		pVtx += (MAX_SCORE_NUMBER * 4); 	// ���_�f�[�^�̃|�C���^�i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//=========================================================================================================================
// �X�R�A�̎擾����
//=========================================================================================================================
Score GetScore(void)
{
	return g_Score[0];		// �l��Ԃ�
}

//=========================================================================================================================
// �X�R�A�̉��Z����
//=========================================================================================================================
void AddScore(SCORETYPE Type, int nValue)
{
	// �ϐ��錾
	int nCntSet;

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		if (g_Score[nCntSet].bUse == true && g_Score[nCntSet].type == Type)
		{// �g�p���Ă���ꍇ
			if (g_Score[nCntSet].type == SCORETYPE_HISCORE)
			{
				g_Score[nCntSet].nScore = nValue;	// �X�R�A����
			}
			else
			{
				g_Score[nCntSet].nScore += nValue;	// �X�R�A����
			}
		}
	}
}

//=========================================================================================================================
// �X�R�A�i�[����
//=========================================================================================================================
void SetHiScore(void)
{
	// �ϐ��錾
	int nCntSet;

	GAMESTATE game = GetGameState();	// �Q�[�����
	GAMESTAGE stage = GetNumStage();	// �Q�[���X�e�[�W���

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		if (g_Score[nCntSet].bUse == true)
		{// �g�p����Ă���ꍇ
			if (game == GAMESTATE_GAMEPLAY && g_Score[nCntSet].type == SCORETYPE_SCORE)
			{
				if (g_HiScore[stage] < g_Score[nCntSet].nScore)
				{
					g_HiScore[stage] += g_Score[nCntSet].nScore;
				}
			}
			else if (game == GAMESTATE_GAMEPLAY && g_Score[nCntSet].type == SCORETYPE_TIME)
			{
				if (g_Score[nCntSet].nScore > 0)
				{
					g_HiScore[stage] += g_Score[nCntSet].nScore * 10;
				}
			}
		}
	}
}

//=========================================================================================================================
// �X�R�A�̔j������
//=========================================================================================================================
void DeleteScore(void)
{
	// �ϐ��錾
	int nCntSet;

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		if (g_Score[nCntSet].bUse == true)
		{// �g�p����Ă���ꍇ
			g_Score[nCntSet].nScore = 0;								// �X�R�A�������� 
			g_Score[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �X�R�A���S�l
			g_Score[nCntSet].posmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�R�A�Ԋu�l
			g_Score[nCntSet].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�R�A�Ԋu�l
			g_Score[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �X�R�A�F
			g_Score[nCntSet].nSetNum = 0;								// ����
			g_Score[nCntSet].type = SCORETYPE_SCORE;					// ���
			g_Score[nCntSet].bUse = false;								// �g�p���Ă��邩�ǂ���
		}
	}
	g_nCntScoreTime = 0;												// ������
}

//=========================================================================================================================
// �X�R�A�̃Z�[�u����
//=========================================================================================================================
void SaveScore(SCORETYPE Type)
{/*
	// �ϐ��錾
	FILE *pFile;		// �t�@�C���|�C���^
	int nCntSet;		// �Z�b�g�J�E���^
	int nCntSetSort;	// ����
	int nScoreData;		// �l���i�[

	// �t�@�C���ݒ�
	pFile = fopen(SCORE_FILE, "w");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
	
		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}*/
}

//=========================================================================================================================
// �X�R�A�̃��[�h����
//=========================================================================================================================
void LoadScore(void)
{/*
	// �ϐ��錾
	FILE *pFile;	// �t�@�C���|�C���^
	int nCntSet;	// �Z�b�g�J�E���^

	// �t�@�C���ݒ�
	pFile = fopen(SCORE_FILE, "r");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
	
		fclose(pFile);				//�t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "���[�h�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}*/
}