//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "fade.h"			// �t�F�[�h
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MC_FADE_TIME	(60.0f)						// �t�F�[�h����
#define MC_ADDCOLOR_A	(1.0f / MC_FADE_TIME)		// �����x���Z��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CFade::FADE			CFade::m_fade		= CFade::FADE_NONE;		// �t�F�[�h���

//=============================================================================
// �R���X�g���N�^										(public)	*** CFade ***
//=============================================================================
CFade::CFade() : CScene2D(CScene::PRIORITY_FADE, CScene::OBJTYPE_FADE)
{
	// �����l�ݒ�
	m_fadeNum	= 0;							// �t�F�[�h�ԍ�
	m_modeNext	= CManager::MODE_NONE;			// ���̃��[�h
	m_gameNext	= CGame::GAME_NONE;				// ���̃Q�[��
	m_nCntTimer = 0;							// ���ԃJ�E���g
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CFade ***
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// ���[�h����										(public)	*** CFade ***
//=============================================================================
HRESULT CFade::Load(void)
{
	return S_OK;
}

//=============================================================================
// �A�����[�h����										(public)	*** CFade ***
//=============================================================================
void CFade::Unload(void)
{

}

//=============================================================================
// �N���G�C�g										(public)	*** CFade ***
//=============================================================================
CFade *CFade::Create(CManager::MODE mode, FADE fade)
{
	CFade *pFade = NULL;					// �|�C���^

	if (m_fade == FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ�
		if (pFade == NULL)
		{// NULL�̏ꍇ
			pFade = new CFade;				// �������m��
		}

		if (pFade != NULL)
		{// NULL�ȊO�̏ꍇ
			pFade->Init(mode, fade);		// ����������
		}

		return pFade;	// �l��Ԃ�
	}

	return NULL;	// �l��Ԃ�
}
CFade *CFade::Create(CGame::GAME game, FADE fade)
{
	CFade *pFade = NULL;					// �|�C���^

	if (m_fade == FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ�
		if (pFade == NULL)
		{// NULL�̏ꍇ
			pFade = new CFade;				// �������m��
		}

		if (pFade != NULL)
		{// NULL�ȊO�̏ꍇ
			pFade->Init(game, fade);		// ����������
		}

		return pFade;	// �l��Ԃ�
	}

	return NULL;	// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CFade ***
//=============================================================================
HRESULT CFade::Init(CManager::MODE mode, FADE fade)
{
	// �����l�ݒ�
	if (fade == FADE_OUT)
	{// �t�F�[�h�A�E�g�̏ꍇ
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	}
	else
	{// ���̑�
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	}
	
	CScene2D::Init();		// ����������

	m_fadeNum = 0;
	m_fade = FADE_OUT;		// �t�F�[�h�A�E�g
	m_modeNext = mode;		// �����[�h
	m_nCntTimer = 0;		// �J�E���g

	return S_OK;	// �l��Ԃ�
}
HRESULT CFade::Init(CGame::GAME game, FADE fade)
{
	// �����l�ݒ�
	if (fade == FADE_OUT)
	{// �t�F�[�h�A�E�g�̏ꍇ
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	}
	else
	{// ���̑�
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	}

	CScene2D::Init();		// ����������

	m_fadeNum = 1;
	m_fade = FADE_OUT;		// �t�F�[�h�A�E�g
	m_gameNext = game;		// ���Q�[��
	m_nCntTimer = 0;		// �J�E���g

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CFade ***
//=============================================================================
void CFade::Uninit(void)
{
	CScene2D::Uninit();		// �I������
}

//=============================================================================
// �X�V����											(public)	*** CFade ***
//=============================================================================
void CFade::Update(void)
{	
	D3DXCOLOR col = CScene2D::Getcol();		// �F���擾
	bool bDelete = false;					// �j������

	if (m_fade != FADE_NONE)
	{// �t�F�[�h�g���Ă�����
		if (m_fade == FADE_IN)
		{// �t�F�[�h�C��
			col.a -= MC_ADDCOLOR_A;			// ������

			if (col.a <= 0.0f)
			{// ����
				col.a = 0.0f;				// �����x�𒲐�
				m_fade = FADE_NONE;			// �������Ă��Ȃ����
				m_nCntTimer = 0;			// �J�E���g������

				bDelete = true;				// �j������
			}
		}
		else if (m_fade == FADE_OUT)
		{// �t�F�[�h�A�E�g
			col.a += MC_ADDCOLOR_A;			// �s������

			if (col.a >= 1.0f)
			{// �s����
				col.a = 1.0f;				// �����x�𒲐�

				m_nCntTimer++;				// �J�E���^�����Z
				if (m_nCntTimer > (int)(MC_FADE_TIME * 0.5f))
				{// ���t���[����
					m_fade = FADE_IN;		// �t�F�[�h�C��
				}
				else if (m_nCntTimer == (int)(MC_FADE_TIME * 0.25f))
				{// �t���[���̒���
					if (m_fadeNum == 0)
					{
						CManager::SetMode(m_modeNext);				// ���[�h�ύX
					}
					else
					{
						CManager::GetGame()->SetGame(m_gameNext);	// �Q�[���ύX
					}

					CScene::SetStartUp(0);				// �X�V�͈͂�������
					CScene::SetUpdateTime(1);			// �X�V�J�E���g��������
				}
			}
		}
	}

	CScene2D::Setcol(col);					// �F��ݒ�
	CScene2D::Update();						// �X�V����

	if (bDelete == true)
	{// �j������
		Uninit();							// �I������
	}
}

//=============================================================================
// �`�揈��											(public)	*** CFade ***
//=============================================================================
void CFade::Draw(void)
{
	CScene2D::Draw();						// �`�揈��
}