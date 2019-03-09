//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "manager.h"		// �}�l�[�W���[
#include "sound.h"			// �T�E���h
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "light.h"			// ���C�g
#include "renderer.h"		// �����_���[
#include "title.h"			// �^�C�g��
#include "tutorial.h"		// �`���[�g���A��
#include "game.h"			// �Q�[��
#include "fade.h"			// �t�F�[�h

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CManager::MODE		CManager::m_mode			= CManager::MODE_NONE;	// ���[�h
CSound				*CManager::m_pSound			= NULL;					// �T�E���h
CInputKeyboard		*CManager::m_pInputKeyboard = NULL;					// �L�[�{�[�h
CInputDIPad			*CManager::m_pInputDIPad	= NULL;					// DI�p�b�h
CInputXPad			*CManager::m_pInputXPad		= NULL;					// X�p�b�h
CRenderer			*CManager::m_pRenderer		= NULL;					// �����_���[
CCamera				*CManager::m_pCamara		= NULL;					// �J����
CLight				*CManager::m_pLight			= NULL;					// ���C�g
CTitle				*CManager::m_pTitle			= NULL;					// �^�C�g��
CTutorial			*CManager::m_pTutorial		= NULL;					// �`���[�g���A��
CGame				*CManager::m_pGame			= NULL;					// �Q�[��
int					CManager::m_nPlayNum		= 0;					// �v���C�l��

