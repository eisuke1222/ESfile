//=============================================================================
//
// ���j���[���� [menu.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "menu.h"			// ���j���[
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "sound.h"			// �T�E���h
#include "renderer.h"		// �����_���[
#include "scene3d.h"		// �V�[��3D
#include "camera.h"			// �J����
#include "title.h"			// �^�C�g��
#include "fade.h"			// �t�F�[�h

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\menu000.png"		// �e�N�X�`��1

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CMenu::m_apTexture = {};		// �e�N�X�`�����ւ̃|�C���^

//=============================================================================
// �R���X�g���N�^									(public)	*** CMenu ***
//=============================================================================
CMenu::CMenu() : CScene(CScene::PRIORITY_4)
{
	// �����l�ݒ�
	m_nSelectNum = 0;	// �Z���N�g�ԍ�
	m_nCntTimer = 0;	// �J�E���g������

	for (int nCntMenu = 0; nCntMenu < MENU_MAX_TEXTURE; nCntMenu++)
	{// �J�E���g
		m_apScene3D[nCntMenu] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CMenu ***
//=============================================================================
CMenu::~CMenu()
{

}

//=============================================================================
// ���[�h����							(public)	*** CMenu ***
//=============================================================================
HRESULT CMenu::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_apTexture == NULL)
	{// NULL�̏ꍇ
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_apTexture);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CMenu ***
//=============================================================================
void CMenu::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_apTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_apTexture->Release();		// ���
		m_apTexture = NULL;			// NULL��
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CMenu ***
//=============================================================================
CMenu *CMenu::Create(void)
{
	CMenu *pMenu = NULL;				// �|�C���^

	if (pMenu == NULL)
	{// NULL�̏ꍇ
		pMenu = new CMenu;				// �������m��
	}

	if (pMenu != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntMenu = 0; nCntMenu < MENU_MAX_TEXTURE; nCntMenu++)
		{// �J�E���g
			if (pMenu->m_apScene3D[nCntMenu] == NULL)
			{// NULL�ȊO�̏ꍇ
				pMenu->m_apScene3D[nCntMenu] = new CScene3D(CScene::PRIORITY_4);																				// �������m��
				
			//	pMenu->m_apScene3D[nCntMenu]->SetInitAll(
			//		D3DXVECTOR3(180.0f, (MENU_SIZE_Y * (2.5f * 1.5f)) - ((nCntMenu) * (MENU_SIZE_Y * 2.5f)), 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			//		D3DXVECTOR3(MENU_SIZE_X, MENU_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.25f), CScene3D::SCENE3DTYPE_NORMAL);		// �����l�ݒ�

				pMenu->m_apScene3D[nCntMenu]->SetInitAll(
					D3DXVECTOR3(180.0f, (MENU_SIZE_Y * (2.5f)) - ((nCntMenu - (nCntMenu / 2)) * (MENU_SIZE_Y * 2.5f)), 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(MENU_SIZE_X, MENU_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.25f), CScene3D::SCENE3DTYPE_NORMAL);		// �����l�ݒ�

				if (nCntMenu == 1)
					pMenu->m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

				pMenu->m_apScene3D[nCntMenu]->Init();																											// ����������
				pMenu->m_apScene3D[nCntMenu]->SetObjType(CScene::OBJTYPE_MENU);																					// �I�u�W�F�N�g�^�C�v�ݒ�
				pMenu->m_apScene3D[nCntMenu]->BindTexture(m_apTexture);																							// �e�N�X�`���ݒ�
				pMenu->m_apScene3D[nCntMenu]->SetTex(1, nCntMenu, D3DXVECTOR2(D3DXVECTOR2(1.0f, 0.25f)), D3DXVECTOR2(0.0f, 0.0f));
			}
		}
		pMenu->Init();								// ����������
	}

	return pMenu;			// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CMenu ***
//=============================================================================
HRESULT CMenu::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CMenu ***
//=============================================================================
void CMenu::Uninit(void)
{
	for (int nCntMenu = 0; nCntMenu < MENU_MAX_TEXTURE; nCntMenu++)
	{// �J�E���g
		if (m_apScene3D[nCntMenu] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apScene3D[nCntMenu]->Uninit();		// �I������
			m_apScene3D[nCntMenu] = NULL;			// NULL��
		}
	}

	CScene::Release();			// �����[�X
}

//=============================================================================
// �X�V����											(public)	*** CMenu ***
//=============================================================================
void CMenu::Update(void)
{
	// �J����
	CCamera *pCamera = CManager::GetCamera();							// �J�����擾
	//CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// ���̓f�o�C�X
	CInputXPad *pInputXPad = CManager::GetInputXPad();					// ���̓f�o�C�X
	CSound *pSound = CManager::GetSound();								// �T�E���h
	CFade::FADE fade = CFade::GetFade();								// �t�F�[�h�擾
	bool bPlayGame = CTitle::GetPlayGame();								// �v���C�Q�[��
	int nPlayerIn = 0;													// �v���C���[����

	if (CManager::GetPlayNum() == 0)
	{
		nPlayerIn = 0;
	}
	else
	{
		nPlayerIn = 1;
	}

	if (fade == CFade::FADE_NONE && bPlayGame == false)
	{// �t�F�[�h�����Ă��Ȃ����
		if (pCamera->GetDest() == false)
		{// �J�����������Ă��Ȃ�
			for (int nCntCont = 0; nCntCont < nPlayerIn; nCntCont++)
			{// �J�E���g
				//if (pInputKeyboard->GetTrigger(DIK_S) == true || pInputKeyboard->GetTrigger(DIK_DOWN) == true)
				if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN, nCntCont) == true || pInputXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, nCntCont) == true)
				{// ���L�[����������
					m_nSelectNum = (m_nSelectNum + 1) % MENU_MAX_TEXTURE;						// �I��������
					m_nCntTimer = 0;			// �J�E���g������

					if (m_nSelectNum == 1)
					{
						m_nSelectNum++;
					}

					pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
				}
				//else if (pInputKeyboard->GetTrigger(DIK_W) == true || pInputKeyboard->GetTrigger(DIK_UP) == true)
				else if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, nCntCont) == true || pInputXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, nCntCont) == true)
				{// ��L�[����������
					m_nSelectNum = (m_nSelectNum + MENU_MAX_TEXTURE - 1) % MENU_MAX_TEXTURE;	// �I�������
					m_nCntTimer = 0;			// �J�E���g������

					if (m_nSelectNum == 1)
					{
						m_nSelectNum--;
					}

					pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
				}
			}
		}
	}

	m_nCntTimer++;		// �J�E���g

	for (int nCntMenu = 0; nCntMenu < MENU_MAX_TEXTURE; nCntMenu++)
	{// �J�E���g
		if (m_apScene3D[nCntMenu] != NULL)
		{// NULL�ȊO�̏ꍇ
			if (nCntMenu == 1)
			{
				//m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));		// �F�ύX
				m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.0f));		// �F�ύX
			}
			else if (nCntMenu == m_nSelectNum)
			{// �I��ԍ��ƈ�v������
				m_apScene3D[nCntMenu]->Setsize(D3DXVECTOR3(MENU_SIZE_X * 1.3f, MENU_SIZE_Y * 1.3f, 0.0f));		// �傫���ύX

				if (m_nCntTimer % 90 < 45)
				{// ���t���[����
					m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_apScene3D[nCntMenu]->Getcol().a - (0.3f / 45.0f)));		// �F�ύX
				}
				else
				{// ���̑�
					m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_apScene3D[nCntMenu]->Getcol().a + (0.3f / 45.0f)));		// �F�ύX
				}
			}
			else
			{// ���̑�
				m_apScene3D[nCntMenu]->Setsize(D3DXVECTOR3(MENU_SIZE_X, MENU_SIZE_Y, 0.0f));					// �f�t�H���g�T�C�Y��
				m_apScene3D[nCntMenu]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));								// �f�t�H���g�J���[��
			}

			if (bPlayGame == true)
			{// �Q�[���ɐi��
				if (m_apScene3D[nCntMenu]->Getpos().x < 500.0f)
				{// ���ʒu�܂�
					m_apScene3D[nCntMenu]->Setpos(D3DXVECTOR3(m_apScene3D[nCntMenu]->Getpos().x + 5.0f, m_apScene3D[nCntMenu]->Getpos().y, m_apScene3D[nCntMenu]->Getpos().z));		// �ړ�
				}
			}

			m_apScene3D[nCntMenu]->Update();		// �X�V����
		}
	}

	if (fade == CFade::FADE_NONE && bPlayGame == false)
	{
		if (pCamera->GetDest() == false)
		{// �J�����������Ă��Ȃ�
			for (int nCntCont = 0; nCntCont < nPlayerIn; nCntCont++)
			{// �J�E���g
				//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true && bPlayGame == false)
				if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_A, nCntCont) == true || pInputXPad->GetTrigger(XINPUT_GAMEPAD_B, nCntCont) == true && bPlayGame == false)
				{// ENTER����������
					switch (m_nSelectNum)
					{// �I��ԍ�
					case 0:		// �Q�[��
						CTitle::SetPlayGame();
						break;
					case 1:		// �J�X�^��
						pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1200.0f, 0.0f, 0.0f), 1200, 90, CCamera::CPOSDEST_MENU_C);
						break;
					case 2:		// �`���[�g���A��
						CFade::Create(CManager::MODE_TUTORIAL, CFade::FADE_OUT);
						break;
					case 3:		// �^�C�g��
						pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 0.0f, 0.0f), 1200, 90, CCamera::CPOSDEST_MENU_T);
						break;
					}

					pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
				}
			}
		}
}

	if (pCamera->GetPosDest(CCamera::CPOSDEST_MENU_C) == true)
	{// �J�X�^���J�����Z�b�g����
		pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1200.0f), 1200, 90, CCamera::CPOSDEST_MENU_C);
		CTitle::SetTitle(CTitle::TITLE_CUSTOM);		// �J�X�^����
	}
	else if (pCamera->GetPosDest(CCamera::CPOSDEST_MENU_T) == true)
	{// �^�C�g���J�����Z�b�g����
		pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1200.0f), 1200, 90, CCamera::CPOSDEST_MENU_T);
		CTitle::SetTitle(CTitle::TITLE_LOGO);		// �^�C�g����
	}
}

//=============================================================================
// �`�揈��											(public)	*** CMenu ***
//=============================================================================
void CMenu::Draw(void)
{
	
}