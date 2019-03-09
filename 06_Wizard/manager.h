//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : Eisuke Sakagawa
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CSound;			// �T�E���h
class CInputKeyboard;	// �L�[�{�[�h
class CInputMouse;		// �}�E�X
class CInputDIPad;		// DI�p�b�h
class CInputXPad;		// X�p�b�h
class CRenderer;		// �����_���[
class CCamera;			// �J����
class CLight;			// ���C�g

class CTitle;			// �^�C�g��
class CGame;			// �Q�[��
class CResult;			// ���U���g
class CRanking;			// �����L���O

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CManager
{// �}�l�[�W���[
public:
	const static int m_PLAYERMAX = 4;					// �v���C���[��

	typedef enum
	{// �J����
		CAMERA_MAIN = 0,	// ���C��
		CAMERA_GAME_MAP,	// �Q�[���}�b�v
		CAMERA_MAX			// �ő吔
	}CAMERA;

	typedef enum
	{// ���[�h
		MODE_NONE = 0,		// �������Ă��Ȃ�
		MODE_TITLE,			// �^�C�g��-
		MODE_GAME,			// �Q�[��
		MODE_RESULT,		// ���U���g
		MODE_MAX			// �ő吔
	}MODE;

	CManager();											// �R���X�g���N�^
	virtual ~CManager();								// �f�X�g���N�^

														// ����������
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);		
	virtual void Uninit(void);							// �I������
	virtual void Update(void);							// �X�V����
	virtual void Draw(void);							// �`�揈��

	static CSound *GetSound(void)						// �T�E���h�擾
		{ return m_pSound; }
	static CInputKeyboard *GetInputKeyboard(void)		// �L�[�{�[�h�擾
		{ return m_pInputKeyboard; }
	static CInputMouse *GetInputMouse(void)				// �}�E�X�擾
		{ return m_pInputMouse; }
	static CInputDIPad *GetInputDIPad(void)				// DI�p�b�h�擾
		{ return m_pInputDIPad; }
	static CInputXPad *GetInputXPad(void)				// X�p�b�h�擾
		{ return m_pInputXPad; }
	static CRenderer *GetRenderer(void)					// �����_���[�擾
		{ return m_pRenderer; }
	static CCamera *GetCamera(int nNum)					// �J�����擾
		{ return m_pCamara[nNum]; }
	static CLight *GetLight(void)						// ���C�g�擾
		{ return m_pLight; }

	static CTitle *GetTitle(void)						// �^�C�g���擾
		{ return m_pTitle; }
	static CGame *GetGame(void)							// �Q�[���擾
		{ return m_pGame; }
	static CResult *GetResult(void)						// ���U���g�擾
		{ return m_pResult; }

	static void SetMode(MODE mode);						// ���[�h�ݒ�
	static MODE GetMode(void)							// ���[�h�擾 
		{ return m_mode; }

	static void SetPlayNum(int nPlayerNum)				// �v���C�l���ݒ�
		{ if (nPlayerNum <= m_PLAYERMAX)m_nPlayNum = nPlayerNum; else m_nPlayNum = m_PLAYERMAX; }
	static int GetPlayNum(void)							// �v���C�l���擾
		{ return m_nPlayNum; }

	static void ConMode(MODE mode, char *cMode);		// ���[�h�����ϊ�

private:
	void DebugFont(void);								// �f�o�b�O�t�H���g

	static MODE				m_mode;						// ���[�h

	static CSound			*m_pSound;					// �T�E���h
	static CInputKeyboard	*m_pInputKeyboard;			// �L�[�{�[�h
	static CInputMouse		*m_pInputMouse;				// �}�E�X
	static CInputDIPad		*m_pInputDIPad;				// DI�p�b�h
	static CInputXPad		*m_pInputXPad;				// X�p�b�h
	static CRenderer		*m_pRenderer;				// �����_���[
	static CCamera			*m_pCamara[CAMERA_MAX];		// �J����
	static CLight			*m_pLight;					// ���C�g

	static CTitle			*m_pTitle;					// �^�C�g��
	static CGame			*m_pGame;					// �Q�[��
	static CResult			*m_pResult;					// ���U���g

	static int				m_nPlayNum;					// �v���C�l��

protected:

};

#endif