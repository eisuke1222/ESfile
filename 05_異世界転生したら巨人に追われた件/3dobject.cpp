//=============================================================================
//
// �A�C�e������ [item.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"		// �V�[��3D
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "font.h"			// �t�H���g
#include "camera.h"			// �J����
#include "3dobject.h"		// 3d�I�u�W�F�N�g
#include "itemicon.h"		// �A�C�e���̃A�C�R��
#include "life.h"			// �̗͕\��
#include "score.h"			// �X�R�A
#include "effect.h"			// �G�t�F�N�g

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
LPDIRECT3DTEXTURE9 C3Dobject::m_pTexture[TYPE_MAX] = { NULL };		// �e�N�X�`�����ւ̃|�C���^

//=============================================================================
// �R���X�g���N�^								(public)	*** CItem ***
//=============================================================================
C3Dobject::C3Dobject() : CScene3D(CScene::PRIORITY_4, CScene::OBJTYPE_3DOBJECT)
{
	// �����l�ݒ�
	m_Type = {};		// ���
}

//=============================================================================
// �f�X�g���N�^									(public)	*** CItem ***
//=============================================================================
C3Dobject::~C3Dobject()
{

}

//=============================================================================
//	�A�C�e���̐���
//=============================================================================
C3Dobject *C3Dobject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	C3Dobject *p3Dobject = NULL;		// �|�C���^

	if (p3Dobject == NULL)
	{//	�A�C�e���̐���
		p3Dobject = new C3Dobject;
		if (p3Dobject != NULL)
		{//�A�C�e���̏�����
			p3Dobject->Init(pos, rot, size, col, TexUV, nType);
		}
	}

	return p3Dobject;		// �l��Ԃ�
}

//=============================================================================
// ����������									(public)	*** CItem ***
//=============================================================================
HRESULT C3Dobject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	// �l�ݒ�
	CScene3D::SetInitAll(pos + D3DXVECTOR3(0.0f, size.y,0.0f), rot, size, col, TexUV, CScene3D::SCENE3DTYPE_BILLBOARD);
	CScene3D::Init();
	CScene3D::BindTexture(m_pTexture[nType]);
	m_Type = (OBJECTTYPE)nType;
	return S_OK;
}

//=============================================================================
// �I������										(public)	*** CItem ***
//=============================================================================
void C3Dobject::Uninit(void)
{
	CScene3D::Uninit();		// �I������
}

//=============================================================================
// �X�V����										(public)	*** CItem ***
//=============================================================================
void C3Dobject::Update(void)
{
	CManager::MODE mode = CManager::GetMode();		// ���[�h�擾

	if (mode == CManager::MODE_GAME)
	{// �Q�[���̏ꍇ
		D3DXVECTOR3 posVDest = CManager::GetCamera()->GetPosVDest();	// ���_�擾
		D3DXVECTOR3 pos = CScene3D::Getpos();							// �ʒu�擾
		D3DXVECTOR3 size = CScene3D::Getsize();							// �傫���擾
		if (pos.x + size.x < posVDest.x - 800.0f)
		{// �͈̓`�F�b�N
			Uninit();		// �I������
		}
	}
}

//=============================================================================
// �`�揈��										(public)	*** CItem ***
//=============================================================================
void C3Dobject::Draw(void)
{
	CScene3D::Draw();		// �`�揈��
}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT C3Dobject::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree000.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree001.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree002.png", &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree002.png", &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\kusa000.png", &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\kusa001.png", &m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\iwa000.png", &m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\iwa001.png", &m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\bamboo001.png", &m_pTexture[8]);

	return S_OK;
}
//=============================================================================
// �A�����[�h����								(public)	*** CItem ***
//=============================================================================
void C3Dobject::Unload(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{// ��ރJ�E���g
		// �e�N�X�`���̔j��
		if (m_pTexture[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTexture[nCount]->Release();		// �J��
			m_pTexture[nCount] = NULL;			// NULL��
		}
	}
}
