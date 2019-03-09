//=============================================================================
//
// ���͏��� [input.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define INPUT_DEADZONE		( 0.24f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CInput
{// ����
public:
	CInput();										// �R���X�g���N�^
	virtual ~CInput();								// �f�X�g���N�^

													// ����������
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	
	virtual void Uninit(void);						// �I������
	virtual void Update(void) = 0;					// �X�V����

protected:
	static LPDIRECTINPUT8 m_pInput;					// ���̓I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8 m_pDevice;					// ���̓f�o�C�X

};

class CInputKeyboard : public CInput
{// �L�[�{�[�h�i�e�FCInput�j
public:
	const static int m_KEYMAX	= 256;				// �L�[�̍ő吔

	CInputKeyboard();								// �R���X�g���N�^
	virtual ~CInputKeyboard();						// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����

	bool GetALL(int nType);							// �I�[��
	bool GetPress(int nKey);						// �v���X
	bool GetTrigger(int nKey);						// �g���K�[
	bool GetRelease(int nKey);						// �����[�X

private:
	BYTE m_aKeyStatePress[m_KEYMAX];				// �v���X	
	BYTE m_aKeyStateTrigger[m_KEYMAX];				// �g���K�[
	BYTE m_aKeyStateRelease[m_KEYMAX];				// �����[�X
};

class CInputMouse : public CInput
{// �}�E�X�i�e�FCInput�j
public:
	const static int m_MOUSEMAX = 8;				// �}�E�X�ő吔

	CInputMouse();									// �R���X�g���N�^
	virtual ~CInputMouse();							// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����

	bool GetALL(int nType);							// �I�[��
	bool GetPress(int nKey);						// �v���X
	bool GetTrigger(int nKey);						// �g���K�[
	bool GetRelease(int nKey);						// �����[�X

	D3DXVECTOR2 GetPointer(void);					// �|�C���^�ʒu�擾
	int GetWheel(void);								// �z�C�[���ԍ��擾

private:
	BYTE m_aMouseStatePress[m_MOUSEMAX];			// �v���X	
	BYTE m_aMouseStateTrigger[m_MOUSEMAX];			// �g���K�[
	BYTE m_aMouseStateRelease[m_MOUSEMAX];			// �����[�X

	D3DXVECTOR2 m_posPointer;						// �|�C���^�ʒu
	int			m_nWheel;							// �z�C�[���ԍ�
};

class CInputDIPad : public CInput
{// DI�p�b�h�i�e�FCInput�j
public:
	const static int m_BUTTONMAX	= 32;			// �{�^���̍ő吔

	typedef enum
	{// �{�^���ȊO�̃^�C�v
		DIPADOTHER_AXES_UP = m_BUTTONMAX,		// �\���L�[��
		DIPADOTHER_AXES_LEFT,					// �\���L�[��
		DIPADOTHER_AXES_RIGHT,					// �\���L�[�E
		DIPADOTHER_AXES_DOWN,					// �\���L�[��
		DIPADOTHER_STICK_L_UP,					// L�X�e�B�b�N��
		DIPADOTHER_STICK_L_LEFT,				// L�X�e�B�b�N��
		DIPADOTHER_STICK_L_RIGHT,				// L�X�e�B�b�N�E
		DIPADOTHER_STICK_L_DOWN,				// L�X�e�B�b�N��
		DIPADOTHER_STICK_R_UP,					// R�X�e�B�b�N��
		DIPADOTHER_STICK_R_LEFT,				// R�X�e�B�b�N��
		DIPADOTHER_STICK_R_RIGHT,				// R�X�e�B�b�N�E
		DIPADOTHER_STICK_R_DOWN,				// R�X�e�B�b�N��
		DIPADOTHER_MAX
	}DIPADOTHER;

	CInputDIPad();									// �R���X�g���N�^
	virtual ~CInputDIPad();							// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����

