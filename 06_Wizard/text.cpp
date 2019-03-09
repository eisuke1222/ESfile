//=============================================================================
//
// �e�L�X�g���� [text.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "text.h"			// ���f��
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXT_NAME	"data\\TEXT\\SYSTEM\\system.ini"		// �t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int						CText::m_nMaxText = 0;								// �e�L�X�g��
int						CText::m_nMaxType[CText::TYPE_MAX] = { 0 };			// �^�C�v�ʃe�L�X�g��
CText::TEXT				*CText::m_Text = NULL;								// �e�L�X�g

CModel::MODEL_INFO		**CText::m_pModelInfo = NULL;						// ���f��
CMotion::MOTION_INFO	**CText::m_pMotionInfo = NULL;						// ���[�V����

int						CText::m_nMaxModel = NULL;							// ���f����
CText::MODEL_STORAGE	*CText::m_pModelStorage = NULL;						// ���f���ۊ�

//=============================================================================
// �R���X�g���N�^								(public)	*** CText ***
//=============================================================================
CText::CText()
{

}

//=============================================================================
// �f�X�g���N�^									(public)	*** CText ***
//=============================================================================
CText::~CText()
{

}

//=============================================================================
// ���[�h����									(public)	*** CText ***
//=============================================================================
HRESULT CText::Load(void)
{
	FILE *pFile = NULL;					// �t�@�C���|�C���^

	pFile = fopen(TEXT_NAME, "r");		// �t�@�C���I�[�v��

	if (pFile != NULL)
	{// NULL�ȊO�̏ꍇ
		int nCntText = 0;				// �e�L�X�g�J�E���^

		char *pStrCut;					// ������̐擪�ւ̃|�C���^
		char aLine[256];				// ������ǂݎ��p�i�P�s���j
		char aStr[256];					// �����񔲂��o���p

		pStrCut = ReadFile(pFile, &aLine[0]);				// �R�����g�`�F�b�N
		strcpy(aStr, pStrCut);								// �����ϊ�

		if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
		{// �ǂݍ��݊J�n
			do
			{// ���[�v
				pStrCut = ReadFile(pFile, &aLine[0]);					// �R�����g�`�F�b�N
				strcpy(aStr, pStrCut);									// �����ϊ�

				if (memcmp(aStr, "NUM_TEXT", strlen("NUM_TEXT")) == 0)
				{// �e�L�X�g��
					pStrCut += strlen("NUM_TEXT");								// �|�C���^��i�߂�
					PopString(pStrCut, aStr);									// �����ϊ�
					m_nMaxText = atoi(aStr);									// �����^�ɕϊ�

					if (m_Text == NULL)
					{// NULL�̏ꍇ
						m_Text = new TEXT[m_nMaxText];							// �e�L�X�g

						for (int nCount = 0; nCount < m_nMaxText; nCount++)
						{// �e�L�X�g���J�E���g
							// �\���̏�����
							m_Text[nCount].type			= TYPE_MODEL;		// ���
							m_Text[nCount].nTypeNum		= 0;				// ��ޔԍ�
							m_Text[nCount].pFileName[0] = '\0';				// �t�@�C����
							m_Text[nCount].nMax			= 0;				// �ő�l
							m_Text[nCount].bLoad		= false;			// ���[�h������
						}
					}
				}

				if (nCntText < m_nMaxText)
				{// �e�L�X�g���ȉ��̏ꍇ
					if (memcmp(aStr, "MODEL_TEXT", strlen("MODEL_TEXT")) == 0)
					{// ���f��
						pStrCut += strlen("MODEL_TEXT");							// �|�C���^��i�߂�
						PopString(pStrCut, aStr);									// �����ϊ�
						strcpy(m_Text[nCntText].pFileName, aStr);					// �����ϊ�
						m_Text[nCntText].type = TYPE_MODEL;							// �^�C�v�ݒ�
						m_Text[nCntText].nTypeNum = m_nMaxType[TYPE_MODEL];			// �^�C�v�ԍ�
						m_nMaxType[TYPE_MODEL]++;									// �^�C�v�ő吔�J�E���g
						m_Text[nCntText].nIdx = nCntText;							// �e�L�X�g�ԍ�
						nCntText++;													// �C���f�b�N�X�J�E���g
					}
					else if (memcmp(aStr, "MOTION_TEXT", strlen("MOTION_TEXT")) == 0)
					{// ���[�V����
						pStrCut += strlen("MOTION_TEXT");							// �|�C���^��i�߂�
						PopString(pStrCut, aStr);									// �����ϊ�
						strcpy(m_Text[nCntText].pFileName, aStr);					// �����ϊ�
						m_Text[nCntText].type = TYPE_MOTION;						// �^�C�v�ݒ�
						m_Text[nCntText].nTypeNum = m_nMaxType[TYPE_MOTION];		// �^�C�v�ԍ�
						m_nMaxType[TYPE_MOTION]++;									// �^�C�v�ő吔�J�E���g
						m_Text[nCntText].nIdx = nCntText;							// �e�L�X�g�ԍ�
						nCntText++;													// �C���f�b�N�X�J�E���g
					}
					else if (memcmp(aStr, "MESHSKIN_TEXT", strlen("MESHSKIN_TEXT")) == 0)
					{// ���b�V���X�L��
						pStrCut += strlen("MESHSKIN_TEXT");							// �|�C���^��i�߂�
						PopString(pStrCut, aStr);									// �����ϊ�
						strcpy(m_Text[nCntText].pFileName, aStr);					// �����ϊ�
						m_Text[nCntText].type = TYPE_MESHSKIN;						// �^�C�v�ݒ�
						m_Text[nCntText].nTypeNum = m_nMaxType[TYPE_MESHSKIN];		// �^�C�v�ԍ�
						m_nMaxType[TYPE_MESHSKIN]++;								// �^�C�v�ő吔�J�E���g
						m_Text[nCntText].nIdx = nCntText;							// �e�L�X�g�ԍ�
						nCntText++;													// �C���f�b�N�X�J�E���g
					}
					else if (memcmp(aStr, "MESHFIELD_TEXT", strlen("MESHFIELD_TEXT")) == 0)
					{// ���b�V���t�B�[���h
						pStrCut += strlen("MESHFIELD_TEXT");						// �|�C���^��i�߂�
						PopString(pStrCut, aStr);									// �����ϊ�
						strcpy(m_Text[nCntText].pFileName, aStr);					// �����ϊ�
						m_Text[nCntText].type = TYPE_MESHFIELD;						// �^�C�v�ݒ�
						m_Text[nCntText].nTypeNum = m_nMaxType[TYPE_MESHFIELD];		// �^�C�v�ԍ�
						m_Text[nCntText].nIdx = nCntText;							// �e�L�X�g�ԍ�
						m_nMaxType[TYPE_MESHFIELD]++;								// �^�C�v�ő吔�J�E���g
						nCntText++;													// �C���f�b�N�X�J�E���g
					}
				}
			} while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0);
		}
		fclose(pFile);		// �t�@�C���N���[�Y
	}

	for (int nCount = 0; nCount < m_nMaxText; nCount++)
	{// �e�L�X�g���J�E���g
		if (m_Text[nCount].type == TYPE_MODEL)
		{// �^�C�v�����f��
			if (m_pModelInfo == NULL)
			{// NULL�̏ꍇ
				m_pModelInfo = new CModel::MODEL_INFO*[m_nMaxType[TYPE_MODEL]];			// �������m��

				// �\���̏�����
				for (int nCount = 0; nCount < m_nMaxType[TYPE_MODEL]; nCount++)
				{// �^�C�v�ʃe�L�X�g���J�E���g
					m_pModelInfo[nCount] = NULL;	// NULL��
				}
			}
		}

		if (m_Text[nCount].type == TYPE_MOTION)
		{// �^�C�v�����[�V����
			if (m_pMotionInfo == NULL)
			{// NULL�̏ꍇ
				m_pMotionInfo = new CMotion::MOTION_INFO*[m_nMaxType[TYPE_MOTION]];		// �������m��

				// �\���̏�����
				for (int nCount = 0; nCount < m_nMaxType[TYPE_MOTION]; nCount++)
				{// �^�C�v�ʃe�L�X�g���J�E���g
					m_pMotionInfo[nCount] = NULL;		// NULL��
				}
			}
		}
	}

	// ���f�����擾
	m_nMaxModel = GetMaxModel();

	if (m_pModelStorage == NULL)
	{// NULL�̏ꍇ
		m_pModelStorage = new MODEL_STORAGE[m_nMaxModel];		// �������m��

		for (int nCount = 0; nCount < m_nMaxModel; nCount++)
		{// ���f�����J�E���g
			m_pModelStorage[nCount].pFileName[0]	= '\0';		// �t�@�C����
			m_pModelStorage[nCount].pTexture		= NULL;		// �e�N�X�`�����ւ̃|�C���^
			m_pModelStorage[nCount].pBuffMat		= NULL;		// �}�e���A�����ւ̃|�C���^
			m_pModelStorage[nCount].pMesh			= NULL;		// ���b�V�����ւ̃|�C���^
			m_pModelStorage[nCount].nNumMat			= 0;		// �}�e���A�����̐�
			m_pModelStorage[nCount].bLoad			= false;	// ���[�h������
		}
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����									(public)	*** CText ***
//=============================================================================
void CText::Unload(void)
{
	// ���f��
	if (m_pModelInfo != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCount = 0; nCount < m_nMaxText; nCount++)
		{// �e�L�X�g���J�E���g
			if (m_Text[nCount].type == TYPE_MODEL && m_Text[nCount].bLoad == true)
			{// �^�C�v�����f�� && ���[�h���Ă���
				delete[] m_pModelInfo[m_Text[nCount].nTypeNum];		// �������J��
				m_pModelInfo[m_Text[nCount].nTypeNum] = NULL;		// NULL��
			}
		}
		delete[] m_pModelInfo;		// �������J��
		m_pModelInfo = NULL;		// NULL��
	}

	// ���[�V����
	if (m_pMotionInfo != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCount = 0; nCount < m_nMaxText; nCount++)
		{// �e�L�X�g���J�E���g
			if (m_Text[nCount].type == TYPE_MOTION && m_Text[nCount].bLoad == true)
			{// �^�C�v�����[�V���� && ���[�h���Ă���
				for (int nCntAnim = 0; nCntAnim < m_Text[nCount].nMax; nCntAnim++)
				{// ���[�V�������J�E���g
					for (int nCntKey = 0; nCntKey < m_pMotionInfo[m_Text[nCount].nTypeNum][nCntAnim].nKeyMax; nCntKey++)
					{// �L�[���J�E���g
						delete[] m_pMotionInfo[m_Text[nCount].nTypeNum][nCntAnim].aKeyInfo[nCntKey].aKey;	// �������J��
						m_pMotionInfo[m_Text[nCount].nTypeNum][nCntAnim].aKeyInfo[nCntKey].aKey = NULL;		// NULL��
					}
					delete[] m_pMotionInfo[m_Text[nCount].nTypeNum][nCntAnim].aKeyInfo;		// �������J��
					m_pMotionInfo[m_Text[nCount].nTypeNum][nCntAnim].aKeyInfo = NULL;		// NULL��
				}
				delete[] m_pMotionInfo[m_Text[nCount].nTypeNum];	// �������J��
				m_pMotionInfo[m_Text[nCount].nTypeNum] = NULL;		// NULL��
			}
		}
		delete[] m_pMotionInfo;		// �������J��
		m_pMotionInfo = NULL;		// NULL��
	}

	// �e�L�X�g
	if (m_Text != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_Text;		// �������J��
		m_Text = NULL;			// NULL��
	}

	// ���f���ۊ�
	if (m_pModelStorage != NULL)
	{// NULL�ȊO�̏ꍇ
		ReleaseXModelData();
		delete[] m_pModelStorage;
		m_pModelStorage = NULL;
	}
}

