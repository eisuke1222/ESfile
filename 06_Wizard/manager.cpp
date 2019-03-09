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
#include "renderer.h"		// �����_���[
#include "sound.h"			// �T�E���h
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "light.h"			// ���C�g
#include "fade.h"			// �t�F�[�h
#include "scene.h"			// �V�[��
#include "imgui_es.h"		// ImGui_ES

#include "text.h"			// �e�L�X�g
#include "title.h"			// �^�C�g��
#include "game.h"			// �Q�[��

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
CInputMouse			*CManager::m_pInputMouse	= NULL;					// �}�E�X
CInputDIPad			*CManager::m_pInputDIPad	= NULL;					// DI�p�b�h
CInputXPad			*CManager::m_pInputXPad		= NULL;					// X�p�b�h
CRenderer			*CManager::m_pRenderer		= NULL;					// �����_���[
CCamera				*CManager::m_pCamara[CManager::CAMERA_MAX] = {NULL};// �J����
CLight				*CManager::m_pLight			= NULL;					// ���C�g

CTitle				*CManager::m_pTitle			= NULL;					// �^�C�g��
CGame				*CManager::m_pGame			= NULL;					// �Q�[��
CResult				*CManager::m_pResult		= NULL;					// ���U���g

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
			m_pSound = new CSound;					// �������m��
		else
			MessageBox(0, "(manager�uInit] : Sound��NULL�łȂ�", "�x��", MB_OK);

		if (m_pSound != NULL)
			if (FAILED(m_pSound->Init(hWnd)))		// ����������
				return -1;		// �l��Ԃ�
	}

	// �L�[�{�[�h
	{
		if (m_pInputKeyboard == NULL)
			m_pInputKeyboard = new CInputKeyboard;	// �������m��
		else
			MessageBox(0, "(manager�uInit] : InputKeyboard��NULL�łȂ�", "�x��", MB_OK);

		if (m_pInputKeyboard != NULL)
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))	// ����������
			{				
				m_pInputKeyboard->Uninit();			// �I������

				delete m_pInputKeyboard;			// �������J��
				m_pInputKeyboard = NULL;			// NULL
			}
	}
	// �}�E�X
	{
		if (m_pInputMouse == NULL)
			m_pInputMouse = new CInputMouse;		// �������m��
		else
			MessageBox(0, "(manager�uInit] : InputMouse��NULL�łȂ�", "�x��", MB_OK);

		if (m_pInputMouse != NULL)
			if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))		// ����������
			{
				m_pInputMouse->Uninit();			// �I������

				delete m_pInputMouse;				// �������J��
				m_pInputMouse = NULL;				// NULL
			}
	}
	// DI�p�b�h
	{
		if (m_pInputDIPad == NULL)
			m_pInputDIPad = new CInputDIPad;		// �������m��
		else
			MessageBox(0, "(manager�uInit] : InputDIPad��NULL�łȂ�", "�x��", MB_OK);

		if (m_pInputDIPad != NULL)
			if (FAILED(m_pInputDIPad->Init(hInstance, hWnd)))		// ����������
			{				
				m_pInputDIPad->Uninit();			// �I������

				delete m_pInputDIPad;				// �������J��
				m_pInputDIPad = NULL;				// NULL
			}
	}
	// X�p�b�h
	{
		if (m_pInputXPad == NULL)
			m_pInputXPad = new CInputXPad;			// �������m��
		else
			MessageBox(0, "(manager�uInit] : InputXPad��NULL�łȂ�", "�x��", MB_OK);

		if (m_pInputXPad != NULL)
			if (FAILED(m_pInputXPad->Init(hInstance, hWnd)))
			{
				m_pInputDIPad->Uninit();			// �I������

				delete m_pInputDIPad;				// �������J��
				m_pInputDIPad = NULL;				// NULL
			}
	}

	// �����_���[
	{
		if (m_pRenderer == NULL)
			m_pRenderer = new CRenderer;			// �������m��
		else
			MessageBox(0, "(manager�uInit] : Renderer��NULL�łȂ�", "�x��", MB_OK);

		if (m_pRenderer != NULL)
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))	// ����������
				return -1;		// �l��Ԃ�
	}

	// �J����
	{
		for (int nCount = 0; nCount < CAMERA_MAX; nCount++)
		{// �J�������J�E���g
			if (m_pCamara[nCount] == NULL)
			{// NULL�̏ꍇ
				if (nCount == 0)
					m_pCamara[nCount] = new CCameraMain;	// �������m��
				else if (nCount == 1)
					m_pCamara[nCount] = new CCameraMap;		// �������m��
			}
			else
				MessageBox(0, "(manager�uInit] : Camera��NULL�łȂ�", "�x��", MB_OK);

			if (m_pCamara[nCount] != NULL)
				m_pCamara[nCount]->Init();			// ����������
		}

		// �~�j�}�b�v�ݒ�
		m_pCamara[CAMERA_GAME_MAP]->SetViewPort(SCREEN_WIDTH - (SCREEN_WIDTH / 6) - (SCREEN_WIDTH / 80), 0, (SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 80), (SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 80));
	}

	// ���C�g
	{
		if (m_pLight == NULL)
			m_pLight = new CLight;					// �������m��
		else
			MessageBox(0, "(manager�uInit] : Light��NULL�łȂ�", "�x��", MB_OK);

		if (m_pLight != NULL)
			m_pLight->Init();						// ����������
	}

	CText::Load();			// �e�L�X�g���[�h

	SetMode(MODE_TITLE);	// ���[�h�ݒ�

	return S_OK;	// �l��Ԃ�
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
			m_pSound->Uninit();			// �I������

			delete m_pSound;			// �������J��
			m_pSound = NULL;			// NULL
		}
	}

	// �L�[�{�[�h
	{
		if (m_pInputKeyboard != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pInputKeyboard->Uninit();	// �I������

			delete m_pInputKeyboard;	// �������J��
			m_pInputKeyboard = NULL;	// NULL
		}
	}
	// �}�E�X
	{
		if (m_pInputMouse != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pInputMouse->Uninit();	// �I������

			delete m_pInputMouse;		// �������J��
			m_pInputMouse = NULL;		// NULL
		}
	}
	// DI�p�b�h
	{
		if (m_pInputDIPad != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pInputDIPad->Uninit();	// �I������

			delete m_pInputDIPad;		// �������J��
			m_pInputDIPad = NULL;		// NULL
		}
	}
	// X�p�b�h
	{
		if (m_pInputXPad != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pInputXPad->Uninit();		// �I������

			delete m_pInputXPad;		// �������J��
			m_pInputXPad = NULL;		// NULL
		}
	}

	switch (m_mode)
	{// ���[�h
	case CManager::MODE_TITLE:			// �^�C�g��
		if (m_pTitle != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTitle->Unload();			// �A�����[�h
			m_pTitle->Uninit();			// �I������

			delete m_pTitle;			// �������J��
			m_pTitle = NULL;			// NULL��
		}

		break;
	case CManager::MODE_GAME:			// �Q�[��
		if (m_pGame != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pGame->Unload();			// �A�����[�h
			m_pGame->Uninit();			// �I������

			delete m_pGame;				// �������J��
			m_pGame = NULL;				// NULL��
		}

		break;
	case CManager::MODE_RESULT:			// ���U���g

		break;
	}

	// �����_���[
	{
		if (m_pRenderer != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pRenderer->Uninit();		// �I������

			delete m_pRenderer;			// �����_���[�������J��
			m_pRenderer = NULL;			// NULL
		}
	}

	// �J����
	{
		for (int nCount = 0; nCount < CAMERA_MAX; nCount++)
		{// �J�������J�E���g
			if (m_pCamara[nCount] != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pCamara[nCount]->Uninit();// �I������

				delete m_pCamara[nCount];	// �J�����������J��
				m_pCamara[nCount] = NULL;	// NULL
			}
		}
	}

	// ���C�g
	{
		if (m_pLight != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pLight->Uninit();			// �I������

			delete m_pLight;			// ���C�g�������J��
			m_pLight = NULL;			// NULL
		}
	}

	CText::Unload();	// �e�L�X�g�A�����[�h
}

