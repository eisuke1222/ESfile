//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "game.h"			// �Q�[��
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "sound.h"			// �T�E���h
#include "input.h"			// ����
#include "scene2d.h"		// �V�[��2D
#include "scene3d.h"		// �V�[��2D
#include "fade.h"			// �t�F�[�h
#include "camera.h"			// �J����
#include "meshfield.h"		// ���b�V���t�B�[���h
#include "object3d.h"		// �I�u�W�F�N�g3D
#include "effect3d.h"		// �G�t�F�N�g3D
#include "bg.h"				// �w�i
#include "enemy.h"			// �G
#include "player.h"			// �v���C���[
#include "bullet.h"			// �e
#include "effect.h"			// �G�t�F�N�g
#include "explosion.h"		// ����
#include "cloud.h"			// �_
#include "number.h"			// ����
#include "bomb.h"			// ���e
#include "gauge.h"			// �Q�[�W
#include "life.h"			// �̗�
#include "score.h"			// �X�R�A
#include "ranking.h"		// �����L���O
#include "pause.h"			// �|�[�Y

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAME_TEXT_MAP		"data\\TEXT\\map.txt"
#define GAME_TEXT_ENEMY		"data\\TEXT\\enemy.txt"
#define TEXTURE_NAME		"data\\TEXTURE\\playerNum.png"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CGame::m_pTexture				= NULL;		// �e�N�X�`�����ւ̃|�C���^
CScene2D			*CGame::m_pScene2D[MAX_PLAYER]	= {};		// �V�[��2D
CRanking			*CGame::m_pRanking			= NULL;			// �����L���O
CPause				*CGame::m_pPause			= NULL;			// �|�[�Y
bool				CGame::m_bPause				= false;		// �|�[�Y�g�p
CGame::GAME			CGame::m_game				= GAME_NONE;	// �Q�[��
int					CGame::m_nCntGame			= 0;			// �Q�[���J�E���^

LPDIRECT3DTEXTURE9	CGame::m_pENDTexture[2] = {};
CScene3D			*CGame::m_pScene3D = NULL;

CGame::MAP			CGame::m_mapState[MAX_MAP] = {};			// �}�b�v���
CGame::ENEMY		CGame::m_enemyState[MAX_MAP]= {};			// �G���
int					CGame::m_mapMax				= 0;			// �}�b�v�ő吔
int					CGame::m_enemyMax			= 0;			// �}�b�v�ő吔