//=============================================================================
// �t�@�C���ǂݍ��ݏ���								(public)	*** CText ***
//=============================================================================
char *CText::ReadFile(FILE *pFile, char *pDest)
{
	fgets(pDest, 256, pFile);	// �P�s�ǂݍ���

	if (memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0)
	{// �i���j��������
		ReadFile(pFile, pDest);		// �ǂݍ���
	}
	else
	{// ���̑�
		do
		{// ���[�v
			if (memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0)
			{// �i���A���s�j��������
				ReadFile(pFile, pDest);
			}
			else if (memcmp(pDest, " ", strlen(" ")) == 0)
			{// �i���p�X�y�[�X�j��������
				wsprintf(&pDest[0], &pDest[strlen(" ")]);
			}
			else if (memcmp(pDest, "\t", strlen("\t")) == 0)
			{// �itab�j��������
				wsprintf(&pDest[0], &pDest[strlen("\t")]);
			}
			else if (memcmp(pDest, "�@", strlen("�@")) == 0)
			{// �i�S�p�X�y�[�X�j��������
				wsprintf(&pDest[0], &pDest[strlen("�@")]);
			}
		} while (
			memcmp(pDest, "#", strlen("#")) == 0 || memcmp(pDest, "\n", strlen("\n")) == 0 ||
			memcmp(pDest, " ", strlen(" ")) == 0 || memcmp(pDest, "\t", strlen("\t")) == 0 ||
			memcmp(pDest, "�@", strlen("�@")) == 0);
		// ����ȊO��������ʂ���
	}

	return pDest;	// �l��Ԃ�
}

