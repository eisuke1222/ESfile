//=============================================================================
//
// �V�[������ [scene.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"		// �V�[��

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
CScene *CScene::m_apScene[PRIORITY_MAX][MAX_SCENE] = {};		// �V�[��
int CScene::m_nNumAll = 0;										// ����
int CScene::m_nStartUp = 0;										// �X�V�͈�

//=============================================================================
// �R���X�g���N�^									(public)	*** CScene ***
//=============================================================================
CScene::CScene(int nPriority)
{
	for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
	{// �V�[���J�E���g
		if (m_apScene[nPriority][nCntScene] == NULL)
		{// NULL�̏ꍇ
			m_apScene[nPriority][nCntScene] = this;				// ��������
			m_nPriority = nPriority;							// ���ʐݒ�
			m_nID = nCntScene;									// ID�ݒ�
			m_objType = OBJTYPE_NONE;							// ���
			m_nNumAll++;										// �����J�E���g

			break;
		}
	}
}

//=============================================================================
// �R���X�g���N�^									(public)	*** CScene ***
//=============================================================================
CScene::CScene(int nPriority, int nID)
{
	for (int nCntScene = nID; nCntScene < MAX_SCENE; nCntScene++)
	{// �V�[���J�E���g
		if (m_apScene[nPriority][nCntScene] == NULL)
		{// NULL�̏ꍇ
			m_apScene[nPriority][nCntScene] = this;				// ��������
			m_nPriority = nPriority;							// ���ʐݒ�
			m_nID = nCntScene;									// ID�ݒ�
			m_objType = OBJTYPE_NONE;							// ���
			m_nNumAll++;										// �����J�E���g

			break;
		}
	}
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CScene ***
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// �S�J������										(public)	*** CScene ***
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// �v���C�I���e�B�J�E���g
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// �V�[���J�E���g
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{// NUll�ȊO�̏ꍇ
				// �I������
				m_apScene[nCntPriority][nCntScene]->Uninit();	// �I������

				delete m_apScene[nCntPriority][nCntScene];		// �������J��
				m_apScene[nCntPriority][nCntScene] = NULL;		// NULL
			}
		}
	}
}

//=============================================================================
// �S�X�V����										(public)	*** CScene ***
//=============================================================================
void CScene::UpadteAll(void)
{
	for (int nCntPriority = m_nStartUp; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// �v���C�I���e�B�J�E���g
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// �V�[���J�E���g
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{// NULL�ȊO�̏ꍇ
				// �|���S���X�V����
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}

//=============================================================================
// �S�`�揈��										(public)	*** CScene ***
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// �v���C�I���e�B�J�E���g
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// �V�[���J�E���g
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{// NULL�ȊO�̏ꍇ
				// �|���S���X�V����
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}

//=============================================================================
// �����擾����										(public)	*** CScene ***
//=============================================================================
int CScene::GetNumAll(void)
{
	return m_nNumAll;		// ������Ԃ�
}

//=============================================================================
// �J������											(protected)	*** CScene ***
//=============================================================================
void CScene::Release(void)
{
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{// NUll�ȊO�̏ꍇ
		int nID = m_nID;					// ID��ۑ��i�f�[�^�̕s��h�~�j
		int nPri = m_nPriority;				// ���ʂ�ۑ��i�f�[�^�̕s��h�~�j

		delete m_apScene[nPri][nID];		// �������J��
		m_apScene[nPri][nID] = NULL;		// NULL��
		m_nNumAll--;						// ���������炷
	}
}