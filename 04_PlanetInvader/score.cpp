//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "score.h"			// �X�R�A
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "number.h"			// ����

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int			CScore::m_nScore[MAX_PLAYER]			= {};		// �X�R�A

//=============================================================================
// �R���X�g���N�^									(public)	*** CScore ***
//=============================================================================
CScore::CScore() : CScene(CScene::PRIORITY_UI)
{
	// �����l�ݒ�
	for (int nCntDigit = 0; nCntDigit < SCORE_MAX_DIGIT; nCntDigit++)
	{// �J�E���g
		m_apNumber2D[nCntDigit] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CScore ***
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// �m�ۏ���											(public)	*** CScore ***
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nScoreNum)
{
	CScore *pScore = NULL;					// �|�C���^

	if (pScore == NULL)
	{// NULL�̏ꍇ
		pScore = new CScore;				// �������m��
	}

	if (pScore != NULL)
	{// NULL�ȊO�̏ꍇ
		pScore->m_nScoreNum = nScoreNum;	// �X�R�A�ԍ�

		for (int nCntDigit = 0; nCntDigit < SCORE_MAX_DIGIT; nCntDigit++)
		{// �J�E���g
			if (pScore->m_apNumber2D[nCntDigit] == NULL)
			{
				pScore->m_apNumber2D[nCntDigit] = new CNumber2D;																		// �������m��
				pScore->m_apNumber2D[nCntDigit]->Init(D3DXVECTOR3(pos.x - ((size.x * 1.8f) * nCntDigit), pos.y, 0.0f), size, col);		// ����������
				pScore->m_apNumber2D[nCntDigit]->SetObjType(CScene::OBJTYPE_SCORE);														// �I�u�W�F�N�g�^�C�v�ݒ�
			}
		}
		pScore->m_nScore[pScore->m_nScoreNum] = 0;

		pScore->Init();						// ����������
	}

	return pScore;			// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CScore ***
//=============================================================================
HRESULT CScore::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CScore ***
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntDigit = 0; nCntDigit < SCORE_MAX_DIGIT; nCntDigit++)
	{// �J�E���g
		m_apNumber2D[nCntDigit]->Uninit();	// �I������
		m_apNumber2D[nCntDigit] = NULL;		// NULL��
	}

	CScene::Release();
}

//=============================================================================
// �X�V����											(public)	*** CScore ***
//=============================================================================
void CScore::Update(void)
{
	float fScoreData = (float)m_nScore[m_nScoreNum];	// �X�R�A���i�[

	for (int nCntDigit = 0; nCntDigit < SCORE_MAX_DIGIT; nCntDigit++)
	{// �J�E���g
		if (fScoreData < 1.0f && nCntDigit != 0)
		{
			m_apNumber2D[nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
		else
		{
			m_apNumber2D[nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		m_apNumber2D[nCntDigit]->SetTex(10, (int)fScoreData % 10, D3DXVECTOR2(0.0f, 0.0f));		// �e�N�X�`���ݒ�

		fScoreData /= 10;		// ����i�߂�
	}
}

//=============================================================================
// �`�揈��											(public)	*** CScore ***
//=============================================================================
void CScore::Draw(void)
{

}