//=============================================================================
// �ǂݍ��ݒ�������									(public)	*** CText ***
//=============================================================================
void CText::PopString(char *pStrCur, char *pStr)
{
	do
	{// ���[�v
		if (memcmp(pStrCur, "=", strlen("=")) == 0)
		{// �i���j��������
			wsprintf(&pStrCur[0], &pStrCur[strlen("=")]);
		}
		else if (memcmp(pStrCur, " ", strlen(" ")) == 0)
		{// �i���p�X�y�[�X�j��������
			wsprintf(&pStrCur[0], &pStrCur[strlen(" ")]);
		}
		else if (memcmp(pStrCur, "\t", strlen("\t")) == 0)
		{// �itab�j��������
			wsprintf(&pStrCur[0], &pStrCur[strlen("\t")]);
		}
		else if (memcmp(pStrCur, "�@", strlen("�@")) == 0)
		{// �i�S�p�X�y�[�X�j��������
			wsprintf(&pStrCur[0], &pStrCur[strlen("�@")]);
		}
	} while (
		memcmp(pStrCur, "=", strlen("=")) == 0 ||
		memcmp(pStrCur, " ", strlen(" ")) == 0 || memcmp(pStrCur, "\t", strlen("\t")) == 0 ||
		memcmp(pStrCur, "�@", strlen("�@")) == 0);
	// ����ȊO��������ʂ���

	int nCntEnd = 0;			// �ǂݍ��ݏI�������ԍ�
	strcpy(pStr, pStrCur);		// �㏑��

	do
	{// ���[�v
		if (memcmp(pStr, "#", strlen("#")) == 0)
		{// �i���j��������
			wsprintf(&pStr[0], &pStr[strlen("#")]);
		}
		else if (memcmp(pStr, "\n", strlen("\n")) == 0)
		{// �i���s�j��������
			wsprintf(&pStr[0], &pStr[strlen("\n")]);
		}
		else if (memcmp(pStr, " ", strlen(" ")) == 0)
		{// �i���p�X�y�[�X�j��������
			wsprintf(&pStr[0], &pStr[strlen(" ")]);
		}
		else if (memcmp(pStr, "\t", strlen("\t")) == 0)
		{// �itab�j��������
			wsprintf(&pStr[0], &pStr[strlen("\t")]);
		}
		else if (memcmp(pStr, "�@", strlen("�@")) == 0)
		{// �i�S�p�X�y�[�X�j��������
			wsprintf(&pStr[0], &pStr[strlen("�@")]);
		}
		else
		{// ����ȊO��������
			wsprintf(&pStr[0], &pStr[1]);
		}

		nCntEnd++;		// �J�E���g
	} while (
		memcmp(pStr, "#", strlen("#")) != 0 && memcmp(pStr, "\n", strlen("\n")) != 0 &&
		memcmp(pStr, " ", strlen(" ")) != 0 && memcmp(pStr, "\t", strlen("\t")) != 0 &&
		memcmp(pStr, "�@", strlen("�@")) != 0);
	// ����ȊO��������ʂ���

	strcpy(pStr, pStrCur);						// �㏑��
	wsprintf(&pStrCur[0], &pStrCur[nCntEnd]);	// �㏑��
	pStr[nCntEnd] = '\0';						// null����
}

