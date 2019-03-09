//=============================================================================
//
// UI���� [ui.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[
#include "scene2d.h"	// �V�[��2D
#include "number.h"		// �ԍ�

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CGauge;			// �Q�[�W

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CUi : public CScene
{// Ui�i�e�FCScene�j
public:
	typedef enum
	{// UI
		UI_TITLE = 0,	// �^�C�g��
		UI_GAME,		// �Q�[��
		UI_RESULT,		// ���U���g
		UI_MAX			// �ő吔
	}UI;

	CUi();										// �R���X�g���N�^
	virtual ~CUi();								// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h

	static CUi *Create(UI ui);					// �N���G�C�g

	virtual HRESULT Init(void) = 0;				// ����������
	virtual void Uninit(void);					// �I������
	virtual void Update(void) = 0;				// �X�V����
	virtual void Draw(void) = 0;				// �`�揈��

private:

protected:

};

class CUi_Title : public CUi
{// �^�C�g��Ui�i�e�FCUi�j
public:
	typedef enum
	{// ���
		TYPE_TITLELOGO_BG = 0,	// �^�C�g�����S�G�t�F�N�g�p
		TYPE_TITLELOGO,			// �^�C�g�����S
		TYPE_MENU_SELECT,		// ���j���[�i�I���j
		TYPE_MENU_SELECT_MARK,	// ���j���[�i�I���}�[�N�j
		TYPE_MENU_GAME,			// ���j���[�i�Q�[���j
		TYPE_MENU_TUTORIAL,		// ���j���[�i�`���[�g���A���j
		TYPE_MENU_EDIT,			// ���j���[�i�ҏW�j
		TYPE_MENU_TAITLE,		// ���j���[�i�^�C�g���j
		TYPE_MAX				// �ő吔
	}TYPE;

	CUi_Title();								// �R���X�g���N�^
	~CUi_Title();								// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	static bool GetUiEdit(void)					// Ui�ύX����
		{ return m_bUiEdit; }

private:
	static bool			m_bUiEdit;				// Ui�ύX����
	CScene2D			*m_pUi[TYPE_MAX];		// Ui
	CScene2D_Preset		*m_pPress;				// �v���X
	int					m_nCntPress;			// �v���X�J�E���g

	int					m_nCntTitle;			// �^�C�g���J�E���g

protected:

};

class CUi_Game : public CUi
{// �Q�[��Ui�i�e�FCUi�j
public:
	typedef enum
	{// ���
		TYPE_ENEMY_DATA = 0,	// �G�f�[�^
		TYPE_PLAYER_DATA,		// �v���C���[�f�[�^
		TYPE_MAP,				// �}�b�v
		TYPE_LOG,				// ���O
		TYPE_WEAPON,			// ����
		TYPE_WEAPON_BUTTON,		// ����{�^��
		TYPE_PLAYER_ICON,		// �v���C���[�A�C�R��
		TYPE_PLAYER_NAME,		// �v���C���[��
		TYPE_PLAYER_HP,			// �v���C���[HP
		TYPE_PLAYER_MP,			// �v���C���[MP
		TYPE_PLAYER_RANK,		// �v���C���[�����N
		TYPE_ENEMY_ICON,		// �G�A�C�R��
		TYPE_ENEMY_NAME,		// �G��
		TYPE_ENEMY_HP,			// �GHP
		TYPE_MAX				// �ő吔
	}TYPE;

	typedef enum
	{
		MESSAGE_00 = 0,
		MESSAGE_01,
		MESSAGE_02,
		MESSAGE_03,
		MESSAGE_MAX
	}MESSAGE;

	typedef enum
	{
		GAUGE_PLAYER_HP = 0,
		GAUGE_PLAYER_MP,
		GAUGE_ENEMY_HP,
		GAUGE_MAX
	}GAUGE;

	CUi_Game();									// �R���X�g���N�^
	~CUi_Game();								// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	void SetGauge(GAUGE gauge, int nNow, int nMax);		// �Q�[�W�ݒ�

	static void SetEnemyName(int nNum)			// �G���O�ԍ�
		{ m_nEnemyName = nNum; }

private:
	CScene2D			*m_pUi[TYPE_MAX];		// Ui
	CScene2D			*m_Message[MESSAGE_MAX];// ���b�Z�[�W
	D3DXVECTOR2			m_TutorialUV;			// �`���[�g���A��UV
	int					m_nCntTutorial;			// �`���[�g���A���J�E���g

	CGauge				*m_pGauge[GAUGE_MAX];	// �Q�[�W

	static int			m_nEnemyName;			// �G���O�ԍ�

protected:

};

class CUi_Result : public CUi
{// ���U���gUi�i�e�FCUi�j
public:
	typedef enum
	{// ���
		TYPE_LOGO_BG = 0,	// ���Sbg
		TYPE_LOGO,			// ���S
		TYPE_PLAYER_NAME,	// �v���C���[�� 
		TYPE_PLAYER_RANK,	// �v���C���[�����N
		TYPE_PLAY_SS00,		// �X�N���[���V���b�g00 
		TYPE_PLAY_SS01,		// �X�N���[���V���b�g01 
		//TYPE_PLAY_SS02,		// �X�N���[���V���b�g02 
		TYPE_MAX			// �ő吔
	}TYPE;

	typedef enum
	{
		SS_00 = 0,
		SS_01,
		//SS_02,
		SS_MAX
	}SS;

	CUi_Result();								// �R���X�g���N�^
	~CUi_Result();								// �f�X�g���N�^

	static HRESULT Load(void);					// ���[�h
	static void Unload(void);					// �A�����[�h

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[SS_MAX];		// �e�N�X�`��

	CScene2D			*m_pUi[TYPE_MAX];		// Ui

	int					m_nCntTimer;			// ���ԃJ�E���g
	int					m_nCntLogo;				// ���S�J�E���g

	CNumber2D *m_pNumber[6];	// �ԍ�

protected:

};

#endif