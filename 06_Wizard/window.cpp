//=============================================================================
//
// �E�B���h�E���� [window.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "window.h"			// �E�B���h�E
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "fade.h"			// �t�F�[�h
#include "texture.h"		// �e�N�X�`��

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

//=============================================================================
// �R���X�g���N�^										(public)	*** CWindow ***
//=============================================================================
CWindow::CWindow() : CScene(CScene::PRIORITY_UI, CScene::OBJTYPE_WINDOW)
{

}

//=============================================================================
// �f�X�g���N�^										(public)	*** CWindow ***
//=============================================================================
CWindow::~CWindow()
{

}

//=============================================================================
// ���[�h����										(public)	*** CWindow ***
//=============================================================================
HRESULT CWindow::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CWindow ***
//=============================================================================
void CWindow::Unload(void)
{

}

//=============================================================================
// �N���G�C�g										(public)	*** CWindow ***
//=============================================================================
CWindow *CWindow::Create(WINDOW window)
{
	CWindow *pWindow = NULL;			// �|�C���^

	if (pWindow == NULL)
	{// NULL�̏ꍇ
		if(window == WINDOW_YESNO)		// �G�t�F�N�g2D
			pWindow = new CWindow_YESNO;	// �������m��
	}

	if (pWindow != NULL)
	{// NULL�ȊO�̏ꍇ
		pWindow->Init();				// ����������
	}

	return pWindow;		// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CWindow ***
//=============================================================================
void CWindow::Uninit(void)
{
	CScene::SetDeath();		// ���S�t���O
}

//-----------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^									(public)	*** CWindow_YESNO ***
//=============================================================================
CWindow_YESNO::CWindow_YESNO() : CWindow()
{
	// �����l�ݒ�
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
		m_pScene2D[nCount] = NULL;		// �V�[��2D

	m_nSelectNum = 0;
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CWindow_YESNO ***
//=============================================================================
CWindow_YESNO::~CWindow_YESNO()
{

}

//=============================================================================
// ���[�h����									(public)	*** CWindow_YESNO ***
//=============================================================================
HRESULT CWindow_YESNO::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����									(public)	*** CWindow_YESNO ***
//=============================================================================
void CWindow_YESNO::Unload(void)
{

}

//=============================================================================
// ����������									(public)	*** CWindow_YESNO ***
//=============================================================================
HRESULT CWindow_YESNO::Init(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// �J�E���g
		m_pScene2D[nCount] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_WINDOW);

		if (nCount == TYPE_BG)					// �w�i
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pScene2D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
		}
		else if (nCount == TYPE_MESSAGE_BG)		// ���b�Z�[�W�w�i
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 30), 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 28, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
		}
		else if (nCount == TYPE_MESSAGE)		// ���b�Z�[�W
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 30), 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 9, SCREEN_HEIGHT / 28, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pScene2D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_END));
		}
		else if (nCount == TYPE_SELECT_BG)		// �I��w�i
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 25), SCREEN_HEIGHT / 2 + (SCREEN_HEIGHT / 8 / 2), 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH / 28, SCREEN_HEIGHT / 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 0.5f));
			//m_pScene2D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_YESNO));
		}
		else if (nCount == TYPE_YES)			// �I�����i�͂��j
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2 - (SCREEN_WIDTH / 25), SCREEN_HEIGHT / 2 + (SCREEN_HEIGHT / 8 / 2), 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH / 28, SCREEN_HEIGHT / 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
			m_pScene2D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_YESNO));
		}
		else if (nCount == TYPE_NO)				// �I�����i�������j
		{
			m_pScene2D[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2 + (SCREEN_WIDTH / 25), SCREEN_HEIGHT / 2 + (SCREEN_HEIGHT / 8 / 2), 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH / 28, SCREEN_HEIGHT / 30, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pScene2D[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_YESNO));
		}
		// ����������
		m_pScene2D[nCount]->Init();

		if (nCount == TYPE_YES)					// �I�����i�͂��j
		{
			m_pScene2D[nCount]->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
		}
		else if (nCount == TYPE_NO)				// �I�����i�������j
		{
			m_pScene2D[nCount]->SetTex(1, 1, D3DXVECTOR2(1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	return S_OK;	// �l��Ԃ�
}	

//=============================================================================
// �I������										(public)	*** CWindow_YESNO ***
//=============================================================================
void CWindow_YESNO::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// �J�E���g
		m_pScene2D[nCount]->Uninit();	// �I������
		m_pScene2D[nCount] = NULL;		// NULL��
	}

	CWindow::Uninit();		// �I������
}

//=============================================================================
// �X�V����										(public)	*** CWindow_YESNO ***
//=============================================================================
void CWindow_YESNO::Update(void)
{
	CInputKeyboard	*pKey = CManager::GetInputKeyboard();		// �L�[�{�[�h�擾
	CInputXPad		*pXPad = CManager::GetInputXPad();			// X�p�b�h�擾

	if (pKey->GetTrigger(DIK_A) == true || pKey->GetTrigger(DIK_LEFT) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_LEFT, 0) == true)
	{// �L�[�{�[�h�iA�j�������ꂽ�AX�p�b�h�iL�X�e�B�b�N���j�������ꂽ
		m_nSelectNum = (m_nSelectNum + (TYPE_MAX - TYPE_YES) - 1) % (TYPE_MAX - TYPE_YES);
	}
	else if (pKey->GetTrigger(DIK_D) == true || pKey->GetTrigger(DIK_RIGHT) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_RIGHT, 0) == true)
	{// �L�[�{�[�h�iD�j�������ꂽ�AX�p�b�h�iL�X�e�B�b�N�E�j�������ꂽ
		m_nSelectNum = (m_nSelectNum + 1) % (TYPE_MAX - TYPE_YES);
	}

	for (int nCount = TYPE_YES; nCount < TYPE_MAX; nCount++)
	{// �J�E���g
		if (m_pScene2D[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			if (nCount == m_nSelectNum + TYPE_YES)
			{// ������������
				m_pScene2D[nCount]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				if (m_pScene2D[TYPE_SELECT_BG] != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pScene2D[TYPE_SELECT_BG]->Setpos(m_pScene2D[nCount]->Getpos());
				}
			}
			else
			{// ���̑�
				m_pScene2D[nCount]->Setcol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			}
		}
	}
}

//=============================================================================
// �`�揈��										(public)	*** CWindow_YESNO ***
//=============================================================================
void CWindow_YESNO::Draw(void)
{

}