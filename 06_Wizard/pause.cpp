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
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "sound.h"			// �T�E���h
#include "fade.h"			// �t�F�[�h
#include "texture.h"		// �e�N�X�`��

#include "scene2d.h"		// �V�[��2D
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

//=============================================================================
// �R���X�g���N�^									(public)	*** CPause ***
//=============================================================================
CPause::CPause() : CScene(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_PAUSE)
{
	// �����l�ݒ�
	m_nSelectNum	= 0;		// �I��ԍ�
	m_nCntTimer		= 0;		// ���ԃJ�E���^

	for (int nCntType= 0; nCntType < TYPE_MAX; nCntType++)
		m_apPauseBg[nCntType]	= NULL;		// �w�i

	for (int nCntSelect= 0; nCntSelect < SELECT_MAX; nCntSelect++)
		m_apSelect[nCntSelect]	= NULL;		// �I��
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CPause ***
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ���[�h����										(public)	*** CPause ***
//=============================================================================
HRESULT CPause::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CPause ***
//=============================================================================
void CPause::Unload(void)
{

}

//=============================================================================
// �N���G�C�g										(public)	*** CPause ***
//=============================================================================
CPause *CPause::Create(void)
{
	CPause *pPause = NULL;		// �|�C���^

	if (pPause == NULL)
	{// NULL�̏ꍇ
		pPause = new CPause;	// �������m��
	}

	if (pPause != NULL)
	{// NULL�ȊO�̏ꍇ
		pPause->Init();			// ����������
	}

	return pPause;		// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CPause ***
//=============================================================================
HRESULT CPause::Init(void)
{
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// ��ރJ�E���g
		if (m_apPauseBg[nCntType] == NULL)
		{// NULL�̏ꍇ
			// �������m��
			m_apPauseBg[nCntType] = new CScene2D(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_PAUSE);
			// �����l�ݒ�
			// �e�N�X�`�����f
			if (nCntType == TYPE_BG)			// �w�i
			{
				m_apPauseBg[nCntType]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
			}
			else if (nCntType == TYPE_FRAME)	// �g
			{
				m_apPauseBg[nCntType]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.48f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH * 0.18f, SCREEN_HEIGHT * 0.35f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
				m_apPauseBg[nCntType]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
			}
			else if (nCntType == TYPE_LINE)		// ��
			{
				m_apPauseBg[nCntType]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.35f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.001f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			}
			else if (nCntType == TYPE_MENU)		// ���j���[
			{
				m_apPauseBg[nCntType]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.25f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.075f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_apPauseBg[nCntType]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_PAUSE_LOGO));
			}
			// ����������
			m_apPauseBg[nCntType]->Init();
		}
	}

	if (m_pSelectChange == NULL)
	{// NULL�̏ꍇ
		// �������m��
		m_pSelectChange = new CScene2D(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_PAUSE);
		// �����l�ݒ�
		m_pSelectChange->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.58f - SCREEN_HEIGHT * 0.12f, 0.0f),
			D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.05f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0714f, 1.0f));
		// ����������
		m_pSelectChange->Init();
		// �e�N�X�`�����f
		m_pSelectChange->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_PAUSE_BG));
		m_pSelectChange->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.333f), D3DXVECTOR2(0.0f, 0.0f));
	}

	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{// �I���J�E���g
		if (m_apSelect[nCntSelect] == NULL)
		{// NULL�̏ꍇ
			// �������m��
			m_apSelect[nCntSelect] = new CScene2D(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_PAUSE);
			// �����l�ݒ�
			m_apSelect[nCntSelect]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.58f - SCREEN_HEIGHT * 0.12f + (SCREEN_HEIGHT * 0.12f * nCntSelect), 0.0f), 
				D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.05f, 0.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXVECTOR2(0.0714f, 1.0f));
			// ����������
			m_apSelect[nCntSelect]->Init();
			// �e�N�X�`�����f
			m_apSelect[nCntSelect]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_PAUSE_MENU));
			m_apSelect[nCntSelect]->SetTex(1, nCntSelect, D3DXVECTOR2(1.0f, 0.333f), D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	for (int nCntMark = 0; nCntMark < MARK_MAX; nCntMark++)
	{// �}�[�N�J�E���g
		if (m_apMark[nCntMark] == NULL)
		{// NULL�̏ꍇ
		 // �������m��
			m_apMark[nCntMark] = new CScene2D(CScene::PRIORITY_PAUSE, CScene::OBJTYPE_PAUSE);
			// �����l�ݒ�
			m_apMark[nCntMark]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.135f) + ((SCREEN_WIDTH * 0.135f * 2.0f) * nCntMark), SCREEN_HEIGHT * 0.58f - SCREEN_HEIGHT * 0.12f, 0.0f),
				D3DXVECTOR3(SCREEN_WIDTH * 0.06f, SCREEN_WIDTH * 0.06f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.333f));
			// ����������
			m_apMark[nCntMark]->Init();
			// �e�N�X�`�����f
			m_apMark[nCntMark]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
			m_apMark[nCntMark]->SetTex(14, 7, D3DXVECTOR2(0.0714f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CPause ***
//=============================================================================
void CPause::Uninit(void)
{
	for (int nCntType = 0; nCntType < TYPE_MAX; nCntType++)
	{// ��ރJ�E���g
		if (m_apPauseBg[nCntType] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apPauseBg[nCntType]->Uninit();	// �I������
			m_apPauseBg[nCntType] = NULL;		// NULL��
		}
	}

	if (m_pSelectChange != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pSelectChange->Uninit();				// �I������
		m_pSelectChange = NULL;					// NULL��
	}

	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{// �I���J�E���g
		if (m_apSelect[nCntSelect] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apSelect[nCntSelect]->Uninit();	// �I������
			m_apSelect[nCntSelect] = NULL;		// NULL��
		}
	}

	for (int nCntMark = 0; nCntMark < MARK_MAX; nCntMark++)
	{// �}�[�N�J�E���g
		if (m_apMark[nCntMark] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_apMark[nCntMark]->Uninit();		// �I������
			m_apMark[nCntMark] = NULL;			// NULL��
		}
	}
	
	CScene::SetDeath();		// ���S�t���O
}

//=============================================================================
// �X�V����											(public)	*** CPause ***
//=============================================================================
void CPause::Update(void)
{
	CSound *pSound = CManager::GetSound();

	CInputKeyboard *pKey	= CManager::GetInputKeyboard();	// �L�[�{�[�h�擾
	CInputXPad		*pXPad	= CManager::GetInputXPad();			// X�p�b�h�擾
	CFade::FADE		fade	= CFade::GetFade();				// �t�F�[�h�擾

	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ�
		if (pKey->GetTrigger(DIK_W) == true || pKey->GetTrigger(DIK_UP) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, 0) == true)
		{// �L�[�{�[�h�iW�j�������ꂽ�AX�p�b�h�iL�X�e�B�b�N��j�������ꂽ
			m_nSelectNum = (m_nSelectNum + SELECT_MAX - 1) % SELECT_MAX;

			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SERECT000);
			}
		}
		else if (pKey->GetTrigger(DIK_S) == true || pKey->GetTrigger(DIK_DOWN) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, 0) == true)
		{// �L�[�{�[�h�iS�j�������ꂽ�AX�p�b�h�iL�X�e�B�b�N���j�������ꂽ
			m_nSelectNum = (m_nSelectNum + 1) % SELECT_MAX;

			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SERECT000);
			}
		}
	}

	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{// �I���J�E���g
		if (m_apSelect[nCntSelect] != NULL)
		{// NULL�ȊO�̏ꍇ
			if (nCntSelect == m_nSelectNum)
			{// �I��ԍ��ƈ�v
				m_apSelect[nCntSelect]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �F�ύX
				
				if (m_pSelectChange != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pSelectChange->Setpos(m_apSelect[nCntSelect]->Getpos());
					m_pSelectChange->SetTex(1, nCntSelect, D3DXVECTOR2(1.0f, 0.333f), D3DXVECTOR2(0.0f, 0.0f));
				}

				for (int nCntMark = 0; nCntMark < MARK_MAX; nCntMark++)
				{// �}�[�N�J�E���g
					if (m_apMark[nCntMark] != NULL)
					{// NULL�ȊO�̏ꍇ
						m_apMark[nCntMark]->Setpos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f - (SCREEN_WIDTH * 0.135f) + ((SCREEN_WIDTH * 0.135f * 2.0f) * nCntMark), m_apSelect[nCntSelect]->Getpos().y, 0.0f));
						m_apMark[nCntMark]->Setrot(m_apMark[nCntMark]->Getrot() + 0.05f);
					}
				}
			}
			else
			{// �I��ԍ��ȊO
				m_apSelect[nCntSelect]->Setcol(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));	// �F�ύX
			}
		}
	}

	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ�
		if (pKey->GetTrigger(DIK_RETURN) == true || pKey->GetTrigger(DIK_E) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
		{// �L�[�{�[�h�iENTER�j�������ꂽ�AX�p�b�h�iB�A4�j�������ꂽ
			if (pSound != NULL)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
			}

			switch (m_nSelectNum)
			{// �I��ԍ�
			case SELECT_CONTINUE:	// �R���e�j���[
				CManager::GetGame()->PauseSeting();		// �|�[�Y����
				break;
			case SELECT_RETRY:		// ���g���C
				CFade::Create(CManager::MODE_GAME);		// �t�F�[�h�����i�Q�[�����[�h�ցj
				break;
			case SELECT_QUIT:		// �N�C�b�g
				CFade::Create(CManager::MODE_TITLE);	// �t�F�[�h�����i�^�C�g�����[�h�ցj
				break;
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