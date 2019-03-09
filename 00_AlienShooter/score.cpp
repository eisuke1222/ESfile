//=========================================================================================================================
// 
// �X�R�A���� [score.cpp]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#define _CRT_SECURE_NO_WARNINGS				// scanf�G���[����
#include <stdio.h>		// io
#include "score.h"		// �X�R�A
#include "ranking.h"	// �����L���O
#include "input.h"		// �L�[�{�[�h

//*************************************************************************************************************************
// �}�N����`
//*************************************************************************************************************************
#define MAX_SCORE_SET		(5)								// �X�R�A�̍ő吔
#define MAX_SCORE_NUMBER	(8)								// �X�R�A�̍ő包��
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SCORE_FILE			"data/SAVE/scoreranking.bin"	// �ǂݍ��ރX�R�A�e�L�X�g�t�@�C����
#define SCORE_SPEED			(15.0f)							// �X�R�A�̈ړ����x

//*************************************************************************************************************************
// �O���[�o���ϐ�
//*************************************************************************************************************************
LPDIRECT3DTEXTURE9			g_pTextureScore = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffScore = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
Score						g_Score[MAX_SCORE_SET];			// �X�R�A���
int							g_nScoreRnking[MAX_SCORE_SET + 1];		// �X�R�A�i�[
int							g_nScoreRankIn;					// �����N�C�������X�R�A�ԍ�
int							g_nModeOld;						// �P�O�̃��[�h
int							g_nSetScore;					// �����N�C���`�F�b�N�p

//=========================================================================================================================
// �X�R�A����������
//=========================================================================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntSet;					// �Z�b�g�J�E���^
	float nCntScore;				// �X�R�A�J�E���^
	float nScorePos;				// �X�R�A�ʒu

	// �X�R�A�̏�����
	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		g_Score[nCntSet].nScore = 0;								// �X�R�A�������� 
		g_Score[nCntSet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �X�R�A���S�l
		g_Score[nCntSet].posmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�R�A�Ԋu�l
		g_Score[nCntSet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �X�R�A�F
		g_Score[nCntSet].bUse = false;								// �g�p���Ă��邩�ǂ���
	}
	for (nCntSet = 0; nCntSet <= MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		g_nScoreRnking[nCntSet] = 0;	//�X�R�A������
	}
	g_nScoreRankIn = 6;					// �����N�C���ԍ�
	g_nSetScore = 0;					// �`�F�b�N�p

	g_nModeOld = GetMode();				// �O�񃂁[�h

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SCORE,
		&g_pTextureScore
	);

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
			pVtx[0].pos = D3DXVECTOR3(nScorePos, 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(nScorePos + 30.0f, 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(nScorePos, 80.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(nScorePos + 30.0f, 80.0f, 0.0f);
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
	{// NULL�ȊO�̏ꍇ
		g_pTextureScore->Release();		// �J��
		g_pTextureScore = NULL;			// NULL��
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{// NULL�ȊO�̏ꍇ
		g_pVtxBuffScore->Release();		// �J��
		g_pVtxBuffScore = NULL;			// NULL��
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

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if (g_nSetScore == 0)
	{
		MODE mode = GetMode();		// ���[�h���擾

		if (mode == MODE_RANKING)
		{// ���U���g�̏ꍇ
			AddScore(SCORETYPE_RNKING_NO1, g_nScoreRnking[0]);
			AddScore(SCORETYPE_RNKING_NO2, g_nScoreRnking[1]);
			AddScore(SCORETYPE_RNKING_NO3, g_nScoreRnking[2]);
			AddScore(SCORETYPE_RNKING_NO4, g_nScoreRnking[3]);
			AddScore(SCORETYPE_RNKING_NO5, g_nScoreRnking[4]);

			for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
			{// �Z�b�g�J�E���^
				if (g_nScoreRnking[nCntSet] == g_nScoreRnking[5])
				{// �X�R�A��������������
					g_nScoreRankIn = nCntSet;
				}
			}
		}

		g_nSetScore = 1;
	}

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		if (g_Score[nCntSet].bUse == true)
		{// �X�R�A���g�p����Ă���ꍇ
			nScoreData = g_Score[nCntSet].nScore;	// �X�R�A���i�[

			for (nCntScore = 0; nCntScore < MAX_SCORE_NUMBER; nCntScore++)
			{// �X�R�A�J�E���g
				// �e�N�X�`���ݒ�
				pVtx[nCntScore * 4].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f, 0.0f);
				pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f + 0.1f, 0.0f);
				pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f, 1.0f);
				pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2((nScoreData % 10) * 0.1f + 0.1f, 1.0f);

				nScoreData /= 10;		// ����i�߂�
			}

			g_Score[nCntSet].posmove = g_Score[nCntSet].pos;	// �ʒu����
			MODE Mode = GetMode();		// ��ʂ��擾

			if (Mode == MODE_RANKING)
			{// �����L���O�̏ꍇ
				for (nCntScore = 0; nCntScore < MAX_SCORE_NUMBER; nCntScore++)
				{// �X�R�A���J�E���g
					// ���_�ݒ�
					pVtx[nCntScore * 4].pos = D3DXVECTOR3(g_Score[nCntSet].posmove.x, g_Score[nCntSet].posmove.y - g_Score[nCntSet].nRadius.y, 0.0f);
					pVtx[nCntScore * 4 + 1].pos = D3DXVECTOR3(g_Score[nCntSet].posmove.x + g_Score[nCntSet].nRadius.x, g_Score[nCntSet].posmove.y - g_Score[nCntSet].nRadius.y, 0.0f);
					pVtx[nCntScore * 4 + 2].pos = D3DXVECTOR3(g_Score[nCntSet].posmove.x, g_Score[nCntSet].posmove.y + g_Score[nCntSet].nRadius.y, 0.0f);
					pVtx[nCntScore * 4 + 3].pos = D3DXVECTOR3(g_Score[nCntSet].posmove.x + g_Score[nCntSet].nRadius.x, g_Score[nCntSet].posmove.y + g_Score[nCntSet].nRadius.y, 0.0f);

					if (g_nModeOld == MODE_RESULT)
					{
						if (nCntSet == g_nScoreRankIn && g_nScoreRnking[5] != 0)
						{
							// ���_�J���[�ݒ�
							pVtx[nCntScore * 4].col = D3DXCOLOR(1.0f, 1.0f, 0.1f, g_Score[nCntSet].col.a);
							pVtx[nCntScore * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 0.1f, g_Score[nCntSet].col.a);
							pVtx[nCntScore * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 0.1f, g_Score[nCntSet].col.a);
							pVtx[nCntScore * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 0.1f, g_Score[nCntSet].col.a);
						}
					}

					g_Score[nCntSet].posmove.x -= g_Score[nCntSet].nRadius.x;	// �ʒu�����炷
				}

				if (GetKeyboardPress(DIK_1) == true && GetKeyboardPress(DIK_2) == true && GetKeyboardPress(DIK_3) == true)
				{// �X�R�A������
					for (nCntSet = 0; nCntSet <= MAX_SCORE_SET; nCntSet++)
					{// �X�R�A���J�E���g
						g_nScoreRnking[nCntSet] = 0;
						g_Score[nCntSet].nScore = 0;
					}
					// �Z�[�u����
					SaveScore(SCORETYPE_RNKING_NO1);
				}

				Ranking g_Ranking = GetRanking();		// �����L���O���擾

				g_Score[nCntSet].pos.x -= g_Ranking.move.x;	// �ʒu���X�V
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
			for (nCntScore = 0; nCntScore < MAX_SCORE_NUMBER; nCntScore++)
			{// �X�R�A�J�E���g
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4 + nCntSet * (MAX_SCORE_NUMBER * 4), 2);
			}
		}
	}
}