//=============================================================================
// ���f���擾����										(public)	*** CText ***
//=============================================================================
CModel::MODEL_INFO	*CText::GetModel(char *cName, int *nMax)
{
	if (m_pModelInfo != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCount = 0; nCount < m_nMaxText; nCount++)
		{// �e�L�X�g���J�E���g
			if (m_Text[nCount].type == TYPE_MODEL && strcmp(m_Text[nCount].pFileName, cName) == 0)
			{// �^�C�v�����f�� && �^�C�v�ԍ��ƈ�v
				if (m_Text[nCount].bLoad == false)
				{// ���[�h���Ă��Ȃ�
					LoadModelText(&m_Text[nCount].pFileName[0], &m_Text[nCount].nMax, m_Text[nCount].nTypeNum);	// ���f�����[�h
					m_Text[nCount].bLoad = true;	// ���[�h����
				}
				*nMax = m_Text[nCount].nMax;		// �ő�l��ݒ�
			
				return m_pModelInfo[m_Text[nCount].nTypeNum];		// �l��Ԃ�

				break;
			}
		}
	}

	MessageBox(0, "CText::GetModel() nNum Error", "�x��", MB_OK);
	return m_pModelInfo[m_nMaxType[TYPE_MODEL]];	// �l��Ԃ�
}

//=============================================================================
// ���[�V�����擾����									(public)	*** CText ***
//=============================================================================
CMotion::MOTION_INFO	*CText::GetMotion(char *cName)
{
	if (m_pMotionInfo != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCount = 0; nCount < m_nMaxText; nCount++)
		{// �e�L�X�g���J�E���g
			if (m_Text[nCount].type == TYPE_MOTION && strcmp(m_Text[nCount].pFileName, cName) == 0)
			{// �^�C�v�����[�V���� && �^�C�v�ԍ��ƈ�v
				if (m_Text[nCount].bLoad == false)
				{// ���[�h���Ă��Ȃ�
					LoadMotionText(&m_Text[nCount].pFileName[0], &m_Text[nCount].nMax, m_Text[nCount].nTypeNum);		// ���[�V�������[�h
					m_Text[nCount].bLoad = true;	// ���[�h����
				}
		
				return m_pMotionInfo[m_Text[nCount].nTypeNum];		// �l��Ԃ�

				break;
			}
		}
	}

	MessageBox(0, "CText::GetMotion() nNum Error", "�x��", MB_OK);
	return m_pMotionInfo[m_nMaxType[TYPE_MOTION]];		// �l��Ԃ�
}

//=============================================================================
// ���f���擾����										(public)	*** CText ***
//=============================================================================
void CText::GetXModelData(char *cName, LPDIRECT3DTEXTURE9 **pTexture, LPD3DXMESH *pMesh, LPD3DXBUFFER *pBuffMat, DWORD *nNumMat)
{
	if (m_pModelStorage != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCount = 0; nCount < m_nMaxModel; nCount++)
		{// �e�L�X�g���J�E���g
			if (m_pModelStorage[nCount].bLoad == false)
			{// ���[�h���Ă��Ȃ�
				strcpy(m_pModelStorage[nCount].pFileName, cName);		// ���f�����ۑ�
				LoadXModelData(cName, nCount);							// ���f���ǂݍ���

				*pTexture	= m_pModelStorage[nCount].pTexture;			// �e�N�X�`�����
				*pMesh		= m_pModelStorage[nCount].pMesh;			// ���b�V�����
				*pBuffMat	= m_pModelStorage[nCount].pBuffMat;			// �}�e���A�����
				*nNumMat	= m_pModelStorage[nCount].nNumMat;			// �}�e���A����

				break;
			}
			else if (m_pModelStorage[nCount].bLoad == true && strcmp(m_pModelStorage[nCount].pFileName, cName) == 0)
			{// ���[�h���Ă�
				*pTexture	= m_pModelStorage[nCount].pTexture;			// �e�N�X�`�����
				*pMesh		= m_pModelStorage[nCount].pMesh;			// ���b�V�����
				*pBuffMat	= m_pModelStorage[nCount].pBuffMat;			// �}�e���A�����
				*nNumMat	= m_pModelStorage[nCount].nNumMat;			// �}�e���A����

				break;
			}
		}
	}
}