//=============================================================================
// �R���X�g���N�^								(public)	*** CManager ***
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
// �f�X�g���N�^									(public)	*** CManager ***
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// ����������									(public)	*** CManager ***
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �T�E���h
	{
		if (m_pSound == NULL)
		{// NULL�̏ꍇ
			m_pSound = new CSound;		// �T�E���h�������m��
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, "(manager�uInit] : Sound��NULL�łȂ�", "�x��", MB_OK);
		}

		if (m_pSound != NULL)
		{// NULL�ȊO�̏ꍇ
		 // �T�E���h����������
			if (FAILED(m_pSound->Init(hWnd)))
			{
				return -1;
			}
		}
		else
		{// NULL�̏ꍇ
			MessageBox(0, "(manager�uInit] : Sound��NULL", "�x��", MB_OK);
		}
	}

	// �L�[�{�[�h
	{
		if (m_pInputKeyboard == NULL)
		{// NULL�̏ꍇ
			m_pInputKeyboard = new CInputKeyboard;		// �L�[�{�[�h�������m��
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, "(manager�uInit] : InputKeyboard��NULL�łȂ�", "�x��", MB_OK);
		}

		if (m_pInputKeyboard != NULL)
		{// NULL�ȊO�̏ꍇ
			// �L�[�{�[�h����������
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{
				return -1;
			}
		}
		else
		{// NULL�̏ꍇ
			MessageBox(0, "(manager�uInit] : InputKeyboard��NULL", "�x��", MB_OK);
		}
	}
	// DI�p�b�h
	{
		if (m_pInputDIPad == NULL)
		{// NULL�̏ꍇ
			m_pInputDIPad = new CInputDIPad;		// DI�p�b�h�������m��
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, "(manager�uInit] : InputDIPad��NULL�łȂ�", "�x��", MB_OK);
		}

		if (m_pInputDIPad != NULL)
		{// NULL�ȊO�̏ꍇ
			// DI�p�b�h����������
			if (FAILED(m_pInputDIPad->Init(hInstance, hWnd)))
			{
				return -1;
			}
		}
		else
		{// NULL�̏ꍇ
			MessageBox(0, "(manager�uInit] : InputDIPad��NULL", "�x��", MB_OK);
		}
	}
	// X�p�b�h
	{
		if (m_pInputXPad == NULL)
		{// NULL�̏ꍇ
			m_pInputXPad = new CInputXPad;		// DI�p�b�h�������m��
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, "(manager�uInit] : InputXPad��NULL�łȂ�", "�x��", MB_OK);
		}

		if (m_pInputXPad != NULL)
		{// NULL�ȊO�̏ꍇ
		 // DI�p�b�h����������
			if (FAILED(m_pInputXPad->Init(hInstance, hWnd)))
			{
				return -1;
			}
		}
		else
		{// NULL�̏ꍇ
			MessageBox(0, "(manager�uInit] : InputXPad��NULL", "�x��", MB_OK);
		}
	}

	// �����_���[
	{
		if (m_pRenderer == NULL)
		{// NULL�̏ꍇ
			m_pRenderer = new CRenderer;			// �����_���[�������m��
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, "(manager�uInit] : Renderer��NULL�łȂ�", "�x��", MB_OK);
		}

		if (m_pRenderer != NULL)
		{// NULL�ȊO�̏ꍇ
			// �����_���[����������
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
			{
				return -1;
			}
		}
		else
		{// NULL�̏ꍇ, 
			MessageBox(0, "(manager�uInit] : Renderer��NULL", "�x��", MB_OK);
		}
	}

	// �J����
	{
		if (m_pCamara == NULL)
		{// NULL�̏ꍇ
			m_pCamara = new CCamera;		// �J�����������m��
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, "(manager�uInit] : Camera��NULL�łȂ�", "�x��", MB_OK);
		}

		if (m_pCamara != NULL)
		{// NULL�ȊO�̏ꍇ
			// �J��������������
			if (FAILED(m_pCamara->Init(D3DXVECTOR3(0.0f, 80.0f, -150.0f), D3DXVECTOR3(0.0f, 30.0f, 0.0f))))
			{
				return -1;
			}
		}
		else
		{// NULL�̏ꍇ
			MessageBox(0, "(manager�uInit] : Camera��NULL", "�x��", MB_OK);
		}
	}

	// ���C�g
	{
		if (m_pLight == NULL)
		{// NULL�̏ꍇ
			m_pLight = new CLight;			// ���C�g�������m��
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, "(manager�uInit] : Light��NULL�łȂ�", "�x��", MB_OK);
		}

		if (m_pLight != NULL)
		{// NULL�ȊO�̏ꍇ
		 // ���C�g����������
			if (FAILED(m_pLight->Init()))
			{
				return -1;
			}
		}
		else
		{// NULL�̏ꍇ
			MessageBox(0, "(manager�uInit] : Light��NULL", "�x��", MB_OK);
		}
	}

	CFade::Create(MODE_TITLE, CFade::FADE_IN);		// �t�F�[�h�N���G�C�g

	return S_OK;
}