//=============================================================================
// �X�V����										(public)	*** CManager ***
//=============================================================================
void CManager::Update(void)
{
	// �L�[�{�[�h
	{
		if (m_pInputKeyboard != NULL)
			m_pInputKeyboard->Update();	// �X�V����
	}
	// �}�E�X
	{
		if (m_pInputMouse != NULL)
			m_pInputMouse->Update();	// �X�V����
	}
	// DI�p�b�h
	{
		if (m_pInputDIPad != NULL)
			m_pInputDIPad->Update();	// �X�V����
	}
	// X�p�b�h
	{
		if (m_pInputXPad != NULL)
			m_pInputXPad->Update();		// �X�V����
	}

	DebugFont();	// �f�o�b�O�t�H���g

	switch (m_mode)
	{// ���[�h
	case CManager::MODE_TITLE:		// �^�C�g��
		if (m_pTitle != NULL)
			m_pTitle->Update();		// �X�V����

		break;
	case CManager::MODE_GAME:		// �Q�[��
		if (m_pGame != NULL)
			m_pGame->Update();		// �X�V����

		break;
	case CManager::MODE_RESULT:		// ���U���g

		break;
	}

	// �����_���[
	{
		if (m_pRenderer != NULL)
			m_pRenderer->Update();	// �X�V����
	}

	// �J����
	{
		for (int nCount = 0; nCount < CAMERA_MAX; nCount++)
			if (m_pCamara[nCount] != NULL)
				m_pCamara[nCount]->Update();	// �X�V����
	}

	// ���C�g
	{
		if (m_pLight != NULL)
			m_pLight->Update();		// �X�V����
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F7))
	{
		CManager::SetMode(CManager::MODE_GAME);
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
			m_pRenderer->Draw();	// �`�揈��
	}

	// �J����
	{
		/*for (int nCount = 0; nCount < CAMERA_MAX; nCount++)
			if (m_pCamara[nCount] != NULL)
				m_pCamara[nCount]->SetCamera();	// �`�揈��
		*/
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
			if (m_mode != mode)			// ���[�h���������
				m_pTitle->Unload();		// �A�����[�h
			m_pTitle->Uninit();			// �I������

			delete m_pTitle;			// �������J��
			m_pTitle = NULL;			// NULL��
		}

		break;
	case CManager::MODE_GAME:			// �Q�[��
		if (m_pGame != NULL)
		{// NULL�ȊO�̏ꍇ
			if (m_mode != mode)			// ���[�h���������
				m_pGame->Unload();		// �A�����[�h
			m_pGame->Uninit();			// �I������

			delete m_pGame;				// �������J��
			m_pGame = NULL;				// NULL��
		}

		break;
	case CManager::MODE_RESULT:			// ���U���g

		break;
	}

	char cMode[32];						// ���[�h��
	ConMode(mode, cMode);				// ���[�h�����ϊ�

	CFont *pFont = CRenderer::GetFont(CFont::FONTNUM_LOG);		// �t�H���g�擾

	// �t�H���g�ݒ�
	if (pFont != NULL)
	{// NULL�ȊO�̏ꍇ
		pFont->SetMessage(CFont::SETTYPE_LOG, "\n");
		pFont->SetMessage(CFont::SETTYPE_LOG, "Mode Change[%s]-----\n", cMode);
	}

	// ImGuiLog�ݒ�
	CImgui_ES::SetMessage("\n");
	CImgui_ES::SetMessage("Mode Change[%s]-----\n", cMode);

	if (m_pCamara[CAMERA_MAIN] != NULL)
		m_pCamara[CAMERA_MAIN]->Uninit();
	if (m_pCamara[CAMERA_GAME_MAP] != NULL)
		m_pCamara[CAMERA_GAME_MAP]->Uninit();

	int modeOld = m_mode;				// �O�񃂁[�h���L��
	m_mode = mode;						// ���[�h�ύX

	switch (mode)
	{// ���[�h�i����j
	case CManager::MODE_TITLE:			// �^�C�g��
		if (m_pTitle == NULL)
		{// NULL�̏ꍇ
			m_pTitle = new CTitle;		// �������m��

			if (m_pTitle != NULL)
			{// NULL�ȊO�̏ꍇ
				if (modeOld != mode)	// ���[�h���������
					m_pTitle->Load();	// ���[�h
				m_pTitle->Init();		// ����������
			}
		}

		break;
	case CManager::MODE_GAME:			// �Q�[��
		if (m_pGame == NULL)
		{// NULL�̏ꍇ
			m_pGame = new CGame;		// �������m��

			if (m_pGame != NULL)
			{// NULL�ȊO�̏ꍇ
				if (modeOld != mode)	// ���[�h���������
					m_pGame->Load();	// ���[�h
				m_pGame->Init();		// ����������
			}
		}

		break;
	case CManager::MODE_RESULT:			// ���U���g

		break;
	}
}