//=============================================================================
// ���f���f�[�^�j������								(public)	*** CText ***
//=============================================================================
void CText::ReleaseXModelData(void)
{
	if (m_pModelStorage != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCount = 0; nCount < m_nMaxModel; nCount++)
		{// �e�L�X�g���J�E���g
			if (m_pModelStorage[nCount].bLoad == true)
			{// ���[�h���Ă�
				m_pModelStorage[nCount].pFileName[0] = '\0';		// ������

				if (m_pModelStorage[nCount].pTexture != NULL)
				{// NULL�ȊO�̏ꍇ
					for (int nCntMat = 0; nCntMat < (int)m_pModelStorage[nCount].nNumMat; nCntMat++)
					{// �}�e���A�����J�E���g
						if (m_pModelStorage[nCount].pTexture[nCntMat] != NULL)
						{// NULL�ȊO�̏ꍇ
							m_pModelStorage[nCount].pTexture[nCntMat]->Release();	// �J��
							m_pModelStorage[nCount].pTexture[nCntMat] = NULL;		// NULL��
						}
					}

					delete m_pModelStorage[nCount].pTexture;		// �j��
					m_pModelStorage[nCount].pTexture = NULL;		// NULL��
				}

				if (m_pModelStorage[nCount].pBuffMat != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pModelStorage[nCount].pBuffMat->Release();	// �J��
					m_pModelStorage[nCount].pBuffMat = NULL;		// NULL��
				}

				if (m_pModelStorage[nCount].pMesh != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pModelStorage[nCount].pMesh->Release();		// �J��
					m_pModelStorage[nCount].pMesh = NULL;			// NULL��
				}

				m_pModelStorage[nCount].nNumMat = 0;				// ������
				m_pModelStorage[nCount].bLoad = false;				// ������
			}
		}
	}
}

