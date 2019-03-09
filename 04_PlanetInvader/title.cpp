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
#include "input.h"			// ����
#include "sound.h"			// �T�E���h
#include "fade.h"			// �t�F�[�h
#include "model3d.h"		// ���f��3D
#include "scene3d.h"		// �V�[��3D
#include "scene2d.h"		// �V�[��2D
#include "camera.h"			// �J����
#include "menu.h"			// ���j���[
#include "number.h"			// ����
#include "ranking.h"		// �����L���O
#include "player.h"			// �v���C���[
#include "effect.h"			// �G�t�F�N�g
#include "meshcylinder.h"	// ���b�V���V�����_�[

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME		"data\\MODEL\\titlelogo.x"				// ���f��
#define TEXTURE_NAME_1	"data\\TEXTURE\\press.png"				// �e�N�X�`��1
#define TEXTURE_NAME_2	"data\\TEXTURE\\titlestar000.png"		// �e�N�X�`��2
#define TEXTURE_NAME_3	"data\\TEXTURE\\titleplayer.png"		// �e�N�X�`��3

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	*CTitle::m_pModelTexture	= NULL;					// �e�N�X�`�����ւ̃|�C���^
LPD3DXMESH			CTitle::m_pMesh				= NULL;					// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER		CTitle::m_pBuffMat			= NULL;					// �}�e���A�����ւ̃|�C���^
DWORD				CTitle::m_nNumMat			= NULL;					// �}�e���A�����̐�
CModel3D			*CTitle::m_pModel3D			= NULL;					// ���f��

LPDIRECT3DTEXTURE9	CTitle::m_pPressTexture		= NULL;					// �e�N�X�`�����ւ̃|�C���^
CScene3D			*CTitle::m_pPress3D			= NULL;					// �v���X3D

LPDIRECT3DTEXTURE9	CTitle::m_pStarTexture		= NULL;					// �e�N�X�`�����ւ̃|�C���^
CScene3D			*CTitle::m_pStar3D			= NULL;					// �X�^�[3D

LPDIRECT3DTEXTURE9	CTitle::m_pSceneTexture		= NULL;					// �e�N�X�`�����ւ̃|�C���^
CScene2D			*CTitle::m_pScene2D[MAX_PLAYER] = {};				// �V�[��2D

CRanking			*CTitle::m_pRanking			= NULL;					// �����L���O

CTitle::TITLE		CTitle::m_title				= CTitle::TITLE_NONE;	// �^�C�g��

int					CTitle::m_nCntTitle			= 0;					// �^�C�g���J�E���^
bool				CTitle::m_bTitleSet_T		= false;				// �^�C�g���Z�b�g
bool				CTitle::m_bTitleSet_R		= false;				// �����L���O�Z�b�g
bool				CTitle::m_bPressSet			= false;				// �v���X�Z�b�g
bool				CTitle::m_bPress_R			= false;				// �v���X�����L���O

CPlayer				*CTitle::m_pPlayer[MAX_PLAYER] = {};				// �v���C���[
bool				CTitle::m_bPlayGame			= false;				// �v���C�Q�[��

bool				CTitle::m_bPlayerUse[MAX_CONTROLLERS] = {};			// �v���C���[�g�p
bool				CTitle::m_bPlayerPress[MAX_CONTROLLERS] = {};		// �v���C���[�v���X

