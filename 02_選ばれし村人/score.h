//=========================================================================================================================
// 
// �X�R�A���� [score.h]
// Author : Eisuke Sakagawa
// 
//=========================================================================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

//*************************************************************************************************************************
// �C���N���[�h�t�@�C��
//*************************************************************************************************************************
#include "main.h"

//*************************************************************************************************************************
// �\���̒�`
//*************************************************************************************************************************
typedef enum
{// ���
	SCORETYPE_SCORE = 0,		// �X�R�A
	SCORETYPE_RANKING,			// �����L���O
	SCORETYPE_MAX,		
}SCORETYPE;

typedef struct
{// �X�R�A
	int nScore;					// �X�R�A
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3 move;			// �ړ�
	float fLimitMove;			// �ړ�����
	D3DXVECTOR3 fRadius;		// �傫���i���a�j
	D3DXCOLOR col;				// �F
	int nSetNum;				// ����
	SCORETYPE type;				// ���
	bool bUse;					// �g�p���Ă��邩�ǂ���
}Score;

//*************************************************************************************************************************
// �v���g�^�C�v�錾
//*************************************************************************************************************************
void InitScore(void);			// �X�R�A����������
void UninitScore(void);			// �X�R�A�I������
void UpdateScore(void);			// �X�R�A�X�V����
void DrawScore(void);			// �X�R�A�`��

void SetScore(D3DXVECTOR3 pos, D3DXVECTOR3 fRadius, D3DXCOLOR col, SCORETYPE type);		// �X�R�A�ݒ菈��
Score GetScore(void);							// �X�R�A�擾����
void AddScore(SCORETYPE Type, int nValue);		// �X�R�A���Z����
void SetRankingScore(void);						// �X�R�A�i�[����
void DeleteScore(void);							// �X�R�A�j������

void SaveScore(void);						// �X�R�A�Z�[�u����
void LoadScore(void);							// �X�R�A���[�h����

#endif