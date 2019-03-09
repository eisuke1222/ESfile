//=============================================================================
//
// �G(���l)���� [player.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "enemy.h"			// �v���C���[
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "sound.h"			// �T�E���h
#include "game.h"			// �Q�[��
#include "input.h"			// ����
#include "model.h"			// ���f��
#include "motion.h"			// ���[�V����
#include "camera.h"			// �J����
#include "meshField.h"		// ���b�V���t�B�[���h
#include "block.h"			// �u���b�N
#include "item.h"			// �A�C�e��
#include "effect.h"			// �G�t�F�N�g
#include "shadow.h"			// �e

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ANIM_NAME		"data\\TEXT\\giantmodeldata.txt"		// ���[�V�����f�[�^
#define MOVE_SPEED		(0.95f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int						CEnemy::m_MaxModel = 0;				// ���f����
int						CEnemy::m_MaxAnim = 0;				// ���[�V������

LPDIRECT3DTEXTURE9		**CEnemy::m_pTexture = NULL;		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				*CEnemy::m_pMesh = NULL;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			*CEnemy::m_pBuffMat = NULL;			// �}�e���A�����ւ̃|�C���^
DWORD					*CEnemy::m_nNumMat = NULL;			// �}�e���A�����̐�

CEnemy::MODEL_INFO		*CEnemy::m_ModelInfo = NULL;		// ���f��
CMotion::ANIM_INFO		*CEnemy::m_AnimInfo = NULL;			// ���[�V����

