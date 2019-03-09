//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "player.h"			// �v���C���[
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "sound.h"			// �T�E���h
#include "input.h"			// ����
#include "text.h"			// �e�L�X�g
#include "camera.h"			// �J����
#include "fade.h"			// �t�F�[�h
#include "texture.h"		// �e�N�X�`��

#include "scenemesh.h"		// �V�[�����b�V��
#include "scenemeshorbit.h"	// �I�[�r�b�g
#include "texanim.h"		// �e�N�X�`���A�j���[�V����
#include "title.h"			// �^�C�g��
#include "game.h"			// �Q�[��
#include "model.h"			// ���f��
#include "motion.h"			// ���[�V����
#include "map.h"			// �}�b�v
#include "ui.h"				// Ui
#include "mapicon.h"		// �}�b�v�A�C�R��
#include "effect.h"			// �G�t�F�N�g
#include "gauge.h"			// �Q�[�W
#include "shadow.h"			// �e
#include "enemy.h"			// �G

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MC_MOVE_UP		(2.7f)								// ����
#define MC_MOVE_DOWN	(0.75f)								// ����
#define MC_JUMP			(9.0f)								// �W�����v��
#define MC_GRAVITY		(0.6f)								// �d��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int						CPlayer::m_MaxModel		= 0;		// ���f����
int						CPlayer::m_MaxMotion	= 0;		// ���[�V������

LPDIRECT3DTEXTURE9		**CPlayer::m_pTexture	= NULL;		// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH				*CPlayer::m_pMesh		= NULL;		// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER			*CPlayer::m_pBuffMat	= NULL;		// �}�e���A�����ւ̃|�C���^
DWORD					*CPlayer::m_nNumMat		= NULL;		// �}�e���A�����̐�

CModel::MODEL_INFO		*CPlayer::m_ModelInfo	= NULL;		// ���f���C���t�H
CMotion::MOTION_INFO	*CPlayer::m_MotionInfo	= NULL;		// ���[�V�����C���t�H

//=============================================================================
// �R���X�g���N�^									(public)	*** CPlayer ***
//=============================================================================
CPlayer::CPlayer() : CScene(CScene::PRIORITY_3, CScene::OBJTYPE_PLAYER)
{
	// �����l�ݒ�
	m_MapIcon		= NULL;									// �}�b�v�A�C�R��

	m_bTutorialSet	= false;								// �`���[�g���A���ݒ�
	m_nCntTutorialSet = 0;									// �`���[�g���A���ݒ�J�E���g

	for(int nCount = 0; nCount < ORBIT_MAX; nCount++)
		m_pOrbit[nCount];	// �I�[�r�b�g

	m_bWarp			= false;								// ���[�v���Ă��邩
	m_nCntWarp		= 0;									// ���[�v�J�E���g
	m_fcol_a		= 1.0f;									// �F�A���t�@
	m_posWarp		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���[�v�ʒu
	m_fWarpLenght	= 0.0f;
	m_fWarpAngle	= 0.0f;

	m_bUpdata		= true;									// �X�V����
	m_bDraw			= true;									// �`�悷��
	m_bMove			= false;								// �ړ��ł���

	m_nHP			= 0;									// HP
	m_nMP			= 0;									// MP

	m_nHatNum		= 0;									// �X�q�ԍ�
	m_nCntTimer		= 0;									// �^�C�}�[�J�E���g

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
	m_nAttackCnt	= 0;									// �U���J�E���g
	m_nAttackNum	= ATTACK_N_1;							// �U�����͐�
	m_bAttack_Nor	= false;								// �ʏ�U��
	m_bAttack_Sky	= false;								// �󒆍U��
	m_bAction_Sky	= false;								// �󒆃A�N�V����
	m_nAttackRecast = 0;									// �U�����L���X�g
	m_nGuardCnt		= 0;									// �K�[�h�J�E���g
	m_bGuard		= false;								// �K�[�h
	m_nCntJastGuard	= 0;									// �W���X�g�K�[�h�J�E���g
	m_fDamageCat	= 1.0f;									// �_���[�W�J�b�g
	m_nStepCnt		= 0;									// �X�e�b�v�J�E���g
	m_bStep			= false;								// �X�e�b�v
	m_bSkill		= false;								// �X�L��
	m_nSkillRecast	= 0;									// �X�L�����L���X�g

	m_brot			= false;								// ��]�\��

	m_shadowHeight = 0.0f;									// �e����

	m_bNotAttack	= false;								// �U���֎~
	m_bNotSkill		= false;								// �X�L���֎~

	for (int nCount = 0; nCount < GUARD_DOME_MAX; nCount++)
		m_pGuardDome[nCount] = NULL;						// �K�[�h�h�[��

	m_GuardDomeUV = D3DXVECTOR2(0.0f, 0.0f);				// �K�[�h�h�[��UV
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CPlayer ***
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ���[�h����										(public)	*** CPlayer ***
//=============================================================================
HRESULT CPlayer::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾

	m_ModelInfo		= CText::GetModel("data\\TEXT\\MODEL\\playermodel.txt", &m_MaxModel);		// ���f���擾
	m_MotionInfo	= CText::GetMotion("data\\TEXT\\MOTION\\playermotion.txt");					// ���[�V�����擾

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
// �A�����[�h����								(public)	*** CBullet ***
//=============================================================================
void CPlayer::Unload(void)
{
	// �e���̃������J���ANULL��
	if (m_pTexture	!= NULL)	{ delete[] m_pTexture;	m_pTexture	= NULL; }
	if (m_pMesh		!= NULL)	{ delete[] m_pMesh;		m_pMesh		= NULL; }
	if (m_pBuffMat	!= NULL)	{ delete[] m_pBuffMat;	m_pBuffMat	= NULL; }
	if (m_nNumMat	!= NULL)	{ delete[] m_nNumMat;	m_nNumMat	= NULL; }

	// ���f���C���t�H
	if (m_ModelInfo != NULL)	m_ModelInfo = NULL;		// NULL��

	// ���[�V�����C���t�H
	if (m_MotionInfo != NULL)	m_MotionInfo = NULL;	// NULL��
}

//=============================================================================
// �N���G�C�g										(public)	*** CPlayer ***
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *pPlayer = NULL;		// �|�C���^

	if (pPlayer == NULL)			// NULL�̏ꍇ
		pPlayer = new CPlayer;		// �������m��

	if (pPlayer != NULL)
	{// NULL�ȊO�̏ꍇ
		pPlayer->m_pos = pos;			// �ʒu
		pPlayer->m_rot = rot;			// ����
		pPlayer->m_frotDest = rot.y;	// �ڕW����
		pPlayer->Init();				// ����������
	}

	return pPlayer;		// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CPlayer ***
//=============================================================================
HRESULT CPlayer::Init()
{
	CManager::MODE mode = CManager::GetMode();		// ���[�h�擾

	// �}�b�v�A�C�R��
	if (m_MapIcon == NULL)
	{// NULL�̏ꍇ
		if (mode == CManager::MODE_TITLE)
			m_MapIcon = CMapIcon::Create(PRIORITY_MAP_2, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), m_rot, D3DXVECTOR3(30.0f, 0.0f, 50.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		else if (mode == CManager::MODE_GAME)
			m_MapIcon = CMapIcon::Create(PRIORITY_MAP_2, m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f), m_rot, D3DXVECTOR3(80.0f, 0.0f, 100.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_MapIcon->SetTex(2, 2, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
	}

	// ���f��
	if (m_pModel == NULL && m_ModelInfo != NULL)
	{// NULL�̏ꍇ
		m_pModel = new CModel[m_MaxModel];		// �������m��

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

		m_pMotion->SetMotionNumrot(MOTION_NEUTRAL, 0);	// �ҋ@���[�V����
	}

	// ����ݒ�
	m_vtxMax = D3DXVECTOR3(16.0f, 50.0f, 16.0f);	// �ő�
	m_vtxMin = D3DXVECTOR3(-16.0f, 0.0f, -16.0f);	// �ŏ�

	// �̗͐ݒ�
	m_nHP = m_MAX_HP;	// HP
	m_nMP = m_MAX_MP;	// MP

	// �I�[�r�b�g
	{
		// �E��
		m_pOrbit[ORBIT_RIGHT] = CSceneMeshOrbit::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(5.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.2f, 0.2f, 30);
		m_pOrbit[ORBIT_RIGHT]->SetMtxParent(&m_pModel[5].GetmtxWorld());

		// ����
		m_pOrbit[ORBIT_LEFT] = CSceneMeshOrbit::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-5.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0.2f, 0.2f, 30);
		m_pOrbit[ORBIT_LEFT]->SetMtxParent(&m_pModel[11].GetmtxWorld());
	}

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Uninit(void)
{
	// �}�b�v�A�C�R��
	if (m_MapIcon != NULL)
	{// NULL�ȊO�̏ꍇ
		m_MapIcon->Uninit();	// �j��
		m_MapIcon = NULL;		// NULL��
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

	// �I�[�r�b�g
	for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
	{
		if (m_pOrbit[nCount] != NULL)
		{
			m_pOrbit[nCount]->Uninit();
			m_pOrbit[nCount] = NULL;
		}
	}

	for (int nCount = 0; nCount < GUARD_DOME_MAX; nCount++)
	{
		if (m_pGuardDome[nCount] != NULL)
		{
			m_pGuardDome[nCount]->Uninit();
			m_pGuardDome[nCount] = NULL;
		}
	}

	CScene::SetDeath();		// ���S�t���O
}

//=============================================================================
// �X�V����											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Update(void)
{
	CSound *pSound = CManager::GetSound();

	CManager::MODE	mode		= CManager::GetMode();		// ���[�h�擾
	CCameraMain		*pCamera	= (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);		// �J�����擾

	if (m_bUpdata == true)
	{// �X�V����
		if (mode == CManager::MODE_TITLE)
		{// �^�C�g���̂�
			Title();			// �^�C�g������
		}

		State();				// ��ԏ���

		m_posOld = m_pos;		// �ʒu�ۑ�

		if (m_state != STATE_DEATH)
		{
			if (m_nCntTutorialSet == 0)
			{
				Move();				// �ړ�����
				Step();				// �X�e�b�v����
				Jump();				// �W�����v����
				Action();			// �A�N�V��������		
			}
		}

		m_pos += m_move;	// �ړ��ʉ��Z

		if (mode == CManager::MODE_TITLE)
		{// �^�C�g���̂�
			// �`���[�g���A�����擾
			bool bTutorial = CManager::GetTitle()->GetTutorial();

			if (m_nCntTutorialSet == 0)
			{// �J�E���g���O��������
				if (bTutorial == true && m_bTutorialSet == true)
				{// �`���[�g���A���� + �`���[�g���A���̏������I�����
					// ����
					if (m_pos.x < 170.0f) { m_pos.x = 170.0f;		m_move.x = 0.0f; }
					if (690.0f < m_pos.x) { m_pos.x = 690.0f;		m_move.x = 0.0f; }
					if (m_pos.z < -160.0f) { m_pos.z = -160.0f;	m_move.z = 0.0f; }
					if (160.0f < m_pos.z) { m_pos.z = 160.0f;		m_move.z = 0.0f; }
				}
			}
		}
		else if (mode == CManager::MODE_GAME)
		{// �Q�[���̂�
			// ����
			if (m_pos.x < -1470.0f) { m_pos.x = -1470.0f;	m_move.x = 0.0f; }
			if (1470.0f < m_pos.x) { m_pos.x = 1470.0f;	m_move.x = 0.0f; }
			if (m_pos.z < -1470.0f) { m_pos.z = -1470.0f;	m_move.z = 0.0f; }
			if (1470.0f < m_pos.z) { m_pos.z = 1470.0f;	m_move.z = 0.0f; }
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

				if (m_bJump == true)
				{
					// �A�j���[�V�����Đ�
					CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 3.0f, 0.0f), D3DXVECTOR3(0.0f, 0.5f, 0.0f), D3DXVECTOR3(6.0f, 6.0f, 0.0f),
						D3DXVECTOR2(0.125f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_WALK));
					pTexAnim->SetAnim(8, 8, 2, 0, false, true);

					if (pSound != NULL)
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_LANDING);
				}

				m_bJump = false;		// �W�����v���Ȃ�
				m_bAttack_Sky = false;	// �󒆍U�����Ȃ�
				m_bAction_Sky = false;	// �󒆃A�N�V�������Ȃ�
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

			if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
			{
				fRatio = 5.0;
			}
			else
			{
				if (pCamera->GetLockOn() == true)
				{
					fRatio = 5.0;
				}
				else
				{
					fRatio = 30.0f;
				}
			}

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
			if (m_bAttack_Sky == false && m_bWarp == false)		m_move.y -= MC_GRAVITY;
			else if (m_bAttack_Sky == true)	m_move.y = 0.0f;
		}

		// �}�b�v�A�C�R��
		if (m_MapIcon != NULL)
		{// NULL�ȊO�̏ꍇ
			m_MapIcon->Setpos(m_pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f));
			m_MapIcon->Setrot(m_rot);
		}
	}

	// ���[�V�����X�V
	{
		if (m_pMotion != NULL)	// NULL�ȊO�̏ꍇ
			m_pMotion->Update(m_MaxModel, &m_MotionInfo[0], &m_pModel[0]);
	}

	{
		// �����ݒ�
		CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "Player : �ʒu�@[%.2f, %.2f, %.2f]\n", m_pos.x, m_pos.y, m_pos.z);
		CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "Player : �����@[%.2f, %.2f, %.2f]\n", m_rot.x, m_rot.y, m_rot.z);
		CFont::SetMessageData(CScene::OBJTYPE_PLAYER, CFont::SETTYPE_ADD, "\n");
	}
}

//=============================================================================
// �`�揈��											(public)	*** CPlayer ***
//=============================================================================
void CPlayer::Draw(void)
{
	CManager::MODE		mode = CManager::GetMode();		// ���[�h�擾
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();		// �f�o�C�X�擾
	D3DXMATRIX			mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X

	if (m_bDraw == true)
	{// �`�悷��
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

				if (m_fcol_a != 0.0f)
				{
					// �e�\��
					CShadow::SetShadow(m_pModel[nCntModel].GetmtxWorld(), m_pMesh[nCntModel], m_nNumMat[nCntModel], D3DXVECTOR3(m_pos.x, m_shadowHeight + 0.1f, m_pos.z));
				}
			}
		}
	}
}