//=============================================================================
// �I������										(public)	*** CManager ***
//=============================================================================
void CManager::Uninit(void)
{
	// �T�E���h
	{
		if (m_pSound != NULL)
		{// NULL�ȊO�̏ꍇ
			// �I������
			m_pSound->Uninit();

			delete m_pSound;	// �T�E���h�������J��
			m_pSound = NULL;	// NULL
		}
	}

	// �L�[�{�[�h
	{
		if (m_pInputKeyboard != NULL)
		{// NULL�ȊO�̏ꍇ
			// �I������
			m_pInputKeyboard->Uninit();

			delete m_pInputKeyboard;	// �L�[�{�[�h�������J��
			m_pInputKeyboard = NULL;	// NULL
		}
	}
	// DI�p�b�h
	{
		if (m_pInputDIPad != NULL)
		{// NULL�ȊO�̏ꍇ
			// �I������
			m_pInputDIPad->Uninit();

			delete m_pInputDIPad;	// DI�p�b�h�������J��
			m_pInputDIPad = NULL;	// NULL
		}
	}
	// X�p�b�h
	{
		if (m_pInputXPad != NULL)
		{// NULL�ȊO�̏ꍇ
		 // �I������
			m_pInputXPad->Uninit();

			delete m_pInputXPad;	// X�p�b�h�������J��
			m_pInputXPad = NULL;	// NULL
		}
	}

	switch (m_mode)
	{// ���[�h
	case CManager::MODE_TITLE:		// �^�C�g��
		if (m_pTitle != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTitle->Unload();		// �A�����[�h
			m_pTitle->Uninit();		// �I������
			m_pTitle = NULL;		// NULL��
		}

		break;
	case CManager::MODE_TUTORIAL:	// �`���[�g���A��
		if (m_pTutorial != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTutorial->Uninit();	// �I������
			m_pTutorial = NULL;		// NULL��
		}

		break;
	case CManager::MODE_GAME:		// �Q�[��
		if (m_pGame != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pGame->Uninit();		// �I������
			m_pGame = NULL;			// NULL��
		}

		break;
	}

	CScene::ReleaseAll();

	// �����_���[
	{
		if (m_pRenderer != NULL)
		{// NULL�ȊO�̏ꍇ
			// �I������
			m_pRenderer->Uninit();

			delete m_pRenderer;	// �����_���[�������J��
			m_pRenderer = NULL;	// NULL
		}
	}

	// �J����
	{
		if (m_pCamara != NULL)
		{// NULL�ȊO�̏ꍇ
			// �I������
			m_pCamara->Uninit();

			delete m_pCamara;	// �J�����������J��
			m_pCamara = NULL;	// NULL
		}
	}

	// ���C�g
	{
		if (m_pLight != NULL)
		{// NULL�ȊO�̏ꍇ
			// �I������
			m_pLight->Uninit();

			delete m_pLight;	// ���C�g�������J��
			m_pLight = NULL;	// NULL
		}
	}
}

//=============================================================================
// �X�V����										(public)	*** CManager ***
//=============================================================================
void CManager::Update(void)
{
	// �L�[�{�[�h
	{
		if (m_pInputKeyboard != NULL)
		{// NULL�ȊO�̏ꍇ
			// �L�[�{�[�h�X�V����
			m_pInputKeyboard->Update();
		}
	}
	// DI�p�b�h
	{
		if (m_pInputDIPad != NULL)
		{// NULL�ȊO�̏ꍇ
			// DI�p�b�h�X�V����
			m_pInputDIPad->Update();
		}
	}
	// X�p�b�h
	{
		if (m_pInputXPad != NULL)
		{// NULL�ȊO�̏ꍇ
			// X�p�b�h�X�V����
			m_pInputXPad->Update();
		}
	}

	switch (m_mode)
	{// ���[�h
	case CManager::MODE_TITLE:		// �^�C�g��
		if (m_pTitle != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTitle->Update();		// �X�V����
		}

		break;
	case CManager::MODE_TUTORIAL:	// �Q�[��
		if (m_pTutorial != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTutorial->Update();	// �X�V����
		}

		break;
	case CManager::MODE_GAME:		// �Q�[��
		if (m_pGame != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pGame->Update();		// �X�V����
		}

		break;
	}

	// �����_���[
	{
		if (m_pRenderer != NULL)
		{// NULL�ȊO�̏ꍇ
			// �����_���[�X�V����
			m_pRenderer->Update();
		}
	}

	// �J����
	{
		if (m_pCamara != NULL)
		{// NULL�ȊO�̏ꍇ
			// �J�����X�V����
			m_pCamara->Update();
		}
	}

	// ���C�g
	{
		if (m_pLight != NULL)
		{// NULL�ȊO�̏ꍇ
			// ���C�g�X�V����
			m_pLight->Update();
		}
	}
}

//=============================================================================
// �`�揈��										(public)	*** CManager ***
//=============================================================================
void CManager::Draw(void)
{
	// �����_���[
	{
		if (m_pRenderer != NULL)
		{// NULL�ȊO�̏ꍇ
			// �����_���[�`�揈��
			m_pRenderer->Draw();
		}
	}

	// �J����
	{
		if (m_pCamara != NULL)
		{// NULL�ȊO�̏ꍇ
			// �J�����`�揈��
			m_pCamara->SetCamera();
		}
	}
}

