//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "title.h"			// �^�C�g��
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "sound.h"			// �T�E���h
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "fade.h"			// �t�F�[�h
#include "window.h"			// �E�B���h�E
#include "text.h"			// �e�L�X�g
#include "texture.h"		// �e�N�X�`��
#include "effect.h"			// �G�t�F�N�g
#include "ui.h"				// UI
#include "scene2d.h"		// �V�[���QD
#include "map.h"			// �}�b�v
#include "ranking.h"		// �����L���O
#include "player.h"			// �v���C���[

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
// �R���X�g���N�^										(public)	*** CTitle ***
//=============================================================================
CTitle::CTitle()
{
	// �����l�ݒ�
	m_pPlayer		= NULL;			// �v���C���[

	m_bTitle		= false;		// �^�C�g��

	m_nSelectNum	= 0;			// �I��ԍ�
	m_nCntTimer		= 0;			// ���ԃJ�E���^

	m_bTutorial		= false;		// �`���[�g���A����
	m_bSelectSet	= false;		// �I��ݒ肵�Ă��邩

	m_pWindow		= NULL;			// �E�B���h�E
	m_pIcon			= NULL;			// �A�C�R��
	m_pReturnCircle = NULL;			// �߂�T�[�N��

	for (int nCount = 0; nCount < m_MAX_WALL; nCount++)
		m_pWall[nCount] = NULL;		// ��

	m_pUi_Game		= NULL;			// UI

	m_bRanking		= false;		// �����L���O
	m_pRanking		= NULL;			// �����L���O
	m_pRankingBG	= NULL;			// �����L���OBG
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CTitle ***
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ���[�h����										(public)	*** CTitle ***
//=============================================================================
HRESULT CTitle::Load(void)
{
	// ���[�h
	CUi_Title::Load();			// UI�^�C�g��
	CMap_01::Load();			// �}�b�v
	CEffect::Load();			// ���[�h
	CPlayer::Load();			// �v���C���[
	CRanking::LoadRanking();	// �����L���O���[�h

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CTitle ***
//=============================================================================
void CTitle::Unload(void)
{
	// �A�����[�h
	CUi_Title::Unload();		// Ui�^�C�g��
	CMap_01::Unload();			// �}�b�v
	CEffect::Unload();			// �G�t�F�N�g
	CPlayer::Unload();			// �v���C���[

	CTexture::ReleaseAll();		// �e�N�X�`���j��
	CText::ReleaseXModelData();	// ���f���f�[�^�j��
}

//=============================================================================
// ����������										(public)	*** CTitle ***
//=============================================================================
HRESULT CTitle::Init(void)
{
	CSound *pSound = CManager::GetSound();
	if (pSound != NULL)
	{// NULL�ȊO�̏ꍇ
		pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME000);
		pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME001);
		pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME002);
		pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE000);
	}

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// �f�o�C�X�擾
	CCamera *pCamera = CManager::GetCamera(CManager::CAMERA_MAIN);				// �J�����擾
	pCamera->SetPos(D3DXVECTOR3(120.0f, 80.0f, -150.0f), D3DXVECTOR3(80.0f, 30.0f, 0.0f), D3DXVECTOR3(-0.1f, D3DX_PI, 0.0f));		// �J�����ʒu�ݒ�
	pCamera->SetInitPos(D3DXVECTOR3(120.0f, 80.0f, -150.0f), D3DXVECTOR3(80.0f, 30.0f, 0.0f));	// �J���������ʒu�ݒ�
	CCameraMain *pCameraMain = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	pCameraMain->SetMove(false);

	// �}�b�v�J����
	pCamera = CManager::GetCamera(CManager::CAMERA_GAME_MAP);	// �J�����擾
	pCamera->SetPos(D3DXVECTOR3(400.0f, 1500.0f, -1.0f), D3DXVECTOR3(400.0f, 0.0f, 0.0f));		// �J�����ʒu�ݒ�
	pCamera->SetInitPos(D3DXVECTOR3(400.0f, 1500.0f, -1.0f), D3DXVECTOR3(400.0f, 0.0f, 0.0f));	// �J���������ʒu�ݒ�

	// �N���G�C�g
	// UI����
	CUi::Create(CUi::UI_TITLE);

	// �}�b�v
	CMap::Create(CMap::TYPE_NO_01);

	// �v���C���[
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));

	m_bTitle		= true;		// �^�C�g��
	m_bSelectSet	= false;	// �I��ݒ�

	//�t�H�O�̐ݒ�
	/*FLOAT StartPos	= 100;		// �J�n�ʒu
	FLOAT EndPos	= 1000;		// �I���ʒu
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);								// �t�H�O�FON
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)); // ���F�ŕs����
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);					// ���_���[�h
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);					// �e�[�u�����[�h
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos));				// �J�n�ʒu
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));					// �I���ʒu*/
	
	if (m_pUi_Game == NULL)
	{// NULL�̏ꍇ
		m_pUi_Game = CUi::Create(CUi::UI_GAME);
		m_pUi_Game->Uninit();
		m_pUi_Game = NULL;
	}

	CManager::GetRenderer()->SetDrawMap(false);

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CTitle ***
//=============================================================================
void CTitle::Uninit(void)
{
	CSound *pSound = CManager::GetSound();
	if (pSound != NULL)
	{// NULL�ȊO�̏ꍇ
		pSound->StopSound(CSound::SOUND_LABEL_BGM_TITLE000);
	}

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// �f�o�C�X�擾

	// �v���C���[
	if (m_pPlayer != NULL)
	{
		m_pPlayer->Uninit();	// �I������
		m_pPlayer = NULL;		// NULL��
	}

	// �E�B���h�E
	if (m_pWindow != NULL)
	{
		m_pWindow->Uninit();	// �I������
		m_pWindow = NULL;		// NULL��
	}

	// �A�C�R��
	if (m_pIcon != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pIcon->Uninit();		// �I������
		m_pIcon = NULL;			// NULL��
	}
	
	// �߂�T�[�N��
	if (m_pReturnCircle != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pReturnCircle->Uninit();		// �I������
		m_pReturnCircle = NULL;			// NULL��
	}

	// ��
	for (int nCount = 0; nCount < m_MAX_WALL; nCount++)
	{// �ǃJ�E���g
		if (m_pWall[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pWall[nCount]->Uninit();		// �I������
			m_pWall[nCount] = NULL;			// NULL��
		}
	}

	if (m_pUi_Game != NULL)
	{
		m_pUi_Game->Uninit();	// �I������
		m_pUi_Game = NULL;		// NULL��
	}

	if (m_pRanking != NULL)
	{
		m_pRanking->Uninit();	// �I������
		m_pRanking = NULL;	 	// NULL��
	}

	if (m_pRankingBG != NULL)
	{
		m_pRankingBG->Uninit();	// �I������
		m_pRankingBG = NULL;   	// NULL��
	}

	CScene::ReleaseMode();		// ���[�h�j��

	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);							// �t�H�O�FOFF
}