//=============================================================================
// �^�C�g������										(private)	*** CPlayer ***
//=============================================================================
void CPlayer::Title(void)
{
	bool bTutorial = CManager::GetTitle()->GetTutorial();

	if (m_nCntTutorialSet == 0)
	{// �O��������
		if (bTutorial == true && m_bTutorialSet == false)
		{
			m_bTutorialSet = true;
			m_nCntTutorialSet = 20;
		}
		else if (bTutorial == false && m_bTutorialSet == true)
		{
			m_bTutorialSet = false;
			m_nCntTutorialSet = 20;

			m_bGuard = false;			// �K�[�h���Ă��Ȃ�
			m_nAttackNum = ATTACK_N_1;		// �O����
			m_bAttack_Nor = false;			// �ʏ�U�����Ă��Ȃ�
			m_bAttack_Sky = false;			// �󒆍U�����Ă��Ȃ�
			m_bSkill = false;
			m_nAttackCnt = 0;

			for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
				if (m_pOrbit[nCount] != NULL)
					m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

			m_brot = false;
		}
	}

	if (m_nCntTutorialSet != 0)
	{
		CTexAnim *pTexAnim = NULL;		// �e�N�X�`���A�j���[�V����

		if (m_nCntTutorialSet == 20)
		{// �w�肳�ꂽ�J�E���g��������
			if (m_pMotion->GetMotionNum() != MOTION_LANDING)
				m_pMotion->SetMotionNum(MOTION_LANDING, 0, 3);	//	���n���[�V����
			m_bDraw = false;	// ��\��

								// �A�j���[�V�����Đ�
			pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
				D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
			pTexAnim->SetAnim(10, 10, 3, 0, false, true);
		}

		m_nCntTutorialSet--;	// �X�e�b�v�J�E���g

		if (m_nCntTutorialSet == 15)
		{
			if (m_bTutorialSet == true)
			{
				m_pos = D3DXVECTOR3(180.0f, 0.0f, 0.0f);
				m_frotDest = D3DX_PI * 0.5f;
			}
			else
			{
				m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				m_frotDest = D3DX_PI;
			}
		}

		if (m_nCntTutorialSet == 10)
		{// �w�肳�ꂽ�J�E���g��������
			m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// �j���[�g�������[�V����
			m_bDraw = true;		// �\��

			// �A�j���[�V�����Đ�
			pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
				D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
			pTexAnim->SetAnim(10, 10, 3, 0, false, true);
		}

		if (m_nCntTutorialSet < 0)
		{// ���J�E���g�ȉ���������
			m_nCntTutorialSet = 0;		// �X�e�b�v�J�E���g������		
			m_bStep = false;	// �X�e�b�v���Ă��Ȃ�
		}
	}
}

