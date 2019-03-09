//=============================================================================
//
// �e���� [bullet.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "enemy.h"			// �G
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "camera.h"			// �J����
#include "bullet.h"			// �e
#include "player.h"			// �v���C���[
#include "effect.h"			// �G�t�F�N�g

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME_1		"data\\MODEL\\enemy000.x"	// ���f��1
#define MODEL_NAME_2		"data\\MODEL\\enemy100.x"	// ���f��2
#define MODEL_NAME_3		"data\\MODEL\\enemy200.x"	// ���f��3
#define MODEL_NAME_4		"data\\MODEL\\enemy300.x"	// ���f��4
#define MAX_LIFE_1			(4)							// �̗�1
#define MAX_LIFE_2			(6)							// �̗�2
#define MAX_LIFE_3			(7)							// �̗�3
#define MAX_LIFE_4			(600)						// �̗�4
#define MOVE_SPEED_UP		(3.0f)						// �ړ����x
#define MOVE_SPEED_DOWN		(0.3f)						// �ړ����x

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			*CEnemy::m_pTexture[ENEMYTYPE_MAX]	= {};	// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH					CEnemy::m_pMesh[ENEMYTYPE_MAX]		= {};	// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER				CEnemy::m_pBuffMat[ENEMYTYPE_MAX]	= {};	// �}�e���A�����ւ̃|�C���^
DWORD						CEnemy::m_nNumMat[ENEMYTYPE_MAX]	= {};	// �}�e���A�����̐�

