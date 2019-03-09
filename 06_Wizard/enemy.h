//=============================================================================
//
// �G���� [enemy.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene.h"		// �V�[��
#include "motion.h"		// ���[�V����

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;			// ���f��
class CMotion;			// ���[�V����
class CScene3D;			// �V�[��3D
class CMapIcon;			// �}�b�v�A�C�R��
class CGauge;			// �Q�[�W

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEnemy : public CScene
{// �G�i�e�FCScene�j
public:
	typedef enum
	{// ���
		TYPE_N_SLIME = 0,	// �X���C��
		TYPE_B_SLIME,		// �X���C��
		TYPE_MAX			// �ő吔
	}TYPE;

	typedef enum
	{// ���
		STATE_NORMAL = 0,		// �ʏ�
		STATE_DAMAGE,			// �_���[�W
		STATE_DEATH,			// ����
		STATE_MAX				// �ő吔
	}STATE;

	CEnemy();										// �R���X�g���N�^
	virtual ~CEnemy();								// �f�X�g���N�^

	static HRESULT Load(void);						// ���[�h
	static void Unload(void);						// �A�����[�h

													// �N���G�C�g
	static CEnemy *Create(TYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	virtual HRESULT Init(void);						// ����������
	virtual void Uninit(void);						// �I������
	virtual void Update(void) = 0;					// �X�V����
	virtual void Draw(void) = 0;					// �`�揈��

	int GetType(void)								// �^�C�v�ԍ��擾
		{ return m_nType; }

													// �Փ˔��菈��
	virtual bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin, bool bSet = false) = 0;
	bool HitDamage(int nDamage);					// �q�b�g�_���[�W
	int GetHP(void) { return m_nHP; }				// HP�擾
	int GetMAXHP(void) { return m_nMAXHP; }			// MAXHP�擾

	void Setpos(D3DXVECTOR3 pos)					// �ʒu�ݒ�
		{ m_pos = pos; }	
	D3DXVECTOR3 Getpos(void)						// �ʒu�擾
		{ return m_pos; }		

	void SetMove(D3DXVECTOR3 move)					// �ړ��ݒ�
		{ m_move = move; }

	void SetbUpdate(bool bUpdate)					// �X�V�ݒ�
		{ m_bUpdata = bUpdate; }
	bool GetbUpdate(void)							// �X�V�擾
		{ return m_bUpdata; }

	void SetKnockBack(bool bKnockBack)				// �m�b�N�o�b�N
		{ m_bKnockBack = bKnockBack; }

	void SetLockOn(bool bLockOn)					// ���b�N�I���ݒ�
		{ m_bLockOn = bLockOn; }

private:
	virtual void ColisionChack(float *fHeight) = 0;	// ����`�F�b�N

protected:
	virtual void State(void)	= 0;				// ��ԏ���
	virtual void Move(void)		= 0;				// �ړ�����				
	virtual void Ai(void)		= 0;				// Ai����				

	int							m_nType;			// �^�C�v�ԍ�
	
	CMapIcon					*m_MapIcon;			// �}�b�v�A�C�R��

	bool						m_bUpdata;			// �X�V
	bool						m_bMove;			// �ړ��ł��邩

	int							m_nHP;				// HP
	int							m_nMAXHP;			// MAXHP

	STATE						m_state;			// ���
	int							m_nCntState;		// ��ԃJ�E���g

	D3DXVECTOR3					m_vtxMax;			// ����MAX
	D3DXVECTOR3					m_vtxMin;			// ����MIN
	D3DXVECTOR3					m_pos;				// �ʒu
	D3DXVECTOR3					m_posOld;			// �ʒu�O��
	D3DXVECTOR3					m_rot;				// ����
	D3DXVECTOR3					m_move;				// �ړ���
	float						m_frotDest;			// �ڕW����
	bool						m_bJump;			// �W�����v

	float						m_shadowHeight;		// �e����

	bool						m_bKnockBack;		// �m�b�N�o�b�N
	int							m_nCntKnockBack;	// �m�b�N�o�b�N�J�E���g

	bool						m_bLockOn;			// ���b�N�I��
};

class CSlime : public CEnemy
{// �X���C���i�e�FCEnemy�j
public:
	const static int m_MAX_HP = 50;				// �̗͂̍ő�l

	typedef enum
	{// ���f��
		MODEL_00_body = 0,	// ��
		MODEL_MAX
	}MODEL;	

	typedef enum
	{// AI
		AI_NORMAL = 0,		// �ʏ�
		AI_MAGIC,			// ���@
		AI_MAX				// �ő吔
	}AI;

	CSlime();										// �R���X�g���N�^
	~CSlime();										// �f�X�g���N�^

	static HRESULT Load(void);						// ���[�h
	static void Unload(void);						// �A�����[�h

	HRESULT Init(void);								// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

													// �Փ˔��菈��
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin, bool bSet = false);

private:
	void ColisionChack(float *fHeight);				// ����`�F�b�N

	void State(void);								// ��ԏ���
	void Move(void);								// �ړ�����	
	void Ai(void);									// Ai����	

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

	int							m_AiType;			// AI�^�C�v
	int							m_nMoveRecast;		// �ړ����L���X�g
	bool						m_bAttack;			// �U�����Ă���
	int							m_nCntAttack;		// �U���J�E���g

	bool						m_bAutoMove;		// �����ړ����Ă��邩
	int							m_nCntAutoMove;		// �����ړ��J�E���g
	int							m_nMaxAutoMove;		// �����ړ��J�E���g�ő�
	int							m_nAutoMoveRecast;	// �����ړ����L���X�g

protected:

};

class CSlime_King : public CEnemy
{// �X���C���L���O�i�e�FCEnemy�j
public:
	const static int m_MAX_HP = 450;				// �̗͂̍ő�l

	typedef enum
	{// ���f��
		MODEL_00_body = 0,	// ��
		MODEL_MAX
	}MODEL;

	typedef enum
	{// AI
		AI_NORMAL = 0,		// �ʏ�
		AI_MAGIC,			// ���@
		AI_DASH,			// �_�b�V��
		AI_CREATE,			// �N���G�C�g
		AI_STOP,			// �X�g�b�v
		AI_MAX				// �ő吔
	}AI;

	CSlime_King();									// �R���X�g���N�^
	~CSlime_King();									// �f�X�g���N�^

	static HRESULT Load(void);						// ���[�h
	static void Unload(void);						// �A�����[�h

	HRESULT Init(void);								// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

													// �Փ˔��菈��
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin, bool bSet = false);

private:
	void ColisionChack(float *fHeight);				// ����`�F�b�N

	void State(void);								// ��ԏ���
	void Move(void);								// �ړ�����	
	void Ai(void);									// Ai����	

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

	int							m_AiType;			// AI�^�C�v
	int							m_nMoveRecast;		// �ړ����L���X�g
	bool						m_bAttack;			// �U�����Ă���
	int							m_nCntAttack;		// �U���J�E���g

	bool						m_bAutoMove;		// �����ړ����Ă��邩
	int							m_nCntAutoMove;		// �����ړ��J�E���g
	int							m_nMaxAutoMove;		// �����ړ��J�E���g�ő�
	int							m_nAutoMoveRecast;	// �����ړ����L���X�g

protected:

};

#endif