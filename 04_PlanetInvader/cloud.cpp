//=============================================================================
//
// �_���� [cloud.cpp]
// Author : Eisuke Sakagawa
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "cloud.h"			// �_
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[
#include "camera.h"			// �J����

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME		"data\\TEXTURE\\cloud000.png"		// �e�N�X�`�����ւ̃|�C���^
#define MOVE_SPEED			(-1.3f)								// �ړ����x

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CCloud::m_pTexture = NULL;		// �e�N�X�`��

//=============================================================================
// �R���X�g���N�^									(public)	*** CCloud ***
//=============================================================================
CCloud::CCloud() : CScene3D(CScene::PRIORITY_5)
{
	// �����l�ݒ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
}

//=============================================================================
// �f�X�g���N�^										(public)	*** CCloud ***
//=============================================================================
CCloud::~CCloud()
{

}

//=============================================================================
// ���[�h����							(public)	*** CCloud ***
//=============================================================================
HRESULT CCloud::Load(void)
{
	if (m_pTexture == NULL)
	{// NULL�̏ꍇ
		// �f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����								(public)	*** CCloud ***
//=============================================================================
void CCloud::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pTexture->Release();		// ���
		m_pTexture = NULL;			// NULL��
	}
}

//=============================================================================
// �m�ۏ���											(public)	*** CCloud ***
//=============================================================================
CCloud *CCloud::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType)
{
	CCloud *pMeshCloud = NULL;				// �|�C���^

	if (pMeshCloud == NULL)
	{// NULL�̏ꍇ
		pMeshCloud = new CCloud;			// �������m��
	}

	if (pMeshCloud != NULL)
	{// NULL�ȊO�̏ꍇ
		pMeshCloud->Init(pos, rot, size, col, TexUV, scene3dType);		// ����������
		pMeshCloud->SetObjType(CScene::OBJTYPE_CLOUD);					// �I�u�W�F�N�g�^�C�v�ݒ�
		pMeshCloud->BindTexture(m_pTexture);							// �e�N�X�`���ݒ�
	}

	return pMeshCloud;		// �l��Ԃ�
}

//=============================================================================
// ����������										(public)	*** CCloud ***
//=============================================================================
HRESULT CCloud::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, SCENE3DTYPE scene3dType)
{
	CScene3D::SetInitAll(pos, rot, size, col, TexUV, scene3dType);		// �����l�ݒ�
	CScene3D::Init();													// ����������

	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();			// �J���������_�擾

	m_pos = pos - Camerapos;				// �J�����ړ�������

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CCloud ***
//=============================================================================
void CCloud::Uninit(void)
{
	CScene3D::Uninit();						// �I������
}

//=============================================================================
// �X�V����											(public)	*** CCloud ***
//=============================================================================
void CCloud::Update(void)
{
	D3DXVECTOR3 Camerapos = CManager::GetCamera()->GetposR();			// �J���������_�擾
	D3DXVECTOR3 pos = CScene3D::Getpos();	// �ʒu���擾

	// �J�����ɑ΂��Ă̈ʒu��ݒ�
	pos.x = Camerapos.x + m_pos.x;			
	pos.z = Camerapos.z + m_pos.z;

	pos.z += MOVE_SPEED;					// �ړ��ʑ��

	CScene3D::Setpos(pos);					// �ʒu��ݒ�

	m_pos = pos - Camerapos;				// ���݈ʒu���v�Z

	Camerapos = CManager::GetCamera()->GetposV();						// �J�������_�擾

	if (Camerapos.z - pos.z >= 0)
	{// �J�����O�ɏo����
		Uninit();		// �I������
	}
}

//=============================================================================
// �`�揈��											(public)	*** CCloud ***
//=============================================================================
void CCloud::Draw(void)
{
	CScene3D::Draw();						// �`�揈��
}