//=============================================================================
// ��ԏ���											(private)	*** CPlayer ***
//=============================================================================
void CPlayer::State(void)
{
	// �J�����擾
	CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	CGame *pGame = CManager::GetGame();

	float col_a = 1.0f;	// �F�ݒ�p

	// ��ԏ���	
	{
		switch (m_state)
		{// ���
		case STATE_NORMAL:		// �ʏ�
			if (m_pGuardDome[GUARD_DOME_UP] != NULL)
				m_pGuardDome[GUARD_DOME_UP]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));
			if (m_pGuardDome[GUARD_DOME_DOWN] != NULL)
				m_pGuardDome[GUARD_DOME_DOWN]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));

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

			if (m_fDamageCat == 0.3f)
			{// �W���X�g�_���[�W�J�b�g
				if (m_pGuardDome[GUARD_DOME_UP] != NULL)
					m_pGuardDome[GUARD_DOME_UP]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));
				if (m_pGuardDome[GUARD_DOME_DOWN] != NULL)
					m_pGuardDome[GUARD_DOME_DOWN]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));

				if (pGame != NULL)
				{// NULL�ȊO�̏ꍇ
					CGame::GAME game = pGame->GetGame();
					if (game != CGame::GAME_TUTORIAL && game != CGame::GAME_CLEAR && game != CGame::GAME_OVER)
					{// �`���[�g���A�� + �N���A + �I�[�o�[�ȊO
						pGame->AddJustGuard();
					}
				}
			}
			else
			{// ���̑�
				if (m_pGuardDome[GUARD_DOME_UP] != NULL)
					m_pGuardDome[GUARD_DOME_UP]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));
				if (m_pGuardDome[GUARD_DOME_DOWN] != NULL)
					m_pGuardDome[GUARD_DOME_DOWN]->SetStart(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(-D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0, 0.5f));
			}	

			if (pGame != NULL)
			{// NULL�ȊO�̏ꍇ
				CGame::GAME game = pGame->GetGame();
				if (game != CGame::GAME_TUTORIAL && game != CGame::GAME_CLEAR && game != CGame::GAME_OVER)
				{// �`���[�g���A�� + �N���A + �I�[�o�[�ȊO
					pGame->SetNoDamage();
				}
			}

			break;
		case STATE_DEATH:		// ����
			CManager::MODE mode = CManager::GetMode();
			if (mode == CManager::MODE_TITLE)
			{// ���[�h���^�C�g����������
				m_nHP = 100;
				m_nMP = 100;
				m_state = STATE_NORMAL;
			}
			else if (mode == CManager::MODE_GAME)
			{// ���[�h���Q�[����������
				if (pGame != NULL)
				{// NULL�ȊO�̏ꍇ
					if (pGame->GetGame() == CGame::GAME_TUTORIAL)
					{// �`���[�g���A����������
						m_nHP = 100;
						m_nMP = 100;
						m_state = STATE_NORMAL;
					}
					else
					{// ���̑�
						if (pGame->GetGame() != CGame::GAME_OVER)
							pGame->SetGame(CGame::GAME_OVER, false);

						pCamera->SetGameEnd(true);
						m_bMove = false;

						if (m_pMotion->GetMotionNum() != MOTION_DEATH)
							m_pMotion->SetMotionNum(MOTION_DEATH);	// �j���[�g�������[�V����
						m_pMotion->SetKeyStop(true);				// �ŏI�L�[�Œ�~
						m_bGuard = false;			// �K�[�h���Ă��Ȃ�
						m_nAttackNum = ATTACK_N_1;		// �O����
						m_bAttack_Nor = false;			// �ʏ�U�����Ă��Ȃ�
						m_bAttack_Sky = false;			// �󒆍U�����Ă��Ȃ�
						m_bSkill = false;
						m_nAttackCnt = 0;

						for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
							if (m_pOrbit[nCount] != NULL)
								m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

						m_brot = false;
					}
				}
			}

			for (int nCount = 0; nCount < GUARD_DOME_MAX; nCount++)
			{// �K�[�h�h�[���J�E���g
				if (m_pGuardDome[nCount] != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pGuardDome[nCount]->Uninit();		// �I������
					m_pGuardDome[nCount] = NULL;		// NULL��
				}
			}

			break;
		}
	}

	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_TITLE)
	{// ���[�h���^�C�g����������
		CUi_Game *pUi_Game = (CUi_Game*)CManager::GetTitle()->GetUi();
		if (pUi_Game != NULL)
		{// NULL�ȊO�̏ꍇ
			pUi_Game->SetGauge(CUi_Game::GAUGE_PLAYER_HP, m_nHP, m_MAX_HP);
			pUi_Game->SetGauge(CUi_Game::GAUGE_PLAYER_MP, m_nMP, m_MAX_MP);
		}
	}
	else if (mode == CManager::MODE_GAME)
	{// ���[�h���Q�[����������
		CUi_Game *pUi_Game = (CUi_Game*)CManager::GetGame()->GetUi();
		if (pUi_Game != NULL)
		{// NULL�ȊO�̏ꍇ
			pUi_Game->SetGauge(CUi_Game::GAUGE_PLAYER_HP, m_nHP, m_MAX_HP);
			pUi_Game->SetGauge(CUi_Game::GAUGE_PLAYER_MP, m_nMP, m_MAX_MP);
		}
	}

	D3DXVECTOR3 Camerapos	= pCamera->Getpos(0);	// �ʒu�擾

	// �����v�Z
	float		fLenght		= sqrtf(powf(m_pos.x - Camerapos.x, 2.0f) + powf(m_pos.z - Camerapos.z, 2.0f));

	// �����ɂ���ē��߂���
	if (fLenght < 80.0f)
		for (int nCount = 0; nCount < MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(fLenght / 80.0f);
	else
		for (int nCount = 0; nCount< MODEL_MAX; nCount++)
			m_pModel[nCount].Setcol_a(col_a);
}

//=============================================================================
// �ړ�����											(private)	*** CPlayer ***
//=============================================================================
void CPlayer::Move(void)
{
	CSound *pSound = CManager::GetSound();

	bool			bWalk		= false;							// ��������
	bool			bInput		= false;							// ���͂�����
	CCameraMain		*pCamera	= (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);		// �J�����擾
	D3DXVECTOR3		Camerarot	= pCamera->Getrot();				// �J�����p�x�擾
	float			fLockOn		= pCamera->GetLockOnRot();			// ���b�N�I���p�x�擾
	CInputKeyboard	*pKey		= CManager::GetInputKeyboard();		// �L�[�{�[�h�擾
	CInputXPad		*pXPad		= CManager::GetInputXPad();			// X�p�b�h�擾

	// �ړ�����
	{
		if (m_bMove == true)
		{// �ړ��ł���ꍇ
			if (m_bGuard == false && m_bStep == false && m_brot == false)
			{// �K�[�h���Ă��Ȃ��A�X�e�b�v���Ă��Ȃ�
				// X�p�b�h
				{
					if (pXPad != NULL)
					{// NULL�ȊO�̏ꍇ
						if (bInput == false)
						{// ���͂��Ă��Ȃ�
							if (pXPad->GetStick(0, 0) == true)
							{// X�p�b�h�i���X�e�B�b�N�j����͂���
								// ���͂�������������
								m_frotDest = pXPad->GetStickRot(0, 0) + Camerarot.y + D3DX_PI;

								if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
									bWalk = true;	// ������
								bInput = true;	// ���͂���
							}
						}
					}
				}

				// �L�[�{�[�h
				{
					if (pKey != NULL)
					{// NULL�ȊO�̏ꍇ
						if (bInput == false)
						{// ���͂��Ă��Ȃ�
							if (pKey->GetPress(DIK_W) == true || pKey->GetTrigger(DIK_UP) == true)
							{// �L�[�{�[�h�iW�j�������ꂽ
								if (pKey->GetPress(DIK_A) == true || pKey->GetTrigger(DIK_LEFT) == true)
									m_frotDest = -D3DX_PI * 0.25f + Camerarot.y + D3DX_PI;		// ����
								else if (pKey->GetPress(DIK_D) == true || pKey->GetTrigger(DIK_RIGHT) == true)
									m_frotDest = D3DX_PI * 0.25f + Camerarot.y + D3DX_PI;		// �E��
								else
									m_frotDest = D3DX_PI * 0.0f + Camerarot.y + D3DX_PI;		// ��

								if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
									bWalk = true;	// ������
							}
							else if (pKey->GetPress(DIK_S) == true || pKey->GetTrigger(DIK_DOWN) == true)
							{// �L�[�{�[�h�iS�j�������ꂽ
								if (pKey->GetPress(DIK_A) == true || pKey->GetTrigger(DIK_LEFT) == true)
									m_frotDest = -D3DX_PI * 0.75f + Camerarot.y + D3DX_PI;		// ����
								else if (pKey->GetPress(DIK_D) == true || pKey->GetTrigger(DIK_RIGHT) == true)
									m_frotDest = D3DX_PI * 0.75f + Camerarot.y + D3DX_PI;		// �E��
								else
									m_frotDest = D3DX_PI * 1.0f + Camerarot.y + D3DX_PI;		// ��

								if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
									bWalk = true;	// ������
							}
							else if (pKey->GetPress(DIK_A) == true || pKey->GetTrigger(DIK_LEFT) == true)
							{// �L�[�{�[�h�iA�j�������ꂽ
								if (pKey->GetPress(DIK_W) == true || pKey->GetTrigger(DIK_UP) == true)
									m_frotDest = -D3DX_PI * 0.25f + Camerarot.y + D3DX_PI;		// ����
								else if (pKey->GetPress(DIK_S) == true || pKey->GetTrigger(DIK_DOWN) == true)
									m_frotDest = -D3DX_PI * 0.75f + Camerarot.y + D3DX_PI;		// ����
								else
									m_frotDest = -D3DX_PI * 0.5f + Camerarot.y + D3DX_PI;		// ��

								if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
									bWalk = true;	// ������
							}
							else if (pKey->GetPress(DIK_D) == true || pKey->GetTrigger(DIK_RIGHT) == true)
							{// �L�[�{�[�h�iD�j�������ꂽ
								if (pKey->GetPress(DIK_W) == true || pKey->GetTrigger(DIK_UP) == true)
									m_frotDest = D3DX_PI * 0.25f + Camerarot.y + D3DX_PI;		// �E��
								else if (pKey->GetPress(DIK_S) == true || pKey->GetTrigger(DIK_DOWN) == true)
									m_frotDest = D3DX_PI * 0.75f + Camerarot.y + D3DX_PI;		// �E��
								else
									m_frotDest = D3DX_PI * 0.5f + Camerarot.y + D3DX_PI;		// �E

								if (m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false)
									bWalk = true;	// ������
							}
						}
					}
				}
			}

			if (bWalk == true)
			{// ������
				if (m_bStep == false)
				{// �X�e�b�v���Ă��Ȃ�
					// �ړ�
					m_move.x += sinf(m_frotDest) * MC_MOVE_UP;
					m_move.z += cosf(m_frotDest) * MC_MOVE_UP;
				}
			}
		}
	}

	// ���[�V�����ݒ�
	{
		if (m_bJump == false && m_bAttack_Nor == false && m_bGuard == false && m_bStep == false && m_bSkill == false)
		{// �W�����v���Ă��Ȃ��A�ʏ�U�����Ă��Ȃ��A�K�[�h���Ă��Ȃ��A�X�e�b�v���Ă��Ȃ�
			if (bWalk == true)
			{// ������
				if (m_pMotion->GetMotionNum() != MOTION_WALK)
					m_pMotion->SetMotionNum(MOTION_WALK);		// �������[�V����

				if (m_pMotion->GetMotionKeyNum() % 2 == 0 && m_pMotion->GetCntFrame() == 0)
				{// ���Ԋu��
					// �p�x
					float fAngle = 0.0f;
					if (m_pMotion->GetMotionKeyNum() / 2 == 0) fAngle = m_rot.y + (D3DX_PI * 0.5f);
					else fAngle = m_rot.y - (D3DX_PI * 0.5f);

					// �A�j���[�V�����Đ�
					CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(sinf(fAngle) * 3.0f, 3.0f, cosf(fAngle) * 3.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(4.0f, 4.0f, 0.0f),
						D3DXVECTOR2(0.125f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_WALK));
					pTexAnim->SetAnim(8, 8, 2, 0, false, true);

					if (pSound != NULL)
					{
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WOLK);
					}
				}
			}
			else
			{// �����Ă��Ȃ�
				if (m_pMotion->GetMotionNum() == MOTION_WALK)
				{// �������[�V�����̂�
					if (m_pMotion->GetMotionKeyNum() % 2 == 1 && m_pMotion->GetCntFrame() == m_MotionInfo[MOTION_WALK].aKeyInfo[m_pMotion->GetMotionKeyNum()].nFrame - 1)
					{// �ύX�^�C�~���O����
						m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// �j���[�g�������[�V����

						if (pCamera->GetLockOn() == true)
							m_frotDest = fLockOn + D3DX_PI;		// ���b�N�I������������
					}
				}
				else if (m_pMotion->GetMotionNum() != MOTION_NEUTRAL)
				{// �j���[�g�������[�V�����ȊO��������
					m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// �j���[�g�������[�V����

					if (pCamera->GetLockOn() == true)
						m_frotDest = fLockOn + D3DX_PI;		// ���b�N�I������������
				}
			}
		}
	}
}

