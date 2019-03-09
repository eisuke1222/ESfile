//=============================================================================
//
// UI���� [ui.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "ui.h"				// Ui
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "fade.h"			// �t�F�[�h
#include "texture.h"		// �e�N�X�`��
#include "title.h"			// �^�C�g��
#include "gauge.h"			// �Q�[�W
#include "ranking.h"		// �����L���O

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
// �R���X�g���N�^										(public)	*** CUi ***
//=============================================================================
CUi::CUi() : CScene(CScene::PRIORITY_UI, CScene::OBJTYPE_UI)
{

}

//=============================================================================
// �f�X�g���N�^										(public)	*** CUi ***
//=============================================================================
CUi::~CUi()
{

}

//=============================================================================
// ���[�h����										(public)	*** CUi ***
//=============================================================================
HRESULT CUi::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CUi ***
//=============================================================================
void CUi::Unload(void)
{

}

//=============================================================================
// �N���G�C�g										(public)	*** CUi ***
//=============================================================================
CUi *CUi::Create(UI ui)
{
	CUi *pUi = NULL;				// �|�C���^

	if (pUi == NULL)
	{// NULL�̏ꍇ
		if(ui == UI_TITLE)			// �������m��
			pUi = new CUi_Title;		// UI�^�C�g��
		else if (ui == UI_GAME)		// �������m��
			pUi = new CUi_Game;			// UI�Q�[��
		else if (ui == UI_RESULT)	// �������m��
			pUi = new CUi_Result;		// UI���U���g
	}

	if (pUi != NULL)
	{// NULL�ȊO�̏ꍇ
		pUi->Init();				// ����������
	}

	return pUi;	// �l��Ԃ�
}

//=============================================================================
// �I������											(public)	*** CUi ***
//=============================================================================
void CUi::Uninit(void)
{
	CScene::SetDeath();		// ���S�t���O
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
bool CUi_Title::m_bUiEdit = false;		// Ui�ύX����

//=============================================================================
// �R���X�g���N�^										(public)	*** CUi_Title ***
//=============================================================================
CUi_Title::CUi_Title() : CUi()
{
	m_bUiEdit = false;		// Ui�ύX����

	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
		m_pUi[nCount] = NULL;		// UI

	m_pPress = NULL;		// �v���X
	m_nCntPress = 0;		// �v���X�J�E���g
	
	m_nCntTitle = 0;		// �^�C�g���J�E���g
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CUi_Title ***
//=============================================================================
CUi_Title::~CUi_Title()
{

}

//=============================================================================
// ���[�h����										(public)	*** CUi_Title ***
//=============================================================================
HRESULT CUi_Title::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CUi_Title ***
//=============================================================================
void CUi_Title::Unload(void)
{

}

//=============================================================================
// ����������										(public)	*** CUi_Title ***
//=============================================================================
HRESULT CUi_Title::Init(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// ��ރJ�E���g
		if (m_pUi[nCount] == NULL)
		{// NULL�̏ꍇ		
			m_pUi[nCount] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);		// �������m��

			// �����l�ݒ�
			// �e�N�X�`�����f
			if (nCount == TYPE_TITLELOGO_BG)			// �^�C�g�����S�w�i
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 2) - ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)), (SCREEN_WIDTH / 6), 0.0f), D3DXVECTOR3(0.0f, (SCREEN_WIDTH / 12), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_TITLE_LOGO_BG));
			}
			else if (nCount == TYPE_TITLELOGO)			// �^�C�g�����S
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_WIDTH / 6), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25), (SCREEN_WIDTH / 12), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_TITLE_LOGO));
			}
			else if (nCount == TYPE_MENU_SELECT)		// �I��
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.8f, SCREEN_HEIGHT * 0.32f - SCREEN_HEIGHT * 0.18f + (SCREEN_HEIGHT * 0.18f * (nCount - TYPE_MENU_SELECT)), 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.08f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.25f));
			}
			else if (nCount == TYPE_MENU_SELECT_MARK)	// �I���}�[�N
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.8f - SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.32f - SCREEN_HEIGHT * 0.18f + (SCREEN_HEIGHT * 0.18f * (nCount - TYPE_MENU_SELECT_MARK)), 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH * 0.08f, SCREEN_WIDTH * 0.08f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0714f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_PLAYER_SKILL));
			}
			else										// �I����
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.84f, SCREEN_HEIGHT * 0.32f - SCREEN_HEIGHT * 0.18f + (SCREEN_HEIGHT * 0.18f * (nCount - TYPE_MENU_GAME)), 0.0f),
					D3DXVECTOR3(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT * 0.08f, 0.0f), D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.0f), D3DXVECTOR2(1.0f, 0.25f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_TITLE_MENU));
			}

			// ����������
			m_pUi[nCount]->Init();
			
			if (nCount == TYPE_MENU_SELECT_MARK)		// �I���}�[�N
			{
				m_pUi[nCount]->SetTex(14, 7, D3DXVECTOR2(0.0714f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if(TYPE_MENU_SELECT < nCount)			// �I����
			{
				m_pUi[nCount]->SetTex(1, (nCount - TYPE_MENU_GAME), D3DXVECTOR2(1.0f, 0.25f), D3DXVECTOR2(0.0f, 0.0f));
			}

			// �`��ҏW
			m_pUi[nCount]->DrawEdit(false, false, false);
		}
	}

	// �v���X
	if (m_pPress == NULL)
	{// NULL�̏ꍇ
		m_pPress = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
		m_pPress->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 6, SCREEN_HEIGHT / 14, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
		m_pPress->Init();
		m_pPress->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_PRESS));
	}

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CUi_Title ***
//=============================================================================
void CUi_Title::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// ��ރJ�E���g
		if (m_pUi[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pUi[nCount]->Uninit();	// �I������
			m_pUi[nCount] = NULL;		// NULL��
		}
	}

	if (m_pPress != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pPress->Uninit();		// �I������
		m_pPress = NULL;		// NULL��
	}

	CUi::Uninit();		// �I������
}