//=============================================================================
// �R���X�g���N�^									(public)	*** CTitle ***
//=============================================================================
CTitle::CTitle()
{
	// �����l�ݒ�
	m_nCntTitle		= 0;			// �^�C�g���J�E���^
	m_bTitleSet_T	= false;		// �^�C�g���Z�b�g 
	m_bTitleSet_R	= false;		// �����L���O�Z�b�g
	m_bPressSet		= false;		// �v���X�Z�b�g
	m_bPress_R		= false;		// �v���X�����L���O
	m_bPlayGame		= false;		// �v���C�Q�[��

	for (int nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{// �J�E���g
		m_bPlayerUse[nCntCont] = false;
	}
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CTitle ***
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ���[�h����							(public)	*** CBullet ***
//=============================================================================
HRESULT CTitle::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�擾

	if (m_pBuffMat == NULL && m_pMesh == NULL)
	{// NULL�̏ꍇ
		// X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(MODEL_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

		D3DXMATERIAL *pmat;							// �}�e���A���f�[�^�ւ̃|�C���^
		D3DMATERIAL9 *matDef;						// ���݂̃}�e���A���ۑ��p
		pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
		matDef = new D3DMATERIAL9[m_nNumMat];
		m_pModelTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];

		for (DWORD tex = 0; tex < m_nNumMat; tex++)
		{// �J�E���g
			matDef[tex] = pmat[tex].MatD3D;
			matDef[tex].Ambient = matDef[tex].Diffuse;
			m_pModelTexture[tex] = NULL;
			if (pmat[tex].pTextureFilename != NULL &&
				lstrlen(pmat[tex].pTextureFilename) > 0)
			{// �e�N�X�`�����g�p���Ă���
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					pmat[tex].pTextureFilename,
					&m_pModelTexture[tex])))
				{
					MessageBox(NULL, "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
				}
			}
		}
	}

	if (m_pPressTexture == NULL)
	{// NULL�̏ꍇ
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_1, &m_pPressTexture);
	}

	if (m_pStarTexture == NULL)
	{// NULL�̏ꍇ
	 // �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_2, &m_pStarTexture);
	}

	if (m_pSceneTexture == NULL)
	{// NULL�̏ꍇ
	 // �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME_3, &m_pSceneTexture);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CBullet ***
//=============================================================================
void CTitle::Unload(void)
{
	// �e�N�X�`���̔j��
	for (DWORD tex = 0; tex < m_nNumMat; tex++)
	{// �J�E���g
		if (m_pModelTexture[tex] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pModelTexture[tex]->Release();		// ���
			m_pModelTexture[tex] = NULL;			// NULL��
		}
	}

	// �}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pBuffMat->Release();						// ���
		m_pBuffMat = NULL;							// NULL��
	}

	// �}�e���A���̊J��
	if (m_pMesh != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pMesh->Release();							// ���
		m_pMesh = NULL;								// NULL��
	}

	// �e�N�X�`���̔j��
	if (m_pPressTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pPressTexture->Release();					// ���
		m_pPressTexture = NULL;						// NULL��
	}

	if (m_pStarTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pStarTexture->Release();					// ���
		m_pStarTexture = NULL;						// NULL��
	}

	if (m_pSceneTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pSceneTexture->Release();					// ���
		m_pSceneTexture = NULL;						// NULL��
	}
}

//=============================================================================
// ����������										(public)	*** CTitle ***
//=============================================================================
HRESULT CTitle::Init(void)
{
	// 3D���[�h
	CMeshCylinder::Load();			// ���b�V���V�����_�[
	CMenu::Load();					// ���j���[
	CPlayer::Load();				// �v���C���[
	// 3D�N���G�C�g
	CMeshCylinder::Create(CMesh3D::MESHTYPE_CYLINDER, D3DXVECTOR3(24, 1, 0),
		D3DXVECTOR3(0.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1000.0f, -1.0f), 
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(6.0f, 1.0f), 0, D3DXVECTOR2(0.0005f, -0.0002f), CMesh3D::MESHTEX_ALL);			// ���b�V���V�����_�[
	CMeshCylinder::Create(CMesh3D::MESHTYPE_CYLINDER, D3DXVECTOR3(24, 1, 0),
		D3DXVECTOR3(0.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1000.0f, -1.0f), 
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(6.0f, 1.0f), 1, D3DXVECTOR2(0.001f, -0.0005f), CMesh3D::MESHTEX_ALL);			// ���b�V���V�����_�[
	CMeshCylinder::Create(CMesh3D::MESHTYPE_CYLINDER, D3DXVECTOR3(24, 1, 0),
		D3DXVECTOR3(0.0f, -500.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 1000.0f, -1.0f), 
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(6.0f, 1.0f), 2, D3DXVECTOR2(0.0015f, -0.001f), CMesh3D::MESHTEX_ALL);			// ���b�V���V�����_�[

	CNumber::Load();				// ����				
	CRanking::Load();				// �����L���O
	CRanking::LoadRanking();		// �����L���O���[�h
	CEffect::Load();				// �G�t�F�N�g

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pScene2D[nCnt] == NULL)
		{
			m_pScene2D[nCnt] = new CScene2D(CScene::PRIORITY_UI);
			m_pScene2D[nCnt]->SetInitAll(D3DXVECTOR3(0.0f + 100.0f + ((SCREEN_WIDTH - 100.0f - 100.0f) * nCnt), SCREEN_HEIGHT - 80.0f, 0.0f), D3DXVECTOR3(100.0f, 80.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.25f));
			m_pScene2D[nCnt]->Init();
			m_pScene2D[nCnt]->SetObjType(CScene::OBJTYPE_TITLEPLAYER);
			m_pScene2D[nCnt]->BindTexture(m_pSceneTexture);

			m_pScene2D[nCnt]->SetTex(1, nCnt * MAX_PLAYER, D3DXVECTOR2(0.0f, 0.0f));
		}
	}

	m_pModel3D = NULL;
	m_pPress3D = NULL;
	m_pStar3D = NULL;

	SetTitle(TITLE_LOGO);			// �^�C�g���ݒ�

	m_nCntTitle		= 0;			// �^�C�g���J�E���^
	m_bTitleSet_T	= false;		// �^�C�g���Z�b�g 
	m_bTitleSet_R	= false;		// �����L���O�Z�b�g
	m_bPressSet		= false;		// �v���X�Z�b�g
	m_bPress_R		= false;		// �v���X�����L���O

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;		// �v���C���[
	}
	m_bPlayGame		= false;		// �v���C�Q�[��

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CTitle ***
//=============================================================================
void CTitle::Uninit(void)
{
	// ���f��
	if (m_pModel3D != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pModel3D->Uninit();		// �I������
		m_pModel3D = NULL;			// NULL��
	}

	// �����L���O
	if (m_pRanking != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pRanking->Uninit();		// �I������
		m_pRanking = NULL;			// NULL��
	}

	// �V�[��3D
	if (m_pPress3D != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pPress3D->Uninit();		// �I������
		m_pPress3D = NULL;			// NULL��
	}

	// �v���C���[
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pPlayer[nCntPlayer]->Uninit();		// �I������
			m_pPlayer[nCntPlayer] = NULL;			// NULL��
		}
	}

	// �V�[��3D
	if (m_pStar3D != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pStar3D->Uninit();		// �I������
		m_pStar3D = NULL;			// NULL��
	}

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (m_pScene2D[nCnt] != NULL)
		{
			m_pScene2D[nCnt]->Uninit();		// �I������
			m_pScene2D[nCnt] = NULL;		// NULL��
		}
	}

	// 3D�A�����[�h
	CMeshCylinder::Unload();		// ���b�V���V�����_�[
	CMenu::Unload();				// ���j���[
	CPlayer::Unload();				// �v���C���[

	// 2D�A�����[�h
	CNumber::Unload();				// ����
	CRanking::Unload();				// �����L���O
	CRanking::SaveRanking();		// �����L���O�Z�[�u
	CEffect::Unload();				// �G�t�F�N�g

	CScene *pScene;					// �V�[���|�C���^

	for (int nCntPriority = 0; nCntPriority <= CScene::PRIORITY_PAUSE; nCntPriority++)
	{// �v���C�I���e�B�J�E���g
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// �V�[���J�E���g
			pScene = CScene::GetScene(nCntPriority, nCntScene);		// �V�[���ǂݍ���

			if (pScene != NULL)
			{// NULL�ȊO�̏ꍇ
				pScene->Uninit();	// �I������
				pScene = NULL;		// NULL��
			}
		}
	}
}