//=============================================================================
// �X�e�b�v����										(private)	*** CPlayer ***
//=============================================================================
void CPlayer::Step(void)
{
	CSound			*pSound		= CManager::GetSound();				// �T�E���h�擾

	bool			bStep		= false;							// �X�e�b�v������
	bool			bInput		= false;							// ���͂�����
	CCameraMain		*pCamera	= (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);		// �J�����擾
	D3DXVECTOR3		Camerarot	= pCamera->Getrot();				// �J�����p�x�擾
	float			fLockOn		= pCamera->GetLockOnRot();			// ���b�N�I���p�x�擾
	CInputKeyboard	*pKey		= CManager::GetInputKeyboard();		// �L�[�{�[�h�擾
	CInputMouse		*pMouse		= CManager::GetInputMouse();		// �}�E�X�擾
	CInputXPad		*pXPad		= CManager::GetInputXPad();			// X�p�b�h�擾

	// �X�e�b�v����
	{
		if (m_bMove == true)
		{// �ړ��ł���ꍇ
			if (m_bGuard == false && m_bStep == false)
			{// �K�[�h���Ă��Ȃ��A�X�e�b�v���Ă��Ȃ�
				if ((pKey != NULL && pKey->GetTrigger(DIK_LSHIFT) == true) ||
					(pXPad != NULL && pXPad->GetTrigger(CInputXPad::XPADOTHER_TRIGGER_RIGHT, 0) == true))
				{// NULL�ȊO�̏ꍇ�A�i��SHIFT�AR2�j�������ꂽ
					if (m_bAttack_Nor == true || m_bAttack_Sky == true || m_bSkill == true)
					{// �U�����Ă���
						if (11 < m_nAttackCnt)
						{// ���͒����i�U���j
							bStep = true;		// �X�e�b�v����

							if (m_bAttack_Nor == true)
							{// �ʏ�U�����Ă���
								m_nAttackNum = ATTACK_N_1;	// �O����
								m_bAttack_Nor = false;		// �ʏ�U�����Ă��Ȃ�
							}

							m_bAttack_Sky = false;		// �󒆍U�����Ă��Ȃ�
							m_bAction_Sky = false;		// �󒆃A�N�V�������Ă��Ȃ�

							m_bSkill = false;
							m_brot = false;

							for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
								if (m_pOrbit[nCount] != NULL)
									m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

							m_nStepCnt = 20;		// �X�e�b�v�J�E���g		
							m_bStep = true;		// �X�e�b�v����
						}
					}
					else if (0 < m_nGuardCnt)
					{// �U�����Ă��Ȃ��A���͒����i�K�[�h�j
						if (m_nGuardCnt < 5)
						{// ���͒����i�K�[�h�j
							bStep = true;		// �X�e�b�v����

							m_brot = false;

							m_bGuard = false;	// �K�[�h���Ă��Ȃ�

							m_nStepCnt = 20;		// �X�e�b�v�J�E���g
							m_bStep = true;		// �X�e�b�v����
						}
					}
					else
					{// ���̑�
						m_brot = false;

						m_nStepCnt = 20;		// �X�e�b�v�J�E���g
						m_bStep = true;		// �X�e�b�v����
					}
				}
			}
		}

		if (m_bStep == true)
		{// �X�e�b�v����
			if ((m_bAttack_Nor == false && m_bAttack_Sky == false && m_bSkill == false) || bStep == true)
			{// �U�����Ă��Ȃ��A�X�e�b�v����
				m_nAttackCnt = 0;

				CTexAnim *pTexAnim = NULL;		// �e�N�X�`���A�j���[�V����

				if (m_nStepCnt == 20)
				{// �w�肳�ꂽ�J�E���g��������
					if (m_pMotion->GetMotionNum() != MOTION_LANDING)
						m_pMotion->SetMotionNum(MOTION_LANDING, 0, 3);	//	���n���[�V����
					m_bDraw = false;	// ��\��

					// �A�j���[�V�����Đ�
					pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
						D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
					pTexAnim->SetAnim(10, 10, 3, 0, false, true);

					if (pSound != NULL)
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WORP000);
				}

				m_nStepCnt--;	// �X�e�b�v�J�E���g

				if (10 < m_nStepCnt)
				{// ���J�E���g�ȏゾ������
					// �ړ�
					m_move.x += sinf(m_frotDest) * MC_MOVE_UP * 3.0f;
					m_move.z += cosf(m_frotDest) * MC_MOVE_UP * 3.0f;
				}

				if (m_nStepCnt == 10)
				{// �w�肳�ꂽ�J�E���g��������
					m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// �j���[�g�������[�V����
					m_bDraw = true;		// �\��

					// �A�j���[�V�����Đ�
					pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
						D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
					pTexAnim->SetAnim(10, 10, 3, 0, false, true);

					if (pCamera->GetLockOn() == true)
						m_frotDest = fLockOn + D3DX_PI;		// ���b�N�I������������

					//if (pSound != NULL)
					//	pSound->PlaySoundA(CSound::SOUND_LABEL_SE_WORP000);
				}

				if (m_nStepCnt < 0)
				{// ���J�E���g�ȉ���������
					m_nStepCnt	= 0;		// �X�e�b�v�J�E���g������		
					m_bStep		= false;	// �X�e�b�v���Ă��Ȃ�
				}
			}
			else
			{// �U������
				// �A�j���[�V�����Đ�
				CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
					D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
				pTexAnim->SetAnim(10, 10, 3, 0, false, true);

				m_bDraw		= true;		// �\��
				m_nStepCnt	= 0;		// �X�e�b�v�J�E���g������
				m_bStep		= false;	// �X�e�b�v���Ă��Ȃ�

				m_nAttackCnt = 0;
			}
		}
		else
		{// �X�e�b�v���Ă��Ȃ�
			m_bDraw = true;		// �\��
		}
	}
}

