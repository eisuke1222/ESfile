//=============================================================================
//
// �v���C���[���� [player.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene.h"		// �V�[��
#include "motion.h"		// ���[�V����
#include "fade.h"		// �t�F�[�h
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CSceneMesh;		// ���b�V��
class CSceneMeshOrbit;	// �I�[�r�b�g
class CGauge;			// �Q�[�W
class CMapIcon;			// �}�b�v�A�C�R��
class CGauge;			// �Q�[�W

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CPlayer : public CScene
{// �v���C���[�i�e�FCScene�j
public:
	typedef enum
	{// ���
		STATE_NORMAL = 0,		// �ʏ�
		STATE_DAMAGE,			// �_���[�W
		STATE_DEATH,			// ����
		STATE_MAX				// �ő吔
	}STATE;

	typedef enum
	{// ���f��
		MODEL_00_body = 0,		// ��
		MODEL_01_head,			// ��
		MODEL_02_hat,			// �X�q
		MODEL_03_none,			// �E�r
		MODEL_04_handR,			// �E��
		MODEL_05_fingerR_1,		// �E��w�P
		MODEL_06_fingerR_2,		// �E��w�Q
		MODEL_07_fingerR_3,		// �E��w�R
		MODEL_08_fingerR_4,		// �E��w�S
		MODEL_09_none,			// ���r
		MODEL_10_handL,			// ����
		MODEL_11_fingerL_1,		// ����w�P
		MODEL_12_fingerL_2,		// ����w�Q
		MODEL_13_fingerL_3,		// ����w�R
		MODEL_14_fingerL_4,		// ����w�S
		MODEL_15_none,			// �E��
		MODEL_16_legR,			// �E��
		MODEL_17_none,			// ����
		MODEL_18_legL,			// ����
		MODEL_MAX				// �ő吔
	}MODEL;

	typedef enum
	{// ���[�V����
		MOTION_NEUTRAL = 0,		// �j���[�g����
		MOTION_WALK,			// ����
		MOTION_JUMP,			// �W�����v
		MOTION_LANDING,			// ���n
		MOTION_GUARD,			// �K�[�h
		MOTION_DEATH,			// ����
		MOTION_DAMAGE_1,		// �_���[�W�P
		MOTION_DAMAGE_2,		// �_���[�W�Q�i������сj
		MOTION_RECOVERY_1,		// �N���オ��P
		MOTION_RECOVERY_2,		// �N���オ��Q�i�N�C�b�N�j
		MOTION_ATTACK_N,		// �ʏ�U���i�R���j
		MOTION_ATTACK_S,		// �󒆍U��
		MOTION_ATTACK_SKILL_1,	// �X�L���U���P
		MOTION_ATTACK_SPECIAL_1,// �X�y�V�����U���P
		MOTION_MAX				// �ő吔
	}MOTION;

	typedef enum
	{// �ʏ�U��
		ATTACK_N_1		= 1,	// �P���ڃL�[�ԍ�
		ATTACK_N_2		= 3,	// �Q���ڃL�[�ԍ�
		ATTACK_N_3		= 6,	// �R���ڃL�[�ԍ�
		ATTACK_SHOT_1	= 1,	// �P���ڔ��˔ԍ�
		ATTACK_SHOT_2	= 3,	// �Q���ڔ��˔ԍ�
		ATTACK_SHOT_3	= 5,	// �R���ڔ��˔ԍ�
		ATTACK_N_MAX	= 3		// �ő�U����
	}ATTACK_N;

	typedef enum
	{
		GUARD_DOME_UP = 0,
		GUARD_DOME_DOWN,
		GUARD_DOME_MAX
	}ORBIT;

	typedef enum
	{
		ORBIT_RIGHT = 0,
		ORBIT_LEFT,
		ORBIT_MAX
	}GUARD_DOME;

	const static int m_MAX_HP = 500;				// HP�̍ő�l
	const static int m_MAX_MP = 100;				// MP�̍ő�l

	CPlayer();										// �R���X�g���N�^
	~CPlayer();										// �f�X�g���N�^

	static HRESULT Load(void);						// ���[�h
	static void Unload(void);						// �A�����[�h

													// ����
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	HRESULT Init(void);								// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

	void Setpos(D3DXVECTOR3 pos)					// �ʒu�ݒ�
		{ m_pos = pos; }	
	D3DXVECTOR3 Getpos(void)						// �ʒu�擾
		{ return m_pos; }		

	D3DXVECTOR3 Getrot(void)						// �p�x�擾
		{ return m_rot; }	

	D3DXVECTOR3 GetvtxMax(void)						// �傫���擾
		{ return m_vtxMax; }

	D3DXVECTOR3 Getmove(void)						// �ړ��ʎ擾
		{ return m_move; }		

	void SetrotDest(float rotDest)					// �ڕW�p�x�ݒ�
		{ m_frotDest = rotDest; }

	void SetUpdate(bool bUpdate)					// �X�V�ݒ�
		{ m_bUpdata = bUpdate; }
	bool GetUpdate(void)							// �X�V�擾
		{ return m_bUpdata; }
	void SetMove(bool bMove)						// �ړ��ݒ�
		{ m_bMove = bMove; }
	bool GetMove(void)								// �ړ��擾
		{ return m_bMove; }

	void AddHP(int nValue)
	{// HP
		if (0 <= nValue)
		{
			m_nHP += nValue;
			if (m_MAX_HP < m_nHP) m_nHP = m_MAX_HP;
		}
		else
		{
			CSound *pSound = CManager::GetSound();
			CFade::FADE	fade = CFade::GetFade();	// �t�F�[�h�擾

			if (fade == CFade::FADE_NONE)
			{
				if (m_state == STATE_NORMAL)
				{
					if (m_bStep == false)
					{
						m_state = STATE_DAMAGE;
						m_nHP += (int)((float)nValue * m_fDamageCat);
						if (m_nHP < 0)
						{
							m_state = STATE_DEATH;
							m_nHP = 0;
						}

						if (pSound != NULL) pSound->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE);
					}
				}
			}
		}
	}
	void SetHP(int nHP)
		{ m_nHP = nHP; }
	int GetHP(void)
		{ return m_nHP; }

	void AddMP(int nValue)
	{// MP
		if (0 <= nValue)
		{
			m_nMP += nValue;
			if (100 < m_nMP) m_nMP = 100;
		}
		else
		{
			m_nMP += nValue;
			if (m_nMP < 0)
			{
				AddHP(m_nMP);
				m_nMP = 0;
				if (m_nHP < 0) m_nHP = 0;
			}
		}
	}
	void SetMP(int nMP)
		{ m_nMP = nMP; }
	int GetMP(void)
		{ return m_nMP; }

	void SetAction(bool bNotAttack, bool bNotSkill)
		{ m_bNotAttack = bNotAttack; m_bNotSkill = bNotSkill; }

	bool Collision(D3DXVECTOR3 pos);