//=============================================================================
// �X�V����											(public)	*** CTitle ***
//=============================================================================
void CTitle::Update(void)
{
	CCamera *pCamera = CManager::GetCamera();						// �J�����擾
	//CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();	// ���̓f�o�C�X
	CInputXPad *pInputXPad = CManager::GetInputXPad();				// ���̓f�o�C�X
	CSound *pSound = CManager::GetSound();							// �T�E���h
	CFade::FADE fade = CFade::GetFade();							// �t�F�[�h�擾
	D3DXVECTOR3 posModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	D3DXVECTOR3 rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���
	D3DXVECTOR3 posPress = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	D3DXVECTOR3 sizePress = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �傫��
	D3DXVECTOR2 TexUVPress = D3DXVECTOR2(0.0f, 0.0f);				// UV
	D3DXCOLOR colPress = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);			// �F

	// �r���Ŕ����ƃG���[���o��A�R���g���[���[�\�[�g�Ȃ�
	if (/*pCamera->GetDest() == false &&*/ fade == CFade::FADE_NONE && m_bPlayGame == false)
	{
		for (int nCntCont = 0; nCntCont < CInputXPad::GetInputNum(); nCntCont++)
		{// �J�E���g
			if (m_bPlayerUse[nCntCont] == false)
			{
				if (pInputXPad->GetRelease(XINPUT_GAMEPAD_START, nCntCont) == true)
				{// �L�[����������
					if (m_bPlayerPress[nCntCont] == false)
					{
						m_bPlayerUse[nCntCont] = true;

						CManager::SetPlayNum(CManager::GetPlayNum() + 1);
					}
				}
				else if (pInputXPad->GetPress(XINPUT_GAMEPAD_START, nCntCont) == false)
				{// �L�[����������
					m_bPlayerPress[nCntCont] = false;
				}

				if (m_pScene2D[nCntCont] != NULL && nCntCont < 2)
				{
					m_pScene2D[nCntCont]->SetTex(1, nCntCont * MAX_PLAYER, D3DXVECTOR2(0.0f, 0.0f));
				}
			}
			else
			{
				if (pInputXPad->GetTrigger(XINPUT_GAMEPAD_START, nCntCont) == true)
				{// �L�[����������
					m_bPlayerUse[nCntCont] = false;
					m_bPlayerPress[nCntCont] = true;

					CManager::SetPlayNum(CManager::GetPlayNum() - 1);
				}

				if (m_pScene2D[nCntCont] != NULL && nCntCont < 2)
				{
					m_pScene2D[nCntCont]->SetTex(1, (nCntCont * MAX_PLAYER) + 1, D3DXVECTOR2(0.0f, 0.0f));
				}
			}
		}
	}

	switch (m_title)
	{// �^�C�g�����[�h
	case TITLE_LOGO:		// ���S
	{
		if (m_pModel3D != NULL)
		{// NULL�ȊO�̏ꍇ
			posModel = m_pModel3D->Getpos();		// �ʒu�擾
		}
		if (m_pPress3D != NULL)
		{// NULL�ȊO�̏ꍇ
			posPress = m_pPress3D->Getpos();		// �ʒu�擾
			sizePress = m_pPress3D->Getsize();		// �傫���擾
			TexUVPress = m_pPress3D->GetTex();		// UV�擾
			colPress = m_pPress3D->Getcol();		// �F�擾
		}

		if (m_bTitleSet_T == false)
		{// �^�C�g�����S�ݒ�
			if (m_nCntTitle == 0)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE007);
			}

			m_nCntTitle++;							// �J�E���g

			// �ړ��ʌv�Z
			move.x = ((0.0f - posModel.x) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));
			move.y = ((3.0f - posModel.y) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));
			move.z = ((-50.0f - posModel.z) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));

			if (m_nCntTitle == 120)
			{// �ړI�ʒu����
				// �ʒu����
				posModel.x = 0.0f;
				posModel.y = 3.0f;
				posModel.z = -50.0f;
				m_bTitleSet_T = true;		// �Z�b�g����
				m_nCntTitle = 0;			// �J�E���g������
			}
			else
			{// �ړ�
				posModel += move;			// �ړ��ʉ��Z
			}
		}
		else
		{// �^�C�g�����S�ݒ�I��
			m_nCntTitle++;					// �J�E���g

			if (m_bPressSet == false)
			{// �v���X�ݒ�
				if (30 < m_nCntTitle)
				{// ���t���[����
					if (sizePress.x != 16.0f)
					{// �傫�����ݒ�l�ȉ�
						if (16.0f < sizePress.x + (16.0f / 60.0f))
						{// �傫�����ݒ�l�𒴂���
							// �傫������
							sizePress.x = 16.0f;
							TexUVPress.x = 1.0f;
						}
						else
						{// ���̑�
							// �傫�����Z
							sizePress.x += 16.0f / 60.0f;
							TexUVPress.x = (sizePress.x / 16.0f);
						}

						posPress.x = (16.0f - sizePress.x);			// �ړ��ʉ��Z
					}

					if (m_nCntTitle == 120)
					{// ���t���[��
						m_bPressSet = true;							// �Z�b�g����
					}
				}
			}

			// �����ړ�
			if (m_nCntTitle % 240 < 120)
			{// ���t���[��
				posModel.y += 0.003f;		// �ړ��ʉ��Z

				if (m_bPressSet == true)
				{// �Z�b�g����
					// �F�ύX
					colPress.r -= 0.5f / 120.0f;
					colPress.g -= 0.5f / 120.0f;
					colPress.b -= 0.5f / 120.0f;
				}
			}
			else
			{// ���̑�
				posModel.y -= 0.003f;		// �ړ��ʉ��Z

				if (m_bPressSet == true)
				{// �Z�b�g����
					// �F�ύX
					colPress.r += 0.5f / 120.0f;
					colPress.g += 0.5f / 120.0f;
					colPress.b += 0.5f / 120.0f;
				}
			}
		}

		if (pCamera->GetDest() == false)
		{// �J�����������Ă��Ȃ�
			if (m_bTitleSet_T == true && m_bPressSet == true)
			{// ���ׂăZ�b�g����
				//if (pInputKeyboard->GetALL(2) == true)
				for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
				{// �J�E���g
					if (pInputXPad->GetALL(2, nCntCont) == true)
					{// �L�[����������
						pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1200.0f, 0.0f, 0.0f), 1200, 90, CCamera::CPOSDEST_TITLE_LOGO);

						pSound->PlaySoundA(CSound::SOUND_LABEL_SE002);
					}
				}
			}
			else
			{// ���̑�
				//if (pInputKeyboard->GetALL(2) == true)
				for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
				{// �J�E���g
					if (pInputXPad->GetALL(2, nCntCont) == true)
					{// �L�[����������
						// �ڕW�l�ɃX�L�b�v
						posModel = D3DXVECTOR3(0.0f, 3.0f, -50.0f);			// �ʒu
						posPress = D3DXVECTOR3(0.0f, -13.0f, -50.0f);		// �ʒu
						sizePress = D3DXVECTOR3(16.0f, 2.5f, 0.0f);			// �傫��	
						TexUVPress = D3DXVECTOR2(1.0f, 1.0f);				// UV
						colPress = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �F

						// �Z�b�g����
						m_bTitleSet_T = true;
						m_bPressSet = true;

						m_nCntTitle = 0;			// �J�E���g������
					}
				}
			}
		}

		if (m_pModel3D != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pModel3D->Setpos(posModel);										// �ʒu�ݒ�
		}
		if (m_pPress3D != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pPress3D->Setpos(posPress);										// �ʒu�ݒ�
			m_pPress3D->Setsize(sizePress);										// �傫���ݒ�
			m_pPress3D->SetTex(1, 0, TexUVPress, D3DXVECTOR2(0.0, 0.0f));		// �e�N�X�`���ݒ�
			m_pPress3D->Setcol(colPress);										// �F
		}

		if (pCamera->GetPosDest(CCamera::CPOSDEST_TITLE_LOGO) == true)
		{// �^�C�g���J�����Z�b�g����
			pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1200.0f), 1200, 90, CCamera::CPOSDEST_TITLE_LOGO);
			SetTitle(TITLE_MENU);			// ���j���[��
		}

		if (m_nCntTitle == 720 && pCamera->GetDest() == false)
		{// ���t���[��
			posModel = D3DXVECTOR3(0.0f, 3.0f, -50.0f);				// �ʒu
			posPress = D3DXVECTOR3(0.0f, -13.0f, -50.0f);			// �ʒu
			sizePress = D3DXVECTOR3(16.0f, 2.5f, 0.0f);				// �傫��
			TexUVPress = D3DXVECTOR2(1.0f, 1.0f);					// UV
			colPress = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �F

			if (m_pModel3D != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pModel3D->Setpos(posModel);									// �ʒu�ݒ�
			}
			if (m_pPress3D != NULL)
			{
				m_pPress3D->Setpos(posPress);									// �ʒu�ݒ�
				m_pPress3D->Setsize(sizePress);									// �傫���ݒ�
				m_pPress3D->SetTex(1, 0, TexUVPress, D3DXVECTOR2(0.0, 0.0f));	// �e�N�X�`���ݒ�
				m_pPress3D->Setcol(colPress);									// �F
			}

			SetTitle(TITLE_RANKING);		// �����L���O��
		}
	}
		break;
	case TITLE_MENU:		// ���j���[
		if (m_pPlayer != NULL)
		{// NULL�ȊO�̏ꍇ
			posModel = m_pPlayer[0]->Getpos();		// �ʒu�擾
			rotModel = m_pPlayer[0]->Getrot();		// �����擾
		}

		if (m_bPlayGame == false)
		{
			m_nCntTitle++;					// �J�E���g

			// �����ړ�
			if (m_nCntTitle % 240 < 120)
			{// ���t���[��
				posModel.y += 0.005f;		// �ړ��ʉ��Z
			}
			else
			{// ���̑�
				posModel.y -= 0.005f;		// �ړ��ʉ��Z
			}
		}
		else
		{
			if (m_nCntTitle == 0)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE008);

				if (CManager::GetPlayNum() == MAX_PLAYER)
				{
					if (m_pPlayer[1] == NULL)
					{
						m_pPlayer[1] = CPlayer::Create(D3DXVECTOR3(-15.0f, 0.0f, -140.0f), D3DXVECTOR3(0.0f, -(D3DX_PI * 0.25f), 0.0f), 0);
					}
				}
			}

			m_nCntTitle++;					// �J�E���g

			if (m_nCntTitle < 90)
			{// ���t���[���܂�
				rotModel.y += -(D3DX_PI * 0.75f) / 90.0f;		// ��]
			}

			if (120 < m_nCntTitle)
			{// ���t���[���ȏ�
				CEffect::Create(D3DXVECTOR3(posModel.x, posModel.y, posModel.z - 20.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_PLAYER_T, CEffect::TEXTURE_EFFECT);		// �G�t�F�N�g
			
				posModel.z += (5.0f * ((m_nCntTitle - 120.0f) * 0.006f)) * 5.0f * ((m_nCntTitle - 120.0f) * 0.006f);		// �ړ�
			}

			if (m_nCntTitle == 240)
			{// ���t���[���ɂȂ�����
				CFade::Create(CManager::MODE_GAME, CFade::FADE_OUT);		// �Q�[���w
			}
		}

		if (m_pPlayer[0] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pPlayer[0]->Setpos(posModel);		// �ʒu�ݒ�
			m_pPlayer[0]->Setrot(rotModel);		// �����ݒ�
		}

		if (m_pPlayer[1] != NULL)
		{
			CEffect::Create(D3DXVECTOR3(posModel.x, posModel.y, posModel.z - 220.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), D3DXVECTOR3(EFFECT_SIZE_X, EFFECT_SIZE_Y, 0.0f), D3DXVECTOR3(-0.9f, -0.9f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, CEffect::EFFECTTYPE_PLAYER_T, CEffect::TEXTURE_EFFECT);		// �G�t�F�N�g

			m_pPlayer[1]->Setpos(D3DXVECTOR3(posModel.x, posModel.y, posModel.z - 200.0f));		// �ʒu�ݒ�
			m_pPlayer[1]->Setrot(rotModel);		// �����ݒ�
		}

		break;
	case TITLE_CUSTOM:		// �J�X�^��
		if (pCamera->GetDest() == false)
		{// �J�����������Ă��Ȃ�
		//if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
			{// �J�E���g
				if (pInputXPad->GetALL(2, nCntCont) == true)
				{// �L�[����������
					pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-1200.0f, 0.0f, 0.0f), 1200, 90, CCamera::CPOSDEST_CUSTOM);
				}
			}
		}

		if (pCamera->GetPosDest(CCamera::CPOSDEST_CUSTOM) == true)
		{// 
			pCamera->SetPosDest(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1200.0f), 1200, 90, CCamera::CPOSDEST_CUSTOM);
			SetTitle(TITLE_MENU);			// ���j���[��
		}

		break;
	case TITLE_RANKING:		// �����L���O
	{
		if (m_pModel3D != NULL)
		{// NULL�ȊO�̏ꍇ
			posModel = m_pModel3D->Getpos();		// �ʒu�擾
		}
		if (m_pPress3D != NULL)
		{// NULL�ȊO�̏ꍇ
			posPress = m_pPress3D->Getpos();		// �ʒu�擾
			sizePress = m_pPress3D->Getsize();		// �傫���擾
			TexUVPress = m_pPress3D->GetTex();		// UV�擾
			colPress = m_pPress3D->Getcol();		// �F�擾
		}

		if (m_bTitleSet_R == false)
		{// �^�C�g�����S�ݒ�
			if (m_nCntTitle == 0)
			{
				pSound->PlaySoundA(CSound::SOUND_LABEL_SE007);
			}

			m_nCntTitle++;					// �J�E���g

			// �ړ��ʌv�Z
			move.x = ((100.0f - posModel.x) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));
			move.y = ((60.0f - posModel.y) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));
			move.z = ((-180.0f - posModel.z) * ((float)((float)m_nCntTitle * 0.1f) / 60.0f));

			if (m_nCntTitle == 120)
			{// �ړI�ʒu����
				// �ʒu����
				posModel.x = 100.0f;
				posModel.y = 60.0f;
				posModel.z = -180.0f;
				m_bTitleSet_R = true;		// �Z�b�g����
				m_nCntTitle = 0;			// �J�E���g������
			}
			else
			{// �ړ�
				posModel += move;			// �ړ��ʉ��Z
			}
		}
		else
		{// �^�C�g�����S�ݒ�I��
			m_nCntTitle++;					// �J�E���g

			// �����ړ�
			if (m_nCntTitle % 240 < 120)
			{// ���t���[��
				posModel.y += 0.003f;		// �ړ��ʉ��Z

				if (m_bPressSet == true)
				{// �Z�b�g����
					// �F�ύX
					colPress.r -= 0.5f / 120.0f;
					colPress.g -= 0.5f / 120.0f;
					colPress.b -= 0.5f / 120.0f;
				}
			}
			else
			{
				posModel.y -= 0.003f;		// �ړ��ʉ��Z

				if (m_bPressSet == true)
				{// �Z�b�g����
					// �F�ύX
					colPress.r += 0.5f / 120.0f;
					colPress.g += 0.5f / 120.0f;
					colPress.b += 0.5f / 120.0f;
				}
			}
		}

		if (m_pModel3D != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pModel3D->Setpos(posModel);										// �ʒu�ݒ�
		}
		if (m_pPress3D != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pPress3D->Setpos(posPress);										// �ʒu�ݒ�
			m_pPress3D->Setsize(sizePress);										// �傫���ݒ�
			m_pPress3D->SetTex(1, 0, TexUVPress, D3DXVECTOR2(0.0, 0.0f));		// �e�N�X�`���ݒ�
			m_pPress3D->Setcol(colPress);										// �F
		}

		//if (m_nCntTitle == 480 || pInputKeyboard->GetALL(2) == true)
		if (CManager::GetPlayNum() != 0)
		{
			if (pCamera->GetDest() == false)
			{// �J�����������Ă��Ȃ�
				for (int nCntCont = 0; nCntCont < CManager::GetPlayNum(); nCntCont++)
				{// �J�E���g
					if (m_nCntTitle == 480 || pInputXPad->GetALL(2, nCntCont) == true)
					{// ���t���[��or�L�[����������
						// �ڕW�l�ɃX�L�b�v
						posModel = D3DXVECTOR3(100.0f, 60.0f, -180.0f);			// �ʒu
						posPress = D3DXVECTOR3(0.0f, -13.0f, -50.0f);			// �ʒu
						sizePress = D3DXVECTOR3(16.0f, 2.5f, 0.0f);				// �傫��
						TexUVPress = D3DXVECTOR2(1.0f, 1.0f);					// UV
						colPress = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �F

						if (m_pModel3D != NULL)
						{// NULL�ȊO�̏ꍇ
							m_pModel3D->Setpos(posModel);										// �ʒu�ݒ�
						}
						if (m_pPress3D != NULL)
						{// NULL�ȊO�̏ꍇ
							m_pPress3D->Setpos(posPress);										// �ʒu�ݒ�
							m_pPress3D->Setsize(sizePress);										// �傫���ݒ�
							m_pPress3D->SetTex(1, 0, TexUVPress, D3DXVECTOR2(0.0, 0.0f));		// �e�N�X�`���ݒ�
							m_pPress3D->Setcol(colPress);										// �F
						}

						m_bPress_R = true;				// �Z�b�g����

						SetTitle(TITLE_LOGO);			// �^�C�g����
					}
				}
			}
		}
		else
		{
			if (m_nCntTitle == 480)
			{// ���t���[��or�L�[����������
				// �ڕW�l�ɃX�L�b�v
				posModel = D3DXVECTOR3(100.0f, 60.0f, -180.0f);			// �ʒu
				posPress = D3DXVECTOR3(0.0f, -13.0f, -50.0f);			// �ʒu
				sizePress = D3DXVECTOR3(16.0f, 2.5f, 0.0f);				// �傫��
				TexUVPress = D3DXVECTOR2(1.0f, 1.0f);					// UV
				colPress = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �F

				if (m_pModel3D != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pModel3D->Setpos(posModel);										// �ʒu�ݒ�
				}
				if (m_pPress3D != NULL)
				{// NULL�ȊO�̏ꍇ
					m_pPress3D->Setpos(posPress);										// �ʒu�ݒ�
					m_pPress3D->Setsize(sizePress);										// �傫���ݒ�
					m_pPress3D->SetTex(1, 0, TexUVPress, D3DXVECTOR2(0.0, 0.0f));		// �e�N�X�`���ݒ�
					m_pPress3D->Setcol(colPress);										// �F
				}

				m_bPress_R = true;				// �Z�b�g����

				SetTitle(TITLE_LOGO);			// �^�C�g����
			}
		}
	}
	break;
	}
}