void CManager::ConMode(CManager::MODE mode, char *cMode)
{
	switch (mode)
	{// ���[�h�i����j
	case CManager::MODE_TITLE:			// �^�C�g��
		wsprintf(cMode, "Title");
		break;
	case CManager::MODE_GAME:			// �Q�[��
		wsprintf(cMode, "Game");
		break;
	case CManager::MODE_RESULT:			// ���U���g
		wsprintf(cMode, "Result");
		break;
	}
}

//=============================================================================
// �f�o�b�O�t�H���g����									(public)	*** CManager ***
//=============================================================================
void CManager::DebugFont(void)
{
	LPDIRECT3DDEVICE9 pDevice = m_pRenderer->GetDevice();

	CFont *pFont[CFont::FONTNUM_MAX] = {};

	// ������
	for (int nCntFont = 0; nCntFont < CFont::FONTNUM_MAX; nCntFont++)
	{
		pFont[nCntFont] = CRenderer::GetFont(nCntFont);
	}

	// �f�o�b�O���\���p�t�H���g(FPS�X�V)
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_TAB) == true)
	{
		if (pFont[CFont::FONTNUM_LOG] != NULL)
		{// NULL�ȊO�̏ꍇ
			pFont[CFont::FONTNUM_LOG]->SetMessage(CFont::SETTYPE_NEW, "");
		}
	}

	if (pFont[CFont::FONTNUM_DATA] != NULL)
	{
		pFont[CFont::FONTNUM_DATA]->SetMessage(CFont::SETTYPE_NEW, "");
	}

	if (pFont[CFont::FONTNUM_TEST] != NULL)
	{
		pFont[CFont::FONTNUM_TEST]->SetMessage(CFont::SETTYPE_NEW, "");
	}
}