//=============================================================================
// �R���X�g���N�^									(public)	*** CEnemy ***
//=============================================================================
CEnemy::CEnemy() : CModel3D(CScene::PRIORITY_3)
{
	// �����l�ݒ�
	m_enemyType = ENEMYTYPE_S_1;				// ���
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_nCntTimer = 0;							// ���ԃJ�E���^
	m_nLife = MAX_LIFE_1;						// �̗�

	m_nNumPattern = 0;
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CEnemy ***
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ���[�h����							(public)	*** CBullet ***
//=============================================================================
HRESULT CEnemy::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	char cName[255] = {};		// ���O�i�[

	for (int nCntModel = 0; nCntModel < ENEMYTYPE_MAX; nCntModel++)
	{// �J�E���g
		if (m_pBuffMat[nCntModel] == NULL && m_pMesh[nCntModel] == NULL)
		{// NULL�̏ꍇ
			switch (nCntModel)
			{// �J�E���g�ԍ�
			case ENEMYTYPE_S_1:
			case ENEMYTYPE_S_2:
			case ENEMYTYPE_B_2:
				wsprintf(cName, MODEL_NAME_1);
				
				break;
			case ENEMYTYPE_G_1:
			case ENEMYTYPE_G_2:
			case ENEMYTYPE_B_3:
				wsprintf(cName, MODEL_NAME_2);

				break;
			case ENEMYTYPE_O_1:
			case ENEMYTYPE_O_2:
				wsprintf(cName, MODEL_NAME_3);

				break;
			case ENEMYTYPE_B_1:
				wsprintf(cName, MODEL_NAME_4);

				break;
			}

			// X�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX
			(
				cName,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_pBuffMat[nCntModel],
				NULL,
				&m_nNumMat[nCntModel],
				&m_pMesh[nCntModel]
			);

			D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
			D3DMATERIAL9 *matDef;						// ���݂̃}�e���A���ۑ��p
			pmat = (D3DXMATERIAL*)m_pBuffMat[nCntModel]->GetBufferPointer();
			matDef = new D3DMATERIAL9[m_nNumMat[nCntModel]];
			m_pTexture[nCntModel] = new LPDIRECT3DTEXTURE9[m_nNumMat[nCntModel]];

			for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
			{
				matDef[tex] = pmat[tex].MatD3D;
				matDef[tex].Ambient = matDef[tex].Diffuse;
				m_pTexture[nCntModel][tex] = NULL;
				if (pmat[tex].pTextureFilename != NULL &&
					lstrlen(pmat[tex].pTextureFilename) > 0)
				{
					if (FAILED(D3DXCreateTextureFromFile(pDevice,
						pmat[tex].pTextureFilename,
						&m_pTexture[nCntModel][tex])))
					{
						MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
					}
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CBullet ***
//=============================================================================
void CEnemy::Unload(void)
{
	for (int nCntModel = 0; nCntModel < ENEMYTYPE_MAX; nCntModel++)
	{// �J�E���g
		// �e�N�X�`���̔j��
		for (DWORD tex = 0; tex < m_nNumMat[nCntModel]; tex++)
		{// �J�E���g
			if (m_pTexture[nCntModel][tex] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pTexture[nCntModel][tex]->Release();		// ���
				m_pTexture[nCntModel][tex] = NULL;			// NULL��
			}
		}

		// �}�e���A���̊J��
		if (m_pBuffMat[nCntModel] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pBuffMat[nCntModel]->Release();				// ���
			m_pBuffMat[nCntModel] = NULL;					// NULL��
		}

		// �}�e���A���̊J��
		if (m_pMesh[nCntModel] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pMesh[nCntModel]->Release();					// ���
			m_pMesh[nCntModel] = NULL;						// NULL��
		}

		m_nNumMat[nCntModel] = 0;							// �����l��
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CEnemy ***
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, ENEMYTYPE enemyType, int nShotTiming)
{
	CEnemy *pEnemy = NULL;					// �|�C���^

	if (pEnemy == NULL)
	{// NULL�̏ꍇ
		pEnemy = new CEnemy;				// �������m��
	}

	if (pEnemy != NULL)
	{// NULL�ȊO�̏ꍇ
		pEnemy->Init(pos, rot, move, enemyType, nShotTiming);		// ����������
		pEnemy->SetObjType(CScene::OBJTYPE_ENEMY);			// �I�u�W�F�N�g�^�C�v�ݒ�
	}

	return pEnemy;		// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CEnemy ***
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, ENEMYTYPE enemyType, int nShotTiming)
{
	CModel3D::SetInitAll(m_pTexture[enemyType], m_pMesh[enemyType], m_pBuffMat[enemyType], m_nNumMat[enemyType], pos, rot);		// �����l�ݒ�
	CModel3D::Init();																											// ����������

	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();		// �J���������_�擾

	m_pos = pos - Camerapos;		// �J�����ړ�������
	m_move = move;					// �ړ���
	m_enemyType = enemyType;		// ���
	m_nShotTiming = nShotTiming;	// ���˃^�C�~���O

	switch (enemyType)
	{
	case ENEMYTYPE_S_1:
		m_nLife = MAX_LIFE_1;
		break;
	case ENEMYTYPE_G_1:
		m_nLife = (int)(MAX_LIFE_1 * 1.4);
		break;
	case ENEMYTYPE_O_1:
		m_nLife = (int)(MAX_LIFE_1 * 1.8);
		break;
	case ENEMYTYPE_S_2:
		m_nLife = MAX_LIFE_2;
		break;
	case ENEMYTYPE_G_2:
		m_nLife = (int)(MAX_LIFE_2 * 1.2);
		break;
	case ENEMYTYPE_O_2:
		m_nLife = (int)(MAX_LIFE_2 * 1.4);
		break;
	case ENEMYTYPE_B_1:
		m_nLife = (int)(MAX_LIFE_4);
		m_nLife += (int)(m_nLife * (1.5 * CManager::GetPlayNum()));
		break;
	case ENEMYTYPE_B_2:
		m_nLife = (int)(MAX_LIFE_3 * 1.0);
		break;
	}

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Uninit(void)
{
	CModel3D::Uninit();				// �I������
}

//=============================================================================
// �X�V����											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Update(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();		// �J���������_�擾
	D3DXVECTOR3 pos = CModel3D::Getpos();	// �ʒu���擾
	D3DXVECTOR3 rot = CModel3D::Getrot();	// �������擾
	float frotplayer = 0.0f;				// �v���C���[����
	float frotDiff = 0.0f;					// ��������
	bool bPlayer = false;					// �v���C���[���邩

	// �J�����ɑ΂��Ă̈ʒu��ݒ�
	pos.x = Camerapos.x + m_pos.x;
	pos.z = Camerapos.z + m_pos.z;

	// ����`�F�b�N------------------------------------------------------------
	CModel3D *pModel3D;						// ���f��3D
	CScene::OBJTYPE objType;				// �I�u�W�F�N�g���
	CPlayer *pPlayer_1 = NULL;				// �v���C���[1
	CPlayer *pPlayer_2 = NULL;				// �v���C���[2
	D3DXVECTOR3 targetpos;					// ����p : ����̈ʒu
	int nCntPlayer = 0;

	for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
	{// �V�[���J�E���g
		pModel3D = (CModel3D*)CScene::GetScene(CScene::PRIORITY_3, nCntScene);	// �V�[���ǂݍ���

		if (pModel3D != NULL)
		{// NULL�ȊO�̏ꍇ
			objType = pModel3D->GetObjType();				// �I�u�W�F�N�g��ނ̎擾

			if (objType == CScene::OBJTYPE_PLAYER)
			{// ��ރ`�F�b�N
				CPlayer *pPlayer = (CPlayer*)pModel3D;

				if (pPlayer->GetPlayerNum() == 0)
				{
					pPlayer_1 = pPlayer;
				}
				else if (pPlayer->GetPlayerNum() == 1)
				{
					pPlayer_2 = pPlayer;
				}

				nCntPlayer++;

				bPlayer = true;

				if (nCntPlayer == CManager::GetPlayNum())
				{
					break;
				}
			}
		}
	}

	if (bPlayer == true)
	{// �v���C���[���邩
		D3DXVECTOR3 targetpos_1, targetpos_2;

		for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
		{
			if (nCnt == 0)
			{
				if (pPlayer_1 != NULL)
				{
					targetpos_1 = pPlayer_1->Getpos();;
				}
			}
			else if (nCnt == 1)
			{
				if (pPlayer_2 != NULL)
				{
					targetpos_2 = pPlayer_2->Getpos();
				}
			}
		}

		if (CManager::GetPlayNum() == MAX_PLAYER)
		{
			float diff_1, diff_2;

			diff_1 = sqrtf(((targetpos_1.x - pos.x) * (targetpos_1.x - pos.x)) + ((targetpos_1.z - pos.z) * (targetpos_1.z - pos.z)));
			diff_2 = sqrtf(((targetpos_2.x - pos.x) * (targetpos_2.x - pos.x)) + ((targetpos_2.z - pos.z) * (targetpos_2.z - pos.z)));
			
			if (diff_1 < diff_2)
			{
				targetpos = targetpos_1;
			}
			else
			{
				targetpos = targetpos_2;
			}
		}
		else
		{
			targetpos = targetpos_1;
		}
	}

	m_nCntTimer++;							// �J�E���g

	if (m_enemyType == ENEMYTYPE_S_1 || m_enemyType == ENEMYTYPE_S_2)
	{// ��1
		if (bPlayer == true)
		{// �v���C���[���邩
			if (m_enemyType == ENEMYTYPE_S_1)
			{
				frotplayer = atan2f(-targetpos.x - -pos.x, -targetpos.z - -pos.z);		// �����v�Z

				if (targetpos.z + 200.0f < pos.z)
				{// �g�p�͈�
					frotDiff = (frotplayer - rot.y);		// �����v�Z

					// ��������
					if (frotDiff < -D3DX_PI)
					{
						frotDiff += D3DX_PI * 2.0f;
					}
					if (D3DX_PI < frotDiff)
					{
						frotDiff -= D3DX_PI * 2.0f;
					}

					rot.y += frotDiff / 30.0f;				// �������Z
				}
			}

			if ((m_nCntTimer % m_nShotTiming) == 0)
			{// �p�^�[���Œe������
				CBullet::Create(pos, D3DXVECTOR3(sinf(rot.y) * -5.0f, 0.0f, cosf(rot.y) * -5.0f),
					D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), CBullet::BULLETTYPE_ENEMY, CBullet::BULLETSTATE_NORMAL);
			}
		}

		pos += D3DXVECTOR3(sinf(rot.y) * m_move.x, 0.0f, cosf(rot.y) * m_move.z);			// �ʒu���Z

		CEffect::Create(D3DXVECTOR3(pos.x + sinf(rot.y) * 20.0f, pos.y, pos.z + cosf(rot.y) * 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(0.0f, 0.0f, 3.0f), D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_ENEMY, CEffect::TEXTURE_EFFECT);	// �G�t�F�N�g
	}
	else if (m_enemyType == ENEMYTYPE_G_1 || m_enemyType == ENEMYTYPE_G_2)
	{// ��1
		if (bPlayer == true)
		{// �v���C���[���邩
			// �ʒu�v�Z
			D3DXVECTOR3 posOBJ = D3DXVECTOR3(
				Camerapos.x + (((pos.x) - (Camerapos.x)) * 0.6f),
				300.0f,
				Camerapos.z - 350.0f + (((pos.z) - (Camerapos.z - 250.0f)) * 0.6f));

			frotplayer = atan2f(-targetpos.x - -pos.x, -targetpos.z - -pos.z);		// �����v�Z

			frotDiff = (frotplayer - rot.y);		// �����v�Z

			// ��������
			if (frotDiff < -D3DX_PI)
			{
				frotDiff += D3DX_PI * 2.0f;
			}
			if (D3DX_PI < frotDiff)
			{
				frotDiff -= D3DX_PI * 2.0f;
			}

			if ((m_nCntTimer % m_nShotTiming) == 0)
			{// �p�^�[���Œe������
				CBullet::Create(posOBJ, D3DXVECTOR3(sinf(frotplayer) * -5.0f, 0.0f, cosf(frotplayer) * -5.0f),
					D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), CBullet::BULLETTYPE_ENEMY, CBullet::BULLETSTATE_NORMAL);
			}
		}

		if (m_enemyType == ENEMYTYPE_G_1)
		{
			pos += D3DXVECTOR3(m_move.x, 0.0f, m_move.z);		// �ʒu���Z
		}

		rot.y += frotDiff / 20.0f;					// �������Z

		// ��������
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		if (D3DX_PI < rot.y)
		{
			rot.y -= D3DX_PI * 2.0f;
		}

		if (m_enemyType == ENEMYTYPE_G_2)
		{
			if (-D3DX_PI * 0.5f < rot.y && rot.y < D3DX_PI * 0.5f)
			{
				pos += D3DXVECTOR3(sinf(rot.y) * m_move.x, 0.0f, cosf(rot.y) * m_move.z);			// �ʒu���Z
			}
			else
			{
				pos += D3DXVECTOR3(sinf(rot.y) * m_move.x, 0.0f, cosf(rot.y) * (m_move.z * -0.3f));
			}
		}

		if ((m_nCntTimer % 2) == 0)
		{// �p�^�[���Œe������
			CEffect::Create(D3DXVECTOR3(pos.x + sinf(rot.y) * 20.0f, pos.y, pos.z + cosf(rot.y) * 20.0f), D3DXVECTOR3(0.0f, rot.y, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, -2.8f), D3DXVECTOR3(EFFECT_SIZE_X * 2.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.02f, CEffect::EFFECTTYPE_ENEMY, CEffect::TEXTURE_TANK);	// �G�t�F�N�g
		}
	}
	else if (m_enemyType == ENEMYTYPE_O_1)
	{// �C1
		if (bPlayer == true)
		{// �v���C���[���邩
			// �ʒu�v�Z
			D3DXVECTOR3 posOBJ = D3DXVECTOR3(
				Camerapos.x + (((pos.x) - (Camerapos.x)) * 0.55f),
				300.0f,
				Camerapos.z - 350.0f + (((pos.z) - (Camerapos.z - 250.0f)) * 0.55f));

			frotplayer = atan2f(-targetpos.x - -posOBJ.x, -targetpos.z - -posOBJ.z);	// �����v�Z

			frotDiff = (frotplayer - rot.y);		// �����v�Z

			// ��������
			if (frotDiff < -D3DX_PI)
			{
				frotDiff += D3DX_PI * 2.0f;
			}
			if (D3DX_PI < frotDiff)
			{
				frotDiff -= D3DX_PI * 2.0f;
			}

			if (pos.z - Camerapos.z < (Camerapos.z + 120.0f))
			{
				if ((m_nCntTimer % m_nShotTiming) == 0)
				{// �p�^�[���Œe������
					CBullet::Create(posOBJ, D3DXVECTOR3(sinf(frotplayer) * -5.0f, 0.0f, cosf(frotplayer) * -5.0f),
						D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), CBullet::BULLETTYPE_ENEMY, CBullet::BULLETSTATE_NORMAL);
				}
			}

			if ((m_nCntTimer % 2) == 0)
			{// �p�^�[���Œe������
				CEffect::Create(D3DXVECTOR3(pos.x + sinf(rot.y) * 20.0f, pos.y, pos.z + cosf(rot.y) * 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, -2.0f), D3DXVECTOR3(EFFECT_SIZE_X * 2.0f, EFFECT_SIZE_Y * 2.0f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.02f, CEffect::EFFECTTYPE_ENEMY, CEffect::TEXTURE_WATER);	// �G�t�F�N�g
			}
		}

		pos += D3DXVECTOR3(m_move.x, 0.0f, m_move.z);
	}
	else if (m_enemyType == ENEMYTYPE_B_1)
	{
		if (bPlayer == true)
		{// �v���C���[���邩
			if (m_nNumPattern == 0)
			{
				if (Camerapos.z + 300.0f < pos.z)
				{
					pos += D3DXVECTOR3(m_move.x, 0.0f, m_move.z);
				}
				else
				{
					m_nNumPattern = 1;
					m_nCntTimer = 0;
				}

				if (CManager::GetPlayNum() == MAX_PLAYER)
				{
					if (m_nLife < (MAX_LIFE_4 * 1.5f) * 0.5f)
					{
						m_nNumPattern = 3;
						m_nCntTimer = 0;
					}
				}
				else
				{
					if (m_nLife < MAX_LIFE_4 * 0.5f)
					{
						m_nNumPattern = 3;
						m_nCntTimer = 0;
					}
				}
			}
			else if (m_nNumPattern == 1)
			{
				if (60 < m_nCntTimer)
				{
					if (pos.z < Camerapos.z + 300.0f)
					{
						pos.z += 2.0f;
					}
					else
					{
						if (pos.x <= Camerapos.x + 300.0f)
						{
							pos.x += 2.0f;
							
							if (Camerapos.x + 300.0f < pos.x)
							{
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z + 100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z + 30.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z - 40.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z - 110.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
							}
						}	

						if (m_nCntTimer == 510)
						{
							m_nNumPattern = 2;
							m_nCntTimer = 0;
						}

						if (CManager::GetPlayNum() == MAX_PLAYER)
						{
							if (m_nLife < (MAX_LIFE_4 * 1.5f) * 0.5f)
							{
								m_nNumPattern = 3;
								m_nCntTimer = 0;
							}
						}
						else
						{
							if (m_nLife < MAX_LIFE_4 * 0.5f)
							{
								m_nNumPattern = 3;
								m_nCntTimer = 0;
							}
						}
					}
				}
			}
			else if (m_nNumPattern == 2)
			{
				if (60 < m_nCntTimer)
				{
					if (pos.z < Camerapos.z + 300.0f)
					{
						pos.z += 2.0f;
					}
					else
					{
						if (Camerapos.x - 300.0f <= pos.x)
						{
							pos.x -= 2.0f;

							if (pos.x < Camerapos.x - 300.0f)
							{
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z + 100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z + 30.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z - 40.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
								CEnemy::Create(D3DXVECTOR3(pos.x - 30.0f, 40.0f, pos.z - 110.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 60);
							}
						}

						if (m_nCntTimer == 510)
						{
							m_nNumPattern = 1;
							m_nCntTimer = 0;
						}

						if (CManager::GetPlayNum() == MAX_PLAYER)
						{
							if (m_nLife < (MAX_LIFE_4 * 1.5f) * 0.5f)
							{
								m_nNumPattern = 3;
								m_nCntTimer = 0;
							}
						}
						else
						{
							if (m_nLife < MAX_LIFE_4 * 0.5f)
							{
								m_nNumPattern = 3;
								m_nCntTimer = 0;
							}
						}
					}
				}
			}
			else if (m_nNumPattern == 3)
			{
				if (m_nCntTimer < 60)
				{
					if (pos.x != 0.0f)
					{
						pos.x += ((0.0f - pos.x) / (60.0f - m_nCntTimer));

						if (pos.x < 1.0f && -1.0f < pos.x)
						{
							pos.x = 0.0f;

							CEnemy::Create(D3DXVECTOR3(pos.x - 22.0f, 18.0f, pos.z + 100.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_3, 90);
							CEnemy::Create(D3DXVECTOR3(pos.x - 22.0f, 18.0f, pos.z + 30.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_3, 90);
							CEnemy::Create(D3DXVECTOR3(pos.x - 22.0f, 18.0f, pos.z - 40.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_3, 90);
							CEnemy::Create(D3DXVECTOR3(pos.x - 22.0f, 18.0f, pos.z - 110.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_3, 90);
						}
					}
				}
				else
				{
					if (m_nCntTimer % 300 == 0)
					{
						CEnemy::Create(D3DXVECTOR3(pos.x, 40.0f, pos.z + 150.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 75);
						CEnemy::Create(D3DXVECTOR3(pos.x, 40.0f, pos.z + 90.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 75);
						CEnemy::Create(D3DXVECTOR3(pos.x, 40.0f, pos.z + 30.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), ENEMYTYPE_B_2, 75);
					}
				}
			}
		}

		if ((m_nCntTimer % 2) == 0)
		{// �p�^�[���Œe������
			CEffect::Create(D3DXVECTOR3(pos.x + sinf(rot.y) * 180.0f, pos.y, pos.z + cosf(rot.y) * -180.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, -5.0f), D3DXVECTOR3(EFFECT_SIZE_X * 2.0f, EFFECT_SIZE_Y * 2.0f, 0.0f), D3DXVECTOR3(1.5f, 1.5f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.02f, CEffect::EFFECTTYPE_ENEMY, CEffect::TEXTURE_WATER);	// �G�t�F�N�g
		}
	}
	else if (m_enemyType == ENEMYTYPE_B_2)
	{
		if (20 < m_nCntTimer)
		{
			if (m_nCntTimer < 100)
			{
				pos += D3DXVECTOR3(0.0f, 2.0f, 4.0f);			// �ʒu���Z
			}
			else 
			{
				if (m_nCntTimer == 100)
				{
					int nNum = rand() % 5;

					pos.x = -300.0f + (150.0f * nNum);
					pos.y = 300.0f;
					CModel3D::Setpos(pos);					// �ʒu��ݒ�
				}

				if (bPlayer == true)
				{// �v���C���[���邩			
					frotplayer = atan2f(-targetpos.x - -pos.x, -targetpos.z - -pos.z);		// �����v�Z

					if (targetpos.z + 200.0f < pos.z)
					{// �g�p�͈�
						frotDiff = (frotplayer - rot.y);		// �����v�Z

						// ��������
						if (frotDiff < -D3DX_PI)
						{
							frotDiff += D3DX_PI * 2.0f;
						}
						if (D3DX_PI < frotDiff)
						{
							frotDiff -= D3DX_PI * 2.0f;
						}

						rot.y += frotDiff / 30.0f;				// �������Z
					}

					if ((m_nCntTimer % m_nShotTiming) == 0)
					{// �p�^�[���Œe������
						CBullet::Create(pos, D3DXVECTOR3(sinf(rot.y) * -5.0f, 0.0f, cosf(rot.y) * -5.0f),
							D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), CBullet::BULLETTYPE_ENEMY, CBullet::BULLETSTATE_NORMAL);
					}
				}

				pos += D3DXVECTOR3(sinf(rot.y) * m_move.x, 0.0f, cosf(rot.y) * m_move.z);			// �ʒu���Z

				CEffect::Create(D3DXVECTOR3(pos.x + sinf(rot.y) * 20.0f, pos.y, pos.z + cosf(rot.y) * 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(0.0f, 0.0f, 3.0f), D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_ENEMY, CEffect::TEXTURE_EFFECT);	// �G�t�F�N�g
			}
		}
	}
	else if (m_enemyType == ENEMYTYPE_B_3)
	{
		if (bPlayer == true)
		{// �v���C���[���邩
		 // �ʒu�v�Z
			D3DXVECTOR3 posOBJ = D3DXVECTOR3(
				Camerapos.x + (((pos.x) - (Camerapos.x)) * 0.6f),
				300.0f,
				Camerapos.z - 350.0f + (((pos.z) - (Camerapos.z - 250.0f)) * 0.6f));

			frotplayer = atan2f(-targetpos.x - -pos.x, -targetpos.z - -pos.z);		// �����v�Z

			frotDiff = (frotplayer - rot.y);		// �����v�Z

			// ��������
			if (frotDiff < -D3DX_PI)
			{
				frotDiff += D3DX_PI * 2.0f;
			}
			if (D3DX_PI < frotDiff)
			{
				frotDiff -= D3DX_PI * 2.0f;
			}

			rot.y += frotDiff / 20.0f;					// �������Z

			// ��������
			if (rot.y < -D3DX_PI)
			{
				rot.y += D3DX_PI * 2.0f;
			}
			if (D3DX_PI < rot.y)
			{
				rot.y -= D3DX_PI * 2.0f;
			}

			if ((m_nCntTimer % m_nShotTiming) == 0)
			{// �p�^�[���Œe������
				CBullet::Create(posOBJ, D3DXVECTOR3(sinf(frotplayer) * -5.0f, 0.0f, cosf(frotplayer) * -5.0f),
					D3DXVECTOR3(BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), CBullet::BULLETTYPE_ENEMY, CBullet::BULLETSTATE_NORMAL);
			}
		}
	}

	// ��������
	if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2.0f;
	}
	if (D3DX_PI < rot.y)
	{
		rot.y -= D3DX_PI * 2.0f;
	}

	CModel3D::Setpos(pos);					// �ʒu��ݒ�
	CModel3D::Setrot(rot);					// ������ݒ�

	m_pos = pos - Camerapos;				// ���݈ʒu���v�Z

	if (Camerapos.z - 450.0f > pos.z)
	{// �J�����͈͊O�ɂł���
		Uninit();							// �I������
	}
}

//=============================================================================
// �`�揈��											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Draw(void)
{
	CModel3D::Draw();						// �`�揈��
}