//=============================================================================
// �A�N�V��������										(private)	*** CPlayer ***
//=============================================================================
void CPlayer::Action(void)
{
	CCameraMain		*pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);		// �J�����擾
	float			fLockOn = pCamera->GetLockOnRot();			// ���b�N�I���p�x�擾
	CInputKeyboard	*pKey = CManager::GetInputKeyboard();		// �L�[�{�[�h�擾
	CInputMouse		*pMouse = CManager::GetInputMouse();		// �}�E�X�擾
	CInputXPad		*pXPad = CManager::GetInputXPad();			// X�p�b�h�擾
	CSound			*pSound = CManager::GetSound();				// �T�E���h�擾

	// �ʏ�U��
	{
		if (m_bGuard == false && m_bJump == false && m_bSkill == false)
		{// �K�[�h���Ă��Ȃ��A�W�����v���Ă��Ȃ�
			if (m_bMove == true)
			{// �ړ��ł���ꍇ
				if (m_bNotAttack == false)
				{
					if (m_nStepCnt < 12)
					{// ���͒����i�X�e�b�v�j
						if ((pKey != NULL && pKey->GetTrigger(DIK_B) == true) ||
							(pMouse != NULL && pMouse->GetTrigger(0) == true) ||
							(pXPad != NULL && pXPad->GetTrigger(XINPUT_GAMEPAD_X, 0) == true))
						{// NULL�ȊO�̏ꍇ�A�L�[�{�[�h�iB�j�AX�p�b�h�iX�j�������ꂽ
							if (m_bAttack_Nor == false && m_nAttackRecast == 0)
							{// �ʏ�U�����Ă��Ȃ�
								if (m_pMotion->GetMotionNum() != MOTION_ATTACK_N)
									m_pMotion->SetMotionNum(MOTION_ATTACK_N);		// �ʏ�U�����[�V����
								m_nAttackNum = ATTACK_N_1;			// �P����
								m_bAttack_Nor = true;				// �ʏ�U������

								m_nAttackRecast = 8;				// ���L���X�g��t����

								m_brot = false;

								if (pCamera->GetLockOn() == true)
									m_frotDest = fLockOn + D3DX_PI;		// ���b�N�I������������
								//m_frotDest = cameraRot.y + D3DX_PI;
							}
							else
							{// �ʏ�U������
								if (m_pMotion->GetMotionNum() == MOTION_ATTACK_N)
								{// �ʏ�U�����[�V�����̂�
									if (m_nAttackNum == ATTACK_N_1)
									{// �P����
										if (m_pMotion->GetMotionKeyNum() <= 1)
										{// ���t���[���ȓ���������
											m_nAttackNum = ATTACK_N_2;		// �Q����
										}
									}
									else if (m_nAttackNum == ATTACK_N_2)
									{// �Q����
										if (m_pMotion->GetMotionKeyNum() <= 3)
										{// ���t���[���ȓ���������
											m_nAttackNum = ATTACK_N_3;		// �R����
											m_nAttackRecast = 0;			// ���L���X�g��������
										}
									}
								}
							}
						}
					}
				}
			}

			if (m_bAttack_Nor == true)
			{// �ʏ�U�����Ă���
				if (m_pMotion->GetMotionNum() == MOTION_ATTACK_N)
				{// �ʏ�U�����[�V�����̂�
					if (pCamera->GetLockOn() == true)
						m_frotDest = fLockOn + D3DX_PI;		// ���b�N�I������������
					//m_frotDest = cameraRot.y + D3DX_PI;

					// �G�t�F�N�g
					if (m_pMotion->GetMotionKeyNum() == ATTACK_SHOT_1 && m_pMotion->GetCntFrame() == 0)
					{// �P��
						CEffect::Create(CEffect::TYPE_3D, CEffect::SHOT_PLAYER, m_pos + D3DXVECTOR3(0.0f, 14.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(m_rot.y) * 8.0f, 0.0f, cosf(m_rot.y) * 8.0f), D3DXVECTOR3(7.0f, 7.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SHOT000);
					}
					if (m_pMotion->GetMotionKeyNum() == ATTACK_SHOT_2 && m_pMotion->GetCntFrame() == 0)
					{// �Q��
						CEffect::Create(CEffect::TYPE_3D, CEffect::SHOT_PLAYER, m_pos + D3DXVECTOR3(0.0f, 14.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(m_rot.y) * 8.0f, 0.0f, cosf(m_rot.y) * 8.0f), D3DXVECTOR3(7.0f, 7.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SHOT000);
					}
					if (m_pMotion->GetMotionKeyNum() == ATTACK_SHOT_3 && m_pMotion->GetCntFrame() == 0)
					{// �R��
						CEffect::Create(CEffect::TYPE_C01, CEffect::SHOT_PLAYER, m_pos + D3DXVECTOR3(0.0f, 14.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(m_rot.y) * 8.0f, 0.0f, cosf(m_rot.y) * 8.0f), D3DXVECTOR3(9.0f, 9.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SHOT000);
					}

					if (m_pMotion->GetMotionKeyNum() <= ATTACK_SHOT_1)
					{// �P��
						if (m_pOrbit[ORBIT_LEFT] != NULL)
							m_pOrbit[ORBIT_LEFT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
						if (m_pOrbit[ORBIT_RIGHT] != NULL)
							m_pOrbit[ORBIT_RIGHT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
					}
					else if (m_pMotion->GetMotionKeyNum() <= ATTACK_SHOT_2)
					{// �Q��
						if (m_pOrbit[ORBIT_LEFT] != NULL)
							m_pOrbit[ORBIT_LEFT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
						if (m_pOrbit[ORBIT_RIGHT] != NULL)
							m_pOrbit[ORBIT_RIGHT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
					}
					else
					{// �R��
						if (m_pOrbit[ORBIT_LEFT] != NULL)
							m_pOrbit[ORBIT_LEFT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
						if (m_pOrbit[ORBIT_RIGHT] != NULL)
							m_pOrbit[ORBIT_RIGHT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));
					}

					if (m_pMotion->GetMotionKeyNum() == m_nAttackNum)
					{// �w��t���[���ɂȂ�����
						if (m_pMotion->GetMotionNum() != MOTION_NEUTRAL)
							m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// �j���[�g�������[�V����
						m_nAttackNum	= ATTACK_N_1;		// �O����
						m_bAttack_Nor	= false;			// �ʏ�U�����Ă��Ȃ�
						m_nAttackCnt	= 0;

						for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
							if (m_pOrbit[nCount] != NULL)
								m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

						m_brot = false;

						if (m_bStep == true)
						{// �X�e�b�v���Ă���
							// �A�j���[�V�����Đ�
							CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
								D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
							pTexAnim->SetAnim(10, 10, 3, 0, false, true);

							m_bDraw		= true;		// �\��
							m_nStepCnt	= 0;		// �X�e�b�v�J�E���g������
							m_bStep		= false;	// �X�e�b�v���Ă��Ȃ�
						}
					}
				}
				m_nAttackCnt++;
			}
			else
			{
				m_nAttackCnt = 0;
			}
		}
		
		if (0 < m_nAttackRecast)
		{// �O��葽��������
			m_nAttackRecast--;		// ���炷
		}
	}

	// �󒆍U��
	{
		if (m_bGuard == false && m_bJump == true && m_bSkill == false)
		{// �K�[�h���Ă��Ȃ��A�W�����v���Ă���
			if (m_bMove == true)
			{// �ړ��ł���ꍇ
				if (m_bNotAttack == false)
				{
					if (m_bAction_Sky == false)
					{// �󒆃A�N�V�������Ă��Ȃ�
						if (m_nStepCnt < 12)
						{// ���͒����i�X�e�b�v�j
							if ((pKey != NULL && pKey->GetTrigger(DIK_B) == true) ||
								(pMouse != NULL && pMouse->GetTrigger(0) == true) ||
								(pXPad != NULL && pXPad->GetTrigger(XINPUT_GAMEPAD_X, 0) == true))
							{// NULL�ȊO�̏ꍇ�A�L�[�{�[�h�iB�j�AX�p�b�h�iX�j�������ꂽ
								if (m_pMotion->GetMotionNum() != MOTION_ATTACK_S)
									m_pMotion->SetMotionNum(MOTION_ATTACK_S);		// �󒆍U�����[�V����
								m_bAttack_Sky = true;			// �󒆍U������
								m_bAction_Sky = true;			// �󒆃A�N�V��������

								for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
									if (m_pOrbit[nCount] != NULL)
										m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));

								m_brot = false;

								if (pCamera->GetLockOn() == true)
									m_frotDest = fLockOn + D3DX_PI;		// ���b�N�I������������
								//m_frotDest = cameraRot.y + D3DX_PI;
							}
						}
					}
				}
			}

			if (m_bAttack_Sky == true)
			{// �󒆍U�����Ă���
				if (m_pMotion->GetMotionNum() == MOTION_ATTACK_S)
				{// �󒆍U�����[�V�����̂�
					if (pCamera->GetLockOn() == true)
						m_frotDest = fLockOn + D3DX_PI;		// ���b�N�I������������

					// �G�t�F�N�g
					if (m_pMotion->GetMotionKeyNum() == m_MotionInfo[MOTION_ATTACK_S].nKeyMax - 2 && m_pMotion->GetCntFrame() == 0)
					{
						CEffect_C01 *pEffect = (CEffect_C01*)CEffect::Create(CEffect::TYPE_C01, CEffect::SHOT_PLAYER, m_pos + D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(sinf(m_rot.y) * 6.0f, -5.0f, cosf(m_rot.y) * 6.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						pEffect->SetExplosion(true);

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SHOT000);
					}

					if (m_pMotion->GetMotionKeyNum() == m_MotionInfo[MOTION_ATTACK_S].nKeyMax - 1)
					{// �w��t���[����������
						if (m_pMotion->GetMotionNum() != MOTION_LANDING)
							m_pMotion->SetMotionNum(MOTION_LANDING);	// ���n���[�V����
						m_bAttack_Sky = false;			// �󒆍U�����Ă��Ȃ�
						m_nAttackCnt = 0;

						for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
							if (m_pOrbit[nCount] != NULL)
								m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

						m_brot = false;

						if (m_bStep == true)
						{// �X�e�b�v���Ă���
							// �A�j���[�V�����Đ�
							CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
								D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
							pTexAnim->SetAnim(10, 10, 3, 0, false, true);

							m_bDraw		= true;		// �\��
							m_nStepCnt	= 0;		// �X�e�b�v�J�E���g������
							m_bStep		= false;	// �X�e�b�v���Ă��Ȃ�
						}	
					}
				}
				m_nAttackCnt++;
			}
			else
			{
				m_nAttackCnt = 0;
			}
		}
	}

	// �X�L��
	{
		if (m_bGuard == false)
		{// �K�[�h���Ă��Ȃ��A�W�����v���Ă��Ȃ�
			if (m_bMove == true)
			{// �ړ��ł���ꍇ
				if (m_bNotSkill == false)
				{
					if (m_bSkill == false)
					{// �X�L�����Ă��Ȃ�
						if (m_nStepCnt < 12)
						{// ���͒����i�X�e�b�v�j
							if (m_nSkillRecast == 0)
							{// �X�L�����L���X�g���O��������
								if ((pKey != NULL && pKey->GetTrigger(DIK_N) == true) ||
									(pMouse != NULL && pMouse->GetTrigger(1) == true) ||
									(pXPad != NULL && pXPad->GetTrigger(XINPUT_GAMEPAD_Y, 0) == true))
								{// NULL�ȊO�̏ꍇ�A�L�[�{�[�h�iB�j�AX�p�b�h�iX�j�������ꂽ
									if (m_pMotion->GetMotionNum() != MOTION_ATTACK_SKILL_1)
										m_pMotion->SetMotionNum(MOTION_ATTACK_SKILL_1);		// �K�[�h���[�V����

									m_bSkill = true;
									m_nSkillRecast = 30;

									AddMP(-20);								// MP����

									if (m_pOrbit[ORBIT_RIGHT] != NULL)
										m_pOrbit[ORBIT_RIGHT]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 0.0f));

									if (m_bAttack_Nor == true)
									{// �ʏ�U�����Ă���
										m_nAttackNum = ATTACK_N_1;			// �O����
										m_bAttack_Nor = false;				// �ʏ�U�����Ă��Ȃ�
										m_nAttackCnt = 0;
									}

									if (m_bAttack_Sky == true)
									{// �󒆍U�����Ă���
										m_bAttack_Sky = false;				// �󒆍U�����Ă��Ȃ�
									}

									if (m_bJump == true)
									{// �W�����v���Ă���
										m_move.y = 7.0f;					// �����㏸
									}

									m_brot = false;

									if (pCamera->GetLockOn() == true)
										m_frotDest = fLockOn + D3DX_PI;		// ���b�N�I������������

									if (pSound != NULL)
										pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SKILL001);
								}
							}
						}
					}
				}
			}

			if (m_bSkill == true)
			{// �X�L���g���Ă���
				if (m_pMotion->GetMotionNum() == MOTION_ATTACK_SKILL_1)
				{// �󒆍U�����[�V�����̂�
					if (pCamera->GetLockOn() == true)
						m_frotDest = fLockOn + D3DX_PI;		// ���b�N�I������������
			
					if (m_pMotion->GetMotionKeyNum() == m_MotionInfo[MOTION_ATTACK_SKILL_1].nKeyMax - 3)
					{
						m_brot = true;
						m_frotDest = m_rot.y;
					}

					if (m_pMotion->GetMotionKeyNum() == m_MotionInfo[MOTION_ATTACK_SKILL_1].nKeyMax - 2 && m_pMotion->GetCntFrame() == 0)
					{
						CEffect::Create(CEffect::TYPE_C03, CEffect::SHOT_PLAYER, D3DXVECTOR3(m_pModel[MODEL_04_handR].GetmtxWorld()._41, m_pos.y + 1.8f, m_pModel[MODEL_04_handR].GetmtxWorld()._43), m_rot,
							D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(80.0f, 0.0f, 80.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SKILL000);
					}

					if (m_pMotion->GetMotionKeyNum() == m_MotionInfo[MOTION_ATTACK_SKILL_1].nKeyMax - 1)
					{// �w��t���[����������
						if (m_pMotion->GetMotionNum() != MOTION_NEUTRAL)
							m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// �j���[�g�������[�V����

						m_bSkill = false;

						for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
							if (m_pOrbit[nCount] != NULL)
								m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

						m_brot = false;

						if (m_bStep == true)
						{// �X�e�b�v���Ă���
						 // �A�j���[�V�����Đ�
							CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
								D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
							pTexAnim->SetAnim(10, 10, 3, 0, false, true);

							m_bDraw = true;		// �\��
							m_nStepCnt = 0;		// �X�e�b�v�J�E���g������
							m_bStep = false;	// �X�e�b�v���Ă��Ȃ�
						}
					}
				}
				m_nAttackCnt++;
			}
			else
			{
				m_nAttackCnt = 0;
			}
		}

		if (0 < m_nSkillRecast)
		{
			m_nSkillRecast--;
		}
	}

	// �K�[�h
	{
		if ((pKey != NULL && pKey->GetPress(DIK_V) == true) ||
			(pMouse != NULL && pMouse->GetPress(2) == true) ||
			(pXPad != NULL && pXPad->GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER, 0) == true))
		{// NULL�ȊO�̏ꍇ�A�L�[�{�[�h�iV�j�AX�p�b�h�iR1�j�������ꂽ
			if (m_bMove == true)
			{// �ړ��ł���ꍇ
				if (m_bGuard == false)
				{// �K�[�h���Ă��Ȃ�
					if (m_pMotion->GetMotionNum() != MOTION_GUARD)
					{// �K�[�h���[�V�����ȊO
						m_pMotion->SetMotionNum(MOTION_GUARD);		// �K�[�h���[�V����
						m_pMotion->SetKeyStop(true);				// �ŏI�L�[�Œ�~
					}
					m_bGuard = true;				// �K�[�h���Ă���
					m_fDamageCat = 0.3f;			// �_���[�W�J�b�g

					if (m_bJump == true)
					{// �W�����v���Ă���
						m_bAttack_Sky = false;		// �󒆍U�����Ă��Ȃ�
					}

					m_nAttackNum = ATTACK_N_1;		// �O����
					m_bAttack_Nor = false;			// �ʏ�U�����Ă��Ȃ�
					m_bAttack_Sky = false;			// �󒆍U�����Ă��Ȃ�
					m_bSkill = false;
					m_nAttackCnt = 0;

					for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
						if (m_pOrbit[nCount] != NULL)
							m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

					m_brot = false;

					if (m_pGuardDome[GUARD_DOME_UP] == NULL)
					{// NULL�̏ꍇ
						m_pGuardDome[GUARD_DOME_UP] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_GUARD);

						m_pGuardDome[GUARD_DOME_UP]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(20.0f, 10.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f),
							D3DXVECTOR3(D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.5f));
						m_pGuardDome[GUARD_DOME_UP]->Init();
						m_pGuardDome[GUARD_DOME_UP]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_WARP));
						m_pGuardDome[GUARD_DOME_UP]->DrawEdit(false, false, false, false, true);
					}
					if (m_pGuardDome[GUARD_DOME_DOWN] == NULL)
					{// NULL�̏ꍇ
						m_pGuardDome[GUARD_DOME_DOWN] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_GUARD);

						m_pGuardDome[GUARD_DOME_DOWN]->SetInitAll(CSceneMesh::MESHTYPE_DOME, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(20.0f, 10.0f, 0.0f), m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f),
							D3DXVECTOR3(-D3DX_PI * 0.5f, m_rot.y, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 0.5f));
						m_pGuardDome[GUARD_DOME_DOWN]->Init();
						m_pGuardDome[GUARD_DOME_DOWN]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_WARP));
						m_pGuardDome[GUARD_DOME_DOWN]->DrawEdit(false, false, false, true, true);
					}
					m_GuardDomeUV = D3DXVECTOR2(0.0f, 0.0f);

					if (m_bStep == true)
					{// �X�e�b�v���Ă���
						// �A�j���[�V�����Đ�
						CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
							D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
						pTexAnim->SetAnim(10, 10, 3, 0, false, true);

						m_bDraw = true;		// �\��
						m_nStepCnt = 0;		// �X�e�b�v�J�E���g������
						m_bStep = false;	// �X�e�b�v���Ă��Ȃ�
					}
				}
				else
				{
					if (m_bGuard == true)
					{// �K�[�h���Ă���
						if (m_nCntJastGuard < 5)
							m_fDamageCat = 0.3f;			// �_���[�W�J�b�g
						else
							m_fDamageCat = 0.5f;			// �_���[�W�J�b�g					

						if (m_nCntJastGuard < 5)
							m_nCntJastGuard++;

						if (m_pGuardDome[GUARD_DOME_UP] != NULL)
						{// NULL�ȊO�̏ꍇ
							m_pGuardDome[GUARD_DOME_UP]->Setpos(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f));
							m_pGuardDome[GUARD_DOME_UP]->SetTex(-m_GuardDomeUV);
						}
						if (m_pGuardDome[GUARD_DOME_DOWN] != NULL)
						{// NULL�ȊO�̏ꍇ
							m_pGuardDome[GUARD_DOME_DOWN]->Setpos(m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f));
							m_pGuardDome[GUARD_DOME_DOWN]->SetTex(m_GuardDomeUV);
						}
						m_GuardDomeUV.y += 0.02f;
					}
				}
			}
		}
		else
		{// �K�[�h������
			if (m_bGuard == true)
			{// �K�[�h���Ă���
				if (m_pMotion->GetMotionNum() != MOTION_NEUTRAL)
					m_pMotion->SetMotionNum(MOTION_NEUTRAL);	// �j���[�g�������[�V����
				m_pMotion->SetKeyStop(false);		// �ŏI�L�[�Œ�~����
				m_bGuard		= false;			// �K�[�h���Ă��Ȃ�
				m_nCntJastGuard = 0;				// �W���X�g�K�[�h�J�E���g
				m_fDamageCat	= 1.0f;				// �_���[�W�J�b�g
				m_nAttackNum	= ATTACK_N_1;		// �O����
				m_bAttack_Nor	= false;			// �ʏ�U�����Ă��Ȃ�
				m_bAttack_Sky	= false;			// �󒆍U�����Ă��Ȃ�
				m_bSkill = false;
				m_nAttackCnt = 0;

				for (int nCount = 0; nCount < ORBIT_MAX; nCount++)
					if (m_pOrbit[nCount] != NULL)
						m_pOrbit[nCount]->Setcol(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

				m_brot = false;

				if (m_pGuardDome[GUARD_DOME_UP] != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pGuardDome[GUARD_DOME_UP]->Uninit();		// �I������
					m_pGuardDome[GUARD_DOME_UP] = NULL;			// NULL��
				}
				if (m_pGuardDome[GUARD_DOME_DOWN] != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pGuardDome[GUARD_DOME_DOWN]->Uninit();	// �I������
					m_pGuardDome[GUARD_DOME_DOWN] = NULL;		// NULL��
				}
			}
		}

		// �K�[�h�t���[��
		if (m_bGuard == true)
			m_nGuardCnt = 10;		// �K�[�h�J�E���g�ݒ�
		else
			if (0 < m_nGuardCnt)	// ���t���[���ȏゾ������
				m_nGuardCnt--;			// �K�[�h�J�E���g
	}
}