//=============================================================================
// �R���X�g���N�^									(public)	*** CGame ***
//=============================================================================
CGame::CGame()
{
	// �����ݒ�
	m_bPause	= false;		// �|�[�Y�g�p
	m_game		= GAME_NONE;	// �Q�[��
	m_nCntGame	= 0;			// �Q�[���J�E���^
	m_mapNum	= 0;			// �}�b�v�ԍ�
	m_enemyNum	= 0;			// �}�b�v�ԍ�
	m_mapfield	= 0;			// �t�B�[���h�ԍ�
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CGame ***
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ���[�h����							(public)	*** CBullet ***
//=============================================================================
HRESULT CGame::Load(void)
{
	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{// �J�E���g
		m_mapState[nCntMap].nNumModel = 0;							// ���f���ԍ�
		m_mapState[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		m_mapState[nCntMap].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	}

	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{// �J�E���g
		m_enemyState[nCntMap].nNumEnemy = 0;							// ���f���ԍ�
		m_enemyState[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
		m_enemyState[nCntMap].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
		m_enemyState[nCntMap].nShotTiming = 0;							// ����
	}

	m_mapMax = 0;				// �}�b�v�ő吔
	m_enemyMax = 0;

	FILE *pFile;				// �t�@�C���|�C���^
	char cMessage[255] = {};	// �����`�F�b�N

	{
		// �t�@�C���ݒ�
		pFile = fopen(GAME_TEXT_MAP, "r");

		// �v���C���[�e�L�X�g�f�[�^�ǂݍ���
		if (pFile != NULL)
		{// �t�@�C�����J�����ꍇ
			fscanf(pFile, "%s", &cMessage[0]);		// �ǂݍ���

			if (strcmp(cMessage, "SCRIPT") == 0)
			{// �ǂݍ��݊J�n
				do
				{// �I���x���܂Ń��[�v
					fscanf(pFile, "%s", &cMessage[0]);		// �ǂݍ���

					if (cMessage[0] == 35)
					{// #�Ŏn�܂��Ă���ꍇ
						fgets(cMessage, 255, pFile);		// �R�����g�Ƃ��Ĉ�s�ǂݎ��
					}
					else
					{// ���̑�
						if (strcmp(cMessage, "MAP_SET") == 0)
						{// �}�b�v�ݒ�J�n
							do
							{// �I���x���܂Ń��[�v
								fscanf(pFile, "%s", &cMessage[0]);		// �ǂݍ���

								if (cMessage[0] == 35)
								{// #�Ŏn�܂��Ă���ꍇ
									fgets(cMessage, 255, pFile);		// �R�����g�Ƃ��Ĉ�s�ǂݎ��
								}
								else
								{// ���̑�
									if (strcmp(cMessage, "MODEL_SET") == 0)
									{// ���f���ݒ�J�n
										do
										{// �I���x���܂Ń��[�v
											fscanf(pFile, "%s", &cMessage[0]);		// �ǂݍ���

											if (cMessage[0] == 35)
											{// #�Ŏn�܂��Ă���ꍇ
												fgets(cMessage, 255, pFile);		// �R�����g�Ƃ��Ĉ�s�ǂݎ��
											}
											else
											{// ���̑�
												if (strcmp(cMessage, "MODEL_NUM") == 0)
												{// ���f���ԍ�
													fscanf(pFile, "%s %d", &cMessage[0], &m_mapState[m_mapMax].nNumModel);
												}
												else if (strcmp(cMessage, "POS") == 0)
												{// �ʒu
													fscanf(pFile, "%s %f %f %f", &cMessage[0], &m_mapState[m_mapMax].pos.x, &m_mapState[m_mapMax].pos.y, &m_mapState[m_mapMax].pos.z);
												}
												else if (strcmp(cMessage, "ROT") == 0)
												{// ����
													fscanf(pFile, "%s %f %f %f", &cMessage[0], &m_mapState[m_mapMax].rot.x, &m_mapState[m_mapMax].rot.y, &m_mapState[m_mapMax].rot.z);
												}
											}
										} while (strcmp(cMessage, "END_MODEL_SET") != 0);		// ���f���ݒ�I��
										m_mapMax++;		// ���f���ő吔�𑝂₷
									}
								}
							} while (strcmp(cMessage, "END_MAP_SET") != 0);			// �}�b�v�ݒ�I��
						}
					}
				} while (strcmp(cMessage, "END_SCRIPT") != 0);		// �ǂݍ��ݏI��
			}

			fclose(pFile);	// �t�@�C�������
		}
		else
		{// �t�@�C�����J���Ȃ��ꍇ
			MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
		}

		int nMin;	// �\�[�g�Ώ�
		MAP map;	// �}�b�v

		for (int nCntMap = 1; nCntMap < m_mapMax; nCntMap++)
		{// �J�E���g
			nMin = nCntMap;		// �ԍ����L��

			while (nMin > 0 && m_mapState[nMin - 1].pos.z > m_mapState[nMin].pos.z)
			{// �l��������������
				 // �}�b�v���\�[�g
				map = m_mapState[nMin - 1];
				m_mapState[nMin - 1] = m_mapState[nMin];
				m_mapState[nMin] = map;

				nMin--;			// �\�[�g�Ώۂ��ړ�

				// �\�[�g�����b�Z�[�W�ŕ\��
				wsprintf(cMessage, "%d �� %d �����ւ��܂���", nMin, nCntMap);
				MessageBox(0, cMessage, "�}�b�v�f�[�^���\�[�g", MB_OK);
			}
		}
	}

	{
		// �t�@�C���ݒ�
		pFile = fopen(GAME_TEXT_ENEMY, "r");

		// �v���C���[�e�L�X�g�f�[�^�ǂݍ���
		if (pFile != NULL)
		{// �t�@�C�����J�����ꍇ
			fscanf(pFile, "%s", &cMessage[0]);		// �ǂݍ���

			if (strcmp(cMessage, "SCRIPT") == 0)
			{// �ǂݍ��݊J�n
				do
				{// �I���x���܂Ń��[�v
					fscanf(pFile, "%s", &cMessage[0]);		// �ǂݍ���

					if (cMessage[0] == 35)
					{// #�Ŏn�܂��Ă���ꍇ
						fgets(cMessage, 255, pFile);		// �R�����g�Ƃ��Ĉ�s�ǂݎ��
					}
					else
					{// ���̑�
						if (strcmp(cMessage, "ENEMY_SET") == 0)
						{// �}�b�v�ݒ�J�n
							do
							{// �I���x���܂Ń��[�v
								fscanf(pFile, "%s", &cMessage[0]);		// �ǂݍ���

								if (cMessage[0] == 35)
								{// #�Ŏn�܂��Ă���ꍇ
									fgets(cMessage, 255, pFile);		// �R�����g�Ƃ��Ĉ�s�ǂݎ��
								}
								else
								{// ���̑�
									if (strcmp(cMessage, "MODEL_SET") == 0)
									{// ���f���ݒ�J�n
										do
										{// �I���x���܂Ń��[�v
											fscanf(pFile, "%s", &cMessage[0]);		// �ǂݍ���

											if (cMessage[0] == 35)
											{// #�Ŏn�܂��Ă���ꍇ
												fgets(cMessage, 255, pFile);		// �R�����g�Ƃ��Ĉ�s�ǂݎ��
											}
											else
											{// ���̑�
												if (strcmp(cMessage, "ENEMY_NUM") == 0)
												{// ���f���ԍ�
													fscanf(pFile, "%s %d", &cMessage[0], &m_enemyState[m_enemyMax].nNumEnemy);
												}
												else if (strcmp(cMessage, "POS") == 0)
												{// �ʒu
													fscanf(pFile, "%s %f %f %f", &cMessage[0], &m_enemyState[m_enemyMax].pos.x, &m_enemyState[m_enemyMax].pos.y, &m_enemyState[m_enemyMax].pos.z);
												}
												else if (strcmp(cMessage, "MOVE") == 0)
												{// ����
													fscanf(pFile, "%s %f %f %f", &cMessage[0], &m_enemyState[m_enemyMax].move.x, &m_enemyState[m_enemyMax].move.y, &m_enemyState[m_enemyMax].move.z);
												}
												else if (strcmp(cMessage, "SHOT") == 0)
												{// ����
													fscanf(pFile, "%s %d", &cMessage[0], &m_enemyState[m_enemyMax].nShotTiming);
												}
											}
										} while (strcmp(cMessage, "END_MODEL_SET") != 0);		// ���f���ݒ�I��
										m_enemyMax++;		// ���f���ő吔�𑝂₷
									}
								}
							} while (strcmp(cMessage, "END_ENEMY_SET") != 0);			// �}�b�v�ݒ�I��
						}
					}
				} while (strcmp(cMessage, "END_SCRIPT") != 0);		// �ǂݍ��ݏI��
			}

			fclose(pFile);	// �t�@�C�������
		}
		else
		{// �t�@�C�����J���Ȃ��ꍇ
			MessageBox(0, "�Z�[�u�t�@�C���̃`�F�b�N�Ɏ��s�I", "�x���I", MB_ICONWARNING);
		}

		int nMin;	// �\�[�g�Ώ�
		ENEMY enemy;	// �G

		for (int nCntMap = 1; nCntMap < m_enemyMax; nCntMap++)
		{// �J�E���g
			nMin = nCntMap;		// �ԍ����L��

			while (nMin > 0 && m_enemyState[nMin - 1].pos.z > m_enemyState[nMin].pos.z)
			{// �l��������������
			 // �}�b�v���\�[�g
				enemy = m_enemyState[nMin - 1];
				m_enemyState[nMin - 1] = m_enemyState[nMin];
				m_enemyState[nMin] = enemy;

				nMin--;			// �\�[�g�Ώۂ��ړ�

				// �\�[�g�����b�Z�[�W�ŕ\��
				wsprintf(cMessage, "%d �� %d �����ւ��܂���", nMin, nCntMap);
				MessageBox(0, cMessage, "�}�b�v�f�[�^���\�[�g", MB_OK);
			}
		}
	}

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�擾

	if (m_pTexture == NULL)
	{// NULL�̏ꍇ
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	if (m_pENDTexture[0] == NULL)
	{// NULL�̏ꍇ
	 // �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\GAMECLEAR.png", &m_pENDTexture[0]);
		D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\GAMEOVER.png", &m_pENDTexture[1]);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CBullet ***
//=============================================================================
void CGame::Unload(void)
{
	for (int nCntMap = 0; nCntMap < MAX_MAP; nCntMap++)
	{// �J�E���g
		m_mapState[nCntMap].nNumModel = 0;							// ���f���ԍ�
		m_mapState[nCntMap].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
		m_mapState[nCntMap].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	}

	m_mapMax = 0;		// ���f���ő吔

	if (m_pTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pTexture->Release();			// ���
		m_pTexture = NULL;				// NULL��
	}
}

//=============================================================================
// ����������										(public)	*** CGame ***
//=============================================================================
HRESULT CGame::Init(void)
{
	CCamera *pCamera = CManager::GetCamera();		// �J�����擾
	pCamera->SetPos(D3DXVECTOR3(0.0f, 750.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �J���������ݒ�

#ifdef _DEBUG
	CManager::SetPlayNum(CInputXPad::GetInputNum());
#endif // !1

	// 3D���[�h
	CMeshField::Load();		// ���b�V���t�B�[���h
	CObject3D::Load();		// ���f��
	CEffect3D::Load();		// ���f��
	CEnemy::Load();			// �G
	CPlayer::Load();		// �v���C���[
	CBullet::Load();		// �e
	CEffect::Load();		// �G�t�F�N�g
	CExplosion::Load();		// ����
	CCloud::Load();			// �_
	// 3D�N���G�C�g
	CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
		D3DXVECTOR3(0.0f, -100.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);		// ���b�V���t�B�[���h
	CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
		D3DXVECTOR3(0.0f, -100.0f, 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);	// ���b�V���t�B�[���h

	// 2D���[�h
	CBg::Load();			// �w�i
	CLife::Load();			// �̗�
	CBomb::Load();			// ���e
	CGauge::Load();			// �Q�[�W
	CNumber::Load();		// ����
	CRanking::Load();		// �����L���O
	// 2D�N���G�C�g
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, CBg::TEXTURE_GAME);	// �w�i

	for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
	{
		CLife::Create(D3DXVECTOR3(50.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 180, 0.0f), D3DXVECTOR3(LIFE_SIZE_X, LIFE_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);					// �̗�
		CBomb::Create(3, D3DXVECTOR3(50.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 260, 0.0f), D3DXVECTOR3(BOMB_SIZE_X, BOMB_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);				// ���e
		CGauge::Create(D3DXVECTOR3(100.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 340, 0.0f), D3DXVECTOR3(GAUGE_SIZE_X, GAUGE_SIZE_Y, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.1f, 1.0f), nCntCont);				// �Q�[�W
		CScore::Create(D3DXVECTOR3(SCREEN_WIDTH_0 - 50.0f + ((SCREEN_WIDTH - 300.0f) * nCntCont), 100.0f, 0.0f), D3DXVECTOR3(12.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);			// �X�R�A
	}

	CRanking::LoadRanking();			// �����L���O���[�h
	//CRanking::Create(D3DXVECTOR3(0.0f, 500.0f, -300.0f), D3DXVECTOR3(0.89f, 0.0f, 0.0f), D3DXVECTOR3(18.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -20.0f), 1);

	CPause::Load();			// �|�[�Y

	for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
	{
		if (m_pScene2D[nCnt] == NULL)
		{
			m_pScene2D[nCnt] = new CScene2D(CScene::PRIORITY_UI);
			m_pScene2D[nCnt]->SetInitAll(D3DXVECTOR3(0.0f + 60.0f + ((SCREEN_WIDTH - 60.0f - 60.0f) * nCnt), SCREEN_HEIGHT - 40.0f, 0.0f), D3DXVECTOR3(35.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
			m_pScene2D[nCnt]->Init();
			m_pScene2D[nCnt]->SetObjType(CScene::OBJTYPE_PLAYERNUM);
			m_pScene2D[nCnt]->BindTexture(m_pTexture);

			m_pScene2D[nCnt]->SetTex(1, nCnt, D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	// �����ݒ�
	m_pPause	= NULL;			// �|�[�Y
	m_bPause	= false;		// �|�[�Y�g�p
	m_pRanking	= NULL;			// �����L���O
	m_game		= GAME_PLAY;	// �Q�[��
	m_nCntGame	= 0;			// �Q�[���J�E���^

	m_pScene3D = NULL;

	m_mapNum	= 0;			// �}�b�v�ԍ�
	m_mapfield	= 0;			// �t�B�[���h�ԍ�
	
	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CGame ***
//=============================================================================
void CGame::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pScene2D[nCnt] != NULL)
		{
			m_pScene2D[nCnt]->Uninit();		// �I������
			m_pScene2D[nCnt] = NULL;		// NULL��
		}
	}

	if (m_pPause != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pPause->Uninit();		// �I������
		m_pPause = NULL;		// NULL
	}

	if (m_pRanking != NULL)
	{
		m_pRanking->Uninit();	// �I������
		m_pRanking = NULL;		// NULL
	}

	if (m_pScene3D != NULL)
	{
		m_pScene3D->Uninit();
		m_pScene3D = NULL;
	}

	// 3D�A�����[�h
	CMeshField::Unload();	// ���b�V���t�B�[���h
	CObject3D::Unload();	// ���f��
	CEffect3D::Unload();	// ���f��
	CEnemy::Unload();		// �G
	CPlayer::Unload();		// �v���C���[
	CBullet::Unload();		// �e
	CEffect::Unload();		// �G�t�F�N�g
	CExplosion::Unload();	// ����

	// 2D�A�����[�h
	CBg::Unload();			// �w�i
	CLife::Unload();		// �̗�
	CBomb::Unload();		// ���e
	CGauge::Unload();		// �Q�[�W
	CNumber::Unload();		// ����
	CRanking::Unload();		// �����L���O

	CPause::Unload();		// �|�[�Y

	CScene *pScene;			// �V�[���|�C���^

	for (int nCntPriority = 0; nCntPriority <= CScene::PRIORITY_PAUSE; nCntPriority++)
	{// �v���C�I���e�B�J�E���g
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// �V�[���J�E���g
			pScene = CScene::GetScene(nCntPriority, nCntScene);		// �V�[���ǂݍ���

			if (pScene != NULL)
			{// NULL�ȊO�̏ꍇ
				pScene->Uninit();		// �I������
				pScene = NULL;			// NULL��
			}
		}
	}
}

//=============================================================================
// �X�V����											(public)	*** CGame ***
//=============================================================================
void CGame::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// ���̓f�o�C�X
	CInputXPad *pInputXPad= CManager::GetInputXPad();				// ���̓f�o�C�X
	CSound *pSound = CManager::GetSound();							// �T�E���h
	CFade::FADE fade = CFade::GetFade();							// �t�F�[�h��Ԏ擾
	D3DXVECTOR3 CameraposR = CManager::GetCamera()->GetposR();		// �J���������_�擾

	if (m_bPause == false)
	{
		SetMap();		// �}�b�v�ݒ�
	}

	switch (m_game)
	{// �Q�[��
	case GAME_PLAY:		// �v���C
		//if (pInputKeyboard->GetTrigger(DIK_P) == true && fade == CFade::FADE_NONE)
		for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
		{
			if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_START, nCnt) == true && fade == CFade::FADE_NONE)
			{// �C�ӂ̃L�[�iP�j���������ꍇ
				m_bPause = m_bPause ? false : true;		// �\��or��\��

				switch (m_bPause)
				{// �|�[�Y���g�p
				case true:
					if (m_pPause == NULL)
					{// �����ƍX�V����
						m_pPause = CPause::Create();					// �|�[�Y�N���G�C�g

						CScene::SetStartUp(CScene::PRIORITY_PAUSE);		// �X�V�͈͂��|�[�Y����
					}

					break;
				case false:
					if (m_pPause != NULL)
					{// NULL�ȊO�̏ꍇ
						m_pPause->Uninit();		// �I������
						m_pPause = NULL;		// NULL

						CScene::SetStartUp(0);	// �X�V�͈͂�������
					}

					break;
				}

				pSound->PlaySoundA(CSound::SOUND_LABEL_SE000);
			}
		}

		break;

	case GAME_CLEAR:	// �N���A
	case GAME_OVER:		// �I�[�o�[
		if (m_nCntGame == 0)
		{
			CRanking::SaveRanking();		// �����L���O�Z�[�u

			if (m_pScene3D == NULL)
			{
				m_pScene3D = new CScene3D(CScene::PRIORITY_UI);
				m_pScene3D->SetInitAll(D3DXVECTOR3(0.0f, 550.0f, -300.0f + CameraposR.z), D3DXVECTOR3(0.89f, 0.0f, 0.0f), D3DXVECTOR3(80.0f, 40.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);
				m_pScene3D->Init();

				if (m_game == GAME_CLEAR)
				{
					m_pScene3D->BindTexture(m_pENDTexture[0]);
				}
				else
				{
					m_pScene3D->BindTexture(m_pENDTexture[1]);
				}				
			}
		}

		m_nCntGame++;						// �Q�[���J�E���^

		if (m_pScene3D != NULL)
		{
			m_pScene3D->Setpos(D3DXVECTOR3(m_pScene3D->Getpos().x, m_pScene3D->Getpos().y, m_pScene3D->Getpos().z + 3.0f));
		}

		if (m_nCntGame == 180)
		{
			m_pRanking = CRanking::Create(D3DXVECTOR3(0.0f, 470.0f, -300.0f + CameraposR.z), D3DXVECTOR3(0.89f, 0.0f, 0.0f), D3DXVECTOR3(18.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -20.0f), 1);
		}
		else if (m_nCntGame == 300)
		{
			CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
		}

		break;
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true && m_bPause == false)
	{
		CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
	}
}

//=============================================================================
// �`�揈��											(public)	*** CGame ***
//=============================================================================
void CGame::Draw(void)
{
	
}

//=============================================================================
// �|�[�Y�ݒ菈��									(public)	*** CGame ***
//=============================================================================
void CGame::SetPause(bool bPause)
{
	m_bPause = bPause;		// �|�[�Y

	switch (CGame::m_bPause)
	{// �|�[�Y
	case true:
		if (CGame::m_pPause == NULL)
		{// NULL�̏ꍇ
			m_pPause = CPause::Create();					// �|�[�Y�N���G�C�g

			CScene::SetStartUp(CScene::PRIORITY_PAUSE);		// �X�V�͈͂��|�[�Y����
		}

		break;
	case false:
		if (m_pPause != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pPause->Uninit();		// �I������
			m_pPause = NULL;		// NULL

			CScene::SetStartUp(0);	// �X�V�͈͂�������
		}

		break;
	}
}

//=============================================================================
// �}�b�v�ݒ菈��									(public)	*** CGame ***
//=============================================================================
void CGame::SetMap(void)
{
	CSound *pSound = CManager::GetSound();							// �T�E���h
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposV();		// �J�������_�擾

	if ((int)(Camerapos.z - 1.0f) % 1200 == 0)
	{// �J���������Ԋu�i�񂾂�
		if ((7200.0f < Camerapos.z && Camerapos.z < 12000.0f)
			)
		{// �n��
			CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
				D3DXVECTOR3(0.0f, 0.0f, 2600 + (m_mapfield * 1200.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), 
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_G, CMesh3D::MESHTEX_NUMSIZE);				// ���b�V���t�B�[���h

			if (rand() % 3 == 0)
			{
				CObject3D::Create(D3DXVECTOR3(-150.0f + (150.0f * (rand() % 3)), 0.0f, Camerapos.z + 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject3D::OBJECTTYPE_LIFE);		// ���f��
			}
		}
		else
		{// �C
			CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
				D3DXVECTOR3(0.0f, -100.0f, 2600 + (m_mapfield * 1200.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), 
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);				// ���b�V���t�B�[���h

			if (rand() % 3 == 0)
			{
				CObject3D::Create(D3DXVECTOR3(-150.0f + (150.0f * (rand() % 3)), 0.0f, Camerapos.z + 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject3D::OBJECTTYPE_LIFE);		// ���f��
			}

			if ((int)(Camerapos.z - 1.0f) == 8000)
			{
				CObject3D::Create(D3DXVECTOR3(-150.0f + (150.0f * (rand() % 3)), 0.0f, Camerapos.z + 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject3D::OBJECTTYPE_LIFE);		// ���f��
			}
		}
		m_mapfield++;		// �t�B�[���h�J�E���g
	}

	if ((int)(Camerapos.z - 1.0f) % 300 == 0)
	{// �J���������Ԋu�i�񂾂�
		float size = (float)((rand() % 50) * 0.01f) + 1.0f;		// �傫�������_��

		CCloud::Create(D3DXVECTOR3(-150.0f + (float)(rand() % 300), 400.0f, Camerapos.z + 600.0f + (float)(rand() % 100)), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f), 
			D3DXVECTOR3(50.0f * size, 30.0f * size, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f - ((rand() % 2) * 2.0f), 1.0f - ((rand() % 2) * 2.0f)), CScene3D::SCENE3DTYPE_NORMAL);			// �_
	}

	while (1)
	{// ���[�v
		if (m_mapNum < m_mapMax)
		{
			if (m_mapState[m_mapNum].pos.z <= Camerapos.z + 1400.0f)
			{// �J�������߂��Ȃ�����
				CObject3D::Create(m_mapState[m_mapNum].pos, m_mapState[m_mapNum].rot, (CObject3D::OBJECTTYPE)m_mapState[m_mapNum].nNumModel);		// ���f��
				m_mapNum++;		// �}�b�v�J�E���g
			}
			else
			{// �Y���Ȃ�
				break;			// �����𔲂���
			}
		}
		else
		{// �Y���Ȃ�
			break;			// �����𔲂���
		}
	}

	while (1)
	{// ���[�v
		if (m_enemyNum < m_enemyMax)
		{
			if (m_enemyState[m_enemyNum].pos.z <= Camerapos.z + 1400.0f)
			{// �J�������߂��Ȃ�����
				CEnemy::Create(m_enemyState[m_enemyNum].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_enemyState[m_enemyNum].move, (CEnemy::ENEMYTYPE)m_enemyState[m_enemyNum].nNumEnemy, m_enemyState[m_enemyNum].nShotTiming);
				m_enemyNum++;		// �}�b�v�J�E���g

				if (m_enemyNum == m_enemyMax)
				{
					pSound->PlaySoundA(CSound::SOUND_LABEL_SE006);
				}
			}
			else
			{// �Y���Ȃ�
				break;			// �����𔲂���
			}
		}
		else
		{// �Y���Ȃ�
			break;			// �����𔲂���
		}
	}
}