//=============================================================================
// ���f�����[�h����									(public)	*** CText ***
//=============================================================================
void CText::LoadModelText(char *cName, int *nMax, int nNum)
{
	int nModelMax = 0;					// ���f����

	FILE *pFile = NULL;					// �t�@�C���|�C���^

	pFile = fopen(cName, "r");			// �t�@�C���I�[�v��

	if (pFile != NULL)
	{// NULL�ȊO�̏ꍇ
		int nCntModel = 0;				// ���f���J�E���^

		char *pStrCut;					// ������̐擪�ւ̃|�C���^
		char aLine[256];				// ������ǂݎ��p�i�P�s���j
		char aStr[256];					// �����񔲂��o���p

		pStrCut = ReadFile(pFile, &aLine[0]);				// �R�����g�`�F�b�N
		strcpy(aStr, pStrCut);								// �����ϊ�

		if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
		{// �ǂݍ��݊J�n
			do
			{// ���[�v
				pStrCut = ReadFile(pFile, &aLine[0]);			// �R�����g�`�F�b�N
				strcpy(aStr, pStrCut);							// �����ϊ�

				if (memcmp(aStr, "NUM_MODEL", strlen("NUM_MODEL")) == 0)
				{// ���f����
					pStrCut += strlen("NUM_MODEL");						// �|�C���^��i�߂�
					PopString(pStrCut, aStr);							// �����ϊ�
					nModelMax = atoi(aStr);								// �����^�ɕϊ�

					if (m_pModelInfo[nNum] == NULL)
					{// NULL�̏ꍇ
						m_pModelInfo[nNum] = new CModel::MODEL_INFO[nModelMax];	// �������m��

						for (int nCount = 0; nCount < nModelMax; nCount++)
						{
							m_pModelInfo[nNum][nCount].cFileName[0] = '\0';
							m_pModelInfo[nNum][nCount].nIndex = -1;
							m_pModelInfo[nNum][nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							m_pModelInfo[nNum][nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						}
					}
				}
				else if (memcmp(aStr, "MODEL_FILENAME", strlen("MODEL_FILENAME")) == 0)
				{// ���f���t�@�C����
					pStrCut += strlen("MODEL_FILENAME");						// �|�C���^��i�߂�
					PopString(pStrCut, aStr);									// �����ϊ�
					strcpy(m_pModelInfo[nNum][nCntModel].cFileName, aStr);		// �t�@�C������ۑ�

					nCntModel++;		// ���f���J�E���g
				}
				else if (memcmp(aStr, "CHARACTERSET", strlen("CHARACTERSET")) == 0)
				{// �L�����N�^�[�ݒ�
					nCntModel = 0;		// ���f���J�E���^��������

					do
					{// ���[�v
						pStrCut = ReadFile(pFile, &aLine[0]);						// �R�����g�`�F�b�N
						strcpy(aStr, pStrCut);										// �����ϊ�

						if (memcmp(aStr, "PARTSSET", strlen("PARTSSET")) == 0)
						{// �p�[�c�ݒ�
							do
							{// ���[�v
								pStrCut = ReadFile(pFile, &aLine[0]);					// �R�����g�`�F�b�N
								strcpy(aStr, pStrCut);									// �����ϊ�

								if (memcmp(aStr, "INDEX", strlen("INDEX")) == 0)
								{// ���f���ԍ�
									pStrCut += strlen("INDEX");									// �|�C���^��i�߂�
									PopString(pStrCut, aStr);									// �����ϊ�
									m_pModelInfo[nNum][nCntModel].nIndex = atoi(aStr);			// �����^�ɕϊ�
								}
								else if (memcmp(aStr, "PARENT", strlen("PARENT")) == 0)
								{// �e���f���ԍ�
									pStrCut += strlen("PARENT");								// �|�C���^��i�߂�
									PopString(pStrCut, aStr);									// �����ϊ�
									m_pModelInfo[nNum][nCntModel].nIndex = atoi(aStr);			// �����^�ɕϊ�
								}
								else if (memcmp(aStr, "POS", strlen("POS")) == 0)
								{// �ʒu
									pStrCut += strlen("POS");									// �|�C���^��i�߂�
									PopString(pStrCut, aStr);									// �����ϊ�
									m_pModelInfo[nNum][nCntModel].pos.x = (float)atof(aStr);	// �����^�ɕϊ�
									PopString(pStrCut, aStr);									// �����ϊ�
									m_pModelInfo[nNum][nCntModel].pos.y = (float)atof(aStr);	// �����^�ɕϊ�
									PopString(pStrCut, aStr);									// �����ϊ�
									m_pModelInfo[nNum][nCntModel].pos.z = (float)atof(aStr);	// �����^�ɕϊ�
								}
								else if (memcmp(aStr, "ROT", strlen("ROT")) == 0)
								{// ����
									pStrCut += strlen("ROT");									// �|�C���^��i�߂�
									PopString(pStrCut, aStr);									// �����ϊ�
									m_pModelInfo[nNum][nCntModel].rot.x = (float)atof(aStr);	// �����^�ɕϊ�
									PopString(pStrCut, aStr);									// �����ϊ�
									m_pModelInfo[nNum][nCntModel].rot.y = (float)atof(aStr);	// �����^�ɕϊ�
									PopString(pStrCut, aStr);									// �����ϊ�
									m_pModelInfo[nNum][nCntModel].rot.z = (float)atof(aStr);	// �����^�ɕϊ�
								}
							} while (memcmp(aStr, "END_PARTSSET", strlen("END_PARTSSET")) != 0);
							nCntModel++;	// ���f���J�E���g
						}
					} while (memcmp(aStr, "END_CHARACTERSET", strlen("END_CHARACTERSET")) != 0);
				}
			} while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0);
		}
		fclose(pFile);					// �t�@�C���N���[�Y
	}

	*nMax = nModelMax;		// ���f������ݒ�
}

//=============================================================================
// ���[�V�������[�h����								(public)	*** CText ***
//=============================================================================
void CText::LoadMotionText(char *cName, int *nMax, int nNum)
{
	int nModelMax = 0;					// ���f����
	int nMotionMax = 0;					// ���[�V������

	FILE *pFile = NULL;					// �t�@�C���|�C���^

	pFile = fopen(cName, "r");			// �t�@�C���I�[�v��

	if (pFile != NULL)
	{// NULL�ȊO�̏ꍇ
		int nCntModel = 0;				// ���f���J�E���^
		int nCntMotion = 0;				// ���[�V�����J�E���^
		int nCntKey = 0;				// �L�[�J�E���^

		char *pStrCut;					// ������̐擪�ւ̃|�C���^
		char aLine[256];				// ������ǂݎ��p�i�P�s���j
		char aStr[256];					// �����񔲂��o���p

		pStrCut = ReadFile(pFile, &aLine[0]);				// �R�����g�`�F�b�N
		strcpy(aStr, pStrCut);								// �����ϊ�

		if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
		{// �ǂݍ��݊J�n
			do
			{// ���[�v
				pStrCut = ReadFile(pFile, &aLine[0]);			// �R�����g�`�F�b�N
				strcpy(aStr, pStrCut);							// �����ϊ�

				if (memcmp(aStr, "NUM_MODEL", strlen("NUM_MODEL")) == 0)
				{// ���f����
					pStrCut += strlen("NUM_MODEL");						// �|�C���^��i�߂�
					PopString(pStrCut, aStr);							// �����ϊ�
					nModelMax = atoi(aStr);								// �����^�ɕϊ�					
				}
				else if (memcmp(aStr, "NUM_MOTION", strlen("NUM_MOTION")) == 0)
				{// ���[�V������
					pStrCut += strlen("NUM_MOTION");					// �|�C���^��i�߂�
					PopString(pStrCut, aStr);							// �����ϊ�
					nMotionMax = atoi(aStr);							// �����^�ɕϊ�
					
					if (m_pMotionInfo[nNum] == NULL)
					{// NULL�̏ꍇ
						m_pMotionInfo[nNum] = new CMotion::MOTION_INFO[nMotionMax];	// �������m��

						for (int nCount = 0; nCount < nMotionMax; nCount++)
						{// �A�j���[�V�����J�E���g
							m_pMotionInfo[nNum][nCount].aKeyInfo = NULL;			// NULL��
						}
					}
				}
				else if (memcmp(aStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
				{// ���[�V�����ݒ�
					nCntKey = 0;	// �L�[�J�E���^������

					do
					{// ���[�v
						pStrCut = ReadFile(pFile, &aLine[0]);						// �R�����g�`�F�b�N
						strcpy(aStr, pStrCut);										// �����ϊ�

						if (memcmp(aStr, "LOOP", strlen("LOOP")) == 0)
						{// ���[�v���邩
							pStrCut += strlen("LOOP");										// �|�C���^��i�߂�
							PopString(pStrCut, aStr);										// �����ϊ�
							m_pMotionInfo[nNum][nCntMotion].bLoop = atoi(aStr);				// �����^�ɕϊ�
						}
						else if (memcmp(aStr, "NUM_KEY", strlen("NUM_KEY")) == 0)
						{// �L�[��
							pStrCut += strlen("NUM_KEY");									// �|�C���^��i�߂�
							PopString(pStrCut, aStr);										// �����ϊ�
							m_pMotionInfo[nNum][nCntMotion].nKeyMax = atoi(aStr);			// �����^�ɕϊ�

							if (m_pMotionInfo[nNum][nCntMotion].aKeyInfo == NULL)
							{// NULL�̏ꍇ
								m_pMotionInfo[nNum][nCntMotion].aKeyInfo = new CMotion::KEY_INFO[m_pMotionInfo[nNum][nCntMotion].nKeyMax];	// �������m��

								for (int nCount = 0; nCount < m_pMotionInfo[nNum][nCntMotion].nKeyMax; nCount++)
								{// �L�[�J�E���g
									m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCount].aKey = NULL;		// NULL��
								}
							}
						}
						else if (memcmp(aStr, "BLEND_FRAME", strlen("BLEND_FRAME")) == 0)
						{// �u�����h�t���[��
							pStrCut += strlen("BLEND_FRAME");								// �|�C���^��i�߂�
							PopString(pStrCut, aStr);										// �����ϊ�
							m_pMotionInfo[nNum][nCntMotion].nBrendFrame = atoi(aStr);		// �����^�ɕϊ�
						}
						else if (memcmp(aStr, "KEYSET", strlen("KEYSET")) == 0)
						{// �L�[�ݒ�
							nCntModel = 0;		// ���f���J�E���^������

							if (nCntKey < m_pMotionInfo[nNum][nCntMotion].nKeyMax)
							{// �L�[�ő吔�܂�
								if (m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey == NULL)
								{// NULL�̏ꍇ
									m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey = new CMotion::KEY[nModelMax];		// �������m��
								}
							}

							do
							{// ���[�v
								pStrCut = ReadFile(pFile, &aLine[0]);				// �R�����g�`�F�b�N
								strcpy(aStr, pStrCut);								// �����ϊ�

								if (memcmp(aStr, "FRAME", strlen("FRAME")) == 0)
								{// �t���[��
									pStrCut += strlen("FRAME");												// �|�C���^��i�߂�
									PopString(pStrCut, aStr);												// �����ϊ�
									m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].nFrame = atoi(aStr);	// �����^�ɕϊ�
								}
								else if (memcmp(aStr, "KEY", strlen("KEY")) == 0)
								{// �L�[���f���ݒ�
									do
									{// ���[�v
										pStrCut = ReadFile(pFile, &aLine[0]);		// �R�����g�`�F�b�N
										strcpy(aStr, pStrCut);						// �����ϊ�

										if (memcmp(aStr, "POS", strlen("POS")) == 0)
										{// �ʒu
											pStrCut += strlen("POS");																		// �|�C���^��i�߂�
											PopString(pStrCut, aStr);																		// �����ϊ�
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x = (float)atof(aStr);	// �����^�ɕϊ�
											PopString(pStrCut, aStr);																		// �����ϊ�
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y = (float)atof(aStr);	// �����^�ɕϊ�
											PopString(pStrCut, aStr);																		// �����ϊ�
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z = (float)atof(aStr);	// �����^�ɕϊ�
										}
										else if (memcmp(aStr, "ROT", strlen("ROT")) == 0)
										{// ����
											pStrCut += strlen("ROT");																		// �|�C���^��i�߂�
											PopString(pStrCut, aStr);																		// �����ϊ�
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x = (float)atof(aStr);	// �����^�ɕϊ�
											PopString(pStrCut, aStr);																		// �����ϊ�
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y = (float)atof(aStr);	// �����^�ɕϊ�
											PopString(pStrCut, aStr);																		// �����ϊ�
											m_pMotionInfo[nNum][nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z = (float)atof(aStr);	// �����^�ɕϊ�
										}
									} while (memcmp(aStr, "END_KEY", strlen("END_KEY")) != 0);
									nCntModel++;	// ���f���J�E���^
								}
							} while (memcmp(aStr, "END_KEYSET", strlen("END_KEYSET")) != 0);
							nCntKey++;		// �L�[�J�E���^
						}
					} while (memcmp(aStr, "END_MOTIONSET", strlen("END_MOTIONSET")) != 0);
					nCntMotion++;		// ���[�V�����J�E���^
				}
			} while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0);
		}
		fclose(pFile);					// �t�@�C���N���[�Y
	}

	*nMax = nMotionMax;		// ���[�V��������ݒ�
}

//=============================================================================
// ���f��������										(public)	*** CText ***
//=============================================================================
int CText::GetMaxModel(void)
{
	int nMaxModel = 0;

	if (m_pModelInfo != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCount = 0; nCount < m_nMaxText; nCount++)
		{// �e�L�X�g���J�E���g
			if (m_Text[nCount].type == TYPE_MODEL)
			{// �^�C�v�����f��
				FILE *pFile = NULL;					// �t�@�C���|�C���^

				pFile = fopen(m_Text[nCount].pFileName, "r");	// �t�@�C���I�[�v��

				if (pFile != NULL)
				{// NULL�ȊO�̏ꍇ
					int nCntModel = 0;				// ���f���J�E���^

					char *pStrCut;					// ������̐擪�ւ̃|�C���^
					char aLine[256];				// ������ǂݎ��p�i�P�s���j
					char aStr[256];					// �����񔲂��o���p

					pStrCut = ReadFile(pFile, &aLine[0]);				// �R�����g�`�F�b�N
					strcpy(aStr, pStrCut);								// �����ϊ�

					if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
					{// �ǂݍ��݊J�n
						do
						{// ���[�v
							pStrCut = ReadFile(pFile, &aLine[0]);			// �R�����g�`�F�b�N
							strcpy(aStr, pStrCut);							// �����ϊ�

							if (memcmp(aStr, "NUM_MODEL", strlen("NUM_MODEL")) == 0)
							{// ���f����
								pStrCut += strlen("NUM_MODEL");						// �|�C���^��i�߂�
								PopString(pStrCut, aStr);							// �����ϊ�
								nMaxModel += atoi(aStr);							// �����^�ɕϊ�

								break;
							}
						} while (memcmp(aStr, "END_SCRIPT", strlen("END_SCRIPT")) != 0);
					}
				}
			}
		}
	}

	return nMaxModel;
}