//=============================================================================
// �W�����v����										(private)	*** CPlayer ***
//=============================================================================
void CPlayer::Jump(void)
{
	CSound			*pSound = CManager::GetSound();				// �T�E���h�擾

	CInputKeyboard	*pKey = CManager::GetInputKeyboard();		// �L�[�{�[�h�擾
	CInputXPad		*pXPad = CManager::GetInputXPad();			// X�p�b�h�擾

	// �W�����v����
	{
		if (m_bMove == true)
		{// �ړ��ł���ꍇ
			if (m_bJump == false && m_bGuard == false && m_bSkill == false)
			{// �W�����v���Ă��Ȃ��A�K�[�h���Ă��Ȃ�
				if (m_nStepCnt < 12)
				{// ���͒����i�X�e�b�v�j
					if ((pKey != NULL && pKey->GetTrigger(DIK_SPACE) == true) ||
						(pXPad != NULL && pXPad->GetTrigger(XINPUT_GAMEPAD_A, 0) == true))
					{// NULL�ȊO�̏ꍇ�A�L�[�{�[�h�i�X�y�[�X�j�AX�p�b�h�iA�j�������ꂽ
						if (m_pMotion->GetMotionNum() != MOTION_JUMP)
							m_pMotion->SetMotionNum(MOTION_JUMP);		// �W�����v���[�V����
						m_move.y = MC_JUMP;		// �㏸
						m_bJump = true;			// �W�����v����

						if (m_bAttack_Nor == true)
						{// �ʏ�U�����Ă���
							m_nAttackNum = ATTACK_N_1;	// �O����
							m_bAttack_Nor = false;		// �ʏ�U�����Ă��Ȃ�
						}

						if (m_bStep == true)
						{// �X�e�b�v���Ă���
							// �A�j���[�V�����Đ�
							CTexAnim *pTexAnim = CTexAnim::Create(CTexAnim::TYPE_3D_BILL, m_pos + D3DXVECTOR3(0.0f, 20.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(20.0f, 20.0f, 0.0f),
								D3DXVECTOR2(0.1f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_STEP));
							pTexAnim->SetAnim(10, 10, 3, 0, false, true);

							m_bDraw = true;		// �\��
							m_nStepCnt = 0;		// �X�e�b�v�J�E���g������
							m_bStep = false;	// �X�e�b�v���Ă��Ȃ�
						}

						if (pSound != NULL)
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_JUMP);
					}
				}
			}
		}
	}

	// ���[�V�����ݒ�
	{
		if (m_bJump == true && m_bGuard == false && m_bSkill == false)
		{// �W�����v���Ă���A�K�[�h���Ă��Ȃ�
			if (m_move.y < 0.0f)	// ���~���Ă���
			{
				if (m_pMotion->GetMotionNum() != MOTION_LANDING)
					m_pMotion->SetMotionNum(MOTION_LANDING);	//	���n���[�V����
			}
		}
	}
}

