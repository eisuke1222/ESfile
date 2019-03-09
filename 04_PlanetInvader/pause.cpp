//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "pause.h"			// �|�[�Y
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "sound.h"			// �T�E���h
#include "scene2d.h"		// �V�[��2D
#include "game.h"			// �Q�[��
#include "tutorial.h"		// �`���[�g���A��
#include "fade.h"			// �t�F�[�h

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME_1	"data\\TEXTURE\\menu002.png"		// �e�N�X�`��1
#define TEXTURE_NAME_2	"data\\TEXTURE\\menu001.png"		// �e�N�X�`��2
#define TEXTURE_NAME_3	"data\\TEXTURE\\titlelogo.png"		// �e�N�X�`��3

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		CPause::m_apTexture[CPause::TEXTURE_MAX] = {};		// �e�N�X�`�����ւ̃|�C���^

//=============================================================================
// �R���X�g���N�^									(public)	*** CPause ***
//=============================================================================
CPause::CPause() : CScene(CScene::PRIORITY_PAUSE)
{
	// �����l�ݒ�
	m_nSelectNum = 0;		// �I��ԍ�
	m_nCntTimer = 0;		// ���ԃJ�E���^

	for (int nCntPause = 0; nCntPause < PAUSE_MAX_BG; nCntPause++)
	{// �J�E���g
		m_apScene2D[nCntPause] = NULL;
	}
	for (int nCntPause = 0; nCntPause < PAUSE_MAX_SELECT; nCntPause++)
	{// �J�E���g
		m_apScene2DSelect[nCntPause] = NULL;
	}
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CPause ***
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ���[�h����							(public)	*** CPause ***
//=============================================================================
HRESULT CPause::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	if (m_apTexture[0] == NULL)
	{// NULL�̏ꍇ
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_apTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_apTexture[1]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_3, &m_apTexture[2]);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CPause ***
//=============================================================================
void CPause::Unload(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TEXTURE_MAX; nCntTex++)
	{// �J�E���g
		if (m_apTexture[nCntTex] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apTexture[nCntTex]->Release();		// ���
			m_apTexture[nCntTex] = NULL;			// NULL��
		}
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CPause ***
//=============================================================================
CPause *CPause::Create(void)
{
	CPause *pPause = NULL;				// �|�C���^

	if (pPause == NULL)
	{// NULL�̏ꍇ
		pPause = new CPause;			// �������m��
	}

	if (pPause != NULL)
	{// NULL�ȊO�̏ꍇ
		for (int nCntPause = 0; nCntPause < PAUSE_MAX_BG; nCntPause++)
		{// �J�E���g
			if (pPause->m_apScene2D[nCntPause] == NULL)
			{// NULL�ȊO�̏ꍇ
				pPause->m_apScene2D[nCntPause] = new CScene2D(CScene::PRIORITY_PAUSE);															// �������m��
				switch (nCntPause)
				{// �J�E���g�ԍ�
				case 0:
					pPause->m_apScene2D[nCntPause]->SetInitAll(
						D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 
						D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.8f), D3DXVECTOR2(1.0f, 1.0f));	// �����l�ݒ�

					break;
				case 1:
					pPause->m_apScene2D[nCntPause]->SetInitAll(
						D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 
						D3DXVECTOR3(200.0f, 260.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));							// �����l�ݒ�

					break;
				case 2:
					pPause->m_apScene2D[nCntPause]->SetInitAll(
						D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 170.0f, 0.0f),
						D3DXVECTOR3(PAUSE_SIZE_X, PAUSE_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.25f));			// �����l�ݒ�

					break;
				case 3:
					pPause->m_apScene2D[nCntPause]->SetInitAll(
						D3DXVECTOR3(220.0f, 60.0f, 0.0f),
						D3DXVECTOR3(200.0f, 40.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));							// �����l�ݒ�

					break;
				}
				pPause->m_apScene2D[nCntPause]->Init();																							// ����������
				pPause->m_apScene2D[nCntPause]->SetObjType(CScene::OBJTYPE_PAUSE);																// �I�u�W�F�N�g�^�C�v�ݒ�

				switch (nCntPause)
				{// �J�E���g�ԍ�
				case 0:

					break;
				case 1:
					pPause->m_apScene2D[nCntPause]->BindTexture(m_apTexture[TEXTURE_PAUSE_BG]);													// �e�N�X�`���ݒ�

					break;
				case 2:
					pPause->m_apScene2D[nCntPause]->BindTexture(m_apTexture[TEXTURE_PAUSE]);													// �e�N�X�`���ݒ�
					pPause->m_apScene2D[nCntPause]->SetTex(1, 0, D3DXVECTOR2(0.0f, 0.0f));

					break;
				case 3:
					pPause->m_apScene2D[nCntPause]->BindTexture(m_apTexture[TEXTURE_TITLE]);													// �e�N�X�`���ݒ�

					break;
				}
			}
		}
		for (int nCntPause = 0; nCntPause < PAUSE_MAX_SELECT; nCntPause++)
		{// �J�E���g
			if (pPause->m_apScene2DSelect[nCntPause] == NULL)
			{// NULL�ȊO�̏ꍇ
				pPause->m_apScene2DSelect[nCntPause] = new CScene2D(CScene::PRIORITY_PAUSE);													// �������m��
				pPause->m_apScene2DSelect[nCntPause]->SetInitAll(
					D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 60.0f - 110.0f + ((nCntPause) * 110.0f), 0.0f),
					D3DXVECTOR3(PAUSE_SIZE_X, PAUSE_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.25f));				// �����l�ݒ�

				pPause->m_apScene2DSelect[nCntPause]->Init();																					// ����������
				pPause->m_apScene2DSelect[nCntPause]->SetObjType(CScene::OBJTYPE_PAUSE);														// �I�u�W�F�N�g�^�C�v�ݒ�
				pPause->m_apScene2DSelect[nCntPause]->BindTexture(m_apTexture[TEXTURE_PAUSE]);																// �e�N�X�`���ݒ�
				pPause->m_apScene2DSelect[nCntPause]->SetTex(1, nCntPause + 1, D3DXVECTOR2(0.0f, 0.0f));
			}
		}
		pPause->Init();					// ����������
	}

	return pPause;			// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CPause ***
//=============================================================================
HRESULT CPause::Init(void)
{
	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CPause ***
//=============================================================================
void CPause::Uninit(void)
{
	for (int nCntPause = 0; nCntPause < PAUSE_MAX_BG; nCntPause++)
	{// �J�E���g
		if (m_apScene2D[nCntPause] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apScene2D[nCntPause]->Uninit();			// �I������
			m_apScene2D[nCntPause] = NULL;				// NULL��
		}
	}
	for (int nCntPause = 0; nCntPause < PAUSE_MAX_SELECT; nCntPause++)
	{// �J�E���g
		if (m_apScene2DSelect[nCntPause] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apScene2DSelect[nCntPause]->Uninit();		// �I������
			m_apScene2DSelect[nCntPause] = NULL;		// NULL��
		}
	}

	CScene::Release();
}

//=============================================================================
// �X�V����											(public)	*** CPause ***
//=============================================================================
void CPause::Update(void)
{
	CManager::MODE mode = CManager::GetMode();						// ���[�h
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// ���̓f�o�C�X
	CInputXPad *pInputXPad = CManager::GetInputXPad();				// ���̓f�o�C�X
	CSound *pSound = CManager::GetSound();							// �T�E���h
	CFade::FADE fade = CFade::GetFade();							// �t�F�[�h�擾

	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ����
		for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
		{
			//if (pInputKeyboard->GetTrigger(DIK_S) == true || pInputKeyboard->GetTrigger(DIK_DOWN) == true)
			if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN, nCnt) == true || pInputXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, nCnt) == true)
			{// ���L�[����������
				m_nSelectNum = (m_nSelectNum + 1) % 3;					// �I��������
				m_nCntTimer = 0;			// �J�E���g������

				pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
			}
			//else if (pInputKeyboard->GetTrigger(DIK_W) == true || pInputKeyboard->GetTrigger(DIK_UP) == true)
			else if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_DPAD_UP, nCnt) == true || pInputXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, nCnt) == true)
			{// ��L�[����������
				m_nSelectNum = (m_nSelectNum + 2) % 3;					// �I�������
				m_nCntTimer = 0;			// �J�E���g������

				pSound->PlaySoundA(CSound::SOUND_LABEL_SE001);
			}
		}
	}

	m_nCntTimer++;		// �J�E���g

	for (int nCntPause = 0; nCntPause < PAUSE_MAX_SELECT; nCntPause++)
	{// �J�E���g
		if (m_apScene2DSelect[nCntPause] != NULL)
		{// NULL�ȊO�̏ꍇ
			if (nCntPause == m_nSelectNum)
			{// �I��ԍ��ƈ�v������
				if (m_nCntTimer % 90 < 45)
				{// ���t���[����
					m_apScene2DSelect[nCntPause]->Setcol(D3DXCOLOR(m_apScene2DSelect[nCntPause]->Getcol().r - (0.5f / 45.0f), m_apScene2DSelect[nCntPause]->Getcol().g - (0.5f / 45.0f), m_apScene2DSelect[nCntPause]->Getcol().b - (0.5f / 45.0f), 1.0f));		// �F�ύX
				}
				else
				{// ���̑�
					m_apScene2DSelect[nCntPause]->Setcol(D3DXCOLOR(m_apScene2DSelect[nCntPause]->Getcol().r + (0.5f / 45.0f), m_apScene2DSelect[nCntPause]->Getcol().g + (0.5f / 45.0f), m_apScene2DSelect[nCntPause]->Getcol().b + (0.5f / 45.0f), 1.0f));		// �F�ύX
				}
			}
			else
			{// ���̑�
				m_apScene2DSelect[nCntPause]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �F�ύX
			}

			m_apScene2DSelect[nCntPause]->Update();			// �X�V����
		}
	}

	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ����
		for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
		{
			//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_A, nCnt) == true || pInputXPad->GetTrigger(XINPUT_GAMEPAD_B, nCnt) == true)
			{// ENTER�L�[����������
				switch (m_nSelectNum)
				{// �I��ԍ�
				case 0:			// �I������
					if (mode == CManager::MODE_TUTORIAL)
					{
						CTutorial::SetPause(false);
					}
					else
					{
						CGame::SetPause(false);
					}

					break;
				case 1:			// ���g���C
					if (mode == CManager::MODE_TUTORIAL)
					{
						CFade::Create(CManager::MODE_TUTORIAL, CFade::FADE_OUT);
					}
					else
					{
						CFade::Create(CManager::MODE_GAME, CFade::FADE_OUT);
					}

					break;
				case 2:			// �^�C�g��
					CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);

					break;
				}

				pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
			}
		}
	}
}

//=============================================================================
// �`�揈��											(public)	*** CPause ***
//=============================================================================
void CPause::Draw(void)
{
	
}