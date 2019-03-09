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
#include "sound.h"
#include "input.h"					// �L�[�{�[�h
#include "fade.h"					// �t�F�[�h
#include "game.h"
#include "window.h"

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_SCORE_SET		(10)							// �X�R�A�̍ő吔
#define MAX_SCORE_NUMBER	(6)								// �X�R�A�̍ő包��
#define MAX_SCORE_RANKING	(6)								// �����L���O�̍ő吔
#define	TEXTURE_SCORE		"data/TEXTURE/number002.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SCORE_FILE			"data/SAVE/scorernking.bin"		// �ǂݍ��ރX�R�A�e�L�X�g�t�@�C����
#define SCORE_SPEED			(15.0f)							// �X�R�A�̈ړ����x

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureScore = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffScore = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
Score						g_Score[MAX_SCORE_SET];			// �X�R�A���
int							g_RankingScore[MAX_SCORE_RANKING];		// �n�C�X�R�A
int							g_nCntScoreTime;				// ���ԃJ�E���^
int							g_RankinScore = 5;

//=========================================================================================================================
// �X�R�A����������
//=========================================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntSet, nCntRanking;	// �Z�b�g�J�E���^
	float nCntScore;			// �X�R�A�J�E���^
	float nScorePos;			// �X�R�A�ʒu
	MODE mode = GetMode();

	// �X�R�A�̏�����
	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		g_Score[nCntSet].nScore = 0;								// �X�R�A�������� 
		g_Score[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �X�R�A���S�l
		g_Score[nCntSet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �X�R�A�Ԋu�l
		g_Score[nCntSet].fLimitMove = 0.0f;		//
		g_Score[nCntSet].fRadius = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�R�A�Ԋu�l
		g_Score[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �X�R�A�F
		g_Score[nCntSet].nSetNum = 0;								// ����
		g_Score[nCntSet].type = SCORETYPE_SCORE;					// ���
		g_Score[nCntSet].bUse = false;								// �g�p���Ă��邩�ǂ���
	}
	for (nCntRanking = 0; nCntRanking < MAX_SCORE_RANKING; nCntRanking++)
	{
		g_RankingScore[nCntRanking] = 0;
	}

	g_nCntScoreTime = 0;											// ������

	if (mode != MODE_RESULT)
	{
		g_RankinScore = 5;
	}

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
		for (nCntScore = 0, nScorePos = 1200; nCntScore < MAX_SCORE_NUMBER; nCntScore++, nScorePos -= 50)
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
	D3DXVECTOR3 nPos;	// �ʒu���i�[
	MODE mode = GetMode();
	GAMESTATE game = GetGameState();

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

			switch (g_Score[nCntSet].type)
			{// �X�R�A���
			case SCORETYPE_SCORE:		// �X�R�A

				if (mode == MODE_RESULT)
				{// ���U���g
					if (1000 + 10 > g_Score[nCntSet].pos.x && 1000 - 10 < g_Score[nCntSet].pos.x && 500 + 10 > g_Score[nCntSet].pos.y && 500 - 10 < g_Score[nCntSet].pos.y)
					{// �~�܂�
						g_Score[nCntSet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

						if (g_nCntScoreTime == 1)
						{
							StopSound(SOUND_LABEL_SE_RESULT_0);
							PlaySound(SOUND_LABEL_SE_RESULT_1);
							PlaySound(SOUND_LABEL_SE_RESULT_2);
							g_nCntScoreTime = 2;
						}
					}
					else
					{// �����ړ�
						g_Score[nCntSet].move.x = sinf(atan2f((1000) - g_Score[nCntSet].pos.x, (500) - g_Score[nCntSet].pos.y)) * (g_Score[nCntSet].fLimitMove) * 20.0f;
						g_Score[nCntSet].move.y = cosf(atan2f((1000) - g_Score[nCntSet].pos.x, (500) - g_Score[nCntSet].pos.y)) * (g_Score[nCntSet].fLimitMove) * 20.0f;

						g_Score[nCntSet].fRadius.x += 0.8f;
						g_Score[nCntSet].fRadius.y += 0.8f;

						if (g_nCntScoreTime == 0)
						{
							PlaySound(SOUND_LABEL_SE_RESULT_0);
							g_nCntScoreTime = 1;
						}
					}

					if (g_Score[nCntSet].fLimitMove > 1.0f)
					{
						g_Score[nCntSet].fLimitMove = 1.0f;
					}
					else
					{
						g_Score[nCntSet].fLimitMove += 0.015f;
					}

					g_Score[nCntSet].pos += g_Score[nCntSet].move;
				}

				break;
			case SCORETYPE_RANKING:		// �����L���O

				if (game == GAMESTATE_GAMECLEAR || game == GAMESTATE_GAMEOVER)
				{// �N���A�E�I�[�o�[
					if (g_nCntScoreTime == 0 && g_RankinScore < 5)
					{
						PlaySound(SOUND_LABEL_SE_RESULT_2);
					}
					else if (g_nCntScoreTime == 0 && g_RankinScore == 5)
					{
						PlaySound(SOUND_LABEL_SE_RANKING);
					}

					// �����N
					if (game == GAMESTATE_GAMECLEAR)
					{
						if (g_Score[g_RankinScore].nScore > 40000)
						{
							SetWindow(D3DXVECTOR3(275.0f, 530.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, WINDOWTEXTURE_RANKING_A, SETTEXTURE_FRONT);
						}
						else
						{
							SetWindow(D3DXVECTOR3(275.0f, 530.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, WINDOWTEXTURE_RANKING_B, SETTEXTURE_FRONT);
						}
					}
					else if (game == GAMESTATE_GAMEOVER)
					{
						if (g_Score[g_RankinScore].nScore > 20000)
						{
							SetWindow(D3DXVECTOR3(275.0f, 530.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, WINDOWTEXTURE_RANKING_C, SETTEXTURE_FRONT);
						}
						else
						{
							SetWindow(D3DXVECTOR3(275.0f, 530.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 150, 150, WINDOWTEXTURE_RANKING_D, SETTEXTURE_FRONT);
						}
					}
				}

				g_nCntScoreTime++;

				// �����X�R�A�_��
				if (g_nCntScoreTime < 30)
				{
					g_Score[g_RankinScore].col = D3DXCOLOR(0.8f, 1.0f, 0.1f, 1.0f);
				}
				else
				{
					g_Score[g_RankinScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				if (g_nCntScoreTime == 61)
				{
					g_nCntScoreTime = 1;
				}

				break;
			}

			nPos = g_Score[nCntSet].pos;			// �ʒu���i�[
			nScoreData = g_Score[nCntSet].nScore;	// �X�R�A���i�[

			for (nCntScore = 0; nCntScore < MAX_SCORE_NUMBER; nCntScore++)
			{// �X�R�A�J�E���g
				// ���_�ݒ�
				pVtx[nCntScore * 4].pos = D3DXVECTOR3(nPos.x, nPos.y - g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 1].pos = D3DXVECTOR3(nPos.x + g_Score[nCntSet].fRadius.x, nPos.y - g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 2].pos = D3DXVECTOR3(nPos.x, nPos.y + g_Score[nCntSet].fRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 3].pos = D3DXVECTOR3(nPos.x + g_Score[nCntSet].fRadius.x, nPos.y + g_Score[nCntSet].fRadius.y, 0.0f);
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

				nPos.x -= g_Score[nCntSet].fRadius.x + 3;	//�ʒu�����炷
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
	bool SetScore = 0;
	D3DXVECTOR3 nPos;	// �ʒu���i�[

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
		if (g_Score[nCntSet].bUse == true && g_Score[nCntSet].type == SCORETYPE_SCORE && type == SCORETYPE_SCORE)
		{// �g�p����Ă���ꍇ
			SetScore = 1;
		}
	}

	if (SetScore == 0)
	{
		for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
		{// �Z�b�g���J�E���g
			if (g_Score[nCntSet].bUse == false)
			{// �g�p����Ă���ꍇ
				g_Score[nCntSet].pos = pos;							// �ʒu
				g_Score[nCntSet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ��l
				g_Score[nCntSet].fLimitMove = 0.01f;
				nPos = g_Score[nCntSet].pos;						// �ʒu���i�[
				g_Score[nCntSet].fRadius = fRadius;					// �傫��
				g_Score[nCntSet].col = col;							// �F
				g_Score[nCntSet].type = type;						// �X�R�A�^�C�v
				g_Score[nCntSet].bUse = true;						// �g�p���Ă�����

				switch (g_Score[nCntSet].type)
				{// �X�R�A���
				case SCORETYPE_SCORE:		// �X�R�A
				case SCORETYPE_RANKING:		// �����L���O
					g_Score[nCntSet].nScore = 0;					// �X�R�A�����l
					g_Score[nCntSet].nSetNum = MAX_SCORE_NUMBER;	// ����
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

					nPos.x -= g_Score[nCntSet].fRadius.x + 3;	//�ʒu�����炷
				}

				break;
			}

			pVtx += (MAX_SCORE_NUMBER * 4); 	// ���_�f�[�^�̃|�C���^�i�߂�
		}
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
			if (g_Score[nCntSet].type == SCORETYPE_RANKING)
			{
				LoadScore();

				for (int nCntRanking = nCntSet, nCntScore = 0; nCntRanking < MAX_SCORE_RANKING - 1; nCntRanking++, nCntScore++)
				{// �Z�b�g���J�E���g
					g_Score[nCntRanking].nScore = g_RankingScore[nCntScore];	// �X�R�A����
				}
				
				break;
			}
			else
			{
				if (g_Score[nCntSet].nScore < 999999)
				{
					g_Score[nCntSet].nScore += nValue;	// �X�R�A����
				}
				if (g_Score[nCntSet].nScore > 999999)
				{
					g_Score[nCntSet].nScore = 999999;
				}
			}

			break;
		}
	}
}

//=========================================================================================================================
// �X�R�A�i�[����
//=========================================================================================================================
void SetRankingScore(void)
{
	// �ϐ��錾
	int nCntSet;
	int nCntNum1, nCntNum2, nNumDate;

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		if (g_Score[nCntSet].bUse == true && g_Score[nCntSet].type == SCORETYPE_SCORE)
		{// �g�p���Ă���ꍇ
			LoadScore();

			g_RankingScore[MAX_SCORE_RANKING - 1] = g_Score[nCntSet].nScore;

			for (nCntNum1 = 0; nCntNum1 < MAX_SCORE_RANKING - 1; nCntNum1++)
			{
				for (nCntNum2 = nCntNum1; nCntNum2 < MAX_SCORE_RANKING; nCntNum2++)
				{
					if (g_RankingScore[nCntNum1] < g_RankingScore[nCntNum2])
					{
						nNumDate = g_RankingScore[nCntNum1];
						g_RankingScore[nCntNum1] = g_RankingScore[nCntNum2];
						g_RankingScore[nCntNum2] = nNumDate;

						if (g_RankinScore == 5)
						{
							g_RankinScore = nCntNum1;
						}
					}
				}
			}

			break;
		}
	}

	SaveScore();
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
			g_Score[nCntSet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �X�R�A�Ԋu�l
			g_Score[nCntSet].fLimitMove = 0.0f;
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
void SaveScore(void)
{
	// �ϐ��錾
	FILE *pFile;		// �t�@�C���|�C���^
	int nCntRanking;	// �Z�b�g�J�E���^

	// �t�@�C���ݒ�
	pFile = fopen(SCORE_FILE, "wb");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		/*for (nCntRanking = 0; nCntRanking < MAX_SCORE_RANKING; nCntRanking++)
		{
			g_RankingScore[nCntRanking] = 0;
		}*/

		fwrite(&g_RankingScore, sizeof(int), MAX_SCORE_RANKING, pFile);

		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}

//=========================================================================================================================
// �X�R�A�̃��[�h����
//=========================================================================================================================
void LoadScore(void)
{
	// �ϐ��錾
	FILE *pFile;		// �t�@�C���|�C���^
	int nCntRanking;	// �Z�b�g�J�E���^

	// �t�@�C���ݒ�
	pFile = fopen(SCORE_FILE, "rb");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		/*for (nCntRanking = 0; nCntRanking < MAX_SCORE_RANKING; nCntRanking++)
		{
			g_RankingScore[nCntRanking] = 0;
		}*/

		fread(&g_RankingScore, sizeof(int), MAX_SCORE_RANKING, pFile);

		fclose(pFile);				//�t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "���[�h�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}