//=========================================================================================================================
// �X�R�A�ݒ菈��
//=========================================================================================================================
void SetScore(D3DXVECTOR3 pos, D3DXVECTOR3 nRadius, D3DXCOLOR col)
{
	int nCntSet;		// �Z�b�g�J�E���^
	int nCntScore;		// �X�R�A�J�E���^
	D3DXVECTOR3 nPos;	// �ʒu���i�[

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

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
			g_Score[nCntSet].nRadius = nRadius;					// �傫��
			g_Score[nCntSet].col = col;							// �F
			g_Score[nCntSet].bUse = true;						// �g�p���Ă�����

			for (nCntScore = 0; nCntScore < MAX_SCORE_NUMBER; nCntScore++)
			{// �X�R�A���J�E���g
				// ���_�ݒ�
				pVtx[nCntScore * 4].pos = D3DXVECTOR3(nPos.x, nPos.y - g_Score[nCntSet].nRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 1].pos = D3DXVECTOR3(nPos.x + g_Score[nCntSet].nRadius.x, nPos.y - g_Score[nCntSet].nRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 2].pos = D3DXVECTOR3(nPos.x, nPos.y + g_Score[nCntSet].nRadius.y, 0.0f);
				pVtx[nCntScore * 4 + 3].pos = D3DXVECTOR3(nPos.x + g_Score[nCntSet].nRadius.x, nPos.y + g_Score[nCntSet].nRadius.y, 0.0f);
				// ���_�J���[�ݒ�
				pVtx[nCntScore * 4].col = D3DXCOLOR(g_Score[nCntSet].col.r, g_Score[nCntSet].col.g, g_Score[nCntSet].col.b, g_Score[nCntSet].col.a);
				pVtx[nCntScore * 4 + 1].col = D3DXCOLOR(g_Score[nCntSet].col.r, g_Score[nCntSet].col.g, g_Score[nCntSet].col.b, g_Score[nCntSet].col.a);
				pVtx[nCntScore * 4 + 2].col = D3DXCOLOR(g_Score[nCntSet].col.r, g_Score[nCntSet].col.g, g_Score[nCntSet].col.b, g_Score[nCntSet].col.a);
				pVtx[nCntScore * 4 + 3].col = D3DXCOLOR(g_Score[nCntSet].col.r, g_Score[nCntSet].col.g, g_Score[nCntSet].col.b, g_Score[nCntSet].col.a);

				nPos.x -= g_Score[nCntSet].nRadius.x;	//�ʒu�����炷
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
	MODE mode = GetMode();		// ���[�h���擾

	if (mode != MODE_TUTORIAL)
	{// �`���[�g���A���ȊO�̏ꍇ
		if (g_Score[Type].bUse == true)
		{// �g�p���Ă���ꍇ
			g_Score[Type].nScore += nValue;		// �X�R�A����
		}
	}
}

//=========================================================================================================================
// �X�R�A�̃Z�[�u����
//=========================================================================================================================
void SaveScore(SCORETYPE Type)
{
	// �ϐ��錾
	FILE *pFile;		// �t�@�C���|�C���^
	int nCntSet;		// �Z�b�g�J�E���^
	int nCntSetSort;	// ����
	int nScoreData;		// �l���i�[

	g_nScoreRnking[5] = g_Score[Type].nScore;	// �z��Ɋi�[

	for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
	{// �Z�b�g���J�E���g
		for (nCntSetSort = nCntSet + 1; nCntSetSort < MAX_SCORE_SET + 1; nCntSetSort++)
		{// ��r�Ώ�
			if (g_nScoreRnking[nCntSet] < g_nScoreRnking[nCntSetSort])
			{// �~���ɐ���
				nScoreData = g_nScoreRnking[nCntSetSort];	// �l���i�[

				g_nScoreRnking[nCntSetSort] = g_nScoreRnking[nCntSet];	// �l�̓���ւ�
				g_nScoreRnking[nCntSet] = nScoreData;		// �l�̓���ւ�
			}
		}
	}
	g_nScoreRnking[5] = g_Score[Type].nScore;	// �z��Ɋi�[

	// �t�@�C���ݒ�
	/*pFile = fopen(SCORE_FILE, "w");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		for (nCntSet = 0; nCntSet < MAX_SCORE_SET; nCntSet++)
		{// �Z�b�g���J�E���g


			//if (g_Score[Type].nScore > g_nScoreRnking[nCntSet])
			//{
			//	for (nCntSetSort = MAX_SCORE_SET; nCntSetSort > nCntSet; nCntSetSort--)
			//	{// ��r�Ώ�
			//		g_nScoreRnking[nCntSetSort] = g_nScoreRnking[nCntSetSort - 1];	// �l�̓���ւ�
			//	}
			//	g_nScoreRnking[nCntSet] = g_Score[Type].nScore;
			//}
			//fprintf(pFile, "%d\n", g_nScoreRnking[nCntSet]);	// ��������

			
			for (nCntSetSort = nCntSet + 1; nCntSetSort < MAX_SCORE_SET + 1; nCntSetSort++)
			{// ��r�Ώ�
				if (g_nScoreRnking[nCntSet] < g_nScoreRnking[nCntSetSort])
				{// �~���ɐ���
					nScoreData = g_nScoreRnking[nCntSetSort];	// �l���i�[

					g_nScoreRnking[nCntSetSort] = g_nScoreRnking[nCntSet];	// �l�̓���ւ�
					g_nScoreRnking[nCntSet] = nScoreData;		// �l�̓���ւ�
				}
			}
			fprintf(pFile, "%d\n", g_nScoreRnking[nCntSet]);	// ��������
		}
		fprintf(pFile, "%d\n", g_Score[Type].nScore);			// ��������

		fclose(pFile);	// �t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}*/
	
	// �t�@�C���ݒ�
	pFile = fopen(SCORE_FILE, "wb");

	int nData[6] = { g_nScoreRnking[0], g_nScoreRnking[1], g_nScoreRnking[2], g_nScoreRnking[3], g_nScoreRnking[4], g_nScoreRnking[5] };

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		fwrite(&nData, sizeof(int), 6, pFile);
		fclose(pFile);
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
	FILE *pFile;	// �t�@�C���|�C���^
	//int nCntSet;	// �Z�b�g�J�E���^

	// �t�@�C���ݒ�
	pFile = fopen(SCORE_FILE, "rb");

	if (pFile != NULL)
	{// �t�@�C�����J�����ꍇ
		/*for (nCntSet = 0; nCntSet < MAX_SCORE_SET + 1; nCntSet++)
		{// �Z�b�g�J�E���^
			fscanf(pFile, "%d", &g_nScoreRnking[nCntSet]);		// �ǂݍ���
		}*/
		fread(&g_nScoreRnking, sizeof(int), 6, pFile);
		fclose(pFile);				//�t�@�C�������
	}
	else
	{// �t�@�C�����J���Ȃ��ꍇ
		MessageBox(0, "���[�h�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
	}
}