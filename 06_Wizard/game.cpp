//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "game.h"			// �Q�[��
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "sound.h"			// �T�E���h
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "fade.h"			// �t�F�[�h
#include "text.h"			// �e�L�X�g
#include "texture.h"		// �e�N�X�`��
#include "effect.h"			// �G�t�F�N�g
#include "ranking.h"		// �����L���O

#include "pause.h"			// �|�[�Y
#include "ui.h"				// UI
#include "map.h"			// �}�b�v
#include "player.h"			// �v���C���[
#include "enemy.h"			// �G

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
// �R���X�g���N�^										(public)	*** CGame ***
//=============================================================================
CGame::CGame()
{
	// �����l�ݒ�
	m_game					= GAME_TUTORIAL;					// �Q�[��
	m_pPause				= NULL;								// �|�[�Y
	m_bPause				= false;							// �|�[�Y���Ă邩

	m_pMap					= NULL;								// �}�b�v
	m_pUi					= NULL;								// UI
	m_pPlayer				= NULL;								// �v���C���[

	m_nMapNum				= 0;								// �}�b�v�ԍ�
	posOld					= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �O��ʒu

	m_nSetEnemyNum			= 0;								// �G����

	m_nCntTimer				= 0;								// ���ԃJ�E���g

	m_bSetSS				= false;							// �X�N���[���V���b�g���B��
	m_bSetSS_Death			= false;							// �X�N���[���V���b�g���B�� 
	m_bPlay_SS00			= false;							// �X�N���[���V���b�g00
	m_bPlay_SS01			= false;							// �X�N���[���V���b�g01
	m_bPlay_SS02			= false;							// �X�N���[���V���b�g02

	m_nTutorialNum			= 0;								// �`���[�g���A���ԍ�
	m_nCntTutorial			= 0;								// �`���[�g���A���J�E���g

	for (int nCount = 0; nCount < TUTORIAL_MAX; nCount++)
		m_bTutorial[nCount] = false;							// �`���[�g���A��

	m_pClear				= NULL;								// �N���A

	m_nCntCheck				= 0;								// �`�F�b�N�J�E���g
	m_pCheck				= NULL;								// �`�F�b�N

	m_nCntSkip				= 0;								// �X�L�b�v�J�E���g
	m_pSkip					= NULL;								// �X�L�b�v

	m_nPlayerHP				= CPlayer::m_MAX_HP;				// �v���C���[HP
	m_nPlayerMP				= CPlayer::m_MAX_MP;				// �v���C���[MP

	m_nTimer				= 0;								// �^�C�}�[
	m_nDeathEnemy			= 0;								// �|�����G����
	m_bNoDamage				= true;								// �m�[�_���[�W
	m_nJastGuard			= 0;								// �W���X�g�K�[�h����
	m_bAllDeathEnemy		= true;								// �S�Ă̓G��|����

	m_nCntTutorialPlus		= 0;								// �`���[�g���A���v���X�J�E���g
	m_pTutorialPlus			= NULL;								// �`���[�g���A���v���X
	m_pTutorialPlusButton	= NULL;								// �`���[�g���A���v���X�{�^��
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CGame ***
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ���[�h����										(public)	*** CGame ***
//=============================================================================
HRESULT CGame::Load(void)
{
	// ���[�h
	CUi_Game::Load();			// Ui�Q�[��
	CMap_02::Load();			// �}�b�v
	CMap_03::Load();			// �}�b�v
	CEffect::Load();			// �G�t�F�N�g
	CPlayer::Load();			// �v���C���[
	CEnemy::Load();				// �G
	CRanking::LoadRanking();	// �����L���O���[�h

	return S_OK;	// �l��Ԃ�
}

//=============================================================================
// �A�����[�h����										(public)	*** CGame ***
//=============================================================================
void CGame::Unload(void)
{
	// �A�����[�h
	CUi_Game::Unload();			// Ui�Q�[��
	CUi_Result::Unload();		// Ui���U���g
	CMap_02::Unload();			// �}�b�v
	CMap_03::Unload();			// �}�b�v
	CEffect::Unload();			// �G�t�F�N�g
	CPlayer::Unload();			// �v���C���[
	CEnemy::Unload();			// �G

	CTexture::ReleaseAll();		// �e�N�X�`���j��
	CText::ReleaseXModelData();	// ���f���f�[�^�j��
}

//=============================================================================
// ����������										(public)	*** CGame ***
//=============================================================================
HRESULT CGame::Init(void)
{
	CSound *pSound = CManager::GetSound();		// �T�E���h�擾
	if (pSound != NULL)
	{// NULL�ȊO�̏ꍇ
		if (m_game == GAME_TUTORIAL)
		{// �`���[�g���A����������
			pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME000);
		}
	}

	CCamera *pCamera = NULL;	// �J����

	D3DXVECTOR3 posPlayer;		// �v���C���[�ʒu
	
	//m_game = GAME_PLAY_1;
	//m_game = GAME_PLAY_B;
	if (m_game == GAME_TUTORIAL || m_game == GAME_PLAY_B)
	{// �`���[�g���A���A�{�X��������
		posPlayer = D3DXVECTOR3(0.0f, 0.0f, -500.0f);
	}
	else if (m_game == GAME_PLAY_1)
	{// �v���C�P��������
		posPlayer.x = (float)((rand() % ((1470 - 250) * 2)) - (1470 - 250));
		posPlayer.y = 0.0f;
		posPlayer.z = (float)((rand() % ((1470 - 250) * 2)) - (1470 - 250));
	}
	else
	{// ���̑�
		posPlayer = posOld;
	}

	// ���C���J����
	pCamera = CManager::GetCamera(CManager::CAMERA_MAIN);	// �J�����擾
	pCamera->SetPos(D3DXVECTOR3(posPlayer.x, 120.0f, posPlayer.z - 230.0f), D3DXVECTOR3(posPlayer.x, 50.0f, posPlayer.z), D3DXVECTOR3(-0.3f, D3DX_PI, 0.0f));		// �J�����ʒu�ݒ�
	pCamera->SetInitPos(D3DXVECTOR3(posPlayer.x, 120.0f, posPlayer.z - 230.0f), D3DXVECTOR3(posPlayer.x, 50.0f, posPlayer.z));	// �J���������ʒu�ݒ�
	CCameraMain *pCameraMain = (CCameraMain*)pCamera;
	pCameraMain->SetMove(true);
	// �}�b�v�J����
	pCamera = CManager::GetCamera(CManager::CAMERA_GAME_MAP);	// �J�����擾
	pCamera->SetPos(D3DXVECTOR3(0.0f, 4000.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �J�����ʒu�ݒ�
	pCamera->SetInitPos(D3DXVECTOR3(0.0f, 4000.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �J���������ʒu�ݒ�
//	pCamera->SetPos(D3DXVECTOR3(0.0f, 500.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �J�����ʒu�ݒ�
//	pCamera->SetInitPos(D3DXVECTOR3(0.0f, 500.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));	// �J���������ʒu�ݒ�

	// �N���G�C�g
	// UI
	m_pUi = CUi::Create(CUi::UI_GAME);
	
	// �G����������
	m_nSetEnemyNum = 0;

	// �}�b�v
	if (m_game == GAME_TUTORIAL || m_game == GAME_PLAY_B)
	{// �`���[�g���A���A�{�X��������
		m_pMap = CMap::Create(CMap::TYPE_NO_04);
		CMap_04 *pMap_04 = (CMap_04*)m_pMap;
		pMap_04->SetField(posPlayer);
		pMap_04->SetObject();

		m_nMapNum = 4;
	}
	else
	{// ���̑�
		int nNum = 1;
		
		/*if (-1200.0f < posOld.x && posOld.x < 1200.0f &&
			-1200.0f < posOld.z && posOld.z < 1200.0f)
		{// �͈͊O��������
			nNum = 1;
		}
		else
		{
			nNum = rand() % 10;
		}*/

		if (nNum == 0)
		{
			m_pMap = CMap::Create(CMap::TYPE_NO_03);
			CMap_03 *pMap_03 = (CMap_03*)m_pMap;
			pMap_03->SetField(posPlayer);
			pMap_03->SetObject();

			m_nMapNum = 3;
		}
		else
		{
			m_pMap = CMap::Create(CMap::TYPE_NO_02);
			CMap_02 *pMap_02 = (CMap_02*)m_pMap;
			pMap_02->SetField(posPlayer);
			pMap_02->SetObject();

			m_nMapNum = 2;
		}
	}

	// �v���C���[
	m_pPlayer = CPlayer::Create(posPlayer, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pPlayer->SetMove(true);

	if (m_game != CGame::GAME_TUTORIAL && m_game != CGame::GAME_PLAY_1)
	{// �`���[�g���A�� + �v���C�P�ȊO
		m_pPlayer->SetHP(m_nPlayerHP);
		m_pPlayer->SetMP(m_nPlayerMP);
	}

	if (m_game == GAME_TUTORIAL)
	{// �`���[�g���A��
		if (m_pPlayer != NULL)
			m_pPlayer->SetAction(true, true);	// ����

		if (m_pClear == NULL)
		{// NULL�̏ꍇ
			m_pClear = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			m_pClear->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 10), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 10), (SCREEN_HEIGHT / 12), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pClear->Init();
			m_pClear->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CLEAR));
		}

		if (m_pCheck == NULL)
		{// NULL�̏ꍇ
			m_pCheck = new CScene3D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			m_pCheck->SetInitAll(D3DXVECTOR3(0.0f, 120.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(35.0f, 20.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);
			m_pCheck->Init();
			m_pCheck->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CHECK));
			m_pCheck->DrawEdit(false, false, false, true, true, 0, D3DCULL_NONE, D3DCMP_ALWAYS);
			m_pCheck->SetTex(8, 3, D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		}

		if (m_pSkip == NULL)
		{// NULL�̏ꍇ
			m_pSkip = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			m_pSkip->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 7) - (SCREEN_WIDTH / 8), SCREEN_HEIGHT - (SCREEN_WIDTH / 24 * 2) - (SCREEN_HEIGHT / 10 * 2) - (SCREEN_HEIGHT / 100), 0.0f), D3DXVECTOR3(0.0f, (SCREEN_HEIGHT / 100), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pSkip->Init();
		}

		if (m_pTutorialPlus == NULL)
		{// NULL�̏ꍇ
			m_pTutorialPlus = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			m_pTutorialPlus->SetInitAll(D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
			m_pTutorialPlus->Init();
			m_pTutorialPlus->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_TUTORIAL_PLUS));
		}

		if (m_pTutorialPlusButton == NULL)
		{// NULL�̏ꍇ
			m_pTutorialPlusButton = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			m_pTutorialPlusButton->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 35), SCREEN_HEIGHT - (SCREEN_WIDTH / 35), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 40), (SCREEN_WIDTH / 40), 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.125f, 1.0f));
			m_pTutorialPlusButton->Init();
			m_pTutorialPlusButton->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
			m_pTutorialPlusButton->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
		}

		if (m_pPlayer != NULL)
			m_pPlayer->SetMove(false);		// ����
		if (pCameraMain != NULL)
			pCameraMain->SetMove(false);	// ����
	}

	CManager::GetRenderer()->SetDrawMap(true);		// �}�b�v�`��

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CGame ***
//=============================================================================
void CGame::Uninit(void)
{
	// �|�[�Y
	if (m_pPause != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pPause->Uninit();		// �I������
		m_pPause = NULL;		// NULL��
	}

	// �}�b�v
	if (m_pMap != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pMap->Uninit();		// �I������
		m_pMap = NULL;			// NULL��
	}

	// UI
	if (m_pUi != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pUi->Uninit();		// �I������
		m_pUi = NULL;			// NULL��
	}

	// �v���C���[
	if (m_pPlayer != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pPlayer->Uninit();	// �I������
		m_pPlayer = NULL;		// NULL��
	}

	// �N���A
	if (m_pClear != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pClear->Uninit();		// �I������
		m_pClear = NULL;		// NULL��
	}

	// �`�F�b�N
	if (m_pCheck != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pCheck->Uninit();		// �I������
		m_pCheck = NULL;		// NULL��
	}

	// �X�L�b�v
	if (m_pSkip != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pSkip->Uninit();		// �I������
		m_pSkip = NULL;			// NULL��
	}

	// �`���[�g���A���v���X
	if (m_pTutorialPlus != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pTutorialPlus->Uninit();		// �I������
		m_pTutorialPlus = NULL;			// NULL��
	}

	// �`���[�g���A���v���X�{�^��
	if (m_pTutorialPlusButton != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pTutorialPlusButton->Uninit();		// �I������
		m_pTutorialPlusButton = NULL;			// NULL��
	}

	CScene::ReleaseMode();		// ���[�h�j��
}