private:
	void Title(void);								// �^�C�g������
	void State(void);								// ��ԏ���
	void Move(void);								// �ړ�����
	void Step(void);								// �X�e�b�v����
	void Action(void);								// �A�N�V��������
	void Jump(void);								// �W�����v����
	void ColisionChack(float *fHeight);				// ����`�F�b�N����

	static int					m_MaxModel;			// ���f����
	static int					m_MaxMotion;		// ���[�V������

	static LPDIRECT3DTEXTURE9	**m_pTexture;		// �e�N�X�`�����ւ̃|�C���^
	static LPD3DXMESH			*m_pMesh;			// ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER			*m_pBuffMat;		// �}�e���A�����ւ̃|�C���^
	static DWORD				*m_nNumMat;			// �}�e���A�����̐�

	static CModel::MODEL_INFO	*m_ModelInfo;		// ���f���C���t�H
	static CMotion::MOTION_INFO	*m_MotionInfo;		// ���[�V�����C���t�H

	D3DXMATRIX					m_mtxWorld;			// ���[���h�}�g���b�N�X
	CModel						*m_pModel;			// ���f��
	CMotion						*m_pMotion;			// ���[�V����

	CMapIcon					*m_MapIcon;			// �}�b�v�A�C�R��

	bool						m_bTutorialSet;		// �`���[�g���A���ݒ�
	int							m_nCntTutorialSet;	// �`���[�g���A���ݒ�J�E���g

	CSceneMeshOrbit				*m_pOrbit[ORBIT_MAX];	// �I�[�r�b�g

	bool						m_bWarp;			// ���[�v���Ă��邩
	int							m_nCntWarp;			// ���[�v�J�E���g
	float						m_fcol_a;			// �A���t�@�F
	D3DXVECTOR3					m_posWarp;			// ���[�v�ʒu
	float						m_fWarpLenght;
	float						m_fWarpAngle;

	bool						m_bUpdata;			// �X�V
	bool						m_bDraw;			// �`��
	bool						m_bMove;			// �ړ��ł��邩

	int							m_nHP;				// HP
	int							m_nMP;				// MP

	STATE						m_state;			// ���
	int							m_nCntState;		// ��ԃJ�E���g

	int							m_nHatNum;			// �X�q�ԍ�
	int							m_nCntTimer;		// �^�C�}�[�J�E���g

	D3DXVECTOR3					m_vtxMax;			// ����MAX
	D3DXVECTOR3					m_vtxMin;			// ����MIN
	D3DXVECTOR3					m_pos;				// �ʒu
	D3DXVECTOR3					m_posOld;			// �ʒu�O��
	D3DXVECTOR3					m_rot;				// ����
	D3DXVECTOR3					m_move;				// �ړ���
	float						m_frotDest;			// �ڕW����
	bool						m_bJump;			// �W�����v
	int							m_nAttackCnt;		// �U���J�E���g
	int							m_nAttackNum;		// �U�����͐�
	bool						m_bAttack_Nor;		// �ʏ�U��
	bool						m_bAttack_Sky;		// �󒆍U��
	bool						m_bAction_Sky;		// �󒆃A�N�V����
	int							m_nAttackRecast;	// �U�����L���X�g
	int							m_nGuardCnt;		// �K�[�h�J�E���g
	bool						m_bGuard;			// �K�[�h
	int							m_nCntJastGuard;	// �W���X�g�K�[�h�J�E���g
	float						m_fDamageCat;		// �_���[�W�J�b�g
	int							m_nStepCnt;			// �X�e�b�v�J�E���g
	bool						m_bStep;			// �X�e�b�v
	bool						m_bSkill;			// �X�L��
	int							m_nSkillRecast;		// �X�L�����L���X�g

	bool						m_brot;				// ��]�\��

	float						m_shadowHeight;		// �e����

	bool						m_bNotAttack;		// �U���֎~
	bool						m_bNotSkill;		// �X�L���֎~

	CSceneMesh					*m_pGuardDome[GUARD_DOME_MAX];	// �K�[�h�h�[��
	D3DXVECTOR2					m_GuardDomeUV;		// �K�[�h�h�[��UV

protected:

};

#endif