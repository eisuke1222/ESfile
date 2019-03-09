//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "tutorial.h"		// �`���[�g���A��
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "scene2d.h"		// �V�[��2D
#include "fade.h"			// �t�F�[�h
#include "camera.h"			// �J����
#include "meshfield.h"		// ���b�V���t�B�[���h
#include "object3d.h"		// �I�u�W�F�N�g3D
#include "effect3d.h"		// �G�t�F�N�g3D
#include "bg.h"				// �w�i
#include "enemy.h"			// �G
#include "player.h"			// �v���C���[
#include "bullet.h"			// �e
#include "effect.h"			// �G�t�F�N�g
#include "explosion.h"		// ����
#include "cloud.h"			// �_
#include "number.h"			// ����
#include "bomb.h"			// ���e
#include "gauge.h"			// �Q�[�W
#include "life.h"			// �̗�
#include "score.h"			// �X�R�A
#include "pause.h"			// �|�[�Y

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\playerNum.png"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CTutorial::m_pTexture				= NULL;		// �e�N�X�`�����ւ̃|�C���^
CScene2D			*CTutorial::m_pScene2D[MAX_PLAYER]	= {};		// �V�[��2D
CPause				*CTutorial::m_pPause			= NULL;			// �|�[�Y
bool				CTutorial::m_bPause				= false;		// �|�[�Y�g�p