//=============================================================================
// �X�V����											(public)	*** CGame ***
//=============================================================================
void CGame::Update(void)
{
	CSound *pSound = CManager::GetSound();						// �T�E���h�擾

	CInputKeyboard	*pKey = CManager::GetInputKeyboard();		// �L�[�{�[�h�擾
	CInputMouse		*pMouse = CManager::GetInputMouse();		// �}�E�X�擾
	CInputXPad		*pXPad = CManager::GetInputXPad();			// X�p�b�h�擾
	CFade::FADE		fade = CFade::GetFade();					// �t�F�[�h�擾
	CCameraMain		*pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);	// �J�����擾

	if (m_game != GAME_CLEAR && m_game != GAME_OVER)
	{// �N���A + �I�[�o�[�ȊO
		if (pKey != NULL)
		{// NULL�ȊO�̏ꍇ
			if (fade == CFade::FADE_NONE)
			{// �t�F�[�h���Ă��Ȃ�
				if (pKey->GetTrigger(DIK_P) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_START, 0) == true)
				{// �L�[�{�[�h�iP�j�������ꂽ�AX�p�b�h�iSTART�A12�j�������ꂽ
					PauseSeting();		// �|�[�Y�ݒ�

					if (pSound != NULL)
					{// NULL�ȊO�̏ꍇ
						pSound->PlaySoundA(CSound::SOUND_LABEL_SE_PAUSE000);
					}
				}
			}
		}
	}

	if (m_game == GAME_TUTORIAL)
	{// �`���[�g���A����
		if (m_pTutorialPlus != NULL || m_pTutorialPlusButton != NULL)
		{// NULL�ȊO�̏ꍇ
			if (fade == CFade::FADE_NONE)
			{// �t�F�[�h���Ă��Ȃ�
				if (pKey->GetPress(DIK_Z) == true || pKey->GetPress(DIK_E) == true || pKey->GetPress(DIK_RETURN) == true || pXPad->GetPress(XINPUT_GAMEPAD_B, 0) == true)
				{// �A�N�V������������
					if (m_pPlayer != NULL)
						m_pPlayer->SetMove(true);		// ������
					if (pCamera != NULL)
						pCamera->SetMove(true);			// ������

					if (m_nCntTutorialPlus < 150)
					{// ���t���[�����Ⴉ������
						m_nCntTutorialPlus = 150;

						if (pSound != NULL)
						{// NULL�ȊO�̏ꍇ
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
						}
					}
				}

				if (m_nCntTutorialPlus == 150)
				{// ���t���[����������
					if (m_pTutorialPlus != NULL)
					{// NULL�ȊO�̏ꍇ
						D3DXCOLOR col = m_pTutorialPlus->Getcol();
						col.a -= 0.05f;
						m_pTutorialPlus->Setcol(col);

						if (col.a <= 0.0f)
						{// �����ɂȂ�����
							m_pTutorialPlus->Uninit();			// �I������
							m_pTutorialPlus = NULL;				// NULL��
						}
					}

					if (m_pTutorialPlusButton != NULL)
					{// NULL�ȊO�̏ꍇ
						D3DXCOLOR col = m_pTutorialPlusButton->Getcol();
						col.a -= 0.05f;
						m_pTutorialPlusButton->Setcol(col);

						if (col.a <= 0.0f)
						{// �����ɂȂ�����
							m_pTutorialPlusButton->Uninit();	// �I������
							m_pTutorialPlusButton = NULL;		// NULL��
						}
					}
				}
				else
				{// ���̑�
					if (m_pTutorialPlus != NULL)
					{// NULL�ȊO�̏ꍇ
						if (m_nCntTutorialPlus == 0)
						{// ���t���[����������
							if (m_pTutorialPlusButton != NULL)
								m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
						}
						else if (m_nCntTutorialPlus == 60)
						{// ���t���[����������
							if (m_pTutorialPlusButton != NULL)
								m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						}
						m_nCntTutorialPlus = (m_nCntTutorialPlus + 1) % 120;	// �J�E���g
					}
				}
			}
		}
		else
		{// NULL�̏ꍇ
			if (m_nCntTutorial == 0)
			{// �O��������
				if (m_bTutorial[m_nTutorialNum] == false)
				{// �N���A���Ă��Ȃ�
					if (fade == CFade::FADE_NONE)
					{// �t�F�[�h���Ă��Ȃ�
						if (m_nTutorialNum == TUTORIAL_MOVE)
						{// �ړ�
							if (pKey->GetPress(DIK_W) == true || pKey->GetPress(DIK_S) == true || pKey->GetPress(DIK_A) == true || pKey->GetPress(DIK_D) == true ||
								pKey->GetPress(DIK_UP) == true || pKey->GetPress(DIK_DOWN) == true || pKey->GetPress(DIK_LEFT) == true || pKey->GetPress(DIK_RIGHT) == true ||
								pXPad->GetStick(0, 0) == true)
							{// �ړ��L�[��������
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								m_nCntTutorial = 40;
							}
						}
						else if (m_nTutorialNum == TUTORIAL_CAMERA)
						{// �J����
							if (pMouse->GetPointer().x != 0.0f || pMouse->GetPointer().y != 0.0f ||
								pXPad->GetStick(1, 0) == true)
							{// �J�����𓮂�����
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								m_nCntTutorial = 40;
							}
						}
						else if (m_nTutorialNum == TUTORIAL_JUMP)
						{// �W�����v
							if (pKey->GetPress(DIK_SPACE) == true ||
								pXPad->GetPress(XINPUT_GAMEPAD_A, 0) == true)
							{// �W�����v�L�[��������
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								m_nCntTutorial = 40;
								CEnemy::Create(CEnemy::TYPE_N_SLIME, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
								CEffect::Create(CEffect::TYPE_CREATE, CEffect::SHOT_NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}
						}
						else if (m_nTutorialNum == TUTORIAL_LOCKON)
						{// ���b�N�I��
							if (pCamera->GetLockOn() == true)
							{// ���b�N�I������
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								if (m_pPlayer != NULL)
									m_pPlayer->SetAction(false, true);

								m_nCntTutorial = 40;
							}
						}
						else if (m_nTutorialNum == TUTORIAL_ATTACK)
						{// �U��
							if (m_nSetEnemyNum == 0)
							{// �|����
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								m_nCntTutorial = 40;
							}
						}
						else if (m_nTutorialNum == TUTORIAL_GUARD)
						{// �K�[�h
							if (pKey->GetPress(DIK_V) == true ||
								pMouse->GetPress(2) == true ||
								pXPad->GetPress(XINPUT_GAMEPAD_RIGHT_SHOULDER, 0) == true)
							{// �K�[�h�L�[��������
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								if (m_pPlayer != NULL)
									m_pPlayer->SetAction(false, false);

								m_nCntTutorial = 40;
								CEnemy::Create(CEnemy::TYPE_N_SLIME, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
								CEffect::Create(CEffect::TYPE_CREATE, CEffect::SHOT_NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(30.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							}
						}
						else if (m_nTutorialNum == TUTORIAL_SKILL)
						{// �X�L��
							if (m_nSetEnemyNum == 0)
							{// �|����
								m_bTutorial[m_nTutorialNum] = true;
								//	m_nTutorialNum++;

								CMap_04 *pMap_04 = (CMap_04*)m_pMap;
								if (pMap_04->GetGoal() == false)
									pMap_04->SetGoal(true);

								m_nCntTutorial = 40;
							}
						}
						else if (m_nTutorialNum == TUTORIAL_ACTION)
						{// �A�N�V����
							if (fade != CFade::FADE_NONE)
							{// �t�F�[�h����
								m_bTutorial[m_nTutorialNum] = true;
								//m_nTutorialNum++;

								m_nCntTutorial = 40;
							}

							if (m_pCheck != NULL)
							{// NULL�ȊO�̏ꍇ
								m_pCheck->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

								D3DXVECTOR3 pos = m_pCheck->Getpos();		// �ʒu�擾
								
								// �㉺�Ɉړ�
								if (m_nCntCheck < 60)
									pos.y -= 0.1f;
								else
									pos.y += 0.1f;

								m_pCheck->Setpos(pos);						// �ړ��ݒ�

								m_nCntCheck = (m_nCntCheck + 1) % 120;		// �J�E���g
							}
						}
					}
				}
			}
			else
			{// 0�ȊO��������
				if (m_nCntTutorial == 40)
				{// ���t���[����������
					if (m_bTutorial[m_nTutorialNum] == true)
					{// �N���A��������
						if (m_pClear != NULL)
						{// NULL�ȊO�̏ꍇ
							m_pClear->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 10), 0.0f));
							m_pClear->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 10) * 2.0f, (SCREEN_HEIGHT / 12) * 2.0, 0.0f));
							m_pClear->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
							m_pClear->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_CLEAR));
							m_pClear->SetSizeType(10, D3DXVECTOR3((SCREEN_WIDTH / 10), (SCREEN_HEIGHT / 12), 0.0f));
						}

						if (pSound != NULL)
						{// NULL�ȊO�̏ꍇ
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_TUTORIAL_CLEAR);
						}
					}
				}
				else if (m_nCntTutorial == 20)
				{// ���t���[����������
					if (m_bTutorial[m_nTutorialNum] == true)
					{// �N���A��������
						if (m_pClear != NULL)
						{// NULL�ȊO�̏ꍇ
							m_pClear->SetcolType(10, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
						}

						if (m_nTutorialNum < TUTORIAL_ACTION)	// �A�N�V�������O�̂�
							m_nTutorialNum++;
					}
				}
				else if (m_nCntTutorial == 1)
				{// ���t���[����������
					if (m_nTutorialNum == TUTORIAL_ATTACK || m_nTutorialNum == TUTORIAL_SKILL)
					{// �U���A�X�L����������
						if (m_pClear != NULL)
						{// NULL�ȊO�̏ꍇ
							if (m_pClear->Getcol().a != 1.0f)
							{// �s�����ȊO
								//m_pClear->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 7), SCREEN_HEIGHT - (SCREEN_WIDTH / 6), 0.0f));
								m_pClear->Setpos(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 2), SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.6f), 0.0f));
								m_pClear->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 10) * 2.0f, (SCREEN_HEIGHT / 12) * 2.0, 0.0f));
								m_pClear->Setcol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
								m_pClear->BindTexture(CTexture::GetTexture(CTexture::TYPE_GAME_UI_MISION));
								m_pClear->SetSizeType(10, D3DXVECTOR3((SCREEN_WIDTH / 8), (SCREEN_HEIGHT / 14), 0.0f));

								m_nCntTutorial = 39;
							}
						}
					}
				}

				m_nCntTutorial--;
				if (m_nCntTutorial < 0) m_nCntTutorial = 0;
			}

			if (fade == CFade::FADE_NONE)
			{// �t�F�[�h���Ă��Ȃ�
				if (pKey->GetPress(DIK_Z) == true || pKey->GetPress(DIK_E) == true || pKey->GetPress(DIK_RETURN) == true || pXPad->GetPress(XINPUT_GAMEPAD_B, 0) == true)
				{// �A�N�V������������
					if (m_nCntSkip < 60)
					{// ���t���[�����Ⴉ������
						m_nCntSkip++;		// �J�E���g
					}
					else if (m_nCntSkip == 60)
					{// ���t���[���ɂȂ�����
						if (m_game < GAME_PLAY_B)
						{// �{�X���O��������
							m_game = (CGame::GAME)(m_game + 1);
							CFade::Create(m_game);
							m_pPlayer->SetMove(false);

							if (pSound != NULL)
							{// NULL�ȊO�̏ꍇ
								pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
							}
						}
					}
				}
				else
				{// �����Ă��Ȃ�
					m_nCntSkip = 0;
				}

				if (m_pSkip != NULL)
				{// NULL�ȊO�̏ꍇ
					// �ʒu�Ƒ傫��
					m_pSkip->Setpos(D3DXVECTOR3(((SCREEN_WIDTH / 7) - (SCREEN_WIDTH / 8)) + ((SCREEN_WIDTH / 8) * ((float)m_nCntSkip / 60.0f)), SCREEN_HEIGHT - (SCREEN_WIDTH / 24 * 2) - (SCREEN_HEIGHT / 10 * 2) - (SCREEN_HEIGHT / 100), 0.0f));
					m_pSkip->Setsize(D3DXVECTOR3((SCREEN_WIDTH / 8) * ((float)m_nCntSkip / 60.0f), (SCREEN_HEIGHT / 100), 0.0f));
				}
			}
		}
	}
	else if (m_game == GAME_PLAY_B)
	{// �{�X��
		if (m_nSetEnemyNum == 0)
		{// �G���O
			m_game = GAME_CLEAR;
			m_nCntTimer = 0;

			if (m_bPlay_SS02 == false)
			{
				if (fade == CFade::FADE_NONE)
				{// �t�F�[�h���Ă��Ȃ�
					if (ScreenShot("result02", true))
						m_bPlay_SS02 = true;
				}
			}
		}
		else
		{// �G������
			if (m_nCntTimer == 0)
			{// �O��������
				m_pPlayer->SetMove(false);		// �ړ��ł��Ȃ�
				pCamera->SetPickUp(true);		// �s�b�N�A�b�v
				m_nCntTimer = 1;

				CScene::SetDrawUi(false);		// UI��\��

				if (pSound != NULL)
				{// NULL�ȊO�̏ꍇ
					pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME000);
				}
			}

			if (m_nCntTimer == 1 && pCamera->GetPickUp() == false)
			{// �s�b�N�A�b�v���I������
				m_pPlayer->SetMove(true);		// �ړ��ł���
				m_nCntTimer = 2;

				CScene::SetDrawUi(true);		// UI�\��

				if (pSound != NULL)
				{// NULL�ȊO�̏ꍇ
					pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME001);
				}
			}
		}
	}
	else if (m_game == GAME_CLEAR)
	{// �N���A
		if (m_nCntTimer == 0)
		{// ���t���[����������
			CScene::SetStartUp(CScene::PRIORITY_UI);
			pCamera->SetMove(false);
			m_pPlayer->SetMove(false);

			if (pSound != NULL)
			{// NULL�ȊO�̏ꍇ
				pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME001);
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_GAMEEND);
			}
		}
		else if (m_nCntTimer == 45)
		{// ���t���[����������
			CScene::SetUpdateTime(4);					// �X���[
			CScene::SetStartUp(CScene::PRIORITY_BG);	// �w�i����X�V
		}
		else if (m_nCntTimer == 165)
		{// ���t���[����������
			CScene::SetStartUp(CScene::PRIORITY_UI);
			CScene::SetUpdateTime(1);

			CScene2D_Preset *pScene2D_Preset = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			pScene2D_Preset->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			pScene2D_Preset->Init();
			pScene2D_Preset->SetcolType(30, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.75f));
		}
		else if (m_nCntTimer == 210)
		{// ���t���[����������
			int nScore = GetScore();		// �X�R�A�擾

			// �N���A
			nScore += 15000;

			// �{�X���j
			nScore += 1000;

			CRanking::SetScore(nScore);		// �X�R�A�ݒ�
			CRanking::SaveRanking();		// �����L���O�Z�[�u
			CUi_Result::Load();				// Ui���U���g
			CUi::Create(CUi::UI_RESULT);	// Ui���U���g����

			if (m_pTutorialPlusButton == NULL)
			{// NULL�̏ꍇ
				m_pTutorialPlusButton = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
				m_pTutorialPlusButton->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 35), SCREEN_HEIGHT - (SCREEN_WIDTH / 35), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 40), (SCREEN_WIDTH / 40), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.125f, 1.0f));
				m_pTutorialPlusButton->Init();
				m_pTutorialPlusButton->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
				m_pTutorialPlusButton->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			}

			if (pSound != NULL)
			{// NULL�ȊO�̏ꍇ
				pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME002);
			}
		}

		if (210 < m_nCntTimer)
		{// ���t���[�����傫��������
			if (pKey != NULL)
			{// NULL�ȊO�̏ꍇ
				if (fade == CFade::FADE_NONE)
				{// �t�F�[�h���Ă��Ȃ�
					if (pKey->GetTrigger(DIK_RETURN) == true || pKey->GetTrigger(DIK_E) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
					{// �L�[�{�[�h�iP�j�������ꂽ�AX�p�b�h�iSTART�A12�j�������ꂽ
						CFade::Create(CManager::MODE_TITLE);

						if (pSound != NULL)
						{// NULL�ȊO�̏ꍇ
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
						}
					}
				}
			}
		}

		m_nCntTimer++;		// �J�E���g

		if (m_pTutorialPlusButton != NULL)
		{// NULL�ȊO�̏ꍇ
			if (m_nCntTutorialPlus == 0)
			{// ���t���[����������
				m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
			else if (m_nCntTutorialPlus == 60)
			{// ���t���[����������
				m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			m_nCntTutorialPlus = (m_nCntTutorialPlus + 1) % 120;		// �J�E���g
		}
	}
	else if (m_game == GAME_OVER)
	{// �I�[�o�[
		if (m_nCntTimer == 0)
		{// ���t���[����������
			CScene::SetUpdateTime(4);						// �X���[
			CManager::GetRenderer()->SetDrawMap(false);		// �}�b�v��\��

			if (m_pUi != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pUi->Uninit();
				m_pUi = NULL;
			}

			if (pSound != NULL)
			{// NULL�ȊO�̏ꍇ
				pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME001);
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE_GAMEEND);
			}
		}
		else if (m_nCntTimer == 165)
		{// ���t���[����������
			//CScene::SetStartUp(CScene::PRIORITY_UI);
			CScene::SetUpdateTime(1);

			CScene2D_Preset *pScene2D_Preset = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
			pScene2D_Preset->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
			pScene2D_Preset->Init();
			pScene2D_Preset->SetcolType(30, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.75f));
		}
		else if (m_nCntTimer == 210)
		{// ���t���[����������
			CUi_Result::Load();				// Ui���U���g
			CUi::Create(CUi::UI_RESULT);	// Ui���U���g����

			if (m_pTutorialPlusButton == NULL)
			{// NULL�̏ꍇ
				m_pTutorialPlusButton = new CScene2D_Preset(CScene::PRIORITY_UI, CScene::OBJTYPE_UI);
				m_pTutorialPlusButton->SetInitAll(D3DXVECTOR3(SCREEN_WIDTH - (SCREEN_WIDTH / 35), SCREEN_HEIGHT - (SCREEN_WIDTH / 35), 0.0f), D3DXVECTOR3((SCREEN_WIDTH / 40), (SCREEN_WIDTH / 40), 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.125f, 1.0f));
				m_pTutorialPlusButton->Init();
				m_pTutorialPlusButton->BindTexture(CTexture::GetTexture(CTexture::TYPE_MODE_BUTTON));
				m_pTutorialPlusButton->SetTex(8, 3, D3DXVECTOR2(0.125f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
			}

			if (pSound != NULL)
			{// NULL�ȊO�̏ꍇ
				pSound->PlaySoundA(CSound::SOUND_LABEL_BGM_GAME002);
			}
		}

		if (210 < m_nCntTimer)
		{// ���t���[�����傫��������
			if (pKey != NULL)
			{// NULL�ȊO�̏ꍇ
				if (fade == CFade::FADE_NONE)
				{// �t�F�[�h���Ă��Ȃ�
					if (pKey->GetTrigger(DIK_RETURN) == true || pKey->GetTrigger(DIK_E) == true || pXPad->GetTrigger(XINPUT_GAMEPAD_B, 0) == true)
					{// �L�[�{�[�h�iP�j�������ꂽ�AX�p�b�h�iSTART�A12�j�������ꂽ
						CFade::Create(CManager::MODE_TITLE);

						if (pSound != NULL)
						{// NULL�ȊO�̏ꍇ
							pSound->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER000);
						}
					}
				}
			}
		}

		m_nCntTimer++;		// �J�E���g

		if (m_pTutorialPlusButton != NULL)
		{// NULL�ȊO�̏ꍇ
			if (m_nCntTutorialPlus == 0)
			{// ���t���[����������
				m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			}
			else if (m_nCntTutorialPlus == 60)
			{// ���t���[����������
				m_pTutorialPlusButton->SetcolType(60, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			m_nCntTutorialPlus = (m_nCntTutorialPlus + 1) % 120;		// �J�E���g
		}
	}

	fade = CFade::GetFade();					// �t�F�[�h�擾
	if (m_bPlay_SS00 == false)
	{// �܂��B���Ă��Ȃ�
		if (m_game == GAME_PLAY_1)
		{// �v���C�P��������
			if (fade == CFade::FADE_NONE)
			{// �t�F�[�h���Ă��Ȃ�
				if (ScreenShot("result00", true))
					m_bPlay_SS00 = true;
			}
		}
	}

	if (m_bSetSS == true)
	{// �X�N�V�����B��
		if (m_bPlay_SS01 == false)
		{// �܂��B���Ă��Ȃ�
			if (GAME_PLAY_1 <= m_game)
			{// �v���C�P�ȏゾ������
				if (fade == CFade::FADE_NONE)
				{// �t�F�[�h���Ă��Ȃ�
					if (ScreenShot("result01", true))
						m_bPlay_SS01 = true;
				}
			}
		}
	}
	
	if (m_bSetSS_Death == true)
	{// �X�N�V�����B��
		if (m_bPlay_SS02 == false)
		{// �܂��B���Ă��Ȃ�
			if (GAME_PLAY_1 <= m_game)
			{// �v���C�P�ȏゾ������
				if (fade == CFade::FADE_NONE)
				{// �t�F�[�h���Ă��Ȃ�
					if (ScreenShot("result02", true))
						m_bPlay_SS02 = true;
				}
			}
		}
	}

	m_bSetSS = false;
	m_bSetSS_Death = false;

	if (fade == CFade::FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ�
		if (m_bPause == false)
		{// �|�[�Y���Ă��Ȃ�
			if (m_game != GAME_TUTORIAL && m_game != GAME_CLEAR && m_game != GAME_OVER)
			{// �`���[�g���A�� + �N���A + �I�[�o�[�ȊO
				m_nTimer++;
			}
		}
	}
}

//=============================================================================
// �`�揈��											(public)	*** CGame ***
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// �Q�[���ݒ菈��											(public)	*** CGame ***
//=============================================================================
void CGame::SetGame(CGame::GAME game, bool bReset)
{
	if (m_pPlayer != NULL)
	{// NULL�ȊO�̏ꍇ
		if (m_game != CGame::GAME_TUTORIAL)
		{// �`���[�g���A���ȊO
			m_nPlayerHP = m_pPlayer->GetHP();
			m_nPlayerMP = m_pPlayer->GetMP();
		}
	}

	if (bReset == true)
	{// ���Z�b�g����
		if (m_game != GAME_TUTORIAL && m_game != GAME_CLEAR && m_game != GAME_OVER)
		{// �`���[�g���A�� + �N���A + �I�[�o�[�ȊO
			if (m_nSetEnemyNum != 0)
			{// �G������
				m_bAllDeathEnemy = false;
			}
		}

		posOld = m_pPlayer->Getpos();		// �ʒu�L��

		// �I������
		Uninit();

		// �J�������Z�b�g
		{
			CCameraMain *pCameraMein = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
			CCameraMap *pCameraMap = (CCameraMap*)CManager::GetCamera(CManager::CAMERA_GAME_MAP);

			if (pCameraMein != NULL)
				pCameraMein->Uninit();		// �I������
			if (pCameraMap != NULL)
				pCameraMap->Uninit();		// �I������
		}
	}

	// �Q�[�����X�V
	m_game = game;
	m_nCntTimer = 0;

	if (bReset == true)
	{// ���Z�b�g����
		// ����������
		Init();
	}
}

//=============================================================================
// �|�[�Y�ݒ菈��										(public)	*** CGame ***
//=============================================================================
void CGame::PauseSeting(void)
{
	CCameraMain		*pCamera = (CCameraMain*)CManager::GetCamera(CManager::CAMERA_MAIN);
	m_bPause = m_bPause ? false : true;		// �|�[�Y�؂�ւ�

	if (m_bPause == true)
	{// �|�[�Y���Ă���
		if (m_pPause == NULL)
		{// NULL�̏ꍇ
			m_pPause = CPause::Create();	// �|�[�Y
		}
		CScene::SetStartUp(CScene::PRIORITY_PAUSE);		// �X�V�͈́i�|�[�Y����j
		pCamera->SetMove(false);

		if (pCamera->GetPickUp() == true)
			CScene::SetDrawUi(true);
	}
	else
	{// �|�[�Y���Ă��Ȃ�
		if (m_pPause != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pPause->Uninit();	// �I������
			m_pPause = NULL;	// NULL��
		}
		CScene::SetStartUp(0);							// �X�V�͈́i�O����j
		pCamera->SetMove(true);

		if (pCamera->GetPickUp() == true)
			CScene::SetDrawUi(false);
	}
}

//=============================================================================
// �X�R�A�擾����										(public)	*** CGame ***
//=============================================================================
int CGame::GetScore(void)
{
	int nScore = 0;

	// �^�C��
	if (m_nTimer < 4000) nScore += 15000;
	else if (m_nTimer < 4500) nScore += 12000;
	else if (m_nTimer < 5000) nScore += 10000;
	else if (m_nTimer < 5500) nScore += 8000;
	else if (m_nTimer < 6000) nScore += 5000;
	else nScore += 3000;

	// ���j��
	nScore += (500 * m_nDeathEnemy);

	// �m�[�_���[�W
	if (m_bNoDamage == true) nScore += 10000;
	else if (m_pPlayer->GetHP() == CPlayer::m_MAX_HP) nScore += 5000;

	// �W���X�g�K�[�h����
	nScore += (800 * m_nJastGuard);

	// �S�Č��j
	if (m_bAllDeathEnemy == true) nScore += 10000;

	// �{�X���s
	if (m_nTimer < 3000) nScore += 5000;

	return nScore;
}