	bool GetPress(int nButton);						// �v���X
	bool GetTrigger(int nButton);					// �g���K�[
	bool GetRelease(int nButton);					// �����[�X
	bool GetStick(int nLR);							// �X�e�B�b�N
	float GetStickRot(int nLR);						// �X�e�B�b�N����

private:
	BYTE m_aJoyStatePress[DIPADOTHER_MAX];			// �v���X	
	BYTE m_aJoyStateTrigger[DIPADOTHER_MAX];		// �g���K�[
	BYTE m_aJoyStateRelease[DIPADOTHER_MAX];		// �����[�X

	float m_LStickRot;								// L�X�e�B�b�N����
	float m_RStickRot;								// R�X�e�B�b�N����
};

class CInputXPad
{// X�p�b�h
public:
	const static int m_CONTROLLER	= 4;			// �R���g���[���̍ő吔
	const static int m_STICKMAX		= 32767;		// �R���g���[���̍ő吔
	const static int m_STICKMIN		= -32768;		// �R���g���[���̍ő吔

	typedef enum
	{// �{�^���ȊO�̃^�C�v
		XPADOTHER_BUTTON = 0,					// �{�^��
		XPADOTHER_TRIGGER_LEFT,					// L�g���K�[
		XPADOTHER_TRIGGER_RIGHT,				// R�g���K�[
		XPADOTHER_STICK_L_UP,					// L�X�e�B�b�N��
		XPADOTHER_STICK_L_LEFT,					// L�X�e�B�b�N��
		XPADOTHER_STICK_L_RIGHT,				// L�X�e�B�b�N�E
		XPADOTHER_STICK_L_DOWN,					// L�X�e�B�b�N��
		XPADOTHER_STICK_R_UP,					// R�X�e�B�b�N��
		XPADOTHER_STICK_R_LEFT,					// R�X�e�B�b�N��
		XPADOTHER_STICK_R_RIGHT,				// R�X�e�B�b�N�E
		XPADOTHER_STICK_R_DOWN,					// R�X�e�B�b�N��
		XPADOTHER_MAX
	}XPADOTHER;

	typedef struct
	{// XINPUT�R���g���[���[
		XINPUT_STATE state;		// ���
		bool bConnected;		// ���͗L��
	}CONTROLER_STATE;

	CInputXPad();										// �R���X�g���N�^
	virtual ~CInputXPad();								// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����

	static int GetInputNum(void);						// ���͐��擾

	bool GetALL(int nType, int nIdxPad);				// �I�[��
	bool GetPress(int nButton, int nIdxPad);			// �v���X
	bool GetPress(XPADOTHER nButton, int nIdxPad);		// �v���X�i���̑��j
	bool GetTrigger(int nButton, int nIdxPad);			// �g���K�[
	bool GetTrigger(XPADOTHER nButton, int nIdxPad);	// �g���K�[�i���̑��j
	bool GetRelease(int nButton, int nIdxPad);			// �����[�X
	bool GetRelease(XPADOTHER nButton, int nIdxPad);	// �����[�X�i���̑��j

	bool GetStick(int nLR, int nIdxPad);				// �X�e�B�b�N
	D3DXVECTOR2 GetStickNum(int nLR, int nIdxPad);		// �X�e�B�b�N���l
	float GetStickRot(int nLR, int nIdxPad);			// �X�e�B�b�N����
	D3DXVECTOR2 GetStickMove(int nLR, int nIdxPad);		// �X�e�B�b�N�ړ���

private:
	HRESULT UpdateControllerState(void);				// �R���g���[�����͐��X�V
	static int		m_nInputNum;						// �R���g���[�����͐�

	CONTROLER_STATE m_Controllers[m_CONTROLLER];		// �R���g���[��
	bool			m_bDeadZoneOn;						// �X�e�B�b�N

	WORD m_aJoyStatePress[m_CONTROLLER][XPADOTHER_MAX];		// �v���X	
	WORD m_aJoyStateTrigger[m_CONTROLLER][XPADOTHER_MAX];	// �g���K�[
	WORD m_aJoyStateRelease[m_CONTROLLER][XPADOTHER_MAX];	// �����[�X

	float m_LStickRot[m_CONTROLLER];					// L�X�e�B�b�N����
	float m_RStickRot[m_CONTROLLER];					// R�X�e�B�b�N����
};

#endif