//=============================================================================
// �`�揈��											(public)	*** CTitle ***
//=============================================================================
void CTitle::Draw(void)
{
	
}

//=============================================================================
// �^�C�g���ݒ菈��									(public)	*** CGame ***
//=============================================================================
void CTitle::SetTitle(CTitle::TITLE title)
{
	CScene *pScene = NULL;					// �V�[���|�C���^

	switch (m_title)
	{// �^�C�g���i���݁j
	case TITLE_LOGO:		// ���S
		if (title != TITLE_RANKING)
		{// ���񂪃����L���O�ȊO
			// ���f��
			if (m_pModel3D != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pModel3D->Uninit();		// �I������
				m_pModel3D = NULL;			// NULL��
			}

			// �V�[��3D
			if (m_pPress3D != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pPress3D->Uninit();		// �I������
				m_pPress3D = NULL;			// NULL��
			}
		}

		break;
	case TITLE_MENU:		// ���j���[
		// �v���C���[
		if (m_pPlayer[0] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pPlayer[0]->Uninit();		// �I������
			m_pPlayer[0] = NULL;			// NULL��
		}

		// �V�[��3D
		if (m_pStar3D != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pStar3D->Uninit();		// �I������
			m_pStar3D = NULL;			// NULL��
		}

		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{// �V�[���J�E���g
			pScene = CScene::GetScene(CScene::PRIORITY_4, nCntScene);	// �V�[���ǂݍ���

			// �V�[��3D
			if (pScene != NULL)
			{// NULL�ȊO�̏ꍇ
				pScene->Uninit();			// �I������
				pScene = NULL;				// NULL��
			}
		}
		break;
	case TITLE_CUSTOM:		// �J�X�^��
		break;
	case TITLE_RANKING:		// �����L���O
		if (m_bPress_R == false)
		{// �v���X�Z�b�g������
			// �����L���O
			if (m_pRanking != NULL)
			{// NULL�ȊO�̏ꍇ
				m_pRanking->Uninit();		// �I������
				m_pRanking = NULL;			// NULL��
			}
		}

		break;
	}

	CScene::SetStartUp(0);	// �X�V�͈͂�������

	// �J����
	CCamera *pCamera = CManager::GetCamera();													// �擾
	pCamera->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));				// �����l�ݒ�

	switch (title)
	{// �^�C�g���i����j
	case TITLE_LOGO:		// ���S
		pCamera->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1200.0f));		// �����l�ݒ�
		
		if (m_title != TITLE_RANKING)
		{// ���݂������L���O�ȊO
			if (m_pModel3D == NULL)
			{// NULL�̏ꍇ
				m_pModel3D = new CModel3D(CScene::PRIORITY_4);									// �������m��

				// �^�C�g���Z�b�g�����L��
				if (m_bTitleSet_T == false)
					m_pModel3D->SetInitAll(m_pModelTexture, m_pMesh, m_pBuffMat, m_nNumMat, 
						D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));		// �����l�ݒ�
				else
					m_pModel3D->SetInitAll(m_pModelTexture, m_pMesh, m_pBuffMat, m_nNumMat, 
						D3DXVECTOR3(0.0f, 3.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));		// �����l�ݒ�

				m_pModel3D->Init();																// ����������
				m_pModel3D->SetObjType(CScene::OBJTYPE_TITLE);									// �I�u�W�F�N�g�^�C�v�ݒ�
			}

			if (m_pPress3D == NULL)
			{// NULL�ȊO�̏ꍇ
				m_pPress3D = new CScene3D(CScene::PRIORITY_4);															// �������m��					

				// �v���X�Z�b�g�����L��
				if (m_bPressSet == false)
					m_pPress3D->SetInitAll(D3DXVECTOR3(16.0f, -13.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 2.5f, 0.0f), 
						D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXVECTOR2(0.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);		// ����������
				else
					m_pPress3D->SetInitAll(D3DXVECTOR3(0.0f, -13.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(16.0f, 2.5f, 0.0f), 
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);		// ����������

				m_pPress3D->Init();																						// ����������
				m_pPress3D->SetObjType(CScene::OBJTYPE_PRESS);															// �I�u�W�F�N�g�^�C�v�ݒ�
				m_pPress3D->BindTexture(m_pPressTexture);																// �e�N�X�`���𔽉f
			}
		}

		// �Z�b�g�ݒ�
		if (m_title == TITLE_MENU)
		{
			m_bTitleSet_T = true;
		}
		else
		{
			m_bTitleSet_T = false;
		}

		m_bTitleSet_R = true;

		m_nCntTitle = 0;			// �J�E���g������

		break;
	case TITLE_MENU:		// ���j���[
		pCamera->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1200.0f));		// �����l�ݒ�

		// 3D�N���G�C�g
		CMenu::Create();

		// �v���C���[
		if (m_pPlayer[0] == NULL)
		{// NULL�̏ꍇ
			m_pPlayer[0] = CPlayer::Create(D3DXVECTOR3(-15.0f, 0.0f, 60.0f), D3DXVECTOR3(0.0f, -(D3DX_PI * 0.25f), 0.0f), 0);
		}

		if (m_pStar3D == NULL)
		{// NULL�ȊO�̏ꍇ
			m_pStar3D = new CScene3D(CScene::PRIORITY_4);															// �������m��					

			m_pStar3D->SetInitAll(D3DXVECTOR3(10.0f, 20.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_NORMAL);			// ����������

			m_pStar3D->Init();																						// ����������
			m_pStar3D->SetObjType(CScene::OBJTYPE_STAR);															// �I�u�W�F�N�g�^�C�v�ݒ�
			m_pStar3D->BindTexture(m_pStarTexture);																	// �e�N�X�`���𔽉f
		}

		m_nCntTitle = 0;			// �J�E���g������
		m_bPlayGame = false;		// �v���C�Q�[��

		break;
	case TITLE_CUSTOM:		// �J�X�^��
		break;
	case TITLE_RANKING:		// �����L���O
		pCamera->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, -1200.0f));		// �����l�ݒ�

		// �����L���O
		if (m_pRanking == NULL)
		{// NULL�̏ꍇ
			m_pRanking = CRanking::Create(D3DXVECTOR3(60.0f, 8.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(1.2f, 1.5f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), -1);
		}

		// �Z�b�g�ݒ�
		m_bTitleSet_T = true;
		m_bTitleSet_R = false;
		m_bPress_R = false;

		m_nCntTitle = 0;			// �J�E���g������

		break;
	}

	m_title = title;		// �^�C�g���ύX
}