//=============================================================================
// ���[�h�ݒ菈��								(public)	*** CManager ***
//=============================================================================
void CManager::SetMode(CManager::MODE mode)
{
	switch (m_mode)
	{// ���[�h�i���݁j
	case CManager::MODE_TITLE:			// �^�C�g��
		if (m_pTitle != NULL)
		{// NULL�ȊO�̏ꍇ
			if (m_mode != mode)
			{// ���[�h���������
				m_pTitle->Unload();		// �A�����[�h
			}
			m_pTitle->Uninit();			// �I������
			m_pTitle = NULL;			// NULL��
		}

		m_pSound->StopSound(CSound::SOUND_LABEL_BGM000);

		break;
	case CManager::MODE_TUTORIAL:		// �`���[�g���A��
		if (m_pTutorial != NULL)
		{// NULL�ȊO�̏ꍇ
			if (m_mode != mode)
			{// ���[�h���������
				m_pTutorial->Unload();	// �A�����[�h
			}
			m_pTutorial->Uninit();		// �I������
			m_pTutorial = NULL;			// NULL��
		}

		m_pSound->StopSound(CSound::SOUND_LABEL_BGM001);

		break;
	case CManager::MODE_GAME:			// �Q�[��
		if (m_pGame != NULL)
		{// NULL�ȊO�̏ꍇ
			if (m_mode != mode)
			{// ���[�h���������
				m_pGame->Unload();		// �A�����[�h
			}
			m_pGame->Uninit();			// �I������
			m_pGame = NULL;				// NULL��
		}

		m_pSound->StopSound(CSound::SOUND_LABEL_BGM001);

		break;
	}

	m_pSound->StopSound();

	switch (mode)
	{// ���[�h�i����j
	case CManager::MODE_TITLE:			// �^�C�g��
		if (m_pTitle == NULL)
		{// NULL�̏ꍇ
			m_pTitle = new CTitle;		// �������m��

			if (m_pTitle != NULL)
			{// NULL�ȊO�̏ꍇ
				if (m_mode != mode)
				{// ���[�h���������
					m_pTitle->Load();	// ���[�h
				}
				m_pTitle->Init();		// ����������
				SetPlayNum(0);			// �v���C���[��������
			}

		}

		m_pSound->PlaySoundA(CSound::SOUND_LABEL_BGM000);

		break;
	case CManager::MODE_TUTORIAL:		// �`���[�g���A��
		if (m_pTutorial == NULL)
		{// NULL�̏ꍇ
			m_pTutorial = new CTutorial;	// �������m��

			if (m_pTutorial != NULL)
			{// NULL�ȊO�̏ꍇ
				if (m_mode != mode)
				{// ���[�h���������
					m_pTutorial->Load();	// ���[�h
				}
				m_pTutorial->Init();		// ����������
			}
		}

		m_pSound->PlaySoundA(CSound::SOUND_LABEL_BGM001);

		break;
	case CManager::MODE_GAME:			// �Q�[��
		if (m_pGame == NULL)
		{// NULL�̏ꍇ
			m_pGame = new CGame;		// �������m��

			if (m_pGame != NULL)
			{// NULL�ȊO�̏ꍇ
				if (m_mode != mode)
				{// ���[�h���������
					m_pGame->Load();	// ���[�h
				}
				m_pGame->Init();		// ����������
			}
		}

		m_pSound->PlaySoundA(CSound::SOUND_LABEL_BGM001);

		break;
	}

	m_mode = mode;			// ���[�h�ύX
}

//=============================================================================
// ���[�h�擾����								(public)	*** CManager ***
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_mode;		// �l��Ԃ�
}

//=============================================================================
// �v���C�l���擾����								(public)	*** CManager ***
//=============================================================================
int CManager::GetPlayNum(void)
{
	return m_nPlayNum;	// �l��Ԃ�
}