//=============================================================================
// ����`�F�b�N����									(private)	*** CPlayer ***
//=============================================================================
void CPlayer::ColisionChack(float *fHeight)
{
	CSound *pSound = CManager::GetSound();

	CScene *pScene		= NULL;		// �V�[��
	CScene *pSceneNext	= NULL;		// ���V�[��

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

	// �G
	{
		if (m_bStep == false || (m_bStep == true && m_nStepCnt < 15))
		{// �X�e�b�v���Ă��Ȃ��A�i�X�e�b�v���Ă���A���t���[���ȉ���������j
			pScene = CScene::GetScene(CScene::PRIORITY_3);	// �V�[���擾

			while (pScene != NULL)
			{// NULL�ȊO�̏ꍇ
				pSceneNext = pScene->GetSceneNext();		// ���V�[�����L��

				if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
				{// �I�u�W�F�N�g�^�C�v���G�̏ꍇ
					CEnemy *pEnemy = (CEnemy*)pScene;		// �G�̌^�ɃL���X�g
					pEnemy->Collision(&m_pos, &m_posOld, &m_move, m_vtxMax, m_vtxMin);
				}
				pScene = pSceneNext;	// ���V�[����
			}
		}
	}

	CInputKeyboard *pKey = CManager::GetInputKeyboard();
	CInputXPad *pXPad = CManager::GetInputXPad();
	CFade::FADE fade = CFade::GetFade();

	// �}�b�v
	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_GAME)
	{// �Q�[���̂�
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// NULL�ȊO�̏ꍇ
			CMap *pMap = pGame->GetMap();
			if (pMap != NULL)
			{// NULL�ȊO�̏ꍇ
				int nMapNum = pGame->GetMapNum();
				if (nMapNum == 2)
				{// �}�b�v�Q
					CMap_02 *pMap_02 = (CMap_02*)pMap;

					if (m_bWarp == false)
						pMap_02->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f);		// �}�b�v�����蔻��

					if (m_bWarp == false)
					{// ���[�v���Ă��Ȃ�
						bool bWarpIn = false;
						if (pMap_02->CollisionWarp(&m_pos, &m_posWarp) == true)
						{// ���[�v���ɂ��邩
							*fHeight = 6.0f;
							bWarpIn = true;
						}

						if ((pKey->GetTrigger(DIK_Z) == true || pKey->GetTrigger(DIK_E) == true || pKey->GetTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true) && m_bStep == false)
						{// �A�N�V������������
							if (bWarpIn == true)
							{
								m_bWarp = true;			// ���[�v����
								m_bMove = false;		// �ړ�����

								if (pSound != NULL)
								{// NULL�ȊO�̏ꍇ
									pSound->PlaySoundA(CSound::SOUND_LABEL_SE_EREA_WORP);
								}
							}
						}
					}

					if (m_bWarp == true)
					{// ���[�v����
						CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);

						if (m_nCntWarp < 30)
						{// ���t���[���܂�
							if (0.0f < m_fcol_a)
							{
								for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
									m_pModel[nCntModel].Setcol_a(m_fcol_a);				// �F�ݒ�

								m_fcol_a -= (float)(1.0f / (float)30);
							}
						}
						else if (m_nCntWarp < 120)
						{// ���t���[���܂�
							if (m_nCntWarp == 30)
							{// ���t���[����������
								m_fWarpLenght = sqrtf(powf(m_posWarp.x - m_pos.x, 2.0f) + powf(m_posWarp.z - m_pos.z, 2.0f));
								m_fWarpAngle = atan2f(m_posWarp.x - m_pos.x, m_posWarp.z - m_pos.z);								

								pCamera->SetMoveCollision(false);

								pCamera->SetDistance(0, pCamera->GetDistance(0) + 500.0f);
								pCamera->Setrot(D3DXVECTOR3(-0.9f, pCamera->Getrot().y, pCamera->Getrot().z));
							}

							if (40 < m_nCntWarp && m_nCntWarp < 110)
							{// ���t���[���̊�
								m_pos.x += sinf(m_fWarpAngle) * (m_fWarpLenght / (float)(110 - 40));
								m_pos.z += cosf(m_fWarpAngle) * (m_fWarpLenght / (float)(110 - 40));
							}
							
							if (m_nCntWarp == 110)
							{// ���t���[����������
								m_pos = m_posWarp;

								pCamera->SetMoveCollision(true);
							}

							m_fcol_a = 0.0f;
							for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
								m_pModel[nCntModel].Setcol_a(m_fcol_a);					// �F�ݒ�							
						}
						else if (m_nCntWarp < 150)
						{// ���t���[���܂�
							if (m_nCntWarp == 120)
							{// ���t���[����������
								m_bMove = true;
							}

							if (m_fcol_a < 1.0f)
							{
								for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
									m_pModel[nCntModel].Setcol_a(m_fcol_a);				// �F�ݒ�

								m_fcol_a += (float)(1.0f / (float)30);
							}
						}

						m_nCntWarp++;

						if (m_nCntWarp == 150)
						{// ���t���[����������
							m_nCntWarp = 0;
							m_bWarp = false;
						}
					}

					if (pMap_02->CollisionGoal(&m_pos) == true)
					{// �S�[���ɂ��邩
						if (fade == CFade::FADE_NONE)
						{// �t�F�[�h���Ă��Ȃ�
							if ((pKey->GetTrigger(DIK_Z) == true || pKey->GetTrigger(DIK_E) == true || pKey->GetTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true) && m_bStep == false)
							{// �A�N�V������������
								CGame::GAME game = pGame->GetGame();
								if (game < CGame::GAME_PLAY_B)
								{// �{�X���O��������
									game = (CGame::GAME)(game + 1);
									CFade::Create(game);
									m_bMove = false;

									if (pSound != NULL)
									{// NULL�ȊO�̏ꍇ
										pSound->PlaySoundA(CSound::SOUND_LABEL_SE_EREA_WORP);
									}
								}
							}
						}
					}
				}
				else if (nMapNum == 3)
				{// �}�b�v�R
					CMap_03 *pMap_03 = (CMap_03*)pMap;

					if (m_bWarp == false)
						pMap_03->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f);		// �}�b�v�����蔻��

					if (m_bWarp == false)
					{// ���[�v���Ă��Ȃ�
						bool bWarpIn = false;
						if (pMap_03->CollisionWarp(&m_pos, &m_posWarp) == true)
						{// ���[�v���ɂ��邩
							*fHeight = 6.0f;
							bWarpIn = true;
						}

						if ((pKey->GetTrigger(DIK_Z) == true || pKey->GetTrigger(DIK_E) == true || pKey->GetTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true) && m_bStep == false)
						{// �A�N�V������������
							if (bWarpIn == true)
							{
								m_bWarp = true;			// ���[�v����
								m_bMove = false;		// �ړ�����

								if (pSound != NULL)
								{// NULL�ȊO�̏ꍇ
									pSound->PlaySoundA(CSound::SOUND_LABEL_SE_EREA_WORP);
								}
							}
						}
					}

					if (m_bWarp == true)
					{// ���[�v����
						CCameraMain *pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);

						if (m_nCntWarp < 30)
						{// ���t���[���܂�
							if (0.0f < m_fcol_a)
							{
								for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
									m_pModel[nCntModel].Setcol_a(m_fcol_a);				// �F�ݒ�

								m_fcol_a -= (float)(1.0f / (float)30);
							}
						}
						else if (m_nCntWarp < 120)
						{// ���t���[���܂�
							if (m_nCntWarp == 30)
							{// ���t���[����������
								m_fWarpLenght = sqrtf(powf(m_posWarp.x - m_pos.x, 2.0f) + powf(m_posWarp.z - m_pos.z, 2.0f));
								m_fWarpAngle = atan2f(m_posWarp.x - m_pos.x, m_posWarp.z - m_pos.z);

								pCamera->SetMoveCollision(false);

								pCamera->SetDistance(0, pCamera->GetDistance(0) + 500.0f);
								pCamera->Setrot(D3DXVECTOR3(-0.9f, pCamera->Getrot().y, pCamera->Getrot().z));
							}

							if (40 < m_nCntWarp && m_nCntWarp < 110)
							{// ���t���[���̊�
								m_pos.x += sinf(m_fWarpAngle) * (m_fWarpLenght / (float)(110 - 40));
								m_pos.z += cosf(m_fWarpAngle) * (m_fWarpLenght / (float)(110 - 40));
							}

							if (m_nCntWarp == 110)
							{// ���t���[����������
								m_pos = m_posWarp;

								pCamera->SetMoveCollision(true);
							}

							m_fcol_a = 0.0f;
							for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
								m_pModel[nCntModel].Setcol_a(m_fcol_a);					// �F�ݒ�							
						}
						else if (m_nCntWarp < 150)
						{// ���t���[���܂�
							if (m_nCntWarp == 120)
							{// ���t���[����������
								m_bMove = true;
							}

							if (m_fcol_a < 1.0f)
							{
								for (int nCntModel = 0; nCntModel < m_MaxModel; nCntModel++)
									m_pModel[nCntModel].Setcol_a(m_fcol_a);				// �F�ݒ�

								m_fcol_a += (float)(1.0f / (float)30);
							}
						}

						m_nCntWarp++;

						if (m_nCntWarp == 150)
						{// ���t���[����������
							m_nCntWarp = 0;
							m_bWarp = false;
						}
					}

					if (pMap_03->CollisionGoal(&m_pos) == true)
					{// �S�[���ɂ��邩
						if (fade == CFade::FADE_NONE)
						{// �t�F�[�h���Ă��Ȃ�
							if ((pKey->GetTrigger(DIK_Z) == true || pKey->GetTrigger(DIK_E) == true || pKey->GetTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true) && m_bStep == false)
							{// �A�N�V������������
								CGame::GAME game = pGame->GetGame();
								if (game < CGame::GAME_PLAY_B)
								{// �{�X���O��������
									game = (CGame::GAME)(game + 1);
									CFade::Create(game);
									m_bMove = false;

									if (pSound != NULL)
									{// NULL�ȊO�̏ꍇ
										pSound->PlaySoundA(CSound::SOUND_LABEL_SE_EREA_WORP);
									}
								}
							}
						}
					}
				}
				else if (nMapNum == 4)
				{// �}�b�v�S
					CMap_04 *pMap_04 = (CMap_04*)pMap;

					if (m_bWarp == false)
						pMap_04->Collision(&m_pos, &m_posOld, &m_move, &m_rot, 100.0f);		// �}�b�v�����蔻��

					CGame::GAME game = pGame->GetGame();
					if (game == CGame::GAME_TUTORIAL)
					{// �`���[�g���A����������
						if (pMap_04->GetGoal() == true)
						{// �S�[��������
							if (pMap_04->CollisionGoal(&m_pos) == true)
							{// �S�[���ɂ�����
								if ((pKey->GetTrigger(DIK_Z) == true || pKey->GetTrigger(DIK_E) == true || pKey->GetTrigger(DIK_RETURN) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true) && m_bStep == false)
								{// �A�N�V������������
									if (game < CGame::GAME_PLAY_B)
									{// �{�X���O��������
										game = (CGame::GAME)(game + 1);
										CFade::Create(game);
										m_bMove = false;

										if (pSound != NULL)
										{// NULL�ȊO�̏ꍇ
											pSound->PlaySoundA(CSound::SOUND_LABEL_SE_EREA_WORP);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// �Փ˔��菈��									(private)	*** CPlayer ***
//=============================================================================
bool CPlayer::Collision(D3DXVECTOR3 pos)
{
	bool bLand = false;

	float fRange = sqrtf(powf(pos.x - m_pos.x, 2.0f) + powf(pos.z - m_pos.z, 2.0f));
	if (fRange < m_vtxMax.x)
	{
		bLand = true;
	}

	return bLand;
}