//=============================================================================
// �R���X�g���N�^									(public)	*** CTutorial ***
//=============================================================================
CTutorial::CTutorial()
{
	// �����ݒ�
	m_bPause	= false;		// �|�[�Y�g�p
	m_mapfield = 0;				// �t�B�[���h�ԍ�
	m_mapfieldOld	= 0;		// �t�B�[���h�ԍ�
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CTutorial ***
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// ���[�h����							(public)	*** CTutorial ***
//=============================================================================
HRESULT CTutorial::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�擾

	if (m_pTexture == NULL)
	{// NULL�̏ꍇ
	 // �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CTutorial ***
//=============================================================================
void CTutorial::Unload(void)
{
	if (m_pTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pTexture->Release();					// ���
		m_pTexture = NULL;						// NULL��
	}
}

//=============================================================================
// ����������										(public)	*** CTutorial ***
//=============================================================================
HRESULT CTutorial::Init(void)
{
	CCamera *pCamera = CManager::GetCamera();		// �J�����擾
	pCamera->SetPos(D3DXVECTOR3(0.0f, 750.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));		// �J���������ݒ�

#ifdef _DEBUG
	CManager::SetPlayNum(CInputXPad::GetInputNum());
#endif // !1

	// 3D���[�h
	CMeshField::Load();		// ���b�V���t�B�[���h
	CObject3D::Load();		// ���f��
	CEffect3D::Load();		// ���f��
	CEnemy::Load();			// �G
	CPlayer::Load();		// �v���C���[
	CBullet::Load();		// �e
	CEffect::Load();		// �G�t�F�N�g
	CExplosion::Load();		// ����
	CCloud::Load();			// �_
	// 3D�N���G�C�g
	CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
		D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);		// ���b�V���t�B�[���h
	CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
		D3DXVECTOR3(0.0f, -100.0f, 1200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);	// ���b�V���t�B�[���h

	// 2D���[�h
	CBg::Load();			// �w�i
	CLife::Load();			// �̗�
	CBomb::Load();			// ���e
	CGauge::Load();			// �Q�[�W
	CNumber::Load();		// ����
	// 2D�N���G�C�g
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1, CBg::TEXTURE_TUTORIAL);	// �w�i

	for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
	{
		CLife::Create(D3DXVECTOR3(50.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 180, 0.0f), D3DXVECTOR3(LIFE_SIZE_X, LIFE_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);					// �̗�
		CBomb::Create(3, D3DXVECTOR3(50.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 260, 0.0f), D3DXVECTOR3(BOMB_SIZE_X, BOMB_SIZE_Y, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);				// ���e
		CGauge::Create(D3DXVECTOR3(100.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 340, 0.0f), D3DXVECTOR3(GAUGE_SIZE_X, GAUGE_SIZE_Y, 0.0f), D3DXCOLOR(0.6f, 1.0f, 0.1f, 1.0f), nCntCont);				// �Q�[�W
		CScore::Create(D3DXVECTOR3(SCREEN_WIDTH_0 - 50.0f + ((SCREEN_WIDTH - 200.0f) * nCntCont), 100.0f, 0.0f), D3DXVECTOR3(12.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), nCntCont);			// �X�R�A
	}
	CPause::Load();			// �|�[�Y

	for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
	{
		if (m_pScene2D[nCnt] == NULL)
		{
			m_pScene2D[nCnt] = new CScene2D(CScene::PRIORITY_UI);
			m_pScene2D[nCnt]->SetInitAll(D3DXVECTOR3(0.0f + 60.0f + ((SCREEN_WIDTH - 60.0f - 60.0f) * nCnt), SCREEN_HEIGHT - 40.0f, 0.0f), D3DXVECTOR3(35.0f, 30.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.5f));
			m_pScene2D[nCnt]->Init();
			m_pScene2D[nCnt]->SetObjType(CScene::OBJTYPE_PLAYERNUM);
			m_pScene2D[nCnt]->BindTexture(m_pTexture);

			m_pScene2D[nCnt]->SetTex(1, nCnt, D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	// �����ݒ�
	m_pPause	= NULL;			// �|�[�Y
	m_bPause	= false;		// �|�[�Y�g�p
	m_mapfield	= 0;			// �t�B�[���h�ԍ�
	
	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CTutorial ***
//=============================================================================
void CTutorial::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pScene2D[nCnt] != NULL)
		{
			m_pScene2D[nCnt]->Uninit();		// �I������
			m_pScene2D[nCnt] = NULL;		// NULL��
		}
	}

	if (m_pPause != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pPause->Uninit();	// �I������
		m_pPause = NULL;	// NULL
	}

	// 3D�A�����[�h
	CMeshField::Unload();	// ���b�V���t�B�[���h
	CObject3D::Unload();	// ���f��
	CEffect3D::Unload();	// ���f��
	CEnemy::Unload();		// �G
	CPlayer::Unload();		// �v���C���[
	CBullet::Unload();		// �e
	CEffect::Unload();		// �G�t�F�N�g
	CExplosion::Unload();	// ����

	// 2D�A�����[�h
	CBg::Unload();			// �w�i
	CLife::Unload();		// �̗�
	CBomb::Unload();		// ���e
	CGauge::Unload();		// �Q�[�W
	CNumber::Unload();		// ����

	CPause::Unload();		// �|�[�Y

	CScene *pScene;			// �V�[���|�C���^

	for (int nCntPriority = 0; nCntPriority <= CScene::PRIORITY_PAUSE; nCntPriority++)
	{// �v���C�I���e�B�J�E���g
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// �V�[���J�E���g
			pScene = CScene::GetScene(nCntPriority, nCntScene);		// �V�[���ǂݍ���

			if (pScene != NULL)
			{// NULL�ȊO�̏ꍇ
				pScene->Uninit();		// �I������
				pScene = NULL;			// NULL��
			}
		}
	}
}

//=============================================================================
// �X�V����											(public)	*** CTutorial ***
//=============================================================================
void CTutorial::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// ���̓f�o�C�X
	CInputXPad *pInputXPad = CManager::GetInputXPad();				// ���̓f�o�C�X
	CFade::FADE fade = CFade::GetFade();	// �t�F�[�h��Ԏ擾

	if (m_bPause == false)
	{
		SetMap();		// �}�b�v�ݒ�
	}

	//if (pInputKeyboard->GetTrigger(DIK_P) == true && fade == CFade::FADE_NONE)
	for (int nCnt = 0; nCnt < CManager::GetPlayNum(); nCnt++)
	{
		if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_START, nCnt) == true && fade == CFade::FADE_NONE)
		{// �C�ӂ̃L�[�iP�j���������ꍇ
			m_bPause = m_bPause ? false : true;		// �\��or��\��

			switch (m_bPause)
			{// �|�[�Y���g�p
			case true:
				if (m_pPause == NULL)
				{// �����ƍX�V����
					m_pPause = CPause::Create();					// �|�[�Y�N���G�C�g

					CScene::SetStartUp(CScene::PRIORITY_PAUSE);		// �X�V�͈͂��|�[�Y����
				}

				break;
			case false:
				if (m_pPause != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pPause->Uninit();		// �I������
					m_pPause = NULL;		// NULL

					CScene::SetStartUp(0);	// �X�V�͈͂�������
				}

				break;
			}
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true && m_bPause == false)
	{
		CFade::Create(CManager::MODE_TITLE, CFade::FADE_OUT);
	}
}

//=============================================================================
// �`�揈��											(public)	*** CTutorial ***
//=============================================================================
void CTutorial::Draw(void)
{
	
}

