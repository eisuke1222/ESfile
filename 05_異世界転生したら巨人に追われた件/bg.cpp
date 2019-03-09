//**************************************************************
//
//				�w�i�̏���[bg.h]
//				Auther : Shun Yokomichi
//
//**************************************************************
#include "scene3d.h"		// �V�[���RD
#include "bg.h"				// �w�i
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "game.h"			// �Q�[��
#include "player.h"			// �v���C���[

//**************************************************************
//				�}�N����`
//**************************************************************
#define BG_TEXTURENAME000	"data/TEXTURE/game/BG001.jpg"		// �ǂݍ��ރe�N�X�`��000

//**************************************************************
//				�ÓI�����o�ϐ�
//**************************************************************
LPDIRECT3DTEXTURE9 CBg::m_pTexture = NULL;						// �e�N�X�`�����ւ̃|�C���^

//**************************************************************
//				�w�i�̐���
//**************************************************************
CBg *CBg::Create()
{
	CBg *pBg = NULL;	// �|�C���^�̐���

	// �V�[���̐���
	if (pBg == NULL)
	{
		pBg = new CBg;
		if (pBg != NULL)
		{
			pBg->Init();
		}
	}

	return pBg;
}

//**************************************************************
//				�ǂݍ��ރe�N�X�`��
//**************************************************************
HRESULT CBg::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();			// �����_���[�擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// �f�o�C�X�擾
	CManager::MODE pMode = CManager::GetMode();				// ���[�h�̎擾

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME000, &m_pTexture);
	
	return S_OK;
}

//**************************************************************
//				�J������e�N�X�`��
//**************************************************************
void CBg::Unload(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pTexture->Release();		// �J��
		m_pTexture = NULL;			// NULL��
	}
	
}

//**************************************************************
//		�R���X�g���N�^
//**************************************************************
CBg::CBg() : CScene(PRIORITY_BG, OBJTYPE_BG)
{
	// �f�t�H���g�l
	m_mvoetex_x = 0.0f;
}

//**************************************************************
//		�f�X�g���N�^
//**************************************************************
CBg::~CBg()
{

}

//**************************************************************
//		����������
//**************************************************************
HRESULT CBg::Init(void)
{
	CManager::MODE pMode = CManager::GetMode();			// ���[�h�擾

	m_pScene = new CScene3D(PRIORITY_BG, OBJTYPE_BG);	// �������m��

	if (m_pScene != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pScene->SetInitAll(D3DXVECTOR3(0.0f, 300.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXVECTOR3(1000.0f, 300.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f), CScene3D::SCENE3DTYPE_BILLBOARD);		// �����l�ݒ�
		m_pScene->Init();								// ����������
		m_pScene->BindTexture(m_pTexture);				// �e�N�X�`���ݒ�
	}

	return S_OK;
}

//**************************************************************
//		�I������
//**************************************************************
void CBg::Uninit(void)
{
	if (m_pScene != NULL)
	{	// NULL����Ȃ��ꍇ
		m_pScene->Uninit();		// �I������
		m_pScene = NULL;		// NULL��
	}
	

	CScene::SetDeath();		// ���S�t���O
}

//**************************************************************
//		�X�V����
//**************************************************************
void CBg::Update(void)
{
	CManager::MODE pMode = CManager::GetMode();		// ���[�h�擾

	if (pMode == CManager::MODE_TITLE)
	{// �^�C�g���̏ꍇ
		m_mvoetex_x += 0.0008f;		// �ړ�

		// �ʒu�ݒ�
		m_pScene->Setpos(D3DXVECTOR3(CManager::GetCamera()->GetPosVDest().x, m_pScene->Getpos().y, m_pScene->Getpos().z));
		// �e�N�X�`���ݒ�
		m_pScene->SetTex(1, 0, D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(m_mvoetex_x, 0.0f));
	}
	else if (pMode == CManager::MODE_GAME)
	{// �Q�[���̏ꍇ
		if (CManager::GetCamera()->GetPosVDest().x - 2.0f <= CGame::GetPlayer()->Getpos().x)
		{
				m_mvoetex_x += 0.0004f;
		}
	
		// �ʒu�ݒ�
		m_pScene->Setpos(D3DXVECTOR3(CManager::GetCamera()->GetPosVDest().x, m_pScene->Getpos().y, m_pScene->Getpos().z));
		// �e�N�X�`���ݒ�
		m_pScene->SetTex(1, 0, D3DXVECTOR2(1.0f, 1.0f), D3DXVECTOR2(m_mvoetex_x, 0.0f));
	}
}

//**************************************************************
//		�`�揈��
//**************************************************************
void CBg::Draw(void)
{
	m_pScene->Draw();		// �`�揈��
}