//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "ranking.h"		// �����L���O
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "title.h"			// �^�C�g��
#include "number.h"			// ����
#include "texture.h"		// �e�N�X�`��
#include "scene3d.h"		// �V�[��3D
#include "camera.h"			// �J����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RANKING_TEXT_NAME	"data\\TEXT\\SCORE\\ranking.bin"		// �e�L�X�g

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int		CRanking::m_nRanking[m_RANKING_NUM] = { 0 };		// �����L���O
int		CRanking::m_nScore					= 0;			// �X�R�A
int		CRanking::m_RankInNum				= 0;			// �����N�C���ԍ�

//=============================================================================
// �R���X�g���N�^									(public)	*** CRanking ***
//=============================================================================
CRanking::CRanking() : CScene(CScene::PRIORITY_UI, CScene::OBJTYPE_RANKING)
{
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �p�x
	m_size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �傫��
	m_col			= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F
	m_NextInterval	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �Ԋu
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CRanking ***
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// ���[�h����							(public)	*** CNumber ***
//=============================================================================
HRESULT CRanking::Load(void)
{
	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CNumber ***
//=============================================================================
void CRanking::Unload(void)
{
	
}

//=============================================================================
// �����L���O�Z�[�u									(public)	*** CRanking ***
//=============================================================================
void CRanking::SaveRanking(void)
{
	FILE *pFile;						// �t�@�C���|�C���^
	int nScore = m_nScore;

	if (m_nRanking[4] < nScore)
	{
		m_nRanking[4] = nScore;

		int nMin;	// �\�[�g�Ώ�
		int nData;	// �X�R�A

		for (int nCntRanking = 0; nCntRanking < m_RANKING_NUM; nCntRanking++)
		{// �J�E���g
			nMin = nCntRanking;		// �ԍ����L��

			while (nMin > 0 && m_nRanking[nMin - 1] < m_nRanking[nMin])
			{// �l��������������
				// �}�b�v���\�[�g
				nData = m_nRanking[nMin - 1];
				m_nRanking[nMin - 1] = m_nRanking[nMin];
				m_nRanking[nMin] = nData;

				nMin--;			// �\�[�g�Ώۂ��ړ�
			}
		}
	}

	m_RankInNum = 99;

	for (int nCntRanking = 0; nCntRanking < m_RANKING_NUM; nCntRanking++)
	{// �J�E���g
		if (nScore == m_nRanking[nCntRanking] && nScore != 0)
		{// �����N�C���ԍ�
			m_RankInNum = nCntRanking;

			break;
		}
	}

/*	m_nRanking[0] = 30000;
	m_nRanking[1] = 25000;
	m_nRanking[2] = 20000;
	m_nRanking[3] = 15000;
	m_nRanking[4] = 10000;*/

	pFile = fopen(RANKING_TEXT_NAME, "wb");		// �t�@�C�����J��

	if (pFile != NULL)
	{// NULL�ȊO�̏ꍇ
		fwrite(&m_nRanking, sizeof(int), m_RANKING_NUM, pFile);		// ��������

		fclose(pFile);							// �t�@�C�������
	}

	m_nScore = 0;
}

//=============================================================================
// �����L���O���[�h									(public)	*** CRanking ***
//=============================================================================
void CRanking::LoadRanking(void)
{
	FILE *pFile;						// �t�@�C���|�C���^

	pFile = fopen(RANKING_TEXT_NAME, "rb");		// �t�@�C�����J��

	if (pFile != NULL)
	{
		fread(&m_nRanking, sizeof(int), m_RANKING_NUM, pFile);		// �ǂݍ���

		fclose(pFile);							// �t�@�C�������
	}

	m_RankInNum = 99;
	m_nScore = 0;
}

//=============================================================================
// �m�ۏ���											(public)	*** CRanking ***
//=============================================================================
CRanking *CRanking::Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 nextInterval)
{
	CRanking *pRanking = NULL;					// �|�C���^

	if (pRanking == NULL)
	{// NULL�̏ꍇ
		if (type == TYPE_2D)
			pRanking = new CRanking_2D;			// �������m��
	}

	if (pRanking != NULL)
	{// NULL�ȊO�̏ꍇ
		pRanking->m_pos = pos;					// �ʒu
		pRanking->m_rot = rot;					// �p�x
		pRanking->m_size = size;				// �傫��
		pRanking->m_col = col;					// �F
		pRanking->m_NextInterval = nextInterval;// �Ԋu
		pRanking->Init();						// ����������
	}

	return pRanking;			// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CRanking ***
//=============================================================================
void CRanking::Uninit(void)
{
	CScene::SetDeath();			// ���S�t���O
}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CRanking_2D ***
//=============================================================================
CRanking_2D::CRanking_2D() : CRanking()
{
	for (int nCntNum = 0; nCntNum < m_RANKING_NUM; nCntNum++)
	{
		for (int nCntDigit = 0; nCntDigit < m_RANKING_DIGIT; nCntDigit++)
			m_pNumber[nCntNum][nCntDigit] = NULL;

		m_pScene2D[nCntNum] = NULL;		// �V�[��2D
	}

	m_nCntTimer = 0;					// ���ԃJ�E���g
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CRanking_2D ***
//=============================================================================
CRanking_2D::~CRanking_2D()
{

}

//=============================================================================
// ���[�h����							(public)	*** CNumber ***
//=============================================================================
HRESULT CRanking_2D::Load(void)
{
	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CNumber ***
//=============================================================================
void CRanking_2D::Unload(void)
{

}

//=============================================================================
// ����������								(public)	*** CNumber ***
//=============================================================================
HRESULT CRanking_2D::Init(void)
{
	for (int nCntNum = 0; nCntNum < m_RANKING_NUM; nCntNum++)
	{// �����L���O���J�E���g
		float fScoreData = (float)m_nRanking[nCntNum];

		for (int nCntDigit = 0; nCntDigit < m_RANKING_DIGIT; nCntDigit++)
		{// �����J�E���g
			if (m_pNumber[nCntNum][nCntDigit] == NULL)
			{// NULL�̏ꍇ
				m_pNumber[nCntNum][nCntDigit] = new CNumber2D;
				m_pNumber[nCntNum][nCntDigit]->Init(m_pos + D3DXVECTOR3((-((m_size.x * 2.0f) + m_NextInterval.x) * nCntDigit) - m_size.x, (((m_size.y * 2.0f) + m_NextInterval.y) * nCntNum) + m_size.y, 0.0f), m_size, m_col);
				
				m_pNumber[nCntNum][nCntDigit]->SetTex(10, (int)fScoreData % 10, D3DXVECTOR2(0.1f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));					// �e�N�X�`���ݒ�
			}
	
			fScoreData /= 10;		// ����i�߂�
		}

		// ���ʕ\�L
		m_pScene2D[nCntNum] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
		m_pScene2D[nCntNum]->SetInitAll(m_pos + D3DXVECTOR3((-((m_size.x * 2.0f) + m_NextInterval.x) * m_RANKING_DIGIT) - (m_size.x * 2.0f), (((m_size.y * 2.0f) + m_NextInterval.y) * nCntNum) + (m_size.y * 0.8f), 0.0f),
			D3DXVECTOR3((m_size.x * 2.0f), (m_size.y * 0.8f), m_size.z), m_col, D3DXVECTOR2(1.0f, 0.2f));
		m_pScene2D[nCntNum]->Init();
		m_pScene2D[nCntNum]->BindTexture(CTexture::GetTexture(CTexture::TYPE_RESULT_RANK));
		m_pScene2D[nCntNum]->SetTex(1, nCntNum, D3DXVECTOR2(1.0f, 0.2f), D3DXVECTOR2(0.0f, 0.0f));
	}

	return S_OK;
}

//=============================================================================
// �I������								(public)	*** CNumber ***
//=============================================================================
void CRanking_2D::Uninit(void)
{
	for (int nCntNum = 0; nCntNum < m_RANKING_NUM; nCntNum++)
	{// �����L���O���J�E���g
		for (int nCntDigit = 0; nCntDigit < m_RANKING_DIGIT; nCntDigit++)
		{// �����J�E���g
			if (m_pNumber[nCntNum][nCntDigit] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pNumber[nCntNum][nCntDigit]->Uninit();	// �I������
				m_pNumber[nCntNum][nCntDigit] = NULL;		// NULL��
			}
		}

		if (m_pScene2D[nCntNum] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pScene2D[nCntNum]->Uninit();		// �I������
			m_pScene2D[nCntNum] = NULL;			// NULL��
		}
	}

	CRanking::Uninit();
}

//=============================================================================
// �X�V����								(public)	*** CNumber ***
//=============================================================================
void CRanking_2D::Update(void)
{
	for (int nCntNum = 0; nCntNum < m_RANKING_NUM; nCntNum++)
	{// �����L���O���J�E���g
		for (int nCntDigit = 0; nCntDigit < m_RANKING_DIGIT; nCntDigit++)
		{// �����J�E���g
			if (m_pNumber[nCntNum][nCntDigit] != NULL)
			{// NULL�ȊO�̏ꍇ
				if (m_RankInNum == nCntNum)
				{// �����N�C�������ԍ��Ɠ���
					if (m_nCntTimer < 30)
						m_pNumber[nCntNum][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
					else
						m_pNumber[nCntNum][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else
				{// ���̑�
					m_pNumber[nCntNum][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
		}
	}

	m_nCntTimer = (m_nCntTimer + 1) % 60;
}

//=============================================================================
// �`�揈��								(public)	*** CNumber ***
//=============================================================================
void CRanking_2D::Draw(void)
{

}