//=============================================================================
// �X�V����											(public)	*** CUi_Title ***
//=============================================================================
void CUi_Title::Update(void)
{
	bool bEdit = false;											// �ύX����
	bool bTitle = CManager::GetTitle()->GetTitle();				// �^�C�g���擾
	int nSelectNum = CManager::GetTitle()->GetSelectNum();		// �I��ԍ��擾
	bool bSelectSet = CManager::GetTitle()->GetSelectSet();		// �I���擾

	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// ��ރJ�E���g
		if (m_pUi[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			float fcol_a = m_pUi[nCount]->Getcol().a;

			if (nCount == TYPE_TITLELOGO_BG || nCount == TYPE_TITLELOGO)
			{// �^�C�g�����S
				if (bTitle == true)
				{// �^�C�g���\��
					if (fcol_a != 1.0f)
					{// �s�����ȊO
						fcol_a = m_pUi[nCount]->Getcol().a + 0.03f;
						if (1.0f < fcol_a)	fcol_a = 1.0f;
						bEdit = true;
					}
				}
				else
				{// �^�C�g����\��
					if (fcol_a != 0.0f)
					{// �����ȊO
						fcol_a = m_pUi[nCount]->Getcol().a - 0.03f;
						if (fcol_a < 0.0f)	fcol_a = 0.0f;
						bEdit = true;
					}
				}

				m_pUi[nCount]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, fcol_a));	// �F�ύX

				if (nCount == TYPE_TITLELOGO_BG)
				{// �^�C�g�����S�w�i
					if (bTitle == true)
					{// �^�C�g���\��
						if (m_nCntTitle < 60)
						{// ���t���[���܂�
							D3DXVECTOR3 pos = m_pUi[nCount]->Getpos();
							pos.x += ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)) / 60.0f;
							m_pUi[nCount]->Setpos(pos);

							D3DXVECTOR3 size = m_pUi[nCount]->Getsize();
							size.x += ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)) / 60.0f;
							m_pUi[nCount]->Setsize(size);

							D3DXVECTOR2 TexUV = m_pUi[nCount]->GetTexUV();
							TexUV.x += 1.0f / 60.0f;
							m_pUi[nCount]->SetTex(0, 0, TexUV, D3DXVECTOR2(0.0f, 0.0f));
						}
						else
						{// ���̑�
							D3DXVECTOR3 pos = m_pUi[nCount]->Getpos();
							pos.x += ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)) / 60.0f;
							m_pUi[nCount]->Setpos(pos);

							D3DXVECTOR3 size = m_pUi[nCount]->Getsize();
							size.x -= ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)) / 60.0f;
							m_pUi[nCount]->Setsize(size);

							D3DXVECTOR2 TexUV = m_pUi[nCount]->GetTexUV();
							TexUV.x -= 1.0f / 60.0f;
							m_pUi[nCount]->SetTex(0, 0, TexUV, D3DXVECTOR2(1.0f - TexUV.x, 0.0f));
						}

						m_nCntTitle++;		// �J�E���g

						if (m_nCntTitle == 240)
						{// ���t���[����������
							m_pUi[nCount]->Setpos(D3DXVECTOR3((SCREEN_WIDTH / 2) - ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)), (SCREEN_WIDTH / 6), 0.0f));
							m_pUi[nCount]->Setsize(D3DXVECTOR3(0.0f, (SCREEN_WIDTH / 12), 0.0f));
							m_pUi[nCount]->SetTex(0, 0, D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
							m_nCntTitle = 0;
						}
					}
					else
					{// �^�C�g����\��
						if (m_nCntTitle != 0)
						{
							m_pUi[nCount]->Setpos(D3DXVECTOR3((SCREEN_WIDTH / 2) - ((SCREEN_WIDTH / 4) + (SCREEN_WIDTH / 25)), (SCREEN_WIDTH / 6), 0.0f));
							m_pUi[nCount]->Setsize(D3DXVECTOR3(0.0f, (SCREEN_WIDTH / 12), 0.0f));
							m_pUi[nCount]->SetTex(0, 0, D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
							m_nCntTitle = 0;
						}
					}
				}
			}
			else if (nCount == TYPE_MENU_SELECT || nCount == TYPE_MENU_SELECT_MARK)
			{// �I���A�I���}�[�N�̂�
				float fMaxcol_a = 1.0f;

				if (nCount == TYPE_MENU_SELECT)
					fMaxcol_a = 0.7f;
				else
					fMaxcol_a = 1.0f;

				if (bSelectSet == true)
				{// �^�C�g���\��
					if (fcol_a != fMaxcol_a)
					{// 
						fcol_a = m_pUi[nCount]->Getcol().a + 0.03f;
						if (fMaxcol_a < fcol_a)	fcol_a = fMaxcol_a;
						bEdit = true;
					}
				}
				else
				{// �^�C�g����\��
					if (fcol_a != 0.0f)
					{
						fcol_a = m_pUi[nCount]->Getcol().a - 0.03f;
						if (fcol_a < 0.0f)	fcol_a = 0.0f;
						bEdit = true;
					}
				}

				m_pUi[nCount]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, fcol_a));	// �F�ύX

				if (bTitle == true || (bTitle == false && fcol_a != 0.0f))
				{// �^�C�g���\��
					D3DXVECTOR3 pos = m_pUi[nCount]->Getpos();
					D3DXVECTOR3 posSelect = m_pUi[(nSelectNum + TYPE_MENU_GAME)]->Getpos();
					m_pUi[nCount]->Setpos(D3DXVECTOR3(pos.x, posSelect.y, 0.0f));

					if (nCount == TYPE_MENU_SELECT_MARK)
						m_pUi[nCount]->Setrot(m_pUi[nCount]->Getrot() + 0.05f);
				}
			}
			else
			{// ���j���[�\��
				if (bSelectSet == true)
				{
					if (fcol_a != 1.0f)
					{
						fcol_a = m_pUi[nCount]->Getcol().a + 0.03f;
						if (1.0f < fcol_a)	fcol_a = 1.0f;
						bEdit = true;

						m_pUi[nCount]->Setpos(D3DXVECTOR3(SCREEN_WIDTH * 0.84f, SCREEN_HEIGHT * 0.32f - SCREEN_HEIGHT * 0.18f + (SCREEN_HEIGHT * 0.18f * (nCount - TYPE_MENU_GAME)), 0.0f));
					}
				}
				else
				{
					if (fcol_a != 0.0f)
					{
						fcol_a = m_pUi[nCount]->Getcol().a - 0.03f;
						if (fcol_a < 0.0f)	fcol_a = 0.0f;
						bEdit = true;
					}
					else
					{
						m_pUi[nCount]->Setpos(D3DXVECTOR3(500.0f + SCREEN_WIDTH * 0.84f, SCREEN_HEIGHT * 0.32f - SCREEN_HEIGHT * 0.18f + (SCREEN_HEIGHT * 0.18f * (nCount - TYPE_MENU_GAME)), 0.0f));
					}
				}

				if (nCount == (nSelectNum + TYPE_MENU_GAME))
				{// �I��ԍ��ƈ�v
					m_pUi[nCount]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, fcol_a));	// �F�ύX
				}
				else
				{// �I��ԍ��ȊO
					m_pUi[nCount]->Setcol(D3DXCOLOR(0.5f, 0.5f, 0.5f, fcol_a));	// �F�ύX
				}
			}
		}
	}

	if (m_pPress != NULL)
	{// NULL�ȊO�̏ꍇ
		D3DXCOLOR col = m_pPress->Getcol();

		if (bTitle == true)
		{// �^�C�g���\��
			if (bEdit == true)
			{
				if (col.a != 1.0f)
				{
					col.a += 0.03f;
					if (1.0f < col.a)	col.a = 1.0f;
					bEdit = true;
				}
			}
			else
			{
				if (m_nCntPress == 0)
				{
					m_pPress->SetcolType(90, D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
				}
				else if (m_nCntPress == 90)
				{
					m_pPress->SetcolType(90, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				}
				m_nCntPress++;

				if (m_nCntPress == 180)
				{
					m_nCntPress = 0;
				}
			}
		}
		else
		{// ���j���[�\��
			if (col.a != 0.0f)
			{
				col.a -= 0.03f;
				if (col.a < 0.0f)	col.a = 0.0f;
				bEdit = true;
			}

			m_nCntPress = 0;
		}

		m_pPress->Setcol(col);	// �F�ύX
	}

	// Ui�ҏW������
	if (bEdit == true)
		m_bUiEdit = true;	// ����
	else
		m_bUiEdit = false;	// ���Ă��Ȃ�
}

//=============================================================================
// �`�揈��											(public)	*** CUi_Title ***
//=============================================================================
void CUi_Title::Draw(void)
{

}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
int		CUi_Game::m_nEnemyName = 0;				// �G���O�ԍ�

//=============================================================================
// �R���X�g���N�^										(public)	*** CUi_Game ***
//=============================================================================
CUi_Game::CUi_Game() : CUi()
{
	for(int nCount = 0; nCount < TYPE_MAX; nCount++)
		m_pUi[nCount] = NULL;		// UI

	for (int nCount = 0; nCount < MESSAGE_MAX; nCount++)
		m_Message[nCount] = NULL;	// ���b�Z�[�W

	m_TutorialUV = D3DXVECTOR2(0.0f, 0.0f);			// �`���[�g���A��UV
	m_nCntTutorial = 0;				// �`���[�g���A���J�E���g

	for (int nCount = 0; nCount < GAUGE_MAX; nCount++)
		m_pGauge[nCount] = NULL;	// �Q�[�W

	m_nEnemyName = 0;
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CUi_Game ***
//=============================================================================
CUi_Game::~CUi_Game()
{

}

//=============================================================================
// ���[�h����										(public)	*** CUi_Game ***
//=============================================================================
HRESULT CUi_Game::Load(void)
{
	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CUi_Game ***
//=============================================================================
void CUi_Game::Unload(void)
{

}

//=============================================================================
// ����������										(public)	*** CUi_Game ***
//=============================================================================
HRESULT CUi_Game::Init(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// ��ރJ�E���g
		if (m_pUi[nCount] == NULL)
		{// NULL�̏ꍇ
			m_pUi[nCount] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);		// �������m��

			// �����l�ݒ�
			// �e�N�X�`�����f				
			if (nCount == TYPE_ENEMY_DATA)				// �G�f�[�^
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_WIDTH / 24), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_WIDTH / 24), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
			}
			else if (nCount == TYPE_PLAYER_DATA)		// �v���C���[�f�[�^
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 24), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_WIDTH / 24), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
			}
			else if (nCount == TYPE_MAP)				// �}�b�v
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 11) + 2.0f, (SCREEN_WIDTH / 11) - 2.0f, 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 12) + (SCREEN_WIDTH / 180) + 2.0f, (SCREEN_WIDTH / 12) + (SCREEN_WIDTH / 180) + 2.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_MAP));
			}
			else if (nCount == TYPE_LOG)				// ���O
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 10), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_WIDTH / 10), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
			}
			else if (nCount == TYPE_WEAPON)				// ����
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (SCREEN_WIDTH / 28), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 5), (SCREEN_WIDTH / 28), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_FRAME));
			}
			else if (nCount == TYPE_WEAPON_BUTTON)		// ����{�^��
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - (SCREEN_WIDTH / 28), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 5), (SCREEN_WIDTH / 26) - (SCREEN_WIDTH / 120), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_WEAPON));
			}
			else if (nCount == TYPE_PLAYER_ICON)		// �v���C���[�A�C�R��
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 26), SCREEN_HEIGHT - (SCREEN_WIDTH / 25), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 30), (SCREEN_WIDTH / 30), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 0.5f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CHAR));
			}
			else if (nCount == TYPE_PLAYER_NAME)		// �v���C���[��
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7) + (SCREEN_WIDTH / 28), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38 * 4) - (SCREEN_HEIGHT / 70), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 10), (SCREEN_WIDTH / 70), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_PLAYER_NAME));
			}
			else if (nCount == TYPE_PLAYER_HP)			// �v���C���[HP
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 11), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38 * 2) - (SCREEN_HEIGHT / 80), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 80), (SCREEN_WIDTH / 100), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_HPSP));
			}
			else if (nCount == TYPE_PLAYER_MP)			// �v���C���[MP
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 11), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 80), (SCREEN_WIDTH / 100), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_HPSP));
			}
			else if (nCount == TYPE_PLAYER_RANK)		// �v���C���[�����N
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7) + (SCREEN_WIDTH / 10) + (SCREEN_WIDTH / 50), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38 * 4) - (SCREEN_HEIGHT / 70), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 50), (SCREEN_WIDTH / 70), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.2f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_RANK));
			}
			else if (nCount == TYPE_ENEMY_ICON)			// �G�A�C�R��
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 26), (SCREEN_WIDTH / 24), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 30), (SCREEN_WIDTH / 30), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.5f, 0.5f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CHAR));
			}
			else if (nCount == TYPE_ENEMY_NAME)			// �G��
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7) + (SCREEN_WIDTH / 28), (SCREEN_HEIGHT / 38) + (SCREEN_HEIGHT / 50), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 10), (SCREEN_WIDTH / 50), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.333f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_ENEMY_NAME));
			}
			else if (nCount == TYPE_ENEMY_HP)			// �GHP
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 11), (SCREEN_HEIGHT / 9), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 70), (SCREEN_WIDTH / 90), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_HPSP));
			}

			// ����������
			m_pUi[nCount]->Init();

			if (nCount == TYPE_PLAYER_ICON)				// �v���C���[�A�C�R��
			{
				m_pUi[nCount]->SetTex(2, 1, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_ENEMY_ICON)			// �v���C���[�A�C�R��
			{
				m_pUi[nCount]->SetTex(2, 0, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_PLAYER_HP)			// �v���C���[MP
			{
				m_pUi[nCount]->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_PLAYER_MP)			// �v���C���[MP
			{
				m_pUi[nCount]->SetTex(1, 1, D3DXVECTOR2(1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_PLAYER_RANK)		// �v���C���[�����N
			{
				m_pUi[nCount]->SetTex(5, 0, D3DXVECTOR2(0.2f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_ENEMY_NAME)			// �G��
			{
				m_pUi[nCount]->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.333f), D3DXVECTOR2(0.0f, 0.0f));
			}
			else if (nCount == TYPE_ENEMY_HP)			// �GMP
			{
				m_pUi[nCount]->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));
			}

			// �`��ҏW
			if (nCount == TYPE_MAP)
				m_pUi[nCount]->DrawEdit(false, false, true, 50);
		}
	}

	for (int nCount = 0; nCount < MESSAGE_MAX; nCount++)
	{// ���b�Z�[�W�J�E���g
		if (m_Message[nCount] == NULL)
		{// NULL�̏ꍇ
			m_Message[nCount] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);		// �������m��

			// �����l�ݒ�
			m_Message[nCount]->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 40) - ((SCREEN_WIDTH / 40.0f * 2.0f) * nCount), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 8), (SCREEN_WIDTH / 40), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			// ����������
			m_Message[nCount]->Init();
			// �e�N�X�`�����f
			m_Message[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_EFFECT));
		}
	}

	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_GAME)
	{// ���[�h���Q�[����������
		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// NULL�ȊO�̏ꍇ
			if (pGame->GetGame() == CGame::GAME_TUTORIAL)
			{// �`���[�g���A���̂�
				m_Message[MESSAGE_00]->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 10), 0.0f));
				m_Message[MESSAGE_00]->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 8), (SCREEN_WIDTH / 10), 0.0f));
				m_Message[MESSAGE_00]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_Message[MESSAGE_00]->SetTexUV(D3DXVECTOR2(1.0f, 0.125f));
				m_Message[MESSAGE_00]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_TUTORIAL));

				m_Message[MESSAGE_01]->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 4), 0.0f));
				m_Message[MESSAGE_01]->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 10), (SCREEN_WIDTH / 18), 0.0f));
				m_Message[MESSAGE_01]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_Message[MESSAGE_01]->SetTexUV(D3DXVECTOR2(1.0f, 1.0f));
				m_Message[MESSAGE_01]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CHECK));

				m_Message[MESSAGE_02]->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 2), SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.9f), 0.0f));
				m_Message[MESSAGE_02]->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_HEIGHT / 14), 0.0f));
				m_Message[MESSAGE_02]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_Message[MESSAGE_02]->SetTexUV(D3DXVECTOR2(1.0f, 1.0f));
				m_Message[MESSAGE_02]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_TUTORIAL_LOGO));

				m_Message[MESSAGE_03]->Setpos(D3DXVECTOR3((SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 24 * 2) - (SCREEN_HEIGHT / 10), 0.0f));
				m_Message[MESSAGE_03]->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 7), (SCREEN_HEIGHT / 10), 0.0f));
				m_Message[MESSAGE_03]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_Message[MESSAGE_03]->SetTexUV(D3DXVECTOR2(1.0f, 1.0f));
				m_Message[MESSAGE_03]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_SKIP));
			}
		}
	}

	for (int nCount = 0; nCount < GAUGE_MAX; nCount++)
	{// �Q�[�W�J�E���g
		if (m_pGauge[nCount] == NULL)
		{// NULL�̏ꍇ
			if (nCount == GAUGE_PLAYER_HP)		// �v���C���[HP
				m_pGauge[nCount] = CGauge::Create(CGauge::TYPE_2D, D3DXVECTOR3((SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 35), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38 * 2) - (SCREEN_HEIGHT / 80), 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 12, SCREEN_HEIGHT / 60, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			if (nCount == GAUGE_PLAYER_MP)		// �v���C���[MP
				m_pGauge[nCount] = CGauge::Create(CGauge::TYPE_2D, D3DXVECTOR3((SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 35), SCREEN_HEIGHT - (SCREEN_HEIGHT / 38), 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 12, SCREEN_HEIGHT / 60, 0.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
			if (nCount == GAUGE_ENEMY_HP)		// �GHP
				m_pGauge[nCount] = CGauge::Create(CGauge::TYPE_2D, D3DXVECTOR3((SCREEN_WIDTH / 6) + (SCREEN_WIDTH / 35), (SCREEN_HEIGHT / 9), 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 12, SCREEN_HEIGHT / 50, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CUi_Game ***
//=============================================================================
void CUi_Game::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// ��ރJ�E���g
		if (m_pUi[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pUi[nCount]->Uninit();	// �I������
			m_pUi[nCount] = NULL;		// NULL��
		}
	}

	for (int nCount = 0; nCount < MESSAGE_MAX; nCount++)
	{// ���b�Z�[�W�J�E���g
		if (m_Message[nCount] != NULL)
		{// NULL�̏ꍇ
			m_Message[nCount]->Uninit();// �I������
			m_Message[nCount] = NULL;	// NULL��
		}
	}

	for (int nCount = 0; nCount < GAUGE_MAX; nCount++)
	{// �Q�[�W�J�E���g
		if (m_pGauge[nCount] != NULL)
		{// NULL�̏ꍇ
			m_pGauge[nCount]->Uninit();	// �I������
			m_pGauge[nCount] = NULL;	// NULL��
		}
	}

	CUi::Uninit();	// �I������
}

//=============================================================================
// �X�V����											(public)	*** CUi_Game ***
//=============================================================================
void CUi_Game::Update(void)
{
	CManager::MODE mode = CManager::GetMode();
	if (mode == CManager::MODE_GAME)
	{// ���[�h���Q�[����������
		if (m_pUi[TYPE_PLAYER_RANK] != NULL)		// �v���C���[�����N
		{// NULL�ȊO�̏ꍇ
			int nNum = 0;
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{// NULL�ȊO�̏ꍇ
				int nScore = pGame->GetScore();
				if (55000 < nScore) nNum = 4;
				else if (45000 < nScore) nNum = 3;
				else if (35000 < nScore) nNum = 2;
				else if (25000 < nScore) nNum = 1;
				else					nNum = 0;
			}

			m_pUi[TYPE_PLAYER_RANK]->SetTex(5, nNum, D3DXVECTOR2(0.2f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		}
		if (m_pUi[TYPE_ENEMY_ICON] != NULL)
		{// NULL�ȊO�̏ꍇ
			int nNum = 0;
			if (m_nEnemyName == 0)
				nNum = m_nEnemyName;
			else
				nNum = m_nEnemyName + 1;

			m_pUi[TYPE_ENEMY_ICON]->SetTex(2, nNum, D3DXVECTOR2(0.5f, 0.5f), D3DXVECTOR2(0.0f, 0.0f));

		}
		if (m_pUi[TYPE_ENEMY_NAME] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pUi[TYPE_ENEMY_NAME]->SetTex(1, m_nEnemyName, D3DXVECTOR2(1.0f, 0.333f), D3DXVECTOR2(0.0f, 0.0f));
		}

		CGame *pGame = CManager::GetGame();
		if (pGame != NULL)
		{// NULL�ȊO�̏ꍇ
			if (pGame->GetGame() == CGame::GAME_TUTORIAL)
			{// �`���[�g���A���̂�
				int nTutorialNum = pGame->GetTutprialNum();
				if (m_TutorialUV.y != (0.125f * (float)(nTutorialNum)))
				{
					m_TutorialUV.y += 0.02f;
					if ((0.125f * (float)(nTutorialNum)) < m_TutorialUV.y) m_TutorialUV.y = (0.125f * (float)(nTutorialNum));
				}
				m_Message[MESSAGE_00]->SetTex(1, 0, D3DXVECTOR2(1.0f, 0.125f), m_TutorialUV);

				if (nTutorialNum != CGame::TUTORIAL_ACTION)
				{// �A�N�V�����ȊO
					D3DXVECTOR3 pos = m_Message[MESSAGE_01]->Getpos();
					if (m_nCntTutorial < 90)
					{// ���t���[���܂�
						pos.y -= 0.3f;
					}
					else
					{// ���̑�
						pos.y += 0.3f;
					}
					m_Message[MESSAGE_01]->Setpos(pos);

					D3DXCOLOR col = m_Message[MESSAGE_02]->Getcol();
					if (m_nCntTutorial < 90)
					{// ���t���[���܂�
						col.a -= (1.0f / 90.0f);
						if (col.a < 0.0f) col.a = 0.0f;
					}
					else
					{// ���̑�
						col.a += (1.0f / 90.0f);
						if (1.0f < col.a) col.a = 1.0f;
					}
					m_Message[MESSAGE_02]->Setcol(col);

					m_nCntTutorial = (m_nCntTutorial + 1) % 180;
				}
				else
				{// ���̑�
					m_Message[MESSAGE_01]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					m_Message[MESSAGE_02]->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��											(public)	*** CUi_Game ***
//=============================================================================
void CUi_Game::Draw(void)
{

}

//=============================================================================
// �Q�[�W�ݒ菈��									(public)	*** CUi_Game ***
//=============================================================================
void CUi_Game::SetGauge(GAUGE gauge, int nNow, int nMax)
{
	if (m_pGauge[gauge] != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pGauge[gauge]->SetBar(D3DXVECTOR3(0.0f, 0.0f, 0.0f), nNow, nMax);
	}
}

//-----------------------------------------------------------------------------

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CUi_Result::m_pTexture[SS_MAX] = { NULL };		// �e�N�X�`��

//=============================================================================
// �R���X�g���N�^										(public)	*** CUi_Result ***
//=============================================================================
CUi_Result::CUi_Result() : CUi()
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
		m_pUi[nCount] = NULL;		// UI

	m_nCntTimer = 0;				// ���ԃJ�E���g
	m_nCntLogo = 0;					// ���S�J�E���g

	for (int nCount = 0; nCount < 6; nCount++)
		m_pNumber[nCount];	// �ԍ�
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CUi_Result ***
//=============================================================================
CUi_Result::~CUi_Result()
{

}

//=============================================================================
// ���[�h����										(public)	*** CUi_Result ***
//=============================================================================
HRESULT CUi_Result::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	char cName[64] = {};

	/*strcpy(cName, "data\\TEXTURE\\ScreenShot\\result00.bmp");
	D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[0]);	// �e�N�X�`���ǂݍ���
	strcpy(cName, "data\\TEXTURE\\ScreenShot\\result01.bmp");
	D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[1]);	// �e�N�X�`���ǂݍ���
	strcpy(cName, "data\\TEXTURE\\ScreenShot\\result02.bmp");
	D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[2]);	// �e�N�X�`���ǂݍ���*/

	strcpy(cName, "data\\TEXTURE\\ScreenShot\\result01.bmp");
	D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[0]);	// �e�N�X�`���ǂݍ���
	strcpy(cName, "data\\TEXTURE\\ScreenShot\\result02.bmp");
	D3DXCreateTextureFromFile(pDevice, cName, &m_pTexture[1]);	// �e�N�X�`���ǂݍ���

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CUi_Result ***
//=============================================================================
void CUi_Result::Unload(void)
{
	for (int nCount = 0; nCount < SS_MAX; nCount++)
	{// SS�J�E���g
		if (m_pTexture[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
// ����������										(public)	*** CUi_Result ***
//=============================================================================
HRESULT CUi_Result::Init(void)
{
	CGame *pGame = CManager::GetGame();

	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// ��ރJ�E���g
		if (m_pUi[nCount] == NULL)
		{// NULL�̏ꍇ
			m_pUi[nCount] = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);		// �������m��

			// �����l�ݒ�
			// �e�N�X�`�����f
			if (nCount == TYPE_LOGO_BG)			// ���Sbg
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 4) - (SCREEN_WIDTH / 4), (SCREEN_HEIGHT / 10), 0.0f), D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 10), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

				if (pGame != NULL)
				{
					if (pGame->GetGame() == CGame::GAME_CLEAR)
					{// �N���A
						m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_RESULT_CLEAR_BG));
					}
					else if (pGame->GetGame() == CGame::GAME_OVER)
					{// �I�[�o�[
						m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_RESULT_OVER_BG));
					}
				}				
			}
			else if (nCount == TYPE_LOGO)		// ���S
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 4), (SCREEN_HEIGHT / 10), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 5), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				
				if (pGame != NULL)
				{// NULL�ȊO�̏ꍇ
					if (pGame->GetGame() == CGame::GAME_CLEAR)
					{// �N���A
						m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_RESULT_CLEAR));
					}
					else if (pGame->GetGame() == CGame::GAME_OVER)
					{// �I�[�o�[
						m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_RESULT_OVER));
					}
				}
			}
			else if (nCount == TYPE_PLAYER_NAME)		// ���S
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH) - (SCREEN_WIDTH / 4), (SCREEN_HEIGHT / 6), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 5), (SCREEN_HEIGHT / 15), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_PLAYER_NAME));
			}
			else if (nCount == TYPE_PLAYER_RANK)		// ���S
			{
				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH) - (SCREEN_WIDTH / 12), (SCREEN_HEIGHT / 6), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 30), (SCREEN_WIDTH / 30), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.2f, 1.0f));
				m_pUi[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_RANK));
			}
			else
			{
				//m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 6) + ((SCREEN_WIDTH / 9) * (float)(nCount - TYPE_PLAY_SS00)), (SCREEN_HEIGHT / 3) + ((SCREEN_HEIGHT / 4 + (SCREEN_HEIGHT / 80)) * (float)(nCount - TYPE_PLAY_SS00)), 0.0f),
				//	D3DXVECTOR3((SCREEN_WIDTH / 8), (SCREEN_HEIGHT / 8), 0.0f),	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				//m_pUi[nCount]->BindTexture(m_pTexture[(nCount - TYPE_PLAY_SS00)]);

				m_pUi[nCount]->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 5) + ((SCREEN_WIDTH / 8 * 2 - 80.0f) * (float)(nCount - TYPE_PLAY_SS00)), (SCREEN_HEIGHT / 2) - (SCREEN_HEIGHT / 10) + ((SCREEN_HEIGHT / 8 * 2 - 20.0f) * (float)(nCount - TYPE_PLAY_SS00)), 0.0f),
					D3DXVECTOR3((SCREEN_WIDTH / 6), (SCREEN_HEIGHT / 6), 0.0f),	D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
				m_pUi[nCount]->BindTexture(m_pTexture[(nCount - TYPE_PLAY_SS00)]);
			}

			// ����������
			m_pUi[nCount]->Init();
			// �`��ҏW

			if (nCount == TYPE_PLAYER_RANK)		// ���S
			{
				int nNum = 0;
				if (pGame != NULL)
				{// NULL�ȊO�̏ꍇ
					int nScore = pGame->GetScore();

					// �N���A
					nScore += 15000;

					// �{�X���j
					nScore += 1000;

					if (55000 < nScore) nNum = 4;
					else if (45000 < nScore) nNum = 3;
					else if (35000 < nScore) nNum = 2;
					else if (25000 < nScore) nNum = 1;
					else					nNum = 0;
				}

				m_pUi[nCount]->SetTex(5, nNum, D3DXVECTOR2(0.2f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			}
		}
	}

	float fScoreData = 0.0f;
	if (pGame != NULL)
	{// NULL�ȊO�̏ꍇ
		fScoreData = (float)pGame->GetScore();

		// �N���A
		fScoreData += 15000.0f;

		// �{�X���j
		fScoreData += 1000.0f;
	}

	for (int nCount = 0; nCount < 6; nCount++)
	{// �J�E���g
		if (m_pNumber[nCount] == NULL)
		{// NULL�̏ꍇ
			m_pNumber[nCount] = new CNumber2D;
			m_pNumber[nCount]->Init(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 9, 0.0f) + 
				D3DXVECTOR3((-((40.0f * 2.0f) + 10.0f) * nCount) - 40.0f, 0.0f, 0.0f), D3DXVECTOR3(40.0f, 60.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			m_pNumber[nCount]->SetTex(10, (int)fScoreData % 10, D3DXVECTOR2(0.1f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));					// �e�N�X�`���ݒ�
		}

		fScoreData /= 10;		// ����i�߂�
	}

	CRanking::Create(CRanking::TYPE_2D, D3DXVECTOR3(SCREEN_WIDTH - 80.0f, 180.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(30.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CUi_Result ***
//=============================================================================
void CUi_Result::Uninit(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// ��ރJ�E���g
		if (m_pUi[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pUi[nCount]->Uninit();	// �I������
			m_pUi[nCount] = NULL;		// NULL��
		}
	}

	for (int nCount = 0; nCount < 6; nCount++)
	{// �J�E���g
		if (m_pNumber[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pNumber[nCount]->Uninit();// �I������
			m_pNumber[nCount] = NULL;	// NULL��
		}
	}

	CUi::Uninit();	// �I������
}

//=============================================================================
// �X�V����											(public)	*** CUi_Result ***
//=============================================================================
void CUi_Result::Update(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// ��ރJ�E���g
		if (m_pUi[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			if (nCount == TYPE_LOGO_BG)			// ���S
			{
				if (8 < m_nCntTimer)
				{
					if (m_nCntLogo < 60)
					{// ���t���[���܂�
						D3DXVECTOR3 pos = m_pUi[nCount]->Getpos();
						pos.x += (SCREEN_WIDTH / 4) / 60.0f;
						m_pUi[nCount]->Setpos(pos);

						D3DXVECTOR3 size = m_pUi[nCount]->Getsize();
						size.x += (SCREEN_WIDTH / 4) / 60.0f;
						m_pUi[nCount]->Setsize(size);

						D3DXVECTOR2 TexUV = m_pUi[nCount]->GetTexUV();
						TexUV.x += 1.0f / 60.0f;
						m_pUi[nCount]->SetTex(0, 0, TexUV, D3DXVECTOR2(0.0f, 0.0f));
					}
					else
					{// ���̑�
						D3DXVECTOR3 pos = m_pUi[nCount]->Getpos();
						pos.x += (SCREEN_WIDTH / 4) / 60.0f;
						m_pUi[nCount]->Setpos(pos);

						D3DXVECTOR3 size = m_pUi[nCount]->Getsize();
						size.x -= (SCREEN_WIDTH / 4) / 60.0f;
						m_pUi[nCount]->Setsize(size);

						D3DXVECTOR2 TexUV = m_pUi[nCount]->GetTexUV();
						TexUV.x -= 1.0f / 60.0f;
						m_pUi[nCount]->SetTex(0, 0, TexUV, D3DXVECTOR2(1.0f - TexUV.x, 0.0f));
					}

					m_nCntLogo++;

					if (m_nCntLogo == 240)
					{// ���t���[����������
						m_pUi[nCount]->Setpos(D3DXVECTOR3((SCREEN_WIDTH / 4) - (SCREEN_WIDTH / 4), (SCREEN_HEIGHT / 10), 0.0f));
						m_pUi[nCount]->Setsize(D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 10), 0.0f));
						m_pUi[nCount]->SetTex(0, 0, D3DXVECTOR2(0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
						m_nCntLogo = 0;
					}
				}
			}
			else if (nCount == TYPE_LOGO)		// ���S
			{
				if (m_nCntTimer < 8)
				{
					D3DXVECTOR3 size = m_pUi[nCount]->Getsize();
					size.x += ((SCREEN_WIDTH / 4) - (SCREEN_WIDTH / 2)) / 8;
					size.y += ((SCREEN_HEIGHT / 10) - (SCREEN_HEIGHT / 5)) / 8;
					m_pUi[nCount]->Setsize(size);
				}				
			}
		}
	}

	m_nCntTimer++;
}

//=============================================================================
// �`�揈��											(public)	*** CUi_Result ***
//=============================================================================
void CUi_Result::Draw(void)
{

}