//=============================================================================
// �X�V����											(public)	*** CTitle ***
//=============================================================================
void CTitle::Update(void)
{
	CSound *pSound = CManager::GetSound();	

	CInputKeyboard	*pKey		= CManager::GetInputKeyboard();		// �L�[�{�[�h�擾
	CInputXPad		*pXPad		= CManager::GetInputXPad();			// X�p�b�h�擾
	CCameraMain		*pCamera	= (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);		// �J�����擾�i���C���j
	CFade::FADE		fade		= CFade::GetFade();					// �t�F�[�h�擾
	bool			bUiEdit		= CUi_Title::GetUiEdit();			// Ui�ҏW����
	
	if (m_bTitle == true)
	{// �^�C�g���\��
		if (bUiEdit == false)
		{// Ui�ҏW���łȂ�
			if (fade == CFade::FADE_NONE)
			{// �t�F�[�h���Ă��Ȃ�
				if (pKey->GetALL(1) == true || pXPad->GetALL(1, 0) == true)
				{// �Ȃɂ���������
					m_bTitle = false;
					m_bSelectSet = true;

					if (pSound != NULL)
					{// NULL�ȊO�̏ꍇ
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
					}
				}
			}
			m_nSelectNum = 0;
		}
	}
	else
	{// ���j���[�\��
		if (m_bSelectSet == true)
		{// �I��ݒ�
			if (fade == CFade::FADE_NONE)
			{// �t�F�[�h���Ă��Ȃ�
				if (pKey->GetTrigger(DIK_W) == true || pKey->GetTrigger(DIK_UP) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_UP, 0) == true)
				{// �L�[�{�[�h�iW�j�������ꂽ�AX�p�b�h�iL�X�e�B�b�N��j�������ꂽ
					m_nSelectNum = (m_nSelectNum + SELECT_MAX - 1) % SELECT_MAX;

					if (pSound != NULL)
					{// NULL�ȊO�̏ꍇ
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SERECT000);
					}
				}
				else if (pKey->GetTrigger(DIK_S) == true || pKey->GetTrigger(DIK_DOWN) == true || pXPad->GetTrigger(CInputXPad::XPADOTHER_STICK_L_DOWN, 0) == true)
				{// �L�[�{�[�h�iS�j�������ꂽ�AX�p�b�h�iL�X�e�B�b�N���j�������ꂽ
					m_nSelectNum = (m_nSelectNum + 1) % SELECT_MAX;

					if (pSound != NULL)
					{// NULL�ȊO�̏ꍇ
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_SERECT000);
					}
				}
			}
		}

		D3DXVECTOR3 posPlayer;
		D3DXVECTOR3 posCenter;
		D3DXVECTOR3 sizeCenter;
		float fRange;

		if (m_pPlayer != NULL && m_pReturnCircle != NULL)
		{
			posPlayer = m_pPlayer->Getpos();
			posCenter = m_pReturnCircle->Getpos();
			sizeCenter = m_pReturnCircle->Getsize();
			fRange = sqrtf(powf(posPlayer.x - posCenter.x, 2.0f) + powf(posPlayer.z - posCenter.z, 2.0f));

			// �A�C�R��
			if (m_pIcon != NULL)
			{// NULL�ȊO�̏ꍇ
				D3DXCOLOR col = m_pIcon->Getcol();
				if (fRange < sizeCenter.x * 0.8f)
				{// �͈͓�
					if (col.a != 1.0f)
					{// �s�����ȊO�ȊO
						col.a += 0.05f;
						if (1.0f < col.a) col.a = 1.0f;
					}
				}
				else
				{// �͈͊O
					if (col.a != 0.0f)
					{// �����ȊO
						col.a -= 0.05f;
						if (col.a < 0.0f) col.a = 0.0f;
					}
				}
				m_pIcon->Setcol(col);
			}

			// �߂�T�[�N��
			if (m_pReturnCircle != NULL)
			{// NULL�ȊO�̏ꍇ
				D3DXVECTOR3 size = m_pReturnCircle->Getsize();
				if (size.y != 100.0f)
				{
					size.y += 8.0f;
					if (100.0f < size.y) size.y = 100.0f;
					m_pReturnCircle->SetStart(m_pReturnCircle->Getpos(), m_pReturnCircle->Getrot(), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
				}
			}
		}

		// ��
		for (int nCount = 0; nCount < m_MAX_WALL; nCount++)
		{// �ǃJ�E���g
			if (m_pWall[nCount] != NULL)
			{// NULL�ȊO�̏ꍇ
				D3DXVECTOR3 size = m_pWall[nCount]->Getsize();
				if (size.y != 100.0f)
				{
					size.y += 8.0f;
					if (100.0f < size.y) size.y = 100.0f;
					m_pWall[nCount]->SetStart(m_pWall[nCount]->Getpos(), m_pWall[nCount]->Getrot(), size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
				}
			}
		}

		if (fade == CFade::FADE_NONE)
		{// �t�F�[�h���Ă��Ȃ�
			if (bUiEdit == false)
			{// Ui�ҏW���łȂ�
				if (pKey->GetTrigger(DIK_RETURN) == true || pKey->GetTrigger(DIK_E) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
				{// �L�[�{�[�h�iENTER�j�������ꂽ�AX�p�b�h�iB�A4�j�������ꂽ
					if (pSound != NULL)
					{// MILLI
						if (m_nSelectNum != SELECT_TUTORIAL)
						{// �`���[�g���A���ȊO
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
						}
					}

					switch (m_nSelectNum)
					{// �I��ԍ�
					case SELECT_GAME:		// �Q�[��
						CFade::Create(CManager::MODE_GAME);		// �t�F�[�h�����i�Q�[�����[�h�ցj

						break;
					case SELECT_TUTORIAL:	// �`���[�g���A��
					{
						bool bTutorial = m_bTutorial ? false : true;

						if (bTutorial == true)
						{// �`���[�g���A����
							if (pSound != NULL)
							{// NULL�ȊO�̏ꍇ
								pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
							}

							m_bTutorial = m_bTutorial ? false : true;
							m_bSelectSet = m_bSelectSet ? false : true;

							m_pPlayer->SetMove(true);
							pCamera->SetMove(true);
							pCamera->Setrot(D3DXVECTOR3(-0.3f, -D3DX_PI * 0.5f, 0.0f));
							pCamera->SetDistance(0, 230.0f);
							pCamera->SetDistance(1, 230.0f);

							if (m_pIcon == NULL)
							{// NULL�̏ꍇ
								m_pIcon = new CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_MAPICON);
								m_pIcon->SetInitAll(D3DXVECTOR3(180.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(0.125f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
								m_pIcon->Init();
								m_pIcon->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
								m_pIcon->DrawEdit(false, false, false, true, true, 0, D3DCULL_NONE);
								m_pIcon->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
							}
							if (m_pReturnCircle == NULL)
							{// NULL�̏ꍇ
								m_pReturnCircle = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_WARP);
								m_pReturnCircle->SetInitAll(CSceneMesh::MESHTYPE_CYLINDER, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(10.0f, 1.0f, 0.0f), D3DXVECTOR3(180.0f, 0.0f, 0.0f),
									D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
								m_pReturnCircle->Init();
								m_pReturnCircle->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
								m_pReturnCircle->DrawEdit(true, false, false, true, true, 0, D3DCULL_NONE);
							}
							/*for (int nCount = 0; nCount < m_MAX_WALL; nCount++)
							{// �ǃJ�E���g
								if (m_pWall[nCount] == NULL)
								{// NULL�̏ꍇ
									D3DXVECTOR3 pos, rot, size;
									if (nCount == 0)
									{// �O��������
										pos = D3DXVECTOR3(160.0f - 10.0f, 0.0f, 0.0f);
										rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
										size = D3DXVECTOR3(180.0f, 0.0f, 0.0f);
									}
									else if (nCount == 1)
									{// �P��������
										pos = D3DXVECTOR3((690.0f - 160.0f) / 2.0f + 160.0f + 5.0f, 0.0f, -160.0f - 20.0f);
										rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										size = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
									}
									else if (nCount == 2)
									{// �Q��������
										pos = D3DXVECTOR3(690.0f + 20.0f, 0.0f, 0.0f);
										rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);
										size = D3DXVECTOR3(180.0f, 0.0f, 0.0f);
									}
									else if (nCount == 3)
									{// �R��������
										pos = D3DXVECTOR3((690.0f - 160.0f) / 2.0f + 160.0f + 5.0f, 0.0f, 160.0f + 20.0f);
										rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
										size = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
									}

									m_pWall[nCount] = new CSceneMesh(CScene::PRIORITY_4, CScene::OBJTYPE_WARP);
									m_pWall[nCount]->SetInitAll(CSceneMesh::MESHTYPE_WALL, CSceneMesh::MESHTEX_ALL, D3DXVECTOR3(1.0f, 1.0f, 0.0f), pos,
										rot, size, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), D3DXVECTOR2(1.0f, 1.0f));
									m_pWall[nCount]->Init();
									m_pWall[nCount]->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_GOAL));
									m_pWall[nCount]->DrawEdit(true, false, false, true, true, 0, D3DCULL_NONE);
								}
							}*/

							if (m_pUi_Game == NULL)
							{// NULL�̏ꍇ
								m_pUi_Game = CUi::Create(CUi::UI_GAME);
							}

							CManager::GetRenderer()->SetDrawMap(true);
						}
						else
						{// ���j���[�\��
							if (m_pPlayer != NULL && m_pReturnCircle != NULL)
							{// NULL�ȊO�̏ꍇ
								if (fRange < sizeCenter.x * 0.8f)
								{// �͈͓�
									if (pSound != NULL)
									{// NULL�ȊO�̏ꍇ
										pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
									}

									if (m_pWindow == NULL)
									{// NULL�̏ꍇ
										m_pWindow = CWindow::Create(CWindow::WINDOW_YESNO);

										m_pPlayer->SetMove(false);
										pCamera->SetMove(false);
									}
									else
									{// NULL�ȊO�̏ꍇ
										CWindow_YESNO *pWindow_YESNO = (CWindow_YESNO*)m_pWindow;
										if (pWindow_YESNO->GetSelectNum() == 0)
										{// �G���Ȃ��Ȃ�����
											m_bTutorial = m_bTutorial ? false : true;
											m_bSelectSet = m_bSelectSet ? false : true;

											m_pPlayer->SetMove(false);
											pCamera->SetMove(false);
											pCamera->SetPosDest(pCamera->GetInitPos(0), pCamera->GetInitPos(1));
											pCamera->Setrot(D3DXVECTOR3(-0.1f, D3DX_PI, 0.0f));
											pCamera->SetDistance(0, 150.0f);
											pCamera->SetDistance(1, 150.0f);

											// �A�C�R��
											if (m_pIcon != NULL)
											{// NULL�ȊO�̏ꍇ
												m_pIcon->Uninit();		// �I������
												m_pIcon = NULL;			// NULL��
											}

											// �߂�T�[�N��
											if (m_pReturnCircle != NULL)
											{// NULL�ȊO�̏ꍇ
												m_pReturnCircle->Uninit();	// �I������
												m_pReturnCircle = NULL;	  	// NULL��
											}

											// ��
											for (int nCount = 0; nCount < m_MAX_WALL; nCount++)
											{// �ǃJ�E���g
												if (m_pWall[nCount] != NULL)
												{// NULL�ȊO�̏ꍇ
													m_pWall[nCount]->Uninit();	// �I������
													m_pWall[nCount] = NULL;	  	// NULL��
												}
											}

											if (m_pUi_Game != NULL)
											{// NULL�ȊO�̏ꍇ
												m_pUi_Game->Uninit();	// �I������
												m_pUi_Game = NULL;	 	// NULL��
											}

											CManager::GetRenderer()->SetDrawMap(false);
										}
										else
										{// �G������
											m_pPlayer->SetMove(true);	// �ړ��ł���
											pCamera->SetMove(true);		// �J������������
										}
										m_pWindow->Uninit();	// �I������
										m_pWindow = NULL;		// NULL��
									}
								}
							}
						}						
					}
						break;
					case SELECT_EDIT:		// �G�f�B�b�g
						m_bRanking = m_bRanking ? false : true;
						m_bSelectSet = m_bSelectSet ? false : true;

						if (m_bRanking == true)
						{// �����L���O
							if (m_pRankingBG == NULL)
							{// NULL�̏ꍇ
								m_pRankingBG = new CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
								m_pRankingBG->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH * 0.75f, SCREEN_HEIGHT * 0.6f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH * 0.2f, SCREEN_HEIGHT * 0.4f, 0.0f), 
									D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f), D3DXVECTOR2(1.0f, 1.0f));
								m_pRankingBG->Init();
							}
							if (m_pRanking == NULL)
							{// NULL�̏ꍇ
								m_pRanking = CRanking::Create(CRanking::TYPE_2D, D3DXVECTOR3(SCREEN_WIDTH - 80.0f, 180.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
									D3DXVECTOR3(30.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f));
							}
						}
						else
						{
							if (m_pRankingBG != NULL)
							{// NULL�ȊO�̏ꍇ
								m_pRankingBG->Uninit();		// �I������
								m_pRankingBG = NULL;		// NULL��
							}
							if (m_pRanking != NULL)
							{// NULL�ȊO�̏ꍇ
								m_pRanking->Uninit();		// �I������
								m_pRanking = NULL;			// NULL��
							}
						}

						break;
					case SELECT_TITLE:		// �^�C�g��
						m_bTitle = true;		// �^�C�g��
						m_bSelectSet = false;	// �I��ݒ�

						break;
					}
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��											(public)	*** CTitle ***
//=============================================================================
void CTitle::Draw(void)
{
	
}