//=============================================================================
// �R���X�g���N�^									(public)	*** CPlayer ***
//=============================================================================
CEnemy::CEnemy() : CScene(CScene::PRIORITY_3, CScene::OBJTYPE_ENEMY)
{
	m_bStart = true;										// �X�^�[�g
	m_nCntStart = 0;										// �X�^�[�g�J�E���g
		
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �傫���i�ő�j
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �傫���i�ŏ��j
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �p�x
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ړ���
	m_frotDest = 0.0f;										// �ڕW�p�x
	m_bJump = false;										// �W�����v
	m_bSlip = false;										// ����
	m_fDownSpeed = 0.2f;									// �������x
	m_nWallJump = 5;										// �ǃL�b�N�J�E���g
	m_bWallJump = false;									// �ǃL�b�N
	m_bWallJumpmove = 0.0f;									// �ǃL�b�N�ړ���
	m_bmoveUp = false;										// ����
	m_nMoveCounter = 0;										// �ړ��ʃJ�E���g

	m_shadowHeight = 0.0f;									// �e����
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CPlayer ***
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ���[�h����							(public)	*** CBullet ***
//=============================================================================
HRESULT CEnemy::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	FILE *pFile = NULL;					// �t�@�C���|�C���^

	pFile = fopen(ANIM_NAME, "r");		// �t�@�C���I�[�v��

	if (pFile != NULL)
	{// NULL�ȊO�̏ꍇ
		int nCntModel = 0;	// ���f���J�E���g
		int nCntMotion = 0;	// ���[�V�����J�E���g
		int nCntKey = 0;	// �L�[�J�E���g

		char *pStrCut;		// ������̐擪�ւ̃|�C���^
		char aLine[256];	// ������ǂݎ��p�i�P�s���j
		char aStr[256];		// �����񔲂��o���p

		pStrCut = ReadFile(pFile, &aLine[0]);		// �R�����g�`�F�b�N
		strcpy(aStr, pStrCut);						// �����ϊ�

		if (memcmp(aStr, "SCRIPT", strlen("SCRIPT")) == 0)
		{
			do
			{// ���[�v
				pStrCut = ReadFile(pFile, &aLine[0]);			// �R�����g�`�F�b�N
				strcpy(aStr, pStrCut);							// �����ϊ�

				if (memcmp(aStr, "NUM_MODEL", strlen("NUM_MODEL")) == 0)
				{// ���f����
					pStrCut += strlen("NUM_MODEL");				// �|�C���^��i�߂�
					PopString(pStrCut, aStr);					// �����ϊ�
					m_MaxModel = atoi(aStr);					// �����^�ɕϊ�

					if (m_ModelInfo == NULL)
					{// NULL�̏ꍇ
						m_ModelInfo = new MODEL_INFO[m_MaxModel];	// �������m��
					}
				}
				else if (memcmp(aStr, "MODEL_FILENAME", strlen("MODEL_FILENAME")) == 0)
				{// ���f���t�@�C����
					pStrCut += strlen("MODEL_FILENAME");						// �|�C���^��i�߂�
					PopString(pStrCut, aStr);									// �����ϊ�
					strcpy(m_ModelInfo[nCntModel].cFileName, aStr);				// �t�@�C������ۑ�

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
						{
							do
							{
								pStrCut = ReadFile(pFile, &aLine[0]);					// �R�����g�`�F�b�N
								strcpy(aStr, pStrCut);									// �����ϊ�

								if (memcmp(aStr, "INDEX", strlen("INDEX")) == 0)
								{// ���f���ԍ�
									pStrCut += strlen("INDEX");								// �|�C���^��i�߂�
									PopString(pStrCut, aStr);								// �����ϊ�
									m_ModelInfo[nCntModel].nIndex = atoi(aStr);				// �����^�ɕϊ�
								}
								else if (memcmp(aStr, "PARENT", strlen("PARENT")) == 0)
								{// �e���f���ԍ�
									pStrCut += strlen("PARENT");							// �|�C���^��i�߂�
									PopString(pStrCut, aStr);								// �����ϊ�
									m_ModelInfo[nCntModel].nIndex = atoi(aStr);				// �����^�ɕϊ�
								}
								else if (memcmp(aStr, "POS", strlen("POS")) == 0)
								{// �ʒu
									pStrCut += strlen("POS");								// �|�C���^��i�߂�
									PopString(pStrCut, aStr);								// �����ϊ�
									m_ModelInfo[nCntModel].pos.x = (float)atof(aStr);		// �����^�ɕϊ�
									PopString(pStrCut, aStr);								// �����ϊ�
									m_ModelInfo[nCntModel].pos.y = (float)atof(aStr);		// �����^�ɕϊ�
									PopString(pStrCut, aStr);								// �����ϊ�
									m_ModelInfo[nCntModel].pos.z = (float)atof(aStr);		// �����^�ɕϊ�
								}
								else if (memcmp(aStr, "ROT", strlen("ROT")) == 0)
								{// ����
									pStrCut += strlen("ROT");								// �|�C���^��i�߂�
									PopString(pStrCut, aStr);								// �����ϊ�
									m_ModelInfo[nCntModel].rot.x = (float)atof(aStr);		// �����^�ɕϊ�
									PopString(pStrCut, aStr);								// �����ϊ�
									m_ModelInfo[nCntModel].rot.y = (float)atof(aStr);		// �����^�ɕϊ�
									PopString(pStrCut, aStr);								// �����ϊ�
									m_ModelInfo[nCntModel].rot.z = (float)atof(aStr);		// �����^�ɕϊ�
								}
							} while (memcmp(aStr, "END_PARTSSET", strlen("END_PARTSSET")) != 0);
							nCntModel++;	// ���f���J�E���g
						}
					} while (memcmp(aStr, "END_CHARACTERSET", strlen("END_CHARACTERSET")) != 0);
				}

				if (memcmp(aStr, "NUM_MOTION", strlen("NUM_MOTION")) == 0)
				{// ���[�V������
					pStrCut += strlen("NUM_MOTION");			// �|�C���^��i�߂�
					PopString(pStrCut, aStr);					// �����ϊ�
					m_MaxAnim = atoi(aStr);						// �����^�ɕϊ�		

					if (m_AnimInfo == NULL)
					{// NULL�̏ꍇ
						m_AnimInfo = new CMotion::ANIM_INFO[m_MaxAnim];		// �������m��

						for (int nCount = 0; nCount < m_MaxAnim; nCount++)
						{// �J�E���g
							m_AnimInfo[nCount].aKeyInfo = NULL;		// ������
						}
					}
				}
				else if (memcmp(aStr, "MOTIONSET", strlen("MOTIONSET")) == 0)
				{// ���[�V�����ݒ�
					nCntKey = 0;	// �L�[������

					do
					{// ���[�v
						pStrCut = ReadFile(pFile, &aLine[0]);				// �R�����g�`�F�b�N
						strcpy(aStr, pStrCut);								// �����ϊ�

						if (memcmp(aStr, "LOOP", strlen("LOOP")) == 0)
						{// ���[�v���邩
							pStrCut += strlen("LOOP");							// �|�C���^��i�߂�
							PopString(pStrCut, aStr);							// �����ϊ�
							m_AnimInfo[nCntMotion].bLoop = atoi(aStr);			// �����^�ɕϊ�
						}
						else if (memcmp(aStr, "NUM_KEY", strlen("NUM_KEY")) == 0)
						{// �L�[��
							pStrCut += strlen("NUM_KEY");						// �|�C���^��i�߂�
							PopString(pStrCut, aStr);							// �����ϊ�
							m_AnimInfo[nCntMotion].nKeyMax = atoi(aStr);		// �����^�ɕϊ�

							if (m_AnimInfo[nCntMotion].aKeyInfo == NULL)
							{// NULL�̏ꍇ
								m_AnimInfo[nCntMotion].aKeyInfo = new CMotion::KEY_INFO[m_AnimInfo[nCntMotion].nKeyMax];	// �������m��

								for (int nCount = 0; nCount < m_AnimInfo[nCntMotion].nKeyMax; nCount++)
								{// �J�E���g
									m_AnimInfo[nCntMotion].aKeyInfo[nCount].aKey = NULL;	// ������
								}
							}
						}
						else if (memcmp(aStr, "BLEND_FRAME", strlen("BLEND_FRAME")) == 0)
						{// �u�����h�t���[��
							pStrCut += strlen("BLEND_FRAME");						// �|�C���^��i�߂�
							PopString(pStrCut, aStr);								// �����ϊ�
							m_AnimInfo[nCntMotion].nBrendFrame = atoi(aStr);		// �����^�ɕϊ�
						}
						else if (memcmp(aStr, "KEYSET", strlen("KEYSET")) == 0)
						{// �L�[�ݒ�
							nCntModel = 0;		// ���f���J�E���g������

							if (nCntKey < m_AnimInfo[nCntMotion].nKeyMax)
							{// �L�[���ő�ȉ��̏ꍇ
								if (m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey == NULL)
								{// NULL�̏ꍇ
									m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey = new CMotion::KEY[m_MaxModel];	// �������m��
								}
							}

							do
							{// ���[�v
								pStrCut = ReadFile(pFile, &aLine[0]);					// �R�����g�`�F�b�N
								strcpy(aStr, pStrCut);									// �����ϊ�

								if (memcmp(aStr, "FRAME", strlen("FRAME")) == 0)
								{// �t���[��
									pStrCut += strlen("FRAME");										// �|�C���^��i�߂�
									PopString(pStrCut, aStr);										// �����ϊ�
									m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].nFrame = atoi(aStr);	// �����^�ɕϊ�
								}
								else if (memcmp(aStr, "KEY", strlen("KEY")) == 0)
								{// �L�[���f���ݒ�
									do
									{// ���[�v
										pStrCut = ReadFile(pFile, &aLine[0]);			// �R�����g�`�F�b�N
										strcpy(aStr, pStrCut);							// �����ϊ�

										if (memcmp(aStr, "POS", strlen("POS")) == 0)
										{// �ʒu
											pStrCut += strlen("POS");																// �|�C���^��i�߂�
											PopString(pStrCut, aStr);																// �����ϊ�
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.x = (float)atof(aStr);		// �����^�ɕϊ�
											PopString(pStrCut, aStr);																// �����ϊ�
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.y = (float)atof(aStr);		// �����^�ɕϊ�
											PopString(pStrCut, aStr);																// �����ϊ�
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].pos.z = (float)atof(aStr);		// �����^�ɕϊ�
										}
										else if (memcmp(aStr, "ROT", strlen("ROT")) == 0)
										{// ����
											pStrCut += strlen("ROT");																// �|�C���^��i�߂�
											PopString(pStrCut, aStr);																// �����ϊ�
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.x = (float)atof(aStr);		// �����^�ɕϊ�
											PopString(pStrCut, aStr);																// �����ϊ�
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.y = (float)atof(aStr);		// �����^�ɕϊ�
											PopString(pStrCut, aStr);																// �����ϊ�
											m_AnimInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntModel].rot.z = (float)atof(aStr);		// �����^�ɕϊ�
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

		fclose(pFile);		// �t�@�C���N���[�Y
	}

	// �������m��
	if (m_pTexture == NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_MaxModel];
	if (m_pMesh == NULL)	m_pMesh = new LPD3DXMESH[m_MaxModel];
	if (m_pBuffMat == NULL) m_pBuffMat = new LPD3DXBUFFER[m_MaxModel];
	if (m_nNumMat == NULL)	m_nNumMat = new DWORD[m_MaxModel];


	for (int nCntParnet = 0; nCntParnet < m_MaxModel; nCntParnet++)
	{
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(m_ModelInfo[nCntParnet].cFileName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[nCntParnet], NULL, &m_nNumMat[nCntParnet], &m_pMesh[nCntParnet]);

		D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
		D3DMATERIAL9 *matDef;						// ���݂̃}�e���A���ۑ��p
		pmat = (D3DXMATERIAL*)m_pBuffMat[nCntParnet]->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat[nCntParnet]];
		m_pTexture[nCntParnet] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntParnet]];

		for (DWORD tex = 0; tex < m_nNumMat[nCntParnet]; tex++)
		{// �J�E���g
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pTexture[nCntParnet][tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// �e�N�X�`�����g�p���Ă���
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pTexture[nCntParnet][tex])))
				{
					//MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���(ememy)", NULL, MB_OK);
				}
			}
		}

		delete[] matDef;		// �������J��
		matDef = NULL;			// NULL��
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CBullet ***
//=============================================================================
void CEnemy::Unload(void)
{
	for (int nCntParnet = 0; nCntParnet < m_MaxModel; nCntParnet++)
	{// �J�E���g
		// �e�N�X�`���̔j��
		for (DWORD tex = 0; tex < m_nNumMat[nCntParnet]; tex++)
		{// �J�E���g
			if (m_pTexture[nCntParnet][tex] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pTexture[nCntParnet][tex]->Release();		// ���
				m_pTexture[nCntParnet][tex] = NULL;			// NULL��
			}
		}

		delete[] m_pTexture[nCntParnet];		// �������J��
		m_pTexture[nCntParnet] = NULL;			// NULL��

		// �}�e���A���̊J��
		if (m_pBuffMat[nCntParnet] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pBuffMat[nCntParnet]->Release();				// ���
			m_pBuffMat[nCntParnet] = NULL;					// NULL��
		}

		// �}�e���A���̊J��
		if (m_pMesh[nCntParnet] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pMesh[nCntParnet]->Release();					// ���
			m_pMesh[nCntParnet] = NULL;						// NULL��
		}

		m_nNumMat[nCntParnet] = 0;				// ������
	}

	// �������J��
	if (m_pTexture != NULL) { delete[] m_pTexture; m_pTexture = NULL; }
	if (m_pMesh != NULL) { delete[] m_pMesh; m_pMesh = NULL; }
	if (m_pBuffMat != NULL) { delete[] m_pBuffMat; m_pBuffMat = NULL; }
	if (m_nNumMat != NULL) { delete[] m_nNumMat; m_nNumMat = NULL; }

	// ���f��
	if (m_ModelInfo != NULL)
	{// NULL�ȊO�̏ꍇ
		delete[] m_ModelInfo;		// �������J��
		m_ModelInfo = NULL;			// NULL��
	}

	// ���[�V����
	if (m_AnimInfo != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntAnim = 0; nCntAnim < m_MaxAnim; nCntAnim++)
		{// ���[�V�����J�E���g
			for (int nCntKey = 0; nCntKey < m_AnimInfo[nCntAnim].nKeyMax; nCntKey++)
			{// �L�[�J�E���g
				delete[] m_AnimInfo[nCntAnim].aKeyInfo[nCntKey].aKey;	// �������J��
				m_AnimInfo[nCntAnim].aKeyInfo[nCntKey].aKey = NULL;		// NULL��
			}
			delete[] m_AnimInfo[nCntAnim].aKeyInfo;		// �������J��
			m_AnimInfo[nCntAnim].aKeyInfo = NULL;		// NULL��
		}

		delete[] m_AnimInfo;	// �������J��
		m_AnimInfo = NULL;		// NULL��
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CEnemy ***
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = NULL;			// �|�C���^

	if (pEnemy == NULL)
	{// NULL�̏ꍇ
		pEnemy = new CEnemy;			// �������m��
	}

	if (pEnemy != NULL)
	{// NULL�ȊO�̏ꍇ
		pEnemy->m_pModel = new CModel[m_MaxModel];		// �������m��

		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// �J�E���g
			// �����l�ݒ�
			pEnemy->m_pModel[nCntModel].SetInitAll(m_pTexture[nCntModel], m_pMesh[nCntModel], m_pBuffMat[nCntModel], m_nNumMat[nCntModel], m_ModelInfo[nCntModel].pos, m_ModelInfo[nCntModel].rot);
			// ����������
			pEnemy->m_pModel[nCntModel].Init();

			if (0 <= m_ModelInfo[nCntModel].nIndex)
			{// �q�̏ꍇ
				// �e��ݒ�
				pEnemy->m_pModel[nCntModel].SetParent(&pEnemy->m_pModel[m_ModelInfo[nCntModel].nIndex]);
			}
		}

		pEnemy->m_pos = pos;			// �ʒu
		pEnemy->m_rot = rot;			// �p�x
		pEnemy->m_frotDest = rot.y;		// �ڕW�p�x
		pEnemy->Init();					// ����������
	}

	return pEnemy;			// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CEnemy ***
//=============================================================================
HRESULT CEnemy::Init()
{
	// ���[�V����
	if (m_pMotion == NULL)
	{// NULL�̏ꍇ
		m_pMotion = new CMotion;	// �������m��
	}

	// ����ݒ�
	m_vtxMax = D3DXVECTOR3(50.0f, 1000.0f, 50.0f);
	m_vtxMin = D3DXVECTOR3(-50.0f, 0.0f, -50.0f);

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Uninit(void)
{
	// ���f��
	if (m_pModel != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// �J�E���g
			m_pModel[nCntModel].Uninit();		// �I������
		}
		delete[] m_pModel;		// �������J��
		m_pModel = NULL;		// NULL��
	}

	// ���[�V����
	if (m_pMotion != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pMotion->Uninit();	// �I������
		delete m_pMotion;		// �������J��
		m_pMotion = NULL;		// NULL��
	}

	CGame::SetEnemy();			// NULL��

	CScene::SetDeath();			// �I������
}

//=============================================================================
// �X�V����											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Update(void)
{
	bool bMeshField = false;					// ���b�V���t�B�[���h�L��
	CMeshField *pMeshField = NULL;				// ���b�V���t�B�[���h�|�C���^
	CSound *pSound = CManager::GetSound();		// �T�E���h�擾

	for (int nCount = 0; nCount < CGame::m_MeshFieldMax; nCount++)
	{// �t�B�[���h�J�E���^
		pMeshField = CGame::GetMeshField(nCount);	// ���b�V���t�B�[���h�擾

		if (pMeshField != NULL)
		{// NULL�ȊO�̏ꍇ
			if (pMeshField->ColInMesh(m_pos + m_vtxMax))
			{// ������ɂ��邩
				bMeshField = true;			// ���b�V���L��
				break;
			}
			else if (pMeshField->ColInMesh(m_pos))
			{// ������ɂ��邩
				bMeshField = true;			// ���b�V���L��
				break;
			}
			else if (pMeshField->ColInMesh(m_pos + m_vtxMin))
			{// ������ɂ��邩
				bMeshField = true;			// ���b�V���L��
				break;
			}
		}
	}

	float fHeight = -500.0f;		// ����
	float fUpDown = 0.0f;			// ���x

	if (bMeshField == true)
	{// ���b�V���t�B�[���h������
		fHeight = pMeshField->GetHeight(m_pos);		// �����擾
	}
	m_shadowHeight = fHeight;		// �e�����ݒ�

	m_posOld = m_pos;		// �ʒu�ۑ�

	if (m_bStart == true)
	{// �X�^�[�g
		if (m_pos.x < -300.0f)
		{// ���ʒu�ȉ��̂�
			if (m_pos.x < -420.0f)
			{// ���ʒu�ȉ��̂�
				m_pos.y = 300.0f;	// ����
			}
			else
			{// ���̑�
				if (m_pos.y == 300.0f)
				{// ���ʒu�ɂȂ�����
					if (m_pMotion->GetAnimNum() != 3)
					{// �������[�V�����ȊO��������
						m_pMotion->SetAnimNum(3);	//	�������[�V����
					}
				}
				m_pos.y -= 20.0f;					// �d��
			}

			// �ړ�����
			m_frotDest = -D3DX_PI * 0.5f;
			m_move.x += sinf(-m_frotDest) * ((0.8f - m_bWallJumpmove) + (0.8f * fUpDown));
			m_move.z += cosf(-m_frotDest) * ((0.8f - m_bWallJumpmove) + (0.8f * fUpDown));
		}
		else
		{// ���̑�
			m_nCntStart++;

			if (m_nCntStart == 180)
			{// �X�^�[�g�J�E���g
				m_bStart = false;		// �X�^�[�g�I���
				m_nCntStart = 0;		// �X�^�[�g�J�E���g
			}
		}
	}
	else
	{// ���̑�
		// �ړ�����
		m_frotDest = -D3DX_PI * 0.5f;
		m_move.x += sinf(-m_frotDest) * ((MOVE_SPEED - m_bWallJumpmove) + (MOVE_SPEED * fUpDown));
		m_move.z += cosf(-m_frotDest) * ((MOVE_SPEED - m_bWallJumpmove) + (MOVE_SPEED * fUpDown));

		m_pMotion->SetAnimNum(1);	//	�������[�V����
	}

	m_pos += m_move;		// �ړ��ʉ��Z

	bool bBlock = false;	// �u���b�N�ɓ���������

	CScene *pScene = CScene::GetScene(CScene::PRIORITY_3);		// �V�[���擾
	CScene *pSceneNext = NULL;									// ���V�[��

	while (pScene != NULL)
	{// NULL�ȊO�̏ꍇ
		pSceneNext = pScene->GetSceneNext();					// ���V�[�����L��
		if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK)
		{//	�u���b�N�̏ꍇ
			CBlock *pBlock = (CBlock*)pScene;					// �u���b�N�ɃL���X�g
			pBlock->Collision(&m_pos, &m_posOld, &m_move, &m_fDownSpeed, m_vtxMax, m_vtxMin);
			bool *bLand = pBlock->GetHit();			// �Փˈʒu�̎擾
			for (int nCount = 0; nCount < 4; nCount++)
			{// �J�E���g
				if (bLand[nCount] == true)
				{// �Փ˂��Ă�����
					bBlock = true;	// �u���b�N�ɓ�������

					D3DXVECTOR3 posblock = pBlock->Getpos();		// �ʒu�擾
					CEffect::Create(posblock, 5, 1);//�@�e�N�X�`���@�e�L�X�g
					CEffect::Create(posblock, 2, 3);//�@�e�N�X�`���@�e�L�X�g
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE032);	// �T�E���h
					pBlock->Uninit();		// �u���b�N�I������
				}
			}
		}
		pScene = pSceneNext;		// ���V�[��
	}

	if (m_pos.y < fHeight)
	{// �n�ʂ��Ⴍ�Ȃ�����
		m_pos.y = fHeight;			// ��������
		m_move.y = 0.0f;			// �ړ��ʁiY�j���O��
		m_fDownSpeed = 0.2f;		// �������x��������
		m_nWallJump = 5;			// �ǃL�b�N�J�E���g������
		m_bWallJumpmove = 0.0f;		// �ǃL�b�N�ړ��ʏ�����
		m_bJump = false;			// �W�����v���Ă��Ȃ�
	}


	if (0.65f < m_bWallJumpmove)
	{// �ǃL�b�N�ړ��ʂ����ȉ�
		// �p�x����
		m_rot.y += ((m_frotDest - m_rot.y) / 5.0f) * ((0.65f - (m_bWallJumpmove / 2.0f)) / 0.65f);
	}
	else
	{// ���̑�
		// �p�x����
		m_rot.y += ((m_frotDest - m_rot.y) / 5.0f);
	}

	// ��������
	m_move.x -= (m_move.x - 0.0f) * 0.2f;
	//m_move.y -= 0.7f;
	m_move.z -= (m_move.z - 0.0f) * 0.2f;

	if (m_bStart == false || (m_bStart == true && m_nCntStart < 60))
	{// �X�^�[�g��ԂłȂ��A�X�^�[�g��� && ���J�E���g�ȉ�
		m_pMotion->Update(m_MaxModel, &m_AnimInfo[0], &m_pModel[0], m_ModelInfo[0].pos);		// ���[�V�����X�V
	}

	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_TEST);		// �t�H���g�擾

	// �����ݒ�
	pFont->SetMessage(CFont::SETTYPE_ADD, "Enemy : �ʒu�@[%.2f, %.2f, %.2f]\n", m_pos.x, m_pos.y, m_pos.z);
	pFont->SetMessage(CFont::SETTYPE_ADD, "Enemy : fHeight�@[%.2f]\n", fHeight);
	pFont->SetMessage(CFont::SETTYPE_ADD, "Enemy : bMesh�@[%d]\n", bMeshField);

	if (m_pos.x >= 12260)
	{// �S�[���ɋ߂Â�����
		m_move.y -= 0.02f;	// ����
	}
	if (m_pos.x >= 12780)
	{//	�G����������
		CEffect::Create(m_pos + D3DXVECTOR3(0.0f, 80.0f, 0.0f), 3, 10);//�@�e�N�X�`���@�e�L�X�g
		CEffect::Create(m_pos + D3DXVECTOR3(0.0f, 80.0f, 0.0f), 6, 9);//�@�e�N�X�`���@�e�L�X�g
		Uninit();
		pSound->PlaySoundA(CSound::SOUND_LABEL_SE028);
	}
}

