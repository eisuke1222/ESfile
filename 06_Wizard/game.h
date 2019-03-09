//=============================================================================
//
// �Q�[������ [game.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPause;			// �|�[�Y
class CMap;				// �}�b�v
class CUi;				// UI
class CPlayer;			// �v���C���[
class CScene2D_Preset;	// �V�[��2D�v���Z�b�g
class CScene3D;			// �V�[��3D

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame
{// �Q�[��
public:
	typedef enum
	{	// �Q�[��
		GAME_NONE = 0,		// �������Ă��Ȃ�
		GAME_TUTORIAL,		// �`���[�g���A��
		GAME_PLAY_1,		// �Q�[���v���C1
		GAME_PLAY_2,		// �Q�[���v���C2
		//GAME_PLAY_3,		// �Q�[���v���C3
		GAME_PLAY_B,		// �Q�[���v���CBoss
		GAME_CLEAR,			// �Q�[���N���A
		GAME_OVER,			// �Q�[���I�[�o�[
		GAME_MAX			// �ő吔
	}GAME;

	typedef enum
	{
		TUTORIAL_MOVE = 0,
		TUTORIAL_CAMERA,
		TUTORIAL_JUMP,
		TUTORIAL_LOCKON,
		TUTORIAL_ATTACK,
		TUTORIAL_GUARD,
		TUTORIAL_SKILL,
		TUTORIAL_ACTION,
		TUTORIAL_MAX
	}TUTORIAL;

	CGame();						// �R���X�g���N�^
	~CGame();						// �f�X�g���N�^

	static HRESULT Load(void);		// ���[�h
	static void Unload(void);		// �A�����[�h

	HRESULT Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	void SetGame(GAME game, bool bReset = true);	// �Q�[���ݒ�
	GAME GetGame(void)				// �Q�[���擾
		{ return m_game; };

	CMap *GetMap(void)				// �}�b�v�擾
		{ return m_pMap; }
	CUi *GetUi(void)				// Ui�擾
		{ return m_pUi; }
	CPlayer *GetPlayer(void)		// �v���C���[�擾
		{ return m_pPlayer; }

	void PauseSeting(void);			// �|�[�Y�ݒ�

	int GetMapNum(void)				// �}�b�v�ԍ��擾
		{ return m_nMapNum; }

	void SetEnemyNum(int nNum)		// �G�����ݒ�
		{ m_nSetEnemyNum = nNum; }
	int GetEnemyNum(void)			// �G�����擾
		{ return m_nSetEnemyNum; }

	void SetSS(bool bSetSS)			// �X�N���[���V���b�g���B��
		{ m_bSetSS = bSetSS; }
	void SetSS_Death(bool bSetSS)	// �X�N���[���V���b�g���B��
		{ m_bSetSS_Death = bSetSS; }

	int GetTutprialNum(void)		// �`���[�g���A���ԍ��擾
		{ return m_nTutorialNum; }

	void AddDeathEnemy(void)		// �|�����G���Z
		{ m_nDeathEnemy++; }
	void SetNoDamage(void)			// �m�[�_���[�W
		{ m_bNoDamage = false; }
	void AddJustGuard(void)			// �W���X�g�K�[�h����
		{ m_nJastGuard++; }

	int GetScore(void);				// �X�R�A�擾

private:
	GAME		m_game;				// �Q�[��
	CPause		*m_pPause;			// �|�[�Y
	bool		m_bPause;			// �|�[�Y���Ă邩

	CMap		*m_pMap;			// �}�b�v
	CUi			*m_pUi;				// UI
	CPlayer		*m_pPlayer;			// �v���C���[

	int			m_nMapNum;			// �}�b�v�ԍ�
	D3DXVECTOR3 posOld;				// �O��ʒu

	int			m_nSetEnemyNum;		// �G����

	int			m_nCntTimer;		// ���ԃJ�E���g

	bool		m_bSetSS;			// �X�N���[���V���b�g���B��
	bool		m_bSetSS_Death;		// �X�N���[���V���b�g���B��
	bool		m_bPlay_SS00;		// �X�N���[���V���b�g00
	bool		m_bPlay_SS01;		// �X�N���[���V���b�g01
	bool		m_bPlay_SS02;		// �X�N���[���V���b�g02

	int			m_nTutorialNum;				// �`���[�g���A���ԍ�
	int			m_nCntTutorial;				// �`���[�g���A���J�E���g
	bool		m_bTutorial[TUTORIAL_MAX];  // �`���[�g���A��
	CScene2D_Preset *m_pClear;				// �N���A
	int			m_nCntCheck;				// �`�F�b�N�J�E���g
	CScene3D	*m_pCheck;					// �`�F�b�N

	int			m_nCntSkip;					// �X�L�b�v�J�E���g
	CScene2D_Preset *m_pSkip;				// �X�L�b�v

	int			m_nPlayerHP;				// �v���C���[HP
	int			m_nPlayerMP;				// �v���C���[MP

	int			m_nTimer;					// �^�C�}�[
	int			m_nDeathEnemy;				// �|�����G����
	bool		m_bNoDamage;				// �m�[�_���[�W
	int			m_nJastGuard;				// �W���X�g�K�[�h����
	bool		m_bAllDeathEnemy;			// �S�Ă̓G��|����

	int				m_nCntTutorialPlus;		// �`���[�g���A���v���X�J�E���g
	CScene2D_Preset *m_pTutorialPlus;		// �`���[�g���A���v���X
	CScene2D_Preset *m_pTutorialPlusButton;	// �`���[�g���A���v���X�{�^��

protected:

};

#endif