//=============================================================================
// ���f���f�[�^���[�h����								(public)	*** CText ***
//=============================================================================
void CText::LoadXModelData(char *cName, int nNum)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	if (m_pModelStorage != NULL)
	{// NULL�ȊO�̏ꍇ
		if (m_pModelStorage[nNum].bLoad == false)
		{
			D3DXLoadMeshFromX(cName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pModelStorage[nNum].pBuffMat, NULL, &m_pModelStorage[nNum].nNumMat, &m_pModelStorage[nNum].pMesh);

			if (m_pModelStorage[nNum].pBuffMat != NULL)
			{// NULL�ȊO�̏ꍇ
				D3DXMATERIAL *pmat;								// �}�e���A���f�[�^�ւ̃|�C���^
				D3DMATERIAL9 *matDef;							// ���݂̃}�e���A���ۑ��p
				pmat = (D3DXMATERIAL*)m_pModelStorage[nNum].pBuffMat->GetBufferPointer();			// �}�e���A�����擾
				matDef = new D3DMATERIAL9[m_pModelStorage[nNum].nNumMat];			// �}�e���A�����̃������m��
				m_pModelStorage[nNum].pTexture = new LPDIRECT3DTEXTURE9[m_pModelStorage[nNum].nNumMat];	// �}�e���A�����̃������m��

				for (DWORD tex = 0; tex < m_pModelStorage[nNum].nNumMat; tex++)
				{// �}�e���A�����J�E���g
					matDef[tex] = pmat[tex].MatD3D;
					matDef[tex].Ambient = matDef[tex].Diffuse;
					m_pModelStorage[nNum].pTexture[tex] = NULL;

					if (pmat[tex].pTextureFilename != NULL && lstrlen(pmat[tex].pTextureFilename) > 0)
					{// �e�N�X�`�����g�p���Ă���
						if (FAILED(D3DXCreateTextureFromFile(pDevice, pmat[tex].pTextureFilename, &m_pModelStorage[nNum].pTexture[tex])))
							MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
					}
				}

				delete[] matDef;	// �������J��
				matDef = NULL;		// NULL��
			}

			m_pModelStorage[nNum].bLoad = true;
		}
	}
}