//=============================================================================
// �`�揈��											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX mtxRot, mtxTrans, matScale;			// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�g��k���s��̍쐬
	D3DXMatrixScaling(&matScale, m_fScaleX, m_fScaleY, m_fScaleZ);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &matScale);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
	{
		if (m_pModel != NULL)
		{
			m_pModel[nCntModel].Draw();
			CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(m_pos.x, m_shadowHeight, m_pos.z));
		}
	}
}
//=============================================================================
//	�G�̓����蔻��									(public)	*** CEnemy ***
//=============================================================================
bool CEnemy::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	bool bHit = false;		// �Փ�

	// ����`�F�b�N
	if (m_pos.x + (m_vtxMax.x / 2) > pPos->x + sizeMin.x && //�@�E
		m_pos.x + (m_vtxMin.x / 2) < pPos->x + sizeMax.x && //�@��
		m_pos.z + (m_vtxMax.z / 2) > pPos->z + sizeMin.z && //�@��
		m_pos.z + (m_vtxMin.z / 2) < pPos->z + sizeMax.z && //�@��O
		m_pos.y + (m_vtxMax.y / 2) > pPos->y + sizeMin.y && //�@��
		m_pos.y + (m_vtxMin.y / 2) < pPos->y + sizeMax.y)	//�@��O
	{
		bHit = true;			// ��������

		// �ړ��ʒ���
		pMove->x += 200.0f;		
		pMove->y += 10.0f;
	}

	return bHit;		// �l��Ԃ�
}