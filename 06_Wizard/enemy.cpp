//=============================================================================
//
// �G���� [enemy.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "enemy.h"			// �G
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "sound.h"			// �T�E���h
#include "input.h"			// ����
#include "text.h"			// �e�L�X�g
#include "camera.h"			// �J����
#include "fade.h"			// �t�F�[�h

#include "scenemesh.h"		// �V�[�����b�V��
#include "game.h"			// �Q�[��
#include "model.h"			// ���f��
#include "motion.h"			// ���[�V����
#include "mapicon.h"		// �}�b�v�A�C�R��
#include "map.h"			// �}�b�v
#include "texture.h"		// �e�N�X�`��
#include "texanim.h"		// �e�N�X�`���A�j���[�V����
#include "gauge.h"			// �Q�[�W
#include "player.h"			// �v���C���[
#include "shadow.h"			// �e
#include "effect.h"			// �G�t�F�N�g

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MC_MOVE_UP		(1.5f)								// ����
#define MC_MOVE_DOWN	(0.75f)								// ����
#define MC_JUMP			(4.0f)								// �W�����v��
#define MC_GRAVITY		(0.3f)								// �d��
#define MC_DRAW_RANGE	(80.0f)								// �`�拗��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
// �R���X�g���N�^										(public)	*** CEnemy ***
//=============================================================================
CEnemy::CEnemy() : CScene(CScene::PRIORITY_3, CScene::OBJTYPE_ENEMY)
{
	// �����l�ݒ�
	m_MapIcon		= NULL;									// �}�b�v�A�C�R��

	m_bUpdata		= true;									// �X�V����
	m_bMove			= true;									// �ړ��ł���

	m_nHP			= 0;									// HP
	m_nMAXHP		= 0;									// MAXHP

	m_state			= STATE_NORMAL;							// ���
	m_nCntState		= 0;									// ��ԃJ�E���g
	m_vtxMax		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����MAX
	m_vtxMin		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����MIN
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu�O��
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_move			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړ���
	m_frotDest		= 0.0f;									// �ڕW����
	m_bJump			= false;								// �W�����v

	m_shadowHeight = 0.0f;									// �e����
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CEnemy ***
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ���[�h����										(public)	*** CEnemy ***
//=============================================================================
HRESULT CEnemy::Load(void)
{
	CSlime::Load();			// �X���C��
	CSlime_King::Load();	// �X���C��

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Unload(void)
{
	CSlime::Unload();			// �X���C��
	CSlime_King::Unload();		// �X���C���L���O
}

//=============================================================================
// �N���G�C�g										(public)	*** CEnemy ***
//=============================================================================
CEnemy *CEnemy::Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = NULL;		// �|�C���^

	if (pEnemy == NULL)
	{// NULL�̏ꍇ
		if (type == TYPE_N_SLIME)		// �X���C��
			pEnemy = new CSlime;			// �������m��
		if (type == TYPE_B_SLIME)		// �X���C���L���O
			pEnemy = new CSlime_King;		// �������m��
	}

	if (pEnemy != NULL)
	{// NULL�ȊO�̏ꍇ
		pEnemy->m_nType		= type;		// �^�C�v�ԍ�
		pEnemy->m_pos		= pos;		// �ʒu
		pEnemy->m_rot		= rot;		// ����
		pEnemy->m_frotDest	= rot.y;	// �ڕW����
		pEnemy->Init();					// ����������
	}

	return pEnemy;		// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CEnemy ***
//=============================================================================
HRESULT CEnemy::Init()
{
	// �}�b�v�A�C�R��
	if (m_MapIcon == NULL)
	{// NULL�̏ꍇ
		m_MapIcon = CMapIcon::Create(PRIORITY_MAP_1, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), m_rot, D3DXVECTOR3(70.0f, 0.0f, 70.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_MapIcon->SetTex(2, 1, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	}

	CGame *pGame = CManager::GetGame();
	if (pGame != NULL)
	{
		pGame->SetEnemyNum(pGame->GetEnemyNum() + 1);
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CEnemy ***
//=============================================================================
void CEnemy::Uninit(void)
{
	// �}�b�v�A�C�R��
	if (m_MapIcon != NULL)
	{// NULL�ȊO�̏ꍇ
		m_MapIcon->Uninit();	// �j��
		m_MapIcon = NULL;		// NULL��
	}

	CGame *pGame = CManager::GetGame();
	if (pGame != NULL)
	{
		CGame::GAME game = pGame->GetGame();
		if (game != CGame::GAME_TUTORIAL && game != CGame::GAME_CLEAR && game != CGame::GAME_OVER)
		{
			pGame->AddDeathEnemy();
		}
		pGame->SetEnemyNum(pGame->GetEnemyNum() - 1);

		CPlayer *pPlayer = pGame->GetPlayer();
		if (pPlayer != NULL)
		{
			pPlayer->AddMP(4);
		}
	}

	CScene::SetDeath();		// ���S�t���O
}

//=============================================================================
// �q�b�g�_���[�W����									(public)	*** CEnemy ***
//=============================================================================
bool CEnemy::HitDamage(int nDamage)
{
	bool bLand = false;

	if (m_state == STATE_NORMAL)
	{
		m_state = STATE_DAMAGE;
		m_nHP += nDamage;	// �_���[�W���
		bLand = true;

		if (m_nHP < 0)
			m_nHP = 0;		// HP����
	}

	return bLand;
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int						CSlime::m_MaxModel = 0;		// ���f����
int						CSlime::m_MaxMotion = 0;		// ���[�V������

LPDIRECT3DTEXTURE9		**CSlime::m_pTexture = NULL;		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				*CSlime::m_pMesh = NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			*CSlime::m_pBuffMat = NULL;		// �}�e���A�����ւ̃|�C���^
DWORD					*CSlime::m_nNumMat = NULL;		// �}�e���A�����̐�

CModel::MODEL_INFO		*CSlime::m_ModelInfo = NULL;		// ���f���C���t�H
CMotion::MOTION_INFO	*CSlime::m_MotionInfo = NULL;		// ���[�V�����C���t�H

//=============================================================================
// �R���X�g���N�^										(public)	*** CSlime ***
//=============================================================================
CSlime::CSlime() : CEnemy()
{
	m_pModel = NULL;									// ���f��
	m_pMotion = NULL;									// ���[�V����

	m_AiType = AI_NORMAL;								// AI�^�C�v
	m_nMoveRecast = 0;									// �ړ����L���X�g
	m_bAttack = false;									// �U�����Ă���
	m_nCntAttack = 0;									// �U���J�E���g

	m_bAutoMove	= false;								// �����ړ����Ă��邩
	m_nCntAutoMove = 0;									// �����ړ��J�E���g
	m_nMaxAutoMove = 0;									// �����ړ��J�E���g�ő�
	m_nAutoMoveRecast = 0;								// �����ړ����L���X�g

	m_bKnockBack = false;								// �m�b�N�o�b�N
	m_nCntKnockBack = 0;								// �m�b�N�o�b�N�J�E���g

	m_bLockOn = false;									// ���b�N�I��
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CSlime ***
//=============================================================================
CSlime::~CSlime()
{

}

//=============================================================================
// ���[�h����										(public)	*** CSlime ***
//=============================================================================
HRESULT CSlime::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MODEL\\slimemodel.txt", &m_MaxModel);		// ���f���擾
	//m_MotionInfo	= CText::GetMotion(0);					// ���[�V�����擾 

	// �e���̃������m��
	if (m_pTexture	== NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_MaxModel];
	if (m_pMesh		== NULL) m_pMesh	= new LPD3DXMESH[m_MaxModel];
	if (m_pBuffMat	== NULL) m_pBuffMat = new LPD3DXBUFFER[m_MaxModel];
	if (m_nNumMat	== NULL) m_nNumMat	= new DWORD[m_MaxModel];

	// �e���̏�����
	for (int nCount = 0; nCount < m_MaxModel; nCount++)		// ���f�����J�E���g
	{	m_pTexture[nCount] = NULL;		m_pMesh[nCount] = NULL;		m_pBuffMat[nCount] = NULL;		m_nNumMat[nCount] = NULL;	}

	for (int nCntParnet = 0; nCntParnet < m_MaxModel; nCntParnet++)
	{// ���f�����J�E���g
		// X�t�@�C���̓ǂݍ���
		CText::GetXModelData(m_ModelInfo[nCntParnet].cFileName, &m_pTexture[nCntParnet], &m_pMesh[nCntParnet], &m_pBuffMat[nCntParnet], &m_nNumMat[nCntParnet]);
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CSlime ***
//=============================================================================
void CSlime::Unload(void)
{
	// �e���̃������J���ANULL��
	if (m_pTexture	!= NULL) { delete[] m_pTexture;	m_pTexture	= NULL; }
	if (m_pMesh		!= NULL) { delete[] m_pMesh;	m_pMesh		= NULL; }
	if (m_pBuffMat	!= NULL) { delete[] m_pBuffMat;	m_pBuffMat	= NULL; }
	if (m_nNumMat	!= NULL) { delete[] m_nNumMat;	m_nNumMat	= NULL; }

	// ���f���C���t�H
	if (m_ModelInfo	 != NULL)	m_ModelInfo = NULL;		// NULL��

	// ���[�V�����C���t�H
	if (m_MotionInfo != NULL)	m_MotionInfo = NULL;	// NULL��
}

//=============================================================================
// ����������										(public)	*** CSlime ***
//=============================================================================
HRESULT CSlime::Init(void)
{
	CEnemy::Init();		// ����������

	// ���f��
	if (m_pModel == NULL && m_ModelInfo != NULL)
	{// NULL�̏ꍇ
		m_pModel = new CModel[m_MaxModel];	// �������m��

		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// ���f�����J�E���g
		 // �����l�ݒ�
			m_pModel[nCntModel].SetInitAll(m_pTexture[nCntModel], m_pMesh[nCntModel], m_pBuffMat[nCntModel], m_nNumMat[nCntModel], m_ModelInfo[nCntModel].pos, m_ModelInfo[nCntModel].rot);
			// ����������
			m_pModel[nCntModel].Init();

			// �e���f����ݒ�
			if (0 <= m_ModelInfo[nCntModel].nIndex)		// �e�ȊO
				m_pModel[nCntModel].SetParent(&m_pModel[m_ModelInfo[nCntModel].nIndex]);
		}
	}

	// ���[�V����
	if (m_pMotion == NULL && m_MotionInfo != NULL)
	{// NULL�̏ꍇ
		m_pMotion = new CMotion;	// �������m��

		m_pMotion->SetMotionNum(0);	// �ҋ@���[�V����
	}

	m_nHP = m_MAX_HP;	// �̗͐ݒ�
	m_nMAXHP = m_MAX_HP;	// �̗͐ݒ�

	// ����ݒ�
	m_vtxMax = D3DXVECTOR3(14.0f, 20.0f, 14.0f);
	m_vtxMin = D3DXVECTOR3(-14.0f, 0.0f, -14.0f);

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CSlime ***
//=============================================================================
void CSlime::Uninit(void)
{
	CEffect::Create(CEffect::TYPE_CREATE, CEffect::SHOT_NONE, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	CCameraMain *pCameraMain = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	if (pCameraMain->GetLockOn() == true)
	{// ���b�N�I�����Ă���
		if (pCameraMain->GetLockOnEnemy() == this)
		{// ������������
			pCameraMain->SetLockOn(false);		// ���b�N�I�����Ȃ�
		}
	}

	// ���f��
	if (m_pModel != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++) // ���f�����J�E���g
			m_pModel[nCntModel].Uninit();	// �I������

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

	CEnemy::Uninit();	// �I������
}

//=============================================================================
// �X�V����											(public)	*** CSlime ***
//=============================================================================
void CSlime::Update(void)
{
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);

	State();	// ��ԏ���

	m_posOld = m_pos;				// �ʒu�ۑ�

	if (m_bKnockBack == false)
	{
		if (pCamera->GetPickUp() == false)
			Ai();					// Ai
	}
	else
	{
		if (m_nCntKnockBack < 30)
		{
			m_nCntKnockBack++;
		}
		else
		{
			m_bKnockBack = false;
			m_nCntKnockBack = 0;
		}
	}

	m_pos += m_move;				// �ړ��ʉ��Z

	CScene *pScene		= NULL;		// �V�[��
	CScene *pSceneNext	= NULL;		// ���V�[��

	// �G
	{
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
				CEnemy *pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
				if (pEnemy != this)
				{
					pEnemy->Collision(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin, true);
				}
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	float fHeight = -500.0f;		// �n�ʍ���

	ColisionChack(&fHeight);		// ����`�F�b�N����
	m_shadowHeight = fHeight;		// �e�����ݒ�

	// �n�ʒ���
	{
		if (m_pos.y < fHeight)
		{// �w�荂������
			m_pos.y = fHeight;		// �����ݒ�
			m_move.y = 0.0f;		// �ړ��ʁiY�j��������

			if (m_nMoveRecast == 0)
				m_bJump = false;		// �W�����v���Ȃ�
			else		
				m_nMoveRecast--;
		}
	}

	// �p�x����
	{
		// �ڕW�p�x����
		if (m_frotDest < -D3DX_PI)
			m_frotDest += D3DX_PI * 2.0f;
		if (D3DX_PI < m_frotDest)
			m_frotDest -= D3DX_PI * 2.0f;

		// �ڕW�p�x�܂ł̍��������߂�
		float rotDiff = (m_frotDest - m_rot.y);

		// ��������
		if (rotDiff < -D3DX_PI)
			rotDiff += D3DX_PI * 2.0f;
		if (D3DX_PI < rotDiff)
			rotDiff -= D3DX_PI * 2.0f;

		float fRatio = 0.0f;		// ����
		fRatio = 5.0;

		// �p�x�ݒ�
		m_rot.y += ((rotDiff) / fRatio);

		// �p�x����
		if (m_rot.y < -D3DX_PI)
			m_rot.y += D3DX_PI * 2.0f;
		if (D3DX_PI < m_rot.y)
			m_rot.y -= D3DX_PI * 2.0f;
	}

	// �ړ��ʒ���
	{
		m_move.x -= (m_move.x - 0.0f) * MC_MOVE_DOWN;		// �ړ��ʁiX�j������
		m_move.z -= (m_move.z - 0.0f) * MC_MOVE_DOWN;		// �ړ��ʁiZ�j������

		// �d�͉��Z
		m_move.y -= MC_GRAVITY;
	}

	// �}�b�v�A�C�R��
	if (m_MapIcon != NULL)
	{// NULL�ȊO�̏ꍇ
		m_MapIcon->Setpos(m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f));
		m_MapIcon->Setrot(m_rot);
	}

	// ���[�V�����X�V
	{
		if (m_pMotion != NULL)	// NULL�ȊO�̏ꍇ
			m_pMotion->Update(m_MaxModel, &m_MotionInfo[0], &m_pModel[0]);
	}
}

//=============================================================================
// �`�揈��											(public)	*** CSlime ***
//=============================================================================
void CSlime::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
	{// ���f�����J�E���g
		if (m_pModel != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pModel[nCntModel].Draw();

			// �e�\��
			CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(m_pos.x, m_shadowHeight + 0.1f, m_pos.z));
		}
	}
}

//=============================================================================
// �Փ˔��菈��											(public)	*** CSlime ***
//=============================================================================
bool CSlime::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin, bool bSet)
{
	CCameraMain	*pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);	// �J�����擾
	bool		bLand = false;	// ����������

	// �����v�Z
	float fLenght = sqrtf(powf(m_vtxMax.x, 2.0f) + powf(m_vtxMax.z, 2.0f));

	if (sqrtf(powf(m_pos.x - pPos->x, 2.0f) + powf(m_pos.z - pPos->z, 2.0f)) < fLenght)
	{// ��苗���ȉ���������
		if (pCamera->GetLockOn() == true || pCamera->GetLockOn() == false && (m_pos.y + m_vtxMin.y - 5.0f < pPos->y && pPos->y < m_pos.y + m_vtxMax.y) || m_bAttack == true || bSet == true)
		{// ��������
			// ��]�v�Z
			float frot = atan2f(m_pos.x - pPos->x, m_pos.z - pPos->z) + D3DX_PI;

			if (pMove != NULL)
				*pMove = D3DXVECTOR3(sinf(frot) * 8.0f, pMove->y, cosf(frot) * 8.0f);	// �����Ԃ�

			if (sqrtf(powf(m_pos.x - pPos->x, 2.0f) + powf(m_pos.z - pPos->z, 2.0f)) < (fLenght * 0.5f))
			{// ��苗���ȉ���������
				if (pPos != NULL)
					*pPos = D3DXVECTOR3(m_pos.x + sinf(frot) * (fLenght * 0.5f), pPos->y, m_pos.z + cosf(frot) * (fLenght * 0.5f));		// �~�߂�
			}

			bLand = true;	// ��������
		}
	}

	return bLand;	// �l��Ԃ�
}

//=============================================================================
// ����`�F�b�N����									(public)	*** CSlime ***
//=============================================================================
void CSlime::ColisionChack(float *fHeight)
{
	CScene *pScene = NULL;		// �V�[��
	CScene *pSceneNext = NULL;		// ���V�[��

	// �t�B�[���h
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();			// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// �I�u�W�F�N�g�^�C�v�����b�V���t�B�[���h�̏ꍇ
				CSceneMesh *pMesh = (CSceneMesh*)pScene;	// �V�[�����b�V���̌^�ɃL���X�g

				if (pMesh->ColInMesh(m_pos) == true)
				{// �͈̓`�F�b�N
					*fHeight = pMesh->GetHeight(m_pos);		// �����擾

					break;
				}
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	CManager::MODE mode = CManager::GetMode();

	// �}�b�v
	if (mode == CManager::MODE_GAME)
	{// �Q�[���̂�
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			CMap *pMap = pGame->GetMap();
			if (pMap != NULL)
			{
				int nMapNum = pGame->GetMapNum();
				if (nMapNum == 2)
				{
					CMap_02 *pMap_02 = (CMap_02*)pMap;

					if (pMap_02->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f) == true)
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
				else if (nMapNum == 3)
				{
					CMap_03 *pMap_03 = (CMap_03*)pMap;

					if (pMap_03->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f))
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
				else if (nMapNum == 4)
				{
					CMap_04 *pMap_04 = (CMap_04*)pMap;

					if (pMap_04->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f))
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
			}
		}
	}

	CGame *pGame = CManager::GetGame();

	if (mode == CManager::MODE_GAME && pGame != NULL)
	{
		CPlayer *pPlayer = pGame->GetPlayer();
		if (pPlayer != NULL)
		{
			if (m_bAttack == true)
			{
				// �ʒu�擾
				D3DXVECTOR3 pos = pPlayer->Getpos();
				if (pos.y < m_vtxMax.y * 1.2f)
				{
					// �����v�Z
					float fLenght = sqrtf(powf(m_vtxMax.x, 2.0f) + powf(m_vtxMax.z, 2.0f));

					if (sqrtf(powf(m_pos.x - pos.x, 2.0f) + powf(m_pos.z - pos.z, 2.0f)) < fLenght)
					{// ��苗���ȉ���������
						pPlayer->AddHP(-10);
					}
				}
			}
		}
	}
}

//=============================================================================
// ��ԏ���											(public)	*** CSlime ***
//=============================================================================
void CSlime::State(void)
{
	float col_a = 1.0f;	// �F�ݒ�p

	// ��ԏ���	
	{
		switch (m_state)
		{// ���
		case STATE_NORMAL:		// �ʏ�
			break;
		case STATE_DAMAGE:		// �_���[�W
			m_nCntState++;		// ��ԃJ�E���g

			// �_��
			if (m_nCntState / 5 % 2 != 1)
				col_a = 0.0f;	// ����
			else
				col_a = 1.0f;	// �s����

			if (m_nCntState == 15)
			{// ���̃J�E���g�ɂȂ�����
				m_nCntState = 0;									// ��ԃJ�E���g������
				m_state		= STATE_NORMAL;							// ��Ԃ�ʏ��
				col_a		= 1.0f;									// �F��������
			}

			for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
				m_pModel[nCntModel].Setcol_a(col_a);				// �F�ݒ�

			break;
		case STATE_DEATH:		// ����
			break;
		}
	}

	// �J�����擾
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	D3DXVECTOR3 Camerapos = pCamera->Getpos(0);

	// �����v�Z
	float fLenght = sqrtf(powf(m_pos.x - Camerapos.x, 2.0f) + powf(m_pos.z - Camerapos.z, 2.0f));

	// �����ɂ���ē��߂���
	if (fLenght < MC_DRAW_RANGE)
		for (int nCount = 0; nCount < MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(fLenght / MC_DRAW_RANGE);
	else
		for (int nCount = 0; nCount< MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(col_a);
}

//=============================================================================
// �ړ�����											(public)	*** CSlime ***
//=============================================================================
void CSlime::Move(void)
{

}

//=============================================================================
// Ai����											(public)	*** CSlime ***
//=============================================================================
void CSlime::Ai(void)
{
	CSound *pSound = CManager::GetSound();		// �T�E���h�擾

	CManager::MODE mode = CManager::GetMode();	// ���[�h�擾
	CGame *pGame = CManager::GetGame();			// �Q�[���擾

	if (mode == CManager::MODE_GAME && pGame != NULL)
	{// ���[�h���Q�[���������� + NULL�ȊO�̏ꍇ
		CPlayer *pPlayer = pGame->GetPlayer();			// �v���C���[�擾
		if (pPlayer != NULL)
		{// NULL�ȊO�̏ꍇ
			D3DXVECTOR3 pos		= pPlayer->Getpos();		// �ʒu�擾
			D3DXVECTOR3 size	= pPlayer->GetvtxMax();		// �傫���擾

			// �����Ɗp�x���v�Z
			float fRange = sqrtf(powf(m_pos.x - pos.x, 2.0f) + powf(m_pos.z - pos.z, 2.0f));
			float fAngle = atan2f(pos.x - m_pos.x, pos.z - m_pos.z);
		
			// AI
			if (m_AiType == AI_NORMAL)
			{// �ʏ�
				bool bAttack = false;	// �U��

				if (m_bAttack == false)
				{// �U�����Ă��Ȃ�
					if (fRange < 60.0f)
					{// �͈͓���������i�U���j
						m_frotDest = fAngle;
						m_move.x = 0.0f;
						m_move.z = 0.0f;

						// �U��
						bAttack = true;
						m_bLockOn = false;

						if (m_bAutoMove == true)
						{// �����ړ�����������
							// �����ړ����L�����Z��
							m_bAutoMove			= false;
							m_nCntAutoMove		= 0;
							m_nMaxAutoMove		= 0;
							m_nAutoMoveRecast	= 0;
						}
					}
					else if (fRange < 400.0f || m_bLockOn == true)
					{// �͈͓���������i�U�������j�A���b�N�I�����Ă���
						m_frotDest = fAngle;

						if (m_nMoveRecast == 20)
						{// �ړ����n�߂�
							m_move.x = sinf(fAngle) * MC_MOVE_UP;
							m_move.z = cosf(fAngle) * MC_MOVE_UP;
						}

						if (m_bJump == false)
						{// �W�����v���Ă��Ȃ�
							// �W�����v���Ĉړ��J�n
							m_move.y += MC_JUMP;
							m_bJump			= true;
							m_nMoveRecast	= 20;
						}

						if (m_bAutoMove == true)
						{// �����ړ�����������
							// �����ړ����L�����Z��
							m_bAutoMove			= false;
							m_nCntAutoMove		= 0;
							m_nMaxAutoMove		= 0;
							m_nAutoMoveRecast	= 0;
						}
					}
					else
					{// ���̑��i�͈͊O�j
						if (m_nAutoMoveRecast == 0)
						{// �����ړ����L���X�g���O��������
							if (m_bAutoMove == false)
							{// �����ړ����Ă��Ȃ�
								// �����ړ�����
								m_bAutoMove = true;
								m_nCntAutoMove = 0;
								m_nMaxAutoMove = rand() % 120 + 90;

								m_frotDest = (float)(rand() % (int)(D3DX_PI * 2.0f * 100.0f)) * 0.01f;
							}
							else
							{// �����ړ����Ă���
								if (m_nMoveRecast == 20)
								{// �ړ����n�߂�
									m_move.x = sinf(m_frotDest) * MC_MOVE_UP;
									m_move.z = cosf(m_frotDest) * MC_MOVE_UP;
								}

								if (m_bJump == false)
								{// �W�����v���Ă��Ȃ�
									// �W�����v���Ĉړ��J�n
									m_move.y += MC_JUMP;
									m_bJump = true;
									m_nMoveRecast = 20;
								}

								m_nCntAutoMove++;		// �����ړ��J�E���g

								if (m_nCntAutoMove == m_nMaxAutoMove)
								{// �J�E���g���ő�Ɠ����ɂȂ�����
									// �����ړ����L�����Z��
									m_bAutoMove = false;
									m_nCntAutoMove = 0;
									m_nMaxAutoMove = 0;
									m_nAutoMoveRecast = 90;
								}
							}
						}
						else
						{// �����ړ����L���X�g���O�ȊO
							if (0 < m_nAutoMoveRecast)	// �O�ȏゾ������
								m_nAutoMoveRecast--;	// ���炷
						}
					}
				}
				else
				{// �U������
					if (fRange < 80.0f)
					{// �͈͓���������
						bAttack = true;		// �U������
					}
					else
					{// �͈͊O��������
						m_bAttack = false;	// �U�����Ȃ�
					}
				}

				if (bAttack == true)
				{// �U������
					if (m_bJump == false)
					{// �W�����v���Ă��Ȃ�
						if (00 < m_nCntAttack && m_nCntAttack < 38)
						{// ���t���[���̊Ԃ�������
							m_bAttack = true;		// �U������
						}
						else
						{// ���̑�
							m_bAttack = false;		// �U�����Ȃ�
						}

						if (0 < m_nCntAttack && m_nCntAttack < 30)
						{// ���t���[���̊Ԃ�������
							m_move.x = sinf(fAngle) * -0.2f;
							m_move.z = cosf(fAngle) * -0.2f;
						}
						else if (30 < m_nCntAttack && m_nCntAttack < 35)
						{// ���t���[���̊Ԃ�������
							m_move.x = sinf(fAngle) * (fRange / 3);
							m_move.z = cosf(fAngle) * (fRange / 3);

							if (m_nCntAttack == 31)
							{// ���t���[����������
								m_move.y += MC_JUMP;

								if (pSound != NULL)
								{// NULL�ȊO�̏ꍇ
									pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMY_ATTACK000);
								}
							}

							CEffect::Create(CEffect::TYPE_C00, CEffect::SHOT_NONE, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
						else if (35 < m_nCntAttack && m_nCntAttack < 38)
						{// ���t���[���̊Ԃ�������
							m_move.x = sinf(fAngle) * -(fRange / 3);
							m_move.z = cosf(fAngle) * -(fRange / 3);
						}
						m_nCntAttack++;

						int nNum = rand() % 120 + 150;
						if (nNum < m_nCntAttack)
							m_nCntAttack = 0;
					}
				}
				else
				{// �U�����Ă��Ȃ�
					m_nCntAttack = 0;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int						CSlime_King::m_MaxModel		= 0;		// ���f����
int						CSlime_King::m_MaxMotion	= 0;		// ���[�V������

LPDIRECT3DTEXTURE9		**CSlime_King::m_pTexture	= NULL;		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				*CSlime_King::m_pMesh		= NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			*CSlime_King::m_pBuffMat	= NULL;		// �}�e���A�����ւ̃|�C���^
DWORD					*CSlime_King::m_nNumMat		= NULL;		// �}�e���A�����̐�

CModel::MODEL_INFO		*CSlime_King::m_ModelInfo	= NULL;		// ���f���C���t�H
CMotion::MOTION_INFO	*CSlime_King::m_MotionInfo	= NULL;		// ���[�V�����C���t�H

//=============================================================================
// �R���X�g���N�^										(public)	*** CSlime_King ***
//=============================================================================
CSlime_King::CSlime_King() : CEnemy()
{
	m_pModel = NULL;									// ���f��
	m_pMotion = NULL;									// ���[�V����

	m_AiType = AI_NORMAL;								// AI�^�C�v
	m_nMoveRecast = 0;									// �ړ����L���X�g
	m_bAttack = false;									// �U�����Ă���
	m_nCntAttack = 0;									// �U���J�E���g

	m_bAutoMove = false;								// �����ړ����Ă��邩
	m_nCntAutoMove = 0;									// �����ړ��J�E���g
	m_nMaxAutoMove = 0;									// �����ړ��J�E���g�ő�
	m_nAutoMoveRecast = 0;								// �����ړ����L���X�g
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CSlime_King ***
//=============================================================================
CSlime_King::~CSlime_King()
{

}

//=============================================================================
// ���[�h����										(public)	*** CSlime_King ***
//=============================================================================
HRESULT CSlime_King::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	m_ModelInfo = CText::GetModel("data\\TEXT\\MODEL\\slimekingmodel.txt", &m_MaxModel);		// ���f���擾
	//m_MotionInfo	= CText::GetMotion(0);					// ���[�V�����擾 

	// �e���̃������m��
	if (m_pTexture == NULL) m_pTexture = new LPDIRECT3DTEXTURE9*[m_MaxModel];
	if (m_pMesh == NULL) m_pMesh = new LPD3DXMESH[m_MaxModel];
	if (m_pBuffMat == NULL) m_pBuffMat = new LPD3DXBUFFER[m_MaxModel];
	if (m_nNumMat == NULL) m_nNumMat = new DWORD[m_MaxModel];

	// �e���̏�����
	for (int nCount = 0; nCount < m_MaxModel; nCount++)		// ���f�����J�E���g
	{
		m_pTexture[nCount] = NULL;		m_pMesh[nCount] = NULL;		m_pBuffMat[nCount] = NULL;		m_nNumMat[nCount] = NULL;
	}

	for (int nCntParnet = 0; nCntParnet < m_MaxModel; nCntParnet++)
	{// ���f�����J�E���g
	 // X�t�@�C���̓ǂݍ���
		CText::GetXModelData(m_ModelInfo[nCntParnet].cFileName, &m_pTexture[nCntParnet], &m_pMesh[nCntParnet], &m_pBuffMat[nCntParnet], &m_nNumMat[nCntParnet]);
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Unload(void)
{
	// �e���̃������J���ANULL��
	if (m_pTexture != NULL) { delete[] m_pTexture;	m_pTexture = NULL; }
	if (m_pMesh != NULL) { delete[] m_pMesh;	m_pMesh = NULL; }
	if (m_pBuffMat != NULL) { delete[] m_pBuffMat;	m_pBuffMat = NULL; }
	if (m_nNumMat != NULL) { delete[] m_nNumMat;	m_nNumMat = NULL; }

	// ���f���C���t�H
	if (m_ModelInfo != NULL)	m_ModelInfo = NULL;		// NULL��

	// ���[�V�����C���t�H
	if (m_MotionInfo != NULL)	m_MotionInfo = NULL;	// NULL��
}

//=============================================================================
// ����������										(public)	*** CSlime_King ***
//=============================================================================
HRESULT CSlime_King::Init(void)
{
	CEnemy::Init();		// ����������

	// ���f��
	if (m_pModel == NULL && m_ModelInfo != NULL)
	{// NULL�̏ꍇ
		m_pModel = new CModel[m_MaxModel];	// �������m��

		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
		{// ���f�����J�E���g
		 // �����l�ݒ�
			m_pModel[nCntModel].SetInitAll(m_pTexture[nCntModel], m_pMesh[nCntModel], m_pBuffMat[nCntModel], m_nNumMat[nCntModel], m_ModelInfo[nCntModel].pos, m_ModelInfo[nCntModel].rot);
			// ����������
			m_pModel[nCntModel].Init();

			// �e���f����ݒ�
			if (0 <= m_ModelInfo[nCntModel].nIndex)		// �e�ȊO
				m_pModel[nCntModel].SetParent(&m_pModel[m_ModelInfo[nCntModel].nIndex]);
		}
	}

	// ���[�V����
	if (m_pMotion == NULL && m_MotionInfo != NULL)
	{// NULL�̏ꍇ
		m_pMotion = new CMotion;	// �������m��

		m_pMotion->SetMotionNum(0);	// �ҋ@���[�V����
	}

	m_nHP = m_MAX_HP;	// �̗͐ݒ�
	m_nMAXHP = m_MAX_HP;	// �̗͐ݒ�

	// ����ݒ�
	m_vtxMax = D3DXVECTOR3(30.0f, 40.0f, 30.0f);
	m_vtxMin = D3DXVECTOR3(-30.0f, 0.0f, -30.0f);

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Uninit(void)
{
	CEffect::Create(CEffect::TYPE_CREATE, CEffect::SHOT_NONE, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	CCameraMain *pCameraMain = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	if (pCameraMain->GetLockOn() == true)
	{// ���b�N�I�����Ă���
		if (pCameraMain->GetLockOnEnemy() == this)
		{// ������������
			pCameraMain->SetLockOn(false);		// ���b�N�I�����Ȃ�
		}
	}

	CScene *pScene		= NULL;		// �V�[��
	CScene *pSceneNext	= NULL;		// ���V�[��

	// �G
	{
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
				CEnemy *pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
				if (pEnemy != this)
				{
					pEnemy->Uninit();
				}
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	// ���f��
	if (m_pModel != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++) // ���f�����J�E���g
			m_pModel[nCntModel].Uninit();	// �I������

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

	CEnemy::Uninit();	// �I������
}

//=============================================================================
// �X�V����											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Update(void)
{
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);

	State();	// ��ԏ���

	m_posOld = m_pos;				// �ʒu�ۑ�

	if (pCamera->GetPickUp() == false)
		Ai();							// Ai

	m_pos += m_move;				// �ړ��ʉ��Z

	CScene *pScene		= NULL;		// �V�[��
	CScene *pSceneNext	= NULL;		// ���V�[��

	// �G
	{
		pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
				CEnemy *pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
				if (pEnemy != this)
				{
					pEnemy->Collision(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin, true);
				}
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	float fHeight = -500.0f;		// �n�ʍ���

	ColisionChack(&fHeight);		// ����`�F�b�N����
	m_shadowHeight = fHeight;		// �e�����ݒ�

	// �n�ʒ���
	{
		if (m_pos.y < fHeight)
		{// �w�荂������
			m_pos.y = fHeight;		// �����ݒ�
			m_move.y = 0.0f;		// �ړ��ʁiY�j��������

			if (m_nMoveRecast == 0)
				m_bJump = false;		// �W�����v���Ȃ�
			else
				m_nMoveRecast--;
		}
	}

	// �p�x����
	{
		// �ڕW�p�x����
		if (m_frotDest < -D3DX_PI)
			m_frotDest += D3DX_PI * 2.0f;
		if (D3DX_PI < m_frotDest)
			m_frotDest -= D3DX_PI * 2.0f;

		// �ڕW�p�x�܂ł̍��������߂�
		float rotDiff = (m_frotDest - m_rot.y);

		// ��������
		if (rotDiff < -D3DX_PI)
			rotDiff += D3DX_PI * 2.0f;
		if (D3DX_PI < rotDiff)
			rotDiff -= D3DX_PI * 2.0f;

		float fRatio = 0.0f;		// ����
		fRatio = 5.0;

		// �p�x�ݒ�
		m_rot.y += ((rotDiff) / fRatio);

		// �p�x����
		if (m_rot.y < -D3DX_PI)
			m_rot.y += D3DX_PI * 2.0f;
		if (D3DX_PI < m_rot.y)
			m_rot.y -= D3DX_PI * 2.0f;
	}

	// �ړ��ʒ���
	{
		m_move.x -= (m_move.x - 0.0f) * MC_MOVE_DOWN;		// �ړ��ʁiX�j������
		m_move.z -= (m_move.z - 0.0f) * MC_MOVE_DOWN;		// �ړ��ʁiZ�j������

		// �d�͉��Z
		m_move.y -= MC_GRAVITY;
	}

	// �}�b�v�A�C�R��
	if (m_MapIcon != NULL)
	{// NULL�ȊO�̏ꍇ
		m_MapIcon->Setpos(m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f));
		m_MapIcon->Setrot(m_rot);
	}

	// ���[�V�����X�V
	{
		if (m_pMotion != NULL)	// NULL�ȊO�̏ꍇ
			m_pMotion->Update(m_MaxModel, &m_MotionInfo[0], &m_pModel[0]);
	}
}

//=============================================================================
// �`�揈��											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
	{// ���f�����J�E���g
		if (m_pModel != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pModel[nCntModel].Draw();

			// �e�\��
			CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(m_pos.x, m_shadowHeight + 0.1f, m_pos.z));
		}
	}
}

//=============================================================================
// �Փ˔��菈��											(public)	*** CSlime_King ***
//=============================================================================
bool CSlime_King::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin, bool bSet)
{
	CCameraMain	*pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);	// �J�����擾
	bool		bLand = false;	// ����������

	// �����v�Z
	float fLenght = sqrtf(powf(m_vtxMax.x, 2.0f) + powf(m_vtxMax.z, 2.0f)) + sqrtf(powf(sizeMax.x, 2.0f) + powf(sizeMax.z, 2.0f));

	if (sqrtf(powf(m_pos.x - pPos->x, 2.0f) + powf(m_pos.z - pPos->z, 2.0f)) < fLenght)
	{// ��苗���ȉ���������
		if (pCamera->GetLockOn() == true || pCamera->GetLockOn() == false && (m_pos.y + m_vtxMin.y - 5.0f < pPos->y && pPos->y < m_pos.y + m_vtxMax.y) || m_bAttack == true || bSet == true)
		{// ��������
			// ��]�v�Z
			float frot = atan2f(m_pos.x - pPos->x, m_pos.z - pPos->z) + D3DX_PI;

			if (pMove != NULL)
				*pMove = D3DXVECTOR3(sinf(frot) * 8.0f, pMove->y, cosf(frot) * 8.0f);	// �����Ԃ�

			if (sqrtf(powf(m_pos.x - pPos->x, 2.0f) + powf(m_pos.z - pPos->z, 2.0f)) < (fLenght * 0.5f))
			{// ��苗���ȉ���������
				if (pPos != NULL)
					*pPos = D3DXVECTOR3(m_pos.x + sinf(frot) * (fLenght * 0.5f), pPos->y, m_pos.z + cosf(frot) * (fLenght * 0.5f));		// �~�߂�
			}

			bLand = true;	// ��������
		}
	}

	return bLand;	// �l��Ԃ�
}

//=============================================================================
// ����`�F�b�N����									(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::ColisionChack(float *fHeight)
{
	CScene *pScene = NULL;		// �V�[��
	CScene *pSceneNext = NULL;		// ���V�[��

	// �t�B�[���h
	{
		pScene = CScene::GetScene(CScene::PRIORITY_1);		// �V�[���擾

		while (pScene != NULL)
		{// NULL�ȊO�̏ꍇ
			pSceneNext = pScene->GetSceneNext();			// ���V�[�����L��

			if (pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD)
			{// �I�u�W�F�N�g�^�C�v�����b�V���t�B�[���h�̏ꍇ
				CSceneMesh *pMesh = (CSceneMesh*)pScene;	// �V�[�����b�V���̌^�ɃL���X�g

				if (pMesh->ColInMesh(m_pos) == true)
				{// �͈̓`�F�b�N
					*fHeight = pMesh->GetHeight(m_pos);		// �����擾

					break;
				}
			}
			pScene = pSceneNext;	// ���V�[����
		}
	}

	CManager::MODE mode = CManager::GetMode();

	// �}�b�v
	if (mode == CManager::MODE_GAME)
	{// �Q�[���̂�
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{
			CMap *pMap = pGame->GetMap();
			if (pMap != NULL)
			{
				int nMapNum = pGame->GetMapNum();
				if (nMapNum == 2)
				{
					CMap_02 *pMap_02 = (CMap_02*)pMap;

					if (pMap_02->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f) == true)
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
				else if (nMapNum == 3)
				{
					CMap_03 *pMap_03 = (CMap_03*)pMap;

					if (pMap_03->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f))
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
				else if (nMapNum == 4)
				{
					CMap_04 *pMap_04 = (CMap_04*)pMap;

					if (pMap_04->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f))
					{
						if (m_bAutoMove == true)
						{
							m_frotDest += D3DX_PI;
						}
					}
				}
			}
		}
	}

	CGame *pGame = CManager::GetGame();

	if (mode == CManager::MODE_GAME && pGame != NULL)
	{
		CPlayer *pPlayer = pGame->GetPlayer();
		if (pPlayer != NULL)
		{
			if (m_bAttack == true)
			{
				// �ʒu�擾
				D3DXVECTOR3 pos = pPlayer->Getpos();
				if (pos.y < m_vtxMax.y * 1.2f)
				{
					// �����v�Z
					float fLenght = sqrtf(powf(m_vtxMax.x, 2.0f) + powf(m_vtxMax.z, 2.0f));

					if (sqrtf(powf(m_pos.x - pos.x, 2.0f) + powf(m_pos.z - pos.z, 2.0f)) < fLenght)
					{// ��苗���ȉ���������
						pPlayer->AddHP(-15);
					}
				}
			}
		}
	}
}

//=============================================================================
// ��ԏ���											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::State(void)
{
	float col_a = 1.0f;	// �F�ݒ�p

	// ��ԏ���	
	{
		switch (m_state)
		{// ���
		case STATE_NORMAL:		// �ʏ�
			break;
		case STATE_DAMAGE:		// �_���[�W
			m_nCntState++;		// ��ԃJ�E���g

			// �_��
			if (m_nCntState / 5 % 2 != 1)
				col_a = 0.0f;	// ����
			else
				col_a = 1.0f;	// �s����

			if (m_nCntState == 15)
			{// ���̃J�E���g�ɂȂ�����
				m_nCntState = 0;									// ��ԃJ�E���g������
				m_state		= STATE_NORMAL;							// ��Ԃ�ʏ��
				col_a		= 1.0f;									// �F��������
			}

			for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
				m_pModel[nCntModel].Setcol_a(col_a);				// �F�ݒ�

			break;
		case STATE_DEATH:		// ����
			break;
		}
	}

	// �J�����擾
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	D3DXVECTOR3 Camerapos = pCamera->Getpos(0);

	// �����v�Z
	float fLenght = sqrtf(powf(m_pos.x - Camerapos.x, 2.0f) + powf(m_pos.z - Camerapos.z, 2.0f));

	// �����ɂ���ē��߂���
	if (fLenght < MC_DRAW_RANGE)
		for (int nCount = 0; nCount < MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(fLenght / MC_DRAW_RANGE);
	else
		for (int nCount = 0; nCount< MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(col_a);
}

//=============================================================================
// �ړ�����											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Move(void)
{

}

//=============================================================================
// Ai����											(public)	*** CSlime_King ***
//=============================================================================
void CSlime_King::Ai(void)
{
	CSound *pSound = CManager::GetSound();			// �T�E���h�擾

	CManager::MODE mode = CManager::GetMode();		// ���[�h�擾
	CGame *pGame = CManager::GetGame();				// �Q�[���擾

	if (mode == CManager::MODE_GAME && pGame != NULL)
	{// ���[�h���Q�[���������� + NULL�ȊO�̏ꍇ
		CPlayer *pPlayer = pGame->GetPlayer();			// �v���C���[�擾
		if (pPlayer != NULL)
		{// NULL�ȊO�̏ꍇ
			D3DXVECTOR3 pos = pPlayer->Getpos();			// �ʒu�擾
			D3DXVECTOR3 size = pPlayer->GetvtxMax();		// �傫���擾

			// �����Ɗp�x���v�Z
			float fRange = sqrtf(powf(m_pos.x - pos.x, 2.0f) + powf(m_pos.z - pos.z, 2.0f));
			float fAngle = atan2f(pos.x - m_pos.x, pos.z - m_pos.z);

			// AI
			if (m_AiType == AI_NORMAL)
			{// �ʏ�
				bool bAttack = false;	// �U��

				if (m_bAttack == false)
				{// �U�����Ă��Ȃ�
					if (fRange < 200.0f)
					{// �͈͓���������i�U���j
						m_frotDest = fAngle;
						m_move.x = 0.0f;
						m_move.z = 0.0f;

						// �U��
						bAttack = true;

						if (m_bAutoMove == true)
						{// �����ړ�����������
							// �����ړ����L�����Z��
							m_bAutoMove = false;
							m_nCntAutoMove = 0;
							m_nMaxAutoMove = 0;
							m_nAutoMoveRecast = 0;
						}
					}
					else if (fRange < 400.0f)
					{// �͈͓���������i�U�������j
						m_frotDest = fAngle;

						if (m_nMoveRecast == 20)
						{// �ړ����n�߂�
							m_move.x = sinf(fAngle) * (MC_MOVE_UP * 1.7f);
							m_move.z = cosf(fAngle) * (MC_MOVE_UP * 1.7f);
						}

						if (m_bJump == false)
						{// �W�����v���Ă��Ȃ�
							// �W�����v���Ĉړ��J�n
							m_move.y += MC_JUMP;
							m_bJump = true;
							m_nMoveRecast = 20;
						}

						if (m_bAutoMove == true)
						{// �����ړ�����������
							// �����ړ����L�����Z��
							m_bAutoMove = false;
							m_nCntAutoMove = 0;
							m_nMaxAutoMove = 0;
							m_nAutoMoveRecast = 0;
						}

						if (300.0f < fRange)
						{// �͈͓���������
							int nAi = rand() % 10;
							if (nAi == 1)
							{// �P��������
								m_AiType = AI_MAGIC;	// ���@
							}
							else if (nAi == 2)
							{// �Q��������
								m_AiType = AI_DASH;		// �_�b�V��
							}
						}
					}
					else if (fRange < 600.0f)
					{// �͈͓���������i���@�j
						if (m_nMoveRecast == 20)
						{// �ړ����n�߂�
							m_move.x = sinf(fAngle) * (MC_MOVE_UP * 1.8f);
							m_move.z = cosf(fAngle) * (MC_MOVE_UP * 1.8f);
						}

						if (m_bJump == false)
						{// �W�����v���Ă��Ȃ�
							m_AiType = AI_MAGIC;	// ���@
						}
					}
					else
					{// �͈͊O
						if (m_bJump == false)
						{// �W�����v���Ă��Ȃ�
							m_AiType = AI_DASH;		// �_�b�V��
						}
					}
				}
				else
				{// �U������
					if (fRange < 250.0f)
					{// �͈͓�
						bAttack = true;			// �U������
					}
					else
					{
						m_bAttack = false;		// �U�����Ȃ�
					}
				}

				if (bAttack == true)
				{// �U������
					m_frotDest = fAngle;

					if (m_bJump == false)
					{// �W�����v���Ă��Ȃ�
						if (0 < m_nCntAttack && m_nCntAttack < 38)
						{// ���t���[���̊Ԃ�������
							m_bAttack = true;							
						}
						else
						{// ���̑�
							m_bAttack = false;
						}

						if (0 < m_nCntAttack && m_nCntAttack < 30)
						{// ���t���[���̊Ԃ�������
							m_move.x = sinf(fAngle) * -0.2f;
							m_move.z = cosf(fAngle) * -0.2f;
						}
						else if (30 < m_nCntAttack && m_nCntAttack < 35)
						{// ���t���[���̊Ԃ�������
							m_move.x = sinf(fAngle) * (fRange / 3);
							m_move.z = cosf(fAngle) * (fRange / 3);

							if (m_nCntAttack == 31)
							{// ���t���[����������
								m_move.y += MC_JUMP;

								if (pSound != NULL)
								{// NULL�ȊO�̏ꍇ
									pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENEMY_ATTACK001);
								}
							}

							CEffect::Create(CEffect::TYPE_C00, CEffect::SHOT_NONE, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
						else if (35 < m_nCntAttack && m_nCntAttack < 38)
						{// ���t���[���̊Ԃ�������
							m_move.x = sinf(fAngle) * -(fRange / 3);
							m_move.z = cosf(fAngle) * -(fRange / 3);
						}
						m_nCntAttack++;

						int nNum = rand() % 120 + 150;
						if (nNum < m_nCntAttack)
						{
							m_nCntAttack = 0;

							int nAi = rand() % 3;
							if (nAi == 0)
							{
								m_AiType = AI_CREATE;
							}					
						}
					}
				}
				else
				{// �U�����Ă��Ȃ�
					m_nCntAttack = 0;
				}
			}
			else if (m_AiType == AI_MAGIC)
			{// ���@
				bool bAttack = false;

				if (fRange < 600.0f)
				{// �͈͓�
					m_frotDest = fAngle;
					m_move.x = 0.0f;
					m_move.z = 0.0f;

					bAttack = true;

					if (m_bAutoMove == true)
					{// �����ړ�����������
						// �����ړ����L�����Z��
						m_bAutoMove = false;
						m_nCntAutoMove = 0;
						m_nMaxAutoMove = 0;
						m_nAutoMoveRecast = 0;
					}
				}
				else
				{// �͈͊O
					m_frotDest = fAngle;

					if (m_bJump == false)
					{
						m_AiType = AI_DASH;
					}
				}

				if (bAttack == true)
				{// �U������
					m_frotDest = fAngle;

					if (m_nCntAttack % 80 == 0)
					{// ���t���[����������
						CTexAnim *pTexAnim = NULL;
						pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(sinf(m_rot.y + (D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + (D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR2(0.0714f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
						pTexAnim->SetAnim(14, 14, 3, 0, false, true);

						CEffect::Create(CEffect::TYPE_C01, CEffect::SHOT_ENEMY, m_pos + D3DXVECTOR3(sinf(m_rot.y + (D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + (D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(sinf(m_rot.y) * 8.0f, -2.0f, cosf(m_rot.y) * 8.0f), D3DXVECTOR3(9.0f, 9.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
					else if (m_nCntAttack % 80 == 40)
					{// ���t���[����������
						CTexAnim *pTexAnim = NULL;
						pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(sinf(m_rot.y + -(D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + -(D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXVECTOR2(0.0714f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
						pTexAnim->SetAnim(14, 14, 3, 0, false, true);

						CEffect::Create(CEffect::TYPE_C01, CEffect::SHOT_ENEMY, m_pos + D3DXVECTOR3(sinf(m_rot.y + -(D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + -(D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(sinf(m_rot.y) * 8.0f, -2.0f, cosf(m_rot.y) * 8.0f), D3DXVECTOR3(9.0f, 9.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}

					m_nCntAttack++;

					if (m_nCntAttack == 120)
					{// ���t���[����������
						m_nCntAttack = 0;
						m_AiType = AI_NORMAL;

						if (m_bJump == false)
						{// �����ړ�����������
							// �����ړ����L�����Z��
							m_move.y += MC_JUMP;
							m_bJump = true;
							m_nMoveRecast = 20;
						}

						if (420.0f < fRange)
						{// �͈͓�
							m_AiType = AI_DASH;		// �_�b�V��
						}

						int nAi = rand() % 3;
						if (nAi == 0)
						{// �O��������
							m_AiType = AI_CREATE;	// �N���G�C�g
						}
					}
				}
				else
				{// �U�����Ă��Ȃ�
					m_nCntAttack = 0;
				}
			}
			else if (m_AiType == AI_DASH)
			{// �_�b�V��
				m_frotDest = fAngle;

				CEffect::Create(CEffect::TYPE_C00, CEffect::SHOT_NONE, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				if (m_nMoveRecast == 20)
				{// �ړ����n�߂�
					m_move.x = sinf(fAngle) * (MC_MOVE_UP * 7.0f);
					m_move.z = cosf(fAngle) * (MC_MOVE_UP * 7.0f);
				}

				if (m_bJump == false)
				{// �W�����v���Ă��Ȃ�
					if (fRange < 300.0f)
					{// �͈͓��i�ʏ�j
						m_AiType = AI_NORMAL;
					}
					else if (fRange < 350.0f)
					{// �͈͓��i�N���G�C�g�j
						m_AiType = AI_CREATE;
					}
					else if (fRange < 400.0f)
					{// �͈͓��i�X�g�b�v�j
						m_AiType = AI_STOP;
					}
					else
					{// �͈͊O
						m_move.y += (MC_JUMP * 1.5f);
						m_bJump = true;
						m_nMoveRecast = 20;
					}
				}
			}
			else if (m_AiType == AI_CREATE)
			{// �N���G�C�g
				if (pGame->GetEnemyNum() < 10)
				{// �P�O�̈ȉ���������
					CTexAnim *pTexAnim = NULL;
					// ���@�w
					pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D, m_pos + D3DXVECTOR3(sinf(m_rot.y + (D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + (D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 50.0f), D3DXVECTOR2(0.0714f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
					pTexAnim->SetAnim(14, 14, 3, 0, false, true);
					// ���@�w
					pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D, m_pos + D3DXVECTOR3(sinf(m_rot.y + -(D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + -(D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 80.0f, 0.0f),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 50.0f), D3DXVECTOR2(0.0714f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
					pTexAnim->SetAnim(14, 14, 3, 0, false, true);

					// �G����
					CEnemy::Create(CEnemy::TYPE_N_SLIME, m_pos + D3DXVECTOR3(sinf(m_rot.y + (D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + (D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
					CEnemy::Create(CEnemy::TYPE_N_SLIME, m_pos + D3DXVECTOR3(sinf(m_rot.y + -(D3DX_PI * 0.5f)), 0.0f, cosf(m_rot.y + -(D3DX_PI * 0.5f))) * 50.0f + D3DXVECTOR3(0.0f, 50.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
				}

				m_AiType = AI_STOP;		// ��~
			}
			else if (m_AiType == AI_STOP)
			{// ��~
				m_frotDest = fAngle;
				m_nCntAttack++;

				if (m_nCntAttack == 90)
				{// ���t���[���ɂȂ�����
					m_nCntAttack = 0;
					m_AiType = AI_NORMAL;	// �ʏ�
				}
			}
		}
	}
}