//=============================================================================
// �|�[�Y�ݒ菈��									(public)	*** CTutorial ***
//=============================================================================
void CTutorial::SetPause(bool bPause)
{
	m_bPause = bPause;		// �|�[�Y

	switch (CTutorial::m_bPause)
	{// �|�[�Y
	case true:
		if (CTutorial::m_pPause == NULL)
		{// NULL�̏ꍇ
			m_pPause = CPause::Create();					// �|�[�Y�N���G�C�g

			CScene::SetStartUp(CScene::PRIORITY_PAUSE);		// �X�V�͈͂��|�[�Y����
		}

		break;
	case false:
		if (m_pPause != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pPause->Uninit();		// �I������
			m_pPause = NULL;		// NULL

			CScene::SetStartUp(0);	// �X�V�͈͂�������
		}

		break;
	}
}

//=============================================================================
// �}�b�v�ݒ菈��									(public)	*** CTutorial ***
//=============================================================================
void CTutorial::SetMap(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposV();		// �J�������_�擾
	int nNummap = m_mapfieldOld;

	if ((int)(Camerapos.z - 1.0f) % 1200 == 0)
	{// �J���������Ԋu�i�񂾂�
		if (m_mapfield % 6 < 3)
		{// �C
			CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
				D3DXVECTOR3(0.0f, -100.0f, 2400.0f + (m_mapfield * 1200.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_O, CMesh3D::MESHTEX_NUMSIZE);				// ���b�V���t�B�[���h

			m_mapfieldOld = 0;
		}
		else
		{// �n��
			CMeshField::Create(CMesh3D::MESHTYPE_FIELD, D3DXVECTOR3(6, 0, 8),
				D3DXVECTOR3(0.0f, 0.0f, 2400.0f + (m_mapfield * 1200.0f)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 0.0f, 600.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CMeshField::MESHFIELDTYPE_G, CMesh3D::MESHTEX_NUMSIZE);				// ���b�V���t�B�[���h

			m_mapfieldOld = 1;
		}

		if (0 < m_mapfield && nNummap != m_mapfieldOld)
		{
			CObject3D::Create(D3DXVECTOR3(0.0f, 0.0f, 2400.0f + (m_mapfield * 1200.0f) - 600.0f), D3DXVECTOR3(0.0f, (D3DX_PI * (m_mapfieldOld ^ 1)), 0.0f), CObject3D::OBJECTTYPE_O_G);
		}

		m_mapfield++;		// �t�B�[���h�J�E���g
	}

	if ((int)(Camerapos.z - 1.0f + 690.0f) % 1200 == 0)
	{// �J���������Ԋu�i�񂾂�
		if ((m_mapfield - 1) % 2 == 0)
		{
			CEnemy::Create(D3DXVECTOR3(-200.0f + (200.0f * (rand() % 3)), 300.0f, Camerapos.z + 700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-3.0f, 0.0f, -3.0f), CEnemy::ENEMYTYPE_S_1, 60);
		}
		else if ((m_mapfield - 1) % 6 < 3)
		{
			CEnemy::Create(D3DXVECTOR3(-300.0f + (300.0f * (rand() % 3)), -100.0f, Camerapos.z + 1400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), CEnemy::ENEMYTYPE_O_1, 80);
		}
		else
		{
			CEnemy::Create(D3DXVECTOR3(0.0f, 10.0f, Camerapos.z + 1700.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), CEnemy::ENEMYTYPE_G_1, 70);

			CObject3D::Create(D3DXVECTOR3(-200.0f, 0.0f, Camerapos.z + 1500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject3D::OBJECTTYPE_BILL_1);		// ���f��
			CObject3D::Create(D3DXVECTOR3(200.0f, 0.0f, Camerapos.z + 1500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CObject3D::OBJECTTYPE_BILL_1);		// ���f��
		}
	}

	if ((int)(Camerapos.z - 1.0f) % 300 == 0)
	{// �J���������Ԋu�i�񂾂�
		float size = (float)((rand() % 50) * 0.01f) + 1.0f;		// �傫�������_��

		CCloud::Create(D3DXVECTOR3(-150.0f + (float)(rand() % 300), 400.0f, Camerapos.z + 600.0f + (float)(rand() % 100)), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f), 
			D3DXVECTOR3(50.0f * size, 30.0f * size, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f), D3DXVECTOR2(1.0f - ((rand() % 2) * 2.0f), 1.0f - ((rand() % 2) * 2.0f)), CScene3D::SCENE3DTYPE_NORMAL);			// �_
	}
}