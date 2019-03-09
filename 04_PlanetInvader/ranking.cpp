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
#include "scene3d.h"		// �V�[��3D
#include "camera.h"			// �J����
#include "score.h"			// �X�R�A

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\ranking.png"		// �e�N�X�`��
#define RANKING_TEXT_NAME	"data\\TEXT\\ranking.bin"			// �e�L�X�g

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CRanking::m_pTexture					= NULL;						// �e�N�X�`�����ւ̃|�C���^
CNumber3D			*CRanking::m_apNumber3D[RANKING_MAX_NUM][RANKING_MAX_DIGIT] = {};	// �i���o�[
CScene3D			*CRanking::m_apScene3D[RANKING_MAX_NUM] = {};						// �V�[��3D
int					CRanking::m_nRanking[RANKING_MAX_NUM]	= {};						// �����L���O
int					CRanking::m_RankInNum = 99;

//=============================================================================
// �R���X�g���N�^									(public)	*** CRanking ***
//=============================================================================
CRanking::CRanking() : CScene(CScene::PRIORITY_UI)
{
	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
	{// �J�E���g
		for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
		{// �J�E���g
			m_apNumber3D[nCntRanking][nCntDigit] = NULL;
		}
		m_pos[nCntRanking] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	m_fScene3DDiff = 0.0f;
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
	if (m_pTexture == NULL)
	{// NULL�̏ꍇ
		// �f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CNumber ***
//=============================================================================
void CRanking::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pTexture->Release();			// ���
		m_pTexture = NULL;				// NULL��
	}
}

//=============================================================================
// �����L���O�Z�[�u									(public)	*** CRanking ***
//=============================================================================
void CRanking::SaveRanking(void)
{
	FILE *pFile;						// �t�@�C���|�C���^
	int nScore = 0;
	
	for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
	{
		nScore += CScore::GetScore(nCnt);
	}

	if (m_nRanking[4] < nScore)
	{
		m_nRanking[4] = nScore;

		int nMin;	// �\�[�g�Ώ�
		int nData;	// �X�R�A

		for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
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

	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
	{// �J�E���g
		if (nScore == m_nRanking[nCntRanking] && nScore != 0)
		{
			m_RankInNum = nCntRanking;

			break;
		}
	}

	/*m_nRanking[0] = 5000;
	m_nRanking[1] = 4000;
	m_nRanking[2] = 3000;
	m_nRanking[3] = 2000;
	m_nRanking[4] = 1000;*/

	pFile = fopen(RANKING_TEXT_NAME, "wb");		// �t�@�C�����J��

	if (pFile != NULL)
	{// NULL�ȊO�̏ꍇ
		fwrite(&m_nRanking, sizeof(int), RANKING_MAX_NUM, pFile);		// ��������

		fclose(pFile);							// �t�@�C�������
	}
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
		fread(&m_nRanking, sizeof(int), RANKING_MAX_NUM, pFile);		// �ǂݍ���

		fclose(pFile);							// �t�@�C�������
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CRanking ***
//=============================================================================
CRanking *CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR3 nextInterval, int nNum)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();			// �J���������_�擾

	CRanking *pRanking = NULL;					// �|�C���^

	if (nNum != -1) nNum = 1;					// �\���^�C�v

	if (pRanking == NULL)
	{// NULL�̏ꍇ
		pRanking = new CRanking;				// �������m��
	}

	if (pRanking != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
		{// �J�E���g
			for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
			{// �J�E���g
				if (pRanking->m_apNumber3D[nCntRanking][nCntDigit] == NULL)
				{
					pRanking->m_apNumber3D[nCntRanking][nCntDigit] = new CNumber3D;																	// �������m��
					pRanking->m_apNumber3D[nCntRanking][nCntDigit]->Init(D3DXVECTOR3(
						((pos.x + (((size.x * 1.8f) * (RANKING_MAX_DIGIT - 1)) * 0.5f)) * nNum) - (((size.x * 1.8f) * nCntDigit) * nNum),
						pos.y + cosf(rot.x + (D3DX_PI * 0.0f)) * (nextInterval.z * nCntRanking),
						pos.z + sinf(rot.x + (D3DX_PI * 0.0f)) * (nextInterval.z * nCntRanking)), rot, size, col, CScene3D::SCENE3DTYPE_NORMAL);	// �����l�ݒ�
					pRanking->m_apNumber3D[nCntRanking][nCntDigit]->SetObjType(CScene::OBJTYPE_RANKING);											// �I�u�W�F�N�g�^�C�v�ݒ�

					pRanking->m_pos[nCntRanking] = D3DXVECTOR3(
						(pos.x - ((size.x * 1.8f) * nCntDigit)),
						pos.y + cosf(rot.x + (D3DX_PI * 0.0f)) * (nextInterval.z * nCntRanking),
						pos.z + sinf(rot.x + (D3DX_PI * 0.0f)) * (nextInterval.z * nCntRanking)) - Camerapos;										// �ʒu
				}
			}

			D3DXVECTOR3 pos = pRanking->m_apNumber3D[nCntRanking][RANKING_MAX_DIGIT - 1]->Getpos();													// �ʒu�擾
			pRanking->m_fScene3DDiff = ((size.x * 1.8f) * nNum);																					// ���ʂƃX�R�A�̍�

			if (pRanking->m_apScene3D[nCntRanking] == NULL)
			{
				pRanking->m_apScene3D[nCntRanking] = new CScene3D(CScene3D::PRIORITY_UI);															// �������m��
				pRanking->m_apScene3D[nCntRanking]->SetInitAll(
					D3DXVECTOR3(pos.x - ((size.x * 2.0f) * nNum), pos.y, pos.z), rot,
					D3DXVECTOR3(size.x * 1.5f, size.y, size.z), col, D3DXVECTOR2(1.0f, 0.2f), CScene3D::SCENE3DTYPE_NORMAL);						// �����l�ݒ�
				pRanking->m_apScene3D[nCntRanking]->Init();																							// ����������
				pRanking->m_apScene3D[nCntRanking]->SetObjType(CScene::OBJTYPE_RANKING);															// �I�u�W�F�N�g�^�C�v�ݒ�
				pRanking->m_apScene3D[nCntRanking]->BindTexture(m_pTexture);																		// �e�N�X�`���𔽉f
				pRanking->m_apScene3D[nCntRanking]->SetTex(0, nCntRanking, D3DXVECTOR2(1.0f, 0.2f), D3DXVECTOR2(0.0f, 0.0f));						// �e�N�X�`���ݒ�
			}
		}
		pRanking->Init();						// ����������
	}

	return pRanking;			// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CRanking ***
//=============================================================================
HRESULT CRanking::Init(void)
{
	float fScoreData;				// �X�R�A�i�[�p

	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
	{// �J�E���g
		fScoreData = (float)m_nRanking[nCntRanking];	// �X�R�A���i�[

		for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
		{// �J�E���g
			if (fScoreData < 1.0f && nCntDigit != 0)
			{
				m_apNumber3D[nCntRanking][nCntDigit]->Setcol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			}
			else
			{
				m_apNumber3D[nCntRanking][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}


			m_apNumber3D[nCntRanking][nCntDigit]->SetTex(10, (int)fScoreData % 10, D3DXVECTOR2(0.1f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));					// �e�N�X�`���ݒ�

			fScoreData /= 10;		// ����i�߂�
		}
	}

	if (m_RankInNum < 0 || RANKING_MAX_NUM <= m_RankInNum)
	{
		m_RankInNum = 99;
	}

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CRanking ***
//=============================================================================
void CRanking::Uninit(void)
{
	for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
	{// �J�E���g
		for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
		{// �J�E���g
			if (m_apNumber3D[nCntRanking][nCntDigit] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_apNumber3D[nCntRanking][nCntDigit]->Uninit();			// �I������
				m_apNumber3D[nCntRanking][nCntDigit] = NULL;			// NULL��
			}
		}

		if (m_apScene3D[nCntRanking] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apScene3D[nCntRanking]->Uninit();							// �I������
			m_apScene3D[nCntRanking] = NULL;   							// NULL��
		}
	}

	CScene::Release();
}

//=============================================================================
// �X�V����											(public)	*** CRanking ***
//=============================================================================
void CRanking::Update(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();			// �J���������_�擾
	CManager::MODE mode = CManager::GetMode();							// ���[�h�擾
	bool bTitlePress = CTitle::GetTitlePress();							// �^�C�g���v���X�擾
	D3DXVECTOR3 pos;													// �ʒu
	D3DXVECTOR3 sizeNum, sizeScene;										// �傫��
	bool bDelete = false;												// �j��

	switch (mode)
	{// ���[�h
	case CManager::MODE_TITLE:		// �^�C�g��
		for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
		{// �J�E���g
			for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
			{// �J�E���g
				pos = m_apNumber3D[nCntRanking][nCntDigit]->Getpos();				// �ʒu�擾
				sizeNum = m_apNumber3D[nCntRanking][nCntDigit]->Getsize();			// �傫���擾

				if (bTitlePress == true)
				{// �^�C�g���v���X
					// �傫��������
					sizeNum.x = sizeNum.x * 0.9f;
					sizeNum.y = sizeNum.y * 0.9f;

					if (sizeNum.x < 0.1f || sizeNum.y < 0.1f)
					{// �����Ȃ��Ȃ�����
						bDelete = true;			// �j������
					}
				}
				else
				{
					pos.x += 0.2f;				// �ړ�
				}
				
				m_apNumber3D[nCntRanking][nCntDigit]->Setpos(pos);					// �ʒu��ݒ�
				m_apNumber3D[nCntRanking][nCntDigit]->Setsize(sizeNum);				// �傫����ݒ�
				m_apNumber3D[nCntRanking][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}

			sizeScene = m_apScene3D[nCntRanking]->Getsize();						// �傫���擾

			if (bTitlePress == true)
			{// �^�C�g���v���X
				// �傫��������
				sizeScene.x = sizeScene.x * 0.9f;
				sizeScene.y = sizeScene.y * 0.9f;

				if (sizeScene.x < 0.1f || sizeScene.y < 0.1f)
				{// �����Ȃ��Ȃ�����
					bDelete = true;				// �j������
				}
			}

			m_apScene3D[nCntRanking]->Setpos(D3DXVECTOR3(pos.x - m_fScene3DDiff, pos.y, pos.z));		// �ʒu��ݒ�
			m_apScene3D[nCntRanking]->Setsize(sizeScene);												// �傫����ݒ�
		}

		break;
	case CManager::MODE_GAME:		// �Q�[��
		for (int nCntRanking = 0; nCntRanking < RANKING_MAX_NUM; nCntRanking++)
		{// �J�E���g
			for (int nCntDigit = 0; nCntDigit < RANKING_MAX_DIGIT; nCntDigit++)
			{// �J�E���g
				pos = m_apNumber3D[nCntRanking][nCntDigit]->Getpos();				// �ʒu�擾

				// �J�����ɑ΂��Ă̈ʒu��ݒ�
				//pos.x = Camerapos.x + m_pos[nCntRanking].x;						
				pos.z = Camerapos.z + m_pos[nCntRanking].z;

				if (m_RankInNum != 99 && nCntRanking == m_RankInNum)
				{
					m_apNumber3D[nCntRanking][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				}
				else
				{
					m_apNumber3D[nCntRanking][nCntDigit]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}

				m_apNumber3D[nCntRanking][nCntDigit]->Setpos(pos);					// �ʒu��ݒ�

				m_pos[nCntRanking] = pos - Camerapos;								// ���݈ʒu���v�Z
			}

			m_apScene3D[nCntRanking]->Setpos(D3DXVECTOR3(pos.x - m_fScene3DDiff, pos.y, pos.z));		// �ʒu��ݒ�
		}

		break;
	}

	if (bDelete == true)
	{// �j������
		Uninit();		// �I������

		if (mode == CManager::MODE_TITLE)
		{// ���[�h���^�C�g��
			CTitle::SetTitleRDelete();		// �^�C�g���j��
		}
	}
}

//=============================================================================
// �`�揈��											(public)	*** CRanking ***
//=============================================================================
void CRanking::Draw(void)
{

}